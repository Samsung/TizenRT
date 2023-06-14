#ifndef _AMEBAD2_DDRPHY_H_
#define _AMEBAD2_DDRPHY_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup DDRPHY
  * @brief DDRPHY driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup DDRPHY_Register_Definitions DDRPHY Register Definitions
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @defgroup DDRPHY_CRT_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_MCK_CKE_SEL                 ((u32)0x00000001 << 30)          /*!<R/W ’b0  1’b0 : mck_cke = mck_ca 1’b1 : mck_cke = mck_cs */
#define DDRPHY_MCK_CKE_SEL(x)                  ((u32)(((x) & 0x00000001) << 30))
#define DDRPHY_BIT_CLK_TEN_VALUE               ((u32)0x00000001 << 29)          /*!<R/W ‘b0  Value for ten mode of clk */
#define DDRPHY_CLK_TEN_VALUE(x)                ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_CLK_TEN_MODE                ((u32)0x00000001 << 28)          /*!<R/W ‘b0  1 for ten mode */
#define DDRPHY_CLK_TEN_MODE(x)                 ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_FORCE_RSTN                  ((u32)0x00000001 << 27)          /*!<R/W ‘b1  1 for forcing PAD_DDR_RSTN value to rstn_value */
#define DDRPHY_FORCE_RSTN(x)                   ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_BIT_RSTN_VALUE                  ((u32)0x00000001 << 26)          /*!<R/W ‘b1  Value of PAD_DDR_RSTN when force_rstn = 1 */
#define DDRPHY_RSTN_VALUE(x)                   ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_MASK_FIFO_RST_INI_TX            ((u32)0x00000007 << 23)          /*!<R/W ‘h0  Initial value of Tx CTS FIFO read point */
#define DDRPHY_FIFO_RST_INI_TX(x)              ((u32)(((x) & 0x00000007) << 23))
#define DDRPHY_GET_FIFO_RST_INI_TX(x)          ((u32)(((x >> 23) & 0x00000007)))
#define DDRPHY_MASK_FIFO_RST_INI_RX            ((u32)0x00000007 << 20)          /*!<R/W ‘h0  Initial value of Rx CTS FIFO read point */
#define DDRPHY_FIFO_RST_INI_RX(x)              ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_FIFO_RST_INI_RX(x)          ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_BIT_CA_DDR3_MODE                ((u32)0x00000001 << 19)          /*!<R/W ‘h1  Connect to command/address pad’s DDR3_MODE */
#define DDRPHY_CA_DDR3_MODE(x)                 ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_FORCE_CKE_1                 ((u32)0x00000001 << 18)          /*!<R/W ‘b1  1 for forcing PAD_DDR_CKE_1 value to cke_1_value */
#define DDRPHY_FORCE_CKE_1(x)                  ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_BIT_CKE_1_VALUE                 ((u32)0x00000001 << 17)          /*!<R/W ‘b0  Value of PAD_DDR_CKE_1 when force_cke_1 = 1 */
#define DDRPHY_CKE_1_VALUE(x)                  ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_FORCE_CS                    ((u32)0x00000001 << 16)          /*!<R/W ‘b0  1 for forcing PAD_DDR_CS_N value to cs_value */
#define DDRPHY_FORCE_CS(x)                     ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_BIT_CS_VALUE                    ((u32)0x00000001 << 15)          /*!<R/W ‘b1  Value of PAD_DDR_CS_N when force_cs = 1 */
#define DDRPHY_CS_VALUE(x)                     ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_BIT_FORCE_CS_1                  ((u32)0x00000001 << 14)          /*!<R/W ‘b0  1 for forcing PAD_DDR_CS_1_N value to cs_1_value */
#define DDRPHY_FORCE_CS_1(x)                   ((u32)(((x) & 0x00000001) << 14))
#define DDRPHY_BIT_CS_1_VALUE                  ((u32)0x00000001 << 13)          /*!<R/W ‘b1  Value of PAD_DDR_CS_1_N when force_cs_1 = 1 */
#define DDRPHY_CS_1_VALUE(x)                   ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_PAD_DDR2_MODE               ((u32)0x00000001 << 12)          /*!<R/W ‘h1  Connect to P_PAD_DDR_VREF 0 : DDR3_MODE 1 : DDR2 MODE */
#define DDRPHY_PAD_DDR2_MODE(x)                ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_FORCE_CKE                   ((u32)0x00000001 << 11)          /*!<R/W ‘b1  1 for forcing PAD_DDR_CKE value to cke_value */
#define DDRPHY_FORCE_CKE(x)                    ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_BIT_CKE_VALUE                   ((u32)0x00000001 << 10)          /*!<R/W ‘b0  Value of PAD_DDR_CKE when force_cke = 1 */
#define DDRPHY_CKE_VALUE(x)                    ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_PIN_MUX_LPDDR3_2CELL        ((u32)0x00000001 << 9)          /*!<R/W ’b0  DDR3 : b0_0001 DDR2 : b0_1000 */
#define DDRPHY_PIN_MUX_LPDDR3_2CELL(x)         ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_PIN_MUX_LPDDR3_1CELL        ((u32)0x00000001 << 8)          /*!<R/W ’b0  DDR3 : b0_0001 DDR2 : b0_1000 */
#define DDRPHY_PIN_MUX_LPDDR3_1CELL(x)         ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_PIN_MUX_LPDDR2_2CELL        ((u32)0x00000001 << 7)          /*!<R/W ’b0  DDR3 : b0_0001 DDR2 : b0_1000 */
#define DDRPHY_PIN_MUX_LPDDR2_2CELL(x)         ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_BIT_PIN_MUX_LPDDR2_1CELL        ((u32)0x00000001 << 6)          /*!<R/W ’b0  DDR3 : b0_0001 DDR2 : b0_1000 */
#define DDRPHY_PIN_MUX_LPDDR2_1CELL(x)         ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_PIN_MUX_DDR3                ((u32)0x00000001 << 5)          /*!<R/W ’b1  DDR3 : b0_0001 DDR2 : b0_1000 */
#define DDRPHY_PIN_MUX_DDR3(x)                 ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_PIN_MUX_MODE                ((u32)0x00000001 << 4)          /*!<R/W ’b0  1’b1 : pin_mux bypass mode 1’b0 : pin_mux mode */
#define DDRPHY_PIN_MUX_MODE(x)                 ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_BIT_DPI_LPDDR_EN                ((u32)0x00000001 << 3)          /*!<R/W ’b0  1’b1 : LPDDR mode enable */
#define DDRPHY_DPI_LPDDR_EN(x)                 ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_DPI_DAT_RATE_SEL            ((u32)0x00000001 << 2)          /*!<R/W ’b0  1’b0 : 2to1 mode mck_cmd = mck_mc = 1/2 DCK 1’b1 : 1to1 mode mck_cmd = mck_mc = DCK */
#define DDRPHY_DPI_DAT_RATE_SEL(x)             ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_DPI_DDR2_EN                 ((u32)0x00000001 << 1)          /*!<R/W ’b0  ddr2_en=1 & ddr3_mod=1 DDR2(LPDDR2) mode ddr2_en=0 & ddr3_mod=1 DDR3(LPDDR3) mode ddr2_en=0 & ddr3_mod=0 DDR4 mode */
#define DDRPHY_DPI_DDR2_EN(x)                  ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_DPI_DDR3_MODE               ((u32)0x00000001 << 0)          /*!<R/W ’b1  ddr2_en=1 & ddr3_mod=1 DDR2(LPDDR2) mode ddr2_en=0 & ddr3_mod=1 DDR3(LPDDR3) mode ddr2_en=0 & ddr3_mod=0 DDR4 mode */
#define DDRPHY_DPI_DDR3_MODE(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_CTL0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_AFA_DCG_EN                  ((u32)0x00000001 << 29)          /*!<R/W ‘h0  Rx Alpha eye calibration clock gating enable */
#define DDRPHY_AFA_DCG_EN(x)                   ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_TMUX_DCG_EN                 ((u32)0x00000001 << 28)          /*!<R/W ’h0  Bist test mux clock gating enable */
#define DDRPHY_TMUX_DCG_EN(x)                  ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_CA_AFIFO_DCG_EN             ((u32)0x00000001 << 23)          /*!<R/W ’h0  CA AFIFO clock gating enable */
#define DDRPHY_CA_AFIFO_DCG_EN(x)              ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_MASK_CA_AFIFO_DCG_CNT           ((u32)0x00000007 << 20)          /*!<R/W ’h2  CA AFIFO clock gating count = {ca_afifo_dcg_cnt, 2’b11} */
#define DDRPHY_CA_AFIFO_DCG_CNT(x)             ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_CA_AFIFO_DCG_CNT(x)         ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_DPI_MCK_CLK_EN             ((u32)0x00001FFF << 0)          /*!<R/W ’h000  DDR_PHY internal clock ICG enable ck_en[12:0] :clock enable ck_en[0]: mck_ck ck_en[1]: mck_cmd ck_en[2]: mck_dqs_0 ck_en[3]: mck_dqs_1 ck_en[4]: mck_dqs_2 (No use) ck_en[5]: mck_dqs_3 (No use) ck_en[6]: mck_dq_0 ck_en[7]: mck_dq_1 ck_en[8]: mck_dq_2 (No use) ck_en[9]: mck_dq_3 (No use) ck_en[10]: mck_cs ck_en[11]: mck_cs_1 (No use) ck_en[12]: mck_ck_1 (No use) */
#define DDRPHY_DPI_MCK_CLK_EN(x)               ((u32)(((x) & 0x00001FFF) << 0))
#define DDRPHY_GET_DPI_MCK_CLK_EN(x)           ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_CTL1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_MCK_MC_EN                   ((u32)0x00000001 << 31)          /*!<R/W ‘h0  0: gated mck_mc to low */
#define DDRPHY_MCK_MC_EN(x)                    ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_DPI_REG_FIX_DIV1            ((u32)0x00000001 << 30)          /*!<R/W ‘h0  PLL sync CLK_OE clk is fix div1(1) or div2 (0 default) */
#define DDRPHY_DPI_REG_FIX_DIV1(x)             ((u32)(((x) & 0x00000001) << 30))
#define DDRPHY_BIT_DPI_REG_CRT_PWROFF_DIS      ((u32)0x00000001 << 29)          /*!<R/W ‘h1  clk_oe to PLL is from crt table or FSM, default is from crt table。 */
#define DDRPHY_DPI_REG_CRT_PWROFF_DIS(x)       ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_MASK_DPI_CLK_OE                 ((u32)0x00000007 << 16)          /*!<R/W ’h00  clk_oe[2:0]: PLL clock output enable clk_oe[0]:mck_ck clk_oe[1]: mck_cmd , mck_dqs_0, mck_dqs_1, mck_dqs_2, mck_dqs_3, mck_dq_0, mck_dq_1, mck_dq_2, mck_dq_3 clk_oe[2]: mck_ck_1 */
#define DDRPHY_DPI_CLK_OE(x)                   ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_DPI_CLK_OE(x)               ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_BIT_DPI_OESYNC_OP_SEL_FIX       ((u32)0x00000001 << 13)          /*!<R/W ‘b0  free run fix clock op_sel */
#define DDRPHY_DPI_OESYNC_OP_SEL_FIX(x)        ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_MASK_DPI_OESYNC_OP_SEL          ((u32)0x00001FFF << 0)          /*!<R/W ’h000  control signal for output clock enable sync op_sel[0]:mck_ck op_sel[1]:mck_cmd op_sel[2]:mck_dqs_0 op_sel[3]:mck_dqs_1 op_sel[4]:mck_dqs_2 op_sel[5]:mck_dqs_3 op_sel[6]:mck_dq_0 op_sel[7]:mck_dq_1 op_sel[8]:mck_dq_2 op_sel[9]:mck_dq_3 op_sel[10]:mck_cs op_sel[11]:mck_cs_1 op_sel[12]:mck_ck_1 */
#define DDRPHY_DPI_OESYNC_OP_SEL(x)            ((u32)(((x) & 0x00001FFF) << 0))
#define DDRPHY_GET_DPI_OESYNC_OP_SEL(x)        ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_CTL2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_REG_CTS_FIFO_SYNC_CNT  ((u32)0x00000007 << 29)          /*!<R/W ‘h2  sync cts fifo tx/rx pointer,wait cycles when pweroff_rst_n sync from tx to rx */
#define DDRPHY_DPI_REG_CTS_FIFO_SYNC_CNT(x)    ((u32)(((x) & 0x00000007) << 29))
#define DDRPHY_GET_DPI_REG_CTS_FIFO_SYNC_CNT(x) ((u32)(((x >> 29) & 0x00000007)))
#define DDRPHY_BIT_DPI_PRBS_EN                 ((u32)0x00000001 << 28)          /*!<R/W ’b0  PLL Noise Generator Enable */
#define DDRPHY_DPI_PRBS_EN(x)                  ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_DPI_NC_SEL                 ((u32)0x00000003 << 26)          /*!<R/W ’h0  PLL Noise Generator noise current control */
#define DDRPHY_DPI_NC_SEL(x)                   ((u32)(((x) & 0x00000003) << 26))
#define DDRPHY_GET_DPI_NC_SEL(x)               ((u32)(((x >> 26) & 0x00000003)))
#define DDRPHY_MASK_DPI_NC_CLK_DIV_SEL         ((u32)0x00000003 << 24)          /*!<R/W ’h0  Noise Generator clock select */
#define DDRPHY_DPI_NC_CLK_DIV_SEL(x)           ((u32)(((x) & 0x00000003) << 24))
#define DDRPHY_GET_DPI_NC_CLK_DIV_SEL(x)       ((u32)(((x >> 24) & 0x00000003)))
#define DDRPHY_MASK_DPI_NC_PRBS13_SEED         ((u32)0x00001FFF << 11)          /*!<R/W ’h0  Noise Generator PRBS SEED */
#define DDRPHY_DPI_NC_PRBS13_SEED(x)           ((u32)(((x) & 0x00001FFF) << 11))
#define DDRPHY_GET_DPI_NC_PRBS13_SEED(x)       ((u32)(((x >> 11) & 0x00001FFF)))
#define DDRPHY_BIT_DPI_NC_PRBS13_EN            ((u32)0x00000001 << 10)          /*!<R/W ’h0  Noise Generator enable */
#define DDRPHY_DPI_NC_PRBS13_EN(x)             ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_DPI_NC_PRBS13_INVS          ((u32)0x00000001 << 9)          /*!<R/W ’h0  Noise Generator PRBS INVS */
#define DDRPHY_DPI_NC_PRBS13_INVS(x)           ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_DPI_NC_PRBS13_MAX           ((u32)0x00000001 << 8)          /*!<R/W ’h0  Noise Generator PRBS MAX */
#define DDRPHY_DPI_NC_PRBS13_MAX(x)            ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_MASK_DPI_PLL_DUM                ((u32)0x000000FF << 0)          /*!<R/W ’hF  dummy Register */
#define DDRPHY_DPI_PLL_DUM(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define DDRPHY_GET_DPI_PLL_DUM(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_PI0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_POST_PI_SEL3           ((u32)0x0000001F << 24)          /*!<R/W ’h0  PLL phase select for mck_dqs_1 */
#define DDRPHY_DPI_POST_PI_SEL3(x)             ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_DPI_POST_PI_SEL3(x)         ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL2           ((u32)0x0000001F << 16)          /*!<R/W ’h0  PLL phase select for mck_dqs_0 */
#define DDRPHY_DPI_POST_PI_SEL2(x)             ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_POST_PI_SEL2(x)         ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL1           ((u32)0x0000001F << 8)          /*!<R/W ’h0  PLL phase select for mck_cmd */
#define DDRPHY_DPI_POST_PI_SEL1(x)             ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_DPI_POST_PI_SEL1(x)         ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL0           ((u32)0x0000001F << 0)          /*!<R/W ’h0  PLL phase select for mck_ck */
#define DDRPHY_DPI_POST_PI_SEL0(x)             ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_DPI_POST_PI_SEL0(x)         ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_PI1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_POST_PI_SEL            ((u32)0x0000001F << 24)          /*!<R/W ’h0  PLL internal clock phase select */
#define DDRPHY_DPI_POST_PI_SEL(x)              ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_DPI_POST_PI_SEL(x)          ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL6           ((u32)0x0000001F << 16)          /*!<R/W ’h0  PLL phase select for mck_dq_0 */
#define DDRPHY_DPI_POST_PI_SEL6(x)             ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_POST_PI_SEL6(x)         ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL5           ((u32)0x0000001F << 8)          /*!<R/W ’h0  PLL phase select for mck_dqs_3 */
#define DDRPHY_DPI_POST_PI_SEL5(x)             ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_DPI_POST_PI_SEL5(x)         ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL4           ((u32)0x0000001F << 0)          /*!<R/W ’h0  PLL phase select for mck_dqs_2 */
#define DDRPHY_DPI_POST_PI_SEL4(x)             ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_DPI_POST_PI_SEL4(x)         ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_CTL3
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_V11_LDO_VSEL           ((u32)0x00000007 << 29)          /*!<R/W ’h0  Select PLL 1.0V LDO output 000: tracking digital core power 001: 0.85V; 010: 0.9V; 011: 1.0V; 100: 1.05V; 101: 1.1V 110: 1.15V 111: 1.2V */
#define DDRPHY_DPI_V11_LDO_VSEL(x)             ((u32)(((x) & 0x00000007) << 29))
#define DDRPHY_GET_DPI_V11_LDO_VSEL(x)         ((u32)(((x >> 29) & 0x00000007)))
#define DDRPHY_BIT_DPI_CKREF_SEL               ((u32)0x00000001 << 28)          /*!<R/W ’b0  Ref Clock select 1’b0: use CKREF_PAD 1’b1: use CKREF_TOP */
#define DDRPHY_DPI_CKREF_SEL(x)                ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_DPI_POST_PI_RS              ((u32)0x00000001 << 27)          /*!<R/W ’b0  Post PI loading cap control: 0: 112.5fF; 1: 15fF */
#define DDRPHY_DPI_POST_PI_RS(x)               ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_MASK_DPI_POST_PI_RL             ((u32)0x00000003 << 25)          /*!<R/W ’h2  Post PI RL control, changed with PLL freq: 00:0.2G~0.5G; 01:0.5G~0.8G; 10:0.8G~1.25G */
#define DDRPHY_DPI_POST_PI_RL(x)               ((u32)(((x) & 0x00000003) << 25))
#define DDRPHY_GET_DPI_POST_PI_RL(x)           ((u32)(((x >> 25) & 0x00000003)))
#define DDRPHY_MASK_DPI_POST_PI_BIAS           ((u32)0x00000003 << 23)          /*!<R/W ’h2  Post PI bias current, changed with PLL freq: 00:0.2G~0.5G; 01:0.5G~0.8G; 10:0.8G~1.25G */
#define DDRPHY_DPI_POST_PI_BIAS(x)             ((u32)(((x) & 0x00000003) << 23))
#define DDRPHY_GET_DPI_POST_PI_BIAS(x)         ((u32)(((x >> 23) & 0x00000003)))
#define DDRPHY_BIT_DPI_PLL_SEL_CPMODE          ((u32)0x00000001 << 22)          /*!<R/W ’b1  PLL charge pump mode selector, always set 1 */
#define DDRPHY_DPI_PLL_SEL_CPMODE(x)           ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_MASK_DPI_PLL_LDO_VSEL           ((u32)0x00000007 << 19)          /*!<R/W ’h5  Select PLL 1.0V LDO output; 000: tracking core power; 001: 0.85V; 010: 0.9V; 011: 1.0V; 100: 1.05V; 101: 1.1V 110: 1.15V 111: 1.2V */
#define DDRPHY_DPI_PLL_LDO_VSEL(x)             ((u32)(((x) & 0x00000007) << 19))
#define DDRPHY_GET_DPI_PLL_LDO_VSEL(x)         ((u32)(((x >> 19) & 0x00000007)))
#define DDRPHY_BIT_DPI_PLL_DBUG_EN             ((u32)0x00000001 << 18)          /*!<R/W ’b0  Enable PLL debug mode */
#define DDRPHY_DPI_PLL_DBUG_EN(x)              ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_MASK_DPI_PDIV                   ((u32)0x00000003 << 16)          /*!<R/W ’h0  PLL pre-divider divisor register 00: /1; 01: /2; 10: /4; 11: /8 */
#define DDRPHY_DPI_PDIV(x)                     ((u32)(((x) & 0x00000003) << 16))
#define DDRPHY_GET_DPI_PDIV(x)                 ((u32)(((x >> 16) & 0x00000003)))
#define DDRPHY_MASK_DPI_LPF_SR                 ((u32)0x00000007 << 13)          /*!<R/W ’h3  PLL LPF resistor selector Register for LPF RS, combined used with REG_ICP and REG_LPF_CP to tune PLL bandwidth */
#define DDRPHY_DPI_LPF_SR(x)                   ((u32)(((x) & 0x00000007) << 13))
#define DDRPHY_GET_DPI_LPF_SR(x)               ((u32)(((x >> 13) & 0x00000007)))
#define DDRPHY_BIT_DPI_LPF_CP                  ((u32)0x00000001 << 12)          /*!<R/W ’b1  Register for LPF CP, combined used with REG_ICP and REG_LPF_SR to tune PLL bandwidth */
#define DDRPHY_DPI_LPF_CP(x)                   ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_MASK_DPI_LOOP_PI_ISEL           ((u32)0x0000000F << 8)          /*!<R/W ’h6  Select loop pi current, changed with PLL freq: 0010:0.2G~0.4G; 0011:0.4G~0.6G; 0100:0.6G~0.8G; 0101:0.8G~1G; 0110:1G~1.25G */
#define DDRPHY_DPI_LOOP_PI_ISEL(x)             ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DPI_LOOP_PI_ISEL(x)         ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_DPI_ICP                    ((u32)0x0000000F << 4)          /*!<R/W ’hC  CP bias current control, combined used with REG_LPF_CP and REG_LPF_SR to tune PLL bandwidth */
#define DDRPHY_DPI_ICP(x)                      ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DPI_ICP(x)                  ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_BIT_DPI_CCO_KVCO                ((u32)0x00000001 << 2)          /*!<R/W ’b1  PLL kvco control, combined used with REG_CCO_BAND to tune PLL freq */
#define DDRPHY_DPI_CCO_KVCO(x)                 ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_MASK_DPI_CCO_BAND               ((u32)0x00000003 << 0)          /*!<R/W ’h1  PLL band select, combined used with REG_CCO_KVCO to tune PLL freq */
#define DDRPHY_DPI_CCO_BAND(x)                 ((u32)(((x) & 0x00000003) << 0))
#define DDRPHY_GET_DPI_CCO_BAND(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_SSC0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_OC_DONE_DELAY          ((u32)0x0000003F << 8)          /*!<R/W ’h3F  Delay to setting OC_DONE=1’b1 after over-clock or under-clock is done by (REG_OC_DONE_delay*Tref) */
#define DDRPHY_DPI_OC_DONE_DELAY(x)            ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_DPI_OC_DONE_DELAY(x)        ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_DPI_HS_OC_STOP_DIFF        ((u32)0x00000003 << 6)          /*!<R/W ’h1  HS stop control 2’b00: HS OC stop at (Target_freq-2*fref) 2’b01: HS OC stop at (Target_freq-6*fref) 2’b10: HS OC stop at (Target_freq-10*fref) 2’b11: HS OC stop at (Target_freq-14*fref) */
#define DDRPHY_DPI_HS_OC_STOP_DIFF(x)          ((u32)(((x) & 0x00000003) << 6))
#define DDRPHY_GET_DPI_HS_OC_STOP_DIFF(x)      ((u32)(((x >> 6) & 0x00000003)))
#define DDRPHY_MASK_DPI_SEL_OC_MODE            ((u32)0x00000003 << 4)          /*!<R/W ’h0  Over-clock / Under-clock mode selection 2’b00: Low speed mode 2’b01: High speed mode 2’b10: Adaptive high speed mode */
#define DDRPHY_DPI_SEL_OC_MODE(x)              ((u32)(((x) & 0x00000003) << 4))
#define DDRPHY_GET_DPI_SEL_OC_MODE(x)          ((u32)(((x >> 4) & 0x00000003)))
#define DDRPHY_BIT_DPI_SSC_FLAG_INIT           ((u32)0x00000001 << 3)          /*!<R/W ’b0  SSC initial flag */
#define DDRPHY_DPI_SSC_FLAG_INIT(x)            ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_DPI_ORDER                   ((u32)0x00000001 << 2)          /*!<R/W ’b0  Select 1st-order or 2nd-order SDM */
#define DDRPHY_DPI_ORDER(x)                    ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_DPI_EN_SSC                  ((u32)0x00000001 << 1)          /*!<R/W ’b0  Enable SSC function “1” : Enable “0” : Disable */
#define DDRPHY_DPI_EN_SSC(x)                   ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_DPI_BYPASS_PI               ((u32)0x00000001 << 0)          /*!<R/W ’b0  When no fractional divider or SSC, bypass Phase Interpolator for CMU loop 0: including PI inside the loop 1: bypass PI inside the loop */
#define DDRPHY_DPI_BYPASS_PI(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_SSC1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DPI_GRAN_AUTO_RST           ((u32)0x00000001 << 23)          /*!<R/W ’b1  When FMOD transition, reset SDM */
#define DDRPHY_DPI_GRAN_AUTO_RST(x)            ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_MASK_DPI_DOT_GRAN               ((u32)0x00000007 << 20)          /*!<R/W ’h4  Decimal point position of GRAN 3’b000: 4.15 3’b001: 5.14 3’b010: 6.13 3’b011: 7.12 3’b100: 8.11 */
#define DDRPHY_DPI_DOT_GRAN(x)                 ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_DPI_DOT_GRAN(x)             ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_DPI_GRAN_SET               ((u32)0x0007FFFF << 0)          /*!<R/W ’h021FB  SSC step SSC step= ((Ncode_T*2048+Fcode_T –Ncode_ssc*2048- Fcode_ssc)*33K*2^11) / ((Frequency of CKREF) * 0.5) */
#define DDRPHY_DPI_GRAN_SET(x)                 ((u32)(((x) & 0x0007FFFF) << 0))
#define DDRPHY_GET_DPI_GRAN_SET(x)             ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_SSC2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_F_CODE_T               ((u32)0x000007FF << 16)          /*!<R/W ’h50A  Fractional code of loop divider (target Fcode of overclocking) */
#define DDRPHY_DPI_F_CODE_T(x)                 ((u32)(((x) & 0x000007FF) << 16))
#define DDRPHY_GET_DPI_F_CODE_T(x)             ((u32)(((x >> 16) & 0x000007FF)))
#define DDRPHY_MASK_DPI_F_CODE                 ((u32)0x000007FF << 0)          /*!<R/W ’h5ED  Fractional code of loop divider */
#define DDRPHY_DPI_F_CODE(x)                   ((u32)(((x) & 0x000007FF) << 0))
#define DDRPHY_GET_DPI_F_CODE(x)               ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_SSC3
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DPI_OC_EN                   ((u32)0x00000001 << 26)          /*!<R/W ’b0  Enable over-clock or under-clock */
#define DDRPHY_DPI_OC_EN(x)                    ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_MASK_DPI_OC_STEP_SET            ((u32)0x000003FF << 16)          /*!<R/W ’h28  Over-clock / Under-clock slope control of low speed mode */
#define DDRPHY_DPI_OC_STEP_SET(x)              ((u32)(((x) & 0x000003FF) << 16))
#define DDRPHY_GET_DPI_OC_STEP_SET(x)          ((u32)(((x >> 16) & 0x000003FF)))
#define DDRPHY_MASK_DPI_N_CODE_T               ((u32)0x000000FF << 8)          /*!<R/W ’h1A  Integer code of loop divider,real divider code is (target Ncode of overclocking) CKREF=27M, REG_PDIV=2’b00, REG_BYPASS_PI=1’b0, VCO frequency=27*( REG_N_CODE [7:0] +3+ REG_F_CODE<10:0>/2048)=1.6GHz -> REG_N_CODE [7:0] =56, REG_F_CODE<10:0>=531 */
#define DDRPHY_DPI_N_CODE_T(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define DDRPHY_GET_DPI_N_CODE_T(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define DDRPHY_MASK_DPI_N_CODE                 ((u32)0x000000FF << 0)          /*!<R/W ’h19  Integer code of loop divider,real divider code is CKREF=27M, REG_PDIV=2’b00, REG_BYPASS_PI=1’b0, VCO frequency=27*( REG_N_CODE [7:0] +3+ REG_F_CODE<10:0>/2048)=1.6GHz -> REG_N_CODE [7:0] =56, REG_F_CODE<10:0>=531 */
#define DDRPHY_DPI_N_CODE(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define DDRPHY_GET_DPI_N_CODE(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_WDOG
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_TIME2_RST_WIDTH        ((u32)0x00000003 << 12)          /*!<R/W ’h1  Reset timing control (REG_TIME_RST_WIDTH x CKREF) 2’d0: 2 2’d1: 4 2’d2: 8 2’d3: 20 */
#define DDRPHY_DPI_TIME2_RST_WIDTH(x)          ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_DPI_TIME2_RST_WIDTH(x)      ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_DPI_TIME_RDY_CKOUT         ((u32)0x00000003 << 8)          /*!<R/W ’h2  Clock ready timing control ( clock ready time : 2 times the value of REG_TIME_WD x CKREF + REG_TIME_RDY_CKOUT x CKTXPLL) 2’d0: 3 2’d1: 100 2’d2: 200 2’d3: 600 */
#define DDRPHY_DPI_TIME_RDY_CKOUT(x)           ((u32)(((x) & 0x00000003) << 8))
#define DDRPHY_GET_DPI_TIME_RDY_CKOUT(x)       ((u32)(((x >> 8) & 0x00000003)))
#define DDRPHY_MASK_DPI_TIME0_CK               ((u32)0x00000007 << 4)          /*!<R/W ’h3  Watchdog timing control (clock output time: 2 times the value of REG_TIME_WD x CKREF) 3’d0: 2 3’d1: 200 3’d2: 600 3’d3: 1200 3’d4: 3000 3’d5: 6400 3’d6: 12800 3’d7: 16000 */
#define DDRPHY_DPI_TIME0_CK(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DPI_TIME0_CK(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DPI_F390K                  ((u32)0x00000003 << 1)          /*!<R/W ’h1  F390K output delay, not used */
#define DDRPHY_DPI_F390K(x)                    ((u32)(((x) & 0x00000003) << 1))
#define DDRPHY_GET_DPI_F390K(x)                ((u32)(((x >> 1) & 0x00000003)))
#define DDRPHY_BIT_DPI_WD_ENABLE               ((u32)0x00000001 << 0)          /*!<R/W ’b0  Enable watch dog function 0: disable Watch Dog function 1: enable Watch Dog function */
#define DDRPHY_DPI_WD_ENABLE(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DCC_CMD
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DPI_DCC_CMD_DSP_DBG_SEL     ((u32)0x00000001 << 31)          /*!<R/W ’b0  mck_cmd duty control debug output enable */
#define DDRPHY_DPI_DCC_CMD_DSP_DBG_SEL(x)      ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_DPI_DCC_CMD_DUTY_SEL        ((u32)0x00000001 << 29)          /*!<R/W ’b0  mck_cmd duty control reg_dpi_dcc_cmd_duty_sel=0: output from close loop; reg_dpi_dcc_cmd_duty_sel=1: output from reg_dpi_dcc_cmd_duty[6:0] */
#define DDRPHY_DPI_DCC_CMD_DUTY_SEL(x)         ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_DPI_DCC_CMD_DSP_STOP        ((u32)0x00000001 << 28)          /*!<R/W ’b1  mck_cmd duty control dsp function enable */
#define DDRPHY_DPI_DCC_CMD_DSP_STOP(x)         ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_DPI_DCC_CMD_DIV            ((u32)0x00000007 << 24)          /*!<R/W ’h5  mck_cmd duty control dsp clock frequency setting: 000~111: /2^4~/2^11 */
#define DDRPHY_DPI_DCC_CMD_DIV(x)              ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DPI_DCC_CMD_DIV(x)          ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_CMD_INT_GAIN       ((u32)0x0000001F << 16)          /*!<R/W ’h05  mck_cmd duty control close loop gain setting: 00000~11111: /2^6~/2 */
#define DDRPHY_DPI_DCC_CMD_INT_GAIN(x)         ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_DCC_CMD_INT_GAIN(x)     ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_DCC_CMD_DUTY           ((u32)0x0000007F << 8)          /*!<R/W ’h38  mck_cmd duty control when reg_dpi_dcc_cmd_duty_sel=1'b0,output value from this register */
#define DDRPHY_DPI_DCC_CMD_DUTY(x)             ((u32)(((x) & 0x0000007F) << 8))
#define DDRPHY_GET_DPI_DCC_CMD_DUTY(x)         ((u32)(((x >> 8) & 0x0000007F)))
#define DDRPHY_MASK_DPI_DCC_CMD_DUTY_PRESETB   ((u32)0x00000007 << 4)          /*!<R/W ’h3  mck_cmd duty control DCD negitive duty preset setting */
#define DDRPHY_DPI_DCC_CMD_DUTY_PRESETB(x)     ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DPI_DCC_CMD_DUTY_PRESETB(x) ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_CMD_DUTY_PRESETA   ((u32)0x00000007 << 0)          /*!<R/W ’h3  mck_cmd duty control DCD positive duty preset setting */
#define DDRPHY_DPI_DCC_CMD_DUTY_PRESETA(x)     ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DPI_DCC_CMD_DUTY_PRESETA(x) ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DCC_DQS_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DPI_DCC_DQS_DUTY_SEL        ((u32)0x00000001 << 29)          /*!<R/W ’b0  mck_dqs duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQS_DUTY_SEL(x)         ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_DPI_DCC_DQS_DSP_STOP        ((u32)0x00000001 << 28)          /*!<R/W ’b1  mck_dqs duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQS_DSP_STOP(x)         ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_DPI_DCC_DQS_DIV            ((u32)0x00000007 << 24)          /*!<R/W ’h5  mck_dqs duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQS_DIV(x)              ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DPI_DCC_DQS_DIV(x)          ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_DQS_INT_GAIN       ((u32)0x0000001F << 16)          /*!<R/W ’h05  mck_dqs duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQS_INT_GAIN(x)         ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_DCC_DQS_INT_GAIN(x)     ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_DCC_DQS_DUTY           ((u32)0x0000007F << 8)          /*!<R/W ’h38  mck_dqs duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQS_DUTY(x)             ((u32)(((x) & 0x0000007F) << 8))
#define DDRPHY_GET_DPI_DCC_DQS_DUTY(x)         ((u32)(((x >> 8) & 0x0000007F)))
#define DDRPHY_MASK_DPI_DCC_DQS_DUTY_PRESETB   ((u32)0x00000007 << 4)          /*!<R/W ’h3  mck_dqs duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQS_DUTY_PRESETB(x)     ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DPI_DCC_DQS_DUTY_PRESETB(x) ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_DQS_DUTY_PRESETA   ((u32)0x00000007 << 0)          /*!<R/W ’h3  mck_dqs duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQS_DUTY_PRESETA(x)     ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DPI_DCC_DQS_DUTY_PRESETA(x) ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DCC_DQ_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DPI_DCC_DQ_DUTY_SEL         ((u32)0x00000001 << 29)          /*!<R/W ’b0  mck_dq duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQ_DUTY_SEL(x)          ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_DPI_DCC_DQ_DSP_STOP         ((u32)0x00000001 << 28)          /*!<R/W ’b1  mck_dq duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQ_DSP_STOP(x)          ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_DPI_DCC_DQ_DIV             ((u32)0x00000007 << 24)          /*!<R/W ’h5  mck_dq duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQ_DIV(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DPI_DCC_DQ_DIV(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_DQ_INT_GAIN        ((u32)0x0000001F << 16)          /*!<R/W ’h05  mck_dq duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQ_INT_GAIN(x)          ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_DCC_DQ_INT_GAIN(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_DCC_DQ_DUTY            ((u32)0x0000007F << 8)          /*!<R/W ’h38  mck_dq duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQ_DUTY(x)              ((u32)(((x) & 0x0000007F) << 8))
#define DDRPHY_GET_DPI_DCC_DQ_DUTY(x)          ((u32)(((x >> 8) & 0x0000007F)))
#define DDRPHY_MASK_DPI_DCC_DQ_DUTY_PRESETB    ((u32)0x00000007 << 4)          /*!<R/W ’h3  mck_dq duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQ_DUTY_PRESETB(x)      ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DPI_DCC_DQ_DUTY_PRESETB(x)  ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_DQ_DUTY_PRESETA    ((u32)0x00000007 << 0)          /*!<R/W ’h3  mck_dq duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_DQ_DUTY_PRESETA(x)      ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DPI_DCC_DQ_DUTY_PRESETA(x)  ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DCC_CK_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DPI_DCC_CK_DUTY_SEL         ((u32)0x00000001 << 29)          /*!<R/W ’b0  mck_ck duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_CK_DUTY_SEL(x)          ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_DPI_DCC_CK_DSP_STOP         ((u32)0x00000001 << 28)          /*!<R/W ’b1  mck_ck duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_CK_DSP_STOP(x)          ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_DPI_DCC_CK_DIV             ((u32)0x00000007 << 24)          /*!<R/W ’h5  mck_ck duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_CK_DIV(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DPI_DCC_CK_DIV(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_CK_INT_GAIN        ((u32)0x0000001F << 16)          /*!<R/W ’h05  mck_ck duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_CK_INT_GAIN(x)          ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_DCC_CK_INT_GAIN(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_DCC_CK_DUTY            ((u32)0x0000007F << 8)          /*!<R/W ’h38  mck_ck duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_CK_DUTY(x)              ((u32)(((x) & 0x0000007F) << 8))
#define DDRPHY_GET_DPI_DCC_CK_DUTY(x)          ((u32)(((x >> 8) & 0x0000007F)))
#define DDRPHY_MASK_DPI_DCC_CK_DUTY_PRESETB    ((u32)0x00000007 << 4)          /*!<R/W ’h3  mck_ck duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_CK_DUTY_PRESETB(x)      ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DPI_DCC_CK_DUTY_PRESETB(x)  ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_CK_DUTY_PRESETA    ((u32)0x00000007 << 0)          /*!<R/W ’h3  mck_ck duty control setting as mck_cmd setting */
#define DDRPHY_DPI_DCC_CK_DUTY_PRESETA(x)      ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DPI_DCC_CK_DUTY_PRESETA(x)  ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CRT_CTL_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_MCK_TEST_CLK_SEL           ((u32)0x0000000F << 28)          /*!<R/W ‘h0  to PAD MP test debug clock select (PLL output clock /32 to debug port) */
#define DDRPHY_MCK_TEST_CLK_SEL(x)             ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_MCK_TEST_CLK_SEL(x)         ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_BIT_ZQ_CAL_CLK_DIS              ((u32)0x00000001 << 19)          /*!<R/W ‘b0  set 1 to disable zq calibration clock */
#define DDRPHY_ZQ_CAL_CLK_DIS(x)               ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_ALERT_CHK_EN                ((u32)0x00000001 << 3)          /*!<R/W ‘b0  set 1 to enable alert pad security check */
#define DDRPHY_ALERT_CHK_EN(x)                 ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_TOPREG_CLK_DIS              ((u32)0x00000001 << 1)          /*!<R/W ‘b0  set 1 to disable regif clock of top */
#define DDRPHY_TOPREG_CLK_DIS(x)               ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_RWREG_CLK_DIS               ((u32)0x00000001 << 0)          /*!<R/W ‘b0  set 1 to disable regif clock of rw_blk */
#define DDRPHY_RWREG_CLK_DIS(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CRT_DMY
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_MCK_MC_CLK_SEL              ((u32)0x00000001 << 0)          /*!<R/W ‘b0  Select clock source of mck_mc 1’b0: select mck_cmd 1’b1: select PLL internal clock */
#define DDRPHY_MCK_MC_CLK_SEL(x)               ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_AFIFO_STR_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_RD_STR_NUM_3           ((u32)0x00000007 << 28)          /*!<R/W ’h1  slice3 DQS TX FIFO threshold setting */
#define DDRPHY_DQS_RD_STR_NUM_3(x)             ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_DQS_RD_STR_NUM_3(x)         ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_DQ_RD_STR_NUM_3            ((u32)0x00000007 << 24)          /*!<R/W ’h1  slice3 DQ TX FIFO threshold setting */
#define DDRPHY_DQ_RD_STR_NUM_3(x)              ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DQ_RD_STR_NUM_3(x)          ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DQS_RD_STR_NUM_2           ((u32)0x00000007 << 20)          /*!<R/W ’h1  slice2 DQS TX FIFO threshold setting */
#define DDRPHY_DQS_RD_STR_NUM_2(x)             ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_DQS_RD_STR_NUM_2(x)         ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_DQ_RD_STR_NUM_2            ((u32)0x00000007 << 16)          /*!<R/W ’h1  slice2 DQ TX FIFO threshold setting */
#define DDRPHY_DQ_RD_STR_NUM_2(x)              ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_DQ_RD_STR_NUM_2(x)          ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_DQS_RD_STR_NUM_1           ((u32)0x00000007 << 12)          /*!<R/W ’h1  slice1 DQS TX FIFO threshold setting */
#define DDRPHY_DQS_RD_STR_NUM_1(x)             ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQS_RD_STR_NUM_1(x)         ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQ_RD_STR_NUM_1            ((u32)0x00000007 << 8)          /*!<R/W ’h1  slice1 DQ TX FIFO threshold setting */
#define DDRPHY_DQ_RD_STR_NUM_1(x)              ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQ_RD_STR_NUM_1(x)          ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQS_RD_STR_NUM_0           ((u32)0x00000007 << 4)          /*!<R/W ’h1  slice0 DQS TX FIFO threshold setting */
#define DDRPHY_DQS_RD_STR_NUM_0(x)             ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQS_RD_STR_NUM_0(x)         ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQ_RD_STR_NUM_0            ((u32)0x00000007 << 0)          /*!<R/W ’h1  slice0 DQ TX FIFO threshold setting */
#define DDRPHY_DQ_RD_STR_NUM_0(x)              ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQ_RD_STR_NUM_0(x)          ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_AFIFO_STR_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_BYPASS_EN_RX                ((u32)0x00000001 << 21)          /*!<R/W ’h0  Bypass RX FIFO */
#define DDRPHY_BYPASS_EN_RX(x)                 ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_BYPASS_EN_TX                ((u32)0x00000001 << 20)          /*!<R/W ’h0  Bypass TX FIFO */
#define DDRPHY_BYPASS_EN_TX(x)                 ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_BIT_FORCE_RST_N_RX              ((u32)0x00000001 << 19)          /*!<R/W ’h1  Force reset RX FIFO */
#define DDRPHY_FORCE_RST_N_RX(x)               ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_FORCE_RST_N_TX              ((u32)0x00000001 << 18)          /*!<R/W ’h1  Force reset TX FIFO */
#define DDRPHY_FORCE_RST_N_TX(x)               ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_BIT_REF_RST_EN_RX               ((u32)0x00000001 << 17)          /*!<R/W ’h0  Enable reset RX FIFO during refresh */
#define DDRPHY_REF_RST_EN_RX(x)                ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_REF_RST_EN_TX               ((u32)0x00000001 << 16)          /*!<R/W ’h0  Enable reset TX FIFO during refresh */
#define DDRPHY_REF_RST_EN_TX(x)                ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_MASK_CMD_RD_STR_NUM_SEL_H       ((u32)0x000000FF << 8)          /*!<R/W ’h0  [15] : PAD cke_1 [14] : PAD alert_1 [13] : PAD par_1 [12] : PAD ddr4_cke [11] : PAD act_n [10] : PAD bg0 [09] : PAD par [08] : PAD ten */
#define DDRPHY_CMD_RD_STR_NUM_SEL_H(x)         ((u32)(((x) & 0x000000FF) << 8))
#define DDRPHY_GET_CMD_RD_STR_NUM_SEL_H(x)     ((u32)(((x >> 8) & 0x000000FF)))
#define DDRPHY_MASK_CMD_EX_RD_STR_NUM          ((u32)0x00000007 << 4)          /*!<R/W ’h2  CMD TX FIFO threshold setting (Another, Select by cmd_rd_str_num_sel) */
#define DDRPHY_CMD_EX_RD_STR_NUM(x)            ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CMD_EX_RD_STR_NUM(x)        ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CMD_RD_STR_NUM             ((u32)0x00000007 << 0)          /*!<R/W ’h1  CMD TX FIFO threshold setting */
#define DDRPHY_CMD_RD_STR_NUM(x)               ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CMD_RD_STR_NUM(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_AFIFO_STR_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RX_RD_STR_NUM_3            ((u32)0x00000007 << 12)          /*!<R/W ’h1  slice3 RX FIFO threshold setting */
#define DDRPHY_RX_RD_STR_NUM_3(x)              ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_RX_RD_STR_NUM_3(x)          ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_RX_RD_STR_NUM_2            ((u32)0x00000007 << 8)          /*!<R/W ’h1  slice2 RX FIFO threshold setting */
#define DDRPHY_RX_RD_STR_NUM_2(x)              ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_RX_RD_STR_NUM_2(x)          ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_RX_RD_STR_NUM_1            ((u32)0x00000007 << 4)          /*!<R/W ’h1  slice1 RX FIFO threshold setting */
#define DDRPHY_RX_RD_STR_NUM_1(x)              ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_RX_RD_STR_NUM_1(x)          ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_RX_RD_STR_NUM_0            ((u32)0x00000007 << 0)          /*!<R/W ’h1  slice0 RX FIFO threshold setting */
#define DDRPHY_RX_RD_STR_NUM_0(x)              ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_RX_RD_STR_NUM_0(x)          ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CRT_RST_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_BG_ON_MIPI                  ((u32)0x00000001 << 13)          /*!<R/W ‘b1  Bandgap MIPI current enable */
#define DDRPHY_BG_ON_MIPI(x)                   ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_BG_ON_USB2                  ((u32)0x00000001 << 12)          /*!<R/W ‘b1  Bandgap USB2 current enable */
#define DDRPHY_BG_ON_USB2(x)                   ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_PLL_BG_LPF                  ((u32)0x00000001 << 11)          /*!<R/W ‘b1  Bandgap low pass filter */
#define DDRPHY_PLL_BG_LPF(x)                   ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_MASK_PLL_BG                     ((u32)0x00000003 << 9)          /*!<R/W ‘h1  Bandgap enable mode */
#define DDRPHY_PLL_BG(x)                       ((u32)(((x) & 0x00000003) << 9))
#define DDRPHY_GET_PLL_BG(x)                   ((u32)(((x >> 9) & 0x00000003)))
#define DDRPHY_BIT_PLL_BG_RST_N                ((u32)0x00000001 << 8)          /*!<R/W ’b0  DDRPLL bandgap reset, low active */
#define DDRPHY_PLL_BG_RST_N(x)                 ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_PWCT                        ((u32)0x00000001 << 6)          /*!<R/W ‘b1  1’b0: 3.3V power cut */
#define DDRPHY_PWCT(x)                         ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_POW_LDO_V11                 ((u32)0x00000001 << 5)          /*!<R/W ‘h0  LDO for CK PAD 下面的AVDD1V為外灌的1V 开机时： 如果AVDD33先来,VD10A_DLL后来,在VD10A_DL没电时,pll_ldo_rst_n经过CDMCELL(吃AVDD1V)为0,LDO power down 且斷開外灌VD10A_DLL 如果AVDD1V先来,AVDD33后来,LDO处于power down状态,core power也无电,外灌1V的power switch也断开 Standby：pll_ldo_rst_n =0,LDO关闭且外灌断开； AVDD1V 有电,DVDD和AVDD3V无电,pll_ldo_rst_n 在经过CDMCELL时被DVDD POR卡住为0, LDO 无电且外灌断开,不会有漏电, Normal：pll_ldo_rst_n =1,REG_POW_LDO_V11=0 or 1不会出现外灌与LDO并联的情况； reg_pow_ ldo_v11 pll_ldo_rst_n 0 0 LDO power down 且外灌1V和LDO output间power switch断开 0 1 选外灌,此时 LDO 是power down 1 0 LDO power down 且外灌1V和LDO output间power switch断开 1 1 选LDO, 外灌的1V会给其他电路 */
#define DDRPHY_POW_LDO_V11(x)                  ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_POW_LDO_PLL                 ((u32)0x00000001 << 4)          /*!<R/W ‘h0  LDO for PLL CCO,控制和LDO_V11 控制相同 reg_pow_ldo_pll pll_ldo_rst_n 0 0 LDO power down 且外灌1V和LDO output间power switch断开 0 1 选外灌,此时 LDO 是power down 1 0 LDO power down 且外灌1V和LDO output间power switch断开 1 1 选LDO, 外灌的1V会给其他电路 */
#define DDRPHY_POW_LDO_PLL(x)                  ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_BIT_PTR_RST_N                   ((u32)0x00000001 << 3)          /*!<R/W ’b1  reset 3-point and dqsen calibration results, low active */
#define DDRPHY_PTR_RST_N(x)                    ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_RST_N                       ((u32)0x00000001 << 2)          /*!<R/W ’b0  reset all digital circuits, except 3-point calibration, dqsen calibration, and PLL SSC block, low active */
#define DDRPHY_RST_N(x)                        ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_PLL_SSC_DIG_RST_N           ((u32)0x00000001 << 1)          /*!<R/W ’b0  reset PLL SSC circuit, low active */
#define DDRPHY_PLL_SSC_DIG_RST_N(x)            ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_PLL_LDO_RST_N               ((u32)0x00000001 << 0)          /*!<R/W ’b0  DDRPLL LDO reset, low active */
#define DDRPHY_PLL_LDO_RST_N(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_AFIFO_STR_SEL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CMD_RD_STR_NUM_SEL         ((u32)0xFFFFFFFF << 0)          /*!<R/W ’h0  CMD TX FIFO threshold value select 1’b0 : cmd_rd_str_num 1’b1 : cmd_ex_rd_str_num [31] : PAD alert [30] : PAD ddr4_cs_n [29] : PAD cs_n_1 [28] : PAD cs_n [27] : PAD odt [26] : PAD cke [25] : PAD cas_n [24] : PAD ras_n [23] : PAD we_n [22] : PAD reset_n, [21:19] : PAD Bank[2:0] [18:17] : PAD Adr_ext[5:4] [16: 0] : PAD Address[16:0] */
#define DDRPHY_CMD_RD_STR_NUM_SEL(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_CMD_RD_STR_NUM_SEL(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_PI2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_POST_PI_SEL10          ((u32)0x0000001F << 24)          /*!<R/W ’h0  PLL phase select for mck_cs */
#define DDRPHY_DPI_POST_PI_SEL10(x)            ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_DPI_POST_PI_SEL10(x)        ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL9           ((u32)0x0000001F << 16)          /*!<R/W ’h0  PLL phase select for mck_dq_3 */
#define DDRPHY_DPI_POST_PI_SEL9(x)             ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_POST_PI_SEL9(x)         ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL8           ((u32)0x0000001F << 8)          /*!<R/W ’h0  PLL phase select for mck_dq_2 */
#define DDRPHY_DPI_POST_PI_SEL8(x)             ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_DPI_POST_PI_SEL8(x)         ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL7           ((u32)0x0000001F << 0)          /*!<R/W ’h0  PLL phase select for mck_dq_1 */
#define DDRPHY_DPI_POST_PI_SEL7(x)             ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_DPI_POST_PI_SEL7(x)         ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_CTL4
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_CLK_PI_CTRL                 ((u32)0x00000001 << 31)          /*!<R/W ‘h0  0: PI of CLK1X/CLK1XB is controlled by dpi_post_pi_sel* 1: PI of CLK1X/CLK1XB is controlled by dpi_post_pi_sel* and fw_set_wr_dly */
#define DDRPHY_CLK_PI_CTRL(x)                  ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_DPI_DLY_SEL                ((u32)0x0000001F << 24)          /*!<R/W ‘h10  Delay chain selection for CLK1X_0 and CLK1XB_0 */
#define DDRPHY_DPI_DLY_SEL(x)                  ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_DPI_DLY_SEL(x)              ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_DPI_DLY_SEL_CK1            ((u32)0x0000001F << 16)          /*!<R/W ‘h10  Delay chain selection for CLK1X_12 and CLK1XB_12(No use) */
#define DDRPHY_DPI_DLY_SEL_CK1(x)              ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_DLY_SEL_CK1(x)          ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_EN_POST_PI             ((u32)0x00003FFF << 0)          /*!<R/W ’h3FFF  Enable PLL phase selector post_pi[0]: mck_ck post_pi[1]: mck_cmd post_pi[2]: mck_dqs_0 post_pi[3]: mck_dqs_1 post_pi[4]: mck_dqs_2 post_pi[5]: mck_dqs_3 post_pi[6]: mck_dq_0 post_pi[7]: mck_dq_1 post_pi[8]: mck_dq_2 post_pi[9]: mck_dq_3 post_pi[10]: mck_cs post_pi[11]: mck_cs_1 post_pi[12]: PLL internal clock */
#define DDRPHY_DPI_EN_POST_PI(x)               ((u32)(((x) & 0x00003FFF) << 0))
#define DDRPHY_GET_DPI_EN_POST_PI(x)           ((u32)(((x >> 0) & 0x00003FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_PI3
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_POST_PI_SEL12          ((u32)0x0000001F << 8)          /*!<R/W ’h0  PLL phase select for mck_ck_1 */
#define DDRPHY_DPI_POST_PI_SEL12(x)            ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_DPI_POST_PI_SEL12(x)        ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_DPI_POST_PI_SEL11          ((u32)0x0000001F << 0)          /*!<R/W ’h0  PLL phase select for mck_cs_1 */
#define DDRPHY_DPI_POST_PI_SEL11(x)            ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_DPI_POST_PI_SEL11(x)        ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DCC_CSx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DPI_DCC_CS_x_DSP_DBG_SEL    ((u32)0x00000001 << 31)          /*!<R/W ’b0  mck_csx duty control debug output enable */
#define DDRPHY_DPI_DCC_CS_x_DSP_DBG_SEL(x)     ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_DPI_DCC_CS_x_DUTY_SEL       ((u32)0x00000001 << 29)          /*!<R/W ’b0  mck_csx duty control reg_dpi_dcc_cs_duty_sel=0: output from close loop; reg_dpi_dcc_cs_duty_sel=1: output from reg_dpi_dcc_cs_duty[6:0] */
#define DDRPHY_DPI_DCC_CS_x_DUTY_SEL(x)        ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_DPI_DCC_CS_x_DSP_STOP       ((u32)0x00000001 << 28)          /*!<R/W ’b1  mck_csx duty control dsp function enable */
#define DDRPHY_DPI_DCC_CS_x_DSP_STOP(x)        ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_DPI_DCC_CS_x_DIV           ((u32)0x00000007 << 24)          /*!<R/W ’h5  mck_csx duty control dsp clock frequency setting: 000~111: /2^4~/2^11 */
#define DDRPHY_DPI_DCC_CS_x_DIV(x)             ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DPI_DCC_CS_x_DIV(x)         ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_CS_x_INT_GAIN      ((u32)0x0000001F << 16)          /*!<R/W ’h05  mck_csx duty control close loop gain setting: 00000~11111: /2^6~/2 */
#define DDRPHY_DPI_DCC_CS_x_INT_GAIN(x)        ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_DPI_DCC_CS_x_INT_GAIN(x)    ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_DPI_DCC_CS_x_DUTY          ((u32)0x0000007F << 8)          /*!<R/W ’h38  mck_csx duty control when reg_dpi_dcc_cs_duty_sel=1'b0, output value from this register */
#define DDRPHY_DPI_DCC_CS_x_DUTY(x)            ((u32)(((x) & 0x0000007F) << 8))
#define DDRPHY_GET_DPI_DCC_CS_x_DUTY(x)        ((u32)(((x >> 8) & 0x0000007F)))
#define DDRPHY_MASK_DPI_DCC_CS_x_DUTY_PRESETB  ((u32)0x00000007 << 4)          /*!<R/W ’h3  mck_csx duty control DCD negitive duty preset setting */
#define DDRPHY_DPI_DCC_CS_x_DUTY_PRESETB(x)    ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DPI_DCC_CS_x_DUTY_PRESETB(x) ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DPI_DCC_CS_x_DUTY_PRESETA  ((u32)0x00000007 << 0)          /*!<R/W ’h3  mck_csx duty control DCD positive duty preset setting */
#define DDRPHY_DPI_DCC_CS_x_DUTY_PRESETA(x)    ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DPI_DCC_CS_x_DUTY_PRESETA(x) ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PLL_CTL5
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_PWROFF_PWR_WAIT_CYCLE  ((u32)0x00000007 << 12)          /*!<R/W ‘h2  clk on wait cycle when dfi_pwr is 0 0: wait 0 cycle 1: wait 8 mck_mc cycles 2: wait 8x2 mck_mc cycles 3: wait 8x3 mck_mc cycles,etc. */
#define DDRPHY_DPI_PWROFF_PWR_WAIT_CYCLE(x)    ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DPI_PWROFF_PWR_WAIT_CYCLE(x) ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DPI_PWROFF_RD_WAIT_CYCLE   ((u32)0x00000007 << 9)          /*!<R/W ‘h2  clk on wait cycle when RD is end, 0: wait 1 mck_mc cycle 1: wait 8 mck_mc cycles 2: wait 8x2 mck_mc cycles 3: wait 8x3 mck_mc cycles,etc. */
#define DDRPHY_DPI_PWROFF_RD_WAIT_CYCLE(x)     ((u32)(((x) & 0x00000007) << 9))
#define DDRPHY_GET_DPI_PWROFF_RD_WAIT_CYCLE(x) ((u32)(((x >> 9) & 0x00000007)))
#define DDRPHY_MASK_DPI_PWROFF_WR_WAIT_CYCLE   ((u32)0x00000007 << 6)          /*!<R/W ‘h2  clk on wait cycle when WR is end 0: wait 1 mck_mc cycle 1: wait 8 mck_mc cycles 2: wait 8x2 mck_mc cycles 3: wait 8x3 mck_mc cycles,etc. */
#define DDRPHY_DPI_PWROFF_WR_WAIT_CYCLE(x)     ((u32)(((x) & 0x00000007) << 6))
#define DDRPHY_GET_DPI_PWROFF_WR_WAIT_CYCLE(x) ((u32)(((x >> 6) & 0x00000007)))
#define DDRPHY_MASK_DPI_RW_PWROFF_MODE         ((u32)0x00000003 << 4)          /*!<R/W ‘h0  rw blk clocks gating mode: 0: no gating 1: no REF and dfi_pwr = 0, 2: no REF/WR/RD 3: no REF and (no WR/RD or dfi_pwr = 0) */
#define DDRPHY_DPI_RW_PWROFF_MODE(x)           ((u32)(((x) & 0x00000003) << 4))
#define DDRPHY_GET_DPI_RW_PWROFF_MODE(x)       ((u32)(((x >> 4) & 0x00000003)))
#define DDRPHY_MASK_DPI_CA_PWROFF_MODE         ((u32)0x00000003 << 2)          /*!<R/W ‘h0  CMD/ADDR clocks gating mode: 0: no gating 1: gating DPI clock when dfi_pwr = 0 2: gating DPI clock and DCK when dfi_pwr = 0 3: reserved */
#define DDRPHY_DPI_CA_PWROFF_MODE(x)           ((u32)(((x) & 0x00000003) << 2))
#define DDRPHY_GET_DPI_CA_PWROFF_MODE(x)       ((u32)(((x >> 2) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_DLY_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR07_DLY_SEL              ((u32)0x0000000F << 28)          /*!<R/W ’h0  Address[7] delay chain control */
#define DDRPHY_ADR07_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_ADR07_DLY_SEL(x)            ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_ADR06_DLY_SEL              ((u32)0x0000000F << 24)          /*!<R/W ’h0  Address[6] delay chain control */
#define DDRPHY_ADR06_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_ADR06_DLY_SEL(x)            ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_ADR05_DLY_SEL              ((u32)0x0000000F << 20)          /*!<R/W ’h0  Address[5] delay chain control */
#define DDRPHY_ADR05_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_ADR05_DLY_SEL(x)            ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_ADR04_DLY_SEL              ((u32)0x0000000F << 16)          /*!<R/W ’h0  Address[4] delay chain control */
#define DDRPHY_ADR04_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_ADR04_DLY_SEL(x)            ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_ADR03_DLY_SEL              ((u32)0x0000000F << 12)          /*!<R/W ’h0  Address[3] delay chain control */
#define DDRPHY_ADR03_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_ADR03_DLY_SEL(x)            ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_ADR02_DLY_SEL              ((u32)0x0000000F << 8)          /*!<R/W ’h0  Address[2] delay chain control */
#define DDRPHY_ADR02_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_ADR02_DLY_SEL(x)            ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_ADR01_DLY_SEL              ((u32)0x0000000F << 4)          /*!<R/W ’h0  Address[1] delay chain control */
#define DDRPHY_ADR01_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_ADR01_DLY_SEL(x)            ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_ADR00_DLY_SEL              ((u32)0x0000000F << 0)          /*!<R/W ’h0  Address[0] delay chain control */
#define DDRPHY_ADR00_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_ADR00_DLY_SEL(x)            ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_DLY_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR15_DLY_SEL              ((u32)0x0000000F << 28)          /*!<R/W ’h0  Address[15] delay chain control */
#define DDRPHY_ADR15_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_ADR15_DLY_SEL(x)            ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_ADR14_DLY_SEL              ((u32)0x0000000F << 24)          /*!<R/W ’h0  Address[14] delay chain control */
#define DDRPHY_ADR14_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_ADR14_DLY_SEL(x)            ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_ADR13_DLY_SEL              ((u32)0x0000000F << 20)          /*!<R/W ’h0  Address[13] delay chain control */
#define DDRPHY_ADR13_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_ADR13_DLY_SEL(x)            ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_ADR12_DLY_SEL              ((u32)0x0000000F << 16)          /*!<R/W ’h0  Address[12] delay chain control */
#define DDRPHY_ADR12_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_ADR12_DLY_SEL(x)            ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_ADR11_DLY_SEL              ((u32)0x0000000F << 12)          /*!<R/W ’h0  Address[11] delay chain control */
#define DDRPHY_ADR11_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_ADR11_DLY_SEL(x)            ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_ADR10_DLY_SEL              ((u32)0x0000000F << 8)          /*!<R/W ’h0  Address[10] delay chain control */
#define DDRPHY_ADR10_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_ADR10_DLY_SEL(x)            ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_ADR09_DLY_SEL              ((u32)0x0000000F << 4)          /*!<R/W ’h0  Address[9] delay chain control */
#define DDRPHY_ADR09_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_ADR09_DLY_SEL(x)            ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_ADR08_DLY_SEL              ((u32)0x0000000F << 0)          /*!<R/W ’h0  Address[8] delay chain control */
#define DDRPHY_ADR08_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_ADR08_DLY_SEL(x)            ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_DLY_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ALERT_DLY_SEL              ((u32)0x0000000F << 28)          /*!<R/W ’h0  Alert delay chain control */
#define DDRPHY_ALERT_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_ALERT_DLY_SEL(x)            ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_TEN_DLY_SEL                ((u32)0x0000000F << 24)          /*!<R/W ’h0  Ten delay chain control ( for DDR4 BG0 use) */
#define DDRPHY_TEN_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_TEN_DLY_SEL(x)              ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_ADR16_DLY_SEL              ((u32)0x0000000F << 20)          /*!<R/W ’h0  Address[16] delay chain control (for DDR4 A1 used) */
#define DDRPHY_ADR16_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_ADR16_DLY_SEL(x)            ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_BA2_DLY_SEL                ((u32)0x0000000F << 16)          /*!<R/W ’h0  BANK[2] delay chain control */
#define DDRPHY_BA2_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_BA2_DLY_SEL(x)              ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_BA1_DLY_SEL                ((u32)0x0000000F << 12)          /*!<R/W ’h0  BANK[1] delay chain control */
#define DDRPHY_BA1_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_BA1_DLY_SEL(x)              ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_BA0_DLY_SEL                ((u32)0x0000000F << 8)          /*!<R/W ’h0  BANK[0] delay chain control */
#define DDRPHY_BA0_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_BA0_DLY_SEL(x)              ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_ADR5_EXT_DLY_SEL           ((u32)0x0000000F << 4)          /*!<R/W ’h0  Extent address[5] delay chain control */
#define DDRPHY_ADR5_EXT_DLY_SEL(x)             ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_ADR5_EXT_DLY_SEL(x)         ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_ADR4_EXT_DLY_SEL           ((u32)0x0000000F << 0)          /*!<R/W ’h0  Extent address[4] delay chain control */
#define DDRPHY_ADR4_EXT_DLY_SEL(x)             ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_ADR4_EXT_DLY_SEL(x)         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CMD_DLY_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CS_N_1_DLY_SEL             ((u32)0x0000000F << 26)          /*!<R/W ’h0  CS_N_1 delay chain control */
#define DDRPHY_CS_N_1_DLY_SEL(x)               ((u32)(((x) & 0x0000000F) << 26))
#define DDRPHY_GET_CS_N_1_DLY_SEL(x)           ((u32)(((x >> 26) & 0x0000000F)))
#define DDRPHY_MASK_CS_N_DLY_SEL               ((u32)0x0000000F << 20)          /*!<R/W ’h0  CS_N delay chain control */
#define DDRPHY_CS_N_DLY_SEL(x)                 ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_CS_N_DLY_SEL(x)             ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_ODT_DLY_SEL                ((u32)0x0000000F << 16)          /*!<R/W ’h0  ODT delay chain control */
#define DDRPHY_ODT_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_ODT_DLY_SEL(x)              ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_WE_N_DLY_SEL               ((u32)0x0000000F << 12)          /*!<R/W ’h0  WE_N delay chain control */
#define DDRPHY_WE_N_DLY_SEL(x)                 ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_WE_N_DLY_SEL(x)             ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_CAS_N_DLY_SEL              ((u32)0x0000000F << 8)          /*!<R/W ’h0  CAS_N delay chain control */
#define DDRPHY_CAS_N_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_CAS_N_DLY_SEL(x)            ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_RAS_N_DLY_SEL              ((u32)0x0000000F << 4)          /*!<R/W ’h0  RAS_N delay chain control */
#define DDRPHY_RAS_N_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_RAS_N_DLY_SEL(x)            ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_CKE_DLY_SEL                ((u32)0x0000000F << 0)          /*!<R/W ’h0  CKE delay chain control */
#define DDRPHY_CKE_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_CKE_DLY_SEL(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CMD_DLY_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CKE_1_DLY_SEL              ((u32)0x0000000F << 28)          /*!<R/W ’h0  CKE_1 delay chain control */
#define DDRPHY_CKE_1_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_CKE_1_DLY_SEL(x)            ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_DDR4_CS_N_DLY_SEL          ((u32)0x0000000F << 24)          /*!<R/W ’h0  DDR4_CS_N delay chain control */
#define DDRPHY_DDR4_CS_N_DLY_SEL(x)            ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_DDR4_CS_N_DLY_SEL(x)        ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_ALERT_1_DLY_SEL            ((u32)0x0000000F << 20)          /*!<R/W ’h0  ALERT_1 delay chain control */
#define DDRPHY_ALERT_1_DLY_SEL(x)              ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_ALERT_1_DLY_SEL(x)          ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_PARITY_1_DLY_SEL           ((u32)0x0000000F << 16)          /*!<R/W ’h0  PARITY_1 delay chain control */
#define DDRPHY_PARITY_1_DLY_SEL(x)             ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_PARITY_1_DLY_SEL(x)         ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_PARITY_DLY_SEL             ((u32)0x0000000F << 12)          /*!<R/W ’h0  PARITY delay chain control */
#define DDRPHY_PARITY_DLY_SEL(x)               ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_PARITY_DLY_SEL(x)           ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_DDR4_BG0_DLY_SEL           ((u32)0x0000000F << 8)          /*!<R/W ’h0  DDR4_BG0 delay chain control */
#define DDRPHY_DDR4_BG0_DLY_SEL(x)             ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DDR4_BG0_DLY_SEL(x)         ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_DDR4_CKE_DLY_SEL           ((u32)0x0000000F << 4)          /*!<R/W ’h0  DDR4_CKE delay chain control */
#define DDRPHY_DDR4_CKE_DLY_SEL(x)             ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DDR4_CKE_DLY_SEL(x)         ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_ACT_N_DLY_SEL              ((u32)0x0000000F << 0)          /*!<R/W ’h0  ACT_N delay chain control */
#define DDRPHY_ACT_N_DLY_SEL(x)                ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_ACT_N_DLY_SEL(x)            ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_OE_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_ADR54_1TO1                  ((u32)0x00000001 << 31)          /*!<R/W ’b1  Address[5:4] OCD control: 1: controlled by ocd_set_5 (1 -> 1) 0: controlled by ocd_set_4 (1 -> 2) */
#define DDRPHY_ADR54_1TO1(x)                   ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_DCK_1_OE                    ((u32)0x00000001 << 28)          /*!<R/W ’b1  Output enable of DCK_1 */
#define DDRPHY_DCK_1_OE(x)                     ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_DCK_OE                      ((u32)0x00000001 << 27)          /*!<R/W ’b1  Output enable of DCK */
#define DDRPHY_DCK_OE(x)                       ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_BIT_CKE_OE                      ((u32)0x00000001 << 26)          /*!<R/W ’b1  Output enable of CKE */
#define DDRPHY_CKE_OE(x)                       ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_BIT_RAS_N_OE                    ((u32)0x00000001 << 25)          /*!<R/W ’b1  Output enable of RAS_N */
#define DDRPHY_RAS_N_OE(x)                     ((u32)(((x) & 0x00000001) << 25))
#define DDRPHY_BIT_CAS_N_OE                    ((u32)0x00000001 << 24)          /*!<R/W ’b1  Output enable of CAS_N */
#define DDRPHY_CAS_N_OE(x)                     ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_WE_N_OE                     ((u32)0x00000001 << 23)          /*!<R/W ’b1  Output enable of WE_N */
#define DDRPHY_WE_N_OE(x)                      ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_ODT_OE                      ((u32)0x00000001 << 22)          /*!<R/W ’b1  Output enable of ODT */
#define DDRPHY_ODT_OE(x)                       ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_BIT_RESET_N_OE                  ((u32)0x00000001 << 21)          /*!<R/W ’b1  Output enable of Reset_N */
#define DDRPHY_RESET_N_OE(x)                   ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_BK2_OE                      ((u32)0x00000001 << 20)          /*!<R/W ’b1  Output enable of BANK[2] */
#define DDRPHY_BK2_OE(x)                       ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_BIT_BK1_OE                      ((u32)0x00000001 << 19)          /*!<R/W ’b1  Output enable of BANK[1] */
#define DDRPHY_BK1_OE(x)                       ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_BK0_OE                      ((u32)0x00000001 << 18)          /*!<R/W ’b1  Output enable of BANK[0] */
#define DDRPHY_BK0_OE(x)                       ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_BIT_EXT_ADR5_OE                 ((u32)0x00000001 << 17)          /*!<R/W ’b1  Output enable of Extent address[5] */
#define DDRPHY_EXT_ADR5_OE(x)                  ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_EXT_ADR4_OE                 ((u32)0x00000001 << 16)          /*!<R/W ’b1  Output enable of Extent address[4] */
#define DDRPHY_EXT_ADR4_OE(x)                  ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_BIT_ADR15_OE                    ((u32)0x00000001 << 15)          /*!<R/W ’b1  Output enable of address[15] */
#define DDRPHY_ADR15_OE(x)                     ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_BIT_ADR14_OE                    ((u32)0x00000001 << 14)          /*!<R/W ’b1  Output enable of address[14] */
#define DDRPHY_ADR14_OE(x)                     ((u32)(((x) & 0x00000001) << 14))
#define DDRPHY_BIT_ADR13_OE                    ((u32)0x00000001 << 13)          /*!<R/W ’b1  Output enable of address[13] */
#define DDRPHY_ADR13_OE(x)                     ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_ADR12_OE                    ((u32)0x00000001 << 12)          /*!<R/W ’b1  Output enable of address[12] */
#define DDRPHY_ADR12_OE(x)                     ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_ADR11_OE                    ((u32)0x00000001 << 11)          /*!<R/W ’b1  Output enable of address[11] */
#define DDRPHY_ADR11_OE(x)                     ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_BIT_ADR10_OE                    ((u32)0x00000001 << 10)          /*!<R/W ’b1  Output enable of address[10] */
#define DDRPHY_ADR10_OE(x)                     ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_ADR09_OE                    ((u32)0x00000001 << 9)          /*!<R/W ’b1  Output enable of address[09] */
#define DDRPHY_ADR09_OE(x)                     ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_ADR08_OE                    ((u32)0x00000001 << 8)          /*!<R/W ’b1  Output enable of address[08] */
#define DDRPHY_ADR08_OE(x)                     ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_ADR07_OE                    ((u32)0x00000001 << 7)          /*!<R/W ’b1  Output enable of address[07] */
#define DDRPHY_ADR07_OE(x)                     ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_BIT_ADR06_OE                    ((u32)0x00000001 << 6)          /*!<R/W ’b1  Output enable of address[06] */
#define DDRPHY_ADR06_OE(x)                     ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_ADR05_OE                    ((u32)0x00000001 << 5)          /*!<R/W ’b1  Output enable of address[05] */
#define DDRPHY_ADR05_OE(x)                     ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_ADR04_OE                    ((u32)0x00000001 << 4)          /*!<R/W ’b1  Output enable of address[04] */
#define DDRPHY_ADR04_OE(x)                     ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_BIT_ADR03_OE                    ((u32)0x00000001 << 3)          /*!<R/W ’b1  Output enable of address[03] */
#define DDRPHY_ADR03_OE(x)                     ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_ADR02_OE                    ((u32)0x00000001 << 2)          /*!<R/W ’b1  Output enable of address[02] */
#define DDRPHY_ADR02_OE(x)                     ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_ADR01_OE                    ((u32)0x00000001 << 1)          /*!<R/W ’b1  Output enable of address[01] */
#define DDRPHY_ADR01_OE(x)                     ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_ADR00_OE                    ((u32)0x00000001 << 0)          /*!<R/W ’b1  Output enable of address[00] */
#define DDRPHY_ADR00_OE(x)                     ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_OE_CTL_EX
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_ODT_FORCE                   ((u32)0x00000001 << 31)          /*!<R/W ‘h0  1 for forcing ODT to high when do write leveling */
#define DDRPHY_ODT_FORCE(x)                    ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_ACT_N_OE                    ((u32)0x00000001 << 17)          /*!<R/W ’b1  Output enable of ACTN */
#define DDRPHY_ACT_N_OE(x)                     ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_DDR4_TEN_OE                 ((u32)0x00000001 << 16)          /*!<R/W ’b1  Output enable of DDR4 TEN */
#define DDRPHY_DDR4_TEN_OE(x)                  ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_BIT_DDR4_ALERT0_OE              ((u32)0x00000001 << 15)          /*!<R/W ’b1  Output enable of DDR4 ALERT_0 */
#define DDRPHY_DDR4_ALERT0_OE(x)               ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_BIT_DDR4_ALERT1_OE              ((u32)0x00000001 << 14)          /*!<R/W ’b1  Output enable of DDR4 ALERT_1 */
#define DDRPHY_DDR4_ALERT1_OE(x)               ((u32)(((x) & 0x00000001) << 14))
#define DDRPHY_BIT_DDR4_CKE_OE                 ((u32)0x00000001 << 13)          /*!<R/W ’b1  Output enable of DDR4 CKE */
#define DDRPHY_DDR4_CKE_OE(x)                  ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_DDR4_BG0_OE                 ((u32)0x00000001 << 12)          /*!<R/W ’b1  Ouput enable of DDR4 BG0 */
#define DDRPHY_DDR4_BG0_OE(x)                  ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_PARITY_OE                   ((u32)0x00000001 << 11)          /*!<R/W ’b1  Ouput enable of PARITY */
#define DDRPHY_PARITY_OE(x)                    ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_BIT_PARITY_1_OE                 ((u32)0x00000001 << 10)          /*!<R/W ’b1  Ouput enable of PARITY_1 */
#define DDRPHY_PARITY_1_OE(x)                  ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_ADR16_OE                    ((u32)0x00000001 << 9)          /*!<R/W ’b1  Ouput enable of A16 */
#define DDRPHY_ADR16_OE(x)                     ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_DDR4_CS_N_OE                ((u32)0x00000001 << 8)          /*!<R/W ’b1  Output enable of DDR4 DCS_N */
#define DDRPHY_DDR4_CS_N_OE(x)                 ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_CKE_1_OE                    ((u32)0x00000001 << 2)          /*!<R/W ’b1  Output enable of CKE_1 */
#define DDRPHY_CKE_1_OE(x)                     ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_CS_N_1_OE                   ((u32)0x00000001 << 1)          /*!<R/W ’b1  Output enable of DCS_N_1 */
#define DDRPHY_CS_N_1_OE(x)                    ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_CS_N_OE                     ((u32)0x00000001 << 0)          /*!<R/W ’b1  Output enable of DCS_N */
#define DDRPHY_CS_N_OE(x)                      ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_ALERT_x_SEL_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ALERT_SEL_0                ((u32)0xFFFFFFFF << 0)          /*!<R/W ’h00000000  ALERT enable of { DDR4 ALERT_1, DDR4 CKE, DDR4 BG0, PARITY, PARITY_1, CKE, RAS_N, CAS_N, WE_N, ODT, BANK[2], BANK[1], BANK[0], Extent address[5], Extent address[4], Address[16:0]} */
#define DDRPHY_ALERT_SEL_0(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_ALERT_SEL_0(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_ALERT_x_SEL_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ALERT_SEL_1                ((u32)0x0000000F << 0)          /*!<R/W ’h1  ALERT enable of { CKE_1, ACTN, DDR4 TEN, DDR4 ALERT_0} */
#define DDRPHY_ALERT_SEL_1(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_ALERT_SEL_1(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_IDDQ
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_IDDQ                       ((u32)0x00000FFF << 0)          /*!<R/W ’h0  For power-down input comparator bit0: dqs[0] bit1: dqs_n[0] …… bit6: dqs[3] bit7: dqs_n[3] bit8: dq[7:0] …… bit11: dq[31:24] */
#define DDRPHY_IDDQ(x)                         ((u32)(((x) & 0x00000FFF) << 0))
#define DDRPHY_GET_IDDQ(x)                     ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_CTRL_PROG
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_ZCTRL_FILTER_EN             ((u32)0x00000001 << 31)          /*!<R/W ‘h0  ZQ Calibration filter enable */
#define DDRPHY_ZCTRL_FILTER_EN(x)              ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_DZQ_UP_SEL                 ((u32)0x00000007 << 28)          /*!<R/W ’h0  Enable auto update OCD / ODT set0~set7 */
#define DDRPHY_DZQ_UP_SEL(x)                   ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_DZQ_UP_SEL(x)               ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_BIT_DZQ_AUTO_UP                 ((u32)0x00000001 << 27)          /*!<R/W ’h0  Auto update TTFP, TTCP, TTFN, TTCN, PT, NT, E2 of dq31~dq0 and dqs3~dqs0 when calibration done. */
#define DDRPHY_DZQ_AUTO_UP(x)                  ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_MASK_ZCTRL_CLK_SEL              ((u32)0x00000003 << 25)          /*!<R/W ’h2  Zctrl clock select 00: zclk/16 01: zclk/32 10: zclk/64 11: zclk/128 */
#define DDRPHY_ZCTRL_CLK_SEL(x)                ((u32)(((x) & 0x00000003) << 25))
#define DDRPHY_GET_ZCTRL_CLK_SEL(x)            ((u32)(((x >> 25) & 0x00000003)))
#define DDRPHY_BIT_ZCTRL_START                 ((u32)0x00000001 << 24)          /*!<R/W ’h0  Calibration start */
#define DDRPHY_ZCTRL_START(x)                  ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_ZCOMP_MODE                  ((u32)0x00000001 << 23)          /*!<R/W ’b0  1’b0 : catch first result 1’b1 : catch last result */
#define DDRPHY_ZCOMP_MODE(x)                   ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_MASK_ZCTRL_MODE                 ((u32)0x00000007 << 20)          /*!<R/W ’h0  3’h0 : NOCD(CAL00)+POCD(CAL01)+ NODT(CAL10)+PODT(CAL11) 3’h1 : NOCD(CAL00) only 3’h2 : POCD(CAL01) only 3’h3 : NODT(CAL10) only 3’h4 : PODT(CAL11) only */
#define DDRPHY_ZCTRL_MODE(x)                   ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ZCTRL_MODE(x)               ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_BIT_RZQ_EXT_R240                ((u32)0x00000001 << 19)          /*!<R/W ’h0  1: select external calibration resistor 0: select internal calibration resistor */
#define DDRPHY_RZQ_EXT_R240(x)                 ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_DZQ_REF_UP                  ((u32)0x00000001 << 18)          /*!<R/W ’h0  Auto update TTFP, TTCP, TTFN, TTCN, PT, NT, E2 of dq31~dq0 and dqs3~dqs0 during refresh */
#define DDRPHY_DZQ_REF_UP(x)                   ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_BIT_RZQ_COMP_INV                ((u32)0x00000001 << 17)          /*!<R/W ’h0  Invert comp polarity */
#define DDRPHY_RZQ_COMP_INV(x)                 ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_RZQ_CAL_EN                  ((u32)0x00000001 << 16)          /*!<R/W ’h0  Enable resistor 480Ω calibration */
#define DDRPHY_RZQ_CAL_EN(x)                   ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_MASK_ZCTR_NCYC                  ((u32)0x0000007F << 8)          /*!<R/W ’d12  ZQ calibration PRE POCD/NODT/PODT null cycle (unit : zclk_p) */
#define DDRPHY_ZCTR_NCYC(x)                    ((u32)(((x) & 0x0000007F) << 8))
#define DDRPHY_GET_ZCTR_NCYC(x)                ((u32)(((x >> 8) & 0x0000007F)))
#define DDRPHY_BIT_ZQ_CAL_CMD                  ((u32)0x00000001 << 7)          /*!<R/W ‘h0  for ZQ PAD */
#define DDRPHY_ZQ_CAL_CMD(x)                   ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_MASK_ZCTR_NCYC_LRG              ((u32)0x0000007F << 0)          /*!<R/W ’d25  ZQ calibration PRE NOCD null cycle (unit : zclk_p) */
#define DDRPHY_ZCTR_NCYC_LRG(x)                ((u32)(((x) & 0x0000007F) << 0))
#define DDRPHY_GET_ZCTR_NCYC_LRG(x)            ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ZQ_NOCD2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ZQ_OVWR_NOCD_DATA          ((u32)0x0000003F << 8)          /*!<R/W ’h0  when zctrl_mode = 2 or 4 (POCD/ PDOT only) ZQ _PAD NOCD = zq_ovwr_nocd_data */
#define DDRPHY_ZQ_OVWR_NOCD_DATA(x)            ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ZQ_OVWR_NOCD_DATA(x)        ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_BIT_ZQ_ENA_NOCD2                ((u32)0x00000001 << 0)          /*!<R/W ’b0  directly control ZQ module NOCD2 enable */
#define DDRPHY_ZQ_ENA_NOCD2(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_ZCTRL_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ZCTRL_STATUS               ((u32)0xFFFFFFFF << 0)          /*!<R/W ’h0  Zctrl execution status xxx_sts : 2’b00 : no error 2’b01 : overflow error 2’b10 : underflow error 2’b11 : calibration in progress [31] : zctrl_done [30] : rzq480_done [29:24] : Reserved [23:22] : Reserved [21:20] : rzq480_sts [19:18] : Reserved [17:16] : nocd2_sts [15:14] : Reserved [13:12] : podt_sts [11:10] : Reserved [ 9: 8] : nodt_sts [ 7: 6] : Reserved [ 5: 4] : pocd_sts [ 3: 2] : Reserved [ 1: 0] : nocd_sts */
#define DDRPHY_ZCTRL_STATUS(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_ZCTRL_STATUS(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_ZCTRL_RESULT
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ZCTRL_RESULT               ((u32)0xFFFFFFFF << 0)          /*!<R ’h0  Zctrl execution result [31:30] : Reserved [29:24] : PODT [23] : Reserved [22:16] : NODT [15:14] : Reserved [13: 8] : POCD [ 7] : Reserved [ 6: 0] : NOCD */
#define DDRPHY_ZCTRL_RESULT(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_ZCTRL_RESULT(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_ZCTRL_RESULT_D
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ZCTRL_RESULT_DBG           ((u32)0xFFFFFFFF << 0)          /*!<R ’h0  Zctrl execution result [31: 8] : Reserved [ 7] : Reserved [ 6: 0] : NOCD2 */
#define DDRPHY_ZCTRL_RESULT_DBG(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_ZCTRL_RESULT_DBG(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_RZCTRL_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RZQ_480CODE                ((u32)0x0000001F << 1)          /*!<R/W ’hf  Control resistor value */
#define DDRPHY_RZQ_480CODE(x)                  ((u32)(((x) & 0x0000001F) << 1))
#define DDRPHY_GET_RZQ_480CODE(x)              ((u32)(((x >> 1) & 0x0000001F)))
#define DDRPHY_BIT_RZQ_CAL_DONE                ((u32)0x00000001 << 0)          /*!<R ’h0  Resistor 480Ω calibration done */
#define DDRPHY_RZQ_CAL_DONE(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_DQS_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_TEN_EN                      ((u32)0x00000001 << 24)          /*!<R/W ‘h0  1 for ten enable */
#define DDRPHY_TEN_EN(x)                       ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_DQS_OE_SIG                  ((u32)0x00000001 << 3)          /*!<R/W ‘h1  DQS OE */
#define DDRPHY_DQS_OE_SIG(x)                   ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_DQ_LATCH_EN                 ((u32)0x00000001 << 2)          /*!<R/W ‘h0  Capture DQ bus enable */
#define DDRPHY_DQ_LATCH_EN(x)                  ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_TRIG_DQS                    ((u32)0x00000001 << 1)          /*!<W ’h0  dqs 1T pulse trigger (one cycle pulse, HW auto clear). */
#define DDRPHY_TRIG_DQS(x)                     ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_DQS_OE_FRC_EN               ((u32)0x00000001 << 0)          /*!<R/W ’h0  when set to 1, oe of dqs pad state will depend on the vaule of bit[3] (dqs_oe_sig) */
#define DDRPHY_DQS_OE_FRC_EN(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CMD_ADR_PH
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_CKE_1_PH_EN                 ((u32)0x00000001 << 30)          /*!<R/W ‘h0  CKE_1 output delay enable */
#define DDRPHY_CKE_1_PH_EN(x)                  ((u32)(((x) & 0x00000001) << 30))
#define DDRPHY_BIT_CKE_PH_EN                   ((u32)0x00000001 << 29)          /*!<R/W ‘h0  CKE output delay enable */
#define DDRPHY_CKE_PH_EN(x)                    ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_CS_1_PH_EN                  ((u32)0x00000001 << 28)          /*!<R/W ‘h0  CS_1 output delay enable */
#define DDRPHY_CS_1_PH_EN(x)                   ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_CS_PH_EN                    ((u32)0x00000001 << 27)          /*!<R/W ‘h0  CS output delay enable */
#define DDRPHY_CS_PH_EN(x)                     ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_BIT_CMD_PH_EN                   ((u32)0x00000001 << 26)          /*!<R/W ‘h0  Command output delay enable */
#define DDRPHY_CMD_PH_EN(x)                    ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_BIT_CKE_1_GEARDOWN              ((u32)0x00000001 << 24)          /*!<R/W ‘h0  0 : normal; 1 is gear down */
#define DDRPHY_CKE_1_GEARDOWN(x)               ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_CKE_GEARDOWN                ((u32)0x00000001 << 23)          /*!<R/W ‘h0  0 : normal; 1 is gear down */
#define DDRPHY_CKE_GEARDOWN(x)                 ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_CS_1_GEARDOWN               ((u32)0x00000001 << 22)          /*!<R/W ‘h0  0 : normal; 1 is gear down */
#define DDRPHY_CS_1_GEARDOWN(x)                ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_BIT_CS_GEARDOWN                 ((u32)0x00000001 << 21)          /*!<R/W ‘h0  0 : normal; 1 is gear down */
#define DDRPHY_CS_GEARDOWN(x)                  ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_CMD_GEARDOWN                ((u32)0x00000001 << 20)          /*!<R/W ‘h0  0 : normal; 1 is gear down */
#define DDRPHY_CMD_GEARDOWN(x)                 ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_MASK_CKE_1_PH_SEL               ((u32)0x00000007 << 16)          /*!<R/W ‘h0  CKE_1 output delay : 3’b000 : delay 1 cycle 3’b001 : delay 2 cycles 3’b010 : delay 3 cycles 3’b011 : delay 4 cycles …………… 3’b111 : delay 8 cycles */
#define DDRPHY_CKE_1_PH_SEL(x)                 ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_CKE_1_PH_SEL(x)             ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_CKE_PH_SEL                 ((u32)0x00000007 << 12)          /*!<R/W ‘h0  CKE output delay : 3’b000 : delay 1 cycle 3’b001 : delay 2 cycles 3’b010 : delay 3 cycles 3’b011 : delay 4 cycles …………… 3’b111 : delay 8 cycles */
#define DDRPHY_CKE_PH_SEL(x)                   ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CKE_PH_SEL(x)               ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CS_1_PH_SEL                ((u32)0x00000007 << 8)          /*!<R/W ‘h0  CS_1 output delay : 3’b000 : delay 1 cycle 3’b001 : delay 2 cycles 3’b010 : delay 3 cycles 3’b011 : delay 4 cycles …………… 3’b111 : delay 8 cycles */
#define DDRPHY_CS_1_PH_SEL(x)                  ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CS_1_PH_SEL(x)              ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_CS_PH_SEL                  ((u32)0x00000007 << 4)          /*!<R/W ‘h0  CS output delay : 3’b000 : delay 1 cycle 3’b001 : delay 2 cycles 3’b010 : delay 3 cycles 3’b011 : delay 4 cycles …………… 3’b111 : delay 8 cycles */
#define DDRPHY_CS_PH_SEL(x)                    ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CS_PH_SEL(x)                ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CMD_PH_SEL                 ((u32)0x00000007 << 0)          /*!<R/W ‘h0  Command output delay : 3’b000 : delay 1 cycle 3’b001 : delay 2 cycles 3’b010 : delay 3 cycles 3’b011 : delay 4 cycles …………… 3’b111 : delay 8 cycles */
#define DDRPHY_CMD_PH_SEL(x)                   ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CMD_PH_SEL(x)               ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_PAT_IN0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ_PAT_IN0                 ((u32)0xFFFFFFFF << 0)          /*!<R ’h0  Read data from DDR when dq_latch_en is high (dq_pat_in0 = 0 when dq_latch_en is low) [31] dq31 .............. [1] dq01 [0] dq00 */
#define DDRPHY_DQ_PAT_IN0(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_DQ_PAT_IN0(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_PAT_IN1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ_PAT_IN1                 ((u32)0x0000000F << 0)          /*!<R ’h0  Read data from DDR when dq_latch_en is high (dq_pat_in1 = 0 when dq_latch_en is low) [3] dbi3 [2] dbi2 [1] dbi1 [0] dbi0 */
#define DDRPHY_DQ_PAT_IN1(x)                   ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DQ_PAT_IN1(x)               ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_WRLVL_DQS_DATA
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_WRLVL_DQSDATA              ((u32)0x000000FF << 0)          /*!<R ’h0  Read data from DDR when ten_en is high (wrlvl_dqsdata = 0 when ten_en is low) [7] dqsb_3 [6] dqs_3 [5] dqsb_2 [4] dqs_2 [3] dqsb_1 [2] dqs_1 [1] dqsb_0 [0] dqs_0 */
#define DDRPHY_WRLVL_DQSDATA(x)                ((u32)(((x) & 0x000000FF) << 0))
#define DDRPHY_GET_WRLVL_DQSDATA(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_TEST_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_LBK_MODE                   ((u32)0x00000003 << 30)          /*!<R/W ’h0  Self-loopback mode [00] self-loopback mode disable [01] self-loopback bypass pad [10] self-loopback through pad [11] self-loopback dfi signals */
#define DDRPHY_LBK_MODE(x)                     ((u32)(((x) & 0x00000003) << 30))
#define DDRPHY_GET_LBK_MODE(x)                 ((u32)(((x >> 30) & 0x00000003)))
#define DDRPHY_BIT_LBK_SET                     ((u32)0x00000001 << 29)          /*!<R/W ‘h0  Set below registers for loopback test automatically: rd_dly_pos_sel_* = 5’h0, rd_dly_neg_sel_* = 5’h0, cal_set_mode = 2’b11, rst_fifo_mode = 2’b10, data_pre = 0 */
#define DDRPHY_LBK_SET(x)                      ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_DATA_EN_CHK                 ((u32)0x00000001 << 28)          /*!<R/W ‘h0  1: Check burst length of dfi_wr/rddata_en, and return check results to data_en_err_type. */
#define DDRPHY_DATA_EN_CHK(x)                  ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_TOP_DBG_SEL                ((u32)0x00000007 << 25)          /*!<R/W ’h0  Debug selection for top */
#define DDRPHY_TOP_DBG_SEL(x)                  ((u32)(((x) & 0x00000007) << 25))
#define DDRPHY_GET_TOP_DBG_SEL(x)              ((u32)(((x >> 25) & 0x00000007)))
#define DDRPHY_MASK_RW_DBG_SEL0                ((u32)0x00000003 << 20)          /*!<R/W ’h0  Debug selection for read / write slice top */
#define DDRPHY_RW_DBG_SEL0(x)                  ((u32)(((x) & 0x00000003) << 20))
#define DDRPHY_GET_RW_DBG_SEL0(x)              ((u32)(((x >> 20) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_TEST_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DCC_DSP_DBG_SEL             ((u32)0x00000001 << 26)          /*!<R/W ’h0  Debug selection for all DCC DSP */
#define DDRPHY_DCC_DSP_DBG_SEL(x)              ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_BIT_DPI_DBG_DLL_SEL             ((u32)0x00000001 << 24)          /*!<R/W ‘h0  Debug selection for dpi_debug_dll[63:0] to read-only register */
#define DDRPHY_DPI_DBG_DLL_SEL(x)              ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_MASK_DQSEN_DBG_SEL              ((u32)0x00000003 << 22)          /*!<R/W ‘h0  Debug selection for dqsen_dbg[31:0] to read-only register */
#define DDRPHY_DQSEN_DBG_SEL(x)                ((u32)(((x) & 0x00000003) << 22))
#define DDRPHY_GET_DQSEN_DBG_SEL(x)            ((u32)(((x >> 22) & 0x00000003)))
#define DDRPHY_MASK_RW_DBG_SEL1                ((u32)0x00000003 << 20)          /*!<R/W ’h0  Debug selection for read / write slice top */
#define DDRPHY_RW_DBG_SEL1(x)                  ((u32)(((x) & 0x00000003) << 20))
#define DDRPHY_GET_RW_DBG_SEL1(x)              ((u32)(((x >> 20) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_HALF_CYC_DET
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DET_CLK_ENABLE              ((u32)0x00000001 << 15)          /*!<R/W ‘h0  Half cycle delay tap detector clk enable. Need first enable before det_enable. */
#define DDRPHY_DET_CLK_ENABLE(x)               ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_BIT_DET_ENABLE                  ((u32)0x00000001 << 14)          /*!<R/W ’h0  Half cycle delay tap detector enable */
#define DDRPHY_DET_ENABLE(x)                   ((u32)(((x) & 0x00000001) << 14))
#define DDRPHY_BIT_DET_DONE                    ((u32)0x00000001 << 13)          /*!<R/W ’h0  Half cycle delay tap detector done */
#define DDRPHY_DET_DONE(x)                     ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_MASK_DET_RESULT                 ((u32)0x00001FFF << 0)          /*!<R ’h0  the number of delay tap */
#define DDRPHY_DET_RESULT(x)                   ((u32)(((x) & 0x00001FFF) << 0))
#define DDRPHY_GET_DET_RESULT(x)               ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_READ_CTRL_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_RX_CAL_DIS                  ((u32)0x00000001 << 31)          /*!<R/W ‘h0  1 :RX calibration disable Should set fw_set_rd_dly to active the function due to double-buffer design. In order to prevent the te/delta/vref setting from restoring to FW default value, please make sure the following registers should be set to 0 before triggering fw_set_rd_dly.. fw_dqsen_ftun_upd =0 (in rw_spec) fw_rd_delta_upd =0 (in rw_spec) fw_rd_te_upd =0 (in rw_spec) vr_upd =0 (in dll_spec) */
#define DDRPHY_RX_CAL_DIS(x)                   ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_RDDATA_EN_DLY_SEL          ((u32)0x00000003 << 28)          /*!<R/W ‘h0  Select dfi_rddata_en_p* delay cycles in DPI. 00: delay 0T 01: delay 4T 10: delay 6T 11: delay 8T */
#define DDRPHY_RDDATA_EN_DLY_SEL(x)            ((u32)(((x) & 0x00000003) << 28))
#define DDRPHY_GET_RDDATA_EN_DLY_SEL(x)        ((u32)(((x >> 28) & 0x00000003)))
#define DDRPHY_BIT_NCON_3T_MODE                ((u32)0x00000001 << 27)          /*!<R/W ’b0  for PUPD3 non-con 3T mode (ie, 4T margin for tm_dqs_en) */
#define DDRPHY_NCON_3T_MODE(x)                 ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_BIT_NCON_2T_MODE                ((u32)0x00000001 << 26)          /*!<R/W ’b0  for PUPD3 non-con 2T mode (ie, 3T margin for tm_dqs_en) */
#define DDRPHY_NCON_2T_MODE(x)                 ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_BIT_RD_DLY_FOLLOW_DQ0           ((u32)0x00000001 << 10)          /*!<R/W ‘h0  When this field is set to 1, FW can program read delay chain (dq31 ~ dq0) setting by dq0 (1800_006C ~ 1800_00C8) */
#define DDRPHY_RD_DLY_FOLLOW_DQ0(x)            ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_DQ_LAT_EN_FORCE_SEL         ((u32)0x00000001 << 9)          /*!<R/W ‘h0  dq latch control (in receive_1st & n/p_FIFO) 0 : controlled by read command 1: controlled by dq_lat_en_force_sig */
#define DDRPHY_DQ_LAT_EN_FORCE_SEL(x)          ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_DQ_LAT_EN_FORCE_SIG         ((u32)0x00000001 << 8)          /*!<R/W ‘h0  If set dq_lat_en_force_sel, dq latch enable is controlled by dq_lat_en_force_sig. */
#define DDRPHY_DQ_LAT_EN_FORCE_SIG(x)          ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_MASK_TM_RD_FIFO                 ((u32)0x0000003F << 0)          /*!<R/W ‘h2  read FIFO latency from read command 2T DCK unit for 2to1 mode 1T DCK unit for 1to1 mode */
#define DDRPHY_TM_RD_FIFO(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_TM_RD_FIFO(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_TX_RPT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_CKE_1_RPT_DEC               ((u32)0x00000001 << 27)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_CKE_1_RPT_DEC(x)                ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_BIT_CKE_1_RPT_ADD               ((u32)0x00000001 << 26)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_CKE_1_RPT_ADD(x)                ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_BIT_CKE_RPT_DEC                 ((u32)0x00000001 << 25)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_CKE_RPT_DEC(x)                  ((u32)(((x) & 0x00000001) << 25))
#define DDRPHY_BIT_CKE_RPT_ADD                 ((u32)0x00000001 << 24)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_CKE_RPT_ADD(x)                  ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_CS_1_RPT_DEC                ((u32)0x00000001 << 23)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_CS_1_RPT_DEC(x)                 ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_CS_1_RPT_ADD                ((u32)0x00000001 << 22)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_CS_1_RPT_ADD(x)                 ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_BIT_CS_RPT_DEC                  ((u32)0x00000001 << 21)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_CS_RPT_DEC(x)                   ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_CS_RPT_ADD                  ((u32)0x00000001 << 20)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_CS_RPT_ADD(x)                   ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_BIT_CA_RPT_DEC                  ((u32)0x00000001 << 19)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_CA_RPT_DEC(x)                   ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_CA_RPT_ADD                  ((u32)0x00000001 << 18)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_CA_RPT_ADD(x)                   ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_BIT_RX_RPT_DEC                  ((u32)0x00000001 << 17)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_RX_RPT_DEC(x)                   ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_RX_RPT_ADD                  ((u32)0x00000001 << 16)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_RX_RPT_ADD(x)                   ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_BIT_DQS_RPT_DEC_3               ((u32)0x00000001 << 15)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_DEC_3(x)                ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_BIT_DQS_RPT_ADD_3               ((u32)0x00000001 << 14)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_ADD_3(x)                ((u32)(((x) & 0x00000001) << 14))
#define DDRPHY_BIT_DQS_RPT_DEC_2               ((u32)0x00000001 << 13)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_DEC_2(x)                ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_DQS_RPT_ADD_2               ((u32)0x00000001 << 12)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_ADD_2(x)                ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_DQS_RPT_DEC_1               ((u32)0x00000001 << 11)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_DEC_1(x)                ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_BIT_DQS_RPT_ADD_1               ((u32)0x00000001 << 10)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_ADD_1(x)                ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_DQS_RPT_DEC_0               ((u32)0x00000001 << 9)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_DEC_0(x)                ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_DQS_RPT_ADD_0               ((u32)0x00000001 << 8)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQS_RPT_ADD_0(x)                ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_DQ_RPT_DEC_3                ((u32)0x00000001 << 7)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_DEC_3(x)                 ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_BIT_DQ_RPT_ADD_3                ((u32)0x00000001 << 6)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_ADD_3(x)                 ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_DQ_RPT_DEC_2                ((u32)0x00000001 << 5)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_DEC_2(x)                 ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_DQ_RPT_ADD_2                ((u32)0x00000001 << 4)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_ADD_2(x)                 ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_BIT_DQ_RPT_DEC_1                ((u32)0x00000001 << 3)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_DEC_1(x)                 ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_DQ_RPT_ADD_1                ((u32)0x00000001 << 2)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_ADD_1(x)                 ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_DQ_RPT_DEC_0                ((u32)0x00000001 << 1)          /*!<R/W ‘b0  set 1’b1 per time : delay 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_DEC_0(x)                 ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_DQ_RPT_ADD_0                ((u32)0x00000001 << 0)          /*!<R/W ‘b0  set 1’b1 per time : lead 1T (auto clear 0) */
#define DDRPHY_DQ_RPT_ADD_0(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CAL_SHIFT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CAL_LEAD3                  ((u32)0x00000007 << 28)          /*!<R/W ‘h2  Central point shift right control when 3-point calibration result is lead[3] = 1. */
#define DDRPHY_CAL_LEAD3(x)                    ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_CAL_LEAD3(x)                ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_CAL_LEAD2                  ((u32)0x00000007 << 24)          /*!<R/W ‘h2  Central point shift right control when 3-point calibration result is lead[2] = 1. */
#define DDRPHY_CAL_LEAD2(x)                    ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_CAL_LEAD2(x)                ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_CAL_LEAD1                  ((u32)0x00000007 << 20)          /*!<R/W ‘h1  Central point shift right control when 3-point calibration result is lead[1] = 1. */
#define DDRPHY_CAL_LEAD1(x)                    ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_CAL_LEAD1(x)                ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_CAL_LEAD0                  ((u32)0x00000007 << 16)          /*!<R/W ‘h1  Central point shift right control when 3-point calibration result is lead[0] = 1. [2’s complement format] (sign, 2.0) 000: 0 001: 1 010: 2 011: 3 [Note] lead[3] <- lead[2] <- lead[1] <- lead[0] <- central */
#define DDRPHY_CAL_LEAD0(x)                    ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_CAL_LEAD0(x)                ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_CAL_LAG3                   ((u32)0x00000007 << 12)          /*!<R/W ‘h6  Central point shift left control when 3-point calibration result is lag[3] = 1. */
#define DDRPHY_CAL_LAG3(x)                     ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CAL_LAG3(x)                 ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CAL_LAG2                   ((u32)0x00000007 << 8)          /*!<R/W ‘h6  Central point shift left control when 3-point calibration result is lag[2] = 1. */
#define DDRPHY_CAL_LAG2(x)                     ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CAL_LAG2(x)                 ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_CAL_LAG1                   ((u32)0x00000007 << 4)          /*!<R/W ‘h7  Central point shift left control when 3-point calibration result is lag[1] = 1. */
#define DDRPHY_CAL_LAG1(x)                     ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CAL_LAG1(x)                 ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CAL_LAG0                   ((u32)0x00000007 << 0)          /*!<R/W ‘h7  Central point shift left control when 3-point calibration result is lag[0] = 1. [2’s complement format] (sign, 2.0) 111: -1 110: -2 101: -3 100: -4 [Note] central -> lag[0] -> lag[1] -> lag[2] -> lag[3] */
#define DDRPHY_CAL_LAG0(x)                     ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CAL_LAG0(x)                 ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DT_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DTN_CLK                    ((u32)0x00000003 << 20)          /*!<R/W ‘h3  [0]: DTN of CLK, [1]: DTN of CLK_1 */
#define DDRPHY_DTN_CLK(x)                      ((u32)(((x) & 0x00000003) << 20))
#define DDRPHY_GET_DTN_CLK(x)                  ((u32)(((x >> 20) & 0x00000003)))
#define DDRPHY_MASK_DTP_CLK                    ((u32)0x00000003 << 16)          /*!<R/W ‘h0  [0]: DTP of CLK, [1]: DTP of CLK_1 */
#define DDRPHY_DTP_CLK(x)                      ((u32)(((x) & 0x00000003) << 16))
#define DDRPHY_GET_DTP_CLK(x)                  ((u32)(((x >> 16) & 0x00000003)))
#define DDRPHY_MASK_DTN_DQS                    ((u32)0x0000000F << 12)          /*!<R/W ‘hf  [0]: DTN of DQS0, [1]: DTN of DQS1, [2]: DTN of DQS2, [3]: DTN of DQS3 */
#define DDRPHY_DTN_DQS(x)                      ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_DTN_DQS(x)                  ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_DTP_DQS                    ((u32)0x0000000F << 8)          /*!<R/W ‘h0  [0]: DTP of DQS0, [1]: DTP of DQS1, [2]: DTP of DQS2, [3]: DTP of DQS3 */
#define DDRPHY_DTP_DQS(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DTP_DQS(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_DTN_DQ                     ((u32)0x0000000F << 4)          /*!<R/W ‘hf  [0]: DTN of DQ[7:0], [1]: DTN of DQ[15:8], [2]: DTN of DQ[23:16], [3]: DTN of DQ[31:24] */
#define DDRPHY_DTN_DQ(x)                       ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DTN_DQ(x)                   ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_DTP_DQ                     ((u32)0x0000000F << 0)          /*!<R/W ‘h0  [0]: DTP of DQ[7:0], [1]: DTP of DQ[15:8], [2]: DTP of DQ[23:16], [3]: DTP of DQ[31:24] [Note1] DTP: high-period input duty trimming [Note2] DTN: low-period input duty trimming */
#define DDRPHY_DTP_DQ(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DTP_DQ(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCP0_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCP_SET_3_PRE         ((u32)0x0000003F << 24)          /*!<R/W ’h0  P-ODT coarse trimming set_3. */
#define DDRPHY_ODT_TTCP_SET_3_PRE(x)           ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCP_SET_3_PRE(x)       ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_2_PRE         ((u32)0x0000003F << 16)          /*!<R/W ’h0  P-ODT coarse trimming set_2. */
#define DDRPHY_ODT_TTCP_SET_2_PRE(x)           ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCP_SET_2_PRE(x)       ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_1_PRE         ((u32)0x0000003F << 8)          /*!<R/W ’h0  P-ODT coarse trimming set_1. */
#define DDRPHY_ODT_TTCP_SET_1_PRE(x)           ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCP_SET_1_PRE(x)       ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_0_PRE         ((u32)0x0000003F << 0)          /*!<R/W ’h0  P-ODT coarse trimming set_0. */
#define DDRPHY_ODT_TTCP_SET_0_PRE(x)           ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCP_SET_0_PRE(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCP1_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCP_SET_7_PRE         ((u32)0x0000003F << 24)          /*!<R/W ’h0  P-ODT coarse trimming set_7. */
#define DDRPHY_ODT_TTCP_SET_7_PRE(x)           ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCP_SET_7_PRE(x)       ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_6_PRE         ((u32)0x0000003F << 16)          /*!<R/W ’h0  P-ODT coarse trimming set_6. */
#define DDRPHY_ODT_TTCP_SET_6_PRE(x)           ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCP_SET_6_PRE(x)       ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_5_PRE         ((u32)0x0000003F << 8)          /*!<R/W ’h0  P-ODT coarse trimming set_5. */
#define DDRPHY_ODT_TTCP_SET_5_PRE(x)           ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCP_SET_5_PRE(x)       ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_4_PRE         ((u32)0x0000003F << 0)          /*!<R/W ’h0  P-ODT coarse trimming set_4. */
#define DDRPHY_ODT_TTCP_SET_4_PRE(x)           ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCP_SET_4_PRE(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCN0_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCN_SET_3_PRE         ((u32)0x0000003F << 24)          /*!<R/W ’h0  N-ODT coarse trimming set_3. */
#define DDRPHY_ODT_TTCN_SET_3_PRE(x)           ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCN_SET_3_PRE(x)       ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_2_PRE         ((u32)0x0000003F << 16)          /*!<R/W ’h0  N-ODT coarse trimming set_2. */
#define DDRPHY_ODT_TTCN_SET_2_PRE(x)           ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCN_SET_2_PRE(x)       ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_1_PRE         ((u32)0x0000003F << 8)          /*!<R/W ’h0  N-ODT coarse trimming set_1. */
#define DDRPHY_ODT_TTCN_SET_1_PRE(x)           ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCN_SET_1_PRE(x)       ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_0_PRE         ((u32)0x0000003F << 0)          /*!<R/W ’h0  N-ODT coarse trimming set_0. */
#define DDRPHY_ODT_TTCN_SET_0_PRE(x)           ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCN_SET_0_PRE(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCN1_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCN_SET_7_PRE         ((u32)0x0000003F << 24)          /*!<R/W ’h0  N-ODT coarse trimming set_7. */
#define DDRPHY_ODT_TTCN_SET_7_PRE(x)           ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCN_SET_7_PRE(x)       ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_6_PRE         ((u32)0x0000003F << 16)          /*!<R/W ’h0  N-ODT coarse trimming set_6. */
#define DDRPHY_ODT_TTCN_SET_6_PRE(x)           ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCN_SET_6_PRE(x)       ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_5_PRE         ((u32)0x0000003F << 8)          /*!<R/W ’h0  N-ODT coarse trimming set_5. */
#define DDRPHY_ODT_TTCN_SET_5_PRE(x)           ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCN_SET_5_PRE(x)       ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_4_PRE         ((u32)0x0000003F << 0)          /*!<R/W ’h0  N-ODT coarse trimming set_4. */
#define DDRPHY_ODT_TTCN_SET_4_PRE(x)           ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCN_SET_4_PRE(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDP0_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDP_SET_3_PRE             ((u32)0x0000003F << 24)          /*!<R/W ’h26  P-OCD trimming set_3. */
#define DDRPHY_OCDP_SET_3_PRE(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDP_SET_3_PRE(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_2_PRE             ((u32)0x0000003F << 16)          /*!<R/W ’h26  P-OCD trimming set_2. */
#define DDRPHY_OCDP_SET_2_PRE(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDP_SET_2_PRE(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_1_PRE             ((u32)0x0000003F << 8)          /*!<R/W ’h26  P-OCD trimming set_1. */
#define DDRPHY_OCDP_SET_1_PRE(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDP_SET_1_PRE(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_0_PRE             ((u32)0x0000003F << 0)          /*!<R/W ’h26  P-OCD trimming set_0. */
#define DDRPHY_OCDP_SET_0_PRE(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDP_SET_0_PRE(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDP1_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDP_SET_7_PRE             ((u32)0x0000003F << 24)          /*!<R/W ’h26  P-OCD trimming set_7. */
#define DDRPHY_OCDP_SET_7_PRE(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDP_SET_7_PRE(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_6_PRE             ((u32)0x0000003F << 16)          /*!<R/W ’h26  P-OCD trimming set_6. */
#define DDRPHY_OCDP_SET_6_PRE(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDP_SET_6_PRE(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_5_PRE             ((u32)0x0000003F << 8)          /*!<R/W ’h26  P-OCD trimming set_5. */
#define DDRPHY_OCDP_SET_5_PRE(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDP_SET_5_PRE(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_4_PRE             ((u32)0x0000003F << 0)          /*!<R/W ’h26  P-OCD trimming set_4. */
#define DDRPHY_OCDP_SET_4_PRE(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDP_SET_4_PRE(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDN0_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDN_SET_3_PRE             ((u32)0x0000003F << 24)          /*!<R/W ’h26  N-OCD trimming set_3. */
#define DDRPHY_OCDN_SET_3_PRE(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDN_SET_3_PRE(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_2_PRE             ((u32)0x0000003F << 16)          /*!<R/W ’h26  N-OCD trimming set_2. */
#define DDRPHY_OCDN_SET_2_PRE(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDN_SET_2_PRE(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_1_PRE             ((u32)0x0000003F << 8)          /*!<R/W ’h26  N-OCD trimming set_1. */
#define DDRPHY_OCDN_SET_1_PRE(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDN_SET_1_PRE(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_0_PRE             ((u32)0x0000003F << 0)          /*!<R/W ’h26  N-OCD trimming set_0. */
#define DDRPHY_OCDN_SET_0_PRE(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDN_SET_0_PRE(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDN1_SET0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDN_SET_7_PRE             ((u32)0x0000003F << 24)          /*!<R/W ’h26  N-OCD trimming set_7. */
#define DDRPHY_OCDN_SET_7_PRE(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDN_SET_7_PRE(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_6_PRE             ((u32)0x0000003F << 16)          /*!<R/W ’h26  N-OCD trimming set_6. */
#define DDRPHY_OCDN_SET_6_PRE(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDN_SET_6_PRE(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_5_PRE             ((u32)0x0000003F << 8)          /*!<R/W ’h26  N-OCD trimming set_5. */
#define DDRPHY_OCDN_SET_5_PRE(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDN_SET_5_PRE(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_4_PRE             ((u32)0x0000003F << 0)          /*!<R/W ’h26  N-OCD trimming set_4. */
#define DDRPHY_OCDN_SET_4_PRE(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDN_SET_4_PRE(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCP0_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCP_SET_3             ((u32)0x0000003F << 24)          /*!<R/W ’h0  P-ODT coarse trimming set_3. */
#define DDRPHY_ODT_TTCP_SET_3(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCP_SET_3(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_2             ((u32)0x0000003F << 16)          /*!<R/W ’h0  P-ODT coarse trimming set_2. */
#define DDRPHY_ODT_TTCP_SET_2(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCP_SET_2(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_1             ((u32)0x0000003F << 8)          /*!<R/W ’h0  P-ODT coarse trimming set_1. */
#define DDRPHY_ODT_TTCP_SET_1(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCP_SET_1(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_0             ((u32)0x0000003F << 0)          /*!<R/W ’h0  P-ODT coarse trimming set_0. */
#define DDRPHY_ODT_TTCP_SET_0(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCP_SET_0(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCP1_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCP_SET_7             ((u32)0x0000003F << 24)          /*!<R/W ’h0  P-ODT coarse trimming set_7. */
#define DDRPHY_ODT_TTCP_SET_7(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCP_SET_7(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_6             ((u32)0x0000003F << 16)          /*!<R/W ’h0  P-ODT coarse trimming set_6. */
#define DDRPHY_ODT_TTCP_SET_6(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCP_SET_6(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_5             ((u32)0x0000003F << 8)          /*!<R/W ’h0  P-ODT coarse trimming set_5. */
#define DDRPHY_ODT_TTCP_SET_5(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCP_SET_5(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCP_SET_4             ((u32)0x0000003F << 0)          /*!<R/W ’h0  P-ODT coarse trimming set_4. */
#define DDRPHY_ODT_TTCP_SET_4(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCP_SET_4(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCN0_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCN_SET_3             ((u32)0x0000003F << 24)          /*!<R/W ’h0  N-ODT coarse trimming set_3. */
#define DDRPHY_ODT_TTCN_SET_3(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCN_SET_3(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_2             ((u32)0x0000003F << 16)          /*!<R/W ’h0  N-ODT coarse trimming set_2. */
#define DDRPHY_ODT_TTCN_SET_2(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCN_SET_2(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_1             ((u32)0x0000003F << 8)          /*!<R/W ’h0  N-ODT coarse trimming set_1. */
#define DDRPHY_ODT_TTCN_SET_1(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCN_SET_1(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_0             ((u32)0x0000003F << 0)          /*!<R/W ’h0  N-ODT coarse trimming set_0. */
#define DDRPHY_ODT_TTCN_SET_0(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCN_SET_0(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_TTCN1_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ODT_TTCN_SET_7             ((u32)0x0000003F << 24)          /*!<R/W ’h0  N-ODT coarse trimming set_7. */
#define DDRPHY_ODT_TTCN_SET_7(x)               ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_ODT_TTCN_SET_7(x)           ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_6             ((u32)0x0000003F << 16)          /*!<R/W ’h0  N-ODT coarse trimming set_6. */
#define DDRPHY_ODT_TTCN_SET_6(x)               ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ODT_TTCN_SET_6(x)           ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_5             ((u32)0x0000003F << 8)          /*!<R/W ’h0  N-ODT coarse trimming set_5. */
#define DDRPHY_ODT_TTCN_SET_5(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_ODT_TTCN_SET_5(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_ODT_TTCN_SET_4             ((u32)0x0000003F << 0)          /*!<R/W ’h0  N-ODT coarse trimming set_4. */
#define DDRPHY_ODT_TTCN_SET_4(x)               ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ODT_TTCN_SET_4(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDP0_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDP_SET_3                 ((u32)0x0000003F << 24)          /*!<R/W ’h26  P-OCD trimming set_3. */
#define DDRPHY_OCDP_SET_3(x)                   ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDP_SET_3(x)               ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_2                 ((u32)0x0000003F << 16)          /*!<R/W ’h26  P-OCD trimming set_2. */
#define DDRPHY_OCDP_SET_2(x)                   ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDP_SET_2(x)               ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_1                 ((u32)0x0000003F << 8)          /*!<R/W ’h26  P-OCD trimming set_1. */
#define DDRPHY_OCDP_SET_1(x)                   ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDP_SET_1(x)               ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_0                 ((u32)0x0000003F << 0)          /*!<R/W ’h26  P-OCD trimming set_0. */
#define DDRPHY_OCDP_SET_0(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDP_SET_0(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDP1_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDP_SET_7                 ((u32)0x0000003F << 24)          /*!<R/W ’h26  P-OCD trimming set_7. */
#define DDRPHY_OCDP_SET_7(x)                   ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDP_SET_7(x)               ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_6                 ((u32)0x0000003F << 16)          /*!<R/W ’h26  P-OCD trimming set_6. */
#define DDRPHY_OCDP_SET_6(x)                   ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDP_SET_6(x)               ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_5                 ((u32)0x0000003F << 8)          /*!<R/W ’h26  P-OCD trimming set_5. */
#define DDRPHY_OCDP_SET_5(x)                   ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDP_SET_5(x)               ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDP_SET_4                 ((u32)0x0000003F << 0)          /*!<R/W ’h26  P-OCD trimming set_4. */
#define DDRPHY_OCDP_SET_4(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDP_SET_4(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDN0_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDN_SET_3                 ((u32)0x0000003F << 24)          /*!<R/W ’h26  N-OCD trimming set_3. */
#define DDRPHY_OCDN_SET_3(x)                   ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDN_SET_3(x)               ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_2                 ((u32)0x0000003F << 16)          /*!<R/W ’h26  N-OCD trimming set_2. */
#define DDRPHY_OCDN_SET_2(x)                   ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDN_SET_2(x)               ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_1                 ((u32)0x0000003F << 8)          /*!<R/W ’h26  N-OCD trimming set_1. */
#define DDRPHY_OCDN_SET_1(x)                   ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDN_SET_1(x)               ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_0                 ((u32)0x0000003F << 0)          /*!<R/W ’h26  N-OCD trimming set_0. */
#define DDRPHY_OCDN_SET_0(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDN_SET_0(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCDN1_SET1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_OCDN_SET_7                 ((u32)0x0000003F << 24)          /*!<R/W ’h26  N-OCD trimming set_7. */
#define DDRPHY_OCDN_SET_7(x)                   ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_OCDN_SET_7(x)               ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_6                 ((u32)0x0000003F << 16)          /*!<R/W ’h26  N-OCD trimming set_6. */
#define DDRPHY_OCDN_SET_6(x)                   ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_OCDN_SET_6(x)               ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_5                 ((u32)0x0000003F << 8)          /*!<R/W ’h26  N-OCD trimming set_5. */
#define DDRPHY_OCDN_SET_5(x)                   ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_OCDN_SET_5(x)               ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_OCDN_SET_4                 ((u32)0x0000003F << 0)          /*!<R/W ’h26  N-OCD trimming set_4. */
#define DDRPHY_OCDN_SET_4(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_OCDN_SET_4(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_ODT_SEL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ_1_TTCN_SEL              ((u32)0x00000007 << 12)          /*!<R/W ’h0  N-ODT coarse tune selection for slice1 / slice3 DQ 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_DQ_1_TTCN_SEL(x)                ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQ_1_TTCN_SEL(x)            ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQ_0_TTCN_SEL              ((u32)0x00000007 << 8)          /*!<R/W ’h0  N-ODT coarse tune selection for slice0 / slice2 DQ 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_DQ_0_TTCN_SEL(x)                ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQ_0_TTCN_SEL(x)            ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQ_1_TTCP_SEL              ((u32)0x00000007 << 4)          /*!<R/W ’h0  P-ODT coarse tune selection for slice1 / slice3 DQ 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_DQ_1_TTCP_SEL(x)                ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQ_1_TTCP_SEL(x)            ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQ_0_TTCP_SEL              ((u32)0x00000007 << 0)          /*!<R/W ’h0  P-ODT coarse tune selection for slice0 / slice2 DQ 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_DQ_0_TTCP_SEL(x)                ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQ_0_TTCP_SEL(x)            ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_OCD_SEL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ_1_OCDN_SEL              ((u32)0x00000007 << 12)          /*!<R/W ’h0  N-OCD selection for slice1 / slice3 DQ 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQ_1_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQ_1_OCDN_SEL(x)            ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQ_0_OCDN_SEL              ((u32)0x00000007 << 8)          /*!<R/W ’h0  N-OCD selection for slice0 / slice2 DQ 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQ_0_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQ_0_OCDN_SEL(x)            ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQ_1_OCDP_SEL              ((u32)0x00000007 << 4)          /*!<R/W ’h0  P-OCD selection for slice1 / slice3 DQ 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQ_1_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQ_1_OCDP_SEL(x)            ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQ_0_OCDP_SEL              ((u32)0x00000007 << 0)          /*!<R/W ’h0  P-OCD selection for slice0 / slice2 DQ 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQ_0_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQ_0_OCDP_SEL(x)            ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_P_ODT_SEL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_P_1_TTCN_SEL           ((u32)0x00000007 << 12)          /*!<R/W ’h0  N-ODT coarse tune selection for DQS[1] / DQS[3] 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_DQS_P_1_TTCN_SEL(x)             ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQS_P_1_TTCN_SEL(x)         ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQS_P_0_TTCN_SEL           ((u32)0x00000007 << 8)          /*!<R/W ’h0  N-ODT coarse tune selection for DQS[0] / DQS[2] 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_DQS_P_0_TTCN_SEL(x)             ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQS_P_0_TTCN_SEL(x)         ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQS_P_1_TTCP_SEL           ((u32)0x00000007 << 4)          /*!<R/W ’h0  P-ODT coarse tune selection for DQS[1] / DQS[3] 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_DQS_P_1_TTCP_SEL(x)             ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQS_P_1_TTCP_SEL(x)         ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQS_P_0_TTCP_SEL           ((u32)0x00000007 << 0)          /*!<R/W ’h0  P-ODT coarse tune selection for DQS[0] / DQS[2] 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_DQS_P_0_TTCP_SEL(x)             ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQS_P_0_TTCP_SEL(x)         ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_N_ODT_SEL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_N_1_TTCN_SEL           ((u32)0x00000007 << 12)          /*!<R/W ’h0  N-ODT coarse tune selection for DQS#[1] / DQS#[3] 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_DQS_N_1_TTCN_SEL(x)             ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQS_N_1_TTCN_SEL(x)         ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQS_N_0_TTCN_SEL           ((u32)0x00000007 << 8)          /*!<R/W ’h0  N-ODT coarse tune selection for DQS#[0] / DQS#[2] 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_DQS_N_0_TTCN_SEL(x)             ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQS_N_0_TTCN_SEL(x)         ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQS_N_1_TTCP_SEL           ((u32)0x00000007 << 4)          /*!<R/W ’h0  P-ODT coarse tune selection for DQS#[1] / DQS#[3] 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_DQS_N_1_TTCP_SEL(x)             ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQS_N_1_TTCP_SEL(x)         ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQS_N_0_TTCP_SEL           ((u32)0x00000007 << 0)          /*!<R/W ’h0  P-ODT coarse tune selection for DQS#[0] / DQS#[2] 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_DQS_N_0_TTCP_SEL(x)             ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQS_N_0_TTCP_SEL(x)         ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_OCD_SEL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_N_1_OCDN_SEL           ((u32)0x00000007 << 28)          /*!<R/W ’h0  N-OCD selection for DQS#[1] / DQS#[3] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_N_1_OCDN_SEL(x)             ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_DQS_N_1_OCDN_SEL(x)         ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_DQS_N_0_OCDN_SEL           ((u32)0x00000007 << 24)          /*!<R/W ’h0  N-OCD selection for DQS#[0] / DQS#[2] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_N_0_OCDN_SEL(x)             ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DQS_N_0_OCDN_SEL(x)         ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DQS_N_1_OCDP_SEL           ((u32)0x00000007 << 20)          /*!<R/W ’h0  P-OCD selection for DQS#[1] / DQS#[3] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_N_1_OCDP_SEL(x)             ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_DQS_N_1_OCDP_SEL(x)         ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_DQS_N_0_OCDP_SEL           ((u32)0x00000007 << 16)          /*!<R/W ’h0  P-OCD selection for DQS#[0] / DQS#[2] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_N_0_OCDP_SEL(x)             ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_DQS_N_0_OCDP_SEL(x)         ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_DQS_P_1_OCDN_SEL           ((u32)0x00000007 << 12)          /*!<R/W ’h0  N-OCD selection for DQS[1] / DQS[3] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_P_1_OCDN_SEL(x)             ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQS_P_1_OCDN_SEL(x)         ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQS_P_0_OCDN_SEL           ((u32)0x00000007 << 8)          /*!<R/W ’h0  N-OCD selection for DQS[0] / DQS[2] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_P_0_OCDN_SEL(x)             ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQS_P_0_OCDN_SEL(x)         ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQS_P_1_OCDP_SEL           ((u32)0x00000007 << 4)          /*!<R/W ’h0  P-OCD selection for DQS[1] / DQS[3] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_P_1_OCDP_SEL(x)             ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQS_P_1_OCDP_SEL(x)         ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQS_P_0_OCDP_SEL           ((u32)0x00000007 << 0)          /*!<R/W ’h0  P-OCD selection for DQS[0] / DQS[2] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_P_0_OCDP_SEL(x)             ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQS_P_0_OCDP_SEL(x)         ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CKE_OCD_SEL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CKE_1_OCDN_SEL             ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for CKE_1 (no use) 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CKE_1_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CKE_1_OCDN_SEL(x)           ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CKE_1_OCDP_SEL             ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for CKE_1 (no use) 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CKE_1_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CKE_1_OCDP_SEL(x)           ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_CKE_OCDN_SEL               ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for CKE 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CKE_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CKE_OCDN_SEL(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CKE_OCDP_SEL               ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for CKE 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CKE_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CKE_OCDP_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_OCD_SEL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CS_1_OCDN_SEL              ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for CS_1 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CS_1_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_CS_1_OCDN_SEL(x)            ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_CS_1_OCDP_SEL              ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for CS_1 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CS_1_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_CS_1_OCDP_SEL(x)            ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_CS_OCDN_SEL                ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for CS 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CS_OCDN_SEL(x)                  ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CS_OCDN_SEL(x)              ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CS_OCDP_SEL                ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for CS 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CS_OCDP_SEL(x)                  ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CS_OCDP_SEL(x)              ((u32)(((x >> 8) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CK_OCD_SEL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CK_N_1_OCDN_SEL            ((u32)0x00000007 << 28)          /*!<R/W ’h2  N-OCD selection for DCK#[1] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CK_N_1_OCDN_SEL(x)              ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_CK_N_1_OCDN_SEL(x)          ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_CK_N_0_OCDN_SEL            ((u32)0x00000007 << 24)          /*!<R/W ’h2  N-OCD selection for DCK#[0] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CK_N_0_OCDN_SEL(x)              ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_CK_N_0_OCDN_SEL(x)          ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_CK_N_1_OCDP_SEL            ((u32)0x00000007 << 20)          /*!<R/W ’h2  P-OCD selection for DCK#[1] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CK_N_1_OCDP_SEL(x)              ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_CK_N_1_OCDP_SEL(x)          ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_CK_N_0_OCDP_SEL            ((u32)0x00000007 << 16)          /*!<R/W ’h2  P-OCD selection for DCK#[0] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CK_N_0_OCDP_SEL(x)              ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_CK_N_0_OCDP_SEL(x)          ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_CK_P_1_OCDN_SEL            ((u32)0x00000007 << 12)          /*!<R/W ’h2  N-OCD selection for DCK[1] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CK_P_1_OCDN_SEL(x)              ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CK_P_1_OCDN_SEL(x)          ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CK_P_0_OCDN_SEL            ((u32)0x00000007 << 8)          /*!<R/W ’h2  N-OCD selection for DCK[0] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CK_P_0_OCDN_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CK_P_0_OCDN_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_CK_P_1_OCDP_SEL            ((u32)0x00000007 << 4)          /*!<R/W ’h2  P-OCD selection for DCK[1] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CK_P_1_OCDP_SEL(x)              ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CK_P_1_OCDP_SEL(x)          ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CK_P_0_OCDP_SEL            ((u32)0x00000007 << 0)          /*!<R/W ’h2  P-OCD selection for DCK[0] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CK_P_0_OCDP_SEL(x)              ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CK_P_0_OCDP_SEL(x)          ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_BUS_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_EN_DCC                      ((u32)0x00000001 << 31)          /*!<R/W ‘h1  enable O_DCC output of CLK pad */
#define DDRPHY_EN_DCC(x)                       ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_PD_CK                       ((u32)0x00000001 << 30)          /*!<R/W ‘h0  Rx power down of CLK pad */
#define DDRPHY_PD_CK(x)                        ((u32)(((x) & 0x00000001) << 30))
#define DDRPHY_BIT_EN_DMYI                     ((u32)0x00000001 << 29)          /*!<R/W ‘h0  enable dummy current of of REFZQ pad */
#define DDRPHY_EN_DMYI(x)                      ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_VREF_SEL                    ((u32)0x00000001 << 28)          /*!<R/W ‘h0  VREF SEL for VREFZQ pad When REFZQ.VREF_SEL = 1’b1,use resistor voltage divider to calibrate R480Ohm, when vref_sel = 1’b0, use bandgap voltage divider to calibrate R480 (Useless in RLE1080) */
#define DDRPHY_VREF_SEL(x)                     ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_VREF_RANGE                  ((u32)0x00000001 << 27)          /*!<R/W ‘h1  VREF VREF_RANGE for VREF_CA pad 1’b1: 44% - 59% 1’b0: 38% – 53% */
#define DDRPHY_VREF_RANGE(x)                   ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_MASK_VREF_S                     ((u32)0x0000003F << 21)          /*!<R/W ‘hA  VREF S for VREF_CA pad Control Vref voltage with vref_range, and add 1% voltage when add 1 level */
#define DDRPHY_VREF_S(x)                       ((u32)(((x) & 0x0000003F) << 21))
#define DDRPHY_GET_VREF_S(x)                   ((u32)(((x >> 21) & 0x0000003F)))
#define DDRPHY_MASK_VREF_CURRENTSEL            ((u32)0x00000003 << 19)          /*!<R/W ‘h3  Connect to CLKPG.PEAKING_EN */
#define DDRPHY_VREF_CURRENTSEL(x)              ((u32)(((x) & 0x00000003) << 19))
#define DDRPHY_GET_VREF_CURRENTSEL(x)          ((u32)(((x >> 19) & 0x00000003)))
#define DDRPHY_BIT_VREF_PD                     ((u32)0x00000001 << 18)          /*!<R/W ‘h0  VREF pd for VREF_CA pad Power down Vref. zq_pd = 1 when use external Vref, zq_pad = 0 when use internal Vref */
#define DDRPHY_VREF_PD(x)                      ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_MASK_PAD_BUS_0                  ((u32)0x0003FFFF << 0)          /*!<R/W ’h0  [17:16]: Rx IBIAS_SEL for DQ/DQS/CLK pads [15:14]: No use [13:12]: No use [11:9]: slice3 DQS current selection for PUPD4 [8:6]: slice2 DQS current selection for PUPD4 [5:3]: slice1 DQS current selection for PUPD4 [2:0]: slice0 DQS current selection for PUPD4 */
#define DDRPHY_PAD_BUS_0(x)                    ((u32)(((x) & 0x0003FFFF) << 0))
#define DDRPHY_GET_PAD_BUS_0(x)                ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_BUS_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_PD_CK_1                     ((u32)0x00000001 << 31)          /*!<R/W ‘h0  Rx power down of CLK_1 pad */
#define DDRPHY_PD_CK_1(x)                      ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_VREF_RANGE_4                ((u32)0x00000001 << 15)          /*!<R/W ‘h1  VREF VREF_RANGE for normal Command/Address(CA) VREF pad 1’b1: 0.5% - 64.5% 1’b0: 28% – 92% (Useless in RLE1080) */
#define DDRPHY_VREF_RANGE_4(x)                 ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_MASK_VREF_S_4                   ((u32)0x0000003F << 9)          /*!<R/W ‘hA  VREF S for normal CA VREF pad Control Vref voltage with vref_range, and add 1% voltage when add 1 level (Useless in RLE1080) */
#define DDRPHY_VREF_S_4(x)                     ((u32)(((x) & 0x0000003F) << 9))
#define DDRPHY_GET_VREF_S_4(x)                 ((u32)(((x >> 9) & 0x0000003F)))
#define DDRPHY_BIT_VREF_PD_4                   ((u32)0x00000001 << 8)          /*!<R/W ‘h0  VREF pd for normal CA VREF pad (Useless in RLE1080) */
#define DDRPHY_VREF_PD_4(x)                    ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_ZQ_POWER_DOWN               ((u32)0x00000001 << 6)          /*!<R/W ‘h0  ZQ power down */
#define DDRPHY_ZQ_POWER_DOWN(x)                ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_MASK_PAD_BUS_1                  ((u32)0x0000003F << 0)          /*!<R/W ’h0  [3:0]: No use [4]: No Use [5]: No Use */
#define DDRPHY_PAD_BUS_1(x)                    ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_PAD_BUS_1(x)                ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_BUS_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_VREF_RANGE_3                ((u32)0x00000001 << 31)          /*!<R/W ‘h1  VREF VREF_RANGE for normal byte-3 VREF pad 1’b1: 0.5% - 64.5% 1’b0: 28% – 92% (Useless in RLE1080) */
#define DDRPHY_VREF_RANGE_3(x)                 ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_VREF_S_3                   ((u32)0x0000003F << 25)          /*!<R/W ‘hA  VREF S for normal byte-3 VREF pad Control Vref voltage with vref_range, and add 1% voltage when add 1 level (Useless in RLE1080) */
#define DDRPHY_VREF_S_3(x)                     ((u32)(((x) & 0x0000003F) << 25))
#define DDRPHY_GET_VREF_S_3(x)                 ((u32)(((x >> 25) & 0x0000003F)))
#define DDRPHY_BIT_VREF_PD_3                   ((u32)0x00000001 << 24)          /*!<R/W ‘h0  VREF pd for normal byte-3 VREF pad */
#define DDRPHY_VREF_PD_3(x)                    ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_VREF_RANGE_2                ((u32)0x00000001 << 23)          /*!<R/W ‘h1  VREF VREF_RANGE for normal byte-2 VREF pad 1’b1: 0.5% - 64.5% 1’b0: 28% – 92% (Useless in RLE1080) */
#define DDRPHY_VREF_RANGE_2(x)                 ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_MASK_VREF_S_2                   ((u32)0x0000003F << 17)          /*!<R/W ‘hA  VREF S for normal byte-2 VREF pad Control Vref voltage with vref_range, and add 1% voltage when add 1 level (Useless in RLE1080) */
#define DDRPHY_VREF_S_2(x)                     ((u32)(((x) & 0x0000003F) << 17))
#define DDRPHY_GET_VREF_S_2(x)                 ((u32)(((x >> 17) & 0x0000003F)))
#define DDRPHY_BIT_VREF_PD_2                   ((u32)0x00000001 << 16)          /*!<R/W ‘h0  VREF pd for normal byte-2 VREF pad */
#define DDRPHY_VREF_PD_2(x)                    ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_BIT_VREF_RANGE_1                ((u32)0x00000001 << 15)          /*!<R/W ‘h1  VREF VREF_RANGE for normal byte-1 VREF pad 1’b1: 0.5% - 64.5% 1’b0: 28% – 92% (Useless in RLE1080) */
#define DDRPHY_VREF_RANGE_1(x)                 ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_MASK_VREF_S_1                   ((u32)0x0000003F << 9)          /*!<R/W ‘hA  VREF S for normal byte-1 VREF pad Control Vref voltage with vref_range, and add 1% voltage when add 1 level (Useless in RLE1080) */
#define DDRPHY_VREF_S_1(x)                     ((u32)(((x) & 0x0000003F) << 9))
#define DDRPHY_GET_VREF_S_1(x)                 ((u32)(((x >> 9) & 0x0000003F)))
#define DDRPHY_BIT_VREF_PD_1                   ((u32)0x00000001 << 8)          /*!<R/W ‘h0  VREF pd for normal byte-1 VREF pad */
#define DDRPHY_VREF_PD_1(x)                    ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_VREF_RANGE_0                ((u32)0x00000001 << 7)          /*!<R/W ‘h1  VREF VREF_RANGE for normal byte-0 VREF pad 1’b1: 44% - 59% 1’b0: 38% – 53% */
#define DDRPHY_VREF_RANGE_0(x)                 ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_MASK_VREF_S_0                   ((u32)0x0000003F << 1)          /*!<R/W ‘hA  VREF S for normal byte-0 VREF pad Control Vref voltage with vref_range, and add 1% voltage when add 1 level */
#define DDRPHY_VREF_S_0(x)                     ((u32)(((x) & 0x0000003F) << 1))
#define DDRPHY_GET_VREF_S_0(x)                 ((u32)(((x >> 1) & 0x0000003F)))
#define DDRPHY_BIT_VREF_PD_0                   ((u32)0x00000001 << 0)          /*!<R/W ‘h0  VREF pd for normal byte-0 VREF pad */
#define DDRPHY_VREF_PD_0(x)                    ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ZQ_TRIM
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_R480_TRIM                   ((u32)0x00000001 << 0)          /*!<R/W ‘h0  control R480 calibration trim 1’b1 : r480 result -1 */
#define DDRPHY_R480_TRIM(x)                    ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR00_OCDN_SEL             ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for A0 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR00_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ADR00_OCDN_SEL(x)           ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ADR00_OCDP_SEL             ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for A0 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR00_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ADR00_OCDP_SEL(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ADR01_OCDN_SEL             ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for A1 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR01_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ADR01_OCDN_SEL(x)           ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR01_OCDP_SEL             ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for A1 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR01_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR01_OCDP_SEL(x)           ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR02_OCDN_SEL             ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for A2 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR02_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR02_OCDN_SEL(x)           ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_ADR02_OCDP_SEL             ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for A2 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR02_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR02_OCDP_SEL(x)           ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ADR03_OCDN_SEL             ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for A3 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR03_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_ADR03_OCDN_SEL(x)           ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_ADR03_OCDP_SEL             ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for A3 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR03_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_ADR03_OCDP_SEL(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR04_OCDN_SEL             ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for A4 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR04_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ADR04_OCDN_SEL(x)           ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ADR04_OCDP_SEL             ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for A4 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR04_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ADR04_OCDP_SEL(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ADR05_OCDN_SEL             ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for A5 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR05_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ADR05_OCDN_SEL(x)           ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR05_OCDP_SEL             ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for A5 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR05_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR05_OCDP_SEL(x)           ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR06_OCDN_SEL             ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for A6 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR06_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR06_OCDN_SEL(x)           ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_ADR06_OCDP_SEL             ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for A6 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR06_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR06_OCDP_SEL(x)           ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ADR07_OCDN_SEL             ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for A7 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR07_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_ADR07_OCDN_SEL(x)           ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_ADR07_OCDP_SEL             ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for A7 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR07_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_ADR07_OCDP_SEL(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR08_OCDN_SEL             ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for A8 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR08_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ADR08_OCDN_SEL(x)           ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ADR08_OCDP_SEL             ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for A8 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR08_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ADR08_OCDP_SEL(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ADR09_OCDN_SEL             ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for A9 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR09_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ADR09_OCDN_SEL(x)           ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR09_OCDP_SEL             ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for A9 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR09_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR09_OCDP_SEL(x)           ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR10_OCDN_SEL             ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for A10 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR10_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR10_OCDN_SEL(x)           ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_ADR10_OCDP_SEL             ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for A10 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR10_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR10_OCDP_SEL(x)           ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ADR11_OCDN_SEL             ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for A11 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR11_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_ADR11_OCDN_SEL(x)           ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_ADR11_OCDP_SEL             ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for A11 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR11_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_ADR11_OCDP_SEL(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_3
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR12_OCDN_SEL             ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for A12 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR12_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ADR12_OCDN_SEL(x)           ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ADR12_OCDP_SEL             ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for A12 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR12_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ADR12_OCDP_SEL(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ADR13_OCDN_SEL             ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for A13 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR13_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ADR13_OCDN_SEL(x)           ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR13_OCDP_SEL             ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for A13 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR13_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR13_OCDP_SEL(x)           ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR14_OCDN_SEL             ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for A14 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR14_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR14_OCDN_SEL(x)           ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_ADR14_OCDP_SEL             ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for A14 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR14_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR14_OCDP_SEL(x)           ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ADR15_OCDN_SEL             ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for A15 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR15_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_ADR15_OCDN_SEL(x)           ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_ADR15_OCDP_SEL             ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for A15 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR15_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_ADR15_OCDP_SEL(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_4
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR16_OCDN_SEL             ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for A16 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR16_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ADR16_OCDN_SEL(x)           ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ADR16_OCDP_SEL             ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for A16 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR16_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ADR16_OCDP_SEL(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ADR_EXT_4_OCDN_SEL         ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for AD1_4 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR_EXT_4_OCDN_SEL(x)           ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ADR_EXT_4_OCDN_SEL(x)       ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR_EXT_4_OCDP_SEL         ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for AD1_4 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR_EXT_4_OCDP_SEL(x)           ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR_EXT_4_OCDP_SEL(x)       ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR_EXT_5_OCDN_SEL         ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for AD1_5 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ADR_EXT_5_OCDN_SEL(x)           ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR_EXT_5_OCDN_SEL(x)       ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_ADR_EXT_5_OCDP_SEL         ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for AD1_5 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ADR_EXT_5_OCDP_SEL(x)           ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR_EXT_5_OCDP_SEL(x)       ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_BA0_OCDN_SEL               ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for BA0 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_BA0_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_BA0_OCDN_SEL(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_BA0_OCDP_SEL               ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for BA0 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_BA0_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_BA0_OCDP_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_5
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_BA1_OCDN_SEL               ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for BA1 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_BA1_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_BA1_OCDN_SEL(x)             ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_BA1_OCDP_SEL               ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for BA1 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_BA1_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_BA1_OCDP_SEL(x)             ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_BA2_OCDN_SEL               ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for BA2 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_BA2_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_BA2_OCDN_SEL(x)             ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_BA2_OCDP_SEL               ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for BA2 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_BA2_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_BA2_OCDP_SEL(x)             ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_RAS_OCDN_SEL               ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for RASN 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_RAS_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_RAS_OCDN_SEL(x)             ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_RAS_OCDP_SEL               ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for RASN 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_RAS_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_RAS_OCDP_SEL(x)             ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_CAS_OCDN_SEL               ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for CASN 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_CAS_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CAS_OCDN_SEL(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CAS_OCDP_SEL               ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for CASN 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_CAS_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CAS_OCDP_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_6
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_WE_OCDN_SEL                ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for WEN 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_WE_OCDN_SEL(x)                  ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_WE_OCDN_SEL(x)              ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_WE_OCDP_SEL                ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for WEN 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_WE_OCDP_SEL(x)                  ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_WE_OCDP_SEL(x)              ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ODT_OCDN_SEL               ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for ODT 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ODT_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ODT_OCDN_SEL(x)             ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ODT_OCDP_SEL               ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for ODT 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ODT_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ODT_OCDP_SEL(x)             ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_RST_OCDN_SEL               ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for RSTN 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_RST_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_RST_OCDN_SEL(x)             ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_RST_OCDP_SEL               ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for RSTN 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_RST_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_RST_OCDP_SEL(x)             ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_PAR_OCDN_SEL               ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for PAR 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_PAR_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_PAR_OCDN_SEL(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_PAR_OCDP_SEL               ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for PAR 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_PAR_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_PAR_OCDP_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_7
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_PAR1_OCDN_SEL              ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for PAR_1 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_PAR1_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_PAR1_OCDN_SEL(x)            ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_PAR1_OCDP_SEL              ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for PAR_1 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_PAR1_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_PAR1_OCDP_SEL(x)            ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ACT_OCDN_SEL               ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for ACTN 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ACT_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ACT_OCDN_SEL(x)             ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ACT_OCDP_SEL               ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for ACTN 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ACT_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ACT_OCDP_SEL(x)             ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_DDR4_BG0_OCDN_SEL          ((u32)0x00000007 << 12)          /*!<R/W ’h1  N-OCD selection for DDR4_BG0 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DDR4_BG0_OCDN_SEL(x)            ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DDR4_BG0_OCDN_SEL(x)        ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DDR4_BG0_OCDP_SEL          ((u32)0x00000007 << 8)          /*!<R/W ’h1  P-OCD selection for DDR4_BG0 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DDR4_BG0_OCDP_SEL(x)            ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DDR4_BG0_OCDP_SEL(x)        ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_TEN_OCDN_SEL               ((u32)0x00000007 << 4)          /*!<R/W ’h1  N-OCD selection for DDR4_TEN 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_TEN_OCDN_SEL(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_TEN_OCDN_SEL(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_TEN_OCDP_SEL               ((u32)0x00000007 << 0)          /*!<R/W ’h1  P-OCD selection for DDR4_TEN 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_TEN_OCDP_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_TEN_OCDP_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_OCD_SEL_8
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ALERT_OCDN_SEL             ((u32)0x00000007 << 28)          /*!<R/W ’h1  N-OCD selection for ALERT 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ALERT_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ALERT_OCDN_SEL(x)           ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ALERT_OCDP_SEL             ((u32)0x00000007 << 24)          /*!<R/W ’h1  P-OCD selection for ALERT 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ALERT_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ALERT_OCDP_SEL(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ALERT1_OCDN_SEL            ((u32)0x00000007 << 20)          /*!<R/W ’h1  N-OCD selection for ALERT_1 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_ALERT1_OCDN_SEL(x)              ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ALERT1_OCDN_SEL(x)          ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ALERT1_OCDP_SEL            ((u32)0x00000007 << 16)          /*!<R/W ’h1  P-OCD selection for ALERT_1 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_ALERT1_OCDP_SEL(x)              ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ALERT1_OCDP_SEL(x)          ((u32)(((x >> 16) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ODT_SEL_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CA_TTCN_SEL                ((u32)0x00000007 << 4)          /*!<R/W ’h1  TTCN selection for all address/command pad, except CKE and CSN 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_CA_TTCN_SEL(x)                  ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CA_TTCN_SEL(x)              ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CA_TTCP_SEL                ((u32)0x00000007 << 0)          /*!<R/W ’h1  TTCP selection for all address/command pad, except CKE and CSN 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_CA_TTCP_SEL(x)                  ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CA_TTCP_SEL(x)              ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CKE_ODT_SEL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CKE_1_TTCN_SEL             ((u32)0x00000007 << 12)          /*!<R/W ’h1  TTCN selection for CKE_1 (no use) 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_CKE_1_TTCN_SEL(x)               ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CKE_1_TTCN_SEL(x)           ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CKE_1_TTCP_SEL             ((u32)0x00000007 << 8)          /*!<R/W ’h1  TTCP selection for CKE_1 (no use) 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_CKE_1_TTCP_SEL(x)               ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CKE_1_TTCP_SEL(x)           ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_CKE_TTCN_SEL               ((u32)0x00000007 << 4)          /*!<R/W ’h1  TTCN selection for CKE 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_CKE_TTCN_SEL(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CKE_TTCN_SEL(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CKE_TTCP_SEL               ((u32)0x00000007 << 0)          /*!<R/W ’h1  TTCP selection for CKE 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_CKE_TTCP_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CKE_TTCP_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_ODT_SEL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CS_1_TTCN_SEL              ((u32)0x00000007 << 28)          /*!<R/W ’h1  TTCN selection for CS_1 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_CS_1_TTCN_SEL(x)                ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_CS_1_TTCN_SEL(x)            ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_CS_1_TTCP_SEL              ((u32)0x00000007 << 24)          /*!<R/W ’h1  TTCP selection for CS_1 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_CS_1_TTCP_SEL(x)                ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_CS_1_TTCP_SEL(x)            ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_CS_TTCN_SEL                ((u32)0x00000007 << 12)          /*!<R/W ’h1  TTCN selection for CS 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_CS_TTCN_SEL(x)                  ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CS_TTCN_SEL(x)              ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CS_TTCP_SEL                ((u32)0x00000007 << 8)          /*!<R/W ’h1  TTCP selection for CS 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_CS_TTCP_SEL(x)                  ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CS_TTCP_SEL(x)              ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_RST_TTCN_SEL               ((u32)0x00000007 << 4)          /*!<R/W ’h1  TTCN selection for RSTN 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_RST_TTCN_SEL(x)                 ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_RST_TTCN_SEL(x)             ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_RST_TTCP_SEL               ((u32)0x00000007 << 0)          /*!<R/W ’h1  TTCP selection for RSTN 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_RST_TTCP_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_RST_TTCP_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CLK_ODT_SEL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CK_1_TTCN_SEL              ((u32)0x00000007 << 12)          /*!<R/W ’h0  TTCN selection for CLK_1 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_CK_1_TTCN_SEL(x)                ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CK_1_TTCN_SEL(x)            ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CK_1_TTCP_SEL              ((u32)0x00000007 << 8)          /*!<R/W ’h0  TTCP selection for CLK_1 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_CK_1_TTCP_SEL(x)                ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_CK_1_TTCP_SEL(x)            ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_CK_TTCN_SEL                ((u32)0x00000007 << 4)          /*!<R/W ’h0  TTCN selection for CLK 000: odt_ttcn_set_0 … 111: odt_ttcn_set_7 */
#define DDRPHY_CK_TTCN_SEL(x)                  ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CK_TTCN_SEL(x)              ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CK_TTCP_SEL                ((u32)0x00000007 << 0)          /*!<R/W ’h0  TTCP selection for CLK 000: odt_ttcp_set_0 … 111: odt_ttcp_set_7 */
#define DDRPHY_CK_TTCP_SEL(x)                  ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CK_TTCP_SEL(x)              ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DPI_CTRL_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_EN_CRC                      ((u32)0x00000001 << 29)          /*!<R/W ‘h0  [1] CRC enable (post one T more) for DDR4 only [0] no CRC */
#define DDRPHY_EN_CRC(x)                       ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_2T_WR_PRE                   ((u32)0x00000001 << 28)          /*!<R/W ‘h0  [1] 2T preamble for DDR4 only(Write) [0] 1T preamble */
#define DDRPHY_2T_WR_PRE(x)                    ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_DATA_PRE                    ((u32)0x00000001 << 21)          /*!<R/W ‘b1  1: Enable Tx DQ preamble, enable this function will add 2T write delay in 2to1 mode, or 1T write delay in 1to1 mode */
#define DDRPHY_DATA_PRE(x)                     ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_DATA_INIT                   ((u32)0x00000001 << 20)          /*!<R/W ‘b1  Set the Tx initial value of DQ/DM */
#define DDRPHY_DATA_INIT(x)                    ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_BIT_CAL_DQSEN_UPDATE_ENA        ((u32)0x00000001 << 19)          /*!<R/W ’b1  Enable HW auto-update tm_dqsen_ftun calibration value */
#define DDRPHY_CAL_DQSEN_UPDATE_ENA(x)         ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_MASK_CAL_DQSEN_MODE             ((u32)0x00000003 << 17)          /*!<R/W ’h3  tm_dqs_en calibration set mode [00] during no read operation [01] during refresh [10] [11] disable auto calibration */
#define DDRPHY_CAL_DQSEN_MODE(x)               ((u32)(((x) & 0x00000003) << 17))
#define DDRPHY_GET_CAL_DQSEN_MODE(x)           ((u32)(((x >> 17) & 0x00000003)))
#define DDRPHY_BIT_2T_RD_PRE                   ((u32)0x00000001 << 15)          /*!<R/W ‘h0  [1] 2T preamble for DDR4 only (READ) [0] 1T preamble */
#define DDRPHY_2T_RD_PRE(x)                    ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_MASK_REF_UPDATE_DLY             ((u32)0x0000003F << 8)          /*!<R/W ’h20  Update delay chain setting after receiving refresh command */
#define DDRPHY_REF_UPDATE_DLY(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_REF_UPDATE_DLY(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_CAL_SET_MODE               ((u32)0x00000003 << 4)          /*!<R/W ’h0  3-point calibration set mode [00] during no read operation [01] during refresh [10] immediately [11] disable auto calibration */
#define DDRPHY_CAL_SET_MODE(x)                 ((u32)(((x) & 0x00000003) << 4))
#define DDRPHY_GET_CAL_SET_MODE(x)             ((u32)(((x >> 4) & 0x00000003)))
#define DDRPHY_MASK_RST_FIFO_MODE              ((u32)0x00000003 << 2)          /*!<R/W ’h1  Reset read FIFO pointer mode [00] during no read operation [01] during refresh [10] disable reset FIFO pointer [11] force reset FIFO pointer */
#define DDRPHY_RST_FIFO_MODE(x)                ((u32)(((x) & 0x00000003) << 2))
#define DDRPHY_GET_RST_FIFO_MODE(x)            ((u32)(((x >> 2) & 0x00000003)))
#define DDRPHY_MASK_FW_SET_MODE                ((u32)0x00000003 << 0)          /*!<R/W ’h0  FW set delay chain of data slice mode [00] READ:during no read; WRITE: during refresh [01] during refresh [10] immediately [11] reserved */
#define DDRPHY_FW_SET_MODE(x)                  ((u32)(((x) & 0x00000003) << 0))
#define DDRPHY_GET_FW_SET_MODE(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DPI_CTRL_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_WRITE_EN_4                  ((u32)0x00000001 << 31)          /*!<W -  Enable of the following row */
#define DDRPHY_WRITE_EN_4(x)                   ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_SET_REG_VALID               ((u32)0x00000001 << 30)          /*!<R/W ‘h0  Set dll/rw register flow is as 1st: set_reg_valid = 0 2nd: ser any other dll/rw register 3rd : set_reg_valid = 1 */
#define DDRPHY_SET_REG_VALID(x)                ((u32)(((x) & 0x00000001) << 30))
#define DDRPHY_BIT_WRITE_EN_3                  ((u32)0x00000001 << 7)          /*!<W -  Enable of the following row */
#define DDRPHY_WRITE_EN_3(x)                   ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_BIT_FW_SET_RD_DET               ((u32)0x00000001 << 6)          /*!<W ‘h0  FW set read delay chain of valid window detection (one cycle pulse, HW auto clear) */
#define DDRPHY_FW_SET_RD_DET(x)                ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_WRITE_EN_1                  ((u32)0x00000001 << 3)          /*!<W -  Enable of the following row */
#define DDRPHY_WRITE_EN_1(x)                   ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_FW_SET_RD_DLY               ((u32)0x00000001 << 2)          /*!<W ‘h0  FW set read delay chain TE and Delta of data slice (one cycle pulse, HW auto clear) */
#define DDRPHY_FW_SET_RD_DLY(x)                ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_WRITE_EN_0                  ((u32)0x00000001 << 1)          /*!<W -  Enable of the following row */
#define DDRPHY_WRITE_EN_0(x)                   ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_FW_SET_WR_DLY               ((u32)0x00000001 << 0)          /*!<R/W ‘h0  FW set write delay chain of data slice (one cycle pulse, HW auto clear) */
#define DDRPHY_FW_SET_WR_DLY(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DPI_CTRL_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_RST_DQSEN_CAL               ((u32)0x00000001 << 31)          /*!<R/W ’h1  1’b0: disable reset fw_tm_dqs_en_ftun when reset dqsen_gen 1’b1: enable reset fw_tm_dqs_en_ftun when reset dqsen_gen */
#define DDRPHY_RST_DQSEN_CAL(x)                ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_SET_REG_VALID_MODE          ((u32)0x00000001 << 28)          /*!<R/W ‘h0  [0] trigger set_reg_valid manually [1] trigger set_reg_valid automatically */
#define DDRPHY_SET_REG_VALID_MODE(x)           ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_RST_3POINT_MODE            ((u32)0x00000003 << 26)          /*!<R/W ’h0  Reset dqsen_gen mode [00] disable reset 3 point [01] force reset 3 point [10] reserved [11] during refresh */
#define DDRPHY_RST_3POINT_MODE(x)              ((u32)(((x) & 0x00000003) << 26))
#define DDRPHY_GET_RST_3POINT_MODE(x)          ((u32)(((x >> 26) & 0x00000003)))
#define DDRPHY_MASK_RST_DQSEN_MODE             ((u32)0x00000003 << 24)          /*!<R/W ’h0  Reset dqsen_gen mode [00] disable reset RX dqsen_gen [01] force reset RX dqsen_gen [10] reserved [11] during refresh */
#define DDRPHY_RST_DQSEN_MODE(x)               ((u32)(((x) & 0x00000003) << 24))
#define DDRPHY_GET_RST_DQSEN_MODE(x)           ((u32)(((x >> 24) & 0x00000003)))
#define DDRPHY_BIT_ALERT_ENA                   ((u32)0x00000001 << 23)          /*!<R/W ’b0  DFI alert output enable */
#define DDRPHY_ALERT_ENA(x)                    ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_ALERT_1_ENA                 ((u32)0x00000001 << 22)          /*!<R/W ’b0  DFI alert_1 output enable */
#define DDRPHY_ALERT_1_ENA(x)                  ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_MASK_RD_UPDATE_DLY              ((u32)0x0000003F << 16)          /*!<R/W ’h16  Update delay chain setting after read data enable Cmd domain 2to1 mode : > 1/2 rd_update_dly_rx +N 1to1 mode : > rd_update_dly_rx +N */
#define DDRPHY_RD_UPDATE_DLY(x)                ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_RD_UPDATE_DLY(x)            ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_RD_UPDATE_DLY_RX           ((u32)0x0000007F << 8)          /*!<R/W ’h1f  Update delay chain setting after read data enable Rx domain */
#define DDRPHY_RD_UPDATE_DLY_RX(x)             ((u32)(((x) & 0x0000007F) << 8))
#define DDRPHY_GET_RD_UPDATE_DLY_RX(x)         ((u32)(((x >> 8) & 0x0000007F)))
#define DDRPHY_MASK_PWROFF_CMD_GATE            ((u32)0x0000000F << 4)          /*!<R/W ‘h0  1: Let the following signals always be 1. [0]: let dfi_wr_cmd_01 always be 1. [1]: let dfi_wr_cmd_23 always be 1. [2]: let dfi_rd_cmd_01 always be 1. [3]: let dfi_rd_cmd_23 always be 1. */
#define DDRPHY_PWROFF_CMD_GATE(x)              ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_PWROFF_CMD_GATE(x)          ((u32)(((x >> 4) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_BIST_2TO1_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_BIST_CA_XOR                 ((u32)0x00000001 << 24)          /*!<R/W ‘h0  XOR dfi_wrdata_p* with dfi C/A signals for controller loopback test dfi_wrdata_xor_p0[63:0] = dfi_wrdata_p0[63:0] ^ { 8'h0, dfi_ten_in_p0, dfi_parity_in_p0, dfi_act_n_p0, dfi_bg0_p0, ^dfi_odt_p0[1:0], ^dfi_cke_1_p0[1:0], ^dfi_cke_p0[1:0], ^dfi_cs_n_1_p0[1:0], ^dfi_cs_n_p0[1:0], ^dfi_we_n_p0[1:0], ^dfi_ras_n_p0[1:0], ^dfi_cas_n_p0[1:0], dfi_bank_p0[5:0], dfi_address_ext_p0[3:0], dfi_address_p0[33:0]}; dfi_wrdata_xor_p1[63:0] = dfi_wrdata_p1[63:0] ^ { 8'h0, dfi_ten_in_p1, dfi_parity_in_p1, dfi_act_n_p1, dfi_bg0_p1, ^dfi_odt_p1[1:0], ^dfi_cke_1_p1[1:0], ^dfi_cke_p1[1:0], ^dfi_cs_n_1_p1[1:0], ^dfi_cs_n_p1[1:0], ^dfi_we_n_p1[1:0], ^dfi_ras_n_p1[1:0], ^dfi_cas_n_p1[1:0], dfi_bank_p1[5:0], dfi_address_ext_p1[3:0], dfi_address_p1[33:0]}; */
#define DDRPHY_BIST_CA_XOR(x)                  ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_BIST_EN                     ((u32)0x00000001 << 23)          /*!<R/W ‘h0  1Enable bist circuit in PHY。 0Disable bist circuit & reset all flag of bist。 */
#define DDRPHY_BIST_EN(x)                      ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_MASK_BIST_MODE                  ((u32)0x00000007 << 20)          /*!<R/W ‘h0  Bist mode： 000 PRBS11。 001 PRBS13。 010 PRBS31。 */
#define DDRPHY_BIST_MODE(x)                    ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_BIST_MODE(x)                ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_BIST_DEBUG_SEL             ((u32)0x0000001F << 0)          /*!<R/W ‘h0  debug selection */
#define DDRPHY_BIST_DEBUG_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_BIST_DEBUG_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_BIST_2TO1_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_3POINT_VIO_DET              ((u32)0x00000001 << 6)          /*!<R ‘h0  1 means Rx 3-point lead/lag has happened. To clear this flag, set VALID_WIN_DET_PFIFO_*, VALID_WIN_DET_NFIFO_* to all 0. */
#define DDRPHY_3POINT_VIO_DET(x)               ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_BIST_RDY                    ((u32)0x00000001 << 5)          /*!<R ‘h0  When bist is finished, then bist_rdy = 1 until bist_en = 0。 */
#define DDRPHY_BIST_RDY(x)                     ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_BIST_FAIL_CMD               ((u32)0x00000001 << 4)          /*!<R ‘h0  Reserved。 */
#define DDRPHY_BIST_FAIL_CMD(x)                ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_MASK_BIST_FAIL_DATA             ((u32)0x0000000F << 0)          /*!<R ‘h0  Fail flag for DDR PHY’s data slice。 If bist fail, then fail flag = 1 until bist_en = 0。 */
#define DDRPHY_BIST_FAIL_DATA(x)               ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_BIST_FAIL_DATA(x)           ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_BIST_PT
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_BIST_PATTERN               ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  BIST initial pattern */
#define DDRPHY_BIST_PATTERN(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_BIST_PATTERN(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_BIST_DEBUG
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_BIST_DEBUG                 ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  BIST debug out */
#define DDRPHY_BIST_DEBUG(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_BIST_DEBUG(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_INT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_INT_WRITE_EN_15             ((u32)0x00000001 << 31)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_15(x)              ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_BIT_INT_VREF_DBG_EN             ((u32)0x00000001 << 30)          /*!<W ‘h0  Vref debug INT enable */
#define DDRPHY_INT_VREF_DBG_EN(x)              ((u32)(((x) & 0x00000001) << 30))
#define DDRPHY_BIT_INT_WRITE_EN_14             ((u32)0x00000001 << 29)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_14(x)              ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_INT_DQS_BURST_ERR_EN        ((u32)0x00000001 << 28)          /*!<R/W ‘h0  Rx DQS burst length error INT enable */
#define DDRPHY_INT_DQS_BURST_ERR_EN(x)         ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_BIT_INT_WRITE_EN_13             ((u32)0x00000001 << 27)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_13(x)              ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_BIT_INT_TM_DQSEN_EN             ((u32)0x00000001 << 26)          /*!<R/W ‘h0  tm_dqs_en calibration INT enable */
#define DDRPHY_INT_TM_DQSEN_EN(x)              ((u32)(((x) & 0x00000001) << 26))
#define DDRPHY_BIT_INT_WRITE_EN_12             ((u32)0x00000001 << 25)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_12(x)              ((u32)(((x) & 0x00000001) << 25))
#define DDRPHY_BIT_INT_FIFO_PTR _EN            ((u32)0x00000001 << 24)          /*!<R/W ‘h0  fifo_prt_int enable */
#define DDRPHY_INT_FIFO_PTR _EN(x)             ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_INT_WRITE_EN_11             ((u32)0x00000001 << 23)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_11(x)              ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_INT_ZQ_EN                   ((u32)0x00000001 << 22)          /*!<R/W ‘h0  Enable ZQ / RZQ done interrupt */
#define DDRPHY_INT_ZQ_EN(x)                    ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_BIT_INT_WRITE_EN_8              ((u32)0x00000001 << 17)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_8(x)               ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_INT_ALERT_EN                ((u32)0x00000001 << 16)          /*!<R/W ‘h0  Enable alert_out interrupt */
#define DDRPHY_INT_ALERT_EN(x)                 ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_BIT_INT_WRITE_EN_7              ((u32)0x00000001 << 15)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_7(x)               ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_BIT_INT_PLL_EN                  ((u32)0x00000001 << 14)          /*!<R/W ‘h0  Enable PLL stable interrupt */
#define DDRPHY_INT_PLL_EN(x)                   ((u32)(((x) & 0x00000001) << 14))
#define DDRPHY_BIT_INT_WRITE_EN_6              ((u32)0x00000001 << 13)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_6(x)               ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_INT_CPU_3_EN                ((u32)0x00000001 << 12)          /*!<R/W ‘h0  To CPU 3 interrupt enable */
#define DDRPHY_INT_CPU_3_EN(x)                 ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_INT_WRITE_EN_5              ((u32)0x00000001 << 11)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_5(x)               ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_BIT_INT_CPU_2_EN                ((u32)0x00000001 << 10)          /*!<R/W ‘h0  To CPU 2 interrupt enable */
#define DDRPHY_INT_CPU_2_EN(x)                 ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_INT_WRITE_EN_4              ((u32)0x00000001 << 9)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_4(x)               ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_INT_CPU_1_EN                ((u32)0x00000001 << 8)          /*!<R/W ‘h0  To CPU 1 interrupt enable */
#define DDRPHY_INT_CPU_1_EN(x)                 ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_INT_WRITE_EN_3              ((u32)0x00000001 << 7)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_3(x)               ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_BIT_INT_CPU_0_EN                ((u32)0x00000001 << 6)          /*!<R/W ‘h0  To CPU 0 interrupt enable */
#define DDRPHY_INT_CPU_0_EN(x)                 ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_INT_WRITE_EN_2              ((u32)0x00000001 << 5)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_2(x)               ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_FW_CLR_DQS_INT              ((u32)0x00000001 << 4)          /*!<W ‘h0  FW clear dqs_int & tm_dqsen_late_int & tm_dqsen_early_int (one cycle pulse, HW auto clear) */
#define DDRPHY_FW_CLR_DQS_INT(x)               ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_BIT_INT_WRITE_EN_1              ((u32)0x00000001 << 3)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_1(x)               ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_INT_CAL_BDY_EN              ((u32)0x00000001 << 2)          /*!<R/W ‘h0  cal_bdy_int enable */
#define DDRPHY_INT_CAL_BDY_EN(x)               ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_INT_WRITE_EN_0              ((u32)0x00000001 << 1)          /*!<W -  Enable of the following row */
#define DDRPHY_INT_WRITE_EN_0(x)               ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_INT_DQS_EN                  ((u32)0x00000001 << 0)          /*!<R/W ‘h0  dqs_int enable */
#define DDRPHY_INT_DQS_EN(x)                   ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_INT_STATUS_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DQS_BURST_ERR_INT_3         ((u32)0x00000001 << 23)          /*!<R/W ‘h0  dqs_burst_err_int of data slice3 */
#define DDRPHY_DQS_BURST_ERR_INT_3(x)          ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_DQS_BURST_ERR_INT_2         ((u32)0x00000001 << 22)          /*!<R/W ‘h0  dqs_burst_err_int of data slice2 */
#define DDRPHY_DQS_BURST_ERR_INT_2(x)          ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_BIT_DQS_BURST_ERR_INT_1         ((u32)0x00000001 << 21)          /*!<R/W ‘h0  dqs_burst_err_int of data slice1 */
#define DDRPHY_DQS_BURST_ERR_INT_1(x)          ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_DQS_BURST_ERR_INT_0         ((u32)0x00000001 << 20)          /*!<R/W ‘h0  dqs_burst_err_int of data slice0 */
#define DDRPHY_DQS_BURST_ERR_INT_0(x)          ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_BIT_VR_HIBDY_INT_3              ((u32)0x00000001 << 19)          /*!<R/W ‘h0  Byte-3 Vref hit high boundary */
#define DDRPHY_VR_HIBDY_INT_3(x)               ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_VR_LOBDY_INT_3              ((u32)0x00000001 << 18)          /*!<R/W ‘h0  Byte-3 Vref hit low boundary */
#define DDRPHY_VR_LOBDY_INT_3(x)               ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_BIT_VR_HIBDY_INT_2              ((u32)0x00000001 << 17)          /*!<R/W ‘h0  Byte-2 Vref hit high boundary */
#define DDRPHY_VR_HIBDY_INT_2(x)               ((u32)(((x) & 0x00000001) << 17))
#define DDRPHY_BIT_VR_LOBDY_INT_2              ((u32)0x00000001 << 16)          /*!<R/W ‘h0  Byte-2 Vref hit low boundary */
#define DDRPHY_VR_LOBDY_INT_2(x)               ((u32)(((x) & 0x00000001) << 16))
#define DDRPHY_BIT_VR_HIBDY_INT_1              ((u32)0x00000001 << 15)          /*!<R/W ‘h0  Byte-1 Vref hit high boundary */
#define DDRPHY_VR_HIBDY_INT_1(x)               ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_BIT_VR_LOBDY_INT_1              ((u32)0x00000001 << 14)          /*!<R/W ‘h0  Byte-1 Vref hit low boundary */
#define DDRPHY_VR_LOBDY_INT_1(x)               ((u32)(((x) & 0x00000001) << 14))
#define DDRPHY_BIT_VR_HIBDY_INT_0              ((u32)0x00000001 << 13)          /*!<R/W ‘h0  Byte-0 Vref hit high boundary */
#define DDRPHY_VR_HIBDY_INT_0(x)               ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_VR_LOBDY_INT_0              ((u32)0x00000001 << 12)          /*!<R/W ‘h0  Byte-0 Vref hit low boundary */
#define DDRPHY_VR_LOBDY_INT_0(x)               ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_TM_DQSEN_EARLY_INT_3        ((u32)0x00000001 << 11)          /*!<R/W ‘h0  tm_dqs_en calibration early int of slice 3 */
#define DDRPHY_TM_DQSEN_EARLY_INT_3(x)         ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_BIT_TM_DQSEN_EARLY_INT_2        ((u32)0x00000001 << 10)          /*!<R/W ‘h0  tm_dqs_en calibration early int of slice 2 */
#define DDRPHY_TM_DQSEN_EARLY_INT_2(x)         ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_TM_DQSEN_EARLY_INT_1        ((u32)0x00000001 << 9)          /*!<R/W ‘h0  tm_dqs_en calibration early int of slice 1 */
#define DDRPHY_TM_DQSEN_EARLY_INT_1(x)         ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_TM_DQSEN_EARLY_INT_0        ((u32)0x00000001 << 8)          /*!<R/W ‘h0  tm_dqs_en calibration early int of slice 0 */
#define DDRPHY_TM_DQSEN_EARLY_INT_0(x)         ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_TM_DQSEN_LATE_INT_3         ((u32)0x00000001 << 7)          /*!<R/W ‘h0  tm_dqs_en calibration late int of slice 3 */
#define DDRPHY_TM_DQSEN_LATE_INT_3(x)          ((u32)(((x) & 0x00000001) << 7))
#define DDRPHY_BIT_TM_DQSEN_LATE_INT_2         ((u32)0x00000001 << 6)          /*!<R/W ‘h0  tm_dqs_en calibration late int of slice 2 */
#define DDRPHY_TM_DQSEN_LATE_INT_2(x)          ((u32)(((x) & 0x00000001) << 6))
#define DDRPHY_BIT_TM_DQSEN_LATE_INT_1         ((u32)0x00000001 << 5)          /*!<R/W ‘h0  tm_dqs_en calibration late int of slice 1 */
#define DDRPHY_TM_DQSEN_LATE_INT_1(x)          ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_TM_DQSEN_LATE_INT_0         ((u32)0x00000001 << 4)          /*!<R/W ‘h0  tm_dqs_en calibration late int of slice 0 */
#define DDRPHY_TM_DQSEN_LATE_INT_0(x)          ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_BIT_DQS_INT_3                   ((u32)0x00000001 << 3)          /*!<R/W ‘h0  dqs_int of data slice3 */
#define DDRPHY_DQS_INT_3(x)                    ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_DQS_INT_2                   ((u32)0x00000001 << 2)          /*!<R/W ‘h0  dqs_int of data slice2 */
#define DDRPHY_DQS_INT_2(x)                    ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_DQS_INT_1                   ((u32)0x00000001 << 1)          /*!<R/W ‘h0  dqs_int of data slice1 */
#define DDRPHY_DQS_INT_1(x)                    ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_DQS_INT_0                   ((u32)0x00000001 << 0)          /*!<R/W ‘h0  dqs_int of data slice0 */
#define DDRPHY_DQS_INT_0(x)                    ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_INT_STATUS_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_ALERT_TIE_HI                ((u32)0x00000001 << 13)          /*!<R/W ‘h0  1’b1 means alert pad was tied to high */
#define DDRPHY_ALERT_TIE_HI(x)                 ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_ALERT_OUT_1                 ((u32)0x00000001 << 12)          /*!<R/W ‘h0  1’b1 means alert_out_1 goes low */
#define DDRPHY_ALERT_OUT_1(x)                  ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_BIT_ALERT_OUT                   ((u32)0x00000001 << 11)          /*!<R/W ‘h0  1’b1 means alert_out_0 goes low */
#define DDRPHY_ALERT_OUT(x)                    ((u32)(((x) & 0x00000001) << 11))
#define DDRPHY_BIT_RZQ_DONE                    ((u32)0x00000001 << 10)          /*!<R/W ‘h0  RZQ calibration done */
#define DDRPHY_RZQ_DONE(x)                     ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_BIT_ZQ_DONE                     ((u32)0x00000001 << 9)          /*!<R/W ‘h0  ZQ calibration done */
#define DDRPHY_ZQ_DONE(x)                      ((u32)(((x) & 0x00000001) << 9))
#define DDRPHY_BIT_PLL_STABLE                  ((u32)0x00000001 << 0)          /*!<R/W ‘h0  PLL stable */
#define DDRPHY_PLL_STABLE(x)                   ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_SSC_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_SSC_NCODE_DEBUG            ((u32)0x000000FF << 16)          /*!<R ‘h0  Ncode of PLL using now, for debug */
#define DDRPHY_SSC_NCODE_DEBUG(x)              ((u32)(((x) & 0x000000FF) << 16))
#define DDRPHY_GET_SSC_NCODE_DEBUG(x)          ((u32)(((x >> 16) & 0x000000FF)))
#define DDRPHY_BIT_SSC_FMOD                    ((u32)0x00000001 << 13)          /*!<R ‘h0  1 : under-clock 0 : over-clock */
#define DDRPHY_SSC_FMOD(x)                     ((u32)(((x) & 0x00000001) << 13))
#define DDRPHY_BIT_SSC_OC_DONE                 ((u32)0x00000001 << 12)          /*!<R ‘h0  Indicate over-clock or under-clock done */
#define DDRPHY_SSC_OC_DONE(x)                  ((u32)(((x) & 0x00000001) << 12))
#define DDRPHY_MASK_SSC_FCODE_DEBUG            ((u32)0x000007FF << 0)          /*!<R ‘h0  Fcode of PLL using now, for debug */
#define DDRPHY_SSC_FCODE_DEBUG(x)              ((u32)(((x) & 0x000007FF) << 0))
#define DDRPHY_GET_SSC_FCODE_DEBUG(x)          ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_PRBS11_TEN                  ((u32)0x00000001 << 31)          /*!<R/W ‘h0  PRBS11 test enable */
#define DDRPHY_PRBS11_TEN(x)                   ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_ODT_SEL                    ((u32)0x00000007 << 28)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ODT_SEL(x)                      ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ODT_SEL(x)                  ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_BIT_PRBS11_DUAL_EN              ((u32)0x00000001 << 27)          /*!<R/W ‘h0  PRBS11 Dual edge enable */
#define DDRPHY_PRBS11_DUAL_EN(x)               ((u32)(((x) & 0x00000001) << 27))
#define DDRPHY_MASK_CAS_N_SEL                  ((u32)0x00000007 << 24)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_CAS_N_SEL(x)                    ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_CAS_N_SEL(x)                ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_BIT_CKE_TEST                    ((u32)0x00000001 << 23)          /*!<R/W ‘h0  1: force 1 ,0: force 0 */
#define DDRPHY_CKE_TEST(x)                     ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_MASK_RAS_N_SEL                  ((u32)0x00000007 << 20)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_RAS_N_SEL(x)                    ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_RAS_N_SEL(x)                ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_BIT_PRBS11_2T_EN                ((u32)0x00000001 << 19)          /*!<R/W ‘h0  1: CMD 2T ,0: CMD 1T */
#define DDRPHY_PRBS11_2T_EN(x)                 ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_MASK_WE_N_SEL                   ((u32)0x00000007 << 16)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_WE_N_SEL(x)                     ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_WE_N_SEL(x)                 ((u32)(((x >> 16) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR07_SEL                  ((u32)0x00000007 << 28)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR07_SEL(x)                    ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ADR07_SEL(x)                ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ADR06_SEL                  ((u32)0x00000007 << 24)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR06_SEL(x)                    ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ADR06_SEL(x)                ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ADR05_SEL                  ((u32)0x00000007 << 20)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR05_SEL(x)                    ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ADR05_SEL(x)                ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR04_SEL                  ((u32)0x00000007 << 16)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR04_SEL(x)                    ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR04_SEL(x)                ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR03_SEL                  ((u32)0x00000007 << 12)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR03_SEL(x)                    ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR03_SEL(x)                ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_ADR02_SEL                  ((u32)0x00000007 << 8)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR02_SEL(x)                    ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR02_SEL(x)                ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ADR01_SEL                  ((u32)0x00000007 << 4)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR01_SEL(x)                    ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_ADR01_SEL(x)                ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_ADR00_SEL                  ((u32)0x00000007 << 0)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR00_SEL(x)                    ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_ADR00_SEL(x)                ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR15_SEL                  ((u32)0x00000007 << 28)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR15_SEL(x)                    ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_ADR15_SEL(x)                ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_ADR14_SEL                  ((u32)0x00000007 << 24)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR14_SEL(x)                    ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ADR14_SEL(x)                ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ADR13_SEL                  ((u32)0x00000007 << 20)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR13_SEL(x)                    ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ADR13_SEL(x)                ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR12_SEL                  ((u32)0x00000007 << 16)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR12_SEL(x)                    ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR12_SEL(x)                ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR11_SEL                  ((u32)0x00000007 << 12)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR11_SEL(x)                    ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR11_SEL(x)                ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_ADR10_SEL                  ((u32)0x00000007 << 8)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR10_SEL(x)                    ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR10_SEL(x)                ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ADR09_SEL                  ((u32)0x00000007 << 4)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR09_SEL(x)                    ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_ADR09_SEL(x)                ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_ADR08_SEL                  ((u32)0x00000007 << 0)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR08_SEL(x)                    ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_ADR08_SEL(x)                ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX3
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_BA02_SEL                   ((u32)0x00000007 << 28)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_BA02_SEL(x)                     ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_BA02_SEL(x)                 ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_BA01_SEL                   ((u32)0x00000007 << 24)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_BA01_SEL(x)                     ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_BA01_SEL(x)                 ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_BA00_SEL                   ((u32)0x00000007 << 20)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_BA00_SEL(x)                     ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_BA00_SEL(x)                 ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_ADR05_EXT_SEL              ((u32)0x00000007 << 16)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR05_EXT_SEL(x)                ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_ADR05_EXT_SEL(x)            ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_ADR04_EXT_SEL              ((u32)0x00000007 << 12)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR04_EXT_SEL(x)                ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_ADR04_EXT_SEL(x)            ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_CS_N_SEL                   ((u32)0x00000007 << 4)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_CS_N_SEL(x)                     ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_CS_N_SEL(x)                 ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_CS_N_1_SEL                 ((u32)0x00000007 << 0)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_CS_N_1_SEL(x)                   ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CS_N_1_SEL(x)               ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX6
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DDR4_CS_N                  ((u32)0x00000007 << 24)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DDR4_CS_N(x)                    ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DDR4_CS_N(x)                ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ALERT_IN_1                 ((u32)0x00000007 << 20)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ALERT_IN_1(x)                   ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_ALERT_IN_1(x)               ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_PARITY_IN_1                ((u32)0x00000007 << 16)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_PARITY_IN_1(x)                  ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_PARITY_IN_1(x)              ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_PARITY_IN                  ((u32)0x00000007 << 12)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_PARITY_IN(x)                    ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_PARITY_IN(x)                ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_TEN_IN                     ((u32)0x00000007 << 4)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_TEN_IN(x)                       ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_TEN_IN(x)                   ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_ALERT_IN                   ((u32)0x00000007 << 0)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ALERT_IN(x)                     ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_ALERT_IN(x)                 ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX7
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR16_SEL                  ((u32)0x00000007 << 8)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ADR16_SEL(x)                    ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ADR16_SEL(x)                ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ACT_N                      ((u32)0x00000007 << 4)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_ACT_N(x)                        ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_ACT_N(x)                    ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_BG0                        ((u32)0x00000007 << 0)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_BG0(x)                          ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_BG0(x)                      ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX8
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_TEST_PATTERN               ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  32-bit test pattern for prbs11 or ten: [16: 0] : Addr[16:0] [18:17] : Ext_Addr[5:4] [21:19] : BA [22] : CS_n [23] : CS_n_1 [24] : RESET_n [25] : CKE [26] : ODT [27] : WE_n [28] : CAS_n [29] : RAS_n [30] : CKE_1 */
#define DDRPHY_TEST_PATTERN(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_TEST_PATTERN(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX9
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_TEST4_PATTERN              ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  32-bit test pattern for prbs11 or ten: [31:8] : reserved [07] : DDR4_CS_n [06] : ACT_n [05] : DDR4_CKE [04] : DDR4_BG0 [03] : PARITY_in [02] : PARITY_in_1 [01] : ALERT_in [00] : ALERT_in_1 */
#define DDRPHY_TEST4_PATTERN(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_TEST4_PATTERN(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX10
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_CS_PRBS11_INVS              ((u32)0x00000001 << 31)          /*!<R/W ‘h0  inverted PRBS11 for CS */
#define DDRPHY_CS_PRBS11_INVS(x)               ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_CS_PRBS11_SEED             ((u32)0x000007FF << 16)          /*!<R/W ‘h1  PRBS11 seed for CS */
#define DDRPHY_CS_PRBS11_SEED(x)               ((u32)(((x) & 0x000007FF) << 16))
#define DDRPHY_GET_CS_PRBS11_SEED(x)           ((u32)(((x >> 16) & 0x000007FF)))
#define DDRPHY_BIT_CMD_PRBS11_INVS             ((u32)0x00000001 << 15)          /*!<R/W ‘h0  inverted PRBS11 for CMD/ADR */
#define DDRPHY_CMD_PRBS11_INVS(x)              ((u32)(((x) & 0x00000001) << 15))
#define DDRPHY_MASK_CMD_PRBS11_SEED            ((u32)0x000007FF << 0)          /*!<R/W ‘h1  PRBS11 seed for CMD / ADR */
#define DDRPHY_CMD_PRBS11_SEED(x)              ((u32)(((x) & 0x000007FF) << 0))
#define DDRPHY_GET_CMD_PRBS11_SEED(x)          ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DUMMY_3
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_SCAN_WRAP                  ((u32)0x0000000F << 0)          /*!<R/W ‘h0  dummy register for scan test coverage */
#define DDRPHY_SCAN_WRAP(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_SCAN_WRAP(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CS_EMPH_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CS1_SLEWRATE               ((u32)0x00000003 << 28)          /*!<R/W ‘h0  CS1_n slew rate control pin */
#define DDRPHY_CS1_SLEWRATE(x)                 ((u32)(((x) & 0x00000003) << 28))
#define DDRPHY_GET_CS1_SLEWRATE(x)             ((u32)(((x >> 28) & 0x00000003)))
#define DDRPHY_MASK_CS1_DLY_EMPH               ((u32)0x0000000F << 24)          /*!<R/W ‘h0  CS1_n Delay tap control pin (no use) */
#define DDRPHY_CS1_DLY_EMPH(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_CS1_DLY_EMPH(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_CS1_EN_EMPHN               ((u32)0x0000000F << 20)          /*!<R/W ‘h7  CS1_n Emphasis control pin (no use) */
#define DDRPHY_CS1_EN_EMPHN(x)                 ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_CS1_EN_EMPHN(x)             ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_CS1_EN_EMPHP               ((u32)0x0000000F << 16)          /*!<R/W ‘h7  CS1_n Emphasis control pin (no use) */
#define DDRPHY_CS1_EN_EMPHP(x)                 ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_CS1_EN_EMPHP(x)             ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_CS_SLEWRATE                ((u32)0x00000003 << 12)          /*!<R/W ‘h0  CS_n slew rate control pin */
#define DDRPHY_CS_SLEWRATE(x)                  ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_CS_SLEWRATE(x)              ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_CS_DLY_EMPH                ((u32)0x0000000F << 8)          /*!<R/W ‘h0  CS_n Delay tap control pin (no use) */
#define DDRPHY_CS_DLY_EMPH(x)                  ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_CS_DLY_EMPH(x)              ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_CS_EN_EMPHN                ((u32)0x0000000F << 4)          /*!<R/W ‘h7  CS_n Emphasis control pin (no use) */
#define DDRPHY_CS_EN_EMPHN(x)                  ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_CS_EN_EMPHN(x)              ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_CS_EN_EMPHP                ((u32)0x0000000F << 0)          /*!<R/W ‘h7  CS_n Emphasis control pin (no use) */
#define DDRPHY_CS_EN_EMPHP(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_CS_EN_EMPHP(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CS_SEL_EMPH_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CS1_SEL_EMPHN              ((u32)0x0000000F << 20)          /*!<R/W ‘h7  CS1_n SEL Emphasis control pin */
#define DDRPHY_CS1_SEL_EMPHN(x)                ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_CS1_SEL_EMPHN(x)            ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_CS1_SEL_EMPHP              ((u32)0x0000000F << 16)          /*!<R/W ‘h7  CS1_n SEL Emphasis control pin */
#define DDRPHY_CS1_SEL_EMPHP(x)                ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_CS1_SEL_EMPHP(x)            ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_CS_SEL_EMPHN               ((u32)0x0000000F << 4)          /*!<R/W ‘h7  CS_n SEL Emphasis control pin */
#define DDRPHY_CS_SEL_EMPHN(x)                 ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_CS_SEL_EMPHN(x)             ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_CS_SEL_EMPHP               ((u32)0x0000000F << 0)          /*!<R/W ‘h7  CS_n SEL Emphasis control pin */
#define DDRPHY_CS_SEL_EMPHP(x)                 ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_CS_SEL_EMPHP(x)             ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DDR4_CS_EMPH_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CS_DDR4_SEL_EMPHN          ((u32)0x0000000F << 20)          /*!<R/W ‘h7  CS_n SEL Emphasis control pin */
#define DDRPHY_CS_DDR4_SEL_EMPHN(x)            ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_CS_DDR4_SEL_EMPHN(x)        ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_CS_DDR4_SEL_EMPHP          ((u32)0x0000000F << 16)          /*!<R/W ‘h7  CS_n SEL Emphasis control pin */
#define DDRPHY_CS_DDR4_SEL_EMPHP(x)            ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_CS_DDR4_SEL_EMPHP(x)        ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_CS_DDR4_SLEWRATE           ((u32)0x00000003 << 12)          /*!<R/W ‘h0  DDR4 CS_n slew rate control pin */
#define DDRPHY_CS_DDR4_SLEWRATE(x)             ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_CS_DDR4_SLEWRATE(x)         ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_CS_DDR4_DLY_EMPH           ((u32)0x0000000F << 8)          /*!<R/W ‘h0  DDR4 CS_n Delay tap control pin (no use) */
#define DDRPHY_CS_DDR4_DLY_EMPH(x)             ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_CS_DDR4_DLY_EMPH(x)         ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_CS_DDR4_EN_EMPHN           ((u32)0x0000000F << 4)          /*!<R/W ‘h7  DDR4 CS_n Emphasis control pin (no use) */
#define DDRPHY_CS_DDR4_EN_EMPHN(x)             ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_CS_DDR4_EN_EMPHN(x)         ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_CS_DDR4_EN_EMPHP           ((u32)0x0000000F << 0)          /*!<R/W ‘h7  DDR4 CS_n Emphasis control pin (no use) */
#define DDRPHY_CS_DDR4_EN_EMPHP(x)             ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_CS_DDR4_EN_EMPHP(x)         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CK_EMPH_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CK1_SLEWRATE               ((u32)0x00000003 << 28)          /*!<R/W ‘h0  CK1 slew rate control pin */
#define DDRPHY_CK1_SLEWRATE(x)                 ((u32)(((x) & 0x00000003) << 28))
#define DDRPHY_GET_CK1_SLEWRATE(x)             ((u32)(((x >> 28) & 0x00000003)))
#define DDRPHY_MASK_CK1_DLY_EMPH               ((u32)0x0000000F << 24)          /*!<R/W ‘h0  CK1 Delay tap control pin (no use) */
#define DDRPHY_CK1_DLY_EMPH(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_CK1_DLY_EMPH(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_CK1_EN_EMPHN               ((u32)0x0000000F << 20)          /*!<R/W ‘h7  CK1 Emphasis control pin (no use) */
#define DDRPHY_CK1_EN_EMPHN(x)                 ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_CK1_EN_EMPHN(x)             ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_CK1_EN_EMPHP               ((u32)0x0000000F << 16)          /*!<R/W ‘h7  CK1 Emphasis control pin (no use) */
#define DDRPHY_CK1_EN_EMPHP(x)                 ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_CK1_EN_EMPHP(x)             ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_CK_SLEWRATE                ((u32)0x00000003 << 12)          /*!<R/W ‘h0  CK slew rate control pin */
#define DDRPHY_CK_SLEWRATE(x)                  ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_CK_SLEWRATE(x)              ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_CK_DLY_EMPH                ((u32)0x0000000F << 8)          /*!<R/W ‘h0  CK Delay tap control pin (no use) */
#define DDRPHY_CK_DLY_EMPH(x)                  ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_CK_DLY_EMPH(x)              ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_CK_EN_EMPHN                ((u32)0x0000000F << 4)          /*!<R/W ‘h7  CK Emphasis control pin (no use) */
#define DDRPHY_CK_EN_EMPHN(x)                  ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_CK_EN_EMPHN(x)              ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_CK_EN_EMPHP                ((u32)0x0000000F << 0)          /*!<R/W ‘h7  CK Emphasis control pin (no use) */
#define DDRPHY_CK_EN_EMPHP(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_CK_EN_EMPHP(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CK_SEL_EMPH_CTL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CK1_SEL_EMPHN              ((u32)0x0000000F << 20)          /*!<R/W ‘h7  CK1 SEL Emphasis control pin */
#define DDRPHY_CK1_SEL_EMPHN(x)                ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_CK1_SEL_EMPHN(x)            ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_CK1_SEL_EMPHP              ((u32)0x0000000F << 16)          /*!<R/W ‘h7  CK1 SEL Emphasis control pin */
#define DDRPHY_CK1_SEL_EMPHP(x)                ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_CK1_SEL_EMPHP(x)            ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_CK_SEL_EMPHN               ((u32)0x0000000F << 4)          /*!<R/W ‘h7  CK sel Emphasis control pin */
#define DDRPHY_CK_SEL_EMPHN(x)                 ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_CK_SEL_EMPHN(x)             ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_CK_SEL_EMPHP               ((u32)0x0000000F << 0)          /*!<R/W ‘h7  CK sel Emphasis control pin */
#define DDRPHY_CK_SEL_EMPHP(x)                 ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_CK_SEL_EMPHP(x)             ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_EMPH_CTL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR_SLEWRATE_1             ((u32)0x00000003 << 28)          /*!<R/W ‘h0  ADR/CMD slew rate control pin */
#define DDRPHY_ADR_SLEWRATE_1(x)               ((u32)(((x) & 0x00000003) << 28))
#define DDRPHY_GET_ADR_SLEWRATE_1(x)           ((u32)(((x >> 28) & 0x00000003)))
#define DDRPHY_MASK_ADR_DLY_EMPH_1             ((u32)0x0000000F << 24)          /*!<R/W ‘h0  ADR/CMD Delay tap control pin 1 (no use) */
#define DDRPHY_ADR_DLY_EMPH_1(x)               ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_ADR_DLY_EMPH_1(x)           ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_ADR_EN_EMPHN_1             ((u32)0x0000000F << 20)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 1 (no use) */
#define DDRPHY_ADR_EN_EMPHN_1(x)               ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_ADR_EN_EMPHN_1(x)           ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_ADR_EN_EMPHP_1             ((u32)0x0000000F << 16)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 1 (no use) */
#define DDRPHY_ADR_EN_EMPHP_1(x)               ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_ADR_EN_EMPHP_1(x)           ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_ADR_SLEWRATE_0             ((u32)0x00000003 << 12)          /*!<R/W ‘h0  ADR/CMD slew rate control pin 0 */
#define DDRPHY_ADR_SLEWRATE_0(x)               ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_ADR_SLEWRATE_0(x)           ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_ADR_DLY_EMPH_0             ((u32)0x0000000F << 8)          /*!<R/W ‘h0  ADR/CMD Delay tap control pin 0 (no use) */
#define DDRPHY_ADR_DLY_EMPH_0(x)               ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_ADR_DLY_EMPH_0(x)           ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_ADR_EN_EMPHN_0             ((u32)0x0000000F << 4)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 0 (no use) */
#define DDRPHY_ADR_EN_EMPHN_0(x)               ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_ADR_EN_EMPHN_0(x)           ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_ADR_EN_EMPHP_0             ((u32)0x0000000F << 0)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 0 (no use) */
#define DDRPHY_ADR_EN_EMPHP_0(x)               ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_ADR_EN_EMPHP_0(x)           ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_SEL_EMPH_CTL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ADR_SEL_EMPHN_1            ((u32)0x0000000F << 20)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 1 */
#define DDRPHY_ADR_SEL_EMPHN_1(x)              ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_ADR_SEL_EMPHN_1(x)          ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_ADR_SEL_EMPHP_1            ((u32)0x0000000F << 16)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 1 */
#define DDRPHY_ADR_SEL_EMPHP_1(x)              ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_ADR_SEL_EMPHP_1(x)          ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_ADR_SEL_EMPHN_0            ((u32)0x0000000F << 4)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 0 */
#define DDRPHY_ADR_SEL_EMPHN_0(x)              ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_ADR_SEL_EMPHN_0(x)          ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_ADR_SEL_EMPHP_0            ((u32)0x0000000F << 0)          /*!<R/W ‘h7  ADR/CMD Emphasis control pin 0 */
#define DDRPHY_ADR_SEL_EMPHP_0(x)              ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_ADR_SEL_EMPHP_0(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_EMPH_SEL_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_A15_EMPH_SEL               ((u32)0x00000003 << 30)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A15 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A15_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 30))
#define DDRPHY_GET_A15_EMPH_SEL(x)             ((u32)(((x >> 30) & 0x00000003)))
#define DDRPHY_MASK_A14_EMPH_SEL               ((u32)0x00000003 << 28)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A14 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A14_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 28))
#define DDRPHY_GET_A14_EMPH_SEL(x)             ((u32)(((x >> 28) & 0x00000003)))
#define DDRPHY_MASK_A13_EMPH_SEL               ((u32)0x00000003 << 26)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A13 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A13_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 26))
#define DDRPHY_GET_A13_EMPH_SEL(x)             ((u32)(((x >> 26) & 0x00000003)))
#define DDRPHY_MASK_A12_EMPH_SEL               ((u32)0x00000003 << 24)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A12 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A12_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 24))
#define DDRPHY_GET_A12_EMPH_SEL(x)             ((u32)(((x >> 24) & 0x00000003)))
#define DDRPHY_MASK_A11_EMPH_SEL               ((u32)0x00000003 << 22)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A11 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A11_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 22))
#define DDRPHY_GET_A11_EMPH_SEL(x)             ((u32)(((x >> 22) & 0x00000003)))
#define DDRPHY_MASK_A10_EMPH_SEL               ((u32)0x00000003 << 20)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A10 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A10_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 20))
#define DDRPHY_GET_A10_EMPH_SEL(x)             ((u32)(((x >> 20) & 0x00000003)))
#define DDRPHY_MASK_A9_EMPH_SEL                ((u32)0x00000003 << 18)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A9 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A9_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 18))
#define DDRPHY_GET_A9_EMPH_SEL(x)              ((u32)(((x >> 18) & 0x00000003)))
#define DDRPHY_MASK_A8_EMPH_SEL                ((u32)0x00000003 << 16)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A8 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A8_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 16))
#define DDRPHY_GET_A8_EMPH_SEL(x)              ((u32)(((x >> 16) & 0x00000003)))
#define DDRPHY_MASK_A7_EMPH_SEL                ((u32)0x00000003 << 14)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A7 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A7_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 14))
#define DDRPHY_GET_A7_EMPH_SEL(x)              ((u32)(((x >> 14) & 0x00000003)))
#define DDRPHY_MASK_A6_EMPH_SEL                ((u32)0x00000003 << 12)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A6 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A6_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_A6_EMPH_SEL(x)              ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_A5_EMPH_SEL                ((u32)0x00000003 << 10)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A5 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A5_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 10))
#define DDRPHY_GET_A5_EMPH_SEL(x)              ((u32)(((x >> 10) & 0x00000003)))
#define DDRPHY_MASK_A4_EMPH_SEL                ((u32)0x00000003 << 8)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A4 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A4_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 8))
#define DDRPHY_GET_A4_EMPH_SEL(x)              ((u32)(((x >> 8) & 0x00000003)))
#define DDRPHY_MASK_A3_EMPH_SEL                ((u32)0x00000003 << 6)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A3 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A3_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 6))
#define DDRPHY_GET_A3_EMPH_SEL(x)              ((u32)(((x >> 6) & 0x00000003)))
#define DDRPHY_MASK_A2_EMPH_SEL                ((u32)0x00000003 << 4)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A2 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A2_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 4))
#define DDRPHY_GET_A2_EMPH_SEL(x)              ((u32)(((x >> 4) & 0x00000003)))
#define DDRPHY_MASK_A1_EMPH_SEL                ((u32)0x00000003 << 2)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_A1 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A1_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 2))
#define DDRPHY_GET_A1_EMPH_SEL(x)              ((u32)(((x >> 2) & 0x00000003)))
#define DDRPHY_MASK_A0_EMPH_SEL                ((u32)0x00000003 << 0)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A0 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A0_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 0))
#define DDRPHY_GET_A0_EMPH_SEL(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_EMPH_SEL_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ACT_EMPH_SEL               ((u32)0x00000003 << 30)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_ACTN 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_ACT_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 30))
#define DDRPHY_GET_ACT_EMPH_SEL(x)             ((u32)(((x >> 30) & 0x00000003)))
#define DDRPHY_MASK_RST_EMPH_SEL               ((u32)0x00000003 << 28)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_RSTN 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_RST_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 28))
#define DDRPHY_GET_RST_EMPH_SEL(x)             ((u32)(((x >> 28) & 0x00000003)))
#define DDRPHY_MASK_WE_EMPH_SEL                ((u32)0x00000003 << 26)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_WEN 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_WE_EMPH_SEL(x)                  ((u32)(((x) & 0x00000003) << 26))
#define DDRPHY_GET_WE_EMPH_SEL(x)              ((u32)(((x >> 26) & 0x00000003)))
#define DDRPHY_MASK_RAS_EMPH_SEL               ((u32)0x00000003 << 24)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_RASN 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_RAS_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 24))
#define DDRPHY_GET_RAS_EMPH_SEL(x)             ((u32)(((x >> 24) & 0x00000003)))
#define DDRPHY_MASK_PAR1_EMPH_SEL              ((u32)0x00000003 << 22)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR4_PAR_1 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_PAR1_EMPH_SEL(x)                ((u32)(((x) & 0x00000003) << 22))
#define DDRPHY_GET_PAR1_EMPH_SEL(x)            ((u32)(((x >> 22) & 0x00000003)))
#define DDRPHY_MASK_PAR_EMPH_SEL               ((u32)0x00000003 << 20)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR4_PAR 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_PAR_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 20))
#define DDRPHY_GET_PAR_EMPH_SEL(x)             ((u32)(((x >> 20) & 0x00000003)))
#define DDRPHY_MASK_ODT_EMPH_SEL               ((u32)0x00000003 << 18)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_ODT 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_ODT_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 18))
#define DDRPHY_GET_ODT_EMPH_SEL(x)             ((u32)(((x >> 18) & 0x00000003)))
#define DDRPHY_MASK_CKE1_EMPH_SEL              ((u32)0x00000003 << 16)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_CKE_1 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_CKE1_EMPH_SEL(x)                ((u32)(((x) & 0x00000003) << 16))
#define DDRPHY_GET_CKE1_EMPH_SEL(x)            ((u32)(((x >> 16) & 0x00000003)))
#define DDRPHY_MASK_CKE_EMPH_SEL               ((u32)0x00000003 << 14)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_CKE 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_CKE_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 14))
#define DDRPHY_GET_CKE_EMPH_SEL(x)             ((u32)(((x >> 14) & 0x00000003)))
#define DDRPHY_MASK_CAS_EMPH_SEL               ((u32)0x00000003 << 12)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_CASN 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_CAS_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_CAS_EMPH_SEL(x)             ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_BA2_EMPH_SEL               ((u32)0x00000003 << 10)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_BA2 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_BA2_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 10))
#define DDRPHY_GET_BA2_EMPH_SEL(x)             ((u32)(((x >> 10) & 0x00000003)))
#define DDRPHY_MASK_BA1_EMPH_SEL               ((u32)0x00000003 << 8)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_BA1 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_BA1_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 8))
#define DDRPHY_GET_BA1_EMPH_SEL(x)             ((u32)(((x >> 8) & 0x00000003)))
#define DDRPHY_MASK_BA0_EMPH_SEL               ((u32)0x00000003 << 6)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_BA0 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_BA0_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 6))
#define DDRPHY_GET_BA0_EMPH_SEL(x)             ((u32)(((x >> 6) & 0x00000003)))
#define DDRPHY_MASK_AD1_5_EMPH_SEL             ((u32)0x00000003 << 4)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_AD1_5 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_AD1_5_EMPH_SEL(x)               ((u32)(((x) & 0x00000003) << 4))
#define DDRPHY_GET_AD1_5_EMPH_SEL(x)           ((u32)(((x >> 4) & 0x00000003)))
#define DDRPHY_MASK_AD1_4_EMPH_SEL             ((u32)0x00000003 << 2)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_AD1_4 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_AD1_4_EMPH_SEL(x)               ((u32)(((x) & 0x00000003) << 2))
#define DDRPHY_GET_AD1_4_EMPH_SEL(x)           ((u32)(((x >> 2) & 0x00000003)))
#define DDRPHY_MASK_A16_EMPH_SEL               ((u32)0x00000003 << 0)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_A16 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_A16_EMPH_SEL(x)                 ((u32)(((x) & 0x00000003) << 0))
#define DDRPHY_GET_A16_EMPH_SEL(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_ADR_EMPH_SEL_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ALERT1_EMPH_SEL            ((u32)0x00000003 << 8)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR_ALERT_1 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_ALERT1_EMPH_SEL(x)              ((u32)(((x) & 0x00000003) << 8))
#define DDRPHY_GET_ALERT1_EMPH_SEL(x)          ((u32)(((x >> 8) & 0x00000003)))
#define DDRPHY_MASK_ALERT_EMPH_SEL             ((u32)0x00000003 << 6)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR_ALERT 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_ALERT_EMPH_SEL(x)               ((u32)(((x) & 0x00000003) << 6))
#define DDRPHY_GET_ALERT_EMPH_SEL(x)           ((u32)(((x >> 6) & 0x00000003)))
#define DDRPHY_MASK_DDR4_TEN_EMPH_SEL          ((u32)0x00000003 << 4)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR4_TEN 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_DDR4_TEN_EMPH_SEL(x)            ((u32)(((x) & 0x00000003) << 4))
#define DDRPHY_GET_DDR4_TEN_EMPH_SEL(x)        ((u32)(((x >> 4) & 0x00000003)))
#define DDRPHY_MASK_DDR4_CKE_EMPH_SEL          ((u32)0x00000003 << 2)          /*!<R/W ‘h0  Tx emphasis control of PAD_DDR4_CKE 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_DDR4_CKE_EMPH_SEL(x)            ((u32)(((x) & 0x00000003) << 2))
#define DDRPHY_GET_DDR4_CKE_EMPH_SEL(x)        ((u32)(((x >> 2) & 0x00000003)))
#define DDRPHY_MASK_DDR4_BG0_EMPH_SEL          ((u32)0x00000003 << 0)          /*!<R/W ’h0  Tx emphasis control of PAD_DDR4_BG0 00: use ADR/CMD emphasis control set 0 01: use ADR/CMD emphasis control set 1 10: use ADR/CMD emphasis control set 2 11: use ADR/CMD emphasis control set 3 */
#define DDRPHY_DDR4_BG0_EMPH_SEL(x)            ((u32)(((x) & 0x00000003) << 0))
#define DDRPHY_GET_DDR4_BG0_EMPH_SEL(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DBG_READ_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQSEN_DBG                  ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Use dqsen_dbg_sel[1:0] to select dqsen_dbg_0, dqsen_dbg_1, dqsen_dbg_2, dqsen_dbg_3 */
#define DDRPHY_DQSEN_DBG(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_DQSEN_DBG(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DBG_READ_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DPI_DBG_DLL                ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Use dpi_dbg_dll_sel to select dpi_debug_dll[31:0] or dpi_debug_dll[63:32] */
#define DDRPHY_DPI_DBG_DLL(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_DPI_DBG_DLL(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_RX_CAL_CTRL_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_VR_TARGET_3                ((u32)0x0000000F << 28)          /*!<R/W ‘h5  Vref calibration process will stop if the minimum delta width is greater than or equal to this value. This is for byte-3 Vref. */
#define DDRPHY_VR_TARGET_3(x)                  ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_VR_TARGET_3(x)              ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_VR_TARGET_2                ((u32)0x0000000F << 24)          /*!<R/W ‘h5  Vref calibration process will stop if the minimum delta width is greater than or equal to this value. This is for byte-2 Vref. */
#define DDRPHY_VR_TARGET_2(x)                  ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_VR_TARGET_2(x)              ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_VR_TARGET_1                ((u32)0x0000000F << 20)          /*!<R/W ‘h5  Vref calibration process will stop if the minimum delta width is greater than or equal to this value. This is for byte-1 Vref. */
#define DDRPHY_VR_TARGET_1(x)                  ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_VR_TARGET_1(x)              ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_VR_TARGET_0                ((u32)0x0000000F << 16)          /*!<R/W ‘h5  Vref calibration process will stop if the minimum delta width is greater than or equal to this value. This is for byte-0 Vref. */
#define DDRPHY_VR_TARGET_0(x)                  ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_VR_TARGET_0(x)              ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_VR_SRC_SEL                 ((u32)0x00000007 << 12)          /*!<R/W ‘h0  Select data source of Vref update criteria */
#define DDRPHY_VR_SRC_SEL(x)                   ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_VR_SRC_SEL(x)               ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_BIT_CAL_OFFLINE                 ((u32)0x00000001 << 8)          /*!<R/W ‘h0  1: TE only move in one direction, and Delta only decrease. This mode should only be used in offline or fore-ground calibration to find the minimum working Rx delay taps. */
#define DDRPHY_CAL_OFFLINE(x)                  ((u32)(((x) & 0x00000001) << 8))
#define DDRPHY_BIT_DELTA_FORCE                 ((u32)0x00000001 << 5)          /*!<R/W ‘h0  force delta value without double-buffer */
#define DDRPHY_DELTA_FORCE(x)                  ((u32)(((x) & 0x00000001) << 5))
#define DDRPHY_BIT_VR_UPD_3                    ((u32)0x00000001 << 3)          /*!<R/W ‘h1  1: vref_s_3 can be applied to Vref pad by fw_set_rd_dly */
#define DDRPHY_VR_UPD_3(x)                     ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_VR_UPD_2                    ((u32)0x00000001 << 2)          /*!<R/W ‘h1  1: vref_s_2 can be applied to Vref pad by fw_set_rd_dly */
#define DDRPHY_VR_UPD_2(x)                     ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_VR_UPD_1                    ((u32)0x00000001 << 1)          /*!<R/W ‘h1  1: vref_s_1 can be applied to Vref pad by fw_set_rd_dly */
#define DDRPHY_VR_UPD_1(x)                     ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_VR_UPD_0                    ((u32)0x00000001 << 0)          /*!<R/W ‘h1  1: vref_s_0 can be applied to Vref pad by fw_set_rd_dly */
#define DDRPHY_VR_UPD_0(x)                     ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_RX_CAL_CTRL_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_VR_CNT_TARGET              ((u32)0x0000003F << 0)          /*!<R/W ‘h0  update Vref when target count is reached 0 : disable Vref calibration */
#define DDRPHY_VR_CNT_TARGET(x)                ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_VR_CNT_TARGET(x)            ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_VREF_BDY_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_VR_HI_BDY_1                ((u32)0x0000003F << 24)          /*!<R/W ‘h26  Vref1 high boundary for rw_blk_01 */
#define DDRPHY_VR_HI_BDY_1(x)                  ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_VR_HI_BDY_1(x)              ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_VR_LO_BDY_1                ((u32)0x0000003F << 16)          /*!<R/W ‘h04  Vref1 low boundary for rw_blk_01 */
#define DDRPHY_VR_LO_BDY_1(x)                  ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_VR_LO_BDY_1(x)              ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_VR_HI_BDY_0                ((u32)0x0000003F << 8)          /*!<R/W ‘h26  Vref0 high boundary for rw_blk_01 */
#define DDRPHY_VR_HI_BDY_0(x)                  ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_VR_HI_BDY_0(x)              ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_VR_LO_BDY_0                ((u32)0x0000003F << 0)          /*!<R/W ‘h04  Vref0 low boundary for rw_blk_01 */
#define DDRPHY_VR_LO_BDY_0(x)                  ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_VR_LO_BDY_0(x)              ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_VREF_BDY_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_VR_HI_BDY_3                ((u32)0x0000003F << 24)          /*!<R/W ‘h26  Vref3 high boundary for rw_blk_23 */
#define DDRPHY_VR_HI_BDY_3(x)                  ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_VR_HI_BDY_3(x)              ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_VR_LO_BDY_3                ((u32)0x0000003F << 16)          /*!<R/W ‘h04  Vref3 low boundary for rw_blk_23 */
#define DDRPHY_VR_LO_BDY_3(x)                  ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_VR_LO_BDY_3(x)              ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_VR_HI_BDY_2                ((u32)0x0000003F << 8)          /*!<R/W ‘h26  Vref2 high boundary for rw_blk_23 */
#define DDRPHY_VR_HI_BDY_2(x)                  ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_VR_HI_BDY_2(x)              ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_VR_LO_BDY_2                ((u32)0x0000003F << 0)          /*!<R/W ‘h04  Vref2 low boundary for rw_blk_23 */
#define DDRPHY_VR_LO_BDY_2(x)                  ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_VR_LO_BDY_2(x)              ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DELTA_WID_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CAL_EXTEND0                ((u32)0x00000007 << 16)          /*!<R/W ‘h1  extend width of delta when calibration result is extend[0] = 1 [2’s complement format] (sign, 2.0) 000: 0 001: 1 010: 2 011: 3 */
#define DDRPHY_CAL_EXTEND0(x)                  ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_CAL_EXTEND0(x)              ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_CAL_SHRINK0                ((u32)0x00000007 << 0)          /*!<R/W ‘h7  shrink width of delta when calibration result is shrink[0] = 1 [2’s complement format] (sign, 2.0) 111: -1 110: -2 101: -3 100: -4 */
#define DDRPHY_CAL_SHRINK0(x)                  ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_CAL_SHRINK0(x)              ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_RX_CAL_MON
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RX_VREF_S_3                ((u32)0x0000003F << 24)          /*!<R ‘h0  calibrated Vref value of byte-3 */
#define DDRPHY_RX_VREF_S_3(x)                  ((u32)(((x) & 0x0000003F) << 24))
#define DDRPHY_GET_RX_VREF_S_3(x)              ((u32)(((x >> 24) & 0x0000003F)))
#define DDRPHY_MASK_RX_VREF_S_2                ((u32)0x0000003F << 16)          /*!<R ‘h0  calibrated Vref value of byte-2 */
#define DDRPHY_RX_VREF_S_2(x)                  ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_RX_VREF_S_2(x)              ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_RX_VREF_S_1                ((u32)0x0000003F << 8)          /*!<R ‘h0  calibrated Vref value of byte-1 */
#define DDRPHY_RX_VREF_S_1(x)                  ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_RX_VREF_S_1(x)              ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_RX_VREF_S_0                ((u32)0x0000003F << 0)          /*!<R ‘h0  calibrated Vref value of byte-0 */
#define DDRPHY_RX_VREF_S_0(x)                  ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_RX_VREF_S_0(x)              ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_CTRL_ZPROG_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_ZPROG_3                    ((u32)0x00000007 << 24)          /*!<R/W ‘h0  NODT vpcal_sel */
#define DDRPHY_ZPROG_3(x)                      ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_ZPROG_3(x)                  ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_ZPROG_2                    ((u32)0x0000003F << 16)          /*!<R/W ‘h0  NODT pad pocd value */
#define DDRPHY_ZPROG_2(x)                      ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_ZPROG_2(x)                  ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_ZPROG_1                    ((u32)0x00000007 << 8)          /*!<R/W ‘h0  NOCD vpcal_sel */
#define DDRPHY_ZPROG_1(x)                      ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_ZPROG_1(x)                  ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_ZPROG_0                    ((u32)0x0000003F << 0)          /*!<R/W ‘h0  NOCD pad pocd value */
#define DDRPHY_ZPROG_0(x)                      ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_ZPROG_0(x)                  ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PAD_NOCD2_ZPROG_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_NOCD2_ZPROG_1              ((u32)0x00000007 << 8)          /*!<R/W ‘h0  NOCD2 vpcal_sel */
#define DDRPHY_NOCD2_ZPROG_1(x)                ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_NOCD2_ZPROG_1(x)            ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_NOCD2_ZPROG_0              ((u32)0x0000003F << 0)          /*!<R/W ‘h0  NOCD2 pad pocd value */
#define DDRPHY_NOCD2_ZPROG_0(x)                ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_NOCD2_ZPROG_0(x)            ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_OCD_SEL_2_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ_7_OCDN_SEL              ((u32)0x00000007 << 28)          /*!<R/W ’h0  Set 0: NT2 selection for slice3 DQ Set 1: EN_EMPHN selection for slice3 DQ 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQ_7_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_DQ_7_OCDN_SEL(x)            ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_DQ_6_OCDN_SEL              ((u32)0x00000007 << 24)          /*!<R/W ’h0  Set 0: NT2 selection for slice2 DQ Set 1: EN_EMPHN selection for slice2 DQ 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQ_6_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DQ_6_OCDN_SEL(x)            ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DQ_7_OCDP_SEL              ((u32)0x00000007 << 20)          /*!<R/W ’h0  Set 0: PT2 selection for slice3 DQ Set 1: EN_EMPHP selection for slice3 DQ 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQ_7_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_DQ_7_OCDP_SEL(x)            ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_DQ_6_OCDP_SEL              ((u32)0x00000007 << 16)          /*!<R/W ’h0  Set 0: PT2 selection for slice2 DQ Set 1: EN_EMPHP selection for slice2 DQ 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQ_6_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_DQ_6_OCDP_SEL(x)            ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_DQ_5_OCDN_SEL              ((u32)0x00000007 << 12)          /*!<R/W ’h0  Set 0: NT2 selection for slice1 DQ Set 1: EN_EMPHN selection for slice1 DQ 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQ_5_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQ_5_OCDN_SEL(x)            ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQ_4_OCDN_SEL              ((u32)0x00000007 << 8)          /*!<R/W ’h0  Set 0: NT2 selection for slice0 DQ Set 1: EN_EMPHN selection for slice0 DQ 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQ_4_OCDN_SEL(x)                ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQ_4_OCDN_SEL(x)            ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQ_5_OCDP_SEL              ((u32)0x00000007 << 4)          /*!<R/W ’h0  Set 0: PT2 selection for slice1 DQ Set 1: EN_EMPHP selection for slice1 DQ 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQ_5_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQ_5_OCDP_SEL(x)            ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQ_4_OCDP_SEL              ((u32)0x00000007 << 0)          /*!<R/W ’h0  Set 0: PT2 selection for slice0 DQ Set 1: EN_EMPHP selection for slice0 DQ 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQ_4_OCDP_SEL(x)                ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQ_4_OCDP_SEL(x)            ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_OCD_SEL_2_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_7_OCDN_SEL             ((u32)0x00000007 << 28)          /*!<R/W ’h0  Set 0: NT2 selection for DQS[3] Set 1: EN_EMPHN selection for DQS[3] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_7_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_DQS_7_OCDN_SEL(x)           ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_DQS_6_OCDN_SEL             ((u32)0x00000007 << 24)          /*!<R/W ’h0  Set 0: NT2 selection for DQS[2] Set 1: EN_EMPHN selection for DQS[2] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_6_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DQS_6_OCDN_SEL(x)           ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DQS_7_OCDP_SEL             ((u32)0x00000007 << 20)          /*!<R/W ’h0  Set 0: PT2 selection for DQS[3] Set 1: EN_EMPHP selection for DQS[3] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_7_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_DQS_7_OCDP_SEL(x)           ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_DQS_6_OCDP_SEL             ((u32)0x00000007 << 16)          /*!<R/W ’h0  Set 0: PT2 selection for DQS[2] Set 1: EN_EMPHP selection for DQS[2] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_6_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_DQS_6_OCDP_SEL(x)           ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_DQS_5_OCDN_SEL             ((u32)0x00000007 << 12)          /*!<R/W ’h0  Set 0: NT2 selection for DQS[1] Set 1: EN_EMPHN selection for DQS[1] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_5_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQS_5_OCDN_SEL(x)           ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQS_4_OCDN_SEL             ((u32)0x00000007 << 8)          /*!<R/W ’h0  Set 0: NT2 selection for DQS[0] Set 1: EN_EMPHN selection for DQS[0] 000: ocdn_set_0 … 111: ocdn_set_7 */
#define DDRPHY_DQS_4_OCDN_SEL(x)               ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQS_4_OCDN_SEL(x)           ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQS_5_OCDP_SEL             ((u32)0x00000007 << 4)          /*!<R/W ’h0  Set 0: PT2 selection for DQS[1] Set 1: EN_EMPHP selection for DQS[1] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_5_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQS_5_OCDP_SEL(x)           ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQS_4_OCDP_SEL             ((u32)0x00000007 << 0)          /*!<R/W ’h0  Set 0: PT2 selection for DQS[0] Set 1: EN_EMPHP selection for DQS[0] 000: ocdp_set_0 … 111: ocdp_set_7 */
#define DDRPHY_DQS_4_OCDP_SEL(x)               ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQS_4_OCDP_SEL(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DBG_READ_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DATA_EN_ERR_TYPE           ((u32)0x0000FFFF << 0)          /*!<R ‘h0  [1:0] check result of dfi_wrdata_en_p0/p1[0] [3:2] check result of dfi_wrdata_en_p0/p1[1] [5:4] check result of dfi_wrdata_en_p0/p1[2] [7:6] check result of dfi_wrdata_en_p0/p1[3] [9:8] check result of dfi_rddata_en_p0/p1[0] [11:10] check result of dfi_rddata_en_p0/p1[1] [13:12] check result of dfi_rddata_en_p0/p1[2] [15:14] check result of dfi_rddata_en_p0/p1[3] 2’b00: No error; 2’b01: burst length cycle is odd number; 2’b10: burst length cycle is 2. */
#define DDRPHY_DATA_EN_ERR_TYPE(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRPHY_GET_DATA_EN_ERR_TYPE(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DATA_VAL_CTRL_0
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RDDATA_DBI_VAL             ((u32)0x0000000F << 8)          /*!<R/W ‘h0  Only available if rddata_force = 1. [3:0]: Control rddata_dbi value of read slice 3/2/1/0. */
#define DDRPHY_RDDATA_DBI_VAL(x)               ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_RDDATA_DBI_VAL(x)           ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_WRDATA_MASK_VAL            ((u32)0x0000000F << 4)          /*!<R/W ‘h0  Only available if wrdata_force = 1. [3:0]: Control wrdata_mask value of write slice 3/2/1/0. */
#define DDRPHY_WRDATA_MASK_VAL(x)              ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_WRDATA_MASK_VAL(x)          ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_BIT_RDDATA_FORCE                ((u32)0x00000001 << 1)          /*!<R/W ‘b0  1: Force the value of dfi_rddata_wN and dfi_rddata_dbi_wN to rddata_val and rddata_dbi_val. */
#define DDRPHY_RDDATA_FORCE(x)                 ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_WRDATA_FORCE                ((u32)0x00000001 << 0)          /*!<R/W ‘b0  1: Force the value of dfi_wrdata_pN and dfi_wrdata_mask_pN to wrdata_val and wrdata_mask_val. */
#define DDRPHY_WRDATA_FORCE(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DATA_VAL_CTRL_1
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_WRDATA_VAL                 ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  Only available if wrdata_force = 1. [ 7: 0]: Control wrdata of write slice 0 [15:8]: Control wrdata of write slice 1 [23:16]: Control wrdata of write slice 2 [31:24]: Control wrdata of write slice 3 */
#define DDRPHY_WRDATA_VAL(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_WRDATA_VAL(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DATA_VAL_CTRL_2
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RDDATA_VAL                 ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  Only available if rddata_force = 1. [ 7: 0]: Control rddata of read slice 0 [15:8]: Control rddata of read slice 1 [23:16]: Control rddata of read slice 2 [31:24]: Control rddata of read slice 3 */
#define DDRPHY_RDDATA_VAL(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_RDDATA_VAL(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_READ_CTRL_2_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_EN_ODT_3_STG                ((u32)0x00000001 << 24)          /*!<R/W ‘h0  1: turn on/off DQ/DM/DQS ODT by 3 stages. When turn on ODT, stage 1: {4’h0, ODT_value[5:4]} stage 2: {2’h0, ODT_value[5:2]} stage 3: {ODT_value[5:0]} When turn off ODT, stage 1: {ODT_value[5:0]} stage 2: {2’h0, ODT_value[5:2]} stage 3: {4’h0, ODT_value[5:4]} Enable this function will extend odt_en/IE by 2 tCK in the end. If want to enable this function, the duration of odt_en/IE should be at least 6 tCK. */
#define DDRPHY_EN_ODT_3_STG(x)                 ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_DQS_ODT_FORCE_SEL           ((u32)0x00000001 << 23)          /*!<R/W ‘h0  DQS Rx ODT pad control. 0 : controlled by read command 1: controlled by dqs_odt_force_sig */
#define DDRPHY_DQS_ODT_FORCE_SEL(x)            ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_DQS_ODT_FORCE_SIG           ((u32)0x00000001 << 22)          /*!<R/W ‘h0  If set dqs_odt_force_sel, DQS TE is controlled by dqs_odt_force_sig. */
#define DDRPHY_DQS_ODT_FORCE_SIG(x)            ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_BIT_DQ_ODT_FORCE_SEL            ((u32)0x00000001 << 21)          /*!<R/W ‘h0  DQ Rx ODT control. 0 : controlled by read command 1: controlled by dq_odt_force_sig */
#define DDRPHY_DQ_ODT_FORCE_SEL(x)             ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_DQ_ODT_FORCE_SIG            ((u32)0x00000001 << 20)          /*!<R/W ‘h0  If set dq_odt_force_sel, DQ TE is controlled by dq_odt_force_sig. */
#define DDRPHY_DQ_ODT_FORCE_SIG(x)             ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_BIT_TM_ODT_EN_ODD               ((u32)0x00000001 << 3)          /*!<R/W ‘h1  for 2to1 mode b1 : odd length for tm_odt_en b0 : even length for tm_odt_en for 1to1 mode b1 : length add 1T */
#define DDRPHY_TM_ODT_EN_ODD(x)                ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_MASK_TM_ODT_EN_SEL              ((u32)0x00000003 << 1)          /*!<R/W ‘h0  b00: follow (odt_en + odt_en_dly) setting. b01: delay (odt_en + odt_en_dly) assert 0.5 T. b10, 2’b11: delay (odt_en + odt_en_dly) desert 0.5T. */
#define DDRPHY_TM_ODT_EN_SEL(x)                ((u32)(((x) & 0x00000003) << 1))
#define DDRPHY_GET_TM_ODT_EN_SEL(x)            ((u32)(((x >> 1) & 0x00000003)))
#define DDRPHY_BIT_TM_ODT_EN_DLY               ((u32)0x00000001 << 0)          /*!<R/W ‘h0  b1 : odt_en delay 1DCK */
#define DDRPHY_TM_ODT_EN_DLY(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_READ_CTRL_3_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DQSEN_CAL_OFFSET4           ((u32)0x00000001 << 25)          /*!<R/W ‘h0  0: DQSEN Calibration offset + 2 1: DQSEN Calibration offset + 4 */
#define DDRPHY_DQSEN_CAL_OFFSET4(x)            ((u32)(((x) & 0x00000001) << 25))
#define DDRPHY_BIT_DQSEN_DQS_INV               ((u32)0x00000001 << 24)          /*!<R/W ‘h0  0: use dqs for dqsen calibration 1: use inverted dqs for dqsen calibration (in ddr4) */
#define DDRPHY_DQSEN_DQS_INV(x)                ((u32)(((x) & 0x00000001) << 24))
#define DDRPHY_BIT_MCK_RX_INV_SEL              ((u32)0x00000001 << 23)          /*!<R/W ‘h0  0: delay dqs_en1_set by 0.5T mck_rx 1: delay dqs_en1_set by 1T mck_rx */
#define DDRPHY_MCK_RX_INV_SEL(x)               ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_DQSEN_CAL_SEL               ((u32)0x00000001 << 22)          /*!<R/W ‘h0  Select dqs enable calibration algorithm. */
#define DDRPHY_DQSEN_CAL_SEL(x)                ((u32)(((x) & 0x00000001) << 22))
#define DDRPHY_BIT_DQS_EARLY_SEL               ((u32)0x00000001 << 21)          /*!<R/W ‘h0  Select source signal of dqs early detection 0: dqs_en2 1: dqs */
#define DDRPHY_DQS_EARLY_SEL(x)                ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_MASK_TM_DQS_CAL_PRE             ((u32)0x0000001F << 16)          /*!<R/W ‘h0  for tm_dqs_en_ftun calibration 2to1 : tm_dqs_cal_pre = tm_dqs_en - 1 */
#define DDRPHY_TM_DQS_CAL_PRE(x)               ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_TM_DQS_CAL_PRE(x)           ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_TM_DQSEN_EARLY_PH          ((u32)0x0000003F << 8)          /*!<R/W ‘hB  tm_dqs_en calibration early delay tap */
#define DDRPHY_TM_DQSEN_EARLY_PH(x)            ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_TM_DQSEN_EARLY_PH(x)        ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_TM_DQSEN_LATE_PH           ((u32)0x0000003F << 0)          /*!<R/W ‘hB  tm_dqs_en calibration late delay tap */
#define DDRPHY_TM_DQSEN_LATE_PH(x)             ((u32)(((x) & 0x0000003F) << 0))
#define DDRPHY_GET_TM_DQSEN_LATE_PH(x)         ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_READ_CTRL_6_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_TM_ODT_EN                  ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0fe  odt_en length setting 2T DCK unit for 2to1 mode 1T DCK unit for 1to1 mode */
#define DDRPHY_TM_ODT_EN(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_TM_ODT_EN(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DEL_POS_SEL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DELTA_WID_POS_SEL_7        ((u32)0x0000000F << 28)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[7]. */
#define DDRPHY_DELTA_WID_POS_SEL_7(x)          ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_DELTA_WID_POS_SEL_7(x)      ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_POS_SEL_6        ((u32)0x0000000F << 24)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[6]. */
#define DDRPHY_DELTA_WID_POS_SEL_6(x)          ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_DELTA_WID_POS_SEL_6(x)      ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_POS_SEL_5        ((u32)0x0000000F << 20)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[5]. */
#define DDRPHY_DELTA_WID_POS_SEL_5(x)          ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_DELTA_WID_POS_SEL_5(x)      ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_POS_SEL_4        ((u32)0x0000000F << 16)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[4]. */
#define DDRPHY_DELTA_WID_POS_SEL_4(x)          ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_DELTA_WID_POS_SEL_4(x)      ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_POS_SEL_3        ((u32)0x0000000F << 12)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[3]. */
#define DDRPHY_DELTA_WID_POS_SEL_3(x)          ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_DELTA_WID_POS_SEL_3(x)      ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_POS_SEL_2        ((u32)0x0000000F << 8)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[2]. */
#define DDRPHY_DELTA_WID_POS_SEL_2(x)          ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DELTA_WID_POS_SEL_2(x)      ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_POS_SEL_1        ((u32)0x0000000F << 4)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[1]. */
#define DDRPHY_DELTA_WID_POS_SEL_1(x)          ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DELTA_WID_POS_SEL_1(x)      ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_POS_SEL_0        ((u32)0x0000000F << 0)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dq[0]. 0: shift 1 taps from central point 1: shift 2 taps from central point ……………. 15: shift 16 taps from central point */
#define DDRPHY_DELTA_WID_POS_SEL_0(x)          ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DELTA_WID_POS_SEL_0(x)      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DEL_POS_SEL_DBI_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DELTA_WID_POS_SEL_8        ((u32)0x0000000F << 0)          /*!<R/W ‘h2  Left/right sampling point of positive edge 3-point calibration for dbi. 0: shift 1 taps from central point 1: shift 2 taps from central point ……………. 15: shift 16 taps from central point */
#define DDRPHY_DELTA_WID_POS_SEL_8(x)          ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DELTA_WID_POS_SEL_8(x)      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DEL_NEG_SEL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_7        ((u32)0x0000000F << 28)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[7]. */
#define DDRPHY_DELTA_WID_NEG_SEL_7(x)          ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_7(x)      ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_6        ((u32)0x0000000F << 24)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[6]. */
#define DDRPHY_DELTA_WID_NEG_SEL_6(x)          ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_6(x)      ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_5        ((u32)0x0000000F << 20)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[5]. */
#define DDRPHY_DELTA_WID_NEG_SEL_5(x)          ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_5(x)      ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_4        ((u32)0x0000000F << 16)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[4]. */
#define DDRPHY_DELTA_WID_NEG_SEL_4(x)          ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_4(x)      ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_3        ((u32)0x0000000F << 12)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[3]. */
#define DDRPHY_DELTA_WID_NEG_SEL_3(x)          ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_3(x)      ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_2        ((u32)0x0000000F << 8)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[2]. */
#define DDRPHY_DELTA_WID_NEG_SEL_2(x)          ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_2(x)      ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_1        ((u32)0x0000000F << 4)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[1]. */
#define DDRPHY_DELTA_WID_NEG_SEL_1(x)          ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_1(x)      ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_0        ((u32)0x0000000F << 0)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dq[0]. 0: shift 1 taps from central point 1: shift 2 taps from central point ……………. 15: shift 16 taps from central point */
#define DDRPHY_DELTA_WID_NEG_SEL_0(x)          ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_0(x)      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DEL_NEG_SEL_DBI_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DELTA_WID_NEG_SEL_8        ((u32)0x0000000F << 0)          /*!<R/W ‘h2  Left/right sampling point of negative edge 3-point calibration for dbi. 0: shift 1 taps from central point 1: shift 2 taps from central point ……………. 15: shift 16 taps from central point */
#define DDRPHY_DELTA_WID_NEG_SEL_8(x)          ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DELTA_WID_NEG_SEL_8(x)      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_IN_DLY_0_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_3        ((u32)0x0000001F << 24)          /*!<R/W ‘hA  DQS_IN[3] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_3(x)          ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_3(x)      ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_2        ((u32)0x0000001F << 16)          /*!<R/W ‘hA  DQS_IN[2] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_2(x)          ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_2(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_1        ((u32)0x0000001F << 8)          /*!<R/W ‘hA  DQS_IN[1] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_1(x)          ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_1(x)      ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_0        ((u32)0x0000001F << 0)          /*!<R/W ‘hA  DQS_IN[0] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_0(x)          ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_0(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_IN_DLY_1_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_7        ((u32)0x0000001F << 24)          /*!<R/W ‘hA  DQS_IN[7] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_7(x)          ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_7(x)      ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_6        ((u32)0x0000001F << 16)          /*!<R/W ‘hA  DQS_IN[6] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_6(x)          ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_6(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_5        ((u32)0x0000001F << 8)          /*!<R/W ‘hA  DQS_IN[5] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_5(x)          ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_5(x)      ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_4        ((u32)0x0000001F << 0)          /*!<R/W ‘hA  DQS_IN[4] raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_4(x)          ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_4(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_IN_DLY_1_DBI_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_FW_RD_DLY_POS_SEL_8        ((u32)0x0000001F << 0)          /*!<R/W ‘hA  DBI raising edge delay chain control */
#define DDRPHY_FW_RD_DLY_POS_SEL_8(x)          ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_FW_RD_DLY_POS_SEL_8(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_IN_DLY_2_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_3        ((u32)0x0000001F << 24)          /*!<R/W ‘hA  DQS_IN[3] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_3(x)          ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_3(x)      ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_2        ((u32)0x0000001F << 16)          /*!<R/W ‘hA  DQS_IN[2] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_2(x)          ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_2(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_1        ((u32)0x0000001F << 8)          /*!<R/W ‘hA  DQS_IN[1] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_1(x)          ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_1(x)      ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0        ((u32)0x0000001F << 0)          /*!<R/W ‘hA  DQS_IN[0] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_0(x)          ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_0(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_IN_DLY_3_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_7        ((u32)0x0000001F << 24)          /*!<R/W ‘hA  DQS_IN[7] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_7(x)          ((u32)(((x) & 0x0000001F) << 24))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_7(x)      ((u32)(((x >> 24) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_6        ((u32)0x0000001F << 16)          /*!<R/W ‘hA  DQS_IN[6] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_6(x)          ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_6(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_5        ((u32)0x0000001F << 8)          /*!<R/W ‘hA  DQS_IN[5] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_5(x)          ((u32)(((x) & 0x0000001F) << 8))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_5(x)      ((u32)(((x >> 8) & 0x0000001F)))
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_4        ((u32)0x0000001F << 0)          /*!<R/W ‘hA  DQS_IN[4] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_4(x)          ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_4(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQS_IN_DLY_3_DBI_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8        ((u32)0x0000001F << 0)          /*!<R/W ‘hA  DQS_IN[4] falling edge delay chain control */
#define DDRPHY_FW_RD_DLY_NEG_SEL_8(x)          ((u32)(((x) & 0x0000001F) << 0))
#define DDRPHY_GET_FW_RD_DLY_NEG_SEL_8(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_VALID_WIN_DET_PFIFO_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RX_LAG_DET_PFIFO           ((u32)0x000001FF << 9)          /*!<R/W ‘h0  Lag flag of pfifo valid window detection in data slice0 */
#define DDRPHY_RX_LAG_DET_PFIFO(x)             ((u32)(((x) & 0x000001FF) << 9))
#define DDRPHY_GET_RX_LAG_DET_PFIFO(x)         ((u32)(((x >> 9) & 0x000001FF)))
#define DDRPHY_MASK_RX_LEAD_DET_PFIFO          ((u32)0x000001FF << 0)          /*!<R/W ‘h0  Lead flag of pfifo valid window detection in data slice0 */
#define DDRPHY_RX_LEAD_DET_PFIFO(x)            ((u32)(((x) & 0x000001FF) << 0))
#define DDRPHY_GET_RX_LEAD_DET_PFIFO(x)        ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_VALID_WIN_DET_NFIFO_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RX_LAG_DET_NFIFO           ((u32)0x000001FF << 9)          /*!<R/W ‘h0  Lag flag of nfifo valid window detection in data slice0 */
#define DDRPHY_RX_LAG_DET_NFIFO(x)             ((u32)(((x) & 0x000001FF) << 9))
#define DDRPHY_GET_RX_LAG_DET_NFIFO(x)         ((u32)(((x >> 9) & 0x000001FF)))
#define DDRPHY_MASK_RX_LEAD_DET_NFIFO          ((u32)0x000001FF << 0)          /*!<R/W ‘h0  Lead flag of nfifo valid window detection in data slice0 */
#define DDRPHY_RX_LEAD_DET_NFIFO(x)            ((u32)(((x) & 0x000001FF) << 0))
#define DDRPHY_GET_RX_LEAD_DET_NFIFO(x)        ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_DLY_0_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ7_DLY_SEL                ((u32)0x0000000F << 28)          /*!<R/W ’h0  DQ[7] delay chain control */
#define DDRPHY_DQ7_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 28))
#define DDRPHY_GET_DQ7_DLY_SEL(x)              ((u32)(((x >> 28) & 0x0000000F)))
#define DDRPHY_MASK_DQ6_DLY_SEL                ((u32)0x0000000F << 24)          /*!<R/W ’h0  DQ[6] delay chain control */
#define DDRPHY_DQ6_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_DQ6_DLY_SEL(x)              ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_DQ5_DLY_SEL                ((u32)0x0000000F << 20)          /*!<R/W ’h0  DQ[5] delay chain control */
#define DDRPHY_DQ5_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_DQ5_DLY_SEL(x)              ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_DQ4_DLY_SEL                ((u32)0x0000000F << 16)          /*!<R/W ’h0  DQ[4] delay chain control */
#define DDRPHY_DQ4_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_DQ4_DLY_SEL(x)              ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_DQ3_DLY_SEL                ((u32)0x0000000F << 12)          /*!<R/W ’h0  DQ[3] delay chain control */
#define DDRPHY_DQ3_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_DQ3_DLY_SEL(x)              ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_DQ2_DLY_SEL                ((u32)0x0000000F << 8)          /*!<R/W ’h0  DQ[2] delay chain control */
#define DDRPHY_DQ2_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DQ2_DLY_SEL(x)              ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_DQ1_DLY_SEL                ((u32)0x0000000F << 4)          /*!<R/W ’h0  DQ[1] delay chain control */
#define DDRPHY_DQ1_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DQ1_DLY_SEL(x)              ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_DQ0_DLY_SEL                ((u32)0x0000000F << 0)          /*!<R/W ’h0  DQ[0] delay chain control */
#define DDRPHY_DQ0_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DQ0_DLY_SEL(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_DLY_1_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_OE_DLY_SEL             ((u32)0x0000000F << 8)          /*!<R/W ’h0  Slice DQS output enable delay chain control */
#define DDRPHY_DQS_OE_DLY_SEL(x)               ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DQS_OE_DLY_SEL(x)           ((u32)(((x >> 8) & 0x0000000F)))
#define DDRPHY_MASK_DM_DLY_SEL                 ((u32)0x0000000F << 4)          /*!<R/W ’h0  Slice DM delay chain control */
#define DDRPHY_DM_DLY_SEL(x)                   ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DM_DLY_SEL(x)               ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_DQS_DLY_SEL                ((u32)0x0000000F << 0)          /*!<R/W ’h0  Slice DQS delay chain control */
#define DDRPHY_DQS_DLY_SEL(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DQS_DLY_SEL(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CAL_OUT_0_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CAL_RD_DLY_OUT             ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  3-point sampling result from data slice0 ~ 3, selected by cal_rd_dly_out_* */
#define DDRPHY_CAL_RD_DLY_OUT(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_CAL_RD_DLY_OUT(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_CAL_OUT_1_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CAL_DELTA_OUT              ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  3-point sampling delta width result from data slice0 ~ 3, selected by cal_rd_dly_out_* */
#define DDRPHY_CAL_DELTA_OUT(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_CAL_DELTA_OUT(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_WR_FIFO_0_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_WR_ADR_POS                 ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Write FIFO pointer of raising edge dqs { wr_adr_pos_7[3:0], wr_adr_pos_6[3:0], wr_adr_pos_5[3:0], wr_adr_pos_4[3:0], wr_adr_pos_3[3:0], wr_adr_pos_2[3:0], wr_adr_pos_1[3:0], wr_adr_pos_0[3:0]} */
#define DDRPHY_WR_ADR_POS(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_WR_ADR_POS(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_WR_FIFO_1_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_WR_ADR_NEG                 ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Write FIFO pointer of falling edge dqs {wr_adr_neg_7[3:0], wr_adr_neg_6[3:0], wr_adr_neg_5[3:0], wr_adr_neg_4[3:0], wr_adr_neg_3[3:0], wr_adr_neg_2[3:0], wr_adr_neg_1[3:0], wr_adr_neg_0[3:0]} */
#define DDRPHY_WR_ADR_NEG(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_WR_ADR_NEG(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_RD_FIFO_0_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RD_ADR_POS                 ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Read FIFO pointer of raising edge dqs {rd_adr_pos_7[3:0], rd_adr_pos_6[3:0], rd_adr_pos_5[3:0], rd_adr_pos_4[3:0], rd_adr_pos_3[3:0], rd_adr_pos_2[3:0], rd_adr_pos_1[3:0], rd_adr_pos_0[3:0]} */
#define DDRPHY_RD_ADR_POS(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_RD_ADR_POS(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_RD_FIFO_1_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RD_ADR_NEG                 ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Read FIFO pointer of falling edge dqs {rd_adr_neg_7[3:0], rd_adr_neg_6[3:0], rd_adr_neg_5[3:0], rd_adr_neg_4[3:0], rd_adr_neg_3[3:0], rd_adr_neg_2[3:0], rd_adr_neg_1[3:0], rd_adr_neg_0[3:0]} */
#define DDRPHY_RD_ADR_NEG(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRPHY_GET_RD_ADR_NEG(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DBI_FIFO_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_RW_ADR_DBI                 ((u32)0x0000FFFF << 0)          /*!<R ‘h0  {wr_adr_dbi_pos[3:0], wr_adr_dbi_neg[3:0], rd_adr_dbi_pos[3:0], rd_adr_dbi_neg[3:0]} */
#define DDRPHY_RW_ADR_DBI(x)                   ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRPHY_GET_RW_ADR_DBI(x)               ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_INT_STATUS_1_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_CAL_RBDY_INT1_NEG          ((u32)0x000000FF << 24)          /*!<R/W ‘h0  interrupt of DQS falling edge hit delay tap right boundary in 3-point calibration */
#define DDRPHY_CAL_RBDY_INT1_NEG(x)            ((u32)(((x) & 0x000000FF) << 24))
#define DDRPHY_GET_CAL_RBDY_INT1_NEG(x)        ((u32)(((x >> 24) & 0x000000FF)))
#define DDRPHY_MASK_CAL_LBDY_INT1_NEG          ((u32)0x000000FF << 16)          /*!<R/W ‘h0  interrupt of DQS falling edge hit delay tap left boundary in 3-point calibration */
#define DDRPHY_CAL_LBDY_INT1_NEG(x)            ((u32)(((x) & 0x000000FF) << 16))
#define DDRPHY_GET_CAL_LBDY_INT1_NEG(x)        ((u32)(((x >> 16) & 0x000000FF)))
#define DDRPHY_MASK_CAL_RBDY_INT1_POS          ((u32)0x000000FF << 8)          /*!<R/W ‘h0  interrupt of DQS raising edge hit delay tap right boundary in 3-point calibration */
#define DDRPHY_CAL_RBDY_INT1_POS(x)            ((u32)(((x) & 0x000000FF) << 8))
#define DDRPHY_GET_CAL_RBDY_INT1_POS(x)        ((u32)(((x >> 8) & 0x000000FF)))
#define DDRPHY_MASK_CAL_LBDY_INT1_POS          ((u32)0x000000FF << 0)          /*!<R/W ‘h0  interrupt of DQS raising edge hit delay tap left boundary in 3-point calibration */
#define DDRPHY_CAL_LBDY_INT1_POS(x)            ((u32)(((x) & 0x000000FF) << 0))
#define DDRPHY_GET_CAL_LBDY_INT1_POS(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_INT_STATUS_3_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_CAL_RBDY_INT3_NEG           ((u32)0x00000001 << 21)          /*!<R/W ‘h0  interrupt of DQS falling edge hit delay tap right boundary in DBI 3-point calibration */
#define DDRPHY_CAL_RBDY_INT3_NEG(x)            ((u32)(((x) & 0x00000001) << 21))
#define DDRPHY_BIT_CAL_LBDY_INT3_NEG           ((u32)0x00000001 << 20)          /*!<R/W ‘h0  interrupt of DQS falling edge hit delay tap left boundary in DBI 3-point calibration */
#define DDRPHY_CAL_LBDY_INT3_NEG(x)            ((u32)(((x) & 0x00000001) << 20))
#define DDRPHY_BIT_CAL_RBDY_INT3_POS           ((u32)0x00000001 << 19)          /*!<R/W ‘h0  interrupt of DQS raising edge hit delay tap right boundary in DBI 3-point calibration */
#define DDRPHY_CAL_RBDY_INT3_POS(x)            ((u32)(((x) & 0x00000001) << 19))
#define DDRPHY_BIT_CAL_LBDY_INT3_POS           ((u32)0x00000001 << 18)          /*!<R/W ‘h0  interrupt of DQS raising edge hit delay tap left boundary in DBI 3-point calibration */
#define DDRPHY_CAL_LBDY_INT3_POS(x)            ((u32)(((x) & 0x00000001) << 18))
#define DDRPHY_MASK_FIFO_PTR_INT3_NEG          ((u32)0x000001FF << 9)          /*!<R/W ‘h0  Rx NFIFO pointer error interrupt */
#define DDRPHY_FIFO_PTR_INT3_NEG(x)            ((u32)(((x) & 0x000001FF) << 9))
#define DDRPHY_GET_FIFO_PTR_INT3_NEG(x)        ((u32)(((x >> 9) & 0x000001FF)))
#define DDRPHY_MASK_FIFO_PTR_INT3_POS          ((u32)0x000001FF << 0)          /*!<R/W ‘h0  Rx PFIFO pointer error interrupt */
#define DDRPHY_FIFO_PTR_INT3_POS(x)            ((u32)(((x) & 0x000001FF) << 0))
#define DDRPHY_GET_FIFO_PTR_INT3_POS(x)        ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX4_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DQS_TEST                    ((u32)0x00000001 << 31)          /*!<R/W ‘h0  1: dqs on ,0: dqs off */
#define DDRPHY_DQS_TEST(x)                     ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_DQ5_SEL                    ((u32)0x00000007 << 28)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ5_SEL(x)                      ((u32)(((x) & 0x00000007) << 28))
#define DDRPHY_GET_DQ5_SEL(x)                  ((u32)(((x >> 28) & 0x00000007)))
#define DDRPHY_MASK_DQ4_SEL                    ((u32)0x00000007 << 24)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ4_SEL(x)                      ((u32)(((x) & 0x00000007) << 24))
#define DDRPHY_GET_DQ4_SEL(x)                  ((u32)(((x >> 24) & 0x00000007)))
#define DDRPHY_MASK_DQ3_SEL                    ((u32)0x00000007 << 20)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ3_SEL(x)                      ((u32)(((x) & 0x00000007) << 20))
#define DDRPHY_GET_DQ3_SEL(x)                  ((u32)(((x >> 20) & 0x00000007)))
#define DDRPHY_MASK_DQ2_SEL                    ((u32)0x00000007 << 16)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ2_SEL(x)                      ((u32)(((x) & 0x00000007) << 16))
#define DDRPHY_GET_DQ2_SEL(x)                  ((u32)(((x >> 16) & 0x00000007)))
#define DDRPHY_MASK_DQ1_SEL                    ((u32)0x00000007 << 12)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ1_SEL(x)                      ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_DQ1_SEL(x)                  ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_MASK_DQ0_SEL                    ((u32)0x00000007 << 8)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ0_SEL(x)                      ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DQ0_SEL(x)                  ((u32)(((x >> 8) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_PRBS_TMUX5_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_DATA_PRBS11_INVS            ((u32)0x00000001 << 31)          /*!<R/W ‘h0  inverted PRBS11 for data slice */
#define DDRPHY_DATA_PRBS11_INVS(x)             ((u32)(((x) & 0x00000001) << 31))
#define DDRPHY_MASK_DATA_PRBS11_SEED           ((u32)0x000007FF << 16)          /*!<R/W ‘h1  PRBS11 seed for data slice */
#define DDRPHY_DATA_PRBS11_SEED(x)             ((u32)(((x) & 0x000007FF) << 16))
#define DDRPHY_GET_DATA_PRBS11_SEED(x)         ((u32)(((x >> 16) & 0x000007FF)))
#define DDRPHY_MASK_DM_SEL                     ((u32)0x00000007 << 8)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DM_SEL(x)                       ((u32)(((x) & 0x00000007) << 8))
#define DDRPHY_GET_DM_SEL(x)                   ((u32)(((x >> 8) & 0x00000007)))
#define DDRPHY_MASK_DQ7_SEL                    ((u32)0x00000007 << 4)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ7_SEL(x)                      ((u32)(((x) & 0x00000007) << 4))
#define DDRPHY_GET_DQ7_SEL(x)                  ((u32)(((x >> 4) & 0x00000007)))
#define DDRPHY_MASK_DQ6_SEL                    ((u32)0x00000007 << 0)          /*!<R/W ‘h0  000: force 0 001: force 1 010: PRBS11 011: PRBS11 invert 100: PRBS11 delay one clock cycle 101: from test_pattern register 110: test_pattern invert */
#define DDRPHY_DQ6_SEL(x)                      ((u32)(((x) & 0x00000007) << 0))
#define DDRPHY_GET_DQ6_SEL(x)                  ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_RW_EMPH_CTL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_SLEWRATE               ((u32)0x00000003 << 28)          /*!<R/W ‘h0  DQS slew rate control pin */
#define DDRPHY_DQS_SLEWRATE(x)                 ((u32)(((x) & 0x00000003) << 28))
#define DDRPHY_GET_DQS_SLEWRATE(x)             ((u32)(((x >> 28) & 0x00000003)))
#define DDRPHY_MASK_DQS_DLY_EMPH               ((u32)0x0000000F << 24)          /*!<R/W ‘h0  DQS Delay tap control pin */
#define DDRPHY_DQS_DLY_EMPH(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_DQS_DLY_EMPH(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_DQ_SLEWRATE                ((u32)0x00000003 << 12)          /*!<R/W ‘h0  DQ/DM slew rate control pin */
#define DDRPHY_DQ_SLEWRATE(x)                  ((u32)(((x) & 0x00000003) << 12))
#define DDRPHY_GET_DQ_SLEWRATE(x)              ((u32)(((x >> 12) & 0x00000003)))
#define DDRPHY_MASK_DQ_DLY_EMPH                ((u32)0x0000000F << 8)          /*!<R/W ‘h0  DQ/DM Delay tap control pin */
#define DDRPHY_DQ_DLY_EMPH(x)                  ((u32)(((x) & 0x0000000F) << 8))
#define DDRPHY_GET_DQ_DLY_EMPH(x)              ((u32)(((x >> 8) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_RW_SEL_EMPH_CTL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_SEL_EMPHN              ((u32)0x0000000F << 20)          /*!<R/W ‘h7  DQS SEL Emphasis pin */
#define DDRPHY_DQS_SEL_EMPHN(x)                ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_DQS_SEL_EMPHN(x)            ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_DQS_SEL_EMPHP              ((u32)0x0000000F << 16)          /*!<R/W ‘h7  DQS SEL Emphasis pin */
#define DDRPHY_DQS_SEL_EMPHP(x)                ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_DQS_SEL_EMPHP(x)            ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_DQ_SEL_EMPHN               ((u32)0x0000000F << 4)          /*!<R/W ‘h7  DQ/DM SEL Emphasis pin */
#define DDRPHY_DQ_SEL_EMPHN(x)                 ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_DQ_SEL_EMPHN(x)             ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_DQ_SEL_EMPHP               ((u32)0x0000000F << 0)          /*!<R/W ‘h7  DQ/DM SEL Emphasis pin */
#define DDRPHY_DQ_SEL_EMPHP(x)                 ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_DQ_SEL_EMPHP(x)             ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_IO_CTL_0_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ_OE_SIG                  ((u32)0x000000FF << 24)          /*!<R/W ’h00  FW can config it to control OE of DQ pad 7 : DQ7 of slice : 0 : DQ0 of slice */
#define DDRPHY_DQ_OE_SIG(x)                    ((u32)(((x) & 0x000000FF) << 24))
#define DDRPHY_GET_DQ_OE_SIG(x)                ((u32)(((x >> 24) & 0x000000FF)))
#define DDRPHY_MASK_DQ_OE_FRC_EN               ((u32)0x000000FF << 16)          /*!<R/W ’h00  FW can set 1 to control OE of DQ pad 7 : DQ7 of slice : 0 : DQ0 of slice */
#define DDRPHY_DQ_OE_FRC_EN(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define DDRPHY_GET_DQ_OE_FRC_EN(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define DDRPHY_MASK_DQ_IE_SIG                  ((u32)0x000000FF << 8)          /*!<R/W ’h00  FW can config it to control IE of DQ pad 7 : DQ7 of slice : 0 : DQ0 of slice */
#define DDRPHY_DQ_IE_SIG(x)                    ((u32)(((x) & 0x000000FF) << 8))
#define DDRPHY_GET_DQ_IE_SIG(x)                ((u32)(((x >> 8) & 0x000000FF)))
#define DDRPHY_MASK_DQ_IE_FRC_EN               ((u32)0x000000FF << 0)          /*!<R/W ’h00  FW can set 1 to control IE of DQ pad 7 : DQ7 of slice : 0 : DQ0 of slice */
#define DDRPHY_DQ_IE_FRC_EN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define DDRPHY_GET_DQ_IE_FRC_EN(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_IO_CTL_1_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQ_PAT_OUT                 ((u32)0x000000FF << 24)          /*!<R/W ’h0  dedicated pattern for DQ out 7 : DQ7 of slice : 0 : DQ0 of slice */
#define DDRPHY_DQ_PAT_OUT(x)                   ((u32)(((x) & 0x000000FF) << 24))
#define DDRPHY_GET_DQ_PAT_OUT(x)               ((u32)(((x >> 24) & 0x000000FF)))
#define DDRPHY_BIT_DM_PAT_OUT                  ((u32)0x00000001 << 23)          /*!<R/W ’h0  dedicated pattern for DM out bit0 : DM of slice */
#define DDRPHY_DM_PAT_OUT(x)                   ((u32)(((x) & 0x00000001) << 23))
#define DDRPHY_BIT_DM_OE_SIG                   ((u32)0x00000001 << 3)          /*!<R/W ’h0  FW can config it to control OE of DM pad bit0 : DM of slice */
#define DDRPHY_DM_OE_SIG(x)                    ((u32)(((x) & 0x00000001) << 3))
#define DDRPHY_BIT_DM_OE_FRC_EN                ((u32)0x00000001 << 2)          /*!<R/W ’h0  array[2] */
#define DDRPHY_DM_OE_FRC_EN(x)                 ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_DM_IE_SIG                   ((u32)0x00000001 << 1)          /*!<R/W ’h0  FW can config it to control IE of DM pad bit0 : DM of slice */
#define DDRPHY_DM_IE_SIG(x)                    ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_DM_IE_FRC_EN                ((u32)0x00000001 << 0)          /*!<R/W ’h1  FW can set 1 to control IE of DM pad bit0 : DM of slice */
#define DDRPHY_DM_IE_FRC_EN(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_READ_DBG_CTRL_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_BURST_MON_WAIT_TIME    ((u32)0x0000000F << 26)          /*!<R/W ’h7  Waiting time for monitoring Rx DQS burst length. */
#define DDRPHY_DQS_BURST_MON_WAIT_TIME(x)      ((u32)(((x) & 0x0000000F) << 26))
#define DDRPHY_GET_DQS_BURST_MON_WAIT_TIME(x)  ((u32)(((x >> 26) & 0x0000000F)))
#define DDRPHY_MASK_RD_DBG_SEL1                ((u32)0x0000001F << 21)          /*!<R/W ‘h0  Debug selection for data slice */
#define DDRPHY_RD_DBG_SEL1(x)                  ((u32)(((x) & 0x0000001F) << 21))
#define DDRPHY_GET_RD_DBG_SEL1(x)              ((u32)(((x >> 21) & 0x0000001F)))
#define DDRPHY_MASK_RD_DBG_SEL0                ((u32)0x0000001F << 16)          /*!<R/W ’h0  Debug selection for data slice */
#define DDRPHY_RD_DBG_SEL0(x)                  ((u32)(((x) & 0x0000001F) << 16))
#define DDRPHY_GET_RD_DBG_SEL0(x)              ((u32)(((x >> 16) & 0x0000001F)))
#define DDRPHY_MASK_CAL_RD_DLY_OUT_SEL         ((u32)0x00000007 << 12)          /*!<R/W ‘h0  3-point sampling result selection for data slice 000: dq[1:0] of data slice 001: dq[3:2] of data slice 010: dq[5:4] of data slice 011: dq[7:6] of data slice 100: dq[8] of data slice */
#define DDRPHY_CAL_RD_DLY_OUT_SEL(x)           ((u32)(((x) & 0x00000007) << 12))
#define DDRPHY_GET_CAL_RD_DLY_OUT_SEL(x)       ((u32)(((x >> 12) & 0x00000007)))
#define DDRPHY_BIT_TX_WDATA_MASK               ((u32)0x00000001 << 10)          /*!<R/W ‘b0  b1 : Slice TX wdata mask to h00 */
#define DDRPHY_TX_WDATA_MASK(x)                ((u32)(((x) & 0x00000001) << 10))
#define DDRPHY_MASK_DQS_DM_PD_EN               ((u32)0x00000003 << 8)          /*!<R/W ‘h0  slice [DQS, DM] PAD power-down b0 : normal, b1: power-down */
#define DDRPHY_DQS_DM_PD_EN(x)                 ((u32)(((x) & 0x00000003) << 8))
#define DDRPHY_GET_DQS_DM_PD_EN(x)             ((u32)(((x >> 8) & 0x00000003)))
#define DDRPHY_MASK_DQ_PD_EN                   ((u32)0x000000FF << 0)          /*!<R/W ‘h0  slice DQ[7:0] PAD power-down b0 : normal, b1: power-down */
#define DDRPHY_DQ_PD_EN(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define DDRPHY_GET_DQ_PD_EN(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_MON_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_BIT_CAL_FTUN_UNDER_FLG          ((u32)0x00000001 << 30)          /*!<R ‘h0  dqs_en calibration under flow, don’t move ftun value. Clear by fw_clr_dqs_int. */
#define DDRPHY_CAL_FTUN_UNDER_FLG(x)           ((u32)(((x) & 0x00000001) << 30))
#define DDRPHY_BIT_CAL_FTUN_OVER_FLG           ((u32)0x00000001 << 29)          /*!<R ‘h0  dqs_en calibration over flow, don’t move ftun value. Clear by fw_clr_dqs_int. */
#define DDRPHY_CAL_FTUN_OVER_FLG(x)            ((u32)(((x) & 0x00000001) << 29))
#define DDRPHY_BIT_CAL_FTUN_BOTH_HIT_FLG       ((u32)0x00000001 << 28)          /*!<R ‘h0  dqs_en calibration bot hit eraly and late, don’t move ftun value. Clear by fw_clr_dqs_int. This flag only change at dqsen_cal_sel = 0. */
#define DDRPHY_CAL_FTUN_BOTH_HIT_FLG(x)        ((u32)(((x) & 0x00000001) << 28))
#define DDRPHY_MASK_FW_TM_DQS_EN_FTUN          ((u32)0x0000000F << 24)          /*!<R ‘h3  Slice, Internal double buffer value of tm_dqs_en_ftun. */
#define DDRPHY_FW_TM_DQS_EN_FTUN(x)            ((u32)(((x) & 0x0000000F) << 24))
#define DDRPHY_GET_FW_TM_DQS_EN_FTUN(x)        ((u32)(((x >> 24) & 0x0000000F)))
#define DDRPHY_MASK_DQS_EN_DLY_SEL_PH          ((u32)0x0000003F << 16)          /*!<R ‘h10  Central sampling point setting of slice0 3-point calibration for DQS enable. */
#define DDRPHY_DQS_EN_DLY_SEL_PH(x)            ((u32)(((x) & 0x0000003F) << 16))
#define DDRPHY_GET_DQS_EN_DLY_SEL_PH(x)        ((u32)(((x >> 16) & 0x0000003F)))
#define DDRPHY_MASK_MON_BURST_CNT              ((u32)0x0000000F << 4)          /*!<R ‘h0  debug monitor : slice burst_cnt */
#define DDRPHY_MON_BURST_CNT(x)                ((u32)(((x) & 0x0000000F) << 4))
#define DDRPHY_GET_MON_BURST_CNT(x)            ((u32)(((x >> 4) & 0x0000000F)))
#define DDRPHY_MASK_MON_DQS_CNT                ((u32)0x0000000F << 0)          /*!<R ‘h0  debug monitor : slice dqs_cnt */
#define DDRPHY_MON_DQS_CNT(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define DDRPHY_GET_MON_DQS_CNT(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_DQ_IO_CTL_2_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQM_OE_TIME                ((u32)0x0000000F << 12)          /*!<R/W ‘h0  [1:0] Control dq/dm oe enable timing. 00: original 01: pre 0.5T enable 10: pre 1T enable 11: pre 1.5T enable [3:2] Control dq/dm oe disable timing. 00: original 01: post 1T disable 11: pre 1T (0.5T if en_crc = 1) disable */
#define DDRPHY_DQM_OE_TIME(x)                  ((u32)(((x) & 0x0000000F) << 12))
#define DDRPHY_GET_DQM_OE_TIME(x)              ((u32)(((x >> 12) & 0x0000000F)))
#define DDRPHY_MASK_DATA_INV                   ((u32)0x000001FF << 0)          /*!<R/W ’h0  Invert output value for Tx/Rx DQ/DM 8 : DM of slice 7 : DQ7 of slice : 0 : DQ0 of slice */
#define DDRPHY_DATA_INV(x)                     ((u32)(((x) & 0x000001FF) << 0))
#define DDRPHY_GET_DATA_INV(x)                 ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_READ_CTRL_4_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_BURST_OBVS_NUM             ((u32)0x0000003F << 5)          /*!<R/W ‘h2  Control the observed continuous cal_chg_rd_dly signal number before increase Delta value. */
#define DDRPHY_BURST_OBVS_NUM(x)               ((u32)(((x) & 0x0000003F) << 5))
#define DDRPHY_GET_BURST_OBVS_NUM(x)           ((u32)(((x >> 5) & 0x0000003F)))
#define DDRPHY_BIT_DELTA_CAL_MODE              ((u32)0x00000001 << 4)          /*!<R/W ‘h0  1: Increase Delta value after observe (burst_obvs_num x256) continuous cal_chg_rd_dly signals with increasing Delta. */
#define DDRPHY_DELTA_CAL_MODE(x)               ((u32)(((x) & 0x00000001) << 4))
#define DDRPHY_BIT_FW_DQSEN_FTUN_UPD           ((u32)0x00000001 << 2)          /*!<R/W ‘h1  1: tm_dqs_en_ftun is applicable */
#define DDRPHY_FW_DQSEN_FTUN_UPD(x)            ((u32)(((x) & 0x00000001) << 2))
#define DDRPHY_BIT_FW_RD_DELTA_UPD             ((u32)0x00000001 << 1)          /*!<R/W ‘h1  1: delta_wid_*_sel are applicable */
#define DDRPHY_FW_RD_DELTA_UPD(x)              ((u32)(((x) & 0x00000001) << 1))
#define DDRPHY_BIT_FW_RD_TE_UPD                ((u32)0x00000001 << 0)          /*!<R/W ‘h1  1: fw_rd_dly_*_sel are applicable */
#define DDRPHY_FW_RD_TE_UPD(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/**************************************************************************//**
 * @defgroup DDRPHY_READ_CTRL_0_Setx
 * @brief
 * @{
 *****************************************************************************/
#define DDRPHY_MASK_DQS_RC_DLY_SEL             ((u32)0x0000001F << 26)          /*!<R/W ‘h0  dqs clock to receive_1st delay control */
#define DDRPHY_DQS_RC_DLY_SEL(x)               ((u32)(((x) & 0x0000001F) << 26))
#define DDRPHY_GET_DQS_RC_DLY_SEL(x)           ((u32)(((x >> 26) & 0x0000001F)))
#define DDRPHY_MASK_DQS_GLITCH_DLY_PH          ((u32)0x0000000F << 20)          /*!<R/W ‘h0  Control delay taps to detect DQS glitch. */
#define DDRPHY_DQS_GLITCH_DLY_PH(x)            ((u32)(((x) & 0x0000000F) << 20))
#define DDRPHY_GET_DQS_GLITCH_DLY_PH(x)        ((u32)(((x >> 20) & 0x0000000F)))
#define DDRPHY_MASK_TM_DQ_LAT_DLY              ((u32)0x0000000F << 16)          /*!<R/W ’h0  programmable dq_lat_en_cmd start time 2T DCK unit for 2to1 mode 1T DCK unit for 1to1 mode */
#define DDRPHY_TM_DQ_LAT_DLY(x)                ((u32)(((x) & 0x0000000F) << 16))
#define DDRPHY_GET_TM_DQ_LAT_DLY(x)            ((u32)(((x >> 16) & 0x0000000F)))
#define DDRPHY_MASK_DQS_FIFO_DLY_SEL           ((u32)0x00000003 << 14)          /*!<R/W ‘h0  dqs clock to read FIFO delay control */
#define DDRPHY_DQS_FIFO_DLY_SEL(x)             ((u32)(((x) & 0x00000003) << 14))
#define DDRPHY_GET_DQS_FIFO_DLY_SEL(x)         ((u32)(((x >> 14) & 0x00000003)))
#define DDRPHY_MASK_DQS_EN_DLY_SEL             ((u32)0x0000003F << 8)          /*!<R/W ‘h0  dqs_en delay chain control */
#define DDRPHY_DQS_EN_DLY_SEL(x)               ((u32)(((x) & 0x0000003F) << 8))
#define DDRPHY_GET_DQS_EN_DLY_SEL(x)           ((u32)(((x >> 8) & 0x0000003F)))
#define DDRPHY_MASK_TM_DQS_EN                  ((u32)0x0000001F << 2)          /*!<R/W ’h0  dqs_en latency from read command 2T DCK unit for 2to1 mode 1T DCK unit for 1to1 mode */
#define DDRPHY_TM_DQS_EN(x)                    ((u32)(((x) & 0x0000001F) << 2))
#define DDRPHY_GET_TM_DQS_EN(x)                ((u32)(((x >> 2) & 0x0000001F)))
#define DDRPHY_MASK_TM_DQS_EN_FTUN             ((u32)0x00000003 << 0)          /*!<R/W ’h3  dqs_en_ftun latency from read command 0.5T DCK unit */
#define DDRPHY_TM_DQS_EN_FTUN(x)               ((u32)(((x) & 0x00000003) << 0))
#define DDRPHY_GET_TM_DQS_EN_FTUN(x)           ((u32)(((x >> 0) & 0x00000003)))
/** @} */
/** @} */

/* Exported types --------------------------------------------------------*/

/** @defgroup DDRPHY_Exported_Types DDRPHY Exported Types
  * @{
*****************************************************************************/

/** @brief DDRPHY port
*****************************************************************************/
typedef struct {

	__IO uint32_t DDRPHY_PAD_ALERT_x_SEL_0;               /*!< Register */
	__IO uint32_t DDRPHY_PAD_ALERT_x_SEL_1;               /*!< Register */
} PAD_ALERT_SEL_TypeDef;

/** @} */



/**************************************************************************//**
 * @defgroup AMEBAD2_DDRPHY
 * @{
 * @brief AMEBAD2_DDRPHY Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t DDRPHY_CRT_CTL;                         /*!<  Register,  Address offset: 0X000 */
	__IO uint32_t DDRPHY_PLL_CTL0;                        /*!<  Register,  Address offset: 0X004 */
	__IO uint32_t DDRPHY_PLL_CTL1;                        /*!<  Register,  Address offset: 0X008 */
	__IO uint32_t DDRPHY_PLL_CTL2;                        /*!<  Register,  Address offset: 0X00C */
	__IO uint32_t DDRPHY_PLL_PI0;                         /*!<  Register,  Address offset: 0X010 */
	__IO uint32_t DDRPHY_PLL_PI1;                         /*!<  Register,  Address offset: 0X014 */
	__IO uint32_t DDRPHY_PLL_CTL3;                        /*!<  Register,  Address offset: 0X018 */
	__IO uint32_t DDRPHY_SSC0;                            /*!<  Register,  Address offset: 0X01C */
	__IO uint32_t DDRPHY_SSC1;                            /*!<  Register,  Address offset: 0X020 */
	__IO uint32_t DDRPHY_SSC2;                            /*!<  Register,  Address offset: 0X024 */
	__IO uint32_t DDRPHY_SSC3;                            /*!<  Register,  Address offset: 0X028 */
	__IO uint32_t DDRPHY_WDOG;                            /*!<  Register,  Address offset: 0X02C */
	__IO uint32_t DDRPHY_DCC_CMD;                         /*!<  Register,  Address offset: 0X030 */
	__IO uint32_t DDRPHY_DCC_DQS_SETx[4];                 /*!<  Register,  Address offset: 0X034-0X040 */
	__IO uint32_t DDRPHY_DCC_DQ_SETx[4];                  /*!<  Register,  Address offset: 0X044-0X050 */
	__IO uint32_t DDRPHY_DCC_CK_SETx[2];                  /*!<  Register,  Address offset: 0X054-0X058 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x5C */
	__IO uint32_t DDRPHY_CRT_CTL_2;                       /*!<  Register,  Address offset: 0X060 */
	__IO uint32_t RSVD1[5];                               /*!<  Reserved,  Address offset:0x64-0x74 */
	__IO uint32_t DDRPHY_CRT_DMY;                         /*!<  Register,  Address offset: 0X078 */
	__IO uint32_t RSVD2;                                  /*!<  Reserved,  Address offset:0x7C */
	__IO uint32_t DDRPHY_AFIFO_STR_0;                     /*!<  Register,  Address offset: 0X080 */
	__IO uint32_t DDRPHY_AFIFO_STR_1;                     /*!<  Register,  Address offset: 0X084 */
	__IO uint32_t DDRPHY_AFIFO_STR_2;                     /*!<  Register,  Address offset: 0X088 */
	__IO uint32_t DDRPHY_CRT_RST_CTL;                     /*!<  Register,  Address offset: 0X08C */
	__IO uint32_t RSVD3;                                  /*!<  Reserved,  Address offset:0x90 */
	__IO uint32_t DDRPHY_AFIFO_STR_SEL;                   /*!<  Register,  Address offset: 0X094 */
	__IO uint32_t DDRPHY_PLL_PI2;                         /*!<  Register,  Address offset: 0X098 */
	__IO uint32_t DDRPHY_PLL_CTL4;                        /*!<  Register,  Address offset: 0X09C */
	__IO uint32_t DDRPHY_PLL_PI3;                         /*!<  Register,  Address offset: 0X0A0 */
	__IO uint32_t DDRPHY_DCC_CSx[2];                      /*!<  Register,  Address offset: 0X0A4-0X0A8 */
	__IO uint32_t DDRPHY_PLL_CTL5;                        /*!<  Register,  Address offset: 0X0AC */
	__IO uint32_t RSVD4[20];                              /*!<  Reserved,  Address offset:0xB0-0xFC */
	__IO uint32_t DDRPHY_ADR_DLY_0;                       /*!<  Register,  Address offset: 0X100 */
	__IO uint32_t DDRPHY_ADR_DLY_1;                       /*!<  Register,  Address offset: 0X104 */
	__IO uint32_t DDRPHY_ADR_DLY_2;                       /*!<  Register,  Address offset: 0X108 */
	__IO uint32_t DDRPHY_CMD_DLY_0;                       /*!<  Register,  Address offset: 0X10C */
	__IO uint32_t DDRPHY_CMD_DLY_1;                       /*!<  Register,  Address offset: 0X110 */
	__IO uint32_t DDRPHY_PAD_OE_CTL;                      /*!<  Register,  Address offset: 0X114 */
	__IO uint32_t DDRPHY_PAD_OE_CTL_EX;                   /*!<  Register,  Address offset: 0X118 */
	PAD_ALERT_SEL_TypeDef ALERT_SEL[2];                   /*!< DDRPHY IP have 2 ports,  Address offset: 0X11C-0X128 */
	__IO uint32_t DDRPHY_PAD_IDDQ;                        /*!<  Register,  Address offset: 0X12C */
	__IO uint32_t DDRPHY_PAD_CTRL_PROG;                   /*!<  Register,  Address offset: 0X130 */
	__IO uint32_t RSVD5[2];                               /*!<  Reserved,  Address offset:0x134-0x138 */
	__IO uint32_t DDRPHY_ZQ_NOCD2;                        /*!<  Register,  Address offset: 0X13C */
	__IO uint32_t DDRPHY_PAD_ZCTRL_STATUS;                /*!<  Register,  Address offset: 0X140 */
	__I  uint32_t DDRPHY_PAD_ZCTRL_RESULT;                /*!<  Register,  Address offset: 0X144 */
	__I  uint32_t DDRPHY_PAD_ZCTRL_RESULT_D;              /*!<  Register,  Address offset: 0X148 */
	__IO uint32_t DDRPHY_PAD_RZCTRL_STATUS;               /*!<  Register,  Address offset: 0X14C */
	__IO uint32_t DDRPHY_DQ_DQS_CTRL;                     /*!<  Register,  Address offset: 0X150 */
	__IO uint32_t DDRPHY_CMD_ADR_PH;                      /*!<  Register,  Address offset: 0X154 */
	__I  uint32_t DDRPHY_DQ_PAT_IN0;                      /*!<  Register,  Address offset: 0X158 */
	__I  uint32_t DDRPHY_DQ_PAT_IN1;                      /*!<  Register,  Address offset: 0X15C */
	__I  uint32_t DDRPHY_WRLVL_DQS_DATA;                  /*!<  Register,  Address offset: 0X160 */
	__IO uint32_t DDRPHY_TEST_CTRL0;                      /*!<  Register,  Address offset: 0X164 */
	__IO uint32_t DDRPHY_TEST_CTRL1;                      /*!<  Register,  Address offset: 0X168 */
	__IO uint32_t DDRPHY_HALF_CYC_DET;                    /*!<  Register,  Address offset: 0X16C */
	__IO uint32_t DDRPHY_READ_CTRL_1;                     /*!<  Register,  Address offset: 0X170 */
	__IO uint32_t DDRPHY_TX_RPT_CTRL;                     /*!<  Register,  Address offset: 0X174 */
	__IO uint32_t DDRPHY_CAL_SHIFT_CTRL;                  /*!<  Register,  Address offset: 0X178 */
	__IO uint32_t DDRPHY_DT_SET0;                         /*!<  Register,  Address offset: 0X17C */
	__IO uint32_t DDRPHY_ODT_TTCP0_SET0;                  /*!<  Register,  Address offset: 0X180 */
	__IO uint32_t DDRPHY_ODT_TTCP1_SET0;                  /*!<  Register,  Address offset: 0X184 */
	__IO uint32_t DDRPHY_ODT_TTCN0_SET0;                  /*!<  Register,  Address offset: 0X188 */
	__IO uint32_t DDRPHY_ODT_TTCN1_SET0;                  /*!<  Register,  Address offset: 0X18C */
	__IO uint32_t DDRPHY_OCDP0_SET0;                      /*!<  Register,  Address offset: 0X190 */
	__IO uint32_t DDRPHY_OCDP1_SET0;                      /*!<  Register,  Address offset: 0X194 */
	__IO uint32_t DDRPHY_OCDN0_SET0;                      /*!<  Register,  Address offset: 0X198 */
	__IO uint32_t DDRPHY_OCDN1_SET0;                      /*!<  Register,  Address offset: 0X19C */
	__IO uint32_t DDRPHY_ODT_TTCP0_SET1;                  /*!<  Register,  Address offset: 0X1A0 */
	__IO uint32_t DDRPHY_ODT_TTCP1_SET1;                  /*!<  Register,  Address offset: 0X1A4 */
	__IO uint32_t DDRPHY_ODT_TTCN0_SET1;                  /*!<  Register,  Address offset: 0X1A8 */
	__IO uint32_t DDRPHY_ODT_TTCN1_SET1;                  /*!<  Register,  Address offset: 0X1AC */
	__IO uint32_t DDRPHY_OCDP0_SET1;                      /*!<  Register,  Address offset: 0X1B0 */
	__IO uint32_t DDRPHY_OCDP1_SET1;                      /*!<  Register,  Address offset: 0X1B4 */
	__IO uint32_t DDRPHY_OCDN0_SET1;                      /*!<  Register,  Address offset: 0X1B8 */
	__IO uint32_t DDRPHY_OCDN1_SET1;                      /*!<  Register,  Address offset: 0X1BC */
	__IO uint32_t DDRPHY_DQ_ODT_SEL_SETx[2];              /*!<  Register,  Address offset: 0X1C0-0X1C4 */
	__IO uint32_t DDRPHY_DQ_OCD_SEL_SETx[2];              /*!<  Register,  Address offset: 0X1C8-0X1CC */
	__IO uint32_t DDRPHY_DQS_P_ODT_SEL_SETx[2];           /*!<  Register,  Address offset: 0X1D0-0X1D4 */
	__IO uint32_t DDRPHY_DQS_N_ODT_SEL_SETx[2];           /*!<  Register,  Address offset: 0X1D8-0X1DC */
	__IO uint32_t DDRPHY_DQS_OCD_SEL_SETx[2];             /*!<  Register,  Address offset: 0X1E0-0X1E4 */
	__IO uint32_t DDRPHY_CKE_OCD_SEL;                     /*!<  Register,  Address offset: 0X1E8 */
	__IO uint32_t DDRPHY_ADR_OCD_SEL;                     /*!<  Register,  Address offset: 0X1EC */
	__IO uint32_t DDRPHY_CK_OCD_SEL;                      /*!<  Register,  Address offset: 0X1F0 */
	__IO uint32_t DDRPHY_PAD_BUS_0;                       /*!<  Register,  Address offset: 0X1F4 */
	__IO uint32_t DDRPHY_PAD_BUS_1;                       /*!<  Register,  Address offset: 0X1F8 */
	__IO uint32_t DDRPHY_PAD_BUS_2;                       /*!<  Register,  Address offset: 0X1FC */
	__IO uint32_t DDRPHY_ZQ_TRIM;                         /*!<  Register,  Address offset: 0X200 */
	__IO uint32_t DDRPHY_OCD_SEL_0;                       /*!<  Register,  Address offset: 0X204 */
	__IO uint32_t DDRPHY_OCD_SEL_1;                       /*!<  Register,  Address offset: 0X208 */
	__IO uint32_t DDRPHY_OCD_SEL_2;                       /*!<  Register,  Address offset: 0X20C */
	__IO uint32_t DDRPHY_OCD_SEL_3;                       /*!<  Register,  Address offset: 0X210 */
	__IO uint32_t DDRPHY_OCD_SEL_4;                       /*!<  Register,  Address offset: 0X214 */
	__IO uint32_t DDRPHY_OCD_SEL_5;                       /*!<  Register,  Address offset: 0X218 */
	__IO uint32_t DDRPHY_OCD_SEL_6;                       /*!<  Register,  Address offset: 0X21C */
	__IO uint32_t DDRPHY_OCD_SEL_7;                       /*!<  Register,  Address offset: 0X220 */
	__IO uint32_t DDRPHY_OCD_SEL_8;                       /*!<  Register,  Address offset: 0X224 */
	__IO uint32_t DDRPHY_ODT_SEL_0;                       /*!<  Register,  Address offset: 0X228 */
	__IO uint32_t DDRPHY_CKE_ODT_SEL;                     /*!<  Register,  Address offset: 0X22C */
	__IO uint32_t DDRPHY_ADR_ODT_SEL;                     /*!<  Register,  Address offset: 0X230 */
	__IO uint32_t DDRPHY_CLK_ODT_SEL;                     /*!<  Register,  Address offset: 0X234 */
	__IO uint32_t DDRPHY_DPI_CTRL_0;                      /*!<  Register,  Address offset: 0X238 */
	__IO uint32_t DDRPHY_DPI_CTRL_1;                      /*!<  Register,  Address offset: 0X23C */
	__IO uint32_t DDRPHY_DPI_CTRL_2;                      /*!<  Register,  Address offset: 0X240 */
	__IO uint32_t DDRPHY_BIST_2TO1_0;                     /*!<  Register,  Address offset: 0X244 */
	__I  uint32_t DDRPHY_BIST_2TO1_1;                     /*!<  Register,  Address offset: 0X248 */
	__IO uint32_t DDRPHY_BIST_PT;                         /*!<  Register,  Address offset: 0X24C */
	__I  uint32_t DDRPHY_BIST_DEBUG;                      /*!<  Register,  Address offset: 0X250 */
	__IO uint32_t DDRPHY_INT_CTRL;                        /*!<  Register,  Address offset: 0X254 */
	__IO uint32_t DDRPHY_INT_STATUS_0;                    /*!<  Register,  Address offset: 0X258 */
	__IO uint32_t DDRPHY_INT_STATUS_2;                    /*!<  Register,  Address offset: 0X25C */
	__I  uint32_t DDRPHY_SSC_STATUS;                      /*!<  Register,  Address offset: 0X260 */
	__IO uint32_t DDRPHY_PRBS_TMUX0;                      /*!<  Register,  Address offset: 0X264 */
	__IO uint32_t DDRPHY_PRBS_TMUX1;                      /*!<  Register,  Address offset: 0X268 */
	__IO uint32_t DDRPHY_PRBS_TMUX2;                      /*!<  Register,  Address offset: 0X26C */
	__IO uint32_t DDRPHY_PRBS_TMUX3;                      /*!<  Register,  Address offset: 0X270 */
	__IO uint32_t DDRPHY_PRBS_TMUX6;                      /*!<  Register,  Address offset: 0X274 */
	__IO uint32_t DDRPHY_PRBS_TMUX7;                      /*!<  Register,  Address offset: 0X278 */
	__IO uint32_t DDRPHY_PRBS_TMUX8;                      /*!<  Register,  Address offset: 0X27C */
	__IO uint32_t DDRPHY_PRBS_TMUX9;                      /*!<  Register,  Address offset: 0X280 */
	__IO uint32_t DDRPHY_PRBS_TMUX10;                     /*!<  Register,  Address offset: 0X284 */
	__IO uint32_t DDRPHY_DUMMY_0;                         /*!<  Register,  Address offset: 0X288 */
	__IO uint32_t DDRPHY_DUMMY_1;                         /*!<  Register,  Address offset: 0X28C */
	__IO uint32_t DDRPHY_DUMMY_2;                         /*!<  Register,  Address offset: 0X290 */
	__IO uint32_t DDRPHY_DUMMY_3;                         /*!<  Register,  Address offset: 0X294 */
	__IO uint32_t DDRPHY_CS_EMPH_CTL;                     /*!<  Register,  Address offset: 0X298 */
	__IO uint32_t DDRPHY_CS_SEL_EMPH_CTL;                 /*!<  Register,  Address offset: 0X29C */
	__IO uint32_t DDRPHY_DDR4_CS_EMPH_CTL;                /*!<  Register,  Address offset: 0X2A0 */
	__IO uint32_t DDRPHY_CK_EMPH_CTL;                     /*!<  Register,  Address offset: 0X2A4 */
	__IO uint32_t DDRPHY_CK_SEL_EMPH_CTL;                 /*!<  Register,  Address offset: 0X2A8 */
	__IO uint32_t DDRPHY_ADR_EMPH_CTL_SETx[2];            /*!<  Register,  Address offset: 0X2AC-0X2B0 */
	__IO uint32_t DDRPHY_ADR_SEL_EMPH_CTL_SETx[2];        /*!<  Register,  Address offset: 0X2B4-0X2B8 */
	__IO uint32_t DDRPHY_ADR_EMPH_SEL_0;                  /*!<  Register,  Address offset: 0X2BC */
	__IO uint32_t DDRPHY_ADR_EMPH_SEL_1;                  /*!<  Register,  Address offset: 0X2C0 */
	__IO uint32_t DDRPHY_ADR_EMPH_SEL_2;                  /*!<  Register,  Address offset: 0X2C4 */
	__I  uint32_t DDRPHY_DBG_READ_0;                      /*!<  Register,  Address offset: 0X2C8 */
	__I  uint32_t DDRPHY_DBG_READ_1;                      /*!<  Register,  Address offset: 0X2CC */
	__IO uint32_t DDRPHY_RX_CAL_CTRL_0;                   /*!<  Register,  Address offset: 0X2D0 */
	__IO uint32_t DDRPHY_RX_CAL_CTRL_1;                   /*!<  Register,  Address offset: 0X2D4 */
	__IO uint32_t DDRPHY_VREF_BDY_0;                      /*!<  Register,  Address offset: 0X2D8 */
	__IO uint32_t DDRPHY_VREF_BDY_1;                      /*!<  Register,  Address offset: 0X2DC */
	__IO uint32_t DDRPHY_DELTA_WID_CTRL;                  /*!<  Register,  Address offset: 0X2E0 */
	__I  uint32_t DDRPHY_RX_CAL_MON;                      /*!<  Register,  Address offset: 0X2E4 */
	__IO uint32_t DDRPHY_PAD_CTRL_ZPROG_SETx[8];          /*!<  Register,  Address offset: 0X2E8-0X304 */
	__IO uint32_t DDRPHY_PAD_NOCD2_ZPROG_SETx[8];         /*!<  Register,  Address offset: 0X308-0X324 */
	__IO uint32_t DDRPHY_DQ_OCD_SEL_2_SETx[2];            /*!<  Register,  Address offset: 0X328-0X32C */
	__IO uint32_t DDRPHY_DQS_OCD_SEL_2_SETx[2];           /*!<  Register,  Address offset: 0X330-0X334 */
	__I  uint32_t DDRPHY_DBG_READ_2;                      /*!<  Register,  Address offset: 0X338 */
	__IO uint32_t DDRPHY_DATA_VAL_CTRL_0;                 /*!<  Register,  Address offset: 0X33C */
	__IO uint32_t DDRPHY_DATA_VAL_CTRL_1;                 /*!<  Register,  Address offset: 0X340 */
	__IO uint32_t DDRPHY_DATA_VAL_CTRL_2;                 /*!<  Register,  Address offset: 0X344 */
	__IO uint32_t RSVD6[48];                              /*!<  Reserved,  Address offset:0x348-0x404 */
	__IO uint32_t DDRPHY_READ_CTRL_2_SETx[2];             /*!<  Register,  Address offset: 0X408-0X40C */
	__IO uint32_t DDRPHY_READ_CTRL_3_SETx[2];             /*!<  Register,  Address offset: 0X410-0X414 */
	__IO uint32_t DDRPHY_READ_CTRL_6_SETx[2];             /*!<  Register,  Address offset: 0X418-0X41C */
	__IO uint32_t DDRPHY_DEL_POS_SEL_SETx[2];             /*!<  Register,  Address offset: 0X420-0X424 */
	__IO uint32_t DDRPHY_DEL_POS_SEL_DBI_SETx[2];         /*!<  Register,  Address offset: 0X428-0X42C */
	__IO uint32_t DDRPHY_DEL_NEG_SEL_SETx[2];             /*!<  Register,  Address offset: 0X430-0X434 */
	__IO uint32_t DDRPHY_DEL_NEG_SEL_DBI_SETx[2];         /*!<  Register,  Address offset: 0X438-0X43C */
	__IO uint32_t DDRPHY_DQS_IN_DLY_0_SETx[2];            /*!<  Register,  Address offset: 0X440-0X444 */
	__IO uint32_t DDRPHY_DQS_IN_DLY_1_SETx[2];            /*!<  Register,  Address offset: 0X448-0X44C */
	__IO uint32_t DDRPHY_DQS_IN_DLY_1_DBI_SETx[2];        /*!<  Register,  Address offset: 0X450-0X454 */
	__IO uint32_t DDRPHY_DQS_IN_DLY_2_SETx[2];            /*!<  Register,  Address offset: 0X458-0X45C */
	__IO uint32_t DDRPHY_DQS_IN_DLY_3_SETx[2];            /*!<  Register,  Address offset: 0X460-0X464 */
	__IO uint32_t DDRPHY_DQS_IN_DLY_3_DBI_SETx[2];        /*!<  Register,  Address offset: 0X468-0X46C */
	__IO uint32_t DDRPHY_VALID_WIN_DET_PFIFO_SETx[2];     /*!<  Register,  Address offset: 0X470-0X474 */
	__IO uint32_t DDRPHY_VALID_WIN_DET_NFIFO_SETx[2];     /*!<  Register,  Address offset: 0X478-0X47C */
	__IO uint32_t DDRPHY_DQ_DLY_0_SETx[2];                /*!<  Register,  Address offset: 0X480-0X484 */
	__IO uint32_t DDRPHY_DQ_DLY_1_SETx[2];                /*!<  Register,  Address offset: 0X488-0X48C */
	__I  uint32_t DDRPHY_CAL_OUT_0_SETx[2];               /*!<  Register,  Address offset: 0X490-0X494 */
	__I  uint32_t DDRPHY_CAL_OUT_1_SETx[2];               /*!<  Register,  Address offset: 0X498-0X49C */
	__I  uint32_t DDRPHY_WR_FIFO_0_SETx[2];               /*!<  Register,  Address offset: 0X4A0-0X4A4 */
	__I  uint32_t DDRPHY_WR_FIFO_1_SETx[2];               /*!<  Register,  Address offset: 0X4A8-0X4AC */
	__I  uint32_t DDRPHY_RD_FIFO_0_SETx[2];               /*!<  Register,  Address offset: 0X4B0-0X4B4 */
	__I  uint32_t DDRPHY_RD_FIFO_1_SETx[2];               /*!<  Register,  Address offset: 0X4B8-0X4BC */
	__I  uint32_t DDRPHY_DBI_FIFO_SETx[2];                /*!<  Register,  Address offset: 0X4C0-0X4C4 */
	__IO uint32_t DDRPHY_INT_STATUS_1_SETx[2];            /*!<  Register,  Address offset: 0X4C8-0X4CC */
	__IO uint32_t DDRPHY_INT_STATUS_3_SETx[2];            /*!<  Register,  Address offset: 0X4D0-0X4D4 */
	__IO uint32_t DDRPHY_PRBS_TMUX4_SETx[2];              /*!<  Register,  Address offset: 0X4D8-0X4DC */
	__IO uint32_t DDRPHY_PRBS_TMUX5_SETx[2];              /*!<  Register,  Address offset: 0X4E0-0X4E4 */
	__IO uint32_t DDRPHY_RW_EMPH_CTL_SETx[2];             /*!<  Register,  Address offset: 0X4E8-0X4EC */
	__IO uint32_t DDRPHY_RW_SEL_EMPH_CTL_SETx[2];         /*!<  Register,  Address offset: 0X4F0-0X4F4 */
	__IO uint32_t DDRPHY_DQ_IO_CTL_0_SETx[2];             /*!<  Register,  Address offset: 0X4F8-0X4FC */
	__IO uint32_t DDRPHY_DQ_IO_CTL_1_SETx[2];             /*!<  Register,  Address offset: 0X500-0X504 */
	__IO uint32_t DDRPHY_READ_DBG_CTRL_SETx[2];           /*!<  Register,  Address offset: 0X508-0X50C */
	__I  uint32_t DDRPHY_MON_SETx[2];                     /*!<  Register,  Address offset: 0X510-0X514 */
	__IO uint32_t DDRPHY_DQ_IO_CTL_2_SETx[2];             /*!<  Register,  Address offset: 0X518-0X51C */
	__IO uint32_t DDRPHY_READ_CTRL_4_SETx[2];             /*!<  Register,  Address offset: 0X520-0X524 */
	__IO uint32_t RSVD7[52];                              /*!<  Reserved,  Address offset:0x528-0x5F4 */
	__IO uint32_t DDRPHY_READ_CTRL_0_SETx[2];             /*!<  Register,  Address offset: 0X5F8-0X5FC */
} DDRPHY_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup DDRPHY_Exported_Types DDRPHY Exported Types
  * @{
  */

/**
 * @brief DDRPHY Init Sturcture Definition
 */
typedef struct {
	int  mck_default;
	int  mdqs_default;
	uint8_t  dqs_en_default;
	uint8_t  rx_fifo_default;
	uint32_t odt_default;
	uint16_t dqs_driving;   //nnt, npt, pnt,ppt
	uint8_t dck_driving;   //nnt, npt, pnt,ppt
	uint8_t cmd_driving;   //nt,pt
	uint8_t ba_addr_driving;   //nt,pt
	uint8_t dq_driving;   //nt,pt
	uint32_t rx_delay_tap_0;  //bit 0 ~ 3, default 0x10101010, 0x28c
	uint32_t rx_delay_tap_1;  //bit 4 ~ 7, default 0x10101010 , 0x29c
	uint32_t rx_delay_tap_2;  //bit 8 ~ 11, default 0x10101010, 0x290
	uint32_t rx_delay_tap_3;  //bit 12 ~ 15, default 0x10101010 , 0x2a0
	uint32_t rx_delay_tap_n_0;  //bit 0 ~ 3, default 0x10101010, 0x2ac
	uint32_t rx_delay_tap_n_1;  //bit 4 ~ 7, default 0x10101010 , 0x2bc
	uint32_t rx_delay_tap_n_2;  //bit 8 ~ 11, default 0x10101010, 0x2b0
	uint32_t rx_delay_tap_n_3;  //bit 12 ~ 15, default 0x10101010 , 0x2c0

	uint32_t pre_shift_0;             	//bit 0 ~ 7, default 0x33333333, 0x26c
	uint32_t pre_shift_1;		     	//bit 8 ~ 15, default 0x33333333, 0x270
	uint32_t post_shift_0;	     	//bit 0 ~ 7, default 0x33333333, 0x27c
	uint32_t post_shift_1;	    	//bit 8 ~ 15, default 0x33333333, 0x280

	uint32_t tx_delay_tap_0;  // bit 0 ~ 7, default 0x44444444, 0x230
	uint32_t tx_delay_tap_1;	// bit 8 ~ 15, default 0x44444444, 0x230
	uint32_t dqs_dm_delay_tap; // bit 15~0 , default 0x4444
	uint32_t *cmd_driving_array;
	uint32_t ddr_freq;
} DDRPHY_InitTypeDef;

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup DDRPHY_Exported_Functions DDRPHY Exported Functions
  * @{
  */

_LONG_CALL_ void DDR_PHY_StructInit(DDRPHY_InitTypeDef *DDRPHY_InitStruct);
_LONG_CALL_ void DDR_PHY_Init(DDRPHY_InitTypeDef *DDRPHY_InitStruct);
_LONG_CALL_ void DDR_PHY_CAL(void);
_LONG_CALL_ void DDR_PHY_AutoGating(void);
_LONG_CALL_ u8 DDR_PHY_ChipInfo_ddrtype(void);
/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
