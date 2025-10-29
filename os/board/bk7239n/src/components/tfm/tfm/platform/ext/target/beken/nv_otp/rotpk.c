/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "tfm_plat_otp.h"
#include "nv_otp_log.h"
//#include "mbedtls/otp.h"

#ifdef BL2
enum tfm_plat_err_t
tfm_plat_get_rotpk_hash(uint8_t image_id,
                        uint8_t *rotpk_hash,
                        uint32_t *rotpk_hash_size)
{
	if (image_id == 0) {
		*rotpk_hash_size = 32;
		return tfm_plat_otp_read(PLAT_OTP_ID_BL2_ROTPK_0, 32, rotpk_hash);
        } else {
		OTP_LOGE("invalid image_id=%d\r\n", image_id);
		return -1;
	}

    return TFM_PLAT_ERR_SUCCESS;
}
#endif /* BL2 */
