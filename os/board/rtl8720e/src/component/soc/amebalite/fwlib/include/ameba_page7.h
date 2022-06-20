#ifndef _AMEBAD2_PAGE7_H_
#define _AMEBAD2_PAGE7_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_MACID1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID1_0                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  MAC Identification Number Register for port 1 */
#define BIT_MACID1_0(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID1_0(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID1_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID1_1                         ((u32)0x0000FFFF << 0)          /*!<R/W 0  MAC Identification Number Register for port 1 */
#define BIT_MACID1_1(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MACID1_1(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID1_0                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Basic Station Set Identification for port 1. */
#define BIT_BSSID1_0(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BSSID1_0(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID1_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID1_1                         ((u32)0x0000FFFF << 0)          /*!<R/W 0  Basic Station Set Identification for port 1. */
#define BIT_BSSID1_1(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BSSID1_1(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_PSR_RPT1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DTIM_CNT1                        ((u32)0x000000FF << 24)          /*!<R 0  DTIM count */
#define BIT_DTIM_CNT1(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_DTIM_CNT1(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DTIM_PERIOD1                     ((u32)0x000000FF << 16)          /*!<R 0  DTIM Period */
#define BIT_DTIM_PERIOD1(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_DTIM_PERIOD1(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_TIM_HAS_AID                           ((u32)0x00000001 << 15)          /*!<R 0  indicate BC/MC packets are buffered at the AP or transimited BSSID */
#define BIT_TIM_HAS_MY_BSSID                      ((u32)0x00000001 << 14)          /*!<R 0  indicate BC/MC packets are buffered at nontransmitted BSSID, sw need to check corresponding dtim count field in multiple bssid index field */
#define BIT_TIM_HAS_MYAID                         ((u32)0x00000001 << 13)          /*!<R 0  my aid in the virtual bitmap is set */
#define BIT_TIM_VALID                             ((u32)0x00000001 << 12)          /*!<R 0  when set to 1 , indicate tim report valid */
#define BIT_MASK_PS_AID_1                         ((u32)0x000007FF << 0)          /*!<R/W 0  AID1 for MACID1 */
#define BIT_PS_AID_1(x)                           ((u32)(((x) & 0x000007FF) << 0))
#define BIT_GET_PS_AID_1(x)                       ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ASSOCIATED_BFMEE_SEL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BFMEE1_EN                             ((u32)0x00000001 << 25)          /*!<R/W 0  If enabled, NDPA would be sent to beamformee1. */
#define BIT_MASK_BFMEE1_AID                       ((u32)0x000001FF << 16)          /*!<R/W 0  P_AID of the target beamformee1 */
#define BIT_BFMEE1_AID(x)                         ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_BFMEE1_AID(x)                     ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_BFMEE0_EN                             ((u32)0x00000001 << 9)          /*!<R/W 0  If enabled, NDPA would be sent to beamformee0. */
#define BIT_MASK_BFMEE0_AID                       ((u32)0x000001FF << 0)          /*!<R/W 0  P_AID of the target beamformee0 */
#define BIT_BFMEE0_AID(x)                         ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_BFMEE0_AID(x)                     ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SND_PTCL_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NDP_RX_STANDBY_TIMER             ((u32)0x000000FF << 24)          /*!<R/W 0x42  A timeout value for MAC to leave NDP_Rx_standby_state and go back to normal Rx mode. (unit: 2us) */
#define BIT_NDP_RX_STANDBY_TIMER(x)               ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_NDP_RX_STANDBY_TIMER(x)           ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_R_WMAC_CHK_RPTPOLL_A2_DIS             ((u32)0x00000001 << 23)          /*!<R/W 0  1: For any A1-matched BF Rpt Poll, send back VHT-CB 0: If A1-matched BF Rpt Poll's A2 fails to match the previous receiving NDPA's A2, do not send VHT-CB */
#define BIT_R_WMAC_CHK_UCNDPA_A2_DIS              ((u32)0x00000001 << 22)          /*!<R/W 0  0: If A1-matched NDPA's A2 fails to match 0x6E4 or 0x6EC, use default csi parameter 1: For any receiving A1-matched NDPA, use 0x6F4[11:0] as csi parameter */
#define BIT_MASK_R_WMAC_HT_CATEGORY               ((u32)0x0000003F << 16)          /*!<R/W 0  csi report offset for ht mode */
#define BIT_R_WMAC_HT_CATEGORY(x)                 ((u32)(((x) & 0x0000003F) << 16))
#define BIT_GET_R_WMAC_HT_CATEGORY(x)             ((u32)(((x >> 16) & 0x0000003F)))
#define BIT_R_WMAC_OFFSET_RPTPOLL_EN              ((u32)0x00000001 << 15)          /*!<R/W 0  1: If previous Tx pkt is BF Rpt Poll or VHT NDP, use vht csi start offset; otherwise ht csi start offset 0: If previous Tx pkt is vht rate, use vht csi start offset; otherwise ht csi start offset */
#define BIT_R_WMAC_CSI_CHKSUM_DIS                 ((u32)0x00000001 << 14)          /*!<R/W 0  1:Disable check 16bit checksum value of csi report from BB 0:Enable check 16bit checksum value of csi report from BB */
#define BIT_MASK_R_WMAC_VHT_CATEGORY              ((u32)0x0000003F << 8)          /*!<R/W 0x02  used as csi report offset */
#define BIT_R_WMAC_VHT_CATEGORY(x)                ((u32)(((x) & 0x0000003F) << 8))
#define BIT_GET_R_WMAC_VHT_CATEGORY(x)            ((u32)(((x >> 8) & 0x0000003F)))
#define BIT_R_WMAC_USE_NSTS                       ((u32)0x00000001 << 7)          /*!<R/W 1  1: use Nsts as Nr, 0: use register value as Nr */
#define BIT_R_DISABLE_CHECK_VHTSIGB_CRC           ((u32)0x00000001 << 6)          /*!<R/W 0  1: disable, 0: enable */
#define BIT_R_DISABLE_CHECK_VHTSIGA_CRC           ((u32)0x00000001 << 5)          /*!<R/W 0  1: disable, 0: enable */
#define BIT_R_WMAC_BFPARAM_SEL                    ((u32)0x00000001 << 4)          /*!<R/W 0  1: use beamforming nc/nr/ng parameter from register 0x6F4 0: use beamforming nc/nr/ng parameter from tx report buffer */
#define BIT_R_WMAC_CSISEQ_SEL                     ((u32)0x00000001 << 3)          /*!<R/W 0  By which the sequence number of CSI frame is controlled 1: non-QoS SEQ number 0: counter */
#define BIT_R_WMAC_CSI_WITHHTC_EN                 ((u32)0x00000001 << 2)          /*!<R/W 0  Set to 1, CSI frame would carry HTC in MAC header. */
#define BIT_R_WMAC_HT_NDPA_EN                     ((u32)0x00000001 << 1)          /*!<R/W 0  Set to 1, MAC would inform BB of preparing to receive NDP if the received NDP Announcement MPDU is valid. */
#define BIT_R_WMAC_VHT_NDPA_EN                    ((u32)0x00000001 << 0)          /*!<R/W 0  Set to 1, MAC would inform BB of preparing to receive NDP if the received NDPA is valid. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BEAMFORM_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WRITE_ENABLE                          ((u32)0x00000001 << 31)          /*!<R 0  Access flag. '0' for read. ‘1’ for write, 0x72C[15:0] must be sett before this bit is set */
#define BIT_WMAC_CHECK_SOUNDING_SEQ               ((u32)0x00000001 << 30)          /*!<R/W 0  Set to 1,MAC would check sounding sequence number of received CSI report and transmitted NDPA */
#define BIT_MASK_VHTHT_MIMO_CTRL_FIELD            ((u32)0x00FFFFFF << 1)          /*!<R 0  when 'read': [24:1]: currently received VHT MIMO Control field or [16:1]: currently received HT MIMO Control field */
#define BIT_VHTHT_MIMO_CTRL_FIELD(x)              ((u32)(((x) & 0x00FFFFFF) << 1))
#define BIT_GET_VHTHT_MIMO_CTRL_FIELD(x)          ((u32)(((x >> 1) & 0x00FFFFFF)))
#define BIT_CSI_INTRRUPT_STATUS                   ((u32)0x00000001 << 0)          /*!<R 0  sounding status: '0’: fail; ‘1’: success; */
/** @} */

/**************************************************************************//**
 * @defgroup REG_NSARP_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_NSARP_RESPEN                          ((u32)0x00000001 << 15)          /*!<R/W 0  NAARP parser enable */
#define BIT_NSARP_PARP                            ((u32)0x00000001 << 9)          /*!<R/W 0  when set to 1, reject ARP */
#define BIT_NSARP_RIPV6                           ((u32)0x00000001 << 8)          /*!<R/W 0  when set to 1, reject IPV6 */
#define BIT_MASK_NSARP_MODEN                      ((u32)0x00000003 << 6)          /*!<R/W 0  r_WMAC_NSARP_MODEN[0]: when set to 1, enable arp request; r_WMAC_NSARP_MODEN[1]: when set to 1, enable ipv6 request; */
#define BIT_NSARP_MODEN(x)                        ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_NSARP_MODEN(x)                    ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_NSARP_RSPFTP                     ((u32)0x00000003 << 4)          /*!<R/W 0  used to select Response_type and TID */
#define BIT_NSARP_RSPFTP(x)                       ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_NSARP_RSPFTP(x)                   ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_NSARP_RSPSEC                     ((u32)0x0000000F << 0)          /*!<R/W 0  select response packet security type */
#define BIT_NSARP_RSPSEC(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_NSARP_RSPSEC(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ZB_STATISTIC_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZB_HI_RX                         ((u32)0x0000FFFF << 16)          /*!<R   for statistic */
#define BIT_ZB_HI_RX(x)                           ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_ZB_HI_RX(x)                       ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_ZB_HI_TX                         ((u32)0x0000FFFF << 0)          /*!<R    */
#define BIT_ZB_HI_TX(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_ZB_HI_TX(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ZB_STATISTIC_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZB_LO_RX                         ((u32)0x0000FFFF << 16)          /*!<R   for statistic */
#define BIT_ZB_LO_RX(x)                           ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_ZB_LO_RX(x)                       ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_ZB_LO_TX                         ((u32)0x0000FFFF << 0)          /*!<R    */
#define BIT_ZB_LO_TX(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_ZB_LO_TX(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BEAMFORMING_INFO_NSARP
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_UPD_BFMEE_USERID                 ((u32)0x00000007 << 13)          /*!<R/W 0  Select BFMEE User ID */
#define BIT_UPD_BFMEE_USERID(x)                   ((u32)(((x) & 0x00000007) << 13))
#define BIT_GET_UPD_BFMEE_USERID(x)               ((u32)(((x >> 13) & 0x00000007)))
#define BIT_UPD_BFMEE_FBTP                        ((u32)0x00000001 << 12)          /*!<R/W 0  Feedback Type(B11) of VHT MIMO Control field, HT is no used */
#define BIT_MASK_UPD_BFMEE_BW                     ((u32)0x00000003 << 10)          /*!<R/W 0  Channel Width of VHT MIMO control field (B7-6) and (B4) when HT mode */
#define BIT_UPD_BFMEE_BW(x)                       ((u32)(((x) & 0x00000003) << 10))
#define BIT_GET_UPD_BFMEE_BW(x)                   ((u32)(((x >> 10) & 0x00000003)))
#define BIT_MASK_UPD_BFMEE_CB                     ((u32)0x00000003 << 8)          /*!<R/W 0  Codebook Information of MIMO control field [8] for VHT (B10) and (B10-9) when HT mode */
#define BIT_UPD_BFMEE_CB(x)                       ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_UPD_BFMEE_CB(x)                   ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_UPD_BFMEE_NG                     ((u32)0x00000003 << 6)          /*!<R/W 0  Grouping of MIMO control field [7:6] for VHT (B9-8) and (B6-5) when HT mode */
#define BIT_UPD_BFMEE_NG(x)                       ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_UPD_BFMEE_NG(x)                   ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_UPD_BFMEE_NR                     ((u32)0x00000007 << 3)          /*!<R/W 0  Nr Index of MIMO control field [5:3] for VHT (B5-3) ,[4:3] for HT mode (B3-2) */
#define BIT_UPD_BFMEE_NR(x)                       ((u32)(((x) & 0x00000007) << 3))
#define BIT_GET_UPD_BFMEE_NR(x)                   ((u32)(((x >> 3) & 0x00000007)))
#define BIT_MASK_UPD_BFMEE_NC                     ((u32)0x00000007 << 0)          /*!<R/W 0  Nc Index of MIMO control field [2:0] for VHT (B2-0) , [1:0] for HT mode (B1-0) */
#define BIT_UPD_BFMEE_NC(x)                       ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_UPD_BFMEE_NC(x)                   ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_IPV6_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_IPV6_MYIPAD_0                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_IPV6_MYIPAD_0(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_IPV6_MYIPAD_0(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_IPV6_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_IPV6_MYIPAD_1                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_IPV6_MYIPAD_1(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_IPV6_MYIPAD_1(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_IPV6_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_IPV6_MYIPAD_2                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_IPV6_MYIPAD_2(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_IPV6_MYIPAD_2(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_IPV6_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_IPV6_MYIPAD_3                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_IPV6_MYIPAD_3(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_IPV6_MYIPAD_3(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ZB_COEX_TABLE_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZB_COEX_TABLE_1                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  coexistence table between WLAN and Zigbee, selected by BIT_COEX_TABLE_SEL = 0 */
#define BIT_ZB_COEX_TABLE_1(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_ZB_COEX_TABLE_1(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ZB_COEX_TABLE_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZB_COEX_TABLE_2                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  coexistence table between WLAN and Zigbee, selected by BIT_COEX_TABLE_SEL = 1 */
#define BIT_ZB_COEX_TABLE_2(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_ZB_COEX_TABLE_2(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ZB_BREAK_TABLE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZB_BREAK_TABLE_2                 ((u32)0x0000FFFF << 16)          /*!<R/W 0  Table used to break ZB activity, when new WLAN request is coming and ZB is on transmitting or receiving */
#define BIT_ZB_BREAK_TABLE_2(x)                   ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_ZB_BREAK_TABLE_2(x)               ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_ZB_BREAK_TABLE_1                 ((u32)0x0000FFFF << 0)          /*!<R/W 0  Table used to break WLAN activity, when new ZB request is coming and WLAN is on transmitting or receiving */
#define BIT_ZB_BREAK_TABLE_1(x)                   ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_ZB_BREAK_TABLE_1(x)               ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EXT_ZB_DETEC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZB_TRX_DETECT_TIME               ((u32)0x000000FF << 24)          /*!<R/W 0x40  Valid if BIT_ZIGBEE_MODE = 1 & BIT_EXT_ZIGBEE_MODE = 0 (external mode, directional PRIORITY) After ZB_PRI_DETECT_TIME duration, if ZB_PRIORITY is low or goes low during ZB_TRX_INIT_DETECT_TIME, Zigbee will receive packets; otherwise, if ZG_PRIORITY is high or goes high, ZB will TX data. Unit 1us. */
#define BIT_ZB_TRX_DETECT_TIME(x)                 ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_ZB_TRX_DETECT_TIME(x)             ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_ZB_PRI_DETECT_TIME               ((u32)0x000000FF << 16)          /*!<R/W 0xC8  Valid if BIT_ZIGBEE_MODE = 1 & BIT_EXT_ZIGBEE_MODE = 0 (external mode, directional PRIORITY) After ZB_REQUEST asserting, if ZG_PRIORITY is asserted within this ZB_PRI_DETECT_TIME duration, ZG will initiate high priority activities; otherwise, if ZB_PRIORITY is not asserted within this duration, it would be low priority activities. Units 1us. (0-255us) */
#define BIT_ZB_PRI_DETECT_TIME(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_ZB_PRI_DETECT_TIME(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_ZB_CNT_THR                       ((u32)0x000000FF << 8)          /*!<R/W 0  ZB count threshold, unit: us */
#define BIT_ZB_CNT_THR(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_ZB_CNT_THR(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_ZB_ARB_TIME                      ((u32)0x000000FF << 0)          /*!<R/W 0  8 bit timer register : Arbitration window of ZB trx request (unit: us) */
#define BIT_ZB_ARB_TIME(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_ZB_ARB_TIME(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_RTX_CTX_SUBTYPE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_CTX_SUBTYPE               ((u32)0x0000000F << 4)          /*!<R/W 0x7   */
#define BIT_R_WMAC_CTX_SUBTYPE(x)                 ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_WMAC_CTX_SUBTYPE(x)             ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_WMAC_RTX_SUBTYPE               ((u32)0x0000000F << 0)          /*!<R/W 0x6   */
#define BIT_R_WMAC_RTX_SUBTYPE(x)                 ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_WMAC_RTX_SUBTYPE(x)             ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_RTX_SWAES_RD_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_SWAES_RD_0                  ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_WMAC_SWAES_RD_0(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_WMAC_SWAES_RD_0(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_RTX_SWAES_RD_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_SWAES_RD_1                  ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_WMAC_SWAES_RD_1(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_WMAC_SWAES_RD_1(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_RTX_SWAES_RD_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_SWAES_RD_2                  ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_WMAC_SWAES_RD_2(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_WMAC_SWAES_RD_2(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_SWAES_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BTCOEX_SW_RST                         ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_GNT_BOTH_POL                          ((u32)0x00000001 << 28)          /*!<R/W 0  grant both choose GNT_BT polarity: 0 : deassert GNT_BT signal as PTA grant both WiFI and BT. 1 : assert GNT_BT signal as PTA grant both WiFi and BT. */
#define BIT_BT_ARB_INST_EN                        ((u32)0x00000001 << 24)          /*!<R/W 0  1: enable GNT_BT_BYPASS_PRIORITY 0 : disable */
#define BIT_MASK_BT_ARB_TIME                      ((u32)0x000000FF << 16)          /*!<R/W 0  8 bit timer register : Arbitration window of bt trx request ( in the unit of us) */
#define BIT_BT_ARB_TIME(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_BT_ARB_TIME(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_SWAES_REQ                             ((u32)0x00000001 << 7)          /*!<R 0  software aes engine request polling */
#define BIT_R_WMAC_SWAES_WE                       ((u32)0x00000001 << 3)          /*!<R/W 0  Write enable of register ro_WMAC_SWAES_RD */
#define BIT_R_WMAC_SWAES_SEL                      ((u32)0x00000001 << 0)          /*!<R/W 0  When R_WMAC_SWAES_SEL =1,the content of register 0750-075F used to be aes engine data in or data out . When R_WMAC_SWAES_SEL =0, the content of register 0754-075F used to be aes engine security key */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_COEX_ENH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_ZIGBEE_EN                             ((u32)0x00000001 << 31)          /*!<R/W 0  Enable ZigBee function */
#define BIT_ZIGBEE_MODE                           ((u32)0x00000001 << 30)          /*!<R/W 0  0: SOC 1: external */
#define BIT_EXT_ZIGBEE_MODE                       ((u32)0x00000001 << 29)          /*!<R/W 0  The mode of external ZigBee 1: static PRIORITY 0: directional PRIORITY */
#define BIT_COEX_TABLE_SEL                        ((u32)0x00000001 << 28)          /*!<R/W 1  Select coexistence table for WLAN & ZB 0: BIT_COEX_TABLE_WL_ZB_1 1: BIT_COEX_TABLE_WL_ZB_2 */
#define BIT_EXT_ZB_GNT_POLAR                      ((u32)0x00000001 << 27)          /*!<R/W 1  Select the polar of ZB_GRANT, for extarnal ZB 0: low active 1: high active */
#define BIT_ZB_ARB_INST_EN                        ((u32)0x00000001 << 26)          /*!<R/W 0  1: enable the time limit of ZigBee request (zb_pri | zb_rx | zb_tx), PTA is valid during ( <=BIT_ZB_ARB_TIME) 0: disable Co-work with BIT_ZB_ARB_TIME */
#define BIT_ZB_CNT_THREN                          ((u32)0x00000001 << 25)          /*!<R/W 0  ZB count threshold enable, for statistics co-work with BIT_ZB_CNT_THR */
#define BIT_ZB_STATISTIC_EN                       ((u32)0x00000001 << 24)          /*!<R/W 0  enable statistic of ZB */
#define BIT_GNT_ZB_BB_SW_EN                       ((u32)0x00000001 << 23)          /*!<R/W 0  Enable SW control gnt_zb_bb */
#define BIT_GNT_ZB_BB_SW                          ((u32)0x00000001 << 22)          /*!<R/W 0  SW value */
#define BIT_GNT_ZB_RFC_SW_EN                      ((u32)0x00000001 << 21)          /*!<R/W 0  Enable SW control gnt_zb_rfc */
#define BIT_GNT_ZB_RFC_SW                         ((u32)0x00000001 << 20)          /*!<R/W 0  SW value */
#define BIT_GNT_ZB_EXT_SW_EN                      ((u32)0x00000001 << 19)          /*!<R/W 0  Enable SW control gnt_zb_o of external ZigBee */
#define BIT_GNT_ZB_EXT_SW                         ((u32)0x00000001 << 18)          /*!<R/W 0  SW value, external ZigBee */
#define BIT_EXT_ZB_REQ_POLAR                      ((u32)0x00000001 << 17)          /*!<R/W 1  Select the polar of ZB_REQUEST, for static mode external ZB 0: low active 1: high active */
#define BIT_DIS_BT_DONT_CARE                      ((u32)0x00000001 << 16)          /*!<R/W 0  1: break table of BT & WL can be all "1" 0: break table of BT & WL cannot be all "1" */
#define BIT_DIS_ZB_DONT_CARE                      ((u32)0x00000001 << 15)          /*!<R/W 0  1: break table of ZB & WL can be all "1" 0: break table of ZB & WL cannot be all "1" */
#define BIT_BTG_LNA1_GAIN_SEL                     ((u32)0x00000001 << 14)          /*!<R/W 0  control by Driver 0: mask for WL 1: original BTG */
#define BIT_R_GNT_BT_RFC_SW                       ((u32)0x00000001 << 12)          /*!<R/W 1  SW control BT RF */
#define BIT_R_GNT_BT_RFC_SW_EN                    ((u32)0x00000001 << 11)          /*!<R/W 1  SW control BT RF enable */
#define BIT_R_GNT_BT_BB_SW                        ((u32)0x00000001 << 10)          /*!<R/W 1  SW control BT baseband */
#define BIT_R_GNT_BT_BB_SW_EN                     ((u32)0x00000001 << 9)          /*!<R/W 1  SW control BT baseband enable */
#define BIT_R_BT_CNT_THREN                        ((u32)0x00000001 << 8)          /*!<R/W 0  BT count threshold enable */
#define BIT_MASK_R_BT_CNT_THR                     ((u32)0x000000FF << 0)          /*!<R/W 0  BT count threshold */
#define BIT_R_BT_CNT_THR(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BT_CNT_THR(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WLAN_ACT_MASK_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RO_PTA_GNT_INFO                  ((u32)0x0000001F << 27)          /*!<R 0  [0]: gnt_wl [1]: gnt_bt_bb [2]: gnt_bt_rfc [3]: gnt_zb_bb [4]: gnt_zb_rfc */
#define BIT_RO_PTA_GNT_INFO(x)                    ((u32)(((x) & 0x0000001F) << 27))
#define BIT_GET_RO_PTA_GNT_INFO(x)                ((u32)(((x >> 27) & 0x0000001F)))
#define BIT_MASK_CYC_PTA_EN_RFC                   ((u32)0x0000001F << 16)          /*!<R/W 0  Enable RFC clock some cycles after gnt_* signals changed. unit: 1 mac_clk */
#define BIT_CYC_PTA_EN_RFC(x)                     ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_CYC_PTA_EN_RFC(x)                 ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_WACTMSK_OFF_DUR                  ((u32)0x000000FF << 8)          /*!<R/W 0  WLAN will reserve channel for BT during MASK OFF duration. Unit 0.625ms */
#define BIT_WACTMSK_OFF_DUR(x)                    ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_WACTMSK_OFF_DUR(x)                ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_WACTMSK_ON_DUR                   ((u32)0x000000FF << 0)          /*!<R/W 0  WLAN can block BT during MASK On duration. Unit 0.625ms */
#define BIT_WACTMSK_ON_DUR(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_WACTMSK_ON_DUR(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WLAN_ACT_MASK_CTRL_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BT_STAT_DELAY                    ((u32)0x0000000F << 28)          /*!<R/W 0  For CSR, BT_PRI and BT_STAT go high form low at the same time. Thus, BT_STAT need delay BT_STAT_DELAY time to trigger hardware. Unit: 1us */
#define BIT_BT_STAT_DELAY(x)                      ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_BT_STAT_DELAY(x)                  ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_BT_TRX_INIT_DETECT               ((u32)0x0000000F << 24)          /*!<R/W 0  After BT_PRI asserting for BT_TRX_INIT_DETECT us, if BT_STAT is low, then BT will receive packets; otherwise, if BT goes high, BT will TX data. Unit 4us. */
#define BIT_BT_TRX_INIT_DETECT(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_BT_TRX_INIT_DETECT(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_BT_PRI_DETECT_TO                 ((u32)0x0000000F << 20)          /*!<R/W 0  After BT_PRI asserting, if BT_STAT is asserted within this TO duration, BT will initiate high priority activities; otherwise, if BT_STAT is not asserted within this duration, it would be low priority activities. Units 1us. */
#define BIT_BT_PRI_DETECT_TO(x)                   ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_BT_PRI_DETECT_TO(x)               ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_R_GRANTALL_WLMASK                     ((u32)0x00000001 << 19)          /*!<R/W 0  grant all mask */
#define BIT_STATIS_BT_EN                          ((u32)0x00000001 << 18)          /*!<R/W 0  When this bit is set, the BT_ACT_STATISTICS counters starts counting */
#define BIT_EN_WL_ACT_MASK                        ((u32)0x00000001 << 17)          /*!<R/W 0  Enable WLAN_ACT Mask Function (related to 0x768h-0x76Dh) */
#define BIT_ENHBT                                 ((u32)0x00000001 << 16)          /*!<R/W 0  Enable Enhanced BT Interface (we suggest to use with BT mode 2) */
#define BIT_WLRX_TER_BY_CTL_1                     ((u32)0x00000001 << 11)          /*!<R/W 0  When wlan receive a control packet without fit address, whether the signal indicating wlan rx can be terminated as soon as the address check finish is not only controlled by r_WLRX_TER_BY_AD, but also should be refereed to this bit. 1: enable; 0: disable */
#define BIT_WLRX_TER_BY_AD_1                      ((u32)0x00000001 << 10)          /*!<R/W 0  When wlan receive the packet without fit address, the signal indicating wlan rx can be terminated as soon as the address check finish. This feature can be enabled by this bit. 1: en */
#define BIT_ANT_DIVERSITY_SEL_1                   ((u32)0x00000001 << 9)          /*!<R/W 0  Antenna diversity for 2 antenna mode: MAC_ANT_SEL = GNT_BT ^ ANT_DIVERDITY_SEL */
#define BIT_ANTSEL_FOR_BT_CTRL_EN_1               ((u32)0x00000001 << 8)          /*!<R/W 0  Enable the control of antenna selection through GNT_BT: GNT_BT=1: ANT_SEL for BT device; GNT_BT=0: ANT_SEL for WL device. */
#define BIT_WLACT_LOW_GNTWL_EN_1                  ((u32)0x00000001 << 2)          /*!<R/W 0  When WLACT_LOW_GNTWL_EN = 1, the TRX of WLAN are blocked according to the BTCOEX table arbitration during MASK OFF duration. WLACT_LOW_GNTWL_EN = 0, the TRX of WLAN are paused during MASK OFF duration.. */
#define BIT_WLACT_HIGH_GNTBT_EN_1                 ((u32)0x00000001 << 1)          /*!<R/W 0  When WLACT_HIGH_GNTBT_EN = 1, the TRX of WLAN are blocked according to the BTCOEX table arbitration during MASK ON duration. When WLACT_HIGH_ GNTBT_EN = 0, the TRX of WLAN are not paused during MASK OFF duration. */
#define BIT_EN_WLACT_MASK_SYNC                    ((u32)0x00000001 << 0)          /*!<R/W 0  When NAV update is beyond this value, then NAV update is aborted. When this field is 0x00, NAV_UPPER function is disable. The unit is 128us. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_ENHANCED_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_STATIS_BT_31_0                   ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_STATIS_BT_31_0(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_STATIS_BT_31_0(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_ENHANCED_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_STATIS_BT_63_32                  ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_STATIS_BT_63_32(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_STATIS_BT_63_32(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_PROPRI
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BT_CMD_RPT                       ((u32)0x0000FFFF << 16)          /*!<R 0   */
#define BIT_BT_CMD_RPT(x)                         ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_BT_CMD_RPT(x)                     ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_RPT_FROM_BT                      ((u32)0x000000FF << 8)          /*!<R 0   */
#define BIT_RPT_FROM_BT(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_RPT_FROM_BT(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_BT_HID_ISR_SET                   ((u32)0x00000003 << 6)          /*!<R/W 0  When EN_BT_PROFILE_OR_HID is set „1‟, hardware also need to check this value to trigger an interrupt. 0: default, TX and RX 1: TX 2: RX 3: Reserved */
#define BIT_BT_HID_ISR_SET(x)                     ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_BT_HID_ISR_SET(x)                 ((u32)(((x >> 6) & 0x00000003)))
#define BIT_TDMA_BT_START_NOTIFY                  ((u32)0x00000001 << 5)          /*!<R/W 0  When the control of TDMA mode is firmware and firmware also receive signal that NULL packet for BT had sent, firmware can fill this bit to notify HW PTA to pull WLAN_ACT to low and the interval is Tbt */
#define BIT_ENABLE_TDMA_HW_MODE                   ((u32)0x00000001 << 4)          /*!<R/W 0  0: default, PTA TDMA mode is hardware mode. 1: enable firmware mode. */
#define BIT_ENABLE_PTA_TDMA_MODE                  ((u32)0x00000001 << 3)          /*!<R/W 0  0: default, disable PTA TDMA mode 1: enable TDMA mode: a. WLAN_ACT default is high. b. Hardware mode: When NULL packet for BT had sent, hardware will notify PTA to start TDMA mode. Firmware mode: When NULL packet for BT had sent, hardware will trigger an interrupt to notify firmware. Then, firmware need to set TDMA_BT_START_NOTIFY to notify PTA to start TDMA mode. */
#define BIT_ENABLE_COEXIST_TAB_IN_TDMA            ((u32)0x00000001 << 2)          /*!<R/W 0  0: default, no matter which WLAN_ACT is high or low, PTA doesn't execute co-exist table scheme 1: enable, When WLAN_ACT is high, PTA execute xo-exist table scheme */
#define BIT_GPIO2_GPIO3_EXANGE_OR_NO_BT_CCA       ((u32)0x00000001 << 1)          /*!<R/W 0  Enable GPIO2 and GPIO3 exchange. (Note: need to enable mode2 and enhance 3 wire) If Enable RTK BT mode, set this bit to 1 to disable granting to BT device when arbitration */
#define BIT_RTK_BT_ENABLE                         ((u32)0x00000001 << 0)          /*!<R/W 0  Enable RTK BT mode */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_Status_Report_Register
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BT_PROFILE                       ((u32)0x000000FF << 24)          /*!<R 0  When WLAN received CMD_ID 0x3, it will recorder to this. */
#define BIT_BT_PROFILE(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_BT_PROFILE(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_BT_POWER                         ((u32)0x000000FF << 16)          /*!<R 0  When WLAN received CMD_ID 0x2, it will recorder to this */
#define BIT_BT_POWER(x)                           ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_BT_POWER(x)                       ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_BT_PREDECT_STATUS                ((u32)0x000000FF << 8)          /*!<R 0  When WLAN received CMD_ID 0x0, it will recorder to this */
#define BIT_BT_PREDECT_STATUS(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BT_PREDECT_STATUS(x)              ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_BT_CMD_ID                        ((u32)0x000000FF << 0)          /*!<R 0  command information from BT */
#define BIT_BT_CMD_ID(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_BT_CMD_ID(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_Interrupt_Control_Register
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WLAN_RPT_NOTIFY                       ((u32)0x00000001 << 31)          /*!<R/W 0  When firmware had prepared packet to send to BT, it need to set this bit to notify hardware that it can transmit. 0: There isn't a valid packet in register” WLAN_RPT_ PKT”. 1: There is a valid packet in register” WLAN_RPT_ PKT”. */
#define BIT_EN_WLAN_RPT_AND_BT_QUERY              ((u32)0x00000001 << 30)          /*!<R/W 0  Enable two interrupt: 1) PTA sent the report packet successfully 2) PTA sent a query packet 0: Disable 1: Enable */
#define BIT_EN_MAC_NULL_PKT_NOTIFY                ((u32)0x00000001 << 29)          /*!<R/W 0  When MAC had sent NULL packet for BT and firmware also set nav_for_bt bit, hardware triggers an interrupt to notify firmware. 0: Disable 1: Enable */
#define BIT_EN_BT_STSTUS_RPT                      ((u32)0x00000001 << 28)          /*!<R/W 0  When WLAN receive cmd_id “0”, hardware trigger an interrupt to notify firmware. 0: Disable 1: Enable */
#define BIT_EN_BT_POWER                           ((u32)0x00000001 << 27)          /*!<R/W 0  When WLAN received the packet including BT Power and check register “BT_STATUS_RPT”, if data contents aren't the same, hardware can trigger an interrupt to notify firmware. 0: Disable 1: Enable */
#define BIT_EN_BT_CHANNEL                         ((u32)0x00000001 << 26)          /*!<R/W 0  When WLAN received the packet including BT channel number, hardware can trigger an interrupt to notify firmware. 0: Disable 1: Enable */
#define BIT_EN_BT_SLOT_CHANGE                     ((u32)0x00000001 << 25)          /*!<R/W 0  When WLAN received the packet including that BT had multi-slots, hardware can trigger an interrupt to notify firmware. 0: Disable 1: Enable */
#define BIT_EN_BT_PROFILE_OR_HID                  ((u32)0x00000001 << 24)          /*!<R/W 0  Enable two interrupt 1) When WLAN received the packet including that WLAN specific profile and check register “BT_STATUS_RPT”, if data contents aren't the same, hardware can trigger an interrupt to notify firmware. 2) When WLAN received the report packet including HID information, hardware will trigger an interrupt to notify firmware. 0: Disable 1: Enable */
#define BIT_MASK_WLAN_RPT_DATA                    ((u32)0x000000FF << 16)          /*!<R/W 0  Data contents, When the report packet had sent, hardware will clear this register. */
#define BIT_WLAN_RPT_DATA(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_WLAN_RPT_DATA(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_TO_BT_CMDID                      ((u32)0x000000FF << 8)          /*!<R/W 0  Command id: 0x80~FF */
#define BIT_TO_BT_CMDID(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_TO_BT_CMDID(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_BT_DATA                          ((u32)0x000000FF << 0)          /*!<R 0  When WLAN received the packet BT sent, hardware will place data to this register in addition to CMD_ID 0x0 ~ 0x3. */
#define BIT_BT_DATA(x)                            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_BT_DATA(x)                        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WLAN_Report_Time_Out_Control_Register
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ISOLATION_CHK_0                  ((u32)0x007FFFFF << 9)          /*!<R/W 0  0: leave PTA 1: enter PTA */
#define BIT_ISOLATION_CHK_0(x)                    ((u32)(((x) & 0x007FFFFF) << 9))
#define BIT_GET_ISOLATION_CHK_0(x)                ((u32)(((x >> 9) & 0x007FFFFF)))
#define BIT_ISOLATION_EN                          ((u32)0x00000001 << 8)          /*!<R/W 0  Isolation table enable bit. 0: disable 1: enable */
#define BIT_MASK_WLAN_RPT_TO                      ((u32)0x000000FF << 0)          /*!<R/W 0  When WLANＨW received query packet, HW timer counts. If timer equals to WLAN_RPT_TO and HW can‟t send packet, it will give up query packet at this time. When BT sent another query packet, WLAN HW need to send the previous packet and it shouldn't trigger an interrupt to firmware. Unit: 4us. */
#define BIT_WLAN_RPT_TO(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_WLAN_RPT_TO(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_Isolation_Table_Register_Register_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ISOLATION_CHK_1                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  0: leave PTA 1: enter PTA */
#define BIT_ISOLATION_CHK_1(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_ISOLATION_CHK_1(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_Interrupt_Status_Register
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BT_HID_ISR                            ((u32)0x00000001 << 31)          /*!<R/W1C 0  When WLAN received the report packet including HID information, hardware will trigger an interrupt to notify firmware */
#define BIT_BT_QUERY_ISR                          ((u32)0x00000001 << 30)          /*!<R/W1C 0  When WLAN received a query packet, hardware triggers an interrupt to notify firmware */
#define BIT_MAC_NULL_PKT_NOTIFY_ISR               ((u32)0x00000001 << 29)          /*!<R/W1C 0  When MAC had sent NULL packet for BT and firmware also set nav_for_bt bit, hardware triggers an interrupt to notify firmware */
#define BIT_WLAN_RPT_ISR                          ((u32)0x00000001 << 28)          /*!<R/W1C 0  When PTA sent the report packet successfully, hardware triggers an interrupt to notify firmware. */
#define BIT_BT_POWER_ISR                          ((u32)0x00000001 << 27)          /*!<R/W1C 0  When WLAN received the packet including BT Power and check register “BT_STATUS_RPT”, if data contents aren't the same, hardware can trigger an interrupt to notify firmware */
#define BIT_BT_CHANNEL_ISR                        ((u32)0x00000001 << 26)          /*!<R/W1C 0  When WLAN received the packet including BT channel number, hardware can trigger an interrupt to notify firmware. */
#define BIT_BT_SLOT_CHANGE_ISR                    ((u32)0x00000001 << 25)          /*!<R/W1C 0  When WLAN received the packet including that BT had multi-slots, hardware can trigger an interrupt to notify firmware */
#define BIT_BT_PROFILE_ISR                        ((u32)0x00000001 << 24)          /*!<R/W1C 0  When WLAN received the packet including that WLAN specific profile and check register “BT_STATUS_RPT”, if data contents aren't the same, hardware can trigger an interrupt to notify firmware. */
#define BIT_MASK_ISOLATION_CHK_2                  ((u32)0x00FFFFFF << 0)          /*!<R/W 0  0: leave PTA 1: enter PTA */
#define BIT_ISOLATION_CHK_2(x)                    ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_ISOLATION_CHK_2(x)                ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_TDMA_Time_Register
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BT_TIME                          ((u32)0x03FFFFFF << 6)          /*!<R/W 0  The time is that WLAN_ACT is low. Unit: us */
#define BIT_BT_TIME(x)                            ((u32)(((x) & 0x03FFFFFF) << 6))
#define BIT_GET_BT_TIME(x)                        ((u32)(((x >> 6) & 0x03FFFFFF)))
#define BIT_MASK_BT_RPT_SAMPLE_RATE               ((u32)0x0000003F << 0)          /*!<R/W 0  When wifi detected the raising edge of BT report packet, wifi need to counter BT_RPT_SAMPLE_RATE. Then hw could detect which is high or low. Uint: 20 ns */
#define BIT_BT_RPT_SAMPLE_RATE(x)                 ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_BT_RPT_SAMPLE_RATE(x)             ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_ACT_Register
 * @brief
 * @{
 *****************************************************************************/
#define BIT_B_AX_BT_BLE_EN                        ((u32)0x00000001 << 24)          /*!<R/W 0  Decide the GNT_BT_TX / RX 0: GNT_BT_TX = GNT_BT & BT_TX ; GNT_BT_RX = GNT_BT & BT_RX 1: GNT_BT_TX = GNT_BT & (BT_TX | BT_BLE) ; GNT_BT_RX = GNT_BT & (BT_RX | BT_BLE) */
#define BIT_MASK_BT_EISR_EN                       ((u32)0x000000FF << 16)          /*!<R/W 0  BT eisr enable */
#define BIT_BT_EISR_EN(x)                         ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_BT_EISR_EN(x)                     ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_BT_ACT_FALLING_ISR                    ((u32)0x00000001 << 10)          /*!<R/W1C 0  When BT_ACT signal is falling edge, it will trigger an interrupt. */
#define BIT_BT_ACT_RISING_ISR                     ((u32)0x00000001 << 9)          /*!<R/W1C 0  When BT_ACT signal is raising edge, it will trigger an interrupt */
#define BIT_TDMA_TO_ISR                           ((u32)0x00000001 << 8)          /*!<R/W1C 0  When BT_TIME expired, it will trigger an interrupt. */
#define BIT_MASK_BT_CH                            ((u32)0x0000007F << 0)          /*!<R 0x7f  When hardware receive cm_id “1”, it will recorder to this. */
#define BIT_BT_CH(x)                              ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_BT_CH(x)                          ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LTR_IDLE_LATENCY_V2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LTR_IDLE_LATENCY                 ((u32)0x00000003 << 30)          /*!<R/W 0  LTR idle latency. */
#define BIT_LTR_IDLE_LATENCY(x)                   ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_LTR_IDLE_LATENCY(x)               ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_OBFF_STATE                       ((u32)0x00000003 << 28)          /*!<R 0   */
#define BIT_OBFF_STATE(x)                         ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_OBFF_STATE(x)                     ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_OBFF_CTRL_1                      ((u32)0x00FFFFFF << 4)          /*!<R/W 0x444440   */
#define BIT_OBFF_CTRL_1(x)                        ((u32)(((x) & 0x00FFFFFF) << 4))
#define BIT_GET_OBFF_CTRL_1(x)                    ((u32)(((x >> 4) & 0x00FFFFFF)))
#define BIT_MASK_OBFF_CTRL_0                      ((u32)0x0000000F << 0)          /*!<R 0   */
#define BIT_OBFF_CTRL_0(x)                        ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_OBFF_CTRL_0(x)                    ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LTR_ACTIVE_LATENCY_V2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LTR_ACTIVE_LATENCY               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x02030405  LTR Active latency. */
#define BIT_LTR_ACTIVE_LATENCY(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_LTR_ACTIVE_LATENCY(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_OBFF_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LTR_CTRL                         ((u32)0x000000FF << 24)          /*!<R/W 0x4a   */
#define BIT_LTR_CTRL(x)                           ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_LTR_CTRL(x)                       ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_LTR_IDLE_FLAG                         ((u32)0x00000001 << 23)          /*!<R 0   */
#define BIT_LTR_ACT_FLAG                          ((u32)0x00000001 << 22)          /*!<R 0   */
#define BIT_MASK_OBFF_CTRL                        ((u32)0x003FFFFF << 0)          /*!<R/W 0x4010   */
#define BIT_OBFF_CTRL(x)                          ((u32)(((x) & 0x003FFFFF) << 0))
#define BIT_GET_OBFF_CTRL(x)                      ((u32)(((x >> 0) & 0x003FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LTR_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LTR_RPBACC_ID_HIGH               ((u32)0x00000007 << 25)          /*!<R/W 3'b000  Threshold to send ltr act msg after accumulating high rate rx payload page number. */
#define BIT_LTR_RPBACC_ID_HIGH(x)                 ((u32)(((x) & 0x00000007) << 25))
#define BIT_GET_LTR_RPBACC_ID_HIGH(x)             ((u32)(((x >> 25) & 0x00000007)))
#define BIT_MASK_LTR_RPBACC_ID_MEDIA              ((u32)0x00000007 << 22)          /*!<R/W 3'b100  Threshold to send ltr act msg after accumulating media rate rx payload page number. */
#define BIT_LTR_RPBACC_ID_MEDIA(x)                ((u32)(((x) & 0x00000007) << 22))
#define BIT_GET_LTR_RPBACC_ID_MEDIA(x)            ((u32)(((x >> 22) & 0x00000007)))
#define BIT_MASK_LTR_RPBACC_ID_LOW                ((u32)0x00000007 << 19)          /*!<R/W 3'b100  Threshold to send ltr act msg after accumulating low rate rx payload page number. */
#define BIT_LTR_RPBACC_ID_LOW(x)                  ((u32)(((x) & 0x00000007) << 19))
#define BIT_GET_LTR_RPBACC_ID_LOW(x)              ((u32)(((x >> 19) & 0x00000007)))
#define BIT_RPBACC_CLRBLOCK                       ((u32)0x00000001 << 18)          /*!<R/W 0  Clear rx dma and int block after rx packet page large than threshold */
#define BIT_LTR_RPBACC_EN                         ((u32)0x00000001 << 17)          /*!<R/W 1  Enable gen ltr active msg according to occupied pages in rx packet buffer */
#define BIT_MASK_LTR_SPACE_ID                     ((u32)0x00000003 << 15)          /*!<R/W 2'b00  Ltr space index.0:h100(for test),1:h3000(300us),2:h5000(512us),3:h9000(921us) */
#define BIT_LTR_SPACE_ID(x)                       ((u32)(((x) & 0x00000003) << 15))
#define BIT_GET_LTR_SPACE_ID(x)                   ((u32)(((x >> 15) & 0x00000003)))
#define BIT_MASK_LTR_IDLE_TIMER_ID                ((u32)0x00000007 << 12)          /*!<R/W 3'b011  Send ltr idle message after trx state machine in idle for more than this time. 0:2 us,1: 13us,2 :100us,3: 800us,4:1.13ms,5:2.25ms,6: 6ms */
#define BIT_LTR_IDLE_TIMER_ID(x)                  ((u32)(((x) & 0x00000007) << 12))
#define BIT_GET_LTR_IDLE_TIMER_ID(x)              ((u32)(((x >> 12) & 0x00000007)))
#define BIT_MASK_BLKHWBCN_TIMER_ID                ((u32)0x00000007 << 9)          /*!<R/W 3'b010  Use obff block timer to send ltr active */
#define BIT_BLKHWBCN_TIMER_ID(x)                  ((u32)(((x) & 0x00000007) << 9))
#define BIT_GET_BLKHWBCN_TIMER_ID(x)              ((u32)(((x >> 9) & 0x00000007)))
#define BIT_OBFF_LTRIDLE_EN                       ((u32)0x00000001 << 8)          /*!<R/W 0  Send LTR idle message in obff or cpu active window */
#define BIT_CFG_DELAY_LTR_ACT                     ((u32)0x00000001 << 7)          /*!<R/W 0  Use obff block timer to send ltr active */
#define BIT_CFG_CTS_LTRACT                        ((u32)0x00000001 << 6)          /*!<R/W 0  CTS direct to send ltr active */
#define BIT_LTRIDLE_NO_SNOOP                      ((u32)0x00000001 << 5)          /*!<R/W 1  Calculating ltr active message sending timing according to no snoop or snoop latency. */
#define BIT_LTRACT_EN_INCPUACT                    ((u32)0x00000001 << 4)          /*!<R/W 0  Send LTR active in OBFF or CPU active window. */
#define BIT_APP_REQ_LTRACT                        ((u32)0x00000001 << 3)          /*!<R/W 0  Driver or cpu direct LTR active message. A message with LTR_ACTIVE_LATENCY will be sent. AUTO CLEAR */
#define BIT_APP_REQ_LTRIDLE                       ((u32)0x00000001 << 2)          /*!<R/W 1  Driver or cpu direct LTR idle message. A message with LTR_IDLE_LATENCY will be sent. AUTO CLEAR */
#define BIT_LTR_HW_EN                             ((u32)0x00000001 << 1)          /*!<R/W 0  Enable hardware LTR message */
#define BIT_LTR_EN                                ((u32)0x00000001 << 0)          /*!<R/W 1  Enable LTR message */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LTR_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_OBFF_TIMER_ID_HIGH               ((u32)0x00000007 << 29)          /*!<R/W 3'b100  When media rate rx packet in , delay rx dma timeout value index. 0: no delay,1: 12.8us; 2: 102.4us; 3: 204.8us; 4: 819.2 us; 5: 1.638ms; 6: 52ms; 7: always block until obff or cpu active window */
#define BIT_OBFF_TIMER_ID_HIGH(x)                 ((u32)(((x) & 0x00000007) << 29))
#define BIT_GET_OBFF_TIMER_ID_HIGH(x)             ((u32)(((x >> 29) & 0x00000007)))
#define BIT_MASK_HIGH_RATE_BDRY                   ((u32)0x0000007F << 16)          /*!<R/W 7'b1100000  High, media, low rate boundary */
#define BIT_HIGH_RATE_BDRY(x)                     ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_HIGH_RATE_BDRY(x)                 ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_LOW_RATE_BDRY                    ((u32)0x0000007F << 9)          /*!<R/W 7'b1000100  High, media, low rate boundary */
#define BIT_LOW_RATE_BDRY(x)                      ((u32)(((x) & 0x0000007F) << 9))
#define BIT_GET_LOW_RATE_BDRY(x)                  ((u32)(((x >> 9) & 0x0000007F)))
#define BIT_MASK_HIGH_RATE_TRIGS                  ((u32)0x00000003 << 7)          /*!<R/W 2'b01  High rate rx ltr trigger point select: 0 plcp ready;1: rx a1 latch OK; 2: fcs ready. */
#define BIT_HIGH_RATE_TRIGS(x)                    ((u32)(((x) & 0x00000003) << 7))
#define BIT_GET_HIGH_RATE_TRIGS(x)                ((u32)(((x >> 7) & 0x00000003)))
#define BIT_MASK_MEDIA_RATE_TRIGS                 ((u32)0x00000003 << 5)          /*!<R/W 2'b11  Media rate rx ltr trigger point select: 0 plcp ready;1: rx a1 latch OK; 2: fcs ready. */
#define BIT_MEDIA_RATE_TRIGS(x)                   ((u32)(((x) & 0x00000003) << 5))
#define BIT_GET_MEDIA_RATE_TRIGS(x)               ((u32)(((x >> 5) & 0x00000003)))
#define BIT_MASK_LOW_RATE_TRIGS                   ((u32)0x00000003 << 3)          /*!<R/W 2'b00  Low rate rx ltr trigger point select: 0 plcp ready;1: rx a1 latch OK; 2: fcs ready. */
#define BIT_LOW_RATE_TRIGS(x)                     ((u32)(((x) & 0x00000003) << 3))
#define BIT_GET_LOW_RATE_TRIGS(x)                 ((u32)(((x >> 3) & 0x00000003)))
#define BIT_MASK_RX_EXHAUST_ID                    ((u32)0x00000007 << 0)          /*!<R/W 3'b000  When rx page less than this threshold, release obff block rx. */
#define BIT_RX_EXHAUST_ID(x)                      ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_RX_EXHAUST_ID(x)                  ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ANTTRN_CTR_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TRAIN_STA_ADDR1                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  The Address of the trained station. Only available when ADDR2_MATCH_EN = 1 */
#define BIT_TRAIN_STA_ADDR1(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TRAIN_STA_ADDR1(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ANTTRN_CTR_1_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_ANTTRN_SWITCH                         ((u32)0x00000001 << 19)          /*!<R/W 0  Smart antenna use address cam or port register to search A2. 0:Use address cam 1:Use port register */
#define BIT_APPEND_MACID_IN_RESP_EN               ((u32)0x00000001 << 18)          /*!<R/W 0  Whether to append MACID in the rx descriptor of rx response packet */
#define BIT_ADDR2_MATCH_EN                        ((u32)0x00000001 << 17)          /*!<R/W 0  Enable the comparison of ADDR2 for the rx packet.. */
#define BIT_ANTTRN_EN                             ((u32)0x00000001 << 16)          /*!<R/W 0  Enable the antenna training function */
#define BIT_MASK_TRAIN_STA_ADDR2                  ((u32)0x0000FFFF << 0)          /*!<R/W 0  The Address of the trained station. Only available when ADDR2_MATCH_EN = 1 */
#define BIT_TRAIN_STA_ADDR2(x)                    ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TRAIN_STA_ADDR2(x)                ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_PKTCNT_RWD
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PKTCNT_BSSIDMAP                  ((u32)0x0000000F << 4)          /*!<R 0  To decide this counter is for which MBSSID (16組), default : 0 for counter 0 1 for counter 1 2 for counter 2 3 for counter 3 4 for counter 4 5 for counter 5 6 for counter 6 7 for counter 7 */
#define BIT_PKTCNT_BSSIDMAP(x)                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_PKTCNT_BSSIDMAP(x)                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_PKTCNT_CNTRST                         ((u32)0x00000001 << 1)          /*!<R 0  Write 1 to reset Counter after DRV read all counters , DRV set this bit and HW return 0 after reset */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_PKTCNT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WMAC_PKTCNT_FEN                       ((u32)0x00000001 << 8)          /*!<R/W 0  Packet counter function enable */
#define BIT_MASK_WMAC_PKTCNT_CFGAD                ((u32)0x000000FF << 0)          /*!<R/W 0  Packet counter configure register read/write address */
#define BIT_WMAC_PKTCNT_CFGAD(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_WMAC_PKTCNT_CFGAD(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_IQ_DUMP
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DUMP_OK_ADDR                     ((u32)0x0000FFFF << 16)          /*!<R 0  [31] : dump signal addr has round up [30:16] : dump finish addr in txpktbuffer */
#define BIT_DUMP_OK_ADDR(x)                       ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_DUMP_OK_ADDR(x)                   ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MACDBG_TRIG_IQDUMP                    ((u32)0x00000001 << 15)          /*!<R/W 0  Enable dump bb ADC signal trigger from MAC debug port which controled by r_LEVEL_PULSE_SEL. */
#define BIT_MASK_R_TRIG_TIME_SEL                  ((u32)0x0000007F << 8)          /*!<R/W 0  dump ok time to finish dump signal(unit is 1us) */
#define BIT_R_TRIG_TIME_SEL(x)                    ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TRIG_TIME_SEL(x)                ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_MAC_TRIG_SEL                   ((u32)0x00000003 << 6)          /*!<R/W 0  dump bb ADC sel mac strigger even 2'b00 : rxpkt ok trig dump ok 2'b01 : rxpkt_fauil trig dump ok 2'b10 : rxpkt cca rise edge to trig dump ok */
#define BIT_R_MAC_TRIG_SEL(x)                     ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_R_MAC_TRIG_SEL(x)                 ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MAC_TRIG_REG                          ((u32)0x00000001 << 5)          /*!<R/W 0  dump bb ADC sel mac strigger even write this reg to 1 to trig dump finish */
#define BIT_MASK_R_LEVEL_PULSE_SEL                ((u32)0x00000003 << 3)          /*!<R/W 0  if dump bb ADC signal : [3] for sel mac trigger event if dump mac dbg signal [4:3] : 2'b00 : level signal match [4:3] : 2'b01 : posedge match [4:3] : 2'b10 : negdge match */
#define BIT_R_LEVEL_PULSE_SEL(x)                  ((u32)(((x) & 0x00000003) << 3))
#define BIT_GET_R_LEVEL_PULSE_SEL(x)              ((u32)(((x >> 3) & 0x00000003)))
#define BIT_EN_LA_MAC                             ((u32)0x00000001 << 2)          /*!<R/W 0  enable dump mac dbg signal */
#define BIT_R_EN_IQDUMP                           ((u32)0x00000001 << 1)          /*!<R/W 0  enable dump bb ADC signal */
#define BIT_R_IQDATA_DUMP                         ((u32)0x00000001 << 0)          /*!<R/W 0  enable data dump for txpktbuffer high 64K */
/** @} */

/**************************************************************************//**
 * @defgroup REG_MASK_LA_MAC
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MASK_LA_MAC                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  used for mask one or more bits of debug port */
#define BIT_MASK_LA_MAC(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MASK_LA_MAC(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MATCH_REF_MAC
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MATCH_REF_MAC                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  used for compare with debug port */
#define BIT_MATCH_REF_MAC(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MATCH_REF_MAC(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_DBG_FTM_CTL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SEL_DEBUG_DATA                   ((u32)0x00000003 << 30)          /*!<R/W 0x3  11: data write to sram 10: data after sync_fifo 01: input data (TXD/RXD/IQ_DMA_DATA from BB) 00: RSVD */
#define BIT_SEL_DEBUG_DATA(x)                     ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_SEL_DEBUG_DATA(x)                 ((u32)(((x >> 30) & 0x00000003)))
#define BIT_EN_TIMEOUT                            ((u32)0x00000001 << 29)          /*!<R/W 0  Set this bit to 1 to reset mdpk read pointer */
#define BIT_EN_RE_START                           ((u32)0x00000001 << 28)          /*!<R/W 0  Before setting 0x7C0[1:0] =2’b11, set this bit to 1 to enable MDPK access mode */
#define BIT_MASK_LA_MAC_TIMEOUT_UNIT_SEL          ((u32)0x00000003 << 26)          /*!<R/W 0  LA mode triggered by mac debug port timeout value unit selection. 2'b00: 1us, 2'b01:32us, 2'b10: 1ms, 2'b11:RSVD */
#define BIT_LA_MAC_TIMEOUT_UNIT_SEL(x)            ((u32)(((x) & 0x00000003) << 26))
#define BIT_GET_LA_MAC_TIMEOUT_UNIT_SEL(x)        ((u32)(((x >> 26) & 0x00000003)))
#define BIT_MASK_LA_MAC_TIMEOUT_VALUE             ((u32)0x0000003F << 20)          /*!<R/W 0  LA mode triggered by mac debug port timeout value, available with non-zero value. */
#define BIT_LA_MAC_TIMEOUT_VALUE(x)               ((u32)(((x) & 0x0000003F) << 20))
#define BIT_GET_LA_MAC_TIMEOUT_VALUE(x)           ((u32)(((x >> 20) & 0x0000003F)))
#define BIT_LEVEL_PULSE_SEL_EXT                   ((u32)0x00000001 << 19)          /*!<R/W 0  LA mode triggered by mac debug port timeout event */
#define BIT_MASK_TRIG_TIME_UNIT_SEL               ((u32)0x00000007 << 16)          /*!<R/W 0  Select trigger time unit of 0x7C0[14:8]. 3’b000 : 1us , 3’b001 : 2us , 3’b010 : 4us , 3’b011 : 8us , 3’b100 : 16us , 3’b101 : 32us , 3’b110 : 64us, 3’b111 : RSV */
#define BIT_TRIG_TIME_UNIT_SEL(x)                 ((u32)(((x) & 0x00000007) << 16))
#define BIT_GET_TRIG_TIME_UNIT_SEL(x)             ((u32)(((x >> 16) & 0x00000007)))
#define BIT_EN_TXD_DEBUG                          ((u32)0x00000001 << 15)          /*!<R/W 0  LA MODE function. enable mactx debug mode */
#define BIT_EN_RXD_DEBUG                          ((u32)0x00000001 << 14)          /*!<R/W 0  LA MODE function. enable macrx debug mode */
#define BIT_TRX_TRIG_SEL_DEBUG                    ((u32)0x00000001 << 13)          /*!<R/W 0  LA MODE function. *1: select trig_stop signals from debug port, in MACTX/MACRX mode */
#define BIT_TRX_TRIG_SEL_PATT                     ((u32)0x00000001 << 12)          /*!<R/W 0  LA MODE function. *1: select trig_stop signals from pattern match (see 0x17F0~0x17FC), in MACTX/MACRX mode */
#define BIT_EN_IQD_LEN_BREAK                      ((u32)0x00000001 << 11)          /*!<R/W 0  LA MODE function. *1: after "trig_stop" signal asserted, if the iqdata's length over the BIT_IQD_LENGTH_REF, finish the iqdump process *0: only used TRIG_TIME finish the iqdump process */
#define BIT_MASK_IQD_LENGTH_REF                   ((u32)0x00000007 << 8)          /*!<R/W 0  LA MODE function. Valid when BIT_EN_IQD_LEN_BREAK = 1 Used to finish IQDUMP, unit is (8KB + 1bit) */
#define BIT_IQD_LENGTH_REF(x)                     ((u32)(((x) & 0x00000007) << 8))
#define BIT_GET_IQD_LENGTH_REF(x)                 ((u32)(((x >> 8) & 0x00000007)))
#define BIT_TRX_TRIG_SEL_WMAC                     ((u32)0x00000001 << 7)          /*!<R/W 1  LA MODE function. *1: select trig_stop signals from mactx or macrx, in MACTX/MACRX mode */
#define BIT_RXFTM_TXACK_SC                        ((u32)0x00000001 << 6)          /*!<R/W 0  When HW receive FTM pkt, HW will use RX FTM BW as response ACK TXBW. */
#define BIT_RXFTM_TXACK_BW                        ((u32)0x00000001 << 5)          /*!<R/W 0  When HW receive FTM pkt, HW will use RX FTM SC as response ACK TXSC. */
#define BIT_RXFTM_EN                              ((u32)0x00000001 << 3)          /*!<R/W 0  If BIT_FTM_TYPE = 1 && BIT_RXFTM_EN=1, HW will parsing rx packet, to find FTM request frame. */
#define BIT_RXFTMREQ_BYDRV                        ((u32)0x00000001 << 2)          /*!<R/W 0  If set this bit to 1,Rx FTMREQ/FTM can be receive into rxpktbuffer. */
#define BIT_RXFTMREQ_EN                           ((u32)0x00000001 << 1)          /*!<R/W 0  If BIT_FTM_TYPE = 1 && BIT_RXFTMREQ_EN=1, HW will parsing rx packet, to find FTM request frame. */
#define BIT_FTM_EN                                ((u32)0x00000001 << 0)          /*!<R/W 0  Fine timing measurement function; 0: Disable 1:enable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_OFDM_CCK_LEN_MASK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RX_OFDM_LEN_TH                   ((u32)0x0000FFFF << 16)          /*!<R/W 0x0FFF  rx ofdm len over this len to drop pkt(not for ht/vht) */
#define BIT_RX_OFDM_LEN_TH(x)                     ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_RX_OFDM_LEN_TH(x)                 ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_RX_CCK_LEN_TH                    ((u32)0x0000FFFF << 0)          /*!<R/W 0x0FFF  rx cck len over this length to drop pkt */
#define BIT_RX_CCK_LEN_TH(x)                      ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_RX_CCK_LEN_TH(x)                  ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_OPTION_FUNCTION_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_RXFIFO_FULL_TH_1          ((u32)0x000000FF << 24)          /*!<R/W 0x54  csi report mac pause bb almost full threshold */
#define BIT_R_WMAC_RXFIFO_FULL_TH_1(x)            ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_WMAC_RXFIFO_FULL_TH_1(x)        ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_R_WMAC_RX_SYNCFIFO_SYNC_1             ((u32)0x00000001 << 23)          /*!<R/W 1   */
#define BIT_R_WMAC_RXRST_DLY_1                    ((u32)0x00000001 << 22)          /*!<R/W 0  when this bit is set 1, rxrst pulse lasts for 20 cycle of MAC clock, when this bit is set 0, rxrst pulse lasts for 10 cycle of MAC clock, */
#define BIT_R_WMAC_SRCH_TXRPT_REF_DROP_1          ((u32)0x00000001 << 21)          /*!<R/W 0  when this bit is set 1, reference macid drop information when macid search */
#define BIT_R_WMAC_SRCH_TXRPT_UA1_1               ((u32)0x00000001 << 20)          /*!<R/W 0  when this bit is set 1, use tx A1 to search txrpt when rx ack/cts, when BIT_r_WMAC_SRCH_TXRPT_TYPE is set 0, BIT_r_WMAC_SRCH_TXRPT_UA1 should be set 1 */
#define BIT_R_WMAC_SRCH_TXRPT_TYPE_1              ((u32)0x00000001 << 19)          /*!<R/W 0  for rx ack/cts pkt not to search txrpt buffer macid */
#define BIT_R_WMAC_NDP_RST_1                      ((u32)0x00000001 << 18)          /*!<R/W 0  rx pkt is ndp to clr macrx_active */
#define BIT_R_WMAC_POWINT_EN_1                    ((u32)0x00000001 << 17)          /*!<R/W 0  enable rx pkt fc column power bit int for FISR */
#define BIT_R_WMAC_SRCH_TXRPT_PERPKT_1            ((u32)0x00000001 << 16)          /*!<R/W 0  enable search txrpt buffer for macid every pkt */
#define BIT_R_WMAC_SRCH_TXRPT_MID_1               ((u32)0x00000001 << 15)          /*!<R/W 1  enable search txrpt buffer for macid */
#define BIT_R_WMAC_PFIN_TOEN_1                    ((u32)0x00000001 << 14)          /*!<R/W 0  rx fifo fsm flush state timeout to error stage */
#define BIT_R_WMAC_FIL_SECERR_1                   ((u32)0x00000001 << 13)          /*!<R/W 1  sec len error drop(sec len is zero) */
#define BIT_R_WMAC_FIL_CTLPKTLEN_1                ((u32)0x00000001 << 12)          /*!<R/W 0  drop the rx pkt if the rx ctrl pkt len large than r_WMAC_RX_FIL_LEN */
#define BIT_R_WMAC_FIL_FCTYPE_1                   ((u32)0x00000001 << 11)          /*!<R/W 0  drop the ofdm pkt if the rx ofdm leg pkt fc type is 2'b11 */
#define BIT_R_WMAC_FIL_FCPROVER_1                 ((u32)0x00000001 << 10)          /*!<R/W 0  drop the ofdm pkt if the rx ofdm leg pkt fc version illegal */
#define BIT_R_WMAC_PHYSTS_SNIF_1                  ((u32)0x00000001 << 9)          /*!<R/W 0  append sniff info after phy status */
#define BIT_R_WMAC_PHYSTS_PLCP_1                  ((u32)0x00000001 << 8)          /*!<R/W 0  append plcp header after phy status */
#define BIT_R_MAC_TCR_VBONF_RD_1                  ((u32)0x00000001 << 7)          /*!<R/W 0  vbon neg edge to read all data of mac rx sync fifo for rx parser */
#define BIT_R_WMAC_TCR_MPAR_NDP_1                 ((u32)0x00000001 << 6)          /*!<R/W 0  nop ok check bb ndp vld & mac rx ndp check ok */
#define BIT_R_WMAC_NDP_FILTER_1                   ((u32)0x00000001 << 5)          /*!<R/W 0  mac rx ndp pkt set ppdu len to 0 */
#define BIT_R_WMAC_RXLEN_SEL_1                    ((u32)0x00000001 << 4)          /*!<R/W 0  rx vht len sel lsig len VHT PPDU length selection 0: 收到VHT PPDU PKT 會根據 VHTSIGB PPDU length 來parsing 與計算 1: 收到VHT PPDU PKT 會根據 BB 算的 LSIG length 來parsing 與計算 */
#define BIT_R_WMAC_RXLEN_SEL1_1                   ((u32)0x00000001 << 3)          /*!<R/W 0  rx vht len sel lsig len if sigb len is more than lsig len */
#define BIT_R_OFDM_FILTER_1                       ((u32)0x00000001 << 2)          /*!<R/W 0  RSVD */
#define BIT_R_WMAC_CHK_OFDM_LEN_1                 ((u32)0x00000001 << 1)          /*!<R/W 0  enable to check ofdm len */
#define BIT_R_WMAC_CHK_CCK_LEN_1                  ((u32)0x00000001 << 0)          /*!<R/W 0  enable to check rx cck len */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RX_FILTER_FUNCTION
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXHANG_EN                             ((u32)0x00000001 << 31)          /*!<R/W 0  Force read error content from fifo to solve rxhang. */
#define BIT_R_WMAC_MHRDDY_LATCH                   ((u32)0x00000001 << 30)          /*!<R/W 0  Adjust mach_rdy timing if rx_parser and rxpktfifo FSM is not sync. */
#define BIT_R_WMAC_MHRDDY_CLR                     ((u32)0x00000001 << 29)          /*!<R/W 1  Reset mach_rdy_latch when rx parser begin parsing mac header. */
#define BIT_R_RXPKTCTL_FSM_BASED_MPDURDY1         ((u32)0x00000001 << 28)          /*!<R/W 1  Adjust mpdu_rdy timing to solve FSM behavier error. */
#define BIT_WMAC_DIS_VHT_PLCP_CHK_MU              ((u32)0x00000001 << 27)          /*!<R/W 0  Disable check sigb crc value of MU plcp header. */
#define BIT_R_CHK_DELIMIT_LEN                     ((u32)0x00000001 << 26)          /*!<R/W 0  Drop mpdu if delimeter mpdu length smaller than mac header length + 4. */
#define BIT_R_REAPTER_ADDR_MATCH                  ((u32)0x00000001 << 25)          /*!<R/W 0  when this bit is set 1, physical address is matched when rx A1 is equal to tx A2, used in repeater mode */
#define BIT_R_RXPKTCTL_FSM_BASED_MPDURDY          ((u32)0x00000001 << 24)          /*!<R/W 0  when this bit is set 1, rxpktfifo_ctl FSM jumps based on mpdu_valid(all MPDU data has been transferred to rxpktbuffer), when this bit is set 0, rxpktfifo_ctl FSM jumps based on fcs_valid(all MPDU data has been parsed by rx_parser) */
#define BIT_R_LATCH_MACHRDY                       ((u32)0x00000001 << 23)          /*!<R/W 1  when this bit set 1,mach_rdy signal is latched to make sure that rxpktfifo_ctl can detect it */
#define BIT_R_WMAC_RXFIL_REND                     ((u32)0x00000001 << 22)          /*!<R/W 0  when this bit set 1,icvmic_valid signal is cleared when icvmic_rdy is valid(ICV/MIC is parsed) */
#define BIT_R_WMAC_MPDURDY_CLR                    ((u32)0x00000001 << 21)          /*!<R/W 0  when the bit is set 1, the last MPDU which will be transferred to rxpktfifo is ready and error occurs, mpdu_rdy signal is also valid(is means all MPDU data has been transferred to rxpktbuffer) */
#define BIT_R_WMAC_CLRRXSEC                       ((u32)0x00000001 << 20)          /*!<R/W 0  If new rx pkt is coming, reset the previous rx key search result. */
#define BIT_R_WMAC_RXFIL_RDEL                     ((u32)0x00000001 << 19)          /*!<R/W 0  If new rx pkt is coming, reset the rxpkt_ctl FSM after parsing plcp. */
#define BIT_R_WMAC_RXFIL_FCSE                     ((u32)0x00000001 << 18)          /*!<R/W 0  If rx pkt is not ampdu and rejected, reset rxpkt_ctl FSM immediately after parsing header */
#define BIT_R_WMAC_RXFIL_MESH_DEL                 ((u32)0x00000001 << 17)          /*!<R/W 0  drop the ofdm pkt if the rx ofdm leg pkt fc type is 2'b11 */
#define BIT_R_WMAC_RXFIL_MASKM                    ((u32)0x00000001 << 16)          /*!<R/W 1  drop the ofdm pkt if the rx ofdm leg pkt fc type is 2'b11 */
#define BIT_MASK_R_WMAC_RX_FIL_LEN_2              ((u32)0x0000FFFF << 0)          /*!<R/W 0x41  use with BIT_r_WMAC_FIL_CTLPKTLEN */
#define BIT_R_WMAC_RX_FIL_LEN_2(x)                ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_RX_FIL_LEN_2(x)            ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_OPTION_FUNCTION_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_TXNDP_SIGB                ((u32)0x001FFFFF << 0)          /*!<R/W 0x1845A5  SIGB for NDP packet when BW =40MHz */
#define BIT_R_WMAC_TXNDP_SIGB(x)                  ((u32)(((x) & 0x001FFFFF) << 0))
#define BIT_GET_R_WMAC_TXNDP_SIGB(x)              ((u32)(((x >> 0) & 0x001FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_DBG_SEL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MAC_DBG_SHIFT                    ((u32)0x00000007 << 8)          /*!<R/W 0   */
#define BIT_MAC_DBG_SHIFT(x)                      ((u32)(((x) & 0x00000007) << 8))
#define BIT_GET_MAC_DBG_SHIFT(x)                  ((u32)(((x >> 8) & 0x00000007)))
#define BIT_MASK_MAC_DBG_SEL                      ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define BIT_MAC_DBG_SEL(x)                        ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_MAC_DBG_SEL(x)                    ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_DBG_SHIFT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACTX_CMD_SHIFT                  ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_MACTX_CMD_SHIFT(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACTX_CMD_SHIFT(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WSEC_OPTION
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXDEC_BM_MGNT                         ((u32)0x00000001 << 22)          /*!<R/W 1  decryption enable of rx multicast management pkt, 0x680[3] shall also be set valid */
#define BIT_TXENC_BM_MGNT                         ((u32)0x00000001 << 21)          /*!<R/W 1  encryption enable of tx multicast management pkt, 0x680[2] shall also be set valid */
#define BIT_RXDEC_UNI_MGNT                        ((u32)0x00000001 << 20)          /*!<R/W 1  decryption enable of rx unicast management pkt, 0x680[3] shall also be set valid */
#define BIT_TXENC_UNI_MGNT                        ((u32)0x00000001 << 19)          /*!<R/W 1  encryption enable of tx unicast management pkt, 0x680[2] shall also be set valid */
#define BIT_WMAC_SEC_MASKIV                       ((u32)0x00000001 << 18)          /*!<R/W 0  MASK IV[15:8] for GCMP security */
#define BIT_MASK_WMAC_SEC_PN_SEL                  ((u32)0x00000003 << 16)          /*!<R/W 0  Security GCMP header PN selection */
#define BIT_WMAC_SEC_PN_SEL(x)                    ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_WMAC_SEC_PN_SEL(x)                ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_BT_TIME_CNT                      ((u32)0x000000FF << 0)          /*!<R/W 0x14  US timer counter boundary of BTCOEXIST */
#define BIT_BT_TIME_CNT(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_BT_TIME_CNT(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RTS_ADDRESS_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_RTS_ADDR0                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  if 0x604[31]=1 & rts is multicast, use this value as Address1 of RTS */
#define BIT_R_WMAC_RTS_ADDR0(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_WMAC_RTS_ADDR0(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RTS_ADDRESS_0_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_RTS_ADDR0_1               ((u32)0x0000FFFF << 0)          /*!<R/W 0  if 0x604[31]=1 & rts is multicast, use this value as Address1 of RTS */
#define BIT_R_WMAC_RTS_ADDR0_1(x)                 ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_RTS_ADDR0_1(x)             ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RTS_ADDRESS_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_RTS_ADDR1                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  if 0x604[31]=1 & rts is multicast, use this value as Address1 of RTS */
#define BIT_R_WMAC_RTS_ADDR1(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_WMAC_RTS_ADDR1(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RTS_ADDRESS_1_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_RTS_ADDR1_1               ((u32)0x0000FFFF << 0)          /*!<R/W 0  if 0x604[31]=1 & rts is multicast, use this value as Address1 of RTS */
#define BIT_R_WMAC_RTS_ADDR1_1(x)                 ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_RTS_ADDR1_1(x)             ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */


/*==========PAGE7 Register Address Definition==========*/
#define REG_MACID1                                   0x0700
#define REG_MACID1_1                                 0x0704
#define REG_BSSID1                                   0x0708
#define REG_BSSID1_1                                 0x070C
#define REG_BCN_PSR_RPT1                             0x0710
#define REG_ASSOCIATED_BFMEE_SEL                     0x0714
#define REG_SND_PTCL_CTRL                            0x0718
#define REG_BEAMFORM_STATUS                          0x071C
#define REG_NSARP_CFG                                0x0720
#define REG_ZB_STATISTIC_1                           0x0724
#define REG_ZB_STATISTIC_2                           0x0728
#define REG_BEAMFORMING_INFO_NSARP                   0x072C
#define REG_IPV6_0                                   0x0730
#define REG_IPV6_1                                   0x0734
#define REG_IPV6_2                                   0x0738
#define REG_IPV6_3                                   0x073C
#define REG_ZB_COEX_TABLE_1                          0x0740
#define REG_ZB_COEX_TABLE_2                          0x0744
#define REG_ZB_BREAK_TABLE                           0x0748
#define REG_EXT_ZB_DETEC_CTRL                        0x074C
#define REG_WMAC_RTX_CTX_SUBTYPE                     0x0750
#define REG_WMAC_RTX_SWAES_RD_1                      0x0754
#define REG_WMAC_RTX_SWAES_RD_2                      0x0758
#define REG_WMAC_RTX_SWAES_RD_3                      0x075C
#define REG_WMAC_SWAES_CFG                           0x0760
#define REG_BT_COEX_ENH                              0x0764
#define REG_WLAN_ACT_MASK_CTRL                       0x0768
#define REG_WLAN_ACT_MASK_CTRL_1                     0x076C
#define REG_BT_ENHANCED_CTRL0                        0x0770
#define REG_BT_ENHANCED_CTRL1                        0x0774
#define REG_BT_PROPRI                                0x0778
#define REG_BT_STATUS_REPORT_REGISTER                0x077C
#define REG_BT_INTERRUPT_CONTROL_REGISTER            0x0780
#define REG_WLAN_REPORT_TIME_OUT_CONTROL_REGISTER    0x0784
#define REG_BT_ISOLATION_TABLE_REGISTER_REGISTER_1   0x0788
#define REG_BT_INTERRUPT_STATUS_REGISTER             0x078C
#define REG_BT_TDMA_TIME_REGISTER                    0x0790
#define REG_BT_ACT_REGISTER                          0x0794
#define REG_LTR_IDLE_LATENCY_V2                      0x0798
#define REG_LTR_ACTIVE_LATENCY_V2                    0x079C
#define REG_OBFF_CTRL                                0x07A0
#define REG_LTR_CTRL                                 0x07A4
#define REG_LTR_CTRL2                                0x07A8
#define REG_ANTTRN_CTR_V1                            0x07B0
#define REG_ANTTRN_CTR_1_V1                          0x07B4
#define REG_WMAC_PKTCNT_RWD                          0x07B8
#define REG_WMAC_PKTCNT_CTRL                         0x07BC
#define REG_IQ_DUMP                                  0x07C0
#define REG_MASK_LA_MAC                              0x07C4
#define REG_MATCH_REF_MAC                            0x07C8
#define REG_WMAC_DBG_FTM_CTL                         0x07CC
#define REG_OFDM_CCK_LEN_MASK                        0x07D0
#define REG_WMAC_OPTION_FUNCTION_1                   0x07D4
#define REG_RX_FILTER_FUNCTION                       0x07D8
#define REG_WMAC_OPTION_FUNCTION_3                   0x07E0
#define REG_WMAC_DBG_SEL                             0x07E4
#define REG_WMAC_DBG_SHIFT                           0x07E8
#define REG_WSEC_OPTION                              0x07EC
#define REG_RTS_ADDRESS_0                            0x07F0
#define REG_RTS_ADDRESS_0_1                          0x07F4
#define REG_RTS_ADDRESS_1                            0x07F8
#define REG_RTS_ADDRESS_1_1                          0x07FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
