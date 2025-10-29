/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file  its_flash_fs.h
 *
 * \brief Internal Trusted Storage service filesystem abstraction APIs.
 *        The purpose of this abstraction is to have the ability to plug-in
 *        other filesystems or filesystem proxies (supplicant).
 */

#ifndef __ITS_FLASH_FS_H__
#define __ITS_FLASH_FS_H__

#include <stddef.h>
#include <stdint.h>

#include "its_flash_fs_mblock.h"
#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Space reserved for user-defined flags. These are stored in the file metadata
 * and can be retrieved by getting the file info.
 */
#define ITS_FLASH_FS_USER_FLAGS_MASK   ((1U << 16) - 1)

/* Filesystem flags that can be provided when calling the write function */
#define ITS_FLASH_FS_WRITE_FLAGS_MASK  ((1U << 24) - (1U << 16))
/* Create the file if it does not already exist */
#define ITS_FLASH_FS_FLAG_CREATE       (1U << 16)
/* Remove existing file data if it exists */
#define ITS_FLASH_FS_FLAG_TRUNCATE     (1U << 17)

/* Invalid block index */
#define ITS_BLOCK_INVALID_ID 0xFFFFFFFFU

/**
 * \struct its_flash_fs_config_t
 *
 * \brief Structure containing the flash filesystem configuration parameters.
 */
struct its_flash_fs_config_t {
    const void *flash_dev;    /**< Pointer to the flash device */
    uint32_t flash_area_addr; /**< Base address of the flash region */
    uint32_t sector_size;     /**< Size of the flash device's physical erase
                               *   unit
                               */
    uint32_t block_size;      /**< Size of a logical filesystem erase unit, a
                               *   multiple of sector_size.
                               */
    uint16_t num_blocks;      /**< Number of logical erase blocks */
    uint16_t program_unit;    /**< Minimum size of a program operation */
    uint16_t max_file_size;   /**< Maximum file size */
    uint16_t max_num_files;   /**< Maximum number of files */
    uint8_t erase_val;        /**< Value of a byte after erase (usually 0xFF) */
};

/**
 * \struct its_flash_fs_ops_t
 *
 * \brief Structure containing the filesystem flash operation parameters.
 */
struct its_flash_fs_ops_t {
    /**
     * \brief Initializes the flash device.
     *
     * \param[in] cfg  Filesystem configuration
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*init)(const struct its_flash_fs_config_t *cfg);

    /**
     * \brief Reads block data from the position specified by block ID and
     *        offset.
     *
     * \param[in]  cfg       Filesystem configuration
     * \param[in]  block_id  Block ID
     * \param[out] buf       Buffer pointer to store the data read
     * \param[in]  offset    Offset position from the init of the block
     * \param[in]  size      Number of bytes to read
     *
     * \note This function assumes all input values are valid. That is, the
     *       address range, based on block_id, offset and size, is a valid range
     *       in flash.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*read)(const struct its_flash_fs_config_t *cfg,
                         uint32_t block_id, uint8_t *buf, size_t offset,
                         size_t size);

    /**
     * \brief Writes block data to the position specified by block ID and
     *        offset.
     *
     * \param[in] cfg       Filesystem configuration
     * \param[in] block_id  Block ID
     * \param[in] buf       Buffer pointer to the write data
     * \param[in] offset    Offset position from the init of the block
     * \param[in] size      Number of bytes to write
     *
     * \note This function assumes all input values are valid. That is, the
     *       address range, based on block_id, offset and size, is a valid range
     *       in flash.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*write)(const struct its_flash_fs_config_t *cfg,
                          uint32_t block_id, const uint8_t *buf, size_t offset,
                          size_t size);

    /**
     * \brief Flushes modifications to a block to flash. Must be called after a
     *        sequence of calls to write() (including via
     *        its_flash_block_to_block_move()) for one block ID, before any call
     *        to the same functions for a different block ID.
     *
     * \param[in] cfg  Filesystem configuration
     *
     * \param[in] block_id  Block ID
     *
     * \note It is permitted for write() to commit block updates immediately, in
     *       which case this function is a no-op.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*flush)(const struct its_flash_fs_config_t *cfg,
                          uint32_t block_id);

    /**
     * \brief Erases block ID data.
     *
     * \param[in] cfg       Filesystem configuration
     * \param[in] block_id  Block ID
     *
     * \note This function assumes the input value is valid.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*erase)(const struct its_flash_fs_config_t *cfg,
                          uint32_t block_id);
};

/**
 * \brief ITS flash filesystem context type, used to maintain state across FS
 *        operations.
 *
 * \details The user should allocate a variable of this type, initialised to
 *          zero, before calling its_flash_fs_prepare, and then pass it to each
 *          subsequent FS operation. The contents are internal to the
 *          filesystem.
 */
typedef struct its_flash_fs_ctx_t its_flash_fs_ctx_t;

/*!
 * \struct its_file_info_t
 *
 * \brief Structure to store the file information.
 */
struct its_file_info_t {
    size_t size_current; /*!< The current size of the flash file data */
    size_t size_max;     /*!< The maximum size of the flash file data in bytes.
                          */
    uint32_t flags;      /*!< Flags set when the file was created */
};

/**
 * \brief Initialises the filesystem context. Must be called successfully before
 *        any other filesystem API is called.
 *
 * \param[in,out] fs_ctx  Filesystem context to initialise. Must have been
 *                        allocated by the caller.
 * \param[in]     fs_cfg  Filesystem configuration to associate with the
 *                        context.
 * \param[in]     fs_ops  Filesystem flash operations to associate with the
 *                        context.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_init_ctx(its_flash_fs_ctx_t *fs_ctx,
                                   const struct its_flash_fs_config_t *fs_cfg,
                                   const struct its_flash_fs_ops_t *fs_ops);

/**
 * \brief Prepares the filesystem to accept operations on the files.
 *
 * \param[in,out] fs_ctx  Filesystem context to prepare. Must have been
 *                        initialised by its_flash_fs_init_ctx().
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_prepare(its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Wipes all files from the filesystem.
 *
 * \param[in,out] fs_ctx  Filesystem context to wipe. Must be prepared again
 *                        before further use.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_wipe_all(its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Checks if a file exists in the filesystem.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     File ID
 *
 * \return Returns PSA_SUCCESS if the file exists. If file does not exist, it
 *         returns PSA_ERROR_DOES_NOT_EXIST. Otherwise, it returns error code as
 *         specified in \ref psa_status_t.
 */
psa_status_t its_flash_fs_file_exist(its_flash_fs_ctx_t *fs_ctx,
                                     const uint8_t *fid);

/**
 * \brief Gets the file information referenced by the file ID.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     File ID
 * \param[out]    info    Pointer to the information structure to store the
 *                        file information values \ref its_file_info_t
 *
 * \return Returns error code specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_get_info(its_flash_fs_ctx_t *fs_ctx,
                                        const uint8_t *fid,
                                        struct its_file_info_t *info);

/**
 * \brief Writes data to a file.
 *
 * \param[in,out] fs_ctx     Filesystem context
 * \param[in]     fid        File ID
 * \param[in]     flags      Flags of the file
 * \param[in]     max_size   Maximum size of the file to be created. Ignored if
 *                           the file is not being created.
 * \param[in]     data_size  Size of the incoming write data.
 * \param[in]     offset     Offset in the file to write. Must be less than or
 *                           equal to the current file size.
 * \param[in]     data       Pointer to buffer containing data to be written
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_write(its_flash_fs_ctx_t *fs_ctx,
                                     const uint8_t *fid,
                                     uint32_t flags,
                                     size_t max_size,
                                     size_t data_size,
                                     size_t offset,
                                     const uint8_t *data);

/**
 * \brief Reads data from an existing file.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     File ID
 * \param[in]     size    Size to be read
 * \param[in]     offset  Offset in the file
 * \param[out]    data    Pointer to buffer to store the data
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_read(its_flash_fs_ctx_t *fs_ctx,
                                    const uint8_t *fid,
                                    size_t size,
                                    size_t offset,
                                    uint8_t *data);

/**
 * \brief Deletes file referenced by the file ID.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     File ID
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_delete(its_flash_fs_ctx_t *fs_ctx,
                                      const uint8_t *fid);

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_FS_H__ */
