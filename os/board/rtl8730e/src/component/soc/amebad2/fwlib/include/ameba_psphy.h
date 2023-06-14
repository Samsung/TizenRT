#ifndef _AMEBAD2_PSPHY_H_
#define _AMEBAD2_PSPHY_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup PSPHY
  * @brief PSPHY driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup PSPHY_Register_Definitions PSPHY Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup PSPHY_CAL_CTRL
 * @brief
 * @{
 **/
#define PSPHY_BIT_CFG_CAL_NBDR_OVF             ((u32)0x00000001 << 16)          /*!<RW1C 0  The N reachs the boundary , which is 0 or Nmax . Cleared by write 1 . */
#define PSPHY_BIT_CFG_CAL_NBDR_ALMOST          ((u32)0x00000001 << 15)          /*!<RW1C 0  The N-J or N+J reachs the boundary , which is 0 or Nmax . Cleared by write 1 . */
#define PSPHY_BIT_CFG_CAL_ERR_UNSURE           ((u32)0x00000001 << 14)          /*!<RW1C 0  The Pre N/J may cause current burst receive error , but it is not sure because the burst len is too small. Cleared by write 1 . */
#define PSPHY_BIT_CFG_CAL_ERR_ALREADY          ((u32)0x00000001 << 13)          /*!<RW1C 0  The Pre N/J may cause current burst receive error , even change J to Jmin , the error still remain . Cleared by write 1 . */
#define PSPHY_BIT_CFG_CAL_ERR_ALMOST           ((u32)0x00000001 << 12)          /*!<RW1C 0  The current burst receive correct , but next burst will set J = Jmin . Cleared by write 1 . */
#define PSPHY_MASK_CFG_CAL_INTR_MASK           ((u32)0x0000001F << 4)          /*!<R/W 5'b00011  Calibration fail interrupt mask , every bit control one event above (1: enable , 0: disable) [0] : err almost [1] : err already [2] : err unsure [3] : nbdr almost [4] : nbdr ovf */
#define PSPHY_CFG_CAL_INTR_MASK(x)             ((u32)(((x) & 0x0000001F) << 4))
#define PSPHY_GET_CFG_CAL_INTR_MASK(x)         ((u32)(((x >> 4) & 0x0000001F)))
#define PSPHY_BIT_CFG_CAL_EN                   ((u32)0x00000001 << 0)          /*!<R/W 1  Hardware auto calibration enable , if disable , N,J will be fixed . */
/** @} */

/** @defgroup PSPHY_CAL_PAR
 * @brief
 * @{
 **/
#define PSPHY_MASK_PRE_CAL_PHASE               ((u32)0x0000000F << 28)          /*!<R/W 4'h1  one-hot phase select, 4-tap delay line to adjust rwds phase before calibration for large freq range or skew can be set to 0, bypassing the default 1-tap delay */
#define PSPHY_PRE_CAL_PHASE(x)                 ((u32)(((x) & 0x0000000F) << 28))
#define PSPHY_GET_PRE_CAL_PHASE(x)             ((u32)(((x >> 28) & 0x0000000F)))
#define PSPHY_MASK_CFG_CAL_JMIN                ((u32)0x00000007 << 24)          /*!<R/W 3'd2  Minimum J . Hardware may adjust J , if J reach this value , an interrup will be raised to alerm software. */
#define PSPHY_CFG_CAL_JMIN(x)                  ((u32)(((x) & 0x00000007) << 24))
#define PSPHY_GET_CFG_CAL_JMIN(x)              ((u32)(((x >> 24) & 0x00000007)))
#define PSPHY_MASK_CFG_CAL_JMAX                ((u32)0x0000000F << 16)          /*!<R/W 4'd14  Maximum J . Hardware may adjust J , but will not exceed this value. */
#define PSPHY_CFG_CAL_JMAX(x)                  ((u32)(((x) & 0x0000000F) << 16))
#define PSPHY_GET_CFG_CAL_JMAX(x)              ((u32)(((x >> 16) & 0x0000000F)))
#define PSPHY_MASK_CFG_CAL_J                   ((u32)0x0000000F << 8)          /*!<R/WPD 4'd10  Initial value J calibrated by software . */
#define PSPHY_CFG_CAL_J(x)                     ((u32)(((x) & 0x0000000F) << 8))
#define PSPHY_GET_CFG_CAL_J(x)                 ((u32)(((x >> 8) & 0x0000000F)))
#define PSPHY_MASK_CFG_CAL_N                   ((u32)0x0000001F << 0)          /*!<R/WPD 5'd15  Initial value N calibrated by software . */
#define PSPHY_CFG_CAL_N(x)                     ((u32)(((x) & 0x0000001F) << 0))
#define PSPHY_GET_CFG_CAL_N(x)                 ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup PSPHY_CAL_STATUS
 * @brief
 * @{
 **/
#define PSPHY_MASK_CFG_CAL_CUR_ST              ((u32)0x0000001F << 24)          /*!<RW1C 0  Calibration current state . Cleared by write all 1 . */
#define PSPHY_CFG_CAL_CUR_ST(x)                ((u32)(((x) & 0x0000001F) << 24))
#define PSPHY_GET_CFG_CAL_CUR_ST(x)            ((u32)(((x >> 24) & 0x0000001F)))
#define PSPHY_MASK_CFG_CAL_PDST                ((u32)0x00000007 << 16)          /*!<RW1C 0  Calibration check result for last read burst . [0] : indicate phase n+j , if 1 , means dn == dn+j [1] : indicate phase n-j , if 1 , means dn == dn-j [2] : if 1 , means the state is under j = jmin condition */
#define PSPHY_CFG_CAL_PDST(x)                  ((u32)(((x) & 0x00000007) << 16))
#define PSPHY_GET_CFG_CAL_PDST(x)              ((u32)(((x >> 16) & 0x00000007)))
#define PSPHY_MASK_CFG_CAL_CUR_J               ((u32)0x0000000F << 8)          /*!<R 0  Current value J read from hardware. */
#define PSPHY_CFG_CAL_CUR_J(x)                 ((u32)(((x) & 0x0000000F) << 8))
#define PSPHY_GET_CFG_CAL_CUR_J(x)             ((u32)(((x >> 8) & 0x0000000F)))
#define PSPHY_MASK_CFG_CAL_CUR_N               ((u32)0x0000001F << 0)          /*!<R 0  Current value N read from hardware. */
#define PSPHY_CFG_CAL_CUR_N(x)                 ((u32)(((x) & 0x0000001F) << 0))
#define PSPHY_GET_CFG_CAL_CUR_N(x)             ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup PSPHY_CMD_INFO
 * @brief
 * @{
 **/
#define PSPHY_MASK_CMD_INFO                    ((u32)0x000000FF << 0)          /*!<R 8'h0  Command information, 1st Byte of Command/Address transaction */
#define PSPHY_CMD_INFO(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define PSPHY_GET_CMD_INFO(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PSPHY_PHY_CTRL
 * @brief
 * @{
 **/
#define PSPHY_BIT_PSRAM_SW_RST_N               ((u32)0x00000001 << 31)          /*!<R/W 1'b1  low active, except for phy register(using system reset), psram software reset: psram, phy_ctrl, calibration */
#define PSPHY_MASK_CMD_END_DLY                 ((u32)0x00000007 << 9)          /*!<R/W 3'h5  delay required cycles(phy clk) after cmd end to bypass high-z DQS, must < (read_latency - 1)*2 */
#define PSPHY_CMD_END_DLY(x)                   ((u32)(((x) & 0x00000007) << 9))
#define PSPHY_GET_CMD_END_DLY(x)               ((u32)(((x >> 9) & 0x00000007)))
#define PSPHY_BIT_CMD_RW_POL                   ((u32)0x00000001 << 8)          /*!<R/W 1'b0  polarity option for read/write bit in command, 0 for APM, 1 for winbond */
#define PSPHY_MASK_CAL_RWDS_PHASE              ((u32)0x0000000F << 4)          /*!<R/W 4'h1  one-hot phase select, delay cal_rwds to push sampling data into rfifo */
#define PSPHY_CAL_RWDS_PHASE(x)                ((u32)(((x) & 0x0000000F) << 4))
#define PSPHY_GET_CAL_RWDS_PHASE(x)            ((u32)(((x >> 4) & 0x0000000F)))
#define PSPHY_MASK_RFIFO_RDY_DLY               ((u32)0x0000000F << 0)          /*!<R/W 4'h6  delay read data cycles(phy clk) from psram to controller, must >= 6 */
#define PSPHY_RFIFO_RDY_DLY(x)                 ((u32)(((x) & 0x0000000F) << 0))
#define PSPHY_GET_RFIFO_RDY_DLY(x)             ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup PSPHY_DDR_PAD_CTRL0
 * @brief
 * @{
 **/
#define PSPHY_MASK_DQ_CTRL_PSRAM               ((u32)0x07FFFFFF << 0)          /*!<R/W 0x9a6   */
#define PSPHY_DQ_CTRL_PSRAM(x)                 ((u32)(((x) & 0x07FFFFFF) << 0))
#define PSPHY_GET_DQ_CTRL_PSRAM(x)             ((u32)(((x >> 0) & 0x07FFFFFF)))
/** @} */

/** @defgroup PSPHY_DDR_PAD_CTRL1
 * @brief
 * @{
 **/
#define PSPHY_MASK_DQ_TTCP_PSRAM               ((u32)0x0000003F << 8)          /*!<R/W 0   */
#define PSPHY_DQ_TTCP_PSRAM(x)                 ((u32)(((x) & 0x0000003F) << 8))
#define PSPHY_GET_DQ_TTCP_PSRAM(x)             ((u32)(((x >> 8) & 0x0000003F)))
#define PSPHY_MASK_DQ_TTCN_PSRAM               ((u32)0x0000003F << 0)          /*!<R/W 0   */
#define PSPHY_DQ_TTCN_PSRAM(x)                 ((u32)(((x) & 0x0000003F) << 0))
#define PSPHY_GET_DQ_TTCN_PSRAM(x)             ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup PSPHY_DDR_PAD_CTRL2
 * @brief
 * @{
 **/
#define PSPHY_MASK_DQS_TTCN_PSRAM              ((u32)0x0000003F << 24)          /*!<R/W 0   */
#define PSPHY_DQS_TTCN_PSRAM(x)                ((u32)(((x) & 0x0000003F) << 24))
#define PSPHY_GET_DQS_TTCN_PSRAM(x)            ((u32)(((x >> 24) & 0x0000003F)))
#define PSPHY_MASK_DQS_TTCNB_PSRAM             ((u32)0x0000003F << 16)          /*!<R/W 0   */
#define PSPHY_DQS_TTCNB_PSRAM(x)               ((u32)(((x) & 0x0000003F) << 16))
#define PSPHY_GET_DQS_TTCNB_PSRAM(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define PSPHY_MASK_DQS_TTCP_PSRAM              ((u32)0x0000003F << 8)          /*!<R/W 0   */
#define PSPHY_DQS_TTCP_PSRAM(x)                ((u32)(((x) & 0x0000003F) << 8))
#define PSPHY_GET_DQS_TTCP_PSRAM(x)            ((u32)(((x >> 8) & 0x0000003F)))
#define PSPHY_MASK_DQS_TTCPB_PSRAM             ((u32)0x0000003F << 0)          /*!<R/W 0   */
#define PSPHY_DQS_TTCPB_PSRAM(x)               ((u32)(((x) & 0x0000003F) << 0))
#define PSPHY_GET_DQS_TTCPB_PSRAM(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup PSPHY_DDR_PAD_CTRL3
 * @brief
 * @{
 **/
#define PSPHY_MASK_DQS_N_CTRL_PSRAM            ((u32)0x00000FFF << 0)          /*!<R/W 0x9a6   */
#define PSPHY_DQS_N_CTRL_PSRAM(x)              ((u32)(((x) & 0x00000FFF) << 0))
#define PSPHY_GET_DQS_N_CTRL_PSRAM(x)          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup PSPHY_DDR_PAD_CTRL4
 * @brief
 * @{
 **/
#define PSPHY_MASK_DQS_P_CTRL_PSRAM            ((u32)0x0FFFFFFF << 0)          /*!<R/W 0x9a6   */
#define PSPHY_DQS_P_CTRL_PSRAM(x)              ((u32)(((x) & 0x0FFFFFFF) << 0))
#define PSPHY_GET_DQS_P_CTRL_PSRAM(x)          ((u32)(((x >> 0) & 0x0FFFFFFF)))
/** @} */

/** @defgroup PSPHY_DDR_PAD_CTRL5
 * @brief
 * @{
 **/
#define PSPHY_MASK_VREF_S_0_PSRAM              ((u32)0x0000003F << 16)          /*!<R/W 0x2c   */
#define PSPHY_VREF_S_0_PSRAM(x)                ((u32)(((x) & 0x0000003F) << 16))
#define PSPHY_GET_VREF_S_0_PSRAM(x)            ((u32)(((x >> 16) & 0x0000003F)))
#define PSPHY_MASK_RD_PD_EN_DQ_PSRAM           ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define PSPHY_RD_PD_EN_DQ_PSRAM(x)             ((u32)(((x) & 0x000000FF) << 8))
#define PSPHY_GET_RD_PD_EN_DQ_PSRAM(x)         ((u32)(((x >> 8) & 0x000000FF)))
#define PSPHY_BIT_REG_VREF_RANGE_SET_0_PSRAM   ((u32)0x00000001 << 6)          /*!<R/W 0x1   */
#define PSPHY_BIT_REG_VREF_PD_SET_0_PSRAM      ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define PSPHY_BIT_REG_DDR2_MODE_PSRAM          ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define PSPHY_BIT_RD_PD_EN_DM_PSRAM            ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define PSPHY_BIT_RD_PD_EN_DQS_PSRAM           ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define PSPHY_BIT_RD_ODT_EN_DQS_PSRAM          ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/** @defgroup PSPHY_DBG_SEL
 * @brief
 * @{
 **/
#define PSPHY_MASK_PSRAM_PHY_DBG_SEL           ((u32)0x000000FF << 0)          /*!<R/W 8'h0  psram phy debug select signal. */
#define PSPHY_PSRAM_PHY_DBG_SEL(x)             ((u32)(((x) & 0x000000FF) << 0))
#define PSPHY_GET_PSRAM_PHY_DBG_SEL(x)         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PSPHY_DBG_INFO
 * @brief
 * @{
 **/
#define PSPHY_MASK_PSRAM_PHY_DBG               ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0  psram phy debug signals. */
#define PSPHY_PSRAM_PHY_DBG(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PSPHY_GET_PSRAM_PHY_DBG(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup PSPHY_Exported_Types PSPHY Exported Types
  * @{
*****************************************************************************/


/**
 * @brief PSPHY Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t PSPHY_CAL_CTRL;                         /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t PSPHY_CAL_PAR;                          /*!<  Register,  Address offset: 0x004 */
	__IO uint32_t PSPHY_CAL_STATUS;                       /*!<  Register,  Address offset: 0x008 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xC */
	__I  uint32_t PSPHY_CMD_INFO;                         /*!<  Register,  Address offset: 0x10 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x14 */
	__IO uint32_t PSPHY_PHY_CTRL;                         /*!<  Register,  Address offset: 0x18 */
	__IO uint32_t RSVD2[25];                              /*!<  Reserved,  Address offset:0x1C-0x7C */
	__IO uint32_t PSPHY_DDR_PAD_CTRL0;                    /*!<  Register,  Address offset: 0x80 */
	__IO uint32_t PSPHY_DDR_PAD_CTRL1;                    /*!<  Register,  Address offset: 0x84 */
	__IO uint32_t PSPHY_DDR_PAD_CTRL2;                    /*!<  Register,  Address offset: 0x88 */
	__IO uint32_t PSPHY_DDR_PAD_CTRL3;                    /*!<  Register,  Address offset: 0x8C */
	__IO uint32_t PSPHY_DDR_PAD_CTRL4;                    /*!<  Register,  Address offset: 0x90 */
	__IO uint32_t PSPHY_DDR_PAD_CTRL5;                    /*!<  Register,  Address offset: 0x94 */
	__IO uint32_t RSVD3[22];                              /*!<  Reserved,  Address offset:0x98-0xEC */
	__IO uint32_t PSPHY_DBG_SEL;                          /*!<  Register,  Address offset: 0xF0 */
	__IO uint32_t RSVD4;                                  /*!<  Reserved,  Address offset:0xF4 */
	__I  uint32_t PSPHY_DBG_INFO;                         /*!<  Register,  Address offset: 0xF8 */
} PSPHY_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/**
  * @brief  PSRAMPhy Init structure definition
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
	u32 PSRAMP_CAL_RWDS_PHASE; 		/*!< Specifies the byte size of an address in read/write command in auto_mode. */
	u32 PSRAMP_RFIFO_RDY_DLY;		/* variable latency: delay read data cycles(phy clk) after detecting first DQS rising edge, must < 4
										fixed latency: delay read data cycles(phy clk) after asserting spi_rx_data_en, recommend 4 ~ 6
										4*read_latency - rd_dummy_length + rfifo_rdy_dly = in_physical_cyc */
} PSPHY_InitTypeDef;

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PSRAM_Exported_Functions PSRAM Exported Functions
  * @{
  */

_LONG_CALL_ void PSRAM_PHY_StructInit(PSPHY_InitTypeDef *PSPHY_InitStruct);
_LONG_CALL_ void PSRAM_PHY_Init(PSPHY_InitTypeDef *PSPHY_InitStruct);

/**
  * @}
  */

#define PSPHY_RFIFO_READY_DELAY		0x7 // latency * 4 + rfifo_rdy_dly = rd_dummy_length + in_physical_cyc

#define PSPHY_CMD_END_DELAY				0x7 //tCQLZ 1~6ns,  recommand by dd
/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
