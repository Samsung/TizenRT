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

#ifndef _boot_protocol_h_
#define _boot_protocol_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include <common/bk_err.h>
#include "boot_update.h"

enum
{
	// comon type cmd. distinguished by cmd_type.
	COMMON_CMD_LINK_CHECK   = 0x00,
	COMMON_RSP_LINK_CHECK   = 0x01,	// used as rsp, not command.
	COMMON_BL2_CMD_LINK_CHECK = 0x02,	
	COMMON_RSP_BL2_CMD_LINK_CHECK = 0x03,
	
	COMMON_CMD_REG_WRITE    = 0x01,	// it is a command.
	COMMON_CMD_REG_READ     = 0x03,
	COMMON_CMD_REBOOT       = 0x0E,
	COMMON_CMD_SET_BAUDRATE = 0x0F,
	COMMON_CMD_CHECK_CRC32  = 0x10,
	COMMON_CMD_RESET        = 0x70,
	COMMON_CMD_STAY_ROM     = 0xAA,

	COMMON_CMD_EXT_REG_WRITE= 0x11,
	COMMON_CMD_EXT_REG_READ = 0x13,
	COMMON_CMD_STARTUP      = 0xFE,		// it is a startup indication.
	COMMON_CMD_SET_BOOT_FLAG = 0xF0,
	COMMON_CMD_RPS_CURRENT_BOOT_PART = 0xF1,

	// flash type cmd. distinguished by cmd_type.
	FLASH_CMD_WRITE         = 0x06,
	FLASH_CMD_SECTOR_WRITE  = 0x07,
	FLASH_CMD_READ          = 0x08,
	FLASH_CMD_SECTOR_READ   = 0x09,
	FLASH_CMD_CHIP_ERASE    = 0x0A,
	FLASH_CMD_SECTOR_ERASE  = 0x0B,
	FLASH_CMD_REG_READ      = 0x0C,
	FLASH_CMD_REG_WRITE     = 0x0D,
	FLASH_CMD_SPI_OPERATE   = 0x0E,
	FLASH_CMD_SIZE_ERASE    = 0x0F,

	// can be flash type cmd or common type cmd. 
	// distinguished by cmd_type.
	EXT_CMD_RAM_WRITE       = 0x21,
	EXT_CMD_RAM_READ        = 0x23,
	EXT_CMD_JUMP            = 0x25,

    SECURE_AES_KEY			= 0x26,
    SECURE_RANDOM_KEY		= 0x27,
    SEC_RANDOM_KEY_REV_OVER	= 0x28,
    SECURE_BOOT_ENABLE		= 0x29,
    
};

typedef struct
{
    u32 flash_id;
    u8  sr_size;
    u16  protect_all;
    u16  protect_none;
} flash_config_t;


#define RX_FRM_BUFF_SIZE	(4200)
#define FLASH_4K_SIZE       (0x1000)
#define FLASH_32K_SIZE      (0x8000)
#define FLASH_64K_SIZE      (0x10000)


typedef struct
{
	u32		rx_buf[RX_FRM_BUFF_SIZE / 4];		// buffer align with 32-bits.
	u16		read_idx;
	u16		write_idx;
} rx_link_buf_t;

extern rx_link_buf_t rx_link_buf;

//void wdt_time_set(u32 val);
extern void      uart_disable(void);
extern void      BK3000_start_wdt(u32 val);
u32 boot_rx_frm_handler(void);
void boot_tx_startup_indication(void);
void legacy_boot_main_adapt(void);
u32 boot_rx_frm_handler(void);
#define   wdt_time_set(u32)                                    BK3000_start_wdt(u32)

#ifdef __cplusplus
}
#endif

#endif /* _boot_protocol_h_ */

