// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!


#pragma once
#include <stdint.h>
#include <soc/soc.h>
#include "hal_port.h"
#include "system_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t bk_otp_read_nsc(uint32_t item);
bk_err_t bk_otp_apb_read_nsc(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size);
bk_err_t bk_otp_update_nsc(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size);
bk_err_t bk_otp_read_permission_nsc(uint8_t map_id, uint32_t item, uint32_t* permission);
bk_err_t bk_otp_write_permission_nsc(uint8_t map_id, uint32_t item, uint32_t permission);
bk_err_t bk_otp_write_mask_nsc(uint8_t map_id, uint32_t item, uint32_t permission);
bk_err_t bk_otp_write_security_nsc(uint32_t item);
uint32_t bk_otp_read_security_nsc(uint32_t item);
bk_err_t bk_otp_write_puf_security_nsc(uint32_t item);
uint32_t bk_otp_read_puf_security_nsc(uint32_t item);
bk_err_t bk_otp_enable_security_flag_nsc(void);
bk_err_t bk_otp_read_security_flag_nsc(void);

/**
 * @brief Read the SWD enable/disable status from OTP via NSC gateway.
 *
 * This function reads the OTP word (OTP_EFUSE) and checks bit[31],
 * which is used to control the SWD (Serial Wire Debug) functionality.
 *
 * - If bit[31] == 0 -> SWD is enabled
 * - If bit[31] == 1 -> SWD is disabled
 *
 * @note This function does not modify the original bk_otp_read_nsc(),
 *       but provides a parallel implementation specifically for checking
 *       the SWD status.
 *
 * @param[out] swd_enabled
 *             Pointer to a bool variable where the result will be stored.
 *             - true  : SWD enabled (bit[31] = 0)
 *             - false : SWD disabled (bit[31] = 1)
 *
 * @return bk_err_t
 *         - BK_OK           : Operation successful, value stored in swd_enabled
 *         - BK_ERR_PARAM    : Invalid parameter (NULL pointer)
 *         - Other error code: OTP read failure
 */
bk_err_t bk_otp_read_swd_nsc(bool *swd_enabled);


bk_err_t bk_otp_read_download_nsc(bool *download_enabled);

/**
 * @brief Verify a SHA256 public key hash against OTP_BL2_BOOT_PUBLIC_KEY_HASH.
 *
 * The comparison is performed in the secure side: the trusted hash is read
 * from OTP and never handed to the non-secure firmware.
 *
 * @param[in] hash  SHA256 digest of the DER encoded SubjectPublicKeyInfo.
 * @param[in] size  Size of @p hash in bytes, must be 32.
 *
 * @return TFM_OTP_PUBKEY_HASH_MATCH      hash matches the trusted OTP value
 *         TFM_OTP_PUBKEY_HASH_MISMATCH   hash does not match
 *         TFM_OTP_PUBKEY_HASH_READ_FAIL  OTP read failed
 *         TFM_OTP_PUBKEY_HASH_BAD_ARG    invalid argument
 */
int32_t bk_otp_verify_pubkey_hash_nsc(const uint8_t *hash, uint32_t size);

#ifdef __cplusplus
}
#endif
