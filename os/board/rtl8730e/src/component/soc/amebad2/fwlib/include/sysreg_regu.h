#ifndef _AMEBAD_REGU_H_
#define _AMEBAD_REGU_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup REGU
  * @brief REGU driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup REGU_Register_Definitions REGU Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup REGU_POWER_CTRL
 * @brief
 * @{
 **/
#define REGU_BIT_THERMAL_CMP_DEG_OUT       ((u32)0x00000001 << 29)          /*!<R 0  For SWR debug only */
#define REGU_BIT_THERMAL_CMP_OUT           ((u32)0x00000001 << 28)          /*!<R 0  For SWR debug only */
#define REGU_BIT_SYS_PWM_REQ               ((u32)0x00000001 << 24)          /*!<R/W/ES 1  system request SWR mode. 0: PFM 1: PWM */
#define REGU_MASK_SYS_SWR_REQ              ((u32)0x00000003 << 20)          /*!<R/W/ES 1  system request SWR voltage. 00: LPS vol 01: Normal vol 10/11: Burst vol */
#define REGU_SYS_SWR_REQ(x)                ((u32)(((x) & 0x00000003) << 20))
#define REGU_GET_SYS_SWR_REQ(x)            ((u32)(((x >> 20) & 0x00000003)))
#define REGU_BIT_MODE_L                    ((u32)0x00000001 << 18)          /*!<R 0  SWR current mode for debug */
#define REGU_BIT_PRDY_SWR                  ((u32)0x00000001 << 16)          /*!<R 0  SWR power ready flag by analog */
#define REGU_BIT_XTAL32K_OK                ((u32)0x00000001 << 13)          /*!<R 0  XTAL32K ok */
#define REGU_BIT_OSC32K_OK                 ((u32)0x00000001 << 12)          /*!<R 0  OSC128K ok */
#define REGU_BIT_HW_PDB_0P9                ((u32)0x00000001 << 8)          /*!<R/W 1  Digital control AIP 3.3v power down 1: enable 0: power down */
#define REGU_BIT_GATED_STUB_OK             ((u32)0x00000001 << 4)          /*!<R/W 0  No used */
#define REGU_BIT_POW_SW                    ((u32)0x00000001 << 3)          /*!<R/W/ES 0  1: power SWR for digital core Note: this bit will be clear by bor/thm reset, and AON PMC will reboot. */
#define REGU_BIT_POW_4MOSC                 ((u32)0x00000001 << 2)          /*!<R/W 1  1: power AON 4M osc */
#define REGU_BIT_POW_32KXTAL               ((u32)0x00000001 << 1)          /*!<R/W 0  1: power AON external 32xtal */
#define REGU_BIT_POW_32KOSC                ((u32)0x00000001 << 0)          /*!<R/W 0  1: power AON 128k osc for SDM */
/** @} */

/** @defgroup REGU_SWR_ARB_CTRL0
 * @brief
 * @{
 **/
#define REGU_BIT_SWR_DBNC_FEN              ((u32)0x00000001 << 31)          /*!<R/W 1  SWR arb debounce enable */
#define REGU_BIT_SWR_MANUAL_EN             ((u32)0x00000001 << 27)          /*!<R/W 0  SWR manual mode enable. In manual mode,SWR's PFM vol value use normal pfm vol register,PWM vol use normal pwm vol register */
#define REGU_BIT_FPWM_L1_MANUAL            ((u32)0x00000001 << 26)          /*!<R/W 0  PWM/PFM manual (original fpwm_l1) */
#define REGU_BIT_PWM_FORCE_MANUAL          ((u32)0x00000001 << 25)          /*!<R/W 0  force pwm bias manual (original pwm_force) */
#define REGU_BIT_SWR_MODECHOSE             ((u32)0x00000001 << 24)          /*!<R/W 0  SWR arb voltage adjustment control signal option 0:arb internal signal 1:aip real signal */
#define REGU_MASK_SWR_TUTIME               ((u32)0x0000003F << 18)          /*!<R/W 0  Because using AON 100k clock, this can set to 0 */
#define REGU_SWR_TUTIME(x)                 ((u32)(((x) & 0x0000003F) << 18))
#define REGU_GET_SWR_TUTIME(x)             ((u32)(((x >> 18) & 0x0000003F)))
#define REGU_MASK_SWR_MODE_WAIT_TIME       ((u32)0x00000003 << 16)          /*!<R/W 1  00: 50u 01: 100u 10: 250u 11: 500us */
#define REGU_SWR_MODE_WAIT_TIME(x)         ((u32)(((x) & 0x00000003) << 16))
#define REGU_GET_SWR_MODE_WAIT_TIME(x)     ((u32)(((x >> 16) & 0x00000003)))
#define REGU_MASK_SWR_REQ_DBNC_CYC         ((u32)0x000000FF << 8)          /*!<R/W 8'd9  Arbit dbc time for every step change . The real time is 10us * (this value + 1) */
#define REGU_SWR_REQ_DBNC_CYC(x)           ((u32)(((x) & 0x000000FF) << 8))
#define REGU_GET_SWR_REQ_DBNC_CYC(x)       ((u32)(((x >> 8) & 0x000000FF)))
#define REGU_MASK_SWR_PSW_WAIT_CYC         ((u32)0x000000FF << 0)          /*!<R/W 8'd19  Stable time for every step change . The real time is 10us * (this value + 1) */
#define REGU_SWR_PSW_WAIT_CYC(x)           ((u32)(((x) & 0x000000FF) << 0))
#define REGU_GET_SWR_PSW_WAIT_CYC(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup REGU_SWR_VOL_CTRL0
 * @brief
 * @{
 **/
#define REGU_BIT_HIT_BST_VOLT              ((u32)0x00000001 << 30)          /*!<R   Current voltage is in bst setting。 */
#define REGU_BIT_HIT_LPS_VOLT              ((u32)0x00000001 << 29)          /*!<R   Current voltage is in lps setting。 */
#define REGU_BIT_HIT_NM_VOLT               ((u32)0x00000001 << 28)          /*!<R   Current voltage is in nm setting。 */
#define REGU_MASK_SWR_BST_PFM_VOL          ((u32)0x0000000F << 24)          /*!<R/W 0111  Same as SWR_NM_PFM_VOL */
#define REGU_SWR_BST_PFM_VOL(x)            ((u32)(((x) & 0x0000000F) << 24))
#define REGU_GET_SWR_BST_PFM_VOL(x)        ((u32)(((x >> 24) & 0x0000000F)))
#define REGU_MASK_SWR_LPS_PFM_VOL          ((u32)0x0000000F << 20)          /*!<R/W 0111  Same as SWR_NM_PFM_VOL */
#define REGU_SWR_LPS_PFM_VOL(x)            ((u32)(((x) & 0x0000000F) << 20))
#define REGU_GET_SWR_LPS_PFM_VOL(x)        ((u32)(((x >> 20) & 0x0000000F)))
#define REGU_MASK_SWR_NM_PFM_VOL           ((u32)0x0000000F << 16)          /*!<R/W 111  SWR/LDO 0000 0.752 0001 0.78 0010 0.801 0011 0.821 0100 0.841 0101 0.861 0110 0.881 0111 0.901 1000 0.921 1001 0.942 1010 0.963 1011 0.985 1100 1.010 1101 1.035 1110 1.062 1111 1.123 */
#define REGU_SWR_NM_PFM_VOL(x)             ((u32)(((x) & 0x0000000F) << 16))
#define REGU_GET_SWR_NM_PFM_VOL(x)         ((u32)(((x >> 16) & 0x0000000F)))
#define REGU_MASK_SWR_BST_PWM_VOL          ((u32)0x0000000F << 8)          /*!<R/W 1000  Same as SWR_NM_PWM_VOL */
#define REGU_SWR_BST_PWM_VOL(x)            ((u32)(((x) & 0x0000000F) << 8))
#define REGU_GET_SWR_BST_PWM_VOL(x)        ((u32)(((x >> 8) & 0x0000000F)))
#define REGU_MASK_SWR_LPS_PWM_VOL          ((u32)0x0000000F << 4)          /*!<R/W 4'b0111  Same as SWR_NM_PWM_VOL */
#define REGU_SWR_LPS_PWM_VOL(x)            ((u32)(((x) & 0x0000000F) << 4))
#define REGU_GET_SWR_LPS_PWM_VOL(x)        ((u32)(((x >> 4) & 0x0000000F)))
#define REGU_MASK_SWR_NM_PWM_VOL           ((u32)0x0000000F << 0)          /*!<R/W 4'b0111  VREF 0.6V 0.575V 0000 0.711 0.682 0001 0.727 0.696 0010 0.761 0.730 0011 0.800 0.767 0100 0.833 0.798 0101 0.857 0.821 0110 0.881 0.844 0111 0.908 0.870 1000 0.937 0.898 1001 0.966 0.926 1010 1.000 0.958 1011 1.031 0.987 1100 1.063 1.018 1101 1.094 1.048 1110 1.123 1.076 1111 1.181 1.131 */
#define REGU_SWR_NM_PWM_VOL(x)             ((u32)(((x) & 0x0000000F) << 0))
#define REGU_GET_SWR_NM_PWM_VOL(x)         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REGU_AON_LDO_CTRL
 * @brief
 * @{
 **/
#define REGU_MASK_AON_LDO_DUMMY0           ((u32)0x0000007F << 9)          /*!<R/W 0  // [REGU][AONLDO] dummy */
#define REGU_AON_LDO_DUMMY0(x)             ((u32)(((x) & 0x0000007F) << 9))
#define REGU_GET_AON_LDO_DUMMY0(x)         ((u32)(((x >> 9) & 0x0000007F)))
#define REGU_BIT_EN_SLEEP_L                ((u32)0x00000001 << 8)          /*!<R/W 0  Enable LDO09H sleep mode 0 : disable (default) 1 : enable */
#define REGU_MASK_AONLDO_VOSEL_H           ((u32)0x0000000F << 4)          /*!<R/W 0101  AONLDO09 voltage select VREF 0P600V 0P575V 0000: 0.600V 0.575V 0001: 0.715V 0.685V 0010: 0.747V 0.716V 0011: 0.781V 0.749V 0100: 0.820V 0.786V 0101: 0.840V 0.806V 0110: 0.862V 0.826V 0111: 0.885V 0.848V 1000: 0.909V 0.871V 1001: 0.934V 0.896V 1010: 0.961V 0.921V 1011: 0.989V 0.949V 1100: 1.020V 0.977V 1101: 1.052V 1.009V 1110: 1.086V 1.041V 1111: 1.122V 1.076V */
#define REGU_AONLDO_VOSEL_H(x)             ((u32)(((x) & 0x0000000F) << 4))
#define REGU_GET_AONLDO_VOSEL_H(x)         ((u32)(((x >> 4) & 0x0000000F)))
#define REGU_MASK_AON_LDO_DUMMY1           ((u32)0x00000003 << 2)          /*!<R/W 0  // [REGU][AONLDO] dummy */
#define REGU_AON_LDO_DUMMY1(x)             ((u32)(((x) & 0x00000003) << 2))
#define REGU_GET_AON_LDO_DUMMY1(x)         ((u32)(((x >> 2) & 0x00000003)))
#define REGU_BIT_LV_POR_VREF_SEL           ((u32)0x00000001 << 1)          /*!<R/W 0  LV_POR VREF 0 (default) VREF = 0P600V 1 VREF = 0P450V */
#define REGU_BIT_AONLDO_VREF_SEL           ((u32)0x00000001 << 0)          /*!<R/W 1  AONLDO VREF_SEL 0 (default) VREF = 0P600V 1 VREF = 0P575V */
/** @} */

/** @defgroup REGU_32KXTAL0
 * @brief
 * @{
 **/
#define REGU_MASK_XTAL32K_GM               ((u16)0x000F << 12)          /*!<R/W 1111  // [RF][32KXTAL] REG_CTRL // gm size */
#define REGU_XTAL32K_GM(x)                 ((u16)(((x) & 0x000F) << 12))
#define REGU_GET_XTAL32K_GM(x)             ((u16)(((x >> 12) & 0x000F)))
#define REGU_MASK_XTAL32K_CUR_REP          ((u16)0x0003 << 10)          /*!<R/W 01  // [RF][32KXTAL] REG_CTRL // replica buffer current */
#define REGU_XTAL32K_CUR_REP(x)            ((u16)(((x) & 0x0003) << 10))
#define REGU_GET_XTAL32K_CUR_REP(x)        ((u16)(((x >> 10) & 0x0003)))
#define REGU_MASK_XTAL32K_CUR_GM           ((u16)0x000F << 6)          /*!<R/W 0101  // [RF][32KXTAL] REG_CTRL // awake gm current, 7pXtal: 0100 */
#define REGU_XTAL32K_CUR_GM(x)             ((u16)(((x) & 0x000F) << 6))
#define REGU_GET_XTAL32K_CUR_GM(x)         ((u16)(((x >> 6) & 0x000F)))
#define REGU_MASK_XTAL32K_CUR_GM_INI       ((u16)0x000F << 2)          /*!<R/W 1001  // [RF][32KXTAL] REG_CTRL // initial gm current, 7pXtal: 0100 */
#define REGU_XTAL32K_CUR_GM_INI(x)         ((u16)(((x) & 0x000F) << 2))
#define REGU_GET_XTAL32K_CUR_GM_INI(x)     ((u16)(((x >> 2) & 0x000F)))
#define REGU_MASK_XTAL32K_CUR_MAIN         ((u16)0x0003 << 0)          /*!<R/W 01  // [RF][32KXTAL] REG_CTRL // xtal main current */
#define REGU_XTAL32K_CUR_MAIN(x)           ((u16)(((x) & 0x0003) << 0))
#define REGU_GET_XTAL32K_CUR_MAIN(x)       ((u16)(((x >> 0) & 0x0003)))
/** @} */

/** @defgroup REGU_32KXTAL1
 * @brief
 * @{
 **/
#define REGU_MASK_XTAL32K_TOK              ((u16)0x0003 << 14)          /*!<R/W 11  // [RF][32KXTAL] REG_CTRL // 00: 100ms 01:200ms 10:400ms 11: 800ms */
#define REGU_XTAL32K_TOK(x)                ((u16)(((x) & 0x0003) << 14))
#define REGU_GET_XTAL32K_TOK(x)            ((u16)(((x >> 14) & 0x0003)))
#define REGU_MASK_XTAL32K_SC_XO            ((u16)0x003F << 8)          /*!<R/W 100000  // [RF][32KXTAL] REG_CTRL // xo cap */
#define REGU_XTAL32K_SC_XO(x)              ((u16)(((x) & 0x003F) << 8))
#define REGU_GET_XTAL32K_SC_XO(x)          ((u16)(((x >> 8) & 0x003F)))
#define REGU_MASK_XTAL32K_SC_XI            ((u16)0x003F << 2)          /*!<R/W 100000  // [RF][32KXTAL] REG_CTRL // xi cap */
#define REGU_XTAL32K_SC_XI(x)              ((u16)(((x) & 0x003F) << 2))
#define REGU_GET_XTAL32K_SC_XI(x)          ((u16)(((x >> 2) & 0x003F)))
#define REGU_BIT_EN_XTAL32K_CAP_AWAKE      ((u16)0x0001 << 1)          /*!<R/W 1  // [RF][32KXTAL] REG_CTRL // internal cap enable as awake */
#define REGU_BIT_EN_XTAL32K_CAP_INITIAL    ((u16)0x0001 << 0)          /*!<R/W 1  // [RF][32KXTAL] REG_CTRL // internal cap enable as initial */
/** @} */

/** @defgroup REGU_32KXTAL2
 * @brief
 * @{
 **/
#define REGU_BIT_XTAL_32K_DUMMY            ((u32)0x00000001 << 7)          /*!<R/W 0  // [RF][32KXTAL] REG_CTRL // dummy */
#define REGU_BIT_XTAL32K_GPIO_SEL          ((u32)0x00000001 << 6)          /*!<R/W 1  // [RF][32KXTAL] REG_CTRL // 0: 32K 1: GPIO */
#define REGU_BIT_XTAL32K_SC_XO_EXTRA       ((u32)0x00000001 << 5)          /*!<R/W 0  // [RF][32KXTAL] REG_CTRL // 0: w/i 2.8pF cap 1: w/o 2.8pF cap */
#define REGU_BIT_XTAL32K_SC_XI_EXTRA       ((u32)0x00000001 << 4)          /*!<R/W 0  // [RF][32KXTAL] REG_CTRL // 0: w/i 2.8pF cap 1: w/o 2.8pF cap */
#define REGU_MASK_XTAL32K_GM_REP           ((u32)0x00000007 << 1)          /*!<R/W 111  // [RF][32KXTAL] REG_CTRL // repilca size */
#define REGU_XTAL32K_GM_REP(x)             ((u32)(((x) & 0x00000007) << 1))
#define REGU_GET_XTAL32K_GM_REP(x)         ((u32)(((x >> 1) & 0x00000007)))
#define REGU_BIT_ENB_XTAL32K_FBRES         ((u32)0x00000001 << 0)          /*!<R/W 0  // [RF][32KXTAL] REG_CTRL // 0: enable feedback resistor */
/** @} */

/** @defgroup REGU_32KOSC
 * @brief
 * @{
 **/
#define REGU_MASK_OSC32K_DUMMY2            ((u32)0x00000003 << 16)          /*!<R/W 0  HW used only , these are for reserved */
#define REGU_OSC32K_DUMMY2(x)              ((u32)(((x) & 0x00000003) << 16))
#define REGU_GET_OSC32K_DUMMY2(x)          ((u32)(((x >> 16) & 0x00000003)))
#define REGU_BIT_SEL_LDO_VREF              ((u32)0x00000001 << 15)          /*!<R/W 0  // [RF][32KOSC] REG_CTRL // 0: LDO low valtage setting 1:high voltage */
#define REGU_MASK_OSC32K_DUMMY             ((u32)0x000001FF << 6)          /*!<R/W 000000000  // [RF][32KOSC] REG_CTRL // dummy */
#define REGU_OSC32K_DUMMY(x)               ((u32)(((x) & 0x000001FF) << 6))
#define REGU_GET_OSC32K_DUMMY(x)           ((u32)(((x >> 6) & 0x000001FF)))
#define REGU_MASK_RCAL                     ((u32)0x0000003F << 0)          /*!<R/W 100000  // [RF][32KOSC] REG_CTRL // osc frequency is highier as RCAL's larger */
#define REGU_RCAL(x)                       ((u32)(((x) & 0x0000003F) << 0))
#define REGU_GET_RCAL(x)                   ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup REGU_4MOSC0
 * @brief
 * @{
 **/
#define REGU_MASK_VCM_SEL_H                ((u16)0x000F << 12)          /*!<R/W 0111  // [RF][4MOSC] REG_CTRL */
#define REGU_VCM_SEL_H(x)                  ((u16)(((x) & 0x000F) << 12))
#define REGU_GET_VCM_SEL_H(x)              ((u16)(((x >> 12) & 0x000F)))
#define REGU_MASK_VCM_SEL_L                ((u16)0x000F << 8)          /*!<R/W 0111  // [RF][4MOSC] REG_CTRL */
#define REGU_VCM_SEL_L(x)                  ((u16)(((x) & 0x000F) << 8))
#define REGU_GET_VCM_SEL_L(x)              ((u16)(((x >> 8) & 0x000F)))
#define REGU_MASK_FREQ_R_SEL               ((u16)0x00FF << 0)          /*!<R/W 01111111  // [RF][4MOSC] REG_CTRL */
#define REGU_FREQ_R_SEL(x)                 ((u16)(((x) & 0x00FF) << 0))
#define REGU_GET_FREQ_R_SEL(x)             ((u16)(((x >> 0) & 0x00FF)))
/** @} */

/** @defgroup REGU_4MOSC1
 * @brief
 * @{
 **/
#define REGU_BIT_C_SEL                     ((u16)0x0001 << 7)          /*!<R/W 0  // [RF][4MOSC] REG_CTRL */
#define REGU_BIT_EN_4MOSC                  ((u16)0x0001 << 6)          /*!<R 0  // [RF][4MOSC] 1'b1 : enable Controlley by HW function . */
#define REGU_BIT_OSC4M_DUMMY               ((u16)0x0001 << 5)          /*!<R/W 0  // [RF][4MOSC] REG_CTRL */
#define REGU_MASK_IB_BIAS_SEL              ((u16)0x001F << 0)          /*!<R/W 00000  // [RF][4MOSC] REG_CTRL */
#define REGU_IB_BIAS_SEL(x)                ((u16)(((x) & 0x001F) << 0))
#define REGU_GET_IB_BIAS_SEL(x)            ((u16)(((x >> 0) & 0x001F)))
/** @} */

/** @defgroup REGU_SWR_ON_CTRL0
 * @brief
 * @{
 **/
#define REGU_BIT_PFMCMP_IQ                 ((u32)0x00000001 << 31)          /*!<R/W 1  PFM comparator bias 0: 50nA 1: 250nA */
#define REGU_BIT_REG_BG_H                  ((u32)0x00000001 << 30)          /*!<R/W 1  1: VREF=0.6 0: VREF=0.575 */
#define REGU_BIT_OFF_END_SEL               ((u32)0x00000001 << 29)          /*!<R/W 1  COT off time controlled by 0: ZCD 1: min off time */
#define REGU_BIT_POW_MINOFF_L              ((u32)0x00000001 << 28)          /*!<R/W 1  0: normal current 1: increase OCP cmp current */
#define REGU_MASK_COT_I_L                  ((u32)0x00000003 << 26)          /*!<R/W 01  COT ON Current 00: 150nA 01: 150nA+100nA 10: 150nA+150nA 11: 150nA+250nA */
#define REGU_COT_I_L(x)                    ((u32)(((x) & 0x00000003) << 26))
#define REGU_GET_COT_I_L(x)                ((u32)(((x >> 26) & 0x00000003)))
#define REGU_MASK_VREFPFM_L                ((u32)0x0000000F << 22)          /*!<R 0111  SWR/LDO 0000: 0.55 0001: 0.60 0010: 0.65 0011: 0.70 0100: 0.75 0101: 0.80 0110: 0.85 0111: 0.90 1000: 0.95 1001: 1.00 1010: 1.05 1011: 1.10 1100: 1.10 1101: 1.10 1110: 1.10 1111: 1.10 */
#define REGU_VREFPFM_L(x)                  ((u32)(((x) & 0x0000000F) << 22))
#define REGU_GET_VREFPFM_L(x)              ((u32)(((x >> 22) & 0x0000000F)))
#define REGU_BIT_FORCE_ZCD_BIAS            ((u32)0x00000001 << 21)          /*!<R/W 1  0: ZCD bias on after SS 1: ZCD bias on when ZCD enable */
#define REGU_MASK_ZCD_SDZ_L                ((u32)0x00000003 << 19)          /*!<R/W 01  Increase ZCD delay cell */
#define REGU_ZCD_SDZ_L(x)                  ((u32)(((x) & 0x00000003) << 19))
#define REGU_GET_ZCD_SDZ_L(x)              ((u32)(((x >> 19) & 0x00000003)))
#define REGU_MASK_REG_ZCDC_H               ((u32)0x00000003 << 17)          /*!<R/W 10  00: 0.1u 01: 0.2u(PFM) 10: 0.7u(PWM) 11: 0.8u PMW mode parameter when system is high load */
#define REGU_REG_ZCDC_H(x)                 ((u32)(((x) & 0x00000003) << 17))
#define REGU_GET_REG_ZCDC_H(x)             ((u32)(((x >> 17) & 0x00000003)))
#define REGU_BIT_POW_ZCD_L                 ((u32)0x00000001 << 16)          /*!<R/W 1  Enable ZCD */
#define REGU_MASK_OCP_L1                   ((u32)0x00000007 << 13)          /*!<R/W 100  PWM current(A) PFM current(A) mP 000 0.5712 0.0816 001 0.6528 0.1224 010 0.7344 0.1632 011 0.816 0.2448 100 0.8976 0.3264 101 0.9792 0.408 110 1.0608 0.4896 111 1.1424 VIN/10 */
#define REGU_OCP_L1(x)                     ((u32)(((x) & 0x00000007) << 13))
#define REGU_GET_OCP_L1(x)                 ((u32)(((x >> 13) & 0x00000007)))
#define REGU_BIT_POWOCP_L1                 ((u32)0x00000001 << 12)          /*!<R/W 1  POWOCP_L1=1=> enable over current protection */
#define REGU_MASK_SAW_FREQ_L               ((u32)0x0000000F << 8)          /*!<R/W 1000  0000 458.0k 0001 680.2k 0010 898.9k 0011 1.113M 0100 1.321M 0101 1.522M 0110 1.716M 0111 1.902M 1000 2.087M 1001 2.258M 1010 2.426M 1011 2.582M 1100 2.743M 1101 2.888M 1110 3.035M 1111 3.165M PMW mode parameter when system is high load */
#define REGU_SAW_FREQ_L(x)                 ((u32)(((x) & 0x0000000F) << 8))
#define REGU_GET_SAW_FREQ_L(x)             ((u32)(((x >> 8) & 0x0000000F)))
#define REGU_BIT_REG_BYPASS_L              ((u32)0x00000001 << 7)          /*!<R/W 1  1: bypass SSR latch */
#define REGU_BIT_FPWM_L1                   ((u32)0x00000001 << 6)          /*!<R 1  FPWM_L1 = 1: force PWM FPWM_L1 = 0: force PFM */
#define REGU_MASK_STD_L1                   ((u32)0x00000003 << 4)          /*!<R/W 01  STD_L1<1:0> time 750u 1.5m 10: 3m 11: 6m */
#define REGU_STD_L1(x)                     ((u32)(((x) & 0x00000003) << 4))
#define REGU_GET_STD_L1(x)                 ((u32)(((x >> 4) & 0x00000003)))
#define REGU_MASK_VOL_L1                   ((u32)0x0000000F << 0)          /*!<R 1000  VREF 0.6V 0.575V 0000 0.595 0.570 0001 0.709 0.679 0010 0.740 0.709 0011 0.774 0.742 0100 0.812 0.778 0101 0.833 0.798 0110 0.854 0.818 0111 0.876 0.840 1000 0.900 0.863 1001 0.925 0.886 1010 0.951 0.912 1011 0.979 0.939 1100 1.009 0.967 1101 1.041 0.997 1110 1.074 1.029 1111 1.110 1.064 */
#define REGU_VOL_L1(x)                     ((u32)(((x) & 0x0000000F) << 0))
#define REGU_GET_VOL_L1(x)                 ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REGU_SWR_ON_CTRL1
 * @brief
 * @{
 **/
#define REGU_MASK_SN_N_L                   ((u32)0x0000000F << 28)          /*!<R/W 1001  NGATE NMOS driving */
#define REGU_SN_N_L(x)                     ((u32)(((x) & 0x0000000F) << 28))
#define REGU_GET_SN_N_L(x)                 ((u32)(((x >> 28) & 0x0000000F)))
#define REGU_MASK_SP_N_L                   ((u32)0x0000000F << 24)          /*!<R/W 0010  NGATE PMOS driving */
#define REGU_SP_N_L(x)                     ((u32)(((x) & 0x0000000F) << 24))
#define REGU_GET_SP_N_L(x)                 ((u32)(((x >> 24) & 0x0000000F)))
#define REGU_MASK_SN_P_L                   ((u32)0x0000000F << 20)          /*!<R/W 0111  PGATE NMOS driving */
#define REGU_SN_P_L(x)                     ((u32)(((x) & 0x0000000F) << 20))
#define REGU_GET_SN_P_L(x)                 ((u32)(((x >> 20) & 0x0000000F)))
#define REGU_MASK_SP_P_L                   ((u32)0x0000000F << 16)          /*!<R/W 1001  PGATE PMOS driving */
#define REGU_SP_P_L(x)                     ((u32)(((x) & 0x0000000F) << 16))
#define REGU_GET_SP_P_L(x)                 ((u32)(((x >> 16) & 0x0000000F)))
#define REGU_BIT_VO_DISCHG_PWM_H           ((u32)0x00000001 << 15)          /*!<R/W 0  1: dummy loading 5mA */
#define REGU_BIT_REG_MODE_PREDRIVER        ((u32)0x00000001 << 14)          /*!<R/W 1  1: 3 level predriver */
#define REGU_MASK_REG_ADJSLDO_L            ((u32)0x0000000F << 10)          /*!<R/W 0101  0000: 0.735 0001: 0.742 0010: 0.750 0011: 0.766 0100: 0.783 0101: 0.800 0110: 0.818 0111: 0.837 1000: 0.857 1001: 0.878 1010: 0.900 1011: 0.923 1100: 0.947 1101: 0.973 1110: 1.029 1111: 1.091 */
#define REGU_REG_ADJSLDO_L(x)              ((u32)(((x) & 0x0000000F) << 10))
#define REGU_GET_REG_ADJSLDO_L(x)          ((u32)(((x >> 10) & 0x0000000F)))
#define REGU_BIT_REG_LDOR_L                ((u32)0x00000001 << 9)          /*!<R/W 1  1: ldos res short */
#define REGU_BIT_PWM_FORCE                 ((u32)0x00000001 << 8)          /*!<R 0  0: PWM bias enabled only at PWM mode 1: Force PWM bias enable when in PWM/PFM mode */
#define REGU_BIT_PFM_PD_RST                ((u32)0x00000001 << 7)          /*!<R/W 1  1: When leave PFM mode, COT logic is reset */
#define REGU_BIT_VC_PFM_RSTB               ((u32)0x00000001 << 6)          /*!<R/W 0  When into PFM, VC is 0: clamped (bit 40 also =1) 1: floating */
#define REGU_BIT_PFM_IN_SEL                ((u32)0x00000001 << 5)          /*!<R/W 1  Power MOS into PFM when PFM CTRL, 0: rising 1: falling */
#define REGU_BIT_VC_RSTB                   ((u32)0x00000001 << 4)          /*!<R/W 0  Before SS VC is 0: pulled low 1: floating */
#define REGU_BIT_FPWMDELAY                 ((u32)0x00000001 << 3)          /*!<R/W 0  1: PFM<=>PWM PFM/PWM change by using DFF (CKMOD) */
#define REGU_BIT_ENFPWMDELAY_H             ((u32)0x00000001 << 2)          /*!<R/W 0  1: PFM=>PWM delay PWM enable and wait one clock cycle then PFM off */
#define REGU_BIT_REG_MOS_HALF_L            ((u32)0x00000001 << 1)          /*!<R/W 0  1: power mos half PMW mode parameter when system is high load */
#define REGU_BIT_CURRENT_SENSE_MOS         ((u32)0x00000001 << 0)          /*!<R/W 0  1: current sensing MOS for large OC current */
/** @} */

/** @defgroup REGU_SWR_ON_CTRL2
 * @brief
 * @{
 **/
#define REGU_MASK_BG_VR1200_TUNE           ((u32)0x00000007 << 12)          /*!<R/W 100  BG */
#define REGU_BG_VR1200_TUNE(x)             ((u32)(((x) & 0x00000007) << 12))
#define REGU_GET_BG_VR1200_TUNE(x)         ((u32)(((x >> 12) & 0x00000007)))
#define REGU_MASK_BG_R2_TUEN               ((u32)0x00000007 << 9)          /*!<R/W 100  BG R2 */
#define REGU_BG_R2_TUEN(x)                 ((u32)(((x) & 0x00000007) << 9))
#define REGU_GET_BG_R2_TUEN(x)             ((u32)(((x >> 9) & 0x00000007)))
#define REGU_MASK_BG_R1_TUNE               ((u32)0x00000007 << 6)          /*!<R/W 100  BG R1 */
#define REGU_BG_R1_TUNE(x)                 ((u32)(((x) & 0x00000007) << 6))
#define REGU_GET_BG_R1_TUNE(x)             ((u32)(((x >> 6) & 0x00000007)))
#define REGU_BIT_REG_PSRR_H                ((u32)0x00000001 << 5)          /*!<R/W 0  0: default 1: res+cap PMW mode parameter when system is high load */
#define REGU_MASK_REG_BG_CURRENT_L         ((u32)0x00000003 << 3)          /*!<R/W 00  00: 23.9n 01: 54.6n 10: 117.8n */
#define REGU_REG_BG_CURRENT_L(x)           ((u32)(((x) & 0x00000003) << 3))
#define REGU_GET_REG_BG_CURRENT_L(x)       ((u32)(((x >> 3) & 0x00000003)))
#define REGU_MASK_REG_SWR_ZCD_CTRL         ((u32)0x00000003 << 1)          /*!<R/W 00  // SWR ZCD CTRL control <1>1: double current <0>1'b1: force in CCM/DCM boundary mode(default), 1'b0 : no boundary PMW mode parameter when system is high load */
#define REGU_REG_SWR_ZCD_CTRL(x)           ((u32)(((x) & 0x00000003) << 1))
#define REGU_GET_REG_SWR_ZCD_CTRL(x)       ((u32)(((x >> 1) & 0x00000003)))
#define REGU_BIT_REG_VC_CLAMP_SEL_L        ((u32)0x00000001 << 0)          /*!<R/W 0  0: unity gain feedback clamp VC=VREF 1: orignal clamp (roll back option) */
/** @} */

/** @defgroup REGU_SWR_OFF_CTRL0
 * @brief
 * @{
 **/
#define REGU_MASK_SDZN_L                   ((u32)0x00000003 << 30)          /*!<R/W 01  Nonoverlap delay cell for NMOS */
#define REGU_SDZN_L(x)                     ((u32)(((x) & 0x00000003) << 30))
#define REGU_GET_SDZN_L(x)                 ((u32)(((x >> 30) & 0x00000003)))
#define REGU_BIT_REG_AUTOZCD_L             ((u32)0x00000001 << 29)          /*!<R/W 0  0: register 1: 10000 */
#define REGU_BIT_REG_VOFB_SEL              ((u32)0x00000001 << 28)          /*!<R/W 0  0: vo15 1: LX_SPS */
#define REGU_MASK_TBOX_L1                  ((u32)0x00000003 << 26)          /*!<R/W 00  TBOX_L1<1:0> output 00: CKOVER/RAMPOVER 01: NI/PI 10: CKSS/SSOVER 11: i/NIOFF_H */
#define REGU_TBOX_L1(x)                    ((u32)(((x) & 0x00000003) << 26))
#define REGU_GET_TBOX_L1(x)                ((u32)(((x >> 26) & 0x00000003)))
#define REGU_BIT_ENOCPMUX_L                ((u32)0x00000001 << 25)          /*!<R/W 1  1: OCP signal control by MUX 0: OCP signal control by I */
#define REGU_BIT_FORCE_LDOS                ((u32)0x00000001 << 24)          /*!<R/W 0  1: LDOS force on */
#define REGU_BIT_VO_DISCHG                 ((u32)0x00000001 << 23)          /*!<R/W 0  1: VO with 1mA discharge */
#define REGU_BIT_LDO_OC_CLAMP              ((u32)0x00000001 << 22)          /*!<R/W 1  0: OC clamp with P/NMOS 1: OC clamp with PMOS */
#define REGU_BIT_MINOFF_LIQ                ((u32)0x00000001 << 21)          /*!<R/W 1  0: Min. off circuit always on 1: Min. off circuit pulse on */
#define REGU_BIT_MINON_LIQ                 ((u32)0x00000001 << 20)          /*!<R/W 1  0: Min. on circuit always on 1: Min. on circuit pulse on */
#define REGU_BIT_POW_AUTO_L                ((u32)0x00000001 << 19)          /*!<R/W 0  1: auto PFM/PWM 0: register control */
#define REGU_BIT_ARENB_H                   ((u32)0x00000001 << 18)          /*!<R/W 1  1: off 0: on */
#define REGU_BIT_NO_OFFTIME_L              ((u32)0x00000001 << 17)          /*!<R/W 0  0: from min OFF time(PFM COT) 1: always on */
#define REGU_BIT_EN_ON_END_L               ((u32)0x00000001 << 16)          /*!<R/W 1  1: EN min OFF time (PFM COT) */
#define REGU_BIT_ENCOT_L                   ((u32)0x00000001 << 15)          /*!<R/W 1  1: PFM COT */
#define REGU_MASK_REG_CLK_SEL              ((u32)0x00000003 << 13)          /*!<R/W 11  00: out of phase CKSS 01: out of phase PWM_CTRIL 10: ckout 11: HG */
#define REGU_REG_CLK_SEL(x)                ((u32)(((x) & 0x00000003) << 13))
#define REGU_GET_REG_CLK_SEL(x)            ((u32)(((x >> 13) & 0x00000003)))
#define REGU_BIT_REG_TYPE_L                ((u32)0x00000001 << 12)          /*!<R/W 1  1: type3 0: type2 */
#define REGU_MASK_R3_L1                    ((u32)0x00000003 << 10)          /*!<R/W 10  00: 36k 01: 30k 10: 24k 11: 24k */
#define REGU_R3_L1(x)                      ((u32)(((x) & 0x00000003) << 10))
#define REGU_GET_R3_L1(x)                  ((u32)(((x >> 10) & 0x00000003)))
#define REGU_MASK_R2_L1                    ((u32)0x00000003 << 8)          /*!<R/W 10  00: 0.4M 01: 0.3M 10: 0.2M 11: 0.1M */
#define REGU_R2_L1(x)                      ((u32)(((x) & 0x00000003) << 8))
#define REGU_GET_R2_L1(x)                  ((u32)(((x >> 8) & 0x00000003)))
#define REGU_MASK_R1_L1                    ((u32)0x00000003 << 6)          /*!<R/W 10  00: 900k 01: 720k 10: 900k 11: 1080k */
#define REGU_R1_L1(x)                      ((u32)(((x) & 0x00000003) << 6))
#define REGU_GET_R1_L1(x)                  ((u32)(((x >> 6) & 0x00000003)))
#define REGU_MASK_C3_L1                    ((u32)0x00000003 << 4)          /*!<R/W 10  00: 1.59p 01: 3.2p 10: 4.76p 11: 6.34p */
#define REGU_C3_L1(x)                      ((u32)(((x) & 0x00000003) << 4))
#define REGU_GET_C3_L1(x)                  ((u32)(((x >> 4) & 0x00000003)))
#define REGU_MASK_C2_L1                    ((u32)0x00000003 << 2)          /*!<R/W 10  00: 13f 01: 26f 10: 39f 11: 41f */
#define REGU_C2_L1(x)                      ((u32)(((x) & 0x00000003) << 2))
#define REGU_GET_C2_L1(x)                  ((u32)(((x >> 2) & 0x00000003)))
#define REGU_MASK_C1_L1                    ((u32)0x00000003 << 0)          /*!<R/W 10  00: 1.03p 01: 2.06p 10: 3.1p 11: 4.12p */
#define REGU_C1_L1(x)                      ((u32)(((x) & 0x00000003) << 0))
#define REGU_GET_C1_L1(x)                  ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup REGU_SWR_OFF_CTRL1
 * @brief
 * @{
 **/
#define REGU_MASK_NULL1                    ((u32)0x0000003F << 8)          /*!<R/W 000000  HW used only , these are for reserved */
#define REGU_NULL1(x)                      ((u32)(((x) & 0x0000003F) << 8))
#define REGU_GET_NULL1(x)                  ((u32)(((x >> 8) & 0x0000003F)))
#define REGU_MASK_NULL0                    ((u32)0x00000003 << 6)          /*!<R/W 11  HW used only , these are for reserved */
#define REGU_NULL0(x)                      ((u32)(((x) & 0x00000003) << 6))
#define REGU_GET_NULL0(x)                  ((u32)(((x >> 6) & 0x00000003)))
#define REGU_BIT_REG_NMOS_OFF_L            ((u32)0x00000001 << 5)          /*!<R/W 0  0: normal 1: NMOS always off PMW mode parameter when system is high load */
#define REGU_BIT_REG_MUX_PI_L              ((u32)0x00000001 << 4)          /*!<R/W 0  1: PI (original) 0: i */
#define REGU_BIT_REG_PWM_CTRL_L            ((u32)0x00000001 << 3)          /*!<R/W 0  0: internal PWM 1: external PWM */
#define REGU_BIT_ENSR_L                    ((u32)0x00000001 << 2)          /*!<R/W 1  1: with SSR latch */
#define REGU_MASK_SDZP_L                   ((u32)0x00000003 << 0)          /*!<R/W 01  Nonoverlap delay cell for PMOS */
#define REGU_SDZP_L(x)                     ((u32)(((x) & 0x00000003) << 0))
#define REGU_GET_SDZP_L(x)                 ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup REGU_SWR_DIG_OCP
 * @brief
 * @{
 **/
#define REGU_BIT_SPS_DIG_OCP_CKE           ((u32)0x00000001 << 31)          /*!<R/W 1  SPS OCP clock enable */
#define REGU_MASK_SPS_DIG_OCP_THD          ((u32)0x00007FFF << 16)          /*!<R/W 0  SPS OCP event threshold */
#define REGU_SPS_DIG_OCP_THD(x)            ((u32)(((x) & 0x00007FFF) << 16))
#define REGU_GET_SPS_DIG_OCP_THD(x)        ((u32)(((x >> 16) & 0x00007FFF)))
#define REGU_MASK_SPS_DIG_OCP_WND          ((u32)0x0000FFFF << 0)          /*!<R/W 0  SPS OCP time window */
#define REGU_SPS_DIG_OCP_WND(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define REGU_GET_SPS_DIG_OCP_WND(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup REGU_SWR_DIG_ZCD
 * @brief
 * @{
 **/
#define REGU_MASK_SPS_DIG_ZCD_DBG          ((u32)0x0000001F << 16)          /*!<R 0  The acture zcd code used for SPS core */
#define REGU_SPS_DIG_ZCD_DBG(x)            ((u32)(((x) & 0x0000001F) << 16))
#define REGU_GET_SPS_DIG_ZCD_DBG(x)        ((u32)(((x >> 16) & 0x0000001F)))
#define REGU_BIT_SPS_DIG_ZCD_MODE          ((u32)0x00000001 << 9)          /*!<R/W 0  SPS ZCD SW mode enable */
#define REGU_BIT_SPS_DIG_ZCD_AUTO          ((u32)0x00000001 << 8)          /*!<R/W 1  SPS HW ZCD code auto count function enable */
#define REGU_MASK_SPS_DIG_ZCD_CODE         ((u32)0x0000001F << 0)          /*!<R/W 10000  SPS_DIG_ZCD_CODE,this is used for SW mode */
#define REGU_SPS_DIG_ZCD_CODE(x)           ((u32)(((x) & 0x0000001F) << 0))
#define REGU_GET_SPS_DIG_ZCD_CODE(x)       ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REGU_SWR_NEW
 * @brief
 * @{
 **/
#define REGU_BIT_REG_NMOS_OFF_L_PFM        ((u32)0x00000001 << 16)          /*!<R/W 0  0: normal 1: NMOS always off PFM mode parametr */
#define REGU_BIT_REG_NMOS_OFF_L_PWM_LL     ((u32)0x00000001 << 15)          /*!<R/W 0  0: normal 1: NMOS always off PWM mode parametr for system low load */
#define REGU_BIT_REG_PSRR_H_PWM_LL         ((u32)0x00000001 << 14)          /*!<R/W 0  0: default 1: res+cap PWM mode parametr for system low load */
#define REGU_MASK_REG_SWR_ZCD_CTRL_PFM     ((u32)0x00000003 << 12)          /*!<R/W 00  // SWR ZCD CTRL control <1>1: double current <0>1'b1: force in CCM/DCM boundary mode (default) 1'b0: no boundary PFM mode parametr */
#define REGU_REG_SWR_ZCD_CTRL_PFM(x)       ((u32)(((x) & 0x00000003) << 12))
#define REGU_GET_REG_SWR_ZCD_CTRL_PFM(x)   ((u32)(((x >> 12) & 0x00000003)))
#define REGU_MASK_REG_SWR_ZCD_CTRL_PWM_LL  ((u32)0x00000003 << 10)          /*!<R/W 00  // SWR ZCD CTRL control <1>1: double current <0>1'b1: force in CCM/DCM boundary mode (default) 1'b0: no boundary PWM mode parametr for system low load */
#define REGU_REG_SWR_ZCD_CTRL_PWM_LL(x)    ((u32)(((x) & 0x00000003) << 10))
#define REGU_GET_REG_SWR_ZCD_CTRL_PWM_LL(x) ((u32)(((x >> 10) & 0x00000003)))
#define REGU_BIT_REG_MOS_HALF_L_PFM        ((u32)0x00000001 << 9)          /*!<R/W 1  1: power mos half PFM mode parametr */
#define REGU_BIT_REG_MOS_HALF_L_PWM_LL     ((u32)0x00000001 << 8)          /*!<R/W 0  1: power mos half PWM mode parametr for system low load */
#define REGU_MASK_REG_ZCDC_H_PFM           ((u32)0x00000003 << 6)          /*!<R/W 1  00: 0.1u 01: 0.2u (PFM) 10: 0.7u (PWM) 11: 0.8u PFM mode parametr */
#define REGU_REG_ZCDC_H_PFM(x)             ((u32)(((x) & 0x00000003) << 6))
#define REGU_GET_REG_ZCDC_H_PFM(x)         ((u32)(((x >> 6) & 0x00000003)))
#define REGU_MASK_REG_ZCDC_H_PWM_LL        ((u32)0x00000003 << 4)          /*!<R/W 10  00: 0.1u 01: 0.2u (PFM) 10: 0.7u (PWM) 11: 0.8u PWM mode parametr for system low load */
#define REGU_REG_ZCDC_H_PWM_LL(x)          ((u32)(((x) & 0x00000003) << 4))
#define REGU_GET_REG_ZCDC_H_PWM_LL(x)      ((u32)(((x >> 4) & 0x00000003)))
#define REGU_MASK_SAW_FREQ_L_PWM_LL        ((u32)0x0000000F << 0)          /*!<R/W 1000  0000: 458.0K 0001: 680.2K 0010: 898.9K 0011: 1.113M 0100: 1.321M 0101: 1.522M 0110: 1.716M 0111: 1.902M 1000: 2.087M 1001: 2.258M 1010: 2.426M 1011: 2.582M 1100: 2.743M 1101: 2.888M 1110: 3.035M 1111: 3.165M PWM mode parametr for system low load */
#define REGU_SAW_FREQ_L_PWM_LL(x)          ((u32)(((x) & 0x0000000F) << 0))
#define REGU_GET_SAW_FREQ_L_PWM_LL(x)      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REGU_AON_BOR_CTRL
 * @brief
 * @{
 **/
#define REGU_BIT_REG_BOD_R_5               ((u32)0x00000001 << 11)          /*!<R/W 0  // 0: disable BOD // 1: enable BOD */
#define REGU_MASK_REG_BOD_R_4_0            ((u32)0x0000001F << 6)          /*!<R/W 01101  00000: 3.3333 00001: 3.2877 00010: 3.2432 00011: 3.2000 00100: 3.1579 00101: 3.1035 00110: 3.0508 00111: 3.0000 01000: 2.9508 01001: 2.9032 01010: 2.8571 01011: 2.8125 01100: 2.7692 01101: 2.7169 01110: 2.6666 01111: 2.6182 10000: 2.5623 10001: 2.5087 10010: 2.4574 10011: 2.4080 10100: 2.3606 10101: 2.3151 10110: 2.2713 10111: 2.2154 11000: 2.1621 11001: 2.1114 11010: 2.0630 11011: 2.0168 11100: 1.9672 11101: 1.9098 11110: 1.8556 11111: 1.8045 */
#define REGU_REG_BOD_R_4_0(x)              ((u32)(((x) & 0x0000001F) << 6))
#define REGU_GET_REG_BOD_R_4_0(x)          ((u32)(((x >> 6) & 0x0000001F)))
#define REGU_BIT_REG_BOD_F_5               ((u32)0x00000001 << 5)          /*!<R/W 0  // dummy */
#define REGU_MASK_REG_BOD_F_4_0            ((u32)0x0000001F << 0)          /*!<R/W 10011  00000: 3.3333 00001: 3.2877 00010: 3.2432 00011: 3.2000 00100: 3.1579 00101: 3.1035 00110: 3.0508 00111: 3.0000 01000: 2.9508 01001: 2.9032 01010: 2.8571 01011: 2.8125 01100: 2.7692 01101: 2.7169 01110: 2.6666 01111: 2.6182 10000: 2.5623 10001: 2.5087 10010: 2.4574 10011: 2.4080 10100: 2.3606 10101: 2.3151 10110: 2.2713 10111: 2.2154 11000: 2.1621 11001: 2.1114 11010: 2.0630 11011: 2.0168 11100: 1.9672 11101: 1.9098 11110: 1.8556 11111: 1.8045 */
#define REGU_REG_BOD_F_4_0(x)              ((u32)(((x) & 0x0000001F) << 0))
#define REGU_GET_REG_BOD_F_4_0(x)          ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REGU_BOR_ACC_CTRL
 * @brief
 * @{
 **/
#define REGU_MASK_REG_BOD_ADJIB            ((u32)0x00000003 << 16)          /*!<R/W 0  TODO */
#define REGU_REG_BOD_ADJIB(x)              ((u32)(((x) & 0x00000003) << 16))
#define REGU_GET_REG_BOD_ADJIB(x)          ((u32)(((x >> 16) & 0x00000003)))
#define REGU_BIT_REG_BOD_EN                ((u32)0x00000001 << 11)          /*!<R/W 0  // 0: disable SYS high accuracy BOD // 1: enable SYS high accuracy BOD */
#define REGU_MASK_REG_BOD_VTHHSEL_4_0      ((u32)0x0000001F << 6)          /*!<R/W 01011  00000: 3.3333 00001: 3.2877 00010: 3.2432 00011: 3.2000 00100: 3.1579 00101: 3.1035 00110: 3.0508 00111: 3.0000 01000: 2.9508 01001: 2.9032 01010: 2.8571 01011: 2.8125 01100: 2.7692 01101: 2.7169 01110: 2.6666 01111: 2.6182 10000: 2.5623 10001: 2.5087 10010: 2.4574 10011: 2.4080 10100: 2.3606 10101: 2.3151 10110: 2.2713 10111: 2.2154 11000: 2.1621 11001: 2.1114 11010: 2.0630 11011: 2.0168 11100: 1.9672 11101: 1.9098 11110: 1.8556 11111: 1.8045 */
#define REGU_REG_BOD_VTHHSEL_4_0(x)        ((u32)(((x) & 0x0000001F) << 6))
#define REGU_GET_REG_BOD_VTHHSEL_4_0(x)    ((u32)(((x >> 6) & 0x0000001F)))
#define REGU_MASK_REG_BOD_VTHLSEL_4_0      ((u32)0x0000001F << 0)          /*!<R/W 10001  00000: 3.3333 00001: 3.2877 00010: 3.2432 00011: 3.2000 00100: 3.1579 00101: 3.1035 00110: 3.0508 00111: 3.0000 01000: 2.9508 01001: 2.9032 01010: 2.8571 01011: 2.8125 01100: 2.7692 01101: 2.7169 01110: 2.6666 01111: 2.6182 10000: 2.5623 10001: 2.5087 10010: 2.4574 10011: 2.4080 10100: 2.3606 10101: 2.3151 10110: 2.2713 10111: 2.2154 11000: 2.1621 11001: 2.1114 11010: 2.0630 11011: 2.0168 11100: 1.9672 11101: 1.9098 11110: 1.8556 11111: 1.8045 */
#define REGU_REG_BOD_VTHLSEL_4_0(x)        ((u32)(((x) & 0x0000001F) << 0))
#define REGU_GET_REG_BOD_VTHLSEL_4_0(x)    ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REGU_AON_POW_32K
 * @brief
 * @{
 **/
#define REGU_MASK_REG_POW_32K              ((u32)0x0000FFFF << 0)          /*!<R/W 0  HW used only , these are for reserved */
#define REGU_REG_POW_32K(x)                ((u32)(((x) & 0x0000FFFF) << 0))
#define REGU_GET_REG_POW_32K(x)            ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup REGU_DUMMY
 * @brief
 * @{
 **/
#define REGU_MASK_REG_DUMMY_L              ((u32)0x0000003F << 0)          /*!<R/W 0   */
#define REGU_REG_DUMMY_L(x)                ((u32)(((x) & 0x0000003F) << 0))
#define REGU_GET_REG_DUMMY_L(x)            ((u32)(((x >> 0) & 0x0000003F)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup REGU_Exported_Types REGU Exported Types
  * @{
*****************************************************************************/


/**
 * @brief REGU Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t REGU_POWER_CTRL;                        /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t REGU_SWR_ARB_CTRL0;                     /*!<  Register,  Address offset: 0x004 */
	__IO uint32_t REGU_SWR_VOL_CTRL0;                     /*!<  Register,  Address offset: 0x008 */
	__IO uint32_t REGU_AON_LDO_CTRL;                      /*!<  Register,  Address offset: 0x00C */
	__IO uint16_t REGU_32KXTAL0;                          /*!<  Register,  Address offset: 0x010 */
	__IO uint16_t REGU_32KXTAL1;                          /*!<  Register,  Address offset: 0x012 */
	__IO uint32_t REGU_32KXTAL2;                          /*!<  Register,  Address offset: 0x014 */
	__IO uint32_t RSVD0[2];                               /*!<  Reserved,  Address offset:0x18-0x1C */
	__IO uint32_t REGU_32KOSC;                            /*!<  Register,  Address offset: 0x020 */
	__IO uint32_t RSVD1[3];                               /*!<  Reserved,  Address offset:0x24-0x2C */
	__IO uint16_t REGU_4MOSC0;                            /*!<  Register,  Address offset: 0x030 */
	__IO uint16_t REGU_4MOSC1;                            /*!<  Register,  Address offset: 0x032 */
	__IO uint16_t RSVD2[6];                               /*!<  Reserved,  Address offset:0x34-0x3C */
	__IO uint32_t REGU_SWR_ON_CTRL0;                      /*!<  Register,  Address offset: 0x040 */
	__IO uint32_t REGU_SWR_ON_CTRL1;                      /*!<  Register,  Address offset: 0x044 */
	__IO uint32_t REGU_SWR_ON_CTRL2;                      /*!<  Register,  Address offset: 0x048 */
	__IO uint32_t RSVD3;                                  /*!<  Reserved,  Address offset:0x4C */
	__IO uint32_t REGU_SWR_OFF_CTRL0;                     /*!<  Register,  Address offset: 0x050 */
	__IO uint32_t REGU_SWR_OFF_CTRL1;                     /*!<  Register,  Address offset: 0x054 */
	__IO uint32_t RSVD4[10];                              /*!<  Reserved,  Address offset:0x58-0x7C */
	__IO uint32_t REGU_SWR_DIG_OCP;                       /*!<  Register,  Address offset: 0x080 */
	__IO uint32_t REGU_SWR_DIG_ZCD;                       /*!<  Register,  Address offset: 0x084 */
	__IO uint32_t RSVD5[2];                               /*!<  Reserved,  Address offset:0x88-0x8C */
	__IO uint32_t REGU_SWR_NEW;                           /*!<  Register,  Address offset: 0x090 */
	__IO uint32_t RSVD6[3];                               /*!<  Reserved,  Address offset:0x94-0x9C */
	__IO uint32_t REGU_AON_BOR_CTRL;                      /*!<  Register,  Address offset: 0x0A0 */
	__IO uint32_t REGU_BOR_ACC_CTRL;                      /*!<  Register,  Address offset: 0x0A4 */
	__IO uint32_t REGU_AON_POW_32K;                       /*!<  Register,  Address offset: 0x0A8 */
	__IO uint32_t REGU_DUMMY;                             /*!<  Register,  Address offset: 0x0AC */
} REGU_TypeDef;

#define REGU_BASE ((REGU_TypeDef *) (0x42008100))

/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here
#define NORMAL_AON_VOL		0x5
#define OTP_AON_VOL			(NORMAL_AON_VOL + 1)
/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
