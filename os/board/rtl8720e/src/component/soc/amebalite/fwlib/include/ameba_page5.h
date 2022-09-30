#ifndef _AMEBAD2_PAGE5_H_
#define _AMEBAD2_PAGE5_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_EDCA_VO_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXOPLIMIT_VO                    ((u32)0x000007FF << 16)          /*!<R/W 0x2f  TXOP Limit An unsigned integer in units of 32 microseconds. Value of zero indicates that a single MSDU or MMPDU in addition to a protection frame exchange can be transmitted at any rate. Default: BK = 0h; BE = 0h; VI = 5Eh; VO = 2Fh. */
#define BIT_TXOPLIMIT_VO(x)                      ((u32)(((x) & 0x000007FF) << 16))
#define BIT_GET_TXOPLIMIT_VO(x)                  ((u32)(((x >> 16) & 0x000007FF)))
#define BIT_MASK_CW_VO                           ((u32)0x000000FF << 8)          /*!<R/W 0x32  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_CW_VO(x)                             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_CW_VO(x)                         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_AIFS_VO                         ((u32)0x000000FF << 0)          /*!<R/W 0x24  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_AIFS_VO(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_AIFS_VO(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EDCA_VI_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXOPLIMIT_VI                    ((u32)0x000007FF << 16)          /*!<R/W 0x5e  TXOP Limit An unsigned integer in units of 32 microseconds. Value of zero indicates that a single MSDU or MMPDU in addition to a protection frame exchange can be transmitted at any rate. Default: BK = 0h; BE = 0h; VI = 5Eh; VO = 2Fh. */
#define BIT_TXOPLIMIT_VI(x)                      ((u32)(((x) & 0x000007FF) << 16))
#define BIT_GET_TXOPLIMIT_VI(x)                  ((u32)(((x >> 16) & 0x000007FF)))
#define BIT_MASK_CW_VI                           ((u32)0x000000FF << 8)          /*!<R/W 0x43  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_CW_VI(x)                             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_CW_VI(x)                         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_AIFS_VI                         ((u32)0x000000FF << 0)          /*!<R/W 0x26  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_AIFS_VI(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_AIFS_VI(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EDCA_BE_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXOPLIMIT_BE                    ((u32)0x000007FF << 16)          /*!<R/W 0x00  TXOP Limit An unsigned integer in units of 32 microseconds. Value of zero indicates that a single MSDU or MMPDU in addition to a protection frame exchange can be transmitted at any rate. Default: BK = 0h; BE = 0h; VI = 5Eh; VO = 2Fh. */
#define BIT_TXOPLIMIT_BE(x)                      ((u32)(((x) & 0x000007FF) << 16))
#define BIT_GET_TXOPLIMIT_BE(x)                  ((u32)(((x >> 16) & 0x000007FF)))
#define BIT_MASK_CW_BE                           ((u32)0x000000FF << 8)          /*!<R/W 0xa4  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_CW_BE(x)                             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_CW_BE(x)                         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_AIFS_BE                         ((u32)0x000000FF << 0)          /*!<R/W 0x2b  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_AIFS_BE(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_AIFS_BE(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EDCA_BK_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXOPLIMIT_BK                    ((u32)0x000007FF << 16)          /*!<R/W 0x00  TXOP Limit An unsigned integer in units of 32 microseconds. Value of zero indicates that a single MSDU or MMPDU in addition to a protection frame exchange can be transmitted at any rate. Default: BK = 0h; BE = 0h; VI = 5Eh; VO = 2Fh. */
#define BIT_TXOPLIMIT_BK(x)                      ((u32)(((x) & 0x000007FF) << 16))
#define BIT_GET_TXOPLIMIT_BK(x)                  ((u32)(((x >> 16) & 0x000007FF)))
#define BIT_MASK_CW_BK                           ((u32)0x000000FF << 8)          /*!<R/W 0xa5  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_CW_BK(x)                             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_CW_BK(x)                         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_AIFS_BK                         ((u32)0x000000FF << 0)          /*!<R/W 0x49  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_AIFS_BK(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_AIFS_BK(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_CFG_PIFS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RDG_PIFS                        ((u32)0x000000FF << 24)          /*!<R/W 0x28  PIFS time. It is only used in RDG mode and in units of us. */
#define BIT_RDG_PIFS(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_RDG_PIFS(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_PIFS                            ((u32)0x000000FF << 16)          /*!<R/W 0x1c  PIFS time. It is only used in AP mode and in units of us. */
#define BIT_PIFS(x)                              ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PIFS(x)                          ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_BCNCW_MAX                       ((u32)0x0000000F << 12)          /*!<R/W 0x4  Beacon Contention Window MAX Defines the contention window value for back off before transmitting beacon frame. This field is set to 0 in AP mode to indicate no back off window. In ad hoc mode, its default value is 6. 26-1 = 63 time slots. */
#define BIT_BCNCW_MAX(x)                         ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_BCNCW_MAX(x)                     ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_BCNCW_MIN                       ((u32)0x0000000F << 8)          /*!<R/W 0x4  Beacon Contention Window Defines the contention window value for back off before transmitting beacon frame. This field is set to 0 in AP mode to indicate no back off window. In ad hoc mode, its default value is 6. 26-1 = 63 time slots. */
#define BIT_BCNCW_MIN(x)                         ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_BCNCW_MIN(x)                     ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_BCNIFS                          ((u32)0x000000FF << 0)          /*!<R/W 0x13  Beacon Interframe Space In units of microseconds, this register defines the medium idle time required before transmitting beacon (AP mode) or entering random back off window. */
#define BIT_BCNIFS(x)                            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_BCNIFS(x)                        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SIFS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SIFS_OFDM_TRX                   ((u32)0x000000FF << 24)          /*!<R/W 0xE  This register sets the SIFS time required in NAV protection period after receiving any immediate response. for example Ack-Data or RTS/CTS-Data. It is in unit of us. */
#define BIT_SIFS_OFDM_TRX(x)                     ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_SIFS_OFDM_TRX(x)                 ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_SIFS_CCK_TRX                    ((u32)0x000000FF << 16)          /*!<R/W 0xA  This register sets the SIFS time required in NAV protection period after receiving any immediate response. for example Ack-Data or RTS/CTS-Data. It is in unit of us. */
#define BIT_SIFS_CCK_TRX(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_SIFS_CCK_TRX(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_SIFS_OFDM_CTX                   ((u32)0x000000FF << 8)          /*!<R/W 0xE  This register sets the SIFS time required for consecutive TX events for example CTS2S-Data, or broadcast bursting. It is in unit of us. */
#define BIT_SIFS_OFDM_CTX(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_SIFS_OFDM_CTX(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_SIFS_CCK_CTX                    ((u32)0x000000FF << 0)          /*!<R/W 0xA  This register sets the SIFS time required for consecutive TX events for example CTS2S-Data, or broadcast bursting. It is in unit of us. */
#define BIT_SIFS_CCK_CTX(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_SIFS_CCK_CTX(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSFT_SYN_OFFSET_SLOT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SLOTTIME                        ((u32)0x000000FF << 24)          /*!<R/W 0x09  This register sets the slot time value mentioned in 802.11 specification. It is in units of us. Default: 0x9 */
#define BIT_SLOTTIME(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_SLOTTIME(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_AGGR_BK_TIME                    ((u32)0x000000FF << 16)          /*!<R/W 0x14  Hw will break aggr. In sifs condition */
#define BIT_AGGR_BK_TIME(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_AGGR_BK_TIME(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_TSFTR_SYNC_OFFSET               ((u32)0x0000FFFF << 0)          /*!<R/W 0x640  tsft sync. Offset (bcn0 sync to bcn1 offset or bcn1 sync to bcn0 offset). Unit : 32us, default is 50ms */
#define BIT_TSFTR_SYNC_OFFSET(x)                 ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TSFTR_SYNC_OFFSET(x)             ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_NOA_ERLY_TIME
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_OFF_ERLY_TIME               ((u32)0x000000FF << 8)          /*!<R/W 0x5  NOA off early interval, Time unit is TU. */
#define BIT_NOA_OFF_ERLY_TIME(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_NOA_OFF_ERLY_TIME(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_NOA_ON_ERLY_TIME                ((u32)0x000000FF << 0)          /*!<R/W 0x0  not valid. NOA on early interval, Time unit is TU. PS: This function has some issue, so it's value must be 0 */
#define BIT_NOA_ON_ERLY_TIME(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_NOA_ON_ERLY_TIME(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXPAUSE_TXPTCL_DISTXREQ_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DIS_BT_CCA                           ((u32)0x00000001 << 31)          /*!<R/W 0x0  Disable BT_CCA. (NOT USED, SEE 0x1594) */
#define BIT_DIS_TXREQ_CLR_CPUMGQ                 ((u32)0x00000001 << 30)          /*!<R/W 0x0  1: if this queue can’t own channel ,txcmd will be cleared 0:Only TX_finish can clear txreq */
#define BIT_DIS_TXREQ_CLR_HI                     ((u32)0x00000001 << 29)          /*!<R/W 0x0  0: if this queue can’t own channel ,txcmd will be cleared 1:Only TX_finish can clear txreq */
#define BIT_DIS_TXREQ_CLR_MGQ                    ((u32)0x00000001 << 28)          /*!<R/W 0x0  1: if this queue can’t own channel ,txcmd will be cleared 0:Only TX_finish can clear txreq */
#define BIT_DIS_TXREQ_CLR_BK                     ((u32)0x00000001 << 27)          /*!<R/W 0x0  0: if this queue can’t own channel ,txcmd will be cleared 1:Only TX_finish can clear txreq */
#define BIT_DIS_TXREQ_CLR_BE                     ((u32)0x00000001 << 26)          /*!<R/W 0x0  0: if this queue can’t own channel ,txcmd will be cleared 1:Only TX_finish can clear txreq */
#define BIT_DIS_TXREQ_CLR_VI                     ((u32)0x00000001 << 25)          /*!<R/W 0x0  0: if this queue can’t own channel ,txcmd will be cleared 1:Only TX_finish can clear txreq */
#define BIT_DIS_TXREQ_CLR_VO                     ((u32)0x00000001 << 24)          /*!<R/W 0x0  0: if this queue can’t own channel ,txcmd will be cleared 1:Only TX_finish can clear txreq */
#define BIT_STOP_CPUMGQ                          ((u32)0x00000001 << 23)          /*!<R/W 0x0  To control CPUMGQ queues contention circuit. 1: CPUMGQ queues all stop. 0: Enable CPUMGQ queues contention circuit */
#define BIT_MAC_STOPBCNQ                         ((u32)0x00000001 << 22)          /*!<R/W 0x0  Stop BCNQ Queue Write a 1 to this bit via software to immediately stop the transmission of packet from this queue including retry attempts. */
#define BIT_MAC_STOPHIQ                          ((u32)0x00000001 << 21)          /*!<R/W 0x0  Stop High Queue Write a 1 to this bit via software to immediately stop the transmission of packet from this queue including retry attempts. */
#define BIT_MAC_STOPMGQ                          ((u32)0x00000001 << 20)          /*!<R/W 0x0  Stop Management Queue Write a 1 to this bit via software to immediately stop the transmission of packet from this queue including retry attempts. */
#define BIT_MAC_STOPBK                           ((u32)0x00000001 << 19)          /*!<R/W 0x0  Stop BK Queue Write a 1 to this bit via software to immediately stop the transmission of packet from this queue including retry attempts. */
#define BIT_MAC_STOPBE                           ((u32)0x00000001 << 18)          /*!<R/W 0x0  Stop BE Queue Write a 1 to this bit via software to immediately stop the transmission of packet from this queue including retry attempts. */
#define BIT_MAC_STOPVI                           ((u32)0x00000001 << 17)          /*!<R/W 0x0  Stop VI Queue Write a 1 to this bit via software to immediately stop the transmission of packet from this queue including retry attempts. */
#define BIT_MAC_STOPVO                           ((u32)0x00000001 << 16)          /*!<R/W 0x0  Stop VO Queue Write a 1 to this bit via software to immediately stop the transmission of packet from this queue including retry attempts. */
#define BIT_DIS_EDCCA                            ((u32)0x00000001 << 15)          /*!<R/W 0x0  Disable EDCCA function. (NOT USED, SEE 0x1594) 0: PHY_EDCCA can block TX (default) 1: PHY_EDCCA cannot block TX OK -> TX beacon; X -> cannot TX beacon 0x528[7] 0x520[15] 0x520[14] TX bcn TX bcn (OFDMCCA=0, (OFDMCCA=1, EDCCA=1) EDCCA=1) 0 0 0 X X 0 0 1 X X 0 1 0 OK X 0 1 1 OK OK 1 0 0 X X 1 0 1 X X 1 1 0 OK X 1 1 1 OK OK */
#define BIT_DIS_CCA                              ((u32)0x00000001 << 14)          /*!<R/W 0x0  Disable CCA. (NOT USED, SEE 0x1594) */
#define BIT_LSIG_TXOP_TXCMD_NAV                  ((u32)0x00000001 << 13)          /*!<R/W 0x1  Set LSIG_TXOP from TXCMD_NAV field */
#define BIT_SIFS_BK_EN                           ((u32)0x00000001 << 12)          /*!<R/W 0x1  SIFS Break Check Criteria enable for AMPDU aggregation */
#define BIT_MASK_TXQ_NAV_MSK                     ((u32)0x0000000F << 8)          /*!<R/W 0xF  1: Queue BKF must wait TX Nav finished 0: Queue not need wait */
#define BIT_TXQ_NAV_MSK(x)                       ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_TXQ_NAV_MSK(x)                   ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_DIS_CW                               ((u32)0x00000001 << 7)          /*!<R/W 0x0  When this bit is set, disable CW. */
#define BIT_NAV_END_TXOP                         ((u32)0x00000001 << 6)          /*!<R/W 0x0  1: In txop mode, Nav protect all txop 0: In txop mode, Nav protect to next pkt */
#define BIT_RDG_END_TXOP                         ((u32)0x00000001 << 5)          /*!<R/W 0x0  1: In tx RDG mode, Nav protect all txop 0: In tx RDG mode, Nav protect to next pkt */
#define BIT_MGTQ_TXOP_EN                         ((u32)0x00000001 << 3)          /*!<R/W 0x1  When this bit is set, MGTQ may sent packets by using ACQ TXOP. */
#define BIT_MGTQ_RTSMF_EN                        ((u32)0x00000001 << 2)          /*!<R/W 0x1  When this bit is set, MTGQ will be TX, not wait until RTS or MF transaction finished. */
#define BIT_HIQ_RTSMF_EN                         ((u32)0x00000001 << 1)          /*!<R/W 0x1  When this bit is set, HIQ will be TX, not wait until RTS or MF transaction finished. */
#define BIT_BCN_RTSMF_EN                         ((u32)0x00000001 << 0)          /*!<R/W 0x1  When this bit is set, BCNQ will be TX, not wait until RTS or MF transaction finished. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA0_MBSSID_RD_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_P2P_NOA0_ALLSTASLEEP                 ((u32)0x00000001 << 31)          /*!<R/W 0x0  It indicates that the state from CTWindow End to BcnEarly 1: Sleep 0: Wake */
#define BIT_P2P_NOA0_DISTX_EN                    ((u32)0x00000001 << 30)          /*!<R/W 0x0  1: Hw will disable TX in P2P_off period 0: Do Nothing */
#define BIT_P2P_NOA0_PWR_MGT_EN                  ((u32)0x00000001 << 29)          /*!<R/W 0x0  A main switch for P2P PowerSaving 1: Enable 0: Disable */
#define BIT_MASK_P2P_NOA0_EN                     ((u32)0x00000003 << 27)          /*!<R/W 0x0  The condition that noa_unit0, [1]-NOA1, [0]-NOA0 1: Enable 0: Disable */
#define BIT_P2P_NOA0_EN(x)                       ((u32)(((x) & 0x00000003) << 27))
#define BIT_GET_P2P_NOA0_EN(x)                   ((u32)(((x >> 27) & 0x00000003)))
#define BIT_MASK_P2P_NOA0_UNIT_SEL               ((u32)0x00000003 << 25)          /*!<R/W 0x0  The selector for the source of noa unit0. 0 for port 0, 1 for port 1,2 for port2 */
#define BIT_P2P_NOA0_UNIT_SEL(x)                 ((u32)(((x) & 0x00000003) << 25))
#define BIT_GET_P2P_NOA0_UNIT_SEL(x)             ((u32)(((x >> 25) & 0x00000003)))
#define BIT_EN_TSFBIT32_RST_NOA0                 ((u32)0x00000001 << 24)          /*!<R/W 0x1  when enable, if tsft_bit32_int is asserted, reset power_cs of noa_unit0 */
#define BIT_MBID_BCNQ7_EN                        ((u32)0x00000001 << 23)          /*!<R/W 0x1  When enable, auto MBSSID for virtual interface 7 is enable. */
#define BIT_MBID_BCNQ6_EN                        ((u32)0x00000001 << 22)          /*!<R/W 0x1  When enable, auto MBSSID for virtual interface 6 is enable. */
#define BIT_MBID_BCNQ5_EN                        ((u32)0x00000001 << 21)          /*!<R/W 0x1  When enable, auto MBSSID for virtual interface 5 is enable. */
#define BIT_MBID_BCNQ4_EN                        ((u32)0x00000001 << 20)          /*!<R/W 0x1  When enable, auto MBSSID for virtual interface 4 is enable. */
#define BIT_MBID_BCNQ3_EN                        ((u32)0x00000001 << 19)          /*!<R/W 0x1  When enable, auto MBSSID for virtual interface 3 is enable. */
#define BIT_MBID_BCNQ2_EN                        ((u32)0x00000001 << 18)          /*!<R/W 0x1  When enable, auto MBSSID for virtual interface 2 is enable. */
#define BIT_MBID_BCNQ1_EN                        ((u32)0x00000001 << 17)          /*!<R/W 0x1  When enable, auto MBSSID for virtual interface 1 is enable. */
#define BIT_MBID_BCNQ0_EN                        ((u32)0x00000001 << 16)          /*!<R/W 0x1  When enable, auto MBSSID for root interface is enable. */
#define BIT_EN_CLR_TXREQ_INCCA                   ((u32)0x00000001 << 15)          /*!<R/W 0x1  not valid。 */
#define BIT_DIS_TX_OVER_BCNQ                     ((u32)0x00000001 << 14)          /*!<R/W 0x0  1: Tx Packet with ignoring Beacon Area time 0: Disable */
#define BIT_EN_BCNERR_INCCCA                     ((u32)0x00000001 << 13)          /*!<R/W 0x0  eneble force tx becon ignore CCA in disable contention area */
#define BIT_EN_BCNERR_INEDCCA                    ((u32)0x00000001 << 12)          /*!<R/W 0x0  eneble force tx becon ignore EDCCA in disable contention area */
#define BIT_EDCCA_MSK_CNTDOWN_EN                 ((u32)0x00000001 << 11)          /*!<R/W 0x1  1: disable EDCCA count down, 0: enable EDCCA count down AC backoff 是否需要參考PHY_EDCCA 0: 不參考PHY_EDCCA 1: 參考PHY_EDCCA (default, PHY_EDCCA=1時停止backoff) */
#define BIT_DIS_TXOP_CFE                         ((u32)0x00000001 << 10)          /*!<R/W 0x0  Disable TXOP_CFE function */
#define BIT_DIS_LSIG_CFE                         ((u32)0x00000001 << 9)          /*!<R/W 0x0  Disable LSIG_TXOP CF-END function */
#define BIT_DIS_STBC_CFE                         ((u32)0x00000001 << 8)          /*!<R/W 0x0  Disable STBC CF-END function */
#define BIT_BKQ_RD_INIT_EN                       ((u32)0x00000001 << 7)          /*!<R/W 0x0  Enable BKQ RD initiator function */
#define BIT_BEQ_RD_INIT_EN                       ((u32)0x00000001 << 6)          /*!<R/W 0x0  Enable BEQ RD initiator function */
#define BIT_VIQ_RD_INIT_EN                       ((u32)0x00000001 << 5)          /*!<R/W 0x0  Enable VIQ RD initiator function */
#define BIT_VOQ_RD_INIT_EN                       ((u32)0x00000001 << 4)          /*!<R/W 0x0  Enable VOQ RD initiator function */
#define BIT_BKQ_RD_RESP_EN                       ((u32)0x00000001 << 3)          /*!<R/W 0x1  Enable BKQ RD responder function */
#define BIT_BEQ_RD_RESP_EN                       ((u32)0x00000001 << 2)          /*!<R/W 0x1  Enable BEQ RD responder function */
#define BIT_VIQ_RD_RESP_EN                       ((u32)0x00000001 << 1)          /*!<R/W 0x1  Enable VIQ RD responder function */
#define BIT_VOQ_RD_RESP_EN                       ((u32)0x00000001 << 0)          /*!<R/W 0x1  Enable VOQ RD responder function */
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA0_PKT_LIFETIME_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_POWER_STATE_0                        ((u32)0x00000001 << 31)          /*!<RO   1: On, 0: Off. (It is including early time. ) */
#define BIT_CTWINDOW_ON_0                        ((u32)0x00000001 << 30)          /*!<RO   Spec time and Beacon Early Time to TBTT Time. (It is including early time. ) */
#define BIT_BEACON_AREA_ON_0                     ((u32)0x00000001 << 29)          /*!<RO   The time is from HOLD Time to BeaconTx or Timeout. (It is including early time. ) */
#define BIT_CTWIN_EARLY_DISTX_0                  ((u32)0x00000001 << 28)          /*!<RO   ctwin_early_distx. (It is including early time. ) */
#define BIT_NOA1_OFF_PERIOD_0                    ((u32)0x00000001 << 27)          /*!<RO   Spec time but exclude BCNDMATIM time for wake. (It is including early time. ) */
#define BIT_FORCE_DOZE1_0                        ((u32)0x00000001 << 26)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. (It is including early time. ) */
#define BIT_NOA0_OFF_PERIOD_0                    ((u32)0x00000001 << 25)          /*!<RO   Spec time but exclude BCNDMATIM time for wake. (It is including early time. ) */
#define BIT_FORCE_DOZE0_0                        ((u32)0x00000001 << 24)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. (It is including early time. ) */
#define BIT_CHANGE_POW_BCN_AREA                  ((u32)0x00000001 << 23)          /*!<R/W 0x0  select real_ctwindow & real_bcnq_area for noa_unit */
#define BIT_EN_BKF_CLR_TXREQ                     ((u32)0x00000001 << 22)          /*!<R/W 0x1  when gen tx command done, while will backoff is invalid, then tx command will be cleared */
#define BIT_EN_BCN_TX_BTCCA                      ((u32)0x00000001 << 20)          /*!<R/W 0x0  1: Enable Beacon Tx at TBTT if BT_CCA is enabled 0: Disable */
#define BIT_DIS_PKT_TX_ATIM                      ((u32)0x00000001 << 19)          /*!<R/W 0x1  1: Disable AC Queue Packet Tx in ATIM (only valid at Nettype: Adhoc/AP) 0: Enable AC Queue packet Tx in ATIM */
#define BIT_DIS_BCN_DIS_CTN                      ((u32)0x00000001 << 18)          /*!<R/W 0x1  Disable the Beacon queue disable contention,when set to 0 enable the force beacon function. */
#define BIT_EN_NAVEND_RST_TXOP                   ((u32)0x00000001 << 17)          /*!<R/W 0x0  When set this bit, TXOP will be reset while NAV end. */
#define BIT_EN_FILTER_CCA                        ((u32)0x00000001 << 16)          /*!<R/W 0x0  Enable CCA filter threshold */
#define BIT_MASK_CCA_FILTER_THRS                 ((u32)0x000000FF << 8)          /*!<R/W 0xA  If PHY_CCA signal width is bigger than this threshold, this PHY_CCA signal is valid for MAC. Unit: us */
#define BIT_CCA_FILTER_THRS(x)                   ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_CCA_FILTER_THRS(x)               ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_EDCCA_THRS                      ((u32)0x000000FF << 0)          /*!<R/W 0x8F  EDCCA threshold value, edcca over this threshold will cause bkf_rst Bit[7]: PHY_EDCCA為1時,當timer timeout時擋住TX Bit[6:0]: timer (單位:32us) */
#define BIT_EDCCA_THRS(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_EDCCA_THRS(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P2PON_DIS_TXTIME
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P2PON_DIS_TXTIME                ((u32)0x000000FF << 8)          /*!<R/W 0x0  The time of the disable TX period after the P2P RF on interrupt occur. Unit: 64us. */
#define BIT_P2PON_DIS_TXTIME(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_P2PON_DIS_TXTIME(x)              ((u32)(((x >> 8) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAN_SETTING_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SYNC_BCN_DELAY                  ((u32)0x00003FFF << 0)          /*!<R/W 0x168  r_HC * r_SLOTTIME * 8'd40 (unit:us) default: r_HC = 1, r_SLOTTIME = 9, => 360us max is 16ms (DW) */
#define BIT_SYNC_BCN_DELAY(x)                    ((u32)(((x) & 0x00003FFF) << 0))
#define BIT_GET_SYNC_BCN_DELAY(x)                ((u32)(((x >> 0) & 0x00003FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_QUEUE_INCOL_THR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BK_QUEUE_THR                    ((u32)0x000000FF << 24)          /*!<R/W 0x0  0: Disable this BK queue condition. Otherwise: When internal collision number of queue equal threshold, the priority of queue enhance. But have more 2 queues which satisfy each queue threshold, the priority will follow VO>VI>BE>BK. */
#define BIT_BK_QUEUE_THR(x)                      ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_BK_QUEUE_THR(x)                  ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_BE_QUEUE_THR                    ((u32)0x000000FF << 16)          /*!<R/W 0x0  0: Disable this BE queue condition. Otherwise: When internal collision number of queue equal threshold, the priority of queue enhance. But have more 2 queues which satisfy each queue threshold, the priority will follow VO>VI>BE>BK. */
#define BIT_BE_QUEUE_THR(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_BE_QUEUE_THR(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_VI_QUEUE_THR                    ((u32)0x000000FF << 8)          /*!<R/W 0x0  0: Disable this VI queue condition. Otherwise: When internal collision number of queue equal threshold, the priority of queue enhance. But have more 2 queues which satisfy each queue threshold, the priority will follow VO>VI>BE>BK. */
#define BIT_VI_QUEUE_THR(x)                      ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_VI_QUEUE_THR(x)                  ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_VO_QUEUE_THR                    ((u32)0x000000FF << 0)          /*!<R/W 0x0  0: Disable this VO queue condition. Otherwise: When internal collision number of queue equal threshold, the priority of queue enhance. But have more 2 queues which satisfy each queue threshold, the priority will follow VO>VI>BE>BK. */
#define BIT_VO_QUEUE_THR(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_VO_QUEUE_THR(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_QUEUE_INCOL_EN
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DIS_ZGBCCA                           ((u32)0x00000001 << 31)          /*!<R/W 0x0  Disable zigbee CCA (NOT USED, SEE 0x1594) */
#define BIT_MASK_TXOP_FAILBREAK_CNT              ((u32)0x000000FF << 22)          /*!<R 0x0  if BIT_TXOP_FAIL_BREAK = 1, counter the times of txfail break TXOP */
#define BIT_TXOP_FAILBREAK_CNT(x)                ((u32)(((x) & 0x000000FF) << 22))
#define BIT_GET_TXOP_FAILBREAK_CNT(x)            ((u32)(((x >> 22) & 0x000000FF)))
#define BIT_TXOP_FAIL_BREAK                      ((u32)0x00000001 << 21)          /*!<R/W 0x0  1: if tx pkt fail in txop, then tx cfend to end txop */
#define BIT_EN_TBTT_AREA_BLK_4AC                 ((u32)0x00000001 << 20)          /*!<R/W 0x1  When set this bit, 4AC backoff is disabled on TBTT setup period */
#define BIT_ENABLE_RANDOM_SHIFT_TX               ((u32)0x00000001 << 19)          /*!<R/W 0x0  When this bit set 1, enable random shift TX function */
#define BIT_ENABLE_EDCA_REF_FUNCTION             ((u32)0x00000001 << 18)          /*!<R/W 0x0  When this bit set 1, enable EDCA REF function */
#define BIT_ENABLE_INTERCOL_EDCA_REF             ((u32)0x00000001 << 17)          /*!<R/W 0x0  When this bit set 1, enable inter-collision edca_ref */
#define BIT_ACQ_MAX_INCOL_EN                     ((u32)0x00000001 << 16)          /*!<R/W 0x0  1:Enable 0 :Disable */
#define BIT_MASK_BK_TRIGGER_NUM                  ((u32)0x0000000F << 12)          /*!<R/W1C 0x0  The number is triggered by bkq inter collision */
#define BIT_BK_TRIGGER_NUM(x)                    ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_BK_TRIGGER_NUM(x)                ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_BE_TRIGGER_NUM                  ((u32)0x0000000F << 8)          /*!<R/W1C 0x0  The number is triggered by beq inter collision */
#define BIT_BE_TRIGGER_NUM(x)                    ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_BE_TRIGGER_NUM(x)                ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_VI_TRIGGER_NUM                  ((u32)0x0000000F << 4)          /*!<R/W1C 0x0  The number is triggered by viq inter collision */
#define BIT_VI_TRIGGER_NUM(x)                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_VI_TRIGGER_NUM(x)                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_VO_TRIGGER_NUM                  ((u32)0x0000000F << 0)          /*!<R/W1C 0x0  The number is triggered by voq inter collision */
#define BIT_VO_TRIGGER_NUM(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_VO_TRIGGER_NUM(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA0_TBTT_PROHIBIT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_SPEC_POWER_STATE_0                   ((u32)0x00000001 << 31)          /*!<RO   1: On, 0: Off */
#define BIT_SPEC_CTWINDOW_ON_0                   ((u32)0x00000001 << 30)          /*!<RO   The time from TBTT <-> CTWEND (with BCNDMATIM early time) */
#define BIT_SPEC_BEACON_AREA_ON_0                ((u32)0x00000001 << 29)          /*!<RO   The time is from TBTT to BeaconTx or Timeout */
#define BIT_SPEC_CTWIN_EARLY_DISTX_0             ((u32)0x00000001 << 28)          /*!<RO   ctwin_early_distx */
#define BIT_SPEC_NOA1_OFF_PERIOD_0               ((u32)0x00000001 << 27)          /*!<RO   Spec time */
#define BIT_SPEC_FORCE_DOZE1_0                   ((u32)0x00000001 << 26)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. */
#define BIT_SPEC_NOA0_OFF_PERIOD_0               ((u32)0x00000001 << 25)          /*!<RO   Spec time */
#define BIT_SPEC_FORCE_DOZE0_0                   ((u32)0x00000001 << 24)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. */
#define BIT_EN_TBTT_AREA_FOR_BB                  ((u32)0x00000001 << 20)          /*!<R/W 0x0  enable notify bb that mac p0 is in tbtt area */
#define BIT_MASK_TBTT_HOLD_TIME                  ((u32)0x00000FFF << 8)          /*!<R/W 0xc8  TBTT_prohibit_hold time for AP mode, unit is 32 us */
#define BIT_TBTT_HOLD_TIME(x)                    ((u32)(((x) & 0x00000FFF) << 8))
#define BIT_GET_TBTT_HOLD_TIME(x)                ((u32)(((x >> 8) & 0x00000FFF)))
#define BIT_MASK_TBTT_HOLD_TIME_INFRA            ((u32)0x0000000F << 4)          /*!<R/W 0x01  TBTT_prohibit_hold time for infrastructure mode , unit is 2 ms */
#define BIT_TBTT_HOLD_TIME_INFRA(x)              ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_TBTT_HOLD_TIME_INFRA(x)          ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_TBTT_PROHIBIT_SETUP             ((u32)0x0000000F << 0)          /*!<R/W 0x02  TBTT_prohibit_setup, unit is 32us */
#define BIT_TBTT_PROHIBIT_SETUP(x)               ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_TBTT_PROHIBIT_SETUP(x)           ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAV_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NAV_PROT_LEN                    ((u32)0x0000FFFF << 16)          /*!<R/W 0x40  In TXOP mode ,NAV setting which protects next packets */
#define BIT_NAV_PROT_LEN(x)                      ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_NAV_PROT_LEN(x)                  ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_RD_NAV_PROT_NXT                 ((u32)0x0000FFFF << 0)          /*!<R/W 0x180  In RDG mode ,NAV setting which protects next packets */
#define BIT_RD_NAV_PROT_NXT(x)                   ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_RD_NAV_PROT_NXT(x)               ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FTM_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FTM_TSF_R2T_PORT                     ((u32)0x00000001 << 24)          /*!<R/W 0x0  0:R2T latch count sync tsf of port 0 1: R2T latch count sync tsf of port 1 */
#define BIT_FTM_TSF_T2R_PORT                     ((u32)0x00000001 << 20)          /*!<R/W 0x0  0:T2R latch count sync tsf of port 0 1: T2R latch count sync tsf of port 1 */
#define BIT_FTM_PTT_PORT                         ((u32)0x00000001 << 16)          /*!<R/W 0x0  0: sync tsf of port 0 1: sync tsf of port 1 */
#define BIT_MASK_FTM_PTT                         ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  partial tsf timer setting */
#define BIT_FTM_PTT(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_FTM_PTT(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FTM_TSF_CNT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FTM_TSF_R2T                     ((u32)0x0000FFFF << 16)          /*!<R 0x0  R2T latch count sync tsf timer */
#define BIT_FTM_TSF_R2T(x)                       ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_FTM_TSF_R2T(x)                   ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_FTM_TSF_T2R                     ((u32)0x0000FFFF << 0)          /*!<R 0x0  T2R latch count sync tsf timer */
#define BIT_FTM_TSF_T2R(x)                       ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_FTM_TSF_T2R(x)                   ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSTRST_P2PRST_BCN_CTRL_PORT0_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_P2P_PWR_RST1                         ((u32)0x00000001 << 30)          /*!<W 0x0  rst p2p circuit NOA 1, noa_unit0 */
#define BIT_SCHEDULER_RST                        ((u32)0x00000001 << 29)          /*!<W 0x0  Write this bit reset scheduler, ioreg_txfail */
#define BIT_P2P_PWR_RST0                         ((u32)0x00000001 << 28)          /*!<W 0x0  rst p2p circuit NOA 0, noa_unit0 */
#define BIT_TSFTR1_SYNC_EN                       ((u32)0x00000001 << 27)          /*!<W 0x0  write this bit, bcn1 will sync to bcn0 with TSFTF_SYN_OFFSET */
#define BIT_TSFTR_SYNC_EN                        ((u32)0x00000001 << 26)          /*!<W 0x0  write this bit, bcn0 will sync to bcn1 with TSFTF_SYN_OFFSET */
#define BIT_TSFTR1_RST                           ((u32)0x00000001 << 25)          /*!<W 0x0  write this bit , bcn1 tsft will auto reset */
#define BIT_TSFTR_RST                            ((u32)0x00000001 << 24)          /*!<W 0x0  write this bit , bcn0 tsft will auto reset */
#define BIT_P2P_PWR_RST5                         ((u32)0x00000001 << 23)          /*!<W 0x0  rst p2p circuit NOA 5, noa_unit2 */
#define BIT_P2P_PWR_RST4                         ((u32)0x00000001 << 22)          /*!<W 0x0  rst p2p circuit NOA 4, noa_unit2 */
#define BIT_P2P_PWR_RST3                         ((u32)0x00000001 << 21)          /*!<W 0x0  rst p2p circuit NOA 3, noa_unit1 */
#define BIT_P2P_PWR_RST2                         ((u32)0x00000001 << 20)          /*!<W 0x0  rst p2p circuit NOA 2, noa_unit1 */
#define BIT_EN_PRE_DL_BEACON                     ((u32)0x00000001 << 19)          /*!<R/W 0x0  If set 1, hw will enable pre_download beacon */
#define BIT_MASK_MBID_BCN_NUM                    ((u32)0x00000007 << 16)          /*!<R/W 0x0  The number of virtual interface number excluding the root number. */
#define BIT_MBID_BCN_NUM(x)                      ((u32)(((x) & 0x00000007) << 16))
#define BIT_GET_MBID_BCN_NUM(x)                  ((u32)(((x >> 16) & 0x00000007)))
#define BIT_P1_RXBCN_RPT_ENABLE                  ((u32)0x00000001 << 15)          /*!<R/W 0x0  When this bit asserted, RXBCNOK report is issued */
#define BIT_P1_DIS_RX_BSSID_FIT                  ((u32)0x00000001 << 14)          /*!<R/W 0x0  0:HW will handle rx_beacon 1:HW will ignore rx_beacon_pk */
#define BIT_P1_TXBCN_RPT_ENABLE                  ((u32)0x00000001 << 13)          /*!<R/W 0x0  When this bit and BIT_P1_EN_BCN_RPT are set, then TXBCN ERROR report is issued */
#define BIT_P1_DIS_TSF_UDT                       ((u32)0x00000001 << 12)          /*!<R/W 0x1  When set, disable clint 0 TSF update when beacon frames or probe response frames are received. */
#define BIT_P1_EN_BCN_FUNCTION                   ((u32)0x00000001 << 11)          /*!<R/W 0x0  When this bit is set, TSF and other beacon related functions are then enabled. 0x700(MACID1)的enable開關 */
#define BIT_P1_EN_BCN_RPT                        ((u32)0x00000001 << 10)          /*!<R/W 0x1  When this bit asserted, txbcnok_int and txbcnerr_int will valid. */
#define BIT_P1_ENP2P_CTWINDOW                    ((u32)0x00000001 << 9)          /*!<R/W 0x0  1: Enable P2P CTN WINDOWS function. 0: Disable P2P CTN WINDOWS function. */
#define BIT_P1_ENP2P_BCNQ_AREA                   ((u32)0x00000001 << 8)          /*!<R/W 0x0  1: Enable P2P BCN area on function. 0 : Disable P2P BCN area on function. */
#define BIT_P0_RXBCN_RPT_ENABLE                  ((u32)0x00000001 << 7)          /*!<R/W 0x0  When this bit asserted, RXBCNOK report is issued */
#define BIT_P0_DIS_RX_BSSID_FIT                  ((u32)0x00000001 << 6)          /*!<R/W 0x0  0:HW will handle rx_beacon 1:HW will ignore rx_beacon_pk */
#define BIT_P0_TXBCN_RPT_ENABLE                  ((u32)0x00000001 << 5)          /*!<R/W 0x0  When this bit and BIT_P0_EN_BCN_RPT are set, then TXBCN ERROR report is issued */
#define BIT_P0_DIS_TSF_UDT                       ((u32)0x00000001 << 4)          /*!<R/W 0x1  When set, disable port 0 TSF update when beacon frames or probe response frames are received. */
#define BIT_P0_EN_BCN_FUNCTION                   ((u32)0x00000001 << 3)          /*!<R/W 0x0  When this bit is set, TSF and other beacon related functions are then enabled. 0x610(MACID)的enable開關 */
#define BIT_P0_EN_BCN_RPT                        ((u32)0x00000001 << 2)          /*!<R/W 0x1  When this bit asserted, txbcnok_int and txbcnerr_int will valid. */
#define BIT_P0_EN_P2P_CTWINDOW                   ((u32)0x00000001 << 1)          /*!<R/W 0x0  1: Enable P2P CTN WINDOWS function. 0: Disable P2P CTN WINDOWS function. */
#define BIT_P0_EN_P2P_BCNQ_AREA                  ((u32)0x00000001 << 0)          /*!<R/W 0x0  1: Enable P2P BCN area on function. 0 : Disable P2P BCN area on function. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_PORT0_BCN_SPACE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCN_SUB_SPACE_PORT0             ((u32)0x0000FFFF << 16)          /*!<R/W 0x64  Unit is ms. The sub-beacon interval of the port 0.(valid if mbssid function is enable) */
#define BIT_BCN_SUB_SPACE_PORT0(x)               ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_BCN_SUB_SPACE_PORT0(x)           ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_BCN_SPACE_PORT0                 ((u32)0x0000FFFF << 0)          /*!<R/W 0x64  Unit is ms. When MBSSID is disable, this field is the beacon interval. When MBSSID is enable, this register is the beacon space of root. */
#define BIT_BCN_SPACE_PORT0(x)                   ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BCN_SPACE_PORT0(x)               ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ATIMWND0_P0_BCN_ERLY_DMA_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ATIMWND0                        ((u32)0x0000FFFF << 16)          /*!<R/W 0x02  ATIM window width of root VAP in units of TU. */
#define BIT_ATIMWND0(x)                          ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_ATIMWND0(x)                      ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_BCNDMATIM_P0                    ((u32)0x000000FF << 8)          /*!<R/W 0x03  Indicates the time before TBTT to perform beacon queue DMA. This register is shared by the time to assert ATIM window end interrupt. Measured in units of “TU” before TBTT and before ATIM window end. */
#define BIT_BCNDMATIM_P0(x)                      ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BCNDMATIM_P0(x)                  ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_DRVERLYITV_P0                   ((u32)0x000000FF << 0)          /*!<R/W 0x05  Time to send interrupt to notify driver to change beacon content. Time unit is TU. The register also is shared by P2P-CTW and P2P-NoA (from NoA-Absent to NoA-On). It is used to wake up early at DriverEarlyTime */
#define BIT_DRVERLYITV_P0(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_DRVERLYITV_P0(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCNERR_TSF_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_OFDM_RXTSF_OFFSET               ((u32)0x000000FF << 24)          /*!<R/W 0x0F  The Datapath of OFDM RX_tsf */
#define BIT_OFDM_RXTSF_OFFSET(x)                 ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_OFDM_RXTSF_OFFSET(x)             ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_CCK_RXTSF_OFFSET                ((u32)0x000000FF << 16)          /*!<R/W 0x0F  The Datapath of CCK RX_tsf */
#define BIT_CCK_RXTSF_OFFSET(x)                  ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_CCK_RXTSF_OFFSET(x)              ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_BCN_MAX_ERR                     ((u32)0x000000FF << 8)          /*!<R/W 0x10  When beacon TX error counter goes over this threshold, Beacon will transmit within PIFS. */
#define BIT_BCN_MAX_ERR(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BCN_MAX_ERR(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_USTIME_TSF                      ((u32)0x000000FF << 0)          /*!<R/W 0x28  Set the microsecond time unit used by MAC TSF clock. 0x14 - 20MHz 0x28 - 40MHz 0x50 - 80MHz */
#define BIT_USTIME_TSF(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_USTIME_TSF(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P0_TSFTR_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P0_TSF_TIMER_L                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x1  low 32bits of port0's tsft */
#define BIT_P0_TSF_TIMER_L(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_P0_TSF_TIMER_L(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P0_TSFTR_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P0_TSF_TIMER_H                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  high 32bits of port0's tsft */
#define BIT_P0_TSF_TIMER_H(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_P0_TSF_TIMER_H(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P1_TSFTR_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P1_TSF_TIMER_L                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x1  low 32bits of port1's tsft */
#define BIT_P1_TSF_TIMER_L(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_P1_TSF_TIMER_L(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P1_TSFTR_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P1_TSF_TIMER_H                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x1  high 32bits of port1's tsft */
#define BIT_P1_TSF_TIMER_H(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_P1_TSF_TIMER_H(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_ERLY_CTW_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCNIVLCUNT_P0                   ((u32)0x0000007F << 24)          /*!<R 0x7f  Be used in multiple BCNIVL early INT. When this counter =1, multiple BCNIVL early INT will be triggered. */
#define BIT_BCNIVLCUNT_P0(x)                     ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_BCNIVLCUNT_P0(x)                 ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_CTWND_P0                        ((u32)0x000000FF << 16)          /*!<R/W 0x0a  CTWindow width in units of TU. PORT0 */
#define BIT_CTWND_P0(x)                          ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_CTWND_P0(x)                      ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_ATIMWND1                        ((u32)0x0000FFFF << 0)          /*!<R/W 0x02  ATIM window width of VAP1 in units of TU. */
#define BIT_ATIMWND1(x)                          ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_ATIMWND1(x)                      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCNDROP_TO_MISC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSF_TIMER_0_SEL                 ((u32)0x00000003 << 30)          /*!<R/W 0x0  genarete tsfime_int[0] 00: PORT0's tsft 01: PORT1's tsft 10：PORT2's tsft 11: FREERUN CUT */
#define BIT_TSF_TIMER_0_SEL(x)                   ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_TSF_TIMER_0_SEL(x)               ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_TSF_TIMER_1_SEL                 ((u32)0x00000003 << 28)          /*!<R/W 0x0  genarete tsfime_int[1] 00: PORT0's tsft 01: PORT1's tsft 10：PORT2's tsft 11: FREERUN CUT */
#define BIT_TSF_TIMER_1_SEL(x)                   ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_TSF_TIMER_1_SEL(x)               ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_TSF_TIMER_2_SEL                 ((u32)0x00000003 << 26)          /*!<R/W 0x0  genarete tsfime_int[2] 00: PORT0's tsft 01: PORT1's tsft 10：PORT2's tsft 11: FREERUN CUT */
#define BIT_TSF_TIMER_2_SEL(x)                   ((u32)(((x) & 0x00000003) << 26))
#define BIT_GET_TSF_TIMER_2_SEL(x)               ((u32)(((x >> 26) & 0x00000003)))
#define BIT_MASK_TXCMD_TIMEOUT_PERIOD            ((u32)0x000000FF << 16)          /*!<R/W 0xFF  When 40M/80M TX CMD ready and secondary_CCA_20/secondary_CCA_40 set for this period, then the TX CMD will be cleared. TU: 1us */
#define BIT_TXCMD_TIMEOUT_PERIOD(x)              ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_TXCMD_TIMEOUT_PERIOD(x)          ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_HGQ_TIMEOUT_PERIOD              ((u32)0x000000FF << 8)          /*!<R/W 0x10  When hgq empty for this period, close atim window. And this will block hgq dma for pcie, unless 0x524[12] set enable. TU: 32us */
#define BIT_HGQ_TIMEOUT_PERIOD(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_HGQ_TIMEOUT_PERIOD(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_BEACON_DROP_EN                       ((u32)0x00000001 << 7)          /*!<R/W 0  enable beacon drop function */
#define BIT_MASK_BEACON_DROP_IVL                 ((u32)0x0000007F << 0)          /*!<R/W 0  indicate the timeout of beacon drop */
#define BIT_BEACON_DROP_IVL(x)                   ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_BEACON_DROP_IVL(x)               ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P2_TSFTR_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P2_TSF_TIMER_L                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x1  low 32bits of port2's tsft */
#define BIT_P2_TSF_TIMER_L(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_P2_TSF_TIMER_L(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P2_TSFTR_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P2_TSF_TIMER_H                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x1  high 32bits of port2's tsft */
#define BIT_P2_TSF_TIMER_H(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_P2_TSF_TIMER_H(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSF_TIMER1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSTTIMER1                       ((u32)0x07FFFFFF << 5)          /*!<R/W 0x00  When the lower 4 bytes of TSFTR matches the content of this register, the FW tsftime_int[0] interrupt is issued. */
#define BIT_TSTTIMER1(x)                         ((u32)(((x) & 0x07FFFFFF) << 5))
#define BIT_GET_TSTTIMER1(x)                     ((u32)(((x >> 5) & 0x07FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSF_TIMER2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSTTIMER2                       ((u32)0x07FFFFFF << 5)          /*!<R/W 0x00  When the lower 4 bytes of TSFTR matches the content of this register, the FW tsftime_int[1] interrupt is issued. */
#define BIT_TSTTIMER2(x)                         ((u32)(((x) & 0x07FFFFFF) << 5))
#define BIT_GET_TSTTIMER2(x)                     ((u32)(((x >> 5) & 0x07FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSF_TIMER3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSTTIMER3                       ((u32)0x07FFFFFF << 5)          /*!<R/W 0x00  When the lower 4 bytes of TSFTR matches the content of this register, the FW tsftime_int[2] interrupt is issued. */
#define BIT_TSTTIMER3(x)                         ((u32)(((x) & 0x07FFFFFF) << 5))
#define BIT_GET_TSTTIMER3(x)                     ((u32)(((x >> 5) & 0x07FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_IFS_TBTT_CTN
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PRE_BCN_DMATIM                  ((u32)0x000000FF << 24)          /*!<R/W 0x06  when 0x552[3] is set, hw will enable pre_download beacon. this register Indicates the time before TBTT to perform beacon queue DMA. */
#define BIT_PRE_BCN_DMATIM(x)                    ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_PRE_BCN_DMATIM(x)                ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_FORCE_BCN_IFS                   ((u32)0x000000FF << 16)          /*!<R/W 0x20  when 0x528 Bit18 set to 0 and TBTT contention area end,force tx beacon use this IFS */
#define BIT_FORCE_BCN_IFS(x)                     ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_FORCE_BCN_IFS(x)                 ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_TBTT_CTN_AREA                   ((u32)0x000000FF << 0)          /*!<R/W 0x60  TBTT contention area,Time unit is 32us,for PORT 0 / 1 / 2 */
#define BIT_TBTT_CTN_AREA(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_TBTT_CTN_AREA(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXOP_CTRL_PRE_BKF
 * @brief
 * @{
 *****************************************************************************/
#define BIT_NOPKT_END_RTSMF                      ((u32)0x00000001 << 31)          /*!<R/W 0x1  1: enable exit rts hold mode when there is no corresponding ac pkt to tx */
#define BIT_MASK_PRETX_US                        ((u32)0x0000000F << 27)          /*!<R/W 0x6  For wep/tkip, enable security engine earlier than mactx by this amount of time. Unit is microsecond (us) */
#define BIT_PRETX_US(x)                          ((u32)(((x) & 0x0000000F) << 27))
#define BIT_GET_PRETX_US(x)                      ((u32)(((x >> 27) & 0x0000000F)))
#define BIT_DTIM_BYPASS                          ((u32)0x00000001 << 26)          /*!<R/W 0x0  1: dtim counter will keep 0 */
#define BIT_RTS_NAV_TXOP                         ((u32)0x00000001 << 25)          /*!<R/W 0x0  enable RTS NAV protect all txop */
#define BIT_NOT_CROSS_TXOP                       ((u32)0x00000001 << 24)          /*!<R/W 0x0  1:tx pkt cross tbtt is prohibited 0:tx pkt cross tbtt is permitted */
#define BIT_MASK_PRE_BKF_TIME                    ((u32)0x000000FF << 16)          /*!<R/W 0x0f  when bkf timer count down this value, sel queue and gen txcmd will start */
#define BIT_PRE_BKF_TIME(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PRE_BKF_TIME(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_NAV_BLK_HGQ                          ((u32)0x00000001 << 15)          /*!<R/W 0x0  rxnav block HGQ */
#define BIT_NAV_BLK_MGQ                          ((u32)0x00000001 << 14)          /*!<R/W 0x0  rxnav block MGQ */
#define BIT_MASK_TXOP_MIN                        ((u32)0x00003FFF << 0)          /*!<R/W 0x0f  if txop counter is more than this value, than tx pkt is considered not cross tbtt */
#define BIT_TXOP_MIN(x)                          ((u32)(((x) & 0x00003FFF) << 0))
#define BIT_GET_TXOP_MIN(x)                      ((u32)(((x >> 0) & 0x00003FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FREERUN_CNT_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FREERUN_CNT_TIMER_L             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x1  LOW 32bits of freerun cnt */
#define BIT_FREERUN_CNT_TIMER_L(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FREERUN_CNT_TIMER_L(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FREERUN_CNT_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FREERUN_CNT_TIMER_H             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x1  high 32bits of freerun cnt */
#define BIT_FREERUN_CNT_TIMER_H(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FREERUN_CNT_TIMER_H(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_ERLY_CTW_CTRL_P1_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCNIVLCUNT_P2                   ((u32)0x0000007F << 24)          /*!<R 0x7f  Be used in multiple BCNIVL early INT. When this counter =1, multiple BCNIVL early INT will be triggered. */
#define BIT_BCNIVLCUNT_P2(x)                     ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_BCNIVLCUNT_P2(x)                 ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_CTWND_P2                        ((u32)0x000000FF << 16)          /*!<R/W 0x0a  CTWindow width in units of TU. PORT2 */
#define BIT_CTWND_P2(x)                          ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_CTWND_P2(x)                      ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_BCNIVLCUNT_P1                   ((u32)0x0000007F << 8)          /*!<R 0x7f  Be used in multiple BCNIVL early INT. When this counter =1, multiple BCNIVL early INT will be triggered. */
#define BIT_BCNIVLCUNT_P1(x)                     ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_BCNIVLCUNT_P1(x)                 ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_CTWND_P1                        ((u32)0x000000FF << 0)          /*!<R/W 0x0a  CTWindow width in units of TU. PORT1 */
#define BIT_CTWND_P1(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_CTWND_P1(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ATIMWND2_5
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ATIMWND5                        ((u32)0x000000FF << 24)          /*!<R/W 0x02  ATIM window width of VAP5 in units of TU. */
#define BIT_ATIMWND5(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_ATIMWND5(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_ATIMWND4                        ((u32)0x000000FF << 16)          /*!<R/W 0x02  ATIM window width of VAP4 in units of TU. */
#define BIT_ATIMWND4(x)                          ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_ATIMWND4(x)                      ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_ATIMWND3                        ((u32)0x000000FF << 8)          /*!<R/W 0x02  ATIM window width of VAP3 in units of TU. */
#define BIT_ATIMWND3(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_ATIMWND3(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_ATIMWND2                        ((u32)0x000000FF << 0)          /*!<R/W 0x02  ATIM window width of VAP2 in units of TU. */
#define BIT_ATIMWND2(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_ATIMWND2(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ATIMWND6_7_URGENT_NOLIMTHGQ
 * @brief
 * @{
 *****************************************************************************/
#define BIT_HIQ_NO_LMT_EN_VAP7                   ((u32)0x00000001 << 31)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_HIQ_NO_LMT_EN_VAP6                   ((u32)0x00000001 << 30)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_HIQ_NO_LMT_EN_VAP5                   ((u32)0x00000001 << 29)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_HIQ_NO_LMT_EN_VAP4                   ((u32)0x00000001 << 28)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_HIQ_NO_LMT_EN_VAP3                   ((u32)0x00000001 << 27)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_HIQ_NO_LMT_EN_VAP2                   ((u32)0x00000001 << 26)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_HIQ_NO_LMT_EN_VAP1                   ((u32)0x00000001 << 25)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_HIQ_NO_LMT_EN_ROOT                   ((u32)0x00000001 << 24)          /*!<R/W 0x0  1: Packet in Hi Queue Tx immediately (No constraint for ATIM Period) 0: Disable */
#define BIT_MASK_ATIM_URGENT                     ((u32)0x000000FF << 16)          /*!<R/W 0x01  Indicates the time before ATIM Window end . When BSSID[n] (n=0~7) has HIQ packet to send, HW only DMA BSSID[n]’s HIQ packet in this interval. (unit : TU) */
#define BIT_ATIM_URGENT(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_ATIM_URGENT(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_ATIMWND7                        ((u32)0x000000FF << 8)          /*!<R/W 0x02  ATIM window width of VAP7 in units of TU. */
#define BIT_ATIMWND7(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_ATIMWND7(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_ATIMWND6                        ((u32)0x000000FF << 0)          /*!<R/W 0x02  ATIM window width of VAP6 in units of TU. */
#define BIT_ATIMWND6(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_ATIMWND6(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DTIM_COUNT_0_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DTIM_COUNT_VAP3                 ((u32)0x000000FF << 24)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_VAP3(x)                   ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_DTIM_COUNT_VAP3(x)               ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DTIM_COUNT_VAP2                 ((u32)0x000000FF << 16)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_VAP2(x)                   ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_DTIM_COUNT_VAP2(x)               ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_DTIM_COUNT_VAP1                 ((u32)0x000000FF << 8)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_VAP1(x)                   ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_DTIM_COUNT_VAP1(x)               ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_DTIM_COUNT_ROOT                 ((u32)0x000000FF << 0)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_ROOT(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_DTIM_COUNT_ROOT(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DTIM_COUNT_4_7
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DTIM_COUNT_VAP7                 ((u32)0x000000FF << 24)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_VAP7(x)                   ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_DTIM_COUNT_VAP7(x)               ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DTIM_COUNT_VAP6                 ((u32)0x000000FF << 16)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_VAP6(x)                   ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_DTIM_COUNT_VAP6(x)               ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_DTIM_COUNT_VAP5                 ((u32)0x000000FF << 8)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_VAP5(x)                   ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_DTIM_COUNT_VAP5(x)               ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_DTIM_COUNT_VAP4                 ((u32)0x000000FF << 0)          /*!<R/W 0x0   */
#define BIT_DTIM_COUNT_VAP4(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_DTIM_COUNT_VAP4(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DISATIM_BCNERLY_TBTTHOLD_BCNCS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_TSFTR3_SYNC_EN                       ((u32)0x00000001 << 31)          /*!<W 0x0  write this bit, PORT3 will sync to PORT2 with TSFTF_SYN_OFFSET */
#define BIT_TSFTR2_SYNC_EN                       ((u32)0x00000001 << 29)          /*!<W 0x0  write this bit, PORT2 will sync to PORT3 with TSFTF_SYN_OFFSET */
#define BIT_TSFTR2_RST                           ((u32)0x00000001 << 28)          /*!<W 0x0  write this bit , bcn2 tsft will auto reset */
#define BIT_MASK_MULTI_BCN_CS                    ((u32)0x0000000F << 24)          /*!<RO 0x0  indicate which port is tx beacon */
#define BIT_MULTI_BCN_CS(x)                      ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_MULTI_BCN_CS(x)                  ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_DIS_BCN_P2                           ((u32)0x00000001 << 23)          /*!<R/W 0x0  set 1 to mask the beacon early interrupt of port2 */
#define BIT_DIS_BCN_P1                           ((u32)0x00000001 << 22)          /*!<R/W 0x0  set 1 to mask the beacon early interrupt of port1 */
#define BIT_DIS_BCN_P0                           ((u32)0x00000001 << 21)          /*!<R/W 0x0  set 1 to mask the beacon early interrupt of port0 */
#define BIT_MASK_TBTT_HOLD_PREDICT_P1            ((u32)0x0000001F << 16)          /*!<R/W 0x2  the predicted hold time after tbtt for beacon tx of port1, unit is 32us */
#define BIT_TBTT_HOLD_PREDICT_P1(x)              ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_TBTT_HOLD_PREDICT_P1(x)          ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_EARLY_128US_P1                  ((u32)0x00000007 << 11)          /*!<R/W 0x0  the BCN EARLY interrupt find tune with unit 128us. The Total Early time of BCN early interrupt is the 0x558 x 1ms + EARLY_128us x 128us. For port 1 */
#define BIT_EARLY_128US_P1(x)                    ((u32)(((x) & 0x00000007) << 11))
#define BIT_GET_EARLY_128US_P1(x)                ((u32)(((x >> 11) & 0x00000007)))
#define BIT_MASK_EARLY_128US_P0                  ((u32)0x00000007 << 8)          /*!<R/W 0x0  the BCN EARLY interrupt find tune with unit 128us. The Total Early time of BCN early interrupt is the 0x558 x 1ms + EARLY_128us x 128us. */
#define BIT_EARLY_128US_P0(x)                    ((u32)(((x) & 0x00000007) << 8))
#define BIT_GET_EARLY_128US_P0(x)                ((u32)(((x >> 8) & 0x00000007)))
#define BIT_DIS_ATIM_VAP7                        ((u32)0x00000001 << 7)          /*!<R/W 0x0   */
#define BIT_DIS_ATIM_VAP6                        ((u32)0x00000001 << 6)          /*!<R/W 0x0   */
#define BIT_DIS_ATIM_VAP5                        ((u32)0x00000001 << 5)          /*!<R/W 0x0   */
#define BIT_DIS_ATIM_VAP4                        ((u32)0x00000001 << 4)          /*!<R/W 0x0   */
#define BIT_DIS_ATIM_VAP3                        ((u32)0x00000001 << 3)          /*!<R/W 0x0   */
#define BIT_DIS_ATIM_VAP2                        ((u32)0x00000001 << 2)          /*!<R/W 0x0   */
#define BIT_DIS_ATIM_VAP1                        ((u32)0x00000001 << 1)          /*!<R/W 0x0   */
#define BIT_DIS_ATIM_ROOT                        ((u32)0x00000001 << 0)          /*!<R/W 0x0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FREERUN_CNT_P2POFF_DIS_TXTIME
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_P2POFF_DIS_TXTIME               ((u32)0x000000FF << 24)          /*!<R/W 0x0  The time of the disable TX period after the P2P RF off interrupt occur. Unit: 64us. */
#define BIT_P2POFF_DIS_TXTIME(x)                 ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_P2POFF_DIS_TXTIME(x)             ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_EN_FREECNT                           ((u32)0x00000001 << 13)          /*!<R/W 0x0  enable freerun tsft */
#define BIT_RESET_FREECNT_P                      ((u32)0x00000001 << 12)          /*!<W 0x0  reset pulse of freerun tsft */
/** @} */

/**************************************************************************//**
 * @defgroup REG_PTA_TRXBCN_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PTA_RXBCN_PRI_MASK                   ((u32)0x00000001 << 31)          /*!<R/W 0x1   */
#define BIT_PTA_RXBCN_EN                         ((u32)0x00000001 << 30)          /*!<R/W 0x1   */
#define BIT_PTA_RXBCN_DIR                        ((u32)0x00000001 << 29)          /*!<R/W 0x1   */
#define BIT_MASK_PTA_RXBCN_DUR                   ((u32)0x000001FF << 20)          /*!<R/W 0x60   */
#define BIT_PTA_RXBCN_DUR(x)                     ((u32)(((x) & 0x000001FF) << 20))
#define BIT_GET_PTA_RXBCN_DUR(x)                 ((u32)(((x >> 20) & 0x000001FF)))
#define BIT_MASK_PTA_RXBCN_TIME                  ((u32)0x0000007F << 13)          /*!<R/W 0x6   */
#define BIT_PTA_RXBCN_TIME(x)                    ((u32)(((x) & 0x0000007F) << 13))
#define BIT_GET_PTA_RXBCN_TIME(x)                ((u32)(((x >> 13) & 0x0000007F)))
#define BIT_PTA_PRETX_EN                         ((u32)0x00000001 << 12)          /*!<R/W 0x1   */
#define BIT_MASK_PTA_PRETX_TIME                  ((u32)0x00000FFF << 0)          /*!<R/W 0x64  WIFI TX shall wait settle time when antenna is switched from BT to WIFI, if the BT use the same RFAFE as WIFI. Unit: 1us */
#define BIT_PTA_PRETX_TIME(x)                    ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_PTA_PRETX_TIME(x)                ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P2P_NOA1_CTRL_STATE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_POWER_STATE_1                        ((u32)0x00000001 << 31)          /*!<RO   1: On, 0: Off. (It is including early time. ) */
#define BIT_CTWINDOW_ON_1                        ((u32)0x00000001 << 30)          /*!<RO   Spec time and Beacon Early Time to TBTT Time. (It is including early time. ) */
#define BIT_BEACON_AREA_ON_1                     ((u32)0x00000001 << 29)          /*!<RO   The time is from HOLD Time to BeaconTx or Timeout. (It is including early time. ) */
#define BIT_CTWIN_EARLY_DISTX_1                  ((u32)0x00000001 << 28)          /*!<RO   ctwin_early_distx. (It is including early time. ) */
#define BIT_NOA1_OFF_PERIOD_1                    ((u32)0x00000001 << 27)          /*!<RO   Spec time but exclude BCNDMATIM time for wake. (It is including early time. ) */
#define BIT_FORCE_DOZE1_1                        ((u32)0x00000001 << 26)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. (It is including early time. ) */
#define BIT_NOA0_OFF_PERIOD_1                    ((u32)0x00000001 << 25)          /*!<RO   Spec time but exclude BCNDMATIM time for wake. (It is including early time. ) */
#define BIT_FORCE_DOZE0_1                        ((u32)0x00000001 << 24)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. (It is including early time. ) */
#define BIT_SPEC_POWER_STATE_1                   ((u32)0x00000001 << 23)          /*!<RO   1: On, 0: Off */
#define BIT_SPEC_CTWINDOW_ON_1                   ((u32)0x00000001 << 22)          /*!<RO   The time from TBTT <-> CTWEND (with BCNDMATIM early time) */
#define BIT_SPEC_BEACON_AREA_ON_1                ((u32)0x00000001 << 21)          /*!<RO   The time is from TBTT to BeaconTx or Timeout */
#define BIT_SPEC_CTWIN_EARLY_DISTX_1             ((u32)0x00000001 << 20)          /*!<RO   ctwin_early_distx */
#define BIT_SPEC_NOA1_OFF_PERIOD_1               ((u32)0x00000001 << 19)          /*!<RO   Spec time */
#define BIT_SPEC_FORCE_DOZE1_1                   ((u32)0x00000001 << 18)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. */
#define BIT_SPEC_NOA0_OFF_PERIOD_1               ((u32)0x00000001 << 17)          /*!<RO   Spec time */
#define BIT_SPEC_FORCE_DOZE0_1                   ((u32)0x00000001 << 16)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. */
#define BIT_P2P_NOA1_ALLSTASLEEP                 ((u32)0x00000001 << 7)          /*!<R/W 0x0  It indicates that the state from CTWindow End to BcnEarly 1: Sleep 0: Wake */
#define BIT_P2P_NOA1_DISTX_EN                    ((u32)0x00000001 << 6)          /*!<R/W 0x0  1: Hw will disable TX in P2P_off period 0: Do Nothing */
#define BIT_P2P_NOA1_PWR_MGT_EN                  ((u32)0x00000001 << 5)          /*!<R/W 0x0  A main switch for P2P PowerSaving 1: Enable 0: Disable */
#define BIT_MASK_P2P_NOA1_EN                     ((u32)0x00000003 << 3)          /*!<R/W 0x0  The condition that noa_unit0, [1]-NOA1, [0]-NOA0 1: Enable 0: Disable */
#define BIT_P2P_NOA1_EN(x)                       ((u32)(((x) & 0x00000003) << 3))
#define BIT_GET_P2P_NOA1_EN(x)                   ((u32)(((x >> 3) & 0x00000003)))
#define BIT_MASK_P2P_NOA1_UNIT_SEL               ((u32)0x00000003 << 1)          /*!<R/W 0x0  The selector for the source of noa unit0. 0 for port 0, 1 for port 1 */
#define BIT_P2P_NOA1_UNIT_SEL(x)                 ((u32)(((x) & 0x00000003) << 1))
#define BIT_GET_P2P_NOA1_UNIT_SEL(x)             ((u32)(((x >> 1) & 0x00000003)))
#define BIT_EN_TSFBIT32_RST_NOA1                 ((u32)0x00000001 << 0)          /*!<R/W 0x1   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_ACMHWCTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_AVGPERIOD                       ((u32)0x0000FFFF << 16)          /*!<R/W 0x00  In unit of 32 us. It is the value Dot11EDCAAveragingPeriod in SPEC. HW will recalculate used_time every this period. Max is 2 sec */
#define BIT_AVGPERIOD(x)                         ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_AVGPERIOD(x)                     ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_BE_ACM_RESET_USED_TIME               ((u32)0x00000001 << 10)          /*!<W 0x0  When HwEn is enabled, software set this bit to inform hardware to reset used_time to 0. Read this bit is always 0. */
#define BIT_VI_ACM_RESET_USED_TIME               ((u32)0x00000001 << 9)          /*!<W 0x0  When HwEn is enabled, software set this bit to inform hardware to reset used_time to 0. Read this bit is always 0. */
#define BIT_VO_ACM_RESET_USED_TIME               ((u32)0x00000001 << 8)          /*!<W 0x0  When HwEn is enabled, software set this bit to inform hardware to reset used_time to 0. Read this bit is always 0. */
#define BIT_BEQ_ACM_STATUS                       ((u32)0x00000001 << 7)          /*!<R 0x0  This bit indicates if the used_time >= admitted_time of AC BE when HW ACM. 0: BEQ ACM Not Exceed. 1: BEQ ACM Exceed. Default: 0h */
#define BIT_VIQ_ACM_STATUS                       ((u32)0x00000001 << 6)          /*!<R 0x0  This bit indicates if the used_time >= admitted_time of AC VI when HW ACM. 0: VIQ ACM Not Exceed. 1: VIQ ACM Exceed. Default: 0h */
#define BIT_VOQ_ACM_STATUS                       ((u32)0x00000001 << 5)          /*!<R 0x0  This bit indicates if the used_time >= admitted_time of AC VO when HW ACM. 0: VOQ ACM Not Exceed. 1: VOQ ACM Exceed. Default: 0h */
#define BIT_BEQ_ACM_EN                           ((u32)0x00000001 << 3)          /*!<R/W 0x0  Set this bit to enable ACM of BE queue. 1: Enable 0: Disable Default: 0h */
#define BIT_VIQ_ACM_EN                           ((u32)0x00000001 << 2)          /*!<R/W 0x0  Set this bit to enable ACM of VI queue. 1: Enable 0: Disable Default: 0h. */
#define BIT_VOQ_ACM_EN                           ((u32)0x00000001 << 1)          /*!<R/W 0x0  Set this bit to enable ACM of VO queue. 1: Enable 0: Disable Default: 0h. */
#define BIT_ACMHWEN                              ((u32)0x00000001 << 0)          /*!<R/W 0x0  Set this bit to enable hardware to take control of ACM. 1: Hardware Control 0: Software Control Default: 0h. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_VO_VI_ADMTIME
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_VI_ADMITTED_TIME                ((u32)0x0000FFFF << 16)          /*!<R/W 0x00  Admitted_time of VI queue. In unit of 32 microsecond. Calculated and set by driver. */
#define BIT_VI_ADMITTED_TIME(x)                  ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_VI_ADMITTED_TIME(x)              ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_VO_ADMITTED_TIME                ((u32)0x0000FFFF << 0)          /*!<R/W 0x00  Admitted_time of VO queue. In unit of 32 microsecond. Calculated and set by driver. */
#define BIT_VO_ADMITTED_TIME(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_VO_ADMITTED_TIME(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BE_ADMTIME
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DIS_NDPA_NAV_CHK                     ((u32)0x00000001 << 24)          /*!<R/W 0x0  1-->tx NDPA/NDP can cross TBTT,0-->tx NDPA/NDP can't cross TBTT */
#define BIT_MASK_MHDR_NAV_OFFSET                 ((u32)0x000000FF << 16)          /*!<R/W 0x0  NDPA NAV compensation value */
#define BIT_MHDR_NAV_OFFSET(x)                   ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_MHDR_NAV_OFFSET(x)               ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_BE_ADMITTED_TIME                ((u32)0x0000FFFF << 0)          /*!<R/W 0x00  Admitted_time of BE queue. In unit of 32 microsecond. Calculated and set by driver. */
#define BIT_BE_ADMITTED_TIME(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BE_ADMITTED_TIME(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOACMD_SEL_EDCA_RANDOM_GEN
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_SEL                         ((u32)0x00000007 << 28)          /*!<R/W 0x0  TX NoA parameter select (noa_unit0~2,parameter0~5) 0: NoA0 , 1:NoA1, 2:NoA2, 3:NoA3, 4:NoA4, default: NoA5 To control the register setting for which NOA. (register 0x05E0[103:0]) */
#define BIT_NOA_SEL(x)                           ((u32)(((x) & 0x00000007) << 28))
#define BIT_GET_NOA_SEL(x)                       ((u32)(((x >> 28) & 0x00000007)))
#define BIT_MASK_TXCMD_SEG_SEL                   ((u32)0x0000000F << 24)          /*!<R/W 0x1  [HW Debug Info]To select which one TXCMD Segment. For example, 可以在0x5CF[3:0]填 0 or 6 讀取 0x5F8[7:0] : Tx Agg 0x5F9[7:0] : Tx Rate */
#define BIT_TXCMD_SEG_SEL(x)                     ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_TXCMD_SEG_SEL(x)                 ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_RANDOM_GEN                      ((u32)0x00FFFFFF << 0)          /*!<R/W 0xe21051  Random number generator polynomial */
#define BIT_RANDOM_GEN(x)                        ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_RANDOM_GEN(x)                    ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXPAUSE_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSF_DIFF_P1P2                   ((u32)0x0000FFFF << 16)          /*!<R 0x0  the difference of beacon count down value between port0 and port1, unit is TU */
#define BIT_TSF_DIFF_P1P2(x)                     ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_TSF_DIFF_P1P2(x)                 ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_TXPAUSE1                        ((u32)0x000000FF << 8)          /*!<R/W 0x0  same as 0x522[7:0], allow FW/DR to control pause jointly */
#define BIT_TXPAUSE1(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_TXPAUSE1(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_DRVERLYITV_P2                   ((u32)0x000000FF << 0)          /*!<R/W 0x05  Time to send interrupt to notify driver to change beacon content. Time unit is TU. It is used to wake up early at DriverEarlyTime. For port 2 */
#define BIT_DRVERLYITV_P2(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_DRVERLYITV_P2(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAN_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EN_MULTI_BCN                         ((u32)0x00000001 << 31)          /*!<R/W 0x1  enable multiple beacons of port0~port2 */
#define BIT_ENP2P_DW_AREA                        ((u32)0x00000001 << 30)          /*!<R/W 0x0  DW condition for P2P,for port2 */
#define BIT_MASK_TBTT_PROHIBIT_HOLD_P2           ((u32)0x00000FFF << 18)          /*!<R/W 0x100  the predicted hold time after tbtt for beacon tx of port2, unit is 32us */
#define BIT_TBTT_PROHIBIT_HOLD_P2(x)             ((u32)(((x) & 0x00000FFF) << 18))
#define BIT_GET_TBTT_PROHIBIT_HOLD_P2(x)         ((u32)(((x >> 18) & 0x00000FFF)))
#define BIT_MASK_BCN_PORT_PRI                    ((u32)0x00000003 << 16)          /*!<R/W 0x1  the priority of port0 and port2. 10: port2 has higher priority than port0; 01:port0 has higher priority than port2. 00: port0 and port2 have the same priority */
#define BIT_BCN_PORT_PRI(x)                      ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_BCN_PORT_PRI(x)                  ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_DRVERLYITV_P1                   ((u32)0x000000FF << 8)          /*!<R/W 0x05  Time to send interrupt to notify driver to change beacon content. Time unit is TU. It is used to wake up early at DriverEarlyTime. For port1 */
#define BIT_DRVERLYITV_P1(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_DRVERLYITV_P1(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_P2_RXBCN_RPT_ENABLE                  ((u32)0x00000001 << 7)          /*!<R/W 0x0  When this bit asserted, RXBCNOK report is issued */
#define BIT_P2_DIS_RX_BSSID_FIT                  ((u32)0x00000001 << 6)          /*!<R/W 0x0  0:HW will handle rx_beacon 1:HW will ignore rx_beacon_pk */
#define BIT_P2_TXBCN_RPT_ENABLE                  ((u32)0x00000001 << 5)          /*!<R/W 0x0  When this bit asserted, then TXBCN ERROR report is issued */
#define BIT_P2_DIS_TSF_UDT                       ((u32)0x00000001 << 4)          /*!<R/W 0x1  When set, disable clint 2 TSF update when beacon frames or probe response frames are received. */
#define BIT_P2_EN_BCN_FUNCTION                   ((u32)0x00000001 << 3)          /*!<R/W 0x0  When this bit is set, TSF and other beacon related functions are then enabled. */
#define BIT_P2_EN_BCN_RPT                        ((u32)0x00000001 << 2)          /*!<R/W 0x1  When this bit asserted, txbcnok_int and txbcnerr_int will valid. */
#define BIT_P2_ENP2P_CTWINDOW                    ((u32)0x00000001 << 1)          /*!<R/W 0x0  1: Enable P2P CTN WINDOWS function. 0: Disable P2P CTN WINDOWS function. */
#define BIT_P2_ENP2P_BCNQ_AREA                   ((u32)0x00000001 << 0)          /*!<R/W 0x0  1: Enable P2P BCN area on function. 0 : Disable P2P BCN area on function. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_SPACE_PORT1_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCN_SPACE4                      ((u32)0x0000FFFF << 16)          /*!<R/W 0x200  sync beacon space of port2 */
#define BIT_BCN_SPACE4(x)                        ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_BCN_SPACE4(x)                    ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_BCN_SPACE3                      ((u32)0x0000FFFF << 0)          /*!<R/W 0x64  sync beacon space of port1 */
#define BIT_BCN_SPACE3(x)                        ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BCN_SPACE3(x)                    ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAN_SETTING_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SYNCBCN_RXNUM                   ((u32)0x0000001F << 27)          /*!<R/W 0x0  if DW is permitted to end early, the number of rx sync beacons is defined in this register */
#define BIT_SYNCBCN_RXNUM(x)                     ((u32)(((x) & 0x0000001F) << 27))
#define BIT_GET_SYNCBCN_RXNUM(x)                 ((u32)(((x >> 27) & 0x0000001F)))
#define BIT_DW_END_EARLY                         ((u32)0x00000001 << 26)          /*!<R/W 0x0  when set, the DW window end earlier than spec definition. When sync bcn and SDF had tx, and rx enough sync bcns defined by 0x5DC[31:27]. DW can early end */
#define BIT_MASK_NAN_ROLE                        ((u32)0x00000003 << 24)          /*!<R/W 0x0  the role of NAN device, including master, no-master sync, and no-master no-sync. */
#define BIT_NAN_ROLE(x)                          ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_NAN_ROLE(x)                      ((u32)(((x >> 24) & 0x00000003)))
#define BIT_MASK_MSLOT_EVTQ                      ((u32)0x000000FF << 16)          /*!<R/W 0x0  evtq packet will tx in the m-th SLOT of the m-th DWs */
#define BIT_MSLOT_EVTQ(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_MSLOT_EVTQ(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_MDW_EVTQ                        ((u32)0x000000FF << 8)          /*!<R/W 0x0  evtq packet will tx in the m-th SLOT of the m-th DWs */
#define BIT_MDW_EVTQ(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MDW_EVTQ(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_EVTQ_EARLY                      ((u32)0x00000007 << 5)          /*!<R/W 0x1  the early time of evtq tx window. unit is TU */
#define BIT_EVTQ_EARLY(x)                        ((u32)(((x) & 0x00000007) << 5))
#define BIT_GET_EVTQ_EARLY(x)                    ((u32)(((x >> 5) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_PARAM_V1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_DURATION_V1                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x00001000  The maximum duration in units of microseconds that the P2P Group Owner shall remain absent following the start of a Notice of Absence interval. */
#define BIT_NOA_DURATION_V1(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_NOA_DURATION_V1(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_PARAM_V2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_INTERVAL_V2                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x00003000  The length of the Notice of Absence interval in units of microseconds. */
#define BIT_NOA_INTERVAL_V2(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_NOA_INTERVAL_V2(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_PARAM_V3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_START_TIME_V3               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x00001000  The start time for the schedule expressed in terms of the lower 4 bytes of the TSF timer. */
#define BIT_NOA_START_TIME_V3(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_NOA_START_TIME_V3(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_PARAM_4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_COUNT_V4                    ((u32)0x000000FF << 0)          /*!<R/W 0x03  indicates the number of absence intervals */
#define BIT_NOA_COUNT_V4(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_NOA_COUNT_V4(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SCHEDULER_DBG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DBG_SCHEDULER                   ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define BIT_DBG_SCHEDULER(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DBG_SCHEDULER(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SCH_TXCMD
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SCH_TXCMD                       ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Scheduler TXCMD Content */
#define BIT_SCH_TXCMD(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_SCH_TXCMD(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PAGE5_DUMMY
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DUMMY                           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0xffff0000  Dummy */
#define BIT_DUMMY(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DUMMY(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_A_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_A_L                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF (that select by reg 0x1520 [25：24]) matches the content of this register, the PS_TIMER_A_INT interrupt is issued */
#define BIT_PS_TIMER_A_L(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_A_L(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_A_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_A_H                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF (that select by reg 0x1520 [25：24]) matches the content of this register, the PS_TIMER_A_INT interrupt is issued */
#define BIT_PS_TIMER_A_H(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_A_H(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_B_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_B_L                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF (that select by reg 0x1520 [29:28]) matches the content of this register, the PS_TIMER_B_INT interrupt is issued */
#define BIT_PS_TIMER_B_L(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_B_L(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_B_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_B_H                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF(that select by reg 0x1520 [29:28]) matches the content of this register, the PS_TIMER_B_INT interrupt is issued */
#define BIT_PS_TIMER_B_H(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_B_H(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_C_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_C_L                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF(that select by reg 0x1520 [21:20]) matches the content of this register, the PS_TIMER_C_INT interrupt is issued */
#define BIT_PS_TIMER_C_L(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_C_L(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_C_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_C_H                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF(that select by reg 0x1520 [21:20]) matches the content of this register, the PS_TIMER_C_INT interrupt is issued */
#define BIT_PS_TIMER_C_H(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_C_H(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CPUMGQ_PARAMETER
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_E_EARLY                ((u32)0x000000FF << 24)          /*!<R/W 0x0  It is used to wake up early at PS_Timer_E_Early Time. Time unit is TU. */
#define BIT_PS_TIMER_E_EARLY(x)                  ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_PS_TIMER_E_EARLY(x)              ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_CW                              ((u32)0x000000FF << 8)          /*!<R/W 0x32  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_CW(x)                                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_CW(x)                            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_AIFS                            ((u32)0x000000FF << 0)          /*!<R/W 0x24  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch */
#define BIT_AIFS(x)                              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_AIFS(x)                          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_Timer_Early
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_D_EARLY                ((u32)0x000000FF << 24)          /*!<R/W 0x0  It is used to wake up early at PS_Timer_D_Early Time. Time unit is TU. */
#define BIT_PS_TIMER_D_EARLY(x)                  ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_PS_TIMER_D_EARLY(x)              ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_PS_TIMER_C_EARLY                ((u32)0x000000FF << 16)          /*!<R/W 0x0  It is used to wake up early at PS_Timer_C_Early Time. Time unit is TU. */
#define BIT_PS_TIMER_C_EARLY(x)                  ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PS_TIMER_C_EARLY(x)              ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_PS_TIMER_B_EARLY                ((u32)0x000000FF << 8)          /*!<R/W 0x0  It is used to wake up early at PS_Timer_B_Early Time. Time unit is TU. */
#define BIT_PS_TIMER_B_EARLY(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_PS_TIMER_B_EARLY(x)              ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_PS_TIMER_A_EARLY                ((u32)0x000000FF << 0)          /*!<R/W 0x0  It is used to wake up early at PS_Timer_A_Early Time. Time unit is TU. */
#define BIT_PS_TIMER_A_EARLY(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_PS_TIMER_A_EARLY(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_TWT_CRTL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PS_TIMER_A_EN                        ((u32)0x00000001 << 31)          /*!<R/W 0x0  Enable PS_TIMER_B */
#define BIT_MASK_PS_TIMER_A_TSF_SEL              ((u32)0x00000003 << 28)          /*!<R/W 0x0  0 for port 0, 1 for port1, 2 for port2, 3 for freerun */
#define BIT_PS_TIMER_A_TSF_SEL(x)                ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_PS_TIMER_A_TSF_SEL(x)            ((u32)(((x >> 28) & 0x00000003)))
#define BIT_PS_TIMER_B_EN                        ((u32)0x00000001 << 27)          /*!<R/W 0x0  Enable PS_TIMER_A */
#define BIT_MASK_PS_TIMER_B_TSF_SEL              ((u32)0x00000003 << 24)          /*!<R/W 0x0  0 for port 0, 1 for port1, 2 for port2, 3 for freerun */
#define BIT_PS_TIMER_B_TSF_SEL(x)                ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_PS_TIMER_B_TSF_SEL(x)            ((u32)(((x >> 24) & 0x00000003)))
#define BIT_PS_TIMER_C_EN                        ((u32)0x00000001 << 23)          /*!<R/W 0x0  Enable PS_TIMER_C */
#define BIT_MASK_PS_TIMER_C_TSF_SEL              ((u32)0x00000003 << 20)          /*!<R/W 0x0  0 for port 0, 1 for port1, 2 for port2, 3 for freerun */
#define BIT_PS_TIMER_C_TSF_SEL(x)                ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_PS_TIMER_C_TSF_SEL(x)            ((u32)(((x >> 20) & 0x00000003)))
#define BIT_PS_TIMER_D_EN                        ((u32)0x00000001 << 19)          /*!<R/W 0x0  Enable PS_TIMER_D */
#define BIT_MASK_PS_TIMER_D_TSF_SEL              ((u32)0x00000003 << 16)          /*!<R/W 0x0  0 for port 0, 1 for port1, 2 for port2, 3 for freerun */
#define BIT_PS_TIMER_D_TSF_SEL(x)                ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_PS_TIMER_D_TSF_SEL(x)            ((u32)(((x >> 16) & 0x00000003)))
#define BIT_PS_TIMER_E_EN                        ((u32)0x00000001 << 15)          /*!<R/W 0x0  Enable PS_TIMER_E */
#define BIT_MASK_PS_TIMER_E_TSF_SEL              ((u32)0x00000003 << 12)          /*!<R/W 0x0  0 for port 0, 1 for port1, 2 for port2, 3 for freerun */
#define BIT_PS_TIMER_E_TSF_SEL(x)                ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_PS_TIMER_E_TSF_SEL(x)            ((u32)(((x >> 12) & 0x00000003)))
#define BIT_GET_TSFT_OFFSET_EN                   ((u32)0x00000001 << 1)          /*!<R/W 0x0  1: Enable Enable get TSFT offset function */
#define BIT_RXBCN_TIME_PORT                      ((u32)0x00000001 << 0)          /*!<R/W 0x0  select port for report the TSFT 0: port0 1: port1 default is port0. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RX_TSFT_DIFF
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXBCN_TIME_VLD                       ((u32)0x00000001 << 31)          /*!<R/W1C   1: indicate [30:0] is valid. Write 1 to clear this bit */
#define BIT_RXBCN_TIME_DIFF_DIR                  ((u32)0x00000001 << 30)          /*!<R   1: wmac_rx_tsf > TSFT 0: wmac_rx_tsf <= TSFT */
#define BIT_MASK_RXBCN_TIME_DIFF                 ((u32)0x3FFFFFFF << 0)          /*!<R   unit:1us wmac_rx_tsf - TSFT (selected by 0x1520[0]). [30] is the sign bit */
#define BIT_RXBCN_TIME_DIFF(x)                   ((u32)(((x) & 0x3FFFFFFF) << 0))
#define BIT_GET_RXBCN_TIME_DIFF(x)               ((u32)(((x >> 0) & 0x3FFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXBCN_TSFT_STATISTIC
 * @brief
 * @{
 *****************************************************************************/
#define BIT_UPD_TSFT_EARLY                       ((u32)0x00000001 << 31)          /*!<R/W 0x1  0: don't update TSFT when the following scenarios occur: tim_no_hit, |TSFT - rxbcn.timestamp| <= BIT_RXBCN_TSFT_THRES, and r_wait_rxbcn_finish = 1 1: update TSFT */
#define BIT_MASK_RXBCN_TSFT_THRES                ((u32)0x00001FFF << 16)          /*!<R/W 0x0  used for LPS MAC, if |TSFT - rxbcn.timestamp| > BIT_RXBCN_TSFT_THRES, need to wait fcsok. Else, update TSFT at wmac_rxtsf_rdy (before fcsok) unit is 1us,max is 8ms */
#define BIT_RXBCN_TSFT_THRES(x)                  ((u32)(((x) & 0x00001FFF) << 16))
#define BIT_GET_RXBCN_TSFT_THRES(x)              ((u32)(((x >> 16) & 0x00001FFF)))
#define BIT_EN_RXBCN_GT_THRES_CNT                ((u32)0x00000001 << 7)          /*!<R/W 0x0  enable the counter of BIT_RXBCN_GT_THRES_CNT */
#define BIT_CLR_RXBCN_GT_THRES_CNT               ((u32)0x00000001 << 6)          /*!<W1C 0x0  clear the BIT_RXBCN_GT_THRES_CNT to 0 */
#define BIT_MASK_RXBCN_GT_THRES_CNT              ((u32)0x0000003F << 0)          /*!<R 0x0  valid when BIT_EN_RXBCN_GT_THRES_CNT = 1. counter the number of beacon with the |TSFT - rxbcn.timestamp| exceed the threshold */
#define BIT_RXBCN_GT_THRES_CNT(x)                ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_RXBCN_GT_THRES_CNT(x)            ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_P2P_NOA2_CTRL_STATE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_POWER_STATE_2                        ((u32)0x00000001 << 31)          /*!<RO   1: On, 0: Off. (It is including early time. ) */
#define BIT_CTWINDOW_ON_2                        ((u32)0x00000001 << 30)          /*!<RO   Spec time and Beacon Early Time to TBTT Time. (It is including early time. ) */
#define BIT_BEACON_AREA_ON_2                     ((u32)0x00000001 << 29)          /*!<RO   The time is from HOLD Time to BeaconTx or Timeout. (It is including early time. ) */
#define BIT_CTWIN_EARLY_DISTX_2                  ((u32)0x00000001 << 28)          /*!<RO   ctwin_early_distx. (It is including early time. ) */
#define BIT_NOA1_OFF_PERIOD_2                    ((u32)0x00000001 << 27)          /*!<RO   Spec time but exclude BCNDMATIM time for wake. (It is including early time. ) */
#define BIT_FORCE_DOZE1_2                        ((u32)0x00000001 << 26)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. (It is including early time. ) */
#define BIT_NOA0_OFF_PERIOD_2                    ((u32)0x00000001 << 25)          /*!<RO   Spec time but exclude BCNDMATIM time for wake. (It is including early time. ) */
#define BIT_FORCE_DOZE0_2                        ((u32)0x00000001 << 24)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. (It is including early time. ) */
#define BIT_SPEC_POWER_STATE_2                   ((u32)0x00000001 << 23)          /*!<RO   1: On, 0: Off */
#define BIT_SPEC_CTWINDOW_ON_2                   ((u32)0x00000001 << 22)          /*!<RO   The time from TBTT <-> CTWEND (with BCNDMATIM early time) */
#define BIT_SPEC_BEACON_AREA_ON_2                ((u32)0x00000001 << 21)          /*!<RO   The time is from TBTT to BeaconTx or Timeout */
#define BIT_SPEC_CTWIN_EARLY_DISTX_2             ((u32)0x00000001 << 20)          /*!<RO   ctwin_early_distx */
#define BIT_SPEC_NOA1_OFF_PERIOD_2               ((u32)0x00000001 << 19)          /*!<RO   Spec time */
#define BIT_SPEC_FORCE_DOZE1_2                   ((u32)0x00000001 << 18)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. */
#define BIT_SPEC_NOA0_OFF_PERIOD_2               ((u32)0x00000001 << 17)          /*!<RO   Spec time */
#define BIT_SPEC_FORCE_DOZE0_2                   ((u32)0x00000001 << 16)          /*!<RO   If NoA_Count == 1, this value is 1. else 0. */
#define BIT_P2P_NOA2_ALLSTASLEEP                 ((u32)0x00000001 << 7)          /*!<R/W 0x0  It indicates that the state from CTWindow End to BcnEarly 1: Sleep 0: Wake */
#define BIT_P2P_NOA2_DISTX_EN                    ((u32)0x00000001 << 6)          /*!<R/W 0x0  1: Hw will disable TX in P2P_off period 0: Do Nothing */
#define BIT_P2P_NOA2_PWR_MGT_EN                  ((u32)0x00000001 << 5)          /*!<R/W 0x0  A main switch for P2P PowerSaving 1: Enable 0: Disable */
#define BIT_MASK_P2P_NOA2_EN                     ((u32)0x00000003 << 3)          /*!<R/W 0x0  The condition that noa_unit0, [1]-NOA1, [0]-NOA0 1: Enable 0: Disable */
#define BIT_P2P_NOA2_EN(x)                       ((u32)(((x) & 0x00000003) << 3))
#define BIT_GET_P2P_NOA2_EN(x)                   ((u32)(((x >> 3) & 0x00000003)))
#define BIT_MASK_P2P_NOA2_UNIT_SEL               ((u32)0x00000003 << 1)          /*!<R/W 0x0  The selector for the source of noa unit0. 0 for port 0, 1 for port 1, 2 for port 2 */
#define BIT_P2P_NOA2_UNIT_SEL(x)                 ((u32)(((x) & 0x00000003) << 1))
#define BIT_GET_P2P_NOA2_UNIT_SEL(x)             ((u32)(((x >> 1) & 0x00000003)))
#define BIT_EN_TSFBIT32_RST_NOA2                 ((u32)0x00000001 << 0)          /*!<R/W 0x1   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TM_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TM_TSFT_PORT_SEL                ((u32)0x00000003 << 3)          /*!<R/W 0x0  selected TM timer port 0: port0; 1: port1; 2: port2; 3: freerun */
#define BIT_TM_TSFT_PORT_SEL(x)                  ((u32)(((x) & 0x00000003) << 3))
#define BIT_GET_TM_TSFT_PORT_SEL(x)              ((u32)(((x >> 3) & 0x00000003)))
#define BIT_RXTM_EN                              ((u32)0x00000001 << 2)          /*!<R/W 0x0  1: HW will parsing rx packet, to find TM frame */
#define BIT_RXTMREQ_EN                           ((u32)0x00000001 << 1)          /*!<R/W 0x0  1: HW will parsing rx packet, to find TM request frame */
#define BIT_TM_FUNC_EN                           ((u32)0x00000001 << 0)          /*!<R/W 0x0  1: Enable Timing Measurement function */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TM_TSFT_T1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TM_TSFT_T1                      ((u32)0xFFFFFFFF << 0)          /*!<RO 0x0  Latch tsft timer of Tx TM frame, unit: 1us */
#define BIT_TM_TSFT_T1(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TM_TSFT_T1(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TM_TSFT_T2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TM_TSFT_T2                      ((u32)0xFFFFFFFF << 0)          /*!<RO 0x0  Latch tsft timer of Rx TM frame, unit: 1us */
#define BIT_TM_TSFT_T2(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TM_TSFT_T2(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TM_TSFT_T3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TM_TSFT_T3                      ((u32)0xFFFFFFFF << 0)          /*!<RO 0x0  Latch tsft timer of Tx ACK frame, unit: 1us */
#define BIT_TM_TSFT_T3(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TM_TSFT_T3(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TM_TSFT_T4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TM_TSFT_T4                      ((u32)0xFFFFFFFF << 0)          /*!<RO 0x0  Latch tsft timer of Rx ACK frame, unit: 1us */
#define BIT_TM_TSFT_T4(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TM_TSFT_T4(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TM_TIMER_OFFSET
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TM_TX_PATH_TIME_OFFSET          ((u32)0x0000FFFF << 16)          /*!<R/W 0x0  Compensate the time, from PHYTXON rising edge to transmit antenna connector. unit: 1us */
#define BIT_TM_TX_PATH_TIME_OFFSET(x)            ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_TM_TX_PATH_TIME_OFFSET(x)        ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_TM_RX_PATH_TIME_OFFSET          ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  Compensate the time, from receive antenna connector to DATAON rising edge. unit: 1us */
#define BIT_TM_RX_PATH_TIME_OFFSET(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TM_RX_PATH_TIME_OFFSET(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_ELY_ADJ
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCN_ELY_ADJ                     ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  beacon early time adjust, unit is 32us */
#define BIT_BCN_ELY_ADJ(x)                       ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BCN_ELY_ADJ(x)                   ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_D_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_D_L                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF(that select by reg 0x1520 [16:17]) matches the content of this register, the PS_TIMER_D_INT interrupt is issued */
#define BIT_PS_TIMER_D_L(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_D_L(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_D_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_D_H                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF(that select by reg 0x1520 [16:17]) matches the content of this register, the PS_TIMER_D_INT interrupt is issued */
#define BIT_PS_TIMER_D_H(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_D_H(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_E_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_E_L                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF (that select by reg 0x1520 [13:12]) matches the content of this register, the PS_TIMER_E_INT interrupt is issued */
#define BIT_PS_TIMER_E_L(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_E_L(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PS_TIMER_E_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PS_TIMER_E_H                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  When the TSF (that select by reg 0x1520 [13:12]) matches the content of this register, the PS_TIMER_E_INT interrupt is issued */
#define BIT_PS_TIMER_E_H(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_PS_TIMER_E_H(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_MUEDCA_VO_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MUEDCA_VO_PARAM_TIMER           ((u32)0x0000FFFF << 16)          /*!<R/W 0x00  MU EDCA TIMER, in units of 32 us Note: SPEC: The MU EDCA Timer field (8b) indicates the duration of time, in units of 8 TUs */
#define BIT_MUEDCA_VO_PARAM_TIMER(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_MUEDCA_VO_PARAM_TIMER(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_MUEDCA_VO_PARAM_CW              ((u32)0x000000FF << 8)          /*!<R/W 0x43  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_MUEDCA_VO_PARAM_CW(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MUEDCA_VO_PARAM_CW(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_MUEDCA_VO_PARAM_AIFS            ((u32)0x000000FF << 0)          /*!<R/W 0x1C  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_MUEDCA_VO_PARAM_AIFS(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_MUEDCA_VO_PARAM_AIFS(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_MUEDCA_VI_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MUEDCA_VI_PARAM_TIMER           ((u32)0x0000FFFF << 16)          /*!<R/W 0x00  MU EDCA TIMER, in units of 32 us Note: SPEC: The MU EDCA Timer field (8b) indicates the duration of time, in units of 8 TUs */
#define BIT_MUEDCA_VI_PARAM_TIMER(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_MUEDCA_VI_PARAM_TIMER(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_MUEDCA_VI_PARAM_CW              ((u32)0x000000FF << 8)          /*!<R/W 0x54  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_MUEDCA_VI_PARAM_CW(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MUEDCA_VI_PARAM_CW(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_MUEDCA_VI_PARAM_AIFS            ((u32)0x000000FF << 0)          /*!<R/W 0x1C  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_MUEDCA_VI_PARAM_AIFS(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_MUEDCA_VI_PARAM_AIFS(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_MUEDCA_BE_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MUEDCA_BE_PARAM_TIMER           ((u32)0x0000FFFF << 16)          /*!<R/W 0x00  MU EDCA TIMER, in units of 32 us Note: SPEC: The MU EDCA Timer field (8b) indicates the duration of time, in units of 8 TUs */
#define BIT_MUEDCA_BE_PARAM_TIMER(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_MUEDCA_BE_PARAM_TIMER(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_MUEDCA_BE_PARAM_CW              ((u32)0x000000FF << 8)          /*!<R/W 0xA5  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_MUEDCA_BE_PARAM_CW(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MUEDCA_BE_PARAM_CW(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_MUEDCA_BE_PARAM_AIFS            ((u32)0x000000FF << 0)          /*!<R/W 0x25  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_MUEDCA_BE_PARAM_AIFS(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_MUEDCA_BE_PARAM_AIFS(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_MUEDCA_BK_PARAM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MUEDCA_BK_PARAM_TIMER           ((u32)0x0000FFFF << 16)          /*!<R/W 0x00  MU EDCA TIMER, in units of 32 us Note: SPEC: The MU EDCA Timer field (8b) indicates the duration of time, in units of 8 TUs */
#define BIT_MUEDCA_BK_PARAM_TIMER(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_MUEDCA_BK_PARAM_TIMER(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_MUEDCA_BK_PARAM_CW              ((u32)0x000000FF << 8)          /*!<R/W 0xA5  ECWmin and ECWmax encode the values of Cwmin and Cwmax respectively in an exponent form. Ex: Cwmax = 2^(ECWmax)-1 Default: BK = Ah/5h; BE = Ah/5h; VI = 5h/4h; VO = 4h/3h. */
#define BIT_MUEDCA_BK_PARAM_CW(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MUEDCA_BK_PARAM_CW(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_MUEDCA_BK_PARAM_AIFS            ((u32)0x000000FF << 0)          /*!<R/W 0x49  Arbitrary Inter frame space In unit of microsecond. AIFS indicates the time a non-AP QSTA should defer before either invoking a backoff or starting a transmission Default: BK = 49h; BE = 25h; VI = 1Ch; VO = 1Ch. */
#define BIT_MUEDCA_BK_PARAM_AIFS(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_MUEDCA_BK_PARAM_AIFS(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_TB_MUEDCA_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TB_TIMER_CNT_INT                ((u32)0x0000001F << 27)          /*!<R/W 0x14  the initial value of th_timer, unit: us */
#define BIT_TB_TIMER_CNT_INT(x)                  ((u32)(((x) & 0x0000001F) << 27))
#define BIT_GET_TB_TIMER_CNT_INT(x)              ((u32)(((x >> 27) & 0x0000001F)))
#define BIT_HE_TB_PPSU_ONLY_EN                   ((u32)0x00000001 << 26)          /*!<R/W 0x0  asserted if the TWT is trigger-enabled TWT */
#define BIT_TB_TXPAUSE                           ((u32)0x00000001 << 25)          /*!<R/W 0x1  valid if BIT_HE_TB_PPSU_ONLY_EN = 1; select pause edca or not during TWT-SP; default is 1, pause. A TWT requesting STA should not transmit frames to the TWT responding STA outside of negotiated TWT SPs and should not transmit frames that are not contained within HE TB PPDUs to the TWT responding STA within trigger-enabled TWT SPs */
#define BIT_DIS_UPD_BEAM_CHG                     ((u32)0x00000001 << 2)          /*!<R/W 0x1  1: txcmd_beam_change TIE 0 0: first PPDU in TXOP will assert txcmd_beam_change */
#define BIT_SET_MUEDCATIMER_TF                   ((u32)0x00000001 << 1)          /*!<R/W 0x0  1: enable HW updating MUEDCATimer after Tx HE TB PPDU 0: disable HW updating MUEDCATimer after Tx HE TB PPDU */
#define BIT_MUEDCA_EN                            ((u32)0x00000001 << 0)          /*!<R/W 0x1  1: enable HW updating MUEDCATimer 0: disable HW updating MUEDCATimer */
/** @} */

/**************************************************************************//**
 * @defgroup REG_CCA_NAV_CHK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_TB_CHK_EDCCA_PER20_BITMAP_SIFS       ((u32)0x00000001 << 23)          /*!<R/W 0x1  When TF CS required = 1 and EDCCA_PER20_BITMAP_SIFS assert in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_TB_CHK_BASIC_NAV                     ((u32)0x00000001 << 22)          /*!<R/W 0x1  When TF CS required = 1 and BAISC NAV > 0 , in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_TB_CHK_ZGBTCCA                       ((u32)0x00000001 << 21)          /*!<R/W 0x1  When TF CS required = 1 and ~GNT_WL(BT/ZigBee) assert in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_TB_CHK_EDCCA                         ((u32)0x00000001 << 20)          /*!<R/W 0x1  When TF CS required = 1 and EDCCA assert in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_TB_CHK_CCA_S80                       ((u32)0x00000001 << 19)          /*!<R/W 0x1  When TF CS required = 1 and SECONDARY_CCA_80 assert in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_TB_CHK_CCA_S40                       ((u32)0x00000001 << 18)          /*!<R/W 0x1  When TF CS required = 1 and SECONDARY_CCA_40 assert in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_TB_CHK_CCA_S20                       ((u32)0x00000001 << 17)          /*!<R/W 0x1  When TF CS required = 1 and SECONDARY_CCA_20 assert in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_TB_CHK_CCA_P20                       ((u32)0x00000001 << 16)          /*!<R/W 0x1  When TF CS required = 1 and CCA assert in the SIFS before HE TB PPDU TX 0: Can TX 1: Cannot TX */
#define BIT_SR_CTN_CHK__NAV                      ((u32)0x00000001 << 12)          /*!<R/W 0x1  When BASIC or INTRA NAV > 0 in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
#define BIT_SR_CTN_CHK_ZGBTCCA                   ((u32)0x00000001 << 11)          /*!<R/W 0x1  When ~GNT_WL(BT/ZigBee) assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
#define BIT_SR_CTN_CHK_EDCCA                     ((u32)0x00000001 << 10)          /*!<R/W 0x1  When EDCCA assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
#define BIT_SR_CTN_CHK_CCA_S40                   ((u32)0x00000001 << 9)          /*!<R/W 0x1  When SECONDARY_CCA_40 assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Can backoff, cannot TX */
#define BIT_SR_CTN_CHK_CCA_S20                   ((u32)0x00000001 << 8)          /*!<R/W 0x1  When SECONDARY_CCA_20 assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Can backoff, cannot TX */
#define BIT_CTN_CHK_INTRA_NAV                    ((u32)0x00000001 << 6)          /*!<R/W 0x1  When INTRA NAV > 0 in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
#define BIT_CTN_CHK_BASIC_NAV                    ((u32)0x00000001 << 5)          /*!<R/W 0x1  When BASIC NAV > 0 in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
#define BIT_CTN_CHK_ZGBTCCA                      ((u32)0x00000001 << 4)          /*!<R/W 0x1  When ~GNT_WL(BT/ZigBee) assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
#define BIT_CTN_CHK_EDCCA                        ((u32)0x00000001 << 3)          /*!<R/W 0x1  When EDCCA assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
#define BIT_CTN_CHK_CCA_S40                      ((u32)0x00000001 << 2)          /*!<R/W 0x1  When SECONDARY_CCA_40 assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Can backoff, cannot TX */
#define BIT_CTN_CHK_CCA_S20                      ((u32)0x00000001 << 1)          /*!<R/W 0x1  When SECONDARY_CCA_20 assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Can backoff, cannot TX */
#define BIT_CTN_CHK_CCA_P20                      ((u32)0x00000001 << 0)          /*!<R/W 0x1  When CCA assert in the AIFS/Backoff before active TX 0: Can backoff, can TX 1: Cannot backoff, cannot TX */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSFT_HIOE_RFON_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSFT_AT_HIOE_RFON_L             ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  when hioe_int_chn_on asserted, latch the tsft[31:0] (select port by 0x1520[0]) */
#define BIT_TSFT_AT_HIOE_RFON_L(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TSFT_AT_HIOE_RFON_L(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSFT_HIOE_RFON_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSFT_AT_HIOE_RFON_H             ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  when hioe_int_chn_on asserted, latch the tsft[63:32] (select port by 0x1520[0]) */
#define BIT_TSFT_AT_HIOE_RFON_H(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TSFT_AT_HIOE_RFON_H(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSFT_BCN_CCAUP_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSFT_AT_BCN_CCA_UP_L            ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  when CCA asserted, latch the tsft[31:0] (select port by 0x1520[0]). updated to this register after judging that it is a beacon frame */
#define BIT_TSFT_AT_BCN_CCA_UP_L(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TSFT_AT_BCN_CCA_UP_L(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSFT_BCN_CCAUP_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TSFT_AT_BCN_CCA_UP_H            ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  when CCA asserted, latch the tsft[63:32] (select port by 0x1520[0]). updated to this register after judging that it is a beacon frame */
#define BIT_TSFT_AT_BCN_CCA_UP_H(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TSFT_AT_BCN_CCA_UP_H(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_POF_BCNELY_AGG_TBTT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EN_P0_TBTT_CNT                       ((u32)0x00000001 << 24)          /*!<R/W 0x0  enable the tbtt counter function. */
#define BIT_MASK_P0_TBTT_CNT_THRS                ((u32)0x0000FFFF << 8)          /*!<R/W 0x0  P0's TBTT counter threshold . Valid when BIT_EN_P0_TBTT_CNT = 1. when tbtt_cnt >= BIT_P0_TBTT_CNT_THRS, assert p0_tbtt_agg_int. */
#define BIT_P0_TBTT_CNT_THRS(x)                  ((u32)(((x) & 0x0000FFFF) << 8))
#define BIT_GET_P0_TBTT_CNT_THRS(x)              ((u32)(((x >> 8) & 0x0000FFFF)))
#define BIT_EN_POF_ELY_AGG                       ((u32)0x00000001 << 7)          /*!<R/W 0x0  update bcn_ely_agg initial value of P0 to BIT_INI_POF_ELY_AGG_CNT 1: enable 0: disable */
#define BIT_MASK_INI_POF_ELY_AGG_CNT             ((u32)0x0000007F << 0)          /*!<R/W 0x3  bcn_ely_agg initial value of P0. Valid when BIT_EN_POF_ELY_AGG = 1 */
#define BIT_INI_POF_ELY_AGG_CNT(x)               ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_INI_POF_ELY_AGG_CNT(x)           ((u32)(((x >> 0) & 0x0000007F)))
/** @} */


/*==========PAGE5 Register Address Definition==========*/
#define REG_EDCA_VO_PARAM                            0x0500
#define REG_EDCA_VI_PARAM                            0x0504
#define REG_EDCA_BE_PARAM                            0x0508
#define REG_EDCA_BK_PARAM                            0x050C
#define REG_BCN_CFG_PIFS                             0x0510
#define REG_SIFS                                     0x0514
#define REG_TSFT_SYN_OFFSET_SLOT                     0x0518
#define REG_PS_NOA_ERLY_TIME                         0x051C
#define REG_TXPAUSE_TXPTCL_DISTXREQ_CTRL             0x0520
#define REG_NOA0_MBSSID_RD_CTRL                      0x0524
#define REG_NOA0_PKT_LIFETIME_CTRL                   0x0528
#define REG_P2PON_DIS_TXTIME                         0x0530
#define REG_NAN_SETTING_2                            0x0534
#define REG_QUEUE_INCOL_THR                          0x0538
#define REG_QUEUE_INCOL_EN                           0x053C
#define REG_NOA0_TBTT_PROHIBIT                       0x0540
#define REG_NAV_CTRL                                 0x0544
#define REG_FTM_CTRL                                 0x0548
#define REG_FTM_TSF_CNT                              0x054C
#define REG_TSTRST_P2PRST_BCN_CTRL_PORT0_1           0x0550
#define REG_PORT0_BCN_SPACE                          0x0554
#define REG_ATIMWND0_P0_BCN_ERLY_DMA_CTRL            0x0558
#define REG_BCNERR_TSF_CTRL                          0x055C
#define REG_P0_TSFTR_L                               0x0560
#define REG_P0_TSFTR_H                               0x0564
#define REG_P1_TSFTR_L                               0x0568
#define REG_P1_TSFTR_H                               0x056C
#define REG_BCN_ERLY_CTW_CTRL                        0x0570
#define REG_BCNDROP_TO_MISC_CTRL                     0x0574
#define REG_P2_TSFTR_L                               0x0578
#define REG_P2_TSFTR_H                               0x057C
#define REG_TSF_TIMER1                               0x0580
#define REG_TSF_TIMER2                               0x0584
#define REG_TSF_TIMER3                               0x0588
#define REG_BCN_IFS_TBTT_CTN                         0x058C
#define REG_TXOP_CTRL_PRE_BKF                        0x0590
#define REG_FREERUN_CNT_L                            0x0594
#define REG_FREERUN_CNT_H                            0x0598
#define REG_BCN_ERLY_CTW_CTRL_P1_2                   0x059C
#define REG_ATIMWND2_5                               0x05A0
#define REG_ATIMWND6_7_URGENT_NOLIMTHGQ              0x05A4
#define REG_DTIM_COUNT_0_3                           0x05A8
#define REG_DTIM_COUNT_4_7                           0x05AC
#define REG_DISATIM_BCNERLY_TBTTHOLD_BCNCS           0x05B0
#define REG_FREERUN_CNT_P2POFF_DIS_TXTIME            0x05B4
#define REG_PTA_TRXBCN_CTRL                          0x05B8
#define REG_P2P_NOA1_CTRL_STATE                      0x05BC
#define REG_ACMHWCTRL                                0x05C0
#define REG_VO_VI_ADMTIME                            0x05C4
#define REG_BE_ADMTIME                               0x05C8
#define REG_NOACMD_SEL_EDCA_RANDOM_GEN               0x05CC
#define REG_TXPAUSE_CFG                              0x05D0
#define REG_NAN_SETTING                              0x05D4
#define REG_BCN_SPACE_PORT1_2                        0x05D8
#define REG_NAN_SETTING_1                            0x05DC
#define REG_NOA_PARAM_V1                             0x05E0
#define REG_NOA_PARAM_V2                             0x05E4
#define REG_NOA_PARAM_V3                             0x05E8
#define REG_NOA_PARAM_4                              0x05EC
#define REG_SCHEDULER_DBG                            0x05F4
#define REG_SCH_TXCMD                                0x05F8
#define REG_PAGE5_DUMMY                              0x05FC
#define REG_PS_TIMER_A_L                             0x1500
#define REG_PS_TIMER_A_H                             0x1504
#define REG_PS_TIMER_B_L                             0x1508
#define REG_PS_TIMER_B_H                             0x150C
#define REG_PS_TIMER_C_L                             0x1510
#define REG_PS_TIMER_C_H                             0x1514
#define REG_CPUMGQ_PARAMETER                         0x1518
#define REG_PS_TIMER_EARLY                           0x151C
#define REG_PS_TIMER_TWT_CRTL                        0x1520
#define REG_RX_TSFT_DIFF                             0x1524
#define REG_RXBCN_TSFT_STATISTIC                     0x1528
#define REG_P2P_NOA2_CTRL_STATE                      0x152C
#define REG_TM_CFG                                   0x1530
#define REG_TM_TSFT_T1                               0x1534
#define REG_TM_TSFT_T2                               0x1538
#define REG_TM_TSFT_T3                               0x153C
#define REG_TM_TSFT_T4                               0x1540
#define REG_TM_TIMER_OFFSET                          0x1544
#define REG_BCN_ELY_ADJ                              0x1548
#define REG_PS_TIMER_D_L                             0x154C
#define REG_PS_TIMER_D_H                             0x1550
#define REG_PS_TIMER_E_L                             0x1554
#define REG_PS_TIMER_E_H                             0x1558
#define REG_AX_MUEDCA_VO_PARAM                       0x1580
#define REG_AX_MUEDCA_VI_PARAM                       0x1584
#define REG_AX_MUEDCA_BE_PARAM                       0x1588
#define REG_AX_MUEDCA_BK_PARAM                       0x158C
#define REG_AX_TB_MUEDCA_CTRL                        0x1590
#define REG_CCA_NAV_CHK                              0x1594
#define REG_TSFT_HIOE_RFON_L                         0x1598
#define REG_TSFT_HIOE_RFON_H                         0x159C
#define REG_TSFT_BCN_CCAUP_L                         0x15A0
#define REG_TSFT_BCN_CCAUP_H                         0x15A4
#define REG_POF_BCNELY_AGG_TBTT                      0x15A8

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
