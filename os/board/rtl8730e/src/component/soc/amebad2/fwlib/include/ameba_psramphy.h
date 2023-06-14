#ifndef _AMEBAD2_PSPHY_H_
#define _AMEBAD2_PSPHY_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup PSPHY_CAL_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_CFG_CAL_NBDR_OVF                 16
#define PSPHY_BIT_CFG_CAL_NBDR_OVF                   ((u32)0x00000001 << PSPHY_SHIFT_CFG_CAL_NBDR_OVF)                 /*!<RW1C 0  The N reachs the boundary , which is 0 or Nmax . Cleared by write 1 . */
#define PSPHY_SHIFT_CFG_CAL_NBDR_ALMOST              15
#define PSPHY_BIT_CFG_CAL_NBDR_ALMOST                ((u32)0x00000001 << PSPHY_SHIFT_CFG_CAL_NBDR_ALMOST)              /*!<RW1C 0  The N-J or N+J reachs the boundary , which is 0 or Nmax . Cleared by write 1 . */
#define PSPHY_SHIFT_CFG_CAL_ERR_UNSURE               14
#define PSPHY_BIT_CFG_CAL_ERR_UNSURE                 ((u32)0x00000001 << PSPHY_SHIFT_CFG_CAL_ERR_UNSURE)               /*!<RW1C 0  The Pre N/J may cause current burst receive error , but it is not sure because the burst len is too small. Cleared by write 1 . */
#define PSPHY_SHIFT_CFG_CAL_ERR_ALREADY              13
#define PSPHY_BIT_CFG_CAL_ERR_ALREADY                ((u32)0x00000001 << PSPHY_SHIFT_CFG_CAL_ERR_ALREADY)              /*!<RW1C 0  The Pre N/J may cause current burst receive error , even change J to Jmin , the error still remain . Cleared by write 1 . */
#define PSPHY_SHIFT_CFG_CAL_ERR_ALMOST               12
#define PSPHY_BIT_CFG_CAL_ERR_ALMOST                 ((u32)0x00000001 << PSPHY_SHIFT_CFG_CAL_ERR_ALMOST)               /*!<RW1C 0  The current burst receive correct , but next burst will set J = Jmin . Cleared by write 1 . */
#define PSPHY_SHIFT_CFG_CAL_INTR_MASK                4
#define PSPHY_MASK_CFG_CAL_INTR_MASK                 ((u32)0x0000001F << PSPHY_SHIFT_CFG_CAL_INTR_MASK)                /*!<R/W 5'b00011  Calibration fail interrupt mask , every bit control one event above (1: enable , 0: disable) [0] : err almost [1] : err already [2] : err unsure [3] : nbdr almost [4] : nbdr ovf */
#define PSPHY_SHIFT_CFG_CAL_EN                       0
#define PSPHY_BIT_CFG_CAL_EN                         ((u32)0x00000001 << PSPHY_SHIFT_CFG_CAL_EN)                       /*!<R/W 1  Hardware auto calibration enable , if disable , N,J will be fixed . */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_CAL_PAR
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_CFG_CAL_JMIN                     24
#define PSPHY_MASK_CFG_CAL_JMIN                      ((u32)0x00000007 << PSPHY_SHIFT_CFG_CAL_JMIN)                     /*!<R/W 3'd2  Minimum J . Hardware may adjust J , if J reach this value , an interrup will be raised to alerm software. */
#define PSPHY_SHIFT_CFG_CAL_JMAX                     16
#define PSPHY_MASK_CFG_CAL_JMAX                      ((u32)0x0000000F << PSPHY_SHIFT_CFG_CAL_JMAX)                     /*!<R/W 4'd14  Maximum J . Hardware may adjust J , but will not exceed this value. */
#define PSPHY_SHIFT_CFG_CAL_J                        8
#define PSPHY_MASK_CFG_CAL_J                         ((u32)0x0000000F << PSPHY_SHIFT_CFG_CAL_J)                        /*!<R/WP 4'd10  Initial value J calibrated by software . */
#define PSPHY_SHIFT_CFG_CAL_N                        0
#define PSPHY_MASK_CFG_CAL_N                         ((u32)0x0000001F << PSPHY_SHIFT_CFG_CAL_N)                        /*!<R/WP 5'd15  Initial value N calibrated by software . */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_CAL_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_CFG_CAL_CUR_ST                   24
#define PSPHY_MASK_CFG_CAL_CUR_ST                    ((u32)0x0000001F << PSPHY_SHIFT_CFG_CAL_CUR_ST)                   /*!<RW1C 0  Calibration current state . Cleared by write all 1 . */
#define PSPHY_SHIFT_CFG_CAL_PDST                     16
#define PSPHY_MASK_CFG_CAL_PDST                      ((u32)0x00000007 << PSPHY_SHIFT_CFG_CAL_PDST)                     /*!<R 0  Calibration check result for last read burst . [0] : indicate phase n+j , if 1 , means dn == dn+j [1] : indicate phase n-j , if 1 , means dn == dn-j [2] : if 1 , means the state is under j = jmin condition */
#define PSPHY_SHIFT_CFG_CAL_CUR_J                    8
#define PSPHY_MASK_CFG_CAL_CUR_J                     ((u32)0x0000000F << PSPHY_SHIFT_CFG_CAL_CUR_J)                    /*!<R 0  Current value J read from hardware. */
#define PSPHY_SHIFT_CFG_CAL_CUR_N                    0
#define PSPHY_MASK_CFG_CAL_CUR_N                     ((u32)0x0000001F << PSPHY_SHIFT_CFG_CAL_CUR_N)                    /*!<R 0  Current value N read from hardware. */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_CMD_INFO
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_CMD_INFO                         0
#define PSPHY_MASK_CMD_INFO                          ((u32)0x000000FF << PSPHY_SHIFT_CMD_INFO)                         /*!<R 8'h0  Command information, 1st Byte of Command/Address transaction */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_PHY_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_PSRAM_SW_RST_N                   31
#define PSPHY_BIT_PSRAM_SW_RST_N                     ((u32)0x00000001 << PSPHY_SHIFT_PSRAM_SW_RST_N)                   /*!<R/W 1'b1  low active, except for phy register(using system reset), psram software reset: psram, phy_ctrl, calibration */
#define PSPHY_SHIFT_CMD_RW_POL                       8
#define PSPHY_BIT_CMD_RW_POL                         ((u32)0x00000001 << PSPHY_SHIFT_CMD_RW_POL)                       /*!<R/W 1'b0  polarity option for read/write bit in command, 0 for APM, 1 for winbond */
#define PSPHY_SHIFT_READ_LATENCY                     4
#define PSPHY_MASK_READ_LATENCY                      ((u32)0x00000007 << PSPHY_SHIFT_READ_LATENCY)                     /*!<R/W 3'h5  psram read latency, keep the same as controller's setting */
#define PSPHY_SHIFT_RFIFO_RDY_DLY                    0
#define PSPHY_MASK_RFIFO_RDY_DLY                     ((u32)0x0000000F << PSPHY_SHIFT_RFIFO_RDY_DLY)                    /*!<R/W 4'h4  delay read data cycles(phy clk) from psram to controller */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DDR_PAD_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_DQ_CTRL_PSRAM                    0
#define PSPHY_MASK_DQ_CTRL_PSRAM                     ((u32)0x07FFFFFF << PSPHY_SHIFT_DQ_CTRL_PSRAM)                    /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DDR_PAD_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_DQ_TTCP_PSRAM                    8
#define PSPHY_MASK_DQ_TTCP_PSRAM                     ((u32)0x0000003F << PSPHY_SHIFT_DQ_TTCP_PSRAM)                    /*!<R/W 0   */
#define PSPHY_SHIFT_DQ_TTCN_PSRAM                    0
#define PSPHY_MASK_DQ_TTCN_PSRAM                     ((u32)0x0000003F << PSPHY_SHIFT_DQ_TTCN_PSRAM)                    /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DDR_PAD_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_DQS_TTCN_PSRAM                   24
#define PSPHY_MASK_DQS_TTCN_PSRAM                    ((u32)0x0000003F << PSPHY_SHIFT_DQS_TTCN_PSRAM)                   /*!<R/W 0   */
#define PSPHY_SHIFT_DQS_TTCNB_PSRAM                  16
#define PSPHY_MASK_DQS_TTCNB_PSRAM                   ((u32)0x0000003F << PSPHY_SHIFT_DQS_TTCNB_PSRAM)                  /*!<R/W 0   */
#define PSPHY_SHIFT_DQS_TTCP_PSRAM                   8
#define PSPHY_MASK_DQS_TTCP_PSRAM                    ((u32)0x0000003F << PSPHY_SHIFT_DQS_TTCP_PSRAM)                   /*!<R/W 0   */
#define PSPHY_SHIFT_DQS_TTCPB_PSRAM                  0
#define PSPHY_MASK_DQS_TTCPB_PSRAM                   ((u32)0x0000003F << PSPHY_SHIFT_DQS_TTCPB_PSRAM)                  /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DDR_PAD_CTRL3
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_DQS_N_CTRL_PSRAM                 0
#define PSPHY_MASK_DQS_N_CTRL_PSRAM                  ((u32)0x00000FFF << PSPHY_SHIFT_DQS_N_CTRL_PSRAM)                 /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DDR_PAD_CTRL4
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_DQS_P_CTRL_PSRAM                 0
#define PSPHY_MASK_DQS_P_CTRL_PSRAM                  ((u32)0x0FFFFFFF << PSPHY_SHIFT_DQS_P_CTRL_PSRAM)                 /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DDR_PAD_CTRL5
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_VREF_S_0_PSRAM                   16
#define PSPHY_MASK_VREF_S_0_PSRAM                    ((u32)0x0000003F << PSPHY_SHIFT_VREF_S_0_PSRAM)                   /*!<R/W 0   */
#define PSPHY_SHIFT_RD_PD_EN_DQ_PSRAM                8
#define PSPHY_MASK_RD_PD_EN_DQ_PSRAM                 ((u32)0x000000FF << PSPHY_SHIFT_RD_PD_EN_DQ_PSRAM)                /*!<R/W 0   */
#define PSPHY_SHIFT_REG_VREF_RANGE_SET_0_PSRAM       6
#define PSPHY_BIT_REG_VREF_RANGE_SET_0_PSRAM         ((u32)0x00000001 << PSPHY_SHIFT_REG_VREF_RANGE_SET_0_PSRAM)       /*!<R/W 0   */
#define PSPHY_SHIFT_REG_VREF_PD_SET_0_PSRAM          5
#define PSPHY_BIT_REG_VREF_PD_SET_0_PSRAM            ((u32)0x00000001 << PSPHY_SHIFT_REG_VREF_PD_SET_0_PSRAM)          /*!<R/W 0   */
#define PSPHY_SHIFT_REG_DDR2_MODE_PSRAM              4
#define PSPHY_BIT_REG_DDR2_MODE_PSRAM                ((u32)0x00000001 << PSPHY_SHIFT_REG_DDR2_MODE_PSRAM)              /*!<R/W 0   */
#define PSPHY_SHIFT_RD_PD_EN_DM_PSRAM                2
#define PSPHY_BIT_RD_PD_EN_DM_PSRAM                  ((u32)0x00000001 << PSPHY_SHIFT_RD_PD_EN_DM_PSRAM)                /*!<R/W 0   */
#define PSPHY_SHIFT_RD_PD_EN_DQS_PSRAM               1
#define PSPHY_BIT_RD_PD_EN_DQS_PSRAM                 ((u32)0x00000001 << PSPHY_SHIFT_RD_PD_EN_DQS_PSRAM)               /*!<R/W 0   */
#define PSPHY_SHIFT_RD_ODT_EN_DQS_PSRAM              0
#define PSPHY_BIT_RD_ODT_EN_DQS_PSRAM                ((u32)0x00000001 << PSPHY_SHIFT_RD_ODT_EN_DQS_PSRAM)              /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DBG_SEL
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_PSRAM_PHY_DBG_SEL                0
#define PSPHY_MASK_PSRAM_PHY_DBG_SEL                 ((u32)0x000000FF << PSPHY_SHIFT_PSRAM_PHY_DBG_SEL)                /*!<R/W 8'h0  psram phy debug select signal. */
/** @} */

/**************************************************************************//**
 * @defgroup PSPHY_DBG_INFO
 * @brief
 * @{
 *****************************************************************************/
#define PSPHY_SHIFT_PSRAM_PHY_DBG                    0
#define PSPHY_MASK_PSRAM_PHY_DBG                     ((u32)0xFFFFFFFF << PSPHY_SHIFT_PSRAM_PHY_DBG)                    /*!<R 32'h0  psram phy debug signals. */
/** @} */



/**************************************************************************//**
 * @defgroup PSPHY
 * @{
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

/* MANUAL_GEN_END */

#endif
