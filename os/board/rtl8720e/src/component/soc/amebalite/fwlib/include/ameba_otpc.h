/**
  ******************************************************************************
  * @file    ameba_otpc.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the EFUSE firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup EFUSE
  * @brief EFUSE driver modules
  * @{
  */

/** @addtogroup EFUSE
  * @verbatim
  *****************************************************************************************
  * logical map space total size 2048byte ,only 1024 byte used now
  *****************************************************************************************
  *
  * 0x00~0x07	8bytes system autoload for software
  * 0x08~0x01F	24bytes  autoload for hardware
  * 0x20~0xCF	WIFI calibration data
  * 0xD0~0x11F	WIFI HCI CIS
  * 0x130~0x13F	WIFI SW/RF  Reserved
  * 0x140~0x1AF	112bytes  autoload for hardware
  * 0x1B0~0x1FF	80bytes BT calibration data
  * 0x1B0~0x1FF	80bytes BT calibration data
  * 0x200~0x20F	ADC calibration data
  * 0x210~0x21F	USB HCI
  * 0x220~0x22F	captouch calibration data
  * 0x230~0x2FF	RSVD for user use
  * 0x300~0x3FF	Realtek RSVD
  * 0x400~0x7FF	No define, SW API can't access.
  *
  *****************************************************************************************
  * physical map (1280B)
  *****************************************************************************************
  *
  * 0x000~0x1FE	For logical efuse, user can read
  * 0x200~0x3FF	Security related function
  * 0x400~0x500 	User define physical area
  *
  *****************************************************************************************
  * USER Section
  *****************************************************************************************
  * can be changed after write
  *
  *0x230~0x2FF	RSVD for user use
  *
  *****************************************************************************************
  * OTP Section (32B)
  *****************************************************************************************
  *
  * can not be changed after write
  *
  *0x400~0x500 	User define physical area
  *
  *****************************************************************************************
  * @endverbatim
  */


#ifndef _AMEBAD2_OTPC_H_
#define _AMEBAD2_OTPC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup OTPC
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup OTPC_Register_Definitions OTPC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup OTPC_OTP_ALD
 * @brief
 * @{
 **/
#define OTPC_BIT_LOAD_FAIL        ((u32)0x00000001 << 2)          /*!<R 0  Autoload fail flag */
#define OTPC_BIT_LOAD_DONE        ((u32)0x00000001 << 1)          /*!<R 0  Autoload done flag */
#define OTPC_BIT_SRST             ((u32)0x00000001 << 0)          /*!<R/W 0  Soft reset data path except register */
/** @} */

/** @defgroup OTPC_OTP_SCAN
 * @brief
 * @{
 **/
#define OTPC_MASK_EF_SCAN_SADDR   ((u32)0x000001FF << 17)          /*!<R/W 0  EFuse Scan start Address (unit in byte) */
#define OTPC_EF_SCAN_SADDR(x)     ((u32)(((x) & 0x000001FF) << 17))
#define OTPC_GET_EF_SCAN_SADDR(x) ((u32)(((x >> 17) & 0x000001FF)))
#define OTPC_MASK_EF_SCAN_EADDR   ((u32)0x000001FF << 8)          /*!<R/W 0  EFuse Scan End address */
#define OTPC_EF_SCAN_EADDR(x)     ((u32)(((x) & 0x000001FF) << 8))
#define OTPC_GET_EF_SCAN_EADDR(x) ((u32)(((x >> 8) & 0x000001FF)))
#define OTPC_BIT_EF_SCAN_FAIL     ((u32)0x00000001 << 7)          /*!<R 0  Test Scan Report 1: Fail 0: Ok */
#define OTPC_MASK_EF_SCAN_THD     ((u32)0x0000007F << 0)          /*!<R/WPD/ES 0  Write this field will trig a scan operation */
#define OTPC_EF_SCAN_THD(x)       ((u32)(((x) & 0x0000007F) << 0))
#define OTPC_GET_EF_SCAN_THD(x)   ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup OTPC_OTP_AS
 * @brief
 * @{
 **/
#define OTPC_BIT_EF_RD_WR_NS      ((u32)0x00000001 << 31)          /*!<R/WPD/ET 0  Write this bit will trig an indirect read or write. Write 1: trigger write write 0: trigger read After this operation done, this bit will toggle. */
#define OTPC_MASK_EF_MODE_SEL_NS  ((u32)0x00000007 << 19)          /*!<R/W 0  OTP Mode Selection 000: User Mode 001: Initial Margin Read Mode（room temp） 010: Program Margin Read Mode（room temp） 011: Initial Margin Read Mode（high temp） 100: Program Margin Read Mode（high temp） */
#define OTPC_EF_MODE_SEL_NS(x)    ((u32)(((x) & 0x00000007) << 19))
#define OTPC_GET_EF_MODE_SEL_NS(x) ((u32)(((x >> 19) & 0x00000007)))
#define OTPC_MASK_EF_ADDR_NS      ((u32)0x000007FF << 8)          /*!<R/W 0  OTP indirect read or write address */
#define OTPC_EF_ADDR_NS(x)        ((u32)(((x) & 0x000007FF) << 8))
#define OTPC_GET_EF_ADDR_NS(x)    ((u32)(((x >> 8) & 0x000007FF)))
#define OTPC_MASK_EF_DATA_NS      ((u32)0x000000FF << 0)          /*!<R/W/ES 0  OTP indirect read or write data */
#define OTPC_EF_DATA_NS(x)        ((u32)(((x) & 0x000000FF) << 0))
#define OTPC_GET_EF_DATA_NS(x)    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup OTPC_OTP_AS_SEC
 * @brief
 * @{
 **/
#define OTPC_BIT_EF_RD_WR_S       ((u32)0x00000001 << 31)          /*!<R/WPD/ET 0  Write this bit will trig an indirect read or write. Write 1: trigger write write 0: trigger read After this operation done, this bit will toggle. */
#define OTPC_MASK_EF_MODE_SEL_S   ((u32)0x00000007 << 19)          /*!<R/W 0  OTP Mode Selection 000: User Mode 001: Initial Margin Read Mode（room temp） 010: Program Margin Read Mode（room temp） 011: Initial Margin Read Mode（high temp） 100: Program Margin Read Mode（high temp） */
#define OTPC_EF_MODE_SEL_S(x)     ((u32)(((x) & 0x00000007) << 19))
#define OTPC_GET_EF_MODE_SEL_S(x) ((u32)(((x >> 19) & 0x00000007)))
#define OTPC_MASK_EF_ADDR_S       ((u32)0x000007FF << 8)          /*!<R/W 0  OTP indirect read or write address */
#define OTPC_EF_ADDR_S(x)         ((u32)(((x) & 0x000007FF) << 8))
#define OTPC_GET_EF_ADDR_S(x)     ((u32)(((x >> 8) & 0x000007FF)))
#define OTPC_MASK_EF_DATA_S       ((u32)0x000000FF << 0)          /*!<R/W/ES 0  OTP indirect read or write data */
#define OTPC_EF_DATA_S(x)         ((u32)(((x) & 0x000000FF) << 0))
#define OTPC_GET_EF_DATA_S(x)     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup OTPC_OTP_TEST_MODE
 * @brief
 * @{
 **/
#define OTPC_MASK_EF_MODE         ((u32)0x0000000F << 0)          /*!<R/WPD 0  OTP Test Mode Selection 0000: User Read Mode 0001: Initial Margin Read Mode（room temp） 0010: Program Margin Read Mode（room temp） 0011: Initial Margin Read Mode（high temp） 0100: Program Margin Read Mode（high temp） 0101: User Write Mode 1001: Repair Margin Read Mode 1010: Repair Check Mode 1011: Repair Program Mode Bit[3]=0: User mode and Test mode Bit[3]=1: Use Repair mode */
#define OTPC_EF_MODE(x)           ((u32)(((x) & 0x0000000F) << 0))
#define OTPC_GET_EF_MODE(x)       ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup OTPC_OTP_CTRL
 * @brief
 * @{
 **/
#define OTPC_MASK_EF_PG_PWD       ((u32)0x000000FF << 24)          /*!<R/W 8'b0  OTP program password. OTP can be programed only when this value is 8'h69. */
#define OTPC_EF_PG_PWD(x)         ((u32)(((x) & 0x000000FF) << 24))
#define OTPC_GET_EF_PG_PWD(x)     ((u32)(((x >> 24) & 0x000000FF)))
#define OTPC_MASK_EF_DSLP_TIM     ((u32)0x0007FFFF << 0)          /*!<R/W 19'd399999  OTP auto return back to dslp if no operation during ef_dslp_tim (40Mhz) */
#define OTPC_EF_DSLP_TIM(x)       ((u32)(((x) & 0x0007FFFF) << 0))
#define OTPC_GET_EF_DSLP_TIM(x)   ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup OTPC_OTP_TEST
 * @brief
 * @{
 **/
#define OTPC_MASK_EF_RP_CHK       ((u32)0x00000003 << 3)          /*!<R 0  Check result 2'b00: Good sample 2'b01: Repairable sample 2'b11: Failure sample */
#define OTPC_EF_RP_CHK(x)         ((u32)(((x) & 0x00000003) << 3))
#define OTPC_GET_EF_RP_CHK(x)     ((u32)(((x >> 3) & 0x00000003)))
#define OTPC_MASK_EF_TROW_EN      ((u32)0x00000003 << 1)          /*!<R/W 0  Test row enable 2'b01: test row enable 2'b10: test column enable */
#define OTPC_EF_TROW_EN(x)        ((u32)(((x) & 0x00000003) << 1))
#define OTPC_GET_EF_TROW_EN(x)    ((u32)(((x >> 1) & 0x00000003)))
#define OTPC_BIT_EF_PGRD_IMR      ((u32)0x00000001 << 0)          /*!<R/W 0  When using mode selection under OTP_Test , this signal needs to be raised to mask mode selection in OTP_AS_SEC/OTP_AS to avoid mode conflict 0: Unmask Program Margin Read Mode and Initial Margin Read Mode of OTP_AS_SEC/OTP_AS register 1: Mask Program Margin Read Mode and Initial Margin Read Mode of OTP_AS_SEC/OTP_AS register */
/** @} */

/** @defgroup OTPC_OTP_TIME
 * @brief
 * @{
 **/
#define OTPC_MASK_EF_PPH          ((u32)0x00000007 << 15)          /*!<R/W 3'b010  OTP Program Hold time. In the unit of cycle time (5us< Tpph <20us) For input clock 20 MHz/26 MHz 0: 4us 1: 6us 2: 8us (default) 3: 10us 4: 12us 5: 14us 6: 18us 7: 21us */
#define OTPC_EF_PPH(x)            ((u32)(((x) & 0x00000007) << 15))
#define OTPC_GET_EF_PPH(x)        ((u32)(((x >> 15) & 0x00000007)))
#define OTPC_MASK_EF_PPS          ((u32)0x00000007 << 12)          /*!<R/W 3'b010  OTP Program Setup time time. In the unit of cycle time (5us< Tpps<20us) For input clock 20 MHz/26 MHz 0: 4us 1: 6us 2: 8us (default) 3: 10us 4: 12us 5: 14us 6: 18us 7: 21us */
#define OTPC_EF_PPS(x)            ((u32)(((x) & 0x00000007) << 12))
#define OTPC_GET_EF_PPS(x)        ((u32)(((x >> 12) & 0x00000007)))
#define OTPC_MASK_EF_PPSR         ((u32)0x00000007 << 9)          /*!<R/W 3'b010  OTP Program Recovery time. In the unit of cycle time (5us< Tppr<100us) For input clock 20 MHz/26 MHz 0: 4us 1: 15us 2: 30us (default) 3: 45us 4: 60us 5: 75us 6: 90us 7: 101us */
#define OTPC_EF_PPSR(x)           ((u32)(((x) & 0x00000007) << 9))
#define OTPC_GET_EF_PPSR(x)       ((u32)(((x >> 9) & 0x00000007)))
#define OTPC_MASK_EF_TPWI         ((u32)0x00000007 << 6)          /*!<R/W 3'b010  Program Pulse Interval Time (1us<ef_tpwi<5us) For input clock 20 MHz/26 MHz 0: 0.5us 1: 1.5us 2: 2us (default) 3: 2.5us 4: 3us 5: 3.5us 6: 4us 7: 5.5us */
#define OTPC_EF_TPWI(x)           ((u32)(((x) & 0x00000007) << 6))
#define OTPC_GET_EF_TPWI(x)       ((u32)(((x >> 6) & 0x00000007)))
#define OTPC_MASK_EF_TPW          ((u32)0x00000007 << 3)          /*!<R/W 3'b010  Program pulse width time.OTP Program Pulse width Time (20us<ef_tpw<25us) For input clock 20 MHz/26 MHz 0: 19.5us 1: 23us 2: 23.5us (default) 3: 24us 4: 24.5us 5: 25.5us */
#define OTPC_EF_TPW(x)            ((u32)(((x) & 0x00000007) << 3))
#define OTPC_GET_EF_TPW(x)        ((u32)(((x >> 3) & 0x00000007)))
#define OTPC_MASK_EF_CSP          ((u32)0x00000007 << 0)          /*!<R/W 3'b010  IP enable time in program (10us<ef_csp<100us) For input clock 20 MHz/26 MHz 0: 5us 1: 15us 2: 30us (default) 3: 45us 4: 60us 5: 75us 6: 90us 7: 101us */
#define OTPC_EF_CSP(x)            ((u32)(((x) & 0x00000007) << 0))
#define OTPC_GET_EF_CSP(x)        ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup OTPC_OTP_ALD_EN
 * @brief
 * @{
 **/
#define OTPC_BIT_USER_ERR_FLAG    ((u32)0x00000001 << 18)          /*!<R 0  When the autoload format in the user zone is incorrectly written, the signal will be raised. */
#define OTPC_BIT_LOAD_LZON_ERR    ((u32)0x00000001 << 17)          /*!<R 0  When the actual autoload data in the logical region is larger than the planned physical interval, this error signal will be high */
#define OTPC_BIT_LOAD_EN          ((u32)0x00000001 << 16)          /*!<R/WA0 0  Soft load enable, write one auto clear */
#define OTPC_MASK_LOAD_LZON       ((u32)0x000000FF << 8)          /*!<R/W 0  Logic load zone select (currently only supports software to trigger autoload in rom area) 0bit: 11'h0 - 11'h7 system data 1bit: 11'h8 - 11'h1F AON hareware data 2bit: 11'h140 - 11'h1AF LS hareware data 3bit: 11'h300 - 11'h32F AON pmc patch 4bit: 11'h340 - 11'h36F LS pmc patch 5bit: 11'h380 - 11'h3FF AON register patch 6bit: 11'h400 - 11'h47F LS register patch 7bit: It's invalid to write 1 or 0. */
#define OTPC_LOAD_LZON(x)         ((u32)(((x) & 0x000000FF) << 8))
#define OTPC_GET_LOAD_LZON(x)     ((u32)(((x >> 8) & 0x000000FF)))
#define OTPC_MASK_LOAD_PZON       ((u32)0x000000FF << 0)          /*!<R/W 0  physical load zone select */
#define OTPC_LOAD_PZON(x)         ((u32)(((x) & 0x000000FF) << 0))
#define OTPC_GET_LOAD_PZON(x)     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup OTPC_OTP_DEBUG
 * @brief
 * @{
 **/
#define OTPC_BIT_EF_DEBUG_EN      ((u32)0x00000001 << 16)          /*!<R/W 0  Debug enable */
#define OTPC_MASK_EF_DEBUG_STATE  ((u32)0x0000FFFF << 0)          /*!<R 0  State machine operating state [8:5]: autoload state. [4:3]: rd/wr ctrl state. [2:0]: wav state.Working status of different modes. */
#define OTPC_EF_DEBUG_STATE(x)    ((u32)(((x) & 0x0000FFFF) << 0))
#define OTPC_GET_EF_DEBUG_STATE(x) ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup OTPC_OTP_CRC_DATA
 * @brief
 * @{
 **/
#define OTPC_MASK_CRC_DATA_ACT    ((u32)0x0000FFFF << 16)          /*!<R 0  Actual crc data by hw calurate . Note : even CRC disabled by crc entry , this value will also get by hw. */
#define OTPC_CRC_DATA_ACT(x)      ((u32)(((x) & 0x0000FFFF) << 16))
#define OTPC_GET_CRC_DATA_ACT(x)  ((u32)(((x >> 16) & 0x0000FFFF)))
#define OTPC_MASK_CRC_DATA_EXP    ((u32)0x0000FFFF << 0)          /*!<R 0  Expect crc data from physical zone */
#define OTPC_CRC_DATA_EXP(x)      ((u32)(((x) & 0x0000FFFF) << 0))
#define OTPC_GET_CRC_DATA_EXP(x)  ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup OTPC_OTP_CRC_RESULT
 * @brief
 * @{
 **/
#define OTPC_BIT_CRC_FAIL         ((u32)0x00000001 << 31)          /*!<R 0  CRC compare fail flag . If CRC is disable by CRC_ENTRY , this bit will be 0 */
#define OTPC_BIT_CRC_TRIGGER_EN   ((u32)0x00000001 << 30)          /*!<R/W 0  The software enables calculation of the CRC in the secure zone and returns the calculated value(200-36f) */
#define OTPC_MASK_CRC_ENTRY_EN    ((u32)0x0000000F << 16)          /*!<R 0  one-hot bit for CRC_entry enable flag . If this value is 0 , it means CRC is disabled */
#define OTPC_CRC_ENTRY_EN(x)      ((u32)(((x) & 0x0000000F) << 16))
#define OTPC_GET_CRC_ENTRY_EN(x)  ((u32)(((x >> 16) & 0x0000000F)))
#define OTPC_MASK_CRC_MAGIC       ((u32)0x0000FFFF << 0)          /*!<R 0  magic data read from valid CRC entry */
#define OTPC_CRC_MAGIC(x)         ((u32)(((x) & 0x0000FFFF) << 0))
#define OTPC_GET_CRC_MAGIC(x)     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup OTPC_OTP_REPAIR_TIME
 * @brief
 * @{
 **/
#define OTPC_MASK_EF_REP_WCNT     ((u32)0x00000FFF << 20)          /*!<R/W 12'd1823  Configure the maximum address in Repair Program Mode.The default values are usually used. Please do not change them arbitrarily */
#define OTPC_EF_REP_WCNT(x)       ((u32)(((x) & 0x00000FFF) << 20))
#define OTPC_GET_EF_REP_WCNT(x)   ((u32)(((x >> 20) & 0x00000FFF)))
#define OTPC_MASK_EF_REP_RCNT     ((u32)0x00000FFF << 0)          /*!<R/W 12'd512  Configure the maximum address to read in both read modes of REPAIR (Repair Margin Read Mode, Repair Check Mode ) 1. for Repair Margin Read Mode, ef_rep_rcnt is 12'd513 2.for Repair Check Mode, er _rep_rcnt is 12'd0 */
#define OTPC_EF_REP_RCNT(x)       ((u32)(((x) & 0x00000FFF) << 0))
#define OTPC_GET_EF_REP_RCNT(x)   ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup OTPC_Exported_Types OTPC Exported Types
  * @{
*****************************************************************************/


/**
 * @brief OTPC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t OTPC_OTP_ALD;                           /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t OTPC_OTP_SCAN;                          /*!<  Register,  Address offset: 0x004 */
	__IO uint32_t OTPC_OTP_AS;                            /*!<  Register,  Address offset: 0x008 */
	__IO uint32_t OTPC_OTP_AS_SEC;                        /*!<  Register,  Address offset: 0x00C */
	__IO uint32_t OTPC_OTP_TEST_MODE;                     /*!<  Register,  Address offset: 0x0010 */
	__IO uint32_t OTPC_OTP_CTRL;                          /*!<  Register,  Address offset: 0x014 */
	__IO uint32_t OTPC_OTP_TEST;                          /*!<  Register,  Address offset: 0x018 */
	__IO uint32_t OTPC_OTP_TIME;                          /*!<  Register,  Address offset: 0x01C */
	__IO uint32_t OTPC_OTP_ALD_EN;                        /*!<  Register,  Address offset: 0x020 */
	__IO uint32_t OTPC_OTP_DEBUG;                         /*!<  Register,  Address offset: 0x024 */
	__IO uint32_t RSVD0[2];                               /*!<  Reserved,  Address offset:0x28-0x2C */
	__I  uint32_t OTPC_OTP_CRC_DATA;                      /*!<  Register,  Address offset: 0x030 */
	__IO uint32_t OTPC_OTP_CRC_RESULT;                    /*!<  Register,  Address offset: 0x034 */
	__IO uint32_t OTPC_OTP_REPAIR_TIME;                   /*!<  Register,  Address offset: 0x038 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x3C */
	__IO uint32_t OTPC_OTP_PARAM;                         /*!<  Register,  Address offset: 0x040 */
} OTPC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

#define OTPC_PHY_ROM_PATCH_ZONE					((u32)0x00000001 << 2)
#define ROM_PATCH_LENGTH							0x200


//Please add your defination here
enum OTP_OPMode {
	OTP_USER_MODE = 0,
	OTP_PGR_MODE = 2,
};


/** @defgroup EFUSE_LOGICAL_definitions
  * @{
  */
#define OTP_LMAP_LEN								0x400 /*!< logical map len in byte 0x400~0x7FF resvd*/
#define OTP_MAX_SECTION							(OTP_LMAP_LEN >> 3) /*!< logical map len in section */
#define PGPKT_DATA_SIZE							8 /*!< logical map section len */
#define OTP_MTP_USER_START						0x2D0 /*!< logical map MTP user start address*/
#define OTP_MTP_USER_END							0x2FF /*!< logical map MTP user end address*/
#define OTP_MTP_USER_LEN							(OTP_MTP_USER_END - OTP_MTP_USER_START+1) /*!< logical map MTP user section len in byte 0x2D0~0x2FF*/
/**
  * @}
  */

/** @defgroup EFUSE_PHYSICAL_definitions
  * @{
  */

/*  physical EFUSE len */
#define OTP_REAL_CONTENT_LEN						0x800
#define AVAILABLE_EFUSE_ADDR(addr)					(addr < OTP_REAL_CONTENT_LEN)
/* physical EFUSE write forbid */
#define LOGICAL_MAP_SECTION_LEN					0x1FD /*!< logical mapping efuse len in physical address */
#define OTP_USER_START								0x380 /*!< physical map user start address*/
#define OTP_USER_END								0x500 /*!< physical map user end address*/
#define OTP_USER_LEN								(OTP_USER_END - OTP_USER_START+1) /*!< physical map user section len in byte 0x380~0x4FF*/

/**
  * @}
  */

#define OTP_POLL_TIMES								20000
#define OTP_ACCESS_PWD								0x69
/*meaningless for AmebaD2*/
#define L25EOUTVOLTAGE								7


/** @defgroup old EFUSE_definitions
  * @{
  */
#define EFUSE_MAP_LEN_8711B						OTP_LMAP_LEN
#define EFUSE_MAX_SECTION_8711B					OTP_MAX_SECTION
#define EFUSE_REAL_CONTENT_LEN					OTP_REAL_CONTENT_LEN
#define EFUSE_OOB_PROTECT_BYTES					(EFUSE_REAL_CONTENT_LEN - LOGICAL_MAP_SECTION_LEN)
/**
  * @}
  */

/** @defgroup EFUSE_LOGICAL_definitions
  * @{
  */
#define OTP_LTYP0									0x00UL
#define OTP_LTYP1									0x01UL
#define OTP_LTYP2									0x02UL
#define OTP_LTYP3									0x03UL
#define OTP_GET_LTYP(x)								((u32)(((x >> 28) & 0x0000000F)))

#define OTP_LBASE_EFUSE							0x07UL
#define OTP_LBASE_PAD								0x06UL
#define OTP_LBASE_REG								0x0FUL
#define OTP_LBASE_ROM								0x0EUL

#define OTP_GET_LTYP1_DATA(x)						((u32)(((x >> 16) & 0x000000FF)))
#define OTP_GET_LTYP1_BASE(x)						((u32)(((x >> 12) & 0x0000000F)))
#define OTP_GET_LTYP1_OFFSET(x)					((u32)(((x >> 0) & 0x00000FFF)))

#define OTP_GET_LTYP2_LEN(x)						((u32)((((x >> 24) & 0x0000000F) + 1) << 2))
#define OTP_SET_LTYP2_LEN(x)						((u32)( ((x & 0x03F) >> 2) - 1))
#define OTP_GET_LTYP2_BASE(x)						((u32)(((x >> 12) & 0x0000000F)))
#define OTP_GET_LTYP2_OFFSET(x)					((u32)(((x >> 0) & 0x00000FFF)))

#define OTP_GET_LTYP3_OFFSET(x)					((u32)(((x >> 0) & 0x0FFFFFFF)))

#define OTP_REGPATCH_BASE							0x4100C000
#define OTP_REGPATCH_END							(OTP_REGPATCH_BASE + 0x5FF)
#define OTP_IS_REGPATCH(x)							((x >= OTP_REGPATCH_BASE) && (x < OTP_REGPATCH_END))

#define OTP_PADPATCH_BASE							0x4100C800
#define OTP_PADPATCH_END							(OTP_PADPATCH_BASE+0x1FF)
#define OTP_IS_PADPATCH(x)							((x >= OTP_PADPATCH_BASE) && (x < OTP_PADPATCH_END))

#define OTP_SUPPERPATCH_BASE						0x40000000
#define OTP_SUPPERPATCH_END						0x50000000
#define OTP_IS_SUPPERPATCH(x)						((x >= OTP_SUPPERPATCH_BASE) && (x < OTP_SUPPERPATCH_END))

#define OTP_RTKPATCH_BASE							0x500
#define OTP_RTKPATCH_END							0x6FC

#define OTP_LPGPKT_SIZE 							16
/**
  * @}
  */


/** @defgroup EFUSE_Physical_Address_functions
  * @{
  */
extern u32 EFUSERead8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 *Data, UNUSED_WARN_DIS u8 L25OutVoltage);
extern u32 EFUSEWrite8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 Data, UNUSED_WARN_DIS u8 L25OutVoltage);
extern u32 EFUSE_PMAP_READ8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 *Data, UNUSED_WARN_DIS u8 L25OutVoltage);
extern u32 EFUSE_PMAP_WRITE8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 Data, UNUSED_WARN_DIS u8 L25OutVoltage);
extern u32 EFUSE_LMAP_READ(u8 *pbuf);
extern u32 EFUSE_LMAP_WRITE(u32 addr, u32 cnts, u8 *data);
extern u32 EFUSE_RemainLength(void);
extern u32 EFUSE_GetCRC(void);
extern u32 OTP_LogicalMap_Read(u8 *pbuf, u32 addr, u32 len);
extern u32 OTP_Read8(u32 Addr, u8 *Data);
extern u32 OTP_Write8(u32 Addr, u8 Data);
extern u32 OTP_LogicalMap_Write(u32 addr, u32 cnts, u8 *data);

/**
  * @}
  */
/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
