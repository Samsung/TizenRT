#ifndef _AMEBAD2_PSRAM_H_
#define _AMEBAD2_PSRAM_H_

#include "ameba_spic.h"
#include "ameba_psphy.h"
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
  * @brief  PSRAM PHY Init Structure Definition
  */
typedef struct {
	//0x04 REG_CAL_PAR
	u32 PSRAMP_PRE_CAL_PHASE;		/*!< one-hot phase select, 4-tap delay line to adjust rwds phase before calibration for large freq range or skew
											can be set to 0, bypassing the default 1-tap delay*/
	u32 PSRAMP_CFG_CAL_JMIN;			/*!< Specifies Minimum J . Hardware may adjust J , if J reach this value , an interrup will be raised to alerm software. */
	u32 PSRAMP_CFG_CAL_JMAX;			/*!< Specifies Maximum J . Hardware may adjust J , but will not exceed this value.*/
	u32 PSRAMP_CFG_CAL_J;			/*!< Specifies Initial value J calibrated by software */
	u32 PSRAMP_CFG_CAL_N;			/*!< Specifies Initial value N calibrated by software . */

	//0x18 REG_PHY_CTRL
	u32 PSRAMP_SW_RST_N;		/*!< low active, except for phy register(using system reset), psram software reset: psram, phy_ctrl, calibrationa */
	u32 PSRAMP_LATENCY_TYPE; 		/*!< set this bit according to spic & psram latency settings 0: variable latency 1: fixed latency */
	u32 PSRAMP_CAL_RWDS_PHASE; 		/*!< Specifies the byte size of an address in read/write command in auto_mode. */
	u32 PSRAMP_RFIFO_RDY_DLY;		/* variable latency: delay read data cycles(phy clk) after detecting first DQS rising edge, must < 4
										fixed latency: delay read data cycles(phy clk) after asserting spi_rx_data_en, recommend 4 ~ 6
										4*read_latency - rd_dummy_length + rfifo_rdy_dly = in_physical_cyc */
	u32 DDR_PAD_CTRL1;
	u32 DDR_PAD_CTRL2;
} PSPHY_InitTypeDef;

#if 0
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
	u32 PSRAMC_atom_size;		/*!< Set (0) when the atomic size of SPI device is 1-byte.Set (1) when the atomic size of SPI device is 2-byte.
									(Usually using when OPI and DDR are enable) */
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
	u32 RD_WEIGHT;				/*Set to determine the read and write priority. RD_WEIGHT should >= 2 (if set to 0 or 1, RD_WEIGHT will use the previous value).*/
	u32 WR_VL_EN;				/*Set to enable variable write latency (must enable RD_VL_EN at the same time). SPIC will set variable write latency according to RWDS.*/
	u32 RD_VL_EN;				/*Set to enable variable read latency. SPIC will not sample read data until rx_data_valid = 1.If RD_VL_EN = 0, SPIC will sample read data according to internal counter.
								Note: when RD_VL_EN = 1，0x11C[15: 12] IN_PHYSICAL_CYC should be set to 0 and 0x138[31: 24] CS_TCEM should be set appropriately to avoid TCEM violation.*/
} PCTL_InitTypeDef;
#endif

/* Exported constants --------------------------------------------------------*/
/** @defgroup PSRAM_Exported_Constants PSRAM Exported Constants
  * @{
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
#define WB_WR_INIT_LATENCY_10CLK	0x5

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

#define APM_WR_DUMMY_LATENCY	APM_WR_DUMMY_LATENCY_250	//Modify macro definition to switch PSRAM model
/**
  * @}
  */

/** @defgroup PSRAMP_Latency_Type
  * @{
  */
#define PSRSAM_FIX_LATENCY			0x0		//0 for variable, 1 for fix

/**
  * @}
  */

/** @defgroup PHY_Latency_Definitions
  * @{
  */
#define PSPHY_RFIFO_READY_DELAY_VL		0x0
#define PSPHY_RFIFO_READY_DELAY_FIX		0x6
/**
  * @}
  */

#define PSRAM_PAGE_SIZE_GET(x)		(x & 0xFF)
#define PSRAM_SIZE_GET(x)			(x & 0xFF00)
#define PSRAM_VENDOR_GET(x)			(x & 0xFF0000)
#define PSRAM_CLK_LIMIT_GET(x)		(x & 0xFF000000)

/** @defgroup PSRAM_Size_define
 * @{
 */
enum PSRAM_Size_Info {
	PSRAM_SIZE_32Mb = 0x100,
	PSRAM_SIZE_64Mb = 0x200,
	PSRAM_SIZE_128Mb = 0x300,
	PSRAM_SIZE_256Mb = 0x400,
	PSRAM_SIZE_512Mb = 0x500,

};
/**
  * @}
  */

/** @defgroup PSRAM_Clk_Limit_define
  * @{
  */
enum PSRAM_VENDOR_INFO {
	PSRAM_VENDOR_WB = 0x10000,
	PSRAM_VENDOR_APM = 0x20000,

	PSRAM_VENDOR_NotClear = 0xAA0000,
	PSRAM_VENDOR_NONE = 0xFF0000,

};
/**
  * @}
  */

/** @defgroup PSRAM_Clk_Limit_define
  * @{
  */
enum PSRAM_CLK_LIMIT {
	PSRAM_DEVICE_CLK_166 = 0x1000000,
	PSRAM_DEVICE_CLK_200 = 0x2000000,
	PSRAM_DEVICE_CLK_250 = 0x3000000,
	PSRAM_DEVICE_CLK_NotClear = 0xFF000000,
};
/**
  * @}
  */

/** @defgroup PSRAM_Clk_Limit_define
  * @{
  */
enum PSRAM_PAGE_SIZE {
	PSRAM_PAGE128 = 0x7,
	PSRAM_PAGE1024 = 0xa,
	PSRAM_PAGE2048 = 0xb,
};
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

/** @defgroup CHIPINFO_TypeDef
  * @{
  */
typedef struct {
	u32 Psram_CLK_Limit;
	u32 Psram_Latency;
	u32 psram_type;
} PSRAM_Latency;
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
_LONG_CALL_	void PSRAM_INFO_Update(void);
_LONG_CALL_ void PSRAM_PHY_StructInit(PSPHY_InitTypeDef *PSPHY_InitStruct);
_LONG_CALL_ void PSRAM_PHY_Init(PSPHY_InitTypeDef *PSPHY_InitStruct);
//_LONG_CALL_ void PSRAM_CTRL_StructInit(PCTL_InitTypeDef *PCTL_InitStruct);
_LONG_CALL_ void PSRAM_CTRL_Init(void);
_LONG_CALL_ void PSRAM_REG_Read(u32 type, u32 addr, u32 read_len, u8 *read_data, u32 CR);
_LONG_CALL_ void PSRAM_REG_Write(u32 type, u32 addr, u32 write_len, u8 *write_data);
_LONG_CALL_ void PSRAM_MEM_Write(u8 cmd, u32 addr, u32 write_len, u8 *write_data);
_LONG_CALL_ BOOL PSRAM_calibration(VOID);
//_LONG_CALL_ void PSRAM_CTRL_WB_StructInit(PCTL_InitTypeDef *PCTL_InitStruct);
_LONG_CALL_ void PSRAM_WB_REG_Write(u32 regnum, u32 write_len, u8 *write_data);
_LONG_CALL_ void PSRAM_WB_REG_Read(u32 regnum, u32 read_len, u8 *read_data, u32 CR);
_LONG_CALL_ void PSRAM_APM_DEVIC_Init(void);
_LONG_CALL_ void PSRAM_WB_DEVIC_Init(void);
_LONG_CALL_ void PSRAM_AutoGating(u32 Enable, u32 IDleCnt, u32 ResumeCnt);
_LONG_CALL_ void set_psram_sleep_mode(u32 type);
_LONG_CALL_ void set_psram_wakeup_mode(u32 type);

_LONG_CALL_ VOID PSRAM_CTRL_SPU(u32 state);

_LONG_CALL_ void set_psram_suspend(void);
_LONG_CALL_ void set_psram_resume(void);

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

#define PSRAM_WB_IR0			0x0
#define PSRAM_WB_IR1			0x1
#define PSRAM_WB_CR0			0x0
#define PSRAM_WB_CR1			0x1

#define WB_RD_CMD				0xa0
#define WB_WR_CMD				0x20
#define APM_RD_CMD				0x2020
#define APM_WR_CMD				0xa0a0

#define Psram_RESUME_TIME		32		//unit ns, worst 50ns
#define Psram_IDLETIME_PLL		1		//unit us
#define Psram_IDLETIME_XTAL		10		//unit us
#define Psram_RESUMECNT_XTAL	3		//unit T, 3T * clk

#define Psram_WB_CSHI133		8
#define Psram_WB_CSHI166		6
#define Psram_APM_CSHI250		28
#define Psram_APM_CSHI200		20
#define Psram_APM_CSHI166		18
#define Psram_APM_CSHI133		15

#define PSRAM_TYPE_APM			0
#define PSRAM_TYPE_WB				1
#define PSRAM_TYPE_NONE			0xFF

#define Psram_Tcem_T25	4000	//unit ns, (tcem*32)*busclk.
#define Psram_Tcem_T85	1000	//unit us, (tcem*32)*busclk.

#define Psram_Seq_timeout	0x10	//unit us, (CS_SEQ_TIMEOUT *4) busclk

#define Psram_WB_CSH			0	//spec 0 ns
#define Psram_APM_CSH			0x7	//spec min 2ns, spic max 16T

#define Psram_ClkDiv2			1

#define Psram_phy_Jmin			2

#define PSRAM_PHY_PAD_DRIVING	0x28
#define PSRAM_PHY_VOL_TUNE		0x2C

extern u8 APM_WR_INIT_LATENCY_SPEC[6];

extern PSRAMINFO_TypeDef PsramInfo;

/* MANUAL_GEN_END */

#endif
