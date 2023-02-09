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
#include <mem_types.h>
#include <bt_utils.h>
#include <rtk_service_config.h>
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
extern T_ATTRIB_APPL *tizenrt_ble_service_tbl;
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
	osif_mem_free(tizenrt_ble_service_tbl);
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

trble_result_e rtw_ble_server_adv_into_idle(void)
{
    rtk_bt_le_gap_dev_state_t new_state;
    if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
    {
		debug_print("get dev state fail \n");
    }

    switch (new_state.gap_adv_state)
    {
    case GAP_ADV_STATE_ADVERTISING:
        {
            debug_print("ADV STATE : ADVERTISING \n");
            if(RTK_BT_OK != rtk_bt_le_gap_stop_adv())
            {
                debug_print("stop adv fail \n");
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
            } while(new_state.gap_adv_state == GAP_ADV_STATE_START);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("ADV STATE : ADVERTISING \n");
                if(RTK_BT_OK != rtk_bt_le_gap_stop_adv())
                {
                    debug_print("stop adv fail \n");
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
            } while(new_state.gap_adv_state == GAP_ADV_STATE_STOP);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("ADV STATE : ADVERTISING \n");
                if(RTK_BT_OK != rtk_bt_le_gap_stop_adv())
                {
                    debug_print("stop adv fail \n");
                    return TRBLE_FAIL;
                }
            }            
        }
        break;
    default:
        break;
    }

    do {   
        debug_print("Waiting for adv idle \n");
        osif_delay(100);
		if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
		{
			debug_print("get dev state fail \n");
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

    rtw_ble_server_adv_into_idle();
	
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

    rtw_ble_server_adv_into_idle();
	
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

trble_result_e rtw_ble_server_disconnect(trble_conn_handle con_handle)
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if (con_handle != 0)
    {
        return TRBLE_NOT_FOUND;
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
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
        debug_print("get active conn fail \n");
		return TRBLE_FAIL;
    }

    if(active_conn.conn_num)
    {
        rtk_bt_le_conn_info_t conn_info;
        for(uint8_t i = 0; i < active_conn.conn_num; i++)
        {
			if (RTK_BT_OK != rtk_bt_le_gap_get_conn_info(i, &conn_info))
			{
				debug_print("get conn info fail \n");
				return TRBLE_FAIL;
			}

            if(conn_info.role == RTK_BT_LE_ROLE_SLAVE)
                return TRBLE_FAIL;
        }
    }

    rtk_bt_le_gap_dev_state_t new_state;
    if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
    {
		debug_print("get dev state fail \n");
    }

    if(new_state.gap_adv_state != GAP_ADV_STATE_IDLE)
    {
		if(RTK_BT_OK != rtk_bt_le_gap_stop_adv())
		{
			debug_print("stop adv fail \n");
			return TRBLE_FAIL;
		}
        do {   
            debug_print("Waiting for adv stop \n");
            osif_delay(100);
			if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
			{
				debug_print("get dev state fail \n");
			}
        } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);
    }
	
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
	
    if(RTK_BT_OK != rtk_bt_le_gap_start_adv(&adv_param))
    {
        debug_print("start adv fail \n");
        return TRBLE_FAIL;
    }
	
    do
    {   
        debug_print("Waiting for adv start \n");
        osif_delay(100);
		if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
		{
			debug_print("get dev state fail \n");
		}
    } while(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING);

    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_stop_adv(void)
{
    rtk_bt_le_gap_dev_state_t new_state;
	if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
	{
		debug_print("get dev state fail \n");
	}

    if(new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        return TRBLE_SUCCESS;
    else {
		if(RTK_BT_OK != rtk_bt_le_gap_stop_adv())
		{
			debug_print("stop adv fail \n");
			return TRBLE_FAIL;
		}
        do {
            debug_print("Waiting for adv stop \n");
            osif_delay(100);
			if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
			{
				debug_print("get dev state fail \n");
			}
        } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);
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

	if (bond_info)
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
    rtw_ble_server_adv_into_idle();

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

    rtw_ble_server_adv_into_idle();

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
