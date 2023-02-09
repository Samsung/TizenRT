#ifndef __WIFIFW_REG_PAGE1_H__
#define __WIFIFW_REG_PAGE1_H__

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_CR
 * @brief MAC TOP Function enable
 * @{
 *****************************************************************************/
#define BIT_DMACLBK                                ((u32)0x00000001 << 28)          /*!<R/W 0  [28:24] LBMODE [4]dual-mac lbk, [3] no buffer, 1: no delay, 0: delay; [2] dmalbk, [1] no_txphy, [0] diglbk. 00000: Normal 01011: Single MAC Direct loopback (no HW encryption/decryption) 00011: Single MAC Delay Loopback (with HW encryption/decryption) */
#define BIT_NO_TBUF                                ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_DMALBK                                 ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_NO_TXPHY                               ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_DIGLBK                                 ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_MASK_NETYPE1                           ((u32)0x00000003 << 18)          /*!<R/W 0  Network Type for Port 1 00: No link 01: Link in ad hoc network 10: Link in infrastructure network 11 : AP mode Default: 00b. */
#define BIT_NETYPE1(x)                             ((u32)(((x) & 0x00000003) << 18))
#define BIT_GET_NETYPE1(x)                         ((u32)(((x >> 18) & 0x00000003)))
#define BIT_MASK_NETYPE0                           ((u32)0x00000003 << 16)          /*!<R/W 0  Network Type for port 0 00: No link 01: Link in ad hoc network 10: Link in infrastructure network 11 : AP mode Default: 00b. */
#define BIT_NETYPE0(x)                             ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_NETYPE0(x)                         ((u32)(((x >> 16) & 0x00000003)))
#define BIT_I2C_MAILBOX_EN                         ((u32)0x00000001 << 12)          /*!<R/W 0  Enable I2C mailbox */
#define BIT_MAC_SEC_EN                             ((u32)0x00000001 << 9)          /*!<R/W 0  Enable MAC security engine When this bit is clear, security engine is reset */
#define BIT_ENSWBCN                                ((u32)0x00000001 << 8)          /*!<R/W 0  Enable SW TX beacon 0 : disable, beacon is DMAed from host memory by HW at BCNDMAINT 1 : enable, beacon is polled to DMA to TXBUF. In both case, as long as a beacon frame is set in BCN page, MAC will automatically sent beacon frame automatically. */
#define BIT_MACRXEN                                ((u32)0x00000001 << 7)          /*!<R/W 0  MAC Receiver Enable. When set to 1, and the receive state machine is idle, the receive machine becomes active. This bit will read back as 1 whenever the receive state machine is active. After initial power-up, software must ensure that the receiver has completely reset before setting this bit. 1: Enable 0: Disable Default: 0b. */
#define BIT_MACTXEN                                ((u32)0x00000001 << 6)          /*!<R/W 0  MAC Transmitter Enable. When set to 1, and the transmit state machine is idle, the transmit state machine becomes active. This bit will read back as 1 whenever the transmit state machine is active. After initial power-up, software must ensure that the transmitter has completely reset before setting this bit. 1: Enable 0: Disable Default: 0b. */
#define BIT_SCHEDULE_EN                            ((u32)0x00000001 << 5)          /*!<R/W 0  Schedule Enable. */
#define BIT_PROTOCOL_EN                            ((u32)0x00000001 << 4)          /*!<R/W 0  protocol Block Function Enable.（FW2HW) */
#define BIT_RXDMA_EN                               ((u32)0x00000001 << 3)          /*!<R/W 0  RXDMA Function Enable */
#define BIT_TXDMA_EN                               ((u32)0x00000001 << 2)          /*!<R/W 0  TXDMA Function Enable */
#define BIT_HCI_RXDMA_EN                           ((u32)0x00000001 << 1)          /*!<R/W 0  HCI to RXDMA Interface Enable */
#define BIT_HCI_TXDMA_EN                           ((u32)0x00000001 << 0)          /*!<R/W 0  HCI to TXDMA Interface Enable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TSF_CLK_STATE
 * @brief Beacon time reset value
 * @{
 *****************************************************************************/
#define BIT_CLK_TSF_IS32K                          ((u32)0x00000001 << 15)          /*!<R 0  Which Clock is Used by TSF Clock 1: 32K Clock 0: MAC Clock */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXDMA_PQ_MAP
 * @brief RX DMA Fuction enable
 * @{
 *****************************************************************************/
#define BIT_MASK_TXDMA_CMQ_MAP                     ((u32)0x00000003 << 16)          /*!<R/W 0  00: Extra priority queue 01: Low priority queue 10: Normal priority queue 11: High priority queue */
#define BIT_TXDMA_CMQ_MAP(x)                       ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_TXDMA_CMQ_MAP(x)                   ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_TXDMA_HIQ_MAP                     ((u32)0x00000003 << 14)          /*!<R/W 0  00: Extra priority queue 01: Low priority queue 10: Normal priority queue 11: High priority queue */
#define BIT_TXDMA_HIQ_MAP(x)                       ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_TXDMA_HIQ_MAP(x)                   ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_TXDMA_MGQ_MAP                     ((u32)0x00000003 << 12)          /*!<R/W 0  00: Extra priority queue 01: Low priority queue 10: Normal priority queue 11: High priority queue */
#define BIT_TXDMA_MGQ_MAP(x)                       ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_TXDMA_MGQ_MAP(x)                   ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_TXDMA_BKQ_MAP                     ((u32)0x00000003 << 10)          /*!<R/W 0  00: Extra priority queue 01: Low priority queue 10: Normal priority queue 11: High priority queue */
#define BIT_TXDMA_BKQ_MAP(x)                       ((u32)(((x) & 0x00000003) << 10))
#define BIT_GET_TXDMA_BKQ_MAP(x)                   ((u32)(((x >> 10) & 0x00000003)))
#define BIT_MASK_TXDMA_BEQ_MAP                     ((u32)0x00000003 << 8)          /*!<R/W 0  00: Extra priority queue 01: Low priority queue 10: Normal priority queue 11: High priority queue */
#define BIT_TXDMA_BEQ_MAP(x)                       ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_TXDMA_BEQ_MAP(x)                   ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_TXDMA_VIQ_MAP                     ((u32)0x00000003 << 6)          /*!<R/W 0  00: Extra priority queue 01: Low priority queue 10: Normal priority queue 11: High priority queue */
#define BIT_TXDMA_VIQ_MAP(x)                       ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_TXDMA_VIQ_MAP(x)                   ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_TXDMA_VOQ_MAP                     ((u32)0x00000003 << 4)          /*!<R/W 0  00: Extra priority queue 01: Low priority queue 10: Normal priority queue 11: High priority queue */
#define BIT_TXDMA_VOQ_MAP(x)                       ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_TXDMA_VOQ_MAP(x)                   ((u32)(((x >> 4) & 0x00000003)))
#define BIT_RXDMA_AGG_EN                           ((u32)0x00000001 << 2)          /*!<R/W 0  When this bit is set, then RXDMA will inform host to receive RX packets, when packet page number in RXFF exceeds the threshold set in RXDMA_AGG_PG_TH register. When this bit is set, then page size of receive packet buffer shall be set larger than 64 bytes/page */
#define BIT_ENSHFT                                 ((u32)0x00000001 << 1)          /*!<R/W 0  When this bit is set, RX shift to fit alignment is enabled. */
#define BIT_RXDMA_BW_EN                            ((u32)0x00000001 << 0)          /*!<R/W 0  Enable RXDMA Arbitrator priority for Host interface. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRXFF_BNDY
 * @brief FIFO Boundary control register
 * @{
 *****************************************************************************/
#define BIT_MASK_RXPKT0ENDADDR                     ((u32)0x0000FFFF << 16)          /*!<R/W 0x3FFF  This register sets the upper boundary of RXFF0. The unit is 1 byte. The available memory space in RXPKTBUF for RXFF0 is configurable. The value is set to the physical LX bus address. */
#define BIT_RXPKT0ENDADDR(x)                       ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_RXPKT0ENDADDR(x)                   ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_RXFFOVFL_RSV                      ((u32)0x0000000F << 8)          /*!<R/W 0xF  Use this register to set reserved entries number of rxpktbuffer for overflow judgment. The rxpktbuffer overflow will be pre-asserted if the left entries count meets the value set here. */
#define BIT_RXFFOVFL_RSV(x)                        ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_RXFFOVFL_RSV(x)                    ((u32)(((x >> 8) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PTA_I2C_MBOX
 * @brief Status register
 * @{
 *****************************************************************************/
#define BIT_MASK_I2C_M_STATUS                      ((u32)0x0000000F << 8)          /*!<R 0  Read these can get the I2C_M_STATUS of PTA_I2C's OUTPUT. */
#define BIT_I2C_M_STATUS(x)                        ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_I2C_M_STATUS(x)                    ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_I2C_M_BUS_GNT_FW                  ((u32)0x00000007 << 4)          /*!<R/W 0  Use this bit to control PTA_I2C's BUS_GNT. 100 for gnt_txdata, 010 for gnt_txack, 001 for gnt_rxstatus. */
#define BIT_I2C_M_BUS_GNT_FW(x)                    ((u32)(((x) & 0x00000007) << 4))
#define BIT_GET_I2C_M_BUS_GNT_FW(x)                ((u32)(((x >> 4) & 0x00000007)))
#define BIT_I2C_GNT_FW                             ((u32)0x00000001 << 3)          /*!<R/W 0  Use this bit to control PTA_I2C_GNT_FW input. 1 for FW I2C bus arbiter, 0 for HW as I2C bus arbiter. */
#define BIT_MASK_I2C_M_SPEED                       ((u32)0x00000003 << 1)          /*!<R/W 0  Use these bit to control PTA_I2C I2C interface data rate. SCL = clk / 2^(3 + I2C_DATA_RATE) */
#define BIT_I2C_M_SPEED(x)                         ((u32)(((x) & 0x00000003) << 1))
#define BIT_GET_I2C_M_SPEED(x)                     ((u32)(((x >> 1) & 0x00000003)))
#define BIT_I2C_M_UNLOCK                           ((u32)0x00000001 << 0)          /*!<R/W 0  If this bit is set, PTA_I2C use 118H and 119H to control PTA_I2C */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXFF_PTR
 * @brief RX  FIFO Read/write pointer
 * @{
 *****************************************************************************/
#define BIT_MASK_RXFF0_RDPTR                       ((u32)0x0000FFFF << 16)          /*!<R 0  The read pointer address of RXFF0. The value is set to the physical LX bus address. */
#define BIT_RXFF0_RDPTR(x)                         ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_RXFF0_RDPTR(x)                     ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_RXFF0_WTPTR                       ((u32)0x0000FFFF << 0)          /*!<R 0  The write pointer address of RXFF0. The value is set to the physical LX bus address. */
#define BIT_RXFF0_WTPTR(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_RXFF0_WTPTR(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE1IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_NOT_BEACON_INT_EN                   ((u32)0x00000001 << 31)          /*!<R/W 0  not beacon packet interrupt */
#define BIT_FS_BB_STOP_RX_INT_EN                   ((u32)0x00000001 << 29)          /*!<R/W 0  BB STOP RX interrupt */
#define BIT_FS_TSF_GT_THRES_INT_EN                 ((u32)0x00000001 << 28)          /*!<R/W 0  The interrupt is issued when difference of tsft in beacon frame of port0 is greater than threshold */
#define BIT_FS_RXPLCP_CHKOK_INT_EN                 ((u32)0x00000001 << 27)          /*!<R/W 0  RXPCLP check ok interrupt */
#define BIT_FS_RXBCN_TIMEOUT_CNT_INT_EN            ((u32)0x00000001 << 26)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame timeout occurs specified times in port0.specified by 0x17C[23:20] */
#define BIT_FS_RXBCN_TIMEUP_INT_EN                 ((u32)0x00000001 << 25)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame timeout occurs in port0. */
#define BIT_FS_RX_BCN_ERR_P1_INT_EN                ((u32)0x00000001 << 24)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame with fcs error occurs in port1. */
#define BIT_FS_RX_BCN_ERR_P0_INT_EN                ((u32)0x00000001 << 23)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame with fcs error occurs in port0. */
#define BIT_FS_RX_BCN_P1_INT_EN                    ((u32)0x00000001 << 22)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame occurs in port1. */
#define BIT_FS_RX_BCN_P0_INT_EN                    ((u32)0x00000001 << 21)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame occurs in port0. */
#define BIT_FS_RX_UMD0_INT_EN                      ((u32)0x00000001 << 20)          /*!<R/W 0  The interrupt is issued while receiving unicast data frame with More Data=0 */
#define BIT_FS_RX_UMD1_INT_EN                      ((u32)0x00000001 << 19)          /*!<R/W 0  The interrupt is issued while receiving unicast data frame with More Data=1 */
#define BIT_FS_RX_BMD0_INT_EN                      ((u32)0x00000001 << 18)          /*!<R/W 0  The interrupt is issued while receiving broadcast data frame with More Data=0 */
#define BIT_FS_RX_BMD1_INT_EN                      ((u32)0x00000001 << 17)          /*!<R/W 0  The interrupt is issued while receiving broadcast data frame with More Data=1 */
#define BIT_FS_RXDONE0_INT_EN                      ((u32)0x00000001 << 16)          /*!<R/W 0  Rx Packet 0 done interrupt (for WOL release packet) */
#define BIT_FS_WWLAN_INT_EN                        ((u32)0x00000001 << 15)          /*!<R/W 0  HW detect rxpkt Wol WLAN event interrupt */
#define BIT_FS_SOUND_DONE_INT_EN                   ((u32)0x00000001 << 14)          /*!<R/W 0  Be a beamformer, this interrupt is issued at the time after sounding finish */
#define BIT_FS_LP_STBY_INT_EN                      ((u32)0x00000001 << 13)          /*!<R/W 0  Lower Power Standby Interrupt */
#define BIT_FS_BF1_PRETO_INT_EN                    ((u32)0x00000001 << 11)          /*!<R/W 0  BF1 pretimeout interrupt */
#define BIT_FS_BF0_PRETO_INT_EN                    ((u32)0x00000001 << 10)          /*!<R/W 0  BF0 pretimeout interrupt */
#define BIT_FS_PTCL_RELEASE_MACID_INT_EN           ((u32)0x00000001 << 9)          /*!<R/W 0  PTCL release macid interrupt */
#define BIT_FS_PRETXERR_HANDLE_FSISR_INT_EN        ((u32)0x00000001 << 8)          /*!<R/W 0  PRETXERR_HANDLE FW Flag Interrupt register. */
#define BIT_FS_WLBB_LOW_PRI_INT_EN                 ((u32)0x00000001 << 7)          /*!<R/W 0  ERROR happened in bb,not fatal */
#define BIT_FS_WLBB_HIGH_PRI_INT_EN                ((u32)0x00000001 << 6)          /*!<R/W 0  fatal error happened in bb */
#define BIT_FS_WLACTOFF_INT_EN                     ((u32)0x00000001 << 5)          /*!<R/W 0  WL_ACT_MASK_ON de-assertion for bt coexistence function interrupt */
#define BIT_FS_WLACTON_INT_EN                      ((u32)0x00000001 << 4)          /*!<R/W 0  WL_ACT_MASK_ON assertion for bt coexistence function interrupt */
#define BIT_FS_BTCMD_INT_EN                        ((u32)0x00000001 << 3)          /*!<R/W 0  the interrupt is issued when btcmd occurs. */
#define BIT_FS_REG_MAILBOX_TO_I2C_INT_EN           ((u32)0x00000001 << 2)          /*!<R/W 0  Mailbox_TO_I2C_INT (BT bridge) */
#define BIT_FS_TRPC_TO_INT_EN                      ((u32)0x00000001 << 1)          /*!<R/W 0  TRPC time out Interrupt */
#define BIT_FS_RPC_O_T_INT_EN                      ((u32)0x00000001 << 0)          /*!<R/W 0  RPC over threshold Interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE1ISR
 * @brief Firmware Extend Interrupt Status Register
 * @{
 *****************************************************************************/
#define BIT_NOT_BEACON_INT                         ((u32)0x00000001 << 31)          /*!<RW1C 0  not beacon packet interrupt */
#define BIT_FS_BB_STOP_RX_INT                      ((u32)0x00000001 << 29)          /*!<RW1C 0  BB STOP RX interrupt */
#define BIT_FS_TSF_GT_THRES_INT                    ((u32)0x00000001 << 28)          /*!<RW1C 0  The interrupt is issued when difference of tsft in beacon frame is greater than threshold */
#define BIT_FS_RXPLCP_CHKOK_INT                    ((u32)0x00000001 << 27)          /*!<RW1C 0  RXPCLP check ok interrupt */
#define BIT_FS_RXBCN_TIMEOUT_CNT_INT               ((u32)0x00000001 << 26)          /*!<RW1C 0  When this bit is enable, the interrupt is issued when RX beacon frame timeout occurs specified times in port0.specified by 0x17C[23:20] */
#define BIT_FS_RXBCN_TIMEUP_INT                    ((u32)0x00000001 << 25)          /*!<RW1C 0  When this bit is enable, the interrupt is issued when RX beacon frame timeout occurs in port0. */
#define BIT_FS_RX_BCN_ERR_P1_INT                   ((u32)0x00000001 << 24)          /*!<RW1C 0  When this bit is enable, the interrupt is issued when RX beacon frame with fcs error occurs in port1. */
#define BIT_FS_RX_BCN_ERR_P0_INT                   ((u32)0x00000001 << 23)          /*!<RW1C 0  When this bit is enable, the interrupt is issued when RX beacon frame with fcs error occurs in port0. */
#define BIT_FS_RX_BCN_P1_INT                       ((u32)0x00000001 << 22)          /*!<RW1C 0  When this bit is enable, the interrupt is issued when RX beacon frame occurs in port1. */
#define BIT_FS_RX_BCN_P0_INT                       ((u32)0x00000001 << 21)          /*!<RW1C 0  When this bit is enable, the interrupt is issued when RX beacon frame occurs in port0. */
#define BIT_FS_RX_UMD0_INT                         ((u32)0x00000001 << 20)          /*!<RW1C 0  The interrupt is issued while receiving unicast data frame with More Data=0 */
#define BIT_FS_RX_UMD1_INT                         ((u32)0x00000001 << 19)          /*!<RW1C 0  The interrupt is issued while receiving unicast data frame with More Data=1 */
#define BIT_FS_RX_BMD0_INT                         ((u32)0x00000001 << 18)          /*!<RW1C 0  The interrupt is issued while receiving broadcast data frame with More Data=0 */
#define BIT_FS_RX_BMD1_INT                         ((u32)0x00000001 << 17)          /*!<RW1C 0  The interrupt is issued while receiving broadcast data frame with More Data=1 */
#define BIT_FS_RXDONE0_INT                         ((u32)0x00000001 << 16)          /*!<RW1C 0  Rx Packet 0 done interrupt (for WOL release packet) */
#define BIT_FS_WWLAN_INT                           ((u32)0x00000001 << 15)          /*!<RW1C 0  HW detect rxpkt Wol WLAN event interrupt */
#define BIT_FS_SOUND_DONE_INT                      ((u32)0x00000001 << 14)          /*!<RW1C 0  Be a beamformer, this interrupt is issued at the time after sounding finish */
#define BIT_FS_LP_STBY_INT                         ((u32)0x00000001 << 13)          /*!<RW1C 0  Lower Power Standby Interrupt */
#define BIT_FS_BF1_PRETO_INT                       ((u32)0x00000001 << 11)          /*!<RW1C 0  BF1 pretimeout interrupt */
#define BIT_FS_BF0_PRETO_INT                       ((u32)0x00000001 << 10)          /*!<RW1C 0  BF0 pretimeout interrupt */
#define BIT_FS_PTCL_RELEASE_MACID_INT              ((u32)0x00000001 << 9)          /*!<RW1C 0  PTCL release macid interrupt */
#define BIT_FS_PRETXERR_HANDLE_FSISR_INT           ((u32)0x00000001 << 8)          /*!<RW1C 0  PRETXERR_HANDLE FW Flag Interrupt register. */
#define BIT_FS_WLBB_LOW_PRI_INT                    ((u32)0x00000001 << 7)          /*!<RW1C 0  ERROR happened in bb,not fatal */
#define BIT_FS_WLBB_HIGH_PRI_INT                   ((u32)0x00000001 << 6)          /*!<RW1C 0  fatal error happened in bb */
#define BIT_FS_WLACTOFF_INT                        ((u32)0x00000001 << 5)          /*!<RW1C 0  WL_ACT_MASK_ON de-assertion for bt coexistence function interrupt */
#define BIT_FS_WLACTON_INT                         ((u32)0x00000001 << 4)          /*!<RW1C 0  WL_ACT_MASK_ON assertion for bt coexistence function interrupt */
#define BIT_FS_BTCMD_INT                           ((u32)0x00000001 << 3)          /*!<RW1C 0  the interrupt is issued when btcmd occurs. */
#define BIT_FS_REG_MAILBOX_TO_I2C_INT              ((u32)0x00000001 << 2)          /*!<RW1C 0  Mailbox_TO_I2C_INT (BT bridge) */
#define BIT_FS_TRPC_TO_INT                         ((u32)0x00000001 << 1)          /*!<RW1C 0  tx/rx pkt time out Interrupt */
#define BIT_FS_RPC_O_T_INT                         ((u32)0x00000001 << 0)          /*!<RW1C 0  RXPKT counter over threshold Interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FWIMR
 * @brief Firmware Interrupt Mask Register
 * @{
 *****************************************************************************/
#define BIT_FS_TXBCNOK_MB7_INT_EN                  ((u32)0x00000001 << 31)          /*!<R/W 0  MBSSID7 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB6_INT_EN                  ((u32)0x00000001 << 30)          /*!<R/W 0  MBSSID6 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB5_INT_EN                  ((u32)0x00000001 << 29)          /*!<R/W 0  MBSSID5 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB4_INT_EN                  ((u32)0x00000001 << 28)          /*!<R/W 0  MBSSID4 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB3_INT_EN                  ((u32)0x00000001 << 27)          /*!<R/W 0  MBSSID3 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB2_INT_EN                  ((u32)0x00000001 << 26)          /*!<R/W 0  MBSSID2 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB1_INT_EN                  ((u32)0x00000001 << 25)          /*!<R/W 0  MBSSID1 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB0_INT_EN                  ((u32)0x00000001 << 24)          /*!<R/W 0  MBSSID0 TX beacon OK interrupt */
#define BIT_FS_TXBCNERR_MB7_INT_EN                 ((u32)0x00000001 << 23)          /*!<R/W 0  MBSSID7 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB6_INT_EN                 ((u32)0x00000001 << 22)          /*!<R/W 0  MBSSID6 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB5_INT_EN                 ((u32)0x00000001 << 21)          /*!<R/W 0  MBSSID5 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB4_INT_EN                 ((u32)0x00000001 << 20)          /*!<R/W 0  MBSSID4 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB3_INT_EN                 ((u32)0x00000001 << 19)          /*!<R/W 0  MBSSID3 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB2_INT_EN                 ((u32)0x00000001 << 18)          /*!<R/W 0  MBSSID2 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB1_INT_EN                 ((u32)0x00000001 << 17)          /*!<R/W 0  MBSSID1 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB0_INT_EN                 ((u32)0x00000001 << 16)          /*!<R/W 0  MBSSID0 TX beacon Error interrupt */
#define BIT_FS_CPUMGN_POLLED_PKT_DONE_INT_EN       ((u32)0x00000001 << 15)          /*!<R/W 0  CPU management queue polled packet released done interrupt enable (check cpu management queue current flow) */
#define BIT_FS_MGNTQ_FF_RELEASE_INT_EN             ((u32)0x00000001 << 13)          /*!<R/W 0  MgntQ ReadPtr FIFO release interrupt (for 8822B) */
#define BIT_FS_CPUMGQ_ERR_INT_EN                   ((u32)0x00000001 << 11)          /*!<R/W 0  CPU management queue polled packet not TX OK while new packet comes in, enable to issue interrupt or not */
#define BIT_FS_DDMA0_LP_INT_EN                     ((u32)0x00000001 << 9)          /*!<R/W 0  Low priority ddma0 down interrupt . */
#define BIT_FS_DDMA0_HP_INT_EN                     ((u32)0x00000001 << 8)          /*!<R/W 0  High priority ddma0 down interrupt */
#define BIT_FS_TXRPT_INT_EN                        ((u32)0x00000001 << 7)          /*!<R/W 0  TX report interrupt */
#define BIT_FS_C2H_W_READY_INT_EN                  ((u32)0x00000001 << 6)          /*!<R/W 0  C2H register ready for write interrupt */
#define BIT_FS_HRCV_INT_EN                         ((u32)0x00000001 << 5)          /*!<R/W 0  Host to CPU Message Interrupt, generated on writing to 0x1CF */
#define BIT_FS_H2CCMD_INT_EN                       ((u32)0x00000001 << 4)          /*!<R/W 0  Host To CPU Message Interrupt */
#define BIT_FS_TX_CLOSE_INT_EN                     ((u32)0x00000001 << 3)          /*!<R/W 0  TX DONE INTERRUPT */
#define BIT_FS_ERRORHDL_INT_EN                     ((u32)0x00000001 << 2)          /*!<R/W 0  FWHW/ TXDMA/ RXDMA/ WMAC error status interrupt(0x210[31:0], 0x288[31:0], 0x4E2[15:0], 0x6B8[31:0]). WMAC error state will be recovered by HW, the others won't. */
#define BIT_FS_TXCCX_INT_EN                        ((u32)0x00000001 << 1)          /*!<R/W 0  CCX PKT TX Report Interrupt */
#define BIT_FS_PTCL_HANDLE_OK_INT_EN               ((u32)0x00000001 << 0)          /*!<R/W 0  TX Finish (Ack/BA process Finish) Interrupt. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FWISR
 * @brief Firmware Interrupt Status Register
 * @{
 *****************************************************************************/
#define BIT_FS_TXBCNOK_MB7_INT                     ((u32)0x00000001 << 31)          /*!<RW1C 0  MBSSID7 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB6_INT                     ((u32)0x00000001 << 30)          /*!<RW1C 0  MBSSID6 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB5_INT                     ((u32)0x00000001 << 29)          /*!<RW1C 0  MBSSID5 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB4_INT                     ((u32)0x00000001 << 28)          /*!<RW1C 0  MBSSID4 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB3_INT                     ((u32)0x00000001 << 27)          /*!<RW1C 0  MBSSID3 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB2_INT                     ((u32)0x00000001 << 26)          /*!<RW1C 0  MBSSID2 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB1_INT                     ((u32)0x00000001 << 25)          /*!<RW1C 0  MBSSID1 TX beacon OK interrupt */
#define BIT_FS_TXBCNOK_MB0_INT                     ((u32)0x00000001 << 24)          /*!<RW1C 0  MBSSID0 TX beacon OK interrupt */
#define BIT_FS_TXBCNERR_MB7_INT                    ((u32)0x00000001 << 23)          /*!<RW1C 0  MBSSID7 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB6_INT                    ((u32)0x00000001 << 22)          /*!<RW1C 0  MBSSID6 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB5_INT                    ((u32)0x00000001 << 21)          /*!<RW1C 0  MBSSID5 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB4_INT                    ((u32)0x00000001 << 20)          /*!<RW1C 0  MBSSID4 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB3_INT                    ((u32)0x00000001 << 19)          /*!<RW1C 0  MBSSID3 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB2_INT                    ((u32)0x00000001 << 18)          /*!<RW1C 0  MBSSID2 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB1_INT                    ((u32)0x00000001 << 17)          /*!<RW1C 0  MBSSID1 TX beacon Error interrupt */
#define BIT_FS_TXBCNERR_MB0_INT                    ((u32)0x00000001 << 16)          /*!<RW1C 0  MBSSID0 TX beacon Error interrupt */
#define BIT_FS_CPUMGN_POLLED_PKT_DONE_INT          ((u32)0x00000001 << 15)          /*!<RW1C 0  CPU management queue polled packet released done interrupt enable (check cpu management queue current flow) */
#define BIT_FS_MGNTQ_FF_RELEASE_INT                ((u32)0x00000001 << 13)          /*!<RW1C 0  MgntQ ReadPtr FIFO release interrupt (for 8822B) */
#define BIT_FS_CPUMGQ_ERR_INT                      ((u32)0x00000001 << 11)          /*!<RW1C 0  CPU management queue polled packet not TX OK while new packet comes in, enable to issue interrupt or not */
#define BIT_FS_DDMA0_LP_INT                        ((u32)0x00000001 << 9)          /*!<RW1C 0  Low priority ddma0 down interrupt . */
#define BIT_FS_DDMA0_HP_INT                        ((u32)0x00000001 << 8)          /*!<RW1C 0  High priority ddma0 down interrupt */
#define BIT_FS_TXRPT_INT                           ((u32)0x00000001 << 7)          /*!<RW1C 0  TX report interrupt */
#define BIT_FS_C2H_W_READY_INT                     ((u32)0x00000001 << 6)          /*!<RW1C 0  C2H register ready for write interrupt */
#define BIT_FS_HRCV_INT                            ((u32)0x00000001 << 5)          /*!<RW1C 0  Host to CPU Message Interrupt, generated on writing to 0x1CF */
#define BIT_FS_H2CCMD_INT                          ((u32)0x00000001 << 4)          /*!<RW1C 0  Host To CPU Message Interrupt */
#define BIT_FS_TX_CLOSE_INT                        ((u32)0x00000001 << 3)          /*!<RW1C 0  TX DONE INTERRUPT */
#define BIT_FS_ERRORHDL_INT                        ((u32)0x00000001 << 2)          /*!<RW1C 0  FWHW/ TXDMA/ RXDMA/ WMAC error status interrupt(0x210[31:0], 0x288[31:0], 0x4E2[15:0], 0x6B8[31:0]). WMAC error state will be recovered by HW, the others won't. */
#define BIT_FS_TXCCX_INT                           ((u32)0x00000001 << 1)          /*!<RW1C 0  CCX PKT TX Report Interrupt */
#define BIT_FS_PTCL_HANDLE_OK_INT                  ((u32)0x00000001 << 0)          /*!<RW1C 0  TX Finish (Ack/BA process Finish) Interrupt. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FTIMR
 * @brief Firmware  Timer Interrupt Mask Register
 * @{
 *****************************************************************************/
#define BIT_FTSR1_INT_EN                           ((u32)0x00000001 << 31)          /*!<R/W 0  FTMR1 Indicator (FTMR1 and FTMR1 are true, this bit is set to 1) */
#define BIT_FTSR2_INT_EN                           ((u32)0x00000001 << 30)          /*!<R/W 0  FTMR2 Indicator (FTMR2 and FTMR2 are true, this bit is set to 1) */
#define BIT_FS_RPWM_INT_EN                         ((u32)0x00000001 << 28)          /*!<R/W 0  RPWM interrupt */
#define BIT_FS_PS_TIMER_E_INT_EN                   ((u32)0x00000001 << 27)          /*!<R/W 0  PS_Timer_E interrupt When the TSF(that select by reg 0x1520[13:12]) matches the content of this register, the PS_Timer_E_INT interrupt is issued */
#define BIT_FS_PS_TIMER_D_INT_EN                   ((u32)0x00000001 << 26)          /*!<R/W 0  PS_Timer_D interrupt When the lower 4 bytes of TSF(that select by reg 0x1520[17:16]) matches the content of this register, the PS_Timer_D_INT interrupt is issued */
#define BIT_FS_PS_TIMER_E_EARLY_INT_EN             ((u32)0x00000001 << 25)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_E_Early register before PS_Timer_E time. */
#define BIT_FS_PS_TIMER_D_EARLY_INT_EN             ((u32)0x00000001 << 24)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_D_Early register before PS_Timer_D time. */
#define BIT_FS_PS_TIMER_C_EARLY_INT_EN             ((u32)0x00000001 << 23)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_C_Early register before PS_Timer_C time. */
#define BIT_FS_PS_TIMER_B_EARLY_INT_EN             ((u32)0x00000001 << 22)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_B_Early register before PS_Timer_B time. */
#define BIT_FS_PS_TIMER_A_EARLY_INT_EN             ((u32)0x00000001 << 21)          /*!<R/W 0  This interrupt is issued at the time set by PS_Timer_A_Early register before PS_Timer_A time. */
#define BIT_FS_PS_TIMER_C_INT_EN                   ((u32)0x00000001 << 19)          /*!<R/W 0  PS_Timer_C interrupt When the lower 4 bytes of TSF(that select by reg 0x1510[18:16]) matches the content of this register, the PS_Timer_C_INT interrupt is issued */
#define BIT_FS_PS_TIMER_B_INT_EN                   ((u32)0x00000001 << 18)          /*!<R/W 0  PS_Timer_B interrupt When the lower 4 bytes of TSF(that select by reg 0x1510[10:8]) matches the content of this register, the PS_Timer_B_INT interrupt is issued */
#define BIT_FS_PS_TIMER_A_INT_EN                   ((u32)0x00000001 << 17)          /*!<R/W 0  PS_Timer_A interrupt When the lower 4 bytes of TSF(that select by reg 0x1510[2:0]) matches the content of this register, the PS_Timer_A_INT interrupt is issued */
#define BIT_FS_PS_TIMEOUT2_INT_EN                  ((u32)0x00000001 << 15)          /*!<R/W 0  PS_TimeOut2 interrupt . When the lower 4 bytes of FREERUN CNT matches the content of this register(0x588), the FW PS_TimeOut2 interrupt is issued. */
#define BIT_FS_PS_TIMEOUT1_INT_EN                  ((u32)0x00000001 << 14)          /*!<R/W 0  PS_TimeOut1 interrupt . When the lower 4 bytes of TSFTR(that select by 0x5b1[5:3]) matches the content of this register(0x584), the FW PS_TimeOut1 interrupt is issued. */
#define BIT_FS_PS_TIMEOUT0_INT_EN                  ((u32)0x00000001 << 13)          /*!<R/W 0  PS_TimeOut0 interrupt . When the lower 4 bytes of TSFTR(that select by 0x5b4[2:0]) matches the content of this register(0x580), the FW PS_TimeOut0 interrupt is issued. */
#define BIT_FS_FTM_PTT_INT_EN                      ((u32)0x00000001 << 7)          /*!<R/W 0  Partial tsf timer */
#define BIT_FS_GTINT6_INT_EN                       ((u32)0x00000001 << 6)          /*!<R/W 0  General Timer[6] interrupt */
#define BIT_FS_GTINT5_INT_EN                       ((u32)0x00000001 << 5)          /*!<R/W 0  General Timer[5] interrupt */
#define BIT_FS_GTINT4_INT_EN                       ((u32)0x00000001 << 4)          /*!<R/W 0  General Timer[4] interrupt */
#define BIT_FS_GTINT3_INT_EN                       ((u32)0x00000001 << 3)          /*!<R/W 0  General Timer[3] interrupt */
#define BIT_FS_GTINT2_INT_EN                       ((u32)0x00000001 << 2)          /*!<R/W 0  General Timer[2] interrupt */
#define BIT_FS_GTINT1_INT_EN                       ((u32)0x00000001 << 1)          /*!<R/W 0  General Timer[1] interrupt */
#define BIT_FS_GTINT0_INT_EN                       ((u32)0x00000001 << 0)          /*!<R/W 0  General Timer[0] interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FTISR
 * @brief Firmware Timer Interrupt Status Register
 * @{
 *****************************************************************************/
#define BIT_FTSR1_INT                              ((u32)0x00000001 << 31)          /*!<RW1C 0  FTMR1 Indicator (FTMR1 and FTMR1 are true, this bit is set to 1) */
#define BIT_FTSR2_INT                              ((u32)0x00000001 << 30)          /*!<RW1C 0  FTMR2 Indicator (FTMR2 and FTMR2 are true, this bit is set to 1) */
#define BIT_FS_RPWM_INT                            ((u32)0x00000001 << 28)          /*!<RW1C 0  RPWM interrupt */
#define BIT_FS_PS_TIMER_E_INT                      ((u32)0x00000001 << 27)          /*!<RW1C 0  PS_Timer_E interrupt When the TSF(that select by reg 0x1520[13:12]) matches the content of this register, the PS_Timer_E_INT interrupt is issued */
#define BIT_FS_PS_TIMER_D_INT                      ((u32)0x00000001 << 26)          /*!<RW1C 0  PS_Timer_D interrupt When the lower 4 bytes of TSF(that select by reg 0x1520[17:16]) matches the content of this register, the PS_Timer_D_INT interrupt is issued */
#define BIT_FS_PS_TIMER_E_EARLY_INT                ((u32)0x00000001 << 25)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_E_Early register before PS_Timer_E time. */
#define BIT_FS_PS_TIMER_D_EARLY_INT                ((u32)0x00000001 << 24)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_D_Early register before PS_Timer_D time. */
#define BIT_FS_PS_TIMER_C_EARLY_INT                ((u32)0x00000001 << 23)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_C_Early register before PS_Timer_C time. */
#define BIT_FS_PS_TIMER_B_EARLY_INT                ((u32)0x00000001 << 22)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_B_Early register before PS_Timer_B time. */
#define BIT_FS_PS_TIMER_A_EARLY_INT                ((u32)0x00000001 << 21)          /*!<RW1C 0  This interrupt is issued at the time set by PS_Timer_A_Early register before PS_Timer_A time. */
#define BIT_FS_PS_TIMER_C_INT                      ((u32)0x00000001 << 19)          /*!<RW1C 0  PS_Timer_C interrupt When the lower 4 bytes of TSF(that select by reg 0x1510[18:16]) matches the content of this register, the PS_Timer_C_INT interrupt is issued */
#define BIT_FS_PS_TIMER_B_INT                      ((u32)0x00000001 << 18)          /*!<RW1C 0  PS_Timer_B interrupt When the lower 4 bytes of TSF(that select by reg 0x1510[10:8]) matches the content of this register, the PS_Timer_B_INT interrupt is issued */
#define BIT_FS_PS_TIMER_A_INT                      ((u32)0x00000001 << 17)          /*!<RW1C 0  PS_Timer_A interrupt When the lower 4 bytes of TSF(that select by reg 0x1510[2:0]) matches the content of this register, the PS_Timer_A_INT interrupt is issued */
#define BIT_FS_PS_TIMEOUT2_INT                     ((u32)0x00000001 << 15)          /*!<RW1C 0  PS_TimeOut2 interrupt . When the lower 4 bytes of FREERUN CNT matches the content of this register(0x588), the FW PS_TimeOut2 interrupt is issued. */
#define BIT_FS_PS_TIMEOUT1_INT                     ((u32)0x00000001 << 14)          /*!<RW1C 0  PS_TimeOut1 interrupt . When the lower 4 bytes of TSFTR(that select by 0x5b1[5:3]) matches the content of this register(0x584), the FW PS_TimeOut1 interrupt is issued. */
#define BIT_FS_PS_TIMEOUT0_INT                     ((u32)0x00000001 << 13)          /*!<RW1C 0  PS_TimeOut0 interrupt . When the lower 4 bytes of TSFTR(that select by 0x5b4[2:0]) matches the content of this register(0x580), the FW PS_TimeOut0 interrupt is issued. */
#define BIT_FS_FTM_PTT_INT                         ((u32)0x00000001 << 7)          /*!<RW1C 0  Partial tsf timer */
#define BIT_FS_GTINT6_INT                          ((u32)0x00000001 << 6)          /*!<RW1C 0  General Timer[6] interrupt */
#define BIT_FS_GTINT5_INT                          ((u32)0x00000001 << 5)          /*!<RW1C 0  General Timer[5] interrupt */
#define BIT_FS_GTINT4_INT                          ((u32)0x00000001 << 4)          /*!<RW1C 0  General Timer[4] interrupt */
#define BIT_FS_GTINT3_INT                          ((u32)0x00000001 << 3)          /*!<RW1C 0  General Timer[3] interrupt */
#define BIT_FS_GTINT2_INT                          ((u32)0x00000001 << 2)          /*!<RW1C 0  General Timer[2] interrupt */
#define BIT_FS_GTINT1_INT                          ((u32)0x00000001 << 1)          /*!<RW1C 0  General Timer[1] interrupt */
#define BIT_FS_GTINT0_INT                          ((u32)0x00000001 << 0)          /*!<RW1C 0  General Timer[0] interrupt */
/** @} */

/** @defgroup REG_PHYSTS_REL
 * @brief PHYSTS REALSE
 * @{
 **/
#define BIT_MASK_PHY_STS_STR                       ((u32)0x000000FF << 24)          /*!<R/W 0  physts start read address,8 bytes unit */
#define BIT_PHY_STS_STR(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_PHY_STS_STR(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_PHY_STS_NUM                       ((u32)0x000000FF << 16)          /*!<R/W 0  physts packet number in physts buffer */
#define BIT_PHY_STS_NUM(x)                         ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PHY_STS_NUM(x)                     ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_PHY_SYS_POLL                           ((u32)0x00000001 << 0)          /*!<R/W 0  physts release polling,when polling done,write the bit to 0 */
/** @} */

/** @defgroup REG_CHANNEL_INFO_REL
 * @brief CHANNEL INFO REALSE
 * @{
 **/
#define BIT_MASK_CHINFO_STR                        ((u32)0x000000FF << 24)          /*!<R/W 0  channel info start read address,8 bytes unit */
#define BIT_CHINFO_STR(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_CHINFO_STR(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_CHINFO_NUM                        ((u32)0x000000FF << 16)          /*!<R/W 0  channel info packet number in csi buffer */
#define BIT_CHINFO_NUM(x)                          ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_CHINFO_NUM(x)                      ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_CHINFO_POLL                            ((u32)0x00000001 << 0)          /*!<R/W 0  channel info release polling,when polling done,write the bit to 0 */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TC0_CTRL
 * @brief Counter 0 data
 * @{
 *****************************************************************************/
#define BIT_TC0INT_EN                              ((u32)0x00000001 << 26)          /*!<R/W 0  When write “1” to this bit, Counter 0 starts counting down. This is a one-bit pulse. */
#define BIT_TC0MODE                                ((u32)0x00000001 << 25)          /*!<R/W 0  Timer/Counter 0 mode 0=counter mode 1=timer mode */
#define BIT_TC0EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  Timer/Counter 0 enable */
#define BIT_MASK_TC0DATA                           ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Timer/Counter 0 data register. It specifies the time-out duration. */
#define BIT_TC0DATA(x)                             ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_TC0DATA(x)                         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TC1_CTRL
 * @brief Counter 1 data
 * @{
 *****************************************************************************/
#define BIT_TC1INT_EN                              ((u32)0x00000001 << 26)          /*!<R/W 0  When write “1” to this bit, Counter 1 starts counting down. This is a one-bit pulse. */
#define BIT_TC1MODE                                ((u32)0x00000001 << 25)          /*!<R/W 0  Timer/Counter 1 mode 0=counter mode 1=timer mode */
#define BIT_TC1EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  Timer/Counter 1 enable */
#define BIT_MASK_TC1DATA                           ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Timer/Counter 1 data register. It specifies the time-out duration. */
#define BIT_TC1DATA(x)                             ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_TC1DATA(x)                         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TC2_CTRL
 * @brief Counter 2 data
 * @{
 *****************************************************************************/
#define BIT_TC2INT_EN                              ((u32)0x00000001 << 26)          /*!<R/W 0  When write “1” to this bit, Counter 2 starts counting down. This is a one-bit pulse. */
#define BIT_TC2MODE                                ((u32)0x00000001 << 25)          /*!<R/W 0  Timer/Counter 2 mode 0=counter mode 1=timer mode */
#define BIT_TC2EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  Timer/Counter 2 enable */
#define BIT_MASK_TC2DATA                           ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Timer/Counter 2 data register. It specifies the time-out duration. */
#define BIT_TC2DATA(x)                             ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_TC2DATA(x)                         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TC3_CTRL
 * @brief Counter 3 data
 * @{
 *****************************************************************************/
#define BIT_TC3INT_EN                              ((u32)0x00000001 << 26)          /*!<R/W 0  When write “1” to this bit, Counter 3 starts counting down. This is a one-bit pulse. */
#define BIT_TC3MODE                                ((u32)0x00000001 << 25)          /*!<R/W 0  Timer/Counter 3 mode 0=counter mode 1=timer mode */
#define BIT_TC3EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  Timer/Counter 3 enable */
#define BIT_MASK_TC3DATA                           ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Timer/Counter 3 data register. It specifies the time-out duration. */
#define BIT_TC3DATA(x)                             ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_TC3DATA(x)                         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TC4_CTRL
 * @brief Counter 4 data
 * @{
 *****************************************************************************/
#define BIT_TC4INT_EN                              ((u32)0x00000001 << 26)          /*!<R/W 0  When write “1” to this bit, Counter 4 starts counting down. This is a one-bit pulse. */
#define BIT_TC4MODE                                ((u32)0x00000001 << 25)          /*!<R/W 0  Timer/Counter 4mode 0=counter mode 1=timer mode */
#define BIT_TC4EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  Timer/Counter 4 enable */
#define BIT_MASK_TC4DATA                           ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Timer/Counter 4 data register. It specifies the time-out duration. */
#define BIT_TC4DATA(x)                             ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_TC4DATA(x)                         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TCUNIT_BASE
 * @brief Control Register
 * @{
 *****************************************************************************/
#define BIT_MASK_TC_UNIT_BASE                      ((u32)0x00003FFF << 0)          /*!<R/W 0  The register set the number of clocks that equals to the base time unit of timer/counter. The clock domain is 32k clock domain. */
#define BIT_TC_UNIT_BASE(x)                        ((u32)(((x) & 0x00003FFF) << 0))
#define BIT_GET_TC_UNIT_BASE(x)                    ((u32)(((x >> 0) & 0x00003FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TC5_CTRL
 * @brief Counter 5 data
 * @{
 *****************************************************************************/
#define BIT_TC50INT_EN                             ((u32)0x00000001 << 26)          /*!<R/W 0  When write “1” to this bit, Counter 5 starts counting down. This is a one-bit pulse. */
#define BIT_TC5MODE                                ((u32)0x00000001 << 25)          /*!<R/W 0  Timer/Counter 5 mode 0=counter mode 1=timer mode */
#define BIT_TC5EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  Timer/Counter5 enable */
#define BIT_MASK_TC5DATA                           ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Timer/Counter 5 data register. It specifies the time-out duration. */
#define BIT_TC5DATA(x)                             ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_TC5DATA(x)                         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TC6_CTRL
 * @brief Counter 6 data
 * @{
 *****************************************************************************/
#define BIT_TC60INT_EN                             ((u32)0x00000001 << 26)          /*!<R/W 0  When write “1” to this bit, Counter 6 starts counting down. This is a one-bit pulse. */
#define BIT_TC6MODE                                ((u32)0x00000001 << 25)          /*!<R/W 0  Timer/Counter 6 mode 0=counter mode 1=timer mode */
#define BIT_TC6EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  Timer/Counter6 enable */
#define BIT_MASK_TC6DATA                           ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Timer/Counter 6 data register. It specifies the time-out duration. */
#define BIT_TC6DATA(x)                             ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_TC6DATA(x)                         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HW_IOE_CR_CHN1
 * @brief HW IO Engine Configure Register_CHN1
 * @{
 *****************************************************************************/
#define BIT_HIOE_ACTIVE_CHN1                       ((u32)0x00000001 << 31)          /*!<R 0  indicate HIOE engine is active */
#define BIT_HIOE_RESTORE_REQ_CHN1                  ((u32)0x00000001 << 25)          /*!<R/W 0  WLOFF REG restore request, clear by hw */
#define BIT_HIOE_BACKUP_REQ_CHN1                   ((u32)0x00000001 << 24)          /*!<R/W 0  WLOFF REG backup request, clear by hw */
#define BIT_MASK_HIOE_END_ADDR_CHN1                ((u32)0x00000FFF << 12)          /*!<R/W 0  HIOE command end address in txpktbuffer, unit:8byte */
#define BIT_HIOE_END_ADDR_CHN1(x)                  ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_HIOE_END_ADDR_CHN1(x)              ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_HIOE_STR_ADDR_CHN1                ((u32)0x00000FFF << 0)          /*!<R/W 0  HIOE command start address in txpktbuffer, unit:8byte */
#define BIT_HIOE_STR_ADDR_CHN1(x)                  ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_HIOE_STR_ADDR_CHN1(x)              ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HW_IOE_CR_CHN_ON
 * @brief HW IO Engine Configure Register_CHN_ON
 * @{
 *****************************************************************************/
#define BIT_HIOE_ACTIVE_CHN_ON                     ((u32)0x00000001 << 31)          /*!<R 0  indicate HIOE engine is active, used for wlan power on flow */
#define BIT_HIOE_BYPASS_CHN_ON                     ((u32)0x00000001 << 26)          /*!<R/W 1  BYPASS HW trig hioe to do rf on flow */
#define BIT_HIOE_RESTORE_REQ_CHN_ON                ((u32)0x00000001 << 25)          /*!<R 0  WLOFF REG restore request, clear by hw */
#define BIT_MASK_HIOE_END_ADDR_CHN_ON              ((u32)0x00000FFF << 12)          /*!<R/W 0  HIOE command end address in txpktbuffer, unit:8byte */
#define BIT_HIOE_END_ADDR_CHN_ON(x)                ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_HIOE_END_ADDR_CHN_ON(x)            ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_HIOE_STR_ADDR_CHN_ON              ((u32)0x00000FFF << 0)          /*!<R/W 0  HIOE command start address in txpktbuffer, unit:8byte */
#define BIT_HIOE_STR_ADDR_CHN_ON(x)                ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_HIOE_STR_ADDR_CHN_ON(x)            ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HW_IOE_CR_CHN_OFF
 * @brief HW IO Engine Configure Register_CHN_OFF
 * @{
 *****************************************************************************/
#define BIT_HIOE_ACTIVE_CHN_OFF                    ((u32)0x00000001 << 31)          /*!<R 0  indicate HIOE engine is active, used for wlan power off flow */
#define BIT_HIOE_BYPASS_CHN_OFF                    ((u32)0x00000001 << 26)          /*!<R/W 1  BYPASS HW trig hioe to do rf off flow */
#define BIT_HIOE_RESTORE_REQ_CHN_OFF               ((u32)0x00000001 << 25)          /*!<R 0  WLOFF REG restore request, clear by hw */
#define BIT_MASK_HIOE_END_ADDR_CHN_OFF             ((u32)0x00000FFF << 12)          /*!<R/W 0  HIOE command end address in txpktbuffer, unit:8byte */
#define BIT_HIOE_END_ADDR_CHN_OFF(x)               ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_HIOE_END_ADDR_CHN_OFF(x)           ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_HIOE_STR_ADDR_CHN_OFF             ((u32)0x00000FFF << 0)          /*!<R/W 0  HIOE command start address in txpktbuffer, unit:8byte */
#define BIT_HIOE_STR_ADDR_CHN_OFF(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_HIOE_STR_ADDR_CHN_OFF(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXBCN_TIMER
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_HW_LPS_EN                           ((u32)0x00000001 << 31)          /*!<R/W 0  enable HW enter lps after hioe_off_int rise when BIT_FS_RXBCN_LPS_EN=1 */
#define BIT_MASK_RXBCN_TIMEOUT_COUNT               ((u32)0x0000000F << 27)          /*!<R 0  rxbcn timeout counter */
#define BIT_RXBCN_TIMEOUT_COUNT(x)                 ((u32)(((x) & 0x0000000F) << 27))
#define BIT_GET_RXBCN_TIMEOUT_COUNT(x)             ((u32)(((x >> 27) & 0x0000000F)))
#define BIT_FS_RXBCN_LPS_EN                        ((u32)0x00000001 << 26)          /*!<R/W 0  enable lps offload flow when rxbcn frame in port0 acting as a non-AP STA. */
#define BIT_FS_WAIT_RXBCN_FINISH                   ((u32)0x00000001 << 25)          /*!<R/W 0  when rxbcn frame, turn off rf or generate interrupt ablout rxbcn after beacon fcs is parsered */
#define BIT_RXBCN_TIMEOUT_COUNT_CLR                ((u32)0x00000001 << 24)          /*!<R/W 0  clear rxbcn timeout counter */
#define BIT_MASK_RXBCN_TIMEOUT_COUNT_THRES         ((u32)0x0000000F << 20)          /*!<R/W 0x3  threshold for rxbcn timeout counter,when 0x17C[30:27] equals to this value,generate interrupt */
#define BIT_RXBCN_TIMEOUT_COUNT_THRES(x)           ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_RXBCN_TIMEOUT_COUNT_THRES(x)       ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_RXBCN_TIMEOUT_THRES               ((u32)0x000FFFFF << 0)          /*!<R/W 0x1F40  threshold for rxbcn timeout,UNIT:us default：8ms */
#define BIT_RXBCN_TIMEOUT_THRES(x)                 ((u32)(((x) & 0x000FFFFF) << 0))
#define BIT_GET_RXBCN_TIMEOUT_THRES(x)             ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HW_IOE_CR_CH0
 * @brief HW IO Engine Configure Register_CH0
 * @{
 *****************************************************************************/
#define BIT_HIOE_ACTIVE_CH0                        ((u32)0x00000001 << 31)          /*!<R 0  indicate HIOE engine is active */
#define BIT_HIOE_RESTORE_REQ_CH0                   ((u32)0x00000001 << 25)          /*!<R/W 0  WLOFF REG restore request, clear by hw */
#define BIT_HIOE_BACKUP_REQ_CH0                    ((u32)0x00000001 << 24)          /*!<R/W 0  WLOFF REG backup request, clear by hw */
#define BIT_MASK_HIOE_END_ADDR_CH0                 ((u32)0x00000FFF << 12)          /*!<R/W 0  HIOE command end address in txpktbuffer, unit:8byte */
#define BIT_HIOE_END_ADDR_CH0(x)                   ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_HIOE_END_ADDR_CH0(x)               ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_MASK_HIOE_STR_ADDR_CH0                 ((u32)0x00000FFF << 0)          /*!<R/W 0  HIOE command start address in txpktbuffer, unit:8byte */
#define BIT_HIOE_STR_ADDR_CH0(x)                   ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_HIOE_STR_ADDR_CH0(x)               ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BB_RFC_ATR
 * @brief BB/RFC REG Access Time Register
 * @{
 *****************************************************************************/
#define BIT_MASK_PHYRE_CNT                         ((u32)0x000000FF << 24)          /*!<R/W 0x07  the timing after RE for host/cpu access BB/RFC REG */
#define BIT_PHYRE_CNT(x)                           ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_PHYRE_CNT(x)                       ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_PHYWR_SETUP_CNT                   ((u32)0x000000FF << 16)          /*!<R/W 0x05  the timing WDATA stable before WE for host/cpu access BB/RFC REG */
#define BIT_PHYWR_SETUP_CNT(x)                     ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PHYWR_SETUP_CNT(x)                 ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_PHYWR_HOLD_CNT                    ((u32)0x000000FF << 8)          /*!<R/W 0x04  the timing WDATA stable after WE for host/cpu access BB/RFC REG */
#define BIT_PHYWR_HOLD_CNT(x)                      ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_PHYWR_HOLD_CNT(x)                  ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_PHYRD_WAIT_CNT                    ((u32)0x000000FF << 0)          /*!<R/W 0x06  the timing RDATA stable after ADDR for host/cpu access BB/RFC REG */
#define BIT_PHYRD_WAIT_CNT(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_PHYRD_WAIT_CNT(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SW_DEFINED_PAGE1
 * @brief SD Reserved register for 8051 information
 * @{
 *****************************************************************************/
#define BIT_MASK_SW_DEFINED_PAGE1_V1               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_SW_DEFINED_PAGE1_V1(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_SW_DEFINED_PAGE1_V1(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SW_DEFINED_PAGE2
 * @brief SD Reserved register for 8051 information
 * @{
 *****************************************************************************/
#define BIT_MASK_SW_DEFINED_PAGE2                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_SW_DEFINED_PAGE2(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_SW_DEFINED_PAGE2(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MCUTST_I
 * @brief Test register
 * @{
 *****************************************************************************/
#define BIT_MASK_MCUDMSG_I                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_MCUDMSG_I(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MCUDMSG_I(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MCUTST_II
 * @brief Test register
 * @{
 *****************************************************************************/
#define BIT_MASK_MCUDMSG_II                        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_MCUDMSG_II(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MCUDMSG_II(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FMETHR
 * @brief Firmware Message Mail Box Register
 * @{
 *****************************************************************************/
#define BIT_FWMSG_INTPS                            ((u32)0x00000001 << 31)          /*!<R/W 0  Toggle this bit will generate C2H interrupt to Host */
#define BIT_MASK_FW_MSG                            ((u32)0x7FFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_FW_MSG(x)                              ((u32)(((x) & 0x7FFFFFFF) << 0))
#define BIT_GET_FW_MSG(x)                          ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMETFR
 * @brief Host Message Mail Box Status register
 * @{
 *****************************************************************************/
#define BIT_MASK_HRCV_MSG                          ((u32)0x000000FF << 24)          /*!<R/W 0  Host Recover Message, Host Write This Byte will generate interrupt to 8051 (0x0134h[5]) */
#define BIT_HRCV_MSG(x)                            ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_HRCV_MSG(x)                        ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_H3MSG_VLD                              ((u32)0x00000001 << 3)          /*!<R/W 0  MSG_BOX_3 Valid. Enable when the Entry is write, Clear when the Entry is Read by 8051. */
#define BIT_H2MSG_VLD                              ((u32)0x00000001 << 2)          /*!<R/W 0  MSG_BOX_2 Valid. Enable when the Entry is write, Clear when the Entry is Read by 8051. */
#define BIT_H1MSG_VLD                              ((u32)0x00000001 << 1)          /*!<R/W 0  MSG_BOX_1 Valid. Enable when the Entry is write, Clear when the Entry is Read by 8051. */
#define BIT_H0MSG_VLD                              ((u32)0x00000001 << 0)          /*!<R/W 0  MSG_BOX_0 Valid. Enable when the Entry is write, Clear when the Entry is Read by 8051. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HOST_MSG_0                        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_0(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HOST_MSG_0(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HOST_MSG_1                        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_1(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HOST_MSG_1(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HOST_MSG_2                        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_2(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HOST_MSG_2(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HOST_MSG_3                        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HOST_MSG_3(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HOST_MSG_3(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GENTST
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_GENTST                            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Dummy register for MAC I/O REG Read/Write Access TEST */
#define BIT_GENTST(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_GENTST(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BB_ACCESS_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BB_WRITE_READ                     ((u32)0x00000003 << 30)          /*!<R/W 0  00 : No Active Baseband cycle 01 : Baseband Write Access (auto return to 00 when access finished) 1X : Baseband Read Access (auto return to 00 when access finished) */
#define BIT_BB_WRITE_READ(x)                       ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_BB_WRITE_READ(x)                   ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_BB_WRITE_EN_V1                    ((u32)0x0000000F << 16)          /*!<R/W 0  Byte Write Enable */
#define BIT_BB_WRITE_EN_V1(x)                      ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_BB_WRITE_EN_V1(x)                  ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_BB_ADDR_V1                        ((u32)0x00003FFF << 2)          /*!<R/W 0  Baseband Access Write/Read Address (in DW unit) */
#define BIT_BB_ADDR_V1(x)                          ((u32)(((x) & 0x00003FFF) << 2))
#define BIT_GET_BB_ADDR_V1(x)                      ((u32)(((x >> 2) & 0x00003FFF)))
#define BIT_BB_ERRACC                              ((u32)0x00000001 << 0)          /*!<R/W 0  Duplicate Access when previous cycle pending (write one clear) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BB_ACCESS_DATA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BB_DATA                           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Baseband Access Write/Read Data (in DW unit) */
#define BIT_BB_DATA(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BB_DATA(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX_E0
 * @brief Host Message Mail Box 4 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_HMEBOX_E0                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HMEBOX_E0(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HMEBOX_E0(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX_E1
 * @brief Host Message Mail Box 5 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_HMEBOX_E1                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HMEBOX_E1(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HMEBOX_E1(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX_E2
 * @brief Host Message Mail Box 6 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_HMEBOX_E2                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HMEBOX_E2(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HMEBOX_E2(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HMEBOX_E3
 * @brief Host Message Mail Box 7 Register
 * @{
 *****************************************************************************/
#define BIT_MASK_HMEBOX_E3                         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_HMEBOX_E3(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_HMEBOX_E3(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FS_FW_DEBUG_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FS_FW_DEBUG_0                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_FS_FW_DEBUG_0(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FS_FW_DEBUG_0(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FS_FW_DEBUG_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FS_FW_DEBUG_1                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_FS_FW_DEBUG_1(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FS_FW_DEBUG_1(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FS_FW_DEBUG_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FS_FW_DEBUG_2                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_FS_FW_DEBUG_2(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FS_FW_DEBUG_2(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FS_FW_DEBUG_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FS_FW_DEBUG_3                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  User Defined Message */
#define BIT_FS_FW_DEBUG_3(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FS_FW_DEBUG_3(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_ZB2WL_MAILBOX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_ZB2WL_MAILBOX                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Write to generate interrupt (0x116C [31]) */
#define BIT_ZB2WL_MAILBOX(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_ZB2WL_MAILBOX(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_WL2ZB_MAILBOX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WL2ZB_MAILBOX                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Write to generate interrupt (0x0C4 [10]) */
#define BIT_WL2ZB_MAILBOX(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_WL2ZB_MAILBOX(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE2IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_ATIM_PORT1_INT_EN                   ((u32)0x00000001 << 24)          /*!<R/W 0  PORT1 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB7_INT_EN                     ((u32)0x00000001 << 23)          /*!<R/W 0  MBSSID7 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB6_INT_EN                     ((u32)0x00000001 << 22)          /*!<R/W 0  MBSSID6 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB5_INT_EN                     ((u32)0x00000001 << 21)          /*!<R/W 0  MBSSID5 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB4_INT_EN                     ((u32)0x00000001 << 20)          /*!<R/W 0  MBSSID4 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB3_INT_EN                     ((u32)0x00000001 << 19)          /*!<R/W 0  MBSSID3 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB2_INT_EN                     ((u32)0x00000001 << 18)          /*!<R/W 0  MBSSID2 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB1_INT_EN                     ((u32)0x00000001 << 17)          /*!<R/W 0  MBSSID1 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB0_INT_EN                     ((u32)0x00000001 << 16)          /*!<R/W 0  MBSSID0 ATIM end window interrupt . */
#define BIT_FS_CTWE_INT2_EN                        ((u32)0x00000001 << 15)          /*!<R/W 0  port2 CTWindow end interrupt for p2p. */
#define BIT_FS_CTWE_INT_EN0                        ((u32)0x00000001 << 14)          /*!<R/W 0  port0 CTWindow end interrupt for p2p. */
#define BIT_FS_CTWE_INT_EN1                        ((u32)0x00000001 << 13)          /*!<R/W 0  port1 CTWindow end interrupt for p2p. */
#define BIT_FS_TBTTALL_INT_EN                      ((u32)0x00000001 << 12)          /*!<R/W 0  the interrupt from the exact TBTT time for all port. */
#define BIT_FS_PO_TBTT_AGG_INT_EN                  ((u32)0x00000001 << 10)          /*!<R/W 0  the interrupt from the multiplt TBTT times for P0. */
#define BIT_FS_TBTT2INT_EN                         ((u32)0x00000001 << 9)          /*!<R/W 0  the interrupt from the exact TBTT time for P2. */
#define BIT_FS_TBTT1INT_EN                         ((u32)0x00000001 << 8)          /*!<R/W 0  the interrupt from the exact TBTT time for P1. */
#define BIT_FS_TBTT0_MB7INT_EN                     ((u32)0x00000001 << 7)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID7. */
#define BIT_FS_TBTT0_MB6INT_EN                     ((u32)0x00000001 << 6)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID6. */
#define BIT_FS_TBTT0_MB5INT_EN                     ((u32)0x00000001 << 5)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID5. */
#define BIT_FS_TBTT0_MB4INT_EN                     ((u32)0x00000001 << 4)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID4. */
#define BIT_FS_TBTT0_MB3INT_EN                     ((u32)0x00000001 << 3)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID3. */
#define BIT_FS_TBTT0_MB2INT_EN                     ((u32)0x00000001 << 2)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID2. */
#define BIT_FS_TBTT0_MB1INT_EN                     ((u32)0x00000001 << 1)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID1. */
#define BIT_FS_TBTT0_INT_EN                        ((u32)0x00000001 << 0)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID0. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE2ISR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_ATIM_PORT1_INT                      ((u32)0x00000001 << 24)          /*!<RW1C 0  PORT1 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB7_INT                        ((u32)0x00000001 << 23)          /*!<RW1C 0  MBSSID7 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB6_INT                        ((u32)0x00000001 << 22)          /*!<RW1C 0  MBSSID6 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB5_INT                        ((u32)0x00000001 << 21)          /*!<RW1C 0  MBSSID5 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB4_INT                        ((u32)0x00000001 << 20)          /*!<RW1C 0  MBSSID4 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB3_INT                        ((u32)0x00000001 << 19)          /*!<RW1C 0  MBSSID3 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB2_INT                        ((u32)0x00000001 << 18)          /*!<RW1C 0  MBSSID2 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB1_INT                        ((u32)0x00000001 << 17)          /*!<RW1C 0  MBSSID1 ATIM end window interrupt . */
#define BIT_FS_ATIM_MB0_INT                        ((u32)0x00000001 << 16)          /*!<RW1C 0  MBSSID0 ATIM end window interrupt . */
#define BIT_FS_CTWE_INT2                           ((u32)0x00000001 << 15)          /*!<RW1C 0  port2 CTWindow end interrupt for p2p. */
#define BIT_FS_CTWE_INT0                           ((u32)0x00000001 << 14)          /*!<RW1C 0  port0 CTWindow end interrupt for p2p. */
#define BIT_FS_CTWE_INT1                           ((u32)0x00000001 << 13)          /*!<RW1C 0  port1 CTWindow end interrupt for p2p. */
#define BIT_FS_TBTTALL_INT                         ((u32)0x00000001 << 12)          /*!<RW1C 0  the interrupt from the exact TBTT time for all port. */
#define BIT_FS_PO_TBTT_AGG_INT                     ((u32)0x00000001 << 10)          /*!<RW1C 0  the interrupt from the multiplt TBTT times for P0. */
#define BIT_FS_TBTT2INT                            ((u32)0x00000001 << 9)          /*!<RW1C 0  the interrupt from the exact TBTT time for P2. */
#define BIT_FS_TBTT1INT                            ((u32)0x00000001 << 8)          /*!<RW1C 0  the interrupt from the exact TBTT time for P1. */
#define BIT_FS_TBTT0_MB7INT                        ((u32)0x00000001 << 7)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID7. */
#define BIT_FS_TBTT0_MB6INT                        ((u32)0x00000001 << 6)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID6. */
#define BIT_FS_TBTT0_MB5INT                        ((u32)0x00000001 << 5)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID5. */
#define BIT_FS_TBTT0_MB4INT                        ((u32)0x00000001 << 4)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID4. */
#define BIT_FS_TBTT0_MB3INT                        ((u32)0x00000001 << 3)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID3. */
#define BIT_FS_TBTT0_MB2INT                        ((u32)0x00000001 << 2)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID2. */
#define BIT_FS_TBTT0_MB1INT                        ((u32)0x00000001 << 1)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID1. */
#define BIT_FS_TBTT0_INT                           ((u32)0x00000001 << 0)          /*!<RW1C 0  the interrupt from the exact TBTT time for P0 MBSSID0. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE3IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FE1ISR_INT_EN                          ((u32)0x00000001 << 31)          /*!<R/W 0  FE1ISR Indicator (FE1ISR and FE1IMR are true, this bit is set to 1) */
#define BIT_FE2ISR_INT_EN                          ((u32)0x00000001 << 30)          /*!<R/W 0  FE2ISR Indicator (FE2ISR and FE2IMR are true, this bit is set to 1) */
#define BIT_FE4ISR_INT_EN                          ((u32)0x00000001 << 29)          /*!<R/W 0  FE4ISR Indicator (FE4ISR and FE4IMR are true, this bit is set to 1) */
#define BIT_FE5ISR_INT_EN                          ((u32)0x00000001 << 28)          /*!<R/W 0  FE5ISR Indicator (FE5ISR and FE5IMR are true, this bit is set to 1) */
#define BIT_FE6ISR_INT_EN                          ((u32)0x00000001 << 27)          /*!<R/W 0  FE6ISR Indicator (FE6ISR and FE6IMR are true, this bit is set to 1) */
#define BIT_FISR_INT_EN                            ((u32)0x00000001 << 26)          /*!<R/W 0  FISR Indicator (FISR and FIMR are true, this bit is set to 1) */
#define BIT_FISR1_INT_EN                           ((u32)0x00000001 << 25)          /*!<R/W 0  FISR1 Indicator (FISR1 and FIMR1 are true, this bit is set to 1) */
#define BIT_FS_BCNELY2_AGGR_INT_EN                 ((u32)0x00000001 << 17)          /*!<R/W 0  port 2 Beacon early aggregation interrupt */
#define BIT_FS_BCNELY1_AGGR_INT_EN                 ((u32)0x00000001 << 16)          /*!<R/W 0  port 1 Beacon early aggregation interrupt */
#define BIT_FS_BCNELY0_AGGR_INT_EN                 ((u32)0x00000001 << 15)          /*!<R/W 0  port 0 Beacon early aggregation interrupt */
#define BIT_FS_ON_SCK_INT_EN                       ((u32)0x00000001 << 14)          /*!<R/W 0  This interrupt is issued at the time set by ALL DRVERLYINT register before TBTT time. */
#define BIT_FS_BCNERLY2_INT_EN                     ((u32)0x00000001 << 9)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT2 time. */
#define BIT_FS_BCNERLY1_INT_EN                     ((u32)0x00000001 << 8)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT1 time. */
#define BIT_FS_BCNERLY0_MB7INT_EN                  ((u32)0x00000001 << 7)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINTregister before TBTT0_MB7 time. */
#define BIT_FS_BCNERLY0_MB6INT_EN                  ((u32)0x00000001 << 6)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB6 time. */
#define BIT_FS_BCNERLY0_MB5INT_EN                  ((u32)0x00000001 << 5)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB5 time. */
#define BIT_FS_BCNERLY0_MB4INT_EN                  ((u32)0x00000001 << 4)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINTregister before TBTT0_MB4 time. */
#define BIT_FS_BCNERLY0_MB3INT_EN                  ((u32)0x00000001 << 3)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB3 time. */
#define BIT_FS_BCNERLY0_MB2INT_EN                  ((u32)0x00000001 << 2)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB2 time. */
#define BIT_FS_BCNERLY0_MB1INT_EN                  ((u32)0x00000001 << 1)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINTregister before TBTT0_MB1 time. */
#define BIT_FS_BCNERLY0_INT_EN                     ((u32)0x00000001 << 0)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB0 time. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE3ISR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FE1ISR_INT                             ((u32)0x00000001 << 31)          /*!<RW1C 0  FE1ISR Indicator (FE1ISR and FE1IMR are true, this bit is set to 1) */
#define BIT_FE2ISR_INT                             ((u32)0x00000001 << 30)          /*!<RW1C 0  FE2ISR Indicator (FE2ISR and FE2IMR are true, this bit is set to 1) */
#define BIT_FE4ISR_INT                             ((u32)0x00000001 << 29)          /*!<RW1C 0  FE4ISR Indicator (FE4ISR and FE4IMR are true, this bit is set to 1) */
#define BIT_FE5ISR_INT                             ((u32)0x00000001 << 28)          /*!<RW1C 0  FE5ISR Indicator (FE5ISR and FE5IMR are true, this bit is set to 1) */
#define BIT_FE6ISR_INT                             ((u32)0x00000001 << 27)          /*!<RW1C 0  FE6ISR Indicator (FE6ISR and FE6IMR are true, this bit is set to 1) */
#define BIT_FISR_INT                               ((u32)0x00000001 << 26)          /*!<RW1C 0  FISR Indicator (FISR and FIMR are true, this bit is set to 1) */
#define BIT_FISR1_INT                              ((u32)0x00000001 << 25)          /*!<RW1C 0  FISR1 Indicator (FISR1 and FIMR1 are true, this bit is set to 1) */
#define BIT_FS_BCNELY2_AGGR_INT                    ((u32)0x00000001 << 17)          /*!<RW1C 0  port 2 Beacon early aggregation interrupt */
#define BIT_FS_BCNELY1_AGGR_INT                    ((u32)0x00000001 << 16)          /*!<RW1C 0  port 1 Beacon early aggregation interrupt */
#define BIT_FS_BCNELY0_AGGR_INT                    ((u32)0x00000001 << 15)          /*!<RW1C 0  port 0 Beacon early aggregation interrupt */
#define BIT_FS_ON_SCK_INT                          ((u32)0x00000001 << 14)          /*!<RW1C 0  This interrupt is issued at the time set by ALL DRVERLYINT register before TBTT time. */
#define BIT_FS_BCNERLY2_INT                        ((u32)0x00000001 << 9)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT2 time. */
#define BIT_FS_BCNERLY1_INT                        ((u32)0x00000001 << 8)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT1 time. */
#define BIT_FS_BCNERLY0_MB7INT                     ((u32)0x00000001 << 7)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINTregister before TBTT0_MB7 time. */
#define BIT_FS_BCNERLY0_MB6INT                     ((u32)0x00000001 << 6)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB6 time. */
#define BIT_FS_BCNERLY0_MB5INT                     ((u32)0x00000001 << 5)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB5 time. */
#define BIT_FS_BCNERLY0_MB4INT                     ((u32)0x00000001 << 4)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINTregister before TBTT0_MB4 time. */
#define BIT_FS_BCNERLY0_MB3INT                     ((u32)0x00000001 << 3)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB3 time. */
#define BIT_FS_BCNERLY0_MB2INT                     ((u32)0x00000001 << 2)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB2 time. */
#define BIT_FS_BCNERLY0_MB1INT                     ((u32)0x00000001 << 1)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINTregister before TBTT0_MB1 time. */
#define BIT_FS_BCNERLY0_INT                        ((u32)0x00000001 << 0)          /*!<RW1C 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB0 time. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE4IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_PCIE_BCNDMAERR_INT_EN               ((u32)0x00000001 << 31)          /*!<R/W 0  PCIE BCN DMA error interrupt */
#define BIT_FS_BCNDMA1_INT_EN                      ((u32)0x00000001 << 30)          /*!<R/W 0  When BCNDMA interval arrives before TBTT1, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB7_INT_EN                  ((u32)0x00000001 << 29)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB7, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB6_INT_EN                  ((u32)0x00000001 << 28)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB6, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB5_INT_EN                  ((u32)0x00000001 << 27)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB5, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB4_INT_EN                  ((u32)0x00000001 << 26)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB4, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB3_INT_EN                  ((u32)0x00000001 << 25)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB3, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB2_INT_EN                  ((u32)0x00000001 << 24)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB2, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB1_INT_EN                  ((u32)0x00000001 << 23)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB1, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_INT_EN                      ((u32)0x00000001 << 22)          /*!<R/W 0  When BCNDMA interval arrives before TBTT0_MB0, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA2_INT_EN                      ((u32)0x00000001 << 21)          /*!<R/W 0  When BCNDMA interval arrives before TBTT2, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_PORT0_PKTIN_INT_EN                     ((u32)0x00000001 << 17)          /*!<R/W 0  port 0 packet assign to queue interrupt */
#define BIT_PORT1_PKTIN_INT_EN                     ((u32)0x00000001 << 16)          /*!<R/W 0  port 1 packet assign to queue interrupt */
#define BIT_PORT1_RXUCMD0_OK_INT_EN                ((u32)0x00000001 << 3)          /*!<R/W 0  port 1 rx unicast packet with more data = 0 ok interrupt */
#define BIT_PORT1_RXUCMD1_OK_INT_EN                ((u32)0x00000001 << 2)          /*!<R/W 0  port 1 rx unicast packet with more data = 1 ok interrupt */
#define BIT_PORT1_RXBCMD0_OK_INT_EN                ((u32)0x00000001 << 1)          /*!<R/W 0  port 1 rx broadcast packet with more data = 0 ok interrupt */
#define BIT_PORT1_RXBCMD1_OK_INT_EN                ((u32)0x00000001 << 0)          /*!<R/W 0  port 1 rx broadcast packet with more data = 1 ok interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE4ISR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_PCIE_BCNDMAERR_INT                  ((u32)0x00000001 << 31)          /*!<RW1C 0  PCIE BCN DMA error interrupt */
#define BIT_FS_BCNDMA1_INT                         ((u32)0x00000001 << 30)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT1, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB7_INT                     ((u32)0x00000001 << 29)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB7, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB6_INT                     ((u32)0x00000001 << 28)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB6, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB5_INT                     ((u32)0x00000001 << 27)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB5, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB4_INT                     ((u32)0x00000001 << 26)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB4, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB3_INT                     ((u32)0x00000001 << 25)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB3, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB2_INT                     ((u32)0x00000001 << 24)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB2, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_MB1_INT                     ((u32)0x00000001 << 23)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB1, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA0_INT                         ((u32)0x00000001 << 22)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT0_MB0, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_FS_BCNDMA2_INT                         ((u32)0x00000001 << 21)          /*!<RW1C 0  When BCNDMA interval arrives before TBTT2, this interrupt informs MCU to prepare related beacon tasks. */
#define BIT_PORT0_PKTIN_INT                        ((u32)0x00000001 << 17)          /*!<RW1C 0  port 0 packet assign to queue interrupt */
#define BIT_PORT1_PKTIN_INT                        ((u32)0x00000001 << 16)          /*!<RW1C 0  port 1 packet assign to queue interrupt */
#define BIT_PORT1_RXUCMD0_OK_INT                   ((u32)0x00000001 << 3)          /*!<RW1C 0  port 1 rx unicast packet with more data = 0 ok interrupt */
#define BIT_PORT1_RXUCMD1_OK_INT                   ((u32)0x00000001 << 2)          /*!<RW1C 0  port 1 rx unicast packet with more data = 1 ok interrupt */
#define BIT_PORT1_RXBCMD0_OK_INT                   ((u32)0x00000001 << 1)          /*!<RW1C 0  port 1 rx broadcast packet with more data = 0 ok interrupt */
#define BIT_PORT1_RXBCMD1_OK_INT                   ((u32)0x00000001 << 0)          /*!<RW1C 0  port 1 rx broadcast packet with more data = 1 ok interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FT1IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_RXFTMREQ_INT_EN                     ((u32)0x00000001 << 28)          /*!<R/W 0  If BIT_RXFTMREQ_INT_EN =1. When HW receive FTMREQ packet, it will generate interrupt to firmware. */
#define BIT_FS_RXFTM_INT_EN                        ((u32)0x00000001 << 27)          /*!<R/W 0  If BIT_RXFTM_INT_EN =1. When HW receive FTM and transmit ACK frame successfully, the BB generate FTM report. It will generate interrupt to firmware. */
#define BIT_FS_TXFTM_INT_EN                        ((u32)0x00000001 << 26)          /*!<R/W 0  If BIT_TXFTM_INT_EN =1. When HW transmit FTM and receive ACK frame successfully, the BB generate FTM report. It will generate interrupt to firmware. */
#define BIT_FS_RXTMREQ_INT_EN                      ((u32)0x00000001 << 25)          /*!<R/W 0  If BIT_RXTMREQ_INT_EN =1. When HW receive TMREQ packet, it will generate interrupt to firmware. */
#define BIT_FS_RXTM_INT_EN                         ((u32)0x00000001 << 24)          /*!<R/W 0  If BIT_RXTM_INT_EN =1. When HW receive TM and transmit ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_FS_TXTM_INT_EN                         ((u32)0x00000001 << 23)          /*!<R/W 0  If BIT_TXTM_INT_EN =1. When HW transmit TM and receive ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_FS_CTWEND2_INT_EN                      ((u32)0x00000001 << 17)          /*!<R/W 0  CTWindow End NOA2 interrupt. */
#define BIT_FS_CTWEND1_INT_EN                      ((u32)0x00000001 << 16)          /*!<R/W 0  CTWindow End NOA1 interrupt. */
#define BIT_FS_CTWEND0_INT_EN                      ((u32)0x00000001 << 15)          /*!<R/W 0  CTWindow End NOA0 interrupt. */
#define BIT_FS_TX_NULL1_P0_INT_EN                  ((u32)0x00000001 << 14)          /*!<R/W 0  TX null 1 packet done interrupt */
#define BIT_FS_TX_NULL0_P0_INT_EN                  ((u32)0x00000001 << 13)          /*!<R/W 0  TX null 0 packet done interrupt */
#define BIT_FS_NOA0_TSF_BIT32_INT_EN               ((u32)0x00000001 << 12)          /*!<R/W 0  NOA unit0 TSFT bit 32 toggle interrupt */
#define BIT_FS_P2P_RFON2_INT_EN                    ((u32)0x00000001 << 11)          /*!<R/W 0  P2P NoA RF on time indication 2 interrupt */
#define BIT_FS_P2P_RFOFF2_INT_EN                   ((u32)0x00000001 << 10)          /*!<R/W 0  P2P NoA RF off time indication 2 interrupt */
#define BIT_FS_P2P_RFON1_INT_EN                    ((u32)0x00000001 << 9)          /*!<R/W 0  P2P NoA RF on time indication 1 interrupt */
#define BIT_FS_P2P_RFOFF1_INT_EN                   ((u32)0x00000001 << 8)          /*!<R/W 0  P2P NoA RF off time indication 1 interrupt */
#define BIT_FS_P2P_RFON0_INT_EN                    ((u32)0x00000001 << 7)          /*!<R/W 0  P2P NoA RF on time indication 0 interrupt */
#define BIT_FS_P2P_RFOFF0_INT_EN                   ((u32)0x00000001 << 6)          /*!<R/W 0  P2P NoA RF off time indication 0 interrupt */
#define BIT_FS_RX_UAPSDMD1_INT_EN                  ((u32)0x00000001 << 5)          /*!<R/W 0  Setting this bit indicates Rx pkt is matched with one of below: 1.) QoS Unicast data frame with MD=1, UAPSD=0 2.) NonQoS Unicast data frame with MD=1 */
#define BIT_FS_RX_UAPSDMD0_INT_EN                  ((u32)0x00000001 << 4)          /*!<R/W 0  Setting this bit indicates Rx pkt is matched with one of below: 1.) QoS Unicast data frame with MD=0, UAPSD=0 2.) NonQoS Unicast data frame with MD=0 */
#define BIT_FS_TRIGGER_PKT_INT_EN                  ((u32)0x00000001 << 3)          /*!<R/W 0  tx TRIGGER_PKT interrupt */
#define BIT_FS_EOSP_INT_EN                         ((u32)0x00000001 << 2)          /*!<R/W 0  EOSP_INT interrupt mask */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FT1ISR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_RXFTMREQ_INT                        ((u32)0x00000001 << 28)          /*!<RW1C 0  If BIT_RXFTMREQ_INT_EN =1. When HW receive FTMREQ packet, it will generate interrupt to firmware. */
#define BIT_FS_RXFTM_INT                           ((u32)0x00000001 << 27)          /*!<RW1C 0  If BIT_RXFTM_INT_EN =1. When HW receive FTM and transmit ACK frame successfully, the BB generate FTM report. It will generate interrupt to firmware. */
#define BIT_FS_TXFTM_INT                           ((u32)0x00000001 << 26)          /*!<RW1C 0  If BIT_TXFTM_INT_EN =1. When HW transmit FTM and receive ACK frame successfully, the BB generate FTM report. It will generate interrupt to firmware. */
#define BIT_FS_RXTMREQ_INT                         ((u32)0x00000001 << 25)          /*!<RW1C 0  If BIT_RXTMREQ_INT_EN =1. When HW receive TMREQ packet, it will generate interrupt to firmware. */
#define BIT_FS_RXTM_INT                            ((u32)0x00000001 << 24)          /*!<RW1C 0  If BIT_RXTM_INT_EN =1. When HW receive TM and transmit ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_FS_TXTM_INT                            ((u32)0x00000001 << 23)          /*!<RW1C 0  If BIT_TXTM_INT_EN =1. When HW transmit TM and receive ACK frame successfully. It will generate interrupt to firmware. */
#define BIT_FS_CTWEND2_INT                         ((u32)0x00000001 << 17)          /*!<RW1C 0  CTWindow End NOA2 interrupt. */
#define BIT_FS_CTWEND1_INT                         ((u32)0x00000001 << 16)          /*!<RW1C 0  CTWindow End NOA1 interrupt. */
#define BIT_FS_CTWEND0_INT                         ((u32)0x00000001 << 15)          /*!<RW1C 0  CTWindow End NOA0 interrupt. */
#define BIT_FS_TX_NULL1_P0_INT                     ((u32)0x00000001 << 14)          /*!<RW1C 0  TX null 1 packet done interrupt */
#define BIT_FS_TX_NULL0_P0_INT                     ((u32)0x00000001 << 13)          /*!<RW1C 0  TX null 0 packet done interrupt */
#define BIT_FS_NOA0_TSF_BIT32_INT                  ((u32)0x00000001 << 12)          /*!<RW1C 0  NOA unit0 TSFT bit 32 toggle interrupt */
#define BIT_FS_P2P_RFON2_INT                       ((u32)0x00000001 << 11)          /*!<RW1C 0  P2P NoA RF on time indication 2 interrupt */
#define BIT_FS_P2P_RFOFF2_INT                      ((u32)0x00000001 << 10)          /*!<RW1C 0  P2P NoA RF off time indication 2 interrupt */
#define BIT_FS_P2P_RFON1_INT                       ((u32)0x00000001 << 9)          /*!<RW1C 0  P2P NoA RF on time indication 1 interrupt */
#define BIT_FS_P2P_RFOFF1_INT                      ((u32)0x00000001 << 8)          /*!<RW1C 0  P2P NoA RF off time indication 1 interrupt */
#define BIT_FS_P2P_RFON0_INT                       ((u32)0x00000001 << 7)          /*!<RW1C 0  P2P NoA RF on time indication 0 interrupt */
#define BIT_FS_P2P_RFOFF0_INT                      ((u32)0x00000001 << 6)          /*!<RW1C 0  P2P NoA RF off time indication 0 interrupt */
#define BIT_FS_RX_UAPSDMD1_INT                     ((u32)0x00000001 << 5)          /*!<RW1C 0  Setting this bit indicates Rx pkt is matched with one of below: 1.) QoS Unicast data frame with MD=1, UAPSD=0 2.) NonQoS Unicast data frame with MD=1 */
#define BIT_FS_RX_UAPSDMD0_INT                     ((u32)0x00000001 << 4)          /*!<RW1C 0  Setting this bit indicates Rx pkt is matched with one of below: 1.) QoS Unicast data frame with MD=0, UAPSD=0 2.) NonQoS Unicast data frame with MD=0 */
#define BIT_FS_TRIGGER_PKT_INT                     ((u32)0x00000001 << 3)          /*!<RW1C 0  tx TRIGGER_PKT interrupt */
#define BIT_FS_EOSP_INT                            ((u32)0x00000001 << 2)          /*!<RW1C 0  EOSP_INT interrupt mask */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TWT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RX_ACKMD_EN                            ((u32)0x00000001 << 1)          /*!<R/W 0  Enable ACK/BA/MU-BA frame more data interrupt */
#define BIT_RX_MORE_TF_EN                          ((u32)0x00000001 << 0)          /*!<R/W 0  Enable trigger frame more TF field interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE5IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_ZIGBEE2WLAN_MAILBOX_MSK             ((u32)0x00000001 << 31)          /*!<R/W 0  Mailbox INT from ZIGBEE CPU */
#define BIT_FS_EXT_INTF4_EN_MSK                    ((u32)0x00000001 << 5)          /*!<R/W 0  ext bt mailbox int,mailbox_outempty_int| mailbox_in_rdy_int */
#define BIT_FS_GNT_BT_BB_FALL_MSK                  ((u32)0x00000001 << 4)          /*!<R/W 0  ext Mailbox INT from btcoex */
#define BIT_FS_GNT_BT_BB_RISE_MSK                  ((u32)0x00000001 << 3)          /*!<R/W 0  ext Mailbox INT from btcoex */
#define BIT_FS_BT2WL_STS_INT_MSK                   ((u32)0x00000001 << 2)          /*!<R/W 0  ext scoreboard INT from BT */
#define BIT_FS_BT_MAILBOX_I2C_MSK                  ((u32)0x00000001 << 1)          /*!<R/W 0  Mailbox INT from I2C */
#define BIT_FS_I2C_INTF4_EN_MSK                    ((u32)0x00000001 << 0)          /*!<R/W 0  bt mailbox int,mailbox_outempty_int| mailbox_in_rdy_int form I2C */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE5ISR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_ZIGBEE2WLAN_MAILBOX                 ((u32)0x00000001 << 31)          /*!<RW1C 0  Mailbox INT from ZIGBEE CPU */
#define BIT_FS_EXT_INTF4_EN                        ((u32)0x00000001 << 5)          /*!<RW1C 0  ext bt mailbox int,mailbox_outempty_int| mailbox_in_rdy_int */
#define BIT_FS_GNT_BT_BB_FALL                      ((u32)0x00000001 << 4)          /*!<RW1C 0  ext Mailbox INT from btcoex */
#define BIT_FS_GNT_BT_BB_RISE                      ((u32)0x00000001 << 3)          /*!<RW1C 0  ext Mailbox INT from btcoex */
#define BIT_FS_BT2WL_STS_INT                       ((u32)0x00000001 << 2)          /*!<RW1C 0  ext Mailbox INT from BT */
#define BIT_FS_BT_MAILBOX_I2C                      ((u32)0x00000001 << 1)          /*!<RW1C 0  Mailbox INT from I2C */
#define BIT_FS_I2C_INTF4_EN                        ((u32)0x00000001 << 0)          /*!<RW1C 0  bt mailbox int,mailbox_outempty_int| mailbox_in_rdy_int form I2C */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE6IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PORT0_RXUCMD0_ACK_INT_EN               ((u32)0x00000001 << 9)          /*!<R/W 0  port 0 rx unicast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT0_RXUCMD1_ACK_INT_EN               ((u32)0x00000001 << 8)          /*!<R/W 0  port 0 rx unicast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_PORT0_RXBCMD0_ACK_INT_EN               ((u32)0x00000001 << 7)          /*!<R/W 0  port 0 rx broadcast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT0_RXBCMD1_ACK_INT_EN               ((u32)0x00000001 << 6)          /*!<R/W 0  port 0 rx broadcast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_PORT1_RXUCMD0_ACK_INT_EN               ((u32)0x00000001 << 5)          /*!<R/W 0  port 1 rx unicast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT1_RXUCMD1_ACK_INT_EN               ((u32)0x00000001 << 4)          /*!<R/W 0  port 1 rx unicast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_PORT1_RXBCMD0_ACK_INT_EN               ((u32)0x00000001 << 3)          /*!<R/W 0  port 1 rx broadcast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT1_RXBCMD1_ACK_INT_EN               ((u32)0x00000001 << 2)          /*!<R/W 0  port 1 rx broadcast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_RX_MORE_TF0_INT_EN                     ((u32)0x00000001 << 1)          /*!<R/W 0  interrupt source from trigger frame with more TF field = 0 not addressed to us */
#define BIT_RX_MORE_TF1_INT_EN                     ((u32)0x00000001 << 0)          /*!<R/W 0  Interrupt source from receiving trigger frame with more TF field = 1 not addressed to us */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FE6ISR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PORT0_RXUCMD0_ACK_INT                  ((u32)0x00000001 << 9)          /*!<RW1C 0  port 0 rx unicast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT0_RXUCMD1_ACK_INT                  ((u32)0x00000001 << 8)          /*!<RW1C 0  port 0 rx unicast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_PORT0_RXBCMD0_ACK_INT                  ((u32)0x00000001 << 7)          /*!<RW1C 0  port 0 rx broadcast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT0_RXBCMD1_ACK_INT                  ((u32)0x00000001 << 6)          /*!<RW1C 0  port 0 rx broadcast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_PORT1_RXUCMD0_ACK_INT                  ((u32)0x00000001 << 5)          /*!<RW1C 0  port 1 rx unicast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT1_RXUCMD1_ACK_INT                  ((u32)0x00000001 << 4)          /*!<RW1C 0  port 1 rx unicast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_PORT1_RXBCMD0_ACK_INT                  ((u32)0x00000001 << 3)          /*!<RW1C 0  port 1 rx broadcast ACK/BA/MBA with more data = 0 ok interrupt */
#define BIT_PORT1_RXBCMD1_ACK_INT                  ((u32)0x00000001 << 2)          /*!<RW1C 0  port 1 rx broadcast ACK/BA/MBA with more data = 1 ok interrupt */
#define BIT_RX_MORE_TF0_INT                        ((u32)0x00000001 << 1)          /*!<RW1C 0  interrupt source from trigger frame with more TF field = 0 not addressed to us */
#define BIT_RX_MORE_TF1_INT                        ((u32)0x00000001 << 0)          /*!<RW1C 0  Interrupt source from receiving trigger frame with more TF field = 1 not addressed to us */
/** @} */

/**************************************************************************//**
 * @defgroup REG_ext_queue_register
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXBUF_WKCAM_OFFSET                ((u32)0x00001FFF << 3)          /*!<R/W 0  the offset of WOL pattern stored in txpktbuffer, unit is 8byte */
#define BIT_TXBUF_WKCAM_OFFSET(x)                  ((u32)(((x) & 0x00001FFF) << 3))
#define BIT_GET_TXBUF_WKCAM_OFFSET(x)              ((u32)(((x >> 3) & 0x00001FFF)))
#define BIT_MASK_H2CQ_PRI                          ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define BIT_H2CQ_PRI(x)                            ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_H2CQ_PRI(x)                        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_I2C_BT_MAILBOX_OUT_LOW
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_I2C_MAILBOX_DATA_51TO81_L         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_I2C_MAILBOX_DATA_51TO81_L(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_I2C_MAILBOX_DATA_51TO81_L(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_I2C_BT_MAILBOX_OUT_HIGH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_I2C_MAILBOX_DATA_51TO81_H         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_I2C_MAILBOX_DATA_51TO81_H(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_I2C_MAILBOX_DATA_51TO81_H(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_I2C_BT_Mailbox_control
 * @brief
 * @{
 *****************************************************************************/
#define BIT_I2C_MAILBOX_IN_RDY_INTEN               ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_I2C_MAILBOX_IN_RDY                     ((u32)0x00000001 << 16)          /*!<RW1C 0   */
#define BIT_I2C_MAILBOX_OUTEMPTY_INTEN             ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_I2C_MAILBOX_OUT_VLD                    ((u32)0x00000001 << 0)          /*!<RW1C 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_I2C_BT_MAILBOX_IN_LOW
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_I2C_MAILBOX_DATA_81TO51_L         ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_I2C_MAILBOX_DATA_81TO51_L(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_I2C_MAILBOX_DATA_81TO51_L(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_I2C_BT_MAILBOX_IN_HIGH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_I2C_MAILBOX_DATA_81TO51_H         ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_I2C_MAILBOX_DATA_81TO51_H(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_I2C_MAILBOX_DATA_81TO51_H(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EXT_BT_MAILBOX_OUT_LOW
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EXT_MAILBOX_DATA_51TO81_L         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_EXT_MAILBOX_DATA_51TO81_L(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_EXT_MAILBOX_DATA_51TO81_L(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EXT_BT_MAILBOX_OUT_HIGH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EXT_MAILBOX_DATA_51TO81_H         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_EXT_MAILBOX_DATA_51TO81_H(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_EXT_MAILBOX_DATA_51TO81_H(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EXT_BT_Mailbox_control
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EXT_MAILBOX_IN_RDY_INTEN               ((u32)0x00000001 << 17)          /*!<R/W 0  Mailbox IN ready interrupt enable */
#define BIT_EXT_MAILBOX_IN_RDY                     ((u32)0x00000001 << 16)          /*!<RW1C 0  Mailbox ready BT2Wifi */
#define BIT_EXT_MAILBOX_OUTEMPTY_INTEN             ((u32)0x00000001 << 1)          /*!<R/W 0  Mailbox OUT empty interrupt enable */
#define BIT_EXT_MAILBOX_OUT_VLD                    ((u32)0x00000001 << 0)          /*!<RW1C 0  Mailbox ACK Wifi2BT */
/** @} */

/**************************************************************************//**
 * @defgroup REG_EXT_BT_MAILBOX_IN_LOW
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EXT_MAILBOX_DATA_81TO51_L         ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_EXT_MAILBOX_DATA_81TO51_L(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_EXT_MAILBOX_DATA_81TO51_L(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_EXT_BT_MAILBOX_IN_HIGH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EXT_MAILBOX_DATA_81_TO51_H        ((u32)0xFFFFFFFF << 0)          /*!<R 0   */
#define BIT_EXT_MAILBOX_DATA_81_TO51_H(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_EXT_MAILBOX_DATA_81_TO51_H(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FWIMR1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_TXBCNOK_PORT1_INT_EN                ((u32)0x00000001 << 31)          /*!<R/W 0  ENABLE PORT1 TX beacon OK interrupt */
#define BIT_FS_TXBCNERR_PORT1_INT_EN               ((u32)0x00000001 << 30)          /*!<R/W 0  ENABLE PORT1 TX beacon Error interrupt */
#define BIT_FS_TXBCNOK_PORT2_INT_EN                ((u32)0x00000001 << 29)          /*!<R/W 0  PORT2 TX beacon OK interrupt */
#define BIT_FS_TXBCNERR_PORT2_INT_EN               ((u32)0x00000001 << 28)          /*!<R/W 0  PORT2 TX beacon Error interrupt */
#define BIT_FS_EVTQ_TXDONE_INT_EN                  ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_FS_EVTQ_START_INT_EN                   ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_FS_DWWIN_END_INT_EN                    ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_FS_BCN_IE_0_HIT_INT_EN                 ((u32)0x00000001 << 24)          /*!<R/W 0  rx bcn with ie0 hit */
#define BIT_FS_BCN_IE_1_HIT_INT_EN                 ((u32)0x00000001 << 23)          /*!<R/W 0  rx bcn with ie1 hit */
#define BIT_FS_RXBCN_NAN_RPT_OK_EN                 ((u32)0x00000001 << 22)          /*!<R/W 0  port2 rxbcn rpt and write to txrptbuf ok */
#define BIT_FS_TIM_NO_HIT_INT_EN                   ((u32)0x00000001 << 21)          /*!<R/W 0  when no my aid or aid is 0(0x16DC[8]) or my bssid is not in tim element(0x16DB[31]),generate this interrupt */
#define BIT_FS_BC_TWT_PARA_CHG_INT_EN              ((u32)0x00000001 << 20)          /*!<R/W 0  RX broadcast twt with parameter changing,notify fw to read twt report */
#define BIT_FS_TIM_HIT_INT_EN                      ((u32)0x00000001 << 19)          /*!<R/W 0  when my aid or aid is 0 (0x16DC[8]) or my bssid is in tim element,generate this interrupt */
#define BIT_FS_HIOE_CHN0_INT_EN                    ((u32)0x00000001 << 18)          /*!<R/W 0  hioe_chn0 done interrupt */
#define BIT_FS_HIOE_CHN1_INT_EN                    ((u32)0x00000001 << 17)          /*!<R/W 0  hioe_chn1 done interrupt */
#define BIT_FS_HIOE_CHN_ON_INT_EN                  ((u32)0x00000001 << 16)          /*!<R/W 0  hioe_chn_on done interrupt */
#define BIT_FS_HIOE_CHN_OFF_INT_EN                 ((u32)0x00000001 << 15)          /*!<R/W 0  hioe_chn_off done interrupt */
#define BIT_FS_CHINFO_SEG_DONE_INT_NE              ((u32)0x00000001 << 14)          /*!<R/W 0  indicate one channel info segment is received ok */
#define BIT_FS_DFS_PKT_INT_EN                      ((u32)0x00000001 << 13)          /*!<R/W 0  enable interrupt when receiving dfs pkt and about to reach threshold of given pkt number or timeout */
#define BIT_FS_RX_PHYSTS_DONE_INT__EN              ((u32)0x00000001 << 12)          /*!<R/W 0  indicate physts is successfully write into physts_mem */
#define BIT_FS_OPS_TIM_HIT_INT_EN                  ((u32)0x00000001 << 11)          /*!<R/W 0  when my aid or my bssid is in tim frame/ops frame,generate this interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FWISR1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_FS_TXBCNOK_PORT1_INT                   ((u32)0x00000001 << 31)          /*!<RW1C 0  PORT1 TX beacon OK interrupt */
#define BIT_FS_TXBCNERR_PORT1_INT                  ((u32)0x00000001 << 30)          /*!<RW1C 0  PORT1 TX beacon Error interrupt */
#define BIT_FS_TXBCNOK_PORT2_INT                   ((u32)0x00000001 << 29)          /*!<RW1C 0  PORT2 TX beacon OK interrupt */
#define BIT_FS_TXBCNERR_PORT2_INT                  ((u32)0x00000001 << 28)          /*!<RW1C 0  PORT2 TX beacon Error interrupt */
#define BIT_FS_EVTQ_TXDONE_INT                     ((u32)0x00000001 << 27)          /*!<RW1C 0  event queue done interrupt for NAN */
#define BIT_FS_EVTQ_START_INT                      ((u32)0x00000001 << 26)          /*!<RW1C 0  event queue start interrupt for NAN */
#define BIT_FS_DWWIN_END_INT                       ((u32)0x00000001 << 25)          /*!<RW1C 0  discovery window end interrupt for NAN */
#define BIT_FS_BCN_IE_0_HIT_INT                    ((u32)0x00000001 << 24)          /*!<RW1C 0  rx bcn with ie0 hit */
#define BIT_FS_BCN_IE_1_HIT_INT                    ((u32)0x00000001 << 23)          /*!<RW1C 0  rx bcn with ie1 hit */
#define BIT_FS_RXBCN_NAN_RPT_OK                    ((u32)0x00000001 << 22)          /*!<RW1C 0  port2 rxbcn rpt and write to txrptbuf ok */
#define BIT_FS_TIM_NO_HIT_INT                      ((u32)0x00000001 << 21)          /*!<RW1C 0  when no my aid or aid is 0(0x16DC[8]) or my bssid is not in tim element(0x16DB[31]),generate this interrupt */
#define BIT_FS_BC_TWT_PARA_CHG_INT                 ((u32)0x00000001 << 20)          /*!<RW1C 0  RX broadcast twt with parameter changing,notify fw to read twt report */
#define BIT_FS_TIM_HIT_INT                         ((u32)0x00000001 << 19)          /*!<RW1C 0  when my aid or aid is 0 (0x16DC[8]) or my bssid is in tim element,generate this interrupt */
#define BIT_FS_HIOE_CHN0_INT                       ((u32)0x00000001 << 18)          /*!<RW1C 0  hioe_chn0 done interrupt */
#define BIT_FS_HIOE_CHN1_INT                       ((u32)0x00000001 << 17)          /*!<RW1C 0  hioe_chn1 done interrupt */
#define BIT_FS_HIOE_CHN_ON_INT                     ((u32)0x00000001 << 16)          /*!<RW1C 0  hioe_chn_on done interrupt */
#define BIT_FS_HIOE_CHN_OFF_INT                    ((u32)0x00000001 << 15)          /*!<RW1C 0  hioe_chn_off done interrupt */
#define BIT_FS_CHINFO_SEG_DONE_INT                 ((u32)0x00000001 << 14)          /*!<RW1C 0  indicate one channel info segment is received ok */
#define BIT_FS_DFS_PKT_INT                         ((u32)0x00000001 << 13)          /*!<RW1C 0  enable interrupt when receiving dfs pkt and about to reach threshold of given pkt number or timeout */
#define BIT_FS_RX_PHYSTS_DONE_INT                  ((u32)0x00000001 << 12)          /*!<RW1C 0  indicate physts is successfully write into physts_mem */
#define BIT_FS_OPS_TIM_HIT_INT                     ((u32)0x00000001 << 11)          /*!<RW1C 0  when my aid or my bssid is in tim frame/ops frame,generate this interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LPSOFFLOAD_IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_LPS_RXBCN_TSF_GT_THRES_INT_EN          ((u32)0x00000001 << 10)          /*!<R/W 0  The interrupt is issued when difference of tsft in beacon frame of port0 is greater than threshold */
#define BIT_LPS_RX_PHYSTS_DONE_INT_EN              ((u32)0x00000001 << 9)          /*!<R/W 0  indicate physts is successfully write into physts_mem */
#define BIT_LPS_HIOE_CHN_ON_INT_EN                 ((u32)0x00000001 << 8)          /*!<R/W 0  hioe_chn_on done interrupt */
#define BIT_LPS_HIOE_CHN_OFF_INT_EN                ((u32)0x00000001 << 7)          /*!<R/W 0  hioe_chn_off done interrupt */
#define BIT_LPS_TIM_NO_HIT_INT_EN                  ((u32)0x00000001 << 6)          /*!<R/W 0  when no my aid or aid is 0(0x16DC[8]) or my bssid is not in tim element(0x16DB[31]),generate this interrupt */
#define BIT_LPS_BCNERLY0_INT_EN                    ((u32)0x00000001 << 5)          /*!<R/W 0  This interrupt is issued at the time set by DRVERLYINT register before TBTT0_MB0 time. */
#define BIT_LPS_BCNELY0_AGGR_INT_EN                ((u32)0x00000001 << 4)          /*!<R/W 0  port 0 Beacon early aggregation interrupt */
#define BIT_LPS_TBTT0_INT_EN                       ((u32)0x00000001 << 3)          /*!<R/W 0  the interrupt from the exact TBTT time for P0 MBSSID0. */
#define BIT_LPS_RX_BCN_P0_INT_EN                   ((u32)0x00000001 << 2)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame occurs in port0. */
#define BIT_LPS_RX_BCN_ERR_P0_INT_EN               ((u32)0x00000001 << 1)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame with fcs error occurs in port0. */
#define BIT_LPS_RXBCN_TIMEUP_INT_EN                ((u32)0x00000001 << 0)          /*!<R/W 0  When this bit is enable, the interrupt is issued when RX beacon frame timeout occurs in port0. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FT2IMR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PORT1_RX_UCMD1_UAPSD0_OK_INT_EN        ((u32)0x00000001 << 19)          /*!<R/W 0  PORT1 rx unicast packet ok interrupt with more data =1 & UAPSD =0 */
#define BIT_PORT1_RX_UCMD0_UAPSD0_OK_INT_EN        ((u32)0x00000001 << 18)          /*!<R/W 0  PORT1 rx unicast packet ok interrupt with more data =0 & UAPSD =0 */
#define BIT_PORT1_TRIPKT_OK_INT_EN                 ((u32)0x00000001 << 17)          /*!<R/W 0  PORT1 trigger packet assign to queue or release interrupt */
#define BIT_PORT1_RX_EOSP_OK_INT_EN                ((u32)0x00000001 << 16)          /*!<R/W 0  PORT1 RX packet with EOSP =1 ok interrupt */
#define BIT_FS_TXPRPT_CNT_FULL_INT_EN              ((u32)0x00000001 << 10)          /*!<R/W 0  txrpt buffer full interrupt */
#define BIT_FS_NOA2_TSFT_BIT32_INT_EN              ((u32)0x00000001 << 9)          /*!<R/W 0  NOA unit2 TSFT bit 32 toggle interrupt */
#define BIT_FS_NOA1_TSFT_BIT32_INT_EN              ((u32)0x00000001 << 8)          /*!<R/W 0  NOA unit1 TSFT bit 32 toggle interrupt */
#define BIT_PORT1_TX_NULL1_DONE_INT_EN             ((u32)0x00000001 << 1)          /*!<R/W 0  PORT1 TX NULL1 packet ok interrupt */
#define BIT_PORT1_TX_NULL0_DONE_INT_EN             ((u32)0x00000001 << 0)          /*!<R/W 0  PORT1 TX NULL0 packet ok interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_FT2ISR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PORT1_RX_UCMD1_UAPSD0_OK_INT           ((u32)0x00000001 << 19)          /*!<RW1C 0  PORT1 rx unicast packet ok interrupt with more data =1 & UAPSD =0 */
#define BIT_PORT1_RX_UCMD0_UAPSD0_OK_INT           ((u32)0x00000001 << 18)          /*!<RW1C 0  PORT1 rx unicast packet ok interrupt with more data =0 & UAPSD =0 */
#define BIT_PORT1_TRIPKT_OK_INT                    ((u32)0x00000001 << 17)          /*!<RW1C 0  PORT1 trigger packet assign to queue or release interrupt */
#define BIT_PORT1_RX_EOSP_OK_INT                   ((u32)0x00000001 << 16)          /*!<RW1C 0  PORT1 RX packet with EOSP =1 ok interrupt */
#define BIT_FS_TXPRPT_CNT_FULL_INT                 ((u32)0x00000001 << 10)          /*!<RW1C 0  txrpt buffer full interrupt */
#define BIT_FS_NOA2_TSFT_BIT32_INT                 ((u32)0x00000001 << 9)          /*!<RW1C 0  NOA unit2 TSFT bit 32 toggle interrupt */
#define BIT_FS_NOA1_TSFT_BIT32_INT                 ((u32)0x00000001 << 8)          /*!<RW1C 0  NOA unit1 TSFT bit 32 toggle interrupt */
#define BIT_PORT1_TX_NULL1_DONE_INT                ((u32)0x00000001 << 1)          /*!<RW1C 0  PORT1 TX NULL1 packet ok interrupt */
#define BIT_PORT1_TX_NULL0_DONE_INT                ((u32)0x00000001 << 0)          /*!<RW1C 0  PORT1 TX NULL0 packet ok interrupt */
/** @} */

/**************************************************************************//**
 * @defgroup REG_MSG2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_MSG2                           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw message 2 */
#define BIT_FW_MSG2(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_MSG2(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MSG3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_MSG3                           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw message 3 */
#define BIT_FW_MSG3(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_MSG3(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MSG4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_MSG4                           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw message 4 */
#define BIT_FW_MSG4(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_MSG4(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MSG5
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_MSG5                           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  fw message 5 */
#define BIT_FW_MSG5(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW_MSG5(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */


/*==========PAGE1 Register Address Definition==========*/
#define REG_CR                                       0x0100
#define REG_PG_SIZE                                  0x0104
#define REG_TSF_CLK_STATE                            0x0108
#define REG_TXDMA_PQ_MAP                             0x010C
//#define REG_                                         0x0110
#define REG_TRXFF_BNDY                               0x0114
#define REG_PTA_I2C_MBOX                             0x0118
#define REG_RXFF_PTR                                 0x011C
#define REG_FE1IMR                                   0x0120
#define REG_FE1ISR                                   0x0124
//#define REG_                                         0x0128
//#define REG_                                         0x012C
#define REG_FWIMR                                    0x0130
#define REG_FWISR                                    0x0134
#define REG_FTIMR                                    0x0138
#define REG_FTISR                                    0x013C
#define REG_PHYSTS_REL                               0x0140
#define REG_CHANNEL_INFO_REL                         0x0144

#define REG_PKTBUF_DBG_DATA_H                        0x0148
//#define REG_                                         0x014C
#define REG_TC0_CTRL                                 0x0150
#define REG_TC1_CTRL                                 0x0154
#define REG_TC2_CTRL                                 0x0158
#define REG_TC3_CTRL                                 0x015C
#define REG_TC4_CTRL                                 0x0160
#define REG_TCUNIT_BASE                              0x0164
#define REG_TC5_CTRL                                 0x0168
#define REG_TC6_CTRL                                 0x016C
#define REG_HW_IOE_CR_CHN1                           0x0170
#define REG_HW_IOE_CR_CHN_ON                         0x0174
#define REG_HW_IOE_CR_CHN_OFF                        0x0178
#define REG_RXBCN_TIMER                              0x017C
#define REG_AES_DECRPT_DATA                          0x0180
#define REG_AES_DECRPT_CFG                           0x0184
#define REG_RSHA1_DATA                               0x0188
#define REG_MACCLKFRQ                                0x018C
#define REG_TMETER                                   0x0190
#define REG_OSC_32K_CTRL                             0x0194
#define REG_32K_CAL_REG0                             0x0198
#define REG_32K_XTAL_CR                              0x019C
#define REG_C2HEVT                                   0x01A0
#define REG_C2HEVT_1                                 0x01A4
#define REG_C2HEVT_2                                 0x01A8
#define REG_C2HEVT_3                                 0x01AC
#define REG_HW_IOE_CR_CH0                            0x01B0
#define REG_BB_RFC_ATR                               0x01B4
//#define REG_SW_DEFINED_PAGE1                         0x01B8
//#define REG_SW_DEFINED_PAGE2                         0x01BC
//#define REG_MCUTST_I                                 0x01C0
//#define REG_MCUTST_II                                0x01C4
#define REG_FMETHR                                   0x01C8
#define REG_HMETFR                                   0x01CC
#define REG_HMEBOX0                                  0x01D0
#define REG_HMEBOX1                                  0x01D4
#define REG_HMEBOX2                                  0x01D8
#define REG_HMEBOX3                                  0x01DC
#define REG_LLT_IND_ACCESS                           0x01E0
#define REG_GENTST                                   0x01E4
#define REG_BB_ACCESS_CTRL                           0x01E8
#define REG_BB_ACCESS_DATA                           0x01EC
#define REG_HMEBOX_E0                                0x01F0
#define REG_HMEBOX_E1                                0x01F4
#define REG_HMEBOX_E2                                0x01F8
#define REG_HMEBOX_E3                                0x01FC
//#define REG_FS_FW_DEBUG_0                            0x1100
//#define REG_FS_FW_DEBUG_1                            0x1104
//#define REG_FS_FW_DEBUG_2                            0x1108
//#define REG_FS_FW_DEBUG_3                            0x110C
#define REG_ZB2WL_MAILBOX                            0x1110
#define REG_WL2ZB_MAILBOX                            0x1114
//#define REG_                                         0x1118
//#define REG_                                         0x111C
#define REG_FE2IMR                                   0x1120
#define REG_FE2ISR                                   0x1124
#define REG_FE3IMR                                   0x1128
#define REG_FE3ISR                                   0x112C
#define REG_FE4IMR                                   0x1130
#define REG_FE4ISR                                   0x1134
#define REG_FT1IMR                                   0x1138
#define REG_FT1ISR                                   0x113C
#define REG_TWT_CTRL                                 0x1140
//#define REG_                                         0x1144
//#define REG_                                         0x1148
//#define REG_                                         0x114C
//#define REG_                                         0x1150
//#define REG_                                         0x1154
//#define REG_                                         0x1158
//#define REG_                                         0x115C
//#define REG_                                         0x1160
//#define REG_                                         0x1164
#define REG_FE5IMR                                   0x1168
#define REG_FE5ISR                                   0x116C
#define REG_FE6IMR                                   0x1170
#define REG_FE6ISR                                   0x1174
//#define REG_                                         0x1178
#define REG_EXT_QUEUE_REGISTER                       0x117C
//#define REG_                                         0x1180
//#define REG_                                         0x1184
//#define REG_                                         0x1188
//#define REG_                                         0x118C
#define REG_I2C_BT_MAILBOX_OUT_LOW                   0x1190
#define REG_I2C_BT_MAILBOX_OUT_HIGH                  0x1194
#define REG_I2C_BT_MAILBOX_CONTROL                   0x1198
#define REG_I2C_BT_MAILBOX_IN_LOW                    0x119C
#define REG_I2C_BT_MAILBOX_IN_HIGH                   0x11A0
#define REG_EXT_BT_MAILBOX_OUT_LOW                   0x11A4
#define REG_EXT_BT_MAILBOX_OUT_HIGH                  0x11A8
#define REG_EXT_BT_MAILBOX_CONTROL                   0x11AC
#define REG_EXT_BT_MAILBOX_IN_LOW                    0x11B0
#define REG_EXT_BT_MAILBOX_IN_HIGH                   0x11B4
//#define REG_                                         0x11B8
//#define REG_                                         0x11BC
#define REG_FWIMR1                                   0x11C0
#define REG_FWISR1                                   0x11C4
#define REG_LPSOFFLOAD_IMR                           0x11C8
//#define REG_                                         0x11CC
//#define REG_                                         0x11D0
//#define REG_                                         0x11D4
//#define REG_                                         0x11D8
//#define REG_                                         0x11DC
#define REG_FT2IMR                                   0x11E0
#define REG_FT2ISR                                   0x11E4
//#define REG_                                         0x11E8
//#define REG_                                         0x11EC
#define REG_MSG2                                     0x11F0
#define REG_MSG3                                     0x11F4
#define REG_MSG4                                     0x11F8
#define REG_MSG5                                     0x11FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
