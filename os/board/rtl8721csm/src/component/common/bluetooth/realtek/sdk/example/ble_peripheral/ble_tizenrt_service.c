#include <ble_tizenrt_service.h>
#include <stddef.h>
#include <gap.h>
#include <os_mem.h>
#include "ble_tizenrt_app.h"

static P_FUN_SERVER_GENERAL_CB tizenet_ble_service_cb = NULL;
static uint8_t tizenrt_ble_write_value[TIZENRT_MAX_VAL_LEN];
/* service table contains several services */
T_ATTRIB_APPL *tizenrt_ble_service_tbl = NULL;
static uint16_t abs_att_count = 0;

TIZENERT_SRV_CNT tizenrt_ble_srv_count = 0;
TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7] = {0};

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

bool tizenrt_ble_service_send_notify(uint8_t conn_id, uint16_t abs_handle, uint8_t *p_value,
                                     uint16_t length)
{
    T_SERVER_ID service_id = 0xff;
    uint16_t cha_index = 0;
    for(uint8_t i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(tizenrt_ble_srv_database[i].start_handle < abs_handle && 
            tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= abs_handle)
        {
            service_id = tizenrt_ble_srv_database[i].srv_id;
            cha_index = abs_handle - tizenrt_ble_srv_database[i].start_handle;
        }
    }
    /* send notification to client */
    debug_print("service_id %d index = 0x%x, value = 0x%x len = 0x%x \n",
                                        service_id, cha_index, *p_value, length);
    return server_send_data(conn_id, service_id, cha_index, p_value,
                            length, GATT_PDU_TYPE_NOTIFICATION);
}

trble_attr_handle indicate_attr;
bool tizenrt_ble_service_send_indicate(uint8_t conn_id, uint16_t abs_handle, uint8_t *p_value,
                                     uint16_t length)
{
    T_SERVER_ID service_id = 0xff;
    uint16_t cha_index = 0;
    for(uint8_t i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(tizenrt_ble_srv_database[i].start_handle < abs_handle &&
            tizenrt_ble_srv_database[i].start_handle + tizenrt_ble_srv_database[i].att_count >= abs_handle)
        {
            service_id = tizenrt_ble_srv_database[i].srv_id;
            cha_index = abs_handle - tizenrt_ble_srv_database[i].start_handle;
        }
    }
    /* send notification to client */
    debug_print("service_id %d index = 0x%x, value = 0x%x len = 0x%x \n",
                                        service_id, cha_index, *p_value, length);
    indicate_attr = cha_index;
    return server_send_data(conn_id, service_id, cha_index, p_value,
                            length, GATT_PDU_TYPE_INDICATION);
}

void tizenrt_ble_service_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                     uint16_t cccbits)
{
    TIZENRT_CALLBACK_DATA callback_data;
    callback_data.conn_id = conn_id;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    callback_data.val = cccbits;
    for(uint8_t i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(service_id == tizenrt_ble_srv_database[i].srv_id)
        {
            callback_data.srv_index = i;
            for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
            {
                if(attrib_index == tizenrt_ble_srv_database[i].chrc_info[j].index)
                {
                    callback_data.att_index = j;
                    debug_print("Attribute 0x%x cccbits 0x%x cb %p srv_index %d cha_index 0x%x \n",
                                                                    tizenrt_ble_srv_database[i].chrc_info[j].abs_handle,
                                                                    cccbits, tizenrt_ble_srv_database[i].chrc_info[j].cb,
                                                                    i, j);
                }
            }
        }
    }

    /* Notify Application. */
    if (tizenet_ble_service_cb)
    {
        tizenet_ble_service_cb(service_id, (void *)&callback_data);
    }
}

T_APP_RESULT tizenrt_ble_service_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                            uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                            P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;
    TIZENRT_CALLBACK_DATA callback_data;
    TIZENERT_CHA_INFO *p_char_info = NULL;

    /* Make sure written value size is valid. */
    if (p_value == NULL)
    {
        cause  = APP_RESULT_INVALID_VALUE_SIZE;
    }
    else
    {
        /* Notify Application. */
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
        callback_data.conn_id  = conn_id;
        callback_data.val = write_type;

        for(int i = 0; i < tizenrt_ble_srv_count; i++)
        {
            if(service_id == tizenrt_ble_srv_database[i].srv_id)
            {
                callback_data.srv_index = i;
                for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
                {
                    if(attrib_index == tizenrt_ble_srv_database[i].chrc_info[j].index)
                    {
                        p_char_info = &tizenrt_ble_srv_database[i].chrc_info[j];
                        callback_data.att_index = j;
                        memcpy(p_char_info->data, p_value, length);
                        p_char_info->data_len = length;
                        debug_print("service_id %d index 0x%x att_index %d abs_handle 0x%x data 0x \n",
                                                                service_id, attrib_index, j,
                                                                p_char_info->abs_handle);
                        for (int k = 0; k < p_char_info->data_len; k++)
                        {
                            debug_print("%x", *(p_char_info->data + k));
                        }
                    }
                }
            }
        }

        if (tizenet_ble_service_cb)
        {
            tizenet_ble_service_cb(service_id, (void *)&callback_data);
        }
    }

    if(p_char_info->app_reject)
    {
        debug_print("app_reject = 0x%x \n", p_char_info->app_reject);
        return p_char_info->app_reject;
    }
    return cause;
}


T_APP_RESULT  tizenrt_ble_service_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                            uint16_t attrib_index, uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    (void)offset;
    T_APP_RESULT  cause  = APP_RESULT_SUCCESS;

    debug_print("service_id %d index 0x%x \n", service_id, attrib_index);

    TIZENRT_CALLBACK_DATA callback_data;
    TIZENERT_CHA_INFO *p_char_info = NULL;

    callback_data.conn_id = conn_id;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
    for(int i = 0; i < tizenrt_ble_srv_count; i++)
    {
        if(service_id == tizenrt_ble_srv_database[i].srv_id)
        {
            callback_data.srv_index = i;
            for (int j = 0; j < TIZENRT_MAX_ATTR_NUM; j++)
            {
                if(attrib_index == tizenrt_ble_srv_database[i].chrc_info[j].index)
                {   
                    p_char_info = &tizenrt_ble_srv_database[i].chrc_info[j];
                    callback_data.att_index = j;
                    debug_print("att_index %d abs_handle 0x%x \n", 
                                callback_data.att_index, p_char_info->abs_handle);
                }
            }
        }
    }

    if (tizenet_ble_service_cb)
    {
        tizenet_ble_service_cb(service_id, (void *)&callback_data);
    }
    *pp_value = p_char_info->read_ptr;
    *p_length = p_char_info->read_len;

    debug_print("p_char_info->data %p  \n", p_char_info->data);
    debug_print("pp_value %p \n", *pp_value);
    for (int i = 0; i < *p_length; i++)
    {
        debug_print("%x", *(*pp_value + i));
    }
    for (int i = 0; i < *p_length; i++)
    {
        debug_print("%x", *(*pp_value + i));
    }
    if(p_char_info->app_reject)
    {
        debug_print("app_reject = 0x%x \n", p_char_info->app_reject);
        return p_char_info->app_reject;
    }
    return (cause);
}

const T_FUN_GATT_SERVICE_CBS tizenrt_ble_service_cbs =
{
    tizenrt_ble_service_attr_read_cb,   /* Read callback function pointer */
    tizenrt_ble_service_attr_write_cb,  /* Write callback function pointer */
    tizenrt_ble_service_cccd_update_cb  /* CCCD update callback function pointer */
};

T_SERVER_ID tizenrt_add_service(void *p_func, uint8_t index)
{
    T_SERVER_ID service_id;
    T_ATTRIB_APPL *tizenrt_attr_tbl = tizenrt_ble_service_tbl + tizenrt_ble_srv_database[index].abs_handle;
    uint16_t tizenrt_attr_tbl_size = tizenrt_ble_srv_database[index].att_count * sizeof(T_ATTRIB_APPL);
    uint16_t tizenrt_attr_start_handle = tizenrt_ble_srv_database[index].start_handle;
#if 0
    debug_print("tizenrt_ble_srv_database[%d].abs_handle = %d tizenrt_attr_tbl_cnt = %d \n",
                                                index,
                                                tizenrt_ble_srv_database[index].abs_handle,
                                                tizenrt_ble_srv_database[index].att_count);
    if (false == server_add_service(&service_id,
                                    tizenrt_attr_tbl,
                                    tizenrt_attr_tbl_size,
                                    tizenrt_ble_service_cbs))
#else
    debug_print("tizenrt_ble_srv_database[%d].abs_handle = %d tizenrt_attr_tbl_size = %d start_handle = 0x%x \n",
                                                index,
                                                tizenrt_ble_srv_database[index].abs_handle,
                                                tizenrt_attr_tbl_size,
                                                tizenrt_attr_start_handle);
    if (false == server_add_service_by_start_handle(&service_id,
                                    (uint8_t *)tizenrt_attr_tbl,
                                    tizenrt_attr_tbl_size,
                                    tizenrt_ble_service_cbs,
                                    tizenrt_attr_start_handle))
#endif
    {
        dbg("add service fail \n");
        service_id = 0xff;
        return service_id;
    }
    tizenet_ble_service_cb = (P_FUN_SERVER_GENERAL_CB) p_func;
    tizenrt_ble_srv_database[index].srv_id = service_id;
    dbg("add service %d success \n", service_id);
    return service_id;
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
    tizenrt_ble_service_tbl = (T_ATTRIB_APPL *)os_mem_alloc(0, profile_count * sizeof(T_ATTRIB_APPL));
    if (!tizenrt_ble_service_tbl) {
        debug_print("tizenrt_ble_service_tbl malloc fail\n");
        return false;
    }
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
