#ifndef __WIFIFW_REG_PAGE6_H__
#define __WIFIFW_REG_PAGE6_H__

/**************************************************************************//**
 * @defgroup REG_WMAC_CR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_TXPWR_CTL_CLK_EN                       ((u32)0x00000001 << 31)          /*!<R/W 0  IF set to 1, txpwr_ctrl clock enable, only work when BIT_TXPWR_CTL_ENGCLK set to 1, */
#define BIT_TXPWR_CTL_ENGCLK                       ((u32)0x00000001 << 30)          /*!<R/W 0  If set to 1, enable txpwr_ctrl clock gating function. */
#define BIT_WLAN_RX_REQ_SIFS_CLR                   ((u32)0x00000001 << 29)          /*!<R/W 0  When set, enable sifs clear wlan_rx_req */
#define BIT_DIS_WAIT_HE_SIGB                       ((u32)0x00000001 << 28)          /*!<R/W 0  When set, will not wait and check he mu sigb and ndp info */
#define BIT_WMAC_20MHZBW                           ((u32)0x00000001 << 26)          /*!<R/W 1   */
#define BIT_WMAC_M11J                              ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_DATA_FW_STS_FILTER                     ((u32)0x00000001 << 18)          /*!<R/W 0  When this bit is set,Accept data frame in partial length condition. It will take effect when RxFilter3-5 is disabled and RxFilter 0-2 is enabled. */
#define BIT_CTRL_FW_STS_FILTER                     ((u32)0x00000001 << 17)          /*!<R/W 0  When this bit is set,Accept control frame in partial length condition. It will take effect when RxFilter3-5 is disabled and RxFilter 0-2 is enabled. */
#define BIT_MGNT_FW_STS_FILTER                     ((u32)0x00000001 << 16)          /*!<R/W 0  When this bit is set,Accept management frame in partial length condition. It will take effect when RxFilter3-5 is disabled and RxFilter 0-2 is enabled. */
#define BIT_FWRX_EN                                ((u32)0x00000001 << 15)          /*!<R/W 0  Enable to receive pkt into FWFF, default 1’b0. When Fw needs to receive pkt, this bit must be set to 1’b1 */
#define BIT_PHYSTS_PKT_CTRL                        ((u32)0x00000001 << 14)          /*!<R/W 0  If set to 1,HW will send interrupt (0x124[26]) only when there is phy status valid fwsts pkt written into FWFF */
#define BIT_APPHDR_MIDSRCH_FAIL                    ((u32)0x00000001 << 12)          /*!<R/W 0  If set to 1, Hw will append 16 bytes MAC header into FWFF when MACID search fail */
#define BIT_FWPARSING_EN                           ((u32)0x00000001 << 11)          /*!<R/W 0  If set to 1, mac header and rx desc of the pkt into rx buff are also received into FWFF. */
#define BIT_MASK_APPEND_MHDR_LEN                   ((u32)0x00000007 << 8)          /*!<R/W 0  Appended MAC header length, unit in 32 bytes */
#define BIT_APPEND_MHDR_LEN(x)                     ((u32)(((x) & 0x00000007) << 8))
#define BIT_GET_APPEND_MHDR_LEN(x)                 ((u32)(((x >> 8) & 0x00000007)))
#define BIT_APSDOFF                                ((u32)0x00000001 << 6)          /*!<R/W 0  1：MAC issue sleep signal to disable BB/AFE/RF TRX function. After MAC has finished, APSDOFF_STATUS will report“1”. 0：MAC issue wakeup signal to recall BB/AFE/RF back to RX idle mode. After MAC has finished,APSDOFF _STATUS will report “0”. */
#define BIT_R_RXTRIG_TX_CHK                        ((u32)0x00000001 << 5)          /*!<R/W 0  when set to 1,when tx type is tb,will chk fcs of trigger frame and if phy_rst is high when sifs reach,set 0 will not check */
#define BIT_R_RXTRIG_DIS_UPD_AID                   ((u32)0x00000001 << 4)          /*!<R/W 0  1:will only update aid_fit info when the first trigger in a ppdu is parsered 0:will update aid_fit info every time when two or more trigger aggregate in a ppdu */
#define BIT_R_RXTRIG_DIS_CHK_A1                    ((u32)0x00000001 << 3)          /*!<R/W 0  1:will not check if A1 of trigger frame is my unicast or broadcast address 0:will check if A1 of trigger frame is my unicast or broadcast address */
#define BIT_R_DIS_CHK_NEW_TBREQ                    ((u32)0x00000001 << 2)          /*!<R/W 0  1:rxtrig req_in will assert every time when aid fit if two or more trigger aggregate in a ppdu 0:rxtrig req_in will not assert again when there is already a correct trigger received */
#define BIT_R_DIS_ABORT_TB_ELY                     ((u32)0x00000001 << 1)          /*!<R/W 0  1:will abort ptcl generating tb data when macrx_active is low 0:will abort ptcl generating tb data when new rxtrig req_in is assert */
#define BIT_IC_MACPHY_M                            ((u32)0x00000001 << 0)          /*!<R/W 0  To make ASIC become BB+RF only, and put out the MAC_PHY interface */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TCR
 * @brief Transmission Configuration Register
 * @{
 *****************************************************************************/
#define BIT_WMAC_EN_RTS_ADDR                       ((u32)0x00000001 << 31)          /*!<R/W 0  when set to 1, enable change RTS multicast address */
#define BIT_WMAC_DISABLE_CCK                       ((u32)0x00000001 << 30)          /*!<R/W 0  when set to 1, disable cck rate packet */
#define BIT_WMAC_RAW_LEN                           ((u32)0x00000001 << 29)          /*!<R/W 1  1: use raw ppdu_len 0: use 4byte alignment ppdu_len */
#define BIT_WMAC_NOTX_IN_RXNDP                     ((u32)0x00000001 << 28)          /*!<R/W 0  1: block original tx when receiving NDP packet */
#define BIT_WMAC_EN_EOF                            ((u32)0x00000001 << 27)          /*!<R/W 1  when set to 1, enable single ampdu for qos packet. In early mode,not set to 1. */
#define BIT_WMAC_BF_SEL                            ((u32)0x00000001 << 26)          /*!<R/W 0  1: set ndp_rx_standby when rx ppdu of ndpa end; 0: set ndp_rx_standby when rx ndpa packet fcs end; */
#define BIT_WMAC_ANTMODE_SEL                       ((u32)0x00000001 << 25)          /*!<R/W 0  0:4x1 mode, 1:8x1 mode */
#define BIT_WMAC_TCRPWRMGT_HWCTL                   ((u32)0x00000001 << 24)          /*!<R/W 0  If this bit is set to 1, MAC will overwrite power bit of frame control according to PWR_ST. (only for RTS/CTS/ACK/CFEND/BA/BAR frames) */
#define BIT_WMAC_SMOOTH_VAL                        ((u32)0x00000001 << 23)          /*!<R/W 0  smooth value of plcp header HTSIG2 */
#define BIT_WMAC_EN_SCRAM_INC                      ((u32)0x00000001 << 22)          /*!<R/W 0  Increase 1 in service field scrambler seed per tx end */
#define BIT_UNDERFLOWEN_CMPLEN_SEL                 ((u32)0x00000001 << 21)          /*!<R/W 1  compare length selector for enable condition of underflow, consider plcp header when set 1 */
#define BIT_FETCH_MPDU_AFTER_WSECRDY               ((u32)0x00000001 << 20)          /*!<R/W 1  If set to 1,mactx_mpdu_gen will fetch mpdu after wsec is ready when sectype is not tkip/wep40/wep104 */
#define BIT_WMAC_TCR_EN_20MST                      ((u32)0x00000001 << 19)          /*!<R/W 0  when set to 1, enable ndpa signaling TA when BW =20MHz */
#define BIT_WMAC_DIS_SIGTA                         ((u32)0x00000001 << 18)          /*!<R/W 0  when set to 1, disable signaling TA information for response packet */
#define BIT_WMAC_DIS_A2B0                          ((u32)0x00000001 << 17)          /*!<R/W 0  when set to 1, not check bit 0 of rx a2 */
#define BIT_WMAC_MSK_SIGBCRC                       ((u32)0x00000001 << 16)          /*!<R/W 0  when set to 1, mask sigb crc in service field for vht packet */
#define BIT_WMAC_TCR_ERRSTEN_3                     ((u32)0x00000001 << 15)          /*!<R/W 0  Use phytxend_ps to reset mactx state machine */
#define BIT_WMAC_TCR_ERRSTEN_2                     ((u32)0x00000001 << 14)          /*!<R/W 0  If txd fifo underflow when txtype is cmpba, reset mactx state machine */
#define BIT_WMAC_TCR_ERRSTEN_1                     ((u32)0x00000001 << 13)          /*!<R/W 0  If txd fifo underflow, reset mactx state machine */
#define BIT_WMAC_TCR_ERRSTEN_0                     ((u32)0x00000001 << 12)          /*!<R/W 0  Phytxend_ps comes but mactx still active, reset mactx state machine */
#define BIT_WMAC_TCR_TXSK_PERPKT                   ((u32)0x00000001 << 11)          /*!<R/W 0  Search key for each mpdu */
#define BIT_WMAC_TCR_ICV                           ((u32)0x00000001 << 10)          /*!<R/W 0  Integrity Check Value. This bit indicates the existence of an ICV appended to an encipherment packet. 1: No ICV appended 0: ICV appended */
#define BIT_WMAC_CFENDFOR                          ((u32)0x00000001 << 9)          /*!<R/W 0  CF-End Frame Format.The Address 2 field of CF-End is defined to be the address of the STA contained in the AP, but in 802.11n a STA can also send CF-End.The Address 2 field is not specified clearly in this case. This register chooses between MAC ID of the STA or BSSID of the network. 0: BSSID 1: MAC ID */
#define BIT_WMAC_TCR_CRC                           ((u32)0x00000001 << 8)          /*!<R/W 0  Append 32-bit Cyclic Redundancy Check. This bit indicates the existence of a CRC32 appended to a packet. 1: No CRC32 appended 0: A CRC32 is appended */
#define BIT_WMAC_TCR_PWRMGT_CTL                    ((u32)0x00000001 << 7)          /*!<R/W 0  If this bit is set to 1, MAC will overwrite power bit of frame control according to PWR_ST. (only for data frame) */
#define BIT_WMAC_TCR_PWRMGT_VAL                    ((u32)0x00000001 << 6)          /*!<R/W 0  MAC will overwrite power bit of frame control according to PWR_ST.(only for data frame) */
#define BIT_WMAC_TCR_UPD_TIMIE                     ((u32)0x00000001 << 5)          /*!<R/W 0  when set to 1, enable hw to update beacon's time field */
#define BIT_WMAC_TCR_UPD_HGQMD                     ((u32)0x00000001 << 4)          /*!<R/W 0  when set to 1, enable HGQ set moredata bit in Frame control field */
#define BIT_WMAC_TCR_VHTSIGA1_TXPS                 ((u32)0x00000001 << 3)          /*!<R/W 0  This bit controls TXOP_PS_NOT_ALLOWED bit value of VHT-SIG-A1 field in TX VHT packets 1: TXOP_PS_NOT_ALLOWED is set to 1 0: TXOP_PS_NOT_ALLOWED is set to 0 */
#define BIT_WMAC_TCR_PADSEL                        ((u32)0x00000001 << 2)          /*!<R/W 0  AMPDU Padding pattern selection 0: 32-bit Zero padding, 1: Zero length delimiter. */
#define BIT_WMAC_TCR_DISGCLK                       ((u32)0x00000001 << 1)          /*!<R/W 0  when set to 1,disable MACTX clock gating control. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RCR
 * @brief Receive Configuration Register
 * @{
 *****************************************************************************/
#define BIT_APP_FCS                                ((u32)0x00000001 << 31)          /*!<R/W 0  When set, wmac RX will append FCS after payload. */
#define BIT_APP_MIC                                ((u32)0x00000001 << 30)          /*!<R/W 0  When this bit is set to 1, MACRX will retain the MIC at the bottom of the packet. Otherwise, MIC will be removed and reported the status in RXDESC. When a MPDU is encrypted with TKIP without MIC, MACRX will ignore this option, that is, MIC must be remained in back of the plain text. */
#define BIT_APP_ICV                                ((u32)0x00000001 << 29)          /*!<R/W 0  When this bit is set to 1, MACRX will retain the ICV at the bottom of the packet. Otherwise, MIC will be removed and reported the status in RXDESC. */
#define BIT_APP_PHYSTS                             ((u32)0x00000001 << 28)          /*!<R/W 0  Append RXFF0 PHY Status Enable When this bit is set, then PHY Status is appended before RX packet in RXFF. When this bit is set, the RX_DRV_INFO_SZ shall be set to include PHY Status report. */
#define BIT_APP_BASSN                              ((u32)0x00000001 << 27)          /*!<R/W 0  Append SSN of previous TXBA Enable When this bit is set, then SSN of previous TXBA is appended as after original RXDESC as the 4-th DW of RXDESC. */
#define BIT_VHT_DACK                               ((u32)0x00000001 << 26)          /*!<R/W 1  This bit to control response type for vht single mpdu data packet. 1： ACK as response 0： BA as response */
#define BIT_TCPOFLD_EN                             ((u32)0x00000001 << 25)          /*!<R/W 0  When set,Enable TCP checksum offload */
#define BIT_ENMBID                                 ((u32)0x00000001 << 24)          /*!<R/W 0  Enable Multiple Bssid. 1: Enable. Only response ACK to the packets whose DID(A1) matching to the addresses in the MBSSID CAM Entries. 0: Disable. */
#define BIT_LSIGEN                                 ((u32)0x00000001 << 23)          /*!<R/W 0  Enable LSIG TXOP Protection function. 1: Enable. Search KEYCAM for each rx packet to check if LSIGEN bit is set. If LSIGEN is set, then sender support LSIG TXOP protection and we need send response with LSIG TXOP protection 0: Disable */
#define BIT_MFBEN                                  ((u32)0x00000001 << 22)          /*!<R/W 0  Enable immediate MCS Feedback function 1: Enable. When Rx packet with MRQ = 1‟b1, then search KEYCAM to find sender‟s MCS Feedback function and send response 0: Disable */
#define BIT_DISCHKPPDLLEN                          ((u32)0x00000001 << 21)          /*!<R/W 0  When set,do not check PPDU while the PPDU length is smaller than 14 byte. */
#define BIT_PKTCTL_DLKEN                           ((u32)0x00000001 << 20)          /*!<R/W 0  When set,while rx path dead lock occurs, reset rx path */
#define BIT_DISGCLK                                ((u32)0x00000001 << 19)          /*!<R/W 0  When set,disable macrx clock gating control */
#define BIT_TIMPSR_EN                              ((u32)0x00000001 << 18)          /*!<R/W 0  When set, enable RX Beacon TIM Parser */
#define BIT_BCMDINT_EN                             ((u32)0x00000001 << 17)          /*!<R/W 0  Broadcast data packet interrupt enable. When this bit is set, this bit is auto clear by MAC. As long as a data type broadcast packet is received, this bit will be set. */
#define BIT_UCMDINT_EN                             ((u32)0x00000001 << 16)          /*!<R/W 0  Unicast data packet with FC.MD = 1 interrupt enable. When this bit is set, this bit is auto clear by MAC. As long as a data type unicast packet is received, this bit will be set. */
#define BIT_RXSK_PERPKT                            ((u32)0x00000001 << 15)          /*!<R/W 0  Executing key search per MPDU */
#define BIT_HTCBFMC                                ((u32)0x00000001 << 14)          /*!<R/W 0  1: HTC -> MFC 0: MFC-> HTC */
#define BIT_CHK_PREVTXA2                           ((u32)0x00000001 << 13)          /*!<R/W 0  When set,enable check rxa1 with txa2. */
#define BIT_ACK_CBSSID                             ((u32)0x00000001 << 12)          /*!<R/W 0  The field effects behavior of BA/ACK frame. 0:Only A1 match to send BA/ACK frame. 1:A1 match and satisfy BSSID condition(if BIT_CBSSID_DATA = 1) will send BA/ACK frame. If BIT_CBSSID_DATA = 0, hardware send BA/ACK frame decided by A1 match condition. */
#define BIT_CHKTA_MGNT                             ((u32)0x00000001 << 11)          /*!<R/W 0  Check Transmitter_BCN When set to 1, MAC will check the Transmitter Address (A2) of RX beacon or Probe response to determine if this packet belongs to this Transmitter Address (Address 2) . Both in Infrastructure mode and Ad-hoc mode. */
#define BIT_DISDECNMYPKT                           ((u32)0x00000001 << 10)          /*!<R/W 0  This bit determines whether hw need to do decryption. 1: If A1 match or broadcast, do decryption. 0: Do decryption. */
#define BIT_AICV                                   ((u32)0x00000001 << 9)          /*!<R/W 0  Accept Integrity Check Value Error packets. This bit determines whether packets with ICV errors will be accepted or rejected. 1: Accept 0: Reject */
#define BIT_ACRC32                                 ((u32)0x00000001 << 8)          /*!<R/W 0  Accept CRC32 Error packets. When set to 1, packets with CRC32 errors will be accepted. When set to 0, packets with CRC32 errors will be rejected. 1: Accept 0: Reject */
#define BIT_CBSSID_MGNT                            ((u32)0x00000001 << 7)          /*!<R/W 0  Check BSSID_BCN When set to 1, MAC will check the BSSID of RX beacon or Probe response to determine if this packet belongs to this BSSID. */
#define BIT_CBSSID_DATA                            ((u32)0x00000001 << 6)          /*!<R/W 0  Check BSSID_DATA, To DS, From DS Match Packet. When set to 1, MAC will check the Rx data type frame‟s BSSID, To DS, and From DS fields, to determine if it is set to Link OK in an Infrastructure or Ad hoc network. Note this setting is independent of nettype. Driver must set this bit in ad hoc mode and client mode. */
#define BIT_APWRMGT                                ((u32)0x00000001 << 5)          /*!<R/W 0  Accept Power Management Packet. This bit determines whether the RTL8192D will accept or reject packets with the power management bit set. 1: Accept 0: Reject */
#define BIT_ADD3                                   ((u32)0x00000001 << 4)          /*!<R/W 0  Accept Address 3 Match Packets. Set this bit to 1 to accept broadcast/multicast data type frames that Address 3 matching RTL8190‟s MAC address. This bit is valid only when NETYPE (bit 3-2, MSR) is set to Link OK on an Infrastructure network. */
#define BIT_AB                                     ((u32)0x00000001 << 3)          /*!<R/W 0  Accept Broadcast packets. This bit determines whether broadcast packets will be accepted or rejected. 1: Accept 0: Reject */
#define BIT_AM                                     ((u32)0x00000001 << 2)          /*!<R/W 0  Accept Multicast packets. This bit determines whether multicast packets will be accepted or rejected. 1: Accept 0: Reject */
#define BIT_APM                                    ((u32)0x00000001 << 1)          /*!<R/W 0  Accept Physical Match packets. This bit determines whether physical match packets will be accepted or rejected. 1: Accept 0: Reject */
#define BIT_AAP                                    ((u32)0x00000001 << 0)          /*!<R/W 0  Accept Destination Address packets. This bit determines whether packets with a destination address will be accepted or rejected. 1: Accept 0: Reject */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXPKT_CTL
 * @brief RX Packet Control Register
 * @{
 *****************************************************************************/
#define BIT_APP_PHYSTS_PER_SUBMPDU                 ((u32)0x00000001 << 31)          /*!<R/W 0  When this bit is set to 1,append phy status for each mpdu if phy status is valid. Otherwise,append phy status for each ppdu. */
#define BIT_APP_MH_SHIFT_VAL                       ((u32)0x00000001 << 30)          /*!<R/W 0  When this bit is set to 1,append MAC header shift value in rx descriptor,otherwise, not append */
#define BIT_WMAC_ENSHIFT                           ((u32)0x00000001 << 29)          /*!<R/W 0  MAC header shift function enable signal in MACRX */
#define BIT_MASK_RX_DRVINFO_SZ                     ((u32)0x0000000F << 24)          /*!<R/W 0x4  RX DESC Driver Info Size: For RXPKTBUF layout, this field indicates the offset starting from the end of RXDESC to the beginning of RX packet. The unit is 8-byte. This value will be copied to DRV_INFO_SIZE field in RXDESC. */
#define BIT_RX_DRVINFO_SZ(x)                       ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_RX_DRVINFO_SZ(x)                   ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MBID_EN                                ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_MASK_MBID_BCNNO                        ((u32)0x00000007 << 20)          /*!<R/W 0   */
#define BIT_MBID_BCNNO(x)                          ((u32)(((x) & 0x00000007) << 20))
#define BIT_GET_MBID_BCNNO(x)                      ((u32)(((x >> 20) & 0x00000007)))
#define BIT_MASK_RX_DLK_TIME                       ((u32)0x000000FF << 8)          /*!<R/W 0x04  macrx dead lock timer timeout value (unit in 4us). As data_on is deasserted and no RXD is received but macrx still waits coming RXD, the deadlock timer will be enabled in this case */
#define BIT_RX_DLK_TIME(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_RX_DLK_TIME(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_RXPKTLMT                          ((u32)0x0000003F << 0)          /*!<R/W 0x10  RX PKT Upper Limit. The unit is 512 bytes. When “0”, no RX limit is forced. */
#define BIT_RXPKTLMT(x)                            ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_RXPKTLMT(x)                        ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID
 * @brief MAC ID Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID_V1                          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  MAC Identification Number Register BIT[31:0] */
#define BIT_MACID_V1(x)                            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID_V1(x)                        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_H
 * @brief MAC ID Register
 * @{
 *****************************************************************************/
#define BIT_AP_BSSID_FIT_UC                        ((u32)0x00000001 << 21)          /*!<R/W 0  When set1,bssid fit when AP port0 rx unciast data packet */
#define BIT_EN_PORT1                               ((u32)0x00000001 << 17)          /*!<R/W 0x1  When set1,enable PORT1 */
#define BIT_EN_PORT0                               ((u32)0x00000001 << 16)          /*!<R/W 0x1  When set1,enable PORT0 */
#define BIT_MASK_MACID_H                           ((u32)0x0000FFFF << 0)          /*!<R/W 0  MAC Identification Number Register BIT[47:32] */
#define BIT_MACID_H(x)                             ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MACID_H(x)                         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID
 * @brief BSSID Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID_V1                          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Basic Station Set Identification BIT[31:0] */
#define BIT_BSSID_V1(x)                            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BSSID_V1(x)                        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID_H
 * @brief BSSID Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID_H                           ((u32)0x0000FFFF << 0)          /*!<R/W 0  Basic Station Set Identification BIT[47:32] */
#define BIT_BSSID_H(x)                             ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BSSID_H(x)                         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MAR
 * @brief Multicast Address Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MAR_V1                            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Multicast Address Register BIT[31:0]. The value of this register is written by the driver to specify the type of multicast address supported by RTL8192D. The value is created by a hashed calculation that is also implemented by the hardware for multicast address filtering. */
#define BIT_MAR_V1(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MAR_V1(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MAR_H
 * @brief Multicast Address Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MAR_H                             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Multicast Address Register BIT[63:32].The value of this register is written by the driver to specify the type of multicast address supported by RTL8192D. The value is created by a hashed calculation that is also implemented by the hardware for multicast address filtering. */
#define BIT_MAR_H(x)                               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MAR_H(x)                           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MBIDCAMCFG_1
 * @brief MBSSID CAM Configuration Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MBIDCAM_WDATA_L                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  BSSID[31:0] */
#define BIT_MBIDCAM_WDATA_L(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MBIDCAM_WDATA_L(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MBIDCAM_CFG
 * @brief MBSSID CAM Configuration Register
 * @{
 *****************************************************************************/
#define BIT_MBIDCAM_POLL                           ((u32)0x00000001 << 31)          /*!<W 0  Pooling bit. */
#define BIT_MBIDCAM_WT_EN                          ((u32)0x00000001 << 30)          /*!<R/W 0  Write Enable. */
#define BIT_MBIDCAM_DA_EN                          ((u32)0x00000001 << 29)          /*!<R/W 0  CAM Direct Acess Enable */
#define BIT_MASK_MBIDCAM_ADDR_V1                   ((u32)0x0000001F << 24)          /*!<R/W 0  CAM Address */
#define BIT_MBIDCAM_ADDR_V1(x)                     ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_MBIDCAM_ADDR_V1(x)                 ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MBIDCAM_VALID                          ((u32)0x00000001 << 23)          /*!<R/W 0  CAM Valid bit. */
#define BIT_LSIC_TXOP_EN                           ((u32)0x00000001 << 17)          /*!<R/W 0  LSIG TXOP enable */
#define BIT_CTS_EN                                 ((u32)0x00000001 << 16)          /*!<R/W 0  Dual CTS/CFE enable */
#define BIT_MASK_MBIDCAM_RWDATA_H                  ((u32)0x0000FFFF << 0)          /*!<R/W 0  BSSID[47:32] */
#define BIT_MBIDCAM_RWDATA_H(x)                    ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MBIDCAM_RWDATA_H(x)                ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_TCR_CTL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZLD_NUM                           ((u32)0x000000FF << 24)          /*!<R/W 0x8  Fetch zero length delimiter number(unit : 4bytes) when mactx underflow */
#define BIT_ZLD_NUM(x)                             ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_ZLD_NUM(x)                         ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_UDF_THSD_V1                            ((u32)0x00000001 << 23)          /*!<R/W 0x1  BIT_UDF_THSD[7]=1,enable underflow recovery;r_UDF_THSD[7]=0,disable underflow recovery; BIT_UDF_THSD[6:0]: when mac_wrptr - PHY_rdptr is less than this value, set underflow; */
#define BIT_MASK_UDF_THSD_VALUE                    ((u32)0x0000007F << 16)          /*!<R/W 0x6  Underflow threshold */
#define BIT_UDF_THSD_VALUE(x)                      ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_UDF_THSD_VALUE(x)                  ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_WMAC_TCR_TSFT_OFS                 ((u32)0x0000FFFF << 0)          /*!<R/W 0x450  BIT_WMAC_TCR_TSFT_OFS[7:0] is for cck stamp time offset; BIT_WMAC_TCR_TSFT_OFS[15:8] is for ofdm rate time offset; */
#define BIT_WMAC_TCR_TSFT_OFS(x)                   ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_WMAC_TCR_TSFT_OFS(x)               ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_STMP_THSD
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_TCR_E_TSFT_OFS               ((u32)0x000000FF << 8)          /*!<R/W 0x8  BIT_WMAC_TCR_TSFT_OFS[7:0] is for ht/vht/he stamp time offset; */
#define BIT_WMAC_TCR_E_TSFT_OFS(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_WMAC_TCR_E_TSFT_OFS(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_STMP_THSD                         ((u32)0x000000FF << 0)          /*!<R/W 0x2  when PHY_rdptr is greater than this value, then latch STMP value from TSFT */
#define BIT_STMP_THSD(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_STMP_THSD(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_USTIME_EDCA
 * @brief US Time Tuning for EDCA Register
 * @{
 *****************************************************************************/
#define BIT_MASK_SPEC_SIFS_OFDM                    ((u32)0x000000FF << 24)          /*!<R/W 0x10  The spec SIFS value for duration calculation */
#define BIT_SPEC_SIFS_OFDM(x)                      ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_SPEC_SIFS_OFDM(x)                  ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_SPEC_SIFS_CCK                     ((u32)0x000000FF << 16)          /*!<R/W 0x0A  The spec SIFS value for duration calculation */
#define BIT_SPEC_SIFS_CCK(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_SPEC_SIFS_CCK(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_USTIME_EDCA                       ((u32)0x000000FF << 0)          /*!<R/W 0x28  Sets the microsecond time unit used by MAC EDCA clock. */
#define BIT_USTIME_EDCA(x)                         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_USTIME_EDCA(x)                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RESP_SIFS_CCK
 * @brief Response SIFS for CCK Register
 * @{
 *****************************************************************************/
#define BIT_MASK_ACKTO_CCK                         ((u32)0x00000FFF << 12)          /*!<R/W 0x7E  This register sets the Ack time out value after Tx unicast packet with CCK rate. It is in units of us. */
#define BIT_ACKTO_CCK(x)                           ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_ACKTO_CCK(x)                       ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_ACKTO_OFDM                        ((u32)0x00000FFF << 0)          /*!<R/W 0x2D  This register sets the Ack time out value after Tx unicast packet. It is in units of us. */
#define BIT_ACKTO_OFDM(x)                          ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_ACKTO_OFDM(x)                      ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ACKTO
 * @brief ACK Timeout Register
 * @{
 *****************************************************************************/
#define BIT_MASK_EIFS                              ((u32)0x0000FFFF << 16)          /*!<R/W 0x013A  This register sets the EIFS value mentioned in 802.11 specification. It is in units of us. To receive 1Mbps ACK frame, EIFS = SIFS + 144us (preamble length) + 48us (PLCP header length) + 112us (14 bytes of data) = 314us. */
#define BIT_EIFS(x)                                ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_EIFS(x)                            ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_R_RESP_ERSU_DISUSE_RX                  ((u32)0x00000001 << 13)          /*!<R/W 0  0:rx er su will response tx er su */
#define BIT_R_RESP_DCM_DISUSE_RX                   ((u32)0x00000001 << 12)          /*!<R/W 0  0:rx dcm will response tx dcm when ctrl info is valid and inf0_dcm=1 */
#define BIT_MASK_CTS2TO                            ((u32)0x00000FFF << 0)          /*!<R/W 0x2D  This register sets the CTS2 time out value after CTS1 in Dual CTS sequence. It is in units of us. */
#define BIT_CTS2TO(x)                              ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_CTS2TO(x)                          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAV_THRSHOLD
 * @brief NAV Threshold Register
 * @{
 *****************************************************************************/
#define BIT_MASK_NAV_UPBDY                         ((u32)0x000000FF << 16)          /*!<R/W 0  When NAV update is beyond this value, then NAV update is aborted. When this field is 0x00, NAV_UPPER function is disable. The unit is 128us. */
#define BIT_NAV_UPBDY(x)                           ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_NAV_UPBDY(x)                       ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_RXMYRTS_NAV                       ((u32)0x0000000F << 8)          /*!<R/W 0x4  RX my RTS NAV. As rx my unicast RTS and RxMyRTS_NAV is not equal to zero, then rx NAV is set to wait response transition time and RxMyRTS_NAV to avoid contention with the TXOP originator. If RxMyRTS_NAV is equal to zero, this mechanism is disabled */
#define BIT_RXMYRTS_NAV(x)                         ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_RXMYRTS_NAV(x)                     ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_RTS_RST_DUR                       ((u32)0x000000FF << 0)          /*!<R/W 0x26  RTS NAV Reset Time. This is the value in us, not including the time of CTS frame, to reset the NAV set by an RTS frame if no PHY indication occurred during this time. Refer to 802.11 1999 Section 9.2.5.4 for detailed description. */
#define BIT_RTS_RST_DUR(x)                         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RTS_RST_DUR(x)                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BACAMCMD
 * @brief Block ACK CAM Command Register
 * @{
 *****************************************************************************/
#define BIT_BACAM_POLL                             ((u32)0x00000001 << 31)          /*!<W 0  Polling bit BA CAM */
#define BIT_BACAM_RST                              ((u32)0x00000001 << 17)          /*!<RW1C 0  Set 1 to reset BA CAM. Hardware will set the invalid bit for all entries. After reset is finished this bit is reset to 0. */
#define BIT_BACAM_RW                               ((u32)0x00000001 << 16)          /*!<R/W 0  Set 1 = Write, set 0 = Read */
#define BIT_MASK_TXSBMPMOD                         ((u32)0x00000003 << 14)          /*!<R/W 0  Bit[15]: if macrx_dma idle, grant txsbmp_req Bit[14]: if macrx is idle, grant txsbmp_req 00: If FSM is idle, grant txsbmp_req 01: If FSM and mac rx is idle, grant txsbmp_req 10: If FSM and macrx_dma is idle, grant txsbmp_req 11: if FSM, macrx and macrx_dma is idle, grant txsbmp_req */
#define BIT_TXSBMPMOD(x)                           ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_TXSBMPMOD(x)                       ((u32)(((x >> 14) & 0x00000003)))
#define BIT_RXBACAM_DA_EN                          ((u32)0x00000001 << 13)          /*!<R/W 0  CAM Direct Acess enable */
#define BIT_MASK_BACAM_AD                          ((u32)0x0000003F << 0)          /*!<R/W 0  BA CAM address. Memory is double-word access. */
#define BIT_BACAM_AD(x)                            ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_BACAM_AD(x)                        ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BACAM_WD
 * @brief Block ACK CAM Content Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BA_CONTENT_L                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Used for CAM content read and write access register. Because BA CAM is 64 bit access, the double word (32-bit) responds to the lower 32 bits of CAM content. */
#define BIT_BA_CONTENT_L(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BA_CONTENT_L(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BACAM_WD_H
 * @brief Block ACK CAM Content Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BA_CONTENT_H                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Used for CAM content read and write access register. Because BA CAM is 64 bit access, the double word (32-bit) responds to the upper 32 bits of CAM content. */
#define BIT_BA_CONTENT_H(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BA_CONTENT_H(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LBK_DLY
 * @brief Loopback Delay Register
 * @{
 *****************************************************************************/
#define BIT_BITMAP_FLAG_VO                         ((u32)0x00000001 << 31)          /*!<RW1C 0  When BIT_BITMAP_VO = 1, HW will check received packet's sequence number by BIT_BITMAP_CONDITION, and clear the bit after occur one time which received the correct sequence number. */
#define BIT_BITMAP_FLAG_VI                         ((u32)0x00000001 << 30)          /*!<RW1C 0  When BIT_BITMAP_VI = 1, HW will check received packet’s sequence number by BIT_BITMAP_CONDITION, and clear the bit after occur one time which received the correct sequence number. */
#define BIT_BITMAP_FLAG_BE                         ((u32)0x00000001 << 29)          /*!<RW1C 0  When BIT_BITMAP_BE = 1, HW will check received packet's sequence number by BIT_BITMAP_CONDITION, and clear the bit after occur one time which received the correct sequence number. */
#define BIT_BITMAP_FLAG_BK                         ((u32)0x00000001 << 28)          /*!<RW1C 0  When BIT_BITMAP_BK = 1, HW will check received packet's sequence number by BIT_BITMAP_CONDITION, and clear the bit after occur one time which received the correct sequence number. */
#define BIT_MASK_BITMAP_CONDITION                  ((u32)0x00000003 << 26)          /*!<R/W 0  ADDBA condition: 0: HW start sequence - 64. 1: HW start sequence - 128. 2: HW start sequence - 256. 3: HW start sequence - 512. */
#define BIT_BITMAP_CONDITION(x)                    ((u32)(((x) & 0x00000003) << 26))
#define BIT_GET_BITMAP_CONDITION(x)                ((u32)(((x >> 26) & 0x00000003)))
#define BIT_BITMAP_SSNBK_COUNTER_CLR               ((u32)0x00000001 << 25)          /*!<RW1C 0  Clear BIT_BITMAP_SSNBK_COUNTER. */
#define BIT_BITMAP_FORCE                           ((u32)0x00000001 << 24)          /*!<R/W 0  Under the BIT_BITMAP_EN = 1 condition. 1: Rx’s packet always checks sequence number by BIT_BITMAP_CONDITION. 0: Disable */
#define BIT_MASK_RXPKT_TYPE                        ((u32)0x0000003F << 18)          /*!<R 0  The type and subtype field of frame control of rx packet */
#define BIT_RXPKT_TYPE(x)                          ((u32)(((x) & 0x0000003F) << 18))
#define BIT_GET_RXPKT_TYPE(x)                      ((u32)(((x >> 18) & 0x0000003F)))
#define BIT_TXACT_IND                              ((u32)0x00000001 << 17)          /*!<R 0  Indicate MAC/PHY is busy to tx packet */
#define BIT_RXACT_IND                              ((u32)0x00000001 << 16)          /*!<R 0  Indicate MAC is busy to rx packet */
#define BIT_MASK_BITMAP_SSNBK_COUNTER              ((u32)0x0000003F << 10)          /*!<R 0  Count the sequence number out of BITMAP condition (0x663[3:2]). */
#define BIT_BITMAP_SSNBK_COUNTER(x)                ((u32)(((x) & 0x0000003F) << 10))
#define BIT_GET_BITMAP_SSNBK_COUNTER(x)            ((u32)(((x >> 10) & 0x0000003F)))
#define BIT_BITMAP_EN                              ((u32)0x00000001 << 9)          /*!<R/W 0  1:Enable of counter and WMAC_BITMAP_CTL(0x663) 0:Disable function */
#define BIT_BACAM_RPMEN                            ((u32)0x00000001 << 8)          /*!<R/W 0  1:Enable repeat mode 0:Disable function */
#define BIT_MASK_LBK_DLY                           ((u32)0x0000001F << 0)          /*!<R/W 0x19  Loopback Delay. In unit of 'us'; indicates the delay after which the tdrdy_mac starts. In wep/tkip mode, the delay should be larger than 15, because the initial delay of security is about 1320 cycles, approximately to 16.5 us. */
#define BIT_LBK_DLY(x)                             ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_LBK_DLY(x)                         ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXERR_RPT
 * @brief RX Error Report Register
 * @{
 *****************************************************************************/
#define BIT_MASK_RXERR_RPT_SEL_V1_3_0              ((u32)0x0000000F << 28)          /*!<R/W 0  Rx packet counter report selection bit3~bit0 RXERR_RPT_SEL[25,26,31:28]: Counter description 6'd0: OFDM MPDU OK counter 6'd1: OFDM MPDU Fail counter 6'd2: OFDM False Alarm counter 6'd3: CCK MPDU OK counter 6'd4: CCK MPDU Fail counter 6'd5: CCK False Alarm counter; 6'd6: HT MPDU OK counter 6'd7: HT MPDU Fail counter 6'd8: HT PPDU PLCP ok counter 6'd9: HT False Alarm counter 6'd10: Rx full drop packet counter 6'd11: RSVD 6'd12: user define 0 counter 6'd13: user define 1 counter 6'd14: user define 2 counter 6'd15: user define 3 counter 6'd16: VHT SU MPDU OK counter 6'd17: VHT SU MPDU Fail counter 6'd18: VHT SU PPDU plcp ok counter 6'd19: VHT SU False Alarm counter 6'd20: invalid_pkt counter 6'd21: VHT MU MPDU OK counter 6'd22: VHT MU MPDU Fail counter 6'd23: VHT MU PPDU plcp ok counter 6'd24: VHT MU False Alarm counter 6'd25: HE SU MPDU OK counter 6'd26: HE SU MPDU Fail counter 6'd27: HE SU PPDU plcp ok counter 6'd28: HE SU False Alarm counter 6'd29: HE MU MPDU OK counter 6'd30: HE MU MPDU Fail counter 6'd31: HE MU PPDU plcp ok counter 6'd32: HE MU False Alarm counter 6'd33: drop packet counter */
#define BIT_RXERR_RPT_SEL_V1_3_0(x)                ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_RXERR_RPT_SEL_V1_3_0(x)            ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_RXERR_RPT_RST                          ((u32)0x00000001 << 27)          /*!<WA0 0  Write “1” to set the counter to zero. One pulse */
#define BIT_RXERR_RPT_SEL_V1_4                     ((u32)0x00000001 << 26)          /*!<R/W 0  Rx packet counter report selection bit4 */
#define BIT_RXERR_RPT_SEL_V1_5                     ((u32)0x00000001 << 25)          /*!<R/W 0  Rx packet counter report selection bit5 */
#define BIT_UD_SELECT_BSSID_1                      ((u32)0x00000001 << 24)          /*!<R/W 0  {24,22} Select matched BSSID: 2'b0: Port 0 2'b1: Port 1 otherwise, do not care bssid match or not */
#define BIT_W1S                                    ((u32)0x00000001 << 23)          /*!<R/W 0  Write One Setting, it is only valid for RXERR_RPT_SEL (4'd12 ~ 4'd15) */
#define BIT_UD_SELECT_BSSID_0                      ((u32)0x00000001 << 22)          /*!<R/W 0  Select BSSID bit0 */
#define BIT_MASK_UD_SUB_TYPE                       ((u32)0x0000000F << 18)          /*!<R/W 0  User defined: SubType */
#define BIT_UD_SUB_TYPE(x)                         ((u32)(((x) & 0x0000000F) << 18))
#define BIT_GET_UD_SUB_TYPE(x)                     ((u32)(((x >> 18) & 0x0000000F)))
#define BIT_MASK_UD_TYPE                           ((u32)0x00000003 << 16)          /*!<R/W 0  User defined: Type */
#define BIT_UD_TYPE(x)                             ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_UD_TYPE(x)                         ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_RPT_COUNTER                       ((u32)0x0000FFFF << 0)          /*!<R/W 0  The reported counter value corresponding to Report type selected. */
#define BIT_RPT_COUNTER(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_RPT_COUNTER(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_TRXPTCL_CTL
 * @brief WMAC TX/RX Protocol Control Register
 * @{
 *****************************************************************************/
#define BIT_BLK_EDCA_BBSLP                         ((u32)0x00000001 << 31)          /*!<R/W 0  Block EDCA Tx while BB in sleep state */
#define BIT_BLK_EDCA_BBSBY                         ((u32)0x00000001 << 30)          /*!<R/W 0  Block EDCA Tx while BB in standby state */
#define BIT_EN_STOP_UPDNAV                         ((u32)0x00000001 << 28)          /*!<R/W 0  When this bit set 1. If RX packet with EDCA_REF is low, not update NAV. */
#define BIT_RST_EDCA_ACKTO                         ((u32)0x00000001 << 27)          /*!<R/W 1  While in ACKTO duration, reset scheduler top to TX */
#define BIT_RST_EDCA_EIFS                          ((u32)0x00000001 << 26)          /*!<R/W 1  While in EIFS duration, reset scheduler top to TX */
#define BIT_PLCPCHK_RST_EIFS                       ((u32)0x00000001 << 25)          /*!<R/W 1  Reset EIFS counter while receive correct PLCP. */
#define BIT_CCA_RST_EIFS                           ((u32)0x00000001 << 24)          /*!<R/W 0  Reset EIFS counter while CCA on */
#define BIT_DIS_UPD_MYRXPKTNAV                     ((u32)0x00000001 << 23)          /*!<R/W 1  Do not update NAV, while receiving my packet. */
#define BIT_EARLY_TXBA                             ((u32)0x00000001 << 22)          /*!<R/W 0  While phyrxon down, mac_rxactive on. Ignore mac rx busy, respond BA immediately. */
#define BIT_MASK_RESP_CHNBUSY                      ((u32)0x00000003 << 20)          /*!<R/W 0x2  Define channel busy condition in responder to determine RX busy 1: PHYRXON (OFDM_VBON/CCKCCA) 0: CCA (CCKCCA/OFDMCCA) */
#define BIT_RESP_CHNBUSY(x)                        ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_RESP_CHNBUSY(x)                    ((u32)(((x >> 20) & 0x00000003)))
#define BIT_RESP_DCTS_EN                           ((u32)0x00000001 << 19)          /*!<R/W 0  Enable responder send dual CTS for responding Rx RTS */
#define BIT_RESP_DCFE_EN                           ((u32)0x00000001 << 18)          /*!<R/W 0  Enable responder send dual CF-END for responding RX CF-END */
#define BIT_RESP_SPLCPEN                           ((u32)0x00000001 << 17)          /*!<R/W 0  Enable responder send SPLCP CCK response for acknowledging Rx SPLCP CCK packet */
#define BIT_RESP_SGIEN                             ((u32)0x00000001 << 16)          /*!<R/W 0  Enable responder send SGI HT-OFDM response for acknowledging Rx SGI HT-OFDM packet */
#define BIT_RESP_LDPC_EN                           ((u32)0x00000001 << 15)          /*!<R/W 0  if rxpkt with ldpc, response pkt with ldpc */
#define BIT_DIS_RESP_ACKINCCA                      ((u32)0x00000001 << 14)          /*!<R/W 0  Disable to response ack when cca goes high after SIFS */
#define BIT_DIS_RESP_CTSINCCA                      ((u32)0x00000001 << 13)          /*!<R/W 0  8812 Disable to response cts when cca goes high after SIFS */
#define BIT_MASK_SECOND_CCA_TIMER                  ((u32)0x00000007 << 10)          /*!<R/W 0x4  8812 The maximum time interval in us that secondary cca is pull high before primary cca When receive duplicated rts. When station response with cts, if the time that secondary cca pull high before primary cca is larger than this threshold, then secondary cca is judged to be interference, otherwise it is judged to the received RTS frame. default: 3‟b0 */
#define BIT_SECOND_CCA_TIMER(x)                    ((u32)(((x) & 0x00000007) << 10))
#define BIT_GET_SECOND_CCA_TIMER(x)                ((u32)(((x >> 10) & 0x00000007)))
#define BIT_MASK_RFMOD                             ((u32)0x00000003 << 7)          /*!<R/W 0  8812 RF_BW indication, Used to decide response duplicated cts txsc 2‟b00: 20M, 2‟b01: 40M, 2‟b10: 80M e.g: when RF_BW= 80M, duplicated cts BW= 40M, txsc = 9 or 10 when RF_BW = 40M, duplicated cts BW = 40M, txsc = 0 */
#define BIT_RFMOD(x)                               ((u32)(((x) & 0x00000003) << 7))
#define BIT_GET_RFMOD(x)                           ((u32)(((x >> 7) & 0x00000003)))
#define BIT_MASK_RESP_CTS_DYNBW_SEL                ((u32)0x00000003 << 5)          /*!<R/W 0  8812 Response cts bw selection when dynamic rts frame is received 00: current maximum bw indicated by secondary cca 01: bw fall back by one level based on bw in 2‟b00 if not reach 20M 10: bw fall back by one level based on bw in 2‟b01 if not reach 20M 11: bw fall back by one level based on bw in 2‟b10 if not reach 20M e.g, current maximum bw indicated by secondary_cca is 80M, when set to 2‟b00, the response cts bw is 80M, when set to 2‟b01, cts bw is 40M, when set to 2‟b10 or 2‟b11 , cts bw is 20M */
#define BIT_RESP_CTS_DYNBW_SEL(x)                  ((u32)(((x) & 0x00000003) << 5))
#define BIT_GET_RESP_CTS_DYNBW_SEL(x)              ((u32)(((x >> 5) & 0x00000003)))
#define BIT_DLY_TX_WAIT_RXANTSEL                   ((u32)0x00000001 << 4)          /*!<R/W 0  8188E/8812 In some Cases, the phy status may be received too late to be used in the response frame. Enable this bit to postpone the tx of response frame until the phy status is obtained. Otherwise, If the phy status unavailable for its delay, The IOREG value of 0x06D8[6] will be used as before. */
#define BIT_TXRESP_BY_RXANTSEL                     ((u32)0x00000001 << 3)          /*!<R/W 0  8188E/8812 Enable responder send the response frame with the antenna selection information which from the phy status of the previous rx frame. */
#define BIT_MASK_ORIG_DCTS_CHK                     ((u32)0x00000003 << 0)          /*!<R/W 0  Txok condition for dual CTS frame exchange: 00: Either one Rx ok 01: 1st one Rx ok 10: 2nd one Rx ok 11: both Rx ok */
#define BIT_ORIG_DCTS_CHK(x)                       ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_ORIG_DCTS_CHK(x)                   ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_TRXPTCL_CTL_H
 * @brief WMAC TX/RX Protocol Control Register Higher Bit
 * @{
 *****************************************************************************/
#define BIT_MASK_ACKBA_TYPSEL                      ((u32)0x0000000F << 28)          /*!<R/W 0  Determine response type for RX BA, and the definition as the bellow 0: NAV 1: Send ACK ACKBA_TYPESEL[0]: used for Basic BA ACKBA_TYPESEL[1]: used for Compression BA ACKBA_TYPESEL[2]: RSVD ACKBA_TYPESEL[3]: used for MTID BA */
#define BIT_ACKBA_TYPSEL(x)                        ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_ACKBA_TYPSEL(x)                    ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_ACKBA_ACKPCHK                     ((u32)0x0000000F << 24)          /*!<R/W 0  If set 1, then check ACK Policy of BA control field to determine whether send response or not. When this bit set to 1 and a. ACK Policy of Rx BA control field is 0, then response type is controlled by ACLBA_TYPESEL. b. ACK Policy of Rx BA control field is 1, then don't send response If set 0, then ignore the ACK policy of RX BA control field and response type controlled by ACKBA_TYPESEL ACKBA_ACKPCHK[0]: used for Basic BA ACKBA_ACKPCHK[1]: used for Compression BA ACKBA_ACKPCHK[2]: RSVD ACKBA_ACKPCHK[3]: used for MTID BA */
#define BIT_ACKBA_ACKPCHK(x)                       ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_ACKBA_ACKPCHK(x)                   ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_ACKBAR_TYPESEL                    ((u32)0x000000FF << 16)          /*!<R/W 0x4  Determine response type for RX BAR, and the definition as the bellow 00: NAV 01: Send BA 10: Send ACK ACKBAR_TYPESEL[1:0]: used for Basic BAR ACKBAR_TYPESEL[3:2]: used for Compression BAR ACKBAR_TYPESEL[5:4]: RSVD ACKBAR_TYPESEL[7:6]: used for MTID BAR */
#define BIT_ACKBAR_TYPESEL(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_ACKBAR_TYPESEL(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_ACKBAR_ACKPCHK                    ((u32)0x0000000F << 12)          /*!<R/W 0x2  If set 1, then check ACK Policy of BAR control field to determine whether send response or not. When this bit set to 1 and a. ACK Policy of Rx BAR control field is 0, then response type is controlled by ACLBAR_TYPESEL. b. ACK Policy of Rx BAR control field is 1, then don't send response If set 0, then ignore the ACK policy of RX BAR control field and response type controlled by ACKBA_TYPESEL ACKBA_ACKPCHK[0]: used for Basic BAR ACKBA_ACKPCHK[1]: used for Compression BAR ACKBA_ACKPCHK[2]: RSVD ACKBA_ACKPCHK[3]: used for MTID BAR */
#define BIT_ACKBAR_ACKPCHK(x)                      ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_ACKBAR_ACKPCHK(x)                  ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_RXBA_IGNOREA2_V1                       ((u32)0x00000001 << 10)          /*!<R/W 0  Disable check A2 field of RX BA */
#define BIT_EN_SAVE_ALL_TXOPADDR_V1                ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_EN_TXCTS_TO_TXOPOWNER_INRXNAV_V1       ((u32)0x00000001 << 8)          /*!<R/W 0  together with bit41, implement 802.11ac txop owner function */
#define BIT_DIS_TXBA_AMPDUFCSERR_V1                ((u32)0x00000001 << 7)          /*!<R/W 0  Disable to send BA for responding RX AMPDU with FCS error subframe */
#define BIT_DIS_TXBA_RXBARINFULL_V1                ((u32)0x00000001 << 6)          /*!<R/W 0  Disable to send BA for responding RX control wrapper BAR dropped by rxpkbuffer full */
#define BIT_DIS_TXCFE_INFULL_V1                    ((u32)0x00000001 << 5)          /*!<R/W 0  Disable to send CF-END for responding RX control wrapper CF-END dropped by rxpktbuffer full. This bit is valid when RESP_DCFE_EN is set to 1 */
#define BIT_DIS_TXCTS_INFULL_V1                    ((u32)0x00000001 << 4)          /*!<R/W 0  Disable to send CTS for responding RX control wrapper RTS dropped by rxpktbuffer full */
#define BIT_EN_TXACKBA_INTX_RDG_V1                 ((u32)0x00000001 << 3)          /*!<R/W 0  If this bit is set, then MAC will Tx ACK/BA to respond previous rx my unicast packets send by another STA during TX RDG */
#define BIT_EN_TXACKBA_INTXOP_V1                   ((u32)0x00000001 << 2)          /*!<R/W 1  If this bit is set, then MAC will Tx ACK/BA to respond previous rx my unitcast packets send by another STA during my TXOP */
#define BIT_EN_TXCTS_INRXNAV_V1                    ((u32)0x00000001 << 1)          /*!<R/W 1  Enable to send CTS in Rx NAV */
#define BIT_EN_TXCTS_INTXOP_V1                     ((u32)0x00000001 << 0)          /*!<R/W 1  Enable to send CTS for responding RX RTS during our TXOP period */
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAMCMD
 * @brief CAM Command Register
 * @{
 *****************************************************************************/
#define BIT_KEYCAM_POLLING                         ((u32)0x00000001 << 31)          /*!<R 0  Security CAM Polling */
#define BIT_KEYCAM_CLEAR                           ((u32)0x00000001 << 30)          /*!<W1C 0  Set to 1 to clear all valid bits in CAM. After reset will self clear to 0. */
#define BIT_MFBCAM_CLEAR                           ((u32)0x00000001 << 29)          /*!<W1C 0  Write 1 to clear all MFB value in CAM to 7'h7F. After reset will self clear to 0 */
#define BIT_KEYCAM_DA_EN                           ((u32)0x00000001 << 17)          /*!<R/W 0  Security CAM Direct Acess Enable */
#define BIT_SECCAM_WE                              ((u32)0x00000001 << 16)          /*!<R/W 0  Security CAM Write Enable */
#define BIT_MASK_KEYCAM_AD                         ((u32)0x000003FF << 0)          /*!<R/W 0  Security CAM Address Offset CAM address is organized into 8 double-words/entry. */
#define BIT_KEYCAM_AD(x)                           ((u32)(((x) & 0x000003FF) << 0))
#define BIT_GET_KEYCAM_AD(x)                       ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAM_WD
 * @brief Security CAM Content Register
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM_WDATA                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Security CAM Write Content */
#define BIT_KEYCAM_WDATA(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_KEYCAM_WDATA(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CAMREAD
 * @brief CAM Read Register
 * @{
 *****************************************************************************/
#define BIT_MASK_CAMR_DATA                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Security CAM Read Content */
#define BIT_CAMR_DATA(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_CAMR_DATA(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CAMDBG
 * @brief CAM Debug Register
 * @{
 *****************************************************************************/
#define BIT_KEYCAM_DBGSEL                          ((u32)0x00000001 << 31)          /*!<R/W 0  Select TX/RX CAM Information. Set to 1 to retrieve latest Tx search result. Set to 0 to retrieve latest Rx search result. */
#define BIT_SEC_KEYFOUND_V1                        ((u32)0x00000001 << 15)          /*!<R 0  Security Key Found in CAM 0: key not found 1: key found */
#define BIT_MASK_CAMDBG_SEC_TYPE_V1                ((u32)0x00000007 << 12)          /*!<R 0  Found security key type */
#define BIT_CAMDBG_SEC_TYPE_V1(x)                  ((u32)(((x) & 0x00000007) << 12))
#define BIT_GET_CAMDBG_SEC_TYPE_V1(x)              ((u32)(((x >> 12) & 0x00000007)))
#define BIT_EXT_SECTYPE                            ((u32)0x00000001 << 11)          /*!<R 0  Found security key ext_sectype */
#define BIT_MASK_CAMDBG_MIC_KEY_IDX_V3             ((u32)0x0000001F << 5)          /*!<R 0  Mic KEY INDEX */
#define BIT_CAMDBG_MIC_KEY_IDX_V3(x)               ((u32)(((x) & 0x0000001F) << 5))
#define BIT_GET_CAMDBG_MIC_KEY_IDX_V3(x)           ((u32)(((x >> 5) & 0x0000001F)))
#define BIT_MASK_CAMDBG_SEC_KEY_IDX_V2             ((u32)0x0000001F << 0)          /*!<R 0  Security KEY INDEX */
#define BIT_CAMDBG_SEC_KEY_IDX_V2(x)               ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_CAMDBG_SEC_KEY_IDX_V2(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SECCFG
 * @brief Security Configuration Register
 * @{
 *****************************************************************************/
#define BIT_MASK_RXFILTER_ACTION_1                 ((u32)0x000000FF << 24)          /*!<R/W 0  Configured by FW, Refer to 0x68E[10] to decide accepting this frame or not */
#define BIT_RXFILTER_ACTION_1(x)                   ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_RXFILTER_ACTION_1(x)               ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_RXFILTER_CATEGORY_1               ((u32)0x000000FF << 16)          /*!<R/W 0  Configured by FW, Refer to 0x68E[10] to decide accepting this frame or not */
#define BIT_RXFILTER_CATEGORY_1(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_RXFILTER_CATEGORY_1(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_DIS_GCLK_WAPI                          ((u32)0x00000001 << 15)          /*!<R/W 0  Disable gated clock control for WAPI engine */
#define BIT_DIS_GCLK_AES                           ((u32)0x00000001 << 14)          /*!<R/W 0  Disable gated clock control for AES engine. */
#define BIT_DIS_GCLK_TKIP                          ((u32)0x00000001 << 13)          /*!<R/W 0  Disable gated clock control for WEP/TKIP engine. */
#define BIT_AES_SEL_QC_1                           ((u32)0x00000001 << 12)          /*!<R/W 0  1: use QC[7] to compute mic 0: not use QC[7] to compute mic */
#define BIT_AES_SEL_QC_0                           ((u32)0x00000001 << 11)          /*!<R/W 0  1:use QC[7] to compute ctr 0: not use QC[7] to compute ctr */
#define BIT_WMAC_CKECK_BMC                         ((u32)0x00000001 << 9)          /*!<R/W 0  If this bit is set to 1, key search engine need to check if broadcast/multicast bit matched. */
#define BIT_CHK_KEYID                              ((u32)0x00000001 << 8)          /*!<R/W 0  If this bit is set to 1, key search engine need to check if key ID matched. If this bit is set to 0, then key ID table must be filled with 2‟b00. */
#define BIT_RXBCUSEDK                              ((u32)0x00000001 << 7)          /*!<R/W 0  Force RX Broadcast packets Use Default Key If this bit is set to 1, use default key for RX key search when received packet is broadcast and multicast. */
#define BIT_TXBCUSEDK                              ((u32)0x00000001 << 6)          /*!<R/W 0  Force Tx Broadcast packets Use Default Key If this bit is set to 1, use default key for TX key search when transmitted packet is broadcast and multicast. */
#define BIT_NOSKMC                                 ((u32)0x00000001 << 5)          /*!<R/W 0  No Key Search for Multicast Setting 1 to disable key search for multicast frames. Used in MBSSID AP mode. Software should manage security encryption and decryption. */
#define BIT_SKBYA2                                 ((u32)0x00000001 << 4)          /*!<R/W 0  Search Key by A2 Setting 1 to search security CAM by address 2 for TX and search CAM by address 1 for RX. Setting 0 to search security CAM by address 1 for TX and search CAM by address 2 for RX. Used in MBSSID client mode. */
#define BIT_RXDEC                                  ((u32)0x00000001 << 3)          /*!<R/W 1  Enable Rx Decryption Setting 1 to enable Rx hardware decryption. */
#define BIT_TXENC                                  ((u32)0x00000001 << 2)          /*!<R/W 1  Enable Tx Encryption Setting to 1 to enable Tx hardware encryption. */
#define BIT_RXUHUSEDK                              ((u32)0x00000001 << 1)          /*!<R/W 0  Force RX Unicast Use Default Key If this bit is set to 1, use default key for RX key search when pairwise key is not found. */
#define BIT_TXUHUSEDK                              ((u32)0x00000001 << 0)          /*!<R/W 0  Force Tx Unicast Use Default Key If this bit is set to 1, use default key for TX key search when pairwise key is not found. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXFILTER_CATEGORY_ACTION
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXFILTER_ACTION_3                 ((u32)0x000000FF << 24)          /*!<R/W 0x0  Configured by FW, Refer to 0x68E[15] to decide accepting this frame or not */
#define BIT_RXFILTER_ACTION_3(x)                   ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_RXFILTER_ACTION_3(x)               ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_RXFILTER_CATEGORY_3               ((u32)0x000000FF << 16)          /*!<R/W 0x0  Configured by FW, Refer to 0x68E[15] to decide accepting this frame or not */
#define BIT_RXFILTER_CATEGORY_3(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_RXFILTER_CATEGORY_3(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_RXFILTER_ACTION_2                 ((u32)0x000000FF << 8)          /*!<R/W 0x0  Configured by FW, Refer to 0x68E[11] to decide accepting this frame or not */
#define BIT_RXFILTER_ACTION_2(x)                   ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_RXFILTER_ACTION_2(x)               ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_RXFILTER_CATEGORY_2               ((u32)0x000000FF << 0)          /*!<R/W 0x0  Configured by FW, Refer to 0x68E[11] to decide accepting this frame or not */
#define BIT_RXFILTER_CATEGORY_2(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RXFILTER_CATEGORY_2(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXFLTMAP3
 * @brief RX Filter Map Group 0
 * @{
 *****************************************************************************/
#define BIT_CTRLFLT15EN_FW                         ((u32)0x00000001 << 31)          /*!<R/W 0  When set, frames with type=01 and subtype=1111 are received into FWFF. */
#define BIT_CTRLFLT14EN_FW                         ((u32)0x00000001 << 30)          /*!<R/W 0  When set, frames with type=01 and subtype=1110 are received into FWFF. */
#define BIT_CTRLFLT13EN_FW                         ((u32)0x00000001 << 29)          /*!<R/W 0  When set, frames with type=01 and subtype=1101 are received into FWFF. */
#define BIT_CTRLFLT12EN_FW                         ((u32)0x00000001 << 28)          /*!<R/W 0  When set, frames with type=01 and subtype=1100 are received into FWFF. */
#define BIT_CTRLFLT11EN_FW                         ((u32)0x00000001 << 27)          /*!<R/W 0  When set, frames with type=01 and subtype=1011 are received into FWFF. */
#define BIT_CTRLFLT10EN_FW                         ((u32)0x00000001 << 26)          /*!<R/W 0  When set, frames with type=01 and subtype=1010 are received into FWFF. */
#define BIT_CTRLFLT9EN_FW                          ((u32)0x00000001 << 25)          /*!<R/W 0  When set, frames with type=01 and subtype=1001 are received into FWFF. */
#define BIT_CTRLFLT8EN_FW                          ((u32)0x00000001 << 24)          /*!<R/W 0  When set, frames with type=01 and subtype=1000 are received into FWFF. */
#define BIT_CTRLFLT7EN_FW                          ((u32)0x00000001 << 23)          /*!<R/W 0  When set, frames with type=01 and subtype=0111 are received into FWFF. */
#define BIT_CTRLFLT6EN_FW                          ((u32)0x00000001 << 22)          /*!<R/W 0  When set, frames with type=01 and subtype=0110 are received into FWFF. */
#define BIT_CTRLFLT5EN_FW                          ((u32)0x00000001 << 21)          /*!<R/W 0  When set, frames with type=01 and subtype=0101 are received into FWFF. */
#define BIT_CTRLFLT4EN_FW                          ((u32)0x00000001 << 20)          /*!<R/W 0  When set, frames with type=01 and subtype=0100 are received into FWFF. */
#define BIT_CTRLFLT3EN_FW                          ((u32)0x00000001 << 19)          /*!<R/W 0  When set, frames with type=01 and subtype=0011 are received into FWFF. */
#define BIT_CTRLFLT2EN_FW                          ((u32)0x00000001 << 18)          /*!<R/W 0  When set, frames with type=01 and subtype=0010 are received into FWFF. */
#define BIT_CTRLFLT1EN_FW                          ((u32)0x00000001 << 17)          /*!<R/W 0  When set, frames with type=01 and subtype=0001 are received into FWFF. */
#define BIT_CTRLFLT0EN_FW                          ((u32)0x00000001 << 16)          /*!<R/W 0  When set, frames with type=01 and subtype=0000 are received into FWFF. */
#define BIT_MGTFLT15EN_FW                          ((u32)0x00000001 << 15)          /*!<R/W 0  When set, frames with type=00 and subtype=1111 are received into FWFF. */
#define BIT_MGTFLT14EN_FW                          ((u32)0x00000001 << 14)          /*!<R/W 0  When set, all the frames with type=00 and subtype=1110 are received into FWFF. When set to 0, refer to 0x68E[15:12] for deciding which action noack frames will be accepted */
#define BIT_MGTFLT13EN_FW                          ((u32)0x00000001 << 13)          /*!<R/W 0  When set, all the frames with type=00 and subtype=1101 are received into FWFF. When set to 0, refer to 0x68E[11:0] for deciding which action frames will be accepted */
#define BIT_MGTFLT12EN_FW                          ((u32)0x00000001 << 12)          /*!<R/W 0  When set, frames with type=00 and subtype=1100 are received into FWFF. */
#define BIT_MGTFLT11EN_FW                          ((u32)0x00000001 << 11)          /*!<R/W 0  When set, frames with type=00 and subtype=1011 are received into FWFF. */
#define BIT_MGTFLT10EN_FW                          ((u32)0x00000001 << 10)          /*!<R/W 0  When set, frames with type=00 and subtype=1010 are received into FWFF. */
#define BIT_MGTFLT9EN_FW                           ((u32)0x00000001 << 9)          /*!<R/W 0  When set, frames with type=00 and subtype=1001 are received into FWFF. */
#define BIT_MGTFLT8EN_FW                           ((u32)0x00000001 << 8)          /*!<R/W 0  When set, frames with type=00 and subtype=1000 are received into FWFF. */
#define BIT_MGTFLT7EN_FW                           ((u32)0x00000001 << 7)          /*!<R/W 0  When set, frames with type=00 and subtype=0111 are received into FWFF. */
#define BIT_MGTFLT6EN_FW                           ((u32)0x00000001 << 6)          /*!<R/W 0  When set, frames with type=00 and subtype=0110 are received into FWFF. */
#define BIT_MGTFLT5EN_FW                           ((u32)0x00000001 << 5)          /*!<R/W 0  When set, frames with type=00 and subtype=0101 are received into FWFF. */
#define BIT_MGTFLT4EN_FW                           ((u32)0x00000001 << 4)          /*!<R/W 0  When set, frames with type=00 and subtype=0100 are received into FWFF. */
#define BIT_MGTFLT3EN_FW                           ((u32)0x00000001 << 3)          /*!<R/W 0  When set, frames with type=00 and subtype=0011 are received into FWFF. */
#define BIT_MGTFLT2EN_FW                           ((u32)0x00000001 << 2)          /*!<R/W 0  When set, frames with type=00 and subtype=0010 are received into FWFF. */
#define BIT_MGTFLT1EN_FW                           ((u32)0x00000001 << 1)          /*!<R/W 0  When set, frames with type=00 and subtype=0001 are received into FWFF. */
#define BIT_MGTFLT0EN_FW                           ((u32)0x00000001 << 0)          /*!<R/W 0  When set, frames with type=00 and subtype=0000 are received into FWFF. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXFLTMAP5
 * @brief RX Filter Map Group 1
 * @{
 *****************************************************************************/
#define BIT_ACTIONFLT15EN_FW                       ((u32)0x00000001 << 31)          /*!<R/W 0  When set, Action noack frame with category = r_RXFILTER_CATEGORY_3(0x686[7:0]) and action = r_RXFILTER_ACTION_3(0x687[7:0]) will be received into FWFF */
#define BIT_ACTIONFLT14EN_FW                       ((u32)0x00000001 << 30)          /*!<R/W 0  When set, VHT compressed beamforming frame will be received into FWFF. */
#define BIT_ACTIONFLT13EN_FW                       ((u32)0x00000001 << 29)          /*!<R/W 0  When set, HT compressed beamforming frame will be received into FWFF. */
#define BIT_ACTIONFLT12EN_FW                       ((u32)0x00000001 << 28)          /*!<R/W 0  When set, CSI frame will be received into FWFF. */
#define BIT_ACTIONFLT11EN_FW                       ((u32)0x00000001 << 27)          /*!<R/W 0  When set, Action frame with category = r_RXFILTER_CATEGORY_2(0x684[7:0]) and action = r_RXFILTER_ACTION_2(0x685[7:0]) will be received into FWFF */
#define BIT_ACTIONFLT10EN_FW                       ((u32)0x00000001 << 26)          /*!<R/W 0  When set, Action frame with category = r_RXFILTER_CATEGORY_1(0x682[7:0]) and action = r_RXFILTER_ACTION_1(0x683[7:0]) will be received into FWFF */
#define BIT_ACTIONFLT9EN_FW                        ((u32)0x00000001 << 25)          /*!<R/W 0  When set, Operating mode notification frame will be received into FWFF. */
#define BIT_ACTIONFLT8EN_FW                        ((u32)0x00000001 << 24)          /*!<R/W 0  When set, Group ID Management frame will be received into FWFF. */
#define BIT_ACTIONFLT7EN_FW                        ((u32)0x00000001 << 23)          /*!<R/W 0  When set, Notify channel width frame will be received into FWFF. */
#define BIT_ACTIONFLT6EN_FW                        ((u32)0x00000001 << 22)          /*!<R/W 0  When set, DELBA frame will be received into FWFF. */
#define BIT_ACTIONFLT5EN_FW                        ((u32)0x00000001 << 21)          /*!<R/W 0  When set, ADDBA Response frame will be received into FWFF. */
#define BIT_ACTIONFLT4EN_FW                        ((u32)0x00000001 << 20)          /*!<R/W 0  When set, ADDBA Request frame will be received into FWFF. */
#define BIT_ACTIONFLT3EN_FW                        ((u32)0x00000001 << 19)          /*!<R/W 0  When set, DELTS frame will be received into FWFF. */
#define BIT_ACTIONFLT2EN_FW                        ((u32)0x00000001 << 18)          /*!<R/W 0  When set, ADDTS Response frame will be received into FWFF. */
#define BIT_ACTIONFLT1EN_FW                        ((u32)0x00000001 << 17)          /*!<R/W 0  When set, ADDTS Request frame will be received into FWFF. */
#define BIT_ACTIONFLT0EN_FW                        ((u32)0x00000001 << 16)          /*!<R/W 0  When set, Channel Switch Announcement frame will be received into FWFF. */
#define BIT_DATAFLT15EN_FW                         ((u32)0x00000001 << 15)          /*!<R/W 0  When set, frames with type=10 and subtype=1111 are received into FWFF. */
#define BIT_DATAFLT14EN_FW                         ((u32)0x00000001 << 14)          /*!<R/W 0  When set, frames with type=10 and subtype=1110 are received into FWFF. */
#define BIT_DATAFLT13EN_FW                         ((u32)0x00000001 << 13)          /*!<R/W 0  When set, frames with type=10 and subtype=1101 are received into FWFF. */
#define BIT_DATAFLT12EN_FW                         ((u32)0x00000001 << 12)          /*!<R/W 0  When set, frames with type=10 and subtype=1100 are received into FWFF. */
#define BIT_DATAFLT11EN_FW                         ((u32)0x00000001 << 11)          /*!<R/W 0  When set, frames with type=10 and subtype=1011 are received into FWFF. */
#define BIT_DATAFLT10EN_FW                         ((u32)0x00000001 << 10)          /*!<R/W 0  When set, frames with type=10 and subtype=1010 are received into FWFF. */
#define BIT_DATAFLT9EN_FW                          ((u32)0x00000001 << 9)          /*!<R/W 0  When set, frames with type=10 and subtype=1001 are received into FWFF. */
#define BIT_DATAFLT8EN_FW                          ((u32)0x00000001 << 8)          /*!<R/W 0  When set, frames with type=10 and subtype=1000 are received into FWFF. */
#define BIT_DATAFLT7EN_FW                          ((u32)0x00000001 << 7)          /*!<R/W 0  When set, frames with type=10 and subtype=0111 are received into FWFF. */
#define BIT_DATAFLT6EN_FW                          ((u32)0x00000001 << 6)          /*!<R/W 0  When set, frames with type=10 and subtype=0110 are received into FWFF. */
#define BIT_DATAFLT5EN_FW                          ((u32)0x00000001 << 5)          /*!<R/W 0  When set, frames with type=10 and subtype=0101 are received into FWFF. */
#define BIT_DATAFLT4EN_FW                          ((u32)0x00000001 << 4)          /*!<R/W 0  When set, frames with type=10 and subtype=0100 are received into FWFF. */
#define BIT_DATAFLT3EN_FW                          ((u32)0x00000001 << 3)          /*!<R/W 0  When set, frames with type=10 and subtype=0011 are received into FWFF. */
#define BIT_DATAFLT2EN_FW                          ((u32)0x00000001 << 2)          /*!<R/W 0  When set, frames with type=10 and subtype=0010 are received into FWFF. */
#define BIT_DATAFLT1EN_FW                          ((u32)0x00000001 << 1)          /*!<R/W 0  When set, frames with type=10 and subtype=0001 are received into FWFF. */
#define BIT_DATAFLT0EN_FW                          ((u32)0x00000001 << 0)          /*!<R/W 0  When set, frames with type=10 and subtype=0000 are received into FWFF. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WOW_NAN_CTRL
 * @brief Wake On WLAN and NAN Control Register
 * @{
 *****************************************************************************/
#define BIT_WMMPS_UAPSD_TID7                       ((u32)0x00000001 << 31)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID7 */
#define BIT_WMMPS_UAPSD_TID6                       ((u32)0x00000001 << 30)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID6 */
#define BIT_WMMPS_UAPSD_TID5                       ((u32)0x00000001 << 29)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID5 */
#define BIT_WMMPS_UAPSD_TID4                       ((u32)0x00000001 << 28)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID4 */
#define BIT_WMMPS_UAPSD_TID3                       ((u32)0x00000001 << 27)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID3 */
#define BIT_WMMPS_UAPSD_TID2                       ((u32)0x00000001 << 26)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID2 */
#define BIT_WMMPS_UAPSD_TID1                       ((u32)0x00000001 << 25)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID1 */
#define BIT_WMMPS_UAPSD_TID0                       ((u32)0x00000001 << 24)          /*!<R/W 0  Disable interrupt 8051 when rx WMMPS UAPSD Qos data with TID0 */
#define BIT_MASK_PORTSEL_PS_RX_INFO                ((u32)0x00000007 << 21)          /*!<R/W 0  The selector for the source of PS RX information (0x692[4:0]). 0 for port 0, 1 for clint0, 2 for clint1, 3 for clint2, 4 for clint3. */
#define BIT_PORTSEL_PS_RX_INFO(x)                  ((u32)(((x) & 0x00000007) << 21))
#define BIT_GET_PORTSEL_PS_RX_INFO(x)              ((u32)(((x >> 21) & 0x00000007)))
#define BIT_RXCTRLIN0                              ((u32)0x00000001 << 20)          /*!<R 0  It is set to 1 if NIC received control frame with correct DA. */
#define BIT_RXMGTIN0                               ((u32)0x00000001 << 19)          /*!<R 0  It is set to 1 if NIC received management frame with correct BSSID and DA (including unicast and BMC). */
#define BIT_RXDATAIN2                              ((u32)0x00000001 << 18)          /*!<R 0  It is set to 1 if NIC received data frame with correct BSSID and DA (including unicast and BMC). */
#define BIT_RXDATAIN1                              ((u32)0x00000001 << 17)          /*!<R 0  It is set to 1 if NIC received data frame with correct BSSID and DA (including unicast and BMC). */
#define BIT_RXDATAIN0                              ((u32)0x00000001 << 16)          /*!<R 0  It is set to 1 if NIC received data frame with correct BSSID and DA (including unicast and BMC). */
#define BIT_NAN_RX_CHKTA                           ((u32)0x00000001 << 10)          /*!<R/W 0  Enable bit for check TSF filter When set to 1, the TSF address filter will enable. When set to 0, the TSF address filter will disable. */
#define BIT_NAN_RX_CHKBSSID                        ((u32)0x00000001 << 9)          /*!<R/W 0  Check BSSID_BCN for TSF When set to 1, MAC will check the BSSID (A3) of RX beacon or Probe to determine if this packet will sync TSF or not */
#define BIT_NAN_RX_FILTER_EN                       ((u32)0x00000001 << 8)          /*!<R/W 0  When set to 1, the TSF address filter will enable. When set to 0, the TSF address filter will disable. */
#define BIT_MASK_PSF_BSSIDSEL_H                    ((u32)0x00000003 << 6)          /*!<R/W 0  Bit 1,2 of Select to check the MACID of which port for the WMAC PSF NSARP function. 0: to check the MACID of port0; 1: to check the MACID of port1. 2: to check the MACID of port2. 3: to check the MACID of port3. 4: to check the MACID of port4. */
#define BIT_PSF_BSSIDSEL_H(x)                      ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_PSF_BSSIDSEL_H(x)                  ((u32)(((x >> 6) & 0x00000003)))
#define BIT_WOWHCI                                 ((u32)0x00000001 << 5)          /*!<R/W 0  This bit is set to allow wake host as wake on wlan event occurring */
#define BIT_PSF_BSSIDSEL_L                         ((u32)0x00000001 << 4)          /*!<R/W 0  Bit 0 of Select to check the MACID of which port for the WMAC PSF NSARP function. 0: to check the MACID of port0; 1: to check the MACID of port1. 2: to check the MACID of port2. 3: to check the MACID of port3. 4: to check the MACID of port4. */
#define BIT_UWF                                    ((u32)0x00000001 << 3)          /*!<R/W 0  Unicast Packet. 1: Enable to check packet‟s DA match MAC address or not. 0: Disable. (default value) */
#define BIT_MAGIC                                  ((u32)0x00000001 << 2)          /*!<R/W 0  Magic Packet. This bit is valid when the PMEn bit of the PMR register is set. The RTL8192 will assert the PMEB signal to wakeup the operating system when a Magic Packet is received. Once the RTL8192 has been enabled for Magic Packet wakeup, it scans incoming packets addressed to the node for a data sequence. The correct sequence indicates to the controller that this is a Magic Packet frame. A Magic Packet frame must also meet the basic requirements: Frame Control + Duration/ID + Destination address + Address 2 + Address 3 + Sequence Control + data + CRC. The destination address may be the node ID of the receiving station or a multicast address, including the broadcast address. The specific sequence consists of 16 duplications of 6 byte ID registers, with no breaks or interrupts. This sequence can be located anywhere within the packet, but must be preceded by a synchronization stream, 6 bytes of FFh. The device will also accept a multicast address, as long as the 16 duplications of the IEEE address match the address of the ID registers. If the Node ID is 11h 22h 33h 44h 55h 66h, then the magic frame‟s format is as follows: Frame Control + Duration/ID + Destination address + Address 2 + Address 3 + Sequence Control + MISC + FF FF FF FF FF FF + MISC + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + 11 22 33 44 55 66 + MISC + CRC. */
#define BIT_WFMSK                                  ((u32)0x00000001 << 1)          /*!<R/W 0  WoW function on or off. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LPNAV_CTRL
 * @brief Low Power NAV Control Register
 * @{
 *****************************************************************************/
#define BIT_LPNAV_EN                               ((u32)0x00000001 << 31)          /*!<R/W 0  Low Power NAV Mode Enable. When this bit is set, enable MAC low power NAV mode */
#define BIT_MASK_LPNAV_EARLY                       ((u32)0x00007FFF << 16)          /*!<R/W 0x0A  When in LPNAV mode, MAC will recall BB and RF into RX idle mode. The unit is us. */
#define BIT_LPNAV_EARLY(x)                         ((u32)(((x) & 0x00007FFF) << 16))
#define BIT_GET_LPNAV_EARLY(x)                     ((u32)(((x >> 16) & 0x00007FFF)))
#define BIT_MASK_LPNAV_THR                         ((u32)0x0000FFFF << 0)          /*!<R/W 0x200  When protected NAV in the received frame is greater than this threshold value, then MAC enters into LPNAV mode. The time unit is 16us. */
#define BIT_LPNAV_THR(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_LPNAV_THR(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WKFMCAM_CMD
 * @brief Wakeup Frame CAM Number Register
 * @{
 *****************************************************************************/
#define BIT_WKFMCAM_WE                             ((u32)0x00000001 << 16)          /*!<R/W 0  CAM Write Enable */
#define BIT_MASK_WKFMCAM_AD                        ((u32)0x000000FF << 8)          /*!<R/W 0  Entry address */
#define BIT_WKFMCAM_AD(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_WKFMCAM_AD(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_CAM_NUM                           ((u32)0x000000FF << 0)          /*!<R/W 0  CAM Entry Number */
#define BIT_CAM_NUM(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_CAM_NUM(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WKFMCAM_WD
 * @brief Wakeup Frame CAM CONTENT Register
 * @{
 *****************************************************************************/
#define BIT_MASK_WKFMCAM_WD                        ((u32)0xFFFFFFFF << 0)          /*!<R/W   CAM Write Data */
#define BIT_WKFMCAM_WD(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_WKFMCAM_WD(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXFLTMAP0
 * @brief RX Filter Map Group 0
 * @{
 *****************************************************************************/
#define BIT_CTRLFLT15EN                            ((u32)0x00000001 << 31)          /*!<R/W 0  When set, frames with type=01 and subtype=1111 are received into RXFF */
#define BIT_CTRLFLT14EN                            ((u32)0x00000001 << 30)          /*!<R/W 0  When set, frames with type=01 and subtype=1110 are received into RXFF */
#define BIT_CTRLFLT13EN                            ((u32)0x00000001 << 29)          /*!<R/W 0  When set, frames with type=01 and subtype=1101 are received into RXFF. */
#define BIT_CTRLFLT12EN                            ((u32)0x00000001 << 28)          /*!<R/W 0  When set, frames with type=01 and subtype=1100 are received into RXFF. */
#define BIT_CTRLFLT11EN                            ((u32)0x00000001 << 27)          /*!<R/W 0  When set, frames with type=01 and subtype=1011 are received into RXFF */
#define BIT_CTRLFLT10EN                            ((u32)0x00000001 << 26)          /*!<R/W 0  When set, frames with type=01 and subtype=1010 are received into RXFF. */
#define BIT_CTRLFLT9EN                             ((u32)0x00000001 << 25)          /*!<R/W 0  When set, frames with type=01 and subtype=1001 are received into RXFF */
#define BIT_CTRLFLT8EN                             ((u32)0x00000001 << 24)          /*!<R/W 0  When set, frames with type=01 and subtype=1000 are received into RXFF */
#define BIT_CTRLFLT7EN                             ((u32)0x00000001 << 23)          /*!<R/W 0  When set, frames with type=01 and subtype=0111 are received into RXFF */
#define BIT_CTRLFLT6EN                             ((u32)0x00000001 << 22)          /*!<R/W 0  When set, frames with type=01 and subtype=0110 are received into RXFF. */
#define BIT_CTRLFLT5EN                             ((u32)0x00000001 << 21)          /*!<R/W 0  When set, frames with type=01 and subtype=0101 are received into RXFF. */
#define BIT_CTRLFLT4EN                             ((u32)0x00000001 << 20)          /*!<R/W 0  When set, frames with type=01 and subtype=0100 are received into RXFF. */
#define BIT_CTRLFLT3EN                             ((u32)0x00000001 << 19)          /*!<R/W 0  When set, frames with type=01 and subtype=0011 are received into RXFF. */
#define BIT_CTRLFLT2EN                             ((u32)0x00000001 << 18)          /*!<R/W 0  When set, frames with type=01 and subtype=0010 are received into RXFF. */
#define BIT_CTRLFLT1EN                             ((u32)0x00000001 << 17)          /*!<R/W 0  When set, frames with type=01 and subtype=0001 are received into RXFF. */
#define BIT_CTRLFLT0EN                             ((u32)0x00000001 << 16)          /*!<R/W 0  When set, frames with type=01 and subtype=0000 are received into RXFF. */
#define BIT_MGTFLT15EN                             ((u32)0x00000001 << 15)          /*!<R/W 0  When set, frames with type=00 and subtype=1111 are received into RXFF. */
#define BIT_MGTFLT14EN                             ((u32)0x00000001 << 14)          /*!<R/W 0  When set, frames with type=00 and subtype=1110 are received into RXFF. */
#define BIT_MGTFLT13EN                             ((u32)0x00000001 << 13)          /*!<R/W 0  When set, frames with type=00 and subtype=1101 are received into RXFF. */
#define BIT_MGTFLT12EN                             ((u32)0x00000001 << 12)          /*!<R/W 0  When set, frames with type=00 and subtype=1100 are received into RXFF. */
#define BIT_MGTFLT11EN                             ((u32)0x00000001 << 11)          /*!<R/W 0  When set, frames with type=00 and subtype=1011 are received into RXFF. */
#define BIT_MGTFLT10EN                             ((u32)0x00000001 << 10)          /*!<R/W 0  When set, frames with type=00 and subtype=1010 are received into RXFF */
#define BIT_MGTFLT9EN                              ((u32)0x00000001 << 9)          /*!<R/W 0  When set, frames with type=00 and subtype=1001 are received into RXFF. */
#define BIT_MGTFLT8EN                              ((u32)0x00000001 << 8)          /*!<R/W 0  When set, frames with type=00 and subtype=1000 are received into RXFF */
#define BIT_MGTFLT7EN                              ((u32)0x00000001 << 7)          /*!<R/W 0  When set, frames with type=00 and subtype=0111 are received into RXFF. */
#define BIT_MGTFLT6EN                              ((u32)0x00000001 << 6)          /*!<R/W 0  When set, frames with type=00 and subtype=0110 are received into RXFF. */
#define BIT_MGTFLT5EN                              ((u32)0x00000001 << 5)          /*!<R/W 0  When set, frames with type=00 and subtype=0101 are received into RXFF */
#define BIT_MGTFLT4EN                              ((u32)0x00000001 << 4)          /*!<R/W 0  When set, frames with type=00 and subtype=0100 are received into RXFF. */
#define BIT_MGTFLT3EN                              ((u32)0x00000001 << 3)          /*!<R/W 0  When set, frames with type=00 and subtype=0011 are received into RXFF */
#define BIT_MGTFLT2EN                              ((u32)0x00000001 << 2)          /*!<R/W 0  When set, frames with type=00 and subtype=0010 are received into RXFF */
#define BIT_MGTFLT1EN                              ((u32)0x00000001 << 1)          /*!<R/W 0  When set, frames with type=00 and subtype=0001 are received into RXFF */
#define BIT_MGTFLT0EN                              ((u32)0x00000001 << 0)          /*!<R/W 0  When set, frames with type=00 and subtype=0000 are received into RXFF */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXFLTMAP
 * @brief RX Filter Map Group 1
 * @{
 *****************************************************************************/
#define BIT_DATAFLT15EN                            ((u32)0x00000001 << 15)          /*!<R/W 0  When set, frames with type=10 and subtype=1111 are received into RXFF */
#define BIT_DATAFLT14EN                            ((u32)0x00000001 << 14)          /*!<R/W 0  When set, frames with type=10 and subtype=1110 are received into RXFF */
#define BIT_DATAFLT13EN                            ((u32)0x00000001 << 13)          /*!<R/W 0  When set, frames with type=10 and subtype=1101 are received into RXFF. */
#define BIT_DATAFLT12EN                            ((u32)0x00000001 << 12)          /*!<R/W 0  When set, frames with type=10 and subtype=1100 are received into RXFF. */
#define BIT_DATAFLT11EN                            ((u32)0x00000001 << 11)          /*!<R/W 0  When set, frames with type=10 and subtype=1011 are received into RXFF. */
#define BIT_DATAFLT10EN                            ((u32)0x00000001 << 10)          /*!<R/W 0  When set, frames with type=10 and subtype=1010 are received into RXFF. */
#define BIT_DATAFLT9EN                             ((u32)0x00000001 << 9)          /*!<R/W 0  When set, frames with type=10 and subtype=1001 are received into RXFF */
#define BIT_DATAFLT8EN                             ((u32)0x00000001 << 8)          /*!<R/W 0  When set, frames with type=10 and subtype=1000 are received into RXFF */
#define BIT_DATAFLT7EN                             ((u32)0x00000001 << 7)          /*!<R/W 0  When set, frames with type=10 and subtype=0111 are received into RXFF. */
#define BIT_DATAFLT6EN                             ((u32)0x00000001 << 6)          /*!<R/W 0  When set, frames with type=10 and subtype=0110 are received into RXFF */
#define BIT_DATAFLT5EN                             ((u32)0x00000001 << 5)          /*!<R/W 0  When set, frames with type=10 and subtype=0101 are received into RXFF */
#define BIT_DATAFLT4EN                             ((u32)0x00000001 << 4)          /*!<R/W 0  When set, frames with type=10 and subtype=0100 are received into RXFF. */
#define BIT_DATAFLT3EN                             ((u32)0x00000001 << 3)          /*!<R/W 0  When set, frames with type=10 and subtype=0011 are received into RXFF */
#define BIT_DATAFLT2EN                             ((u32)0x00000001 << 2)          /*!<R/W 0  When set, frames with type=10 and subtype=0010 are received into RXFF. */
#define BIT_DATAFLT1EN                             ((u32)0x00000001 << 1)          /*!<R/W 0  When set, frames with type=10 and subtype=0001 are received into RXFF. */
#define BIT_DATAFLT0EN                             ((u32)0x00000001 << 0)          /*!<R/W 0  When set, frames with type=10 and subtype=0000 are received into RXFF. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_PSR_RPT0
 * @brief Beacon Parser Report Register
 * @{
 *****************************************************************************/
#define BIT_MASK_DTIM_CNT0                         ((u32)0x000000FF << 24)          /*!<R 0  The DTIM count field of the TIM element of Rx beacon */
#define BIT_DTIM_CNT0(x)                           ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_DTIM_CNT0(x)                       ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DTIM_PERIOD0                      ((u32)0x000000FF << 16)          /*!<R 0  The DTIM period field of the TIM element of Rx beacon */
#define BIT_DTIM_PERIOD0(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_DTIM_PERIOD0(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_HAS_MY_BC0                             ((u32)0x00000001 << 15)          /*!<R 0  Indicate BC/MC packets are buffered in transmitted BSSID or AP */
#define BIT_HAS_MY_BSSID0                          ((u32)0x00000001 << 14)          /*!<R 0  Indicate BC/MC packets are buffered in nontransmitted BSSID, SW need check crespponding dtim count field in multiple bssid index field */
#define BIT_HAS_MYAID0                             ((u32)0x00000001 << 13)          /*!<R 0  Indicate has my unicast packet */
#define BIT_RPT_VALID0                             ((u32)0x00000001 << 12)          /*!<R 0  when set to 1, indicate the tim report is valid */
#define BIT_MASK_PS_AID_0                          ((u32)0x000007FF << 0)          /*!<R/W 0  AID0 for MACID */
#define BIT_PS_AID_0(x)                            ((u32)(((x) & 0x000007FF) << 0))
#define BIT_GET_PS_AID_0(x)                        ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FLC_CTRL
 * @brief FW LPS Condition PKT Counter
 * @{
 *****************************************************************************/
#define BIT_FLC_RPCT_V1                            ((u32)0x00000001 << 31)          /*!<R/W 0  When write “1” to this bit, FLC_TRPC will be enabled. */
#define BIT_FLC_MODSL                              ((u32)0x00000001 << 30)          /*!<R/W 0  1:Timer Mode 0:Counter Mode */
#define BIT_MASK_TORRD                             ((u32)0x0000003F << 24)          /*!<R/W 0  It specifies the time-out duration. (unit: 32 ms) */
#define BIT_TORRD(x)                               ((u32)(((x) & 0x0000003F) << 24))
#define BIT_GET_TORRD(x)                           ((u32)(((x >> 24) & 0x0000003F)))
#define BIT_FLC_CMFEN                              ((u32)0x00000001 << 18)          /*!<R/W 0  Count Management Frame (1: Enable, 0:Disable) */
#define BIT_FLC_CCFEN                              ((u32)0x00000001 << 17)          /*!<R/W 0  Count Control Frame (1: Enable, 0:Disable) */
#define BIT_FLC_CDFEN                              ((u32)0x00000001 << 16)          /*!<R/W 0  Count Data Frame (1: Enable, 0:Disable) */
#define BIT_MASK_FLC_PKTTH                         ((u32)0x000000FF << 8)          /*!<R/W 0  Rx PKT number threshold */
#define BIT_FLC_PKTTH(x)                           ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_FLC_PKTTH(x)                       ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_FLC_RPC                           ((u32)0x000000FF << 0)          /*!<R 0  Number of Rx PKT in the interval which is specified by FLC_TRPC. The Rx PKT type is specified by 0x6AE[2:0] */
#define BIT_FLC_RPC(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_FLC_RPC(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXPKTMON_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXBKQPKT_SEQ                      ((u32)0x0000000F << 20)          /*!<R 0  The expected Rx BKQ packet index */
#define BIT_RXBKQPKT_SEQ(x)                        ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_RXBKQPKT_SEQ(x)                    ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_RXBEQPKT_SEQ                      ((u32)0x0000000F << 16)          /*!<R 0  The expected Rx BEQ packet index */
#define BIT_RXBEQPKT_SEQ(x)                        ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_RXBEQPKT_SEQ(x)                    ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_RXVIQPKT_SEQ                      ((u32)0x0000000F << 12)          /*!<R 0  The expected Rx VIQ packet index */
#define BIT_RXVIQPKT_SEQ(x)                        ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_RXVIQPKT_SEQ(x)                    ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_RXVOQPKT_SEQ                      ((u32)0x0000000F << 8)          /*!<R 0  The expected Rx VOQ packet index */
#define BIT_RXVOQPKT_SEQ(x)                        ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_RXVOQPKT_SEQ(x)                    ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_RXBKQPKT_ERR                           ((u32)0x00000001 << 7)          /*!<R 0  Indicate monitor find the Rx BKQ packet without incremental index */
#define BIT_RXBEQPKT_ERR                           ((u32)0x00000001 << 6)          /*!<R 0  Indicate monitor find the Rx BEQ packet without incremental index */
#define BIT_RXVIQPKT_ERR                           ((u32)0x00000001 << 5)          /*!<R 0  Indicate monitor find the Rx VIQ packet without incremental index */
#define BIT_RXVOQPKT_ERR                           ((u32)0x00000001 << 4)          /*!<R 0  Indicate monitor find the Rx VOQ packet without incremental index */
#define BIT_RXDMA_MON_EN                           ((u32)0x00000001 << 2)          /*!<R/W 0  Enable packet loss monitor function for Rx DMA. If this bit is set to 1, then MACRX append the incremental index to fragment field of sequence number field of rxmpdu restored to rxpkbuffer successfully */
#define BIT_RXPKT_MON_EN                           ((u32)0x00000001 << 0)          /*!<R/W 0  Enable packet loss monitor function for Rx packets. If this bit is set to 1, then MACRX uses fragment field of sequence number field of rxmpdu as the mpdu index and check if the index is incremental. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_STATE_MON
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_STATE_SEL                         ((u32)0x0000001F << 24)          /*!<R/W 0  WMAC read and write FSM selection： 000,001：read mactx status 010,011：read mactx status 100：read response status 00xxx: write [6：0]to mactx fsm when [7] valid 01xxx：write [6：0]to macrx fsm when [7] valid 100xx: write [6：0]to response fsm when [7] valid */
#define BIT_STATE_SEL(x)                           ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_STATE_SEL(x)                       ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_CUR_STATE1                        ((u32)0x000000FF << 8)          /*!<R 0  when BIT_WRITE_FSM_EN is valid,is rsvd. When BIT_WRITE_FSM_EN is not valid, means read status */
#define BIT_CUR_STATE1(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_CUR_STATE1(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_WRITE_FSM_EN                           ((u32)0x00000001 << 7)          /*!<R 0  when set 1, generate write pulse, otherwise means read status */
#define BIT_MASK_CUR_STATE0                        ((u32)0x0000007F << 0)          /*!<R/W 0  when BIT_WRITE_FSM_EN is valid,Indicate the FSM current state to write. When BIT_WRITE_FSM_EN is not valid, means read status */
#define BIT_CUR_STATE0(x)                          ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_CUR_STATE0(x)                      ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ERROR_EVT_CTL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CSIRPT_LEN_BB_MORE_THAN_MAC            ((u32)0x00000001 << 23)          /*!<R 0  Indicate MAC Rx csi rxd number overflow */
#define BIT_CSI_CHKSUM_ERROR                       ((u32)0x00000001 << 22)          /*!<R 0  Indicate MAC Rx csi rpt checksum error */
#define BIT_MACRX_ERR_4                            ((u32)0x00000001 << 20)          /*!<R 0  Indicate last MPDU length of AMPDU or PPDU length is error. */
#define BIT_MACRX_ERR_3                            ((u32)0x00000001 << 19)          /*!<R 0  Indicate rxfifo or mac phy interface fifo overflow */
#define BIT_MACRX_ERR_2                            ((u32)0x00000001 << 18)          /*!<R 0  Indicate MAC Rx occurs internal halt event */
#define BIT_MACRX_ERR_1                            ((u32)0x00000001 << 17)          /*!<R 0  Indicate Rx packet length >= 12K bytes */
#define BIT_MACRX_ERR_0                            ((u32)0x00000001 << 16)          /*!<R 0  Indicate MAC Rx occurs halt event */
#define BIT_PRETX_ERRHDL_EN                        ((u32)0x00000001 << 15)          /*!<R/W 0  Pre tx error block enable signal */
#define BIT_MACTX_ERR_3                            ((u32)0x00000001 << 3)          /*!<R 0  Indicate PHY fetch plcp before it is ready */
#define BIT_MACTX_ERR_2                            ((u32)0x00000001 << 2)          /*!<R 0  phy_txen is set ,but wait phy_phytxon time out */
#define BIT_MACTX_ERR_1                            ((u32)0x00000001 << 1)          /*!<R 0  Indicate TXD FIFO underflow,ppdu fetch zld */
#define BIT_MACTX_ERR_0                            ((u32)0x00000001 << 0)          /*!<R 0  Indicate TXD FIFO underflow,mpdu fetch zld */
/** @} */

/**************************************************************************//**
 * @defgroup REG_SEARCH_MACID
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EN_TXRPTBUF_CLK                        ((u32)0x00000001 << 31)          /*!<R/W 0  when set to 1,keep txrptbuf sram always have clock */
#define BIT_MASK_INFO_INDEX_OFFSET                 ((u32)0x00001FFF << 16)          /*!<R/W 0x150  Pre-macid information index address offset. */
#define BIT_INFO_INDEX_OFFSET(x)                   ((u32)(((x) & 0x00001FFF) << 16))
#define BIT_GET_INFO_INDEX_OFFSET(x)               ((u32)(((x >> 16) & 0x00001FFF)))
#define BIT_WMAC_SRCH_FIFOFULL                     ((u32)0x00000001 << 15)          /*!<R/W 0  when set to 1, when fifofull, stop macid search; */
#define BIT_DIS_INFOSRCH                           ((u32)0x00000001 << 14)          /*!<R/W 0  when set to 1,disable to search information */
#define BIT_DISABLE_B0                             ((u32)0x00000001 << 13)          /*!<R/W 0  when set to 1,disable to check a2[0] */
#define BIT_MASK_INFO_ADDR_OFFSET                  ((u32)0x00001FFF << 0)          /*!<R/W 0  per-macid information address offset (Unit:8bytes). */
#define BIT_INFO_ADDR_OFFSET(x)                    ((u32)(((x) & 0x00001FFF) << 0))
#define BIT_GET_INFO_ADDR_OFFSET(x)                ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_COEX_TABLE_V1
 * @brief BT Coexistence Control register
 * @{
 *****************************************************************************/
#define BIT_MASK_COEX_TABLE_1                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  coexistence table, used at dual antenna & out of band */
#define BIT_COEX_TABLE_1(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_COEX_TABLE_1(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_COEX_TABLE2_V1
 * @brief BTCoexistence Control register
 * @{
 *****************************************************************************/
#define BIT_MASK_COEX_TABLE_2                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  coexistence table, used at dual antenna & in band or single antenna */
#define BIT_COEX_TABLE_2(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_COEX_TABLE_2(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_COEX_BREAK_TABLE
 * @brief BT Coexistence Control register
 * @{
 *****************************************************************************/
#define BIT_MASK_BREAK_TABLE_2                     ((u32)0x0000FFFF << 16)          /*!<R/W 0  Table used to break BT activity, when new WLAN request is coming and BT is on transmitting or receiving */
#define BIT_BREAK_TABLE_2(x)                       ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_BREAK_TABLE_2(x)                   ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_BREAK_TABLE_1                     ((u32)0x0000FFFF << 0)          /*!<R/W 0  Table used to break WLAN activity, when new BT request is coming and WLAN is on transmitting or receiving */
#define BIT_BREAK_TABLE_1(x)                       ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BREAK_TABLE_1(x)                   ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_COEX_TABLE_H_V1
 * @brief BT Coexistence Control register
 * @{
 *****************************************************************************/
#define BIT_PRI_MASK_RX_RESP_V1                    ((u32)0x00000001 << 30)          /*!<R/W 0  Priority Mask for RX Response Packet */
#define BIT_PRI_MASK_RXOFDM_V1                     ((u32)0x00000001 << 29)          /*!<R/W 0  Priority Mask for RX OFDM */
#define BIT_PRI_MASK_RXCCK_V1                      ((u32)0x00000001 << 28)          /*!<R/W 0  Priority Mask for RX CCK */
#define BIT_MASK_PRI_MASK_TXAC                     ((u32)0x0000007F << 21)          /*!<R/W 0  Priority Mask for Tx Queue */
#define BIT_PRI_MASK_TXAC(x)                       ((u32)(((x) & 0x0000007F) << 21))
#define BIT_GET_PRI_MASK_TXAC(x)                   ((u32)(((x >> 21) & 0x0000007F)))
#define BIT_MASK_PRI_MASK_NAV                      ((u32)0x000000FF << 13)          /*!<R/W 0  Priority Mask for Tx NAV */
#define BIT_PRI_MASK_NAV(x)                        ((u32)(((x) & 0x000000FF) << 13))
#define BIT_GET_PRI_MASK_NAV(x)                    ((u32)(((x >> 13) & 0x000000FF)))
#define BIT_PRI_MASK_CCK_V1                        ((u32)0x00000001 << 12)          /*!<R/W 0  Priority Mask for Tx CCK */
#define BIT_PRI_MASK_OFDM_V1                       ((u32)0x00000001 << 11)          /*!<R/W 0  Priority Mask for Tx OFDM */
#define BIT_PRI_MASK_RTY_V1                        ((u32)0x00000001 << 10)          /*!<R/W 0  Priority Mask for Tx Retry Packet */
#define BIT_MASK_PRI_MASK_NUM                      ((u32)0x0000000F << 6)          /*!<R/W 0  Priority Mask for Tx packet number */
#define BIT_PRI_MASK_NUM(x)                        ((u32)(((x) & 0x0000000F) << 6))
#define BIT_GET_PRI_MASK_NUM(x)                    ((u32)(((x >> 6) & 0x0000000F)))
#define BIT_MASK_PRI_MASK_TYPE                     ((u32)0x0000000F << 2)          /*!<R/W 0  Priority Mask for Tx packet type [98]: Rx response [99]: Tx response [100]: beacon */
#define BIT_PRI_MASK_TYPE(x)                       ((u32)(((x) & 0x0000000F) << 2))
#define BIT_GET_PRI_MASK_TYPE(x)                   ((u32)(((x >> 2) & 0x0000000F)))
#define BIT_OOB_V1                                 ((u32)0x00000001 << 1)          /*!<R/W 0  out of band indication (0: in band ; 1 : out of band) */
#define BIT_ANT_SEL_V1                             ((u32)0x00000001 << 0)          /*!<R/W 0  single or dual antenna selection (0: single; 1 : dual) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXCMD_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXCMD_EN                               ((u32)0x00000001 << 31)          /*!<R/W 0  RXCMD function enable */
#define BIT_MASK_RXCMD_INFO                        ((u32)0x7FFFFFFF << 0)          /*!<R/W 0  The Information is send to rxpktbuffer for host SW */
#define BIT_RXCMD_INFO(x)                          ((u32)(((x) & 0x7FFFFFFF) << 0))
#define BIT_GET_RXCMD_INFO(x)                      ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXCMD_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXCMD_PRD                         ((u32)0x0000FFFF << 0)          /*!<R/W 0  Define the period unit in us to send the RXCMD_INFO to rxpktbuffer for host SW */
#define BIT_RXCMD_PRD(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_RXCMD_PRD(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RESP_TXINFO_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RESP_MFB                          ((u32)0x0000007F << 25)          /*!<R/W 0  MFB information in register for response packet */
#define BIT_RESP_MFB(x)                            ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_RESP_MFB(x)                        ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_ANTINF_SEL                        ((u32)0x00000003 << 23)          /*!<R/W 0  Antsel information for response frame in register or ctrl info 00: Antsel_en always disable, 01:Antsel_en field follow ctrl info 1X:Antsel_en always enable */
#define BIT_ANTINF_SEL(x)                          ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_ANTINF_SEL(x)                      ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_ANTSEL_SEL                        ((u32)0x00000003 << 21)          /*!<R/W 0  antenna select 0: use rx phystatus antsel information; 1: use tx report antsel information; other: use register antsel information. */
#define BIT_ANTSEL_SEL(x)                          ((u32)(((x) & 0x00000003) << 21))
#define BIT_GET_ANTSEL_SEL(x)                      ((u32)(((x >> 21) & 0x00000003)))
#define BIT_MASK_RESP_TXPOWER                      ((u32)0x00000007 << 18)          /*!<R/W 0  tx power setting when send response packet */
#define BIT_RESP_TXPOWER(x)                        ((u32)(((x) & 0x00000007) << 18))
#define BIT_GET_RESP_TXPOWER(x)                    ((u32)(((x >> 18) & 0x00000007)))
#define BIT_MASK_RESP_TXANT                        ((u32)0x0003FFFF << 0)          /*!<R/W 0x3F  tx antenna information */
#define BIT_RESP_TXANT(x)                          ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_RESP_TXANT(x)                      ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CSI_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CTL_IDLE_CLR_CSI_RPT_V1                ((u32)0x00000001 << 31)          /*!<R/W 0  ckr csi report request if response fsm idle */
#define BIT_USE_NDPARATE                           ((u32)0x00000001 << 30)          /*!<R/W 1  1: use ndpa rx rate to decide csi report rate 0: use ndp rx rate to decide csi report rate */
#define BIT_WMAC_CSI_RATE_FORCE_EN                 ((u32)0x00000001 << 15)          /*!<R/W 0  0: disable CSI force rate. Refer to RRSR bitmask 1: CSI frame Tx rate refers to 0x6DC[29:24] */
#define BIT_MASK_WMAC_CSI_RSC_FORCE                ((u32)0x00000003 << 13)          /*!<R/W 0  If csi_force_rate_en = 1, CSI frame sub-channel configuration 00: Primary 20Mhz BW(0x483[3:0]) 01: Follow Rx packet BW. 10: Duplicate mode. 11: Reserved */
#define BIT_WMAC_CSI_RSC_FORCE(x)                  ((u32)(((x) & 0x00000003) << 13))
#define BIT_GET_WMAC_CSI_RSC_FORCE(x)              ((u32)(((x >> 13) & 0x00000003)))
#define BIT_CSI_GID_SEL                            ((u32)0x00000001 << 12)          /*!<R/W 0  SIG-A content 1 : group id=0, 0 : group id=63 */
#define BIT_RDCSIMD_FLAG_TRIG_SEL                  ((u32)0x00000001 << 11)          /*!<R/W 0  0: Trigger read csi mode flag raising when mac_csi_granted & ndp_valid positive edge 1: Trigger read csi mode flag raising when mac_csi_granted positive edge */
#define BIT_NDPVLD_POS_RST_FFPTR_DIS_V1            ((u32)0x00000001 << 10)          /*!<R/W 0  0: Reset rx_fifo read/wrtie pointer when ndp_valid positive edge 1: Not reset rx_fifo read/wrtie pointer when ndp_valid positive edge */
#define BIT_NDPVLD_PROTECT_RDRDY_DIS               ((u32)0x00000001 << 9)          /*!<R/W 0  0: Protect write enable of rx_fifo when ndp_valid =1 1: Not Protect write enable of rx_fifo when ndp_valid =1 */
#define BIT_CSIRD_EMPTY_APPZERO                    ((u32)0x00000001 << 8)          /*!<R/W 0  0: Not append zero data when rx_fifo read csi data to empty. If csi data length of BB transmitted is smaller than MAC expected, Tx CSI report would be FCS error. 1: Append zero data when rx_fifo read csi data to empty. If csi data length of BB transmitted is smaller than MAC expected, there would be zero before FCS field of Tx CSI report pkt and the CSI report pkt would be FCS ok */
#define BIT_WMC_CSI_RATE_FB_EN                     ((u32)0x00000001 << 7)          /*!<R/W 0  Enable CSI frame rate fallback (only one step in CSI RRSR) This bit is set to 1 to enable MAC to use previous latched csi resp rate for retry pkt. */
#define BIT_RXFIFO_WRPTR_WO_CHKSUM                 ((u32)0x00000001 << 6)          /*!<R/W 0  This bit is set to 1 when rx_fifo don’t enable csi data checksum. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_P2P_RX_BCN_NOA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_P2P_NOA_PARSER_EN                      ((u32)0x00000001 << 15)          /*!<R/W 0  NoA Parser enabled. */
#define BIT_MASK_P2P_BSSID_SEL_V1                  ((u32)0x00000007 << 12)          /*!<R/W 0  When this valueis 0/1/2/3/4, then beacon with matched BSSID0/1/2/3/4 is parsed. */
#define BIT_P2P_BSSID_SEL_V1(x)                    ((u32)(((x) & 0x00000007) << 12))
#define BIT_GET_P2P_BSSID_SEL_V1(x)                ((u32)(((x >> 12) & 0x00000007)))
#define BIT_MASK_P2P_OUI_TYPE                      ((u32)0x000000FF << 0)          /*!<R/W 0  NOA OUI Type */
#define BIT_P2P_OUI_TYPE(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_P2P_OUI_TYPE(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ASSOCIATED_BFMER0_INFO
 * @brief Associated Beamformer0 Info Register
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_SOUNDING_RXADD_R0_V1       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  MACID of the associated beamformer0 bit[31:0] */
#define BIT_R_WMAC_SOUNDING_RXADD_R0_V1(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_WMAC_SOUNDING_RXADD_R0_V1(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SOUNDING_CFG1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_TXCSI_AID0                 ((u32)0x000001FF << 16)          /*!<R/W 0  P_AID of the associated bemaformer0 */
#define BIT_R_WMAC_TXCSI_AID0(x)                   ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_R_WMAC_TXCSI_AID0(x)               ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_MASK_R_WMAC_SOUNDING_RXADD_R0_H        ((u32)0x0000FFFF << 0)          /*!<R/W 0  MACID of the associated beamformer0 bit[47:32] */
#define BIT_R_WMAC_SOUNDING_RXADD_R0_H(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_SOUNDING_RXADD_R0_H(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SOUNDING_CFG2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_SOUNDING_RXADD_L           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  MACID of the associated beamformer1 bit[31:0] */
#define BIT_R_WMAC_SOUNDING_RXADD_L(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_WMAC_SOUNDING_RXADD_L(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SOUNDING_CFG3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_TXCSI_AID1                 ((u32)0x000001FF << 16)          /*!<R/W 0  P_AID of the associated bemaformer1 */
#define BIT_R_WMAC_TXCSI_AID1(x)                   ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_R_WMAC_TXCSI_AID1(x)               ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_MASK_R_WMAC_SOUNDING_RXADD_H           ((u32)0x0000FFFF << 0)          /*!<R/W 0  MACID of the associated beamformer1 bit[15:0] */
#define BIT_R_WMAC_SOUNDING_RXADD_H(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_WMAC_SOUNDING_RXADD_H(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SOUNDING_CFG0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_BFINFO_20M_1               ((u32)0x00000FFF << 16)          /*!<R/W 0  BF Info of the associated beamformer1 at BW=20M bit[18:16]: Nc index bit[21:19]: Nr index bit[23:22]: grouping bit[25:24]: codebook info bit[27:26]: coefficient size */
#define BIT_R_WMAC_BFINFO_20M_1(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_R_WMAC_BFINFO_20M_1(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_R_WMAC_BFINFO_20M_0               ((u32)0x00000FFF << 0)          /*!<R/W 0  BF Info of the associated beamformer0 at BW=20M bit[2:0]: Nc index bit[5:3]: Nr index bit[7:6]: grouping bit[9:8]: codebook info bit[11:10]: coefficient size */
#define BIT_R_WMAC_BFINFO_20M_0(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_R_WMAC_BFINFO_20M_0(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ANTCD_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WMAC_RESP_ANTD                    ((u32)0x0000000F << 12)          /*!<R/W 0  used for antenna select d value BIT_WMAC_RESP_ANTCD[3:2], used for antsel_d value; */
#define BIT_WMAC_RESP_ANTD(x)                      ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_WMAC_RESP_ANTD(x)                  ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_WMAC_RESP_ANTC                    ((u32)0x0000000F << 8)          /*!<R/W 0  used for antenna select c value BIT_WMAC_RESP_ANTCD[3:3], used for antsel_d value; */
#define BIT_WMAC_RESP_ANTC(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_WMAC_RESP_ANTC(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_WMAC_RESP_ANTB                    ((u32)0x0000000F << 4)          /*!<R/W 0  used for antenna select b value BIT_WMAC_RESP_ANTCD[3:4], used for antsel_d value; */
#define BIT_WMAC_RESP_ANTB(x)                      ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_WMAC_RESP_ANTB(x)                  ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_WMAC_RESP_ANTA                    ((u32)0x0000000F << 0)          /*!<R/W 0  used for antenna select a value BIT_WMAC_RESP_ANTCD[3:5], used for antsel_d value; */
#define BIT_WMAC_RESP_ANTA(x)                      ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_WMAC_RESP_ANTA(x)                  ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CSI_PTR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_CSI_RADDR_LATCH_V2                ((u32)0x0000FFFF << 16)          /*!<R 0  Tx_fifo read pointer latch value after Tx CSI report pkt (unit: 8B) */
#define BIT_CSI_RADDR_LATCH_V2(x)                  ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_CSI_RADDR_LATCH_V2(x)              ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_CSI_WADDR_LATCH_V2                ((u32)0x0000FFFF << 0)          /*!<R 0  Tx_fifo write pointer latch value at ndp_valid negative edge (unit: 8B) */
#define BIT_CSI_WADDR_LATCH_V2(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_CSI_WADDR_LATCH_V2(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_PSR_RPT2
 * @brief Beacon Parser Report Register2
 * @{
 *****************************************************************************/
#define BIT_MASK_DTIM_CNT2                         ((u32)0x000000FF << 24)          /*!<R 0  The DTIM count field of the TIM element of Rx beacon */
#define BIT_DTIM_CNT2(x)                           ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_DTIM_CNT2(x)                       ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DTIM_PERIOD2                      ((u32)0x000000FF << 16)          /*!<R 0  The DTIM period field of the TIM element of Rx beacon */
#define BIT_DTIM_PERIOD2(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_DTIM_PERIOD2(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_HAS_MY_BC2                             ((u32)0x00000001 << 15)          /*!<R 0  Indicate BC/MC packets are buffered in transmitted BSSID or AP */
#define BIT_HAS_MY_BSSID2                          ((u32)0x00000001 << 14)          /*!<R 0  Indicate BC/MC packets are buffered in nontransmitted BSSID, SW need check crespponding dtim count field in multiple bssid index field */
#define BIT_HAS_MYAID2                             ((u32)0x00000001 << 13)          /*!<R 0  Indicate has my unicast packet */
#define BIT_RPT_VALID2                             ((u32)0x00000001 << 12)          /*!<R 0  when set to 1, indicate the tim report is valid */
#define BIT_MASK_PS_AID_2                          ((u32)0x000007FF << 0)          /*!<R/W 0  AID0 for MACID */
#define BIT_PS_AID_2(x)                            ((u32)(((x) & 0x000007FF) << 0))
#define BIT_GET_PS_AID_2(x)                        ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_PSR_RPT3
 * @brief Beacon Parser Report Register3
 * @{
 *****************************************************************************/
#define BIT_MASK_DTIM_CNT3                         ((u32)0x000000FF << 24)          /*!<R 0  The DTIM count field of the TIM element of Rx beacon */
#define BIT_DTIM_CNT3(x)                           ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_DTIM_CNT3(x)                       ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DTIM_PERIOD3                      ((u32)0x000000FF << 16)          /*!<R 0  The DTIM period field of the TIM element of Rx beacon */
#define BIT_DTIM_PERIOD3(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_DTIM_PERIOD3(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_HAS_MY_BC3                             ((u32)0x00000001 << 15)          /*!<R 0  Indicate BC/MC packets are buffered in transmitted BSSID or AP */
#define BIT_HAS_MY_BSSID3                          ((u32)0x00000001 << 14)          /*!<R 0  Indicate BC/MC packets are buffered in nontransmitted BSSID, SW need check crespponding dtim count field in multiple bssid index field */
#define BIT_HAS_MYAID3                             ((u32)0x00000001 << 13)          /*!<R 0  Indicate has my unicast packet */
#define BIT_RPT_VALID3                             ((u32)0x00000001 << 12)          /*!<R 0  when set to 1, indicate the tim report is valid */
#define BIT_MASK_PS_AID_3                          ((u32)0x000007FF << 0)          /*!<R/W 0  AID0 for MACID */
#define BIT_PS_AID_3(x)                            ((u32)(((x) & 0x000007FF) << 0))
#define BIT_GET_PS_AID_3(x)                        ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_PSR_RPT4
 * @brief Beacon Parser Report Register4
 * @{
 *****************************************************************************/
#define BIT_MASK_DTIM_CNT4                         ((u32)0x000000FF << 24)          /*!<R 0  The DTIM count field of the TIM element of Rx beacon */
#define BIT_DTIM_CNT4(x)                           ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_DTIM_CNT4(x)                       ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DTIM_PERIOD4                      ((u32)0x000000FF << 16)          /*!<R 0  The DTIM period field of the TIM element of Rx beacon */
#define BIT_DTIM_PERIOD4(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_DTIM_PERIOD4(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_HAS_MY_BC4                             ((u32)0x00000001 << 15)          /*!<R 0  Indicate BC/MC packets are buffered in transmitted BSSID or AP */
#define BIT_HAS_MY_BSSID4                          ((u32)0x00000001 << 14)          /*!<R 0  Indicate BC/MC packets are buffered in nontransmitted BSSID, SW need check crespponding dtim count field in multiple bssid index field */
#define BIT_HAS_MYAID4                             ((u32)0x00000001 << 13)          /*!<R 0  Indicate has my unicast packet */
#define BIT_RPT_VALID4                             ((u32)0x00000001 << 12)          /*!<R 0  when set to 1, indicate the tim report is valid */
#define BIT_MASK_PS_AID_4                          ((u32)0x000007FF << 0)          /*!<R/W 0  AID0 for MACID */
#define BIT_PS_AID_4(x)                            ((u32)(((x) & 0x000007FF) << 0))
#define BIT_GET_PS_AID_4(x)                        ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_A1_ADDR_MASK
 * @brief A1 Addr Mask Register
 * @{
 *****************************************************************************/
#define BIT_MASK_A1_ADDR_MASK                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0xFFFFFFFF  mask rx A1 addr [39:8] for test */
#define BIT_A1_ADDR_MASK(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_A1_ADDR_MASK(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXPSF_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXGCK_FIFOTHR_EN                       ((u32)0x00000001 << 28)          /*!<R/W 0  1: enable RXGCK fifo threshold 0: disable RXGCK fifo threshold (ignore fifo threshold) Should be 0 if MAC loopback mode */
#define BIT_MASK_RXGCK_VHT_FIFOTHR                 ((u32)0x00000003 << 26)          /*!<R/W 0  VHT rate RXGCK fifo threshold If RXVHT = 1 and RXGCK fifo write pointer >= this threshold, MAC_RX clock will stop gating. 0: 0 1: 1 (minimum RDRDY threshold for avoiding false alarm), can work at all situation. 2: 3 (number of RDRDY before receiving MAC header - 1), may cause error at some situation. 3: 4 (number of RDRDY before receiving MAC header), may cause error at some situation. */
#define BIT_RXGCK_VHT_FIFOTHR(x)                   ((u32)(((x) & 0x00000003) << 26))
#define BIT_GET_RXGCK_VHT_FIFOTHR(x)               ((u32)(((x >> 26) & 0x00000003)))
#define BIT_MASK_RXGCK_HT_FIFOTHR                  ((u32)0x00000003 << 24)          /*!<R/W 0  HT rate RXGCK fifo threshold If RXHT = 1 and RXGCK fifo write pointer >= this threshold, MAC_RX clock will stop gating. 0: 0 1: 1 (minimum RDRDY threshold for avoiding false alarm), can work at all situation. 2: 2 (number of RDRDY before receiving MAC header - 1), may cause error at some situation. 3: 3 (number of RDRDY before receiving MAC header), may cause error at some situation. */
#define BIT_RXGCK_HT_FIFOTHR(x)                    ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_RXGCK_HT_FIFOTHR(x)                ((u32)(((x >> 24) & 0x00000003)))
#define BIT_MASK_RXGCK_OFDM_FIFOTHR                ((u32)0x00000003 << 22)          /*!<R/W 0  OFDM rate RXGCK fifo threshold If OFDM_VBON = 1 and RXGCK fifo write pointer >= this threshold, MAC_RX clock will stop gating. 0: 0 1: 1 (number of RDRDY before receiving MAC header = minimum RDRDY threshold for avoiding false alarm), can work at all situation. 2: 2, may cause error at some situation. 3: 3 (minimum RDRDY number within one packet), may cause error at some situation. */
#define BIT_RXGCK_OFDM_FIFOTHR(x)                  ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_RXGCK_OFDM_FIFOTHR(x)              ((u32)(((x >> 22) & 0x00000003)))
#define BIT_MASK_RXGCK_CCK_FIFOTHR                 ((u32)0x00000003 << 20)          /*!<R/W 0  CCK rate RXGCK fifo threshold If CCKCCA = 1 and RXGCK fifo write pointer >= this threshold, MAC_RX clock will stop gating. 0: 0 1: 1 (minimum RDRDY threshold for avoiding false alarm), can work at all situation. 2: 8, may cause error at some situation. 3: 15 (number of RDRDY before receiving MAC header), may cause error at some situation. */
#define BIT_RXGCK_CCK_FIFOTHR(x)                   ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_RXGCK_CCK_FIFOTHR(x)               ((u32)(((x >> 20) & 0x00000003)))
#define BIT_MASK_RXGCK_ENTRY_DELAY                 ((u32)0x00000007 << 17)          /*!<R/W 0x2  MAC_RX wait some time to gate clock after making the decision (about gating clock). Unit: T (MAC clock cycle) 0: 0 1: 2 2: 4 3: 8 4: 16 5: 32 6: 64 7: 128 */
#define BIT_RXGCK_ENTRY_DELAY(x)                   ((u32)(((x) & 0x00000007) << 17))
#define BIT_GET_RXGCK_ENTRY_DELAY(x)               ((u32)(((x >> 17) & 0x00000007)))
#define BIT_RXGCK_OFDMCCA_EN                       ((u32)0x00000001 << 16)          /*!<R/W 0  This bit is set to 1 to let OFDMCCA be one of the signals that can wakeup MAC from Rx_idle */
#define BIT_MASK_RXPSF_PKTLENTHR                   ((u32)0x00000007 << 13)          /*!<R/W 0  Packet length threshold 3'd0: 16 Bytes 3'd1: 32 Bytes 3'd2: 64 Bytes 3'd3: 128 Bytes 3'd4: 256 Bytes 3'd5: 512 Bytes 3'd6: 1024 Bytes 3'd7: 2048 Bytes Only the packet which packet length is longer than this threshold will trigger an invalid_pkt pulse. */
#define BIT_RXPSF_PKTLENTHR(x)                     ((u32)(((x) & 0x00000007) << 13))
#define BIT_GET_RXPSF_PKTLENTHR(x)                 ((u32)(((x >> 13) & 0x00000007)))
#define BIT_RXPSF_CTRLEN                           ((u32)0x00000001 << 12)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=01. */
#define BIT_RXPSF_VHTCHKEN                         ((u32)0x00000001 << 11)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet rate = VHT */
#define BIT_RXPSF_HTCHKEN                          ((u32)0x00000001 << 10)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet rate = HT */
#define BIT_RXPSF_OFDMCHKEN                        ((u32)0x00000001 << 9)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet rate = OFDM */
#define BIT_RXPSF_CCKCHKEN                         ((u32)0x00000001 << 8)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet rate = CCK */
#define BIT_RXPSF_OFDMRST                          ((u32)0x00000001 << 7)          /*!<R/W 1  This bit is set to 1 to reset MAC Rx before sending OFDM (include OFDM, HT, VHT) packet triggered invalid_pkt pulse (0 for debug) */
#define BIT_RXPSF_CCKRST                           ((u32)0x00000001 << 6)          /*!<R/W 1  This bit is set to 1 to reset MAC Rx before sending CCK packet triggered invalid_pkt pulse(0 for debug) */
#define BIT_RXPSF_MHCHKEN                          ((u32)0x00000001 << 5)          /*!<R/W 0  This bit is set to 1 to enable MAC to inform BB enter power saving mode as rx unmatch my MACID unicast or unmatch my BSSID Broadcast/Mutlicast packets (including aggrigation and non-aggrigation) */
#define BIT_RXPSF_CONT_ERRCHKEN                    ((u32)0x00000001 << 4)          /*!<R/W 0  This bit is set to 1 to enable MAC to inform BB enter power saving mode as continuous rx FCS error packets number in a PPDU is large than the FCS error packet threshold defined by BBPSF_ERRTHR. This function is independent from rate, type and IDs. */
#define BIT_RXPSF_ALL_ERRCHKEN                     ((u32)0x00000001 << 3)          /*!<R/W 0  This bit is set to 1 to enable MAC to inform BB enter power saving mode as rx FCS error packets number in a PPDU is larger than the FCS error packet threshold defined by BBPSF_ERRTHR. This function is independent from rate, type and IDs. */
#define BIT_MASK_RXPSF_ERRTHR                      ((u32)0x00000007 << 0)          /*!<R/W 0  FCS error packet threshold 3'd0: 1 packet 3'd1: 4 packets 3'd2: 8 packets 3'd3: 12 packets 3'd4: 16 packets 3'd5: 20 packets 3'd6: 24 packets 3'd7: 28 packets */
#define BIT_RXPSF_ERRTHR(x)                        ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_RXPSF_ERRTHR(x)                    ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXPSF_TYPE_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXPSF_DATA15EN                         ((u32)0x00000001 << 31)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1111. */
#define BIT_RXPSF_DATA14EN                         ((u32)0x00000001 << 30)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1110. */
#define BIT_RXPSF_DATA13EN                         ((u32)0x00000001 << 29)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1101. */
#define BIT_RXPSF_DATA12EN                         ((u32)0x00000001 << 28)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1100. */
#define BIT_RXPSF_DATA11EN                         ((u32)0x00000001 << 27)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1011. */
#define BIT_RXPSF_DATA10EN                         ((u32)0x00000001 << 26)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1010. */
#define BIT_RXPSF_DATA9EN                          ((u32)0x00000001 << 25)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1001. */
#define BIT_RXPSF_DATA8EN                          ((u32)0x00000001 << 24)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=1000. */
#define BIT_RXPSF_DATA7EN                          ((u32)0x00000001 << 23)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0111. */
#define BIT_RXPSF_DATA6EN                          ((u32)0x00000001 << 22)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0110. */
#define BIT_RXPSF_DATA5EN                          ((u32)0x00000001 << 21)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0101. */
#define BIT_RXPSF_DATA4EN                          ((u32)0x00000001 << 20)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0100. */
#define BIT_RXPSF_DATA3EN                          ((u32)0x00000001 << 19)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0011. */
#define BIT_RXPSF_DATA2EN                          ((u32)0x00000001 << 18)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0010. */
#define BIT_RXPSF_DATA1EN                          ((u32)0x00000001 << 17)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0001. */
#define BIT_RXPSF_DATA0EN                          ((u32)0x00000001 << 16)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=10 and subtype=0000. */
#define BIT_RXPSF_MGT15EN                          ((u32)0x00000001 << 15)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1111. */
#define BIT_RXPSF_MGT14EN                          ((u32)0x00000001 << 14)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1110. */
#define BIT_RXPSF_MGT13EN                          ((u32)0x00000001 << 13)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1101. */
#define BIT_RXPSF_MGT12EN                          ((u32)0x00000001 << 12)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1100. */
#define BIT_RXPSF_MGT11EN                          ((u32)0x00000001 << 11)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1011. */
#define BIT_RXPSF_MGT10EN                          ((u32)0x00000001 << 10)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1010. */
#define BIT_RXPSF_MGT9EN                           ((u32)0x00000001 << 9)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1001. */
#define BIT_RXPSF_MGT8EN                           ((u32)0x00000001 << 8)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=1000. */
#define BIT_RXPSF_MGT7EN                           ((u32)0x00000001 << 7)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0111. */
#define BIT_RXPSF_MGT6EN                           ((u32)0x00000001 << 6)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0110. */
#define BIT_RXPSF_MGT5EN                           ((u32)0x00000001 << 5)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0101. */
#define BIT_RXPSF_MGT4EN                           ((u32)0x00000001 << 4)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0100. */
#define BIT_RXPSF_MGT3EN                           ((u32)0x00000001 << 3)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0011. */
#define BIT_RXPSF_MGT2EN                           ((u32)0x00000001 << 2)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0010. */
#define BIT_RXPSF_MGT1EN                           ((u32)0x00000001 << 1)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0001. */
#define BIT_RXPSF_MGT0EN                           ((u32)0x00000001 << 0)          /*!<R/W 0  This bit is set to 1 to enable MAC to consider trigging invalid_pkt as rx packet Type=00 and subtype=0000. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WAIT_PHYSTS_CTL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BB_RST_RESP                            ((u32)0x00000001 << 7)          /*!<R/W 0  When set to 1,enable bb/rdrdy_err/cca_spoofing to reset mac and will not tx response */
#define BIT_CCA_SPOOFING_EN                        ((u32)0x00000001 << 6)          /*!<R/W 0  When set to 1,enable cca_spoofing reset macrx */
#define BIT_RDRDY_NUM_CHK_EN                       ((u32)0x00000001 << 5)          /*!<R/W 1  When set, enable check rdrdy number is enough or not. If number is not enough will rst macrx */
#define BIT_WAIT_PHYSTS_EN                         ((u32)0x00000001 << 4)          /*!<R/W 0  When set to 1,enable wait physts rpt valid function */
#define BIT_MASK_WAIT_PHYSTS_TIME                  ((u32)0x0000000F << 0)          /*!<R/W 0x4  Wait physts rpt timeout ,unit μs（6μs is the recommeded value） */
#define BIT_WAIT_PHYSTS_TIME(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_WAIT_PHYSTS_TIME(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID2
 * @brief MAC ID2 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID2_V1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  MAC Identification Number 2 Register */
#define BIT_MACID2_V1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID2_V1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID2_H
 * @brief MAC ID2 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID2_H_V1                       ((u32)0x0000FFFF << 0)          /*!<R/W 0  MAC Identification Number 2 Register */
#define BIT_MACID2_H_V1(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MACID2_H_V1(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID2
 * @brief BSSID2 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID2_V1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Basic Station Set Identification 2 */
#define BIT_BSSID2_V1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BSSID2_V1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID2_H
 * @brief BSSID2 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID2_H_V1                       ((u32)0x0000FFFF << 0)          /*!<R/W 0  Basic Station Set Identification 2 */
#define BIT_BSSID2_H_V1(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BSSID2_H_V1(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID3
 * @brief MAC ID3 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID3_V1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  MAC Identification Number 3 Register */
#define BIT_MACID3_V1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID3_V1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID3_H
 * @brief MAC ID3 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID3_H_V1                       ((u32)0x0000FFFF << 0)          /*!<R/W 0  MAC Identification Number 3 Register */
#define BIT_MACID3_H_V1(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MACID3_H_V1(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID3
 * @brief BSSID3 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID3_V1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Basic Station Set Identification 3 */
#define BIT_BSSID3_V1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BSSID3_V1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID3_H
 * @brief BSSID3 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID3_H_V1                       ((u32)0x0000FFFF << 0)          /*!<R/W 0  Basic Station Set Identification 3 */
#define BIT_BSSID3_H_V1(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BSSID3_H_V1(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID4
 * @brief MAC ID4 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID4_V1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  MAC Identification Number 4 Register */
#define BIT_MACID4_V1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID4_V1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID4_H
 * @brief MAC ID4 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID4_H_V1                       ((u32)0x0000FFFF << 0)          /*!<R/W 0  MAC Identification Number 4 Register */
#define BIT_MACID4_H_V1(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MACID4_H_V1(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID4
 * @brief BSSID4 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID4_V1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Basic Station Set Identification 4 */
#define BIT_BSSID4_V1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BSSID4_V1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BSSID4_H
 * @brief BSSID4 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_BSSID4_H_V1                       ((u32)0x0000FFFF << 0)          /*!<R/W 0  Basic Station Set Identification 4 */
#define BIT_BSSID4_H_V1(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BSSID4_H_V1(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_REPORT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_RPT                           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_NOA_RPT(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_NOA_RPT(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_REPORT_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_RPT_1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_NOA_RPT_1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_NOA_RPT_1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_REPORT_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_RPT_2                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_NOA_RPT_2(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_NOA_RPT_2(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NOA_REPORT_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NOA_RPT_3                         ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_NOA_RPT_3(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_NOA_RPT_3(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PWRBIT_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CLI3_PWRBIT_OW_EN                      ((u32)0x00000001 << 7)          /*!<R/W 0  For client3 (port4). If this bit is set to 1, MAC will overwrite power bit of frame control according to CLI3_PWR_ST. (only for data frame) */
#define BIT_CLI3_PWR_ST                            ((u32)0x00000001 << 6)          /*!<R/W 0  For client3 (port4). MAC will overwrite power bit of frame control according to CLI3_PWR_ST.(only for data frame) */
#define BIT_CLI2_PWRBIT_OW_EN                      ((u32)0x00000001 << 5)          /*!<R/W 0  For client2 (port3). If this bit is set to 1, MAC will overwrite power bit of frame control according to CLI2_PWR_ST. (only for data frame) */
#define BIT_CLI2_PWR_ST                            ((u32)0x00000001 << 4)          /*!<R/W 0  For client2 (port3). MAC will overwrite power bit of frame control according to CLI2_PWR_ST.(only for data frame) */
#define BIT_CLI1_PWRBIT_OW_EN                      ((u32)0x00000001 << 3)          /*!<R/W 0  For client1 (port2). If this bit is set to 1, MAC will overwrite power bit of frame control according to CLI1_PWR_ST. (only for data frame) */
#define BIT_CLI1_PWR_ST                            ((u32)0x00000001 << 2)          /*!<R/W 0  For client1 (port2). MAC will overwrite power bit of frame control according to CLI1_PWR_ST.(only for data frame) */
#define BIT_CLI0_PWRBIT_OW_EN                      ((u32)0x00000001 << 1)          /*!<R/W 0  For client0 (port1). If this bit is set to 1, MAC will overwrite power bit of frame control according to CLI0_PWR_ST. (only for data frame) */
#define BIT_CLI0_PWR_ST                            ((u32)0x00000001 << 0)          /*!<R/W 0  For client0 (port1). MAC will overwrite power bit of frame control according to CLI0_PWR_ST.(only for data frame) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_GENERAL_OPTION
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EXT_DBGSEL                             ((u32)0x00000001 << 6)          /*!<R/W 0  enable extend dbg port sel */
#define BIT_TXSERV_FIELD_SEL                       ((u32)0x00000001 << 2)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXAI_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXAI_PRTCT_REL                         ((u32)0x00000001 << 6)          /*!<R 0  If MAC_reset_protect is set to 0 by MAC decision circuit (address/A-MPDU checking), set this bit to 1. Reset by BIT_RXAI_INFO_RST = 1 */
#define BIT_RXAI_PRTCT_VIO                         ((u32)0x00000001 << 5)          /*!<R 0  If MAC gets BB_reset_MAC pulse when MAC_reset_protect = 1, set this bit to 1. Reset by BIT_RXAI_INFO_RST = 1 */
#define BIT_RXAI_PRTCT_SEL                         ((u32)0x00000001 << 2)          /*!<R/W 0  MAC_reset_protect rises at 0: DATA_ON rises 1: PLCP check OK (OFDM/CCK) or receiving 3rd/2nd (VHT/HT) RDRDY */
#define BIT_RXAI_AGG_CHKEN                         ((u32)0x00000001 << 1)          /*!<R/W 0  This bit is only workable when BIT_RXAI_ADDR_CHKEN = 1. 1: if and only if the receiving address matched PPDU is A-MPDU and is not VHT single MPDU, then MAC inform BB not doing reCCA. */
#define BIT_RXAI_ADDR_CHKEN                        ((u32)0x00000001 << 0)          /*!<R/W 0  This bit is set to 1 to enable MAC to inform BB not doing reCCA during receiving the address matched PPDU. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RX_BCN_TM
 * @brief rx beacon timer
 * @{
 *****************************************************************************/
#define BIT_MASK_RX_BCN_TIMER                      ((u32)0x0000FFFF << 0)          /*!<R 0  indicate rx beacon timer from cca up to fcs ok, units us */
#define BIT_RX_BCN_TIMER(x)                        ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_RX_BCN_TIMER(x)                    ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CSI_RRSR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CSI_LDPC_EN                            ((u32)0x00000001 << 29)          /*!<R/W 0  Enable LDPC in Tx CSI Frame */
#define BIT_CSI_STBC_EN                            ((u32)0x00000001 << 28)          /*!<R/W 0  Enable STBC in Tx CSI Frame */
#define BIT_MASK_CSI_RRSC_BITMAP                   ((u32)0x00FFFFFF << 4)          /*!<R/W 0  BitMap to indicate the CSI frame rate in this BSS. Each bit is mapped as the following: Bit[4:11] = {6M, 9M, 12M, 18M, 24M, 36M, 48M, 54M} Bit[12:19] = HT MCS0~MCS7 BIT[27:20] = VHT 1SSMCS0~7 The modulation of response rate must be less than or equal to the rate of the initiating frame. If none of the bits are set, no response is generated by Rtl8712. If both legacy rates and HT rates are defined as Basic Rates, fill in only the legacy portion of the bitmap. If only HT rates are defined as Basic Rates, fill in both the legacy and HT portion of the bitmap. */
#define BIT_CSI_RRSC_BITMAP(x)                     ((u32)(((x) & 0x00FFFFFF) << 4))
#define BIT_GET_CSI_RRSC_BITMAP(x)                 ((u32)(((x >> 4) & 0x00FFFFFF)))
#define BIT_MASK_OFDM_LEN_TH                       ((u32)0x0000000F << 0)          /*!<R/W 0  CSI frame length threshold for using legacy OFDM (unit: 128B). 0 means disabled. If CSI frame length > threshold, search from VHT or HT bitmap If CSI frame length < threshold, search from legacy OFDM bitmap */
#define BIT_OFDM_LEN_TH(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_OFDM_LEN_TH(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MU_BF_OPTION
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WMAC_NOCHK_BFPOLL_BMP                  ((u32)0x00000001 << 31)          /*!<R/W 0  Set this bit to ignore retransmission bitmap of rx beamforming report poll */
#define BIT_MASK_WMAC_CSI_PARA_RDY_DLYCNT          ((u32)0x0000001F << 24)          /*!<R/W 0x1F  1:Cycle number from txctl state machine IDLE(VBON falling) to CSI_standby falling. 2:Half of this value is txctl state machine IDLE(VBON falling) to CSI_parameter_ready rising cycle number. */
#define BIT_WMAC_CSI_PARA_RDY_DLYCNT(x)            ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_WMAC_CSI_PARA_RDY_DLYCNT(x)        ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_ARB_HW_ADAPT_EN                        ((u32)0x00000001 << 23)          /*!<R/W 0  Enable hardware to arbitrate data according to rate offset of user0 and user1 */
#define BIT_ARB_SW_EN                              ((u32)0x00000001 << 22)          /*!<R/W 0  Enable software to set initial state of MU arbiter */
#define BIT_MASK_ARB_SW_STATE                      ((u32)0x0000003F << 16)          /*!<R/W 0x2A  MU arbiter software initial state */
#define BIT_ARB_SW_STATE(x)                        ((u32)(((x) & 0x0000003F) << 16))
#define BIT_GET_ARB_SW_STATE(x)                    ((u32)(((x >> 16) & 0x0000003F)))
#define BIT_MASK_WMAC_PAUSE_BB_CLR_TH              ((u32)0x000000FF << 8)          /*!<R/W 8'h48  MAC_PAUSE_BB High to Low clear threshold of csidma syncff read-write pointer */
#define BIT_WMAC_PAUSE_BB_CLR_TH(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_WMAC_PAUSE_BB_CLR_TH(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_WMAC_RESP_NONSTA1_DIS                  ((u32)0x00000001 << 7)          /*!<R/W 0  Disable response BA when receive non-STA1 user position MU PPDU */
#define BIT_MASK_WMAC_MU_BFEE_PORT_SEL             ((u32)0x00000007 << 1)          /*!<R/W 0  Enable software to set initial state of MU arbiter */
#define BIT_WMAC_MU_BFEE_PORT_SEL(x)               ((u32)(((x) & 0x00000007) << 1))
#define BIT_GET_WMAC_MU_BFEE_PORT_SEL(x)           ((u32)(((x >> 1) & 0x00000007)))
#define BIT_WMAC_MU_BFEE_DIS                       ((u32)0x00000001 << 0)          /*!<R/W 0  If set to 1, HW will be SU only beamformee and not response to NDPA or Beamforming report poll. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_MU_BF_CTL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BFRPT_PARA_USERID_SEL_V1          ((u32)0x00000007 << 29)          /*!<R/W 0  User ID of associated Bfee, MU: 2~7 ,SU:0~1 */
#define BIT_BFRPT_PARA_USERID_SEL_V1(x)            ((u32)(((x) & 0x00000007) << 29))
#define BIT_GET_BFRPT_PARA_USERID_SEL_V1(x)        ((u32)(((x >> 29) & 0x00000007)))
#define BIT_MASK_BFRPT_PARA_V1                     ((u32)0x00001FFF << 16)          /*!<R 0  CSI report parameter of selected user ID BFee {bw[1:0],cb[1:0],ng[1:0],nr[2:0],nc[2:0]} ; */
#define BIT_BFRPT_PARA_V1(x)                       ((u32)(((x) & 0x00001FFF) << 16))
#define BIT_GET_BFRPT_PARA_V1(x)                   ((u32)(((x >> 16) & 0x00001FFF)))
#define BIT_WMAC_INVLD_BFPRT_CHK                   ((u32)0x00000001 << 15)          /*!<R/W 0  Check invalid case of received beamforming report */
#define BIT_WMAC_RETXBFRPTSEQ_UPD                  ((u32)0x00000001 << 14)          /*!<R/W 0  Update sequence number and not set retry bit when retransmit beamforming report. */
#define BIT_MASK_WMAC_MU_BFRPTSEG_SEL              ((u32)0x00000003 << 12)          /*!<R/W 0  Beamforming report segment length select 2'h00 :3895 bytes(also means csi segmentation enable),2'h01:7991bytes, 2'h10:11454bytes */
#define BIT_WMAC_MU_BFRPTSEG_SEL(x)                ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_WMAC_MU_BFRPTSEG_SEL(x)            ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_WMAC_MU_BF_MYAID                  ((u32)0x00000FFF << 0)          /*!<R/W 0  When I'm one of Bfees, my AID12 to identify STA_info in NDPA */
#define BIT_WMAC_MU_BF_MYAID(x)                    ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_WMAC_MU_BF_MYAID(x)                ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_ASSOCIATED_MU_BFMEE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_STATUS_BFEE3                           ((u32)0x00000001 << 26)          /*!<R 0  beamforming report status of beamformee3 0: fail, 1: success */
#define BIT_WMAC_MU_BFEE3_EN                       ((u32)0x00000001 << 25)          /*!<R/W 0  If enabled, NDPA/BF-rpt-poll would be sent to beamformee3, and CSI rpt of beamformee3 can be read from 0x1682[11:0] when 0x1682[14:12] is 3 */
#define BIT_MASK_WMAC_MU_BFEE3_AID                 ((u32)0x000001FF << 16)          /*!<R/W 0  P_AID of the target beamformee3 */
#define BIT_WMAC_MU_BFEE3_AID(x)                   ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_WMAC_MU_BFEE3_AID(x)               ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_STATUS_BFEE2                           ((u32)0x00000001 << 10)          /*!<R 0  beamforming report status of beamformee2 0: fail, 1: success */
#define BIT_WMAC_MU_BFEE2_EN                       ((u32)0x00000001 << 9)          /*!<R/W 0  If enabled, NDPA/BF-rpt-poll would be sent to beamformee2, and CSI rpt of beamformee2 can be read from 0x1682[11:0] when 0x1682[14:12] is 2 */
#define BIT_MASK_WMAC_MU_BFEE2_AID                 ((u32)0x000001FF << 0)          /*!<R/W 0  P_AID of the target beamformee2 */
#define BIT_WMAC_MU_BFEE2_AID(x)                   ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_WMAC_MU_BFEE2_AID(x)               ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_ASSOCIATED_MU_BFMEE4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WMAC_MU_BFEE5_EN                       ((u32)0x00000001 << 25)          /*!<R/W 0  If enabled, NDPA/BF-rpt-poll would be sent to beamformee5, and CSI rpt of beamformee5 can be read from 0x1682[11:0] when 0x1682[14:12] is 5 */
#define BIT_MASK_WMAC_MU_BFEE5_AID                 ((u32)0x000001FF << 16)          /*!<R/W 0  P_AID of the target beamformee5 */
#define BIT_WMAC_MU_BFEE5_AID(x)                   ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_WMAC_MU_BFEE5_AID(x)               ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_WMAC_MU_BFEE4_EN                       ((u32)0x00000001 << 9)          /*!<R/W 0  If enabled, NDPA/BF-rpt-poll would be sent to beamformee4, and CSI rpt of beamformee4 can be read from 0x1682[11:0] when 0x1682[14:12] is 4 */
#define BIT_MASK_WMAC_MU_BFEE4_AID                 ((u32)0x000001FF << 0)          /*!<R/W 0  P_AID of the target beamformee4 */
#define BIT_WMAC_MU_BFEE4_AID(x)                   ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_WMAC_MU_BFEE4_AID(x)               ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_ASSOCIATED_MU_BFMEE6
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WMAC_MU_BFEE7_EN                       ((u32)0x00000001 << 25)          /*!<R/W 0  If enabled, NDPA/BF-rpt-poll would be sent to beamformee7, and CSI rpt of beamformee7 can be read from 0x1682[11:0] when 0x1682[14:12] is 7 */
#define BIT_MASK_WMAC_MU_BFEE7_AID                 ((u32)0x000001FF << 16)          /*!<R/W 0  P_AID of the target beamformee7 */
#define BIT_WMAC_MU_BFEE7_AID(x)                   ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_WMAC_MU_BFEE7_AID(x)               ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_WMAC_MU_BFEE6_EN                       ((u32)0x00000001 << 9)          /*!<R/W 0  If enabled, NDPA/BF-rpt-poll would be sent to beamformee6, and CSI rpt of beamformee6 can be read from 0x1682[11:0] when 0x1682[14:12] is 6 */
#define BIT_MASK_WMAC_MU_BFEE6_AID                 ((u32)0x000001FF << 0)          /*!<R/W 0  P_AID of the target beamformee6 */
#define BIT_WMAC_MU_BFEE6_AID(x)                   ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_WMAC_MU_BFEE6_AID(x)               ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_BB_STOP_RX_COUNTER
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RST_ALL_COUNTER                        ((u32)0x00000001 << 31)          /*!<W1C 0  Write one to reset all counters to zero. */
#define BIT_MASK_ABORT_RX_VBON_COUNTER             ((u32)0x000000FF << 16)          /*!<R 0  Increase when mac_abort_rx_vbon trigger. 0x120[29] / 0x124[29] : BB_STOP_RX mask/interrupt 0x0B8[0] / 0x0BC[0] : BB_STOP_RX mask/interrupt Interrupts will be trigger when mac detect any counter of 0x1690 increase. */
#define BIT_ABORT_RX_VBON_COUNTER(x)               ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_ABORT_RX_VBON_COUNTER(x)           ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_ABORT_RX_RDRDY_COUNTER            ((u32)0x000000FF << 8)          /*!<R 0  Increase when mac_abort_rx_rdrdy trigger 0x120[29] / 0x124[29] : BB_STOP_RX mask/interrupt 0x0B8[0] / 0x0BC[0] : BB_STOP_RX mask/interrupt Interrupts will be trigger when mac detect any counter of 0x1690 increase. */
#define BIT_ABORT_RX_RDRDY_COUNTER(x)              ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_ABORT_RX_RDRDY_COUNTER(x)          ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_VBON_EARLY_FALLING_COUNTER        ((u32)0x000000FF << 0)          /*!<R 0  Increase when VBON fall & no rx plcp error & rx_parser is waiting for rdrdy which is expected but not received . 0x120[29] / 0x124[29] : BB_STOP_RX mask/interrupt 0x0B8[0] / 0x0BC[0] : BB_STOP_RX mask/interrupt Interrupts will be trigger when mac detect any counter of 0x1690 increase. */
#define BIT_VBON_EARLY_FALLING_COUNTER(x)          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_VBON_EARLY_FALLING_COUNTER(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_PLCP_MONITOR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_WMAC_PLCP_TRX_SEL                      ((u32)0x00000001 << 31)          /*!<R/W 0  Selection of Tx/Rx plcp header 0:Rx 1:Tx */
#define BIT_MASK_WMAC_PLCP_RDSIG_SEL               ((u32)0x00000007 << 28)          /*!<R/W 0  Selection of plcp header signal field 0:LSIG/CCK PLCP 1:HTSIG1/VHTSIGA1/HESIGA1 2:HTSIG2/VHTSIGA2/HESIGA2 3:VHTSIGB/HESIGB, low 4bit is dummy 4: HE USER FIELD 5: SERVICE */
#define BIT_WMAC_PLCP_RDSIG_SEL(x)                 ((u32)(((x) & 0x00000007) << 28))
#define BIT_GET_WMAC_PLCP_RDSIG_SEL(x)             ((u32)(((x >> 28) & 0x00000007)))
#define BIT_MASK_WMAC_RATE_IDX                     ((u32)0x0000000F << 24)          /*!<R 0  PPDU type 0:Long PLCP, 1:Short PLCP, 2: Legacy, 3: HT_MF, 4:HT_GF, 5: VHT-SU, 6: VHT-MU, 7: HE-SU, 8: HE-ER_SU, 9: HE_MU, 10: HE_TB, 11~15:RSVD U(4,0) */
#define BIT_WMAC_RATE_IDX(x)                       ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_WMAC_RATE_IDX(x)                   ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_WMAC_PLCP_RDSIG                   ((u32)0x00FFFFFF << 0)          /*!<R 0  Selected signal field to read out */
#define BIT_WMAC_PLCP_RDSIG(x)                     ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_WMAC_PLCP_RDSIG(x)                 ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WMAC_CSIDMA_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_CSI_SEG_SIZE                      ((u32)0x00000FFF << 16)          /*!<R/W 0xF16  CSI data segmentation size (bytes) = Max. MPDU length - mac hdr (24) - act type (1) - category (1) – mimo ctrl (3) – FCS (4)（default:3862byte） */
#define BIT_CSI_SEG_SIZE(x)                        ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_CSI_SEG_SIZE(x)                    ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_CSI_START_PAGE                    ((u32)0x00000FFF << 0)          /*!<R/W 0xf8  The starting page number of csi buffer in TXPKTBUF */
#define BIT_CSI_START_PAGE(x)                      ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_CSI_START_PAGE(x)                  ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_0
 * @brief TA0 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA0_V1                            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Transmitter address for port 0 */
#define BIT_TA0_V1(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TA0_V1(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_0_H
 * @brief TA0 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA0_H_V1                          ((u32)0x0000FFFF << 0)          /*!<R/W 0  Transmitter address for port 0 */
#define BIT_TA0_H_V1(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TA0_H_V1(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_1
 * @brief TA1 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA1_V1                            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Transmitter address for port 1 */
#define BIT_TA1_V1(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TA1_V1(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_1_H
 * @brief TA1 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA1_H_V1                          ((u32)0x0000FFFF << 0)          /*!<R/W 0  Transmitter address for port 1 */
#define BIT_TA1_H_V1(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TA1_H_V1(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_2
 * @brief TA2 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA2_V1                            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Transmitter address for port 2 */
#define BIT_TA2_V1(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TA2_V1(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_2_H
 * @brief TA2 Register)
 * @{
 *****************************************************************************/
#define BIT_MASK_TA2_H_V1                          ((u32)0x0000FFFF << 0)          /*!<R/W 0  Transmitter address for port 2 */
#define BIT_TA2_H_V1(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TA2_H_V1(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_3
 * @brief TA3 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA3_V1                            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Transmitter address for port 3 */
#define BIT_TA3_V1(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TA3_V1(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_3_H
 * @brief TA3 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA3_H_V1                          ((u32)0x0000FFFF << 0)          /*!<R/W 0  Transmitter address for port 3 */
#define BIT_TA3_H_V1(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TA3_H_V1(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_4
 * @brief TA4 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA4_V1                            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Transmitter address for port 4 */
#define BIT_TA4_V1(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TA4_V1(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRANSMIT_ADDRSS_4_H
 * @brief TA4 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TA4_H_V1                          ((u32)0x0000FFFF << 0)          /*!<R/W 0  Transmitter address for port 4 */
#define BIT_TA4_H_V1(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TA4_H_V1(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAN_FUN
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_ATTRI_ID1                  ((u32)0x000000FF << 24)          /*!<R/W 0  the NAN attribute whose ID is r_WMAC_ATTRI_ID1 will be written into NAN beacon report */
#define BIT_R_WMAC_ATTRI_ID1(x)                    ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_WMAC_ATTRI_ID1(x)                ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_WMAC_ATTRI_ID0                  ((u32)0x000000FF << 16)          /*!<R/W 0  the NAN attribute whose ID is r_WMAC_ATTRI_ID0 will be written into NAN beacon report */
#define BIT_R_WMAC_ATTRI_ID0(x)                    ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_WMAC_ATTRI_ID0(x)                ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_WMAC_ATTRI_IDI_VLD              ((u32)0x0000001F << 8)          /*!<R/W 0  valid flag bitmap of r_WMAC_ATTRI_IDi */
#define BIT_R_WMAC_ATTRI_IDI_VLD(x)                ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_R_WMAC_ATTRI_IDI_VLD(x)            ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_R_TXRPT_NAN_ENTRY_NUM             ((u32)0x00000007 << 4)          /*!<R/W 0  number of NAN beacon report entry in txrptbuffer */
#define BIT_R_TXRPT_NAN_ENTRY_NUM(x)               ((u32)(((x) & 0x00000007) << 4))
#define BIT_GET_R_TXRPT_NAN_ENTRY_NUM(x)           ((u32)(((x >> 4) & 0x00000007)))
#define BIT_MASK_R_NAN_ENTRY_SIZE                  ((u32)0x00000007 << 1)          /*!<R/W 0x2  size of one NAN beacon report entry, unit is 8byte */
#define BIT_R_NAN_ENTRY_SIZE(x)                    ((u32)(((x) & 0x00000007) << 1))
#define BIT_GET_R_NAN_ENTRY_SIZE(x)                ((u32)(((x >> 1) & 0x00000007)))
#define BIT_R_WMAC_RXNANPSR_EN                     ((u32)0x00000001 << 0)          /*!<R/W 0  parse enable of rx NAN sync beacon */
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAN_CTL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXRPT_NAN_ENTRY_FLAG            ((u32)0x000000FF << 24)          /*!<R 0  indicate NAN beacon report(i) in txrptbuffer is valid, it is set by HW and clear by FW */
#define BIT_R_TXRPT_NAN_ENTRY_FLAG(x)              ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_TXRPT_NAN_ENTRY_FLAG(x)          ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_WMAC_ATTRI_ID4                  ((u32)0x000000FF << 16)          /*!<R/W 0  the NAN attribute whose ID is r_WMAC_ATTRI_ID4 will be written into NAN beacon report */
#define BIT_R_WMAC_ATTRI_ID4(x)                    ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_WMAC_ATTRI_ID4(x)                ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_WMAC_ATTRI_ID3                  ((u32)0x000000FF << 8)          /*!<R/W 0  the NAN attribute whose ID is r_WMAC_ATTRI_ID3 will be written into NAN beacon report */
#define BIT_R_WMAC_ATTRI_ID3(x)                    ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_WMAC_ATTRI_ID3(x)                ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_WMAC_ATTRI_ID2                  ((u32)0x000000FF << 0)          /*!<R/W 0  the NAN attribute whose ID is r_WMAC_ATTRI_ID2 will be written into NAN beacon report */
#define BIT_R_WMAC_ATTRI_ID2(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_WMAC_ATTRI_ID2(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAN_ADDR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_RX_NAN_ADDR_FILTER_EN                ((u32)0x00000001 << 13)          /*!<R/W 0  enable of rx nan beacon address filter, can not used in D2 testchip */
#define BIT_MASK_R_NAN_START_ADDR_OFFSET           ((u32)0x00001FFF << 0)          /*!<R/W 0x152  Start offset Location of NAN beacon report FIFO. Unit: 8bytes */
#define BIT_R_NAN_START_ADDR_OFFSET(x)             ((u32)(((x) & 0x00001FFF) << 0))
#define BIT_GET_R_NAN_START_ADDR_OFFSET(x)         ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NAN_ADDR_FLITER
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RX_NAN_ADDR_FILTER              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  nan address filter */
#define BIT_R_RX_NAN_ADDR_FILTER(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_RX_NAN_ADDR_FILTER(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_IEPSR0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_WMAC_BCN_MBSSID_MASK                 ((u32)0x00000001 << 31)          /*!<R/W 0  enable tim no hit don't check whether BC/MC packets are buffered at nontransmitted BSSID */
#define BIT_R_WMAC_BCN_CLR_N                       ((u32)0x00000001 << 30)          /*!<R/W 0  enable clear macbssid indicator when no multiple BSSID element is found. */
#define BIT_R_WMAC_BCN_IE1_PSR                     ((u32)0x00000001 << 29)          /*!<R/W 0  enable parser specified ie 1 element */
#define BIT_R_WMAC_BCN_IE0_PSR                     ((u32)0x00000001 << 28)          /*!<R/W 0  enable parser specified ie0 element */
#define BIT_R_WMAC_BCN_MBSSID_PSR                  ((u32)0x00000001 << 27)          /*!<R/W 0  enable parser multiple BSSID element */
#define BIT_R_WMAC_BCN_SW_MBSSID                   ((u32)0x00000001 << 26)          /*!<R/W 0  enable the MaxBSSID indicator value from SW */
#define BIT_MASK_R_WMAC_BCN_MBSSID_N               ((u32)0x000000FF << 18)          /*!<R/W 0  SW write the value of MaxBSSID Indicator field */
#define BIT_R_WMAC_BCN_MBSSID_N(x)                 ((u32)(((x) & 0x000000FF) << 18))
#define BIT_GET_R_WMAC_BCN_MBSSID_N(x)             ((u32)(((x >> 18) & 0x000000FF)))
#define BIT_R_WMAC_BCN_ID1_EXT                     ((u32)0x00000001 << 17)          /*!<R/W 0  indicate current ie is the extend element which need to parser extend id field */
#define BIT_MASK_R_WMAC_BCN_IE_ID1                 ((u32)0x000000FF << 9)          /*!<R/W 0  specified Element ID, set corresponding interrupt when it is parsed */
#define BIT_R_WMAC_BCN_IE_ID1(x)                   ((u32)(((x) & 0x000000FF) << 9))
#define BIT_GET_R_WMAC_BCN_IE_ID1(x)               ((u32)(((x >> 9) & 0x000000FF)))
#define BIT_R_WMAC_BCN_ID0_EXT                     ((u32)0x00000001 << 8)          /*!<R/W 0  indicate current ie is the extend element which need to parser extend id field */
#define BIT_MASK_R_WMAC_BCN_IE_ID0                 ((u32)0x000000FF << 0)          /*!<R/W 0  specified Element ID, set corresponding interrupt when it is parsed */
#define BIT_R_WMAC_BCN_IE_ID0(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_WMAC_BCN_IE_ID0(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_IEPSR1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_WMAC_BCN_MBSSID_OFS             ((u32)0x0000FFFF << 16)          /*!<R 0  the multiple bssid element offset (byte unit) in frame body */
#define BIT_R_WMAC_BCN_MBSSID_OFS(x)               ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_R_WMAC_BCN_MBSSID_OFS(x)           ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_R_WMAC_BCN_N0_CHG                      ((u32)0x00000001 << 15)          /*!<R 0  indicate parsed maxmbssid indicator field value has changed which leads to the change of N0. */
#define BIT_R_WMAC_BCN_MBSSID_VAL                  ((u32)0x00000001 << 14)          /*!<R 0  indicate ofs and n0 change bit is valid. */
#define BIT_R_WMAC_OPS_PSR_EN                      ((u32)0x00000001 << 10)          /*!<R/W 0  enable parser ops frame tim elemnt and ops element */
#define BIT_R_WMAC_TIM_TIMPSR_EN                   ((u32)0x00000001 << 9)          /*!<R/W 0  enable parser tim frame tim element */
#define BIT_R_WMAC_BCN_BMC_MASK                    ((u32)0x00000001 << 8)          /*!<R/W 0  bcn tim hit bc/mc mask */
#define BIT_MASK_R_WMAC_BCN_MBSSID_IDX             ((u32)0x000000FF << 0)          /*!<R/W 0  the multiple bssid index that current sta connect with */
#define BIT_R_WMAC_BCN_MBSSID_IDX(x)               ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_WMAC_BCN_MBSSID_IDX(x)           ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CHINFO_ADDR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CHINFO_TX_ADDR_L                ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  low 32bit tx address for trigger channel info */
#define BIT_R_CHINFO_TX_ADDR_L(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_CHINFO_TX_ADDR_L(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CHINFO_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_CHINFO_FCS                           ((u32)0x00000001 << 28)          /*!<R/W 0  when set to 1, will not check channel info fcs ok flag */
#define BIT_R_CHINFO_WTM_OUT                       ((u32)0x00000001 << 27)          /*!<R/W1C 0  BB does’t send channel info when timeout occur */
#define BIT_R_CHINFO_TIME_OUT                      ((u32)0x00000001 << 26)          /*!<R/W1C 0  BB send channel info length is less than report length */
#define BIT_R_CSI_BUFF_FULL                        ((u32)0x00000001 << 25)          /*!<R/W1C 0  Indicate rxcsibuff full occur */
#define BIT_MASK_R_CHINFO_TIMER                    ((u32)0x00000007 << 22)          /*!<R/W 0x4  Wait timeout value after repuest channe info, unit 4 mac clock cycles */
#define BIT_R_CHINFO_TIMER(x)                      ((u32)(((x) & 0x00000007) << 22))
#define BIT_GET_R_CHINFO_TIMER(x)                  ((u32)(((x >> 22) & 0x00000007)))
#define BIT_MASK_R_CHINFO_MODE                     ((u32)0x00000003 << 17)          /*!<R/W 0  channel info mode setting. 0: enable normal rx trigger; 1: enable ndpa trigger; note: repsonse mode decided by tx descriptor. Valid when r_CHINFO_EN is 1. */
#define BIT_R_CHINFO_MODE(x)                       ((u32)(((x) & 0x00000003) << 17))
#define BIT_GET_R_CHINFO_MODE(x)                   ((u32)(((x >> 17) & 0x00000003)))
#define BIT_R_CHINFO_EN                            ((u32)0x00000001 << 16)          /*!<R/W 0  enable for getting channel info from BB. When set to 0, will not trigger channel info function */
#define BIT_MASK_R_CHINFO_TX_ADDR_H                ((u32)0x0000FFFF << 0)          /*!<R/W 0  high 16bit tx address for trigger channel info */
#define BIT_R_CHINFO_TX_ADDR_H(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_CHINFO_TX_ADDR_H(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXCSIBUF_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CHINFO_REQ_SIZE                 ((u32)0x0000003F << 16)          /*!<R/W 0  The channel info trigger request rxcsibuffer size, unit 32bytes, the actually request size = (r_chinfo_req_size + 1) * 32byte */
#define BIT_R_CHINFO_REQ_SIZE(x)                   ((u32)(((x) & 0x0000003F) << 16))
#define BIT_GET_R_CHINFO_REQ_SIZE(x)               ((u32)(((x >> 16) & 0x0000003F)))
#define BIT_MASK_R_CHINFO_WDFS_TIME                ((u32)0x0000000F << 12)          /*!<R/W 0xA  Wait DFS timeout value, unit 4 mac clock cycles */
#define BIT_R_CHINFO_WDFS_TIME(x)                  ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_CHINFO_WDFS_TIME(x)              ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_CSIBUF_THRESHOD                 ((u32)0x0000000F << 8)          /*!<R/W 0x4  rxcsibuffer full threshold, size must greater than or equal to csi descriptors */
#define BIT_R_CSIBUF_THRESHOD(x)                   ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_CSIBUF_THRESHOD(x)               ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_CSIBUF_ENDADDR                  ((u32)0x000000FF << 0)          /*!<R/W 0xBF  End address of rxcsibuffer, unit 8bytes. Rxcsibuffer is 1536 bytes */
#define BIT_R_CSIBUF_ENDADDR(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_CSIBUF_ENDADDR(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_ZB_COEX_TB
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BT_ZB_COEX_TABLE                ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  bt and zigbee coexist table */
#define BIT_R_BT_ZB_COEX_TABLE(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_BT_ZB_COEX_TABLE(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CHINFO_TRIG_TYPE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CTL_TRG_15EN                           ((u32)0x00000001 << 31)          /*!<R/W 0  When set, control frames with type=01 and subtype=1111 will trigger channel info */
#define BIT_CTL_TRG_14EN                           ((u32)0x00000001 << 30)          /*!<R/W 0  When set, control frames with type=01 and subtype=1110 will trigger channel info */
#define BIT_CTL_TRG_13EN                           ((u32)0x00000001 << 29)          /*!<R/W 0  When set, control frames with type=01 and subtype=1101 will trigger channel info */
#define BIT_CTL_TRG_12EN                           ((u32)0x00000001 << 28)          /*!<R/W 0  When set, control frames with type=01 and subtype=1100 will trigger channel info */
#define BIT_CTL_TRG_11EN                           ((u32)0x00000001 << 27)          /*!<R/W 0  When set, control frames with type=01 and subtype=1011 will trigger channel info */
#define BIT_CTL_TRG_10EN                           ((u32)0x00000001 << 26)          /*!<R/W 0  When set, control frames with type=01 and subtype=1010 will trigger channel info */
#define BIT_CTL_TRG_9EN                            ((u32)0x00000001 << 25)          /*!<R/W 0  When set, control frames with type=01 and subtype=1001 will trigger channel info */
#define BIT_CTL_TRG_8EN                            ((u32)0x00000001 << 24)          /*!<R/W 0  When set, control frames with type=01 and subtype=1000 will trigger channel info */
#define BIT_CTL_TRG_7EN                            ((u32)0x00000001 << 23)          /*!<R/W 0  When set, control frames with type=01 and subtype=0111 will trigger channel info */
#define BIT_CTL_TRG_6EN                            ((u32)0x00000001 << 22)          /*!<R/W 0  When set, control frames with type=01 and subtype=0110 will trigger channel info */
#define BIT_CTL_TRG_5EN                            ((u32)0x00000001 << 21)          /*!<R/W 0  When set, control frames with type=01 and subtype=0101 will trigger channel info */
#define BIT_CTL_TRG_4EN                            ((u32)0x00000001 << 20)          /*!<R/W 0  When set, control frames with type=01 and subtype=0100 will trigger channel info */
#define BIT_CTL_TRG_3EN                            ((u32)0x00000001 << 19)          /*!<R/W 0  When set, control frames with type=01 and subtype=0011 will trigger channel info */
#define BIT_CTL_TRG_2EN                            ((u32)0x00000001 << 18)          /*!<R/W 0  When set, control frames with type=01 and subtype=0010 will trigger channel info */
#define BIT_CTL_TRG_1EN                            ((u32)0x00000001 << 17)          /*!<R/W 0  When set, control frames with type=01 and subtype=0001 will trigger channel info */
#define BIT_CTL_TRG_0EN                            ((u32)0x00000001 << 16)          /*!<R/W 0  When set, control frames with type=01 and subtype=0000 will trigger channel info */
#define BIT_MGT_TRG_15EN                           ((u32)0x00000001 << 15)          /*!<R/W 0  When set, management frames with type=00 and subtype=1111 will trigger channel info */
#define BIT_MGT_TRG_14EN                           ((u32)0x00000001 << 14)          /*!<R/W 0  When set, management frames with type=00 and subtype=1110 will trigger channel info */
#define BIT_MGT_TRG_13EN                           ((u32)0x00000001 << 13)          /*!<R/W 0  When set, management frames with type=00 and subtype=1101 will trigger channel info */
#define BIT_MGT_TRG_12EN                           ((u32)0x00000001 << 12)          /*!<R/W 0  When set, management frames with type=00 and subtype=1100 will trigger channel info */
#define BIT_MGT_TRG_11EN                           ((u32)0x00000001 << 11)          /*!<R/W 0  When set, management frames with type=00 and subtype=1011 will trigger channel info */
#define BIT_MGT_TRG_10EN                           ((u32)0x00000001 << 10)          /*!<R/W 0  When set, management frames with type=00 and subtype=1010 will trigger channel info */
#define BIT_MGT_TRG_9EN                            ((u32)0x00000001 << 9)          /*!<R/W 0  When set, management frames with type=00 and subtype=1001 will trigger channel info */
#define BIT_MGT_TRG_8EN                            ((u32)0x00000001 << 8)          /*!<R/W 0  When set, management frames with type=00 and subtype=1000 will trigger channel info */
#define BIT_MGT_TRG_7EN                            ((u32)0x00000001 << 7)          /*!<R/W 0  When set, management frames with type=00 and subtype=0111 will trigger channel info */
#define BIT_MGT_TRG_6EN                            ((u32)0x00000001 << 6)          /*!<R/W 0  When set, management frames with type=00 and subtype=0110 will trigger channel info */
#define BIT_MGT_TRG_5EN                            ((u32)0x00000001 << 5)          /*!<R/W 0  When set, management frames with type=00 and subtype=0101 will trigger channel info */
#define BIT_MGT_TRG_4EN                            ((u32)0x00000001 << 4)          /*!<R/W 0  When set, management frames with type=00 and subtype=0100 will trigger channel info */
#define BIT_MGT_TRG_3EN                            ((u32)0x00000001 << 3)          /*!<R/W 0  When set, management frames with type=00 and subtype=0011 will trigger channel info */
#define BIT_MGT_TRG_2EN                            ((u32)0x00000001 << 2)          /*!<R/W 0  When set, management frames with type=00 and subtype=0010 will trigger channel info */
#define BIT_MGT_TRG_1EN                            ((u32)0x00000001 << 1)          /*!<R/W 0  When set, management frames with type=00 and subtype=0001 will trigger channel info */
#define BIT_MGT_TRG_0EN                            ((u32)0x00000001 << 0)          /*!<R/W 0  When set, management frames with type=00 and subtype=0000 will trigger channel info */
/** @} */

/**************************************************************************//**
 * @defgroup REG_CHINFO_TRIG_TYPE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXTRIG_STOP_SEL                 ((u32)0x0000001F << 24)          /*!<R/W 0  select tx trigger stop signal for LA dump */
#define BIT_R_TXTRIG_STOP_SEL(x)                   ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_R_TXTRIG_STOP_SEL(x)               ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_R_RXTRIG_STOP_SEL                 ((u32)0x0000001F << 19)          /*!<R/W 0  select rx trigger stop signal for LA dump */
#define BIT_R_RXTRIG_STOP_SEL(x)                   ((u32)(((x) & 0x0000001F) << 19))
#define BIT_GET_R_RXTRIG_STOP_SEL(x)               ((u32)(((x >> 19) & 0x0000001F)))
#define BIT_R_NEW_RX_RST_EN                        ((u32)0x00000001 << 18)          /*!<R/W 0  when set, enable new rx rst macrx when macrx is active */
#define BIT_MASK_R_RXD_OFFSET_SEL                  ((u32)0x00000003 << 16)          /*!<R/W 0  select 2bytes of rxd to debug port 0: 0~1 bytes; 1: 2~3 bytes; 2: 4~5 bytes; 3: 6~7 bytes; */
#define BIT_R_RXD_OFFSET_SEL(x)                    ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_R_RXD_OFFSET_SEL(x)                ((u32)(((x >> 16) & 0x00000003)))
#define BIT_DATA_TRG_15EN                          ((u32)0x00000001 << 15)          /*!<R/W 0  When set, data frames with type=10 and subtype=1111 will trigger channel info */
#define BIT_DATA_TRG_14EN                          ((u32)0x00000001 << 14)          /*!<R/W 0  When set, data frames with type=10 and subtype=1110 will trigger channel info */
#define BIT_DATA_TRG_13EN                          ((u32)0x00000001 << 13)          /*!<R/W 0  When set, data frames with type=10 and subtype=1101 will trigger channel info */
#define BIT_DATA_TRG_12EN                          ((u32)0x00000001 << 12)          /*!<R/W 0  When set, data frames with type=10 and subtype=1100 will trigger channel info */
#define BIT_DATA_TRG_11EN                          ((u32)0x00000001 << 11)          /*!<R/W 0  When set, data frames with type=10 and subtype=1011 will trigger channel info */
#define BIT_DATA_TRG_10EN                          ((u32)0x00000001 << 10)          /*!<R/W 0  When set, data frames with type=10 and subtype=1010 will trigger channel info */
#define BIT_DATA_TRG_9EN                           ((u32)0x00000001 << 9)          /*!<R/W 0  When set, data frames with type=10 and subtype=1001 will trigger channel info */
#define BIT_DATA_TRG_8EN                           ((u32)0x00000001 << 8)          /*!<R/W 0  When set, data frames with type=10 and subtype=1000 will trigger channel info */
#define BIT_DATA_TRG_7EN                           ((u32)0x00000001 << 7)          /*!<R/W 0  When set, data frames with type=10 and subtype=0111 will trigger channel info */
#define BIT_DATA_TRG_6EN                           ((u32)0x00000001 << 6)          /*!<R/W 0  When set, data frames with type=10 and subtype=0110 will trigger channel info */
#define BIT_DATA_TRG_5EN                           ((u32)0x00000001 << 5)          /*!<R/W 0  When set, data frames with type=10 and subtype=0101 will trigger channel info */
#define BIT_DATA_TRG_4EN                           ((u32)0x00000001 << 4)          /*!<R/W 0  When set, data frames with type=10 and subtype=0100 will trigger channel info */
#define BIT_DATA_TRG_3EN                           ((u32)0x00000001 << 3)          /*!<R/W 0  When set, data frames with type=10 and subtype=0011 will trigger channel info */
#define BIT_DATA_TRG_2EN                           ((u32)0x00000001 << 2)          /*!<R/W 0  When set, data frames with type=10 and subtype=0010 will trigger channel info */
#define BIT_DATA_TRG_1EN                           ((u32)0x00000001 << 1)          /*!<R/W 0  When set, data frames with type=10 and subtype=0001 will trigger channel info */
#define BIT_DATA_TRG_0EN                           ((u32)0x00000001 << 0)          /*!<R/W 0  When set, data frames with type=10 and subtype=0000 will trigger channel info */
/** @} */

/**************************************************************************//**
 * @defgroup REG_DFS_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_DFS_DROP_CNT                    ((u32)0x000001FF << 23)          /*!<R 0  drop dfs packets number when dfsbuffer is full */
#define BIT_R_DFS_DROP_CNT(x)                      ((u32)(((x) & 0x000001FF) << 23))
#define BIT_GET_R_DFS_DROP_CNT(x)                  ((u32)(((x >> 23) & 0x000001FF)))
#define BIT_DFS_BUFF_FULL                          ((u32)0x00000001 << 18)          /*!<R/W1C 0  dfs buffer is full */
#define BIT_DFS_EN_INTRPT                          ((u32)0x00000001 << 17)          /*!<R 0  When valid, enable generate dfs interrupt */
#define BIT_R_DFS_EN_INTRPT                        ((u32)0x00000001 << 16)          /*!<WA0 0  When set to 1, enable generate DFS interrupt, one pulse */
#define BIT_MASK_R_DFS_PKT_LEN                     ((u32)0x000003FF << 6)          /*!<R 0  dfs packet length need Host to read, 4 bytes unit */
#define BIT_R_DFS_PKT_LEN(x)                       ((u32)(((x) & 0x000003FF) << 6))
#define BIT_GET_R_DFS_PKT_LEN(x)                   ((u32)(((x >> 6) & 0x000003FF)))
#define BIT_R_DFS_RST_DROP_CNT                     ((u32)0x00000001 << 5)          /*!<WA0 0  When set to 1, clear r_dfs_drop_cnt, one pulse */
#define BIT_R_DFS_READ_MODE                        ((u32)0x00000001 << 1)          /*!<R/W 0  When set to 1,means host read mode for debug */
#define BIT_R_DFS_STOP                             ((u32)0x00000001 << 0)          /*!<R/W 0  When set to 1, stop to get DFS from BB and clear the DFS buffer */
/** @} */

/**************************************************************************//**
 * @defgroup REG_DFS_AGG_TIME
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXTRIG_TYPE                     ((u32)0x0000003F << 23)          /*!<R/W 0  mapping to frame control type and subtype filed, for LA trigger select */
#define BIT_R_TXTRIG_TYPE(x)                       ((u32)(((x) & 0x0000003F) << 23))
#define BIT_GET_R_TXTRIG_TYPE(x)                   ((u32)(((x >> 23) & 0x0000003F)))
#define BIT_MASK_R_RXTRIG_TYPE                     ((u32)0x0000003F << 17)          /*!<R/W 0  mapping to frame control type and subtype filed, for LA trigger select */
#define BIT_R_RXTRIG_TYPE(x)                       ((u32)(((x) & 0x0000003F) << 17))
#define BIT_GET_R_RXTRIG_TYPE(x)                   ((u32)(((x >> 17) & 0x0000003F)))
#define BIT_MASK_R_DFS_AGG_TIME                    ((u32)0x000000FF << 9)          /*!<R/W 0  DFS packet aggregation time. When reach the target, will inform host by interrupt, unit ms */
#define BIT_R_DFS_AGG_TIME(x)                      ((u32)(((x) & 0x000000FF) << 9))
#define BIT_GET_R_DFS_AGG_TIME(x)                  ((u32)(((x >> 9) & 0x000000FF)))
#define BIT_MASK_R_DFS_AGG_NUM                     ((u32)0x000001FF << 0)          /*!<R/W 0  aggregation DFS packet number selection. When reach the target, will inform host by interrupt, unit 4bytes */
#define BIT_R_DFS_AGG_NUM(x)                       ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_DFS_AGG_NUM(x)                   ((u32)(((x >> 0) & 0x000001FF)))
/** @} */


/*==========PAGE6 Register Address Definition==========*/
#define REG_WMAC_CR                                  0x0600
#define REG_TCR                                      0x0604
#define REG_RCR                                      0x0608
#define REG_RXPKT_CTL                                0x060C
#define REG_MACID                                    0x0610
#define REG_MACID_H                                  0x0614
#define REG_BSSID                                    0x0618
#define REG_BSSID_H                                  0x061C
#define REG_MAR                                      0x0620
#define REG_MAR_H                                    0x0624
#define REG_MBIDCAMCFG_1                             0x0628
#define REG_MBIDCAM_CFG                              0x062C
#define REG_WMAC_TCR_CTL                             0x0630
#define REG_STMP_THSD                                0x0634
#define REG_USTIME_EDCA                              0x0638
#define REG_RESP_SIFS_CCK                            0x063C
#define REG_ACKTO                                    0x0640
#define REG_NAV_THRSHOLD                             0x0650
#define REG_BACAMCMD                                 0x0654
#define REG_BACAM_WD                                 0x0658
#define REG_BACAM_WD_H                               0x065C
#define REG_LBK_DLY                                  0x0660
#define REG_RXERR_RPT                                0x0664
#define REG_WMAC_TRXPTCL_CTL                         0x0668
#define REG_WMAC_TRXPTCL_CTL_H                       0x066C
#define REG_KEYCAMCMD                                0x0670
#define REG_KEYCAM_WD                                0x0674
#define REG_CAMREAD                                  0x0678
#define REG_CAMDBG                                   0x067C
#define REG_SECCFG                                   0x0680
#define REG_RXFILTER_CATEGORY_ACTION                 0x0684
#define REG_RXFLTMAP3                                0x0688
#define REG_RXFLTMAP5                                0x068C
#define REG_WOW_NAN_CTRL                             0x0690
#define REG_LPNAV_CTRL                               0x0694
#define REG_WKFMCAM_CMD                              0x0698
#define REG_WKFMCAM_WD                               0x069C
#define REG_RXFLTMAP0                                0x06A0
#define REG_RXFLTMAP                                 0x06A4
#define REG_BCN_PSR_RPT0                             0x06A8
#define REG_FLC_CTRL                                 0x06AC
#define REG_RXPKTMON_CTRL                            0x06B0
#define REG_STATE_MON                                0x06B4
#define REG_ERROR_EVT_CTL                            0x06B8
#define REG_SEARCH_MACID                             0x06BC
#define REG_BT_COEX_TABLE_V1                         0x06C0
#define REG_BT_COEX_TABLE2_V1                        0x06C4
#define REG_BT_COEX_BREAK_TABLE                      0x06C8
#define REG_BT_COEX_TABLE_H_V1                       0x06CC
#define REG_RXCMD_0                                  0x06D0
#define REG_RXCMD_1                                  0x06D4
#define REG_RESP_TXINFO_CFG                          0x06D8
#define REG_CSI_CTRL                                 0x06DC
#define REG_P2P_RX_BCN_NOA                           0x06E0
#define REG_ASSOCIATED_BFMER0_INFO                   0x06E4
#define REG_SOUNDING_CFG1                            0x06E8
#define REG_SOUNDING_CFG2                            0x06EC
#define REG_SOUNDING_CFG3                            0x06F0
#define REG_SOUNDING_CFG0                            0x06F4
#define REG_ANTCD_INFO                               0x06F8
#define REG_CSI_PTR                                  0x06FC
#define REG_BCN_PSR_RPT2                             0x1600
#define REG_BCN_PSR_RPT3                             0x1604
#define REG_BCN_PSR_RPT4                             0x1608
#define REG_A1_ADDR_MASK                             0x160C
#define REG_RXPSF_CTRL                               0x1610
#define REG_RXPSF_TYPE_CTRL                          0x1614
#define REG_WAIT_PHYSTS_CTL                          0x1618
#define REG_MACID2                                   0x1620
#define REG_MACID2_H                                 0x1624
#define REG_BSSID2                                   0x1628
#define REG_BSSID2_H                                 0x162C
#define REG_MACID3                                   0x1630
#define REG_MACID3_H                                 0x1634
#define REG_BSSID3                                   0x1638
#define REG_BSSID3_H                                 0x163C
#define REG_MACID4                                   0x1640
#define REG_MACID4_H                                 0x1644
#define REG_BSSID4                                   0x1648
#define REG_BSSID4_H                                 0x164C
#define REG_NOA_REPORT                               0x1650
#define REG_NOA_REPORT_1                             0x1654
#define REG_NOA_REPORT_2                             0x1658
#define REG_NOA_REPORT_3                             0x165C
#define REG_PWRBIT_SETTING                           0x1660
#define REG_GENERAL_OPTION                           0x1664
#define REG_RXAI_CTRL                                0x1668
#define REG_RX_BCN_TM                                0x166C
#define REG_CSI_RRSR                                 0x1678
#define REG_MU_BF_OPTION                             0x167C
#define REG_WMAC_MU_BF_CTL                           0x1680
#define REG_WMAC_ASSOCIATED_MU_BFMEE2                0x1684
#define REG_WMAC_ASSOCIATED_MU_BFMEE4                0x1688
#define REG_WMAC_ASSOCIATED_MU_BFMEE6                0x168C
#define REG_WMAC_BB_STOP_RX_COUNTER                  0x1690
#define REG_WMAC_PLCP_MONITOR                        0x1694
#define REG_WMAC_CSIDMA_CFG                          0x169C
#define REG_TRANSMIT_ADDRSS_0                        0x16A0
#define REG_TRANSMIT_ADDRSS_0_H                      0x16A4
#define REG_TRANSMIT_ADDRSS_1                        0x16A8
#define REG_TRANSMIT_ADDRSS_1_H                      0x16AC
#define REG_TRANSMIT_ADDRSS_2                        0x16B0
#define REG_TRANSMIT_ADDRSS_2_H                      0x16B4
#define REG_TRANSMIT_ADDRSS_3                        0x16B8
#define REG_TRANSMIT_ADDRSS_3_H                      0x16BC
#define REG_TRANSMIT_ADDRSS_4                        0x16C0
#define REG_TRANSMIT_ADDRSS_4_H                      0x16C4
#define REG_NAN_FUN                                  0x16C8
#define REG_NAN_CTL                                  0x16CC
#define REG_NAN_ADDR                                 0x16D0
#define REG_NAN_ADDR_FLITER                          0x16D4
#define REG_BCN_IEPSR0                               0x16D8
#define REG_BCN_IEPSR1                               0x16DC
#define REG_CHINFO_ADDR                              0x16E0
#define REG_CHINFO_CFG                               0x16E4
#define REG_RXCSIBUF_CFG                             0x16E8
#define REG_BT_ZB_COEX_TB                            0x16EC
#define REG_CHINFO_TRIG_TYPE0                        0x16F0
#define REG_CHINFO_TRIG_TYPE1                        0x16F4
#define REG_DFS_CFG                                  0x16F8
#define REG_DFS_AGG_TIME                             0x16FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
