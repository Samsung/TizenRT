/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     gap_credit_based_conn.h
* @brief    header file of LE Credit-Based Connection message handle.
* @details  none.
* @author   Tifnan
* @date     2016-03-16
* @version  v0.1
*********************************************************************************************************
*/

#ifndef _LE_CREDIT_BASED_CONN_H_
#define _LE_CREDIT_BASED_CONN_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include "upperstack_config.h"
#include "gap_le.h"
#include "gap.h"

#if F_BT_LE_4_1_COC_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_CREDIT_BASED_CONN GAP LE Credit Based Connection Module
  * @{
  */
typedef enum
{
    GAP_CHANN_STATE_DISCONNECTED, //!< Disconnected.
    GAP_CHANN_STATE_CONNECTING,   //!< Connecting.
    GAP_CHANN_STATE_CONNECTED,    //!< Connected.
    GAP_CHANN_STATE_DISCONNECTING //!< Disconnecting.
} T_GAP_CHANN_STATE;

typedef enum
{
    COC_PARAM_CREDITS_THRESHOLD                = 0x400,
    COC_PARAM_LOCAL_MTU                        = 0x401,
} T_LE_COC_PARAM_TYPE;

typedef enum
{
    COC_CHANN_PARAM_CUR_CREDITS                = 0x410,
    COC_CHANN_PARAM_MAX_CREDITS                = 0x411,
    COC_CHANN_PARAM_MTU                        = 0x412,
} T_LE_COC_CHANN_PARAM_TYPE;

/** @brief Definition of LE security requirement*/
typedef enum
{
    LE_COC_SECURITY_NONE,              /**< Security None */
    LE_COC_SECURITY_UNAUTHEN_ENCRYT,   /**< Security unauthenticated encryption */
    LE_COC_SECURITY_AUTHEN_ENCRYT,     /**< Security authenticated encryption */
    LE_COC_SECURITY_UNAUTHEN_DATA_SIGN,/**< Security unauthenticated data signed */
    LE_COC_SECURITY_AUTHEN_DATA_SIGN,  /**< Security authenticated data signed */
    LE_COC_SECURITY_AUTHOR             /**< Security authorized */
} T_LE_COC_SECURITY_MODE;

typedef struct
{
    uint8_t         conn_id;        /**<  local link ID.  */
    uint16_t        cid;            /**<  channel ID  */
    uint16_t        value_len;      /**<  value length  */
    uint8_t         *p_data;
} T_LE_COC_RECEIVE_DATA;

typedef struct
{
    uint8_t         conn_id;        /**<  local link ID.  */
    uint16_t        cid;            /**<  channel ID  */
    uint16_t        cause;      /**<  value length  */
    uint8_t         credit;
} T_LE_COC_SEND_DATA;

typedef struct
{
    uint8_t         conn_id;
    uint16_t        cid;
    T_GAP_CHANN_STATE conn_state;
    uint16_t        cause;
} T_LE_COC_CHANN_STATE;

/** @brief Response of le credit based security register request.*/
typedef struct
{
    uint16_t        cause;
} T_LE_COC_CREDIT_BASED_SECURITY_REG_RSP;

/** @brief Response of le credit based psm register request.*/
typedef struct
{
    uint16_t        le_psm;
    uint16_t        cause;
} T_LE_COC_CREDIT_BASED_PSM_REG_RSP;

#define GAP_COC_MSG_LE_CHANN_STATE          0x01
#define GAP_COC_MSG_LE_REG_PSM              0x02
#define GAP_COC_MSG_LE_SET_PSM_SECURITY     0x03
#define GAP_COC_MSG_LE_SEND_DATA            0x04
#define GAP_COC_MSG_LE_RECEIVE_DATA         0x05

typedef union
{
    T_LE_COC_CHANN_STATE                            *p_le_chann_state;
    T_LE_COC_RECEIVE_DATA                           *p_le_receive_data;
    T_LE_COC_SEND_DATA                              *p_le_send_data;
    T_LE_COC_CREDIT_BASED_SECURITY_REG_RSP          *p_le_set_psm_security_rsp;
    T_LE_COC_CREDIT_BASED_PSM_REG_RSP               *p_le_reg_psm_rsp;
} T_LE_COC_DATA;

typedef T_APP_RESULT(*P_FUN_LE_COC_APP_CB)(uint8_t coc_type, void *p_coc_data);
/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_CREDIT_BASED_CONN_Exported_Functions GAP LE Credit Based Connection Exported Functions
  * @brief
  * @{
  */
bool        le_coc_init(uint8_t chann_num);
void        le_coc_register_app_cb(P_FUN_LE_COC_APP_CB app_callback);
T_GAP_CAUSE le_coc_set_param(T_LE_COC_PARAM_TYPE param, uint8_t len, void *p_value);
T_GAP_CAUSE le_coc_get_chann_param(T_LE_COC_CHANN_PARAM_TYPE param, void *p_value, uint16_t cid);
T_GAP_CAUSE le_coc_create(uint8_t conn_id, uint16_t le_psm);
T_GAP_CAUSE le_coc_disc(uint16_t cid);
T_GAP_CAUSE le_coc_send_data(uint16_t cid, uint8_t *p_data, uint16_t data_len);
T_GAP_CAUSE le_coc_reg_psm(uint16_t le_psm, uint8_t action);
T_GAP_CAUSE le_coc_set_psm_security(uint16_t le_psm, bool active, T_LE_COC_SECURITY_MODE mode,
                                    uint8_t key_size);
/** End of GAP_LE_CREDIT_BASED_CONN_Exported_Functions
  * @}
  */

/** End of GAP_LE_CREDIT_BASED_CONN
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of GAP
  * @}
  */
#endif


#endif

#endif /* _LE_CREDIT_BASED_CONN_H_ */
