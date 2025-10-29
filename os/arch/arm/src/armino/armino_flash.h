/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/


 #pragma once

 #include <components/log.h>
 #include <driver/flash_types.h>

 #define FLASH_TAG "flash"
 #define FLASH_LOGI(...) BK_LOGI(FLASH_TAG, ##__VA_ARGS__)
 #define FLASH_LOGW(...) BK_LOGW(FLASH_TAG, ##__VA_ARGS__)
 #define FLASH_LOGE(...) BK_LOGE(FLASH_TAG, ##__VA_ARGS__)
 #define FLASH_LOGD(...) BK_LOGD(FLASH_TAG, ##__VA_ARGS__)

 #define FLASH_SIZE_1M                    0x100000
 #define FLASH_SIZE_2M                    0x200000
 #define FLASH_SIZE_4M                    0x400000
 #define FLASH_SIZE_8M                    0x800000
 #define FLASH_SIZE_16M                   0x1000000
 #define FLASH_STATUS_REG_PROTECT_MASK    0xff
 #define FLASH_STATUS_REG_PROTECT_OFFSET  8
 #define FLASH_CMP_MASK                   0x1
 #define FLASH_BUFFER_LEN                 8
 #define FLASH_BYTES_CNT                  32
 #define FLASH_ADDRESS_MASK               0x1f
 #define FLASH_ERASE_SECTOR_MASK          0xFFF000
 #define FLASH_ERASE_BLOCK_MASK           0xFF0000
 #define FLASH_SECTOR_SIZE_MASK           0x000FFF
 #define FLASH_SECTOR_SIZE                0x1000 /* each sector has 4K bytes */
 #define FLASH_SECTOR_SIZE_OFFSET         12
 #define FLASH_PAGE_SIZE                  256 /* each page has 256 bytes */
 #define FLASH_DPLL_DIV_VALUE_TEN         3
 #define FLASH_DPLL_DIV_VALUE_SIX         1
 #define FLASH_ManuFacID_POSI             (16)
 #define FLASH_ManuFacID_GD               (0xC8)
 #define FLASH_ManuFacID_TH               (0xCD)

 #define FLASH_BLOCK32_SIZE               (0x8000)
 #define FLASH_BLOCK_SIZE                 (0x10000)
 #define FLASH_MAX_SIZE                   (FLASH_SIZE_16M)
 #define FLASH_API_MAGIC_CODE             (0x12345678)

 typedef struct {
	 uint32_t flash_id;
	 uint32_t flash_size;
	 uint8_t status_reg_size; /**< the byte count of status register */
	 flash_line_mode_t line_mode;
	 uint8_t cmp_post; /**< CMP bit position in status register */
	 uint8_t protect_post; /**< block protect bits position in status register */
	 uint8_t protect_mask; /**< block protect bits mask value in status register */
	 uint16_t protect_all;
	 uint16_t protect_none;
	 uint16_t unprotect_last_block;
	 uint8_t quad_en_post; /**< quad enable bit position in status register */
	 uint8_t quad_en_val; /**< When the QE pin is set to quad_en_val(1 or 0), the Quad IO2 and IO3 pins are enabled */
	 uint8_t coutinuous_read_mode_bits_val;
 } flash_config_t;


#define CURRENT_ROUTINE_TEXT_SIZE       (0x400)

/*Write Enable for Status Register*/
#define FLASH_CMD_WR_EN_SR              (0x6)
#define FLASH_CMD_WR_DISABLE            (0x04)

/*Write Status Register*/
#define FLASH_CMD_WR_SR                 (0x31)

/*Others*/
#define FLASH_CMD_QUAD_IO_FAST_READ     (0xEB)
#define FLASH_GD25Q32C_SR_QUAD_EN       (0x2)
#define FLASH_CMD_ENTER_DEEP_PWR_DW     (0xB9)
#define FLASH_CMD_EXIT_DEEP_PWR_DW      (0xAB)

/*SYS regs define*/
#define SYS_R_ADD_X(x)                  (0x44010000+((x)*4))

/*SPI0 regs define*/
#define SPI_CFG_TRX_LEN_MASK            (0xffffU)
#define SPI_CFG_TX_TRAHS_LEN_POSI       (8)
#define SPI_CFG_RX_TRAHS_LEN_POSI       (20)
#define SPI_CFG_RX_FIN_INT_EN           (BIT(3))
#define SPI_CFG_TX_FIN_INT_EN           (BIT(2))
#define SPI_CFG_RX_EN                   (BIT(1))
#define SPI_CFG_TX_EN                   (BIT(0))
#define SPI_CFG_TX_EN_ONE_BYTE          (0x10D)
#define SPI_CFG_TX_EN_TWO_BYTE          (0x20D)
#define SPI_STATUS_TX_FINISH_INT        (BIT(13))
#define SPI_STATUS_TXFIFO_WR_READY      (BIT(1))
#define SPI_STATUS_RXFIFO_RD_READY      (BIT(2))

#define FLASH_ID_GD25Q32C               (0xC84016)
#define QE_RETRY_TIMES                  (10)

#if CONFIG_SOC_BK7236XX
__attribute__((section(".itcm_sec_code"))) int flash_bypass_op_write(uint8_t *op_code, uint8_t *tx_buf, uint32_t tx_len);
#else
__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_enable(void);
__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_test(uint32_t quad_enable, uint32_t delay_cycle1, uint32_t delay_cycle2);
#endif