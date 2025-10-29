/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_FWU_H__
#define __TFM_FWU_H__

#include <stddef.h>

#include "psa/update.h"
#include "tfm_bootloader_fwu_abstraction.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initialization for starting a firmware update with image_id.
 */
psa_status_t tfm_internal_fwu_initialize(psa_image_id_t image_id);

psa_status_t tfm_internal_fwu_write(psa_image_id_t image_id,
                                    size_t block_offset,
                                    const void *block,
                                    size_t block_size);

/*
 * Starts the installation of an image with image_id. Check the authenticity
 * and integrity of the image. Error code PSA_SUCCESS_REBOOT is returned if a
 * reboot is needed to complete the check.
 */
psa_status_t tfm_internal_fwu_install(psa_image_id_t image_id,
                                      psa_image_id_t *dependency_uuid,
                                      psa_image_version_t *dependency_version);

/*
 * Abort the firmware update process.
 */
psa_status_t tfm_internal_fwu_abort(psa_image_id_t image_id);

/*
 * Get the image information of the given image_id in staging area
 * or the running area.
 */
psa_status_t tfm_internal_fwu_query(psa_image_id_t image_id,
                                    psa_image_info_t *info);

/*
 * Request a reboot.
 */
psa_status_t tfm_internal_fwu_request_reboot(void);

/*
 * Marks the image in the primary slot as confirmed.
 */
psa_status_t tfm_internal_fwu_accept(psa_image_id_t image_id);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_FWU_H__ */
