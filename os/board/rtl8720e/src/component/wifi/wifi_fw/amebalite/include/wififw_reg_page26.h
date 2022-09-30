#ifndef __WIFIFW_REG_PAGE26_H__
#define __WIFIFW_REG_PAGE26_H__

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_TWT_BC_ID
 * @brief TWT IE ID BITMAP
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BC_TWT_ID           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  TWT ID bitmap, each bit corresponding to a ID value */
#define BIT_R_BC_TWT_ID(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_R_BC_TWT_ID(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TWT_BC_RPT0_PERS
 * @brief TWT IE persistence reference and RPT0
 * @{
 *****************************************************************************/
#define BIT_R_WMAC_BCN_TWT_PSR         ((u32)0x00000001 << 26)          /*!<R/W 0  enable bcn twt bc ie parser */
#define BIT_MASK_R_TWT_PERSISTENCE     ((u32)0x000000FF << 18)          /*!<R/W 0  persistence vlaue, if persistence value in TWT IE is smaller than the reference, bcnpsf will lat the twt id rpt */
#define BIT_R_TWT_PERSISTENCE(x)       ((u32)(((x) & 0x000000FF) << 18))
#define BIT_GET_R_TWT_PERSISTENCE(x)   ((u32)(((x >> 18) & 0x000000FF)))
#define BIT_MASK_TWT_RPT_0             ((u32)0x0000FFFF << 0)          /*!<R/W/ES 0  specified twt id report */
#define BIT_TWT_RPT_0(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TWT_RPT_0(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TWT_BC_RPT1
 * @brief TWT IE ID RPT1
 * @{
 *****************************************************************************/
#define BIT_MASK_TWT_RPT_2             ((u32)0x0000FFFF << 16)          /*!<R/W/ES 0  specified twt id report */
#define BIT_TWT_RPT_2(x)               ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_TWT_RPT_2(x)           ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_TWT_RPT_1             ((u32)0x0000FFFF << 0)          /*!<R/W/ES 0  specified twt id report */
#define BIT_TWT_RPT_1(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TWT_RPT_1(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TWT_BC_RPT2
 * @brief TWT IE ID RPT2
 * @{
 *****************************************************************************/
#define BIT_MASK_TWT_RPT_4             ((u32)0x0000FFFF << 16)          /*!<R/W/ES 0  specified twt id report */
#define BIT_TWT_RPT_4(x)               ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_TWT_RPT_4(x)           ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_MASK_TWT_RPT_3             ((u32)0x0000FFFF << 0)          /*!<R/W/ES 0  specified twt id report */
#define BIT_TWT_RPT_3(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_TWT_RPT_3(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_OPS_PSR_RPT
 * @brief OPS DURATION
 * @{
 *****************************************************************************/
#define BIT_MASK_MBSSID_N_MEM          ((u32)0x000000FF << 8)          /*!<R/W/ES 0  multi-bssid n value */
#define BIT_MBSSID_N_MEM(x)            ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_MBSSID_N_MEM(x)        ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_OPS_DURATION_RPT      ((u32)0x000000FF << 0)          /*!<R/W/ES 0  ops frame duration field rpt */
#define BIT_OPS_DURATION_RPT(x)        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_OPS_DURATION_RPT(x)    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LA_EVENT_OPTION
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_LASTOP_FCS_ERROR    ((u32)0x00000003 << 3)          /*!<R/W 0  stop LA record when continue fcs error ≥ threshold 0：disable 1：4 2：8 3：12 */
#define BIT_R_LASTOP_FCS_ERROR(x)      ((u32)(((x) & 0x00000003) << 3))
#define BIT_GET_R_LASTOP_FCS_ERROR(x)  ((u32)(((x >> 3) & 0x00000003)))
#define BIT_R_LASTOP_RX_FSM_HANG       ((u32)0x00000001 << 2)          /*!<R/W 0  stop LA record when RX FSM timeout */
#define BIT_R_LASTOP_PLCP_ERROR        ((u32)0x00000001 << 1)          /*!<R/W 0  stop LA record when RX PLCP error occur, include Length error, parity error or crc error */
#define BIT_R_LASTOP_RX_INCOMP         ((u32)0x00000001 << 0)          /*!<R/W 0  stop LA record when RX content is incompleted, include PLCP is incompleted or PSDU is incompleted */
/** @} */

/**************************************************************************//**
 * @defgroup REG_RESP_AID_11
 * @brief aid 11 for resp
 * @{
 *****************************************************************************/
#define BIT_MASK_AID11                 ((u32)0x000007FF << 0)          /*!<R/W 0  specified the aid of the sta that mactx responding to */
#define BIT_AID11(x)                   ((u32)(((x) & 0x000007FF) << 0))
#define BIT_GET_AID11(x)               ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MAC_PHY_CR
 * @brief mac phy control reg
 * @{
 *****************************************************************************/
#define BIT_R_ID_MATH_MPDU             ((u32)0x00000001 << 21)          /*!<R/W 0  when set, per mpdu will trigger rx_id_match, else per ppdu trigger rx_id_match */
#define BIT_MASK_R_ID_MATCH_CNT        ((u32)0x0000007F << 14)          /*!<R/W 7'h1f  rxpkt_ok or rxpkt_fail pulse counter,unit 25ns when mac clock is 40MHz */
#define BIT_R_ID_MATCH_CNT(x)          ((u32)(((x) & 0x0000007F) << 14))
#define BIT_GET_R_ID_MATCH_CNT(x)      ((u32)(((x >> 14) & 0x0000007F)))
#define BIT_MASK_R_INVALID_PKT_CNT     ((u32)0x000000FF << 6)          /*!<R/W 8'hf  invalid_pkt pulse counter,unit 25ns when mac clock is 40MHz */
#define BIT_R_INVALID_PKT_CNT(x)       ((u32)(((x) & 0x000000FF) << 6))
#define BIT_GET_R_INVALID_PKT_CNT(x)   ((u32)(((x >> 6) & 0x000000FF)))
#define BIT_MASK_R_RXPKT_OK_FAIL_CNT   ((u32)0x0000003F << 0)          /*!<R/W 6'h9  rxpkt_ok or rxpkt_fail pulse counter,unit 25ns when mac clock is 40MHz */
#define BIT_R_RXPKT_OK_FAIL_CNT(x)     ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_RXPKT_OK_FAIL_CNT(x) ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/*==========PAGE26 Register Address Definition==========*/
#define REG_TWT_BC_ID                                0x2600
#define REG_TWT_BC_RPT0_PERS                         0x2604
#define REG_TWT_BC_RPT1                              0x2608
#define REG_TWT_BC_RPT2                              0x260C
#define REG_OPS_PSR_RPT                              0x2610
#define REG_LA_EVENT_OPTION                          0x2614
#define REG_RESP_AID_11                              0x2618
#define REG_MAC_PHY_CR                               0x261C

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
