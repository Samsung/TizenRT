#ifndef _AMEBAD_WLAFE_H_
#define _AMEBAD_WLAFE_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup WLAFE
  * @brief WLAFE driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup WLAFE_Register_Definitions WLAFE Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup WLAFE_ANAPAR_POW_MAC_0
 * @brief
 * @{
 **/
#define WLAFE_BIT_HW_EN_WLAFE_2REGU          ((u32)0x00000001 << 9)          /*!<R/W 0  control 3.3V power reset of wl afe, need level shift to 3.3V by regu */
#define WLAFE_BIT_HW_EN_WLAFE                ((u32)0x00000001 << 8)          /*!<R/W 0  core power reset of wl afe */
#define WLAFE_BIT_ISO_WLAFE                  ((u32)0x00000001 << 7)          /*!<R/W 1  isolation signal */
#define WLAFE_BIT_HW_EN_WLRF_2REGU           ((u32)0x00000001 << 6)          /*!<R/W 0  control 3.3V power reset of wl rfc, need level shift to 3.3V by regu */
#define WLAFE_BIT_HW_EN_WLRF                 ((u32)0x00000001 << 5)          /*!<R/W 0  core power reset of wl rfc */
#define WLAFE_BIT_POW_LDO_RSSI_ADC           ((u32)0x00000001 << 4)          /*!<R/W 0  Power on ldo for rssi adc */
#define WLAFE_BIT_POW_LDO_ADC                ((u32)0x00000001 << 3)          /*!<R/W 0  Power on ldo for adc */
#define WLAFE_BIT_POW_LDO_DAC                ((u32)0x00000001 << 2)          /*!<R/W 0  Power on ldo for dac */
#define WLAFE_BIT_POW_PLL                    ((u32)0x00000001 << 1)          /*!<R/W 0  Power on pll */
#define WLAFE_BIT_POW_LDO                    ((u32)0x00000001 << 0)          /*!<R/W 0  Power on AFE power cut and pll ldo */
/** @} */

/** @defgroup WLAFE_ANAPAR_MAC_0
 * @brief
 * @{
 **/
#define WLAFE_BIT_CK5M_EN                    ((u32)0x00000001 << 31)          /*!<R/W 0  1: Enable 0: Output "1" */
#define WLAFE_BIT_REG_CP_OFFSET              ((u32)0x00000001 << 30)          /*!<R/W 0  CP current offset 1: on 0:off */
#define WLAFE_MASK_REG_RS_SET                ((u32)0x00000007 << 27)          /*!<R/W 100  Set LPF rs 000~111: 1k/3k/5k/7k/9k/11k/13k/15k */
#define WLAFE_REG_RS_SET(x)                  ((u32)(((x) & 0x00000007) << 27))
#define WLAFE_GET_REG_RS_SET(x)              ((u32)(((x >> 27) & 0x00000007)))
#define WLAFE_MASK_REG_R3_SET                ((u32)0x00000007 << 24)          /*!<R/W 000  Set LPF r3 000~111: 1k/3k/5k/7k/9k/11k/13k/15k */
#define WLAFE_REG_R3_SET(x)                  ((u32)(((x) & 0x00000007) << 24))
#define WLAFE_GET_REG_R3_SET(x)              ((u32)(((x >> 24) & 0x00000007)))
#define WLAFE_MASK_REG_CS_SET                ((u32)0x00000003 << 22)          /*!<R/W 10  Set LPF cs 00/01/10/11: 12pF/20pF/24pF/32pF */
#define WLAFE_REG_CS_SET(x)                  ((u32)(((x) & 0x00000003) << 22))
#define WLAFE_GET_REG_CS_SET(x)              ((u32)(((x >> 22) & 0x00000003)))
#define WLAFE_MASK_REG_CP_SET                ((u32)0x00000003 << 20)          /*!<R/W 01  Set LPF cp 00/01/10/11: 0.8pF/1.2pF/1.6pF/2.0pF */
#define WLAFE_REG_CP_SET(x)                  ((u32)(((x) & 0x00000003) << 20))
#define WLAFE_GET_REG_CP_SET(x)              ((u32)(((x >> 20) & 0x00000003)))
#define WLAFE_MASK_REG_C3_SET                ((u32)0x00000003 << 18)          /*!<R/W 00  Set LPF c3 00/01/10/11: 0.4pF/0.8pF/1.2pF/1.6pF */
#define WLAFE_REG_C3_SET(x)                  ((u32)(((x) & 0x00000003) << 18))
#define WLAFE_GET_REG_C3_SET(x)              ((u32)(((x >> 18) & 0x00000003)))
#define WLAFE_BIT_REG_CK320_SEL              ((u32)0x00000001 << 17)          /*!<R/W 0  Ck 320MHz selection, 1 from ck_ext */
#define WLAFE_BIT_REG_DOGENB                 ((u32)0x00000001 << 16)          /*!<R/W 0  enable watch dog 0: Enable 1: Disable */
#define WLAFE_BIT_REG_VC_TL                  ((u32)0x00000001 << 15)          /*!<R/W 0  Tie vc to 1/4 VDD */
#define WLAFE_BIT_REG_VC_TH                  ((u32)0x00000001 << 14)          /*!<R/W 0  Tie vc to 3/4 VDD */
#define WLAFE_MASK_REG_CP_BIA                ((u32)0x00000007 << 11)          /*!<R/W 000  pll: CP: current: selection: (efuse) 000: 5uA 001: 10uA 010: 15uA 011: 20uA 100: 25uA 101: 30uA 110: 35uA 111: 40uA */
#define WLAFE_REG_CP_BIA(x)                  ((u32)(((x) & 0x00000007) << 11))
#define WLAFE_GET_REG_CP_BIA(x)              ((u32)(((x >> 11) & 0x00000007)))
#define WLAFE_BIT_REG_EN_MON                 ((u32)0x00000001 << 10)          /*!<R/W 0  1: Enable 0: Output "1" */
#define WLAFE_BIT_REG_TESTEN                 ((u32)0x00000001 << 9)          /*!<R/W 0  1: Enable ck_test */
#define WLAFE_MASK_NULL0                     ((u32)0x00000003 << 7)          /*!<R/W 00   */
#define WLAFE_NULL0(x)                       ((u32)(((x) & 0x00000003) << 7))
#define WLAFE_GET_NULL0(x)                   ((u32)(((x >> 7) & 0x00000003)))
#define WLAFE_MASK_REG_PS                    ((u32)0x00000007 << 4)          /*!<R/W 000  Phase swallow setting */
#define WLAFE_REG_PS(x)                      ((u32)(((x) & 0x00000007) << 4))
#define WLAFE_GET_REG_PS(x)                  ((u32)(((x >> 4) & 0x00000007)))
#define WLAFE_BIT_REG_PS_EN                  ((u32)0x00000001 << 3)          /*!<R/W 1  Phase swallow enable */
#define WLAFE_MASK_REG_MONCKSELH             ((u32)0x00000007 << 0)          /*!<R/W 000  ck_mon selection 111: 160MHz 001: 110MHz 010: 101MHz 100: 320MHz 011: ck_200M_test 010: ck_sync 001: 5MHz (duty50-50) 000: ck_test */
#define WLAFE_REG_MONCKSELH(x)               ((u32)(((x) & 0x00000007) << 0))
#define WLAFE_GET_REG_MONCKSELH(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup WLAFE_ANAPAR_MAC_1
 * @brief
 * @{
 **/
#define WLAFE_MASK_F0F                       ((u32)0x00001FFF << 19)          /*!<R/W 0000000000000  SDM divider sel */
#define WLAFE_F0F(x)                         ((u32)(((x) & 0x00001FFF) << 19))
#define WLAFE_GET_F0F(x)                     ((u32)(((x >> 19) & 0x00001FFF)))
#define WLAFE_MASK_DIVN                      ((u32)0x0000003F << 13)          /*!<R/W 001110  SDM divider sel */
#define WLAFE_DIVN(x)                        ((u32)(((x) & 0x0000003F) << 13))
#define WLAFE_GET_DIVN(x)                    ((u32)(((x >> 13) & 0x0000003F)))
#define WLAFE_MASK_BB_DBG_SEL_AFE_SDM        ((u32)0x0000000F << 9)          /*!<R/W 0000  SDM debug selection */
#define WLAFE_BB_DBG_SEL_AFE_SDM(x)          ((u32)(((x) & 0x0000000F) << 9))
#define WLAFE_GET_BB_DBG_SEL_AFE_SDM(x)      ((u32)(((x >> 9) & 0x0000000F)))
#define WLAFE_BIT_RFE_SEL_SDM                ((u32)0x00000001 << 8)          /*!<R/W 1  pll referemce clk select 1:table 0:regesters */
#define WLAFE_MASK_BB_RFE_SEL_AFE_SDM        ((u32)0x0000000F << 4)          /*!<R/W 0000  pll referemce clk select AFE[0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15] = [40M 25M 54M 19.2M 20M 26M 38.4M 17.664M 16M 32M 60M 52M 48M 27M 24M 80M] (efuse) */
#define WLAFE_BB_RFE_SEL_AFE_SDM(x)          ((u32)(((x) & 0x0000000F) << 4))
#define WLAFE_GET_BB_RFE_SEL_AFE_SDM(x)      ((u32)(((x >> 4) & 0x0000000F)))
#define WLAFE_BIT_CK320M_AFE_EN              ((u32)0x00000001 << 3)          /*!<R/W 0  1: Enable 0: Output "1" */
#define WLAFE_BIT_CK_200M_TEST               ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define WLAFE_BIT_CK320M_EN                  ((u32)0x00000001 << 1)          /*!<R/W 0  1: Enable 0: Output "1" */
#define WLAFE_BIT_REG_FREF_EDGE              ((u32)0x00000001 << 0)          /*!<R/W 0  PLL referemce clk edge select 0: positive 1: negtive */
/** @} */

/** @defgroup WLAFE_ANAPAR_MAC_2
 * @brief
 * @{
 **/
#define WLAFE_MASK_R_WLAFE_LDO_ST            ((u32)0x00000003 << 27)          /*!<R/W 10  stable time select for WLAFE LDO 2'b00:10us, 2'b01:20us, 2'b10:100us, 2'b11:200us */
#define WLAFE_R_WLAFE_LDO_ST(x)              ((u32)(((x) & 0x00000003) << 27))
#define WLAFE_GET_R_WLAFE_LDO_ST(x)          ((u32)(((x >> 27) & 0x00000003)))
#define WLAFE_BIT_VPLUSE_LDO_PLL             ((u32)0x00000001 << 26)          /*!<R/W 1  For PLL(WL PLL LDO) */
#define WLAFE_MASK_SW_LDO_OUT_PLL            ((u32)0x00000003 << 24)          /*!<R/W 01  Ldo voltage 00/01/10/11 0.85/0.9/0.95/1V(PLL LDO) */
#define WLAFE_SW_LDO_OUT_PLL(x)              ((u32)(((x) & 0x00000003) << 24))
#define WLAFE_GET_SW_LDO_OUT_PLL(x)          ((u32)(((x >> 24) & 0x00000003)))
#define WLAFE_BIT_SW_LDO2PWRCUT_PLL          ((u32)0x00000001 << 23)          /*!<R/W 0  1 power cut mode 0 ldo mode(PLL LDO) */
#define WLAFE_BIT_SEL_8715_LUT               ((u32)0x00000001 << 22)          /*!<R/W 0  1 -> 8 phase 500MHz 0 -> 6 phase 640MHz */
#define WLAFE_BIT_ORDER_SDM                  ((u32)0x00000001 << 21)          /*!<R/W 0  SDM order 1: 3th order 0: 2nd order */
#define WLAFE_MASK_S_DCB                     ((u32)0x00000003 << 19)          /*!<R/W 00  Driving control buffer 11:larger current */
#define WLAFE_S_DCB(x)                       ((u32)(((x) & 0x00000003) << 19))
#define WLAFE_GET_S_DCB(x)                   ((u32)(((x >> 19) & 0x00000003)))
#define WLAFE_BIT_GPO                        ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define WLAFE_BIT_GPE                        ((u32)0x00000001 << 17)          /*!<R/W 0  gpe=1 and gpo=1=>output gpe=1 and gpo=0=>input */
#define WLAFE_BIT_REG_CP_IDOUBLE             ((u32)0x00000001 << 16)          /*!<R/W 0  1 for cp currnet double */
#define WLAFE_MASK_CK320M_PS                 ((u32)0x00000003 << 14)          /*!<R/W 00  Debug */
#define WLAFE_CK320M_PS(x)                   ((u32)(((x) & 0x00000003) << 14))
#define WLAFE_GET_CK320M_PS(x)               ((u32)(((x >> 14) & 0x00000003)))
#define WLAFE_MASK_NULL1                     ((u32)0x00000003 << 12)          /*!<R/W 0   */
#define WLAFE_NULL1(x)                       ((u32)(((x) & 0x00000003) << 12))
#define WLAFE_GET_NULL1(x)                   ((u32)(((x >> 12) & 0x00000003)))
#define WLAFE_BIT_VPLUSE_LDO_DAC             ((u32)0x00000001 << 11)          /*!<R/W 1  For DAC(WL DAC LDO) */
#define WLAFE_BIT_VPLUSE_LDO_ADC             ((u32)0x00000001 << 10)          /*!<R/W 1  For ADC(WL ADC LDO and RSSI ADC LDO) */
#define WLAFE_MASK_SW_LDO_OUT                ((u32)0x00000003 << 8)          /*!<R/W 01  Ldo voltage 00/01/10/11: 0.85V/0.9V/0.95V/1V (ADDA LDO) */
#define WLAFE_SW_LDO_OUT(x)                  ((u32)(((x) & 0x00000003) << 8))
#define WLAFE_GET_SW_LDO_OUT(x)              ((u32)(((x >> 8) & 0x00000003)))
#define WLAFE_BIT_SW_LDO2PWRCUT              ((u32)0x00000001 << 7)          /*!<R/W 0  1 power cut mode 0 ldo mode (ADDA LDO) */
#define WLAFE_MASK_DITHER                    ((u32)0x0000000F << 3)          /*!<R/W 0000  Dither funtion */
#define WLAFE_DITHER(x)                      ((u32)(((x) & 0x0000000F) << 3))
#define WLAFE_GET_DITHER(x)                  ((u32)(((x >> 3) & 0x0000000F)))
#define WLAFE_MASK_F0N                       ((u32)0x00000007 << 0)          /*!<R/W 000  SDM divider sel */
#define WLAFE_F0N(x)                         ((u32)(((x) & 0x00000007) << 0))
#define WLAFE_GET_F0N(x)                     ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup WLAFE_DBG_WLAFE_PMC
 * @brief
 * @{
 **/
#define WLAFE_MASK_DBG_WLAFE_PMC             ((u32)0xFFFFFFFF << 0)          /*!<R 0  debug signal of wlafe_pmc */
#define WLAFE_DBG_WLAFE_PMC(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define WLAFE_GET_DBG_WLAFE_PMC(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WLAFE_DBG_WLAFE
 * @brief
 * @{
 **/
#define WLAFE_MASK_DBG_WLAFE_REG             ((u32)0x0000FFFF << 0)          /*!<R 0  debug signal of wlafe_reg */
#define WLAFE_DBG_WLAFE_REG(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define WLAFE_GET_DBG_WLAFE_REG(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup WLAFE_ANAPAR_RFC
 * @brief
 * @{
 **/
#define WLAFE_BIT_R_RFAFE_VIO1833_SEL        ((u32)0x00000001 << 10)          /*!<R/W 0  IND_VIO1833 output decided by: 1: 1.8V 0: 3.3V */
#define WLAFE_MASK_R_WLRFC_PWC_ST            ((u32)0x00000003 << 8)          /*!<R/W 01  stable time select for WLRFC power cut 2'b00:40us, 2'b01:50us, 2'b10:100us, 2'b11:200us */
#define WLAFE_R_WLRFC_PWC_ST(x)              ((u32)(((x) & 0x00000003) << 8))
#define WLAFE_GET_R_WLRFC_PWC_ST(x)          ((u32)(((x >> 8) & 0x00000003)))
#define WLAFE_MASK_ANAPOW_RFC                ((u32)0x000000FF << 0)          /*!<R/W 0  wlan power rf control */
#define WLAFE_ANAPOW_RFC(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define WLAFE_GET_ANAPOW_RFC(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup WLAFE_LDO_VPLUSE_SEL
 * @brief
 * @{
 **/
#define WLAFE_BIT_VPLUSE_SEL                 ((u32)0x00000001 << 0)          /*!<R/W 1  vpluse_ldo output decided by: 1: power on ldo 0: ctrl reg */
#define WLAFE_MASK_VPLUSE_CNT                ((u32)0x0000FFFF << 1)          /*!<R/W 0000000000101  vpluse_ldo high output delay cycle after pll_ldo set high,default:5(1.25us) */
#define WLAFE_VPLUSE_CNT(x)                  ((u32)(((x) & 0x0000FFFF) << 1))
#define WLAFE_GET_VPLUSE_CNT(x)              ((u32)(((x >> 1) & 0x0000FFFF)))
#define WLAFE_BIT_VPLUSE_WAIT_MCYCLE         ((u32)0x00000001 << 17)          /*!<R/W 1  1: vpluse_ldo_dac and vpluse_ldo_adc pull low should wait more cycle after ldo pull high */
/** @} */

/** @defgroup WLAFE_WLRFC_CTRL
 * @brief
 * @{
 **/
#define WLAFE_BIT_WLRFC_PWC_EN               ((u32)0x00000001 << 0)          /*!<R/W 0  1: gen request of wlan rfc power cut 0: gen request of turn off wlan rfc power cut */
#define WLAFE_BIT_ISO_WLRFC_EN               ((u32)0x00000001 << 1)          /*!<R/W 1  Isolation control for wlan rfc 1: isolation 0: attach */
#define WLAFE_BIT_WLRFC_SAVE_EN              ((u32)0x00000001 << 2)          /*!<R/W 0  1: backup(save) 0: restore */
#define WLAFE_BIT_WLRFC_PWC_SEL              ((u32)0x00000001 << 3)          /*!<R/W 1  Option to turn off wlrfc pwr cut 1: enable 0: disable */
/** @} */

/** @defgroup WLAFE_ANAPAR_DCK_0
 * @brief
 * @{
 **/
#define WLAFE_BIT_DCK_COMP_ZERO_CR           ((u32)0x00000001 << 31)          /*!<R/W 0  0: DOS [+/-] 1: DOS[+/0/-] */
#define WLAFE_BIT_DCK_CMP_BAR_CR             ((u32)0x00000001 << 30)          /*!<R/W 0  0:AVG_VOS1 - AVG_VOS2 / 1:AVG_VOS2 - AVG_VOS1 */
#define WLAFE_MASK_DCK_WAVE_MID_SH_CR        ((u32)0x0000001F << 25)          /*!<R/W 00000  DA[8:0] to shift -15 ~ +15 the middle value */
#define WLAFE_DCK_WAVE_MID_SH_CR(x)          ((u32)(((x) & 0x0000001F) << 25))
#define WLAFE_GET_DCK_WAVE_MID_SH_CR(x)      ((u32)(((x >> 25) & 0x0000001F)))
#define WLAFE_MASK_DCK_WAVE_OFSP_CR          ((u32)0x000000FF << 17)          /*!<R/W 00000000  id[7:0] or qd[7:0] = wave_mid_val + wave_ofs_val */
#define WLAFE_DCK_WAVE_OFSP_CR(x)            ((u32)(((x) & 0x000000FF) << 17))
#define WLAFE_GET_DCK_WAVE_OFSP_CR(x)        ((u32)(((x >> 17) & 0x000000FF)))
#define WLAFE_BIT_DCK_CK_SYS_BAR_CR          ((u32)0x00000001 << 16)          /*!<R/W 0  0: original clock 1: inverse clock */
#define WLAFE_BIT_DCK_CK_DAC_BAR_CR          ((u32)0x00000001 << 15)          /*!<R/W 0  0: original clock 1: inverse clock */
#define WLAFE_BIT_DCK_CK_ADC_BAR_CR          ((u32)0x00000001 << 14)          /*!<R/W 0  0: original clock 1: inverse clock */
#define WLAFE_MASK_DCK_ADC_OFS_CR            ((u32)0x0000003F << 8)          /*!<R/W 000000  Compensate ADC offset : adc2cal_AD[9:0] + ADC_ofs_CR[5:0] */
#define WLAFE_DCK_ADC_OFS_CR(x)              ((u32)(((x) & 0x0000003F) << 8))
#define WLAFE_GET_DCK_ADC_OFS_CR(x)          ((u32)(((x >> 8) & 0x0000003F)))
#define WLAFE_BIT_DCK_LPOW_CR                ((u32)0x00000001 << 7)          /*!<R/W 0  Low power enable when calibration done */
#define WLAFE_BIT_DCK_CAL_OS_Q_CR            ((u32)0x00000001 << 6)          /*!<R/W 0  DC-offset calibration enable qpath */
#define WLAFE_BIT_DCK_CAL_OS_I_CR            ((u32)0x00000001 << 5)          /*!<R/W 0  DC-offset calibration enable ipath */
#define WLAFE_BIT_DCK_CAL_OS_CR              ((u32)0x00000001 << 4)          /*!<R/W 0  DC-offset calibration enable, ipath and qpath */
#define WLAFE_BIT_DCK_POW_DA_CR              ((u32)0x00000001 << 3)          /*!<R/W 0  power on for DC-offset calibration */
#define WLAFE_BIT_DCK_RSTB                   ((u32)0x00000001 << 2)          /*!<R/W 0  DCK power on reset */
#define WLAFE_BIT_DCK_SWAP_MANU_CR           ((u32)0x00000001 << 1)          /*!<R/W 0  SWAP by manual */
#define WLAFE_BIT_DCK_SWAP_FORCE_CR          ((u32)0x00000001 << 0)          /*!<R/W 0  Force the SWAP from SWAP_Manu_CR */
/** @} */

/** @defgroup WLAFE_ANAPAR_DCK_1
 * @brief
 * @{
 **/
#define WLAFE_MASK_DCK_FETCH_ADDR_CR         ((u32)0x00000003 << 30)          /*!<R/W 00  Fetch the _AD-DATA 0: first 1: half-odd 2: half-even 3: final */
#define WLAFE_DCK_FETCH_ADDR_CR(x)           ((u32)(((x) & 0x00000003) << 30))
#define WLAFE_GET_DCK_FETCH_ADDR_CR(x)       ((u32)(((x >> 30) & 0x00000003)))
#define WLAFE_BIT_DCK_KOS_EN_FORCE_Q_CR      ((u32)0x00000001 << 29)          /*!<R/W 0  Force the KOS_EN_Q[5:0] from KOS_EN_Manu_Q_CR[5:0] */
#define WLAFE_BIT_DCK_KOS_EN_FORCE_I_CR      ((u32)0x00000001 << 28)          /*!<R/W 0  Force the KOS_EN_I[5:0] from KOS_EN_Manu_I_CR[5:0] */
#define WLAFE_MASK_DCK_DOS_MANU_I_CR         ((u32)0x0000003F << 22)          /*!<R/W 000000  DOS_I[5:0] by manual */
#define WLAFE_DCK_DOS_MANU_I_CR(x)           ((u32)(((x) & 0x0000003F) << 22))
#define WLAFE_GET_DCK_DOS_MANU_I_CR(x)       ((u32)(((x >> 22) & 0x0000003F)))
#define WLAFE_MASK_DCK_DOS_MANU_CR           ((u32)0x0000003F << 16)          /*!<R/W 000000  DOS[5:0] by manual */
#define WLAFE_DCK_DOS_MANU_CR(x)             ((u32)(((x) & 0x0000003F) << 16))
#define WLAFE_GET_DCK_DOS_MANU_CR(x)         ((u32)(((x >> 16) & 0x0000003F)))
#define WLAFE_BIT_DCK_KOS_EN_FORCE_CR        ((u32)0x00000001 << 15)          /*!<R/W 0  Force the KOS_EN[5:0] from KOS_EN_Manu_CR[5:0] */
#define WLAFE_BIT_DCK_DOS_FORCE_Q_CR         ((u32)0x00000001 << 14)          /*!<R/W 0  Force the DOS_Q[5:0] from DOS_Manu_Q_CR[5:0] */
#define WLAFE_BIT_DCK_DOS_FORCE_I_CR         ((u32)0x00000001 << 13)          /*!<R/W 0  Force the DOS_I[5:0] from DOS_Manu_I_CR[5:0] */
#define WLAFE_BIT_DCK_DOS_FORCE_CR           ((u32)0x00000001 << 12)          /*!<R/W 0  Force the DOS[5:0] from DOS_Manu_CR[5:0] */
#define WLAFE_BIT_DCK_DEBUG_PATH_CR          ((u32)0x00000001 << 11)          /*!<R/W 0  1: DCK ready for debug */
#define WLAFE_BIT_DCK_VOSK_SEL_Q_CR          ((u32)0x00000001 << 10)          /*!<R/W 0  0: fetch the middle VOS_SR=VOS1-VOS2 0: first / 1: final */
#define WLAFE_BIT_DCK_VOSK_SEL_I_CR          ((u32)0x00000001 << 9)          /*!<R/W 0  0: fetch the middle VOS_SR=VOS1-VOS2 0: first / 1: final */
#define WLAFE_MASK_DCK_SWITCH_WAIT_IQ_CR     ((u32)0x00000007 << 6)          /*!<R/W 000  Switch I2Q waiting time */
#define WLAFE_DCK_SWITCH_WAIT_IQ_CR(x)       ((u32)(((x) & 0x00000007) << 6))
#define WLAFE_GET_DCK_SWITCH_WAIT_IQ_CR(x)   ((u32)(((x >> 6) & 0x00000007)))
#define WLAFE_MASK_DCK_AVG_CYC_CR            ((u32)0x00000007 << 3)          /*!<R/W 111  DC-offset calibration fetch ADC data numbers to average */
#define WLAFE_DCK_AVG_CYC_CR(x)              ((u32)(((x) & 0x00000007) << 3))
#define WLAFE_GET_DCK_AVG_CYC_CR(x)          ((u32)(((x >> 3) & 0x00000007)))
#define WLAFE_MASK_DCK_WAIT_CYC_SET_CR       ((u32)0x00000007 << 0)          /*!<R/W 111  DC-offset calibration digital waiting clock cycle for setting time */
#define WLAFE_DCK_WAIT_CYC_SET_CR(x)         ((u32)(((x) & 0x00000007) << 0))
#define WLAFE_GET_DCK_WAIT_CYC_SET_CR(x)     ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup WLAFE_ANAPAR_DCK_2
 * @brief
 * @{
 **/
#define WLAFE_BIT_DCK_BT_KOS_EN_FORCE_I_CR   ((u32)0x00000001 << 31)          /*!<R/W 0  Manual write KOSEN at BT mode */
#define WLAFE_BIT_DCK_BT_DOS_FORCE_Q_CR      ((u32)0x00000001 << 30)          /*!<R/W 0  Manual write DOS data at BT mode */
#define WLAFE_BIT_DCK_BT_DOS_FORCE_I_CR      ((u32)0x00000001 << 29)          /*!<R/W 0  Manual write DOS data at BT mode */
#define WLAFE_BIT_DCK_FIFO_CLEAR_CR          ((u32)0x00000001 << 28)          /*!<R/W 0  Clear all FIFO */
#define WLAFE_MASK_DCK_KOS_EN_MANU_Q_CR      ((u32)0x0000003F << 22)          /*!<R/W 000000  KOS_EN_Q[5:0] by manual */
#define WLAFE_DCK_KOS_EN_MANU_Q_CR(x)        ((u32)(((x) & 0x0000003F) << 22))
#define WLAFE_GET_DCK_KOS_EN_MANU_Q_CR(x)    ((u32)(((x >> 22) & 0x0000003F)))
#define WLAFE_MASK_DCK_KOS_EN_MANU_I_CR      ((u32)0x0000003F << 16)          /*!<R/W 000000  KOS_EN_I[5:0] by manual */
#define WLAFE_DCK_KOS_EN_MANU_I_CR(x)        ((u32)(((x) & 0x0000003F) << 16))
#define WLAFE_GET_DCK_KOS_EN_MANU_I_CR(x)    ((u32)(((x >> 16) & 0x0000003F)))
#define WLAFE_MASK_DCK_VOS_ADDR_CR           ((u32)0x00000007 << 13)          /*!<R/W 110  Fetch the k-th offset average value to monitor */
#define WLAFE_DCK_VOS_ADDR_CR(x)             ((u32)(((x) & 0x00000007) << 13))
#define WLAFE_GET_DCK_VOS_ADDR_CR(x)         ((u32)(((x >> 13) & 0x00000007)))
#define WLAFE_BIT_DCK_OFS_POLAR_CR           ((u32)0x00000001 << 12)          /*!<R/W 0  id[8:0] & qd[8:0] offset +/- => 0(+) / 1(-) */
#define WLAFE_MASK_DCK_KOS_EN_MANU_CR        ((u32)0x0000003F << 6)          /*!<R/W 000000  KOS_EN[5:0] by manual */
#define WLAFE_DCK_KOS_EN_MANU_CR(x)          ((u32)(((x) & 0x0000003F) << 6))
#define WLAFE_GET_DCK_KOS_EN_MANU_CR(x)      ((u32)(((x >> 6) & 0x0000003F)))
#define WLAFE_MASK_DCK_DOS_MANU_Q_CR         ((u32)0x0000003F << 0)          /*!<R/W 000000  DOS_Q[5:0] by manual */
#define WLAFE_DCK_DOS_MANU_Q_CR(x)           ((u32)(((x) & 0x0000003F) << 0))
#define WLAFE_GET_DCK_DOS_MANU_Q_CR(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup WLAFE_ANAPAR_DCK_3
 * @brief
 * @{
 **/
#define WLAFE_BIT_DCK_AD_DATA_SW_DEBUG_CR    ((u32)0x00000001 << 31)          /*!<R/W 0  0: choose I path vos data 1: choose Q path vos data */
#define WLAFE_MASK_DCK_ADC_OFS_Q_CR          ((u32)0x0000003F << 25)          /*!<R/W 000000  Compensate ADC qpath offset: adc2cal_AD[9:0] + ADC_ofs_CR[5:0] */
#define WLAFE_DCK_ADC_OFS_Q_CR(x)            ((u32)(((x) & 0x0000003F) << 25))
#define WLAFE_GET_DCK_ADC_OFS_Q_CR(x)        ((u32)(((x >> 25) & 0x0000003F)))
#define WLAFE_MASK_DCK_WAVE_OFSN_CR          ((u32)0x000000FF << 17)          /*!<R/W 00000001   */
#define WLAFE_DCK_WAVE_OFSN_CR(x)            ((u32)(((x) & 0x000000FF) << 17))
#define WLAFE_GET_DCK_WAVE_OFSN_CR(x)        ((u32)(((x >> 17) & 0x000000FF)))
#define WLAFE_MASK_DCK_RESERVE_CR            ((u32)0x000007FF << 6)          /*!<R/W 00000000000   */
#define WLAFE_DCK_RESERVE_CR(x)              ((u32)(((x) & 0x000007FF) << 6))
#define WLAFE_GET_DCK_RESERVE_CR(x)          ((u32)(((x >> 6) & 0x000007FF)))
#define WLAFE_BIT_REG_AD_IQ_CHGE             ((u32)0x00000001 << 5)          /*!<R/W 0  0:adci to DCK I ;adcq to DCK Q /1:adci to DCK Q;adcq to DCK I */
#define WLAFE_BIT_DCK_WL_KOS_EN_FORCE_Q_CR   ((u32)0x00000001 << 4)          /*!<R/W 0  Manual write KOSEN at WL mode */
#define WLAFE_BIT_DCK_WL_KOS_EN_FORCE_I_CR   ((u32)0x00000001 << 3)          /*!<R/W 0  Manual write KOSEN at WL mode */
#define WLAFE_BIT_DCK_WL_DOS_FORCE_Q_CR      ((u32)0x00000001 << 2)          /*!<R/W 0  Manual write DOS data at WL mode */
#define WLAFE_BIT_DCK_WL_DOS_FORCE_I_CR      ((u32)0x00000001 << 1)          /*!<R/W 0  Manual write DOS data at WL mode */
#define WLAFE_BIT_DCK_BT_KOS_EN_FORCE_Q_CR   ((u32)0x00000001 << 0)          /*!<R/W 0  Manual write KOSEN at BT mode */
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup WLAFE_Exported_Types WLAFE Exported Types
  * @{
*****************************************************************************/


/**
 * @brief WLAFE Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t WLAFE_ANAPAR_POW_MAC_0;                 /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t WLAFE_ANAPAR_MAC_0;                     /*!<  Register,  Address offset: 0x004 */
	__IO uint32_t WLAFE_ANAPAR_MAC_1;                     /*!<  Register,  Address offset: 0x008 */
	__IO uint32_t WLAFE_ANAPAR_MAC_2;                     /*!<  Register,  Address offset: 0x00C */
	__I  uint32_t WLAFE_DBG_WLAFE_PMC;                    /*!<  Register,  Address offset: 0x0010 */
	__I  uint32_t WLAFE_DBG_WLAFE;                        /*!<  Register,  Address offset: 0x0014 */
	__IO uint32_t RSVD0[2];                               /*!<  Reserved,  Address offset:0x18-0x1C */
	__IO uint32_t WLAFE_ANAPAR_RFC;                       /*!<  Register,  Address offset: 0x0020 */
	__IO uint32_t WLAFE_LDO_VPLUSE_SEL;                   /*!<  Register,  Address offset: 0x0024 */
	__IO uint32_t WLAFE_WLRFC_CTRL;                       /*!<  Register,  Address offset: 0x0028 */
	__IO uint32_t WLAFE_ANAPAR_DCK_0;                     /*!<  Register,  Address offset: 0x002C */
	__IO uint32_t WLAFE_ANAPAR_DCK_1;                     /*!<  Register,  Address offset: 0x0030 */
	__IO uint32_t WLAFE_ANAPAR_DCK_2;                     /*!<  Register,  Address offset: 0x0034 */
	__IO uint32_t WLAFE_ANAPAR_DCK_3;                     /*!<  Register,  Address offset: 0x0038 */
} WLAFE_TypeDef;

#define WLAFE_BASE ((WLAFE_TypeDef *) (0x42008940))

/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
