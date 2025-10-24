// Copyright 2020-2021 Beken
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
#include <driver/flash_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the flash driver
 *
 * This API init the resoure common:
 *   - Init flash driver control memory
 *
 * @attention 1. This API should be called before any other flash APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_driver_init(void);

/**
 * @brief     Deinit the flash driver
 *
 * This API free all resource related to flash and disable flash.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_driver_deinit(void);

/**
 * @brief     Set flash line mode
 *
 * @param line_mode flash line mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_set_line_mode(flash_line_mode_t line_mode);

/**
 * @brief     Get flash line mode
 *
 * @return flash line mode
 */
flash_line_mode_t bk_flash_get_line_mode(void);

/**
 * @brief     Get flash id
 *
 * @return flash line mode
 */
uint32_t bk_flash_get_id(void);

/**
 * @brief     Set flash clock dpll
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_set_clk_dpll(void);

/**
 * @brief     Set flash clock dco
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_set_clk_dco(void);

/**
 * @brief     Enable flash write
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_write_enable(void);

/**
 * @brief     Disable flash write
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_write_disable(void);

/**
 * @brief     Set flash protect type
 *
 * @param type flash protect type
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_set_protect_type(flash_protect_type_t type);

/**
 * @brief     Get flash protect type
 *
 * @return the flash protect type
 */
flash_protect_type_t bk_flash_get_protect_type(void);

/**
 * @brief     Get flash status register value
 *
 * @return the flash status register value
 */
uint16_t bk_flash_read_status_reg(void);

/**
 * @brief     Set flash status register value
 *
 * @param status_reg_data the flash status register data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_write_status_reg(uint16_t status_reg_data);

/**
 * @brief     Erase a sector of flash, use flash command 0x20
 *
 * @param address flash address
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */
__attribute__((section(".itcm_sec_code"))) bk_err_t bk_flash_erase_sector(uint32_t address);

/**
 * @brief     Erase a sector of flash, use flash command 0xD8
 *
 * @param address flash address
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */
__attribute__((section(".itcm_sec_code"))) bk_err_t bk_flash_erase_block_64k(uint32_t address);

/**
 * @brief     Erase a sector of flash, use flash command 0xD8
 *
 * @param address flash address
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */

__attribute__((section(".itcm_sec_code"))) bk_err_t bk_flash_erase_block_32k(uint32_t address);


/**
 * @brief     Write data to flash
 *
 * @param address address to write
 * @param user_buf the pointer to data which is to write
 * @param size size to write
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */
bk_err_t bk_flash_write_bytes(uint32_t address, const uint8_t *user_buf, uint32_t size);

/**
 * @brief     Read data from flash
 *
 * @param address address to read
 * @param user_buf the buffer to read the data
 * @param size size to read
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */
bk_err_t bk_flash_read_bytes(uint32_t address, uint8_t *user_buf, uint32_t size);

/**
 * @brief     Read data from flas
 *
 * @param address address to read
 * @param user_buf the buffer to read the data, intend to psram addr must aline by 4 Bytes
 * @param size size to read, length uint by byte
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */
bk_err_t bk_flash_read_word(uint32_t address, uint32_t *user_buf, uint32_t size);

/**
 * @brief   Get flash  init  flag
 *
 * @return the flash  init  flag
 */
bool bk_flash_is_driver_inited(void);

/**
 * @brief     Get flash total size
 *
 * @return the flash total size
 */
uint32_t bk_flash_get_current_total_size(void);

/**
 * @brief     Register flash power save suspend callback
 *
 * @param ps_suspend_cb power save suspend callback
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_register_ps_suspend_callback(flash_ps_callback_t ps_suspend_cb);

/**
 * @brief     Register flash power save resume callback
 *
 * @param ps_resume_cb power save resume callback
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_register_ps_resume_callback(flash_ps_callback_t ps_resume_cb);

/**
 * @brief  Modify flash speed type
 *
 * @param flash_speed_type the high/low type of flash clock
 * @param modules the app module to control the flash spped
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_flash_clk_switch(uint32_t flash_speed_type, uint32_t modules);

/**
 * @brief  enable cpu write flash
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_cpu_write_enable();

/**
 * @brief  disable cpu write flash
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_cpu_write_disable();

void flash_read_cbus(uint32_t address, void *user_buf, uint32_t size);
void flash_write_cbus(uint32_t address, const uint8_t *user_buf, uint32_t size);
void flash_set_reg_base_addr(uint32_t addr);
void flash_switch_to_line_mode_two(void);
void flash_restore_line_mode(void);
bk_err_t bk_flash_cpu_write_enable(void);
bk_err_t bk_flash_cpu_write_disable(void);

int bk_flash_set_dbus_security_region(uint32_t id, uint32_t start, uint32_t end, bool secure);

#ifdef __cplusplus
}
#endif


