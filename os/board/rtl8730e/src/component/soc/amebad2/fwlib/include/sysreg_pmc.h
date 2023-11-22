#ifndef _AMEBAD2_PMC_H_
#define _AMEBAD2_PMC_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup WAK_STATUS0
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_STA_31_0             ((u32)0xFFFFFFFF << 0)          /*!<R 0  wak evt status. It can't be cleare here, clear at IP side . Bit0: WIFI_FISR_FESR_IRQ Bit1: WIFI_FTSR_MAILBOX_IRQ Bit2: AON Timer Bit3: NP_WAKE_IRQ Bit4: AP_WAKE_IRQ Bit5: WDG0_IRQ Bit6: Timer0 Bit7: Timer1 Bit8: Timer2 Bit9: Timer3 Bit10: Timer4 Bit11: Timer5 Bit12: Timer6 Bit13: Timer7 Bit14: UART_LOG Bit15: GPIOA Bit16: GPIOB Bit17: GPIOC Bit18: RTC Bit19: CTOUCH Bit20: ADC Bit21: ADC_COMP Bit22: BOR Bit23: PWR_DOWN Bit24: VAD Bit25: IPC_NP Bit26: IPC_AP Bit27: USB_OTG Bit28: SPI0 Bit29: SPI1 Bit30: UART0 Bit31: UART1 */
#define PMC_WAK_STA_31_0(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_STA_31_0(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WAK_STATUS1
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_STA_63_32            ((u32)0xFFFFFFFF << 0)          /*!<R 0  wak evt status. It can't be cleare here, clear at IP side . Bit0: UART2 Bit1: WDG1 Bit2: WDG2 Bit3: WDG3 Bit4: WDG4 Bit5: AON_GPIO_WAKE Bit6: BT_WAKE_HOST Bit7: nFIQOUT0_OR_nIRQOUT0 Bit8: nFIQOUT1_OR_nIRQOUT1 Bit9~31: Reserved */
#define PMC_WAK_STA_63_32(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_STA_63_32(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WAK_MASK0_LP
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_LP_IMR_31_0          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake KM0, this used to wak pmc */
#define PMC_WAK_LP_IMR_31_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_LP_IMR_31_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WAK_MASK1_LP
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_LP_IMR_63_32         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake KM0 */
#define PMC_WAK_LP_IMR_63_32(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_LP_IMR_63_32(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WAK_MASK0_NP
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_NP_IMR_31_0          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake NP */
#define PMC_WAK_NP_IMR_31_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_NP_IMR_31_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WAK_MASK1_NP
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_NP_IMR_63_32         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake NP */
#define PMC_WAK_NP_IMR_63_32(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_NP_IMR_63_32(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WAK_MASK0_AP
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_AP_IMR_31_0          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake AP */
#define PMC_WAK_AP_IMR_31_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_AP_IMR_31_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WAK_MASK1_AP
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_WAK_AP_IMR_63_32         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake AP */
#define PMC_WAK_AP_IMR_63_32(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_AP_IMR_63_32(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup WLPMC_ISR
 * @brief
 * @{
 *****************************************************************************/
#define PMC_BIT_WL_OFF_IMR                ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define PMC_BIT_WL_ON_IMR                 ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define PMC_BIT_WL_OFF_ISR                ((u32)0x00000001 << 1)          /*!<RW1CB 0   */
#define PMC_BIT_WL_ON_ISR                 ((u32)0x00000001 << 0)          /*!<RW1CB 0   */
/** @} */

/**************************************************************************//**
 * @defgroup WLAN_PWRCTL
 * @brief
 * @{
 *****************************************************************************/
#define PMC_BIT_SW_CLR_RDY2ACT            ((u32)0x00000001 << 17)          /*!<R/WA0 0  System infrom WLAN ready to active */
#define PMC_BIT_SW_SET_RDY2ACT            ((u32)0x00000001 << 16)          /*!<R/WA0 0   */
#define PMC_BIT_WL_RDY2ACT                ((u32)0x00000001 << 9)          /*!<R 0  System infrom WLAN ready to active */
#define PMC_BIT_WL_REQ2ACT                ((u32)0x00000001 << 8)          /*!<R 0   */
#define PMC_BIT_WL_HW_RESP_FEN            ((u32)0x00000001 << 0)          /*!<R/W 1  1: enable HW auto response */
/** @} */

/**************************************************************************//**
 * @defgroup SYSPMC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_STL_PDPBG                ((u32)0x00000003 << 28)          /*!<R/W 0x1  timer wait for PLL bandgag power stable . Spec is 160us 00:8us * 2^4 01:8us * 2^5 10:8us * 2^6 11:8us * 2^7 */
#define PMC_STL_PDPBG(x)                  ((u32)(((x) & 0x00000003) << 28))
#define PMC_GET_STL_PDPBG(x)              ((u32)(((x >> 28) & 0x00000003)))
#define PMC_MASK_STL_PDSOC                ((u32)0x00000003 << 24)          /*!<R/W 0x3  timer wait for core power power-cut power stable . 00:8us * 2^5 01:8us * 2^6 10:8us * 2^7 11:8us * 2^8 */
#define PMC_STL_PDSOC(x)                  ((u32)(((x) & 0x00000003) << 24))
#define PMC_GET_STL_PDSOC(x)              ((u32)(((x >> 24) & 0x00000003)))
#define PMC_MASK_TUTIME                   ((u32)0x0000003F << 16)          /*!<R/W 0x1F  PMC timeunit setting, in cycles of osc4M e.g, default 0x1F, timeunit = 8us */
#define PMC_TUTIME(x)                     ((u32)(((x) & 0x0000003F) << 16))
#define PMC_GET_TUTIME(x)                 ((u32)(((x >> 16) & 0x0000003F)))
#define PMC_BIT_PST_TMODE_MSEL            ((u32)0x00000001 << 5)          /*!<R/W 0  Set ls plfm msel option . 0: when test mode , msel will set to 0 for SIC 1: when test mode , pmc will not set msel , remain as km0 master. */
#define PMC_BIT_PAD_PST_MODE              ((u32)0x00000001 << 4)          /*!<R/W/ES 0  Pad pu/pd param selection 1: HW select pu/pd slp param for PAD 0: HW select pu/pd active param for PAD HW will auto-set this bit when enter or exit DSLP , SW can set for debug also . */
#define PMC_BIT_POF_WAIT_SLEP             ((u32)0x00000001 << 3)          /*!<R/W 1  1.when SW set pmen_slep or pmen_dslp , hw pmc will wait km0 enter sleep mode , then begin to pof seq 0.Not wait km0 sleep */
#define PMC_BIT_LP_IRQ_MASK               ((u32)0x00000001 << 2)          /*!<R/W 0  1.mask all LP interrupt 0:release all LP interrupt */
#define PMC_BIT_PMEN_SLEP                 ((u32)0x00000001 << 1)          /*!<R/W 0  1: Request PMC to enter sleep state, auto clear to 0 after PMC finishes */
#define PMC_BIT_PMEN_DSLP                 ((u32)0x00000001 << 0)          /*!<R/W 0  1: Request PMC to enter deep-sleep state, auto clear to 0 after PMC finishes */
/** @} */

/**************************************************************************//**
 * @defgroup SYSPMC_OPT
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_PST_NORM_FBUS            ((u32)0x00000003 << 22)          /*!<R/W 0x3  Normal setting for ls bus frequency selection 2'b00/01: ANA4M 2'b10: XTAL 2'b11: XTAL/2 */
#define PMC_PST_NORM_FBUS(x)              ((u32)(((x) & 0x00000003) << 22))
#define PMC_GET_PST_NORM_FBUS(x)          ((u32)(((x >> 22) & 0x00000003)))
#define PMC_MASK_PST_NORM_XMD             ((u32)0x00000003 << 20)          /*!<R/W 0x2  Normal setting for XTAL mode selection 00: XTAL will be off when PMC enter into sleep mode 01: XTAL will be in LPS when PMC enter into sleep mode 10/11: XTAL will be in Normal when PMC enter into sleep mode */
#define PMC_PST_NORM_XMD(x)               ((u32)(((x) & 0x00000003) << 20))
#define PMC_GET_PST_NORM_XMD(x)           ((u32)(((x >> 20) & 0x00000003)))
#define PMC_MASK_PST_NORM_VOLT            ((u32)0x00000003 << 18)          /*!<R/W 0x1  Normal setting for SWR volt selection 00: SWR switch to 0.8v 01: SWR switch to 0.9v 10/11: SWR switch to 1.0v */
#define PMC_PST_NORM_VOLT(x)              ((u32)(((x) & 0x00000003) << 18))
#define PMC_GET_PST_NORM_VOLT(x)          ((u32)(((x >> 18) & 0x00000003)))
#define PMC_BIT_PST_NORM_EPWM             ((u32)0x00000001 << 16)          /*!<R/W 0x1  Normal setting for SWR mode selection 1: SWR switch to PWM mode when nxt PMC enter into sleep mode 0: SWR switch to PFM when PMC enter into sleep mode */
#define PMC_MASK_PST_SLEP_FBUS            ((u32)0x00000003 << 14)          /*!<R/W 0x0  Sleep setting for ls bus frequency selection 2'b00/01: ANA4M 2'b10: XTAL 2'b11: XTAL/2 */
#define PMC_PST_SLEP_FBUS(x)              ((u32)(((x) & 0x00000003) << 14))
#define PMC_GET_PST_SLEP_FBUS(x)          ((u32)(((x >> 14) & 0x00000003)))
#define PMC_MASK_PST_SLEP_XMD             ((u32)0x00000003 << 12)          /*!<R/W 0x0  Sleep setting for XTAL mode selection 00: XTAL will be off when PMC enter into sleep mode 01: XTAL will be in LPS when PMC enter into sleep mode 10/11: XTAL will be in Normal when PMC enter into sleep mode */
#define PMC_PST_SLEP_XMD(x)               ((u32)(((x) & 0x00000003) << 12))
#define PMC_GET_PST_SLEP_XMD(x)           ((u32)(((x >> 12) & 0x00000003)))
#define PMC_MASK_PST_SLEP_VOLT            ((u32)0x00000003 << 10)          /*!<R/W 0x0  Sleep setting for SWR volt selection 00: SWR switch to 0.8v 01: SWR switch to 0.9v 10/11: SWR switch to 1.0v */
#define PMC_PST_SLEP_VOLT(x)              ((u32)(((x) & 0x00000003) << 10))
#define PMC_GET_PST_SLEP_VOLT(x)          ((u32)(((x >> 10) & 0x00000003)))
#define PMC_BIT_PST_SLEP_EPWM             ((u32)0x00000001 << 8)          /*!<R/W 0  Sleep setting for SWR mode selection 1: SWR keeps at PWM mode when PMC enter into sleep mode 0: SWR switch to PFM when PMC enter into sleep mode */
#define PMC_BIT_PST_SLEP_ERCK             ((u32)0x00000001 << 4)          /*!<R/W 0  1: Enable osc4m platform clock when PMC enter into sleep mode 0: Gate osc4m platform clock when PMC enter into sleep mode */
#define PMC_BIT_PST_SLEP_EMPM             ((u32)0x00000001 << 3)          /*!<R/W 0  1: Enable LS system mem still in active when pmc enter sleep 0: LS system mem will in DSLP when pmc enter sleep */
#define PMC_BIT_PST_SLEP_EBUS             ((u32)0x00000001 << 2)          /*!<R/W 0  1: Enable platform clock when PMC entro into sleep mode 0: Gate platform clock when PMC entro into sleep mode */
#define PMC_BIT_PST_SLEP_ESOC             ((u32)0x00000001 << 0)          /*!<R/W 0  1: power enable SOC platform when PMC enter into sleep mode (clock-gating) 0: power off SoC domain when PMC enter into sleep mode (power-gating) */
/** @} */

/**************************************************************************//**
 * @defgroup MEM_PWR_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PMC_BIT_SLEP_RET_RAM_LS           ((u32)0x00000001 << 22)          /*!<R/W 0  1: Enable Retention RAM light-sleep when KM0 in sleep state 0: Disable */
#define PMC_BIT_SLEP_RET_RAM_DS           ((u32)0x00000001 << 21)          /*!<R/W 0  1: Enable Retention RAM deep-sleep when KM0 in sleep state 0: Disable */
#define PMC_BIT_SLEP_RET_RAM_SD           ((u32)0x00000001 << 20)          /*!<R/W 0  1: Shut-down Retention RAM when KM0 in sleep state 0: Power on Retention RAM when KM0 in sleep state */
#define PMC_BIT_SLEP_IDCACHE_LS           ((u32)0x00000001 << 19)          /*!<R/W 0  1: Enable ICACHE/DCACHE light-sleep when KM0 in sleep state 0: Disable */
#define PMC_BIT_SLEP_K0_RAM_LS            ((u32)0x00000001 << 18)          /*!<R/W 0  1: Enable CPU RAM light-sleep when KM0 in sleep state 0: Disable */
#define PMC_BIT_SLEP_K0_RAM_DS            ((u32)0x00000001 << 17)          /*!<R/W 0  1: Enable CPU RAM deep-sleep when KM0 in sleep state 0: Disable */
#define PMC_BIT_SLEP_K0_RAM_SD            ((u32)0x00000001 << 16)          /*!<R/W 0  1: Shut-down CPU RAM when KM0 in sleep state 0: Power on CPU RAM when KM0 in sleep state */
#define PMC_BIT_NORM_RET_RAM_LS           ((u32)0x00000001 << 6)          /*!<R/W 0  1: Enable Retention RAM light-sleep when KM0 in active state 0: Disable */
#define PMC_BIT_NORM_RET_RAM_DS           ((u32)0x00000001 << 5)          /*!<R/W 0  1: Enable Retention RAM deep-sleep when KM0 in active state 0: Disable */
#define PMC_BIT_NORM_RET_RAM_SD           ((u32)0x00000001 << 4)          /*!<R/W 0  1: Shut-down Retention RAM when KM0 in active state 0: Power on Retention RAM when KM0 in active state */
#define PMC_BIT_NORM_IDCACHE_LS           ((u32)0x00000001 << 3)          /*!<R/W 0  1: Enable ICACHE/DCACHE light-sleep when KM0 in active state 0: Disable */
#define PMC_BIT_NORM_K0_RAM_LS            ((u32)0x00000001 << 2)          /*!<R/W 0  1: Enable CPU RAM light-sleep when KM0 in active state 0: Disable */
#define PMC_BIT_NORM_K0_RAM_DS            ((u32)0x00000001 << 1)          /*!<R/W 0  1: Enable CPU RAM deep-sleep when KM0 in active state 0: Disable */
#define PMC_BIT_NORM_K0_RAM_SD            ((u32)0x00000001 << 0)          /*!<R/W 0  ECO_137 : KM0 itcm rom SLP */
/** @} */

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP0_0
 * @brief
 * @{
 *****************************************************************************/
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

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP0_1
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_PATCH_GRP0_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group0 */
#define PMC_PATCH_GRP0_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP0_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP0_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group0,bit[17:6] */
#define PMC_PATCH_GRP0_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP0_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP1_0
 * @brief
 * @{
 *****************************************************************************/
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

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP1_1
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_PATCH_GRP1_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group1 */
#define PMC_PATCH_GRP1_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP1_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP1_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group1,bit[17:6] */
#define PMC_PATCH_GRP1_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP1_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP2_0
 * @brief
 * @{
 *****************************************************************************/
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

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP2_1
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_PATCH_GRP2_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group2 */
#define PMC_PATCH_GRP2_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP2_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP2_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group2,bit[17:6] */
#define PMC_PATCH_GRP2_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP2_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP3_0
 * @brief
 * @{
 *****************************************************************************/
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

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP3_1
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_PATCH_GRP3_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group3 */
#define PMC_PATCH_GRP3_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP3_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP3_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group3,bit[17:6] */
#define PMC_PATCH_GRP3_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP3_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP4_0
 * @brief
 * @{
 *****************************************************************************/
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

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP4_1
 * @brief
 * @{
 *****************************************************************************/
#define PMC_MASK_PATCH_GRP4_INSTR2        ((u32)0x0003FFFF << 12)          /*!<R/W 0  instruction2 of LSYSON PMC patch group4 */
#define PMC_PATCH_GRP4_INSTR2(x)          ((u32)(((x) & 0x0003FFFF) << 12))
#define PMC_GET_PATCH_GRP4_INSTR2(x)      ((u32)(((x >> 12) & 0x0003FFFF)))
#define PMC_MASK_PATCH_GRP4_INSTR1_17_6   ((u32)0x00000FFF << 0)          /*!<R/W 0  instruction1 of LSYSON PMC patch group4,bit[17:6] */
#define PMC_PATCH_GRP4_INSTR1_17_6(x)     ((u32)(((x) & 0x00000FFF) << 0))
#define PMC_GET_PATCH_GRP4_INSTR1_17_6(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP5_0
 * @brief
 * @{
 *****************************************************************************/
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

/**************************************************************************//**
 * @defgroup PMC_PATCH_GRP5_1
 * @brief
 * @{
 *****************************************************************************/
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
#define WAK_MASK0_LP                                 0x0008
#define WAK_MASK1_LP                                 0x000C
#define WAK_MASK0_NP                                 0x0010
#define WAK_MASK1_NP                                 0x0014
#define WAK_MASK0_AP                                 0x0018
#define WAK_MASK1_AP                                 0x001C
#define WLPMC_ISR                                    0x0020
#define WLAN_PWRCTL                                  0x0024
#define SYSPMC_CTRL                                  0x0028
#define SYSPMC_OPT                                   0x002C
#define MEM_PWR_CTRL                                 0x0030
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

/* AUTO_GEN_END */

/* AUTO_GEN_WAKE_SRC_START */

/**************************************************************************//**
* @defgroup WAK_STATUS0/WAK_MASK0_LP/WAK_MASK0_NP/WAK_MASK0_AP
* @brief
* @{
*****************************************************************************/
#define WAKE_SRC_WIFI_FISR_FESR                      ((u32)0x00000001 << 0)
#define WAKE_SRC_WIFI_FTSR_MAILBOX                   ((u32)0x00000001 << 1)
#define WAKE_SRC_AON_TIM                             ((u32)0x00000001 << 2)
#define WAKE_SRC_NP_WAKE                             ((u32)0x00000001 << 3)
#define WAKE_SRC_AP_WAKE                             ((u32)0x00000001 << 4)
#define WAKE_SRC_WDG0                                ((u32)0x00000001 << 5)
#define WAKE_SRC_Timer0                              ((u32)0x00000001 << 6)
#define WAKE_SRC_Timer1                              ((u32)0x00000001 << 7)
#define WAKE_SRC_Timer2                              ((u32)0x00000001 << 8)
#define WAKE_SRC_Timer3                              ((u32)0x00000001 << 9)
#define WAKE_SRC_Timer4                              ((u32)0x00000001 << 10)
#define WAKE_SRC_Timer5                              ((u32)0x00000001 << 11)
#define WAKE_SRC_Timer6                              ((u32)0x00000001 << 12)
#define WAKE_SRC_Timer7                              ((u32)0x00000001 << 13)
#define WAKE_SRC_UART_LOG                            ((u32)0x00000001 << 14)
#define WAKE_SRC_GPIOA                               ((u32)0x00000001 << 15)
#define WAKE_SRC_GPIOB                               ((u32)0x00000001 << 16)
#define WAKE_SRC_GPIOC                               ((u32)0x00000001 << 17)
#define WAKE_SRC_RTC                                 ((u32)0x00000001 << 18)
#define WAKE_SRC_CTOUCH                              ((u32)0x00000001 << 19)
#define WAKE_SRC_ADC                                 ((u32)0x00000001 << 20)
#define WAKE_SRC_ADC_COMP                            ((u32)0x00000001 << 21)
#define WAKE_SRC_BOR                                 ((u32)0x00000001 << 22)
#define WAKE_SRC_PWR_DOWN                            ((u32)0x00000001 << 23)
#define WAKE_SRC_VADBT_OR_VADPC                      ((u32)0x00000001 << 24)
#define WAKE_SRC_IPC_NP                              ((u32)0x00000001 << 25)
#define WAKE_SRC_IPC_AP                              ((u32)0x00000001 << 26)
#define WAKE_SRC_USB_OTG                             ((u32)0x00000001 << 27)
#define WAKE_SRC_SPI0                                ((u32)0x00000001 << 28)
#define WAKE_SRC_SPI1                                ((u32)0x00000001 << 29)
#define WAKE_SRC_UART0                               ((u32)0x00000001 << 30)
#define WAKE_SRC_UART1                               ((u32)0x00000001 << 31)
/** @} */

/**************************************************************************//**
* @defgroup WAK_STATUS1/WAK_MASK1_LP/WAK_MASK1_NP/WAK_MASK1_AP
* @brief
* @{
*****************************************************************************/
#define WAKE_SRC_UART2                               ((u32)(3 << 30) | (0x00000001 << 0))
#define WAKE_SRC_I2C1                                ((u32)(3 << 30) | (0x00000001 << 10))
#define WAKE_SRC_I2C2                                ((u32)(3 << 30) | (0x00000001 << 11))
#define WAKE_SRC_WDG1                                ((u32)(3 << 30) | (0x00000001 << 1))
#define WAKE_SRC_WDG2                                ((u32)(3 << 30) | (0x00000001 << 2))
#define WAKE_SRC_WDG3                                ((u32)(3 << 30) | (0x00000001 << 3))
#define WAKE_SRC_WDG4                                ((u32)(3 << 30) | (0x00000001 << 4))
#define WAKE_SRC_AON_WAKEPIN                         ((u32)(3 << 30) | (0x00000001 << 5))
#define WAKE_SRC_BT_WAKE_HOST                        ((u32)(3 << 30) | (0x00000001 << 6))
#define WAKE_SRC_nFIQOUT0_OR_nIRQOUT0                ((u32)(3 << 30) | (0x00000001 << 7))
#define WAKE_SRC_nFIQOUT1_OR_nIRQOUT1                ((u32)(3 << 30) | (0x00000001 << 8))
/** @} */

/* AUTO_GEN_WAKE_SRC_END */

/* MANUAL_GEN_START */

//Please add your defination here
#define WAKE_MASK1_CHECK	0xC0000000

/** @defgroup PowerCut_Stable_Time_define
  * @{
  */
enum PowerCut_Stable_Time {
	PC_Stable_256 = 0x00,
	PC_Stable_512 = 0x01,
	PC_Stable_1024 = 0x02,
	PC_Stable_2048 = 0x03,
};

/* MANUAL_GEN_END */

#endif
