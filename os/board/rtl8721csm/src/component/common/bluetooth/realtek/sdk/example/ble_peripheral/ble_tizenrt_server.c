/*
 * ble_tizenrt_server.c
 *
 *  Created on: 2020. 7. 27.
 *      Author: lexi_zhao
 */

#ifndef TRBLE_SERVER_C_
#define TRBLE_SERVER_C_

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
trble_server_init_config server_init_parm;
bool (*ble_tizenrt_server_send_msg)(uint16_t sub_type, void *arg) = NULL;
T_SEND_DATA_RESULT *send_indication_result = NULL;
extern T_GAP_DEV_STATE ble_tizenrt_scatternet_gap_dev_state;

trble_result_e rtw_ble_server_init(trble_server_init_config* init_parm)
{
    if (init_parm == NULL || init_parm->profile == NULL || init_parm->profile_count == 0)
    {
        debug_print("Invalid parameters 0x%x 0x%x %d \n", init_parm, init_parm->profile, init_parm->profile_count);
        return TRBLE_INVALID_ARGS;
    }

    server_profile_count = init_parm->profile_count;
    ble_tizenrt_server_send_msg = ble_tizenrt_send_msg;

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
    ble_tizenrt_app_init();
    
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

    ble_tizenrt_app_deinit();
    is_server_init = false;
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_server_get_mac_address(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if (gap_get_param(GAP_PARAM_BD_ADDR, mac) != GAP_CAUSE_SUCCESS) {
        debug_print("Failed to get local addr. \n");
        return TRBLE_FAIL;
    }
    
    return TRBLE_SUCCESS; 
}

extern TIZENERT_SRV_CNT tizenrt_ble_srv_count;
extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];

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
    debug_print("send notify abs_handle 0x%x \n", attr_handle);
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if (attr_handle == 0x0000) /* invalid attr_handle */
    {
        return TRBLE_NOT_FOUND;
    } 

    T_TIZENRT_NOTIFY_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_NOTIFY_PARAM));;
    if(!param)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    param->data = os_mem_alloc(0, data_length);
    if(!param->data)
    {
        os_mem_free(param);
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    memcpy(param->data, data_ptr, data_length);
    param->len = data_length;
    param->conn_id = con_handle;
    param->att_handle = attr_handle;
    if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_NOTIFY, param) == false)
    {
        os_mem_free(param->data);
        os_mem_free(param);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

#if defined(CONFIG_BLE_INDICATION)
void *ble_tizenrt_indicate_sem = NULL;
#endif
trble_result_e rtw_ble_server_charact_indicate(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *data_ptr, uint16_t data_length)
{
    debug_print("send indicate abs_handle 0x%x \n", attr_handle);
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    if (attr_handle == 0x0000) /* invalid attr_handle */
    {
        return TRBLE_NOT_FOUND;
    }

#if defined(CONFIG_BLE_INDICATION)
    if(ble_tizenrt_indicate_sem == NULL)
    {
        if(false == os_mutex_create(&ble_tizenrt_indicate_sem))
        {
            dbg("creat indication mutex fail! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("creat indication mutex 0x%x success \n", ble_tizenrt_indicate_sem);
        }
    }
#endif

    T_TIZENRT_NOTIFY_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_INDICATE_PARAM));;
    if(!param)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    param->data = os_mem_alloc(0, data_length);
    if(!param->data)
    {
        os_mem_free(param);
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    memcpy(param->data, data_ptr, data_length);
    param->len = data_length;
    param->conn_id = con_handle;
    param->att_handle = attr_handle;
    if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_INDICATE, param) == false)
    {
        os_mem_free(param->data);
        os_mem_free(param);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }

#if defined(CONFIG_BLE_INDICATION)
    send_indication_result->cause = 0xFF;
    int wticks = 0;
    while(wticks++ < 30)
    {
        debug_print("wticks %d \n", wticks);
        if(os_mutex_take(ble_tizenrt_indicate_sem, 1000))
        {
            debug_print("take indicate mutex success \n");
            debug_print("conn_id %d att_handle 0x%x! \n", con_handle, attr_handle);
            os_mutex_delete(ble_tizenrt_indicate_sem);
            ble_tizenrt_indicate_sem = NULL;
            if(send_indication_result->cause == GAP_SUCCESS)
            {
                debug_print("send indicate success \n");
                return TRBLE_SUCCESS;
            } else {
                debug_print("send indicate fail \n");
                return TRBLE_FAIL;
            }
        }
    }
    return TRBLE_FAIL;
#else
    return TRBLE_SUCCESS;
#endif
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

static uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN];
uint8_t* rtw_ble_server_get_mac_address_by_conn_handle(trble_conn_handle con_handle)
{
    memset(bd_addr, 0, TRBLE_BD_ADDR_MAX_LEN);
    uint8_t bd_type;
    if(le_get_conn_addr(con_handle, bd_addr, &bd_type))
    {
        debug_print("success \n");
        return bd_addr;
    } else {
        debug_print("fail \n");
        return NULL;
    }
}


trble_conn_handle rtw_ble_server_get_conn_handle_by_address(uint8_t* mac)
{
    //trble_conn_handle conn_id = 0xffff;
    uint8_t conn_id = 0xff;
    if(le_get_conn_id(mac, GAP_REMOTE_ADDR_LE_PUBLIC, &conn_id))
    {
        debug_print("get public add handle \n");
        return conn_id;
    } else if(le_get_conn_id(mac, GAP_REMOTE_ADDR_LE_RANDOM, &conn_id))
    {
        debug_print("get random add handle \n");
        return conn_id;
    } 
    return conn_id;
}

void rtw_ble_server_adv_into_idle(void)
{
    uint8_t max_count = 0;
    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    switch (new_state.gap_adv_state)
    {
    case GAP_ADV_STATE_ADVERTISING:
        {
            debug_print("ADV STATE : ADVERTISING \n");
            if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL) == false)
            {
                debug_print("msg send fail \n");
                return TRBLE_FAIL;
            }
        }
        break;
    case GAP_ADV_STATE_START:
        {
            do {   
                debug_print("ADV STATE : START \n");
                max_count++;
                os_delay(100);
                if(max_count == 20)
                {
                    dbg("loop count reaches %d \n", max_count);
                    return TRBLE_FAIL;
                }
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_START);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("ADV STATE : ADVERTISING \n");
                if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL) == false)
                {
                    debug_print("msg send fail \n");
                    return TRBLE_FAIL;
                }
            }
        }
        break;
    case GAP_ADV_STATE_STOP:
        {
            do {   
                debug_print("ADV STATE : STOPPING \n");
                max_count++;
                os_delay(100);
                if(max_count == 20)
                {
                    dbg("loop count reaches %d \n", max_count);
                    return TRBLE_FAIL;
                }
                le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
            } while(new_state.gap_adv_state == GAP_ADV_STATE_STOP);

            if(new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                debug_print("ADV STATE : ADVERTISING \n");
                if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL) == false)
                {
                    debug_print("msg send fail \n");
                    return TRBLE_FAIL;
                }
            }            
        }
        break;
    default:
        break;
    }

    max_count = 0;
    do {   
        debug_print("Waiting for adv idle \n");
        max_count++;
        os_delay(100);
        if(max_count == 20)
        {
            dbg("loop count reaches %d \n", max_count);
            return TRBLE_FAIL;
        }
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);

    debug_print("ADV STATE : IDLE \n");
}

/* Set Advertisement Data API */
trble_result_e rtw_ble_server_set_adv_data(uint8_t* data, uint16_t length)
{
    if (is_server_init != true)
        return TRBLE_INVALID_STATE;

    if(length > 31)
        return TRBLE_INVALID_ARGS;

    rtw_ble_server_adv_into_idle();

    if(GAP_CAUSE_SUCCESS == le_adv_set_param(GAP_PARAM_ADV_DATA, length, (void *)data))
    {
        le_adv_update_param();
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

    if(GAP_CAUSE_SUCCESS == le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, length, (void *)data))
    {    
        le_adv_update_param();
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

trble_result_e rtw_ble_server_disconnect(trble_conn_handle con_handle)
{
    if (is_server_init != true)
    {
        return TRBLE_INVALID_STATE;
    }

    trble_conn_handle *conn_id = os_mem_alloc(0, sizeof(trble_conn_handle));
    if(conn_id == NULL)
    {
        debug_print("\n[%s] Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    *conn_id = con_handle;
    if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_DISCONNECT, conn_id) == false)
    {
        os_mem_free(conn_id);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_server_start_adv(void)
{
    uint8_t max_count = 0;
    uint8_t link_num = le_get_active_link_num();
    if(link_num)
    {
        T_GAP_CONN_INFO conn_info;
        for(uint8_t i = 0; i < link_num; i++)
        {
            le_get_conn_info(i, &conn_info);
            if(conn_info.role == GAP_LINK_ROLE_SLAVE)
            {
                trble_adv_type_e adv_evt_type;
                T_GAP_CAUSE ret;
                ret = le_adv_get_param(GAP_PARAM_ADV_EVENT_TYPE, (void *)&adv_evt_type);
                if (adv_evt_type != TRBLE_ADV_TYPE_NONCONN_IND || ret != GAP_CAUSE_SUCCESS)
                {
                    return TRBLE_FAIL;
                }
            }
        }
    }

    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    if(new_state.gap_adv_state != GAP_ADV_STATE_IDLE)
    {
        if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL) == false)
        {
            debug_print("msg send fail \n");
            return TRBLE_FAIL;
        }
        do {   
            debug_print("Waiting for adv stop \n");
            max_count++;
            os_delay(100);
            if(max_count == 20)
            {
                dbg("loop count reaches %d \n", max_count);
                return TRBLE_FAIL;
            }
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
        } while(new_state.gap_adv_state != GAP_ADV_STATE_IDLE);
    }

    max_count = 0;
    if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_START_ADV, NULL) == false)
    {
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    do
    {   
        debug_print("Waiting for adv start \n");
        max_count++;
        os_delay(100);
        if (ble_tizenrt_scatternet_gap_dev_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
        {
            ble_tizenrt_scatternet_gap_dev_state.gap_adv_sub_state = GAP_ADV_TO_IDLE_CAUSE_STOP;
            break;
        }
        if(max_count == 20)
        {
            dbg("loop count reaches %d \n", max_count);
            return TRBLE_FAIL;
        }
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    } while(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING);

    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_stop_adv(void)
{
    uint8_t max_count = 0;
    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);

    if(new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        return TRBLE_SUCCESS;
    else {
        if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_STOP_ADV, NULL) == false)
        {
            debug_print("msg send fail \n");
            return TRBLE_FAIL;
        }
        do {
            debug_print("Waiting for adv stop \n");
            os_delay(100);
            if(max_count == 20)
            {
                dbg("loop count reaches %d \n", max_count);
                return TRBLE_FAIL;
            }
            le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
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

    T_LE_KEY_ENTRY *p_entry;
    *device_count = 0;
    uint16_t bonded_num = le_get_bond_dev_num();
    debug_print("bonded num : %d \n", bonded_num);
    for (int i = 0; i < bonded_num; i++)
    {
        p_entry = le_find_key_entry_by_idx(i);
        if (p_entry != NULL) 
        {
            memcpy(bonded_device_list[*device_count].bd_addr.mac, p_entry->remote_bd.addr, TRBLE_BD_ADDR_MAX_LEN);
            bonded_device_list[*device_count].bd_addr.type = p_entry->remote_bd.remote_bd_type;
            (*device_count)++;
            debug_print("bond_dev[%d]: bd 0x%02x%02x%02x%02x%02x%02x, addr_type %d \n",
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
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_delete_bonded_device(uint8_t bond_addr[TRBLE_BD_ADDR_MAX_LEN])
{
    if(bond_addr == NULL)
    {
        debug_print("Invalid input \n");
        return TRBLE_INVALID_ARGS;
    }

    trble_result_e ret = TRBLE_FAIL;
    T_TIZENRT_SERVER_DELETE_BOND_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_SERVER_DELETE_BOND_PARAM));
    if(param == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    memcpy(param->bd_addr, bond_addr, TRBLE_BD_ADDR_MAX_LEN);
    param->flag = false;
    if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_DELETE_BOND, param) == false)
    {
        os_mem_free(param);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }

    do {
        os_delay(100);
    } while(!param->flag);

    if(GAP_CAUSE_SUCCESS == param->result)
    {
        debug_print("success \n");
        ret = TRBLE_SUCCESS;
    } else if(GAP_CAUSE_NOT_FIND == param->result)
    {
        debug_print("not find \n");
        ret = TRBLE_NOT_FOUND;
    }
    os_mem_free(param);

    return ret;
}

trble_result_e rtw_ble_server_delete_bonded_device_all(void)
{
    if(ble_tizenrt_server_send_msg(BLE_TIZENRT_MSG_DELETE_BOND_ALL, NULL) == false)
    {
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

int rtw_ble_server_set_adv_interval(unsigned int interval)
{
    rtw_ble_server_adv_into_idle();

    uint16_t adv_int = interval;
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int), &adv_int);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int), &adv_int);
    return TRBLE_SUCCESS;
}

int rtw_ble_server_set_adv_txpower(unsigned int txpower)
{

    uint16_t tx_gain = txpower;
    le_adv_set_tx_power(0, tx_gain);
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_server_set_adv_type(trble_adv_type_e type, trble_addr *addr)
{
    if(type == TRBLE_ADV_TYPE_DIRECT && addr == NULL)
    {
        debug_print("Invalid Input \n");
        return TRBLE_INVALID_ARGS;
    }

    uint8_t  adv_evt_type = type;

    rtw_ble_server_adv_into_idle();

    if(adv_evt_type == TRBLE_ADV_TYPE_DIRECT) {
        adv_evt_type = 4;/* maps the TRBLE_ADV_TYPE_DIRECT (value is 1) to 4 for Low Duty Cycle Directed advertising */
        uint8_t adv_direct_type;
        if (addr->type == TRBLE_ADDR_TYPE_PUBLIC) {
            adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        } else {
            adv_direct_type = GAP_REMOTE_ADDR_LE_RANDOM;
        }
        uint8_t adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
        memcpy(adv_direct_addr, addr->mac, sizeof(adv_direct_addr));
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
    }
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    return TRBLE_SUCCESS;
}
#endif /* TRBLE_SERVER_C_ */
