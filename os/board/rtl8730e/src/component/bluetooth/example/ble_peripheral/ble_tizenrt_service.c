#include <stddef.h>
#include <gap.h>
#include <os_mem.h>
#include <bt_utils.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_common.h>
#include <rtk_service_config.h>
#include <ble_tizenrt_service.h>
#include <rtk_bt_def.h>

#define BLE_TIZENRT_READ_MAX_LEN     20

static uint8_t ble_tizenrt_read_val[BLE_TIZENRT_READ_MAX_LEN] = {'T', 'i', 'z', 'e', 'n', 'r', 't', '\0'};
static uint8_t tizenrt_ble_write_value[TIZENRT_MAX_VAL_LEN];
/* service table contains several services */
T_ATTRIB_APPL *tizenrt_ble_service_tbl = NULL;
static uint16_t abs_att_count = 0;
TIZENERT_SRV_CNT tizenrt_ble_srv_count = 0;
TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7] = {0};

extern trble_server_init_config server_init_parm;

bool ble_tizenrt_set_server_reject(trble_attr_handle abs_handle, uint8_t app_errorcode)
{
    for (uint8_t i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(tizenrt_ble_srv_database[i].start_handle < abs_handle && 
        tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= abs_handle)
        {
            for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
            {
                if(abs_handle == tizenrt_ble_srv_database[i].chrc_info[j].abs_handle)
                {
                    tizenrt_ble_srv_database[i].chrc_info[j].app_reject = ATT_ERR | app_errorcode;
                    debug_print("abs_handle = 0x%x app_reject = 0x%x \n", abs_handle, 
                                                tizenrt_ble_srv_database[i].chrc_info[j].app_reject);
                     return true;
                }
            }
        }
    }
    return false;
}

extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];
T_APP_RESULT ble_tizenrt_srv_callback(uint8_t event, void *p_data)
{
    uint32_t i = 0;
    switch (event)
    {
    case RTK_BT_GATTS_EVT_REGISTER_SERVICE:
    	{
			rtk_bt_gatts_reg_ind_t* data = (rtk_bt_gatts_reg_ind_t *)p_data;
			if(data->reg_status == RTK_BT_OK)
				debug_print("[APP] ble tizenrt service register succeed \r\n");
			else
				debug_print("[APP] ble tizenrt service register failed \r\n");
			break;
    	}

	case RTK_BT_GATTS_EVT_CCCD_IND:
		{
			rtk_bt_gatts_cccd_ind_t *p_cccd_ind = (rtk_bt_gatts_cccd_ind_t *)p_data;
			if (p_cccd_ind->value & RTK_BT_GATT_CCC_NOTIFY)
            {
                dbg("[APP] BLE tizenrt notify cccd, notify bit enable\r\n");
            }
            else
            {
                dbg("[APP] BLE tizenrt notify cccd, notify bit disable\r\n");
            }

			if (p_cccd_ind->value & RTK_BT_GATT_CCC_INDICATE)
            {
                dbg("[APP] BLE tizenrt indicate cccd, indicate bit enable\r\n");
            }
            else
            {
                dbg("[APP] BLE tizenrt indicate cccd, indicate bit disable\r\n");
            }
			break;
		}
	
    case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND:
	    {
	        rtk_bt_gatts_ntf_and_ind_ind_t *p_ntf_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)p_data;
	        if (RTK_BT_OK == p_ntf_ind->err_code)
	            dbg("[APP] BLE tizenrt notify succeed!\r\n");
	        else
	            dbg("[APP] BLE tizenrt notify failed, local error: %d\r\n", p_ntf_ind->err_code);
	        break;
	    }

    case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND:
	    {
	        rtk_bt_gatts_ntf_and_ind_ind_t *p_ind_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)p_data;
	        if (RTK_BT_OK == p_ind_ind->err_code)
	            dbg("[APP] BLE tizenrt notify succeed!\r\n");
	        else
	            dbg("[APP] BLE tizenrt notify failed, local error: %d\r\n", p_ind_ind->err_code);
	        break;
	    }

	case RTK_BT_GATTS_EVT_READ_IND:
		{
			rtk_bt_gatts_read_ind_t *p_read_ind = (rtk_bt_gatts_read_ind_t *)p_data;
			TIZENERT_CHA_INFO *p_cha_info = NULL;
			uint16_t srv_index = p_read_ind->app_id - TIZENRT_SRV_ID;
			uint16_t actual_len = BLE_TIZENRT_READ_MAX_LEN - p_read_ind->offset;
			for(i = 0; i < TIZENRT_MAX_ATTR_NUM; i++){
				if(p_read_ind->index == tizenrt_ble_srv_database[srv_index].chrc_info[i].index){
					p_cha_info = &tizenrt_ble_srv_database[srv_index].chrc_info[i];
				}
			}
            debug_print("app_id 0x%x index 0x%x abs_handle 0x%x \n", 
                            tizenrt_ble_srv_database[srv_index].app_id,
                            p_cha_info->index,
                            p_cha_info->abs_handle);

			rtk_bt_gatts_read_resp_param_t read_resp = {
				.app_id   = p_read_ind->app_id,
				.conn_handle  = p_read_ind->conn_handle,
				.index	  = p_read_ind->index,
				.data	  = &ble_tizenrt_read_val[p_read_ind->offset],
				.len	  = actual_len,
				.err_code = 0,
				.seq	  = 0,
			};
		
			for (i = 0; i < 3; i++)
			{
				if (RTK_BT_OK == rtk_bt_gatts_read_resp(&read_resp))
				{
					debug_print("[APP] BLE tizenrt respond for client read success, offset: %d\r\n", p_read_ind->offset);
					break;
				}
				osif_delay(50);
			}
			if (i >= 3)
			{
				debug_print("[APP] BLE tizenrt respond for client read failed!\r\n");
			}
			
            /* call user defined callback */
            if (p_cha_info->cb)
            {
            	trble_server_cb_t p_func = p_cha_info->cb;
                p_func(TRBLE_ATTR_CB_READING, p_read_ind->conn_handle,
                                                        p_cha_info->abs_handle, p_cha_info->arg);
            } else {
                debug_print("NULL read callback abs_handle 0x%x \n", p_cha_info->abs_handle);
            }
            break;
        }

    case RTK_BT_GATTS_EVT_WRITE_IND:
        {
        	rtk_bt_gatts_write_ind_t *p_write_ind = (rtk_bt_gatts_write_ind_t *)p_data;
			rtk_bt_gatts_write_resp_param_t write_resp = {0};
			uint16_t srv_index = p_write_ind->app_id - TIZENRT_SRV_ID;
			TIZENERT_CHA_INFO *p_cha_info = NULL;
			for(i = 0; i < TIZENRT_MAX_ATTR_NUM; i++){
				if(p_write_ind->index == tizenrt_ble_srv_database[srv_index].chrc_info[i].index){
					p_cha_info = &tizenrt_ble_srv_database[srv_index].chrc_info[i];
				}
			}
            debug_print("app_id 0x%x Attribute 0x%x write_type %d len %d data 0x \n",
                            tizenrt_ble_srv_database[srv_index].app_id, 
                            p_cha_info->abs_handle,
                            p_write_ind->type,
                            p_cha_info->data_len);
            for (i = 0; i < p_cha_info->data_len; i++)
            {
                debug_print("%x", *(p_cha_info->data + i));
            }
            switch (p_write_ind->type)
            {
            case RTK_BT_GATTS_WRITE_REQ:
                {
                    /* call user defined callback */
                    if (p_cha_info->cb)
                    {
                        trble_server_cb_t p_func = p_cha_info->cb;
                        p_func(TRBLE_ATTR_CB_WRITING, p_write_ind->conn_handle,
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
                        p_func(TRBLE_ATTR_CB_WRITING_NO_RSP, p_write_ind->conn_handle,
                                                                p_cha_info->abs_handle, p_cha_info->arg);
                    } else {
                        debug_print("NULL write callback abs_handle 0x%x \n", p_cha_info->abs_handle);
                    }
                    break;
                }
            default:
                break;
            }

			write_resp.app_id = p_write_ind->app_id;
			write_resp.conn_handle = p_write_ind->conn_handle;
			write_resp.index = p_write_ind->index;
			write_resp.type = p_write_ind->type;
			write_resp.err_code = 0;
			write_resp.seq = 0;
			
			for (i = 0; i < 3; i++)
			{
				if (RTK_BT_OK == rtk_bt_gatts_write_resp(&write_resp))
				{
					debug_print("[APP] respond for client write success\r\n");
					break;
				}
				osif_delay(10);
			}
			if (i >= 3)
				dbg("[APP] respond for client write failed!\r\n");
        	break;
        }

    default:
        break;
    }
    return 0;
}

static uint32_t switch_attr_perm(uint8_t perm)
{
	uint32_t prop = RTK_BT_GATT_PERM_NONE;
	if((perm | TRBLE_ATTR_PERM_R_PERMIT) == 0x00)
		prop = prop | RTK_BT_GATT_PERM_READ;
	if(perm & TRBLE_ATTR_PERM_R_AUTHEN)
		prop = prop | RTK_BT_GATT_PERM_READ_AUTHEN;
	if(perm & TRBLE_ATTR_PERM_R_AUTHOR)
		prop = prop | RTK_BT_GATT_PERM_READ_AUTHEN;
	if(perm & TRBLE_ATTR_PERM_R_ENCRYPT)
		prop = prop | RTK_BT_GATT_PERM_READ_ENCRYPT;
	
	if((perm | TRBLE_ATTR_PERM_W_PERMIT) == 0x00)
		prop = prop | RTK_BT_GATT_PERM_WRITE;
	if(perm & TRBLE_ATTR_PERM_W_AUTHEN)
		prop = prop | RTK_BT_GATT_PERM_WRITE_AUTHEN;
	if(perm & TRBLE_ATTR_PERM_W_AUTHOR)
		prop = prop | RTK_BT_GATT_PERM_WRITE_AUTHEN;
	if(perm & TRBLE_ATTR_PERM_W_ENCRYPT)
		prop = prop | RTK_BT_GATT_PERM_WRITE_ENCRYPT;	

	return prop;
}

static int setup_ble_srv_dec_add_attr(rtk_bt_gatt_attr_t *attr, uint8_t profile_index)
{
	if(server_init_parm.profile[profile_index].uuid_length == BT_UUID_SIZE_16){
		uint16_t uuid_16 = server_init_parm.profile[profile_index].uuid[1] << 8 | server_init_parm.profile[profile_index].uuid[0];
		struct bt_uuid_16 *bt_uuid_16_s = (struct bt_uuid_16 *)osif_mem_alloc(0, sizeof(struct bt_uuid_16));
		bt_uuid_16_s->uuid.type = BT_UUID_TYPE_16;
		bt_uuid_16_s->val = uuid_16;
		attr->user_data = (void *)bt_uuid_16_s;
	}else if(server_init_parm.profile[profile_index].uuid_length == BT_UUID_SIZE_128){
		struct bt_uuid_128 *bt_uuid_128_s = (struct bt_uuid_128 *)osif_mem_alloc(0, sizeof(struct bt_uuid_128));
		bt_uuid_128_s->uuid.type = BT_UUID_TYPE_128;
		memcpy(bt_uuid_128_s->val, server_init_parm.profile[profile_index].uuid, BT_UUID_SIZE_128);
		attr->user_data = (void *)bt_uuid_128_s;
	}
	struct bt_uuid_16 * attuuid = (struct bt_uuid_16 *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct bt_uuid_16));
	if (!attuuid)
	{
		debug_print("[%s]malloc attuuid fail\r\n", __func__);
		return -1;
	}
	memset(attuuid, 0, sizeof(struct bt_uuid_16));
	attuuid->uuid.type = BT_UUID_TYPE_16;
	attuuid->val	   = BT_UUID_GATT_PRIMARY_VAL;
	
	attr->perm = switch_attr_perm(server_init_parm.profile[profile_index].permission);
	attr->flag = RTK_BT_GATT_INTERNAL;
	attr->uuid = (struct bt_uuid*)attuuid;
	attr->len = 0;

	return 0;
}

static int setup_ble_char_dec_add_attr(rtk_bt_gatt_attr_t *attr, uint8_t profile_index)
{
	struct rtk_bt_gatt_chrc *charac = (struct rtk_bt_gatt_chrc *)osif_mem_alloc(0, sizeof(struct rtk_bt_gatt_chrc));
	if(server_init_parm.profile[profile_index].uuid_length == 2){
		uint16_t uuid_16 = server_init_parm.profile[profile_index].uuid[1] << 8 | server_init_parm.profile[profile_index].uuid[0];
		struct bt_uuid_16 *bt_uuid_16_s = (struct bt_uuid_16 *)osif_mem_alloc(0, sizeof(struct bt_uuid_16));
		bt_uuid_16_s->uuid.type = BT_UUID_TYPE_16;
		bt_uuid_16_s->val = uuid_16;
		charac->uuid = (struct bt_uuid*)bt_uuid_16_s;
	}else if(server_init_parm.profile[profile_index].uuid_length == 16){
		struct bt_uuid_128 *bt_uuid_128_s = (struct bt_uuid_128 *)osif_mem_alloc(0, sizeof(struct bt_uuid_128));
		bt_uuid_128_s->uuid.type = BT_UUID_TYPE_128;
		memcpy(bt_uuid_128_s->val, server_init_parm.profile[profile_index].uuid, 16);
		charac->uuid  = (struct bt_uuid*)bt_uuid_128_s;
	}
	charac->properties = server_init_parm.profile[profile_index].property;
	charac->value_handle = 0;
	struct bt_uuid_16 * attuuid = (struct bt_uuid_16 *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct bt_uuid_16));
	attuuid->uuid.type = BT_UUID_TYPE_16;
	attuuid->val	   = BT_UUID_GATT_CHRC_VAL; 				
	attr->uuid = (struct bt_uuid*)attuuid;
	attr->user_data = (void *)charac;
	attr->perm = RTK_BT_GATT_PERM_READ;
	attr->flag = RTK_BT_GATT_INTERNAL;
	attr->len = 0;

	return 0;
}

static int setup_ble_char_val_desc_add_attr(rtk_bt_gatt_attr_t *attr, uint8_t profile_index)
{
	if(server_init_parm.profile[profile_index].uuid_length == 2){
		uint16_t uuid_16 = server_init_parm.profile[profile_index].uuid[1] << 8 | server_init_parm.profile[profile_index].uuid[0];
		struct bt_uuid_16 *bt_uuid_16_s = (struct bt_uuid_16 *)osif_mem_alloc(0, sizeof(struct bt_uuid_16));
		bt_uuid_16_s->uuid.type = BT_UUID_TYPE_16;
		bt_uuid_16_s->val = uuid_16;
		attr->uuid = (struct bt_uuid*)bt_uuid_16_s;
	}else if(server_init_parm.profile[profile_index].uuid_length == 16){
		struct bt_uuid_128 *bt_uuid_128_s = (struct bt_uuid_128 *)osif_mem_alloc(0, sizeof(struct bt_uuid_128));
		bt_uuid_128_s->uuid.type = BT_UUID_TYPE_128;
		memcpy(bt_uuid_128_s->val, server_init_parm.profile[profile_index].uuid, 16);
		attr->uuid  = (struct bt_uuid*)bt_uuid_128_s;
	}
	attr->user_data = NULL;
	attr->perm = switch_attr_perm(server_init_parm.profile[profile_index].permission);
	attr->flag = RTK_BT_GATT_APP;
	attr->len = 0;

	return 0;
}

static int setup_ble_char_ccc_add_attr(rtk_bt_gatt_attr_t *attr, uint8_t profile_index)
{
	struct bt_uuid_16 * attuuid = (struct bt_uuid_16 *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct bt_uuid_16));
	if (!attuuid)
	{
		debug_print("[%s]malloc attuuid fail\r\n", __func__);
		return -1;
	}
	memset(attuuid, 0, sizeof(struct bt_uuid_16));
	attuuid->uuid.type = BT_UUID_TYPE_16;
	attuuid->val	   = BT_UUID_GATT_CCC_VAL;
	attr->perm = switch_attr_perm(server_init_parm.profile[profile_index].permission);
	attr->flag = RTK_BT_GATT_INTERNAL;
	attr->len = 0;
	attr->uuid = (struct bt_uuid*)attuuid;
	attr->user_data = NULL;

	return 0;
}

void ble_tizenrt_free_srv_info(struct rtk_bt_gatt_service *srv_info)
{
	for(int i = 0; i < srv_info->attr_count; i++)
	{
		if(srv_info->attrs)
		{
			if (srv_info->attrs[i].user_data)
			{
				uint16_t uuid16 = BT_UUID_16(srv_info->attrs[i].uuid)->val;
				if (BT_UUID_GATT_CHRC_VAL == uuid16)
				{
					struct rtk_bt_gatt_chrc *char_attr_val = (struct rtk_bt_gatt_chrc*)srv_info->attrs[i].user_data;
					osif_mem_free(char_attr_val->uuid);
				}
				osif_mem_free(srv_info->attrs[i].user_data);
			}
			if (srv_info->attrs[i].uuid)
			{
				osif_mem_free(srv_info->attrs[i].uuid);
			}
		}
	}

}

extern uint16_t server_profile_count;
trble_result_e tizenrt_add_service(uint8_t index, uint16_t app_id)
{	
	struct rtk_bt_gatt_service ble_tizenrt_srv = {0};
    ble_tizenrt_srv.type = GATT_SERVICE_OVER_BLE;
    ble_tizenrt_srv.server_info = 0;
    ble_tizenrt_srv.user_data = NULL;
    ble_tizenrt_srv.register_status = 0;
	ble_tizenrt_srv.start_handle = tizenrt_ble_srv_database[index].start_handle;
	ble_tizenrt_srv.app_id = app_id;
	ble_tizenrt_srv.assgin_handle_flag = true;
	ble_tizenrt_srv.attr_count = tizenrt_ble_srv_database[index].att_count;

	rtk_bt_gatt_attr_t ble_tizenrt_attrs[ble_tizenrt_srv.attr_count];
	int j = 0;
	int srv_count = 0;
	for(int i = 0; i < server_profile_count; i++)
	{
		if(TRBLE_GATT_SERVICE == server_init_parm.profile[i].type){
			srv_count++;
			if(srv_count > 1)
				break;
			setup_ble_srv_dec_add_attr(&ble_tizenrt_attrs[j++], i);
		}else if(TRBLE_GATT_CHARACT == server_init_parm.profile[i].type){
			setup_ble_char_dec_add_attr(&ble_tizenrt_attrs[j++], i);
			setup_ble_char_val_desc_add_attr(&ble_tizenrt_attrs[j++], i);
		}else if(TRBLE_GATT_DESC == server_init_parm.profile[i].type){
			setup_ble_char_ccc_add_attr(&ble_tizenrt_attrs[j++], i);
		}
	}
	ble_tizenrt_srv.attrs = ble_tizenrt_attrs;
    debug_print("tizenrt_ble_srv_database[%d].abs_handle = %d tizenrt_attr_tbl_size = %d start_handle = 0x%x \n",
                                                index,
                                                tizenrt_ble_srv_database[index].abs_handle,
                                                ble_tizenrt_srv.attr_count * sizeof(T_ATTRIB_APPL),
                                                ble_tizenrt_srv.start_handle);

    if (RTK_BT_OK != rtk_bt_gatts_register_service(&ble_tizenrt_srv))
    {
        dbg("add service fail \n");
		ble_tizenrt_free_srv_info(&ble_tizenrt_srv);
        return TRBLE_FAIL;
    }
    tizenrt_ble_srv_database[index].app_id = app_id;
    debug_print("add service success \n");
	ble_tizenrt_free_srv_info(&ble_tizenrt_srv);
    return TRBLE_SUCCESS;
}


bool setup_ble_srv_info(uint8_t service_index, trble_gatt_t *profile)
{
    tizenrt_ble_srv_database[service_index].start_handle = profile->attr_handle;
    tizenrt_ble_srv_database[service_index].att_count++;
    tizenrt_ble_srv_database[service_index].abs_handle = abs_att_count++;

    debug_print("tizenrt_ble_srv_database[%d].abs_handle = 0x%x \n",
                                    service_index,
                                    tizenrt_ble_srv_database[service_index].abs_handle);
    return true;
}

bool setup_ble_char_info(uint8_t service_index, uint8_t character_index, trble_gatt_t *profile)
{
    if (profile->type == TRBLE_GATT_CHARACT)
    {
        abs_att_count += 2;
        tizenrt_ble_srv_database[service_index].att_count += 2;
        tizenrt_ble_srv_database[service_index].chrc_info[character_index].abs_handle = profile->attr_handle + 1;
    } else if(profile->type == TRBLE_GATT_DESC) {
        abs_att_count += 1;
        tizenrt_ble_srv_database[service_index].att_count += 1;
        tizenrt_ble_srv_database[service_index].chrc_info[character_index].abs_handle = profile->attr_handle;
    }
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].index = tizenrt_ble_srv_database[service_index].att_count - 1;
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].app_reject = 0;
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].cb = profile->cb;
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].arg = profile->arg;
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].read_ptr = NULL;
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].read_len = 0;
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].data = tizenrt_ble_write_value;
    tizenrt_ble_srv_database[service_index].chrc_info[character_index].data_len = 0;
    debug_print("tizenrt_ble_srv_database[%d].chrc_info[%d].abs_handle 0x%x .cb %p .index %d \n",
                            service_index,
                            character_index,
                            tizenrt_ble_srv_database[service_index].chrc_info[character_index].abs_handle,
                            tizenrt_ble_srv_database[service_index].chrc_info[character_index].cb,
                            tizenrt_ble_srv_database[service_index].att_count);
    return true;
}


static int setup_ble_char_ccc_attr(T_ATTRIB_APPL *attr, uint16_t cccd, uint16_t uuid_length)
{
    attr->flags         = ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL;
    attr->type_value[0] = LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG);
    attr->type_value[1] = HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG);
    attr->type_value[3] = LO_WORD(cccd);
    attr->type_value[4] = HI_WORD(cccd);
    attr->value_len     = 2;
    attr->p_value_context = NULL;
    attr->permissions   = GATT_PERM_READ | GATT_PERM_WRITE;
    debug_print("cccd = 0x%x \n", cccd);
    return 0;
}

uint32_t switch_perm(uint8_t perm)
{
    uint32_t read_perm = 0x0, write_perm = 0x0;

    if((perm | TRBLE_ATTR_PERM_R_PERMIT) == 0x00)
        read_perm = GATT_PERM_READ;
    if(perm & TRBLE_ATTR_PERM_R_AUTHOR)
        read_perm = GATT_PERM_READ_AUTHEN_REQ;
    if(perm & TRBLE_ATTR_PERM_R_ENCRYPT)
        read_perm = GATT_PERM_READ_AUTHEN_REQ;
    if(perm & TRBLE_ATTR_PERM_R_ENCRYPT)
        read_perm = GATT_PERM_READ_ENCRYPTED_REQ;

    if((perm | TRBLE_ATTR_PERM_W_PERMIT) == 0x00)
        write_perm = GATT_PERM_WRITE;
    if(perm & TRBLE_ATTR_PERM_W_AUTHEN)
        write_perm = GATT_PERM_WRITE_AUTHEN_REQ;
    if(perm & TRBLE_ATTR_PERM_W_AUTHOR)
        write_perm = GATT_PERM_WRITE_AUTHOR_REQ;
    if(perm & TRBLE_ATTR_PERM_W_ENCRYPT)
        write_perm = GATT_PERM_WRITE_ENCRYPTED_REQ;

    return (read_perm|write_perm);
}
static int setup_ble_char_val_attr(T_ATTRIB_APPL *attr, uint8_t *uuid, uint8_t perm, uint16_t uuid_length)
{
    attr->permissions = switch_perm(perm);
    if(uuid_length == 16)
    {
        attr->flags = ATTRIB_FLAG_UUID_128BIT | ATTRIB_FLAG_VALUE_APPL;
        memcpy(attr->type_value, uuid, 16);
        debug_print("char_val_uuid 0x \n");
        for(uint16_t i = 0; i < 16; i++)
            debug_print("%x", attr->type_value[i]);
        debug_print(" permission = 0x%x \n", attr->permissions);
    } else {
        attr->flags = ATTRIB_FLAG_VALUE_APPL;
        attr->type_value[0] = uuid[0];
        attr->type_value[1] = uuid[1];
        debug_print("char_val_uuid = 0x%x%x \n", uuid[0], uuid[1]);
    }
    attr->value_len = 0; 
    attr->p_value_context = NULL;

    return 0;
}

static int setup_ble_char_dec_attr(T_ATTRIB_APPL *attr, uint8_t *uuid, uint8_t prop, uint16_t uuid_length)
{
    attr->flags = ATTRIB_FLAG_VALUE_INCL;
    attr->type_value[0] = LO_WORD(GATT_UUID_CHARACTERISTIC);
    attr->type_value[1] = HI_WORD(GATT_UUID_CHARACTERISTIC);
    attr->type_value[2] = prop;
    attr->value_len     = 1;
    attr->p_value_context = NULL;
    attr->permissions   = GATT_PERM_READ;
    debug_print("set characteristic declartion \n");
    return 0;
}

static int setup_ble_service_attr(T_ATTRIB_APPL *attr, uint8_t *uuid, uint16_t uuid_length)
{
    attr->type_value[0] = LO_WORD(GATT_UUID_PRIMARY_SERVICE);       /* type */
    attr->type_value[1] = HI_WORD(GATT_UUID_PRIMARY_SERVICE);
    if(uuid_length == 16)
    {
        attr->flags = ATTRIB_FLAG_LE | ATTRIB_FLAG_VOID;
        attr->p_value_context = uuid;
        attr->value_len = uuid_length;
        debug_print("srv_uuid 0x \n");
        for(uint16_t i = 0; i < 16; i++)
            debug_print("%x", *(((uint8_t *)attr->p_value_context)+i));
    } else {
        attr->flags = ATTRIB_FLAG_LE | ATTRIB_FLAG_VALUE_INCL;
        attr->type_value[2] = uuid[0];        /* value */
        attr->type_value[3] = uuid[1];
        attr->value_len = 2;
        attr->p_value_context = NULL;
        debug_print("srv_uuid 0x%x%x \n", uuid[0], uuid[1]);
    }

    attr->permissions   = GATT_PERM_READ;
    return 0;
}

extern uint16_t server_profile_count;
bool parse_service_table(trble_gatt_t *profile, uint16_t profile_count)
{
    debug_print("tizenrt_ble_service_tbl profile %p profile_count %d \n", profile, profile_count);
    abs_att_count = 0;
    tizenrt_ble_service_tbl = (T_ATTRIB_APPL *)osif_mem_alloc(0, profile_count * sizeof(T_ATTRIB_APPL));
    memset(tizenrt_ble_service_tbl, 0, profile_count * sizeof(T_ATTRIB_APPL));
    uint8_t srv_index = 0;
    uint8_t char_index = 0;
    uint16_t j = 0;
    for (int i = 0; i < server_profile_count; i++)
    {
        if(profile[i].type ==  TRBLE_GATT_SERVICE)
        {
            srv_index = tizenrt_ble_srv_count++;    /* add the service counter */
            char_index = 0;     /* clr the char_index when begain parse a new service */
            debug_print("Parse service decleration \n");
            setup_ble_service_attr(&tizenrt_ble_service_tbl[j++], profile[i].uuid ,profile[i].uuid_length);
            setup_ble_srv_info(srv_index, &profile[i]);
        } else if(profile[i].type ==  TRBLE_GATT_CHARACT) 
        {
            setup_ble_char_dec_attr(&tizenrt_ble_service_tbl[j++], profile[i].uuid, profile[i].property, profile[i].uuid_length);
            setup_ble_char_val_attr(&tizenrt_ble_service_tbl[j++], profile[i].uuid, profile[i].permission, profile[i].uuid_length);
            setup_ble_char_info(srv_index, char_index, &profile[i]);
            char_index++;
        } else if(profile[i].type ==  TRBLE_GATT_DESC)
        {
            setup_ble_char_ccc_attr(&tizenrt_ble_service_tbl[j++], 0, profile[i].uuid_length);
            setup_ble_char_info(srv_index, char_index, &profile[i]);
            char_index++;
        }
    }
    debug_print("tizenrt_ble_srv_count = %d \n", tizenrt_ble_srv_count);
    debug_print("Init Success \n");
    return true;
}

uint16_t ble_tizenrt_srv_add(void)
{
    parse_service_table(server_init_parm.profile, server_init_parm.profile_count);
    for (int i = 0; i < tizenrt_ble_srv_count ; i++)
    {
        tizenrt_add_service(i, TIZENRT_SRV_ID + i);
    }
    return RTK_BT_OK;
}

