#ifndef _AMEBAD_SWR_H_
#define _AMEBAD_SWR_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup SWR
  * @brief SWR driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup SWR_Register_Definitions SWR Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup REGU_POWER_CTRL
 * @brief
 * @{
 **/
#define SWR_BIT_SWR1P8_MANUAL_EN          ((u32)0x00000001 << 19)          /*!<R/W 1  SWR manual mode enable. In manual mode,SWR's pwm_force pwm_mode use register */
#define SWR_BIT_MODE_L                    ((u32)0x00000001 << 18)          /*!<R 0  SWR current mode for debug */
#define SWR_BIT_BGOK_SWR                  ((u32)0x00000001 << 17)          /*!<R 0  SWR bang ok flag by analog */
#define SWR_BIT_PRDY_SWR                  ((u32)0x00000001 << 16)          /*!<R 0  SWR power ready flag by analog */
#define SWR_BIT_ENB_LDO_1P8_DIODE         ((u32)0x00000001 << 8)          /*!<R/W 0  control bit LDO diode for inrush current */
#define SWR_BIT_POW_LDOS_1P8              ((u32)0x00000001 << 3)          /*!<R/W 0  1:power small LDO */
#define SWR_BIT_POW_LDO_1P8               ((u32)0x00000001 << 2)          /*!<R/W 0  1:power LDO mode */
#define SWR_BIT_POW_BG                    ((u32)0x00000001 << 1)          /*!<R/W 0  1:power swr bg for swr mem . Note:other SWR's bg are shared with POR regu */
#define SWR_BIT_POW_SW                    ((u32)0x00000001 << 0)          /*!<R/W 0  1:power swr */
/** @} */

/** @defgroup SWR_ON_CTRL0
 * @brief
 * @{
 **/
#define SWR_BIT_PFMCMP_IQ                 ((u32)0x00000001 << 31)          /*!<R/W 1  PFM comparator bias, 0: 50nA 1:250nA */
#define SWR_BIT_REG_FORCE_IMIRROR_EN      ((u32)0x00000001 << 30)          /*!<R/W 0  Only valid for MEM, null0 for PSRAM // Force SWR IBIAS Generation circuit enable 1: force enable (need enable this circuit before enable small LDO for SWR, or any LDO using SWR IBIAS) 0: non force, IBIAS enable listen to REG_*_POW_SW (default) */
#define SWR_BIT_OFF_END_SEL               ((u32)0x00000001 << 29)          /*!<R/W 1  COT off time controlled by, 0: ZCD 1: min off time */
#define SWR_BIT_POW_MINOFF_L              ((u32)0x00000001 << 28)          /*!<R/W 1  0:normal current 1:increase OCP cmp current */
#define SWR_MASK_COT_I_L                  ((u32)0x00000003 << 26)          /*!<R/W 01  COT ON Current 00:150nA 01:150nA+100nA 10:150nA+150nA 11:150nA+250nA */
#define SWR_COT_I_L(x)                    ((u32)(((x) & 0x00000003) << 26))
#define SWR_GET_COT_I_L(x)                ((u32)(((x >> 26) & 0x00000003)))
#define SWR_MASK_VREFPFM_L                ((u32)0x0000000F << 22)          /*!<R/W 1100  SWR/LDO 0000 1.200 0001 1.250 0010 1.300 0011 1.350 0100 1.400 0101 1.450 0110 1.500 0111 1.550 1000 1.600 1001 1.650 1010 1.700 1011 1.750 1100 1.800 1101 1.850 1110 1.900 1111 1.950 */
#define SWR_VREFPFM_L(x)                  ((u32)(((x) & 0x0000000F) << 22))
#define SWR_GET_VREFPFM_L(x)              ((u32)(((x >> 22) & 0x0000000F)))
#define SWR_BIT_FORCE_ZCD_BIAS            ((u32)0x00000001 << 21)          /*!<R/W 1  0:ZCD bias on after SS 1:ZCD bias on when ZCD enable */
#define SWR_MASK_ZCD_SDZ_L                ((u32)0x00000003 << 19)          /*!<R/W 01  Increase ZCD delay cell */
#define SWR_ZCD_SDZ_L(x)                  ((u32)(((x) & 0x00000003) << 19))
#define SWR_GET_ZCD_SDZ_L(x)              ((u32)(((x >> 19) & 0x00000003)))
#define SWR_MASK_REG_ZCDC_H               ((u32)0x00000003 << 17)          /*!<R/W 10  00:0.1u 01:0.2u(PFM) 10:0.7u(PWM) 11:0.8u */
#define SWR_REG_ZCDC_H(x)                 ((u32)(((x) & 0x00000003) << 17))
#define SWR_GET_REG_ZCDC_H(x)             ((u32)(((x >> 17) & 0x00000003)))
#define SWR_BIT_POW_ZCD_L                 ((u32)0x00000001 << 16)          /*!<R/W 1  Enable ZCD */
#define SWR_MASK_OCP_L1                   ((u32)0x00000007 << 13)          /*!<R/W 011  FOR MEM FOR PSRAM PWM current(A) PFM current (A) PWM current(A) PFM current(A) 000 0.601 0.201 000 0.398 0.178 001 0.732 0.268 001 0.486 0.222 010 0.864 0.335 010 0.573 0.266 011 0.995 0.402 011 0.660 0.310 100 1.126 0.468 100 0.747 0.354 101 1.257 0.535 101 0.834 0.398 110 1.387 0.601 110 0.920 0.442 111 1.518 VIN/10 111 1.008 VIN/10 */
#define SWR_OCP_L1(x)                     ((u32)(((x) & 0x00000007) << 13))
#define SWR_GET_OCP_L1(x)                 ((u32)(((x >> 13) & 0x00000007)))
#define SWR_BIT_POWOCP_L1                 ((u32)0x00000001 << 12)          /*!<R/W 1  POWOCP_L1=1=> enable over current protection */
#define SWR_MASK_SAW_FREQ_L               ((u32)0x0000000F << 8)          /*!<R/W 1000  0000 458.0k 0001 680.2k 0010 898.9k 0011 1.113M 0100 1.321M 0101 1.522M 0110 1.716M 0111 1.902M 1000 2.087M 1001 2.258M 1010 2.426M 1011 2.582M 1100 2.743M 1101 2.888M 1110 3.035M 1111 3.165M */
#define SWR_SAW_FREQ_L(x)                 ((u32)(((x) & 0x0000000F) << 8))
#define SWR_GET_SAW_FREQ_L(x)             ((u32)(((x >> 8) & 0x0000000F)))
#define SWR_BIT_REG_BYPASS_L              ((u32)0x00000001 << 7)          /*!<R/W 1  1:bypass SSR latch */
#define SWR_BIT_FPWM_L1                   ((u32)0x00000001 << 6)          /*!<R/W 1  FPWM_L1 = 1 => force PWM FPWM_L1 = 0 => force PFM */
#define SWR_MASK_STD_L1                   ((u32)0x00000003 << 4)          /*!<R/W 1  STD_L1<1:0> time 00 750u 01 1.5m 10 3m 11 6m */
#define SWR_STD_L1(x)                     ((u32)(((x) & 0x00000003) << 4))
#define SWR_GET_STD_L1(x)                 ((u32)(((x >> 4) & 0x00000003)))
#define SWR_MASK_VOL_L1                   ((u32)0x0000000F << 0)          /*!<R/W 4'b0111  VREF 0.6V 0.575V 0000 1.426 1.367 0001 1.455 1.394 0010 1.501 1.438 0011 1.549 1.484 0100 1.583 1.517 0101 1.637 1.569 0110 1.695 1.624 0111 1.736 1.664 1000 1.801 1.726 1001 1.871 1.793 1010 1.921 1.841 1011 2.001 1.918 1100 2.088 2.001 1101 2.150 2.060 1110 2.251 2.157 1111 2.362 2.264 */
#define SWR_VOL_L1(x)                     ((u32)(((x) & 0x0000000F) << 0))
#define SWR_GET_VOL_L1(x)                 ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup SWR_ON_CTRL1
 * @brief
 * @{
 **/
#define SWR_MASK_SN_N_L                   ((u32)0x0000000F << 28)          /*!<R/W 1011  NGATE NMOS driving */
#define SWR_SN_N_L(x)                     ((u32)(((x) & 0x0000000F) << 28))
#define SWR_GET_SN_N_L(x)                 ((u32)(((x >> 28) & 0x0000000F)))
#define SWR_MASK_SP_N_L                   ((u32)0x0000000F << 24)          /*!<R/W 1000  NGATE PMOS driving */
#define SWR_SP_N_L(x)                     ((u32)(((x) & 0x0000000F) << 24))
#define SWR_GET_SP_N_L(x)                 ((u32)(((x >> 24) & 0x0000000F)))
#define SWR_MASK_SN_P_L                   ((u32)0x0000000F << 20)          /*!<R/W 0011  PGATE NMOS driving */
#define SWR_SN_P_L(x)                     ((u32)(((x) & 0x0000000F) << 20))
#define SWR_GET_SN_P_L(x)                 ((u32)(((x >> 20) & 0x0000000F)))
#define SWR_MASK_SP_P_L                   ((u32)0x0000000F << 16)          /*!<R/W 0011  PGATE PMOS driving */
#define SWR_SP_P_L(x)                     ((u32)(((x) & 0x0000000F) << 16))
#define SWR_GET_SP_P_L(x)                 ((u32)(((x >> 16) & 0x0000000F)))
#define SWR_BIT_VO_DISCHG_PWM_H           ((u32)0x00000001 << 15)          /*!<R/W 0  1:dummy loading 5mA */
#define SWR_BIT_REG_MODE_PREDRIVER        ((u32)0x00000001 << 14)          /*!<R/W 1  1:3 level predriver */
#define SWR_MASK_REG_ADJSLDO_L            ((u32)0x0000000F << 10)          /*!<R/W 0110  0000 1.413 0001 1.451 0010 1.491 0011 1.534 0100 1.579 0101 1.627 0110 1.678 0111 1.732 1000 1.790 1001 1.852 1010 1.918 1011 1.989 1100 2.065 1101 2.148 1110 2.238 1111 2.335 */
#define SWR_REG_ADJSLDO_L(x)              ((u32)(((x) & 0x0000000F) << 10))
#define SWR_GET_REG_ADJSLDO_L(x)          ((u32)(((x >> 10) & 0x0000000F)))
#define SWR_BIT_EN_LDOS_DIODE             ((u32)0x00000001 << 9)          /*!<R/W 1  1 : LDOS diode enable */
#define SWR_BIT_PWM_FORCE                 ((u32)0x00000001 << 8)          /*!<R/W 0  0: PWM bias enabled only at PWM mode 1: Force PWM bias enable when in PWM/PFM mode */
#define SWR_BIT_PFM_PD_RST                ((u32)0x00000001 << 7)          /*!<R/W 1  1: When leave PFM mode, COT logic is reset */
#define SWR_BIT_VC_PFM_RSTB               ((u32)0x00000001 << 6)          /*!<R/W 0  When into PFM, VC is, 0: clamped (bit 40 also =1) 1. floating */
#define SWR_BIT_PFM_IN_SEL                ((u32)0x00000001 << 5)          /*!<R/W 1  Power MOS into PFM when PFM CTRL, 0: rising 1: falling */
#define SWR_BIT_VC_RSTB                   ((u32)0x00000001 << 4)          /*!<R/W 0  Before SS VC is, 0: pulled low 1: floating */
#define SWR_BIT_FPWMDELAY                 ((u32)0x00000001 << 3)          /*!<R/W 0  1:PFM<=>PWM PFM/PWM change by using DFF(CKMOD) */
#define SWR_BIT_ENFPWMDELAY_H             ((u32)0x00000001 << 2)          /*!<R/W 0  1:PFM=>PWM delay PWM enable and wait one clock cycle then PFM off */
#define SWR_BIT_REG_MOS_HALF_L            ((u32)0x00000001 << 1)          /*!<R/W 0  1:power mos half */
#define SWR_BIT_CURRENT_SENSE_MOS         ((u32)0x00000001 << 0)          /*!<R/W 0  1: current sensing MOS for large OC current */
/** @} */

/** @defgroup SWR_ON_CTRL2
 * @brief
 * @{
 **/
#define SWR_MASK_REG_BG_CURRENT_L         ((u32)0x00000003 << 4)          /*!<R/W 00  Only valid for MEM, null1 for PSRAM 00:23.9n 01:54.6n 10:117.8n */
#define SWR_REG_BG_CURRENT_L(x)           ((u32)(((x) & 0x00000003) << 4))
#define SWR_GET_REG_BG_CURRENT_L(x)       ((u32)(((x >> 4) & 0x00000003)))
#define SWR_MASK_REG_SWR_ZCD_CTRL         ((u32)0x00000003 << 2)          /*!<R/W 10  // SWR ZCD CTRL control <1>1:double current <0>1'b1: force in CCM/DCM boundary mode(default), 1'b0 : no boundary */
#define SWR_REG_SWR_ZCD_CTRL(x)           ((u32)(((x) & 0x00000003) << 2))
#define SWR_GET_REG_SWR_ZCD_CTRL(x)       ((u32)(((x >> 2) & 0x00000003)))
#define SWR_MASK_REG_PWM_VREF_SEL         ((u32)0x00000003 << 0)          /*!<R/W 11  PWM_VREF_SEL 2'b11 0P600V for SWR1P8V (DDR2) (default) 2'b10 0P575V for SWR1P725V (DDR2 trim) 2'b01 0P500V for SWR1P5V (DDR3) 2'b00 0P450V for SWR1P35V (DDR3L) */
#define SWR_REG_PWM_VREF_SEL(x)           ((u32)(((x) & 0x00000003) << 0))
#define SWR_GET_REG_PWM_VREF_SEL(x)       ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup SWR_OFF_CTRL0
 * @brief
 * @{
 **/
#define SWR_MASK_SDZN_L                   ((u32)0x00000003 << 30)          /*!<R/W 01  Nonoverlap delay cell for NMOS */
#define SWR_SDZN_L(x)                     ((u32)(((x) & 0x00000003) << 30))
#define SWR_GET_SDZN_L(x)                 ((u32)(((x >> 30) & 0x00000003)))
#define SWR_BIT_REG_AUTOZCD_L             ((u32)0x00000001 << 29)          /*!<R/W 0  0:register 1:10000 */
#define SWR_BIT_REG_VOFB_SEL              ((u32)0x00000001 << 28)          /*!<R/W 0  0:vo15 1:LX_SPS */
#define SWR_MASK_TBOX_L1                  ((u32)0x00000003 << 26)          /*!<R/W 00  TBOX_L1<1:0> output 00 CKOVER/RAMPOVER 01 NI/PI 10 CKSS/SSOVER 11 i/NIOFF_H */
#define SWR_TBOX_L1(x)                    ((u32)(((x) & 0x00000003) << 26))
#define SWR_GET_TBOX_L1(x)                ((u32)(((x >> 26) & 0x00000003)))
#define SWR_BIT_ENOCPMUX_L                ((u32)0x00000001 << 25)          /*!<R/W 1  1:OCP signal control by MUX 0:OCP signal control by I */
#define SWR_BIT_FORCE_LDOS                ((u32)0x00000001 << 24)          /*!<R/W 0  1:LDOS force on */
#define SWR_BIT_VO_DISCHG                 ((u32)0x00000001 << 23)          /*!<R/W 0  1:VO with 1mA discharge */
#define SWR_BIT_LDO_OC_CLAMP              ((u32)0x00000001 << 22)          /*!<R/W 1  0:OC clamp with P/NMOS 1:OC clamp with PMOS */
#define SWR_BIT_MINOFF_LIQ                ((u32)0x00000001 << 21)          /*!<R/W 1  0:Min. off circuit always on 1:Min. off circuit pulse on */
#define SWR_BIT_MINON_LIQ                 ((u32)0x00000001 << 20)          /*!<R/W 1  0:Min. on circuit always on 1:Min. on circuit pulse on */
#define SWR_BIT_POW_AUTO_L                ((u32)0x00000001 << 19)          /*!<R/W 0  1:auto PFM/PWM 0:register control */
#define SWR_BIT_REG_VC_CLAMP_SEL_L        ((u32)0x00000001 << 18)          /*!<R/W 0  0:unity gain feedback clamp VC=VREF 1.orignal clamp(roll back option) */
#define SWR_BIT_NO_OFFTIME_L              ((u32)0x00000001 << 17)          /*!<R/W 0  0:from min OFF time(PFM COT) 1:always on */
#define SWR_BIT_EN_ON_END_L               ((u32)0x00000001 << 16)          /*!<R/W 1  1:EN min OFF time (PFM COT) */
#define SWR_BIT_ENCOT_L                   ((u32)0x00000001 << 15)          /*!<R/W 1  1:PFM COT */
#define SWR_MASK_REG_CLK_SEL              ((u32)0x00000003 << 13)          /*!<R/W 11  00:out of phase CKSS 01:out of phase PWM_CTRIL 10:ckout 11:HG */
#define SWR_REG_CLK_SEL(x)                ((u32)(((x) & 0x00000003) << 13))
#define SWR_GET_REG_CLK_SEL(x)            ((u32)(((x >> 13) & 0x00000003)))
#define SWR_BIT_REG_TYPE_L                ((u32)0x00000001 << 12)          /*!<R/W 1  1:type3 0:type2 */
#define SWR_MASK_R3_L1                    ((u32)0x00000003 << 10)          /*!<R/W 10  00 => 36k 01 => 30k 10 => 24k 11 => 24k */
#define SWR_R3_L1(x)                      ((u32)(((x) & 0x00000003) << 10))
#define SWR_GET_R3_L1(x)                  ((u32)(((x >> 10) & 0x00000003)))
#define SWR_MASK_R2_L1                    ((u32)0x00000003 << 8)          /*!<R/W 10  00 => 0.4M 01 => 0.3M 10 => 0.2M 11 => 0.1M */
#define SWR_R2_L1(x)                      ((u32)(((x) & 0x00000003) << 8))
#define SWR_GET_R2_L1(x)                  ((u32)(((x >> 8) & 0x00000003)))
#define SWR_MASK_R1_L1                    ((u32)0x00000003 << 6)          /*!<R/W 10  00 => 900k 01 => 720k 10 => 900k 11 => 1080k */
#define SWR_R1_L1(x)                      ((u32)(((x) & 0x00000003) << 6))
#define SWR_GET_R1_L1(x)                  ((u32)(((x >> 6) & 0x00000003)))
#define SWR_MASK_C3_L1                    ((u32)0x00000003 << 4)          /*!<R/W 10  00 => 1.59p 01 => 3.2p 10 => 4.76p 11 => 6.34p */
#define SWR_C3_L1(x)                      ((u32)(((x) & 0x00000003) << 4))
#define SWR_GET_C3_L1(x)                  ((u32)(((x >> 4) & 0x00000003)))
#define SWR_MASK_C2_L1                    ((u32)0x00000003 << 2)          /*!<R/W 10  00 => 13f 01 => 26f 10 => 39f 11 => 41f */
#define SWR_C2_L1(x)                      ((u32)(((x) & 0x00000003) << 2))
#define SWR_GET_C2_L1(x)                  ((u32)(((x >> 2) & 0x00000003)))
#define SWR_MASK_C1_L1                    ((u32)0x00000003 << 0)          /*!<R/W 10  00 => 1.03p 01 => 2.06p 10 => 3.1p 11 => 4.12p */
#define SWR_C1_L1(x)                      ((u32)(((x) & 0x00000003) << 0))
#define SWR_GET_C1_L1(x)                  ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup SWR_OFF_CTRL1
 * @brief
 * @{
 **/
#define SWR_MASK_VOFB_SEL                 ((u32)0x00000003 << 12)          /*!<R/W 00  // output feedback select, need set bits before SWR enable // 00 (default) from SWR internal PAD // 01 : from SWR external PAD_1 // 10: from SWR external PAD_2 // 11: from SWR external PAD_3 */
#define SWR_VOFB_SEL_(x)                  ((u32)(((x) & 0x00000003) << 12))
#define SWR_GET_VOFB_SEL(x)               ((u32)(((x >> 12) & 0x00000003)))
#define SWR_MASK_BG_R1_TUNE               ((u32)0x00000007 << 9)          /*!<R/W 100  BG R2電阻調整 */
#define SWR_BG_R1_TUNE(x)                 ((u32)(((x) & 0x00000007) << 9))
#define SWR_GET_BG_R1_TUNE(x)             ((u32)(((x >> 9) & 0x00000007)))
#define SWR_MASK_BG_R2_TUNE               ((u32)0x00000007 << 6)          /*!<R/W 100  BG R1電阻調整 */
#define SWR_BG_R2_TUNE(x)                 ((u32)(((x) & 0x00000007) << 6))
#define SWR_GET_BG_R2_TUNE(x)             ((u32)(((x >> 6) & 0x00000007)))
#define SWR_BIT_REG_NMOS_OFF_L            ((u32)0x00000001 << 5)          /*!<R/W 0  0:normal 1:NMOS always off */
#define SWR_BIT_REG_MUX_PI_L              ((u32)0x00000001 << 4)          /*!<R/W 0  1:PI (original) 0:i */
#define SWR_BIT_REG_PWM_CTRL_L            ((u32)0x00000001 << 3)          /*!<R/W 0  0:internal PWM 1:external PWM */
#define SWR_BIT_ENSR_L                    ((u32)0x00000001 << 2)          /*!<R/W 1  1:with SSR latch */
#define SWR_MASK_SDZP_L                   ((u32)0x00000003 << 0)          /*!<R/W 01  Nonoverlap delay cell for PMOS */
#define SWR_SDZP_L(x)                     ((u32)(((x) & 0x00000003) << 0))
#define SWR_GET_SDZP_L(x)                 ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup SWR_DIG_OCP
 * @brief
 * @{
 **/
#define SWR_BIT_SPS_DIG_OCP_CKE           ((u32)0x00000001 << 31)          /*!<R/W 1  SPS OCP clock enable */
#define SWR_MASK_SPS_DIG_OCP_THD          ((u32)0x00007FFF << 16)          /*!<R/W 0  SPS OCP event threshold */
#define SWR_SPS_DIG_OCP_THD(x)            ((u32)(((x) & 0x00007FFF) << 16))
#define SWR_GET_SPS_DIG_OCP_THD(x)        ((u32)(((x >> 16) & 0x00007FFF)))
#define SWR_MASK_SPS_DIG_OCP_WND          ((u32)0x0000FFFF << 0)          /*!<R/W 0  SPS OCP time window */
#define SWR_SPS_DIG_OCP_WND(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define SWR_GET_SPS_DIG_OCP_WND(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup SWR_DIG_ZCD
 * @brief
 * @{
 **/
#define SWR_MASK_SPS_DIG_ZCD_DBG          ((u32)0x0000001F << 16)          /*!<R 0  The acture zcd code used for SPS core */
#define SWR_SPS_DIG_ZCD_DBG(x)            ((u32)(((x) & 0x0000001F) << 16))
#define SWR_GET_SPS_DIG_ZCD_DBG(x)        ((u32)(((x >> 16) & 0x0000001F)))
#define SWR_BIT_SPS_DIG_ZCD_MODE          ((u32)0x00000001 << 9)          /*!<R/W 0  SPS ZCD SW mode enable */
#define SWR_BIT_SPS_DIG_ZCD_AUTO          ((u32)0x00000001 << 8)          /*!<R/W 1  SPS HW ZCD code auto count function enable */
#define SWR_MASK_SPS_DIG_ZCD_CODE         ((u32)0x0000001F << 0)          /*!<R/W 10000  SPS_DIG_ZCD_CODE,this is used for SW mode */
#define SWR_SPS_DIG_ZCD_CODE(x)           ((u32)(((x) & 0x0000001F) << 0))
#define SWR_GET_SPS_DIG_ZCD_CODE(x)       ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup SWR_NEW
 * @brief
 * @{
 **/
#define SWR_MASK_OCP_L1_PFM               ((u32)0x00000007 << 17)          /*!<R/W 011  FOR MEM FOR PSRAM PWM current(A) PFM current (A) PWM current(A) PFM current(A) 000 0.601 0.201 000 0.398 0.178 001 0.732 0.268 001 0.486 0.222 010 0.864 0.335 010 0.573 0.266 011 0.995 0.402 011 0.660 0.310 100 1.126 0.468 100 0.747 0.354 101 1.257 0.535 101 0.834 0.398 110 1.387 0.601 110 0.920 0.442 111 1.518 VIN/10 111 1.008 VIN/10 */
#define SWR_OCP_L1_PFM(x)                 ((u32)(((x) & 0x00000007) << 17))
#define SWR_GET_OCP_L1_PFM(x)             ((u32)(((x >> 17) & 0x00000007)))
#define SWR_BIT_REG_NMOS_OFF_L_PFM        ((u32)0x00000001 << 16)          /*!<R/W 0  0:normal 1:NMOS always off PFM mode parametr */
#define SWR_BIT_REG_NMOS_OFF_L_PWM_LL     ((u32)0x00000001 << 15)          /*!<R/W 0  0:normal 1:NMOS always off PWM mode parametr for system low load */
#define SWR_BIT_REG_PSRR_H_PWM_LL         ((u32)0x00000001 << 14)          /*!<R/W 0  0:default 1:res+cap PWM mode parametr for system low load */
#define SWR_MASK_REG_SWR_ZCD_CTRL_PFM     ((u32)0x00000003 << 12)          /*!<R/W 00  // SWR ZCD CTRL control <1>1:double current <0>1'b1: force in CCM/DCM boundary mode(default), 1'b0 : no boundary PFM mode parametr */
#define SWR_REG_SWR_ZCD_CTRL_PFM(x)       ((u32)(((x) & 0x00000003) << 12))
#define SWR_GET_REG_SWR_ZCD_CTRL_PFM(x)   ((u32)(((x >> 12) & 0x00000003)))
#define SWR_MASK_REG_SWR_ZCD_CTRL_PWM_LL  ((u32)0x00000003 << 10)          /*!<R/W 00  // SWR ZCD CTRL control <1>1:double current <0>1'b1: force in CCM/DCM boundary mode(default), 1'b0 : no boundary PWM mode parametr for system low load */
#define SWR_REG_SWR_ZCD_CTRL_PWM_LL(x)    ((u32)(((x) & 0x00000003) << 10))
#define SWR_GET_REG_SWR_ZCD_CTRL_PWM_LL(x) ((u32)(((x >> 10) & 0x00000003)))
#define SWR_BIT_REG_MOS_HALF_L_PFM        ((u32)0x00000001 << 9)          /*!<R/W 1  1:power mos half PFM mode parametr */
#define SWR_BIT_REG_MOS_HALF_L_PWM_LL     ((u32)0x00000001 << 8)          /*!<R/W 1  1:power mos half PWM mode parametr for system low load */
#define SWR_MASK_REG_ZCDC_H_PFM           ((u32)0x00000003 << 6)          /*!<R/W 1  00:0.1u 01:0.2u(PFM) 10:0.7u(PWM) 11:0.8u PFM mode parametr */
#define SWR_REG_ZCDC_H_PFM(x)             ((u32)(((x) & 0x00000003) << 6))
#define SWR_GET_REG_ZCDC_H_PFM(x)         ((u32)(((x >> 6) & 0x00000003)))
#define SWR_MASK_REG_ZCDC_H_PWM_LL        ((u32)0x00000003 << 4)          /*!<R/W 11  00:0.1u 01:0.2u(PFM) 10:0.7u(PWM) 11:0.8u PWM mode parametr for system low load */
#define SWR_REG_ZCDC_H_PWM_LL(x)          ((u32)(((x) & 0x00000003) << 4))
#define SWR_GET_REG_ZCDC_H_PWM_LL(x)      ((u32)(((x >> 4) & 0x00000003)))
#define SWR_MASK_SAW_FREQ_L_PWM_LL        ((u32)0x0000000F << 0)          /*!<R/W 1000  0000 458.0k 0001 680.2k 0010 898.9k 0011 1.113M 0100 1.321M 0101 1.522M 0110 1.716M 0111 1.902M 1000 2.087M 1001 2.258M 1010 2.426M 1011 2.582M 1100 2.743M 1101 2.888M 1110 3.035M 1111 3.165M PWM mode parametr for system low load */
#define SWR_SAW_FREQ_L_PWM_LL(x)          ((u32)(((x) & 0x0000000F) << 0))
#define SWR_GET_SAW_FREQ_L_PWM_LL(x)      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup SWR_Exported_Types SWR Exported Types
  * @{
*****************************************************************************/


/**
 * @brief SWR Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t REGU_POWER_CTRL;                        /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t RSVD0[3];                               /*!<  Reserved,  Address offset:0x4-0xC */
	__IO uint32_t SWR_ON_CTRL0;                           /*!<  Register,  Address offset: 0x010 */
	__IO uint32_t SWR_ON_CTRL1;                           /*!<  Register,  Address offset: 0x014 */
	__IO uint32_t SWR_ON_CTRL2;                           /*!<  Register,  Address offset: 0x018 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x1C */
	__IO uint32_t SWR_OFF_CTRL0;                          /*!<  Register,  Address offset: 0x020 */
	__IO uint32_t SWR_OFF_CTRL1;                          /*!<  Register,  Address offset: 0x024 */
	__IO uint32_t RSVD2[2];                               /*!<  Reserved,  Address offset:0x28-0x2C */
	__IO uint32_t SWR_DIG_OCP;                            /*!<  Register,  Address offset: 0x030 */
	__IO uint32_t RSVD3;                                  /*!<  Reserved,  Address offset:0x34 */
	__IO uint32_t SWR_DIG_ZCD;                            /*!<  Register,  Address offset: 0x038 */
	__IO uint32_t SWR_NEW;                                /*!<  Register,  Address offset: 0x03C */
} SWR_TypeDef;

#define SWR1_BASE ((SWR_TypeDef *) (0x42008880))
#define SWR2_BASE ((SWR_TypeDef *) (0x420088C0))

/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here
/** @defgroup SWR_MODE SWR mode
  * @{
  */
#define SWR_PFM							((u32)0x00000000)
#define SWR_PWM							((u32)0x00000001)
#define DDR3L_Vol						((u32)0x00000000)
#define DDR2_PS_Vol						((u32)0x00000003)

#define DDR3L_Vol_PFM					((u32)0x00000003)
#define DDR2_PS_Vol_PFM					((u32)0x0000000c)

#define IS_SWR_MODE(MODE) (((MODE) == SWR_PFM) || \
										((MODE) == SWR_PWM))

/** @defgroup SWR_Exported_Functions SWR Exported Functions
  * @{
  */
_LONG_CALL_ u32 SWR_MEM(u32 NewState);
_LONG_CALL_ void SWR_MEM_Manual(u32 NewState);
_LONG_CALL_ u32 SWR_MEM_Mode_Get(void);
_LONG_CALL_ u32 SWR_MEM_Mode_Set(u32 SWR_Mode);
_LONG_CALL_ u32 SWR_AUDIO(u32 NewState);
_LONG_CALL_ void SWR_AUDIO_Manual(u32 NewState);
_LONG_CALL_ u32 SWR_AUDIO_Mode_Get(void);
_LONG_CALL_ u32 SWR_AUDIO_Mode_Set(u32 SWR_Mode);
_LONG_CALL_ void SWR_PFM_MODE_Set(u32 MODE);
_LONG_CALL_ void SWR_BST_MODE_Set(u32 MODE);
/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
