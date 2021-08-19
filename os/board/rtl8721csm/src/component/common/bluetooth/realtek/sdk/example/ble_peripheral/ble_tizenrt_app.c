/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_app.c
   * @brief     This file handles BLE peripheral application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <stdio.h>
#include <string.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include <app_msg.h>
#include "ble_tizenrt_app.h"
#include "ble_tizenrt_app_flags.h"
#include <gap_conn_le.h>
#include "platform_stdlib.h"
#include <ble_tizenrt_service.h>
#include <os_msg.h>
#include <os_mem.h>

/** @defgroup  PERIPH_APP Peripheral Application
    * @brief This file handles BLE peripheral application routines.
    * @{
    */
/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @addtogroup  PERIPH_SEVER_CALLBACK Profile Server Callback Event Handler
    * @brief Handle profile server callback event
    * @{
    */
/** @} */ /* End of group PERIPH_SEVER_CALLBACK */
/** @defgroup  PERIPH_GAP_MSG GAP Message Handler
    * @brief Handle GAP Message
    * @{
    */
T_GAP_DEV_STATE ble_tizenrt_gap_dev_state = {0, 0, 0, 0, 0};                 /**< GAP device state */
T_GAP_CONN_STATE ble_tizenrt_gap_conn_state = GAP_CONN_STATE_DISCONNECTED; /**< GAP connection state */
struct {
    uint8_t remote_bd[TRBLE_BD_ADDR_MAX_LEN];
    T_GAP_REMOTE_ADDR_TYPE remote_bd_type;
} tizenrt_remote_bd_info = {0};

/*============================================================================*
 *                              Functions
 *============================================================================*/
int ble_tizenrt_app_handle_upstream_msg(uint16_t subtype, void *pdata)
{
    debug_print("\r\n[%s] subtype = 0x%x", __FUNCTION__, subtype);
	int ret = 0;
    
	switch (subtype) {
		case BLE_TIZENRT_MSG_START_ADV:
        {
            ret = le_adv_start();
            if(GAP_CAUSE_SUCCESS == ret)
                debug_print("\r\n[Upstream] Start Adv Success", __FUNCTION__);
            else
                debug_print("\r\n[Upstream] Start Adv Fail !!", __FUNCTION__);   
        }
			break;
		case BLE_TIZENRT_MSG_STOP_ADV:
			ret = le_adv_stop();
			break;
        case BLE_TIZENRT_MSG_START_DIRECT_ADV:
        {
            T_TIZENRT_DIRECT_ADV_PARAM *param = pdata;
            if(param)
            {
                uint8_t  adv_evt_type = GAP_ADTYPE_ADV_HDC_DIRECT_IND;
                uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
                uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;

                le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
                le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
                le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, (sizeof(uint8_t)*TRBLE_BD_ADDR_MAX_LEN), param->bd_addr);
                le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
                ret = le_adv_start();
                if(GAP_CAUSE_SUCCESS == ret)
                    debug_print("\r\n[Upstream] Start Direct Adv Success", __FUNCTION__);
                else
                    debug_print("\r\n[Upstream] Start Direct Adv Fail !!", __FUNCTION__);   
                os_mem_free(param);
            } else {
                debug_print("\n[%s] Start_direct_adv parameter is NULL", __FUNCTION__);
            }
        }
			break;
		case BLE_TIZENRT_MSG_DISCONNECT:
			ret = le_disconnect(0);
			break;
		case BLE_TIZENRT_MSG_NOTIFY:
        {
            T_TIZENRT_NOTIFY_PARAM *param = pdata;
            if(param)
            {
                debug_print("\r\n[%s] conn_id %d abs_handle 0x%x data %p", __FUNCTION__,
                                            param->conn_id, param->att_handle, param->data);
                if(tizenrt_ble_service_send_notify(param->conn_id, param->att_handle, param->data, param->len))
                    debug_print("\r\n[%s] success : subtype = 0x%x", __FUNCTION__, subtype);
                else
                    debug_print("\r\n[%s] fail : subtype = 0x%x", __FUNCTION__, subtype);
                os_mem_free(param->data);
                os_mem_free(param);
            } else {
                debug_print("\n[%s] Notify parameter is NULL", __FUNCTION__);
            }
        }
			break;
        case BLE_TIZENRT_MSG_DELETE_BOND:
        {
            T_TIZENRT_SERVER_DELETE_BOND_PARAM *param = pdata;
            if(param)
            {
                param->result = le_bond_delete_by_bd(param->bd_addr, GAP_REMOTE_ADDR_LE_PUBLIC);
                if(GAP_CAUSE_NOT_FIND == param->result)
                    param->result = le_bond_delete_by_bd(param->bd_addr, GAP_REMOTE_ADDR_LE_RANDOM);
                param->flag = true;
            } else {
                debug_print("\n[%s] Delete_bond parameter is NULL", __FUNCTION__);
            }
        }
            break;
        case BLE_TIZENRT_MSG_DELETE_BOND_ALL:
        {
            debug_print("\r\n[%s] le_bond_clear_all_keys", __FUNCTION__);
            le_bond_clear_all_keys();
        }
            break;
		default:
			break;
	}

	return ret;
}

extern void *ble_tizenrt_evt_queue_handle; 
extern void *ble_tizenrt_io_queue_handle; 
bool ble_tizenrt_send_msg(uint16_t sub_type, void *arg)
{
    uint8_t event = EVENT_IO_TO_APP;

    T_IO_MSG io_msg;

    io_msg.type = IO_MSG_TYPE_QDECODE;
    io_msg.subtype = sub_type;
    io_msg.u.buf = arg;

    if (ble_tizenrt_evt_queue_handle != NULL && ble_tizenrt_io_queue_handle != NULL) {
        if (os_msg_send(ble_tizenrt_io_queue_handle, &io_msg, 0) == false) {
            printf("\r\n[%s] send msg fail !!! type = 0x%x", __FUNCTION__, io_msg.subtype);
            return false;
        } else if (os_msg_send(ble_tizenrt_evt_queue_handle, &event, 0) == false) {
            printf("\r\n[%s] send evt fail !!! type = 0x%x", __FUNCTION__, io_msg.subtype);
            return false;
        }
    }
    debug_print("\r\n[%s] success : subtype = 0x%x", __FUNCTION__, sub_type);
    return true;
}

void ble_tizenrt_app_handle_gap_msg(T_IO_MSG  *p_gap_msg);
/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void ble_tizenrt_app_handle_io_msg(T_IO_MSG io_msg)
{
    debug_print("\r\n[%s] io_msg.type = 0x%x", __FUNCTION__, io_msg.type);
    uint16_t msg_type = io_msg.type;
    uint16_t subtype;
	void *arg = NULL;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        {
            ble_tizenrt_app_handle_gap_msg(&io_msg);
        }
        break;
    case IO_MSG_TYPE_QDECODE:
        {
			subtype = io_msg.subtype;
			arg = io_msg.u.buf;
			ble_tizenrt_app_handle_upstream_msg(subtype, arg);
        }
        break;
    default:
        break;
    }
}

extern trble_server_init_config server_init_parm;
void  ble_tizenrt_handle_callback_msg(T_TIZENRT_APP_CALLBACK_MSG callback_msg)
{
    debug_print("\r\n[%s] msg type: 0x%x", __FUNCTION__, callback_msg.type);
    switch (callback_msg.type) {
        case BLE_TIZENRT_CALLBACK_TYPE_CONN:
        {
            T_TIZENRT_CONNECTED_CALLBACK_DATA *connected = callback_msg.u.buf;
            if(connected != NULL && server_init_parm.connected_cb)
            {
                debug_print("\r\n[%s] cb %p conn_id 0x%x conn_type 0x%x addr 0x%x0x%x0x%x0x%x0x%x0x%x",
                                __FUNCTION__, server_init_parm.connected_cb,
                                connected->conn_id, connected->conn_type,
                                connected->remote_bd[0], connected->remote_bd[1], connected->remote_bd[2],
                                connected->remote_bd[3], connected->remote_bd[4], connected->remote_bd[5]);
                trble_server_connected_t p_func = server_init_parm.connected_cb;
                p_func(connected->conn_id, connected->conn_type, connected->remote_bd);
            } else {
                debug_print("\r\n[%s] NULL connected callback", __FUNCTION__);
            }
            os_mem_free(connected);
        }
		    break;
        case BLE_TIZENRT_CALLBACK_TYPE_PROFILE:
        {
            T_TIZENRT_PROFILE_CALLBACK_DATA *profile = callback_msg.u.buf;
            if(profile != NULL && profile->cb)
            {
                debug_print("\r\n[%s] Profile callback", __FUNCTION__);
                trble_server_cb_t pfunc = profile->cb;
                pfunc(profile->type, profile->conn_id, profile->att_handle, profile->arg);
            } else {
                debug_print("\r\n[%s] NULL profile callback", __FUNCTION__);
            }
            os_mem_free(profile);
        }
		    break;
        default:
			break;
	}
}

extern void *ble_tizenrt_callback_queue_handle;
bool ble_tizenrt_send_callback_msg(uint16_t type, void *arg)                                                        
{
    T_TIZENRT_APP_CALLBACK_MSG callback_msg;
    callback_msg.type = type;
    callback_msg.u.buf = arg;
    if (ble_tizenrt_callback_queue_handle != NULL) {
		if (os_msg_send(ble_tizenrt_callback_queue_handle, &callback_msg, 0) == false) {
			printf("\r\n[%s] fail!!! msg_type 0x%x", __FUNCTION__, callback_msg.type);
            return false;
		} else {
            debug_print("\r\n[%s] success msg_type 0x%x", __FUNCTION__, callback_msg.type);
        }
	} else {
        debug_print("\r\n[%s] ble_tizenrt_callback_queue_handle is NULL", __FUNCTION__);
        return false;
    }
    return true;
}
/**
 * @brief    Handle msg GAP_MSG_LE_DEV_STATE_CHANGE
 * @note     All the gap device state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] new_state  New gap device state
 * @param[in] cause GAP device state change cause
 * @return   void
 */
void ble_tizenrt_app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    debug_print("\r\n[%s] init state %d, adv state %d, cause 0x%x", __FUNCTION__,
                    new_state.gap_init_state, new_state.gap_adv_state, cause);
    if (ble_tizenrt_gap_dev_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
			printf("\n\r[BLE Tizenrt] GAP stack ready\n\r");
            /*stack ready*/
        }
    }

    if (ble_tizenrt_gap_dev_state.gap_adv_state != new_state.gap_adv_state)
    {
        if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        {
            if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
            {
                printf("\r\n[BLE Tizenrt] GAP adv stoped: because connection created");
            }
            else
            {
                printf("\r\n[BLE Tizenrt] GAP adv stopped");
            }
        }
        else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            printf("\r\n[BLE Tizenrt] GAP adv start");
        }
    }

    ble_tizenrt_gap_dev_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_STATE_CHANGE
 * @note     All the gap conn state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New gap connection state
 * @param[in] disc_cause Use this cause when new_state is GAP_CONN_STATE_DISCONNECTED
 * @return   void
 */
void ble_tizenrt_app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
{
    debug_print("\r\n[%s] conn_id %d old_state %d new_state %d, disc_cause 0x%x", __FUNCTION__,
                    conn_id, ble_tizenrt_gap_conn_state, new_state, disc_cause);
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {
                printf("\r\n[BLE Tizenrt] connection lost cause 0x%x", disc_cause);
            }
			printf("\r\n[BLE Tizenrt] BT Disconnected");

            T_TIZENRT_CONNECTED_CALLBACK_DATA *disconn_data = os_mem_alloc(0, sizeof(T_TIZENRT_CONNECTED_CALLBACK_DATA));
            if(disconn_data)
            {
                disconn_data->conn_id = conn_id;
                disconn_data->conn_type = TRBLE_SERVER_DISCONNECTED;
                memcpy(disconn_data->remote_bd, tizenrt_remote_bd_info.remote_bd, TRBLE_BD_ADDR_MAX_LEN);
                memset(tizenrt_remote_bd_info.remote_bd, 0, TRBLE_BD_ADDR_MAX_LEN);
                //le_adv_start();
                if(ble_tizenrt_send_callback_msg(BLE_TIZENRT_CALLBACK_TYPE_CONN, disconn_data) == false)
                {
                    os_mem_free(disconn_data);
                    debug_print("\r\n[%s] callback msg send fail", __FUNCTION__);
                }
            } else {
                debug_print("\n[%s] Memory allocation failed", __FUNCTION__);
            }
        }
        break;

    case GAP_CONN_STATE_CONNECTED:
        {
            uint16_t conn_interval;
            uint16_t conn_latency;
            uint16_t conn_supervision_timeout;

            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
            le_get_conn_addr(conn_id, tizenrt_remote_bd_info.remote_bd, &tizenrt_remote_bd_info.remote_bd_type);
            debug_print("\r\n[%s] GAP_CONN_STATE_CONNECTED:remote_bd %s, remote_addr_type %d, conn_interval 0x%x, conn_latency 0x%x, conn_supervision_timeout 0x%x",
                                            __FUNCTION__, tizenrt_remote_bd_info.remote_bd, tizenrt_remote_bd_info.remote_bd_type,
                                            conn_interval, conn_latency, conn_supervision_timeout);
			printf("\r\n[BLE Tizenrt] BT Connected");

            T_TIZENRT_CONNECTED_CALLBACK_DATA *conn_data = os_mem_alloc(0, sizeof(T_TIZENRT_CONNECTED_CALLBACK_DATA));
            if(conn_data)
            {
                conn_data->conn_id = conn_id;
                conn_data->conn_type = TRBLE_SERVER_LL_CONNECTED;
                memcpy(conn_data->remote_bd, tizenrt_remote_bd_info.remote_bd, TRBLE_BD_ADDR_MAX_LEN);
                debug_print("\r\n[%s] conn_id 0x%x cont_type 0x%x addr 0x%x0x%x0x%x0x%x0x%x0x%x", __FUNCTION__,
                                conn_data->conn_id, conn_data->conn_type,
                                conn_data->remote_bd[5], conn_data->remote_bd[4], conn_data->remote_bd[3],
                                conn_data->remote_bd[2], conn_data->remote_bd[2], conn_data->remote_bd[0]);
                if(ble_tizenrt_send_callback_msg(BLE_TIZENRT_CALLBACK_TYPE_CONN, conn_data) == false)
                {
                    os_mem_free(conn_data);
                    debug_print("\r\n[%s] callback msg send fail", __FUNCTION__);
                }
            } else {
                debug_print("\n[%s] Memory allocation failed", __FUNCTION__);
            }
        }
        break;

    default:
        break;
    }
    ble_tizenrt_gap_conn_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_AUTHEN_STATE_CHANGE
 * @note     All the gap authentication state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New authentication state
 * @param[in] cause Use this cause when new_state is GAP_AUTHEN_STATE_COMPLETE
 * @return   void
 */
void ble_tizenrt_app_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    debug_print("\r\n[%s] app_handle_authen_state_evt:conn_id %d, cause 0x%x", __FUNCTION__, conn_id, cause);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
        {
            debug_print("\r\n[%s] app_handle_authen_state_evt: GAP_AUTHEN_STATE_STARTED", __FUNCTION__);
        }
        break;

    case GAP_AUTHEN_STATE_COMPLETE:
        {
            if (cause == GAP_SUCCESS)
            {
                printf("\r\n[BLE Tizenrt] Pair success");
                debug_print("\r\n[%s] GAP_AUTHEN_STATE_COMPLETE pair success", __FUNCTION__);

                T_TIZENRT_CONNECTED_CALLBACK_DATA *authed_data = os_mem_alloc(0, sizeof(T_TIZENRT_CONNECTED_CALLBACK_DATA));
                if(authed_data)
                {
                    authed_data->conn_id = conn_id;
                    authed_data->conn_type = TRBLE_SERVER_SM_CONNECTED;
                    memcpy(authed_data->remote_bd, tizenrt_remote_bd_info.remote_bd, TRBLE_BD_ADDR_MAX_LEN);
                    if(ble_tizenrt_send_callback_msg(BLE_TIZENRT_CALLBACK_TYPE_CONN, authed_data) == false)
                    {
                        os_mem_free(authed_data);
                        debug_print("\r\n[%s] callback msg send fail", __FUNCTION__);
                    }
                } else {
                    debug_print("\n[%s] Memory allocation failed", __FUNCTION__);
                }
            }
            else
            {
                printf("\r\n[BLE Tizenrt] Pair failed: cause 0x%x", cause);
                debug_print("\r\n[%s] GAP_AUTHEN_STATE_COMPLETE pair failed", __FUNCTION__);
            }
        }
        break;

    default:
        {
            printf("\r\n[%s] unknown newstate %d", __FUNCTION__, new_state);
        }
        break;
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_MTU_INFO
 * @note     This msg is used to inform APP that exchange mtu procedure is completed.
 * @param[in] conn_id Connection ID
 * @param[in] mtu_size  New mtu size
 * @return   void
 */
void ble_tizenrt_app_handle_conn_mtu_info_evt(uint8_t conn_id, uint16_t mtu_size)
{
    printf("\r\n[%s] conn_id %d, mtu_size %d", __FUNCTION__, conn_id, mtu_size);
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_PARAM_UPDATE
 * @note     All the connection parameter update change  events are pre-handled in this function.
 * @param[in] conn_id Connection ID
 * @param[in] status  New update state
 * @param[in] cause Use this cause when status is GAP_CONN_PARAM_UPDATE_STATUS_FAIL
 * @return   void
 */
void ble_tizenrt_app_handle_conn_param_update_evt(uint8_t conn_id, uint8_t status, uint16_t cause)
{
    switch (status)
    {
    case GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS:
        {
            uint16_t conn_interval;
            uint16_t conn_slave_latency;
            uint16_t conn_supervision_timeout;

            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_slave_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
            printf("\r\n[BLE Tizenrt] conn param update success:conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x",
                            conn_interval, conn_slave_latency, conn_supervision_timeout);
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
        {
            printf("\r\n[BLE Tizenrt] conn param update failed: cause 0x%x", cause);	
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
        {
		    printf("\r\n[BLE Tizenrt] conn param update pending: conn_id %d\r\n", conn_id);
        }
        break;

    default:
        break;
    }
}

/**
 * @brief    All the BT GAP MSG are pre-handled in this function.
 * @note     Then the event handling function shall be called according to the
 *           subtype of T_IO_MSG
 * @param[in] p_gap_msg Pointer to GAP msg
 * @return   void
 */
void ble_tizenrt_app_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    debug_print("\r\n[%s] subtype %d", __FUNCTION__, p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
        {
            ble_tizenrt_app_handle_dev_state_evt(gap_msg.msg_data.gap_dev_state_change.new_state,
                                     gap_msg.msg_data.gap_dev_state_change.cause);
        }
        break;

    case GAP_MSG_LE_CONN_STATE_CHANGE:
        {
            ble_tizenrt_app_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                      (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                      gap_msg.msg_data.gap_conn_state_change.disc_cause);
        }
        break;

    case GAP_MSG_LE_CONN_MTU_INFO:
        {
            ble_tizenrt_app_handle_conn_mtu_info_evt(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                         gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
        }
        break;

    case GAP_MSG_LE_CONN_PARAM_UPDATE:
        {
            ble_tizenrt_app_handle_conn_param_update_evt(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                             gap_msg.msg_data.gap_conn_param_update.status,
                                             gap_msg.msg_data.gap_conn_param_update.cause);
        }
        break;

    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
        {
            ble_tizenrt_app_handle_authen_state_evt(gap_msg.msg_data.gap_authen_state.conn_id,
                                        gap_msg.msg_data.gap_authen_state.new_state,
                                        gap_msg.msg_data.gap_authen_state.status);
        }
        break;

    case GAP_MSG_LE_BOND_JUST_WORK:
        {
            conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
            le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            printf("\r\n[%s] GAP_MSG_LE_BOND_JUST_WORK", __FUNCTION__);
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
		    printf("\r\n[%s] GAP_MSG_LE_BOND_PASSKEY_DISPLAY:passkey %d", __FUNCTION__, display_value);
        }
        break;

    case GAP_MSG_LE_BOND_USER_CONFIRMATION:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            printf("\r\n[%s] GAP_MSG_LE_BOND_USER_CONFIRMATION: passkey %d", __FUNCTION__, display_value);
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_INPUT:
        {
            conn_id = gap_msg.msg_data.gap_bond_passkey_input.conn_id;
		    printf("\r\n[%s] GAP_MSG_LE_BOND_PASSKEY_INPUT: conn_id %d", __FUNCTION__, conn_id);
        }
        break;
#if F_BT_LE_SMP_OOB_SUPPORT
    case GAP_MSG_LE_BOND_OOB_INPUT:
        {
            uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
            printf("\r\n[%s] GAP_MSG_LE_BOND_OOB_INPUT", __FUNCTION__);
            le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
            le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        }
        break;
#endif
    default:
        printf("\r\n[%s] unknown subtype %d", __FUNCTION__, p_gap_msg->subtype);
        break;
    }
}
/** @} */ /* End of group PERIPH_GAP_MSG */

/** @defgroup  PERIPH_GAP_CALLBACK GAP Callback Event Handler
    * @brief Handle GAP callback event
    * @{
    */
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT ble_tizenrt_app_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

    switch (cb_type)
    {
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
    case GAP_MSG_LE_DATA_LEN_CHANGE_INFO:
        printf("\r\n[%s] GAP_MSG_LE_DATA_LEN_CHANGE_INFO: conn_id %d, tx octets 0x%x, max_tx_time 0x%x", __FUNCTION__,
                        p_data->p_le_data_len_change_info->conn_id,
                        p_data->p_le_data_len_change_info->max_tx_octets,
                        p_data->p_le_data_len_change_info->max_tx_time);
        break;
#endif
    case GAP_MSG_LE_MODIFY_WHITE_LIST:
        printf("\r\n[%s] GAP_MSG_LE_MODIFY_WHITE_LIST: operation %d, cause 0x%x", __FUNCTION__,
                        p_data->p_le_modify_white_list_rsp->operation,
                        p_data->p_le_modify_white_list_rsp->cause);
        break;

    case GAP_MSG_LE_ADV_UPDATE_PARAM:
        printf("\r\n[%s] GAP_MSG_LE_ADV_UPDATE_PARAM: cause 0x%x", __FUNCTION__,
                        p_data->p_le_adv_update_param_rsp->cause);
        break;
    default:
        debug_print("\r\n[%s] unhandled cb_type 0x%x", __FUNCTION__, cb_type);
        break;
    }
    return result;
}
/** @} */ /* End of group PERIPH_GAP_CALLBACK */

/** @defgroup  PERIPH_SEVER_CALLBACK Profile Server Callback Event Handler
    * @brief Handle profile server callback event
    * @{
    */
/**
    * @brief    All the BT Profile service callback events are handled in this function
    * @note     Then the event handling function shall be called according to the
    *           service_id
    * @param    service_id  Profile service ID
    * @param    p_data      Pointer to callback data
    * @return   T_APP_RESULT, which indicates the function call is successful or not
    * @retval   APP_RESULT_SUCCESS  Function run successfully
    * @retval   others              Function run failed, and return number indicates the reason
    */

extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];
T_APP_RESULT ble_tizenrt_app_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == SERVICE_PROFILE_GENERAL_ID)
    {
        T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
        switch (p_param->eventId)
        {
        case PROFILE_EVT_SRV_REG_COMPLETE:
            debug_print("\r\n[%s] PROFILE_EVT_SRV_REG_COMPLETE: result %d", __FUNCTION__,
                            p_param->event_data.service_reg_result);
            break;

        case PROFILE_EVT_SEND_DATA_COMPLETE:
            debug_print("\r\n[%s] PROFILE_EVT_SEND_DATA_COMPLETE: conn_id %d, cause 0x%x, service_id %d, attrib_idx 0x%x, credits %d",
                            __FUNCTION__,
                            p_param->event_data.send_data_result.conn_id,
                            p_param->event_data.send_data_result.cause,
                            p_param->event_data.send_data_result.service_id,
                            p_param->event_data.send_data_result.attrib_idx,
                            p_param->event_data.send_data_result.credits);
            if (p_param->event_data.send_data_result.cause == GAP_SUCCESS)
            {
                debug_print("\r\n[%s] PROFILE_EVT_SEND_DATA_COMPLETE success", __FUNCTION__);
            }
            else
            {
                printf("\r\n[%s] PROFILE_EVT_SEND_DATA_COMPLETE failed", __FUNCTION__);
            }
            break;

        default:
            break;
        }
    } else {
        TIZENRT_CALLBACK_DATA *p_tizenrt_cb_data = (TIZENRT_CALLBACK_DATA *)p_data;
        TIZENERT_CHA_INFO *p_cha_info = &tizenrt_ble_srv_database[p_tizenrt_cb_data->srv_index].chrc_info[p_tizenrt_cb_data->att_index];

        switch (p_tizenrt_cb_data->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
            {
                debug_print("\r\n[%s] service_id 0x%x att_handle 0x%x", __FUNCTION__, 
                                            tizenrt_ble_srv_database[p_tizenrt_cb_data->srv_index].srv_id, 
                                            p_cha_info->abs_handle);                       
                if(p_tizenrt_cb_data->val & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
                {
                    printf("\r\n[%s] cccd 0x%x update : notify enable", __FUNCTION__, p_cha_info->abs_handle);
                } else {
                    printf("\r\n[%s] cccd 0x%x update : notify disable", __FUNCTION__, p_cha_info->abs_handle);
                }
                if(p_tizenrt_cb_data->val & GATT_CLIENT_CHAR_CONFIG_INDICATE)
                {
                    printf("\r\n[%s] cccd 0x%x update : indicate enable", __FUNCTION__, p_cha_info->abs_handle);
                } else {
                    printf("\r\n[%s] cccd 0x%x update : indicate disable", __FUNCTION__, p_cha_info->abs_handle);
                }
                break;
            }

        case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE:
            {
                debug_print("\r\n[%s] service_id 0x%x index 0x%x abs_handle 0x%x", __FUNCTION__, 
                                            tizenrt_ble_srv_database[p_tizenrt_cb_data->srv_index].srv_id,
                                            p_cha_info->index,
                                            p_cha_info->abs_handle);
                /* call user defined callback */
                if (p_cha_info->cb)
                {
                    trble_server_cb_t p_func = p_cha_info->cb;
                    p_func(TRBLE_ATTR_CB_READING, p_tizenrt_cb_data->conn_id,
                                                            p_cha_info->abs_handle, p_cha_info->arg);
                } else {
                    debug_print("\r\n[%s] NULL read callback abs_handle 0x%x", __FUNCTION__, p_cha_info->abs_handle);
                }
                break;
            }

        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
            {
                debug_print("\r\n[%s] service_id 0x%x Attribute 0x%x write_type %d len %d data 0x", __FUNCTION__,
                                tizenrt_ble_srv_database[p_tizenrt_cb_data->srv_index].srv_id, 
                                p_cha_info->abs_handle,
                                p_tizenrt_cb_data->val,
                                p_cha_info->data_len);
                for (int i = 0; i < p_cha_info->data_len; i++)
                {
                    debug_print("%x", *(p_cha_info->data + i));
                }
                switch (p_tizenrt_cb_data->val)
                {
                case WRITE_REQUEST:
                    {
                        /* call user defined callback */
                        if (p_cha_info->cb)
                        {
                            trble_server_cb_t p_func = p_cha_info->cb;
                            p_func(TRBLE_ATTR_CB_WRITING, p_tizenrt_cb_data->conn_id,
                                                                    p_cha_info->abs_handle, p_cha_info->arg);
                        } else {
                            debug_print("\r\n[%s] NULL write callback abs_handle 0x%x", __FUNCTION__, p_cha_info->abs_handle);
                        }
                        break;
                    }
                case WRITE_WITHOUT_RESPONSE:
                    {
                        /* call user defined callback */
                        if (p_cha_info->cb)
                        {
                            trble_server_cb_t p_func = p_cha_info->cb;
                            p_func(TRBLE_ATTR_CB_WRITING_NO_RSP, p_tizenrt_cb_data->conn_id,
                                                                    p_cha_info->abs_handle, p_cha_info->arg);
                        } else {
                            debug_print("\r\n[%s] NULL write callback abs_handle 0x%x", __FUNCTION__, p_cha_info->abs_handle);
                        }
                        break;
                    }
                default:
                    break;
                }
                break;                                         
            }

        default:
            break;
        }
    }
    return app_result;
}

/** @} */ /* End of group PERIPH_SEVER_CALLBACK */
/** @} */ /* End of group PERIPH_APP */
