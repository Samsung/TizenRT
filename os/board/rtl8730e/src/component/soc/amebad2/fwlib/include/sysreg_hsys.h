#ifndef _AMEBAD2_HSYS_H_
#define _AMEBAD2_HSYS_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_HSYS_HP_PWC
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_PSW_HP_AP_CORE_2ND_OUT  ((u32)0x00000003 << 30)          /*!<R 0  AP macro core power domain 2nd switch state, active 1 [0]: core0 [1]: core1 */
#define HSYS_PSW_HP_AP_CORE_2ND_OUT(x)    ((u32)(((x) & 0x00000003) << 30))
#define HSYS_GET_PSW_HP_AP_CORE_2ND_OUT(x) ((u32)(((x >> 30) & 0x00000003)))
#define HSYS_MASK_PSW_HP_AP_CORE_OUT      ((u32)0x00000003 << 28)          /*!<R 0  AP macro core power domain switch state,active 1 [0]: core0 [1]: core1 */
#define HSYS_PSW_HP_AP_CORE_OUT(x)        ((u32)(((x) & 0x00000003) << 28))
#define HSYS_GET_PSW_HP_AP_CORE_OUT(x)    ((u32)(((x >> 28) & 0x00000003)))
#define HSYS_BIT_PSW_HP_AP_SOC_OUT        ((u32)0x00000001 << 27)          /*!<R 0  SOC AP top power domain switch state,active 1 */
#define HSYS_BIT_PSW_HP_AP_TOP_2ND_OUT    ((u32)0x00000001 << 26)          /*!<R 0  AP macro top power domain 2nd switch state */
#define HSYS_BIT_PSW_HP_AP_TOP_OUT        ((u32)0x00000001 << 25)          /*!<R 0  AP macro top power domain switch state,active 1 */
#define HSYS_BIT_PSW_HP_AP_L2_OUT         ((u32)0x00000001 << 24)          /*!<R 0  AP macro L2 power domain switch state,active 1 */
#define HSYS_MASK_PSW_HP_AP_CORE_2ND      ((u32)0x00000003 << 6)          /*!<R/W 0  AP macro core power domain 2nd switch enable,active 1 [0]: core0 [1]: core1 */
#define HSYS_PSW_HP_AP_CORE_2ND(x)        ((u32)(((x) & 0x00000003) << 6))
#define HSYS_GET_PSW_HP_AP_CORE_2ND(x)    ((u32)(((x >> 6) & 0x00000003)))
#define HSYS_MASK_PSW_HP_AP_CORE          ((u32)0x00000003 << 4)          /*!<R/W 0  AP macro core power domain switch enable,active 1 [0]: core0 [1]: core1 */
#define HSYS_PSW_HP_AP_CORE(x)            ((u32)(((x) & 0x00000003) << 4))
#define HSYS_GET_PSW_HP_AP_CORE(x)        ((u32)(((x >> 4) & 0x00000003)))
#define HSYS_BIT_PSW_HP_AP_TOP_2ND        ((u32)0x00000001 << 2)          /*!<R/W 0  AP macro top power domain 2nd switch enable,active 1 */
#define HSYS_BIT_PSW_HP_AP_TOP            ((u32)0x00000001 << 1)          /*!<R/W 0  AP macro top power domain switch enable,active 1 */
#define HSYS_BIT_PSW_HP_AP_L2             ((u32)0x00000001 << 0)          /*!<R/W 0  AP macro L2 power domain switch enable,active 1 */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_HP_ISO
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_ISO_HP_AP_CORE          ((u32)0x00000003 << 4)          /*!<R/W 1  AP macro core power domain isolatation, 1: isolation [0]: core0 [1]: core1 */
#define HSYS_ISO_HP_AP_CORE(x)            ((u32)(((x) & 0x00000003) << 4))
#define HSYS_GET_ISO_HP_AP_CORE(x)        ((u32)(((x >> 4) & 0x00000003)))
#define HSYS_BIT_ISO_HP_AP_TOP            ((u32)0x00000001 << 1)          /*!<R/W 1  AP macro top power domain isolatation, 1: isolation */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_HP_FEN
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_FEN_AP                   ((u32)0x00000001 << 0)          /*!<R/W 0  CA7 function enable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_HP_CKE
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_CKE_AP                   ((u32)0x00000001 << 0)          /*!<R/W 0  CA7 clock enable . */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_HP_CKSL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_CKSL_AP                 ((u32)0x00000003 << 2)          /*!<R/W 0  CA7 clock selection 00: ap pll out (1.2G default) 10: ap pll div2 out (1.2G/2) 01/11: np pll out (600M) */
#define HSYS_CKSL_AP(x)                   ((u32)(((x) & 0x00000003) << 2))
#define HSYS_GET_CKSL_AP(x)               ((u32)(((x >> 2) & 0x00000003)))
#define HSYS_MASK_CKD_AP                  ((u32)0x00000003 << 0)          /*!<R/WPD 0  CA7 clock divider , this is after clk select by cksl_ap 00: div1 01: div2 10: div3 11: div4 */
#define HSYS_CKD_AP(x)                    ((u32)(((x) & 0x00000003) << 0))
#define HSYS_GET_CKD_AP(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_HPLAT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_PSRAM_SPIC_BIG_ENDIAN    ((u32)0x00000001 << 28)          /*!<R/W 0  1: big endian 0: little endian */
#define HSYS_BIT_SPI1_MST                 ((u32)0x00000001 << 26)          /*!<R/W 0h  1: SPI1 used as master 0: SPI1 used as slaver */
#define HSYS_BIT_SPI0_MST                 ((u32)0x00000001 << 25)          /*!<R/W 0h  1: SPI0 used as master 0: SPI0 used as slaver */
#define HSYS_BIT_EXT_MEM_IS_DDR           ((u32)0x00000001 << 24)          /*!<R/W 0  system DDR PSRAM select 1: using DDR 0: using PSRAM */
#define HSYS_BIT_DDRC_SPDUPSIM            ((u32)0x00000001 << 23)          /*!<R/W 0  ddrc simulatin speedup enable */
#define HSYS_MASK_GDMA0_CCM               ((u32)0x00000003 << 21)          /*!<R/W 0h  GDMA0 clock control mode */
#define HSYS_GDMA0_CCM(x)                 ((u32)(((x) & 0x00000003) << 21))
#define HSYS_GET_GDMA0_CCM(x)             ((u32)(((x >> 21) & 0x00000003)))
#define HSYS_BIT_PSRAM_SPDUPSIM           ((u32)0x00000001 << 20)          /*!<R/W 0  psramc simulatin speedup enable */
#define HSYS_BIT_PLFM_AUTO_ICG_EN         ((u32)0x00000001 << 19)          /*!<R/W 0  1: Enable HS platform auto clock-gating for power saving 0: Disable */
#define HSYS_BIT_SHARE_BT_MEM             ((u32)0x00000001 << 18)          /*!<R/W 0  1: Enable KM4 to share BT memory */
#define HSYS_BIT_SHARE_WL_MEM             ((u32)0x00000001 << 17)          /*!<R/W 0  1: Enable KM4 to share WL memory */
#define HSYS_BIT_KM4_RETENTION_MODE       ((u32)0x00000001 << 16)          /*!<R/W 0  1: Enable, when KM4 reset, cache will not be cleared 0: Disable, when KM4 reset, cache will be cleared by HW */
#define HSYS_BIT_HW_PDN_AUD               ((u32)0x00000001 << 0)          /*!<R/W 1  After enable audio swr , then this bit set to 1 . Before shutdown audio swr , this bit must set to 0 . Default 1 for old project . */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_HPLAT_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_INIT_XO_RANGE_EN         ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define HSYS_BIT_SHARE_CACHE_MEM          ((u32)0x00000001 << 12)          /*!<R/W 0  global control for cache shared with dtcm0 0: share disable 1: global share enable , to share cache more flex , it need control icrr and dcrr reg in cpu */
#define HSYS_BIT_DRAM_ADAP_IDLE           ((u32)0x00000001 << 10)          /*!<R 0   */
#define HSYS_BIT_LX2_MST_IDLE             ((u32)0x00000001 << 9)          /*!<R 0  Indicate Lx2 mst is idle */
#define HSYS_BIT_LX1_MST_IDLE             ((u32)0x00000001 << 8)          /*!<R 0  Indicate Lx1 mst is idle */
#define HSYS_BIT_KM4_DBGRESTARTED_M       ((u32)0x00000001 << 3)          /*!<R 0  KM4 debug restart status */
#define HSYS_BIT_KM4_HALTED_M             ((u32)0x00000001 << 2)          /*!<R 0  KM4 halt status */
#define HSYS_BIT_KM4_LOCKUP_M             ((u32)0x00000001 << 1)          /*!<R 0  KM4 lockup status */
#define HSYS_BIT_KM4_SL_SLEEPSYS_R        ((u32)0x00000001 << 0)          /*!<R 0  KM4 sleep status */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_KM4_XO_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_INIT_XO_BASE            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define HSYS_INIT_XO_BASE(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define HSYS_GET_INIT_XO_BASE(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_KM4_XO_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_INIT_XO_TOP             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define HSYS_INIT_XO_TOP(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define HSYS_GET_INIT_XO_TOP(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_SPIC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_FLASH_CAL_EN             ((u32)0x00000001 << 15)          /*!<R/W 0  Flash calibration enable 1: delay flash sout for calibration 0: bypass flash sout to spic */
#define HSYS_BIT_FLASH_DIV_EN             ((u32)0x00000001 << 13)          /*!<R/W 0  1: enable to generate flash clock (no phase shift) divided by 600M pll clock, HW detect this signal's rising edge to start the no phase shift division circuit. */
#define HSYS_BIT_FLASH_PS_DIV_EN          ((u32)0x00000001 << 12)          /*!<R/W 0  1: enable to generate flash clock (with phase shift) divided by 600M pll clock, HW detect this signal's rising edge to start the phase shift clock division circuit. */
#define HSYS_MASK_FLASH_DIV_INT           ((u32)0x0000000F << 8)          /*!<R/W 0  Flash clock division ratio, integrate part 0: not support 1: divided by 2 2: divided by 3 3: divided by 4 .. 7: divided by 8 8: divided by 9 9: divided by 10 Note: SPI clock frequency is this divided clock/2 */
#define HSYS_FLASH_DIV_INT(x)             ((u32)(((x) & 0x0000000F) << 8))
#define HSYS_GET_FLASH_DIV_INT(x)         ((u32)(((x >> 8) & 0x0000000F)))
#define HSYS_MASK_FLASH_PS_PHASE          ((u32)0x0000001F << 2)          /*!<R/W 0  Flash clock phase shift in units of 1/2 np PLL clock cycles. If np PLL is 1GHz, the step is 0.5ns. 0: shift 0 step 1: shift 1 step … 19: shift 19 step. Note, this value cannot be set more than (FLASH_DIV_INT+1) * 2-1, for example, if DIV_INT is 1 , which means spic clock is 1/2 NPPLL , then the most shift step setting is 3, which is 4 step can be used. */
#define HSYS_FLASH_PS_PHASE(x)            ((u32)(((x) & 0x0000001F) << 2))
#define HSYS_GET_FLASH_PS_PHASE(x)        ((u32)(((x >> 2) & 0x0000001F)))
#define HSYS_BIT_FLASH_PS_DIV_RDY         ((u32)0x00000001 << 1)          /*!<R 0  Ready flag of Flash clock with phase shift, Read only (input signal) */
#define HSYS_BIT_FLASH_DIV_RDY            ((u32)0x00000001 << 0)          /*!<R 0  Ready flag of Flash clock, Read only (input signal) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_USB_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_USB2_DIGOTGPADEN         ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define HSYS_BIT_USB_OTGMODE              ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define HSYS_BIT_USB2_DIGPADEN            ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define HSYS_BIT_ISO_USBA_EN              ((u32)0x00000001 << 25)          /*!<R/W 1  1: Enable signal from uSBPHY analog 0: disable */
#define HSYS_BIT_ISO_USBD_EN              ((u32)0x00000001 << 24)          /*!<R/W 1   */
#define HSYS_BIT_USBA_LDO_EN              ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define HSYS_BIT_PDN_UPHY_EN              ((u32)0x00000001 << 20)          /*!<R/W 1  0: enable USBPHY shutdown */
#define HSYS_BIT_PWC_UABG_EN              ((u32)0x00000001 << 19)          /*!<R/W 0  1: enable USBPHY BG power cut */
#define HSYS_BIT_PWC_UAHV_EN              ((u32)0x00000001 << 18)          /*!<R/W 0  1: enable USBPHY HV power cut */
#define HSYS_BIT_PWC_UALV_EN              ((u32)0x00000001 << 17)          /*!<R/W 0  1: enable USBPHY LV power cut */
#define HSYS_BIT_OTG_ANA_EN               ((u32)0x00000001 << 16)          /*!<R/W 0  1: enable usb aphy lv2hv, hv2lv check for otg */
#define HSYS_MASK_USBOTG_CTRL             ((u32)0x0000FFFF << 0)          /*!<R/W 0h   */
#define HSYS_USBOTG_CTRL(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define HSYS_GET_USBOTG_CTRL(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_USB_TEST_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_UPHY_SLB_CMD             ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define HSYS_BIT_UPHY_DBG_CLK             ((u32)0x00000001 << 8)          /*!<R 0   */
#define HSYS_BIT_UPHY_SLB_HS              ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define HSYS_BIT_UPHY_HS_SLB_OK           ((u32)0x00000001 << 6)          /*!<R 0   */
#define HSYS_BIT_UPHY_SLB_HW_PRD          ((u32)0x00000001 << 5)          /*!<R 0   */
#define HSYS_BIT_UPHY_FS_SLB_OK           ((u32)0x00000001 << 4)          /*!<R 0   */
#define HSYS_BIT_UPHY_FORCE_SLB           ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define HSYS_BIT_UPHY_SLB_FAIL            ((u32)0x00000001 << 2)          /*!<R 0   */
#define HSYS_BIT_UPHY_SLB_DONE            ((u32)0x00000001 << 1)          /*!<R 0   */
#define HSYS_BIT_USB_TST_SEL              ((u32)0x00000001 << 0)          /*!<R/W 0h  select the location where the USB test signals are from 0: register 1: PINMUX GPIO */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_SDH_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_SDCD_DBNC_EN             ((u32)0x00000001 << 7)          /*!<R/W 0  1: enable SDH card connect debounce */
#define HSYS_MASK_SDCD_DBNC_CNT           ((u32)0x0000003F << 0)          /*!<R/W 0h  debounce count value, unit is 1ms */
#define HSYS_SDCD_DBNC_CNT(x)             ((u32)(((x) & 0x0000003F) << 0))
#define HSYS_GET_SDCD_DBNC_CNT(x)         ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_DDR_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_DDRP_CLKEN_DCPHY         ((u32)0x00000001 << 1)          /*!<R/W 1h  1: enable all rbus signal 0: disable all rbus clock signal */
#define HSYS_BIT_DDRP_CLKEN_PLLREG        ((u32)0x00000001 << 0)          /*!<R/W 1h  1: enable ddr phy crt register rbus clock 0: disable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_LXBUS_DBG_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_LX1_LOCK_OK              ((u32)0x00000001 << 12)          /*!<R 0  lock lx1 bus lock ok */
#define HSYS_BIT_LX1_LOCK_BUS             ((u32)0x00000001 << 8)          /*!<R/W 0  lock lx1 bus for debug */
#define HSYS_MASK_BUSTIMEOUT              ((u32)0x00000003 << 4)          /*!<R/W 0  lx sys bus arb timeout thresh */
#define HSYS_BUSTIMEOUT(x)                ((u32)(((x) & 0x00000003) << 4))
#define HSYS_GET_BUSTIMEOUT(x)            ((u32)(((x >> 4) & 0x00000003)))
#define HSYS_BIT_ENBUSTIMEOUT             ((u32)0x00000001 << 0)          /*!<R/W 0  lx sys bus arb timeout enable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_LXBUS_DBG0
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_TIMEOUT_ADDR_MST        ((u32)0xFFFFFFFF << 0)          /*!<R 0  lx sys bus addr debug out */
#define HSYS_TIMEOUT_ADDR_MST(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define HSYS_GET_TIMEOUT_ADDR_MST(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_LXBUS_DBG1
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_TIMEOUT_ADDR_SLV        ((u32)0xFFFFFFFF << 0)          /*!<R 0  lx sys bus addr debug out */
#define HSYS_TIMEOUT_ADDR_SLV(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define HSYS_GET_TIMEOUT_ADDR_SLV(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_DUMMY_1E0
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_BIT_CKSL_TIM9                ((u32)0x00000001 << 2)          /*!<R/W 0  select tim9 clock source 0: xtal 40M 1: i2s 45M/2 */
#define HSYS_BIT_PI_PWROFF_EN             ((u32)0x00000001 << 1)          /*!<R/W 0  ECO: enable to shutdown ddrphy PI + dq pads after entering self-refresh state, only for ddr */
#define HSYS_BIT_PWDPAD_DQ_EN             ((u32)0x00000001 << 0)          /*!<R/W 0  ECO: enable to shutdown dq pads when idle, for both ddr and psram must be set after configuring pwdpad_idle_val Note: only stop psram auto axi access, manually enable/disable this bit when using psram user axi access */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HSYS_DUMMY_1E4
 * @brief
 * @{
 *****************************************************************************/
#define HSYS_MASK_PI_PWROFF_DLY           ((u32)0x00000007 << 14)          /*!<R/W 3'h7  ECO: after shutdown ddrphy CK_OE, delay several cycles(4T~8T dfi_clk) to shutdown ddrphy PI */
#define HSYS_PI_PWROFF_DLY(x)             ((u32)(((x) & 0x00000007) << 14))
#define HSYS_GET_PI_PWROFF_DLY(x)         ((u32)(((x >> 14) & 0x00000007)))
#define HSYS_MASK_PWDPAD_RESUME_VAL       ((u32)0x0000001F << 9)          /*!<R/W 5'h1f  ECO: dq pads resume counter setting, only for psram, auto axi access will be stopped until resume done */
#define HSYS_PWDPAD_RESUME_VAL(x)         ((u32)(((x) & 0x0000001F) << 9))
#define HSYS_GET_PWDPAD_RESUME_VAL(x)     ((u32)(((x >> 9) & 0x0000001F)))
#define HSYS_MASK_PWDPAD_IDLE_VAL         ((u32)0x000001FF << 0)          /*!<R/W 9'h1ff  ECO: idle counter setting, only for psram, shutdown dq pads after reaching this value */
#define HSYS_PWDPAD_IDLE_VAL(x)           ((u32)(((x) & 0x000001FF) << 0))
#define HSYS_GET_PWDPAD_IDLE_VAL(x)       ((u32)(((x >> 0) & 0x000001FF)))
/** @} */


/*==========HSYS Register Address Definition==========*/
#define REG_HSYS_HP_PWC                              0x0000
#define REG_HSYS_HP_ISO                              0x0004
#define REG_HSYS_HP_FEN                              0x0008
#define REG_HSYS_HP_CKE                              0x000C
#define REG_HSYS_HP_CKSL                             0x0010
#define REG_HSYS_HPLAT_CTRL                          0x0030
#define REG_HSYS_HPLAT_STATUS                        0x0034
#define REG_HSYS_KM4_XO_CTRL0                        0x0038
#define REG_HSYS_KM4_XO_CTRL1                        0x003C
#define REG_HSYS_SPIC_CTRL                           0x0040
#define REG_HSYS_USB_CTRL                            0x0060
#define REG_HSYS_USB_TEST_CTRL                       0x0064
#define REG_HSYS_SDH_CTRL                            0x0070
#define REG_HSYS_DDR_CTRL                            0x0080
#define REG_HSYS_LXBUS_DBG_CTRL                      0x0090
#define REG_HSYS_LXBUS_DBG0                          0x0094
#define REG_HSYS_LXBUS_DBG1                          0x0098
#define REG_HSYS_DUMMY_1E0                           0x01E0
#define REG_HSYS_DUMMY_1E4                           0x01E4
#define REG_HSYS_DUMMY_1E8                           0x01E8
#define REG_HSYS_DUMMY_1EC                           0x01EC
#define REG_HSYS_BOUNDARY_CHK                        0x01F0
#define REG_HSYS_DUMMY_1F4                           0x01F4
#define REG_HSYS_DUMMY_1F8                           0x01F8
#define REG_HSYS_DUMMY_1FC                           0x01FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here
#define AP_CLK_DIV1		0
#define AP_CLK_DIV2		1
#define AP_CLK_DIV3		2
#define AP_CLK_DIV4		3

#define AP_CLK_APPLL			0
#define AP_CLK_APPLL_DIV2		2
#define AP_CLK_NPPLL			3


/* MANUAL_GEN_END */

#endif
