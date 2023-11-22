/**
  ******************************************************************************
  * @file    ameba_sd.h
  * @author
  * @version V1.0.0
  * @date    2018-06-29
  * @brief   This file contains all the functions prototypes for the SDHOST firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_SDIO_SD_H
#define _AMEBA_SDIO_SD_H

/** @addtogroup SDHOST
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * SDHOST:
  *		- Base Address: SDIOH_BASE
  *		- IP Clock: 100MHz
  *		- Support SD Spec. Version 2.0
  *		- High Voltage SD Memory Card
  *		– Operating voltage range: 2.7-3.6 V
  *		- Support 1/4-bit mode SD
  *		– Bus Speed Mode (using 4 parallel data lines)
  *		    -Default Speed mode: 3.3V signaling, Frequency up to 25 MHz, up to 12.5 MB/sec
  *		    -High Speed mode: 3.3V signaling, Frequency up to 50 MHz, up to 25 MB/sec
  *		- Support hardware CRC function for SD
  *
  *****************************************************************************************
  * How to use SDIO Host Controller
  *****************************************************************************************
  *      To use the SDIO Host Controller, the following steps are mandatory.
  *
  *      1. Insert SD card to card slot.
  *
  *      2. Call SD_Init() function to initialize SDHOST and SD card, in which the following operations are executed:
  *        (1) Enable SDHOST peripheral clock.
  *        (2) Configure the SDHOST pinmux.
  *        (3) Initialize SDIO Host to initial-mode and enable card interrupt.
  *        (4) If card insert is detected, card identification is started.
  *        (5) When card identification is successful, card enters into data transfer mode (Default Speed Mode).
  *        (6) Set SDHOST and card to 4-bit bus width or High Speed mode if needed according to configure parameters.
  *
  *      3.  After initialization, users can call the following functions to read or write blocks.
  *             SD_RESULT SD_ReadBlocks(u32 sector,u8 *data,u32 count);
  *             SD_RESULT SD_WriteBlocks(u32 sector,const u8 *data,u32 count);
  *
  *      Note that if users access SD card through FATFS, then the above steps don't need to be implemented manually.
  *      They are already porting to FATFS low-level driver, and users can call FATFS API directly.
  *      Details can be found in FATFS example.
  *
  *****************************************************************************************
  * @endverbatim
  */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @addtogroup SDHOST
  * @brief SDHOST driver modules
  * @{
  */

/* Exported types --------------------------------------------------------*/
/** @addtogroup SDHOST_Exported_Types SDHOST Exported Types
  * @{
  */

/** @brief SD_Command_Index
  */
typedef enum {
	SD_CMD_GoIdleSte           = 0,
	EMMC_CMD_SendOpCond        = 1,   // CMD only for EMMC
	SD_CMD_AllSendCid          = 2,
	SD_CMD_SendRelAddr         = 3,
	SD_CMD_SetDsr              = 4,
	SD_CMD_SwitchFunc          = 6,
	SD_CMD_SetBusWidth         = 6,   // ACMD6
	SD_CMD_SelDeselCard        = 7,
	SD_CMD_SendIfCond          = 8,   // CMD only for SD card
	EMMC_CMD_SendExtCsd        = 8,   // CMD only for EMMC
	SD_CMD_SendCsd             = 9,
	SD_CMD_SendCid             = 10,
	SD_CMD_VolSwitch           = 11,  // CMD only for SD card
	SD_CMD_StopXsmission       = 12,
	SD_CMD_SendSts             = 13,
	SD_CMD_SetBlklen           = 16,
	SD_CMD_RdSingleBlk         = 17,
	SD_CMD_RdMulBlk            = 18,
	SD_CMD_SendTuningBlk       = 19,  // CMD only for SD card
	SD_CMD_SendNumWrBlks       = 22,  // ACMD22
	SD_CMD_SetBlkCnt           = 23,
	SD_CMD_SetWrBlkEraseCnt    = 23,  // ACMD23
	SD_CMD_WrBlk               = 24,
	SD_CMD_WrMulBlk            = 25,
	SD_CMD_ProgCsd             = 27,
	SD_CMD_EraseBlkSt          = 32,  // CMD only for SD card
	SD_CMD_EraseBlkEd          = 33,  // CMD only for SD card
	EMMC_CMD_EraseAddrSt       = 35,  // CMD only for EMMC
	EMMC_CMD_EraseAddrEd       = 36,  // CMD only for EMMC
	SD_CMD_Erase               = 38,
	SD_CMD_SdSendOpCond        = 41,  // ACMD41 cmd only for SD card
	SD_CMD_SendScr             = 51,  // ACMD51 cmd only for SD card
	SD_CMD_AppCmd              = 55   // CMD only for SD card
} SD_COMMAND;

/** @brief SD_Result Definition
  */
typedef enum {
	SD_OK = 0,
	SD_NODISK,
	SD_INSERT,
	SD_INITERR,
	SD_PROTECTED,
	SD_ERROR,
} SD_RESULT;

/** @brief SD CardInfo Structure Definition
  */
typedef struct {
	u8 csd[SDIOH_CSD_LEN]; 	/*!< Store the card-specific data(CSD) of the current SD card. */
	u16 rca;				/*!< Store the relative address(RCA) of the current SD card. */
	u8 is_sdhc_sdxc;		/*!< Specify the current card is SDSC or SDHC/SDXC. */
	u8 sd_spec_ver;			/*!< Specify the physical layer specification version of current
								card, which would be a value of @ref SD_Specification_Version */
	u32 capaticy;			/*!< Specify the capacity of current card. Unit: KByte */
	u32 read_bl_len;		/*!< Specify max. read data block length of current card. Unit: byte */
	u32 write_bl_len;		/*!< Specify max. write data block length. Unit: byte */

	u8 sig_level;			/*!< Specify current signal level, 0: 3.3v, 1: 1.8v */
	u8 bus_spd;				/*!< Specify current bus speed, which would be a value of @ref SD_Access_Mode */
	SD_RESULT sd_status;	/*!< Specify current sd status, which would be a value of @ref SD_Card_Status */

	u8 dma_buf[SDIOH_C6R2_BUF_LEN] __attribute__((aligned(32)));	/*!< DMA buffer, 32 byte-alignment */
} SD_CardInfo;

/** @brief SDHOST CFG Structure Definition
  */
typedef struct {
	u8 sdioh_bus_speed;		/*!< Specify SDIO Host bus speed, should be SD_SPEED_DS or SD_SPEED_HS*/
	u8 sdioh_bus_width;		/*!< Specify SDIO Host bus width, should be a value of @ref SDIOH_Bus_Width */
	u32 sdioh_cd_pin;		/*!< Specify Card Detect pin, should be a value of _PB_25/_PA_6/_PNC */
	u32 sdioh_wp_pin;		/*!< Specify Write Protection pin, should be a value of _PB_25/_PA_6/_PNC */
} SDIOHCFG_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @addtogroup SDHOST_Exported_Constants SDHOST Exported Constants
  * @{
  */

/** @defgroup SD_Card_Status
  * @{
  */
#define SD_CARD_READY					0x00000001
#define SD_CARD_IDENTIFICATION			0x00000002
#define SD_CARD_STANDBY					0x00000003
#define SD_CARD_TRANSFER				0x00000004
#define SD_CARD_SENDING					0x00000005
#define SD_CARD_RECEIVING				0x00000006
#define SD_CARD_PROGRAMMING				0x00000007
#define SD_CARD_DISCONNECTED			0x00000008
#define SD_CARD_ERROR					0x000000FF
/**
  * @}
  */

/** @defgroup SD_Specification_Version
  * @{
  */
#define SD_SPEC_V101					0
#define SD_SPEC_V110					1
#define SD_SPEC_V200 					2
#define SD_SPEC_V300					3
/**
  * @}
  */

/** @defgroup SD_Access_Mode
  * @{
  */
#define SD_SPEED_DS						0 // 3.3V Function 0
#define SD_SPEED_HS						1 // 3.3V Function 1
#define SD_SPEED_SDR12					2 // 1.8V Function 0
#define SD_SPEED_SDR25					3 // 1.8V Function 1
#define SD_SPEED_SDR50					4 // 1.8V Function 2
#define SD_SPEED_SDR104					5 // 1.8V Function 3
#define SD_SPEED_DDR50					6 // 1.8V Function 4
#define SD_KEEP_CUR_SPEED				15
/**
  * @}
  */

/** @defgroup SD_Block_Size
  * @{
  */
#define SD_BLOCK_SIZE					512    //Bytes
/**
  * @}
  */

/** @defgroup SDIO_RESP4_CMD
  * @{
  */
#define SD_APP_CMD						BIT(5)
/**
  * @}
  */

/** @defgroup SDIO_RESP0_CMD
  * @{
  */
#define SD_ADDRESS_ERROR				BIT(6)
#define SD_BLOCK_LEN_ERROR				BIT(5)
#define SD_WP_VIOLATION					BIT(2)
/**
  * @}
  */

/** @defgroup SDXC_Power_Control_ACMD41
  * @{
  */
#define SD_POWER_SAVING					0
#define SD_MAX_PERFORM					1
/**
  * @}
  */

/** @defgroup SD_Switch_18_Request_ACMD41
  * @{
  */
#define SD_USE_CUR_VOL					0
#define SD_SWITCH_18V					1
/**
  * @}
  */

/** @defgroup SD_Operation_Mode_CMD6
  * @{
  */
#define SD_CMD6_CHECK_MODE				0
#define SD_CMD6_SWITCH_MODE				1
/**
  * @}
  */

/** @defgroup SD_Capacity_Support_ACMD41
  * @{
  */
#define SD_SUPPORT_SDSC_ONLY			0
#define SD_SUPPORT_SDHC_SDXC			1
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SDHOST_Exported_Functions SDHOST Exported Functions
  * @{
  */

/** @defgroup SD_Card_Functions SD Card Functions
  * @{
  */
_LONG_CALL_ SD_RESULT SD_Init(SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ SD_RESULT SD_DeInit(void);
_LONG_CALL_ SD_RESULT SD_GetCapacity(u32 *sector_count, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ SD_RESULT SD_ReadBlocks(u32 sector, u8 *data, u32 count, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ SD_RESULT SD_WriteBlocks(u32 sector, const u8 *data, u32 count, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ SD_RESULT SD_Status(void);
_LONG_CALL_ SD_RESULT SD_GetEXTCSD(u8 *pbuf, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ u32 SD_ReadBlock(uint8_t *readbuff, uint32_t BlockIdx, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ u32 SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t BlockIdx, uint32_t NumberOfBlocks, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ u32 SD_WriteBlock(uint8_t *writebuff, uint32_t BlockIdx, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ u32 SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t BlockIdx, uint32_t NumberOfBlocks, SD_DMASemaStruct *SD_SemStruct);
_LONG_CALL_ u32 SD_Erase(uint32_t StartBlock, uint32_t EndBlock);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Other definitions --------------------------------------------------------*/
#define  SD				0
#define  EMMC			1
#define  SDIO			SD
#define SD_SEMA_MAX_DELAY 10

#endif
