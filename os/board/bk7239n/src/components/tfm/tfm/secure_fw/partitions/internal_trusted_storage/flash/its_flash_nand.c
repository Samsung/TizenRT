/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash_nand.h"

#include "flash_fs/its_flash_fs.h"
#include "tfm_memory_utils.h"

/* Valid entries for data item width */
static const uint32_t data_width_byte[] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

/**
 * \brief Gets physical address of the given block ID.
 *
 * \param[in] cfg       Flash FS configuration
 * \param[in] block_id  Block ID
 * \param[in] offset    Offset position from the init of the block
 *
 * \returns Returns physical address for the given block ID.
 */
static uint32_t get_phys_address(const struct its_flash_fs_config_t *cfg,
                                 uint32_t block_id, size_t offset)
{
    return cfg->flash_area_addr + (block_id * cfg->block_size) + offset;
}

static psa_status_t its_flash_nand_init(const struct its_flash_fs_config_t *cfg)
{
    int32_t err;
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;

    if (flash_dev->buf_size < cfg->block_size) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    err = flash_dev->driver->Initialize(NULL);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_read(const struct its_flash_fs_config_t *cfg,
                                        uint32_t block_id, uint8_t *buff,
                                        size_t offset, size_t size)
{
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;
    uint32_t addr;
    uint32_t remaining_len, read_length = 0;
    uint32_t aligned_addr;
    uint32_t item_number;

    /* The max size of flash data_width is 4 bytes. */
    uint8_t temp_buffer[sizeof(uint32_t)];
    ARM_FLASH_CAPABILITIES DriverCapabilities;
    uint8_t data_width;
    int ret;

    if (block_id == ITS_BLOCK_INVALID_ID) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (block_id == flash_dev->buf_block_id_0) {
        (void)tfm_memcpy(buff, flash_dev->write_buf_0 + offset, size);
    } else if (block_id == flash_dev->buf_block_id_1) {
        (void)tfm_memcpy(buff, flash_dev->write_buf_1 + offset, size);
    } else {
        addr = get_phys_address(cfg, block_id, offset);
        remaining_len = size;
        DriverCapabilities = flash_dev->driver->GetCapabilities();
        data_width = data_width_byte[DriverCapabilities.data_width];

        /*
         * CMSIS ARM_FLASH_ReadData API requires the `addr` data type size
         * aligned. Data type size is specified by the data_width in
         * ARM_FLASH_CAPABILITIES.
         */
        aligned_addr = (addr / data_width) * data_width;

        /* Read the first data_width bytes data if `addr` is not aligned. */
        if (aligned_addr != addr) {
            ret = flash_dev->driver->ReadData(aligned_addr, temp_buffer, 1);
            if (ret < 0) {
                return PSA_ERROR_STORAGE_FAILURE;
            }

            /* Record how many target data have been read. */
            read_length = ((addr - aligned_addr + size >= data_width) ?
                                (data_width - (addr - aligned_addr)) : size);
            /* Copy the read data. */
            tfm_memcpy(buff, temp_buffer + addr - aligned_addr, read_length);
            remaining_len -= read_length;
        }

        /*
         * The `cnt` parameter in CMSIS ARM_FLASH_ReadData indicates number of
         * data items to read.
         */
        if (remaining_len) {
            item_number = remaining_len / data_width;
            if (item_number) {
                ret = flash_dev->driver->ReadData(addr + read_length,
                                                  (uint8_t *)buff + read_length,
                                                  item_number);
                if (ret < 0) {
                    return PSA_ERROR_STORAGE_FAILURE;
                }
                read_length += item_number * data_width;
                remaining_len -= item_number * data_width;
            }
        }

        /* Read the last data item if there is still remaing data. */
        if (remaining_len) {
            ret = flash_dev->driver->ReadData(addr + read_length,
                                              temp_buffer, 1);
            if (ret < 0) {
                return PSA_ERROR_STORAGE_FAILURE;
            }
            /* Copy the read data. */
            tfm_memcpy(buff + read_length, temp_buffer, remaining_len);
        }
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_write(
                                    const struct its_flash_fs_config_t *cfg,
                                    uint32_t block_id, const uint8_t *buff,
                                    size_t offset, size_t size)
{
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;

    if (block_id == ITS_BLOCK_INVALID_ID) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /* Write to the match block buffer if exists. Otherwise use the empty
     * buffer if exists. If no more empty buffer, return error.
     */
    if (block_id == flash_dev->buf_block_id_0) {
        (void)tfm_memcpy(flash_dev->write_buf_0 + offset, buff, size);
    } else if (block_id == flash_dev->buf_block_id_1) {
        (void)tfm_memcpy(flash_dev->write_buf_1 + offset, buff, size);
    } else if (flash_dev->buf_block_id_0 == ITS_BLOCK_INVALID_ID) {
        flash_dev->buf_block_id_0 = block_id;
        (void)tfm_memcpy(flash_dev->write_buf_0 + offset, buff, size);
    } else if (flash_dev->buf_block_id_1 == ITS_BLOCK_INVALID_ID) {
        flash_dev->buf_block_id_1 = block_id;
        (void)tfm_memcpy(flash_dev->write_buf_1 + offset, buff, size);
    } else {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_flush(
                                    const struct its_flash_fs_config_t *cfg,
                                    uint32_t block_id)
{
    int32_t err;
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;
    uint32_t addr;
    ARM_FLASH_CAPABILITIES DriverCapabilities;
    uint8_t data_width;

    DriverCapabilities = flash_dev->driver->GetCapabilities();
    data_width = data_width_byte[DriverCapabilities.data_width];
    if (block_id == flash_dev->buf_block_id_0) {
        addr = get_phys_address(cfg, flash_dev->buf_block_id_0, 0);

        /*
         * Flush the buffered write data to flash. For NAND flash,
         * cfg->block_size should always be a multiplier of data_width.
         */
        err = flash_dev->driver->ProgramData(addr, flash_dev->write_buf_0,
                                             cfg->block_size / data_width);
        if (err < 0) {
            return PSA_ERROR_STORAGE_FAILURE;
        }

        /* Clear the write buffer */
        (void)tfm_memset(flash_dev->write_buf_0, 0, flash_dev->buf_size);
        flash_dev->buf_block_id_0 = ITS_BLOCK_INVALID_ID;
    } else if (block_id == flash_dev->buf_block_id_1) {
        addr = get_phys_address(cfg, flash_dev->buf_block_id_1, 0);

        /* Flush the buffered write data to flash*/
        err = flash_dev->driver->ProgramData(addr, flash_dev->write_buf_1,
                                             cfg->block_size / data_width);
        if (err < 0) {
            return PSA_ERROR_STORAGE_FAILURE;
        }

        /* Clear the write buffer */
        (void)tfm_memset(flash_dev->write_buf_1, 0, flash_dev->buf_size);
        flash_dev->buf_block_id_1 = ITS_BLOCK_INVALID_ID;
    } else {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_erase(const struct its_flash_fs_config_t *cfg,
                                         uint32_t block_id)
{
    int32_t err;
    uint32_t addr;
    size_t offset;
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;

    for (offset = 0; offset < cfg->block_size; offset += cfg->sector_size) {
        addr = get_phys_address(cfg, block_id, offset);

        err = flash_dev->driver->EraseSector(addr);
        if (err != ARM_DRIVER_OK) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
    }

    return PSA_SUCCESS;
}

const struct its_flash_fs_ops_t its_flash_fs_ops_nand = {
    .init = its_flash_nand_init,
    .read = its_flash_nand_read,
    .write = its_flash_nand_write,
    .flush = its_flash_nand_flush,
    .erase = its_flash_nand_erase,
};
