/*
 * ble_tizenrt_server.c
 *
 *  Created on: 2020. 7. 27.
 *      Author: lexi_zhao
 */

#ifndef TRBLE_SERVER_C_
#define TRBLE_SERVER_C_

#include "stddef.h"
#include <rtk_bt_def.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_vendor.h>
#include <mem_types.h>
#include <bt_utils.h>
#include <rtk_service_config.h>
#include <rtk_stack_gatt.h>
#include <gap_msg.h>
#include <ble_tizenrt_service.h>

bool is_server_init = false;
uint16_t server_profile_count = 0;
trble_server_init_config server_init_parm;
uint16_t rtw_ble_server_adv_interval = 352;
trble_adv_type_e rtw_ble_server_adv_type = 0;
uint8_t rtw_ble_server_direct_addr_type;
static uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN];
static uint8_t adv_direct_addr[TRBLE_BD_ADDR_MAX_LEN];
extern TIZENERT_SRV_CNT tizenrt_ble_srv_count;
extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];

trble_result_e rtw_ble_server_init(trble_server_init_config* init_parm)
{	
    if (init_parm == NULL || init_parm->profile == NULL || init_parm->profile_count == 0)
    {
        debug_print("Invalid parameters 0x%x 0x%x %d \n", init_parm, init_parm->profile, init_parm->profile_count);
        return TRBLE_INVALID_ARGS;
    }

    server_profile_count = init_parm->profile_count;

    uint16_t gatt_char_num = 0;
    for (int i = 0; i < init_parm->profile_count; i++)
    {
        if(init_parm->profile[i].type == TRBLE_GATT_CHARACT)
            gatt_char_num++;
    }
    server_init_parm.profile_count = init_parm->profile_count + gatt_char_num;
    server_init_parm.profile = init_parm->profile;
    server_init_parm.connected_cb = init_parm->connected_cb;
    server_init_parm.is_secured_connect_allowed = init_parm->is_secured_connect_allowed;

	ble_tizenrt_peripheral_main(1);

    is_server_init = true;
    debug_print("Tizenrt Sever Init Success Profile_count %d \n", server_init_parm.profile_count);
    return TRBLE_SUCCESS;
}

uint16_t rtw_ble_server_get_profile_count(void)
{
    return server_profile_count;
}

trble_result_e rtw_ble_server_deinit(void)
{   
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    ble_tizenrt_peripheral_main(0);
	memset(tizenrt_ble_srv_database, 0, (7 * sizeof(TIZENERT_SRV_DATABASE)));
	tizenrt_ble_srv_count = 0;
    is_server_init = false;
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_server_get_mac_address(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

	rtk_bt_le_addr_t rtk_bd_addr;

    if (RTK_BT_FAIL == rtk_bt_le_gap_get_address(&rtk_bd_addr)) {
        debug_print("Failed to get local addr. \n");
        return TRBLE_FAIL;
    }

	memcpy(mac, &rtk_bd_addr.addr_val, TRBLE_BD_ADDR_MAX_LEN);
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_sm_set_security_param(rtk_bt_le_security_param_t  sec_param)
{
    rtk_bt_le_security_param_t sec_param_input;
    memcpy(&sec_param_input, &sec_param, sizeof(rtk_bt_le_security_param_t));
    sec_param_input.auto_sec_req = 0;

    uint8_t pairing_mode = RTK_PAIRING_MODE_PAIRABLE;
    if(RTK_BT_OK != rtk_bt_le_sm_set_pairing_mode(pairing_mode))
    {
        dbg("secure param set fail \n");
        return TRBLE_FAIL;
    }

    if(RTK_BT_OK != rtk_bt_le_sm_set_security_param(&sec_param_input))
    {
        dbg("secure param set fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_pairing_passkey_confirm(uint8_t *conn_handle, uint8_t *confirm)
{
    rtk_bt_le_auth_key_confirm_t pair_cfm_param = {0};
    pair_cfm_param.conn_handle = *conn_handle;
    pair_cfm_param.confirm = *confirm;
    if(RTK_BT_OK != rtk_bt_le_sm_passkey_confirm(&pair_cfm_param))
    {
        debug_print("passkey confirm fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

/* set data pointer of attribute value */
trble_result_e rtw_ble_server_att_set_data_ptr(trble_attr_handle attr_handle, uint8_t *new_data_ptr)
{
    debug_print("new_data_ptr %p \n", new_data_ptr);
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    for(int i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(tizenrt_ble_srv_database[i].start_handle < attr_handle && 
            tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= attr_handle)
        {
            for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
            {
                if(attr_handle == tizenrt_ble_srv_database[i].chrc_info[j].abs_handle)
                {
                    tizenrt_ble_srv_database[i].chrc_info[j].read_ptr = new_data_ptr;
                    debug_print("tizenrt_ble_srv_database[%d].chrc_info[%d].read_ptr %p \n", i, j,
                                                        tizenrt_ble_srv_database[i].chrc_info[j].read_ptr);
                    return TRBLE_SUCCESS;
                }
            }
        }
    }
    return TRBLE_FAIL; 
}

/* set current length of attribute value */
trble_result_e rtw_ble_server_att_set_length(trble_attr_handle attr_handle, uint16_t new_length)
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    for(int i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if((tizenrt_ble_srv_database[i].start_handle < attr_handle) && 
            (tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= attr_handle))
        {
            for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
            {
                if(attr_handle == tizenrt_ble_srv_database[i].chrc_info[j].abs_handle)
                {
                    tizenrt_ble_srv_database[i].chrc_info[j].read_len = new_length;
					debug_print("rtw_ble_server_att_set_length \r\n");
                    return TRBLE_SUCCESS;
                }
            }
        }
    }

     return TRBLE_FAIL;
}

/* get data pointer of attribute value */
uint8_t* rtw_ble_server_att_get_data_ptr(trble_attr_handle attr_handle)
{
    if (is_server_init != true)
    {
        return NULL;
    }

    for(int i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(tizenrt_ble_srv_database[i].start_handle < attr_handle && 
            tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= attr_handle)
        {
            for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
            {
                if(attr_handle == tizenrt_ble_srv_database[i].chrc_info[j].abs_handle)
                {
                    debug_print("abs_handle 0x%x index 0x%x %p \n", tizenrt_ble_srv_database[i].chrc_info[j].abs_handle,
                                                tizenrt_ble_srv_database[i].chrc_info[j].index,
                                                tizenrt_ble_srv_database[i].chrc_info[j].data);
                    return tizenrt_ble_srv_database[i].chrc_info[j].data;
                }
            }
        }
    }

    return NULL; 
}

/* get current length of attribute value */
uint16_t rtw_ble_server_att_get_length(trble_attr_handle attr_handle)
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    for(int i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(tizenrt_ble_srv_database[i].start_handle < attr_handle && 
            tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= attr_handle)
        {
            for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
            {
                if(attr_handle == tizenrt_ble_srv_database[i].chrc_info[j].abs_handle)
                {
                    debug_print("abs_handle 0x%x att_index 0x%x %d \n", tizenrt_ble_srv_database[i].chrc_info[j].abs_handle,
                                                tizenrt_ble_srv_database[i].chrc_info[j].index,
                                                tizenrt_ble_srv_database[i].chrc_info[j].data_len);
                    return tizenrt_ble_srv_database[i].chrc_info[j].data_len;
                }
            }
        }
    }

    return 0; 
}

trble_result_e rtw_ble_server_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *data_ptr, uint16_t data_length)
{	
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if (attr_handle == 0x0000) /* invalid attr_handle */
    {
        return TRBLE_NOT_FOUND;
    } 

	T_SERVER_ID app_id = 0;
    uint16_t cha_index = 0;
    rtk_bt_gatts_ntf_and_ind_param_t param;

    param.data = (void *)osif_mem_alloc(0, data_length);
    if(!param.data)
    {
        osif_mem_free(param.data);
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    for(uint8_t i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(tizenrt_ble_srv_database[i].start_handle < attr_handle && 
            tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= attr_handle)
        {
            app_id = TIZENRT_SRV_ID + i;
            cha_index = attr_handle - tizenrt_ble_srv_database[i].start_handle;
        }
    }
	param.app_id = app_id;
	param.conn_handle = con_handle;
	param.index = cha_index;
    memcpy((void*)param.data, data_ptr, data_length);
    param.len = data_length;
    param.seq = 0;

    if(RTK_BT_OK != rtk_bt_gatts_notify(&param))
    {
        osif_mem_free(param.data);
        debug_print("send notify fail \n");
        return TRBLE_FAIL;
    }
	osif_mem_free(param.data);
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_charact_indicate(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *data_ptr, uint16_t data_length)
{
	if (is_server_init != true)
	   {
		   return TRBLE_INVALID_STATE;
	   }

	   if (attr_handle == 0x0000) /* invalid attr_handle */
	   {
		   return TRBLE_NOT_FOUND;
	   } 

	   T_SERVER_ID app_id = 0;
	   uint16_t cha_index = 0;
	   rtk_bt_gatts_ntf_and_ind_param_t param;

	   param.data = (void *)osif_mem_alloc(0, data_length);
	   if(!param.data)
	   {
		   osif_mem_free(param.data);
		   debug_print("Memory allocation failed \n");
		   return TRBLE_FAIL;
	   }

	   for(uint8_t i = 0; i < tizenrt_ble_srv_count; i++)
	   {
		   if(tizenrt_ble_srv_database[i].start_handle < attr_handle && 
			   tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= attr_handle)
		   {
			   app_id = TIZENRT_SRV_ID + i;
			   cha_index = attr_handle - tizenrt_ble_srv_database[i].start_handle;
		   }
	   }
	   param.app_id = app_id;
	   param.conn_handle = con_handle;
	   param.index = cha_index;
	   memcpy((void*)param.data, data_ptr, data_length);
	   param.len = data_length;
	   param.seq = 0;

	   if(RTK_BT_OK != rtk_bt_gatts_indicate(&param))
	   {
		   osif_mem_free(param.data);
		   debug_print("send indication fail \n");
		   return TRBLE_FAIL;
	   }
	   osif_mem_free(param.data);
	   return TRBLE_SUCCESS;
}

extern rtk_bt_gatts_app_priv_t *g_rtk_bt_gatts_priv;
trble_result_e rtw_ble_server_indicate_queue_cnt(trble_conn_handle *con_handle, uint8_t *indication_count)
{
	uint8_t con_id;
	rtk_bt_le_gap_get_conn_id(*con_handle, &con_id); 
	rtk_bt_gatt_queue_t *queue;
	queue = &g_rtk_bt_gatts_priv->indicate_queue[con_id];
	*indication_count = queue->pending_ele_num;

	/* The number of element in pending queue should be limited, otherwise
		the notification of high frequnce will use up memory */
	if (queue->pending_ele_num >= BT_QUEUE_PENDING_ELEMENT_MAX)
	{
		debug_print("Error: Server indicate pending queue full, wait a moment to send data again !!!\r\n");
		return TRBLE_BUSY;
	}

	return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_reject(trble_attr_handle attr_handle, uint8_t app_errorcode)
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if (attr_handle == 0x0000) /* invalid attr_handle */
    {
        return TRBLE_NOT_FOUND;
    }

    if(app_errorcode < 0x81)
    {    
        return TRBLE_FAIL;
    }

    if(ble_tizenrt_set_server_reject(attr_handle, app_errorcode))
    {
        return TRBLE_SUCCESS; 
    } else {
        debug_print("fail \n");
        return TRBLE_FAIL;
    }
}

uint8_t* rtw_ble_server_get_mac_address_by_conn_handle(trble_conn_handle con_handle)
{
	rtk_bt_le_conn_info_t info;
    memset(bd_addr, 0, TRBLE_BD_ADDR_MAX_LEN);

    if(RTK_BT_OK == rtk_bt_le_gap_get_conn_info(con_handle, &info))
    {
    	memcpy(bd_addr, info.remote.addr_val, TRBLE_BD_ADDR_MAX_LEN);
        debug_print("success \n");
        return bd_addr;
    } else {
        debug_print("fail \n");
        return NULL;
    }
}

trble_conn_handle rtw_ble_server_get_conn_handle_by_address(uint8_t* mac)
{
	rtk_bt_le_addr_t p_addr;
	memcpy(p_addr.addr_val, mac, RTK_BD_ADDR_LEN);
	p_addr.type = RTK_BT_LE_ADDR_TYPE_PUBLIC;
	uint16_t conn_handle = 0xffff;

    if(RTK_BT_OK == rtk_bt_le_gap_get_conn_handle_by_addr(&p_addr, &conn_handle))
    {
        debug_print("get public add handle \n");
        return conn_handle;
    }

	p_addr.type = RTK_BT_LE_ADDR_TYPE_RANDOM;
	if(RTK_BT_OK == rtk_bt_le_gap_get_conn_handle_by_addr(&p_addr, &conn_handle))
    {
        debug_print("get random add handle \n");
        return conn_handle;
    } 
	
    return conn_handle;
}

trble_result_e rtw_ble_server_set_device_name(uint8_t* name)
{
    if(RTK_BT_OK == rtk_bt_le_gap_set_device_name(name))
    {
        debug_print("Set device name success \n");
    } else {
        debug_print("Set device name fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_adv_into_idle(void)
{
	uint8_t wcount = 0;
	rtk_bt_le_gap_dev_state_t new_state;
	if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state)) {
		dbg("Get dev state fail \n");
		return TRBLE_FAIL;
	}

    switch (new_state.gap_adv_state)
    {
    case GAP_ADV_STATE_ADVERTISING:
        {
            debug_print("ADV STATE : ADVERTISING \n");
			if(RTK_BT_OK != rtk_bt_le_gap_stop_adv()) {
				dbg("stop adv fail \n");
                return TRBLE_FAIL;
            }
        }
        break;
    case GAP_ADV_STATE_START:
        {
            do {
                debug_print("ADV STATE : START \n");
                osif_delay(100);
                if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
			    {
					debug_print("get dev state fail \n");
				}
				wcount++;
				if (wcount > 30) {	/* Wait up to 3 seconds */
					dbg("Wait timeout\n");
					return TRBLE_FAIL;
			    }
            } while(new_state.gap_adv_state == GAP_ADV_STATE_START);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("ADV STATE : ADVERTISING \n");
                if(RTK_BT_OK != rtk_bt_le_gap_stop_adv()) {
					dbg("Stop adv fail \n");
                    return TRBLE_FAIL;
                }
            }
        }
        break;
    case GAP_ADV_STATE_STOP:
        {
            do {
                debug_print("ADV STATE : STOPPING \n");
                osif_delay(100);
				if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
				{
					debug_print("get dev state fail \n");
				}
				wcount++;
				if (wcount > 30) {	/* Wait up to 3 seconds */
					dbg("Wait timeout\n");
					return TRBLE_FAIL;
				}
            } while(new_state.gap_adv_state == GAP_ADV_STATE_STOP);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("ADV STATE : ADVERTISING \n");
                if(RTK_BT_OK != rtk_bt_le_gap_stop_adv()) {
					dbg("Stop adv fail \n");
                    return TRBLE_FAIL;
                }
            }
        }
        break;
    default:
        break;
    }

	wcount = 0;
    do {
        debug_print("Waiting for adv idle \n");
        osif_delay(100);
		if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
		{
			debug_print("get dev state fail \n");
		}
		wcount++;
		if (wcount > 30) {	/* Wait up to 3 seconds */
			dbg("Wait timeout\n");
			return TRBLE_FAIL;
		}
    } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);
    debug_print("ADV STATE : IDLE \n");
	return TRBLE_SUCCESS;
}

/* Set Advertisement Data API */
trble_result_e rtw_ble_server_set_adv_data(uint8_t* data, uint16_t length)
{
    if (is_server_init != true)
        return TRBLE_INVALID_STATE;

    if(length > 31)
        return TRBLE_INVALID_ARGS;

	if (TRBLE_SUCCESS != rtw_ble_server_adv_into_idle())
		return TRBLE_FAIL;

    if(RTK_BT_OK == rtk_bt_le_gap_set_adv_data(data, length))
    {
        debug_print("Set adv data success \n");
    } else {
        debug_print("Set adv data fail!!! \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_set_adv_name(uint8_t* data, uint16_t length)
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if(length > 31)
        return TRBLE_INVALID_ARGS;

	if (TRBLE_SUCCESS != rtw_ble_server_adv_into_idle())
		return TRBLE_FAIL;

    if(RTK_BT_OK == rtk_bt_le_gap_set_scan_rsp_data(data, length))
    {
        debug_print("Set adv name success \n");
    } else {
        debug_print("Set adv name fail!!! \n");
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

bool rtw_ble_server_conn_is_active(trble_conn_handle con_handle)
{
    if (is_server_init != true)
    {
        return false;
    }

    rtk_bt_le_conn_info_t info;
    if (RTK_BT_OK == rtk_bt_le_gap_get_conn_info(con_handle, &info))
    {
        return true;
    }
    return false;
}


bool rtw_ble_server_conn_is_any_active(void)
{
    if (is_server_init != true)
    {
        return false;
    }

	rtk_bt_le_get_active_conn_t active_conn;
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
        debug_print("get active conn fail \n");
		return false;
    }

    if(active_conn.conn_num)
        return true;
    else
        return false;

}

trble_result_e rtw_ble_server_conn_param_update(trble_conn_handle *conn_handle, trble_conn_param *conn_param)
{
	if (is_server_init != true)
	{
		return TRBLE_INVALID_STATE;
	}

	if(conn_handle == NULL || conn_param == NULL)
	{
		debug_print("Invalid input \n");
		return TRBLE_INVALID_ARGS;
	}

	rtk_bt_le_update_conn_param_t param;

    param.conn_handle = *conn_handle;
	param.conn_interval_min = conn_param->min_conn_interval;
	param.conn_interval_max = conn_param->max_conn_interval;
	param.conn_latency = conn_param->slave_latency;
	param.supv_timeout = conn_param->supervision_timeout;

	if(RTK_BT_FAIL == rtk_bt_le_gap_update_conn_param(&param))
	{
		debug_print("connection parameter update fail \n");
		return TRBLE_FAIL;
	}
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_server_disconnect(trble_conn_handle con_handle)
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if(RTK_BT_FAIL == rtk_bt_le_gap_disconnect(con_handle))
    {
        debug_print("disconnect fail \n");
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_server_start_adv(void)
{
	rtk_bt_le_get_active_conn_t active_conn;
	if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn)) {
		dbg("Get active conn fail \n");
		return TRBLE_FAIL;
	}

    if(active_conn.conn_num)
    {
        rtk_bt_le_conn_info_t conn_info;
        for(uint8_t i = 0; i < active_conn.conn_num; i++)
        {
			if (RTK_BT_OK != rtk_bt_le_gap_get_conn_info(active_conn.conn_handle[i], &conn_info))
			{
				dbg("Get conn info fail \n");
				return TRBLE_FAIL;
			}
        }
    }

#if !defined (RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
	rtk_bt_le_gap_dev_state_t new_state;
	if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state)) {
		dbg("Get dev state fail \n");
		return TRBLE_FAIL;
	}

	if(new_state.gap_adv_state != GAP_ADV_STATE_IDLE) {
		dbg("Adv is in invalid state! \n");
		return TRBLE_FAIL;
	}
#endif

	rtk_bt_le_adv_param_t adv_param;
	adv_param.interval_min = rtw_ble_server_adv_interval;
	adv_param.interval_max = rtw_ble_server_adv_interval;
	adv_param.type = rtw_ble_server_adv_type;
	adv_param.own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC;
	adv_param.channel_map = RTK_BT_LE_ADV_CHNL_ALL;
	adv_param.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
	rtk_bt_le_addr_t peer_addr;
	if(rtw_ble_server_adv_type == TRBLE_ADV_TYPE_DIRECT) {
		peer_addr.type = rtw_ble_server_direct_addr_type;
		memcpy(peer_addr.addr_val, adv_direct_addr, RTK_BD_ADDR_LEN);
	} else {
		peer_addr.type = RTK_BT_LE_ADDR_TYPE_PUBLIC;
		memset(peer_addr.addr_val, 0, RTK_BD_ADDR_LEN);
	}
	adv_param.peer_addr = peer_addr;

	if(RTK_BT_OK != rtk_bt_le_gap_start_adv(&adv_param)) {
		dbg("Start adv fail \n");
		return TRBLE_FAIL;
	}
	return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_stop_adv(void)
{
	rtk_bt_le_gap_dev_state_t new_state;
	if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state)) {
		dbg("Get dev state fail \n");
		return TRBLE_FAIL;
	}

	if(new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
		return TRBLE_SUCCESS;
	if (new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING) {
		dbg("Adv is in invalid state! \n");
		return TRBLE_FAIL;
	}

	if(RTK_BT_OK != rtk_bt_le_gap_stop_adv()) {
		dbg("Stop adv fail \n");
		return TRBLE_FAIL;
	}
	return TRBLE_SUCCESS;
}

#if defined (RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
trble_result_e rtw_ble_server_create_multi_adv(uint8_t adv_event_prop, uint32_t primary_adv_interval[2],
													 uint8_t own_addr_type, uint8_t *own_addr_val, uint8_t *adv_handle)
{
	rtk_bt_le_ext_adv_param_t adv_param;
	adv_param.adv_event_prop           = adv_event_prop; 
	adv_param.primary_adv_interval_min = primary_adv_interval[0];
	adv_param.primary_adv_interval_max = primary_adv_interval[1];
	adv_param.primary_adv_channel_map = RTK_BT_LE_ADV_CHNL_ALL;
	adv_param.own_addr.type            = own_addr_type;
	memcpy(adv_param.own_addr.addr_val, own_addr_val, 6);
	adv_param.peer_addr.type           = 0;
	memset(adv_param.peer_addr.addr_val,0, 6);
	adv_param.filter_policy            = 0;
	adv_param.tx_power                 = 0x7f;
	adv_param.primary_adv_phy          = 1;
	adv_param.secondary_adv_max_skip   = 0;
	adv_param.secondary_adv_phy        = 1;
	adv_param.adv_sid                  = 0;
	uint16_t ret = rtk_bt_le_gap_create_ext_adv(&adv_param, adv_handle);

	if (RTK_BT_OK != ret) {
		return TRBLE_FAIL;
	} else {
		return TRBLE_SUCCESS;
	}

} 

trble_result_e rtw_ble_server_delete_multi_adv(uint8_t adv_handle)
{
	uint16_t ret = rtk_bt_le_gap_remove_ext_adv(adv_handle);
	if (RTK_BT_OK != ret) {
		return TRBLE_FAIL;
	} else {
		return TRBLE_SUCCESS;
	}
}

trble_result_e rtw_ble_server_set_multi_adv_data(uint8_t adv_handle, uint8_t *pdata, uint16_t len)
{
	uint16_t ret = rtk_bt_le_gap_set_ext_adv_data(adv_handle, pdata, len);
	if (RTK_BT_OK != ret) {
		return TRBLE_FAIL;
	} else {
		return TRBLE_SUCCESS;
	}
}

trble_result_e rtw_ble_server_set_multi_resp_data(uint8_t adv_handle, uint8_t *pdata, uint16_t len)
{
	uint16_t ret = rtk_bt_le_gap_set_ext_scan_rsp_data(adv_handle, pdata, len);
	if (RTK_BT_OK != ret) {
		return TRBLE_FAIL;
	} else {
		return TRBLE_SUCCESS;
	}
}

trble_result_e rtw_ble_server_start_multi_adv(uint8_t adv_handle)
{
	uint16_t duration = 0; 
	int8_t num_vents = 0; 

	uint16_t ret = rtk_bt_le_gap_start_ext_adv(adv_handle, duration, num_vents);
	if (RTK_BT_OK != ret) {
		return TRBLE_FAIL;
	} else {
		return TRBLE_SUCCESS;
	}
}

trble_result_e rtw_ble_server_stop_multi_adv(uint8_t adv_handle)
{
	uint16_t ret = rtk_bt_le_gap_stop_ext_adv(adv_handle);
	if (RTK_BT_OK != ret) {
		return TRBLE_FAIL;
	} else {
		return TRBLE_SUCCESS;
	}
}
#endif

trble_result_e rtw_ble_server_set_adv_txpower(uint16_t txpower)
{
	rtk_bt_vendor_tx_power_param_t tx_power = {0};
	tx_power.tx_power_type = 0;
	tx_power.adv_tx_power.type = 0;
	tx_power.tx_gain = txpower;
	uint16_t ret = rtk_bt_set_tx_power(&tx_power);
	if (RTK_BT_OK != ret) {
		return TRBLE_FAIL;
	} else {
		return TRBLE_SUCCESS;
	}

	return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_get_bonded_device(trble_bonded_device_list_s* bonded_device_list, uint16_t* device_count)
{
    if(bonded_device_list == NULL || device_count == NULL)
    {
        debug_print("Invalid input \n");
        return TRBLE_INVALID_ARGS;
    }

    if(RTK_BT_OK != rtk_bt_le_sm_get_bond_num((uint8_t *)device_count)){
		debug_print("get bond num failed \n");
    }
    debug_print("bonded num : %d \n", *device_count);

	if (!(*device_count))
    {
        debug_print("no bond device! \n");
        return TRBLE_SUCCESS;
    }

	rtk_bt_le_bond_info_t* bond_info = (rtk_bt_le_bond_info_t*)osif_mem_alloc(RAM_TYPE_DATA_ON, (*device_count) * sizeof(rtk_bt_le_bond_info_t));
	if (!bond_info) {
		dbg("%s allocate bond_info fail \r\n", __func__);
		return TRBLE_FAIL;
	}
	memset(bond_info, 0, (*device_count) * sizeof(rtk_bt_le_bond_info_t));
	if(RTK_BT_OK != rtk_bt_le_sm_get_bond_info(bond_info, (uint8_t *)device_count)){
		debug_print("get bond info failed \n");
		osif_mem_free(bond_info);
		return TRBLE_FAIL;
	}
	for(int i = 0; i < (*device_count); i++){
		memcpy(bonded_device_list[i].bd_addr.mac, bond_info[i].remote_addr.addr_val, TRBLE_BD_ADDR_MAX_LEN);
		bonded_device_list[i].bd_addr.type = bond_info[i].remote_addr.type;
		debug_print("bond_dev[%d]: bd 0x%02x%02x%02x%02x%02x%02x, addr_type %d \n",
                            i,
                            bonded_device_list[i].bd_addr.mac[5],
                            bonded_device_list[i].bd_addr.mac[4],
                            bonded_device_list[i].bd_addr.mac[3],
                            bonded_device_list[i].bd_addr.mac[2],
                            bonded_device_list[i].bd_addr.mac[1],
                            bonded_device_list[i].bd_addr.mac[0],
                            bonded_device_list[i].bd_addr.type);
	}

    osif_mem_free(bond_info);

    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_delete_bonded_device(uint8_t bond_addr[TRBLE_BD_ADDR_MAX_LEN])
{
    if(bond_addr == NULL)
    {
        debug_print("Invalid input \n");
        return TRBLE_INVALID_ARGS;
    }

	trble_bonded_device_list_s *bonded_device_list = (trble_bonded_device_list_s *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(trble_bonded_device_list_s));

    if(bonded_device_list == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

	uint16_t device_count = 0;
    trble_result_e ret = TRBLE_FAIL;
    rtk_bt_le_addr_t addr;
	bool bond_addr_found = false;

	rtw_ble_server_get_bonded_device(bonded_device_list, &device_count);
	if (!device_count)
    {
    	osif_mem_free(bonded_device_list);
        return TRBLE_FAIL;
    }

	for(int i = 0; i < device_count; i++)
	{
		if(!memcmp(bonded_device_list[i].bd_addr.mac, bond_addr, TRBLE_BD_ADDR_MAX_LEN))
		{
			addr.type = bonded_device_list[i].bd_addr.type;
			bond_addr_found = true;
			break;
		}
	}
	if(!bond_addr_found)
	{
		debug_print("bond addr not found! \n");
		osif_mem_free(bonded_device_list);
		return TRBLE_FAIL;
	}

	memcpy(addr.addr_val, bond_addr, TRBLE_BD_ADDR_MAX_LEN);

    if(RTK_BT_OK == rtk_bt_le_sm_delete_bond_device(&addr))
    {
        debug_print("success \n");
        ret = TRBLE_SUCCESS;
    }else{
        debug_print("delete bonded device failed \n");
    }

	if (bonded_device_list)
        osif_mem_free(bonded_device_list);

    return ret;
}

trble_result_e rtw_ble_server_delete_bonded_device_all(void)
{
    if(RTK_BT_FAIL == rtk_bt_le_sm_clear_bond_list())
    {
        debug_print("delete bonded device all fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

int rtw_ble_server_set_adv_interval(unsigned int interval)
{
	if (TRBLE_SUCCESS != rtw_ble_server_adv_into_idle())
		return TRBLE_FAIL;

    rtw_ble_server_adv_interval = interval;
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_set_adv_type(trble_adv_type_e type, trble_addr *addr)
{
    if(type == TRBLE_ADV_TYPE_DIRECT && addr == NULL)
    {
        debug_print("Invalid Input \n");
        return TRBLE_INVALID_ARGS;
    }
	memset(adv_direct_addr, 0, TRBLE_BD_ADDR_MAX_LEN);
    rtw_ble_server_adv_type = type;

	if (TRBLE_SUCCESS != rtw_ble_server_adv_into_idle())
		return TRBLE_FAIL;

    if(rtw_ble_server_adv_type == TRBLE_ADV_TYPE_DIRECT)
    {
        if (addr->type == TRBLE_ADDR_TYPE_PUBLIC) {
            rtw_ble_server_direct_addr_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        } else {
            rtw_ble_server_direct_addr_type = GAP_REMOTE_ADDR_LE_RANDOM;
        }

        memcpy(adv_direct_addr, addr->mac, sizeof(adv_direct_addr));
    }
    return TRBLE_SUCCESS;
}

#endif /* TRBLE_SERVER_C_ */
