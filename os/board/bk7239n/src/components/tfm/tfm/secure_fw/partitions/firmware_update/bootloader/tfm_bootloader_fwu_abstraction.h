/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_BOOTLOADER_FWU_ABSTRACTION_H__
#define __TFM_BOOTLOADER_FWU_ABSTRACTION_H__

#include "stdbool.h"
#include "psa/update.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t bl_image_id_t;

/**
 * Bootloader related initialization for firmware update, such as reading
 * some necessary shared data from the memory if needed.
 *
 * \return PSA_SUCCESS              On success
 *         PSA_ERROR_GENERIC_ERROR  On failure
 */
psa_status_t fwu_bootloader_init(void);

/**
 * \brief Initialize the staging area of the image with given ID.
 *
 * Prepare the staging area of the image with given ID for image download.
 * For example, initialize the staging area, open the flash area, and so on.
 * The image will be written into the staging area later.
 *
 * \param[in] bootloader_image_id The identifier of the target image in
 *                                bootloader
 *
 * \return PSA_SUCCESS                  On success
 *         PSA_ERROR_INVALID_ARGUMENT   Invalid input parameter
 *         PSA_ERROR_GENERIC_ERROR      A fatal error occurred
 *
 */
psa_status_t fwu_bootloader_staging_area_init(bl_image_id_t
                                              bootloader_image_id);

/**
 * \brief Load the image into the target device.
 *
 * Prepare the staging area of the image with given ID for image download.
 * For example, initialize the staging area, open the flash area, and so on.
 * The image will be written into the staging area later.
 *
 * \param[in] bootloader_image_id The identifier of the target image in
 *                                bootloader
 * \param[in] block_offset        The offset of the image being passed into
 *                                block, in bytes
 * \param[in] block               A buffer containing a block of image data.
 *                                This might be a complete image or a subset.
 * \param[in] block_size          Size of block.
 *
 * \return PSA_SUCCESS                     On success
 *         PSA_ERROR_INVALID_ARGUMENT      Invalid input parameter
 *         PSA_ERROR_INSUFFICIENT_MEMORY   There is no enough memory to
 *                                         process the update
 *         PSA_ERROR_INSUFFICIENT_STORAGE  There is no enough storage to
 *                                         process the update
 *         PSA_ERROR_GENERIC_ERROR         A fatal error occurred
 *
 */
psa_status_t fwu_bootloader_load_image(bl_image_id_t bootloader_image_id,
                                       size_t block_offset,
                                       const void *block,
                                       size_t block_size);

/**
 * \brief Starts the installation of an image.
 *
 * Check the authenticity and integrity of the image.
 *
 * If a reboot is required to complete the check, then mark this image as a
 * candidate so that the next time bootloader runs it will take this image
 * as a candidate one to bootup. Return the error code PSA_SUCCESS_REBOOT.
 *
 * \param[in] bootloader_image_id The identifier of the target image in
 *                                bootloader
 * \param[out] dependency         Bootloader image ID of dependency if needed
 * \param[out] dependency_version Bootloader image version of dependency if
 *                                needed
 *
 * \return PSA_SUCCESS         On success
 *         PSA_SUCCESS_REBOOT  A system reboot is needed to finish installation
 *         TFM_SUCCESS_RESTART A restart of the updated component is required
 *                             to complete the update
 *         PSA_ERROR_DEPENDENCY_NEEDED Another image needs to be installed to
 *                                       finish installation
 *         PSA_ERROR_INVALID_ARGUMENT    Invalid input parameter
 *         PSA_ERROR_INVALID_SIGNATURE   The signature is incorrect
 *         PSA_ERROR_GENERIC_ERROR       A fatal error occurred
 *         TFM_ERROR_ROLLBACK_DETECTED   The image is too old to be installed.
 *                                       If the image metadata contains a
 *                                       timestamp and it has expired, then
 *                                       this error is also returned.
 */
psa_status_t fwu_bootloader_install_image(bl_image_id_t bootloader_image_id,
                                          bl_image_id_t *dependency,
                                      psa_image_version_t *dependency_version);

/**
 * \brief Marks the image in the primary slot as confirmed.
 *
 * Call this API to mark the running images as permanent/accepted to avoid
 * revert when next time bootup. Usually, this API is called after the running
 * images have been verified as valid.
 *
 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_GENERIC_ERROR       A fatal error occurred
 */
psa_status_t fwu_bootloader_mark_image_accepted(
                                             bl_image_id_t bootloader_image_id);

/**
 * \brief Abort the current image download process.
 *
 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_INVALID_ARGUMENT    Invalid input parameter
 *         PSA_ERROR_GENERIC_ERROR       A fatal error occurred
 */
psa_status_t fwu_bootloader_abort(bl_image_id_t bootloader_image_id);

/**
 * \brief Get the image information.
 *
 * Get the image information of the given bootloader_image_id in staging area
 * or the running area.
 *
 * \param[in] bootloader_image_id  The identifier of the target image in
 *                                 bootloader
 * \param[in] active_image         Indicates image location
 *                                 True: the running image.
 *                                 False: the image in the passive(or staging)
 *                                 slot.
 * \param[out] info                Buffer containing return the image
 *                                 information
 *
 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_INVALID_ARGUMENT    Invalid input parameter
 *         PSA_ERROR_GENERIC_ERROR       A fatal error occurred
 */
psa_status_t fwu_bootloader_get_image_info(bl_image_id_t bootloader_image_id,
                                           bool active_image,
                                           psa_image_info_t *info);
#ifdef __cplusplus
}
#endif
#endif /* __TFM_BOOTLOADER_FWU_ABSTRACTION_H__ */
