#ifndef _AMEBAD2_PMC_H_
#define _AMEBAD2_PMC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PMC
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup PMC_Register_Definitions PMC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup WAK_STATUS0
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_STA_31_0             ((u32)0xFFFFFFFF << 0)          /*!<R 0  wak evt status. It can't be cleare here, clear at IP side . Bit0: WIFI_FISR_FESR_IRQ Bit1: WIFI_FTSR_MAILBOX_IRQ Bit2: AP_WAKE_IRQ Bit3: DSP_WAKE_IRQ Bit4: BT_WAKE_HOST Bit5: IPC_KR4 Bit6: IPC_KM4 Bit7: IPC_DSP Bit8: IWDG Bit9: Timer0 Bit10: Timer1 Bit11: Timer2 Bit12: Timer3 Bit13: Timer4 Bit14: Timer5 Bit15: Timer6 Bit16: Timer7 Bit17: UART0 Bit18: UART1 Bit19: UART2 Bit20: UART3 Bit21: UART_LOG Bit22: GPIOA Bit23: GPIOB Bit24: CTOUCH Bit25: SPI0 Bit26: SPI1 Bit27: AON_RTS Bit28: ADC Bit29: ADC_COMP Bit30: BOR Bit31: PWR_DOWN */
#define PMC_WAK_STA_31_0(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_STA_31_0(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_STATUS1
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_STA_63_32            ((u32)0xFFFFFFFF << 0)          /*!<R 0  wak evt status. It can't be cleare here, clear at IP side . Bit0: AON_TIMER Bit1: AON_GPIO_WAKE Bit2: VAD Bit3~31: Reserved */
#define PMC_WAK_STA_63_32(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_STA_63_32(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_MASK0_NP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_NP_IMR_31_0          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake NP, this used to wak pmc */
#define PMC_WAK_NP_IMR_31_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_NP_IMR_31_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_MASK1_NP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_NP_IMR_63_32         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake NP */
#define PMC_WAK_NP_IMR_63_32(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_NP_IMR_63_32(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_MASK0_AP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_AP_IMR_31_0          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake AP */
#define PMC_WAK_AP_IMR_31_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_AP_IMR_31_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_MASK1_AP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_AP_IMR_63_32         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake AP */
#define PMC_WAK_AP_IMR_63_32(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_AP_IMR_63_32(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_MASK0_DSP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_DSP_IMR_31_0         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake DSP */
#define PMC_WAK_DSP_IMR_31_0(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_DSP_IMR_31_0(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_MASK1_DSP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_DSP_IMR_63_32        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake DSP */
#define PMC_WAK_DSP_IMR_63_32(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_DSP_IMR_63_32(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WLPMC_ISR
 * @brief
 * @{
 **/
#define PMC_BIT_WL_OFF_IMR                ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define PMC_BIT_WL_ON_IMR                 ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define PMC_BIT_WL_OFF_ISR                ((u32)0x00000001 << 1)          /*!<RW1CB 0   */
#define PMC_BIT_WL_ON_ISR                 ((u32)0x00000001 << 0)          /*!<RW1CB 0   */
/** @} */

/** @defgroup WLAN_PWRCTL
 * @brief
 * @{
 **/
#define PMC_BIT_SW_CLR_RDY2ACT            ((u32)0x00000001 << 17)          /*!<R/WA0 0  System infrom WLAN ready to active */
#define PMC_BIT_SW_SET_RDY2ACT            ((u32)0x00000001 << 16)          /*!<R/WA0 0   */
#define PMC_BIT_WL_RDY2ACT                ((u32)0x00000001 << 9)          /*!<R 0  System infrom WLAN ready to active */
#define PMC_BIT_WL_REQ2ACT                ((u32)0x00000001 << 8)          /*!<R 0   */
#define PMC_BIT_WL_HW_RESP_FEN            ((u32)0x00000001 << 0)          /*!<R/W 1  1: enable HW auto response */
/** @} */

/** @defgroup SYSPMC_CTRL
 * @brief
 * @{
 **/
#define PMC_MASK_STL_PDPBG                ((u32)0x00000003 << 28)          /*!<R/W 0x1  timer wait for PLL bandgag power stable . Spec is 160us 00:8us * 2^4 01:8us * 2^5 10:8us * 2^6 11:8us * 2^7 */
#define PMC_STL_PDPBG(x)                  ((u32)(((x) & 0x00000003) << 28))
#define PMC_GET_STL_PDPBG(x)              ((u32)(((x >> 28) & 0x00000003)))
#define PMC_MASK_STL_PDSOC                ((u32)0x00000003 << 24)          /*!<R/W 0x3  timer wait for core power power-cut power stable . 00:8us * 2^5 01:8us * 2^6 10:8us * 2^7 11:8us * 2^8 */
#define PMC_STL_PDSOC(x)                  ((u32)(((x) & 0x00000003) << 24))
#define PMC_GET_STL_PDSOC(x)              ((u32)(((x >> 24) & 0x00000003)))
#define PMC_MASK_TUTIME                   ((u32)0x0000003F << 16)          /*!<R/W 0x1F  PMC timeunit setting, in cycles of osc4M e.g, default 0x1F, timeunit = 8us */
#define PMC_TUTIME(x)                     ((u32)(((x) & 0x0000003F) << 16))
#define PMC_GET_TUTIME(x)                 ((u32)(((x >> 16) & 0x0000003F)))
#define PMC_BIT_MEM_SLP_MODE              ((u32)0x00000001 << 5)          /*!<R/W 0  memory power mode parameter select 1:select paramter of sleep mode 0:select paramter of normal mode */
#define PMC_BIT_POF_WAIT_SLEP             ((u32)0x00000001 << 4)          /*!<R/W 1  1.when SW set pmen_slep or pmen_dslp , hw pmc will wait km0 enter sleep mode , then begin to pof seq 0.Not wait km0 sleep */
#define PMC_BIT_KR4_IRQ_MASK              ((u32)0x00000001 << 3)          /*!<R/W 0  1.mask all KR4 interrupt 0:release all KR4 interrupt */
#define PMC_BIT_KM4_IRQ_MASK              ((u32)0x00000001 << 2)          /*!<R/W 0  1.mask all KM4 interrupt 0:release all KM4 interrupt */
#define PMC_BIT_PMEN_SLEP                 ((u32)0x00000001 << 1)          /*!<R/W 0  1: Request PMC to enter sleep state, auto clear to 0 after PMC finishes */
#define PMC_BIT_PMEN_DSLP                 ((u32)0x00000001 << 0)          /*!<R/W 0  1: Request PMC to enter deep-sleep state, auto clear to 0 after PMC finishes */
/** @} */

/** @defgroup SYSPMC_OPT
 * @brief
 * @{
 **/
#define PMC_MASK_PST_NORM_XMD             ((u32)0x00000003 << 20)          /*!<R/W 0x2  Normal setting for XTAL mode selection 00: XTAL will be off when PMC enter into sleep mode 01: XTAL will be in LPS when PMC enter into sleep mode 10/11: XTAL will be in Normal when PMC enter into sleep mode */
#define PMC_PST_NORM_XMD(x)               ((u32)(((x) & 0x00000003) << 20))
#define PMC_GET_PST_NORM_XMD(x)           ((u32)(((x >> 20) & 0x00000003)))
#define PMC_MASK_PST_SLEP_XMD             ((u32)0x00000003 << 12)          /*!<R/W 0x0  Sleep setting for XTAL mode selection 00: XTAL will be off when PMC enter into sleep mode 01: XTAL will be in LPS when PMC enter into sleep mode 10/11: XTAL will be in Normal when PMC enter into sleep mode */
#define PMC_PST_SLEP_XMD(x)               ((u32)(((x) & 0x00000003) << 12))
#define PMC_GET_PST_SLEP_XMD(x)           ((u32)(((x >> 12) & 0x00000003)))
#define PMC_BIT_PST_SLEP_LDOM             ((u32)0x00000001 << 9)          /*!<R/W 0  Sleep setting for core LDO mode selection(the bit is discarded,use PST_SLEP_EPWM instead) 1: core LDO keeps at normal mode when PMC enter into sleep mode 0: core switch to standby mode when PMC enter into sleep mode */
#define PMC_BIT_PST_SLEP_EPWM             ((u32)0x00000001 << 8)          /*!<R/W 0  Sleep setting for SWR mode selection 1: SWR keeps at PWM mode when PMC enter into sleep mode 0: SWR switch to PFM when PMC enter into sleep mode */
#define PMC_BIT_PST_SLEP_ERCK             ((u32)0x00000001 << 4)          /*!<R/W 0  1: Enable lpon clock when PMC enter into sleep mode 0: Gate lpon clock when PMC enter into sleep mode */
#define PMC_BIT_PST_SLEP_EBUS             ((u32)0x00000001 << 2)          /*!<R/W 0  1: Enable platform clock when PMC entro into sleep mode 0: Gate platform clock when PMC entro into sleep mode */
#define PMC_BIT_PST_SLEP_ESOC             ((u32)0x00000001 << 0)          /*!<R/W 0  1: power enable SOC platform when PMC enter into sleep mode (clock-gating) 0: power off SoC domain when PMC enter into sleep mode (power-gating) */
/** @} */

/** @defgroup PMC_PATCH_GRP0_0
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP0_INSTR1_5_0    ((u32)0x0000003F << 26)          /*!<R/W 0  instruction1 of LSYSON PMC patch group0,bit[5:0] */
#define PMC_PATCH_GRP0_INSTR1_5_0(x)      ((u32)(((x) & 0x0000003F) << 26))
#define PMC_GET_PATCH_GRP0_INSTR1_5_0(x)  ((u32)(((x >> 26) & 0x0000003F)))
#define PMC_MASK_PATCH_GRP0_INSTR0        ((u32)0x0003FFFF << 8)          /*!<R/W 0  instruction0 of LSYSON PMC patch group0 */
#define PMC_PATCH_GRP0_INSTR0(x)          ((u32)(((x) & 0x0003FFFF) << 8))
#define PMC_GET_PATCH_GRP0_INSTR0(x)      ((u32)(((x >> 8) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP0_SADDR         ((u32)0x000000FF << 0)          /*!<R/W 0  Start address of the first instruction of LSYSON PMC patch group0 */
#define PMC_PATCH_GRP0_SADDR(x)           ((u32)(((x) & 0x000000FF) << 0))
#define PMC_GET_PATCH_GRP0_SADDR(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PMC_PATCH_GRP0_1
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP0_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group0 */
#define PMC_PATCH_GRP0_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP0_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP0_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group0,bit[17:6] */
#define PMC_PATCH_GRP0_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP0_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup PMC_PATCH_GRP1_0
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP1_INSTR1_5_0    ((u32)0x0000003F << 26)          /*!<R/W 0  instruction1 of LSYSON PMC patch group1,bit[5:0] */
#define PMC_PATCH_GRP1_INSTR1_5_0(x)      ((u32)(((x) & 0x0000003F) << 26))
#define PMC_GET_PATCH_GRP1_INSTR1_5_0(x)  ((u32)(((x >> 26) & 0x0000003F)))
#define PMC_MASK_PATCH_GRP1_INSTR0        ((u32)0x0003FFFF << 8)          /*!<R/W 0  instruction0 of LSYSON PMC patch group1 */
#define PMC_PATCH_GRP1_INSTR0(x)          ((u32)(((x) & 0x0003FFFF) << 8))
#define PMC_GET_PATCH_GRP1_INSTR0(x)      ((u32)(((x >> 8) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP1_SADDR         ((u32)0x000000FF << 0)          /*!<R/W 0  Start address of the first instruction of LSYSON PMC patch group1 */
#define PMC_PATCH_GRP1_SADDR(x)           ((u32)(((x) & 0x000000FF) << 0))
#define PMC_GET_PATCH_GRP1_SADDR(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PMC_PATCH_GRP1_1
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP1_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group1 */
#define PMC_PATCH_GRP1_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP1_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP1_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group1,bit[17:6] */
#define PMC_PATCH_GRP1_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP1_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup PMC_PATCH_GRP2_0
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP2_INSTR1_5_0    ((u32)0x0000003F << 26)          /*!<R/W 0  instruction1 of LSYSON PMC patch group2,bit[5:0] */
#define PMC_PATCH_GRP2_INSTR1_5_0(x)      ((u32)(((x) & 0x0000003F) << 26))
#define PMC_GET_PATCH_GRP2_INSTR1_5_0(x)  ((u32)(((x >> 26) & 0x0000003F)))
#define PMC_MASK_PATCH_GRP2_INSTR0        ((u32)0x0003FFFF << 8)          /*!<R/W 0  instruction0 of LSYSON PMC patch group2 */
#define PMC_PATCH_GRP2_INSTR0(x)          ((u32)(((x) & 0x0003FFFF) << 8))
#define PMC_GET_PATCH_GRP2_INSTR0(x)      ((u32)(((x >> 8) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP2_SADDR         ((u32)0x000000FF << 0)          /*!<R/W 0  Start address of the first instruction of LSYSON PMC patch group2 */
#define PMC_PATCH_GRP2_SADDR(x)           ((u32)(((x) & 0x000000FF) << 0))
#define PMC_GET_PATCH_GRP2_SADDR(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PMC_PATCH_GRP2_1
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP2_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group2 */
#define PMC_PATCH_GRP2_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP2_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP2_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group2,bit[17:6] */
#define PMC_PATCH_GRP2_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP2_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup PMC_PATCH_GRP3_0
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP3_INSTR1_5_0    ((u32)0x0000003F << 26)          /*!<R/W 0  instruction1 of LSYSON PMC patch group3,bit[5:0] */
#define PMC_PATCH_GRP3_INSTR1_5_0(x)      ((u32)(((x) & 0x0000003F) << 26))
#define PMC_GET_PATCH_GRP3_INSTR1_5_0(x)  ((u32)(((x >> 26) & 0x0000003F)))
#define PMC_MASK_PATCH_GRP3_INSTR0        ((u32)0x0003FFFF << 8)          /*!<R/W 0  instruction0 of LSYSON PMC patch group3 */
#define PMC_PATCH_GRP3_INSTR0(x)          ((u32)(((x) & 0x0003FFFF) << 8))
#define PMC_GET_PATCH_GRP3_INSTR0(x)      ((u32)(((x >> 8) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP3_SADDR         ((u32)0x000000FF << 0)          /*!<R/W 0  Start address of the first instruction of LSYSON PMC patch group3 */
#define PMC_PATCH_GRP3_SADDR(x)           ((u32)(((x) & 0x000000FF) << 0))
#define PMC_GET_PATCH_GRP3_SADDR(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PMC_PATCH_GRP3_1
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP3_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group3 */
#define PMC_PATCH_GRP3_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP3_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP3_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group3,bit[17:6] */
#define PMC_PATCH_GRP3_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP3_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup PMC_PATCH_GRP4_0
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP4_INSTR1_5_0    ((u32)0x0000003F << 26)          /*!<R/W 0  instruction1 of LSYSON PMC patch group4,bit[5:0] */
#define PMC_PATCH_GRP4_INSTR1_5_0(x)      ((u32)(((x) & 0x0000003F) << 26))
#define PMC_GET_PATCH_GRP4_INSTR1_5_0(x)  ((u32)(((x >> 26) & 0x0000003F)))
#define PMC_MASK_PATCH_GRP4_INSTR0        ((u32)0x0003FFFF << 8)          /*!<R/W 0  instruction0 of LSYSON PMC patch group4 */
#define PMC_PATCH_GRP4_INSTR0(x)          ((u32)(((x) & 0x0003FFFF) << 8))
#define PMC_GET_PATCH_GRP4_INSTR0(x)      ((u32)(((x >> 8) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP4_SADDR         ((u32)0x000000FF << 0)          /*!<R/W 0  Start address of the first instruction of LSYSON PMC patch group4 */
#define PMC_PATCH_GRP4_SADDR(x)           ((u32)(((x) & 0x000000FF) << 0))
#define PMC_GET_PATCH_GRP4_SADDR(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PMC_PATCH_GRP4_1
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP4_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group4 */
#define PMC_PATCH_GRP4_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP4_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP4_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group4,bit[17:6] */
#define PMC_PATCH_GRP4_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP4_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup PMC_PATCH_GRP5_0
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP5_INSTR1_5_0    ((u32)0x0000003F << 26)          /*!<R/W 0  instruction1 of LSYSON PMC patch group5,bit[5:0] */
#define PMC_PATCH_GRP5_INSTR1_5_0(x)      ((u32)(((x) & 0x0000003F) << 26))
#define PMC_GET_PATCH_GRP5_INSTR1_5_0(x)  ((u32)(((x >> 26) & 0x0000003F)))
#define PMC_MASK_PATCH_GRP5_INSTR0        ((u32)0x0003FFFF << 8)          /*!<R/W 0  instruction0 of LSYSON PMC patch group5 */
#define PMC_PATCH_GRP5_INSTR0(x)          ((u32)(((x) & 0x0003FFFF) << 8))
#define PMC_GET_PATCH_GRP5_INSTR0(x)      ((u32)(((x >> 8) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP5_SADDR         ((u32)0x000000FF << 0)          /*!<R/W 0  Start address of the first instruction of LSYSON PMC patch group5 */
#define PMC_PATCH_GRP5_SADDR(x)           ((u32)(((x) & 0x000000FF) << 0))
#define PMC_GET_PATCH_GRP5_SADDR(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup PMC_PATCH_GRP5_1
 * @brief
 * @{
 **/
#define PMC_MASK_PATCH_GRP5_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group5 */
#define PMC_PATCH_GRP5_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP5_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP5_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group5,bit[17:6] */
#define PMC_PATCH_GRP5_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP5_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */


/*==========PMC Register Address Definition==========*/
#define WAK_STATUS0                                  0x0000
#define WAK_STATUS1                                  0x0004
#define WAK_MASK0_NP                                 0x0008
#define WAK_MASK1_NP                                 0x000C
#define WAK_MASK0_AP                                 0x0010
#define WAK_MASK1_AP                                 0x0014
#define WAK_MASK0_DSP                                0x0018
#define WAK_MASK1_DSP                                0x001C
#define WLPMC_ISR                                    0x0020
#define WLAN_PWRCTL                                  0x0024
#define SYSPMC_CTRL                                  0x0028
#define SYSPMC_OPT                                   0x002C
#define PMC_PATCH_GRP0_0                             0x0034
#define PMC_PATCH_GRP0_1                             0x0038
#define PMC_PATCH_GRP1_0                             0x003C
#define PMC_PATCH_GRP1_1                             0x0040
#define PMC_PATCH_GRP2_0                             0x0044
#define PMC_PATCH_GRP2_1                             0x0048
#define PMC_PATCH_GRP3_0                             0x004C
#define PMC_PATCH_GRP3_1                             0x0050
#define PMC_PATCH_GRP4_0                             0x0054
#define PMC_PATCH_GRP4_1                             0x0058
#define PMC_PATCH_GRP5_0                             0x005C
#define PMC_PATCH_GRP5_1                             0x0060

/** @} */

/* AUTO_GEN_END */

/* AUTO_GEN_WAKE_SRC_START */

/**************************************************************************//**
* @defgroup WAK_STATUS0/WAK_MASK0_NP/WAK_MASK0_AP/WAK_MASK0_DSP
* @brief
* @{
*****************************************************************************/
#define WAKE_SRC_WIFI_FISR_FESR                      ((u32)0x00000001 << 0)
#define WAKE_SRC_WIFI_FTSR_MAILBOX                   ((u32)0x00000001 << 1)
#define WAKE_SRC_AP_WAKE                             ((u32)0x00000001 << 2)
#define WAKE_SRC_DSP_WAKE                            ((u32)0x00000001 << 3)
#define WAKE_SRC_BT_WAKE_HOST                        ((u32)0x00000001 << 4)
#define WAKE_SRC_IPC_KR4                             ((u32)0x00000001 << 5)
#define WAKE_SRC_IPC_KM4                             ((u32)0x00000001 << 6)
#define WAKE_SRC_IPC_DSP                             ((u32)0x00000001 << 7)
#define WAKE_SRC_WDG0                                ((u32)0x00000001 << 8)
#define WAKE_SRC_Timer0                              ((u32)0x00000001 << 9)
#define WAKE_SRC_Timer1                              ((u32)0x00000001 << 10)
#define WAKE_SRC_Timer2                              ((u32)0x00000001 << 11)
#define WAKE_SRC_Timer3                              ((u32)0x00000001 << 12)
#define WAKE_SRC_Timer4                              ((u32)0x00000001 << 13)
#define WAKE_SRC_Timer5                              ((u32)0x00000001 << 14)
#define WAKE_SRC_Timer6                              ((u32)0x00000001 << 15)
#define WAKE_SRC_Timer7                              ((u32)0x00000001 << 16)
#define WAKE_SRC_UART0                               ((u32)0x00000001 << 17)
#define WAKE_SRC_UART1                               ((u32)0x00000001 << 18)
#define WAKE_SRC_UART2                               ((u32)0x00000001 << 19)
#define WAKE_SRC_UART3                               ((u32)0x00000001 << 20)
#define WAKE_SRC_UART_LOG                            ((u32)0x00000001 << 21)
#define WAKE_SRC_GPIOA                               ((u32)0x00000001 << 22)
#define WAKE_SRC_GPIOB                               ((u32)0x00000001 << 23)
#define WAKE_SRC_CTOUCH                              ((u32)0x00000001 << 24)
#define WAKE_SRC_SPI0                                ((u32)0x00000001 << 25)
#define WAKE_SRC_SPI1                                ((u32)0x00000001 << 26)
#define WAKE_SRC_AON_RTC                             ((u32)0x00000001 << 27)
#define WAKE_SRC_ADC                                 ((u32)0x00000001 << 28)
#define WAKE_SRC_ADC_COMP                            ((u32)0x00000001 << 29)
#define WAKE_SRC_BOR                                 ((u32)0x00000001 << 30)
#define WAKE_SRC_PWR_DOWN                            ((u32)0x00000001 << 31)
/** @} */

/**************************************************************************//**
* @defgroup WAK_STATUS1/WAK_MASK1_NP/WAK_MASK1_AP/WAK_MASK1_DSP
* @brief
* @{
*****************************************************************************/
#define WAKE_SRC_AON_TIM                             ((u32)0x00000001 << 0)
#define WAKE_SRC_AON_WAKEPIN                         ((u32)0x00000001 << 1)
#define WAKE_SRC_VAD                                 ((u32)0x00000001 << 2)

/** @} */

/* AUTO_GEN_WAKE_SRC_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
