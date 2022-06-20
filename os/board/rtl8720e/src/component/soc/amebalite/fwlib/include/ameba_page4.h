#ifndef _AMEBAD2_PAGE4_H_
#define _AMEBAD2_PAGE4_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_Q0_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q0_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q0_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q0_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q0_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 0. */
#define BIT_QUEUEAC_Q0_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q0_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q0_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 0. */
#define BIT_TAIL_PKT_Q0_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q0_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q0_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 0 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q0_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q0_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q0                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 0. */
#define BIT_HEAD_PKT_Q0(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q0(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_Q1_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q1_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q1_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q1_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q1_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 1. */
#define BIT_QUEUEAC_Q1_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q1_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q1_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 1. */
#define BIT_TAIL_PKT_Q1_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q1_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q1_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 1 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q1_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q1_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q1                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 1. */
#define BIT_HEAD_PKT_Q1(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q1(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_Q2_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q2_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q2_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q2_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q2_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 2. */
#define BIT_QUEUEAC_Q2_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q2_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q2_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 2. */
#define BIT_TAIL_PKT_Q2_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q2_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q2_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 2 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q2_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q2_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q2                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 2. */
#define BIT_HEAD_PKT_Q2(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q2(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_Q3_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q3_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q3_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q3_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q3_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 3. */
#define BIT_QUEUEAC_Q3_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q3_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q3_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 3. */
#define BIT_TAIL_PKT_Q3_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q3_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q3_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 3 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q3_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q3_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q3                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 3. */
#define BIT_HEAD_PKT_Q3(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q3(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MGQ_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_MGQ_V1                     ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_MGQ_V1(x)                       ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_MGQ_V1(x)                   ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_MGQ_V1                        ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of management Queue. */
#define BIT_QUEUEAC_MGQ_V1(x)                          ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_MGQ_V1(x)                      ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_MGQ_V1                       ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of MGQ. */
#define BIT_TAIL_PKT_MGQ_V1(x)                         ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_MGQ_V1(x)                     ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_MGQ_V1                        ((u32)0x0000007F << 8)          /*!<R 0x0  The total MGQ pkt number in TxPktbuffer */
#define BIT_PKT_NUM_MGQ_V1(x)                          ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_MGQ_V1(x)                      ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_MGQ                          ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of MGQ. */
#define BIT_HEAD_PKT_MGQ(x)                            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_MGQ(x)                        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HIQ_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_HIQ_V1                     ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_HIQ_V1(x)                       ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_HIQ_V1(x)                   ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_HIQ_V1                        ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of High Queue. */
#define BIT_QUEUEAC_HIQ_V1(x)                          ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_HIQ_V1(x)                      ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_HIQ_V1                       ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of HIQ. */
#define BIT_TAIL_PKT_HIQ_V1(x)                         ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_HIQ_V1(x)                     ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_HIQ_V1                        ((u32)0x0000007F << 8)          /*!<R 0x0  The total HIQ pkt number in TxPktbuffer */
#define BIT_PKT_NUM_HIQ_V1(x)                          ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_HIQ_V1(x)                      ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_HIQ                          ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of HIQ. */
#define BIT_HEAD_PKT_HIQ(x)                            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_HIQ(x)                        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_QUE_EMPTY_BCNQ_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BCNQ_EMPTY_P2                              ((u32)0x00000001 << 31)          /*!<R 0x1  When this bit is raised, beacon queue of PORT2 is empty. Report by MAC. */
#define BIT_BCNQ_EMPTY_P1                              ((u32)0x00000001 << 30)          /*!<R 0x1  When this bit is raised, beacon queue of PORT1 is empty. Report by MAC. */
#define BIT_BCNQ_EMPTY_P0                              ((u32)0x00000001 << 29)          /*!<R 0x1  When this bit is raised, beacon queue of PORT0 is empty. Report by MAC. */
#define BIT_EVTQ_EMPTY                                 ((u32)0x00000001 << 28)          /*!<R 0x1  When this bit is raised, event queue is empty. Report by MAC. */
#define BIT_BCNQ_EMPTY                                 ((u32)0x00000001 << 27)          /*!<R 0x1  When this bit is raised, beacon queue is empty. Report by MAC. */
#define BIT_HQQ_EMPTY                                  ((u32)0x00000001 << 26)          /*!<R 0x1  When this bit is raised, High queue is empty. Report by MAC. */
#define BIT_MQQ_EMPTY                                  ((u32)0x00000001 << 25)          /*!<R 0x1  When this bit is raised, MGQ queue is empty. Report by MAC. */
#define BIT_MGQ_CPU_EMPTY                              ((u32)0x00000001 << 24)          /*!<R 0x1  When this bit is raised, MGQ queue poll by cpu is empty. Report by MAC. */
#define BIT_AC7Q_EMPTY                                 ((u32)0x00000001 << 23)          /*!<R 0x1  When this bit is raised, AC7 queue is empty. Report by MAC. */
#define BIT_AC6Q_EMPTY                                 ((u32)0x00000001 << 22)          /*!<R 0x1  When this bit is raised, AC6 queue is empty. Report by MAC. */
#define BIT_AC5Q_EMPTY                                 ((u32)0x00000001 << 21)          /*!<R 0x1  When this bit is raised, AC5 queue is empty. Report by MAC. */
#define BIT_AC4Q_EMPTY                                 ((u32)0x00000001 << 20)          /*!<R 0x1  When this bit is raised, AC4 queue is empty. Report by MAC. */
#define BIT_AC3Q_EMPTY                                 ((u32)0x00000001 << 19)          /*!<R 0x1  When this bit is raised, AC3 queue is empty. Report by MAC. */
#define BIT_AC2Q_EMPTY                                 ((u32)0x00000001 << 18)          /*!<R 0x1  When this bit is raised, AC2 queue is empty. Report by MAC. */
#define BIT_AC1Q_EMPTY                                 ((u32)0x00000001 << 17)          /*!<R 0x1  When this bit is raised, AC1 queue is empty. Report by MAC. */
#define BIT_AC0Q_EMPTY                                 ((u32)0x00000001 << 16)          /*!<R 0x1  When this bit is raised, AC0 queue is empty. Report by MAC. */
#define BIT_MASK_PKT_NUM_BCNQ                          ((u32)0x000000FF << 8)          /*!<R 0x0  The total BcnQ pkt number in TxPktbuffer */
#define BIT_PKT_NUM_BCNQ(x)                            ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_PKT_NUM_BCNQ(x)                        ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_BCNQ_HEAD_PG                          ((u32)0x000000FF << 0)          /*!<R 0xF6  The head page of the first packet of Bcnq */
#define BIT_BCNQ_HEAD_PG(x)                            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_BCNQ_HEAD_PG(x)                        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CPU_MGQ_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BCN_POLL2                                  ((u32)0x00000001 << 31)          /*!<W   Polling BCN Queue2 Packet. Not used in 8723F */
#define BIT_BCN_POLL1                                  ((u32)0x00000001 << 30)          /*!<W   Polling BCN Queue1 Packet */
#define BIT_CPUMGT_POLL_SET                            ((u32)0x00000001 << 29)          /*!<W   Polling to set CPU MGT Queue packet */
#define BIT_BCN_POLL                                   ((u32)0x00000001 << 28)          /*!<W   Polling BCN Queue Packet */
#define BIT_CPUMGT_POLL_CLR                            ((u32)0x00000001 << 27)          /*!<W   Polling to clear CPU MGT Queue packet */
#define BIT_EVTQ_VLD                                   ((u32)0x00000001 << 26)          /*!<R/W 0x0  SW set to 1, HW clear to 0 when evtq timeout or tx_finish */
#define BIT_CMDQ_POLL                                  ((u32)0x00000001 << 24)          /*!<W   polling CMD packet */
#define BIT_CPUMGQ_FW_NUM                              ((u32)0x00000001 << 8)          /*!<R 0x0  The total cpu mgq fw number in TxPktbuffer */
#define BIT_MASK_CPUMGQ_HEAD_PG                        ((u32)0x000000FF << 0)          /*!<R 0xF6  The head page of CPU MGQ */
#define BIT_CPUMGQ_HEAD_PG(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_CPUMGQ_HEAD_PG(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FWHW_TXQ_HWSSN_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HW_SSN_EN                             ((u32)0x000000FF << 24)          /*!<R/W 0x0  HW update the SSN [7:0], When this bit enable and EN_HWSEQ of TXDESC, MAC will overwrite MACHDR SEQ according to TXDESC QSEL [0] BIT_HWSEQ_VO_EN [1] BIT_HWSEQ_VI_EN [2] BIT_HWSEQ_BE_EN [3] BIT_HWSEQ_BK_EN [4] BIT_HWSEQ_MGT_EN [5] BIT_HWSEQ_HI_EN [6] BIT_HWSEQ_BCN_EN,don't care EN_HWSEQ of TXDESC [7] BIT_HWSEQ_CPUM_EN */
#define BIT_HW_SSN_EN(x)                               ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_HW_SSN_EN(x)                           ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_RTS_LIMIT_IN_OFDM                          ((u32)0x00000001 << 23)          /*!<R/W 0x0  Rts rate will not down to cck rate */
#define BIT_EN_BCNQ_DL                                 ((u32)0x00000001 << 22)          /*!<R/W 0x1  1: ptcl consider there is bcn pkt in TXBUF when bcn pkt first be transferd in; 0: mask bcn pkt num in bcn queue */
#define BIT_EN_RD_RESP_NAV_BK                          ((u32)0x00000001 << 21)          /*!<R/W 0x1  Control the RD responder NAV criteria for TX. When this bit is clear, RD responder may aggregates only according to the number of MPDUs existing in the TXPKTBUF, the limit of aggregation size and number, and also the break time. When this bit is set, RD responder will be under the RX NAV criteria for RD response sequence. No excess NAV is allowed */
#define BIT_EN_WR_FREE_TAIL                            ((u32)0x00000001 << 20)          /*!<R/W 0x1  1: Drive write Bcnq_boundary, Hw will set free_tail in Boundary-1 0: no update */
#define BIT_TXRPT_DIS                                  ((u32)0x00000001 << 19)          /*!<R/W 0x0  when this bit is enabled, HW will not generate txrpt for driver's fixed rate packets */
#define BIT_TXFAILRPT_DIS                              ((u32)0x00000001 << 18)          /*!<R/W 0x1  when this bit is enabled, HW will not generate txrpt for tx failed packets */
#define BIT_EN_RTY_BC                                  ((u32)0x00000001 << 17)          /*!<R/W 0x0  When this bit is enabled, retry the broadcast packet */
#define BIT_EN_RTY_BK_COND                             ((u32)0x00000001 << 16)          /*!<R/W 0x0  AMPDU aggregation mode with retry MPDUs and new MPDUs. When this bit is enabled, if new MPDU's data rate is not the same as retry MPDUs' fallback data rate, the aggregation will be broken. */
#define BIT_MASK_EN_QUEUE_RPT                          ((u32)0x000000FF << 8)          /*!<R/W 0x3F  8723A/8812 1: HW will report tx_status 0: no report BIT0: BK Queue BIT1: BE Queue BIT2: VI Queue BIT3: VO Queue BIT4: MGT Queue BIT5: HI Queue BIT6: BCN Queue BIT7: CPU_MGT Queue */
#define BIT_EN_QUEUE_RPT(x)                            ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_EN_QUEUE_RPT(x)                        ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_EN_RTY_BK                                  ((u32)0x00000001 << 7)          /*!<R/W 0x1  AMPDU aggregation mode with retry MPDUs and new MPDUs. When this bit is clear enabled, an AMPDU aggregates retried MPDU and new MPDU. All the aggregation criteria is still the same with normal AMPDU operation. */
#define BIT_EN_USE_INI_RAT                             ((u32)0x00000001 << 6)          /*!<R/W 0x0  0: If Txdesc's USE RATE = 0 and this field is equal to 0, the datarate of TXRPT SRAM is used as tx pkt's datarate. (ECO flow) 1: TXInfo's datarate is used as tx pkt's datarate. */
#define BIT_EN_RTS_NAV_BK                              ((u32)0x00000001 << 5)          /*!<R/W 0x0  Control The RTS NAV criteria for TX. When this bit is clear, TX may aggregates only according to the number of MPDUs existing in the TXPKTBUF, the limit of aggregation size and number, and also the break time. When this bit is set, TX will be under the RTS NAV criteria. No excess RTS NAV is allowed. */
#define BIT_DIS_SSN_CHECK                              ((u32)0x00000001 << 4)          /*!<R/W 0x0  Disable checker the SSN rule when generating AMPDU TX cmd. */
#define BIT_MACID_MATCH_RTS                            ((u32)0x00000001 << 3)          /*!<R/W 0x0  Enable the packet that follow the RTS have same MACID */
#define BIT_EN_BCN_TXRPT                               ((u32)0x00000001 << 2)          /*!<R/W 0x1  1: HW will report bcn tx_status 0: no report */
#define BIT_EN_FTM_RPT                                 ((u32)0x00000001 << 1)          /*!<R/W 0x1  When this bit asserted, HW will report FTM rpt */
#define BIT_PROTECT_BMC_NAV                            ((u32)0x00000001 << 0)          /*!<R/W 0x1  Enable the NAV protection when transmit broadcast/multicast */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BNDY_LIFETIME_TAIL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW_FREE_TAIL                          ((u32)0x000000FF << 24)          /*!<R 0xF5  Free Tail Page Number */
#define BIT_FW_FREE_TAIL(x)                            ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_FW_FREE_TAIL(x)                        ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_BT_INT_TYPE                           ((u32)0x00000003 << 22)          /*!<R/W 0x0  BT Interrupt type 01: PTA 10: CPU */
#define BIT_BT_INT_TYPE(x)                             ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_BT_INT_TYPE(x)                         ((u32)(((x >> 22) & 0x00000003)))
#define BIT_HW_SSN_EN_EVTQ                             ((u32)0x00000001 << 21)          /*!<R/W 0x0  HW update the SSN [8] for eventq */
#define BIT_RTYBIT_CTRL_EN                             ((u32)0x00000001 << 20)          /*!<R/W 0x1  1: Enable Retry Count 0: Disable */
#define BIT_LIFETIME_BK_EN                             ((u32)0x00000001 << 19)          /*!<R/W 0x0  Enable BK queue lifetime function */
#define BIT_LIFETIME_BE_EN                             ((u32)0x00000001 << 18)          /*!<R/W 0x0  Enable BE queue lifetime function */
#define BIT_LIFETIME_VI_EN                             ((u32)0x00000001 << 17)          /*!<R/W 0x0  Enable VI queue lifetime function */
#define BIT_LIFETIME_VO_EN                             ((u32)0x00000001 << 16)          /*!<R/W 0x0  Enable VO queue lifetime function */
#define BIT_MASK_MGQ_PGBNDY                            ((u32)0x000000FF << 8)          /*!<R/W 0xF6  TXDMA will pull CPU MGQ Pkt in this address. */
#define BIT_MGQ_PGBNDY(x)                              ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MGQ_PGBNDY(x)                          ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_BCNQ_PGBNDY                           ((u32)0x000000FF << 0)          /*!<R/W 0xF6  TXDMA will pull BCNQ Pkt in this address. */
#define BIT_BCNQ_PGBNDY(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_BCNQ_PGBNDY(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RETRY_LIMIT_SIFS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_SRL                                   ((u32)0x0000003F << 24)          /*!<R/W 0x20  Short Retry Limit */
#define BIT_SRL(x)                                     ((u32)(((x) & 0x0000003F) << 24))
#define BIT_GET_SRL(x)                                 ((u32)(((x >> 24) & 0x0000003F)))
#define BIT_MASK_LRL                                   ((u32)0x0000003F << 16)          /*!<R/W 0x20  Long Retry Limit */
#define BIT_LRL(x)                                     ((u32)(((x) & 0x0000003F) << 16))
#define BIT_GET_LRL(x)                                 ((u32)(((x >> 16) & 0x0000003F)))
#define BIT_MASK_SPEC_SIFS_OFDM_PTCL                   ((u32)0x000000FF << 8)          /*!<R/W 0x10  The spec SIFS value for duration calculation */
#define BIT_SPEC_SIFS_OFDM_PTCL(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_SPEC_SIFS_OFDM_PTCL(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_SPEC_SIFS_CCK_PTCL                    ((u32)0x000000FF << 0)          /*!<R/W 0x0A  The spec SIFS value for duration calculation */
#define BIT_SPEC_SIFS_CCK_PTCL(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_SPEC_SIFS_CCK_PTCL(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXBF_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_ENABLE_NDPA                              ((u32)0x00000001 << 31)          /*!<R/W 0x0  Set ‘1’ for MAC to enable sending NDPA or NDP Announcement MPDU */
#define BIT_USE_NDPA_PARAMETER                         ((u32)0x00000001 << 30)          /*!<R/W 0x0  1: NDPA rate and BW use reg0x45f settings 0: NDPA rate and BW use NDP settings in Tx descriptor */
#define BIT_R_EN_NDPA_INT                              ((u32)0x00000001 << 28)          /*!<R/W 0x0  1: enable ndpa interrupt; 0: disable ndpa interrupt */
#define BIT_R_TXBF1_80M                                ((u32)0x00000001 << 27)          /*!<R/W 0x0  TxBF status of beamformee1 xBW=80M. */
#define BIT_R_TXBF1_40M                                ((u32)0x00000001 << 26)          /*!<R/W 0x0  TxBF status of beamformee1 xBW=40M */
#define BIT_R_TXBF1_20M                                ((u32)0x00000001 << 25)          /*!<R/W 0x0  TxBF status of beamformee1 xBW=20M */
#define BIT_MASK_R_TXBF1_AID                           ((u32)0x000001FF << 16)          /*!<R/W 0x0  P_AID[8:0] of the associated beamformee1 */
#define BIT_R_TXBF1_AID(x)                             ((u32)(((x) & 0x000001FF) << 16))
#define BIT_GET_R_TXBF1_AID(x)                         ((u32)(((x >> 16) & 0x000001FF)))
#define BIT_NDP_BFEN_DIS                               ((u32)0x00000001 << 15)          /*!<R/W 0x1  1: disable beamforming with NDP */
#define BIT_BCN_NOBLOCK_NDP                            ((u32)0x00000001 << 14)          /*!<R/W 0x1  beacon packet will not block NDP packet */
#define BIT_R_TXBF0_80M                                ((u32)0x00000001 << 11)          /*!<R/W 0x0  TxBF status of beamformee0 xBW=80M */
#define BIT_R_TXBF0_40M                                ((u32)0x00000001 << 10)          /*!<R/W 0x0  TxBF status of beamformee0 xBW=40M */
#define BIT_R_TXBF0_20M                                ((u32)0x00000001 << 9)          /*!<R/W 0x0  TxBF status of beamformee0 xBW=20M */
#define BIT_MASK_R_TXBF0_AID                           ((u32)0x000001FF << 0)          /*!<R/W 0x0  P_AID[8:0] of the associated beamformee0 */
#define BIT_R_TXBF0_AID(x)                             ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_TXBF0_AID(x)                         ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DARFRC0_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DATA_FALLBACK_RC4                     ((u32)0x0000001F << 24)          /*!<R/W 0x04  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC4(x)                       ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_DATA_FALLBACK_RC4(x)                   ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK_RC3                     ((u32)0x0000001F << 16)          /*!<R/W 0x03  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC3(x)                       ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_DATA_FALLBACK_RC3(x)                   ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK_RC2                     ((u32)0x0000001F << 8)          /*!<R/W 0x02  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC2(x)                       ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_DATA_FALLBACK_RC2(x)                   ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK_RC1                     ((u32)0x0000001F << 0)          /*!<R/W 0x01  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC1(x)                       ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_DATA_FALLBACK_RC1(x)                   ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DARFRC0_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DATA_FALLBACK_RC8                     ((u32)0x0000001F << 24)          /*!<R/W 0x08  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC8(x)                       ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_DATA_FALLBACK_RC8(x)                   ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK_RC7                     ((u32)0x0000001F << 16)          /*!<R/W 0x07  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC7(x)                       ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_DATA_FALLBACK_RC7(x)                   ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK_RC6                     ((u32)0x0000001F << 8)          /*!<R/W 0x06  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC6(x)                       ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_DATA_FALLBACK_RC6(x)                   ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK_RC5                     ((u32)0x0000001F << 0)          /*!<R/W 0x05  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK_RC5(x)                       ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_DATA_FALLBACK_RC5(x)                   ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RARFRC_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_CTRL_FALLBACK_RC4                     ((u32)0x0000001F << 24)          /*!<R/W 0x04  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC4(x)                       ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_CTRL_FALLBACK_RC4(x)                   ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_CTRL_FALLBACK_RC3                     ((u32)0x0000001F << 16)          /*!<R/W 0x03  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC3(x)                       ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_CTRL_FALLBACK_RC3(x)                   ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_CTRL_FALLBACK_RC2                     ((u32)0x0000001F << 8)          /*!<R/W 0x02  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC2(x)                       ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_CTRL_FALLBACK_RC2(x)                   ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_CTRL_FALLBACK_RC1                     ((u32)0x0000001F << 0)          /*!<R/W 0x01  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC1(x)                       ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_CTRL_FALLBACK_RC1(x)                   ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RARFRC_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_CTRL_FALLBACK_RC8                     ((u32)0x0000001F << 24)          /*!<R/W 0x08  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC8(x)                       ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_CTRL_FALLBACK_RC8(x)                   ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_CTRL_FALLBACK_RC7                     ((u32)0x0000001F << 16)          /*!<R/W 0x07  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC7(x)                       ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_CTRL_FALLBACK_RC7(x)                   ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_CTRL_FALLBACK_RC6                     ((u32)0x0000001F << 8)          /*!<R/W 0x06  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC6(x)                       ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_CTRL_FALLBACK_RC6(x)                   ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_CTRL_FALLBACK_RC5                     ((u32)0x0000001F << 0)          /*!<R/W 0x05  RTS Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_CTRL_FALLBACK_RC5(x)                       ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_CTRL_FALLBACK_RC5(x)                   ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RRSR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RRSR_RSC                              ((u32)0x0000000F << 21)          /*!<R/W 0x3  Response frame sub-channel configuration. Response frames (CTS, ACK, BA) are transmitted in the following condition. 00: Primary 20Mhz BW(0x483[3:0] = 0x8AC[5:2]). 01: Follow Rx packet BW mode. 10: Duplicate mode. */
#define BIT_RRSR_RSC(x)                                ((u32)(((x) & 0x0000000F) << 21))
#define BIT_GET_RRSR_RSC(x)                            ((u32)(((x >> 21) & 0x0000000F)))
#define BIT_MASK_R_RATE_CTRL_TABLE                     ((u32)0x000FFFFF << 0)          /*!<R/W 0x00011  Data Rate Control Table Register BitMap to indicate the response/protection/expected_response rate to use in this BSS. All management and control response frames should obey the rates set in the bitmap. Each bit is mapped as the following: Bit[0:3] = {1M, 2M, 5.5M, 11M } Bit[4:11] = {6M, 9M, 12M, 18M, 24M, 36M, 48M, 54M} Bit[12:19] = HT MCS0~MCS7 Response rate must be less than or equal to the rate of the initiating frame. If none of the bits are set, no response is generated by Rtl8712. If both the legacy rates and HT rates are defined as Basic Rates, fill in only the legacy portion of the bitmap. If only HT rates are defined as Basic Rates, fill in both the legacy and HT portion of the bitmap. */
#define BIT_R_RATE_CTRL_TABLE(x)                       ((u32)(((x) & 0x000FFFFF) << 0))
#define BIT_GET_R_RATE_CTRL_TABLE(x)                   ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCN_AMPDU_CTCL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCNQ1_HEAD                            ((u32)0x000000FF << 24)          /*!<R/W 0xF6  TXDMA will pull BCNQ1 Pkt in this address. Port1 & VAP 1/3/5/7 of port0 */
#define BIT_BCNQ1_HEAD(x)                              ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_BCNQ1_HEAD(x)                          ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_AMPDU_MAX_TIME                        ((u32)0x000000FF << 16)          /*!<R/W 0x64  AMPDU duration limit in the unit of 32us. */
#define BIT_AMPDU_MAX_TIME(x)                          ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_AMPDU_MAX_TIME(x)                      ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_BCNQ2_HEAD                            ((u32)0x000000FF << 8)          /*!<R/W 0xF6  TXDMA will pull BCNQ2 Pkt in this address. Port2, NAN */
#define BIT_BCNQ2_HEAD(x)                              ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BCNQ2_HEAD(x)                          ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_CHECK_CCK_EN                               ((u32)0x00000001 << 7)          /*!<R/W 0x0  If this bit is set, HW check Tx pkt’s data rate. When data rate is CCK rate, modify the rate to NON_CCK_RATE. */
#define BIT_EN_BCN_PKT_REL                             ((u32)0x00000001 << 6)          /*!<R/W 0x0  If this bit is set, the BCN pkt will be set to non-valid after BCN TX. */
#define BIT_BCN_PORT_SEL                               ((u32)0x00000001 << 5)          /*!<R/W 0x0  This bit use to select BCN on port 1 or port 0. This just useful in USB/SDIO interface. */
#define BIT_MOREDATA_BYPASS                            ((u32)0x00000001 << 4)          /*!<R/W 0x0  If this bit is set, the moredata bit will controlled by DES, otherwise controlled by PTCL according TXPKTBUF status. */
#define BIT_EN_CLR_CMD_REL_BCN_PKT                     ((u32)0x00000001 << 3)          /*!<R/W 0x0  If this bit is set, the BCN pkt will be set to non-valid when scheduler clear TX BCN CMD even non-TX. */
#define BIT_EN_SET_MOREDATA                            ((u32)0x00000001 << 2)          /*!<R/W 0x1  1: Enable HW set the more data bit of the PKT that MACID release function tx. 0: Disable HW set the more data bit of the PKT that MACID release function tx. */
#define BIT_DIS_CLEAR_MACID_RELEASE                    ((u32)0x00000001 << 1)          /*!<R/W 0x0  1: HW No clear MACID Release bit after release the pkt. 0: HW will clear MACID Release bit after release the pkt. */
#define BIT_EN_MACID_RELEASE                           ((u32)0x00000001 << 0)          /*!<R/W 0x1  1: Enable MACID Release function. (same as MACID DROP) 0: Disable MACID Release function. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AMPDU_MAX_LENGTH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_AMPDU_MAX_LENGTH                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0xFFFFFFFF  AMPDU length limit */
#define BIT_AMPDU_MAX_LENGTH(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_AMPDU_MAX_LENGTH(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NDPA_CTRL_LBK_ACQ_STOP
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_NDPA_BW                             ((u32)0x00000003 << 24)          /*!<R/W 0x0  NDPA bandwidth setting 3: 160M 2: 80M 1: 40M 0: 20M */
#define BIT_R_NDPA_BW(x)                               ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_R_NDPA_BW(x)                           ((u32)(((x >> 24) & 0x00000003)))
#define BIT_MASK_R_SIGTA_BW                            ((u32)0x00000003 << 22)          /*!<R/W 0x0  Enable signaling TA in duplicate BW, valid if EN_SIGTA = 1 in TX DESC */
#define BIT_R_SIGTA_BW(x)                              ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_R_SIGTA_BW(x)                          ((u32)(((x >> 22) & 0x00000003)))
#define BIT_R_SIGTA_BAR                                ((u32)0x00000001 << 21)          /*!<R/W 0x0  Enable BAR signaling TA in duplicate BW */
#define BIT_R_EOF_EN                                   ((u32)0x00000001 << 20)          /*!<R/W 0x0  when set to 1, enable eof packet qos packet,but also need set agg_en to 0 in tx_desc */
#define BIT_R_EN_GNT_BT_AWAKE                          ((u32)0x00000001 << 19)          /*!<R/W 0x1  Enable pollute bit function.(C card ECO item, modify hardware circuit and add two options) 0: Disable 1: Enable When (GNT_BT option/LTE_RX option/(GNT_WL==0)) == 1, retry count can't increase if Tx fail. One or more packets will record in polluted bit field of txreport when txreport generates in Tx ok or tx drop. */
#define BIT_DIS_RELEASE_RETRY                          ((u32)0x00000001 << 18)          /*!<R/W 0x0  when release one packet for a macid, retry should happen on the same ACQ. Set 1 to remove this requirement */
#define BIT_DIS_OQT_BLOCK                              ((u32)0x00000001 << 17)          /*!<R/W 0x0  If this bit is set, the OQT will not block by all AC queues occupied by other MACID. */
#define BIT_SEARCH_QUEUE_EN                            ((u32)0x00000001 << 16)          /*!<R/W 0x0  Search queue request enable in ptcl idle:0 disable; 1 enable */
#define BIT_MASK_WMAC_LBK_BUF_HEAD                     ((u32)0x000000FF << 8)          /*!<R/W 0xF6  In delay MAC LBK mode, this register indicate the LBK BUF head page. When this buffer size is not enough for LBK packet length, LBK test may fail. In ARP/NS mode, this register indicate the ARP/NS response packet head page. */
#define BIT_WMAC_LBK_BUF_HEAD(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_WMAC_LBK_BUF_HEAD(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_AC7Q_STOP                                  ((u32)0x00000001 << 7)          /*!<R 0x0  When this bit is raised, AC7 queue is stop. Report by MAC. */
#define BIT_AC6Q_STOP                                  ((u32)0x00000001 << 6)          /*!<R 0x0  When this bit is raised, AC6 queue is stop. Report by MAC. */
#define BIT_AC5Q_STOP                                  ((u32)0x00000001 << 5)          /*!<R 0x0  When this bit is raised, AC5 queue is stop. Report by MAC. */
#define BIT_AC4Q_STOP                                  ((u32)0x00000001 << 4)          /*!<R 0x0  When this bit is raised, AC4 queue is stop. Report by MAC. */
#define BIT_AC3Q_STOP                                  ((u32)0x00000001 << 3)          /*!<R 0x0  When this bit is raised, AC3 queue is stop. Report by MAC. */
#define BIT_AC2Q_STOP                                  ((u32)0x00000001 << 2)          /*!<R 0x0  When this bit is raised, AC2 queue is stop. Report by MAC. */
#define BIT_AC1Q_STOP                                  ((u32)0x00000001 << 1)          /*!<R 0x0  When this bit is raised, AC1 queue is stop. Report by MAC. */
#define BIT_AC0Q_STOP                                  ((u32)0x00000001 << 0)          /*!<R 0x0  When this bit is raised, AC0 queue is stop. Report by MAC. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RD_RESP_PKT_TH
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RD_RESP_PKT_TH                        ((u32)0x0000003F << 24)          /*!<R/W 0x03  When RD_CTRL responder enables, and the granted AC queue packet number is beyond this threshold, then responder will carry More PPDU =1 and piggyback data frames back. */
#define BIT_RD_RESP_PKT_TH(x)                          ((u32)(((x) & 0x0000003F) << 24))
#define BIT_GET_RD_RESP_PKT_TH(x)                      ((u32)(((x >> 24) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CMDQ_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PKT_NUM_CMDQ_V2                       ((u32)0x000000FF << 24)          /*!<R 0x0  The total CMD Queue pkt number in TxPktbuffer */
#define BIT_PKT_NUM_CMDQ_V2(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_PKT_NUM_CMDQ_V2(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_TAIL_PKT_CMDQ                         ((u32)0x000000FF << 16)          /*!<R 0xFF  The head page of the last packet of Queue 4. */
#define BIT_TAIL_PKT_CMDQ(x)                           ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_TAIL_PKT_CMDQ(x)                       ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_CMDQ                          ((u32)0x000000FF << 8)          /*!<R 0x0  The total Queue 4 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_CMDQ(x)                            ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_PKT_NUM_CMDQ(x)                        ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_HEAD_PKT_CMDQ                         ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 4. */
#define BIT_HEAD_PKT_CMDQ(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_CMDQ(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_Q4_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q4_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q4_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q4_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q4_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 4. */
#define BIT_QUEUEAC_Q4_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q4_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q4_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 4. */
#define BIT_TAIL_PKT_Q4_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q4_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q4_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 4 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q4_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q4_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q4                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 4. */
#define BIT_HEAD_PKT_Q4(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q4(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_Q5_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q5_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q5_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q5_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q5_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 5. */
#define BIT_QUEUEAC_Q5_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q5_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q5_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 5. */
#define BIT_TAIL_PKT_Q5_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q5_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q5_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 5 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q5_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q5_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q5                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue5. */
#define BIT_HEAD_PKT_Q5(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q5(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_Q6_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q6_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q6_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q6_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q6_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 6. */
#define BIT_QUEUEAC_Q6_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q6_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q6_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 6. */
#define BIT_TAIL_PKT_Q6_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q6_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q6_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 6 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q6_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q6_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q6                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 6. */
#define BIT_HEAD_PKT_Q6(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q6(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_Q7_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_QUEUEMACID_Q7_V1                      ((u32)0x0000007F << 25)          /*!<R 0x0  MacID of head pkt */
#define BIT_QUEUEMACID_Q7_V1(x)                        ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_QUEUEMACID_Q7_V1(x)                    ((u32)(((x >> 25) & 0x0000007F)))
#define BIT_MASK_QUEUEAC_Q7_V1                         ((u32)0x00000003 << 23)          /*!<R 0x0  AC type of Queue 7. */
#define BIT_QUEUEAC_Q7_V1(x)                           ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_QUEUEAC_Q7_V1(x)                       ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_TAIL_PKT_Q7_V1                        ((u32)0x000000FF << 15)          /*!<R 0xFF  The head page of the last packet of Queue 7. */
#define BIT_TAIL_PKT_Q7_V1(x)                          ((u32)(((x) & 0x000000FF) << 15))
#define BIT_GET_TAIL_PKT_Q7_V1(x)                      ((u32)(((x >> 15) & 0x000000FF)))
#define BIT_MASK_PKT_NUM_Q7_V1                         ((u32)0x0000007F << 8)          /*!<R 0x0  The total Queue 7 pkt number in TxPktbuffer */
#define BIT_PKT_NUM_Q7_V1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_PKT_NUM_Q7_V1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_HEAD_PKT_Q7                           ((u32)0x000000FF << 0)          /*!<R 0xFF  The head page of the first packet of Queue 7. */
#define BIT_HEAD_PKT_Q7(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_HEAD_PKT_Q7(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_GCLK_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CLR_TXRPT_RW_PTR                           ((u32)0x00000001 << 2)          /*!<R/W 0x0  Write 1 to clear BIT_AMPDU_TXRPT_W_PTR & BIT_AMPDU_TXRPT_R_PTR (0x47C / 0x47D) */
#define BIT_EN_QUEUE_RPT_B8                            ((u32)0x00000001 << 1)          /*!<R/W 0x1  [8] of BIT_EN_QUEUE_RPT, BIT8: EVENT Queue */
#define BIT_EN_PTCL_GCLK                               ((u32)0x00000001 << 0)          /*!<R/W 0x1  1: enable clock gating */
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXRPT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_AMPDU_TXRPT_TIME_THRS                 ((u32)0x000000FF << 24)          /*!<R/W 0x10  Time threshold of ampdu txrpt */
#define BIT_AMPDU_TXRPT_TIME_THRS(x)                   ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_AMPDU_TXRPT_TIME_THRS(x)               ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_AMPDU_TXRPT_LEN_THRS                  ((u32)0x000000FF << 16)          /*!<R/W 0x01  Length threshold of ampdu txrpt */
#define BIT_AMPDU_TXRPT_LEN_THRS(x)                    ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_AMPDU_TXRPT_LEN_THRS(x)                ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_AMPDU_TXRPT_R_PTR                     ((u32)0x000000FF << 8)          /*!<R/W 0x0  Read pointer of ampdu txrpt */
#define BIT_AMPDU_TXRPT_R_PTR(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_AMPDU_TXRPT_R_PTR(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_AMPDU_TXRPT_W_PTR                     ((u32)0x000000FF << 0)          /*!<RO 0x0  Write pointer of ampdu txrpt */
#define BIT_AMPDU_TXRPT_W_PTR(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_AMPDU_TXRPT_W_PTR(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CFEND_RATE_SC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXSC_40M                              ((u32)0x0000000F << 28)          /*!<R/W 0x0  Value of Data frame Tx sub-carrier mode 40MHz */
#define BIT_TXSC_40M(x)                                ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_TXSC_40M(x)                            ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_TXSC_20M                              ((u32)0x0000000F << 24)          /*!<R/W 0x0  Value of Data frame Tx sub-carrier mode 20MHz */
#define BIT_TXSC_20M(x)                                ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_TXSC_20M(x)                            ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_STBC_CFEND_RATE                       ((u32)0x000001FF << 15)          /*!<R/W 0x80  The initial rate for STBC CFEND frame transmitted as EDCA originator. The response control frame shall be selected by the definition of 802.11n. MCS0 */
#define BIT_STBC_CFEND_RATE(x)                         ((u32)(((x) & 0x000001FF) << 15))
#define BIT_GET_STBC_CFEND_RATE(x)                     ((u32)(((x >> 15) & 0x000001FF)))
#define BIT_MASK_BASIC_CFEND_RATE                      ((u32)0x000001FF << 6)          /*!<R/W 0x04  The initial rate for basic CFEND frame transmitted as EDCA originator. The response control frame shall be selected by the definition of 802.11n. OFDM6M */
#define BIT_BASIC_CFEND_RATE(x)                        ((u32)(((x) & 0x000001FF) << 6))
#define BIT_GET_BASIC_CFEND_RATE(x)                    ((u32)(((x >> 6) & 0x000001FF)))
#define BIT_LEAG_RTS_BW_DUP                            ((u32)0x00000001 << 5)          /*!<R/W 0x0  If this bit is set, leag rate RTS use Duplicate in 40M BW / 80M BW. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_SLEEP3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID127_96_PKTSLEEP                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid 127~Macid 96 Packet Sleep (per bit controlled) */
#define BIT_MACID127_96_PKTSLEEP(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID127_96_PKTSLEEP(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_SLEEP1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID63_32_PKTSLEEP                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid 63~Macid 32 Packet Sleep (per bit controlled) */
#define BIT_MACID63_32_PKTSLEEP(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID63_32_PKTSLEEP(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXRPT_START_OFFSET
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RPTFIFO_1K                                 ((u32)0x00000001 << 16)          /*!<R/W 0x0  select the size of rptfifo */
#define BIT_MASK_MACID_CTRL_OFFSET                     ((u32)0x000000FF << 8)          /*!<R/W 0x0  offset of macid control: ctrl_info in the start of txrptbuf */
#define BIT_MACID_CTRL_OFFSET(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MACID_CTRL_OFFSET(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_AMPDU_TXRPT_OFFSET                    ((u32)0x000000FF << 0)          /*!<R/W 0x5  offset of AMPDU txrpt */
#define BIT_AMPDU_TXRPT_OFFSET(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_AMPDU_TXRPT_OFFSET(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PTA_WL_PRI_MASK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_PTA_WL_PRI_MASK_CPU_MGQ                    ((u32)0x00000001 << 31)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has CPU_MGQ packet */
#define BIT_PTA_WL_PRI_MASK_BCNQ                       ((u32)0x00000001 << 30)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has BCNQ packet */
#define BIT_PTA_WL_PRI_MASK_HIQ                        ((u32)0x00000001 << 29)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has HIQ packet */
#define BIT_PTA_WL_PRI_MASK_MGQ                        ((u32)0x00000001 << 28)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has MGQ packet */
#define BIT_PTA_WL_PRI_MASK_BK                         ((u32)0x00000001 << 27)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has BKQ packet */
#define BIT_PTA_WL_PRI_MASK_BE                         ((u32)0x00000001 << 26)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has BEQ packet */
#define BIT_PTA_WL_PRI_MASK_VI                         ((u32)0x00000001 << 25)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has VIQ packet */
#define BIT_PTA_WL_PRI_MASK_VO                         ((u32)0x00000001 << 24)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has VOQ packet */
/** @} */

/**************************************************************************//**
 * @defgroup REG_EVTQ_RPT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EVTQ_TX_RPT                           ((u32)0x00000007 << 27)          /*!<R 0x0  *000: nothing *100: pktok *010: pktfail / timeout *001: evtq_tx_area_75 */
#define BIT_EVTQ_TX_RPT(x)                             ((u32)(((x) & 0x00000007) << 27))
#define BIT_GET_EVTQ_TX_RPT(x)                         ((u32)(((x >> 27) & 0x00000007)))
#define BIT_PTA_WL_PRI_MASK_EVT                        ((u32)0x00000001 << 25)          /*!<R/W 0x0  PTCL send wl_pri to PTA when pktbuf has EVTQ packet */
/** @} */

/**************************************************************************//**
 * @defgroup REG_SW_AMPDU_BURST_MODE_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_EVTQ_HEAD                             ((u32)0x000000FF << 24)          /*!<R/W 0xf6  TXDMA will pull eventq Pkt in this address. NAN */
#define BIT_EVTQ_HEAD(x)                               ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_EVTQ_HEAD(x)                           ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_DMA_THIS_QUEUE                        ((u32)0x0000000F << 20)          /*!<R/W 0x0  scheduler enable */
#define BIT_DMA_THIS_QUEUE(x)                          ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_DMA_THIS_QUEUE(x)                      ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_TOTAL_LEN_THRS                        ((u32)0x00000FFF << 8)          /*!<R/W 0x400  aggregation length over threshold then fetch pad packet in early mode */
#define BIT_TOTAL_LEN_THRS(x)                          ((u32)(((x) & 0x00000FFF) << 8))
#define BIT_GET_TOTAL_LEN_THRS(x)                      ((u32)(((x >> 8) & 0x00000FFF)))
#define BIT_WEP_PRETX_EN                               ((u32)0x00000001 << 7)          /*!<R/W 0x1  Enable pretx for WEP/TKIP, to allow a slower security clock */
#define BIT_PRE_TX_CMD                                 ((u32)0x00000001 << 6)          /*!<R/W 0x1  enable pre tx cmd to use aggr more packet */
#define BIT_MASK_NUM_SCL_EN                            ((u32)0x00000003 << 4)          /*!<R/W 0x0  SW MPDU Burst mode select 00: each length field for 1 MPDU 01: each length field is the summation of 2 MPDUs 10: each length field is the summation of 3 MPDUs 11: each length field is the summation of 4 MPDUs Note: the total MPDU number is no more than 15. */
#define BIT_NUM_SCL_EN(x)                              ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_NUM_SCL_EN(x)                          ((u32)(((x >> 4) & 0x00000003)))
#define BIT_BK_EN                                      ((u32)0x00000001 << 3)          /*!<R/W 0x0  AMPDU burst mode enable for AC_BK queue */
#define BIT_BE_EN                                      ((u32)0x00000001 << 2)          /*!<R/W 0x0  AMPDU burst mode enable for AC_BE queue */
#define BIT_VI_EN                                      ((u32)0x00000001 << 1)          /*!<R/W 0x0  AMPDU burst mode enable for AC_VI queue */
#define BIT_VO_EN                                      ((u32)0x00000001 << 0)          /*!<R/W 0x0  AMPDU burst mode enable for AC_VO queue */
/** @} */

/**************************************************************************//**
 * @defgroup REG_PKT_LIFE_TIME
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PKT_LIFTIME_BEBK                      ((u32)0x0000FFFF << 16)          /*!<R/W 0x1000  BE/BK packet lifetime unit: 256us */
#define BIT_PKT_LIFTIME_BEBK(x)                        ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_PKT_LIFTIME_BEBK(x)                    ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_PKT_LIFTIME_VOVI                      ((u32)0x0000FFFF << 0)          /*!<R/W 0x1000  VO/VI packet lifetime unit: 256us */
#define BIT_PKT_LIFTIME_VOVI(x)                        ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_PKT_LIFTIME_VOVI(x)                    ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PTA_STBC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EN_SINGLE_APMDU                            ((u32)0x00000001 << 31)          /*!<R/W 0x0  If this bit is set, Enable single sub-frame APMDU. */
#define BIT_FORCE_RND_PRI                              ((u32)0x00000001 << 22)          /*!<R/W 0x0  queue select priority control 1:old round robin method 0:depend on last selected queue */
#define BIT_PTA_EDCCA_EN                               ((u32)0x00000001 << 21)          /*!<R/W 0x0  0 : wl_tx signal to PTA does not consider EDCCA 1 : wl_tx signal to PTA consider EDCCA */
#define BIT_PTA_WL_TX_EN                               ((u32)0x00000001 << 20)          /*!<R/W 0x0  enable WL PTA (should be set in init MAC) */
#define BIT_R_USE_DATA_BW                              ((u32)0x00000001 << 19)          /*!<R/W 0x0  1 : Use des2_data_bw for this packet 0 : when des1_cca_rts = 2'b10, update_bw depend on ch_bw_status */
#define BIT_TRI_PKT_STATUS                             ((u32)0x00000001 << 18)          /*!<R/W 0x1   */
#define BIT_TRI_PKT_PKTIN                              ((u32)0x00000001 << 17)          /*!<R/W 0x0  If trigger frame is transmitted into txpktbuf, HW generates interrupt 1: Enable 0: Disable */
#define BIT_ACQ_MODE_SEL                               ((u32)0x00000001 << 16)          /*!<R/W 0x0  1: Enable 88C 4AC Mode 0: Enable 8723 ACQ0~7 Mode */
#define BIT_MASK_CDEND_TXTIME_H                        ((u32)0x0000001F << 8)          /*!<R/W 0x3  Cfend packet txtime bit[8:4] */
#define BIT_CDEND_TXTIME_H(x)                          ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_CDEND_TXTIME_H(x)                      ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_CDEND_TXTIME_L                        ((u32)0x0000000F << 4)          /*!<R/W 0x4  Cfend packet txtime bit[3:0] */
#define BIT_CDEND_TXTIME_L(x)                          ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_CDEND_TXTIME_L(x)                      ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_NESS                                  ((u32)0x00000003 << 2)          /*!<R/W 0x0  NESS */
#define BIT_NESS(x)                                    ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_NESS(x)                                ((u32)(((x >> 2) & 0x00000003)))
#define BIT_MASK_STBC_CFEND                            ((u32)0x00000003 << 0)          /*!<R/W 0x0  CFEND STBC */
#define BIT_STBC_CFEND(x)                              ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_STBC_CFEND(x)                          ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PROT_MODE_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RTS_MAX_AGG_NUM                       ((u32)0x0000003F << 24)          /*!<R/W 0xC  In RTS frame exchange sequence, the maximum aggregation packet number limit is defined by this register, unit is byte */
#define BIT_RTS_MAX_AGG_NUM(x)                         ((u32)(((x) & 0x0000003F) << 24))
#define BIT_GET_RTS_MAX_AGG_NUM(x)                     ((u32)(((x >> 24) & 0x0000003F)))
#define BIT_MASK_MAX_AGG_NUM                           ((u32)0x0000003F << 16)          /*!<R/W 0x14  the maximum aggregation packet number limit,unit is byte */
#define BIT_MAX_AGG_NUM(x)                             ((u32)(((x) & 0x0000003F) << 16))
#define BIT_GET_MAX_AGG_NUM(x)                         ((u32)(((x >> 16) & 0x0000003F)))
#define BIT_MASK_RTS_TXTIME_TH                         ((u32)0x000000FF << 8)          /*!<R/W 0x1  RTS packet txtime threshold, unit is 64 us */
#define BIT_RTS_TXTIME_TH(x)                           ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_RTS_TXTIME_TH(x)                       ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_RTS_LEN_TH                            ((u32)0x000000FF << 0)          /*!<R/W 0x1  RTS packet Length threshold, unit is 128 bytes */
#define BIT_RTS_LEN_TH(x)                              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RTS_LEN_TH(x)                          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BAR_MODE_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RTY_PKT_LMT                           ((u32)0x0000003F << 24)          /*!<R/W 0x1  retry packet limit */
#define BIT_RTY_PKT_LMT(x)                             ((u32)(((x) & 0x0000003F) << 24))
#define BIT_GET_RTY_PKT_LMT(x)                         ((u32)(((x >> 24) & 0x0000003F)))
#define BIT_MASK_BAR_RTY_LMT                           ((u32)0x00000003 << 16)          /*!<R/W 0x2  BAR retry limit */
#define BIT_BAR_RTY_LMT(x)                             ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_BAR_RTY_LMT(x)                         ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_BAR_PKTTIME_THRS                      ((u32)0x000000FF << 8)          /*!<R/W 0xFF  BAR packet time thrs */
#define BIT_BAR_PKTTIME_THRS(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BAR_PKTTIME_THRS(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_BAR_PKTNUM_TH_V1                      ((u32)0x0000007F << 0)          /*!<R/W 0x7F  BAR Pktnum Thrs */
#define BIT_BAR_PKTNUM_TH_V1(x)                        ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_BAR_PKTNUM_TH_V1(x)                    ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_SLEEP2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID95_64PKTSLEEP                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid 95~Macid 64 Packet Sleep (per bit controlled) */
#define BIT_MACID95_64PKTSLEEP(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID95_64PKTSLEEP(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_SLEEP
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID31_0_PKTSLEEP                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid31 ~ Macid 0Packet Sleep. (per bit controlled) */
#define BIT_MACID31_0_PKTSLEEP(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID31_0_PKTSLEEP(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HW_SEQ0_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HW_SSN_SEQ1                           ((u32)0x00000FFF << 16)          /*!<R/W 0x0  When TXDESC HWSEQEN is set, MAC will overwrite SEQ control by this register. Before DRV hand over the SEQ control to HW, DRV may set the current SEQ to this register. When DRV will take back the SEQ control from HW, DRV may get the current sequence from this register. */
#define BIT_HW_SSN_SEQ1(x)                             ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_HW_SSN_SEQ1(x)                         ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_HW_SSN_SEQ0                           ((u32)0x00000FFF << 0)          /*!<R/W 0x0  When TXDESC HWSEQEN is set, MAC will overwrite SEQ control by this register. Before DRV hand over the SEQ control to HW, DRV may set the current SEQ to this register. When DRV will take back the SEQ control from HW, DRV may get the current sequence from this register. */
#define BIT_HW_SSN_SEQ0(x)                             ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_HW_SSN_SEQ0(x)                         ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_HW_SEQ2_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_HW_SSN_SEQ3                           ((u32)0x00000FFF << 16)          /*!<R/W 0x0  When TXDESC HWSEQEN is set, MAC will overwrite SEQ control by this register. Before DRV hand over the SEQ control to HW, DRV may set the current SEQ to this register. When DRV will take back the SEQ control from HW, DRV may get the current sequence from this register. */
#define BIT_HW_SSN_SEQ3(x)                             ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_HW_SSN_SEQ3(x)                         ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_HW_SSN_SEQ2                           ((u32)0x00000FFF << 0)          /*!<R/W 0x0  When TXDESC HWSEQEN is set, MAC will overwrite SEQ control by this register. Before DRV hand over the SEQ control to HW, DRV may set the current SEQ to this register. When DRV will take back the SEQ control from HW, DRV may get the current sequence from this register. */
#define BIT_HW_SSN_SEQ2(x)                             ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_HW_SSN_SEQ2(x)                         ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PTCLERR_NULLPKT_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_PTCL_TOTAL_PG0_7                      ((u32)0x000000FF << 24)          /*!<RO 0x0  Total Page in PTCL */
#define BIT_PTCL_TOTAL_PG0_7(x)                        ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_PTCL_TOTAL_PG0_7(x)                    ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_PTCL_TOTAL_PG_8                            ((u32)0x00000001 << 23)          /*!<RO 0x0  Total Page in PTCL */
#define BIT_PTCL_RATE_TABLE_INVALID                    ((u32)0x00000001 << 22)          /*!<R/W1C 0x0  during rate fallback process, if the all rate table is invalid, then assert ERROR FLAG */
#define BIT_RD_TXHANG_ERR                              ((u32)0x00000001 << 21)          /*!<R/W1C 0x0  Indicates the txhang error in RDG mode */
#define BIT_RD_STATUS_ERR                              ((u32)0x00000001 << 20)          /*!<R/W1C 0x0  Indicates the status error in RDG mode */
#define BIT_RD_CTN_ERR                                 ((u32)0x00000001 << 19)          /*!<R/W1C 0x0  Indicates the cnt error in RDG mode */
#define BIT_MASK_RD_PKTIN_ERR                          ((u32)0x00000007 << 16)          /*!<R/W1C 0x0  Indicates the pktin error in RDG mode */
#define BIT_RD_PKTIN_ERR(x)                            ((u32)(((x) & 0x00000007) << 16))
#define BIT_GET_RD_PKTIN_ERR(x)                        ((u32)(((x >> 16) & 0x00000007)))
#define BIT_RD_FTMT2R_ERR                              ((u32)0x00000001 << 15)          /*!<R/W1C 0x0  Indicates the timeout error in FTM t2r report */
#define BIT_P1_TX_NULL_1                               ((u32)0x00000001 << 3)          /*!<RO 0x0  Port 1 Tx Null 1 packet ok indicator */
#define BIT_P1_TX_NULL_0                               ((u32)0x00000001 << 2)          /*!<RO 0x0  Port 1 Tx Null 0 packet ok indicator */
#define BIT_P0_TX_NULL_1                               ((u32)0x00000001 << 1)          /*!<RO 0x0  Port 0 Tx Null 1 packet ok indicator */
#define BIT_P0_TX_NULL_0                               ((u32)0x00000001 << 0)          /*!<RO 0x0  Port 0 Tx Null 0 packet ok indicator */
/** @} */

/**************************************************************************//**
 * @defgroup REG_PRETX_DROP_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_COLLI_DETEC_TIME_MAX                  ((u32)0x0000000F << 20)          /*!<R/W 0xF  0~15: The field indicated collision detect time for broadcast or Beacon packet.(unit: us) */
#define BIT_COLLI_DETEC_TIME_MAX(x)                    ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_COLLI_DETEC_TIME_MAX(x)                ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_PRETX_AGGR_EN                              ((u32)0x00000001 << 19)          /*!<R/W 0x1  1: enable PRETX function, default is enable 0: disable PRETX function */
#define BIT_MASK_PRETX_AGGR_TIME_MAX                   ((u32)0x000007FF << 8)          /*!<R/W 0x8C  If BIT_PRETX_AGGR_EN = 1, the bits will count down after PHYTXON signal asserting every transmission in pre-tx mode.(Default: 23F, mac_clk = 20MHz, 3us;Smart,mac_clk = 40MHz, 3.5us) */
#define BIT_PRETX_AGGR_TIME_MAX(x)                     ((u32)(((x) & 0x000007FF) << 8))
#define BIT_GET_PRETX_AGGR_TIME_MAX(x)                 ((u32)(((x >> 8) & 0x000007FF)))
#define BIT_HGQ_DEL_EN                                 ((u32)0x00000001 << 7)          /*!<R/W 0x0  1: HGQ packet will be dropped when enable macid delete function; 0: HGQ packet cannot be dropped */
#define BIT_MGQ_DEL_EN                                 ((u32)0x00000001 << 6)          /*!<R/W 0x0  1: MGQ packet will be dropped when enable macid delete function; 0: MGQ packet cannot be dropped */
/** @} */

/**************************************************************************//**
 * @defgroup REG_BT_POLLUTE_PKT_CNT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BT_RXPOLLUTE_PKT_CNT                  ((u32)0x0000FFFF << 16)          /*!<R 0x0  count the tx fail pkt caused by bt RX pollute, write 0x4E8[31] will clear the counter */
#define BIT_BT_RXPOLLUTE_PKT_CNT(x)                    ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_BT_RXPOLLUTE_PKT_CNT(x)                ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_BT_TXPOLLUTE_PKT_CNT                  ((u32)0x0000FFFF << 0)          /*!<R 0x0  count the tx fail pkt caused by bt TX pollute, write 0x4E8[15] will clear the counter */
#define BIT_BT_TXPOLLUTE_PKT_CNT(x)                    ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BT_TXPOLLUTE_PKT_CNT(x)                ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DROP_PKT_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_LIFE_DROP_NUM                         ((u32)0x0000FFFF << 16)          /*!<R/W 0x0  TIE 0 in 8723F */
#define BIT_LIFE_DROP_NUM(x)                           ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_LIFE_DROP_NUM(x)                       ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_DROP_PKT_NUM                          ((u32)0x0000FFFF << 0)          /*!<R/WC 0x0  Statistic the number of dropped packets */
#define BIT_DROP_PKT_NUM(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_DROP_PKT_NUM(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CPUMGQ_TIMER_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MGQ_TRI_HEAD                          ((u32)0x00000FFF << 16)          /*!<R/W 0x0  The head page of the CPUMGQ pkt If the CPUMGQ is triggered by TX TIMER. */
#define BIT_MGQ_TRI_HEAD(x)                            ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MGQ_TRI_HEAD(x)                        ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_CPUMGQ_TRI_LIFETIME_EN                     ((u32)0x00000001 << 8)          /*!<R/W 0x0  When this bit is set, enable the lifetime function. This lifetime threshold only affect CPUMGQ trigger from Tx_Timer. */
#define BIT_MASK_CPUMGQ_TRI_LIFETIME                   ((u32)0x000000FF << 0)          /*!<R/W 0x0  LIFETIME Threshold: Unit = 256us. When CPU MgntQ is triggered, If BIT_CPUMGQ_TRI_LIFETIME_EN = 1, count down. If BIT_CPUMGQ_TRI_LIFETIME_EN = 0, do nothing. */
#define BIT_CPUMGQ_TRI_LIFETIME(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_CPUMGQ_TRI_LIFETIME(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DUMMY
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DUMMY_REG_1                           ((u32)0x00003FFF << 18)          /*!<R/W 0x3FFF  dummy reg */
#define BIT_DUMMY_REG_1(x)                             ((u32)(((x) & 0x00003FFF) << 18))
#define BIT_GET_DUMMY_REG_1(x)                         ((u32)(((x >> 18) & 0x00003FFF)))
#define BIT_EN_BCN_TRXRPT                              ((u32)0x00000001 << 17)          /*!<R/W 0x1  If this bit is set, PTCL will write BCN TRXRPT when BCNOK/BCNERR INT. FW have clear this bit before enter LPS 32K mode. */
#define BIT_MASK_DUMMY_REG_0                           ((u32)0x0001FFFF << 0)          /*!<R/W 0x10000  dummy reg */
#define BIT_DUMMY_REG_0(x)                             ((u32)(((x) & 0x0001FFFF) << 0))
#define BIT_GET_DUMMY_REG_0(x)                         ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AESIV_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_AESIV_OFFSET                          ((u32)0x00000FFF << 0)          /*!<R/W 0x3  aesiv offset in txrpt buffer (Unit:128Bytes). */
#define BIT_AESIV_OFFSET(x)                            ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_AESIV_OFFSET(x)                        ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BF0_TIME_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BF0_TIMER_SET                              ((u32)0x00000001 << 31)          /*!<W1C 0x0  write 1 to set beamforming 0 timer to BIT_bf0_lifetime */
#define BIT_BF0_TIMER_CLR                              ((u32)0x00000001 << 30)          /*!<W1C 0x0  write 1 to clear beamforming 0 timer to 0 */
#define BIT_EN_BF0_UPDATE                              ((u32)0x00000001 << 29)          /*!<R/W 0x0  1: when tx ndp ok/fail, enable to update beamforming 0 timer */
#define BIT_EN_BF0_TIMER                               ((u32)0x00000001 << 28)          /*!<R/W 0x0  beamforming 0 timer enable */
#define BIT_MASK_BF0_PRETIME_OVER                      ((u32)0x00000FFF << 16)          /*!<R/W 0xFFF  beamforming 0 pre timeout time */
#define BIT_BF0_PRETIME_OVER(x)                        ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_BF0_PRETIME_OVER(x)                    ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_BF0_LIFETIME                          ((u32)0x0000FFFF << 0)          /*!<R/W 0xFFFF  beamforming 0 life time */
#define BIT_BF0_LIFETIME(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BF0_LIFETIME(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BF1_TIME_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BF1_TIMER_SET                              ((u32)0x00000001 << 31)          /*!<W1C 0x0  write 1 to set beamforming 1 timer to BIT_bf1_lifetime */
#define BIT_BF1_TIMER_CLR                              ((u32)0x00000001 << 30)          /*!<W1C 0x0  write 1 to clear beamforming 1 timer to 0 */
#define BIT_EN_BF1_UPDATE                              ((u32)0x00000001 << 29)          /*!<R/W 0x0  1: when tx ndp ok/fail, enable to update beamforming 1 timer */
#define BIT_EN_BF1_TIMER                               ((u32)0x00000001 << 28)          /*!<R/W 0x0  beamforming 1 timer enable */
#define BIT_MASK_BF1_PRETIME_OVER                      ((u32)0x00000FFF << 16)          /*!<R/W 0xFFF  beamforming 1 pre timeout time */
#define BIT_BF1_PRETIME_OVER(x)                        ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_BF1_PRETIME_OVER(x)                    ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_BF1_LIFETIME                          ((u32)0x0000FFFF << 0)          /*!<R/W 0xFFFF  beamforming 1 life time */
#define BIT_BF1_LIFETIME(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BF1_LIFETIME(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BF_TIMEOUT_EN
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EN_VHT_LDPC                                ((u32)0x00000001 << 9)          /*!<R/W 0x1  1: enable ldpc in vht rate mode */
#define BIT_EN_HT_LDPC                                 ((u32)0x00000001 << 8)          /*!<R/W 0x1  1: enable ldpc in ht rate mode */
#define BIT_BF1_TIMEOUT_EN                             ((u32)0x00000001 << 1)          /*!<R/W 0x1  1: when beamforming 1 timer count to 0,then disable beamforming 0 */
#define BIT_BF0_TIMEOUT_EN                             ((u32)0x00000001 << 0)          /*!<R/W 0x1  1: when beamforming 0 timer count to 0,then disable beamforming 0 */
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_RELEASE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID31_0_RELEASE                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid31~Macid0 Release(per bit controlled) */
#define BIT_MACID31_0_RELEASE(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID31_0_RELEASE(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_RELEASE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID63_32_RELEASE                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid63~Macid32 Release(per bit controlled) */
#define BIT_MACID63_32_RELEASE(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID63_32_RELEASE(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_RELEASE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID95_64_RELEASE                    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid95~Macid64 Release (per bit controlled) */
#define BIT_MACID95_64_RELEASE(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID95_64_RELEASE(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_RELEASE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID127_96_RELEASE                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid127~Macid96 Release (per bit controlled) */
#define BIT_MACID127_96_RELEASE(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID127_96_RELEASE(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_RELEASE_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MACID_VALUE                                ((u32)0x00000001 << 7)          /*!<R/W 0x0  write this field to set REG_macid_release[BIT_macid_offset] = BIT_macid_value */
#define BIT_MASK_MACID_OFFSET                          ((u32)0x0000007F << 0)          /*!<R/W 0x0  write this field to set REG_macid_release[BIT_macid_offset] = BIT_macid_value */
#define BIT_MACID_OFFSET(x)                            ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_MACID_OFFSET(x)                        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FAST_EDCA_VOVI_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_VI_FAST_EDCA_TO                       ((u32)0x000000FF << 24)          /*!<R/W 0x8  vi queue fast edca timeout value */
#define BIT_VI_FAST_EDCA_TO(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_VI_FAST_EDCA_TO(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_VI_THRESHOLD_SEL                           ((u32)0x00000001 << 23)          /*!<R/W 0x0  1: use all vi queue pktnum compare to pktnum_threshold; 0: use single macid pktnum compare to pktnum_threshold; */
#define BIT_MASK_VI_FAST_EDCA_PKT_TH                   ((u32)0x0000007F << 16)          /*!<R/W 0x4  vi queue fast edca pkt number threshold */
#define BIT_VI_FAST_EDCA_PKT_TH(x)                     ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_VI_FAST_EDCA_PKT_TH(x)                 ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_VO_FAST_EDCA_TO                       ((u32)0x000000FF << 8)          /*!<R/W 0x8  vo queue fast edca timeout value */
#define BIT_VO_FAST_EDCA_TO(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_VO_FAST_EDCA_TO(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_VO_THRESHOLD_SEL                           ((u32)0x00000001 << 7)          /*!<R/W 0x0  1: use all vo queue pktnum compare to pktnum_threshold; 0: use single macid pktnum compare to pktnum_threshold; */
#define BIT_MASK_VO_FAST_EDCA_PKT_TH                   ((u32)0x0000007F << 0)          /*!<R/W 0x4  vo queue fast edca pkt number threshold */
#define BIT_VO_FAST_EDCA_PKT_TH(x)                     ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_VO_FAST_EDCA_PKT_TH(x)                 ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FAST_EDCA_BEBK_SETTING
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BK_FAST_EDCA_TO                       ((u32)0x000000FF << 24)          /*!<R/W 0x8  bk queue fast edca timeout value */
#define BIT_BK_FAST_EDCA_TO(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_BK_FAST_EDCA_TO(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_BK_THRESHOLD_SEL                           ((u32)0x00000001 << 23)          /*!<R/W 0x0  1: use all bk queue pktnum compare to pktnum_threshold; 0: use single macid pktnum compare to pktnum_threshold; */
#define BIT_MASK_BK_FAST_EDCA_PKT_TH                   ((u32)0x0000007F << 16)          /*!<R/W 0x4  bk queue fast edca pkt number threshold */
#define BIT_BK_FAST_EDCA_PKT_TH(x)                     ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_BK_FAST_EDCA_PKT_TH(x)                 ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_BE_FAST_EDCA_TO                       ((u32)0x000000FF << 8)          /*!<R/W 0x8  be queue fast edca timeout value */
#define BIT_BE_FAST_EDCA_TO(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_BE_FAST_EDCA_TO(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_BE_THRESHOLD_SEL                           ((u32)0x00000001 << 7)          /*!<R/W 0x0  1: use all be queue pktnum compare to pktnum_threshold; 0: use single macid pktnum compare to pktnum_threshold; */
#define BIT_MASK_BE_FAST_EDCA_PKT_TH                   ((u32)0x0000007F << 0)          /*!<R/W 0x4  be queue fast edca pkt number threshold */
#define BIT_BE_FAST_EDCA_PKT_TH(x)                     ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_BE_FAST_EDCA_PKT_TH(x)                 ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_DROP0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID31_0_DROP                        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid31~Macid0 Drop (per bit controlled) */
#define BIT_MACID31_0_DROP(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID31_0_DROP(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_DROP1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID63_32_DROP                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid63~Macid32 Drop (per bit controlled) */
#define BIT_MACID63_32_DROP(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID63_32_DROP(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_DROP2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID95_64_DROP                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid95~Macid64 Drop (per bit controlled) */
#define BIT_MACID95_64_DROP(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID95_64_DROP(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MACID_DROP3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MACID127_96_DROP                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Macid127~Macid96 Drop (per bit controlled) */
#define BIT_MACID127_96_DROP(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MACID127_96_DROP(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_r_macid_release_success_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MACID_RELEASE_SUCCESS_0             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  macid0~31 */
#define BIT_R_MACID_RELEASE_SUCCESS_0(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_MACID_RELEASE_SUCCESS_0(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_r_macid_release_success_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MACID_RELEASE_SUCCESS_1             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  macid32~63 */
#define BIT_R_MACID_RELEASE_SUCCESS_1(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_MACID_RELEASE_SUCCESS_1(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_r_macid_release_success_2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MACID_RELEASE_SUCCESS_2             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  macid64~95 */
#define BIT_R_MACID_RELEASE_SUCCESS_2(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_MACID_RELEASE_SUCCESS_2(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_r_macid_release_success_3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MACID_RELEASE_SUCCESS_3             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  macid96~127 */
#define BIT_R_MACID_RELEASE_SUCCESS_3(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_MACID_RELEASE_SUCCESS_3(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MGQ_FIFO_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MGQ_FIFO_EN                                ((u32)0x00000001 << 31)          /*!<R/W 0x0  1: Enable MGQ FIFO Function. 0: Disable MGQ FIFO Function. */
#define BIT_MASK_MGQ_FIFO_PG_SIZE                      ((u32)0x00000007 << 28)          /*!<R/W 0x1  The page size of MGQ FIFO PKTs. */
#define BIT_MGQ_FIFO_PG_SIZE(x)                        ((u32)(((x) & 0x00000007) << 28))
#define BIT_GET_MGQ_FIFO_PG_SIZE(x)                    ((u32)(((x >> 28) & 0x00000007)))
#define BIT_MASK_MGQ_FIFO_START_PG                     ((u32)0x00000FFF << 16)          /*!<R/W 0x0  The start page of MGQ FIFO. */
#define BIT_MGQ_FIFO_START_PG(x)                       ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MGQ_FIFO_START_PG(x)                   ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MGQ_FIFO_SIZE                         ((u32)0x00000003 << 14)          /*!<R/W 0x0  2'b00 : FIFO size 4 pkts. 2'b01 : FIFO size 8 pkts. 2'b1x : FIFO size 16 pkts. */
#define BIT_MGQ_FIFO_SIZE(x)                           ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_MGQ_FIFO_SIZE(x)                       ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MGQ_FIFO_PAUSE                             ((u32)0x00000001 << 13)          /*!<R/W 0x0  1: use all vo queue pktnum compare to pktnum_threshold; 0: use signle macid pktnum compare to pktnum_threshold; */
#define BIT_MASK_MGQ_FIFO_RPTR                         ((u32)0x0000001F << 8)          /*!<W 0x0  The read pointer of the MGQ FIFO.(clear when disable MGQ FIFO function) */
#define BIT_MGQ_FIFO_RPTR(x)                           ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_MGQ_FIFO_RPTR(x)                       ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MGQ_FIFO_OV                                ((u32)0x00000001 << 7)          /*!<W 0x0  1: MGQ FIFO overflow. 0: NO MGQ FIFO overflow. */
#define BIT_MGQ_FIFO_WPTR_ERR                          ((u32)0x00000001 << 6)          /*!<W 0x0  1: Error of MGQ FIFO write pointer setting. 0: No error of MGQ FIFO write pointer setting. (clear when disable MGQ FIFO function) */
#define BIT_CPUM_LIFETIME_EN                           ((u32)0x00000001 << 5)          /*!<R/W 0x0  1: Enable Lifetime function of the MGQ FIFO pkts. 0: Disable Lifetime function of the MGQ FIFO pkts. */
#define BIT_MASK_MGQ_FIFO_WPTR                         ((u32)0x0000001F << 0)          /*!<R/W 0x0  The write pointer of the MGQ FIFO */
#define BIT_MGQ_FIFO_WPTR(x)                           ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_MGQ_FIFO_WPTR(x)                       ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MGQ_FIFO_RELEASE_INT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MGQ_FIFO_INT_FLAG                     ((u32)0x0000FFFF << 16)          /*!<W 0x0  The flag of the MGQ FIFO PKT release interrupt, write one clear. */
#define BIT_MGQ_FIFO_INT_FLAG(x)                       ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_MGQ_FIFO_INT_FLAG(x)                   ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_MGQ_FIFO_INT_MASK                     ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  The mask of the MGQ FIFO PKT release interrupt. */
#define BIT_MGQ_FIFO_INT_MASK(x)                       ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MGQ_FIFO_INT_MASK(x)                   ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MGQ_FIFO_LIFETIME_VALID
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MGQ_FIFO_LIFETIME                     ((u32)0x0000FFFF << 16)          /*!<R/W 0x1000  MGQ FIFO packet lifetime unit: 256us */
#define BIT_MGQ_FIFO_LIFETIME(x)                       ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_MGQ_FIFO_LIFETIME(x)                   ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_MGQ_FIFO_VALID_CLR_1                  ((u32)0x000000FF << 8)          /*!<R 0x0  Clear the valid map of the MGQ FIFO PKT, high [15:8] bits */
#define BIT_MGQ_FIFO_VALID_CLR_1(x)                    ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MGQ_FIFO_VALID_CLR_1(x)                ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_MGQ_FIFO_VALID_CLR_0                  ((u32)0x000000FF << 0)          /*!<R 0x0  Clear the valid map of the MGQ FIFO PKT, low [7:0] bits */
#define BIT_MGQ_FIFO_VALID_CLR_0(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_MGQ_FIFO_VALID_CLR_0(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_r_macid_release_success_clear_offset
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MACID_RELEASE_SUCCESS_CLEAR_OFFSET  ((u32)0x0000007F << 0)          /*!<R/W 0x0  HW will set 0x2460~0x246C if one packet was released successfully for a given macid. SW use this register to reset the status. */
#define BIT_R_MACID_RELEASE_SUCCESS_CLEAR_OFFSET(x)    ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_MACID_RELEASE_SUCCESS_CLEAR_OFFSET(x) ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_RESP_RRSR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RRSR_CCK                              ((u32)0x0000000F << 16)          /*!<R/W 0x5  Response rate BIT[3:0] = {(msb) CCK11, …, CCK1} */
#define BIT_RRSR_CCK(x)                                ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_RRSR_CCK(x)                            ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_RRSR_RATE_EN                          ((u32)0x0000000F << 8)          /*!<R/W 0x1  Enabled response rate mode BIT[2:0] = {(msb) HE rate, VHT rate, HT rate, CCK_OFDM rate} */
#define BIT_RRSR_RATE_EN(x)                            ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_RRSR_RATE_EN(x)                        ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_RATE_SEL                              ((u32)0x00000007 << 0)          /*!<R/W 0x0  New ARFR (REG 0x1484) rate group selection: 0: CCK1M – OFDM18M 1: OFDM24MRSVDOFDM54 , other no use 2: HT MCS0~MCS7 3: HT others, not use 4: VHT1SS/ MCS0~MCS7 5: VHT1SS/ MCS8~MCS9,other no use 6: HE 1SS / MCS0~MCS7 7: HE 1SS / MCS8~MCS9 ,other no use */
#define BIT_RATE_SEL(x)                                ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_RATE_SEL(x)                            ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_ARFR_WEIGHTING_TABLE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RATE7_WEIGHTING                       ((u32)0x0000000F << 28)          /*!<R/W 0x3  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is OFDM18M */
#define BIT_RATE7_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_RATE7_WEIGHTING(x)                     ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_RATE6_WEIGHTING                       ((u32)0x0000000F << 24)          /*!<R/W 0x2  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is OFDM12M */
#define BIT_RATE6_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_RATE6_WEIGHTING(x)                     ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_RATE5_WEIGHTING                       ((u32)0x0000000F << 20)          /*!<R/W 0x1  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is OFDM9M */
#define BIT_RATE5_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_RATE5_WEIGHTING(x)                     ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_RATE4_WEIGHTING                       ((u32)0x0000000F << 16)          /*!<R/W 0x1  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is OFDM6M */
#define BIT_RATE4_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_RATE4_WEIGHTING(x)                     ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_RATE3_WEIGHTING                       ((u32)0x0000000F << 12)          /*!<R/W 0x2  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is CCK11M */
#define BIT_RATE3_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_RATE3_WEIGHTING(x)                     ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_RATE2_WEIGHTING                       ((u32)0x0000000F << 8)          /*!<R/W 0x1  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is CCK5_5M */
#define BIT_RATE2_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_RATE2_WEIGHTING(x)                     ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_RATE1_WEIGHTING                       ((u32)0x0000000F << 4)          /*!<R/W 0x1  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is CCK2M */
#define BIT_RATE1_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_RATE1_WEIGHTING(x)                     ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_RATE0_WEIGHTING                       ((u32)0x0000000F << 0)          /*!<R/W 0x1  Weighting value of the corresponding data rate with respect to rate_sel in reg 0x1480[2:0] 0: disable fallback to the rate 1~15: valid weighting value default is CCK1M */
#define BIT_RATE0_WEIGHTING(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_RATE0_WEIGHTING(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DARFRC1_L
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DATA_FALLBACK1_RC4                    ((u32)0x0000001F << 24)          /*!<R/W 0x04  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC4(x)                      ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_DATA_FALLBACK1_RC4(x)                  ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK1_RC3                    ((u32)0x0000001F << 16)          /*!<R/W 0x03  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC3(x)                      ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_DATA_FALLBACK1_RC3(x)                  ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK1_RC2                    ((u32)0x0000001F << 8)          /*!<R/W 0x02  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC2(x)                      ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_DATA_FALLBACK1_RC2(x)                  ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK1_RC1                    ((u32)0x0000001F << 0)          /*!<R/W 0x01  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC1(x)                      ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_DATA_FALLBACK1_RC1(x)                  ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DARFRC1_H
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DATA_FALLBACK1_RC8                    ((u32)0x0000001F << 24)          /*!<R/W 0x08  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC8(x)                      ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_DATA_FALLBACK1_RC8(x)                  ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK1_RC7                    ((u32)0x0000001F << 16)          /*!<R/W 0x07  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC7(x)                      ((u32)(((x) & 0x0000001F) << 16))
#define BIT_GET_DATA_FALLBACK1_RC7(x)                  ((u32)(((x >> 16) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK1_RC6                    ((u32)0x0000001F << 8)          /*!<R/W 0x06  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC6(x)                      ((u32)(((x) & 0x0000001F) << 8))
#define BIT_GET_DATA_FALLBACK1_RC6(x)                  ((u32)(((x >> 8) & 0x0000001F)))
#define BIT_MASK_DATA_FALLBACK1_RC5                    ((u32)0x0000001F << 0)          /*!<R/W 0x05  Data Auto Rate Fallback Retry Count After the number of retry, HW will fall back the rate to the next enabled MAC in ARFR. */
#define BIT_DATA_FALLBACK1_RC5(x)                      ((u32)(((x) & 0x0000001F) << 0))
#define BIT_GET_DATA_FALLBACK1_RC5(x)                  ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TB_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_EN_TB_TXRPT                                ((u32)0x00000001 << 24)          /*!<R/W 0x1  When this bit is set, enable generate report for TB PPDU */
#define BIT_DIS_CHK_TB_AC                              ((u32)0x00000001 << 23)          /*!<R/W 0x1  When this bit is set, don't care the AC of DATA */
#define BIT_EN_TB_PRETX                                ((u32)0x00000001 << 22)          /*!<R/W 0x1  When this bit is set, enable pretx function of TB PPDU, assert txen before gen_cmd_rdy. */
#define BIT_EN_UPD_TB_RETRY                            ((u32)0x00000001 << 21)          /*!<R/W 0x1  enable write retrybit in MH and add retrycnt when TB PPDU TX fail. */
#define BIT_EN_TB_SIFS_BK                              ((u32)0x00000001 << 20)          /*!<R/W 0x1  If sifs_timeout_tb_ref (from trxptcl) asserted, break the aggr process of TB PPDU */
#define BIT_HW_SSN_EN_TB                               ((u32)0x00000001 << 19)          /*!<R/W 0x0  HW update the SSN [9] for TB PPDU */
#define BIT_TB_BYPASS_TXPOWER                          ((u32)0x00000001 << 18)          /*!<R/W 0x0  1: NOT abort TB PPDU even the txpwr_abort_tx_idx(from txpwr_ctrl) is illegal 0: enable tx power abort TB */
#define BIT_TB_DEL_EN                                  ((u32)0x00000001 << 17)          /*!<R/W 0x0  Enable TB PPDU packet delete: 1) pkt_ageout; 2) macid_delete */
#define BIT_TB_EN_LIFETIME                             ((u32)0x00000001 << 16)          /*!<R/W 0x0  When this bit is set, enable the lifetime function of TB PPDU. */
#define BIT_MASK_TB_PKT_LIFE_TIME                      ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  LIFETIME Threshold: Unit = 256us. When TB PPDU is triggered, If BIT_TB_EN_LIFETIME = 1, count down. If BIT_TB_EN_LIFETIME = 0, do nothing. */
#define BIT_TB_PKT_LIFE_TIME(x)                        ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TB_PKT_LIFE_TIME(x)                    ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_OTHER_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_GI_LTF_FB_SEL                              ((u32)0x00000001 << 31)          /*!<R/W 0x0  0: no GI_LTF fallback 1: GI_LTF fallback option 1 enabled (downgrade GI_LTF first from init. rate) Others RSVD */
#define BIT_MASK_NDPA_RATE                             ((u32)0x000001FF << 22)          /*!<R/W 0x04  NDPA tx rate setting, from CCK 1M to VHT NSS2 MCS9 */
#define BIT_NDPA_RATE(x)                               ((u32)(((x) & 0x000001FF) << 22))
#define BIT_GET_NDPA_RATE(x)                           ((u32)(((x >> 22) & 0x000001FF)))
#define BIT_EN_A_CTRL_BREAK                            ((u32)0x00000001 << 21)          /*!<R/W 0x1  1: break the AGGR process when the a_ctrl field in TXDESC is different */
#define BIT_MASK_RTS_TXOP_THRS                         ((u32)0x000003FF << 3)          /*!<R/W 0x0  unit:32us, valid when BIT_EN_TXOP_BASE_RTS = 1 */
#define BIT_RTS_TXOP_THRS(x)                           ((u32)(((x) & 0x000003FF) << 3))
#define BIT_GET_RTS_TXOP_THRS(x)                       ((u32)(((x >> 3) & 0x000003FF)))
#define BIT_EN_TXOP_BASE_RTS                           ((u32)0x00000001 << 2)          /*!<R/W 0x0  enable TXOP duration-based RTS/CTS */
#define BIT_EN_BT_POLLUTE_RX                           ((u32)0x00000001 << 1)          /*!<R/W 0x1  enable write BT Polluted field during RX RESP */
#define BIT_EN_SR_CTS2S                                ((u32)0x00000001 << 0)          /*!<R/W 0x0  enable send cts2s frame during SR */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RRSR_RATE_CFG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RRSR_HE                               ((u32)0x000000FF << 24)          /*!<R/W 0x0  Response rate BIT[35:28]={(msb) HE MCS7, MCS6, …, MCS0} */
#define BIT_RRSR_HE(x)                                 ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_RRSR_HE(x)                             ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_RRSR_VHT                              ((u32)0x000000FF << 16)          /*!<R/W 0x0  Response rate BIT[27:20] = { (msb) VHT MCS7, MCS6, …, MCS0} */
#define BIT_RRSR_VHT(x)                                ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_RRSR_VHT(x)                            ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_RRSR_HT                               ((u32)0x000000FF << 8)          /*!<R/W 0x0  Response rate BIT[19:12]={(msb) HT MCS7, MCS6, …, MCS0} */
#define BIT_RRSR_HT(x)                                 ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_RRSR_HT(x)                             ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_RRSR_OFDM                             ((u32)0x000000FF << 0)          /*!<R/W 0x15  Response rate BIT[11:4] = { (msb) OFDM54, OFDM48, …,OFDM6} */
#define BIT_RRSR_OFDM(x)                               ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_RRSR_OFDM(x)                           ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_NDP_RATE
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXPKT_CNT_SEL                         ((u32)0x0000000F << 28)          /*!<R/W 0x0  Select the counter to show in 0X14A0 BIT_TXPKT_CNT_RPT. 0: r_TX_OK_RTY0 1: r_TX_OK_RTY1 2: r_TX_OK_RTY2 3: r_TX_OK_RTY3 4: r_TX_OK_RTY4 5: r_RTY_DROP 6: r_LT_DROP 7: r_BMC_TXOK 8: r_TOTAL_PKT_CNT 9: r_RTS_DROP 10: r_RTS_OK */
#define BIT_TXPKT_CNT_SEL(x)                           ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_TXPKT_CNT_SEL(x)                       ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_REF_MACID                             ((u32)0x0000000F << 24)          /*!<R/W 0xF  if BIT_REF_MACID = 0xF, count all eligible frames and ignore MACID; else, only count the frames whose MACID = BIT_REF_MACID BMC packet ignore MACID. */
#define BIT_REF_MACID(x)                               ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_REF_MACID(x)                           ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_TX_CNT_CLR                                 ((u32)0x00000001 << 23)          /*!<R/W 0x0  Write 1 to clear the counters. */
#define BIT_USE_NDP_PARAMETER                          ((u32)0x00000001 << 9)          /*!<R/W 0x1  1: select NDP rate from CR 0X149C[8:0] */
#define BIT_MASK_NDP_RATE                              ((u32)0x000001FF << 0)          /*!<R/W 0x04  NDPA tx rate setting, from CCK 1M to VHT NSS2 MCS9 */
#define BIT_NDP_RATE(x)                                ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_NDP_RATE(x)                            ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TXPKT_CNT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TXPKT_CNT_RPT                         ((u32)0xFFFFFFFF << 0)          /*!<RO 0x0  The results of TXPKT counter. Depends on BIT_TXPKT_CNT_SEL. */
#define BIT_TXPKT_CNT_RPT(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_TXPKT_CNT_RPT(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */


/*==========PAGE4 Register Address Definition==========*/
#define REG_Q0_INFO                                  0x0400
#define REG_Q1_INFO                                  0x0404
#define REG_Q2_INFO                                  0x0408
#define REG_Q3_INFO                                  0x040C
#define REG_MGQ_INFO                                 0x0410
#define REG_HIQ_INFO                                 0x0414
#define REG_QUE_EMPTY_BCNQ_INFO                      0x0418
#define REG_CPU_MGQ_INFO                             0x041C
#define REG_FWHW_TXQ_HWSSN_CTRL                      0x0420
#define REG_BNDY_LIFETIME_TAIL                       0x0424
#define REG_RETRY_LIMIT_SIFS                         0x0428
#define REG_TXBF_CTRL                                0x042C
#define REG_DARFRC0_L                                0x0430
#define REG_DARFRC0_H                                0x0434
#define REG_RARFRC_L                                 0x0438
#define REG_RARFRC_H                                 0x043C
#define REG_RRSR                                     0x0440
#define REG_BCN_AMPDU_CTCL                           0x0454
#define REG_AMPDU_MAX_LENGTH                         0x0458
#define REG_NDPA_CTRL_LBK_ACQ_STOP                   0x045C
#define REG_RD_RESP_PKT_TH                           0x0460
#define REG_CMDQ_INFO                                0x0464
#define REG_Q4_INFO                                  0x0468
#define REG_Q5_INFO                                  0x046C
#define REG_Q6_INFO                                  0x0470
#define REG_Q7_INFO                                  0x0474
#define REG_GCLK_CFG                                 0x0478
#define REG_TXRPT_CTRL                               0x047C
#define REG_CFEND_RATE_SC_CTRL                       0x0480
#define REG_MACID_SLEEP3                             0x0484
#define REG_MACID_SLEEP1                             0x0488
#define REG_TXRPT_START_OFFSET                       0x04AC
#define REG_PTA_WL_PRI_MASK                          0x04B4
#define REG_EVTQ_RPT                                 0x04B8
#define REG_SW_AMPDU_BURST_MODE_CTRL                 0x04BC
#define REG_PKT_LIFE_TIME                            0x04C0
#define REG_PTA_STBC_CTRL                            0x04C4
#define REG_PROT_MODE_CTRL                           0x04C8
#define REG_BAR_MODE_CTRL                            0x04CC
#define REG_MACID_SLEEP2                             0x04D0
#define REG_MACID_SLEEP                              0x04D4
#define REG_HW_SEQ0_1                                0x04D8
#define REG_HW_SEQ2_3                                0x04DC
#define REG_PTCLERR_NULLPKT_STATUS                   0x04E0
#define REG_PRETX_DROP_CTRL                          0x04E4
#define REG_BT_POLLUTE_PKT_CNT                       0x04E8
#define REG_DROP_PKT_NUM                             0x04EC
#define REG_CPUMGQ_TIMER_CTRL                        0x04F4
#define REG_DUMMY                                    0x04FC
#define REG_AESIV_SETTING                            0x1424
#define REG_BF0_TIME_SETTING                         0x1428
#define REG_BF1_TIME_SETTING                         0x142C
#define REG_BF_TIMEOUT_EN                            0x1430
#define REG_MACID_RELEASE0                           0x1434
#define REG_MACID_RELEASE1                           0x1438
#define REG_MACID_RELEASE2                           0x143C
#define REG_MACID_RELEASE3                           0x1440
#define REG_MACID_RELEASE_SETTING                    0x1444
#define REG_FAST_EDCA_VOVI_SETTING                   0x1448
#define REG_FAST_EDCA_BEBK_SETTING                   0x144C
#define REG_MACID_DROP0                              0x1450
#define REG_MACID_DROP1                              0x1454
#define REG_MACID_DROP2                              0x1458
#define REG_MACID_DROP3                              0x145C
#define REG_R_MACID_RELEASE_SUCCESS_0                0x1460
#define REG_R_MACID_RELEASE_SUCCESS_1                0x1464
#define REG_R_MACID_RELEASE_SUCCESS_2                0x1468
#define REG_R_MACID_RELEASE_SUCCESS_3                0x146C
#define REG_MGQ_FIFO_CTRL                            0x1470
#define REG_MGQ_FIFO_RELEASE_INT                     0x1474
#define REG_MGQ_FIFO_LIFETIME_VALID                  0x1478
#define REG_R_MACID_RELEASE_SUCCESS_CLEAR_OFFSET     0x147C
#define REG_AX_RESP_RRSR                             0x1480
#define REG_AX_ARFR_WEIGHTING_TABLE                  0x1484
#define REG_DARFRC1_L                                0x1488
#define REG_DARFRC1_H                                0x148C
#define REG_TB_CTRL                                  0x1490
#define REG_OTHER_CFG                                0x1494
#define REG_RRSR_RATE_CFG                            0x1498
#define REG_NDP_RATE                                 0x149C
#define REG_TXPKT_CNT                                0x14A0

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
