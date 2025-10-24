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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t card_version;       /**< the card version */
	uint32_t card_type;          /**< the card version(SDSC/SDHC/SDXC) */
	uint32_t class;              /**< the class of card class */
	uint32_t relative_card_addr; /**< the relaive card address */
} sd_card_info_t;

/**
 * @brief current state of card status,  refer to Card Status BIT[12:9]
 */
typedef enum {
	SD_CARD_IDLE = 0,       /**< card state is idle */
	SD_CARD_READY,          /**< card state is ready */
	SD_CARD_IDENTIFICATION, /**< card is in identification state */
	SD_CARD_STANDBY,        /**< card is in standby state */
	SD_CARD_TRANSFER,       /**< card is in transfer state */
	SD_CARD_SENDING,        /**< card is sending an operation */
	SD_CARD_RECEIVING,      /**< card is receiving operation information */
	SD_CARD_PROGRAMMING,    /**< card is in programming state */
	SD_CARD_DISCONNECTED,   /**< card is disconnected */
	SD_CARD_ERROR = 0xff,
} sd_card_state_t;

/**
 * @brief the sdcard csd info.
 * <<Part1_Physical_Layer_Simplified_Specification_Ver7.10>>
 * CSD registers are tranferred by SDIO sequence is 
 * bit[127:96],bit[95:64],bit[63:32],bit[31:0]
 */
typedef volatile struct {
	/* CSD_0x03 */
	union 
	{ 
		struct 
		{ 
			volatile uint32_t  tran_speed			: 8;
			volatile uint32_t  nsac 				: 8;
			volatile uint32_t  taac 				: 8;
			volatile uint32_t  reserved0			: 6;
			volatile uint32_t  csd_structure		: 2;	//bit[127:126]
		};
		uint32_t v; 
	}csd_3; 

	/* CSD_0x02 */
	union 
	{ 
		struct 
		{ 
			volatile uint32_t  c_size_high			: 10;
			volatile uint32_t  reserved0			: 2;
			volatile uint32_t  dsr_imp				: 1;
			volatile uint32_t  read_blk_misalign	: 1;
			volatile uint32_t  write_blk_misalign	: 1;
			volatile uint32_t  read_bl_partial		: 1;
			volatile uint32_t  read_bl_len			: 4;
			volatile uint32_t  ccc					: 12;
		}v1p0; 

		struct 
		{ 
			volatile uint32_t  c_size_high			: 6;
			volatile uint32_t  reserved0			: 6;
			volatile uint32_t  dsr_imp				: 1;
			volatile uint32_t  read_blk_misalign	: 1;
			volatile uint32_t  write_blk_misalign	: 1;
			volatile uint32_t  read_bl_partial		: 1;
			volatile uint32_t  read_bl_len			: 4;
			volatile uint32_t  ccc					: 12;
		}v2p0; 

		struct 
		{ 
			volatile uint32_t  c_size_high			: 12;
			volatile uint32_t  dsr_imp				: 1;
			volatile uint32_t  read_blk_misalign	: 1;
			volatile uint32_t  write_blk_misalign	: 1;
			volatile uint32_t  read_bl_partial		: 1;
			volatile uint32_t  read_bl_len			: 4;
			volatile uint32_t  ccc					: 12;
		}v3p0; 

		uint32_t v; 
	}csd_2; 

	/* CSD_0x01 */
	union 
	{
		//v1p0
		struct 
		{ 
			volatile uint32_t  wp_grp_size			: 7;
			volatile uint32_t  sector_size			: 7;
			volatile uint32_t  erase_blk_en 		: 1;
			volatile uint32_t  c_size_mult			: 3;
			volatile uint32_t  vdd_w_curr_max		: 3;
			volatile uint32_t  vdd_w_curr_min		: 3;
			volatile uint32_t  vdd_r_curr_max		: 3;
			volatile uint32_t  vdd_r_curr_min		: 3;
			volatile uint32_t  c_size_low			: 2;
		}v1p0;

		//
		struct 
		{ 
			volatile uint32_t  wp_grp_size			: 7;
			volatile uint32_t  sector_size			: 7;
			volatile uint32_t  erase_blk_en 		: 1;
			volatile uint32_t  reserved0			: 1;
			volatile uint32_t  c_size_low			: 16;
		}v2p0_v3p0; 
		uint32_t v; 
	}csd_1; 

	/* CSD_0x00 */
	union 
	{ 
		struct 
		{ 
			volatile uint32_t  reserved0			: 1;	//always 1
			volatile uint32_t  crc					: 7;
			volatile uint32_t  reserved1			: 2;
			volatile uint32_t  file_format			: 2;
			volatile uint32_t  tmp_write_protect	: 1;
			volatile uint32_t  perm_write_protect	: 1;
			volatile uint32_t  copy 				: 1;
			volatile uint32_t  file_format_grp		: 1;
			volatile uint32_t  reserved2			: 5;
			volatile uint32_t  write_bl_partial 	: 1;
			volatile uint32_t  write_bl_len 		: 4;
			volatile uint32_t  r2w_factor			: 3;
			volatile uint32_t  reservd_mmc_compatibility: 2;
			volatile uint32_t  wp_grp_enable		: 1;
		}; 
		uint32_t v; 
	}csd_0; 
} sd_card_csd_t;

#ifdef __cplusplus
}
#endif

