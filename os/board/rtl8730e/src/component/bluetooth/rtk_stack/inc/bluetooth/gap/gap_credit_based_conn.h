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
    LE_COC_SECURITY_AUTHOR,            /**< Security authorized */
    LE_COC_SECURITY_SC_UNAUTHEN,       /**< Security unauthenticated LE secure connection */
    LE_COC_SECURITY_SC_AUTHEN          /**< Security LE secure connection */
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
/**
 * @brief       Initialize the number of LE Connection Oriented Channels.
 *
 * @param[in]   chann_num  Channel number.
 * @return Initialize result.
 * @retval true  Initialize success.
 * @retval false Initialize failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool ret = le_coc_init(chann_num);
        le_coc_register_app_cb(app_credit_based_conn_callback);
    }
 * \endcode
 */
bool        le_coc_init(uint8_t chann_num);

/**
 * @brief       Register the callback function of LE Connection Oriented Channels.
 *
 * @param[in]   app_callback  Callback function.
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool ret = le_coc_init(chann_num);
        le_coc_register_app_cb(app_credit_based_conn_callback);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type = %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_CHANN_STATE:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_CHANN_STATE: conn_id %d, cid 0x%x, conn_state %d, cause 0x%x",
                            cb_data.p_le_chann_state->conn_id,
                            cb_data.p_le_chann_state->cid,
                            cb_data.p_le_chann_state->conn_state,
                            cb_data.p_le_chann_state->cause);
            if (cb_data.p_le_chann_state->conn_state == GAP_CHANN_STATE_CONNECTED)
            {
                uint16_t mtu;
                le_coc_get_chann_param(COC_CHANN_PARAM_MTU, &mtu, cb_data.p_le_chann_state->cid);
                APP_PRINT_INFO1("GAP_CHANN_STATE_CONNECTED: mtu %d", mtu);
            }
            break;

        case GAP_COC_MSG_LE_RECEIVE_DATA:
            APP_PRINT_INFO3("GAP_COC_MSG_LE_RECEIVE_DATA: conn_id %d, cid 0x%x, value_len %d",
                            cb_data.p_le_receive_data->conn_id,
                            cb_data.p_le_receive_data->cid,
                            cb_data.p_le_receive_data->value_len);
            break;

        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
void        le_coc_register_app_cb(P_FUN_LE_COC_APP_CB app_callback);

/**
 * @brief       Set parameters of LE Connection Oriented Channel.
 *
 * @param[in]   param    Parameter type: @ref T_LE_COC_PARAM_TYPE.
 * @param[in]   len      Parameter length.
 * @param[in]   p_value  Parameter value.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_set_param(param, len, p_value);
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_set_param(T_LE_COC_PARAM_TYPE param, uint8_t len, void *p_value);

/**
 * @brief       Get parameters of LE Connection Oriented Channel.
 *
 * @param[in]       param    Parameter type: @ref T_LE_COC_CHANN_PARAM_TYPE.
 * @param[in, out]  p_value  Parameter value.
 * @param[in]       cid      Channel cid.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_get_chann_param(param, p_value, cid);
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_get_chann_param(T_LE_COC_CHANN_PARAM_TYPE param, void *p_value, uint16_t cid);

/**
 * @brief       Create LE Connection Oriented Channel.
 *
 * @param[in]   conn_id   Connection id.
 * @param[in]   le_psm    LE PSM.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_create(conn_id, le_psm);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type = %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_CHANN_STATE:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_CHANN_STATE: conn_id %d, cid 0x%x, conn_state %d, cause 0x%x",
                            cb_data.p_le_chann_state->conn_id,
                            cb_data.p_le_chann_state->cid,
                            cb_data.p_le_chann_state->conn_state,
                            cb_data.p_le_chann_state->cause);
            if (cb_data.p_le_chann_state->conn_state == GAP_CHANN_STATE_CONNECTED)
            {
                uint16_t mtu;
                le_coc_get_chann_param(COC_CHANN_PARAM_MTU, &mtu, cb_data.p_le_chann_state->cid);
                APP_PRINT_INFO1("GAP_CHANN_STATE_CONNECTED: mtu %d", mtu);
            }
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_create(uint8_t conn_id, uint16_t le_psm);

/**
 * @brief       Disconnect LE Connection Oriented Channel.
 *
 * @param[in]   cid   CID.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_disc(cid);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type = %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_CHANN_STATE:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_CHANN_STATE: conn_id %d, cid 0x%x, conn_state %d, cause 0x%x",
                            cb_data.p_le_chann_state->conn_id,
                            cb_data.p_le_chann_state->cid,
                            cb_data.p_le_chann_state->conn_state,
                            cb_data.p_le_chann_state->cause);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_disc(uint16_t cid);

/**
 * @brief       Send data by Connection Oriented Channel.
 *
 * @param[in]   cid       CID.
 * @param[in]   p_data    The pointer for data will be sent.
 * @param[in]   data_len  Data length.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_send_data(cid, p_data, data_len);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type = %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_SEND_DATA:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_SEND_DATA: conn_id %d, cid 0x%x, cause 0x%x, credit %d",
                            cb_data.p_le_send_data->conn_id,
                            cb_data.p_le_send_data->cid,
                            cb_data.p_le_send_data->cause,
                            cb_data.p_le_send_data->credit);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_send_data(uint16_t cid, uint8_t *p_data, uint16_t data_len);

/**
 * @brief       Register/Deregister LE PSM.
 *
 * @param[in]   le_psm   LE PSM.
 * @param[in]   action   The action for LE PSM.
 * \arg    1     Register PSM.
 * \arg    0     Deregister PSM.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_reg_psm(le_psm, action);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type = %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_REG_PSM:
            APP_PRINT_INFO2("GAP_COC_MSG_LE_REG_PSM: le_psm 0x%x, cause 0x%x",
                            cb_data.p_le_reg_psm_rsp->le_psm,
                            cb_data.p_le_reg_psm_rsp->cause);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_reg_psm(uint16_t le_psm, uint8_t action);

/**
 * @brief       Register PSM for LE Connection Oriented Channels.
 *
 * @param[in]   le_psm    LE PSM.
 * @param[in]   active    Whether to allocate secure entry.
 * \arg    true     Allocate secure entry.
 * \arg    false    Release secure entry.
 * @param[in]   mode      COC security mode: @ref T_LE_COC_SECURITY_MODE.
 * @param[in]   key_size  Secure key size.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_set_psm_security(le_psm, active, mode, key_size);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type = %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_SET_PSM_SECURITY:
            APP_PRINT_INFO1("GAP_COC_MSG_LE_SET_PSM_SECURITY: cause 0x%x",
                            cb_data.p_le_set_psm_security_rsp->cause);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
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



#endif /* _LE_CREDIT_BASED_CONN_H_ */
