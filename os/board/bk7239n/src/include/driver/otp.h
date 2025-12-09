// Copyright 2020-2025 Beken
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

#pragma once

#include <common/bk_include.h>
#include <driver/otp_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */
#ifdef CONFIG_OTP
/**
 * @brief     OTP driver init
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: lock init fail
 */
bk_err_t bk_otp_driver_init(void);
/**
 * @brief     OTP driver deinit
 *
 *
 */
void bk_otp_driver_deinit(void);
/**
 * @brief     OTP read with item type
 *
 * @param item the item to read
 * @param buf point to the buffer that reads the data
 * @param size length of item to read
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NO_READ_PERMISSION: wrong permission to read
 *    - BK_ERR_OTP_ADDR_OUT_OF_RANGE: param size not match item real size
 *    - others: other errors.
 */
bk_err_t bk_otp_apb_read(otp1_id_t item, uint8_t *buf, uint32_t size);
/**
 * @brief     update OTP write with item type
 *
 * @param item the item to update
 * @param buf point to the buffer that updates the data
 * @param size length of buffer to update
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NO_WRITE_PERMISSION: wrong permission to write
 *    - BK_ERR_OTP_ADDR_OUT_OF_RANGE: param size exceeds item size
 *    - BK_ERR_OTP_UPDATE_NOT_EQUAL: updated value not match expectation
 *    - BK_FAIL: otp is in use.
 *    - others: other errors.
 */
bk_err_t bk_otp_apb_update(otp1_id_t item, uint8_t* buf, uint32_t size);
/**
 * @brief     OTP2 read with item type
 *
 * @param item the item to read
 * @param buf point to the buffer that reads the data
 * @param size length of item to read
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NO_READ_PERMISSION: wrong permission to read
 *    - BK_ERR_OTP_ADDR_OUT_OF_RANGE: param size not match item real size
 *    - BK_FAIL: otp is in use.
 *    - others: other errors.
 */
bk_err_t bk_otp_ahb_read(otp2_id_t item, uint8_t* buf, uint32_t size);
/**
 * @brief     update OTP2 write with item type
 *
 * @param item the item to update
 * @param buf point to the buffer that updates the data
 * @param size length of buffer to update
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NO_WRITE_PERMISSION: wrong permission to write
 *    - BK_ERR_OTP_ADDR_OUT_OF_RANGE: param size exceeds item size
 *    - BK_ERR_OTP_UPDATE_NOT_EQUAL: updated value not match expectation
 *    - BK_FAIL: otp is in use.
 *    - others: other errors.
 */
bk_err_t bk_otp_ahb_update(otp2_id_t item, uint8_t* buf, uint32_t size);

/**
 * @brief     read OTP1 permission
 *
 * @param item the item to be read
 *
 * @return
 *    - OTP_NO_ACCESS: No Access permission
 *    - OTP_READ_ONLY: Read Only permission
 *    - OTP_READ_WRITE: Read Write permission
 *    - BK_FAIL: otp is in use.
 */
otp_privilege_t bk_otp_apb_read_permission(otp1_id_t item);

/**
 * @brief     write OTP1 permission
 *
 * @param item the item to be written
 * @param permission the permission to be written
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: otp is in use.
 */
bk_err_t bk_otp_apb_write_permission(otp1_id_t item, otp_privilege_t permission);

/**
 * @brief     read OTP1 mask
 *
 * @param item the item to be read
 *
 * @return
 *    - OTP_READ_ONLY: Read Only permission
 *    - OTP_READ_WRITE: Read Write permission
 *    - BK_FAIL: otp is in use.
 */
otp_privilege_t bk_otp_apb_read_mask(otp1_id_t item);

/**
 * @brief     write OTP1 permission
 *
 * @param item the item to be written
 * @param permission the permission to be written
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: otp is in use.
 */
bk_err_t bk_otp_apb_write_mask(otp1_id_t item, otp_privilege_t permission);

/**
 * @brief Write security configuration for OTP
 *
 * @param location The OTP location to set security configuration
 * @return bk_err_t
 */
 bk_err_t bk_otp_write_security(uint32_t location);

 /**
  * @brief Read security configuration for OTP
  *
  * @param location The OTP location to get security configuration
  * @return uint32_t The security configuration value
  */
 uint32_t bk_otp_read_security(uint32_t location);

 /**
  * @brief Write security configuration for PUF
  *
  * @param location The PUF location to set security configuration
  * @return bk_err_t
  */
 bk_err_t bk_otp_write_puf_security(uint32_t location);

 /**
  * @brief Read security configuration for PUF
  *
  * @param location The PUF location to get security configuration
  * @return uint32_t The security configuration value
  */
 uint32_t bk_otp_read_puf_security(uint32_t location);

 /**
  * @brief Enable OTP security protection flag
  *
  *
  * @return
  *    - BK_OK: Security protection flag enabled successfully
  *    - BK_FAIL: Failed to enable security protection flag
  */
 bk_err_t bk_otp_enable_security_flag(void);

 /**
  * @brief Read OTP security protection flag status
  *
  * @return uint32_t The security protection flag status
  *    - 0xF: Security protection is fully enabled
  *    - Other values: Security protection status (implementation specific)
  */
 uint32_t bk_otp_read_security_flag(void);

/**
 * @brief     read random number
 *
 * @param buf the buffer to store value
 * @param size the size of buffer
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: otp is in use.
*/
bk_err_t bk_otp_read_random_number(uint32_t* buf, uint32_t size);

/**
 * @brief     active OTP, internal used by dubhe driver
 *
 * @return
 *    - 0: succeed
 *    - other: error
 */
int bk_otp_active(void);

/**
 * @brief     sleep OTP, internal used by dubhe driver
 *
 * @return
 *    - 0: succeed
 *    - other: error
 */
int bk_otp_sleep(void);
#endif

#ifdef CONFIG_FLASH_OTP
bk_err_t bk_flash_otp_update(uint32_t item_id, uint8_t* buf, size_t size);

bk_err_t bk_flash_otp_read(uint32_t item_id, uint8_t* buf, size_t size);

void bk_flash_otp_init();
#endif

/**
 * @}
 */
/**
 * @}
 */

#ifdef __cplusplus
}
#endif


