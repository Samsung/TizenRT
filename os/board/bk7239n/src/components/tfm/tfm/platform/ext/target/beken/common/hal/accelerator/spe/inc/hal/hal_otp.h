// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_OTP_H__
#define __HAL_OTP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

/**
 * Defines all the necessary OTP start offset for IPSS usage.
 * 1. They are aligned with the OTP layout defined in dubhe CE OTP.
 * 2. The offset are Bytes in unit.
 * 3. Don't change them, unless you exactly know what you are doing.
 **/
enum {
    HAL_OTP_MODEL_ID_OFFSET             = 0,
    HAL_OTP_MODEL_KEY_OFFSET            = 4,
    HAL_OTP_DEVICE_ID_OFFSET            = 20,
    HAL_OTP_DEVICE_ROOT_KEY_OFFSET      = 24,
    HAL_OTP_SEC_BOOT_PUBKEY_HASH_OFFSET = 40,
    HAL_OTP_SEC_DBG_PUBKEY_HASH_OFFSET  = 72,
    HAL_OTP_LCS_OFFSET                  = 104,
    HAL_OTP_LOCK_CTRL_OFFSET            = 124,
};

/**
 * The following macros define the
 * 1. primary manifest version
 * 2. recovery manifest version
 * 3. FOTA version
 * start offset in OTP.
 * Customers can change them to their project configuration
 */
#define HAL_OTP_PRIM_MNFT_VER_OFFSET (137)
#define HAL_OTP_RECV_MNFT_VER_OFFSET (136)
#define HAL_OTP_FOTA_VER_OFFSET (128)

/**
 * Defines all the necessary OTP size for IPSS usage.
 * 1. They are aligned with the OTP layout defined in dubhe CE OTP.
 * 2. The offset are Bytes in unit.
 * 3. Don't change them, unless you exactly know what you are doing.
 */

enum {
    HAL_OTP_MODEL_ID_SIZE             = 4,
    HAL_OTP_MODEL_KEY_SIZE            = 16,
    HAL_OTP_DEVICE_ID_SIZE            = 4,
    HAL_OTP_DEVICE_ROOT_KEY_SIZE      = 16,
    HAL_OTP_SEC_BOOT_PUBKEY_HASH_SIZE = 32,
    HAL_OTP_SEC_DBG_PUBKEY_HASH_SIZE  = 32,
    HAL_OTP_LCS_SIZE                  = 4,
    HAL_OTP_LOCK_CTRL_SIZE            = 4,
};

#define HAL_OTP_NECESSARY_TOTAL_SIZE  (HAL_OTP_LOCK_CTRL_OFFSET + HAL_OTP_LOCK_CTRL_SIZE)

/**
 * The following macros define the
 * 1. primary manifest version. The default value is 4, which support maximum 32
 * versions
 * 2. recovery manifest version. The default value is 1, which support maximum 8
 * versions.
 * 3. FOTA version. The default value is 4, which support maximum 32 versions.
 * size in Bytes in OTP.
 * Customers can change them to their project configuration.
 * The maximum version in IPSS Secure boot/FOTA solution only supports multiple
 * of 8.
 */
#define HAL_OTP_PRIM_MNFT_VER_SIZE (4)
#define HAL_OTP_RECV_MNFT_VER_SIZE (1)
#define HAL_OTP_FOTA_VER_SIZE (4)

enum {
    HAL_DBH_PUF_OTP_POWER_ON = 1,
    HAL_DBH_PUF_OTP_POWER_OFF = 2,
};

/**
 * @ingroup	securedebug_hal_otp
 * @brief	hal_otp_init function
 */
HAL_API hal_ret_t hal_otp_init(void);

/**
 * @ingroup	securedebug_hal_otp
 * @brief	hal_otp_cleanup function
 */
HAL_API void hal_otp_cleanup(void);
HAL_API hal_ret_t hal_otp_read(hal_addr_t offset, uint8_t *data, size_t size);
HAL_API hal_ret_t hal_otp_write(hal_addr_t offset,
                                const uint8_t *data,
                                size_t size);
HAL_API hal_ret_t hal_otp_write_user_data(const uint8_t *user_data,
                                          size_t user_data_size);
HAL_API hal_ret_t hal_gen_psk(uint8_t *psk, size_t psk_size);

HAL_API hal_ret_t hal_otp_dbh_puf_power_ctrl(bool is_power_off);
HAL_API hal_ret_t hal_otp_dbh_puf_power_status(uint32_t *status);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_OTP_H__ */