/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ITS_FLASH_FS_MBLOCK_H__
#define __ITS_FLASH_FS_MBLOCK_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "flash/its_flash.h"
#include "its_flash_fs.h"
#include "its_utils.h"
#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \def ITS_SUPPORTED_VERSION
 *
 * \brief Defines the supported version.
 */
#define ITS_SUPPORTED_VERSION  0x02

/*!
 * \def ITS_BACKWARD_SUPPORTED_VERSION
 *
 * \brief Defines the backward supported version.
 */
#define ITS_BACKWARD_SUPPORTED_VERSION  0x01

/*!
 * \def ITS_METADATA_INVALID_INDEX
 *
 * \brief Defines the invalid index value when the metadata table is full
 */
#define ITS_METADATA_INVALID_INDEX 0xFFFF

/*!
 * \def ITS_LOGICAL_DBLOCK0
 *
 * \brief Defines logical data block 0 ID
 */
#define ITS_LOGICAL_DBLOCK0  0

/*!
 * \struct its_metadata_block_header_t
 *
 * \brief Structure to store the metadata block header.
 *
 * \note The active_swap_count must be the last member to allow it to be
 *       programmed last.
 *       The fs_version must be at the same position as it is in
 *       its_metadata_block_header_comp_t.
 *
 * \note This structure is programmed to flash, so its size must be padded
 *       to a multiple of the maximum required flash program unit.
 */
#define _T1 \
    uint32_t scratch_dblock;    /*!< Physical block ID of the data \
                                 *   section's scratch block \
                                 */ \
    uint8_t fs_version;         /*!< Filesystem version */ \
    uint8_t metadata_xor;       /*!< XOR value based on the whole metadata(not \
                                 *   including the metadata block header) \
                                 */ \
    uint8_t active_swap_count;  /*!< Number of times the metadata blocks have \
                                 *   been swapped \
                                 */

struct its_metadata_block_header_t {
    _T1
#if ((ITS_FLASH_MAX_ALIGNMENT) > 4)
    uint8_t roundup[sizeof(struct __attribute__((__aligned__(ITS_FLASH_MAX_ALIGNMENT))) { _T1 }) -
                    sizeof(struct { _T1 })]; /*!< _T1 has aligned to the maximum
                                              *   field scratch_dblock which is
                                              *   4 bytes.
                                              */
#endif
};
#undef _T1

/*!
 * \struct its_metadata_block_header_comp_t
 *
 * \brief The struture of metadata block header in
 *        ITS_BACKWARD_SUPPORTED_VERSION.
 *
 * \note The active_swap_count must be the last member to allow it to be
 *       programmed last.
 *
 * \note This structure is programmed to flash, so its size must be padded
 *       to a multiple of the maximum required flash program unit.
 */
#define _T1_COMP \
    uint32_t scratch_dblock;    /*!< Physical block ID of the data \
                                 *   section's scratch block \
                                 */ \
    uint8_t fs_version;         /*!< Filesystem version */ \
    uint8_t active_swap_count;  /*!< Number of times the metadata blocks have \
                                 *   been swapped \
                                 */

struct its_metadata_block_header_comp_t {
    _T1_COMP
#if ((ITS_FLASH_MAX_ALIGNMENT) > 4)
    uint8_t roundup[sizeof(struct __attribute__((__aligned__(ITS_FLASH_MAX_ALIGNMENT))) { _T1_COMP }) -
                    sizeof(struct { _T1_COMP })];
#endif
};
#undef _T1_COMP

/*!
 * \struct its_block_meta_t
 *
 * \brief Structure to store information about each physical flash memory block.
 *
 * \note This structure is programmed to flash, so its size must be padded
 *       to a multiple of the maximum required flash program unit.
 */
#define _T2 \
    uint32_t phy_id;    /*!< Physical ID of this logical block */ \
    size_t data_start;  /*!< Offset from the beginning of the block to the \
                         *   location where the data starts \
                         */ \
    size_t free_size;   /*!< Number of bytes free at end of block (set during \
                         *   block compaction for gap reuse) \
                         */

struct its_block_meta_t {
    _T2
#if ((ITS_FLASH_MAX_ALIGNMENT) > 4)
    uint8_t roundup[sizeof(struct __attribute__((__aligned__(ITS_FLASH_MAX_ALIGNMENT))) { _T2 }) -
                    sizeof(struct { _T2 })];
#endif
};
#undef _T2

/*!
 * \struct its_file_meta_t
 *
 * \brief Structure to store file metadata.
 *
 * \note This structure is programmed to flash, so its size must be padded
 *       to a multiple of the maximum required flash program unit.
 */
#define _T3 \
    uint32_t lblock;               /*!< Logical datablock where file is \
                                    *   stored \
                                    */ \
    size_t data_idx;               /*!< Offset in the logical data block */ \
    size_t cur_size;               /*!< Size in storage system for this # \
                                    *   fragment \
                                    */ \
    size_t max_size;               /*!< Maximum size of this file */ \
    uint32_t flags;                /*!< Flags set when the file was created */ \
    uint8_t id[ITS_FILE_ID_SIZE];  /*!< ID of this file */

struct its_file_meta_t {
    _T3
#if ((ITS_FLASH_MAX_ALIGNMENT) > 4)
    uint8_t roundup[sizeof(struct __attribute__((__aligned__(ITS_FLASH_MAX_ALIGNMENT))) { _T3 }) -
                    sizeof(struct { _T3 })];
#endif
};
#undef _T3

/**
 * \struct its_flash_fs_ctx_t
 *
 * \brief Structure to store the ITS flash file system context.
 */
struct its_flash_fs_ctx_t {
    const struct its_flash_fs_config_t *cfg; /**< Filesystem configuration */
    const struct its_flash_fs_ops_t *ops;    /**< Filesystem flash operations */
    struct its_metadata_block_header_t meta_block_header; /**< Metadata block
                                                           *   header
                                                           */
    uint32_t active_metablock;  /**< Active metadata block */
    uint32_t scratch_metablock; /**< Scratch metadata block */
};

/**
 * \brief Initializes metadata block with the valid/active metablock.
 *
 * \param[in,out] fs_ctx  Filesystem context
 *
 * \return Returns value as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_init(struct its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Copies the file metadata entries between two indexes from the active
 *        metadata block to the scratch metadata block.
 *
 * \param[in,out] fs_ctx     Filesystem context
 * \param[in]     idx_start  File metadata entry index to start copy, inclusive
 * \param[in]     idx_end    File metadata entry index to end copy, exclusive
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_cp_file_meta(struct its_flash_fs_ctx_t *fs_ctx,
                                              uint32_t idx_start,
                                              uint32_t idx_end);

/**
 * \brief Gets current scratch datablock physical ID.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     lblock  Logical block number
 *
 * \return current scratch data block
 */
uint32_t its_flash_fs_mblock_cur_data_scratch_id(
                                              struct its_flash_fs_ctx_t *fs_ctx,
                                              uint32_t lblock);

/**
 * \brief Gets file metadata entry index.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     ID of the file
 * \param[out]    idx     Index of the file metadata in the file system
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_get_file_idx(struct its_flash_fs_ctx_t *fs_ctx,
                                              const uint8_t *fid,
                                              uint32_t *idx);
/**
 * \brief Gets file metadata entry index of the first file with one of the
 *        provided flags set.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     flags   Flags to search for
 * \param[out]    idx     Index of the file metadata in the file system
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_get_file_idx_flag(
                                              struct its_flash_fs_ctx_t *fs_ctx,
                                              uint32_t flags,
                                              uint32_t *idx);

/**
 * \brief Finalizes an update operation.
 *        Last step when a create/write/delete is performed.
 *
 * \param[in,out] fs_ctx  Filesystem context
 *
 * \return Returns offset value in metadata block
 */
psa_status_t its_flash_fs_mblock_meta_update_finalize(
                                             struct its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Writes the files data area of logical block 0 into the scratch
 *        block.
 *
 * \note The files data in the logical block 0 is stored in same physical
 *       block where the metadata is stored. A change in the metadata requires a
 *       swap of physical blocks. So, the files data stored in the current
 *       medadata block needs to be copied in the scratch block, unless
 *       the data of the file processed is located in the logical block 0.
 *
 * \param[in,out] fs_ctx  Filesystem context
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_migrate_lb0_data_to_scratch(
                                             struct its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Reads specified file metadata.
 *
 * \param[in,out] fs_ctx     Filesystem context
 * \param[in]     idx        File metadata entry index
 * \param[out]    file_meta  Pointer to file meta structure
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_read_file_meta(
                                             struct its_flash_fs_ctx_t *fs_ctx,
                                             uint32_t idx,
                                             struct its_file_meta_t *file_meta);

/**
 * \brief Reads specified logical block metadata.
 *
 * \param[in,out] fs_ctx      Filesystem context
 * \param[in]     lblock      Logical block number
 * \param[out]    block_meta  Pointer to block meta structure
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_read_block_metadata(
                                           struct its_flash_fs_ctx_t *fs_ctx,
                                           uint32_t lblock,
                                           struct its_block_meta_t *block_meta);

/**
 * \brief Reads specified logical block metadata based on the backward
 *        compatible FS.
 *
 * \param[in,out] fs_ctx      Filesystem context
 * \param[in]     lblock      Logical block number
 * \param[out]    block_meta  Pointer to block meta structure
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_read_block_metadata_comp(
                                        struct its_flash_fs_ctx_t *fs_ctx,
                                        uint32_t lblock,
                                        struct its_block_meta_t *block_meta);

/**
 * \brief Reserves space for a file.
 *
 * \param[in,out] fs_ctx         Filesystem context
 * \param[in]     fid            File ID
 * \param[in]     use_spare      If true then the spare file will be used,
 *                               otherwise at least one file will be left free
 * \param[in]     size           Size of the file for which space is reserve
 * \param[in]     flags          Flags set when the file was created
 * \param[out]    file_meta_idx  File metadata entry index
 * \param[out]    file_meta      File metadata entry
 * \param[out]    block_meta     Block metadata entry
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_reserve_file(
                                           struct its_flash_fs_ctx_t *fs_ctx,
                                           const uint8_t *fid,
                                           bool use_spare,
                                           size_t size,
                                           uint32_t flags,
                                           uint32_t *file_meta_idx,
                                           struct its_file_meta_t *file_meta,
                                           struct its_block_meta_t *block_meta);

/**
 * \brief Resets metablock by cleaning and initializing the metadatablock.
 *
 * \param[in,out] fs_ctx  Filesystem context
 *
 * \return Returns value as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_reset_metablock(
                                             struct its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Sets current data scratch block
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     phy_id  Physical ID of scratch data block
 * \param[in]     lblock  Logical block number
 */
void its_flash_fs_mblock_set_data_scratch(struct its_flash_fs_ctx_t *fs_ctx,
                                          uint32_t phy_id, uint32_t lblock);

/**
 * \brief Puts logical block's metadata in scratch metadata block
 *
 * \param[in,out] fs_ctx      Filesystem context
 * \param[in]     lblock      Logical block number
 * \param[in]     block_meta  Pointer to block's metadata
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_update_scratch_block_meta(
                                           struct its_flash_fs_ctx_t *fs_ctx,
                                           uint32_t lblock,
                                           struct its_block_meta_t *block_meta);

/**
 * \brief Writes a file metadata entry into scratch metadata block.
 *
 * \param[in,out] fs_ctx     Filesystem context
 * \param[in]     idx        File's index in the metadata table
 * \param[in]     file_meta  Metadata pointer
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_mblock_update_scratch_file_meta(
                                       struct its_flash_fs_ctx_t *fs_ctx,
                                       uint32_t idx,
                                       const struct its_file_meta_t *file_meta);

/**
 * \brief Moves data from source block ID to destination block ID.
 *
 * \param[in] fs_ctx      Filesystem context
 * \param[in] dst_block   Destination block ID
 * \param[in] dst_offset  Destination offset position from the init of the
 *                        destination block
 * \param[in] src_block   Source block ID
 * \param[in] src_offset  Source offset position from the init of the source
 *                        block
 * \param[in] size        Number of bytes to moves
 *
 * \note This function assumes all input values are valid. That is, the address
 *       range, based on blockid, offset and size, is a valid range in flash.
 *       It also assumes that the destination block is already erased and ready
 *       to be written.
 *
 * \return Returns PSA_SUCCESS if the function is executed correctly. Otherwise,
 *         it returns PSA_ERROR_STORAGE_FAILURE.
 */
psa_status_t its_flash_fs_block_to_block_move(struct its_flash_fs_ctx_t *fs_ctx,
                                              uint32_t dst_block,
                                              size_t dst_offset,
                                              uint32_t src_block,
                                              size_t src_offset,
                                              size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_FS_MBLOCK_H__ */
