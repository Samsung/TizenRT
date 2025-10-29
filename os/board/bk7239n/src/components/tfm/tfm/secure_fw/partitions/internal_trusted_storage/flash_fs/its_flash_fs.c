/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash_fs.h"

#include <stdbool.h>

#include "its_flash_fs_dblock.h"
#include "tfm_memory_utils.h"
#include "its_utils.h"

/* Filesystem-internal flags, which cannot be passed by the caller */
#define ITS_FLASH_FS_INTERNAL_FLAGS_MASK  (UINT32_MAX - ((1U << 24) - 1))
/* Flag that indicates the file is to be deleted in the next block update */
#define ITS_FLASH_FS_FLAG_DELETE          (1U << 24)

static psa_status_t its_flash_fs_delete_idx(struct its_flash_fs_ctx_t *fs_ctx,
                                            uint32_t del_file_idx);

static psa_status_t its_flash_fs_file_write_aligned_data(
                                      struct its_flash_fs_ctx_t *fs_ctx,
                                      const struct its_block_meta_t *block_meta,
                                      const struct its_file_meta_t *file_meta,
                                      size_t offset,
                                      size_t size,
                                      const uint8_t *data)
{
#if (ITS_FLASH_MAX_ALIGNMENT != 1)
    /* Check that the offset is aligned with the flash program unit */
    if (!ITS_UTILS_IS_ALIGNED(offset, fs_ctx->cfg->program_unit)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Set the size to be aligned with the flash program unit */
    size = ITS_UTILS_ALIGN(size, fs_ctx->cfg->program_unit);
#endif

    /* It is not permitted to create gaps in the file */
    if (offset > file_meta->cur_size) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check that the new data is contained within the file's max size */
    if (its_utils_check_contained_in(file_meta->max_size, offset, size)
        != PSA_SUCCESS) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return its_flash_fs_dblock_write_file(fs_ctx, block_meta, file_meta, offset,
                                          size, data);
}

/* TODO This is very similar to (static) its_num_active_dblocks() */
static uint32_t its_flash_fs_num_active_dblocks(
                                        const struct its_flash_fs_config_t *cfg)
{
    /* Total number of datablocks is the number of dedicated datablocks plus
     * logical datablock 0 stored in the metadata block.
     */
    if (cfg->num_blocks == 2) {
        /* Metadata and data are stored in the same physical block, and the
         * other block is required for power failure safe operation.
         */
        /* There are no dedicated data blocks when only two blocks are available
         */
        return 1;
    } else {
        /* One metadata block and two scratch blocks are reserved. One scratch
         * block for metadata operations and the other for file data operations.
         */
        return cfg->num_blocks - 2;
    }
}

static size_t its_flash_fs_all_metadata_size(
                                        const struct its_flash_fs_config_t *cfg)
{
    return sizeof(struct its_metadata_block_header_t)
           + (its_flash_fs_num_active_dblocks(cfg)
              * sizeof(struct its_block_meta_t))
           + (cfg->max_num_files * sizeof(struct its_file_meta_t));
}

/**
 * \brief Validates the configuration of the flash filesystem.
 *
 * This function checks that the flash block provided is compatible with the
 * flash_fs described by the cfg parameter.
 *
 * \param[in] cfg  Filesystem config
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
static psa_status_t its_flash_fs_validate_config(
                                        const struct its_flash_fs_config_t *cfg)
{
    psa_status_t ret = PSA_SUCCESS;

    /* The minimum number of blocks is 2. In this case, metadata and data are
     * stored in the same physical block, and the other block is required for
     * power failure safe operation.
     * If at least 1 data block is available, 1 data scratch block is required
     * for power failure safe operation. So, in this case, the minimum number of
     * blocks is 4 (2 metadata block + 2 data blocks).
     */
    if ((cfg->num_blocks < 2) || (cfg->num_blocks == 3)) {
        ret = PSA_ERROR_INVALID_ARGUMENT;
    }

    if (cfg->num_blocks == 2) {
        /* Metadata and data are stored in the same physical block */
        if (cfg->max_file_size >
                        cfg->block_size - its_flash_fs_all_metadata_size(cfg)) {
            ret = PSA_ERROR_INVALID_ARGUMENT;
        }
    }

    /* It is not required that all files fit in ITS flash area at the same time.
     * So, it is possible that a create action fails because flash is full.
     * However, the larger file must have enough space in the ITS flash area to
     * be created, at least, when the ITS flash area is empty.
     */
    if (cfg->max_file_size > cfg->block_size) {
        ret = PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Metadata must fit in a flash block */
    if (its_flash_fs_all_metadata_size(cfg) > cfg->block_size) {
        ret = PSA_ERROR_INVALID_ARGUMENT;
    }

    return ret;
}

psa_status_t its_flash_fs_init_ctx(its_flash_fs_ctx_t *fs_ctx,
                                   const struct its_flash_fs_config_t *fs_cfg,
                                   const struct its_flash_fs_ops_t *fs_ops)
{
    psa_status_t err;

    if (!fs_ctx || !fs_cfg || !fs_ops) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check for valid filesystem configuration */
    err = its_flash_fs_validate_config(fs_cfg);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Zero the context */
    tfm_memset(fs_ctx, 0, sizeof(*fs_ctx));

    /* Associate the filesystem config and operations with the context */
    fs_ctx->cfg = fs_cfg;
    fs_ctx->ops = fs_ops;

    return PSA_SUCCESS;
}

psa_status_t its_flash_fs_prepare(its_flash_fs_ctx_t *fs_ctx)
{
    psa_status_t err;
    uint32_t idx;

    /* Initialize metadata block with the valid/active metablock */
    err = its_flash_fs_mblock_init(fs_ctx);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Check if a file marked for deletion has been left behind by a power
     * failure. If so, delete it.
     */
    err = its_flash_fs_mblock_get_file_idx_flag(fs_ctx,
                                                ITS_FLASH_FS_FLAG_DELETE, &idx);
    if (err == PSA_SUCCESS) {
        return its_flash_fs_delete_idx(fs_ctx, idx);
    } else if (err != PSA_ERROR_DOES_NOT_EXIST) {
        return err;
    }

    return PSA_SUCCESS;
}

psa_status_t its_flash_fs_wipe_all(struct its_flash_fs_ctx_t *fs_ctx)
{
    /* Clean and initialize the metadata block */
    return its_flash_fs_mblock_reset_metablock(fs_ctx);
}

psa_status_t its_flash_fs_file_exist(struct its_flash_fs_ctx_t *fs_ctx,
                                     const uint8_t *fid)
{
    psa_status_t err;
    uint32_t idx;

    err = its_flash_fs_mblock_get_file_idx(fs_ctx, fid, &idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    return PSA_SUCCESS;
}

psa_status_t its_flash_fs_file_get_info(struct its_flash_fs_ctx_t *fs_ctx,
                                        const uint8_t *fid,
                                        struct its_file_info_t *info)
{
    psa_status_t err;
    uint32_t idx;
    struct its_file_meta_t tmp_metadata;

    /* Get the meta data index */
    err = its_flash_fs_mblock_get_file_idx(fs_ctx, fid, &idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Read file metadata */
    err = its_flash_fs_mblock_read_file_meta(fs_ctx, idx, &tmp_metadata);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Check if index is still referring to same file */
    if (tfm_memcmp(fid, tmp_metadata.id, ITS_FILE_ID_SIZE)) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    info->size_max = tmp_metadata.max_size;
    info->size_current = tmp_metadata.cur_size;
    info->flags = tmp_metadata.flags & ITS_FLASH_FS_USER_FLAGS_MASK;

    return PSA_SUCCESS;
}

psa_status_t its_flash_fs_file_write(struct its_flash_fs_ctx_t *fs_ctx,
                                     const uint8_t *fid,
                                     uint32_t flags,
                                     size_t max_size,
                                     size_t data_size,
                                     size_t offset,
                                     const uint8_t *data)
{
    struct its_block_meta_t block_meta;
    struct its_file_meta_t file_meta = {0};
    uint32_t cur_phys_block;
    psa_status_t err;
    uint32_t idx;
    uint32_t old_idx = ITS_METADATA_INVALID_INDEX;
    uint32_t new_idx = ITS_METADATA_INVALID_INDEX;
    bool use_spare;

    /* Do not permit the user to pass filesystem-internal flags */
    if (flags & ITS_FLASH_FS_INTERNAL_FLAGS_MASK) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

#if (ITS_FLASH_MAX_ALIGNMENT != 1)
    /* Set the max_size to be aligned with the flash program unit */
    max_size = ITS_UTILS_ALIGN(max_size, fs_ctx->cfg->program_unit);
#endif

    /* Check if the file already exists */
    err = its_flash_fs_mblock_get_file_idx(fs_ctx, fid, &old_idx);
    if (err == PSA_SUCCESS) {
        /* Read existing file metadata */
        err = its_flash_fs_mblock_read_file_meta(fs_ctx, old_idx, &file_meta);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_DOES_NOT_EXIST;
        }

        if (flags & ITS_FLASH_FS_FLAG_TRUNCATE) {
            if (file_meta.max_size == max_size) {
                /* Truncate and reuse the existing file, which is already the
                 * correct size.
                 */
                file_meta.cur_size = 0;
                file_meta.flags = flags;
                new_idx = old_idx;
            } else {
                /* Mark the existing file to be deleted in this block update. It
                 * will be deleted in a second block update, and if there is a
                 * power failure before that block update completes, then
                 * deletion will be re-attempted based on this flag.
                 */
                file_meta.flags |= ITS_FLASH_FS_FLAG_DELETE;
                err = its_flash_fs_mblock_update_scratch_file_meta(fs_ctx,
                                                                   old_idx,
                                                                   &file_meta);
                if (err != PSA_SUCCESS) {
                    return PSA_ERROR_GENERIC_ERROR;
                }
            }
        } else {
            /* Write to existing file */
            new_idx = old_idx;
        }
    } else if (err == PSA_ERROR_DOES_NOT_EXIST) {
        /* The create flag must be supplied to create a new file */
        if (!(flags & ITS_FLASH_FS_FLAG_CREATE)) {
            return PSA_ERROR_DOES_NOT_EXIST;
        }
    } else {
        return err;
    }

    /* If the existing file was not reused, then a new one must be reserved */
    if (new_idx == ITS_METADATA_INVALID_INDEX) {
        /* Check that the file's maximum size is valid */
        if (max_size > fs_ctx->cfg->max_file_size) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        /* Only use the spare file if there is an old file to be deleted */
        use_spare = (old_idx != ITS_METADATA_INVALID_INDEX);

        /* Try to reserve a new file based on the input parameters */
        err = its_flash_fs_mblock_reserve_file(fs_ctx, fid, use_spare,
                                               max_size, flags, &new_idx,
                                               &file_meta, &block_meta);
        if (err != PSA_SUCCESS) {
            return err;
        }
    } else {
        /* Read existing block metadata */
        err = its_flash_fs_mblock_read_block_metadata(fs_ctx, file_meta.lblock,
                                                      &block_meta);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    if (data_size != 0) {
        /* Write the content into scratch data block */
        err = its_flash_fs_file_write_aligned_data(fs_ctx, &block_meta,
                                                   &file_meta, offset,
                                                   data_size, data);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        /* Update the file's current size if required */
        if (offset + data_size > file_meta.cur_size) {
            /* Update the file metadata */
            file_meta.cur_size = offset + data_size;
        }

        cur_phys_block = block_meta.phy_id;

        /* Cur scratch block become the active datablock */
        block_meta.phy_id =
            its_flash_fs_mblock_cur_data_scratch_id(fs_ctx, file_meta.lblock);

        /* Swap the scratch data block */
        its_flash_fs_mblock_set_data_scratch(fs_ctx, cur_phys_block,
                                             file_meta.lblock);
    }

    /* Update block metadata in scratch metadata block */
    err = its_flash_fs_mblock_update_scratch_block_meta(fs_ctx,
                                                        file_meta.lblock,
                                                        &block_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Write file metadata in the scratch metadata block */
    err = its_flash_fs_mblock_update_scratch_file_meta(fs_ctx, new_idx,
                                                       &file_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Copy the file metadata entries from the start to the smaller of the two
     * indexes.
     */
    idx = ITS_UTILS_MIN(new_idx, old_idx);
    err = its_flash_fs_mblock_cp_file_meta(fs_ctx, 0, idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Copy the file metadata entries between the two indexes, if necessary */
    if (old_idx != ITS_METADATA_INVALID_INDEX && old_idx != new_idx) {
        err = its_flash_fs_mblock_cp_file_meta(fs_ctx, idx + 1,
                                               ITS_UTILS_MAX(new_idx, old_idx));
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        idx = ITS_UTILS_MAX(new_idx, old_idx);
    }

    /* Copy rest of the file metadata entries */
    err = its_flash_fs_mblock_cp_file_meta(fs_ctx, idx + 1,
                                           fs_ctx->cfg->max_num_files);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* The file data in the logical block 0 is stored in same physical block
     * where the metadata is stored. A change in the metadata requires a swap of
     * physical blocks. So, the file data stored in the current metadata block
     * needs to be copied to the scratch block, if the data of the file
     * processed is not located in the logical block 0. When file data is
     * located in the logical block 0, that copy has been done while processing
     * the file data.
     */
    if ((file_meta.lblock != ITS_LOGICAL_DBLOCK0) || (data_size == 0)) {
        err = its_flash_fs_mblock_migrate_lb0_data_to_scratch(fs_ctx);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    /* Write metadata header, swap metadata blocks and erase scratch blocks */
    err = its_flash_fs_mblock_meta_update_finalize(fs_ctx);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Delete the old file in a second block update.
     * Note: A power failure after this point, but before the deletion has
     * completed, will leave the old file in the filesystem, so it is always
     * necessary to check for files to be deleted at initialisation time.
     */
    if (old_idx != ITS_METADATA_INVALID_INDEX && old_idx != new_idx) {
        err = its_flash_fs_delete_idx(fs_ctx, old_idx);
    }

    return err;
}

static psa_status_t its_flash_fs_delete_idx(struct its_flash_fs_ctx_t *fs_ctx,
                                            uint32_t del_file_idx)
{
    size_t del_file_data_idx;
    uint32_t del_file_lblock;
    size_t del_file_max_size;
    psa_status_t err;
    size_t src_offset = fs_ctx->cfg->block_size;
    size_t nbr_bytes_to_move = 0;
    uint32_t idx;
    struct its_file_meta_t file_meta;

    err = its_flash_fs_mblock_read_file_meta(fs_ctx, del_file_idx, &file_meta);
    if (err != PSA_SUCCESS) {
        return err;
    }

    if (its_utils_validate_fid(file_meta.id) != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Save logical block, data_index and max_size to be used later on */
    del_file_lblock = file_meta.lblock;
    del_file_data_idx = file_meta.data_idx;
    del_file_max_size = file_meta.max_size;

    /* Remove file metadata */
    file_meta = (struct its_file_meta_t){0};

    /* Update file metadata in to the scratch block */
    err = its_flash_fs_mblock_update_scratch_file_meta(fs_ctx, del_file_idx,
                                                       &file_meta);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Read all file metadata */
    for (idx = 0; idx < fs_ctx->cfg->max_num_files; idx++) {
        if (idx == del_file_idx) {
            /* Skip deleted file */
            continue;
        }

        /* Read file meta for the given file index */
        err = its_flash_fs_mblock_read_file_meta(fs_ctx, idx, &file_meta);
        if (err != PSA_SUCCESS) {
            return err;
        }

        /* Check if the file is located in the same logical block and has a
         * valid FID.
         */
        if ((file_meta.lblock == del_file_lblock) &&
            (its_utils_validate_fid(file_meta.id) == PSA_SUCCESS)) {
            /* If a file is located after the data to delete, this
             * needs to be moved.
             */
            if (file_meta.data_idx > del_file_data_idx) {
                /* Check if this is the position after the deleted
                 * data. This will be the first file data to move.
                 */
                if (src_offset > file_meta.data_idx) {
                    src_offset = file_meta.data_idx;
                }

                /* Set the new file data index location in the
                 * data block.
                 */
                file_meta.data_idx -= del_file_max_size;

                /* Increase number of bytes to move */
                nbr_bytes_to_move += file_meta.max_size;
            }
        }
        /* Update file metadata in to the scratch block */
        err = its_flash_fs_mblock_update_scratch_file_meta(fs_ctx, idx,
                                                           &file_meta);
        if (err != PSA_SUCCESS) {
            return err;
        }
    }

    /* Compact data block */
    err = its_flash_fs_dblock_compact_block(fs_ctx, del_file_lblock,
                                            del_file_max_size,
                                            src_offset, del_file_data_idx,
                                            nbr_bytes_to_move);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* The file data in the logical block 0 is stored in same physical block
     * where the metadata is stored. A change in the metadata requires a
     * swap of physical blocks. So, the file data stored in the current
     * metadata block needs to be copied in the scratch block, if the data
     * of the file processed is not located in the logical block 0. When an
     * file data is located in the logical block 0, that copy has been done
     * while processing the file data.
     */
    if (del_file_lblock != ITS_LOGICAL_DBLOCK0) {
        err = its_flash_fs_mblock_migrate_lb0_data_to_scratch(fs_ctx);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    /* Update the metablock header, swap scratch and active blocks,
     * erase scratch blocks.
     */
    return its_flash_fs_mblock_meta_update_finalize(fs_ctx);
}

psa_status_t its_flash_fs_file_delete(struct its_flash_fs_ctx_t *fs_ctx,
                                      const uint8_t *fid)
{
    psa_status_t err;
    uint32_t del_file_idx;

    /* Get the file index */
    err = its_flash_fs_mblock_get_file_idx(fs_ctx, fid, &del_file_idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    return its_flash_fs_delete_idx(fs_ctx, del_file_idx);
}

psa_status_t its_flash_fs_file_read(struct its_flash_fs_ctx_t *fs_ctx,
                                    const uint8_t *fid,
                                    size_t size,
                                    size_t offset,
                                    uint8_t *data)
{
    psa_status_t err;
    uint32_t idx;
    struct its_file_meta_t tmp_metadata;

    /* Get the file index */
    err = its_flash_fs_mblock_get_file_idx(fs_ctx, fid, &idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Read file metadata */
    err = its_flash_fs_mblock_read_file_meta(fs_ctx, idx, &tmp_metadata);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Check if index is still referring to same file */
    if (tfm_memcmp(fid, tmp_metadata.id, ITS_FILE_ID_SIZE)) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Boundary check the incoming request */
    err = its_utils_check_contained_in(tmp_metadata.cur_size, offset, size);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Read the file from flash */
    err = its_flash_fs_dblock_read_file(fs_ctx, &tmp_metadata, offset, size,
                                        data);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}
