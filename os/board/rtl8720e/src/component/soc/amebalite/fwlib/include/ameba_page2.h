#ifndef _AMEBAD2_PAGE2_H_
#define _AMEBAD2_PAGE2_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_RQPN_CTRL_HLPQ
 * @brief
 * @{
 *****************************************************************************/
#define BIT_LOAD_R_PGNUM               ((u32)0x00000001 << 31)          /*!<WA0 0  Write 1 to set RQPN bit[79:0] value to page numbers for initialization */
#define BIT_EPQ_PUBLIC_DIS             ((u32)0x00000001 << 27)          /*!<R/W 0  When this bit is set, Extra queue available Tx page size excludes the public queue. Otherwise, Extra queue available Tx page size includes public queue */
#define BIT_NPQ_PUBLIC_DIS             ((u32)0x00000001 << 26)          /*!<R/W 0  When this bit is set, Normal queue available Tx page size excludes the public queue. Otherwise, Normal queue available Tx page size includes public queue */
#define BIT_LPQ_PUBLIC_DIS             ((u32)0x00000001 << 25)          /*!<R/W 0  When this bit is set, Low queue available Tx page size excludes the public queue. Otherwise, Low queue available Tx page size includes public queue */
#define BIT_HPQ_PUBLIC_DIS             ((u32)0x00000001 << 24)          /*!<R/W 0  When this bit is set, High queue available Tx page size excludes the public queue. Otherwise, High queue available Tx page size includes public queue */
#define BIT_MASK_PUBQ                  ((u32)0x000000FF << 16)          /*!<R/W 0  Public Reserved Page Number */
#define BIT_PUBQ(x)                    ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PUBQ(x)                ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_LPQ                   ((u32)0x000000FF << 8)          /*!<R/W 0  Low Priority Queue Reserved Page Number */
#define BIT_LPQ(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_LPQ(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_HPQ                   ((u32)0x000000FF << 0)          /*!<R/W 0  High Priority Queue Reserved Page Number */
#define BIT_HPQ(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HPQ(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FIFOPAGE_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXPKTNUM              ((u32)0x000000FF << 24)          /*!<R 0  Packet number in TXPKTBUF (Normal/Low) */
#define BIT_TXPKTNUM(x)                ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_TXPKTNUM(x)            ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_PUBQ_AVAL_PG          ((u32)0x000000FF << 16)          /*!<R 0  Available Public Queue Page Number */
#define BIT_PUBQ_AVAL_PG(x)            ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PUBQ_AVAL_PG(x)        ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_LPQ_AVAL_PG           ((u32)0x000000FF << 8)          /*!<R 0  Available Low Priority Queue Page Number */
#define BIT_LPQ_AVAL_PG(x)             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_LPQ_AVAL_PG(x)         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_HPQ_AVAL_PG           ((u32)0x000000FF << 0)          /*!<R 0  Available High Priority Queue Page Number */
#define BIT_HPQ_AVAL_PG(x)             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HPQ_AVAL_PG(x)         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DWBCN0_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LLT_FREE_PAGE         ((u32)0x000000FF << 24)          /*!<R 0  RSVD */
#define BIT_LLT_FREE_PAGE(x)           ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_LLT_FREE_PAGE(x)       ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_RSVD_BYTE1            ((u32)0x0000007F << 17)          /*!<R/W 0  RSVD */
#define BIT_RSVD_BYTE1(x)              ((u32)(((x) & 0x0000007F) << 17))
#define BIT_GET_RSVD_BYTE1(x)          ((u32)(((x >> 17) & 0x0000007F)))
#define BIT_BCN_VALID                  ((u32)0x00000001 << 16)          /*!<R/W1C 0  When this bit is set, the beacon packet has finished to write to txpktbuffer. Clear by SW */
#define BIT_MASK_BCN_HEAD              ((u32)0x000000FF << 8)          /*!<R/W 0xF6  The head page of packet of Bcnq which is Tx DMA filled Bcnq packet from */
#define BIT_BCN_HEAD(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BCN_HEAD(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_BLK_DESC_NUM          ((u32)0x0000000F << 4)          /*!<R/W 1  The Most Number of Tx Descriptor per Bulk Out Only for USB */
#define BIT_BLK_DESC_NUM(x)            ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_BLK_DESC_NUM(x)        ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_BLK_DESC_OPT               ((u32)0x00000001 << 0)          /*!<R/W 0  option to select usb descriptor number(according to 0x208[7:4]) 1 : usb_desc_num = 2 * r_blk_desc_num 0 : usb_desc_num = 4 * r_blk_desc_num */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXDMA_OFFSET_CHK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EN_CHKERR_FINISH           ((u32)0x00000001 << 31)          /*!<R/W 0  Let checksum offload statemachine finish if checksum parser error */
#define BIT_EN_DMA_PERMU_P             ((u32)0x00000001 << 30)          /*!<R/W 0  Enable every dma latch fifo ptr for pcie as usb mode as one pkt trans in permu */
#define BIT_EN_TXQUE_CLR               ((u32)0x00000001 << 29)          /*!<R/W 0  Enable tx queue clear tx pkt in tde in usb mode */
#define BIT_EN_PCIE_FIFO               ((u32)0x00000001 << 28)          /*!<R/W 0  Enable pcie fifo store forward txdesc as before mode */
#define BIT_MASK_PG_UNDER_TH           ((u32)0x000000FF << 16)          /*!<R/W 0xFD  Page threshold value is check by Tx DMA engine. */
#define BIT_PG_UNDER_TH(x)             ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PG_UNDER_TH(x)         ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_DSC_CHKSUM_EN              ((u32)0x00000001 << 13)          /*!<R/W 0  Enable SDIO checksum */
#define BIT_RST_RDPTR                  ((u32)0x00000001 << 12)          /*!<R/W 0  Reset the read pointer of sync FIFO */
#define BIT_RST_WRPTR                  ((u32)0x00000001 << 11)          /*!<R/W 0  Reset the write pointer of sync FIFO */
#define BIT_CHK_PG_TH_EN               ((u32)0x00000001 << 10)          /*!<R/W 0  Enable Tx DMA to check total pages if it is under page threshold */
#define BIT_DROP_DATA_EN               ((u32)0x00000001 << 9)          /*!<R/W 0  Enable Tx DMA to drop the redundant data of packet */
#define BIT_CHECK_OFFSET_EN            ((u32)0x00000001 << 8)          /*!<R/W 0  Enable Tx DMA to check offset value */
#define BIT_MASK_CHK_OFFSET            ((u32)0x000000FF << 0)          /*!<R/W 0  Offset value is check by Tx DMA engine */
#define BIT_CHK_OFFSET(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_CHK_OFFSET(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXDMA_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_HI_OQT_UDN                 ((u32)0x00000001 << 17)          /*!<R/W1C 0  High Priority Queue_Tx OQT underflow */
#define BIT_HI_OQT_OVF                 ((u32)0x00000001 << 16)          /*!<R/W1C 0  High Priority Queue_Tx OQT overflow */
#define BIT_PAYLOAD_CHKSUM_ERR         ((u32)0x00000001 << 15)          /*!<R/W1C 0  Payload checksum error */
#define BIT_PAYLOAD_UDN                ((u32)0x00000001 << 14)          /*!<R/W1C 0  Payload is under the packet length of Tx descriptor. Clear by SW. */
#define BIT_PAYLOAD_OVF                ((u32)0x00000001 << 13)          /*!<R/W1C 0  Payload is over the packet length of Tx descriptor. Clear by SW. */
#define BIT_DSC_CHKSUM_FAIL            ((u32)0x00000001 << 12)          /*!<R/W1C 0  Tx descriptor checksum error. Clear by SW. */
#define BIT_UNKNOWN_QSEL               ((u32)0x00000001 << 11)          /*!<R/W1C 0  An unknown QSEL of Tx descriptor is detected. Clear by SW. */
#define BIT_EP_QSEL_DIFF               ((u32)0x00000001 << 10)          /*!<R/W1C 0  Tx Endpoint is unmatched with the QSEL of descriptor. Clear by SW. */
#define BIT_TX_OFFS_UNMATCH            ((u32)0x00000001 << 9)          /*!<R/W1C 0  Tx offset is unmatched. Clear by SW. */
#define BIT_TXOQT_UDN                  ((u32)0x00000001 << 8)          /*!<R/W1C 0  TXOQT Underflow. Clear by SW. */
#define BIT_TXOQT_OVF                  ((u32)0x00000001 << 7)          /*!<R/W1C 0  TXOQT Overflow. Clear by SW. */
#define BIT_SFF_UDN                    ((u32)0x00000001 << 6)          /*!<R/W1C 0  TXDMA Sync FIFO Underflow. Clear by SW. */
#define BIT_SFF_OVF                    ((u32)0x00000001 << 5)          /*!<R/W1C 0  TXDMA Sync FIFO Overflow. Clear by SW. */
#define BIT_FREE_PG_FF                 ((u32)0x00000001 << 4)          /*!<R/W1C 0  TXDMA reads NULL page. Clear by SW. */
#define BIT_PAGE_UNDER                 ((u32)0x00000001 << 3)          /*!<R/W1C 0  Total pages included PTCL un-return pages are under the total reserved pages minus the page threshold. Clear by SW. */
#define BIT_PAGE_OVER                  ((u32)0x00000001 << 2)          /*!<R/W1C 0  Total pages included PTCL un-return pages are over the total reserved pages. Clear by SW. */
#define BIT_PG_UDN                     ((u32)0x00000001 << 1)          /*!<R/W1C 0  TXFF page underflow in TDE page controller. Clear by SW. */
#define BIT_PG_OVF                     ((u32)0x00000001 << 0)          /*!<R/W1C 0  TXFF page overflow in TDE page controller.. Clear by SW. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RQPN_NPQ
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EPQ_PGNUM             ((u32)0x000000FF << 24)          /*!<R 0  Extra Queue Page Number */
#define BIT_EPQ_PGNUM(x)               ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_EPQ_PGNUM(x)           ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_EPQ_R_PGNUM           ((u32)0x000000FF << 16)          /*!<R/W 0  Extra Queue Reserved Page Number */
#define BIT_EPQ_R_PGNUM(x)             ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_EPQ_R_PGNUM(x)         ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_NPQ_PGNUM             ((u32)0x000000FF << 8)          /*!<R 0  Normal Priority Queue Page Number */
#define BIT_NPQ_PGNUM(x)               ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_NPQ_PGNUM(x)           ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_NPQ_R_PGNUM           ((u32)0x000000FF << 0)          /*!<R/W 0  Normal Priority Queue Reserved Page Number */
#define BIT_NPQ_R_PGNUM(x)             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_NPQ_R_PGNUM(x)         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TQPNT1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_NPQ_HIGH_TH           ((u32)0x000000FF << 24)          /*!<R/W 0  For SDIO, Normal Priority Queue Page Number High Threshold */
#define BIT_NPQ_HIGH_TH(x)             ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_NPQ_HIGH_TH(x)         ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_NPQ_LOW_TH            ((u32)0x000000FF << 16)          /*!<R/W 0  For SDIO, Normal Priority Queue Page Number Low Threshold */
#define BIT_NPQ_LOW_TH(x)              ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_NPQ_LOW_TH(x)          ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_HPQ_HIGH_TH           ((u32)0x000000FF << 8)          /*!<R/W 0  For SDIO, High Priority Queue Page Number High Threshold */
#define BIT_HPQ_HIGH_TH(x)             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_HPQ_HIGH_TH(x)         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_HPQ_LOW_TH            ((u32)0x000000FF << 0)          /*!<R/W 0  For SDIO, High Priority Queue Page Number Low Threshold */
#define BIT_HPQ_LOW_TH(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HPQ_LOW_TH(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TQPNT2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EXQ_HIGH_TH           ((u32)0x000000FF << 24)          /*!<R/W 0  For SDIO, Extra Priority Queue Page Number High Threshold */
#define BIT_EXQ_HIGH_TH(x)             ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_EXQ_HIGH_TH(x)         ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_EXQ_LOW_TH            ((u32)0x000000FF << 16)          /*!<R/W 0  For SDIO, Extra Priority Queue Page Number Low Threshold */
#define BIT_EXQ_LOW_TH(x)              ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_EXQ_LOW_TH(x)          ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_LPQ_HIGH_TH           ((u32)0x000000FF << 8)          /*!<R/W 0  For SDIO, Low Priority Queue Page Number High Threshold */
#define BIT_LPQ_HIGH_TH(x)             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_LPQ_HIGH_TH(x)         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_LPQ_LOW_TH            ((u32)0x000000FF << 0)          /*!<R/W 0  For SDIO, Low Priority Queue Page Number Low Threshold */
#define BIT_LPQ_LOW_TH(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_LPQ_LOW_TH(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TDE_DEBUG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TDE_DEBUG             ((u32)0xFFFFFFFF << 0)          /*!<R 0  tde debug port register. */
#define BIT_TDE_DEBUG(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TDE_DEBUG(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AUTO_LLT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXPKTNUM_H            ((u32)0x000000FF << 24)          /*!<R 0  Packet number in TXPKTBUF (High and Extra) */
#define BIT_TXPKTNUM_H(x)              ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_TXPKTNUM_H(x)          ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_TDE_DBG_SEL                ((u32)0x00000001 << 23)          /*!<R/W 0  debug signals selection of Tx DMA */
#define BIT_MASK_QSEL_DIFF             ((u32)0x00000001 << 22)          /*!<R/W 0  Mask the error bit(tx endpoint is unmatched with qsel, 0x210[10]) */
#define BIT_AUTO_INIT_LLT              ((u32)0x00000001 << 16)          /*!<R/W 0  Write 1 to enable HW init LLT, driver need polling to 0 meaning init success */
#define BIT_MASK_TX_OQT_HE_FREE_SPACE  ((u32)0x000000FF << 8)          /*!<R 0  High and Extra priority queue */
#define BIT_TX_OQT_HE_FREE_SPACE(x)    ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_TX_OQT_HE_FREE_SPACE(x) ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_TX_OQT_NL_FREE_SPACE  ((u32)0x000000FF << 0)          /*!<R 0  Normal and low priority queue */
#define BIT_TX_OQT_NL_FREE_SPACE(x)    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_TX_OQT_NL_FREE_SPACE(x) ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DWBCN1_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCN_HEAD_2            ((u32)0x000000FF << 24)          /*!<R/W 0xFA  The head page of packet of Bcnq which is Tx DMA filled Bcnq packet from. */
#define BIT_BCN_HEAD_2(x)              ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_BCN_HEAD_2(x)          ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_SW_BCN_SEL            ((u32)0x00000003 << 20)          /*!<R/W 0  Bcnq head selection from SW setting 2'b01 : r_bcn_head_1 2'b10 : r_bcn_head_2 */
#define BIT_SW_BCN_SEL(x)              ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_SW_BCN_SEL(x)          ((u32)(((x >> 20) & 0x00000003)))
#define BIT_BCN_VALID_2                ((u32)0x00000001 << 18)          /*!<R/W 0  When this bit is set, the beacon packet has finished to write to txpktbuffer. Clear by SW. */
#define BIT_SW_BCN_SEL_EN              ((u32)0x00000001 << 17)          /*!<R/W 0  Enable Bcnq head selection from SW setting */
#define BIT_BCN_VALID_1                ((u32)0x00000001 << 16)          /*!<R/W 0  When this bit is set, the beacon packet has finished to write to txpktbuffer. Clear by SW. */
#define BIT_MASK_BCN_HEAD_1            ((u32)0x000000FF << 8)          /*!<R/W 0xF8  The head page of packet of Bcnq which is Tx DMA filled Bcnq packet from. */
#define BIT_BCN_HEAD_1(x)              ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BCN_HEAD_1(x)          ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_MAX_TX_PKT            ((u32)0x000000FF << 0)          /*!<R/W 0  Unit : packet number Max_tx_pkt_for_USB_and_SDIO */
#define BIT_MAX_TX_PKT(x)              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_MAX_TX_PKT(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TX_ALIGNMENT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_ADJUSTABLE_SIZE_EN         ((u32)0x00000001 << 15)          /*!<R/W 0  Enable tx alignment for sdio. SW can assign the alignment size of blocks in an aggregated-packet. */
#define BIT_MASK_ALIGNMENT_SIZE        ((u32)0x00000FFF << 0)          /*!<R/W 0  The alignment size of blocks in an aggregated-packet. Unit: Byte */
#define BIT_ALIGNMENT_SIZE(x)          ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_ALIGNMENT_SIZE(x)      ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TDE_GCK_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TDE_GCLK_DLY_CNT      ((u32)0x0000001F << 2)          /*!<R/W 0xF  when start trigger of clk gating happens, count for several cycles then disable clk,default:16 cycles */
#define BIT_TDE_GCLK_DLY_CNT(x)        ((u32)(((x) & 0x0000001F) << 2))
#define BIT_GET_TDE_GCLK_DLY_CNT(x)    ((u32)(((x >> 2) & 0x0000001F)))
#define BIT_DMA_WAIT_MORE_CYCLE        ((u32)0x00000001 << 1)          /*!<R/W 1  enable this bit,tde_ctl will wait for 16 dma_clk when host dma last data to tde_sync_fifo 1:used for 20M mac clk 0:used for 40M/80M mac clk(will wait for 12 dma_clk) */
#define BIT_TDE_GCLK_EN                ((u32)0x00000001 << 0)          /*!<R/W 1  enable this bit,txdma engine will use gated clk */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXDMA_AGG_PG_TH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXDMA_STORE                ((u32)0x00000001 << 31)          /*!<R/W 0  usb rx dma old mode store pkt and forward */
#define BIT_EN_PRE_CALC                ((u32)0x00000001 << 29)          /*!<R/W 0  0: rde start rxdma when aggregated length over specified threshold; 1: rde start rxdma when next packet, if aggregated, will cause the total length over specified threshold */
#define BIT_MASK_PKT_NUM_WOL           ((u32)0x000000FF << 16)          /*!<R 0  In wol mode, 8051 should refer this packet counter in RXFF */
#define BIT_PKT_NUM_WOL(x)             ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_PKT_NUM_WOL(x)         ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_DMA_TIMEOUT_TH        ((u32)0x000000FF << 8)          /*!<R/W 0x20  dma agg timeout time : unit : 32us */
#define BIT_DMA_TIMEOUT_TH(x)          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_DMA_TIMEOUT_TH(x)      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_RXDMA_AGG_PG_TH       ((u32)0x000000FF << 0)          /*!<R/W 0x3  rxdma agg length threshold if sdio : unit 1K, others unit is 4K if use RXDMA_AGG_OLD_MOD in use, unit is 1K */
#define BIT_RXDMA_AGG_PG_TH(x)         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RXDMA_AGG_PG_TH(x)     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXPKT_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXPKT_QUEUED          ((u32)0x000000FF << 24)          /*!<R 0  The number of packets in RXPKTBUF. */
#define BIT_RXPKT_QUEUED(x)            ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_RXPKT_QUEUED(x)        ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_RXDMA_REQ                  ((u32)0x00000001 << 19)          /*!<R 0  rxdma has request */
#define BIT_FW_RELEASE_EN              ((u32)0x00000001 << 18)          /*!<R/W 0  When this bit is set, RXDMA will enter this mode after on-going RXDMA packet to host completed, and stop DMA packet to host. RXDMA will then report */
#define BIT_RXDMA_IDLE                 ((u32)0x00000001 << 17)          /*!<R 0  When RXMA finishes on-going DMA operation, RXMDA will report idle state in this bit. FW can start releasing packets after RXDMA entering idle mode. */
#define BIT_RXPKT_RELEASE_POLL         ((u32)0x00000001 << 16)          /*!<R/W1C 0  When this bit is set, RXDMA will decrease RX PKT counter by one. Before this bit is polled, FW shall update RXFF_RD_PTR first. */
#define BIT_MASK_FW_UPD_RXPKT_RDPTR    ((u32)0x0000FFFF << 0)          /*!<R/W 0  FW shall update this register before FW write RXPKT_RELEASE_POLL to 1. */
#define BIT_FW_UPD_RXPKT_RDPTR(x)      ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_FW_UPD_RXPKT_RDPTR(x)  ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXDMA_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_C2H_PKT_OVF                ((u32)0x00000001 << 7)          /*!<R/W1C 0  CH2 pkt overflow. Clr by SW */
#define BIT_AGG_CONFGI_ISSUE           ((u32)0x00000001 << 6)          /*!<R/W1C 0  rxdma agg config error. Clr by SW */
#define BIT_FW_POLL_ISSUE              ((u32)0x00000001 << 5)          /*!<R/W1C 0  FW Release Poll Error. Clear by SW. */
#define BIT_RX_DATA_UDN                ((u32)0x00000001 << 4)          /*!<R/W1C 0  FW Release Poll Error. Clear by SW. */
#define BIT_RX_SFF_UDN                 ((u32)0x00000001 << 3)          /*!<R/W1C 0  RX Sync FIFO Underflow. Clear by SW. */
#define BIT_RX_SFF_OVF                 ((u32)0x00000001 << 2)          /*!<R/W1C 0  RX Sync FIFO Overflow. Clear by SW. */
#define BIT_RXPKT_OVF                  ((u32)0x00000001 << 0)          /*!<R/W1C 0  When RX Packet is more than 255 packets remaining in FF. Clear by SW. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXDMA_DPR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RDE_DBG               ((u32)0xFFFFFFFF << 0)          /*!<R 0  rde debug port register. */
#define BIT_RDE_DBG(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_RDE_DBG(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXDMA_MODE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXDMA_GCLK_EN              ((u32)0x00000001 << 6)          /*!<R/W 1  rxdma_engine clock gating control,1：enable clock gating；0：disable clock gating */
#define BIT_MASK_BURST_SIZE            ((u32)0x00000003 << 4)          /*!<R/W 0  Every burst pkt len 2’b00 : 1KB 2’b01 : 512B 2’b10 : 64B Others : 64B P.S. USB 3.0 must set : 0x00 USB 2.0 High-speed must set : 0x01 USB 2.0 Full-speed must set : 0x10 */
#define BIT_BURST_SIZE(x)              ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_BURST_SIZE(x)          ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_BURST_CNT             ((u32)0x00000003 << 2)          /*!<R/W 1  Burst number in dma mode, support at most 4burst(2’b11) P.S, DD suggest 0x11 */
#define BIT_BURST_CNT(x)               ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_BURST_CNT(x)           ((u32)(((x >> 2) & 0x00000003)))
#define BIT_DMA_MODE                   ((u32)0x00000001 << 1)          /*!<R/W 0  For usb use dma mode; for pcie clr this bit ; for SDIO use Rx DMA Aggregation mode */
#define BIT_MASK_RX_PKT_PS             ((u32)0x00000001 << 0)          /*!<R/W 0  RSVD */
/** @} */

/**************************************************************************//**
 * @defgroup REG_C2H_PKT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_C2H_PKT_REQ              ((u32)0x00000001 << 16)          /*!<R/W 0  C2h packet dma request */
#define BIT_MASK_R_C2H_STR_ADDR        ((u32)0x0000FFFF << 0)          /*!<R/W 0  C2h packet start address */
#define BIT_R_C2H_STR_ADDR(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_R_C2H_STR_ADDR(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */


/*==========PAGE2 Register Address Definition==========*/
#define REG_RQPN_CTRL_HLPQ                           0x0200
#define REG_FIFOPAGE_INFO                            0x0204
#define REG_DWBCN0_CTRL                              0x0208
#define REG_TXDMA_OFFSET_CHK                         0x020C
#define REG_TXDMA_STATUS                             0x0210
#define REG_RQPN_NPQ                                 0x0214
#define REG_TQPNT1                                   0x0218
#define REG_TQPNT2                                   0x021C
#define REG_TDE_DEBUG                                0x0220
#define REG_AUTO_LLT                                 0x0224
#define REG_DWBCN1_CTRL                              0x0228
#define REG_TX_ALIGNMENT_CTRL                        0x022C
#define REG_TDE_GCK_CTRL                             0x0230
#define REG_TQPNT3_V1                                0x0234
#define REG_                                         0x0238
#define REG_                                         0x023C
#define REG_                                         0x0240
#define REG_H2C_HEAD                                 0x0244
#define REG_                                         0x0248
#define REG_                                         0x024C
#define REG_                                         0x0250
#define REG_                                         0x0254
#define REG_                                         0x0258
#define REG_                                         0x025C
#define REG_                                         0x0260
#define REG_                                         0x0264
#define REG_                                         0x0268
#define REG_                                         0x026C
#define REG_                                         0x0270
#define REG_                                         0x0274
#define REG_                                         0x0278
#define REG_                                         0x027C
#define REG_RXDMA_AGG_PG_TH                          0x0280
#define REG_RXPKT_NUM                                0x0284
#define REG_RXDMA_STATUS                             0x0288
#define REG_RXDMA_DPR                                0x028C
#define REG_RXDMA_MODE                               0x0290
#define REG_C2H_PKT                                  0x0294
#define REG_                                         0x0298
#define REG_                                         0x029C
#define REG_                                         0x02A0
#define REG_                                         0x02A4
#define REG_                                         0x02A8
#define REG_                                         0x02AC
#define REG_                                         0x02B0
#define REG_                                         0x02B4
#define REG_                                         0x02B8
#define REG_                                         0x02BC
#define REG_                                         0x02C0
#define REG_                                         0x02C4
#define REG_                                         0x02C6
#define REG_                                         0x02C8
#define REG_                                         0x02CC
#define REG_                                         0x02D0
#define REG_                                         0x02D4
#define REG_                                         0x02D8
#define REG_                                         0x02DC
#define REG_                                         0x02E0
#define REG_                                         0x02E2
#define REG_                                         0x02E4
#define REG_                                         0x02E8
#define REG_                                         0x02EC
#define REG_                                         0x02F0
#define REG_                                         0x02F4
#define REG_                                         0x02F8
#define REG_                                         0x02FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
