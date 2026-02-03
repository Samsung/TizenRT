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
 * @brief     Get flash capacity
 *
 * @return flash size with bytes
 */
uint32_t bk_flash_get_capacity_bytes(void);
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
__attribute__((section(".itcm_sec_code"))) bk_err_t bk_flash_erase_block(uint32_t address);

/**
 * @brief     Entry flash deep power-down flash, use flash command 0xB9
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: failed
 */
__attribute__((section(".iram"))) bk_err_t bk_flash_enter_deep_sleep(void);

/**
 * @brief     Exit flash deep power-down flash, use flash command 0xAB
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: failed
 */
__attribute__((section(".iram"))) bk_err_t bk_flash_exit_deep_sleep(void);

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
 * @brief  register a callback to be called when flash is busy waiting.
 *         WARNING/NOTES:
 *         1. the wait_cb code is better in ITCM to avoid running in flash.
 *                       or the wait_cb will be blocked until flash is idle.
 *         2.The wait_cb will be called forever until flash is idle, so the implemetion of
 *           wait_cb should garante the programm is safe.
 * @param wait_cb:If flash is writing/erasing, it will block all of other applications.
 *                But maybe the application can't be blocked when flash is writing/erasing.
 *                So the application should register this wait_cb to flash.
 *                When flash is writing/erasing, it will call this wait_cb
 *
 * @return
 *    - BK_OK: succeed
 *    - others: registered too many(>4) wait_cb to flash.
 */
bk_err_t bk_flash_register_wait_cb(flash_wait_callback_t wait_cb);

/**
 * @brief  unregister the wait_cb from flash waiting.
 *
 * @param wait_cb:If flash is writing/erasing, it will block all of other applications.
 *                But maybe the application can't be blocked when flash is writing/erasing.
 *                So the application should register this wait_cb to flash.
 *                When flash is writing/erasing, it will call this wait_cb
 *
 * @return
 *    - BK_OK: succeed
 *    - others: The wait_cb isn't registered to flash.
 */
bk_err_t bk_flash_unregister_wait_cb(flash_wait_callback_t wait_cb);

/**
 * @brief     Set flash operate status
 *
 * @param flash operate status
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_flash_set_operate_status(flash_op_status_t status);

/**
 * @brief     Get flash operate status
 *
 * @return flash operate status
 */
__attribute__((section(".itcm_sec_code"))) flash_op_status_t bk_flash_get_operate_status(void);


/**
 * @brief get flash devices lock
 *
 *
 * @return status
 */
int bk_flash_mutex_lock(void);

/**
 * @brief release flash devices lock
 *
 * @param status
 *
 */
void bk_flash_mutex_unlock(int status);

/**
 * @brief  Set user flash device mutex, the mutex must be recursive mutex
 *
 * @param 
 *    - int (*mutex_lock)(void): flash lock function callback
 *    - void (*mutex_unlock)(int): flash unlock function callback
 */
/*
 * Set user flash device mutex, the mutex must be recursive mutex
 */
void bk_flash_set_mutex(int (*mutex_lock)(void),
                        void (*mutex_unlock)(int));

/**
 * @brief     Check if the address is in the kernel partition
 *
 * @param address address to check
 *
 * @return true if the address is in the kernel partition, false otherwise
 */
bool bk_addr_is_kernel(uint32_t addr);

/**
 * @brief     Check if the address is in the app or common partition
 *
 * @param address address to check
 *
 * @return true if the address is in the app or common partition, false otherwise
 */
bool bk_addr_is_app_or_common(uint32_t addr);

/**
 * @brief     Read data from app or common partition
 *
 * @param address address to read
 * @param user_buf the buffer to read the data
 * @param size size to read
 *
 * @return
 */
int bk_instruction_read_app_or_common(uint32_t address, uint8_t *user_buf, uint32_t size);

/**
 * @brief     Write data to flash with security 
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

/**
 * @brief     Read data from app or common partition
 *
 * @param address address to read
 * @param user_buf the buffer to read the data
 * @param size size to read
 *
 * @return
 */
bk_err_t bk_data_read_app_or_common(uint32_t address, uint8_t *user_buf, uint32_t size);
int bk_security_flash_write_bytes(uint32_t address, const uint8_t *user_buf, uint32_t size);

/**
 * @brief     Read data from flash with security 
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
int bk_security_flash_read_bytes(uint32_t address, uint8_t *user_buf, uint32_t size);

/**
 * @brief     Erase a sector of flash with security 
 *
 * @param address flash address
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */
int bk_security_flash_erase_sector(uint32_t address);

/**
 * @brief     Read data from flash with security 
 *
 * @param address address to read
 * @param user_buf the buffer to read the data
 * @param size size to read
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
int bk_security_flash_read_instruction(uint32_t address, uint8_t *user_buf, uint32_t size, uint32_t offset_flag);

#ifdef __cplusplus
}
#endif


