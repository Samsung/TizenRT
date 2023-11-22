#ifndef _AMEBAD2_PSRAM_H_
#define _AMEBAD2_PSRAM_H_

#include "ameba_spic.h"

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PSRAM
* @{
*/

/* AUTO_GEN_START */


/* AUTO_GEN_END */

/* MANUAL_GEN_START */

/* Exported types ------------------------------------------------------------*/
/** @defgroup PSRAM_Exported_Types PSRAM Exported Types
  * @{
  */

/**
  * @brief  PSRAM Init Structure Definition
  */
typedef struct {
	// 0x0 control reg
	u32 PSRAMC_ddr_en;			/*!< Specifies DDR mode in CMD_CH/DATA_CH/ADDR_CH. */
	u32 PSRAMC_ch;				/*!< Specifies channel number of cmd/data/addr phase in transmitting or receiving data. */
	u32 PSRAMC_ck_mtims;			/*!< Specifies the check times */

	//0x14 clk div
	u32 PSRAMC_clk_div;			/*!< Specifies spi_sclk divider value. spi_sclk = The frequency of bus_clk / (2*SCKDV) */

	//0x118
	u32 PSRAMC_user_rd_dummy_len;			/*!< Specifies rd latency in user mode */
	u32 PSRAMC_user_cmd_len;			/*!< Specifies cmd len in user mode */
	u32 PSRAMC_user_addr_len;			/*!< Specifies addr len in user mode */

	//0x11C delay in auto mode and cmd length
	u32 PSRAMC_auto_rd_latency;		/*!< Specifies delay cycles for receiving data */
	u32 PSRAMC_auto_in_phy_cyc; 		/*!< Specifies the byte size of an address in read/write command in auto_mode. */
	u32 PSRAMC_auto_addr_len; 		/*!< Specifies the byte size of an address in read/write command in auto_mode. */

	//0x120 valid cmd
	u32 PSRAMC_frd_single;		/*!< Specifies whether fast read command is a valid command to execute . */
	u32 PSRAMC_seq_rd_en;		/*!< Specifies whether to enable read sequential transaction read function in auto mode. */
	u32 PSRAMC_seq_wr_en;		/*!< Specifies whether to enable write sequential transaction read function in auto mode. */

	//0x138 Timing para
	u32 PSRAMC_rd_dum_len; 		/*!< Specifies the dummy cycles between sending read cmd to spic. to avoid the timing violation of CS high time*/
	u32 PSRAMC_wr_dum_len;		/*!< Specifies the dummy cycles between sending write cmd to spic. to avoid the timing violation of CS high time.*/
	u32 PSRAMC_cs_active_hold;	/*!< Specifies the active hold time after SCLK rising edge, max 4.*/
	u32 PSRAMC_cs_seq_timeout;	/*!< Specifies the timeout setting of auto cmd after aeq read cmd.*/
	u32 PSRAMC_cs_tcem;			/*!< Set to chop auto cmd when CSN low pulse width = (tcem*32)*busclk.*/

	//0x134 device info
	u32 PSRAMC_page_size;		/*!< Specifies the page size of psram, device page size = 2^PSRAMC_page_size*/
	u32 PSRAMC_wr_page_en;		/*!< Specifies whether to chop the burst write cmd across page boundaries in auto mode */
	u32 PSRAMC_rd_page_en;		/*!< Specifies whether to chop the burst read cmd across page boundaries in auto mode*/
	u32 PSRAMC_qspi_psram;		/*!< choose device type: psram */
	u32 PSRAMC_jedec_p2cmf;		/*!< Set (1) when the SPI cmd is JEDEC Profile 2.0 Command Modifier Formats. (6-byte command and address)*/
	u32 PSRAMC_data_unit_2b;	/*!< Set (1) when the SPI device stores 2-byte data for each address.*/

	//0x110 hw status
	u32 PSRAMC_so_dnum; 			/*!< Specifies single channel(0) connection of multi channels(1) */
	u32 PSRAMC_tx_fifo_entry; 			/*!< <5 when fifo entry = 32 */
	u32 PSRAMC_rx_fifo_entry; 		/*!< Specifies quad data read cmd */
	u32 PSRAMC_full_wr; 		/*!< Specifies whether to enable full write in auto mode, set 0 to use spi_dm to do partial write */
	u32 PSRAMC_dis_dm_ca; 		/*!< Specifies whether to Set to disable spi_dm_oe_n (let the device to drive DM) when SPIC pushing CMD and ADDR for both user mode and auto mode. */

	//0x13c wr latency
	u32 PSRAMC_auto_wr_latency;		/*!< Specifies delay cycles for writing data */

	//0xe0 rd cmd
	u32 PSRAMC_frd_cmd;			/*!< Specifies fast read cmd in auto mode */

	//0xf4 wr cmd
	u32 PSRAMC_wr_cmd;			/*!< Specifies write cmd in auto mode */

} PCTL_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
/** @defgroup PSRAM_Exported_Constants PSRAM Exported Constants
  * @{
  */

/** @defgroup APM_PSRAM_Init_Latency
  * @{
  */
#define APM_WR_INIT_LATENCY_3CLK		0
#define APM_WR_INIT_LATENCY_4CLK		0x4
#define APM_WR_INIT_LATENCY_5CLK		0x2
#define APM_WR_INIT_LATENCY_6CLK		0x6
#define APM_WR_INIT_LATENCY_7CLK		0x1
#define APM_WR_INIT_LATENCY_8CLK		0x5
/**
  * @}
  */

/** @defgroup WB_PSRAM_Init_Latency
  * @{
  */
#define WB_WR_INIT_LATENCY_3CLK		0xe
#define WB_WR_INIT_LATENCY_4CLK		0xf
#define WB_WR_INIT_LATENCY_5CLK		0x0
#define WB_WR_INIT_LATENCY_6CLK		0x1
#define WB_WR_INIT_LATENCY_7CLK		0x2
/**
  * @}
  */

/** @defgroup APM_PSRAM_Dummy_Latency
  * @{
  */
#define APM_WR_DUMMY_LATENCY_100		0x3
#define APM_WR_DUMMY_LATENCY_150		0x6
#define APM_WR_DUMMY_LATENCY_200		0x8
#define APM_WR_DUMMY_LATENCY_250		0xE
/**
  * @}
  */

/** @defgroup PHY_Latency_Definitions
  * @{
  */
#define PSPHY_RFIFO_READY_DELAY_VL		0x0
#define PSPHY_RFIFO_READY_DELAY_FIX		0x4
/**
  * @}
  */

/** @defgroup PSRAMINFO_TypeDef
  * @{
  */
typedef struct {
	u32 Psram_Vendor;
	u32 Psram_Clk_Limit;
	u32 Psram_Size;
	u32 Psram_Clk_Set;
	u32 Psram_Page_size;
	u32 PSRAMC_Clk_Unit;
	u32 Psram_Latency_Set;
	u32 Psram_CSHI;			//cs high between cmd
	u32 Psram_Resume_Cnt;	//for autogating phy resume
} PSRAMINFO_TypeDef;
/**
  * @}
  */

/** @defgroup PSRAM_APM_Device_Register
  * @{
  */
#define PSRAM_READ_LATENCY_CODE(x)		((u8)(((x) & 0x07) << 2))		//100:200M, LC=7; 101:250M, LC=8
#define PSRAM_LT_SELECT(x)				((u8)(((x) & 0x01) << 5))		//0:variable 1:fix
#define PSRAM_WRITE_LATENCY_CODE(x)		((u8)(((x) & 0x07) << 5))		//001: 200M LC=7; 101:250M LC=8
/**
  * @}
  */

/** @defgroup PSRAM_WB_Device_Register
  * @{
  */
#define PSRAM_WB_BURST_LENGTH(x)		((u8)(((x) & 0x03) << 0))	/*00: 128byte 01:64byte 10:16byte 11:32byte */
#define PSRAM_WB_HyBURSE_EN				((u8) 0x01 << 2)	/* 0:hybrid burst sequencing 1:legacy wrapped burst manner */
#define PSRAM_WB_FIX_LATENCY_EN(x)		((u8)((x) & 0x01) << 3)	/* 0:Variable Latency 1:Fixed 2 times Initial Latency */
#define PSRAM_WB_INIT_LATENCY(x)		((u8)(((x) & 0x0f) << 4))	/*0010b:7 Clock Latency @ 250/200MHz Max Frequency  */

#define PSRAM_WB_RESERVER				((u8)(0x0F << 0))	/*When writing this register, these bits should be set to 1 for future compatibility */
#define PSRAM_WB_DRIVE_STRENGTH(x)		((u8)(((x) & 0x07) << 4))	/*001b - 115 ohms */
#define PSRAM_WB_DPD_EN(x)				((u8)(((x) & 0x01) << 7))	/*1:Normal operation 0:Writing 0 to CR0[15] causes the device to enter Deep Power Down (DPD) */
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PSRAM_Exported_Functions PSRAM Exported Functions
  * @{
  */

_LONG_CALL_ void PSRAM_CTRL_StructInit(PCTL_InitTypeDef *PCTL_InitStruct);
_LONG_CALL_ void PSRAM_CTRL_Init(void);
_LONG_CALL_ void PSRAM_REG_Read(u32 addr, u32 read_len, u8 *read_data);
_LONG_CALL_ void PSRAM_REG_Write(u32 addr, u32 write_len, u8 *write_data);
_LONG_CALL_ void PSRAM_MEM_Write(u8 cmd, u32 addr, u32 write_len, u8 *write_data);
_LONG_CALL_ BOOL PSRAM_calibration(void);
_LONG_CALL_ void PSRAM_CTRL_WB_StructInit(PCTL_InitTypeDef *PCTL_InitStruct);
_LONG_CALL_ void PSRAM_WB_REG_Write(u32 regnum, u32 write_len, u8 *write_data);
_LONG_CALL_ void PSRAM_WB_REG_Read(u32 regnum, u32 read_len, u8 *read_data, u32 CR);
_LONG_CALL_ void PSRAM_AutoGating(u32 Enable, u32 IDleCnt, u32 ResumeCnt);
_LONG_CALL_ void PSRAM_APM_DEVIC_Init(void);
_LONG_CALL_ void PSRAM_WB_DEVIC_Init(void);
_LONG_CALL_	void PSRAM_INFO_Update(void);

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
extern u8 APM_WR_INIT_LATENCY_SPEC[6];

#define PSRAM_BIT_MR0_FIX_LATENCY       ((u8)0x01 << 5)
#define PSRAM_SLOW_REFRSH_ENABLE		((u8)0x01 << 3)		//mr4

#define PSRAM_WB_IR0		0x0
#define PSRAM_WB_IR1		0x1
#define PSRAM_WB_CR0		0x0
#define PSRAM_WB_CR1		0x1

#define WB_RD_CMD				0xa0
#define WB_WR_CMD				0x20
#define APM_RD_CMD				0x2020
#define APM_WR_CMD			0xa0a0

#define 	PSRAM_PAGE1024		0xa

#define Psram_RESUME_TIME		32		//unit ns, worst 50ns
#define Psram_IDLETIME			1		//unit us

#define Psram_WB_CSHI133		8
#define Psram_WB_CSHI166		6
#define Psram_APM_CSHI250		28
#define Psram_APM_CSHI200		20
#define Psram_APM_CSHI166		18
#define Psram_APM_CSHI133		15

#define PSRAM_TYPE_APM		0
#define PSRAM_TYPE_WB			1

#define Psram_Tcem_T25	4000	//unit ns, (tcem*32)*busclk.
#define Psram_Tcem_T85	1000	//unit us, (tcem*32)*busclk.

#define Psram_Seq_timeout	0x10	//unit us, (CS_SEQ_TIMEOUT *4) busclk

#define Psram_WB_CSH			0	//spec min 0 ns
#define Psram_APM_CSH			0x7	//spec min 2ns, spic max 16T

#define Psram_ClkDiv2			1

#define Psram_phy_Jmin			2

extern PSRAMINFO_TypeDef PsramInfo;

/* MANUAL_GEN_END */

#endif
