/*
 * ble_tizenrt_da_server.c
 *
 *  Created on: 2020. 7. 27.
 *      Author: lexi_zhao
 */

#ifndef DA_BLE_SERVER_C_
#define DA_BLE_SERVER_C_

#include <ble_tizenrt_service.h>
#include "stddef.h"
#include <gap.h>
#include <gap_msg.h>
#include <gap_adv.h>
#include <gap_conn_le.h>
#include "ble_tizenrt_scatternet_app.h"
#include "ble_tizenrt_scatternet_link_mgr.h"
#include "ble_tizenrt_app.h"
#include <os_mem.h>

bool is_server_init = false;
uint16_t server_profile_count = 0;
da_ble_server_init_parm server_init_parm;
void (*ble_tizenrt_server_send_msg)(uint16_t sub_type, void *arg) = NULL;

da_ble_result_type rtw_ble_server_init(da_ble_server_init_parm* init_parm)
{
    if (init_parm == NULL || init_parm->profile == NULL || init_parm->profile_count == 0)
    {
        debug_print("\r\n[%s] Invalid parameters 0x%x 0x%x %d", __FUNCTION__, init_parm, init_parm->profile, init_parm->profile_count);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    server_profile_count = init_parm->profile_count;
    ble_tizenrt_server_send_msg = ble_tizenrt_send_msg;

    uint16_t gatt_char_num = 0;
    for (int i = 0; i < init_parm->profile_count; i++)
    {
        if(init_parm->profile[i].type == DA_BLE_SERVER_GATT_CHARACT)
            gatt_char_num++;
    }
    server_init_parm.profile_count = init_parm->profile_count + gatt_char_num;
    server_init_parm.profile = init_parm->profile;
    server_init_parm.connected_cb = init_parm->connected_cb;
    server_init_parm.is_secured_connect_allowed = init_parm->is_secured_connect_allowed;
    ble_tizenrt_app_init();
    
    is_server_init = true;
    debug_print("\r\n[%s] Tizenrt Sever Init Success Profile_count %d", __FUNCTION__, server_init_parm.profile_count);
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

uint16_t rtw_ble_server_get_profile_count(void)
{
    return server_profile_count;
}

da_ble_result_type rtw_ble_server_deinit(void)
{   
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    }

    ble_tizenrt_app_deinit();
    is_server_init = false;
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_server_get_mac_address(uint8_t mac[DA_BLE_BD_ADDR_MAX_LEN])
{
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    }

    if (gap_get_param(GAP_PARAM_BD_ADDR, mac) != GAP_CAUSE_SUCCESS) {
        debug_print("Failed to get local addr.\r\n");
        return DA_BLE_RESULT_TYPE_FAILURE;
    }
    
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

extern TIZENERT_SRV_CNT tizenrt_ble_srv_count;
extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];

/* set data pointer of attribute value */
da_ble_result_type rtw_ble_server_att_set_data_ptr(da_ble_attr_handle attr_handle, uint8_t *new_data_ptr)
{
    debug_print("\r\n[%s] new_data_ptr %p", __FUNCTION__, new_data_ptr);
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
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
                    debug_print("[%s] tizenrt_ble_srv_database[%d].chrc_info[%d].read_ptr %p", __FUNCTION__, i, j,
                                                        tizenrt_ble_srv_database[i].chrc_info[j].read_ptr);
                    return DA_BLE_RESULT_TYPE_SUCCESS;
                }
            }
        }
    }
    return DA_BLE_RESULT_TYPE_FAILURE; 
}

/* set current length of attribute value */
da_ble_result_type rtw_ble_server_att_set_length(da_ble_attr_handle attr_handle, uint16_t new_length)
{
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
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
                    return DA_BLE_RESULT_TYPE_SUCCESS;
                }
            }
        }
    }
    
     return DA_BLE_RESULT_TYPE_FAILURE;
}

/* get data pointer of attribute value */
uint8_t* rtw_ble_server_att_get_data_ptr(da_ble_attr_handle attr_handle)
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
                    debug_print("abs_handle 0x%x index 0x%x %p", tizenrt_ble_srv_database[i].chrc_info[j].abs_handle,
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
uint16_t rtw_ble_server_att_get_length(da_ble_attr_handle attr_handle)
{
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
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
                    debug_print("abs_handle 0x%x att_index 0x%x %d", tizenrt_ble_srv_database[i].chrc_info[j].abs_handle,
                                                tizenrt_ble_srv_database[i].chrc_info[j].index,
                                                tizenrt_ble_srv_database[i].chrc_info[j].data_len);
                    return tizenrt_ble_srv_database[i].chrc_info[j].data_len;
                }
            }
        }
    }

    return 0; 
}

da_ble_result_type rtw_ble_server_charact_notify(da_ble_attr_handle attr_handle, da_ble_conn_handle con_handle, uint8_t *data_ptr, uint16_t data_length)
{
    debug_print("\r\n[%s] send notify abs_handle 0x%x", __FUNCTION__, attr_handle);
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    }

    if (attr_handle == 0x0000) /* invalid attr_handle */
    {
        return DA_BLE_RESULT_TYPE_NOT_FOUND;
    } 

    T_TIZENRT_NOTIFY_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_NOTIFY_PARAM));
    param->data = os_mem_alloc(0, data_length);
    memcpy(param->data, data_ptr, data_length);
    param->len = data_length;
    param->conn_id = con_handle;
    param->att_handle = attr_handle;
    ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_NOTIFY, param);
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_server_reject(da_ble_attr_handle attr_handle, uint8_t app_errorcode)
{
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    }

    if (attr_handle == 0x0000) /* invalid attr_handle */
    {
        return DA_BLE_RESULT_TYPE_NOT_FOUND;
    }

    if(app_errorcode < 0x81)
    {    
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    if(ble_tizenrt_set_server_reject(attr_handle, app_errorcode))
    {
        return DA_BLE_RESULT_TYPE_SUCCESS; 
    } else {
        debug_print("\r\n[%s] fail", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    
}

static uint8_t bd_addr[DA_BLE_BD_ADDR_MAX_LEN];
uint8_t* rtw_ble_server_get_mac_address_by_conn_handle(da_ble_conn_handle con_handle)
{
    memset(bd_addr, 0, DA_BLE_BD_ADDR_MAX_LEN);
    uint8_t bd_type;
    if(le_get_conn_addr(con_handle, bd_addr, &bd_type))
    {
        debug_print("\r\n[%s] success", __FUNCTION__);
        return bd_addr;
    } else {
        debug_print("\r\n[%s] fail", __FUNCTION__);
        return NULL;
    }
}


da_ble_conn_handle rtw_ble_server_get_conn_handle_by_address(uint8_t* mac)
{
    //da_ble_conn_handle conn_id = 0xffff;
    uint8_t conn_id = 0xff;
    if(le_get_conn_id(mac, GAP_REMOTE_ADDR_LE_PUBLIC, &conn_id))
    {
        debug_print("\r\n[%s] get public add handle", __FUNCTION__);
        return conn_id;
    } else if(le_get_conn_id(mac, GAP_REMOTE_ADDR_LE_RANDOM, &conn_id))
    {
        debug_print("\r\n[%s] get random add handle", __FUNCTION__);
        return conn_id;
    } 
    return conn_id;
}

/* Set Advertisement Data API */
da_ble_result_type rtw_ble_server_set_adv_data(uint8_t* data, uint16_t length)
{
    if (is_server_init != true)
        return DA_BLE_RESULT_TYPE_INVALID_STATE;

    if(length > 31)
        return DA_BLE_RESULT_TYPE_FAILURE;

    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    switch (new_state.gap_adv_state)
    {
        debug_print("\r\n[%s] ADV STATE : %d", __FUNCTION__, new_state.gap_adv_state);
    case GAP_ADV_STATE_ADVERTISING:
        {
            debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
            ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
        }
        break;
    case GAP_ADV_STATE_START:
        {
            do {   
                debug_print("\r\n[%s] ADV STATE : START", __FUNCTION__);
                os_delay(100);
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_START);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
                ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
            }
        }
        break;
    case GAP_ADV_STATE_STOP:
        {
            do {   
                debug_print("\r\n[%s] ADV STATE : STOPPING", __FUNCTION__);
                os_delay(100);
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_STOP);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
                ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
            }            
        }
        break;
    default:
        break;
    }

    do {   
        debug_print("\r\n[%s] Waiting for adv idle , now %d", __FUNCTION__, new_state.gap_adv_state);
        os_delay(100);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);

    debug_print("\r\n[%s] ADV STATE : IDLE", __FUNCTION__);
    if(GAP_CAUSE_SUCCESS == le_adv_set_param(GAP_PARAM_ADV_DATA, length, (void *)data))
        debug_print("\r\n[%s] Set adv data success", __FUNCTION__);
    else {
        debug_print("\r\n[%s] Set adv data fail!!!", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    if(le_get_active_link_num())
    {
        debug_print("\r\n[%s] Active connection exist", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_SUCCESS;
    } else {
        ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_START_ADV, NULL);
        do
        {   
            debug_print("\r\n[%s] Waiting for adv start", __FUNCTION__);
            os_delay(100);
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
        }while(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING);

        return DA_BLE_RESULT_TYPE_SUCCESS;
    }
}



da_ble_result_type rtw_ble_server_set_adv_name(uint8_t* data, uint16_t length)
{
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    }

    if(length > 31)
        return DA_BLE_RESULT_TYPE_FAILURE;

    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    switch (new_state.gap_adv_state)
    {
    case GAP_ADV_STATE_ADVERTISING:
        {
            debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
            ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
        }
        break;
    case GAP_ADV_STATE_START:
        {
            do {   
                debug_print("\r\n[%s] ADV STATE : START", __FUNCTION__);
                os_delay(100);
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_START);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
                ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
            }
        }
        break;
    case GAP_ADV_STATE_STOP:
        {
            do {   
                debug_print("\r\n[%s] ADV STATE : STOPPING", __FUNCTION__);
                os_delay(100);
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_STOP);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
                ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
            }            
        }
        break;
    default:
        break;
    }

    do {   
        debug_print("\r\n[%s] Waiting for adv idle", __FUNCTION__);
        os_delay(100);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);

    debug_print("\r\n[%s] ADV STATE : IDLE", __FUNCTION__);
    if(GAP_CAUSE_SUCCESS == le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, length, (void *)data))
        debug_print("\r\n[%s] Set adv name success", __FUNCTION__);
    else {
        debug_print("\r\n[%s] Set adv name fail!!!", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    if(le_get_active_link_num())
    {
        debug_print("\r\n[%s] Active connection exist", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_SUCCESS;
    } else {
        ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_START_ADV, NULL);
        do
        {   
            debug_print("\r\n[%s] Waiting for adv start", __FUNCTION__);
            os_delay(100);
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
        }while(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING);

        return DA_BLE_RESULT_TYPE_SUCCESS;
    }
}

bool rtw_ble_server_conn_is_active(da_ble_conn_handle con_handle)
{
    if (is_server_init != true)
    {
        return false;
    }

    T_GAP_CONN_INFO conn_info;
    if (le_get_conn_info(con_handle, &conn_info))
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

    if(le_get_active_link_num())
        return true;
    else
        return false;
}

da_ble_result_type rtw_ble_server_disconnect(da_ble_conn_handle con_handle)
{
    if (is_server_init != true)
    {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    }

    if (con_handle != 0)
    {
        return DA_BLE_RESULT_TYPE_NOT_FOUND;
    }

    ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_DISCONNECT, NULL);

    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_server_start_adv(void)
{
    uint8_t link_num = le_get_active_link_num();
    if(link_num)
    {
        T_GAP_CONN_INFO conn_info;
        for(uint8_t i = 0; i < link_num; i++)
        {
            le_get_conn_info(i, &conn_info);
            if(conn_info.role == GAP_LINK_ROLE_SLAVE)
                return DA_BLE_RESULT_TYPE_FAILURE;
        }
    }

    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    if(new_state.gap_adv_state != GAP_ADV_STATE_IDLE)
    {
        ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
        do {   
            debug_print("\r\n[%s] Waiting for adv stop", __FUNCTION__);
            os_delay(100);
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
        } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);
    }

    ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_START_ADV, NULL);
    do
    {   
        debug_print("\r\n[%s] Waiting for adv start", __FUNCTION__);
        os_delay(100);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    } while(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING);

    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_server_stop_adv(void)
{
    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    if(new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        return DA_BLE_RESULT_TYPE_SUCCESS;
    else {
        ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
        do {
            debug_print("\r\n[%s] Waiting for adv stop", __FUNCTION__);
            os_delay(100);
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
        } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);
    }
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_server_start_adv_directed(uint8_t target_addr[DA_BLE_BD_ADDR_MAX_LEN])
{
    if(target_addr == NULL)
    {
        printf("\r\n[%s] Invalid input", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    uint8_t link_num = le_get_active_link_num();
    if(link_num)
    {
        T_GAP_CONN_INFO conn_info;
        for(uint8_t i = 0; i < link_num; i++)
        {
            le_get_conn_info(i, &conn_info);
            if(conn_info.role == GAP_LINK_ROLE_SLAVE)
                return DA_BLE_RESULT_TYPE_FAILURE;
        }
    }

    T_TIZENRT_DIRECT_ADV_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_DIRECT_ADV_PARAM));
    memcpy(param->bd_addr, target_addr, DA_BLE_BD_ADDR_MAX_LEN);

    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    if(new_state.gap_adv_state != GAP_ADV_STATE_IDLE)
    {
        ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
        do {   
            debug_print("\r\n[%s] Waiting for adv stop", __FUNCTION__);
            os_delay(100);
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
        } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);
    }
    
    ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_START_DIRECT_ADV, param);
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    if(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING)
     {
        do {
            debug_print("\r\n[%s] Waiting for adv start", __FUNCTION__);
            os_delay(100);
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
        } while(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING);
     }
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_server_get_bonded_device(da_ble_server_bonded_device* bonded_device_list, uint16_t* device_count)
{
    if(bonded_device_list == NULL || device_count == NULL)
    {
        printf("\r\n[%s] Invalid input", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    T_LE_KEY_ENTRY *p_entry;
    *device_count = 0;
    uint16_t bonded_num = le_get_bond_dev_num();
    debug_print("\r\n[%s] bonded num : %d", __FUNCTION__, bonded_num);
    for (int i = 0; i < bonded_num; i++)
    {
        p_entry = le_find_key_entry_by_idx(i);
        if (p_entry != NULL) 
        {
            memcpy(bonded_device_list[*device_count].bd_addr, p_entry->remote_bd.addr, DA_BLE_BD_ADDR_MAX_LEN);
            (*device_count)++;
            debug_print("\r\nbond_dev[%d]: bd 0x%02x%02x%02x%02x%02x%02x, addr_type %d",
                            p_entry->idx,
                            p_entry->remote_bd.addr[5],
                            p_entry->remote_bd.addr[4],
                            p_entry->remote_bd.addr[3],
                            p_entry->remote_bd.addr[2],
                            p_entry->remote_bd.addr[1],
                            p_entry->remote_bd.addr[0],
                            p_entry->remote_bd.remote_bd_type);
        }
    }
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_server_delete_bonded_device(uint8_t bond_addr[DA_BLE_BD_ADDR_MAX_LEN])
{
    if(bond_addr == NULL)
    {
        printf("\r\n[%s] Invalid input", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    da_ble_result_type ret = DA_BLE_RESULT_TYPE_FAILURE;
    T_TIZENRT_SERVER_DELETE_BOND_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_SERVER_DELETE_BOND_PARAM));
    memcpy(param->bd_addr, bond_addr, DA_BLE_BD_ADDR_MAX_LEN);
    param->flag = false;
    ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_DELETE_BOND, param);

    do {
        os_delay(100);
    } while(!param->flag);

    if(GAP_CAUSE_SUCCESS == param->result)
    {
        debug_print("\r\n[%s] success", __FUNCTION__);
        ret = DA_BLE_RESULT_TYPE_SUCCESS;
    } else if(GAP_CAUSE_NOT_FIND == param->result)
    {
        debug_print("\r\n[%s] not find", __FUNCTION__);
        ret = DA_BLE_RESULT_TYPE_NOT_FOUND;
    }
    os_mem_free(param);

    return ret;
}

da_ble_result_type rtw_ble_server_delete_bonded_device_all(void)
{
    ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_DELETE_BOND_ALL, NULL);
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

int rtw_ble_server_set_adv_interval(unsigned int interval)
{
    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    switch (new_state.gap_adv_state)
    {
    case GAP_ADV_STATE_ADVERTISING:
        {
            debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
            ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
        }
        break;
    case GAP_ADV_STATE_START:
        {
            do {   
                debug_print("\r\n[%s] ADV STATE : START", __FUNCTION__);
                os_delay(100);
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_START);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
                ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
            }
        }
        break;
    case GAP_ADV_STATE_STOP:
        {
            do {   
                debug_print("\r\n[%s] ADV STATE : STOPPING", __FUNCTION__);
                os_delay(100);
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_STOP);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("\r\n[%s] ADV STATE : ADVERTISING", __FUNCTION__);
                ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL);
            }            
        }
        break;
    default:
        break;
    }

    do {   
        debug_print("\r\n[%s] Waiting for adv idle", __FUNCTION__);
        os_delay(100);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);

    debug_print("\r\n[%s] ADV STATE : IDLE", __FUNCTION__);
    uint16_t adv_int = interval;
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int), &adv_int);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int), &adv_int);
    return DA_BLE_RESULT_TYPE_SUCCESS;
}
#endif /* DA_BLE_SERVER_C_ */
