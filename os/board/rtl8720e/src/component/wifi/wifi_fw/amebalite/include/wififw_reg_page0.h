#ifndef __WIFIFW_REG_PAGE0_H__
#define __WIFIFW_REG_PAGE0_H__

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_SYS_PWC_ISO_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WL_SWR_REQ_1P1V                    ((u32)0x00000001 << 4)          /*!<R/W 0  1: wl request switch SWR vlotage to 1.1V 0: wl request switch SWR vlotage to 0.9V */
#define BIT_WL_PWM_EN                          ((u32)0x00000001 << 3)          /*!<R/W 0  1:WL request switch to PWM mode 0：WL request switch to PFM mode */
#define BIT_WL_SAVE_EN                         ((u32)0x00000001 << 2)          /*!<R/W 0  1 to 0:gen req to restore register in power on flow 1:gen req to backup register in power on flow */
#define BIT_SPC_MAC_EN                         ((u32)0x00000001 << 0)          /*!<R/W 0  1: Enable WLOFF power cut */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_CLK_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WLON_SWITCH_TSF_EN                 ((u32)0x00000001 << 12)          /*!<R/W 0  enable wlpmc switch TSF clock to MAC clock */
#define BIT_WLOFF_SWITCH_TSF_EN                ((u32)0x00000001 << 11)          /*!<R/W 0  enable wlpmc switch TSF clock to 32K clock */
#define BIT_WL_CKEN_IQDUMP                     ((u32)0x00000001 << 10)          /*!<R/W 0  0: disable clock for iqdump (LA MODE) 1: enable clock for iqdump (LA MODE) same clock source as security clock */
#define BIT_IS_SCH_CLK_INSLP_V2                ((u32)0x00000001 << 9)          /*!<RO 0  WL TSF clock status: 0: MAC clock 1: 32K clock */
#define BIT_TSF_CKSLP_SEL                      ((u32)0x00000001 << 8)          /*!<R/W 0  1: TSF clock switch to 32.768KHz; 0: TSF clock switch to MAC clock */
#define BIT_WL_CKTSF_EN                        ((u32)0x00000001 << 6)          /*!<R/W 0  1: enable tsf clock; 0: gated */
#define BIT_WL_FQSEL_CK32K                     ((u32)0x00000001 << 5)          /*!<R/W 0  1:32K frequency select 32.768KHz 0:32K frequency select 32KHz */
#define BIT_CKSL_CK32K                         ((u32)0x00000001 << 4)          /*!<R/W 0  1:select MAC internal 32K clk, 0:select MAC external 32K clk */
#define BIT_WL_CK32K_EN                        ((u32)0x00000001 << 3)          /*!<R/W 0  1: enable 32k clock; 0: gated */
#define BIT_WL_CKMCU_EN                        ((u32)0x00000001 << 2)          /*!<R/W 0  1: enable MCU engine clock; 0: gated */
#define BIT_WL_CKSEC_EN                        ((u32)0x00000001 << 1)          /*!<R/W 0  1: enable MAC security engine clock; 0: gated */
#define BIT_WL_CKMAC_EN                        ((u32)0x00000001 << 0)          /*!<R/W 0  1: enable MAC clock; 0: gated */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_FUNC_EN
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WL_SEL_SW_SI                       ((u32)0x00000001 << 24)          /*!<R/W 0  indicate SW use indirect path to access WLRFC register */
#define BIT_BB_SAVE_EN                         ((u32)0x00000001 << 18)          /*!<R/W 0  1 to 0:gen req to restore register in power on flow 1:gen req to backup register in power on flow */
#define BIT_FEN_BBRSTB_V2                      ((u32)0x00000001 << 17)          /*!<R/W 0  When this bit is set to “0”, CCK and OFDM are disabled, and clock are gated. Otherwise, CCK and OFDM are enabled. */
#define BIT_FEN_BB_GLB_RSTN_V2                 ((u32)0x00000001 << 16)          /*!<R/W 0  When this bit is set to “0”, whole BB is reset. When this bit is set, BB is enabled. */
#define BIT_SEC_ACCESS                         ((u32)0x00000001 << 12)          /*!<R/W 0  enable security access wifi register, no use this bit only access by secutiry */
#define BIT_FEN_MLPLT                          ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_RFAFE_HW33PDB                      ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_FEN_WLON                           ((u32)0x00000001 << 4)          /*!<R/W 0  1: enable WL partial ON; 0: reset */
#define BIT_FEN_WLMCU                          ((u32)0x00000001 << 3)          /*!<R/W 0  1: enable WLMCU; 0: reset */
#define BIT_FEN_MACREG                         ((u32)0x00000001 << 2)          /*!<R/W 0  1: enable MAC REG; 0: reset */
#define BIT_FEN_WLOFF                          ((u32)0x00000001 << 1)          /*!<R/W 0  1: enable WLOFF; 0: reset */
#define BIT_FEN_MAC_PMC                        ((u32)0x00000001 << 0)          /*!<R/W 0  1: enable MAC PMC; 0: reset */
/** @} */

/**************************************************************************//**
 * @defgroup REG_CLKDIV_CKSEL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WL_CKRFC_EN                        ((u32)0x00000001 << 24)          /*!<R/W 0  1: enable WLRFC clock; 0: gated */
#define BIT_MASK_RFC_CLK_SEL                   ((u32)0x00000003 << 22)          /*!<R/W 2'b01  11/10:40M 01:80M 00:160M */
#define BIT_RFC_CLK_SEL(x)                     ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_RFC_CLK_SEL(x)                 ((u32)(((x >> 22) & 0x00000003)))
#define BIT_MASK_SEC_CLK_SEL                   ((u32)0x00000003 << 20)          /*!<R/W 2'b01  11/10:40M 01:80M 00:160M */
#define BIT_SEC_CLK_SEL(x)                     ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_SEC_CLK_SEL(x)                 ((u32)(((x >> 20) & 0x00000003)))
#define BIT_MASK_MAC_CLK_SEL                   ((u32)0x00000003 << 18)          /*!<R/W 2'b01  11/10:20M 01:40M 00:80M */
#define BIT_MAC_CLK_SEL(x)                     ((u32)(((x) & 0x00000003) << 18))
#define BIT_GET_MAC_CLK_SEL(x)                 ((u32)(((x >> 18) & 0x00000003)))
#define BIT_MASK_CPU_CLK_SEL                   ((u32)0x00000003 << 16)          /*!<R/W 0   */
#define BIT_CPU_CLK_SEL(x)                     ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_CPU_CLK_SEL(x)                 ((u32)(((x >> 16) & 0x00000003)))
#define BIT_WL_CLK_SYNC                        ((u32)0x00000001 << 15)          /*!<R/W 0  1: sync 40M/80M clock phase with PLL */
#define BIT_MASK_WL_RFC_PHASE                  ((u32)0x0000000F << 8)          /*!<R/W 0  80M WLRFC clock phase control */
#define BIT_WL_RFC_PHASE(x)                    ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_WL_RFC_PHASE(x)                ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_WL_40M_PHASE                  ((u32)0x0000000F << 4)          /*!<R/W 0  40M MAC clock phase control */
#define BIT_WL_40M_PHASE(x)                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_WL_40M_PHASE(x)                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_WL_80M_PHASE                  ((u32)0x0000000F << 0)          /*!<R/W 0  80M security clock phase control */
#define BIT_WL_80M_PHASE(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_WL_80M_PHASE(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CPWM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CPWM_TOGGLING                      ((u32)0x00000001 << 15)          /*!<R/W 0  Toggling bit : one bit sequence number bit. When this bit is toggled, interrupt is send to HISR.CPWMINT. */
#define BIT_MASK_CPWM_MOD                      ((u32)0x00007FFF << 0)          /*!<R/W 0  The current power mode index. */
#define BIT_CPWM_MOD(x)                        ((u32)(((x) & 0x00007FFF) << 0))
#define BIT_GET_CPWM_MOD(x)                    ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RPWM
 * @brief register for hrpwm
 * @{
 *****************************************************************************/
#define BIT_RPWM_TOGGLING                      ((u32)0x00000001 << 15)          /*!<R/W 0  Toggling bit : one bit sequence number bit. When this bit is toggled, interrupt is send to FISR.RPWMINT. */
#define BIT_MASK_RPWM_MOD                      ((u32)0x00007FFF << 0)          /*!<R/W 0  The current power mode index. */
#define BIT_RPWM_MOD(x)                        ((u32)(((x) & 0x00007FFF) << 0))
#define BIT_GET_RPWM_MOD(x)                    ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RSV_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HREG_DBG                      ((u32)0x00000FFF << 12)          /*!<RO 0  HREG Access Debug information */
#define BIT_HREG_DBG(x)                        ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_HREG_DBG(x)                    ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_LOCK_REGALL_EN                     ((u32)0x00000001 << 7)          /*!<R/W 0  1:Enable ‘Locked All’ setting by Hardware, 0: Hardware Enable ‘Locked All’ denied. */
#define BIT_PRST_WLMAC                         ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_WLOCK_1C_B6                        ((u32)0x00000001 << 5)          /*!<R/W 0  1:Register Write Locked, 0: Register Accessible by SW/FW (offset:0x001C[6]) */
#define BIT_WLOCK_40                           ((u32)0x00000001 << 4)          /*!<R/W 0  1:Register Write Locked, 0: Register Accessible by SW/FW (offset:0x0040h~0x0043h) */
#define BIT_WLOCK_08                           ((u32)0x00000001 << 3)          /*!<R/W 0  1:Register Write Locked, 0: Register Accessible by SW/FW (offset:0x0008h~0x000Bh) */
#define BIT_WLOCK_04                           ((u32)0x00000001 << 2)          /*!<R/W 0  1:Register Write Locked, 0: Register Accessible by SW/FW (offset:0x0004h~0x0007h) */
#define BIT_WLOCK_00                           ((u32)0x00000001 << 1)          /*!<R/W 0  1:Register Write Locked, 0: Register Accessible by SW/FW (offset:0x0000h~0x0003h) */
#define BIT_WLOCK_ALL                          ((u32)0x00000001 << 0)          /*!<R/W 0  1:Register Write Locked, 0: Register Accessible by SW/FW (offset:0x0000h~0x00FFh) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_PMC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WLPON_OPT_EXCKN2A                  ((u32)0x00000001 << 25)          /*!<R/W 1   */
#define BIT_WLPON_OPT_EXCKP2A                  ((u32)0x00000001 << 24)          /*!<R/W 1  PON option to enable AFE clock */
#define BIT_MASK_WL_PMC_CNT_PAR                ((u32)0x0000003F << 8)          /*!<R/W 0x1F  WL PMC FSM counting unit. */
#define BIT_WL_PMC_CNT_PAR(x)                  ((u32)(((x) & 0x0000003F) << 8))
#define BIT_GET_WL_PMC_CNT_PAR(x)              ((u32)(((x >> 8) & 0x0000003F)))
#define BIT_CUR_PST_IS_LPS                     ((u32)0x00000001 << 4)          /*!<RO   indicate current WL power status is LPS mode */
#define BIT_WL_PMC_LPS_EN                      ((u32)0x00000001 << 2)          /*!<R/W 0  Inform WLAN PMC FSM to enter LPS mode. (auto clear when ready) */
#ifndef BIT_WL_PMC_OFFMAC
#define BIT_WL_PMC_OFFMAC                      ((u32)0x00000001 << 1)          /*!<R/W 0  Inform WLAN PMC FSM to power OFF mode, include clock, isolation, power control for MAC only (clear when Power Ready) */
#endif
#define BIT_WL_PMC_ONMAC                       ((u32)0x00000001 << 0)          /*!<R/W 0  Inform WLAN PMC FSM to power On mode, include clock, isolation, power control for MAC only (clear when Power Ready) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_SYSCONFIG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DIS_TIMEOUT_IO                     ((u32)0x00000001 << 24)          /*!<R/W 0  Disabled timeout of IO wrapper */
#define BIT_MCU_WDT_MODE                       ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_WLCPU_ANA_PORT_IDLE                ((u32)0x00000001 << 17)          /*!<R 0   */
#define BIT_WLCPU_MAC_PORT_IDLE                ((u32)0x00000001 << 16)          /*!<R 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_P0_TBTT_TIMER
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P0_ELY_BCNINT_CNT             ((u32)0x0000007F << 24)          /*!<R 0  Unit: bcn_space the total time to next specified TBTT is (new_ely_agg_cnt - 1) * r_bcn_space(0x554) + (beacon_cnt_down - 1)* 1ms + (cnt_128us + 1)* 128us */
#define BIT_P0_ELY_BCNINT_CNT(x)               ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_P0_ELY_BCNINT_CNT(x)           ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_P0_BCN_CNT_DOWN               ((u32)0x0000FFFF << 3)          /*!<R 0  Unit: 1ms beacon_cnt_down - 1 */
#define BIT_P0_BCN_CNT_DOWN(x)                 ((u32)(((x) & 0x0000FFFF) << 3))
#define BIT_GET_P0_BCN_CNT_DOWN(x)             ((u32)(((x >> 3) & 0x0000FFFF)))
#define BIT_MASK_P0_128US_CNT                  ((u32)0x00000007 << 0)          /*!<R 0  Unit: 128us */
#define BIT_P0_128US_CNT(x)                    ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_P0_128US_CNT(x)                ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AFC_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_POW_AFE_REQ                        ((u32)0x00000001 << 13)          /*!<R/W 0  AFE power on request to afe_ctrl 1:request to turn on AFE pwr 0:turn off AFE pwr */
#define BIT_WL_XTAL_ACTIVE_MODE                ((u32)0x00000001 << 7)          /*!<R/W 1  1:WL request XTAL to enter active mode 0:WL request XTAL to enter LPS mode */
#define BIT_WL_XTAL_EN                         ((u32)0x00000001 << 6)          /*!<R/W 0  1:WL enable XTAL 0:WL disable XTAL */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FSIMR_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_PWMERR_INT_EN_V1                ((u32)0x00000001 << 1)          /*!<R/W 0  Enable MCU Access Error interrupt source(TIE0) */
#define BIT_FS_BTON_STS_UPDATE_INT_EN          ((u32)0x00000001 << 0)          /*!<R/W 0  BTON status update interrupt When 0x78[31] toggled, this interrupt is issue to indicate the status updated. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FSISR_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_PWMERR_INT_V1                   ((u32)0x00000001 << 1)          /*!<RW1C 0  MCU Access Error interrupt source Write 1 clear */
#define BIT_FS_BTON_STS_UPDATE_INT             ((u32)0x00000001 << 0)          /*!<RW1C 0  BTON status update interrupt When 0x78[31] toggled, this interrupt is issue to indicate the status updated. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXDMA_H2C_RD_POINTER
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TDE_H2C_RD_ADDR               ((u32)0x0003FFFF << 0)          /*!<R 0  not used in amd2 */
#define BIT_TDE_H2C_RD_ADDR(x)                 ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_TDE_H2C_RD_ADDR(x)             ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXDMA_H2C_WR_POINTER
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TDE_H2C_WR_ADDR               ((u32)0x0003FFFF << 0)          /*!<R 0  not used in amd2 */
#define BIT_TDE_H2C_WR_ADDR(x)                 ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_TDE_H2C_WR_ADDR(x)             ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG0_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG0                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug0 */
#define BIT_FW_DBG0(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG0(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG1_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG1                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug1 */
#define BIT_FW_DBG1(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG1(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG2_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG2                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug2 */
#define BIT_FW_DBG2(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG2(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG3_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG3                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug3 */
#define BIT_FW_DBG3(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG3(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG4_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG4                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug4 */
#define BIT_FW_DBG4(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG4(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG5_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG5                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug5 */
#define BIT_FW_DBG5(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG5(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG6_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG6                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug6 */
#define BIT_FW_DBG6(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG6(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FW_DBG7_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_DBG7                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug7 */
#define BIT_FW_DBG7(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_DBG7(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_PIN_FUNC_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WL_DBG_SEL                    ((u32)0x000000FF << 24)          /*!<R/W 0  WL debug select */
#define BIT_WL_DBG_SEL(x)                      ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_WL_DBG_SEL(x)                  ((u32)(((x >> 24) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_BTCOEX_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_LTECOEX_DBGEN                      ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_MASK_BTMODE_V1                     ((u32)0x00000003 << 17)          /*!<R/W 0  Bluetooth Mode */
#define BIT_BTMODE_V1(x)                       ((u32)(((x) & 0x00000003) << 17))
#define BIT_GET_BTMODE_V1(x)                   ((u32)(((x >> 17) & 0x00000003)))
#define BIT_ENBT_V1                            ((u32)0x00000001 << 16)          /*!<R/W 0  Enable PTA ( from BT side) */
#define BIT_WL_DPDT1_IDV                       ((u32)0x00000001 << 3)          /*!<R/W 1   */
#define BIT_WL_DPDT1_MUX                       ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_WL_DPDT0_IDV                       ((u32)0x00000001 << 1)          /*!<R/W 1   */
#define BIT_WL_DPDT0_MUX                       ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_SCOREBOARD_RD_BT2WL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RD_BT2WL_INFO                 ((u32)0x7FFFFFFF << 0)          /*!<R 0  Read Data : WL TO BT INFO, WLON Status Report */
#define BIT_RD_BT2WL_INFO(x)                   ((u32)(((x) & 0x7FFFFFFF) << 0))
#define BIT_GET_RD_BT2WL_INFO(x)               ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DBG_PORT_REG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DEBUG_ST                      ((u32)0xFFFFFFFF << 0)          /*!<RO 0  Debug signals, sel by 70[31:24] */
#define BIT_DEBUG_ST(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DEBUG_ST(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_MCUFW_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FW_INIT_RDY                        ((u32)0x00000001 << 15)          /*!<R/W 0  CPU fw initialize ready, set by CPU */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BLK_TST
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BLK_TST                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw debug8 */
#define BIT_BLK_TST(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BLK_TST(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HOST_MSG0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HOST_MSG_E1                   ((u32)0x0000FFFF << 16)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_E1(x)                     ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_HOST_MSG_E1(x)                 ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_HOST_MSG_E0                   ((u32)0x0000FFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_E0(x)                     ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_HOST_MSG_E0(x)                 ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HOST_MSG1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HOST_MSG_E3                   ((u32)0x0000FFFF << 16)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_E3(x)                     ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_HOST_MSG_E3(x)                 ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_HOST_MSG_E2                   ((u32)0x0000FFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_E2(x)                     ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_HOST_MSG_E2(x)                 ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LPS_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WL_LPS_OPT_SAVE_REG                ((u32)0x00000001 << 29)          /*!<R/W 0  LPS option to save retention FF 1: enable request to save retention FF 0: disable request to save retention FF */
#define BIT_LPS_MEMPM_PHYON                    ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_MASK_LPS_MEMPM_MACOFF_FTC_ZGB      ((u32)0x00000003 << 25)          /*!<R/W 2'b11  MAC OFF FTC memory power mode when enable LPS memory power mode(for zgb_txbuf/zgb_rxbuf) 00 : normal 01 : NAP 10 : RET 11 : SLP */
#define BIT_LPS_MEMPM_MACOFF_FTC_ZGB(x)        ((u32)(((x) & 0x00000003) << 25))
#define BIT_GET_LPS_MEMPM_MACOFF_FTC_ZGB(x)    ((u32)(((x >> 25) & 0x00000003)))
#define BIT_MASK_LPS_MEMPM_MACOFF_FTC_SHR      ((u32)0x00000003 << 23)          /*!<R/W 2'b11  MAC OFF FTC memory power mode when enable LPS memory power mode(for rxpktbuf/ txpktbuf low 24k[SHARE with system]) 00 : normal 01 : NAP 10 : RET 11 : SLP */
#define BIT_LPS_MEMPM_MACOFF_FTC_SHR(x)        ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_LPS_MEMPM_MACOFF_FTC_SHR(x)    ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_LPS_MEMPM_MACOFF_SNPS_TXRPT   ((u32)0x00000003 << 21)          /*!<R/W 2'b11  MAC OFF SNPS memory power mode when enable LPS memory power mode (for txrptbuf) 00 : normal 01 : LS 10 : DS 11 : SD */
#define BIT_LPS_MEMPM_MACOFF_SNPS_TXRPT(x)     ((u32)(((x) & 0x00000003) << 21))
#define BIT_GET_LPS_MEMPM_MACOFF_SNPS_TXRPT(x) ((u32)(((x >> 21) & 0x00000003)))
#define BIT_MASK_LPS_MEMPM_MACON_FTC_TXPKT     ((u32)0x00000003 << 19)          /*!<R/W 2'b10  MAC ON FTC memory power mode when enable LPS memory power mode (for txpktbuf high 8k) 00 : normal 01 : NAP 10 : RET 11 : SLP */
#define BIT_LPS_MEMPM_MACON_FTC_TXPKT(x)       ((u32)(((x) & 0x00000003) << 19))
#define BIT_GET_LPS_MEMPM_MACON_FTC_TXPKT(x)   ((u32)(((x >> 19) & 0x00000003)))
#define BIT_LPS_MEMPM_MACOFF_FTC_TRXFIFO       ((u32)0x00000001 << 18)          /*!<R/W 0x1  MAC OFF FTC memory power mode when enable LPS memory power mode (for rxfifo/txfifo) 0 : normal 1 : NAP */
#define BIT_LPS_MEMPM_MACOFF_SNPS              ((u32)0x00000001 << 17)          /*!<R/W 0x1  MAC OFF SNPS memory power mode when enable LPS memory power mode (for txllt/rc4/txoqt/keysrch/rxbacam/dmafifo/txpwr_rate/txpwr_macid/txpwr_usrlist/txpwr_srpwr/txpwr_rulim) 0 : normal 1 : LS */
#define BIT_MASK_LPS_MEMPM_MACOFF_FTC          ((u32)0x00000003 << 15)          /*!<R/W 2'b11  MAC OFF FTC memory power mode when enable LPS memory power mode(for rxcsibuf/phystsbuf) 00 : normal 01 : NAP 10 : RET 11 : SLP */
#define BIT_LPS_MEMPM_MACOFF_FTC(x)            ((u32)(((x) & 0x00000003) << 15))
#define BIT_GET_LPS_MEMPM_MACOFF_FTC(x)        ((u32)(((x >> 15) & 0x00000003)))
#define BIT_WL_LPS_OPT_PFM_EN                  ((u32)0x00000001 << 14)          /*!<R/W 0  LPS option to switch to PFM mode 1: set to PFM mode 0: stay in PWM mode */
#define BIT_WL_LPS_OPT_SWR_0P9                 ((u32)0x00000001 << 13)          /*!<R/W 0  LPS option to set SWR voltage 0.9V 1: set SWR voltage 0.9V 0: set SWR voltage 1.1V */
#define BIT_WL_LPS_OPT_LP_XTAL                 ((u32)0x00000001 << 12)          /*!<R/W 0  LPS option to set XTAL low power mode when enable XTAL 1: set XTAL low power mode 0: set XTAL active mode */
#define BIT_WL_LPS_OPT_DIS_XTAL                ((u32)0x00000001 << 11)          /*!<R/W 0  LPS option to disable XTAL 1: disable XTAL 0: enable XTAL */
#define BIT_WL_LPS_OPT_EMACFUN                 ((u32)0x00000001 << 10)          /*!<R/W 0  LPS option to reset WL MAC OFF block when enable WL MAC OFF power 1: not reset WL MAC OFF block 0: reset WL MAC OFF block */
#define BIT_WL_LPS_OPT_EPHYPWR                 ((u32)0x00000001 << 9)          /*!<R/W 0  LPS option to enable WL BB block power 1: Enable WL BB block power 0: Disable WL BB block power */
#define BIT_WL_LPS_OPT_EMACPWR                 ((u32)0x00000001 << 8)          /*!<R/W 0  LPS option to enable WL MAC OFF block power 1: Enable WL MAC OFF block power 0: Disable WL MAC OFF block power */
#define BIT_WL_LPS_MEMPM_EN                    ((u32)0x00000001 << 1)          /*!<R/W 0  enable LPS memory power mode control */
#define BIT_WL_NRM_MEMPM_SL                    ((u32)0x00000001 << 0)          /*!<R/W 0  MAC memory power mode when BIT_WL_LPS_MEMPM_EN is 0, 0 : Normal 1 : SD */
/** @} */

/**************************************************************************//**
 * @defgroup REG_boot_reason
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WL_WDT_PLFM                        ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_WL_WDT_MCU                         ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_SCOREBOARD_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BT_INT_EN                          ((u32)0x00000001 << 31)          /*!<W 0  For WL, write 1 to issue interrupt 1 : Issue one INT to BT 0 : Don't care */
#define BIT_MASK_WL2BT_INFO                    ((u32)0x7FFFFFFF << 0)          /*!<R/W 0  WLON Status Report TO BTON */
#define BIT_WL2BT_INFO(x)                      ((u32)(((x) & 0x7FFFFFFF) << 0))
#define BIT_GET_WL2BT_INFO(x)                  ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SYSON_FSM_MON_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WLPMC_DEBUG_SEL               ((u32)0x0000000F << 24)          /*!<R/W 0  debug select signal for WL PMC */
#define BIT_WLPMC_DEBUG_SEL(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_WLPMC_DEBUG_SEL(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_WLPMC_DEBUG                   ((u32)0x0000FFFF << 0)          /*!<RO 0  debug signal of WL PMC */
#define BIT_WLPMC_DEBUG(x)                     ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_WLPMC_DEBUG(x)                 ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PMC_DBG_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PMC_WR_OVF                         ((u32)0x00000001 << 8)          /*!<RW1C 0  PMC Write Access Timer Overflow */
#define BIT_MASK_WLPMC_ERRINT                  ((u32)0x000000FF << 0)          /*!<RW1C 0  WLPMC Error interrupt */
#define BIT_WLPMC_ERRINT(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_WLPMC_ERRINT(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PMC_DBG_CTRL2_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SYSON_REG_ARB                 ((u32)0x00000003 << 0)          /*!<R/W 0  HW PMC Access Register Time Out Control */
#define BIT_SYSON_REG_ARB(x)                   ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_SYSON_REG_ARB(x)               ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HIMR0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_HISR1_INT_EN                       ((u32)0x00000001 << 31)          /*!<R/W 0  HISR1 Indicator (HIISR1 andHIMR1 are true, this bit is set to 1) */
#define BIT_RXDES_UNAVAIL_INT_EN               ((u32)0x00000001 << 30)          /*!<R/W 0  Rx Descriptor Unavailable */
#define BIT_HISR2_INT_EN                       ((u32)0x00000001 << 29)          /*!<R/W 0  HISR2 Indicator (HIISR2 andHIMR2 are true, this bit is set to 1) */
#define BIT_EVTQ_DONE_INT_EN                   ((u32)0x00000001 << 28)          /*!<R/W 0  event queue done interrupt for NAN */
#define BIT_EVTQ_START_INT_EN_V1               ((u32)0x00000001 << 27)          /*!<R/W 0  event queue start interrupt for NAN */
#define BIT_FWMSG_INT_EN                       ((u32)0x00000001 << 26)          /*!<R/W 0  FW message interrupt,CPU to Host Command INT Status, Write 1 clear */
#define BIT_HCPWM_INT_EN                       ((u32)0x00000001 << 24)          /*!<R/W 0  CPWM interrupt,CPU power Mode exchange INT Status, Write 1 clear */
#define BIT_RXFF_FULL_INT_EN                   ((u32)0x00000001 << 23)          /*!<R/W 0  Rxpktbuffer full interrupt */
#define BIT_RDE_ERR_INT_EN                     ((u32)0x00000001 << 22)          /*!<R/W 0  Rxdma error interrupt */
#define BIT_TDE_ERR_INT_EN                     ((u32)0x00000001 << 21)          /*!<R/W 0  Txdma error interrupt */
#define BIT_TXFF_FIFO_INT_EN                   ((u32)0x00000001 << 20)          /*!<R/W 0  txpktbuffer full interrupt */
#define BIT_TXBCN8_ERR_INT_EN                  ((u32)0x00000001 << 19)          /*!<R/W 0  port0 BSSID7 TX beacon error interrupt */
#define BIT_TXBCN7_ERR_INT_EN                  ((u32)0x00000001 << 18)          /*!<R/W 0  port0 BSSID6 TX beacon error interrupt */
#define BIT_TXBCN6_ERR_INT_EN                  ((u32)0x00000001 << 17)          /*!<R/W 0  port0 BSSID5 TX beacon error interrupt */
#define BIT_TXBCN5_ERR_INT_EN                  ((u32)0x00000001 << 16)          /*!<R/W 0  port0 BSSID4 TX beacon error interrupt */
#define BIT_TXBCN4_ERR_INT_EN                  ((u32)0x00000001 << 15)          /*!<R/W 0  port0 BSSID3 TX beacon error interrupt */
#define BIT_TXBCN3_ERR_INT_EN                  ((u32)0x00000001 << 14)          /*!<R/W 0  port0 BSSID2 TX beacon error interrupt */
#define BIT_TXBCN2_ERR_INT_EN                  ((u32)0x00000001 << 13)          /*!<R/W 0  port0 BSSID1 TX beacon error interrupt */
#define BIT_TXBCN1_ERR_INT_EN                  ((u32)0x00000001 << 12)          /*!<R/W 0  port0 BSSID0 TX beacon error interrupt */
#define BIT_TXBCN8_OK_INT_EN                   ((u32)0x00000001 << 11)          /*!<R/W 0  port0 BSSID7 TX beacon ok interrupt */
#define BIT_TXBCN7_OK_INT_EN                   ((u32)0x00000001 << 10)          /*!<R/W 0  port0 BSSID6 TX beacon ok interrupt */
#define BIT_TXBCN6_OK_INT_EN                   ((u32)0x00000001 << 9)          /*!<R/W 0  port0 BSSID5 TX beacon ok interrupt */
#define BIT_TXBCN5_OK_INT_EN                   ((u32)0x00000001 << 8)          /*!<R/W 0  port0 BSSID4 TX beacon ok interrupt */
#define BIT_TXBCN4_OK_INT_EN                   ((u32)0x00000001 << 7)          /*!<R/W 0  port0 BSSID3 TX beacon ok interrupt */
#define BIT_TXBCN3_OK_INT_EN                   ((u32)0x00000001 << 6)          /*!<R/W 0  port0 BSSID2 TX beacon ok interrupt */
#define BIT_TXBCN2_OK_INT_EN                   ((u32)0x00000001 << 5)          /*!<R/W 0  port0 BSSID1 TX beacon ok interrupt */
#define BIT_TXBCN1_OK_INT_EN                   ((u32)0x00000001 << 4)          /*!<R/W 0  port0 BSSID0 TX beacon ok interrupt */
#define BIT_TSF_BIT32_TOGGLE_INT_EN            ((u32)0x00000001 << 3)          /*!<R/W 0  TSF Timer BIT32 toggle indication interrupt */
#define BIT_TIMEOUT2_INT_EN                    ((u32)0x00000001 << 2)          /*!<R/W 0  tsf Timer2 interrupt */
#define BIT_TIMEOUT1_V1_INT_EN                 ((u32)0x00000001 << 1)          /*!<R/W 0  tsf Timer1 interrupt */
#define BIT_TIMEOUT0_V1_INT_EN                 ((u32)0x00000001 << 0)          /*!<R/W 0  tsf Timer0 interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HISR0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_HISR1_INT                          ((u32)0x00000001 << 31)          /*!<RW1C 0  HISR1 Indicator (HIISR1 andHIMR1 are true, this bit is set to 1) */
#define BIT_RXDES_UNAVAIL_INT                  ((u32)0x00000001 << 30)          /*!<RW1C 0  Rx Descriptor Unavailable */
#define BIT_HISR2_INT                          ((u32)0x00000001 << 29)          /*!<RW1C 0  HISR2 Indicator (HIISR2 andHIMR2 are true, this bit is set to 1) */
#define BIT_EVTQ_DONE_INT                      ((u32)0x00000001 << 28)          /*!<RW1C 0  event queue done interrupt for NAN */
#define BIT_EVTQ_START_INT_V1                  ((u32)0x00000001 << 27)          /*!<RW1C 0  event queue start interrupt for NAN */
#define BIT_FWMSG_INT                          ((u32)0x00000001 << 26)          /*!<RW1C 0  FW message interrupt,CPU to Host Command INT Status, Write 1 clear */
#define BIT_HCPWM_INT                          ((u32)0x00000001 << 24)          /*!<RW1C 0  CPWM interrupt,CPU power Mode exchange INT Status, Write 1 clear */
#define BIT_RXFF_FULL_INT                      ((u32)0x00000001 << 23)          /*!<RW1C 0  Rxpktbuffer full interrupt */
#define BIT_RDE_ERR_INT                        ((u32)0x00000001 << 22)          /*!<RW1C 0  Rxdma error interrupt */
#define BIT_TDE_ERR_INT                        ((u32)0x00000001 << 21)          /*!<RW1C 0  Txdma error interrupt */
#define BIT_TXFF_FIFO_INT                      ((u32)0x00000001 << 20)          /*!<RW1C 0  txpktbuffer full interrupt */
#define BIT_TXBCN8_ERR_INT                     ((u32)0x00000001 << 19)          /*!<RW1C 0  port0 BSSID7 TX beacon error interrupt */
#define BIT_TXBCN7_ERR_INT                     ((u32)0x00000001 << 18)          /*!<RW1C 0  port0 BSSID6 TX beacon error interrupt */
#define BIT_TXBCN6_ERR_INT                     ((u32)0x00000001 << 17)          /*!<RW1C 0  port0 BSSID5 TX beacon error interrupt */
#define BIT_TXBCN5_ERR_INT                     ((u32)0x00000001 << 16)          /*!<RW1C 0  port0 BSSID4 TX beacon error interrupt */
#define BIT_TXBCN4_ERR_INT                     ((u32)0x00000001 << 15)          /*!<RW1C 0  port0 BSSID3 TX beacon error interrupt */
#define BIT_TXBCN3_ERR_INT                     ((u32)0x00000001 << 14)          /*!<RW1C 0  port0 BSSID2 TX beacon error interrupt */
#define BIT_TXBCN2_ERR_INT                     ((u32)0x00000001 << 13)          /*!<RW1C 0  port0 BSSID1 TX beacon error interrupt */
#define BIT_TXBCN1_ERR_INT                     ((u32)0x00000001 << 12)          /*!<RW1C 0  port0 BSSID0 TX beacon error interrupt */
#define BIT_TXBCN8_OK_INT                      ((u32)0x00000001 << 11)          /*!<RW1C 0  port0 BSSID7 TX beacon ok interrupt */
#define BIT_TXBCN7_OK_INT                      ((u32)0x00000001 << 10)          /*!<RW1C 0  port0 BSSID6 TX beacon ok interrupt */
#define BIT_TXBCN6_OK_INT                      ((u32)0x00000001 << 9)          /*!<RW1C 0  port0 BSSID5 TX beacon ok interrupt */
#define BIT_TXBCN5_OK_INT                      ((u32)0x00000001 << 8)          /*!<RW1C 0  port0 BSSID4 TX beacon ok interrupt */
#define BIT_TXBCN4_OK_INT                      ((u32)0x00000001 << 7)          /*!<RW1C 0  port0 BSSID3 TX beacon ok interrupt */
#define BIT_TXBCN3_OK_INT                      ((u32)0x00000001 << 6)          /*!<RW1C 0  port0 BSSID2 TX beacon ok interrupt */
#define BIT_TXBCN2_OK_INT                      ((u32)0x00000001 << 5)          /*!<RW1C 0  port0 BSSID1 TX beacon ok interrupt */
#define BIT_TXBCN1_OK_INT                      ((u32)0x00000001 << 4)          /*!<RW1C 0  port0 BSSID0 TX beacon ok interrupt */
#define BIT_TSF_BIT32_TOGGLE_INT               ((u32)0x00000001 << 3)          /*!<RW1C 0  TSF Timer BIT32 toggle indication interrupt */
#define BIT_TIMEOUT2_INT                       ((u32)0x00000001 << 2)          /*!<RW1C 0  tsf Timer2 interrupt */
#define BIT_TIMEOUT1_INT_V1                    ((u32)0x00000001 << 1)          /*!<RW1C 0  tsf Timer1 interrupt */
#define BIT_TIMEOUT0_INT_V1                    ((u32)0x00000001 << 0)          /*!<RW1C 0  tsf Timer0 interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HEMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CTWEND1_INT_EN                     ((u32)0x00000001 << 31)          /*!<R/W 0  Client1 traffic window end interrupt for P2P */
#define BIT_CTWEND0_INT_EN                     ((u32)0x00000001 << 30)          /*!<R/W 0  Client0 traffic window end interrupt for P2P */
#define BIT_DWWIN_END_INT_EN_V1                ((u32)0x00000001 << 29)          /*!<R/W 0  discovery window end interrupt for NAN */
#define BIT_ATIMEND8_INT_EN                    ((u32)0x00000001 << 28)          /*!<R/W 0  port1 atimend interrupt */
#define BIT_ATIMEND7_INT_EN                    ((u32)0x00000001 << 27)          /*!<R/W 0  port0 BSSID7 atimend interrupt */
#define BIT_ATIMEND6_INT_EN                    ((u32)0x00000001 << 26)          /*!<R/W 0  port0 BSSID6 atimend interrupt */
#define BIT_ATIMEND5_INT_EN                    ((u32)0x00000001 << 25)          /*!<R/W 0  port0 BSSID5 atimend interrupt */
#define BIT_ATIMEND4_INT_EN                    ((u32)0x00000001 << 24)          /*!<R/W 0  port0 BSSID4 atimend interrupt */
#define BIT_ATIMEND3_INT_EN                    ((u32)0x00000001 << 23)          /*!<R/W 0  port0 BSSID3 atimend interrupt */
#define BIT_ATIMEND2_INT_EN                    ((u32)0x00000001 << 22)          /*!<R/W 0  port0 BSSID2 atimend interrupt */
#define BIT_ATIMEND1_INT_EN                    ((u32)0x00000001 << 21)          /*!<R/W 0  port0 BSSID1 atimend interrupt */
#define BIT_ATIMEND0_INT_EN                    ((u32)0x00000001 << 20)          /*!<R/W 0  port0 BSSID0 atimend interrupt */
#define BIT_BCNERLY8_INT_EN                    ((u32)0x00000001 << 19)          /*!<R/W 0  port1 beacon early interrupt */
#define BIT_BCNERLY7_INT_EN                    ((u32)0x00000001 << 18)          /*!<R/W 0  port0 BSSID7 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY6_INT_EN                    ((u32)0x00000001 << 17)          /*!<R/W 0  port0 BSSID6 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY5_INT_EN                    ((u32)0x00000001 << 16)          /*!<R/W 0  port0 BSSID5 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY4_INT_EN                    ((u32)0x00000001 << 15)          /*!<R/W 0  port0 BSSID4 beacon early interrupt, only valid under MBSSID mode */
#define BIT_BCNERLY3_INT_EN                    ((u32)0x00000001 << 14)          /*!<R/W 0  port0 BSSID3 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY2_INT_EN                    ((u32)0x00000001 << 13)          /*!<R/W 0  port0 BSSID2 beacon early interrupt, only valid under MBSSID mode */
#define BIT_BCNERLY1_INT_EN                    ((u32)0x00000001 << 12)          /*!<R/W 0  port0 BSSID1 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY0_INT_EN                    ((u32)0x00000001 << 11)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT time for port 0 root bssid */
#define BIT_HS_BCNELY1_AGGR_INT_EN             ((u32)0x00000001 << 10)          /*!<R/W 0  port 1 Beacon early aggregation interrupt */
#define BIT_CPU_MGQ_TXDONE_INT_EN              ((u32)0x00000001 << 9)          /*!<R/W 0  CPU_MQG_TXDONE ineterrupt source when HW TX CPUMGQ packet success, it issue interrupt to host */
#define BIT_PS_TIMER_C_INT_EN                  ((u32)0x00000001 << 8)          /*!<R/W 0  PS_Timer_C interrupt source When the lower 4 bytes of TSF(that select by reg 0x1510[18:16]) matches the content of this register, the PS_Timer_C_INT interrupt is issued */
#define BIT_PS_TIMER_B_INT_EN                  ((u32)0x00000001 << 7)          /*!<R/W 0  PS_Timer_B interrupt source When the lower 4 bytes of TSF(that select by reg 0x1510[10:8]) matches the content of this register, the PS_Timer_B_INT interrupt is issued */
#define BIT_PS_TIMER_A_INT_EN                  ((u32)0x00000001 << 6)          /*!<R/W 0  PS_Timer_A interrupt source When the lower 4 bytes of TSF(that select by reg 0x1510[2:0]) matches the content of this register, the PS_Timer_A_INT interrupt is issued */
#define BIT_HS_BCNELY0_AGGR_INT_EN             ((u32)0x00000001 << 5)          /*!<R/W 0  port 0 Beacon early aggregation interrupt */
#define BIT_GTINT4_EN                          ((u32)0x00000001 << 4)          /*!<R/W 0  When GTIMER4 expires, this bit is set to 1 */
#define BIT_GTINT3_EN                          ((u32)0x00000001 << 3)          /*!<R/W 0  When GTIMER3 expires, this bit is set to 1 */
#define BIT_TXBCNOK9_INT_EN                    ((u32)0x00000001 << 2)          /*!<R/W 0  PORT1 TX beacon OK interrupt */
#define BIT_TXBCNERE9_INT_EN                   ((u32)0x00000001 << 1)          /*!<R/W 0  PORT1 TX beacon Error interrupt */
#define BIT_RXBCN_NAN_RPT_OK_EN                ((u32)0x00000001 << 0)          /*!<R/W 0  port2 rxbcn rpt and write to txrptbuf ok */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HESR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CTWEND1_INT                        ((u32)0x00000001 << 31)          /*!<RW1C 0  Client1 traffic window end interrupt for P2P */
#define BIT_CTWEND0_INT                        ((u32)0x00000001 << 30)          /*!<RW1C 0  Client0 traffic window end interrupt for P2P */
#define BIT_DWWIN_END_INT_V1                   ((u32)0x00000001 << 29)          /*!<RW1C 0  discovery window end interrupt for NAN */
#define BIT_ATIMEND8_INT                       ((u32)0x00000001 << 28)          /*!<RW1C 0  port1 atimend interrupt */
#define BIT_ATIMEND7_INT                       ((u32)0x00000001 << 27)          /*!<RW1C 0  port0 BSSID7 atimend interrupt */
#define BIT_ATIMEND6_INT                       ((u32)0x00000001 << 26)          /*!<RW1C 0  port0 BSSID6 atimend interrupt */
#define BIT_ATIMEND5_INT                       ((u32)0x00000001 << 25)          /*!<RW1C 0  port0 BSSID5 atimend interrupt */
#define BIT_ATIMEND4_INT                       ((u32)0x00000001 << 24)          /*!<RW1C 0  port0 BSSID4 atimend interrupt */
#define BIT_ATIMEND3_INT                       ((u32)0x00000001 << 23)          /*!<RW1C 0  port0 BSSID3 atimend interrupt */
#define BIT_ATIMEND2_INT                       ((u32)0x00000001 << 22)          /*!<RW1C 0  port0 BSSID2 atimend interrupt */
#define BIT_ATIMEND1_INT                       ((u32)0x00000001 << 21)          /*!<RW1C 0  port0 BSSID1 atimend interrupt */
#define BIT_ATIMEND0_INT                       ((u32)0x00000001 << 20)          /*!<RW1C 0  port0 BSSID0 atimend interrupt */
#define BIT_BCNERLY8_INT                       ((u32)0x00000001 << 19)          /*!<RW1C 0  port1 beacon early interrupt */
#define BIT_BCNERLY7_INT                       ((u32)0x00000001 << 18)          /*!<RW1C 0  port0 BSSID7 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY6_INT                       ((u32)0x00000001 << 17)          /*!<RW1C 0  port0 BSSID6 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY5_INT                       ((u32)0x00000001 << 16)          /*!<RW1C 0  port0 BSSID5 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY4_INT                       ((u32)0x00000001 << 15)          /*!<RW1C 0  port0 BSSID4 beacon early interrupt, only valid under MBSSID mode */
#define BIT_BCNERLY3_INT                       ((u32)0x00000001 << 14)          /*!<RW1C 0  port0 BSSID3 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY2_INT                       ((u32)0x00000001 << 13)          /*!<RW1C 0  port0 BSSID2 beacon early interrupt, only valid under MBSSID mode */
#define BIT_BCNERLY1_INT                       ((u32)0x00000001 << 12)          /*!<RW1C 0  port0 BSSID1 beacon early interrupt , only valid under MBSSID mode */
#define BIT_BCNERLY0_INT                       ((u32)0x00000001 << 11)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT time for port 0 root bssid */
#define BIT_HS_BCNELY1_AGGR_INT                ((u32)0x00000001 << 10)          /*!<RW1C 0  port 1 Beacon early aggregation interrupt */
#define BIT_CPU_MGQ_TXDONE_INT                 ((u32)0x00000001 << 9)          /*!<RW1C 0  CPU_MQG_TXDONE ineterrupt source when HW TX CPUMGQ packet success, it issue interrupt to host */
#define BIT_PS_TIMER_C_INT                     ((u32)0x00000001 << 8)          /*!<RW1C 0  PS_Timer_C interrupt source When the lower 4 bytes of TSF(that select by reg 0x1510[18:16]) matches the content of this register, the PS_Timer_C_INT interrupt is issued */
#define BIT_PS_TIMER_B_INT                     ((u32)0x00000001 << 7)          /*!<RW1C 0  PS_Timer_B interrupt source When the lower 4 bytes of TSF(that select by reg 0x1510[10:8]) matches the content of this register, the PS_Timer_B_INT interrupt is issued */
#define BIT_PS_TIMER_A_INT                     ((u32)0x00000001 << 6)          /*!<RW1C 0  PS_Timer_A interrupt source When the lower 4 bytes of TSF(that select by reg 0x1510[2:0]) matches the content of this register, the PS_Timer_A_INT interrupt is issued */
#define BIT_HS_BCNELY0_AGGR_INT                ((u32)0x00000001 << 5)          /*!<RW1C 0  port 0 Beacon early aggregation interrupt */
#define BIT_GTINT4                             ((u32)0x00000001 << 4)          /*!<RW1C 0  When GTIMER4 expires, this bit is set to 1 */
#define BIT_GTINT3                             ((u32)0x00000001 << 3)          /*!<RW1C 0  When GTIMER3 expires, this bit is set to 1 */
#define BIT_TXBCNOK9_INT                       ((u32)0x00000001 << 2)          /*!<RW1C 0  PORT1 TX beacon OK interrupt */
#define BIT_TXBCNERR9_INT                      ((u32)0x00000001 << 1)          /*!<RW1C 0  PORT1 TX beacon Error interrupt */
#define BIT_RXBCN_NAN_RPT_OK                   ((u32)0x00000001 << 0)          /*!<RW1C 0  port2 rxbcn rpt and write to txrptbuf ok */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HIMR2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_KM0_INT_EN                         ((u32)0x00000001 << 31)          /*!<R/W 0  The gathering of interrupts of KM0 */
#define BIT_PS_TIMER_E_INT_EN                  ((u32)0x00000001 << 25)          /*!<R/W 0  PS_Timer_E interrupt When the TSF(that select by reg 0x1520[13:12]) matches the content of this register, the PS_Timer_E_INT interrupt is issued */
#define BIT_PS_TIMER_D_INT_EN                  ((u32)0x00000001 << 24)          /*!<R/W 0  PS_Timer_D interrupt When the lower 4 bytes of TSF(that select by reg 0x1520[17:16]) matches the content of this register, the PS_Timer_D_INT interrupt is issued */
#define BIT_PS_TIMER_E_EARLY_INT_EN            ((u32)0x00000001 << 23)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_E_Early register before PS_Timer_E time. */
#define BIT_PS_TIMER_D_EARLY_INT_EN            ((u32)0x00000001 << 22)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_D_Early register before PS_Timer_D time. */
#define BIT_PS_TIMER_C_EARLY_INT_EN            ((u32)0x00000001 << 21)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_C_Early register before PS_Timer_C time. */
#define BIT_PS_TIMER_B_EARLY_INT_EN            ((u32)0x00000001 << 20)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_B_Early register before PS_Timer_B time. */
#define BIT_PS_TIMER_A_EARLY_INT_EN            ((u32)0x00000001 << 19)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_A_Early register before PS_Timer_A time. */
#define BIT_TXBCNOK_NAN_INT_EN                 ((u32)0x00000001 << 18)          /*!<R/W 0  PORT2 TX beacon OK interrupt */
#define BIT_TXBCNERR_NAN_INT_EN                ((u32)0x00000001 << 17)          /*!<R/W 0  PORT2 TX beacon Error interrupt */
#define BIT_RXTMREQ_INT_EN                     ((u32)0x00000001 << 16)          /*!<R/W 0  If BIT_RXTMREQ_INT_EN =1. When HW receive TMREQ packet, it will generate interrupt to firmware. */
#define BIT_RXTM_INT_EN                        ((u32)0x00000001 << 15)          /*!<R/W 0  If BIT_RXTM_INT_EN =1. When HW receive TM and transmit ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_TXTM_INT_EN                        ((u32)0x00000001 << 14)          /*!<R/W 0  If BIT_TXTM_INT_EN =1. When HW transmit TM and receive ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_RX_PAYLOAD_MATCH_INT_EN            ((u32)0x00000001 << 13)          /*!<R/W 0  indicate rx payload content match */
#define BIT_CHINFO_SEG_DONE_INT_EN             ((u32)0x00000001 << 12)          /*!<R/W 0  indicate one channel info segment is received ok */
#define BIT_CSI_BUF_FULL_INT_EN                ((u32)0x00000001 << 11)          /*!<R/W 0  indicate channel info rxcsibuffer is full */
#define BIT_HS_RX_PHYSTS_DONE_INT_EN           ((u32)0x00000001 << 6)          /*!<R/W 0  indicate physts is successfully write into physts_mem */
#define BIT_HS_TIM_HIT_INT_EN                  ((u32)0x00000001 << 5)          /*!<R/W 0  when my aid or aid is 0 (0x16DC[8]) or my bssid is in tim element,generate this interrupt */
#define BIT_HS_DFS_PKT_INT_EN                  ((u32)0x00000001 << 4)          /*!<R/W 0  enable interrupt when receiving dfs pkt and about to reach threshold of given pkt number or timeout */
#define BIT_HS_BC_TWT_PARA_CHG_INT_EN          ((u32)0x00000001 << 3)          /*!<R/W 0  RX broadcast twt with parameter changing,notify fw to read twt report */
#define BIT_HS_BCN_IE_0_HIT_INT_EN             ((u32)0x00000001 << 2)          /*!<R/W 0  rx bcn with ie0 hit */
#define BIT_HS_BCN_IE_1_HIT_INT_EN             ((u32)0x00000001 << 1)          /*!<R/W 0  rx bcn with ie1 hit */
#define BIT_HS_TIM_NO_HIT_INT_EN               ((u32)0x00000001 << 0)          /*!<R/W 0  when no my aid or aid is 0(0x16DC[8]) or my bssid is not in tim element(0x16DB[31]),generate this interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HISR2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_KM0_INT                            ((u32)0x00000001 << 31)          /*!<RW1C 0  The gathering of interrupts of KM0 */
#define BIT_PS_TIMER_E_INT                     ((u32)0x00000001 << 25)          /*!<RW1C 0  PS_Timer_E interrupt When the TSF(that select by reg 0x1520[13:12]) matches the content of this register, the PS_Timer_E_INT interrupt is issued */
#define BIT_PS_TIMER_D_INT                     ((u32)0x00000001 << 24)          /*!<RW1C 0  PS_Timer_D interrupt When the lower 4 bytes of TSF(that select by reg 0x1520[17:16]) matches the content of this register, the PS_Timer_D_INT interrupt is issued */
#define BIT_PS_TIMER_E_EARLY_INT               ((u32)0x00000001 << 23)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_E_Early register before PS_Timer_E time. */
#define BIT_PS_TIMER_D_EARLY_INT               ((u32)0x00000001 << 22)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_D_Early register before PS_Timer_D time. */
#define BIT_PS_TIMER_C_EARLY_INT               ((u32)0x00000001 << 21)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_C_Early register before PS_Timer_C time. */
#define BIT_PS_TIMER_B_EARLY_INT               ((u32)0x00000001 << 20)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_B_Early register before PS_Timer_B time. */
#define BIT_PS_TIMER_A_EARLY_INT               ((u32)0x00000001 << 19)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_A_Early register before PS_Timer_A time. */
#define BIT_TXBCNOK_NAN_INT                    ((u32)0x00000001 << 18)          /*!<RW1C 0  PORT2 TX beacon OK interrupt */
#define BIT_TXBCNERR_NAN_INT                   ((u32)0x00000001 << 17)          /*!<RW1C 0  PORT2 TX beacon Error interrupt */
#define BIT_RXTMREQ_INT                        ((u32)0x00000001 << 16)          /*!<RW1C 0  If BIT_RXTMREQ_INT_EN =1. When HW receive TMREQ packet, it will generate interrupt to firmware. */
#define BIT_RXTM_INT                           ((u32)0x00000001 << 15)          /*!<RW1C 0  If BIT_RXTM_INT_EN =1. When HW receive TM and transmit ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_TXTM_INT                           ((u32)0x00000001 << 14)          /*!<RW1C 0  If BIT_TXTM_INT_EN =1. When HW transmit TM and receive ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_RX_PAYLOAD_MATCH_INT               ((u32)0x00000001 << 13)          /*!<RW1C 0  indicate rx payload content match */
#define BIT_CHINFO_SEG_DONE_INT                ((u32)0x00000001 << 12)          /*!<RW1C 0  indicate one channel info segment is received ok */
#define BIT_CSI_BUF_FULL_INT                   ((u32)0x00000001 << 11)          /*!<RW1C 0  indicate channel info rxcsibuffer is full */
#define BIT_HS_RX_PHYSTS_DONE_INT              ((u32)0x00000001 << 6)          /*!<RW1C 0  indicate physts is successfully write into physts_mem */
#define BIT_HS_TIM_HIT_INT                     ((u32)0x00000001 << 5)          /*!<RW1C 0  when my aid or aid is 0 (0x16DC[8]) or my bssid is in tim element,generate this interrupt */
#define BIT_HS_DFS_PKT_INT                     ((u32)0x00000001 << 4)          /*!<RW1C 0  enable interrupt when receiving dfs pkt and about to reach threshold of given pkt number or timeout */
#define BIT_HS_BC_TWT_PARA_CHG_INT             ((u32)0x00000001 << 3)          /*!<RW1C 0  RX broadcast twt with parameter changing,notify fw to read twt report */
#define BIT_HS_BCN_IE_0_HIT_INT                ((u32)0x00000001 << 2)          /*!<RW1C 0  rx bcn with ie0 hit */
#define BIT_HS_BCN_IE_1_HIT_INT                ((u32)0x00000001 << 1)          /*!<RW1C 0  rx bcn with ie1 hit */
#define BIT_HS_TIM_NO_HIT_INT                  ((u32)0x00000001 << 0)          /*!<RW1C 0  when no my aid or aid is 0(0x16DC[8]) or my bssid is not in tim element(0x16DB[31]),generate this interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_SDM_IDR_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SDM_ADDR                      ((u32)0x0000003F << 0)          /*!<R/W 0  SDM register offset */
#define BIT_SDM_ADDR(x)                        ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_SDM_ADDR(x)                    ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL_CK32KGEN_RWD
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_CK32GEN_RD                    ((u32)0xFFFFFFFF << 0)          /*!<RO 0   */
#define BIT_CK32GEN_RD(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_CK32GEN_RD(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SYS_CONFIG0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_AUX_IS_32K                         ((u32)0x00000001 << 9)          /*!<RO 0   */
#define BIT_IS_SCH_CLK_INSLP_V1                ((u32)0x00000001 << 8)          /*!<RO 0  WL TSF clock status: 0: MAC clock 1: 32K clock */
#define BIT_MASK_VENDOR_ID_V1                  ((u32)0x0000000F << 4)          /*!<RO 4'b1000  Vendor ID [7:6] : Download FW version ; 00: TSMC , 01: SMIC, 10: UMC [5] ; 0 : Download FW , init BB and RF registers 1 : Do not download FW and do not init BB and RF registers [4] ; 0 : BT do not exist in IC 1 : BT exist in IC */
#define BIT_VENDOR_ID_V1(x)                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_VENDOR_ID_V1(x)                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_PLL_CK_RDY                         ((u32)0x00000001 << 0)          /*!<RO 0  indicate AFE PLL clock is ready */
/** @} */


/*==========PAGE0 Register Address Definition==========*/
#define REG_SYS_PWC_ISO_CTRL                         0x0000
#define REG_WL_CLK_CTRL                              0x0002
#define REG_WL_FUNC_EN                               0x0004
#define REG_CLKDIV_CKSEL                             0x0008
#define REG_CPWM                                     0x000C
//#define REG_                                    	   0x0010
#define REG_RPWM                                     0x0014
//#define REG_                                    	   0x0018
#define REG_RSV_CTRL                                 0x001C
#define REG_WL_PMC_CTRL                              0x0020
#define REG_WL_SYSCONFIG                             0x0024
#define REG_P0_TBTT_TIMER                            0x0028
//#define REG_                                         0x002C
#define REG_AFC_CTRL0                                0x0030
#define REG_AFE_CTRL1_V1                             0x0034
#define REG_AFE_CTRL2_V1                             0x0038
#define REG_AFE_CTRL3_V1                             0x003C
#define REG_FSIMR_V1                                 0x0040
#define REG_FSISR_V1                                 0x0044
#define REG_TXDMA_H2C_RD_POINTER                     0x0048
#define REG_TXDMA_H2C_WR_POINTER                     0x004C
//#define REG_FW_DBG0_V1                               0x0050
//#define REG_FW_DBG1_V1                               0x0054
//#define REG_FW_DBG2_V1                               0x0058
//#define REG_FW_DBG3_V1                               0x005C
//#define REG_FW_DBG4_V1                               0x0060
//#define REG_FW_DBG5_V1                               0x0064
//#define REG_FW_DBG6_V1                               0x0068
//#define REG_FW_DBG7_V1                               0x006C//move debugrom
#define REG_WL_PIN_FUNC_CTRL0                        0x0070
#define REG_WL_BTCOEX_CTRL                           0x0074
#define REG_SCOREBOARD_RD_BT2WL                      0x0078
#define REG_DBG_PORT_REG                             0x007C
#define REG_WL_MCUFW_CTRL                            0x0080
//#define REG_BLK_TST                                  0x0084
//#define REG_HOST_MSG0                                0x0088
//#define REG_HOST_MSG1                                0x008C
#define REG_LPS_CTRL                                 0x0090
//#define REG_                                         0x0094
#define REG_BOOT_REASON                              0x0098
//#define REG_		                                   0x009C
#define REG_SCOREBOARD_CTRL                          0x00A0
#define REG_SYSON_FSM_MON_V1                         0x00A4
#define REG_PMC_DBG_CTRL1                            0x00A8
#define REG_PMC_DBG_CTRL2_V1                         0x00AC
#define REG_HIMR0                                    0x00B0
#define REG_HISR0                                    0x00B4
#define REG_HEMR                                     0x00B8
#define REG_HESR                                     0x00BC
#define REG_HIMR2                                    0x00C0
#define REG_HISR2                                    0x00C4
//#define REG_                                         0x00C8
//#define REG_                                         0x00CC
//#define REG_                                         0x00D0
//#define REG_                                         0x00D4
//#define REG_                                         0x00D8
//#define REG_                                         0x00DA
//#define REG_                                         0x00DC
//#define REG_                                         0x00E0
//#define REG_                                         0x00E4
#define REG_SDM_IDR_CTRL0                            0x00E8
#define REG_WL_CK32KGEN_RWD                          0x00EC
#define REG_SYS_CONFIG0                              0x00F0
//#define REG_                                         0x00F4
//#define REG_                                         0x00F8
//#define REG_                                         0x00FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
