/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash_ram.h"

#include "flash_fs/its_flash_fs.h"
#include "tfm_memory_utils.h"

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
    return (block_id * cfg->block_size) + offset;
}

static psa_status_t its_flash_ram_init(const struct its_flash_fs_config_t *cfg)
{
    /* Nothing needs to be done in case of flash emulated in RAM */
    (void)cfg;
    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_read(const struct its_flash_fs_config_t *cfg,
                                       uint32_t block_id, uint8_t *buff,
                                       size_t offset, size_t size)
{
    uint32_t idx = get_phys_address(cfg, block_id, offset);

    (void)tfm_memcpy(buff, (uint8_t *)cfg->flash_dev + idx, size);

    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_write(const struct its_flash_fs_config_t *cfg,
                                        uint32_t block_id, const uint8_t *buff,
                                        size_t offset, size_t size)
{
    uint32_t idx = get_phys_address(cfg, block_id, offset);

    (void)tfm_memcpy((uint8_t *)cfg->flash_dev + idx, buff, size);

    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_flush(const struct its_flash_fs_config_t *cfg,
                                        uint32_t block_id)

{
    /* Nothing needs to be done for flash emulated in RAM, as writes are
     * commited immediately.
     */
    (void)cfg;
    (void)block_id;
    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_erase(const struct its_flash_fs_config_t *cfg,
                                        uint32_t block_id)
{
    uint32_t idx = get_phys_address(cfg, block_id, 0);

    (void)tfm_memset((uint8_t *)cfg->flash_dev + idx, cfg->erase_val,
                     cfg->block_size);

    return PSA_SUCCESS;
}

const struct its_flash_fs_ops_t its_flash_fs_ops_ram = {
    .init = its_flash_ram_init,
    .read = its_flash_ram_read,
    .write = its_flash_ram_write,
    .flush = its_flash_ram_flush,
    .erase = its_flash_ram_erase,
};
