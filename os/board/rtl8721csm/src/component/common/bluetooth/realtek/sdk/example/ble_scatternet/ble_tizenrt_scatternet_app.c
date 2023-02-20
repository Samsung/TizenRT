/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_scatternet_app.c
   * @brief     This file handles BLE central application routines.
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
#include <app_msg.h>
#include <string.h>
#include <trace_app.h>
#include <gap_adv.h>
#include <gap_scan.h>
#include <gap.h>
#include <gap_msg.h>
#include <gap_bond_le.h>
#include <gap_le.h>
#include <ble_tizenrt_scatternet_app.h>
#include <ble_tizenrt_scatternet_link_mgr.h>
#include <ble_tizenrt_central_client_app.h>
#include "ble_tizenrt_app.h"
#include "profile_server.h"
#include <ble_tizenrt_service.h>
#include <user_cmd_parse.h>
#include <gcs_client.h>
#include "platform_opts_bt.h"
#include "data_uart.h"
#include "os_msg.h"
#include "os_mem.h"
#include "os_sync.h"

extern trble_server_init_config server_init_parm;
extern trble_client_init_config *client_init_parm;
extern uint8_t g_master_link_num;
extern uint8_t ble_app_link_table_size;
extern void *ble_tizenrt_read_sem;
extern void *ble_tizenrt_write_sem;
extern void *ble_tizenrt_write_no_rsp_sem;
#if defined(CONFIG_BLE_INDICATION)
extern void *ble_tizenrt_indicate_sem;
#endif
extern BLE_TIZENRT_BOND_REQ *ble_tizenrt_bond_req_info;
extern uint8_t ble_client_connect_is_running;
extern void *ble_tizenrt_modify_whitelist_sem;
extern uint8_t modify_whitelist_code;


T_CLIENT_ID   ble_tizenrt_scatternet_gcs_client_id;         /**< General Common Services client client id*/
T_GAP_DEV_STATE ble_tizenrt_scatternet_gap_dev_state = {0, 0, 0, 0, 0};                /**< GAP device state */
BLE_TIZENRT_SCATTERNET_APP_LINK ble_tizenrt_scatternet_app_link_table[BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS] = {0};
T_TIZENRT_CLIENT_READ_RESULT ble_tizenrt_scatternet_read_results[BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS] = {0};

/*============================================================================*
 *                              Functions
 *============================================================================*/

void ble_tizenrt_scatternet_handle_callback_msg(T_TIZENRT_APP_CALLBACK_MSG callback_msg)
{
    switch (callback_msg.type) {
        case BLE_TIZENRT_BONDED_MSG:
        {
            debug_print("Handle bond msg \n");
            trble_device_connected *bonded_dev = callback_msg.u.buf;
            if(bonded_dev)
            {
                debug_print("SM connected %d \n", bonded_dev->conn_handle);
                client_init_parm->trble_device_connected_cb(bonded_dev);
                os_mem_free(bonded_dev);
            } else {
                debug_print("Bonded parameter is NULL \n");
            } 
        }
		    break;
                
		case BLE_TIZENRT_CONNECTED_MSG:
		{
            debug_print("Handle connected_dev msg \n");
            trble_device_connected *connected_dev = callback_msg.u.buf;
            if(connected_dev)
            {
                debug_print("is_boned %x conn_id %d conn_interval 0x%x latency 0x%x mtu 0x%x \n",
                        connected_dev->is_bonded, connected_dev->conn_handle,
                        connected_dev->conn_info.conn_interval, connected_dev->conn_info.slave_latency, connected_dev->conn_info.mtu);
                debug_print("DestAddr: 0x%02X:0x%02X:0x%02X:0x%02X:0x%02X:0x%02X \n", 
                            connected_dev->conn_info.addr.mac[5], connected_dev->conn_info.addr.mac[4], connected_dev->conn_info.addr.mac[3],
                            connected_dev->conn_info.addr.mac[2], connected_dev->conn_info.addr.mac[1], connected_dev->conn_info.addr.mac[0]);
                if(!memcmp(ble_tizenrt_bond_req_info->addr, connected_dev->conn_info.addr.mac, GAP_BD_ADDR_LEN))
                {
                    debug_print("find conn handle \n");
                    if(ble_tizenrt_bond_req_info->is_secured_connect && (ble_tizenrt_scatternet_app_link_table[connected_dev->conn_handle].auth_state != GAP_AUTHEN_STATE_COMPLETE))
                    {
                        trble_conn_handle *conn_id = os_mem_alloc(0, sizeof(trble_conn_handle));
                        if(conn_id == NULL)
                        {
                            debug_print("\r\n[%s] Memory allocation failed", __FUNCTION__);
                        } else {
                            *conn_id = (trble_conn_handle) connected_dev->conn_handle;
                            debug_print("\r\n[%s] LL connected %d, need pairing", __FUNCTION__, connected_dev->conn_handle);
                            if(ble_tizenrt_scatternet_send_msg(BLE_TIZENRT_BOND, conn_id) == false)
                            {
                                os_mem_free(conn_id);
                                debug_print("\r\n[%s] msg send fail", __FUNCTION__);
                            }
                        }
                        if(ble_client_connect_is_running)
                            ble_client_connect_is_running = 0;
                    } else {
                        debug_print("LL connected %d, do not need pairing \n", connected_dev->conn_handle);
                        if(ble_client_connect_is_running)
                            ble_client_connect_is_running = 0;
                        client_init_parm->trble_device_connected_cb(connected_dev);
                    }
                }
                os_mem_free(connected_dev);
            } else {
                debug_print("Connected parameter is NULL \n");
            }
        }
		    break;

        case BLE_TIZENRT_SCAN_STATE_MSG:
        {
            debug_print("Handle scan_state msg \n");
            trble_scan_state_e scan_state = 0;
            uint16_t new_state = (uint32_t) callback_msg.u.buf;
            if(GAP_SCAN_STATE_IDLE == new_state)
            {
                scan_state = TRBLE_SCAN_STOPPED;
                debug_print("SCAN_STOPPED \n");
            } else if(GAP_SCAN_STATE_SCANNING == new_state) {
                scan_state = TRBLE_SCAN_STARTED;
                debug_print("SCAN_STARTED \n");
            }
            client_init_parm->trble_scan_state_changed_cb(scan_state);
        }
			break;

        case BLE_TIZENRT_SCANNED_DEVICE_MSG:
        {
#if defined(CONFIG_DEBUG_SCAN_INFO)
            debug_print("Handle scanned_device msg \n");
#endif
            trble_scanned_device *scanned_device = callback_msg.u.buf;
            if(scanned_device)
            {
                client_init_parm->trble_device_scanned_cb(scanned_device);
                os_mem_free(scanned_device);
            } else {
                debug_print("Scanned_device parameter is NULL \n");
            }   
        }
			break;

        case BLE_TIZENRT_DISCONNECTED_MSG:
        {
            debug_print("Handle disconnected msg \n");
            if(ble_client_connect_is_running)
                ble_client_connect_is_running = 0;

            if(ble_tizenrt_read_sem != NULL)
                os_mutex_give(ble_tizenrt_read_sem);
            if(ble_tizenrt_write_sem != NULL)
                os_mutex_give(ble_tizenrt_write_sem);
            if(ble_tizenrt_write_no_rsp_sem != NULL)
                os_mutex_give(ble_tizenrt_write_no_rsp_sem);
            trble_conn_handle disconnected = (uint32_t) callback_msg.u.buf;
            client_init_parm->trble_device_disconnected_cb(disconnected);
        }
			break;

        case BLE_TIZENRT_NOTIFICATION_MSG:
        {
            debug_print("Handle notify_result msg \n");
            T_TIZENRT_CLIENT_NOTIFICATION *notify_result = callback_msg.u.buf;
            if(notify_result)
            {
                client_init_parm->trble_operation_notification_cb(&notify_result->handle, &notify_result->noti_data);
                os_mem_free(notify_result->noti_data.data);
                os_mem_free(notify_result);
            } else {
                debug_print("Notify_result parameter is NULL \n");
            }
        }
			break;

        case BLE_TIZENRT_INDICATION_MSG:
        {
            debug_print("Handle indicate_result msg \n");
            T_TIZENRT_CLIENT_INDICATION *indicate_result = callback_msg.u.buf;
            if(indicate_result)
            {
                client_init_parm->trble_operation_indication_cb(&indicate_result->handle, &indicate_result->indi_data);
                os_mem_free(indicate_result->indi_data.data);
                os_mem_free(indicate_result);
            } else {
                debug_print("Indicate_result parameter is NULL \n");
            }
        }
            break;

        case BLE_TIZENRT_CALLBACK_TYPE_CONN:
        {
            T_TIZENRT_CONNECTED_CALLBACK_DATA *connected = callback_msg.u.buf;
            if(connected != NULL && server_init_parm.connected_cb)
            {
                debug_print("cb %p conn_id 0x%x conn_type 0x%x addr 0x%x0x%x0x%x0x%x0x%x0x%x \n",
                                server_init_parm.connected_cb,
                                connected->conn_id, connected->conn_type,
                                connected->remote_bd[0], connected->remote_bd[1], connected->remote_bd[2],
                                connected->remote_bd[3], connected->remote_bd[4], connected->remote_bd[5]);
                trble_server_connected_t p_func = server_init_parm.connected_cb;
                p_func(connected->conn_id, connected->conn_type, connected->remote_bd);

#if defined(CONFIG_BLE_INDICATION)
                if(connected->conn_type == TRBLE_SERVER_DISCONNECTED)
                {
                    if(ble_tizenrt_indicate_sem != NULL)
                    {
                        os_mutex_give(ble_tizenrt_indicate_sem);
                    }
                }	
#endif
            } else {
                debug_print("NULL connected callback \n");
            }
            os_mem_free(connected);
        }
            break;

        case BLE_TIZENRT_CALLBACK_TYPE_PROFILE:
        {
            T_TIZENRT_PROFILE_CALLBACK_DATA *profile = callback_msg.u.buf;
            if(profile != NULL && profile->cb)
            {
                debug_print("Profile callback \n");
                trble_server_cb_t pfunc = profile->cb;
                pfunc(profile->type, profile->conn_id, profile->att_handle, profile->arg);
            } else {
                debug_print("NULL profile callback \n");
            }
            os_mem_free(profile);
        }
		    break;

        case BLE_TIZENRT_CALLBACK_TYPE_MTU_UPDATE:
        {
            T_TIZENRT_MTU_UDPATE_CALLBACK_DATA *mtu_update = callback_msg.u.buf;
            if(mtu_update != NULL && server_init_parm.mtu_update_cb)
            {
                trble_server_mtu_update_t p_func = server_init_parm.mtu_update_cb;
                p_func(mtu_update->conn_id, mtu_update->mtu_size);
            } else {
                debug_print("NULL connected callback \n");
            }
            os_mem_free(mtu_update);
        }
            break;

		default:
            debug_print("msg type : 0x%x \n", callback_msg.type);
            break;
	}
}

extern void *ble_tizenrt_scatternet_callback_queue_handle;
bool ble_tizenrt_scatternet_send_callback_msg(uint16_t type, void *arg)
{
    T_TIZENRT_APP_CALLBACK_MSG callback_msg;
    callback_msg.type = type;
    callback_msg.u.buf = arg;
	if (ble_tizenrt_scatternet_callback_queue_handle != NULL) {
		if (os_msg_send(ble_tizenrt_scatternet_callback_queue_handle, &callback_msg, 0) == false) {
            dbg("fail!!! msg_type 0x%x \n", callback_msg.type);
	        return false;
		} else {
            debug_print("success msg_type 0x%x \n", callback_msg.type);
        }
	} else {
        debug_print("ble_tizenrt_callback_queue_handle is NULL \n");
        return false;
    }
	return true;
}

int ble_tizenrt_scatternet_handle_upstream_msg(uint16_t subtype, void *pdata)
{
	int ret = 0xff;
    
	switch (subtype) {
		case BLE_TIZENRT_START_SCAN:
        {
            uint8_t scan_filter_policy = *((uint8_t *)pdata);
            uint8_t scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_DISABLE;

            if(pdata)
            {
                le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(scan_filter_policy),
                                  &scan_filter_policy);
                le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(scan_filter_duplicate),
                                  &scan_filter_duplicate);
                ret = le_scan_start();
            } else {
                debug_print("\n[%s] Scan parameter is NULL", __FUNCTION__);
            }
        }
			break;
		case BLE_TIZENRT_STOP_SCAN:
			ret = le_scan_stop();
			break;
		case BLE_TIZENRT_CONNECT:
		{
            T_TIZENRT_CONN_PARAM *param = pdata;
            if(param)
            {
                T_GAP_LE_CONN_REQ_PARAM conn_req_param;
                conn_req_param.scan_interval = 0x520;	/* 820ms */
                conn_req_param.scan_window = 0x520;		/* 820ms */
                conn_req_param.conn_interval_min = param->conn_interval;
                conn_req_param.conn_interval_max = param->conn_interval;
                conn_req_param.conn_latency = param->conn_latency;
                conn_req_param.supv_tout = 1000;
                conn_req_param.ce_len_min = 2 * (conn_req_param.conn_interval_min - 1);
                conn_req_param.ce_len_max = 2 * (conn_req_param.conn_interval_max - 1);

                if(GAP_CAUSE_SUCCESS == le_set_conn_param(GAP_CONN_PARAM_1M, &conn_req_param))
                {
                    debug_print("le_set_conn_param success! \n");
                } else {
                    debug_print("le_set_conn_param fail!!! \n");
                }

                debug_print("remote_bd 0x%02X:0x%02X:0x%02X:0x%02X:0x%02X:0x%02X, rbd_type %d \n",
                        param->remote_bd[5], param->remote_bd[4], 
                        param->remote_bd[3], param->remote_bd[2], param->remote_bd[1], param->remote_bd[0],
                        param->remote_bd_type);
                debug_print("ci: %d si: %d \n", conn_req_param.conn_interval_max, conn_req_param.conn_latency);
                ret = le_connect(0, param->remote_bd, param->remote_bd_type, GAP_LOCAL_ADDR_LE_PUBLIC, param->scan_timeout);
                if(ret) {
                    dbg("le_connect fail 0x%x \n", ret);
                    if(ble_client_connect_is_running)
                        ble_client_connect_is_running = 0;
                }
            } else {
                debug_print("Connect parameter is NULL \n");
            }
        }	
			break;
		case BLE_TIZENRT_DISCONNECT:
        {
            trble_conn_handle param = *((trble_conn_handle *)pdata);
            if (pdata)
            {
                debug_print("\r\n[%s] disconn_id 0x%x", __FUNCTION__, param);
                ret = le_disconnect(param);
                if(ret)
                    printf("\r\n[%s] le_disconnect fail 0x%x ", __FUNCTION__, ret);
            } else {
                debug_print("\n[%s] Disconnect parameter is NULL", __FUNCTION__);
            }
        }
			break;
        case BLE_TIZENRT_BOND:
        {
            trble_conn_handle param = *((trble_conn_handle *)pdata);
            if (pdata)
            {
                debug_print("\r\n[%s] bond_id 0x%x", __FUNCTION__, param);
                ret = le_bond_pair(param);
            } else {
                debug_print("\n[%s] Bond parameter is NULL", __FUNCTION__);
            }
        }
			break;
        case BLE_TIZENRT_READ:
        {
            BLE_TIZENRT_READ_PARAM *param = pdata;
            if(param)
            {
                ret = gcs_attr_read(param->conn_id, param->att_handle);
                debug_print("read_id 0x%x handle 0x%x ret 0x%x \n", param->conn_id, param->att_handle, ret);
            } else {
                debug_print("Read parameter is NULL \n");
            }
        }
			break;
        case BLE_TIZENRT_WRITE:
        {
            BLE_TIZENRT_WRITE_PARAM *param = pdata;
            if(param)
            {
                debug_print("write_id 0x%x handle 0x%x len 0x%x data \n", param->conn_id,
                                            param->att_handle, param->length);
                for (int i = 0; i < param->length; i++)
                {
                    debug_print("0x%x", param->data[i]);
                }
                
                ret = gcs_attr_write(param->conn_id, GATT_WRITE_TYPE_REQ, param->att_handle,
                                        param->length, param->data);
                if(ret)
                    dbg("gcs_attr_write fail 0x%x \n", ret);
            } else {
                debug_print("Write parameter is NULL \n");
            }
        }
            break;
        case BLE_TIZENRT_WRITE_NO_RSP:
        {
            BLE_TIZENRT_WRITE_PARAM *param = pdata;
            if(param)
            {
                debug_print("write_id 0x%x handle 0x%x len 0x%x data \n", param->conn_id,
                                        param->att_handle, param->length);
                for (int i = 0; i < param->length; i++)
                {
                    debug_print("0x%x", param->data[i]);
                }
                ret = gcs_attr_write(param->conn_id, GATT_WRITE_TYPE_CMD, param->att_handle,
                                        param->length, param->data);
                if(ret)
                    dbg("0x%x gcs_attr_write fail \n", ret);
            } else {
                debug_print("Write_no_rsp parameter is NULL \n");
            }
        }
            break;
        case BLE_TIZENRT_DELETE_BOND:
        {
            T_TIZENRT_DELETE_BOND_PARAM *param = pdata;
            if(param)
            {
                debug_print("le_bond_delete_by_bd \n");
                ret = le_bond_delete_by_bd(param->remote_bd, param->remote_bd_type);
                if(ret == GAP_CAUSE_NOT_FIND)
                    dbg("[upstream] Not found ! \n");
                else
                    dbg("[upstream] delete bond success ! \n");
            } else {
                debug_print("Delete_bond parameter is NULL \n");
            }
        }
	        break;

        case BLE_TIZENRT_CLEAR_ALL_BONDS:
        {
            debug_print("le_bond_clear_all_keys \n");
            le_bond_clear_all_keys();
        }
			break;
		case BLE_TIZENRT_MSG_START_ADV:
        {
            ret = le_adv_start();
            if(GAP_CAUSE_SUCCESS == ret)
                dbg("[Upstream] Start Adv Success \n");
            else
                dbg("[Upstream] Start Adv Fail !! 0x%x \n", ret);
        }
			break;
		case BLE_TIZENRT_MSG_STOP_ADV:
			ret = le_adv_stop();
			if(GAP_CAUSE_SUCCESS == ret)
                           dbg("[Upstream] Stop Adv Success \n");
                       else
                           dbg("[Upstream] Stop Adv Fail !! 0x%x \n", ret);
			break;
        case BLE_TIZENRT_MSG_DISCONNECT:
        {
            trble_conn_handle param = *((trble_conn_handle *)pdata);
            if (pdata)
            {
                debug_print("\r\n[%s] disconn_id 0x%x", __FUNCTION__, param);
                ret = le_disconnect(param);
                if(ret)
                    printf("\r\n[%s] le_disconnect fail 0x%x ", __FUNCTION__, ret);
            } else {
                debug_print("\n[%s] Disconnect parameter is NULL", __FUNCTION__);
            }
        }
            break;
		case BLE_TIZENRT_MSG_NOTIFY:
        {
            T_TIZENRT_NOTIFY_PARAM *param = pdata;
            if(param)
            {
                debug_print("conn_id %d abs_handle 0x%x data %p \n",
                                            param->conn_id, param->att_handle, param->data);
                if(tizenrt_ble_service_send_notify(param->conn_id, param->att_handle, param->data, param->len))
                    debug_print("success : subtype = 0x%x \n", subtype);
                else
                    debug_print("fail : subtype = 0x%x \n", subtype);
                os_mem_free(param->data);
            } else {
                debug_print("Notify parameter is NULL \n");
            }
        }
			break;
		case BLE_TIZENRT_MSG_INDICATE:
        {
            T_TIZENRT_INDICATE_PARAM *param = pdata;
            if(param)
            {
                debug_print("conn_id %d abs_handle 0x%x data %p \n",
                                            param->conn_id, param->att_handle, param->data);
                if(tizenrt_ble_service_send_indicate(param->conn_id, param->att_handle, param->data, param->len))
                    debug_print("success : subtype = 0x%x \n", subtype);
                else
                    debug_print("fail : subtype = 0x%x \n", subtype);
                os_mem_free(param->data);
            } else {
                debug_print("Notify parameter is NULL \n");
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
                debug_print("Delete_bond parameter is NULL \n");
            }
        }
            break;
        case BLE_TIZENRT_MSG_DELETE_BOND_ALL:
        {
            debug_print("le_bond_clear_all_keys \n");
            le_bond_clear_all_keys();
        }
            break;
        case BLE_TIZENRT_MODIFY_WHITELIST:
        {
            T_TIZENRT_MODIFY_WHITELIST_PARAM *param = pdata;
            if(param)
            {
                debug_print("\r\n[%s] le_modify_white_list", __FUNCTION__);
                ret = le_modify_white_list(param->type, param->remote_bd, param->remote_bd_type);
                if (ret)
                    printf("\r\n[%s] 0x%x le_modify_white_list fail  ", __FUNCTION__, ret);
            } else {
                debug_print("\n[%s] Modify_whitelist parameter is NULL", __FUNCTION__);
            }
        }
            break;
		default:
			break;
	}

    if(pdata)
        os_mem_free(pdata);

    return ret;
}

extern void *ble_tizenrt_scatternet_evt_queue_handle; 
extern void *ble_tizenrt_scatternet_io_queue_handle; 
bool ble_tizenrt_scatternet_send_msg(uint16_t sub_type, void *arg)
{
    debug_print("in : subtype = 0x%x \n", sub_type);
    uint8_t event = EVENT_IO_TO_APP;

    T_IO_MSG io_msg;

    io_msg.type = IO_MSG_TYPE_QDECODE;
    io_msg.subtype = sub_type;
    io_msg.u.buf = arg;

    if (ble_tizenrt_scatternet_evt_queue_handle != NULL && ble_tizenrt_scatternet_io_queue_handle != NULL) {
        if (os_msg_send(ble_tizenrt_scatternet_io_queue_handle, &io_msg, 0) == false) {
            dbg("send msg fail : io_msg.subtype = 0x%x \n", io_msg.subtype);
	        return false;
        } else if (os_msg_send(ble_tizenrt_scatternet_evt_queue_handle, &event, 0) == false) {
            dbg("send evt fail : io_msg.subtype = 0x%x \n", io_msg.subtype);
	        return false;
        }
    }
    debug_print("success : subtype = 0x%x \n", sub_type);
	return true;
}

void ble_tizenrt_scatternet_app_handle_gap_msg(T_IO_MSG  *p_gap_msg);
/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void ble_tizenrt_scatternet_app_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        {
            debug_print("Recieve Status msg \n");
            ble_tizenrt_scatternet_app_handle_gap_msg(&io_msg);
        }
        break;
#if	defined (CONFIG_BT_USER_COMMAND) && (CONFIG_BT_USER_COMMAND)
    case IO_MSG_TYPE_UART:
        uint8_t rx_char;
        /* We handle user command informations from Data UART in this branch. */
        rx_char = (uint8_t)io_msg.subtype;
        user_cmd_collect(&user_cmd_if, &rx_char, sizeof(rx_char), user_cmd_table);
        break;
#endif
    case IO_MSG_TYPE_QDECODE:
        {
            debug_print("Recieve Upstream msg \n");
			uint16_t subtype = io_msg.subtype;
			void *arg = io_msg.u.buf;
			ble_tizenrt_scatternet_handle_upstream_msg(subtype, arg);
        }
        break;
    default:
        break;
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_DEV_STATE_CHANGE
 * @note     All the gap device state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] new_state  New gap device state
 * @param[in] cause GAP device state change cause
 * @return   void
 */

void ble_tizenrt_scatternet_app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    APP_PRINT_INFO3("ble_tizenrt_scatternet_app_handle_dev_state_evt: init state  %d, scan state %d, cause 0x%x",
                    new_state.gap_init_state,
                    new_state.gap_scan_state, cause);
    dbg("init state  %d, scan state %d, conn_state %d, adv_state %d, cause 0x%x \n",
                    new_state.gap_init_state,
                    new_state.gap_scan_state,
                    new_state.gap_conn_state,
                    new_state.gap_adv_state,
                    cause);
    if (ble_tizenrt_scatternet_gap_dev_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            uint8_t bt_addr[6];
            APP_PRINT_INFO0("GAP stack ready");
            /*stack ready*/
            gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
            dbg("GAP stack ready \n");
            dbg("local bd addr: 0x%2x:%2x:%2x:%2x:%2x:%2x \n",
                            bt_addr[5],
                            bt_addr[4],
                            bt_addr[3],
                            bt_addr[2],
                            bt_addr[1],
                            bt_addr[0]);
        }
        ble_tizenrt_scatternet_gap_dev_state.gap_init_state = new_state.gap_init_state;
    }

    if (ble_tizenrt_scatternet_gap_dev_state.gap_adv_state != new_state.gap_adv_state)
    {
        if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        {
            if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
            {
                dbg("GAP adv stoped: because connection created \n");
            }
            else
            {
                dbg("GAP adv stopped \n");
            }
            ble_tizenrt_scatternet_gap_dev_state.gap_adv_sub_state = new_state.gap_adv_sub_state;
        }
        else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            dbg("GAP adv start \n");
        }
        ble_tizenrt_scatternet_gap_dev_state.gap_adv_state = new_state.gap_adv_state;
    }

    if (ble_tizenrt_scatternet_gap_dev_state.gap_scan_state != new_state.gap_scan_state)
    {
        uint32_t state = (uint32_t) new_state.gap_scan_state;
        if (new_state.gap_scan_state == GAP_SCAN_STATE_IDLE)
        {
            APP_PRINT_INFO0("GAP scan stop");
            dbg("GAP scan stop \n");
            if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_SCAN_STATE_MSG,  (void *) state) == false)
            {
                debug_print("callback msg send fail \n");
            }
        }
        else if (new_state.gap_scan_state == GAP_SCAN_STATE_SCANNING)
        {
            APP_PRINT_INFO0("GAP scan start");
            dbg("GAP scan start \n");
            //uint32_t state = (uint32_t) new_state.gap_scan_state;
            if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_SCAN_STATE_MSG, (void *) state) == false)
            {
                debug_print("callback msg send fail \n");
            }
        }
        ble_tizenrt_scatternet_gap_dev_state.gap_scan_state = new_state.gap_scan_state;
    }
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
void ble_tizenrt_scatternet_app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
{
    T_GAP_CONN_INFO conn_info;
    if (conn_id >= BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS)
    {
        return;
    }
    APP_PRINT_INFO4("ble_tizenrt_scatternet_app_handle_conn_state_evt: conn_id %d, conn_state(%d -> %d), disc_cause 0x%x",
                    conn_id, ble_tizenrt_scatternet_app_link_table[conn_id].conn_state, new_state, disc_cause);

    ble_tizenrt_scatternet_app_link_table[conn_id].conn_state = new_state;
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {
                debug_print("ble_tizenrt_scatternet_app_handle_conn_state_evt: connection lost, conn_id %d, cause 0x%x \n", conn_id,
                                 disc_cause);
            }
            dbg("Disconnect conn_id %d, cause 0x%x\n", conn_id, disc_cause);
            if (ble_tizenrt_scatternet_app_link_table[conn_id].role == GAP_LINK_ROLE_SLAVE) {
                T_TIZENRT_CONNECTED_CALLBACK_DATA *disconn_data = os_mem_alloc(0, sizeof(T_TIZENRT_CONNECTED_CALLBACK_DATA));
                if(disconn_data)
                {
                    disconn_data->conn_id = conn_id;
                    disconn_data->conn_type = TRBLE_SERVER_DISCONNECTED;
                    memcpy(disconn_data->remote_bd, ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd, TRBLE_BD_ADDR_MAX_LEN);
                    if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_CALLBACK_TYPE_CONN, disconn_data) == false)
                    {
                        os_mem_free(disconn_data);
                        debug_print("callback msg send fail \n");
                    }
                } else {
                    debug_print("Memory allocation failed \n");
                }
            } else {
                if (ble_tizenrt_scatternet_app_link_table[conn_id].role == GAP_LINK_ROLE_MASTER)
                    if (g_master_link_num){
                        g_master_link_num --;
                    }
                uint32_t connid = (uint32_t) conn_id;
                if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_DISCONNECTED_MSG, (void *) connid) == false)
                {
                    debug_print("callback msg send fail \n");
                }
            }
            memset(&ble_tizenrt_scatternet_app_link_table[conn_id], 0, sizeof(BLE_TIZENRT_SCATTERNET_APP_LINK));
        }
        break;

    case GAP_CONN_STATE_CONNECTED:
        {
            le_get_conn_addr(conn_id, ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd,
                             (void *)&ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd_type);
            //get device role
            if (le_get_conn_info(conn_id, &conn_info)){
                ble_tizenrt_scatternet_app_link_table[conn_id].role = conn_info.role;
            }
            dbg("Connected success conn_id %d \n", conn_id);
            debug_print("addr %02x:%02x:%02x:%02x:%02x:%02x \n",
                            ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd[5],
                            ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd[4],
                            ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd[3],
                            ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd[2],
                            ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd[1],
                            ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd[0]);
            if (ble_tizenrt_scatternet_app_link_table[conn_id].role == GAP_LINK_ROLE_MASTER) {
                g_master_link_num ++;
                trble_device_connected *connected_dev = os_mem_alloc(0, sizeof(trble_device_connected));
                if(connected_dev)
                {
                    memcpy(connected_dev->conn_info.addr.mac, ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd, GAP_BD_ADDR_LEN);
                    le_get_conn_param(GAP_PARAM_CONN_BD_ADDR_TYPE, &connected_dev->conn_info.addr.type, conn_id);
                    le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &connected_dev->conn_info.conn_interval, conn_id);
                    le_get_conn_param(GAP_PARAM_CONN_LATENCY, &connected_dev->conn_info.slave_latency, conn_id);
                    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &connected_dev->conn_info.mtu, conn_id);
                    connected_dev->is_bonded = false;
                    connected_dev->conn_handle = conn_id;
                    if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_CONNECTED_MSG, connected_dev) == false)
                    {
                        os_mem_free(connected_dev);
                        debug_print("callback msg send fail \n");
                    }
                } else {
                    debug_print("Memory allocation failed \n");
                }
            } else if (ble_tizenrt_scatternet_app_link_table[conn_id].role == GAP_LINK_ROLE_SLAVE) {
                T_TIZENRT_CONNECTED_CALLBACK_DATA *conn_data = os_mem_alloc(0, sizeof(T_TIZENRT_CONNECTED_CALLBACK_DATA));
                if(conn_data)
                {
                    conn_data->conn_id = conn_id;
                    conn_data->conn_type = TRBLE_SERVER_LL_CONNECTED;
                    memcpy(conn_data->remote_bd, ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd, GAP_BD_ADDR_LEN);
                    if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_CALLBACK_TYPE_CONN, conn_data) == false)
                    {
                        os_mem_free(conn_data);
                        debug_print("callback msg send fail \n");
                    }
                } else {
                    debug_print("Memory allocation failed \n");
                }
            } else {
                debug_print("Undefined GAP Role \n");
            }

#if F_BT_LE_5_0_SET_PHY_SUPPORT
			{
                uint8_t tx_phy;
                uint8_t rx_phy;
                le_get_conn_param(GAP_PARAM_CONN_RX_PHY_TYPE, &rx_phy, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_TX_PHY_TYPE, &tx_phy, conn_id);
                APP_PRINT_INFO2("GAP_CONN_STATE_CONNECTED: tx_phy %d, rx_phy %d", tx_phy, rx_phy);
                dbg("GAP_CONN_STATE_CONNECTED: tx_phy %d, rx_phy %d \n", tx_phy, rx_phy);
			}
#endif
        }
        break;

    default:
        break;

    }
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
void ble_tizenrt_scatternet_app_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    APP_PRINT_INFO2("ble_tizenrt_scatternet_app_handle_authen_state_evt:conn_id %d, cause 0x%x", conn_id, cause);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
        {
            dbg("Auth started \n");
            ble_tizenrt_scatternet_app_link_table[conn_id].auth_state = GAP_AUTHEN_STATE_STARTED;
            APP_PRINT_INFO0("ble_tizenrt_scatternet_app_handle_authen_state_evt: GAP_AUTHEN_STATE_STARTED");
        }
        break;

    case GAP_AUTHEN_STATE_COMPLETE:
        {
            ble_tizenrt_scatternet_app_link_table[conn_id].auth_state = GAP_AUTHEN_STATE_COMPLETE;
            if (cause == GAP_SUCCESS)
            {
                dbg("Pair success \n");
                if(ble_tizenrt_scatternet_app_link_table[conn_id].role == GAP_LINK_ROLE_MASTER)
                {
                    trble_device_connected *connected_dev = os_mem_alloc(0, sizeof(trble_device_connected));
                    if(connected_dev)
                    {
                        memcpy(connected_dev->conn_info.addr.mac, ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd, GAP_BD_ADDR_LEN);
                        le_get_conn_param(GAP_PARAM_CONN_BD_ADDR_TYPE, &connected_dev->conn_info.addr.type, conn_id);
                        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &connected_dev->conn_info.conn_interval, conn_id);
                        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &connected_dev->conn_info.slave_latency, conn_id);
                        le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &connected_dev->conn_info.mtu, conn_id);
                        connected_dev->is_bonded = true;
                        connected_dev->conn_handle = conn_id;
                        if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_BONDED_MSG, connected_dev) == false)
                        {
                            os_mem_free(connected_dev);
                            debug_print("callback msg send fail \n");
                        }
                    } else {
                        debug_print("Memory allocation failed \n");
                    }
                } else if(ble_tizenrt_scatternet_app_link_table[conn_id].role == GAP_LINK_ROLE_SLAVE) {
                    T_TIZENRT_CONNECTED_CALLBACK_DATA *authed_data = os_mem_alloc(0, sizeof(T_TIZENRT_CONNECTED_CALLBACK_DATA));
                    if(authed_data)
                    {
                        authed_data->conn_id = conn_id;
                        authed_data->conn_type = TRBLE_SERVER_SM_CONNECTED;
                        memcpy(authed_data->remote_bd, ble_tizenrt_scatternet_app_link_table[conn_id].remote_bd, GAP_BD_ADDR_LEN);
                        if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_CALLBACK_TYPE_CONN, authed_data) == false)
                        {
                            os_mem_free(authed_data);
                            debug_print("callback msg send fail \n");
                        } else {
                            debug_print("Memory allocation failed \n");
                        }
                    } else {
                        debug_print("Memory allocation failed \n");
                    }
                } else {
                    debug_print("Undefined GAP Role \n");
                }
            } else {
                dbg("Pair failed: cause 0x%x \n", cause);
                ble_tizenrt_scatternet_app_link_table[conn_id].auth_state = 0xff;
            }
        }
        break;

    default:
        {
            debug_print("Unknown Auth State \n");
            APP_PRINT_ERROR1("ble_tizenrt_scatternet_app_handle_authen_state_evt: unknown newstate %d", new_state);
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
void ble_tizenrt_scatternet_app_handle_conn_mtu_info_evt(uint8_t conn_id, uint16_t mtu_size)
{
    APP_PRINT_INFO2("ble_tizenrt_scatternet_app_handle_conn_mtu_info_evt: conn_id %d, mtu_size %d", conn_id, mtu_size);
    T_TIZENRT_MTU_UDPATE_CALLBACK_DATA *mtu_data = os_mem_alloc(0, sizeof(T_TIZENRT_MTU_UDPATE_CALLBACK_DATA));

    if(mtu_data)
    {
        mtu_data->conn_id = conn_id;
        mtu_data->mtu_size = mtu_size;
        if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_CALLBACK_TYPE_MTU_UPDATE, mtu_data) == false)
        {
            os_mem_free(mtu_data);
            debug_print("callback msg send fail \n");
        } else {
            debug_print("Memory allocation failed \n");
        }
        } else {
            debug_print("Memory allocation failed \n");
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_PARAM_UPDATE
 * @note     All the connection parameter update change  events are pre-handled in this function.
 * @param[in] conn_id Connection ID
 * @param[in] status  New update state
 * @param[in] cause Use this cause when status is GAP_CONN_PARAM_UPDATE_STATUS_FAIL
 * @return   void
 */
void ble_tizenrt_scatternet_app_handle_conn_param_update_evt(uint8_t conn_id, uint8_t status, uint16_t cause)
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
            APP_PRINT_INFO4("ble_tizenrt_scatternet_app_handle_conn_param_update_evt update success:conn_id %d, conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x",
                            conn_id, conn_interval, conn_slave_latency, conn_supervision_timeout);
            dbg("conn param update success:conn_id %d, conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x \n",
                            conn_id, conn_interval, conn_slave_latency, conn_supervision_timeout);
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
        {
            APP_PRINT_ERROR2("ble_tizenrt_scatternet_app_handle_conn_param_update_evt update failed: conn_id %d, cause 0x%x",
                             conn_id, cause);
            dbg("conn param update failed: conn_id %d, cause 0x%x \n",
                             conn_id, cause);

        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
        {
            APP_PRINT_INFO1("ble_tizenrt_scatternet_app_handle_conn_param_update_evt update pending: conn_id %d", conn_id);
            dbg("conn param update pending: conn_id %d \n", conn_id);

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
void ble_tizenrt_scatternet_app_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    APP_PRINT_TRACE1("ble_tizenrt_scatternet_app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
        {
            ble_tizenrt_scatternet_app_handle_dev_state_evt(gap_msg.msg_data.gap_dev_state_change.new_state,
                                     gap_msg.msg_data.gap_dev_state_change.cause);
        }
        break;

    case GAP_MSG_LE_CONN_STATE_CHANGE:
        {
            ble_tizenrt_scatternet_app_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                      (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                      gap_msg.msg_data.gap_conn_state_change.disc_cause);
        }
        break;

    case GAP_MSG_LE_CONN_MTU_INFO:
        {
            ble_tizenrt_scatternet_app_handle_conn_mtu_info_evt(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                         gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
        }
        break;

    case GAP_MSG_LE_CONN_PARAM_UPDATE:
        {
            ble_tizenrt_scatternet_app_handle_conn_param_update_evt(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                             gap_msg.msg_data.gap_conn_param_update.status,
                                             gap_msg.msg_data.gap_conn_param_update.cause);
        }
        break;

    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
        {
            ble_tizenrt_scatternet_app_handle_authen_state_evt(gap_msg.msg_data.gap_authen_state.conn_id,
                                        gap_msg.msg_data.gap_authen_state.new_state,
                                        gap_msg.msg_data.gap_authen_state.status);
        }
        break;

    case GAP_MSG_LE_BOND_JUST_WORK:
        {
            debug_print("GAP_MSG_LE_BOND_JUST_WORK \n");
            conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
            le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            APP_PRINT_INFO0("GAP_MSG_LE_BOND_JUST_WORK");
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            APP_PRINT_INFO2("GAP_MSG_LE_BOND_PASSKEY_DISPLAY: conn_id %d, passkey %d",
                            conn_id, display_value);
            le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            dbg("GAP_MSG_LE_BOND_PASSKEY_DISPLAY: conn_id %d, passkey %d \n",
                            conn_id,
                            display_value);
        }
        break;

    case GAP_MSG_LE_BOND_USER_CONFIRMATION:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            APP_PRINT_INFO2("GAP_MSG_LE_BOND_USER_CONFIRMATION: conn_id %d, passkey %d",
                            conn_id, display_value);
            dbg("GAP_MSG_LE_BOND_USER_CONFIRMATION: conn_id %d, passkey %d \n",
                            conn_id,
                            display_value);
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_INPUT:
        {
            conn_id = gap_msg.msg_data.gap_bond_passkey_input.conn_id;
            APP_PRINT_INFO1("GAP_MSG_LE_BOND_PASSKEY_INPUT: conn_id %d", conn_id);
            dbg("GAP_MSG_LE_BOND_PASSKEY_INPUT: conn_id %d \n", conn_id);
        }
        break;

    case GAP_MSG_LE_BOND_OOB_INPUT:
        {
#if F_BT_LE_SMP_OOB_SUPPORT
            uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
            conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
            APP_PRINT_INFO1("GAP_MSG_LE_BOND_OOB_INPUT: conn_id %d", conn_id);
#if F_BT_LE_SMP_OOB_SUPPORT
            le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
            le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
#endif
        }
        break;

    default:
        APP_PRINT_ERROR1("ble_tizenrt_scatternet_app_handle_gap_msg: unknown subtype %d", p_gap_msg->subtype);
        break;
    }
}
/** @} */ /* End of group CENTRAL_CLIENT_GAP_MSG */

/** @defgroup  CENTRAL_CLIENT_GAP_CALLBACK GAP Callback Event Handler
    * @brief Handle GAP callback event
    * @{
    */
/**
  * @brief Used to parse advertising data and scan response data
  * @param[in] scan_info point to scan information data.
  * @retval void
  */
uint8_t ble_tizenrt_scatternet_parse_scanned_devname(T_LE_SCAN_INFO *scan_info, uint8_t *local_name)
{
    uint8_t pos = 0;
    uint8_t length = 0;
    uint8_t type;
    while (scan_info && pos < scan_info->data_len)
    {
        /* Length of the AD structure. */
        length = scan_info->data[pos++];
        if ((length > 0x01) && ((pos + length) <= 31))
        {
            /* Copy the AD Data to buffer. */
            /* AD Type, one octet. */
            type = scan_info->data[pos];
            if(GAP_ADTYPE_LOCAL_NAME_SHORT == type || GAP_ADTYPE_LOCAL_NAME_COMPLETE == type)
            {
                memcpy(local_name, scan_info->data + pos + 1, length - 1);
                local_name[length - 1] = '\0';
                debug_print("GAP_ADTYPE_LOCAL_NAME_XXX: %s \n", local_name);
                return length;
            }
        }
        pos += length;
    }
    return 0;
}
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT ble_tizenrt_scatternet_app_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
	char adv_type[20];
	char remote_addr_type[10];

    switch (cb_type)
    {
    case GAP_MSG_LE_SCAN_INFO:
        APP_PRINT_INFO5("GAP_MSG_LE_SCAN_INFO:adv_type 0x%x, bd_addr %s, remote_addr_type %d, rssi %d, data_len %d",
                        p_data->p_le_scan_info->adv_type,
                        TRACE_BDADDR(p_data->p_le_scan_info->bd_addr),
                        p_data->p_le_scan_info->remote_addr_type,
                        p_data->p_le_scan_info->rssi,
                        p_data->p_le_scan_info->data_len);
        /* If you want to parse the scan info, please reference function ble_tizenrt_scatternet_app_parse_scan_info. */
		sprintf(adv_type,"%s",(p_data->p_le_scan_info->adv_type ==GAP_ADV_EVT_TYPE_UNDIRECTED)? "CON_UNDIRECT":
							  (p_data->p_le_scan_info->adv_type ==GAP_ADV_EVT_TYPE_DIRECTED)? "CON_DIRECT":
							  (p_data->p_le_scan_info->adv_type ==GAP_ADV_EVT_TYPE_SCANNABLE)? "SCANABLE_UNDIRCT":
							  (p_data->p_le_scan_info->adv_type ==GAP_ADV_EVT_TYPE_NON_CONNECTABLE)? "NON_CONNECTABLE":
							  (p_data->p_le_scan_info->adv_type ==GAP_ADV_EVT_TYPE_SCAN_RSP)? "SCAN_RSP":"unknown");
		sprintf(remote_addr_type,"%s",(p_data->p_le_scan_info->remote_addr_type == GAP_REMOTE_ADDR_LE_PUBLIC)? "public":
							   (p_data->p_le_scan_info->remote_addr_type == GAP_REMOTE_ADDR_LE_RANDOM)? "random":"unknown");

        trble_scanned_device *scanned_device = os_mem_alloc(0, sizeof(trble_scanned_device));
        if(scanned_device)
        {
            memset(scanned_device, 0, sizeof(trble_scanned_device));
#if defined(CONFIG_DEBUG_SCAN_INFO)
            debug_print("ADVType\t\t\t| AddrType\t|%-17s\t|rssi \n","BT_Addr");
            debug_print("%-20s\t|%-8s\t|"BD_ADDR_FMT"\t|%d \n",adv_type,remote_addr_type,BD_ADDR_ARG(p_data->p_le_scan_info->bd_addr),
                                                    p_data->p_le_scan_info->rssi);
#endif
            scanned_device->resp_data_length = ble_tizenrt_scatternet_parse_scanned_devname(p_data->p_le_scan_info, scanned_device->resp_data);
            scanned_device->adv_type = p_data->p_le_scan_info->adv_type;
            scanned_device->rssi = p_data->p_le_scan_info->rssi;
            scanned_device->addr.type = p_data->p_le_scan_info->remote_addr_type;
            memcpy(scanned_device->addr.mac, p_data->p_le_scan_info->bd_addr, GAP_BD_ADDR_LEN);
            if(scanned_device->adv_type == GAP_ADV_EVT_TYPE_SCAN_RSP)
            {
                scanned_device->resp_data_length = p_data->p_le_scan_info->data_len;
                memcpy(scanned_device->resp_data, p_data->p_le_scan_info->data, p_data->p_le_scan_info->data_len);
            } else {
                scanned_device->raw_data_length = p_data->p_le_scan_info->data_len;
                memcpy(scanned_device->raw_data, p_data->p_le_scan_info->data, p_data->p_le_scan_info->data_len);
            }

            if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_SCANNED_DEVICE_MSG, scanned_device) == false)
            {
                os_mem_free(scanned_device);
                debug_print("callback msg send fail \n");
                return APP_RESULT_PREP_QUEUE_FULL;
            }
        } else {
            debug_print("Memory allocation failed \n");
        }
        break;

    case GAP_MSG_LE_CONN_UPDATE_IND:
        APP_PRINT_INFO5("GAP_MSG_LE_CONN_UPDATE_IND: conn_id %d, conn_interval_max 0x%x, conn_interval_min 0x%x, conn_latency 0x%x,supervision_timeout 0x%x",
                        p_data->p_le_conn_update_ind->conn_id,
                        p_data->p_le_conn_update_ind->conn_interval_max,
                        p_data->p_le_conn_update_ind->conn_interval_min,
                        p_data->p_le_conn_update_ind->conn_latency,
                        p_data->p_le_conn_update_ind->supervision_timeout);
        dbg("GAP_MSG_LE_CONN_UPDATE_IND: conn_id %d, conn_interval_max 0x%x, conn_interval_min 0x%x \n, conn_latency 0x%x,supervision_timeout 0x%x \n",
                        p_data->p_le_conn_update_ind->conn_id,
                        p_data->p_le_conn_update_ind->conn_interval_max,
                        p_data->p_le_conn_update_ind->conn_interval_min,
                        p_data->p_le_conn_update_ind->conn_latency,
                        p_data->p_le_conn_update_ind->supervision_timeout);
        /* if reject the proposed connection parameter from peer device, use APP_RESULT_REJECT. */
        result = APP_RESULT_ACCEPT;
        break;

#if F_BT_LE_5_0_SET_PHY_SUPPORT
	case GAP_MSG_LE_PHY_UPDATE_INFO:
		APP_PRINT_INFO4("GAP_MSG_LE_PHY_UPDATE_INFO:conn_id %d, cause 0x%x, rx_phy %d, tx_phy %d",
						p_data->p_le_phy_update_info->conn_id,
						p_data->p_le_phy_update_info->cause,
						p_data->p_le_phy_update_info->rx_phy,
						p_data->p_le_phy_update_info->tx_phy);
		dbg("GAP_MSG_LE_PHY_UPDATE_INFO:conn_id %d, cause 0x%x, rx_phy %d, tx_phy %d \n",
						p_data->p_le_phy_update_info->conn_id,
						p_data->p_le_phy_update_info->cause,
						p_data->p_le_phy_update_info->rx_phy,
						p_data->p_le_phy_update_info->tx_phy);
		break;

	case GAP_MSG_LE_REMOTE_FEATS_INFO:
		{
			uint8_t  remote_feats[8];
			APP_PRINT_INFO3("GAP_MSG_LE_REMOTE_FEATS_INFO: conn id %d, cause 0x%x, remote_feats %b",
							p_data->p_le_remote_feats_info->conn_id,
							p_data->p_le_remote_feats_info->cause,
							TRACE_BINARY(8, p_data->p_le_remote_feats_info->remote_feats));
			if (p_data->p_le_remote_feats_info->cause == GAP_SUCCESS)
			{
				memcpy(remote_feats, p_data->p_le_remote_feats_info->remote_feats, 8);
				if (remote_feats[LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1] & LE_SUPPORT_FEATURES_LE_2M_MASK_BIT)
				{
					APP_PRINT_INFO0("GAP_MSG_LE_REMOTE_FEATS_INFO: support 2M");
					dbg("GAP_MSG_LE_REMOTE_FEATS_INFO: support 2M \n");
				}
				if (remote_feats[LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1] & LE_SUPPORT_FEATURES_LE_CODED_PHY_MASK_BIT)
				{
					APP_PRINT_INFO0("GAP_MSG_LE_REMOTE_FEATS_INFO: support CODED");
					dbg("GAP_MSG_LE_REMOTE_FEATS_INFO: support CODED \n");
				}
			}
		}
		break;
#endif
	case GAP_MSG_LE_MODIFY_WHITE_LIST:
   		APP_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation  0x%x, cause 0x%x",
				   p_data->p_le_modify_white_list_rsp->operation,
				   p_data->p_le_modify_white_list_rsp->cause);
		printf("\r\nGAP_MSG_LE_MODIFY_WHITE_LIST: operation  0x%x, cause 0x%x",
			       p_data->p_le_modify_white_list_rsp->operation,
				   p_data->p_le_modify_white_list_rsp->cause);
        modify_whitelist_code = p_data->p_le_modify_white_list_rsp->cause;
        os_mutex_give(ble_tizenrt_modify_whitelist_sem);
   		break;
    default:
        APP_PRINT_ERROR1("ble_tizenrt_scatternet_app_gap_callback: unhandled cb_type 0x%x", cb_type);
        break;
    }
    return result;
}
/** @} */ /* End of group CENTRAL_CLIENT_GAP_CALLBACK */

/** @defgroup  GCS_CLIIENT_CALLBACK GCS Client Callback Event Handler
    * @brief Handle profile client callback event
    * @{
    */
void ble_tizenrt_scatternet_gcs_handle_discovery_result(uint8_t conn_id, T_GCS_DISCOVERY_RESULT discov_result)
{
    uint16_t i;
    T_GCS_DISCOV_RESULT *p_result_table;
    uint16_t    properties;
    switch (discov_result.discov_type)
    {
    case GCS_ALL_PRIMARY_SRV_DISCOV:
        APP_PRINT_INFO2("conn_id %d, GCS_ALL_PRIMARY_SRV_DISCOV, is_success %d",
                        conn_id, discov_result.is_success);
        for (i = 0; i < discov_result.result_num; i++)
        {
            p_result_table = &(discov_result.p_result_table[i]);
            switch (p_result_table->result_type)
            {
            case DISC_RESULT_ALL_SRV_UUID16:
                APP_PRINT_INFO4("ALL SRV UUID16[%d]: service range: 0x%x-0x%x, uuid16 0x%x",
                                i, p_result_table->result_data.srv_uuid16_disc_data.att_handle,
                                p_result_table->result_data.srv_uuid16_disc_data.end_group_handle,
                                p_result_table->result_data.srv_uuid16_disc_data.uuid16);
                dbg("ALL SRV UUID16[%d]: service range: 0x%x-0x%x, uuid16 0x%x \n",
                                i, p_result_table->result_data.srv_uuid16_disc_data.att_handle,
                                p_result_table->result_data.srv_uuid16_disc_data.end_group_handle,
                                p_result_table->result_data.srv_uuid16_disc_data.uuid16);
                break;
            case DISC_RESULT_ALL_SRV_UUID128:
                APP_PRINT_INFO4("ALL SRV UUID128[%d]: service range: 0x%x-0x%x, service=<%b>",
                                i, p_result_table->result_data.srv_uuid128_disc_data.att_handle,
                                p_result_table->result_data.srv_uuid128_disc_data.end_group_handle,
                                TRACE_BINARY(16, p_result_table->result_data.srv_uuid128_disc_data.uuid128));
                dbg("ALL SRV UUID128[%d]: service range: 0x%x-0x%x, service="UUID_128_FORMAT" \n",
                                i, p_result_table->result_data.srv_uuid128_disc_data.att_handle,
                                p_result_table->result_data.srv_uuid128_disc_data.end_group_handle,
                                UUID_128(p_result_table->result_data.srv_uuid128_disc_data.uuid128));
                break;

            default:
                APP_PRINT_ERROR0("Invalid Discovery Result Type!");
                dbg("Invalid Discovery Result Type! \n");
                break;
            }
        }
        break;

    case GCS_BY_UUID128_SRV_DISCOV:
        APP_PRINT_INFO2("conn_id %d, GCS_BY_UUID128_SRV_DISCOV, is_success %d",
                        conn_id, discov_result.is_success);
        dbg("conn_id %d, GCS_BY_UUID128_SRV_DISCOV, is_success %d \n",
                        conn_id, discov_result.is_success);

        for (i = 0; i < discov_result.result_num; i++)
        {
            p_result_table = &(discov_result.p_result_table[i]);
            switch (p_result_table->result_type)
            {
            case DISC_RESULT_SRV_DATA:
                APP_PRINT_INFO3("SRV DATA[%d]: service range: 0x%x-0x%x",
                                i, p_result_table->result_data.srv_disc_data.att_handle,
                                p_result_table->result_data.srv_disc_data.end_group_handle);
                dbg("SRV DATA[%d]: service range: 0x%x-0x%x \n",
                                i, p_result_table->result_data.srv_disc_data.att_handle,
                                p_result_table->result_data.srv_disc_data.end_group_handle);
                break;

            default:
                APP_PRINT_ERROR0("Invalid Discovery Result Type!");
                dbg("Invalid Discovery Result Type! \n");
                break;
            }
        }
        break;

    case GCS_BY_UUID_SRV_DISCOV:
        APP_PRINT_INFO2("conn_id %d, GCS_BY_UUID_SRV_DISCOV, is_success %d",
                        conn_id, discov_result.is_success);
        dbg("conn_id %d, GCS_BY_UUID_SRV_DISCOV, is_success %d \n",
                        conn_id, discov_result.is_success);

        for (i = 0; i < discov_result.result_num; i++)
        {
            p_result_table = &(discov_result.p_result_table[i]);
            switch (p_result_table->result_type)
            {
            case DISC_RESULT_SRV_DATA:
                APP_PRINT_INFO3("SRV DATA[%d]: service range: 0x%x-0x%x",
                                i, p_result_table->result_data.srv_disc_data.att_handle,
                                p_result_table->result_data.srv_disc_data.end_group_handle);
                dbg("SRV DATA[%d]: service range: 0x%x-0x%x \n",
                                i, p_result_table->result_data.srv_disc_data.att_handle,
                                p_result_table->result_data.srv_disc_data.end_group_handle);
                break;

            default:
                APP_PRINT_ERROR0("Invalid Discovery Result Type!");
                dbg("Invalid Discovery Result Type! \n");
                break;
            }
        }
        break;

    case GCS_ALL_CHAR_DISCOV:
        APP_PRINT_INFO2("conn_id %d, GCS_ALL_CHAR_DISCOV, is_success %d",
                        conn_id, discov_result.is_success);
        dbg("conn_id %d, GCS_ALL_CHAR_DISCOV, is_success %d \n",
                        conn_id, discov_result.is_success);

        for (i = 0; i < discov_result.result_num; i++)
        {
            p_result_table = &(discov_result.p_result_table[i]);
            switch (p_result_table->result_type)
            {
            case DISC_RESULT_CHAR_UUID16:
                properties = p_result_table->result_data.char_uuid16_disc_data.properties;
                APP_PRINT_INFO5("CHAR UUID16[%d]: decl_handle 0x%x, properties 0x%x, value_handle 0x%x, uuid16 0x%x",
                                i, p_result_table->result_data.char_uuid16_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid16_disc_data.properties,
                                p_result_table->result_data.char_uuid16_disc_data.value_handle,
                                p_result_table->result_data.char_uuid16_disc_data.uuid16);
                APP_PRINT_INFO5("properties:indicate %d, read %d, write cmd %d, write %d, notify %d\r\n",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY);
                dbg("CHAR UUID16[%d]: decl_handle 0x%x, properties 0x%x, value_handle 0x%x, uuid16 0x%x \n",
                                i, p_result_table->result_data.char_uuid16_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid16_disc_data.properties,
                                p_result_table->result_data.char_uuid16_disc_data.value_handle,
                                p_result_table->result_data.char_uuid16_disc_data.uuid16);
                dbg("properties:indicate %d, read %d, write cmd %d, write %d, notify %d \n",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY);
                break;

            case DISC_RESULT_CHAR_UUID128:
                properties = p_result_table->result_data.char_uuid128_disc_data.properties;
                APP_PRINT_INFO5("CHAR UUID128[%d]:  decl hndl=0x%x, prop=0x%x, value hndl=0x%x, uuid128=<%b>",
                                i, p_result_table->result_data.char_uuid128_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid128_disc_data.properties,
                                p_result_table->result_data.char_uuid128_disc_data.value_handle,
                                TRACE_BINARY(16, p_result_table->result_data.char_uuid128_disc_data.uuid128));
                APP_PRINT_INFO5("properties:indicate %d, read %d, write cmd %d, write %d, notify %d",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY
                               );
                dbg("CHAR UUID128[%d]:  decl hndl=0x%x, prop=0x%x, value hndl=0x%x, uuid128="UUID_128_FORMAT" \n",
                                i, p_result_table->result_data.char_uuid128_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid128_disc_data.properties,
                                p_result_table->result_data.char_uuid128_disc_data.value_handle,
                                UUID_128(p_result_table->result_data.char_uuid128_disc_data.uuid128));
                dbg("properties:indicate %d, read %d, write cmd %d, write %d, notify %d \n",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY
                               );

                break;
            default:
                APP_PRINT_ERROR0("Invalid Discovery Result Type!");
                dbg("Invalid Discovery Result Type! \n");
                break;
            }
        }
        break;

    case GCS_BY_UUID_CHAR_DISCOV:
        APP_PRINT_INFO2("conn_id %d, GCS_BY_UUID_CHAR_DISCOV, is_success %d",
                        conn_id, discov_result.is_success);
        dbg("conn_id %d, GCS_BY_UUID_CHAR_DISCOV, is_success %d \n",
                        conn_id, discov_result.is_success);

        for (i = 0; i < discov_result.result_num; i++)
        {
            p_result_table = &(discov_result.p_result_table[i]);
            switch (p_result_table->result_type)
            {
            case DISC_RESULT_BY_UUID16_CHAR:
                properties = p_result_table->result_data.char_uuid16_disc_data.properties;
                APP_PRINT_INFO5("UUID16 CHAR[%d]: Characteristics by uuid16, decl hndl=0x%x, prop=0x%x, value hndl=0x%x, uuid16=<0x%x>",
                                i, p_result_table->result_data.char_uuid16_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid16_disc_data.properties,
                                p_result_table->result_data.char_uuid16_disc_data.value_handle,
                                p_result_table->result_data.char_uuid16_disc_data.uuid16);
                APP_PRINT_INFO5("properties:indicate %d, read %d, write cmd %d, write %d, notify %d",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY
                               );
                dbg("UUID16 CHAR[%d]: Characteristics by uuid16, decl hndl=0x%x, prop=0x%x, value hndl=0x%x, uuid16=<0x%x> \n",
                                i, p_result_table->result_data.char_uuid16_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid16_disc_data.properties,
                                p_result_table->result_data.char_uuid16_disc_data.value_handle,
                                p_result_table->result_data.char_uuid16_disc_data.uuid16);
                dbg("properties:indicate %d, read %d, write cmd %d, write %d, notify %d \n",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY
                               );
                break;

            default:
                APP_PRINT_ERROR0("Invalid Discovery Result Type!");
                dbg("Invalid Discovery Result Type! \n");
                break;
            }
        }
        break;

    case GCS_BY_UUID128_CHAR_DISCOV:
        APP_PRINT_INFO2("conn_id %d, GCS_BY_UUID128_CHAR_DISCOV, is_success %d",
                        conn_id, discov_result.is_success);
        dbg("conn_id %d, GCS_BY_UUID128_CHAR_DISCOV, is_success %d \n",
                        conn_id, discov_result.is_success);

        for (i = 0; i < discov_result.result_num; i++)
        {
            p_result_table = &(discov_result.p_result_table[i]);
            switch (p_result_table->result_type)
            {
            case DISC_RESULT_BY_UUID128_CHAR:
                properties = p_result_table->result_data.char_uuid128_disc_data.properties;
                APP_PRINT_INFO5("UUID128 CHAR[%d]: Characteristics by uuid128, decl hndl=0x%x, prop=0x%x, value hndl=0x%x, uuid128=<%b>",
                                i, p_result_table->result_data.char_uuid128_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid128_disc_data.properties,
                                p_result_table->result_data.char_uuid128_disc_data.value_handle,
                                TRACE_BINARY(16, p_result_table->result_data.char_uuid128_disc_data.uuid128));
                APP_PRINT_INFO5("properties:indicate %d, read %d, write cmd %d, write %d, notify %d",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY
                               );
                dbg("UUID128 CHAR[%d]: Characteristics by uuid128, decl hndl=0x%x, prop=0x%x, value hndl=0x%x, uuid128="UUID_128_FORMAT" \n",
                                i, p_result_table->result_data.char_uuid128_disc_data.decl_handle,
                                p_result_table->result_data.char_uuid128_disc_data.properties,
                                p_result_table->result_data.char_uuid128_disc_data.value_handle,
                                UUID_128(p_result_table->result_data.char_uuid128_disc_data.uuid128));
                dbg("properties:indicate %d, read %d, write cmd %d, write %d, notify %d \n",
                                properties & GATT_CHAR_PROP_INDICATE,
                                properties & GATT_CHAR_PROP_READ,
                                properties & GATT_CHAR_PROP_WRITE_NO_RSP,
                                properties & GATT_CHAR_PROP_WRITE,
                                properties & GATT_CHAR_PROP_NOTIFY
                               );

                break;

            default:
                APP_PRINT_ERROR0("Invalid Discovery Result Type!");
                dbg("Invalid Discovery Result Type! \n");
                break;
            }
        }
        break;

    case GCS_ALL_CHAR_DESC_DISCOV:
        APP_PRINT_INFO2("conn_id %d, GCS_ALL_CHAR_DESC_DISCOV, is_success %d\r\n",
                        conn_id, discov_result.is_success);
        dbg("conn_id %d, GCS_ALL_CHAR_DESC_DISCOV, is_success %d \n",
                        conn_id, discov_result.is_success);
        for (i = 0; i < discov_result.result_num; i++)
        {
            p_result_table = &(discov_result.p_result_table[i]);
            switch (p_result_table->result_type)
            {
            case DISC_RESULT_CHAR_DESC_UUID16:
                APP_PRINT_INFO3("DESC UUID16[%d]: Descriptors handle=0x%x, uuid16=<0x%x>",
                                i, p_result_table->result_data.char_desc_uuid16_disc_data.handle,
                                p_result_table->result_data.char_desc_uuid16_disc_data.uuid16);
                dbg("DESC UUID16[%d]: Descriptors handle=0x%x, uuid16=<0x%x> \n",
                                i, p_result_table->result_data.char_desc_uuid16_disc_data.handle,
                                p_result_table->result_data.char_desc_uuid16_disc_data.uuid16);
                break;
            case DISC_RESULT_CHAR_DESC_UUID128:
                APP_PRINT_INFO3("DESC UUID128[%d]: Descriptors handle=0x%x, uuid128=<%b>",
                                i, p_result_table->result_data.char_desc_uuid128_disc_data.handle,
                                TRACE_BINARY(16, p_result_table->result_data.char_desc_uuid128_disc_data.uuid128));
                dbg("DESC UUID128[%d]: Descriptors handle=0x%x, uuid128="UUID_128_FORMAT" \n",
                                i, p_result_table->result_data.char_desc_uuid128_disc_data.handle,
                                UUID_128(p_result_table->result_data.char_desc_uuid128_disc_data.uuid128));
                break;

            default:
                APP_PRINT_ERROR0("Invalid Discovery Result Type!");
                dbg("Invalid Discovery Result Type! \n");
                break;
            }
        }
        break;

    default:
        APP_PRINT_ERROR2("Invalid disc type: conn_id %d, discov_type %d",
                         conn_id, discov_result.discov_type);
        dbg("Invalid disc type: conn_id %d, discov_type %d \n",
                         conn_id, discov_result.discov_type);
        break;
    }
}
/**
 * @brief  Callback will be called when data sent from gcs client.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_GCS_WRITE_RESULT g_scatternet_write_result = {0};
T_GCS_WRITE_RESULT g_scatternet_write_no_rsp_result = {0};
T_APP_RESULT ble_tizenrt_scatternet_gcs_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
    T_APP_RESULT  result = APP_RESULT_SUCCESS;
    APP_PRINT_INFO2("ble_tizenrt_scatternet_gcs_client_callback: client_id %d, conn_id %d",
                    client_id, conn_id);
    debug_print("client_id %d, conn_id %d \n", client_id, conn_id);
    if (client_id == ble_tizenrt_scatternet_gcs_client_id)
    {
        T_GCS_CLIENT_CB_DATA *p_gcs_cb_data = (T_GCS_CLIENT_CB_DATA *)p_data;
        switch (p_gcs_cb_data->cb_type)
        {
        case GCS_CLIENT_CB_TYPE_DISC_RESULT:
            ble_tizenrt_scatternet_gcs_handle_discovery_result(conn_id, p_gcs_cb_data->cb_content.discov_result);
            break;
        case GCS_CLIENT_CB_TYPE_READ_RESULT:
            APP_PRINT_INFO3("READ RESULT: cause 0x%x, handle 0x%x,  value_len %d",
                            p_gcs_cb_data->cb_content.read_result.cause,
                            p_gcs_cb_data->cb_content.read_result.handle,
                            p_gcs_cb_data->cb_content.read_result.value_size);
            debug_print("READ RESULT: cause 0x%x, handle 0x%x, value_len %d \n",
                            p_gcs_cb_data->cb_content.read_result.cause,
                            p_gcs_cb_data->cb_content.read_result.handle,
                            p_gcs_cb_data->cb_content.read_result.value_size);
            ble_tizenrt_scatternet_read_results[conn_id].cause = p_gcs_cb_data->cb_content.read_result.cause;
            if (p_gcs_cb_data->cb_content.read_result.cause == GAP_SUCCESS)
            {
                APP_PRINT_INFO1("READ VALUE: %b",
                                TRACE_BINARY(p_gcs_cb_data->cb_content.read_result.value_size,
                                             p_gcs_cb_data->cb_content.read_result.p_value));
                debug_print("READ VALUE: \n");
				for(int i=0; i< p_gcs_cb_data->cb_content.read_result.value_size; i++)
                    debug_print("0x%2X", *(p_gcs_cb_data->cb_content.read_result.p_value + i));

                ble_tizenrt_scatternet_read_results[conn_id].read_data.length = p_gcs_cb_data->cb_content.read_result.value_size;
                ble_tizenrt_scatternet_read_results[conn_id].read_data.data = os_mem_alloc(0, p_gcs_cb_data->cb_content.read_result.value_size);
                if(ble_tizenrt_scatternet_read_results[conn_id].read_data.data)
                {
                    memcpy(ble_tizenrt_scatternet_read_results[conn_id].read_data.data,
                                        p_gcs_cb_data->cb_content.read_result.p_value,
                                        p_gcs_cb_data->cb_content.read_result.value_size);
                } else {
                    debug_print("Memory allocation failed \n");
                }
            }
            if(os_mutex_give(ble_tizenrt_read_sem))
            {
                debug_print("receive read result \n");
            } else {
                if(p_gcs_cb_data->cb_content.read_result.cause == GAP_SUCCESS)
                    os_mem_free(ble_tizenrt_scatternet_read_results[conn_id].read_data.data);
                debug_print("fail to give read semaphore \n");
            }
            break;
        case GCS_CLIENT_CB_TYPE_WRITE_RESULT:
            APP_PRINT_INFO3("WRITE RESULT: cause 0x%x ,handle 0x%x, type %d",
                            p_gcs_cb_data->cb_content.write_result.cause,
                            p_gcs_cb_data->cb_content.write_result.handle,
                            p_gcs_cb_data->cb_content.write_result.type);
            debug_print("[%s] WRITE RESULT: cause 0x%x ,handle 0x%x, type 0x%x \n",
                            p_gcs_cb_data->cb_content.write_result.cause,
                            p_gcs_cb_data->cb_content.write_result.handle,
                            p_gcs_cb_data->cb_content.write_result.type);
            switch (p_gcs_cb_data->cb_content.write_result.type)
            {
            case GATT_WRITE_TYPE_REQ:
                g_scatternet_write_result.cause = p_gcs_cb_data->cb_content.write_result.cause;
                g_scatternet_write_result.handle = p_gcs_cb_data->cb_content.write_result.handle;
                g_scatternet_write_result.type = p_gcs_cb_data->cb_content.write_result.type;
                if(os_mutex_give(ble_tizenrt_write_sem))
                {
                debug_print("receive write response \n");
                } else {
                debug_print("fail to give write semaphore \n");
                }
                break;
            case GATT_WRITE_TYPE_CMD:
                g_scatternet_write_no_rsp_result.cause = p_gcs_cb_data->cb_content.write_result.cause;
                g_scatternet_write_no_rsp_result.handle = p_gcs_cb_data->cb_content.write_result.handle;
                g_scatternet_write_no_rsp_result.type = p_gcs_cb_data->cb_content.write_result.type;
                if(os_mutex_give(ble_tizenrt_write_no_rsp_sem))
                {
                    debug_print("send write cmd success \n");
                } else {
                    debug_print("fail to send write cmd \n");
                }
                break;
            default:
                break;
            }
            break;
        case GCS_CLIENT_CB_TYPE_NOTIF_IND:
            if (p_gcs_cb_data->cb_content.notif_ind.notify == false)
            {
                APP_PRINT_INFO2("INDICATION: handle 0x%x, value_size %d",
                                p_gcs_cb_data->cb_content.notif_ind.handle,
                                p_gcs_cb_data->cb_content.notif_ind.value_size);
                debug_print("INDICATION: handle 0x%x, value_size %d",
                                 p_gcs_cb_data->cb_content.notif_ind.handle,
                                 p_gcs_cb_data->cb_content.notif_ind.value_size);
                T_TIZENRT_CLIENT_INDICATION *indicate_result = os_mem_alloc(0, sizeof(T_TIZENRT_CLIENT_INDICATION));
                if (indicate_result) {
                    indicate_result->indi_data.data = os_mem_alloc(0, p_gcs_cb_data->cb_content.notif_ind.value_size);
                    if (indicate_result->indi_data.data)
                    {
                        memcpy(indicate_result->indi_data.data, p_gcs_cb_data->cb_content.notif_ind.p_value,
                                                        p_gcs_cb_data->cb_content.notif_ind.value_size);
                        indicate_result->indi_data.length = p_gcs_cb_data->cb_content.notif_ind.value_size;
                        indicate_result->handle.conn_handle = conn_id;
                        indicate_result->handle.attr_handle = p_gcs_cb_data->cb_content.notif_ind.handle;
                        debug_print("Indication: 0x \n");
                        for (int i = 0; i < indicate_result->indi_data.length; i++)
                        {
                            debug_print("%x",indicate_result->indi_data.data[i]);
                        }
                        if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_INDICATION_MSG, indicate_result) == false)
                        {
                            os_mem_free(indicate_result->indi_data.data);
                            os_mem_free(indicate_result);
                            debug_print("callback msg send fail \n");
                        }
                    } else {
                        debug_print("Memory allocation failed \n");
                    }
                }
            }
            else
            {
                APP_PRINT_INFO2("NOTIFICATION: handle 0x%x, value_size %d",
                                p_gcs_cb_data->cb_content.notif_ind.handle,
                                p_gcs_cb_data->cb_content.notif_ind.value_size);
                T_TIZENRT_CLIENT_NOTIFICATION *notify_result = os_mem_alloc(0, sizeof(T_TIZENRT_CLIENT_NOTIFICATION));
                if (notify_result) {
                    notify_result->noti_data.data = os_mem_alloc(0, p_gcs_cb_data->cb_content.notif_ind.value_size);
                    if (notify_result->noti_data.data)
                    {
                        memcpy(notify_result->noti_data.data, p_gcs_cb_data->cb_content.notif_ind.p_value,
                                                        p_gcs_cb_data->cb_content.notif_ind.value_size);
                        notify_result->noti_data.length = p_gcs_cb_data->cb_content.notif_ind.value_size;
                        notify_result->handle.conn_handle = conn_id;
                        notify_result->handle.attr_handle = p_gcs_cb_data->cb_content.notif_ind.handle;
                        debug_print("Notification: 0x \n");
                        for (int i = 0; i < notify_result->noti_data.length; i++)
                        {
                            debug_print("%x",notify_result->noti_data.data[i]);
                        }
                        if(ble_tizenrt_scatternet_send_callback_msg(BLE_TIZENRT_NOTIFICATION_MSG, notify_result) == false)
                        {
                            os_mem_free(notify_result->noti_data.data);
                            os_mem_free(notify_result);
                            debug_print("callback msg send fail \n");
                        }
                    } else {
                        debug_print("Memory allocation failed \n");
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    return result;
}

extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];
#if defined(CONFIG_BLE_INDICATION)
T_SEND_DATA_RESULT g_scatternet_indicate_result = {0};
#endif
T_APP_RESULT ble_tizenrt_scatternet_app_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == SERVICE_PROFILE_GENERAL_ID)
    {
        T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
        switch (p_param->eventId)
        {
        case PROFILE_EVT_SRV_REG_COMPLETE:
            debug_print("PROFILE_EVT_SRV_REG_COMPLETE: result %d \n",
                            p_param->event_data.service_reg_result);
            break;

        case PROFILE_EVT_SEND_DATA_COMPLETE:
            debug_print("PROFILE_EVT_SEND_DATA_COMPLETE: conn_id %d, cause 0x%x, service_id %d, attrib_idx 0x%x, credits %d \n",
                            p_param->event_data.send_data_result.conn_id,
                            p_param->event_data.send_data_result.cause,
                            p_param->event_data.send_data_result.service_id,
                            p_param->event_data.send_data_result.attrib_idx,
                            p_param->event_data.send_data_result.credits);
            if (p_param->event_data.send_data_result.cause == GAP_SUCCESS)
            {
                dbg("PROFILE_EVT_SEND_DATA_COMPLETE success \n");
            }
            else
            {
                dbg("PROFILE_EVT_SEND_DATA_COMPLETE failed \n");
            }

#if defined(CONFIG_BLE_INDICATION)
            g_scatternet_indicate_result.credits = p_param->event_data.send_data_result.credits;
            g_scatternet_indicate_result.cause = p_param->event_data.send_data_result.cause,
            g_scatternet_indicate_result.service_id = p_param->event_data.send_data_result.service_id,
            g_scatternet_indicate_result.attrib_idx = p_param->event_data.send_data_result.attrib_idx,
            g_scatternet_indicate_result.conn_id = p_param->event_data.send_data_result.conn_id;

            if(os_mutex_give(ble_tizenrt_indicate_sem))
            {
                dbg("give indicate semaphore success \n");
            } else {
                dbg("fail to give indicate semaphore \n");
            }
#endif
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
                debug_print("service_id 0x%x att_handle 0x%x \n",
                                            tizenrt_ble_srv_database[p_tizenrt_cb_data->srv_index].srv_id,
                                            p_cha_info->abs_handle);
                if(p_tizenrt_cb_data->val & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
                {
                    dbg("cccd 0x%x update : notify enable \n", p_cha_info->abs_handle);
                } else {
                    dbg("cccd 0x%x update : notify disable \n", p_cha_info->abs_handle);
                }
                if(p_tizenrt_cb_data->val & GATT_CLIENT_CHAR_CONFIG_INDICATE)
                {
                    dbg("cccd 0x%x update : indicate enable \n", p_cha_info->abs_handle);
                } else {
                    dbg("cccd 0x%x update : indicate disable \n", p_cha_info->abs_handle);
                }
                break;
            }

        case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE:
            {
                debug_print("service_id 0x%x index 0x%x abs_handle 0x%x \n", 
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
                    debug_print("NULL read callback abs_handle 0x%x \n", p_cha_info->abs_handle);
                }
                break;
            }

        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
            {
                debug_print("service_id 0x%x Attribute 0x%x write_type %d len %d data 0x \n",
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
                            debug_print("NULL write callback abs_handle 0x%x \n", p_cha_info->abs_handle);
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
                            debug_print("NULL write callback abs_handle 0x%x \n", p_cha_info->abs_handle);
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
/** @} */ /* End of group GCS_CLIIENT_CALLBACK */
/** @} */ /* End of group CENTRAL_CLIENT_APP */
