#ifndef _AMEBAD2_PAGE17_H_
#define _AMEBAD2_PAGE17_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_RX_CFG
 * @brief rx configure register
 * @{
 *****************************************************************************/
#define BIT_R_VHTNDPA_SUAID_0_EN            ((u32)0x00000001 << 31)          /*!<R/W 0  when set, will receive vht/he su ndpa which aid field is set to 0. */
#define BIT_R_DIS_HE_SIGB_CHK               ((u32)0x00000001 << 30)          /*!<R/W 0  when set, disable check he-mu sigb crc8 */
#define BIT_R_MBA_RSVD_AID_EN               ((u32)0x00000001 << 29)          /*!<R/W 0  enable parsing resevd aid msta-ba for pre-association */
#define BIT_R_RXBCN_TIM_NO_HIT_EN           ((u32)0x00000001 << 28)          /*!<R/W 0  enable rxbcn_ok_int when receiving bcn with tim_no_hit,set 0: rxbcn_ok_int will only assert when tim_hit */
#define BIT_R_TIM_NO_HIT_REJECT             ((u32)0x00000001 << 27)          /*!<R/W 0  enable reject tim no hit beacon */
#define BIT_R_DIS_CHK_HENDP_LEN             ((u32)0x00000001 << 26)          /*!<R/W 0  disable check HE NDP lsig length value */
#define BIT_R_CHK_MGNT_BSSID                ((u32)0x00000001 << 25)          /*!<R/W 0  enable check management frame bssid */
#define BIT_R_CHK_CTL_BSSID                 ((u32)0x00000001 << 24)          /*!<R/W 0  enable check trigger,vht/hendpa and block ack bssid */
#define BIT_R_DIS_STS_GCLK                  ((u32)0x00000001 << 23)          /*!<R/W 0  disable ppdu status clock gating */
#define BIT_MASK_R_STS_CHKRX_COND           ((u32)0x0000000F << 19)          /*!<R/W 0  check macrx condition when receiving ppdu status */
#define BIT_R_STS_CHKRX_COND(x)             ((u32)(((x) & 0x0000000F) << 19))
#define BIT_GET_R_STS_CHKRX_COND(x)         ((u32)(((x >> 19) & 0x0000000F)))
#define BIT_R_INTER_PPDU_REJECT             ((u32)0x00000001 << 18)          /*!<R/W 0  enable reject inter ppdu */
#define BIT_R_EN_MACH_UPD_LEN               ((u32)0x00000001 << 17)          /*!<R/W 0  enable macrx_pktfifo_ctl update mpdu length in MACHDR_CHK state */
#define BIT_R_SNIFFER_TB                    ((u32)0x00000001 << 16)          /*!<R/W 0  enable to receive TB PPDU for debug mode */
#define BIT_R_RXPSF_INTER_PLCP              ((u32)0x00000001 << 6)          /*!<R/W 0  enable invalid packet check for inter ppdu, inter ppdu valid when plcp is inter bss */
#define BIT_R_RXPSF_INTER_MPDU              ((u32)0x00000001 << 5)          /*!<R/W 0  enable invalid packet check for inter ppdu, inter ppdu valid when plcp and mac header are inter bss */
#define BIT_R_RXPSF_HE_MU_CHKEN             ((u32)0x00000001 << 4)          /*!<R/W 0  enable HE MU invalid packet check for mac header */
#define BIT_R_RXPSF_HE_ERSU_CHKEN           ((u32)0x00000001 << 3)          /*!<R/W 0  enable HE ER SU invalid packet check for mac header */
#define BIT_R_RXPSF_HE_SU_CHKEN             ((u32)0x00000001 << 2)          /*!<R/W 0  enable HE SU invalid packet check for mac header */
#define BIT_R_RXPSF_VHT_MU_CHKEN            ((u32)0x00000001 << 1)          /*!<R/W 0  enable VHT MU invalid packet check for mac header */
#define BIT_R_RXPSF_VHT_SU_CHKEN            ((u32)0x00000001 << 0)          /*!<R/W 0  enable VHT SU invalid packet check for mac header */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RPL_CFG
 * @brief rx receive power level configure register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RPLCFG_UPPER_DBM         ((u32)0x000001FF << 11)          /*!<R/W 0  rx power upper level */
#define BIT_R_RPLCFG_UPPER_DBM(x)           ((u32)(((x) & 0x000001FF) << 11))
#define BIT_GET_R_RPLCFG_UPPER_DBM(x)       ((u32)(((x >> 11) & 0x000001FF)))
#define BIT_R_RPLCFG_UPPER_EN               ((u32)0x00000001 << 10)          /*!<R/W 0  enable use r_rplcfg_upper_dbm as rx power level */
#define BIT_MASK_R_RPLCFG_DBM               ((u32)0x000001FF << 1)          /*!<R/W 0  rx power level */
#define BIT_R_RPLCFG_DBM(x)                 ((u32)(((x) & 0x000001FF) << 1))
#define BIT_GET_R_RPLCFG_DBM(x)             ((u32)(((x >> 1) & 0x000001FF)))
#define BIT_R_RPLCFG_EN                     ((u32)0x00000001 << 0)          /*!<R/W 0  enable use r_rplcfg_dbm as rx power level */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AID
 * @brief aid register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MAC_AID12_1              ((u32)0x00000FFF << 12)          /*!<R/W 0  aid1 */
#define BIT_R_MAC_AID12_1(x)                ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_R_MAC_AID12_1(x)            ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_R_MAC_AID12_0              ((u32)0x00000FFF << 0)          /*!<R/W 0  aid0 */
#define BIT_R_MAC_AID12_0(x)                ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_R_MAC_AID12_0(x)            ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRAN_BSSID0_L
 * @brief trasmitted bssid register0_l
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_TRANSBSSID0_L       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  lower 32 bits of transmitted bssid0 */
#define BIT_R_WMAC_TRANSBSSID0_L(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_WMAC_TRANSBSSID0_L(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRAN_BSSID0_H
 * @brief trasmitted bssid register0_h
 * @{
 *****************************************************************************/
#define BIT_R_MBSSID_CTLINFO_SRCH_EN        ((u32)0x00000001 << 24)          /*!<R/W 0  when set, rx trigger and vht/he ndpa, will search control info using assocaiated bssid */
#define BIT_R_EN_PARTAID                    ((u32)0x00000001 << 23)          /*!<R/W 0  enable inter ppdu decision according to partial aid for VHT ppdu */
#define BIT_R_EN_MBSSID                     ((u32)0x00000001 << 22)          /*!<R/W 0  enable multibssid function */
#define BIT_MASK_R_BSS_COLOR0               ((u32)0x0000003F << 16)          /*!<R/W 0  bss color of current BSS */
#define BIT_R_BSS_COLOR0(x)                 ((u32)(((x) & 0x0000003F) << 16))
#define BIT_GET_R_BSS_COLOR0(x)             ((u32)(((x >> 16) & 0x0000003F)))
#define BIT_MASK_R_WMAC_TRANSBSSID0_H       ((u32)0x0000FFFF << 0)          /*!<R/W 0  higher 16 bits of transmitted bssid0 */
#define BIT_R_WMAC_TRANSBSSID0_H(x)         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_TRANSBSSID0_H(x)     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRAN_BSSID1_L
 * @brief trasmitted bssid register1_l
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_TRANSBSSID1_L       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  lower 32 bits of transmitted bssid1 */
#define BIT_R_WMAC_TRANSBSSID1_L(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_WMAC_TRANSBSSID1_L(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRAN_BSSID1_H
 * @brief trasmitted bssid register1_h
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BSS_COLOR1               ((u32)0x0000003F << 16)          /*!<R/W 0  bss color of current BSS */
#define BIT_R_BSS_COLOR1(x)                 ((u32)(((x) & 0x0000003F) << 16))
#define BIT_GET_R_BSS_COLOR1(x)             ((u32)(((x >> 16) & 0x0000003F)))
#define BIT_MASK_R_WMAC_TRANSBSSID1_H       ((u32)0x0000FFFF << 0)          /*!<R/W 0  higher 16 bits of transmitted bssid1 */
#define BIT_R_WMAC_TRANSBSSID1_H(x)         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_TRANSBSSID1_H(x)     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GID_POS_EN_l
 * @brief VHT group id position enable register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GID_POSITION_EN_L        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  VHT group id position enable */
#define BIT_R_GID_POSITION_EN_L(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_GID_POSITION_EN_L(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GID_POS0
 * @brief VHT group id position0 register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GID_POSITION0            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  VHT group id position0 */
#define BIT_R_GID_POSITION0(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_GID_POSITION0(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GID_POS1
 * @brief VHT group id position1 register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GID_POSITION1            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  VHT group id position1 */
#define BIT_R_GID_POSITION1(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_GID_POSITION1(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GID_POS2
 * @brief VHT group id position2 register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GID_POSITION2            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  VHT group id position2 */
#define BIT_R_GID_POSITION2(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_GID_POSITION2(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GID_POS3
 * @brief VHT group id position3 register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GID_POSITION3            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  VHT group id position3 */
#define BIT_R_GID_POSITION3(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_GID_POSITION3(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSCOLOR_BITMAP_L
 * @brief bss color bitmap register_l
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BSSCOLOR_BITMAP_L        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  low 32bits bss color bitmap */
#define BIT_R_BSSCOLOR_BITMAP_L(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_BSSCOLOR_BITMAP_L(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSCOLOR_BITMAP_H
 * @brief bss color bitmap register_h
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BSSCOLOR_BITMAP_H        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  high 32bits bss color bitmap */
#define BIT_R_BSSCOLOR_BITMAP_H(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_BSSCOLOR_BITMAP_H(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PBSSID_BITMAP_L
 * @brief partial bssid bitmap register_l
 * @{
 *****************************************************************************/
#define BIT_MASK_R_PARTIAL_BSSID_BITMAP_L   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  low 32bits partial bssid bitmap */
#define BIT_R_PARTIAL_BSSID_BITMAP_L(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_PARTIAL_BSSID_BITMAP_L(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PBSSID_BITMAP_H
 * @brief partial bssid bitmap register_h
 * @{
 *****************************************************************************/
#define BIT_MASK_R_PARTIAL_BSSID_BITMAP_H   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  high 32bits partial bssid bitmap */
#define BIT_R_PARTIAL_BSSID_BITMAP_H(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_PARTIAL_BSSID_BITMAP_H(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SR_CFG0
 * @brief spatial reuse configure register0
 * @{
 *****************************************************************************/
#define BIT_R_SPRUSE_RSVD_EN                ((u32)0x00000001 << 3)          /*!<R/W 0  enable spatial reuse when plcp header spatial reuse field value smaller than 14 or value is 15 and is srg ppdu */
#define BIT_R_CTL_SRG_CHECK_EN              ((u32)0x00000001 << 2)          /*!<R/W 0  enable control packet srg ppdu check */
#define BIT_R_SRG_CHECK_EN                  ((u32)0x00000001 << 1)          /*!<R/W 0  enable srg ppdu check */
#define BIT_R_SR_EN                         ((u32)0x00000001 << 0)          /*!<R/W 0  enable spatial reuse function */
/** @} */

/**************************************************************************//**
 * @defgroup REG_SR_CFG1
 * @brief spatial reuse configure register1
 * @{
 *****************************************************************************/
#define BIT_MASK_R_NONSRG_OBSS_PD_MAX       ((u32)0x000000FF << 24)          /*!<R/W 0  the maximum value of non_SRG OBSS_PD */
#define BIT_R_NONSRG_OBSS_PD_MAX(x)         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_NONSRG_OBSS_PD_MAX(x)     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_NONSRG_OBSS_PD_MIN       ((u32)0x000000FF << 16)          /*!<R/W 0  the minimum value of non_SRG OBSS_PD */
#define BIT_R_NONSRG_OBSS_PD_MIN(x)         ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_NONSRG_OBSS_PD_MIN(x)     ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_SRG_OBSS_PD_MAX          ((u32)0x000000FF << 8)          /*!<R/W 0  the maximum value of OBSS_PD */
#define BIT_R_SRG_OBSS_PD_MAX(x)            ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_SRG_OBSS_PD_MAX(x)        ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_SRG_OBSS_PD_MIN          ((u32)0x000000FF << 0)          /*!<R/W 0  the minimum value of OBSS_PD */
#define BIT_R_SRG_OBSS_PD_MIN(x)            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_SRG_OBSS_PD_MIN(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACTX_SR
 * @brief SR field configure
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_WAIT_PHY_REF        ((u32)0x0000003F << 18)          /*!<R/W 0x10  mactx hang handle, reset mactx if the phy_txen is set and wait the begin pulse of phy_txon timeout */
#define BIT_R_WMAC_WAIT_PHY_REF(x)          ((u32)(((x) & 0x0000003F) << 18))
#define BIT_GET_R_WMAC_WAIT_PHY_REF(x)      ((u32)(((x >> 18) & 0x0000003F)))
#define BIT_R_WMAC_SR_FIELD_EN              ((u32)0x00000001 << 17)          /*!<R/W 0  enable sr field reg cfg */
#define BIT_R_WMAC_SR_VAL15_ALLOW           ((u32)0x00000001 << 16)          /*!<R/W 0  sr field can be the value 15 */
#define BIT_MASK_R_WMAC_PLCP_SR_FIELD       ((u32)0x0000FFFF << 0)          /*!<R/W 0  value of SR field */
#define BIT_R_WMAC_PLCP_SR_FIELD(x)         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_PLCP_SR_FIELD(x)     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACTX_MISC
 * @brief misc ctrl cfg
 * @{
 *****************************************************************************/
#define BIT_R_TXTIME_CAL_DISGCLK            ((u32)0x00000001 << 30)          /*!<R/W 0  disable the clk gating for txtime compute module */
#define BIT_R_WMAC_CCK_LOCKED_CLK           ((u32)0x00000001 << 29)          /*!<R/W 0x1  indicate that the transmit frequency and symbol clocks are derived from the same oscillator */
#define BIT_MASK_R_WMAC_RST_BB_CNT          ((u32)0x00000007 << 26)          /*!<R/W 0x3  macclk cycle number of wmac_rst_bb signal */
#define BIT_R_WMAC_RST_BB_CNT(x)            ((u32)(((x) & 0x00000007) << 26))
#define BIT_GET_R_WMAC_RST_BB_CNT(x)        ((u32)(((x >> 26) & 0x00000007)))
#define BIT_MASK_R_TXDFIFO_INTERVAL_THRE    ((u32)0x00003FFF << 12)          /*!<R/W 0x5  the interval threshold between txdfifo write and read pointer, if the interval is smaller than the threshold, mactx_mpdu moudle can pad dummy between mpdu(if eanbled) */
#define BIT_R_TXDFIFO_INTERVAL_THRE(x)      ((u32)(((x) & 0x00003FFF) << 12))
#define BIT_GET_R_TXDFIFO_INTERVAL_THRE(x)  ((u32)(((x >> 12) & 0x00003FFF)))
#define BIT_SW_TXDFIFO_RDY                  ((u32)0x00000001 << 11)          /*!<R/W1C/ES 0  sw read txd fifo ready */
#define BIT_MASK_SW_TXDFIFO_ADDR            ((u32)0x0000007F << 4)          /*!<R/W 0  sw read txd fifo address,[6:1] is address, [0] : 0 read low 32bit; 1 read high 32bit; */
#define BIT_SW_TXDFIFO_ADDR(x)              ((u32)(((x) & 0x0000007F) << 4))
#define BIT_GET_SW_TXDFIFO_ADDR(x)          ((u32)(((x >> 4) & 0x0000007F)))
#define BIT_R_WMAC_FORCE_READ_TXDFIFO       ((u32)0x00000001 << 3)          /*!<R/W 0  during tx procedure, force read txd fifo, which is only use at the situation when bb is hang. */
#define BIT_R_WMAC_SW_RD                    ((u32)0x00000001 << 2)          /*!<R/W 0  eanble sw read txdfifo */
#define BIT_R_WMAC_MPDU_PADEN               ((u32)0x00000001 << 1)          /*!<R/W 0x1  when txdfifo underflow ,eanble the mactx_mpdu moudle to pad dummy between mpdu. */
#define BIT_R_EOFPAD_EN                     ((u32)0x00000001 << 0)          /*!<R/W 0x1  enable eof pad operation at the end of mpdu */
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACTX_MHDR0
 * @brief MAC header configure
 * @{
 *****************************************************************************/
#define BIT_R_QNULL_ACK_POL_VAL             ((u32)0x00000001 << 30)          /*!<R/W 1  basic trigger resp tb QOS NULL ack policy bit 0 value */
#define BIT_R_WMAC_EN_HW_SSN                ((u32)0x00000001 << 29)          /*!<R/W 0x1  enable the hw ssn for qos null frame. */
#define BIT_R_TB_QOSNULL_PWRMGT_VAL         ((u32)0x00000001 << 28)          /*!<R/W 0  the power management bit value in tb qos null, used during twt operation */
#define BIT_R_WMAC_MHDR_UPH_EN              ((u32)0x00000001 << 27)          /*!<R/W 0  eanble reg wirte HTC field of tb_ppdu, which contain UPH a-control(4) */
#define BIT_R_TCR_BA_RSVD_VAL               ((u32)0x00000001 << 26)          /*!<R/W 0  multi-sta BA bactl rsvd filed value */
#define BIT_MASK_R_QOS_NULL_TID_VAL         ((u32)0x0000000F << 22)          /*!<R/W 0  tid value of qos null */
#define BIT_R_QOS_NULL_TID_VAL(x)           ((u32)(((x) & 0x0000000F) << 22))
#define BIT_GET_R_QOS_NULL_TID_VAL(x)       ((u32)(((x >> 22) & 0x0000000F)))
#define BIT_MASK_R_WMAC_LEGACY_USTIMER      ((u32)0x0000001F << 17)          /*!<R/W 0xA  timer cnt before bb fetch txd for ofdm */
#define BIT_R_WMAC_LEGACY_USTIMER(x)        ((u32)(((x) & 0x0000001F) << 17))
#define BIT_GET_R_WMAC_LEGACY_USTIMER(x)    ((u32)(((x >> 17) & 0x0000001F)))
#define BIT_MASK_R_WMAC_NON_LEGACY_USTIMER  ((u32)0x0000001F << 12)          /*!<R/W 0xD  timer cnt before bb fetch txd for he/vht/ht/cck */
#define BIT_R_WMAC_NON_LEGACY_USTIMER(x)    ((u32)(((x) & 0x0000001F) << 12))
#define BIT_GET_R_WMAC_NON_LEGACY_USTIMER(x) ((u32)(((x >> 12) & 0x0000001F)))
#define BIT_MASK_R_BQR_PRIMCH               ((u32)0x00000007 << 9)          /*!<R/W 0  shift bqr bitmap to the allocatied primary 20M channel */
#define BIT_R_BQR_PRIMCH(x)                 ((u32)(((x) & 0x00000007) << 9))
#define BIT_GET_R_BQR_PRIMCH(x)             ((u32)(((x >> 9) & 0x00000007)))
#define BIT_MASK_R_BQR_TID_VAL              ((u32)0x0000000F << 5)          /*!<R/W 0  bandwidth query report TID value */
#define BIT_R_BQR_TID_VAL(x)                ((u32)(((x) & 0x0000000F) << 5))
#define BIT_GET_R_BQR_TID_VAL(x)            ((u32)(((x >> 5) & 0x0000000F)))
#define BIT_R_BSR_QOS_SEL                   ((u32)0x00000001 << 4)          /*!<R/W 0x1  Buffer status report enable */
#define BIT_R_BSR_VO_TID_SEL                ((u32)0x00000001 << 3)          /*!<R/W 0  1: tid 7; 0: tid 6; */
#define BIT_R_BSR_VI_TID_SEL                ((u32)0x00000001 << 2)          /*!<R/W 0  1: tid 5; 0: tid 4; */
#define BIT_R_BSR_BE_TID_SEL                ((u32)0x00000001 << 1)          /*!<R/W 0  1: tid 2; 0: tid 1; */
#define BIT_R_BSR_BK_TID_SEL                ((u32)0x00000001 << 0)          /*!<R/W 0  1: tid 3; 0: tid 0; */
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACTX_MHDR1
 * @brief MAC header HTC
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_NHDR_HTC            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  HTC field of tb_ppdu, which contain UPH a-control(4) */
#define BIT_R_WMAC_NHDR_HTC(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_WMAC_NHDR_HTC(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACTX_TXD_FIFO
 * @brief fifo do
 * @{
 *****************************************************************************/
#define BIT_MASK_SW_TXDFIFO_DOUT            ((u32)0xFFFFFFFF << 0)          /*!<R 0  sw read rxd_out */
#define BIT_SW_TXDFIFO_DOUT(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_SW_TXDFIFO_DOUT(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXTRIG_TEST_COMM_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXTRIG_COMMON_0            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  low 32bit of comm field in trigger frame of test mode */
#define BIT_RXTRIG_COMMON_0(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_RXTRIG_COMMON_0(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXTRIG_TEST_COMM_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXTRIG_COMMON_1            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  high32bit of comm field in trigger frame of test mode */
#define BIT_RXTRIG_COMMON_1(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_RXTRIG_COMMON_1(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXTRIG_TEST_USER_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXTRIG_USERINFO_0          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  low 32bit of user field in trigger frame of test mode */
#define BIT_RXTRIG_USERINFO_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_RXTRIG_USERINFO_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXTRIG_TEST_USER_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXTRIG_USERINFO_1          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  high 32bit of user field in trigger frame of test mode */
#define BIT_RXTRIG_USERINFO_1(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_RXTRIG_USERINFO_1(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXTRIG_TEST_USER_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXTRIG_BSS_COLOR           ((u32)0x0000003F << 26)          /*!<R/W 0x0  bss color */
#define BIT_RXTRIG_BSS_COLOR(x)             ((u32)(((x) & 0x0000003F) << 26))
#define BIT_GET_RXTRIG_BSS_COLOR(x)         ((u32)(((x >> 26) & 0x0000003F)))
#define BIT_MASK_RXTRIG_MACID               ((u32)0x0000007F << 19)          /*!<R/W 0x0  macid */
#define BIT_RXTRIG_MACID(x)                 ((u32)(((x) & 0x0000007F) << 19))
#define BIT_GET_RXTRIG_MACID(x)             ((u32)(((x >> 19) & 0x0000007F)))
#define BIT_RXTRIG_FCSCHK_EN                ((u32)0x00000001 << 18)          /*!<R/W 0x0  1:enable fcs check */
#define BIT_RXTRIG_PORT_SEL                 ((u32)0x00000001 << 17)          /*!<R/W 0x0  port sel */
#define BIT_RXTRIG_EN                       ((u32)0x00000001 << 16)          /*!<WA0 0x0  enable rx trigger test mode */
#define BIT_MASK_RXTRIG_USERINFO_2          ((u32)0x000000FF << 0)          /*!<R/W 0x0  msb 8bit of user field in trigger frame of test mode */
#define BIT_RXTRIG_USERINFO_2(x)            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RXTRIG_USERINFO_2(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXTRIG_TEST_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXTRIG_STATUS              ((u32)0x000000FF << 0)          /*!<R 0x0  [7:4]:trig_cntr [3:0]:ctl_state */
#define BIT_RXTRIG_STATUS(x)                ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RXTRIG_STATUS(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXTRIG_TEST_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXTRIG_DURATION            ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  duration of trigger frame */
#define BIT_RXTRIG_DURATION(x)              ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_RXTRIG_DURATION(x)          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_CLIENT_OM_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXTRIG_RU26_DIS                 ((u32)0x00000001 << 7)          /*!<R/W 0  disable response to trigger frame when ru_allocation addressed to me is 26-tone ru */
#define BIT_CSI_LEN_ERR_FLAG                ((u32)0x00000001 << 6)          /*!<R 0  csi_length error flag */
#define BIT_CLR_CSI_ERR_FLAG                ((u32)0x00000001 << 5)          /*!<R/W 0  clear csi_length error flag */
#define BIT_WMAC_RXTB_IGNOREA2              ((u32)0x00000001 << 4)          /*!<R/W 1  will not check a2 when receive tb ppdu */
#define BIT_MASK_UL_MU_DIS                  ((u32)0x00000003 << 2)          /*!<R/W 0  Disable to respond UL response triggered by any trigger frame. Per port config. */
#define BIT_UL_MU_DIS(x)                    ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_UL_MU_DIS(x)                ((u32)(((x >> 2) & 0x00000003)))
#define BIT_MASK_UL_MU_DATA_DIS             ((u32)0x00000003 << 0)          /*!<R/W 0  Disable to respond UL data triggered by basic trigger frame. Per port config. (BA/ACK/BSR/BQR are still allowed to Tx) */
#define BIT_UL_MU_DATA_DIS(x)               ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_UL_MU_DATA_DIS(x)           ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SIFS_TIMING_CTRL_CCK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_100NS_TIME            ((u32)0x000000FF << 24)          /*!<R/W 0x4  Sets the 100ns time unit used by MAC clock.(DEFAULT:40M CLK,eg：20M clk should set this value to 2) */
#define BIT_WMAC_100NS_TIME(x)              ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_WMAC_100NS_TIME(x)          ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R2T_SIFS_CCK               ((u32)0x00000FFF << 12)          /*!<R/W 0x47  This register sets the SIFS time required for any immediate response for example Data-Ack. It is in unit of 100ns.(default:7.1us) */
#define BIT_R2T_SIFS_CCK(x)                 ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_R2T_SIFS_CCK(x)             ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_T2T_SIFS_CCK               ((u32)0x00000FFF << 0)          /*!<R/W 0x47  This register sets the SIFS time required for consecutive TX events for example CTS-Data, or broadcast bursting. It is in unit of 100ns.(default:7.1us) */
#define BIT_T2T_SIFS_CCK(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_T2T_SIFS_CCK(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SIFS_TIMING_CTRL_OFDM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RX_AIR_END_TIME            ((u32)0x000000FF << 24)          /*!<R/W 0x3D  This register sets the time from cca fall to 6us. It is in unit of 100ns.(default:6.1us) */
#define BIT_RX_AIR_END_TIME(x)              ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_RX_AIR_END_TIME(x)          ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R2T_SIFS_OFDM              ((u32)0x00000FFF << 12)          /*!<R/W 0x83  This register sets the SIFS time required for any immediate response for example Data-Ack. It is in unit of 100ns.(default:13.1us) */
#define BIT_R2T_SIFS_OFDM(x)                ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_R2T_SIFS_OFDM(x)            ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_T2T_SIFS_OFDM              ((u32)0x00000FFF << 0)          /*!<R/W 0x83  This register sets the SIFS time required for consecutive TX events for example CTS-Data, or broadcast bursting. It is in unit of 100ns.(default:13.1us) */
#define BIT_T2T_SIFS_OFDM(x)                ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_T2T_SIFS_OFDM(x)            ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_TB_TIMING_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TB_TIMEOUT_REF             ((u32)0x00000FFF << 0)          /*!<R/W 0xB1  This register sets the max time for tb data aggragation, It is in unit of 100ns.(default:17.7us) */
#define BIT_TB_TIMEOUT_REF(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_TB_TIMEOUT_REF(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SR_CONTROL_DBG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXPPDU_SRG                      ((u32)0x00000001 << 27)          /*!<R 0  the received pkt is an srg ppdu */
#define BIT_SR_UPD_EN                       ((u32)0x00000001 << 26)          /*!<R 0  the received pkt is allowed to update basic nav */
#define BIT_SR_RESTRICTED                   ((u32)0x00000001 << 25)          /*!<R 0  the spatial reuse type is restricted */
#define BIT_MASK_SR_PD_TX                   ((u32)0x000001FF << 16)          /*!<R 0  the value of received power level minus obss_pd_min */
#define BIT_SR_PD_TX(x)                     ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_SR_PD_TX(x)                 ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_MASK_SR_TIMER_TX                ((u32)0x0000FFFF << 0)          /*!<R 0  time can be used for spatial reuse */
#define BIT_SR_TIMER_TX(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_SR_TIMER_TX(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SR_CONTROL_DBG1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SR_USABLE_TIMER_THRES      ((u32)0x000000FF << 0)          /*!<R/W 0x4  If received pkt can operate spatial reuse,the min threshold for sr_timer_tx. unit:256us(default:1024us) */
#define BIT_SR_USABLE_TIMER_THRES(x)        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_SR_USABLE_TIMER_THRES(x)    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RX_NAV_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WMAC_PLCP_UP_NAV_EN             ((u32)0x00000001 << 4)          /*!<R/W 0  1:enable using txop_dur in plcp header to update nav */
#define BIT_WMAC_TF_UP_NAV_EN               ((u32)0x00000001 << 3)          /*!<R/W 1  1:enable updating intra nav when receiving trigger frame */
#define BIT_SRCTRL_PLCP_EN                  ((u32)0x00000001 << 2)          /*!<R/W 0  1:use txop_dur as sr_tx_timer */
#define BIT_WMAC_INTRANAV_INTXOP            ((u32)0x00000001 << 1)          /*!<R/W 1  enable updating intra nav in my txop */
#define BIT_WMAC_BASICNAV_INTXOP            ((u32)0x00000001 << 0)          /*!<R/W 1  enable updating basic nav in my txop */
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAV_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_INTRA_NAV_UPD         ((u32)0x00000003 << 30)          /*!<R 0  intra nav updated by 10: plcp header 01: mac header */
#define BIT_WMAC_INTRA_NAV_UPD(x)           ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_WMAC_INTRA_NAV_UPD(x)       ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_WMAC_INTRA_NAV_DUR         ((u32)0x00003FFF << 16)          /*!<R 0  intra_nav */
#define BIT_WMAC_INTRA_NAV_DUR(x)           ((u32)(((x) & 0x00003FFF) << 16))
#define BIT_GET_WMAC_INTRA_NAV_DUR(x)       ((u32)(((x >> 16) & 0x00003FFF)))
#define BIT_MASK_WMAC_BASIC_NAV_UPD         ((u32)0x00000003 << 14)          /*!<R 0  inter nav updated by 10: plcp header 01: mac header */
#define BIT_WMAC_BASIC_NAV_UPD(x)           ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_WMAC_BASIC_NAV_UPD(x)       ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_WMAC_BASIC_NAV_DUR         ((u32)0x00003FFF << 0)          /*!<R 0  basic_nav */
#define BIT_WMAC_BASIC_NAV_DUR(x)           ((u32)(((x) & 0x00003FFF) << 0))
#define BIT_GET_WMAC_BASIC_NAV_DUR(x)       ((u32)(((x >> 0) & 0x00003FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RESP_CONTROL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RESP_CHK_TXNAV                  ((u32)0x00000001 << 31)          /*!<R/W 0  check txnav when need to response */
#define BIT_RESP_CHK_INTRA_NAV              ((u32)0x00000001 << 30)          /*!<R/W 1  check intra nav when need to response */
#define BIT_RESP_CHK_BASIC_NAV              ((u32)0x00000001 << 29)          /*!<R/W 1  check basic nav when need to response */
#define BIT_RESP_STBC_EN                    ((u32)0x00000001 << 28)          /*!<R/W 0  enable stbc when need to response */
#define BIT_RESP_HTC_EN                     ((u32)0x00000001 << 27)          /*!<R/W 0  enable appending htc when need to response */
#define BIT_RESP_RATE_DOWNGRADE             ((u32)0x00000001 << 26)          /*!<R/W 0  enable downgrade response rate from HE to VHT, VHT to HT, HE to HT when need to response */
#define BIT_RESP_DCM_EN                     ((u32)0x00000001 << 25)          /*!<R/W 1  enable using dcm need to response */
#define BIT_RESP_TBPPDU_CHK_PWR             ((u32)0x00000001 << 23)          /*!<R/W 1  check tb ppdu tx power when rx trigger, if txpwr_ctrl send signal of abort,will not reply with tb ppdu */
#define BIT_RESP_RXTRIG_CHK_INSIFS          ((u32)0x00000001 << 22)          /*!<R/W 1  check whether channel is idle during sifs, if csreq=1 in trigger frame, will not reply with tb ppdu */
#define BIT_RESP_CHK_EDCCA                  ((u32)0x00000001 << 21)          /*!<R/W 1  check edcca when response */
#define BIT_RESP_CHK_BTCCA                  ((u32)0x00000001 << 20)          /*!<R/W 0  check btcca when response */
#define BIT_RESP_CHK_SEC_CCA20              ((u32)0x00000001 << 19)          /*!<R/W 0  check sec 20 cca when response */
#define BIT_RESP_CHK_SEC_CCA40              ((u32)0x00000001 << 18)          /*!<R/W 0  check sec 40 cca when response */
#define BIT_RESP_CHK_SEC_CCA80              ((u32)0x00000001 << 17)          /*!<R/W 0  check sec 80 cca when response */
#define BIT_RESP_EARLY_LEG_SEC              ((u32)0x00000001 << 16)          /*!<R/W 1  when receive pkt of tkip or wep, While phyrxon down, mac_rxactive on. Ignore mac rx busy or physts_on, respond immediately. */
#define BIT_MASK_RESP_GI_LTF                ((u32)0x00000007 << 13)          /*!<R/W 0  5:1x+0.8 4:1x+1.6 3:2x+0.8 2:2x+1.6 1:4x+0.8 0:4x+3.2 not support 1x+1.6 when response */
#define BIT_RESP_GI_LTF(x)                  ((u32)(((x) & 0x00000007) << 13))
#define BIT_GET_RESP_GI_LTF(x)              ((u32)(((x >> 13) & 0x00000007)))
#define BIT_RESP_CHK_ZBCCA                  ((u32)0x00000001 << 12)          /*!<R/W 0  check zigbee cca when response */
#define BIT_RESP_CHK_EDCCA_PER20            ((u32)0x00000001 << 11)          /*!<R/W 0  check edcca_per20_bitmap_sifs[0] when response */
#define BIT_MASK_RESP_TXRATE                ((u32)0x000003FF << 0)          /*!<R/W 0x04  tx rate use to decide response rate */
#define BIT_RESP_TXRATE(x)                  ((u32)(((x) & 0x000003FF) << 0))
#define BIT_GET_RESP_TXRATE(x)              ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RESP_RATE_RRSR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RESP_RRSR_HE               ((u32)0x000000FF << 24)          /*!<R/W 1  Data Rate Control Table Register BitMap to indicate the response/protection/expected_response rate to use in this BSS. All management and control response frames should obey the rates set in the bitmap. Each bit is mapped as the following: HE 1SS MCS0~MCS7 Response rate must be less than or equal to the rate of the initiating frame. If none of the bits are set, no response is generated by Rtl8712. If both the legacy rates and HT rates are defined as Basic Rates, fill in only the legacy portion of the bitmap. If only HT rates are defined as Basic Rates, fill in both the legacy and HT portion of the bitmap. */
#define BIT_RESP_RRSR_HE(x)                 ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_RESP_RRSR_HE(x)             ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_RESP_RRSR_VHT              ((u32)0x000000FF << 16)          /*!<R/W 1  VHT 1SS MCS0~MCS7 */
#define BIT_RESP_RRSR_VHT(x)                ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_RESP_RRSR_VHT(x)            ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_RESP_RRSR_HT               ((u32)0x000000FF << 8)          /*!<R/W 1  HT 1SS MCS0~MCS7 */
#define BIT_RESP_RRSR_HT(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_RESP_RRSR_HT(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_RESP_RRSR_OFDM             ((u32)0x000000FF << 0)          /*!<R/W 1  {6M, 9M, 12M, 18M, 24M, 36M, 48M, 54M} */
#define BIT_RESP_RRSR_OFDM(x)               ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RESP_RRSR_OFDM(x)           ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RESP_RATE_RRSR_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RESP_RRSR_CCK              ((u32)0x0000000F << 4)          /*!<R/W 0x3  {1M, 2M, 5.5M, 11M } */
#define BIT_RESP_RRSR_CCK(x)                ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_RESP_RRSR_CCK(x)            ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_RESP_RATE_EN               ((u32)0x0000000F << 0)          /*!<R/W 0xf  [0]：enable respnse rate of CCK and OFDM [1]:enable respnse rate of HT [2]:enable respnse rate of VHT [3]:enable respnse rate of HE */
#define BIT_RESP_RATE_EN(x)                 ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_RESP_RATE_EN(x)             ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BEAMFORMING_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_HE_CSI_RATE           ((u32)0x000001FF << 23)          /*!<R/W 0x180  HE CSI init rate(HE mode+NSS + MCS) */
#define BIT_WMAC_HE_CSI_RATE(x)             ((u32)(((x) & 0x000001FF) << 23))
#define BIT_GET_WMAC_HE_CSI_RATE(x)         ((u32)(((x >> 23) & 0x000001FF)))
#define BIT_MASK_WMAC_VHT_CSI_RATE          ((u32)0x000001FF << 14)          /*!<R/W 0x100  VHT CSI init rate(VHT mode+ NSS + MCS) */
#define BIT_WMAC_VHT_CSI_RATE(x)            ((u32)(((x) & 0x000001FF) << 14))
#define BIT_GET_WMAC_VHT_CSI_RATE(x)        ((u32)(((x >> 14) & 0x000001FF)))
#define BIT_MASK_WMAC_HT_CSI_RATE           ((u32)0x000001FF << 5)          /*!<R/W 0x080  HT CSI init rate(HT mode + NSS + MCS) */
#define BIT_WMAC_HT_CSI_RATE(x)             ((u32)(((x) & 0x000001FF) << 5))
#define BIT_GET_WMAC_HT_CSI_RATE(x)         ((u32)(((x >> 5) & 0x000001FF)))
#define BIT_MASK_WMAC_MIMO_PARA_CNT         ((u32)0x00000007 << 2)          /*!<R/W 0x1  width of mimo_para_en,deafult:1(mac clk:20M,pluse width:100ns) */
#define BIT_WMAC_MIMO_PARA_CNT(x)           ((u32)(((x) & 0x00000007) << 2))
#define BIT_GET_WMAC_MIMO_PARA_CNT(x)       ((u32)(((x >> 2) & 0x00000007)))
#define BIT_WMAC_CSI_BFRP_STOPTX            ((u32)0x00000001 << 1)          /*!<R/W 0x1  stop tx when rx ndpa/ndp and wait for bfrp poll frame */
#define BIT_WMAC_HE_NDPA_EN                 ((u32)0x00000001 << 0)          /*!<R/W 1  Set to 1, MAC would inform BB of preparing to receive NDP if the received NDPA is valid. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_CSI_RRSR_BITMAP
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_CSI_RRSR_BITMAP       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x3  BitMap to indicate the CSI frame rate in this BSS. Each bit is mapped as the following: Bit[7:0] = OFDM {6M, 9M, 12M, 18M, 24M, 36M, 48M, 54M} Bit[15:8] = HT MCS0~MCS7 BIT[23:16] = VHT 1SS MCS0~7 bit[31:24] = HE 1SS MCS0~7 The modulation of response rate must be less than or equal to the rate of the initiating frame. If none of the bits are set, no response is generated by Rtl8712. If both legacy rates and HT rates are defined as Basic Rates, fill in only the legacy portion of the bitmap. If only HT rates are defined as Basic Rates, fill in both the legacy and HT portion of the bitmap. */
#define BIT_WMAC_CSI_RRSR_BITMAP(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_WMAC_CSI_RRSR_BITMAP(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LBK_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MACLBK_RDY_PERIOD        ((u32)0x00000FFF << 0)          /*!<R/W 0x12  tdrdy/rdrdy interval count */
#define BIT_R_MACLBK_RDY_PERIOD(x)          ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_R_MACLBK_RDY_PERIOD(x)      ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GID_POS_EN_H
 * @brief VHT group id position enable register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GID_POSITION_EN_H        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  VHT group id position enable */
#define BIT_R_GID_POSITION_EN_H(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_GID_POSITION_EN_H(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRX_PKTCNT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TXPKTCNT_RST                  ((u32)0x00000001 << 11)          /*!<R/W 0  Reset tx packet counter, write “one” to set the counter to zero */
#define BIT_R_TX_UDPKTCNT_SET               ((u32)0x00000001 << 10)          /*!<R/W 0  Set user define packet configure, write one setting, it is only valid for r_wmac_txpktcnt_sel (4'd0 ~ 4'd3) */
#define BIT_MASK_R_WMAC_TXPKTCNT_SEL        ((u32)0x0000000F << 6)          /*!<R/W 0  Tx packet counter report selection 4'd0: user define 0 counter 4'd1: user define 1 counter 4'd2: user define 2 counter 4'd3: user define 3 counter 4'd4: HT mpdu tx ok counter 4'd5: VHT SU mpdu tx ok counter 4'd6: HE SU mpdu tx ok counter 4'd7: HE TB mpdu tx ok counter 4'd8: CCK ppdu tx ok counter 4'd9: OFDM ppdu tx ok counter 4'd10: HT ppdu tx ok counter 4'd11: VHT SU ppdu tx ok counter 4'd12: HE SU ppdu tx ok counter 4'd13: HE TB ppdu tx ok counter 4'd14~4'd15: RSVD */
#define BIT_R_WMAC_TXPKTCNT_SEL(x)          ((u32)(((x) & 0x0000000F) << 6))
#define BIT_GET_R_WMAC_TXPKTCNT_SEL(x)      ((u32)(((x >> 6) & 0x0000000F)))
#define BIT_MASK_R_WMAC_TXPKTCNT_CFG        ((u32)0x0000003F << 0)          /*!<R/W 0  Tx packet conuter user define configuration [5:2]:subtype [1:0]:type */
#define BIT_R_WMAC_TXPKTCNT_CFG(x)          ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_WMAC_TXPKTCNT_CFG(x)      ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRX_PKTCNT_RPT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RO_WMAC_TXPKTCNT_RPT       ((u32)0x03FFFFFF << 0)          /*!<R 0  [25:22]:Tx packet counter report selection,r_wmac_txpktcnt_sel [21:16]:user define->tx packet type,[21:18]:subtype,[17:16]:type; others->6'h0 [15:0]:The tx reported counter value corresponding to Report type selected */
#define BIT_RO_WMAC_TXPKTCNT_RPT(x)         ((u32)(((x) & 0x03FFFFFF) << 0))
#define BIT_GET_RO_WMAC_TXPKTCNT_RPT(x)     ((u32)(((x >> 0) & 0x03FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BEAMFORMING_MIMO_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RO_WMAC_HE_CSI_MIMO_H      ((u32)0x00FFFFFF << 0)          /*!<R 0  msb 24bit of he csi mimo field */
#define BIT_RO_WMAC_HE_CSI_MIMO_H(x)        ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_RO_WMAC_HE_CSI_MIMO_H(x)    ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PPDU_STATUS_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_STSBUF_ENDADDR           ((u32)0x000000FF << 4)          /*!<R/W 0xFF  ppdu status buffer end address */
#define BIT_R_STSBUF_ENDADDR(x)             ((u32)(((x) & 0x000000FF) << 4))
#define BIT_GET_R_STSBUF_ENDADDR(x)         ((u32)(((x >> 4) & 0x000000FF)))
#define BIT_MASK_R_STSBUF_THRESHOD          ((u32)0x0000000F << 0)          /*!<R/W 0x4  ppdu status buffer almost full threshold */
#define BIT_R_STSBUF_THRESHOD(x)            ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_STSBUF_THRESHOD(x)        ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RESP_CONTROL_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WMAC_TXRAW_IGNOREA1             ((u32)0x00000001 << 17)          /*!<R/W 1  If set to 1, will not check A1 of rxpkt when txraw */
#define BIT_RESP_ACTRL_BQR_EN               ((u32)0x00000001 << 16)          /*!<R/W 1  If set to 1, HW would update BQR info to A-CTRL */
#define BIT_RESP_ACTRL_UPH_EN               ((u32)0x00000001 << 15)          /*!<R/W 1  If set to 1, HW would update UPH info to A-CTRL */
#define BIT_RESP_ACTRL_BSR_EN               ((u32)0x00000001 << 14)          /*!<R/W 1  If set to 1, HW would update BSR info to A-CTRL */
#define BIT_RESP_ACTRL_CAS_EN               ((u32)0x00000001 << 13)          /*!<R/W 1  If set to 1, HW would update CAS info to A-CTRL */
#define BIT_RESP_LDPC_SEL                   ((u32)0x00000001 << 12)          /*!<R/W 1  1: follow ctrl reg(0x668[15]) 0: follow rxpkt */
#define BIT_RESP_GID_SEL                    ((u32)0x00000001 << 11)          /*!<R/W 1  HE SIG-A content 1 : group id=0, 0 : group id=63 */
#define BIT_MASK_RESP_DOPPLER_CTRL          ((u32)0x00000003 << 9)          /*!<R/W 0  0: Doppler unsupported 1: Doppler supported without moving 2: 10 symbol midamble periodicity 3: 20 symbol midamble periodicity */
#define BIT_RESP_DOPPLER_CTRL(x)            ((u32)(((x) & 0x00000003) << 9))
#define BIT_GET_RESP_DOPPLER_CTRL(x)        ((u32)(((x >> 9) & 0x00000003)))
#define BIT_RESP_DOPPLER_SEL                ((u32)0x00000001 << 8)          /*!<R/W 0  1: follow ctrl reg(0x17E0[10]) 0: follow rxpkt */
#define BIT_RESP_DCM_SEL                    ((u32)0x00000001 << 7)          /*!<R/W 0  1: follow ctrl reg(0x17b4[25]) 0: follow rxpkt */
#define BIT_RESP_BSR_FORMAT                 ((u32)0x00000001 << 6)          /*!<R/W 0  0: legacy format of queue size 1: HE */
#define BIT_MASK_RESP_NOMINAL_PAD           ((u32)0x00000003 << 4)          /*!<R/W 0  nominal pkt pad info when response */
#define BIT_RESP_NOMINAL_PAD(x)             ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_RESP_NOMINAL_PAD(x)         ((u32)(((x >> 4) & 0x00000003)))
#define BIT_RESP_EARLY_START                ((u32)0x00000001 << 3)          /*!<R/W 1  when set to 1:use mac header rdy when rx data frame and use aid fit when rx trigger frame, and use fcsok otherwise to start responding process when set to 0:use fcs ok to start responding process when rx any frame need reply */
#define BIT_RESP_USE_EARLY_FCS              ((u32)0x00000001 << 2)          /*!<R/W 1  use fcs result from rxpktfifo instead of macrx_parser when receive packet used wep/tkip */
#define BIT_RESP_ALLACK_EN                  ((u32)0x00000001 << 1)          /*!<R/W 0  enable response with all-ack MBA */
/** @} */

/**************************************************************************//**
 * @defgroup REG_SPATIAL_REUSE_CONTROL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXPWR_CH20_DATA            ((u32)0x000000FF << 1)          /*!<R/W 0  indicate which 20M channel is punctured in RU */
#define BIT_TXPWR_CH20_DATA(x)              ((u32)(((x) & 0x000000FF) << 1))
#define BIT_GET_TXPWR_CH20_DATA(x)          ((u32)(((x >> 1) & 0x000000FF)))
#define BIT_TXPWR_FAST_MODE_EN              ((u32)0x00000001 << 0)          /*!<R/W 1  fast mode enable for txpwr_ctrl,tie 1 for AMEBAD2 */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RX_SUPPER_ADDRESS3_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_SUP_ADDR3_L              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Address3 for data packet check. When address3 is match, RX will receive the packet to RXFF without considering other filter condition except crc32 */
#define BIT_R_SUP_ADDR3_L(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_SUP_ADDR3_L(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RX_SUPPER_ADDRESS3_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_SUP_ADDR3_EN                  ((u32)0x00000001 << 16)          /*!<R/W 0  when set, enable check supper Address3 */
#define BIT_MASK_R_SUP_ADDR3_H              ((u32)0x0000FFFF << 0)          /*!<R/W 0  Address3 for data packet check. When address3 is match, RX will receive the mpdu to RXFF without considering other filter condition except crc32 */
#define BIT_R_SUP_ADDR3_H(x)                ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_SUP_ADDR3_H(x)            ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MASK_LA_TRX_DBG_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LA_TRX_PATTERN_MASK_L      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  used for mask one or more bits of debug port */
#define BIT_LA_TRX_PATTERN_MASK_L(x)        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_LA_TRX_PATTERN_MASK_L(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MASK_LA_TRX_DBG_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LA_TRX_PATTERN_MASK_H      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  used for mask one or more bits of debug port */
#define BIT_LA_TRX_PATTERN_MASK_H(x)        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_LA_TRX_PATTERN_MASK_H(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_REF_LA_TRX_DBG_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LA_TRX_PATTERN_REF_L       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  used for mask one or more bits of debug port */
#define BIT_LA_TRX_PATTERN_REF_L(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_LA_TRX_PATTERN_REF_L(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_REF_LA_TRX_DBG_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LA_TRX_PATTERN_REF_H       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  used for mask one or more bits of debug port */
#define BIT_LA_TRX_PATTERN_REF_H(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_LA_TRX_PATTERN_REF_H(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */


/*==========PAGE17 Register Address Definition==========*/
#define REG_RX_CFG                                   0x1710
#define REG_RPL_CFG                                  0x1714
#define REG_AID                                      0x1718
#define REG_TRAN_BSSID0_L                            0x171C
#define REG_TRAN_BSSID0_H                            0x1720
#define REG_TRAN_BSSID1_L                            0x1724
#define REG_TRAN_BSSID1_H                            0x1728
#define REG_GID_POS_EN_L                             0x172C
#define REG_GID_POS0                                 0x1730
#define REG_GID_POS1                                 0x1734
#define REG_GID_POS2                                 0x1738
#define REG_GID_POS3                                 0x173C
#define REG_BSSCOLOR_BITMAP_L                        0x1740
#define REG_BSSCOLOR_BITMAP_H                        0x1744
#define REG_PBSSID_BITMAP_L                          0x1748
#define REG_PBSSID_BITMAP_H                          0x174C
#define REG_SR_CFG0                                  0x1750
#define REG_SR_CFG1                                  0x1754
#define REG_MACTX_SR                                 0x1758
#define REG_MACTX_MISC                               0x175C
#define REG_MACTX_MHDR0                              0x1760
#define REG_MACTX_MHDR1                              0x1764
#define REG_MACTX_TXD_FIFO                           0x1768
#define REG_RXTRIG_TEST_COMM_0                       0x1780
#define REG_RXTRIG_TEST_COMM_1                       0x1784
#define REG_RXTRIG_TEST_USER_0                       0x1788
#define REG_RXTRIG_TEST_USER_1                       0x178C
#define REG_RXTRIG_TEST_USER_2                       0x1790
#define REG_RXTRIG_TEST_CTRL1                        0x1794
#define REG_RXTRIG_TEST_CTRL2                        0x1795
#define REG_AX_CLIENT_OM_CTRL                        0x1797
#define REG_SIFS_TIMING_CTRL_CCK                     0x1798
#define REG_SIFS_TIMING_CTRL_OFDM                    0x179C
#define REG_AX_TB_TIMING_CTRL                        0x17A0
#define REG_SR_CONTROL_DBG                           0x17A4
#define REG_SR_CONTROL_DBG1                          0x17A8
#define REG_RX_NAV_CTRL                              0x17A9
#define REG_NAV_STATUS                               0x17B0
#define REG_RESP_CONTROL                             0x17B4
#define REG_RESP_RATE_RRSR                           0x17B8
#define REG_RESP_RATE_RRSR_1                         0x17BC
#define REG_BEAMFORMING_CTRL                         0x17C0
#define REG_CSI_RRSR_BITMAP                          0x17C4
#define REG_LBK_CFG                                  0x17C8
#define REG_GID_POS_EN_H                             0x17CC
#define REG_TRX_PKTCNT_CTRL                          0x17D0
#define REG_TRX_PKTCNT_RPT                           0x17D4
#define REG_BEAMFORMING_MIMO_STATUS                  0x17D8
#define REG_PPDU_STATUS_CFG                          0x17DC
#define REG_RESP_CONTROL_1                           0x17E0
#define REG_SPATIAL_REUSE_CONTROL                    0x17E4
#define REG_RX_SUPPER_ADDRESS3_L                     0x17E8
#define REG_RX_SUPPER_ADDRESS3_H                     0x17EC
#define REG_MASK_LA_TRX_DBG_L                        0x17F0
#define REG_MASK_LA_TRX_DBG_H                        0x17F4
#define REG_REF_LA_TRX_DBG_L                         0x17F8
#define REG_REF_LA_TRX_DBG_H                         0x17FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
