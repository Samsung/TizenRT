/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <platform_opts_bt.h>
#include <rtk_bt_common.h>
#include <rtk_bt_def.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_avrcp.h>
#include <rtk_bt_a2dp.h>
#include <rtk_bt_sdp.h>
#include <bt_ipc_cfg.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_profile_config.h>
#include <bt_ipc_parameter_conversion.h>

#define ALIGN_UP(a, size)       ((a+size-1) & (~(size-1)))

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP

/* ===============================================rtk_bt_le_gap ipc action======================================= */
static struct act_mem_option rtk_bt_le_gap_act_mem_op_tab[] = {
	{RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR, sizeof(rtk_bt_le_addr_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_BOND_NUM, sizeof(uint8_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_BOND_INFO, sizeof(rtk_bt_le_bond_info_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_CONN_INFO,sizeof(rtk_bt_le_conn_info_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_DEV_STATE,sizeof(rtk_bt_le_gap_dev_state_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN, sizeof(rtk_bt_le_get_active_conn_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR,sizeof(uint16_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_MTU_SIZE, sizeof(uint16_t)},
	{RTK_BT_LE_GAP_ACT_MAX, 0}
};

uint32_t rtk_le_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_gap_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_le_gap_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_le_gap_act_mem_op_tab[i].act == RTK_BT_LE_GAP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_le_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR || act == RTK_BT_LE_GAP_ACT_GET_BOND_NUM ) {
		memcpy((void *)dst, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_CONN_INFO) {
		rtk_bt_le_get_conn_info_param_t *pget_info = (rtk_bt_le_get_conn_info_param_t *)dst;
		memcpy((void *)pget_info->p_conn_info, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_BOND_INFO) {
		rtk_bt_le_get_bond_info_param_t *pget_bondinfo = (rtk_bt_le_get_bond_info_param_t *)dst;
		memcpy((void *)pget_bondinfo->p_size, (void *)src, sizeof(uint8_t));
		memcpy((void *)pget_bondinfo->p_bond_info, (void *)((uint8_t *)src + 4), sizeof(rtk_bt_le_bond_info_t)*(*pget_bondinfo->p_size));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_DEV_STATE) {
		rtk_bt_le_gap_dev_state_t *pget_dev_state = (rtk_bt_le_gap_dev_state_t *)dst;
		memcpy((void *)pget_dev_state, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN) {
		rtk_bt_le_get_active_conn_t *pactive_conn = (rtk_bt_le_get_active_conn_t *)dst;
		memcpy((void *)pactive_conn, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR) {
		rtk_bt_le_get_conn_handle_by_addr_param_t *pconn_handle_param_t = (rtk_bt_le_get_conn_handle_by_addr_param_t *)dst;
		memcpy((void *)pconn_handle_param_t->p_conn_handle, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_MTU_SIZE) {
		rtk_bt_le_get_mtu_size_param_t *pget_mtu_size_param_t = (rtk_bt_le_get_mtu_size_param_t *)dst;
		memcpy((void *)pget_mtu_size_param_t->p_mtu_size, (void *)src, rtk_le_get_act_ret_param_size(act));
	}

	return RTK_BT_OK;
}

static int le_ipc_scan_info_filter_push(uint8_t *buf, rtk_bt_le_scan_info_filter_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	/* memory space for rtk_bt_gatts_ntf_and_ind_param_t */
	size = sizeof(rtk_bt_le_scan_info_filter_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_filter, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_scan_info_filter_param_t *)buf)->p_filter = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_scan_info_filter_param_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_le_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	case RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER:
		if (le_ipc_scan_info_filter_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_scan_info_filter_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_le_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_gatts ipc action======================================= */
static struct act_mem_option rtk_bt_gatts_act_mem_op_tab[] = {
	{RTK_BT_GATTS_ACT_MAX, 0},
};

uint32_t rtk_gatts_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gatts_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_gatts_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_gatts_act_mem_op_tab[i].act == RTK_BT_GATTS_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_gatts_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	/* TODO */
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

static void gatts_ipc_write_evt_pop(rtk_bt_gatts_write_ind_t *write_evt)
{
	write_evt->value = (void *)((uint8_t *)write_evt + (uint32_t)(write_evt->value));
}

static uint32_t get_struct_bt_uuid_size(struct bt_uuid *uuid)
{
	uint32_t size = 0;
	if (BT_UUID_TYPE_16 == uuid->type) {
		size = sizeof(struct bt_uuid_16);
	} else if (BT_UUID_TYPE_128 == uuid->type) {
		size = sizeof(struct bt_uuid_128);
	}
	return size;
}

static uint32_t get_struct_attr_data_size(void *param)
{
	rtk_bt_gatt_attr_t *attr = (rtk_bt_gatt_attr_t *)param;
	uint32_t size = 0;
	uint16_t uuid = ((struct bt_uuid_16 *)attr->uuid)->val;
	switch (uuid) {
	case BT_UUID_GATT_PRIMARY_VAL:
	case BT_UUID_GATT_SECONDARY_VAL:
		size = get_struct_bt_uuid_size((struct bt_uuid *)attr->user_data);
		break;
	case BT_UUID_GATT_INCLUDE_VAL:
		size = sizeof(struct rtk_bt_gatt_service);
		break;
	case BT_UUID_GATT_CHRC_VAL:
		size = sizeof(struct rtk_bt_gatt_chrc);
		break;
	case BT_UUID_GATT_CEP_VAL:
		size = sizeof(struct rtk_bt_gatt_cep);
		break;
	case BT_UUID_GATT_CUD_VAL:
		size = strlen(attr->user_data) + 1;
		break;
	case BT_UUID_GATT_CCC_VAL:
		size = 0;
		break;
	case BT_UUID_GATT_SCC_VAL:
		size = sizeof(struct rtk_bt_gatt_scc);
		break;
	case BT_UUID_GATT_CPF_VAL:
		size = sizeof(struct rtk_bt_gatt_cpf);
		break;
	case BT_UUID_GATT_CAF_VAL:
		size = sizeof(struct rtk_bt_gatt_caf);
		break;
	default:
		size = attr->len;
		break;
	}
	
	return size;
}

/* push the multi-level pointer param to a blocked memory buf */
static int gatts_ipc_reg_srv_param_push(uint8_t *buf, struct rtk_bt_gatt_service *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint16_t uuid_val;
	struct rtk_bt_gatt_service *buf_ptr = (struct rtk_bt_gatt_service *)buf;
	struct rtk_bt_gatt_chrc *charac = NULL;
	struct rtk_bt_gatt_caf *caf = NULL;
	uint32_t size = 0;
	uint32_t attr_count = param->attr_count;
	uint32_t i = 0;

	/* memory space for struct rtk_bt_gatt_service */
	size = sizeof(struct rtk_bt_gatt_service);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	buf_ptr->attrs = (rtk_bt_gatt_attr_t *)end;

	/* memory space for struct rtk_bt_gatt_attr_t of attr_count num */
	size = sizeof(rtk_bt_gatt_attr_t) * attr_count;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->attrs, size);
	end += ALIGN_UP(size, 4);
	
	for (i = 0; i < attr_count; i++) {
		buf_ptr->attrs[i].uuid = (const struct bt_uuid *)end;
		/* memory space for struct bt_uuid */
		size = get_struct_bt_uuid_size((struct bt_uuid *)param->attrs[i].uuid);

		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->attrs[i].uuid, size);
		end += ALIGN_UP(size, 4);

		/* memory space for void *user_data */
		if (param->attrs[i].user_data) {
			buf_ptr->attrs[i].user_data = end;
			size =  get_struct_attr_data_size(&param->attrs[i]);
			
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->attrs[i].user_data, size);
			end += ALIGN_UP(size, 4);
		} else {
			buf_ptr->attrs[i].user_data = NULL;
		}
	}

	void *data_in_user_data = NULL;
	for (i = 0; i < attr_count; i++) {
		uuid_val = ((struct bt_uuid_16 *)param->attrs[i].uuid)->val;
		/* memory space for second level pointer in user_data */
		if (uuid_val == BT_UUID_GATT_CHRC_VAL) {
			charac = (struct rtk_bt_gatt_chrc *)param->attrs[i].user_data;
			((struct rtk_bt_gatt_chrc *)buf_ptr->attrs[i].user_data)->uuid = (const struct bt_uuid *)end;
			size = get_struct_bt_uuid_size((struct bt_uuid *)charac->uuid);
			data_in_user_data = (void *)charac->uuid;
		} else if (uuid_val == BT_UUID_GATT_CAF_VAL) {
			caf = (struct rtk_bt_gatt_caf *)param->attrs[i].user_data;
			((struct rtk_bt_gatt_caf *)buf_ptr->attrs[i].user_data)->handles = (uint16_t *)end;
			size = (caf->count) * sizeof(*(caf->handles));
			data_in_user_data = (void *)caf->handles;
		} else {
			continue;
		}

		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, data_in_user_data, size);
		end += ALIGN_UP(size, 4);
	}
	/* !!! The value of pointer should be assigned with memory address offset */ 
	/* cause once the memory is copied by another core, the pointer memory addres is meanless, so memory address offset is useful */
	for (i = 0; i < attr_count; i++) {
		uuid_val = ((struct bt_uuid_16 *)param->attrs[i].uuid)->val;
		if (param->attrs[i].user_data) {
			if (uuid_val == BT_UUID_GATT_CHRC_VAL) {
				charac = (struct rtk_bt_gatt_chrc *)buf_ptr->attrs[i].user_data;
				charac->uuid = (const struct bt_uuid *)(((uint8_t *)charac->uuid) - buf);
			} else if (uuid_val == BT_UUID_GATT_CAF_VAL) {
				caf = (struct rtk_bt_gatt_caf *)buf_ptr->attrs[i].user_data;
				caf->handles = (uint16_t *)(((uint8_t *)caf->handles) - buf);
			}
			buf_ptr->attrs[i].user_data = (void *)((uint8_t *)buf_ptr->attrs[i].user_data - buf);
		}
		buf_ptr->attrs[i].uuid = (const struct bt_uuid *)((uint8_t *)buf_ptr->attrs[i].uuid - buf);
	}
	buf_ptr->attrs = (rtk_bt_gatt_attr_t *)((uint8_t *)buf_ptr->attrs - buf);
	
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatt_service size is over deep copy buf\r\n");
	return -1;
}

static int gatts_ipc_indicate_param_push(uint8_t *buf, rtk_bt_gatts_ntf_and_ind_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	/* memory space for rtk_bt_gatts_ntf_and_ind_param_t */
	size = sizeof(rtk_bt_gatts_ntf_and_ind_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gatts_ntf_and_ind_param_t *)buf)->data = (const void *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatts_ntf_and_ind_param_t size is over deep copy buf\r\n");
	return -1;
}

static int gatts_ipc_notify_param_push(uint8_t *buf, rtk_bt_gatts_ntf_and_ind_param_t *param, 
							uint32_t size_limit, uint32_t *actual_size)
{
	/* the structure of notify param is same as indicate param */
	return gatts_ipc_indicate_param_push(buf, param, size_limit, actual_size);
}

static int gatts_ipc_read_resp_param_push(uint8_t *buf, rtk_bt_gatts_read_resp_param_t *param, 
							uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_gatts_read_resp_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gatts_read_resp_param_t *)buf)->data = (const void *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatts_read_resp_param_t size is over deep copy buf\r\n");
	return -1;
}

void bt_gatts_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_GATTS_EVT_WRITE_IND:
		gatts_ipc_write_evt_pop(param);
		break;
	default:
		break;
	}
}

void *bt_gatts_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	case RTK_BT_GATTS_ACT_REGISTER_SERVICE:
		if (gatts_ipc_reg_srv_param_push((uint8_t *)host_msg->param_buf, (struct rtk_bt_gatt_service *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	
	case RTK_BT_GATTS_ACT_NOTIFY:
		if (gatts_ipc_notify_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gatts_ntf_and_ind_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)){
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_GATTS_ACT_INDICATE:
		if (gatts_ipc_indicate_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gatts_ntf_and_ind_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_GATTS_ACT_READ_RSP:
		if (gatts_ipc_read_resp_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gatts_read_resp_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

/* ===============================================rtk_bt_gattc ipc action======================================= */
static struct act_mem_option rtk_bt_gattc_act_mem_op_tab[] = {
	{RTK_BT_GATTC_ACT_MAX, 0},
};

uint32_t rtk_gattc_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gattc_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_gattc_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_gattc_act_mem_op_tab[i].act == RTK_BT_GATTC_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_gattc_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	/* TODO */
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

static void gattc_ipc_read_evt_pop(rtk_bt_gattc_read_ind_t *read_evt)
{
	switch (read_evt->type) {
	case RTK_BT_GATT_CHAR_READ_BY_HANDLE:
		read_evt->by_handle.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->by_handle.value));
		break;

	case RTK_BT_GATT_CHAR_READ_BY_UUID:
		read_evt->by_uuid_per.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->by_uuid_per.value));
		break;
	
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		read_evt->multiple_per.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->multiple_per.value));
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		read_evt->multiple_variable_per.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->multiple_variable_per.value));
		break;

	default:
		printf("rtk_bt_gattc_read_ind_t pop type unknown\r\n");
		break;
	}
}

static void gattc_ipc_notify_indicate_evt_pop(rtk_bt_gattc_cccd_value_ind_t *evt)
{
	evt->value = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->value));
}

/* TODO */
void bt_gattc_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		gattc_ipc_read_evt_pop(param);
		break;

	case RTK_BT_GATTC_EVT_NOTIFY_IND:
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		gattc_ipc_notify_indicate_evt_pop(param);
		break;

	default:
		break;
	}
}

/* push the multi-level pointer param to a blocked memory buf */
static int gattc_ipc_read_param_push(uint8_t *buf, rtk_bt_gattc_read_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	rtk_bt_gattc_read_param_t *p_read_param = (rtk_bt_gattc_read_param_t *)buf;
	uint32_t size = 0;
	uint32_t handle_cout=0;

	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		handle_cout = param->multiple.handle_count;
		/* memory space for struct rtk_bt_gattc_read_param_t */
		size = sizeof(rtk_bt_gattc_read_param_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		/* parsing handles */
		p_read_param->multiple.handles = (uint16_t *)end;
		if (end + handle_cout * 2 > buf + size_limit)
			goto exit;
		memcpy((void *)p_read_param->multiple.handles, (void *)param->multiple.handles, handle_cout * 2);
		p_read_param->multiple.handles = (uint16_t *)(end - buf);
		*actual_size = (uint32_t)((end + handle_cout * 2) - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		handle_cout = param->multiple_variable.handle_count;
		/* memory space for struct rtk_bt_gattc_read_param_t */
		size = sizeof(rtk_bt_gattc_read_param_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		/* parsing handles */
		p_read_param->multiple_variable.handles = (uint16_t *)end;
		if (end + handle_cout * 2 > buf + size_limit)
			goto exit;
		memcpy((void *)p_read_param->multiple_variable.handles, (void *)param->multiple_variable.handles, handle_cout * 2);
		p_read_param->multiple_variable.handles = (uint16_t *)(end - buf);
		*actual_size = (uint32_t)((end + handle_cout * 2) - buf);
		break;

	default:
		memcpy((void *)buf, (void *)param, sizeof(rtk_bt_gattc_read_param_t));
		*actual_size = sizeof(rtk_bt_gattc_read_param_t);
		break;
	}

	return 0;

exit:
	printf("rtk_bt_gattc_read_param_t size is over deep copy buf\r\n");
	return -1;
}

/* push the multi-level pointer param to a blocked memory buf */
static int gattc_ipc_write_param_push(uint8_t *buf, rtk_bt_gattc_write_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	rtk_bt_gattc_write_param_t *p_write_param = (rtk_bt_gattc_write_param_t *)buf;
	uint32_t size = 0;

	/* memory space for struct rtk_bt_gattc_write_param_t */
	size = sizeof(rtk_bt_gattc_write_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_write_param->data = (void *)end;
	if (end + param->length > buf + size_limit)
		goto exit;
	memcpy((void *)p_write_param->data, (void *)param->data, param->length);
	p_write_param->data = (void *)(end - buf);
	*actual_size = (uint32_t)((end + param->length) - buf);

	return 0;

exit:
	printf("rtk_bt_gattc_write_param_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_gattc_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	case RTK_BT_GATTC_ACT_READ:
		if (gattc_ipc_read_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gattc_read_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_GATTC_ACT_WRITE:
		if (gattc_ipc_write_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gattc_write_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

/* ===============================================rtk_bt_br_gap ipc action======================================= */
static struct act_mem_option rtk_bt_br_gap_act_mem_op_tab[] = {
	{RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR, sizeof(rtk_bt_br_bd_addr_t)}, 
	{RTK_BT_BR_GAP_ACT_MAX, 0}
};

uint32_t rtk_br_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_br_gap_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_br_gap_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_br_gap_act_mem_op_tab[i].act == RTK_BT_BR_GAP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_br_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR) {
		memcpy((void *)dst, (void *)src, rtk_br_get_act_ret_param_size(act));
	}

	return RTK_BT_OK;
}

void *bt_br_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_br_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_a2dp ipc action======================================= */
static struct act_mem_option rtk_bt_a2dp_act_mem_op_tab[] = {
	{RTK_BT_A2DP_ACT_MAX, 0}
};

uint32_t rtk_a2dp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_a2dp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_a2dp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_a2dp_act_mem_op_tab[i].act == RTK_BT_A2DP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_a2dp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

void *bt_a2dp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_a2dp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_avrcp ipc action======================================= */
static struct act_mem_option rtk_bt_avrcp_act_mem_op_tab[] = {
	{RTK_BT_AVRCP_ACT_MAX, 0}
};

uint32_t rtk_avrcp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_avrcp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_avrcp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_avrcp_act_mem_op_tab[i].act == RTK_BT_AVRCP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}

	return max_size;
}

uint16_t rtk_avrcp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

void *bt_avrcp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_avrcp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_sdp ipc action======================================= */
static struct act_mem_option rtk_bt_sdp_act_mem_op_tab[] = {
	{RTK_BT_SDP_ACT_MAX, 0}
};

uint32_t rtk_sdp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_sdp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_sdp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_sdp_act_mem_op_tab[i].act == RTK_BT_SDP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}

	return max_size;
}

uint16_t rtk_sdp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

void *bt_sdp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_sdp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

#elif defined(CONFIG_BT_NP) && CONFIG_BT_NP
/* ===============================================rtk_bt_le_gap_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_le_gap_evt_mem_op_tab[] = {
	{RTK_BT_LE_GAP_EVT_ADV_START_IND, sizeof(rtk_bt_le_adv_start_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_ADV_STOP_IND, sizeof(rtk_bt_le_adv_stop_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_CONNECT_IND, sizeof(rtk_bt_le_conn_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_DISCONN_IND, sizeof(rtk_bt_le_disconn_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_SCAN_START_IND, sizeof(uint16_t)}, 
	{RTK_BT_LE_GAP_EVT_SCAN_RES_IND, sizeof(rtk_bt_le_scan_res_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_SCAN_STOP_IND, sizeof(uint16_t)}, 
	{RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND, sizeof(rtk_bt_le_conn_update_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND, sizeof(rtk_bt_le_remote_conn_update_req_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_DATA_LEN_CHANGE_IND, sizeof(rtk_bt_le_data_len_change_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_PHY_UPDATE_IND, sizeof(rtk_bt_le_phy_update_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_WHITELIST_MODIFY_IND, sizeof(rtk_bt_le_wl_modify_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND, sizeof(rtk_bt_le_auth_pair_cfm_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND, sizeof(rtk_bt_le_auth_key_display_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND, sizeof(rtk_bt_le_auth_key_input_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND, sizeof(rtk_bt_le_auth_key_cfm_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND, sizeof(rtk_bt_le_auth_oob_input_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND, sizeof(rtk_bt_le_auth_complete_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND, sizeof(rtk_bt_le_bond_modify_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_MAX, 0}
};

uint32_t get_bt_le_gap_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_gap_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_le_gap_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_le_gap_evt_mem_op_tab[i].evt == RTK_BT_LE_GAP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

static void le_ipc_scan_info_filter_pop(rtk_bt_le_scan_info_filter_param_t *p_scan_info)
{
	p_scan_info->p_filter = (uint8_t *)((uint8_t *)p_scan_info + (uint32_t)p_scan_info->p_filter);
}

void bt_le_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER:
		le_ipc_scan_info_filter_pop((rtk_bt_le_scan_info_filter_param_t *)param);
		break;

	default:
		break;
	}
}

void *bt_le_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_gatts_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_gatts_evt_mem_op_tab[] = {
	{RTK_BT_GATTS_EVT_REGISTER_SERVICE, sizeof(rtk_bt_gatts_reg_ind_t)}, 
	{RTK_BT_GATTS_EVT_MTU_EXCHANGE, sizeof(rtk_bt_gatt_mtu_exchange_ind_t)}, 
	{RTK_BT_GATTS_EVT_READ_IND, sizeof(rtk_bt_gatts_read_ind_t)}, 
	{RTK_BT_GATTS_EVT_WRITE_IND,sizeof(rtk_bt_gatts_write_ind_t)}, 
	{RTK_BT_GATTS_EVT_CCCD_IND,sizeof(rtk_bt_gatts_cccd_ind_t)}, 
	{RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND, sizeof(rtk_bt_gatts_ntf_and_ind_ind_t)}, 
	{RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND,sizeof(rtk_bt_gatts_ntf_and_ind_ind_t)}, 
	{RTK_BT_GATTS_EVT_MAX, 0}
};

uint32_t get_bt_gatts_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gatts_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_gatts_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_gatts_evt_mem_op_tab[i].evt == RTK_BT_GATTS_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

static void gatts_ipc_reg_srv_param_pop(struct rtk_bt_gatt_service *srv_reg)
{
	uint32_t i, attr_count = 0;
	uint16_t uuid_val = 0;

	attr_count = srv_reg->attr_count;
	srv_reg->attrs = (void *) ((uint8_t *)srv_reg + (uint32_t)(srv_reg->attrs));
	for (i = 0; i < attr_count; i++) {
		srv_reg->attrs[i].uuid = (void *)((uint8_t *)srv_reg + (uint32_t)(srv_reg->attrs[i].uuid));
		if (srv_reg->attrs[i].user_data) {
			srv_reg->attrs[i].user_data = (void *)((uint8_t *)srv_reg + 
											(uint32_t)(srv_reg->attrs[i].user_data));

			if (BT_UUID_TYPE_16 == ((struct bt_uuid *)srv_reg->attrs[i].uuid)->type) {
				uuid_val =  ((struct bt_uuid_16 *)srv_reg->attrs[i].uuid)->val;
				if (BT_UUID_GATT_CHRC_VAL == uuid_val) {
					struct rtk_bt_gatt_chrc *chrc_user_data = srv_reg->attrs[i].user_data;
					chrc_user_data->uuid = (void *)((uint8_t *)(srv_reg) + (uint32_t)(chrc_user_data->uuid));
				} else if (BT_UUID_GATT_CAF_VAL == uuid_val) {
					struct rtk_bt_gatt_caf *caf = srv_reg->attrs[i].user_data;
					caf->handles = (uint16_t *)((uint8_t *)(srv_reg) + (uint32_t)(caf->handles));
				}
			}								
		}
	}
	
}

static void gatts_ipc_indicate_param_pop(rtk_bt_gatts_ntf_and_ind_param_t *ind)
{
	ind->data = (void *)((uint8_t *)ind + (uint32_t)ind->data);
}

static void gatts_ipc_notify_param_pop(rtk_bt_gatts_ntf_and_ind_param_t *notify)
{
	gatts_ipc_indicate_param_pop(notify);
}

static void gatts_ipc_read_resp_param_pop(rtk_bt_gatts_read_resp_param_t *resp_read)
{
	resp_read->data = (void *)((uint8_t *)resp_read + (uint32_t)(resp_read->data));
}

int gatts_ipc_write_evt_push(uint8_t *buf, rtk_bt_gatts_write_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_gatts_write_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->value, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gatts_write_ind_t *)buf)->value = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatts_write_ind_t size is over deep copy buf\r\n");
	return -1;
}

void bt_gatts_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_GATTS_ACT_REGISTER_SERVICE:
		gatts_ipc_reg_srv_param_pop((struct rtk_bt_gatt_service *)param);
		break;

	case RTK_BT_GATTS_ACT_NOTIFY:
		gatts_ipc_notify_param_pop((rtk_bt_gatts_ntf_and_ind_param_t *)param);
		break;

	case RTK_BT_GATTS_ACT_INDICATE:
		gatts_ipc_indicate_param_pop((rtk_bt_gatts_ntf_and_ind_param_t *)param);
		break;

	case RTK_BT_GATTS_ACT_READ_RSP:
		gatts_ipc_read_resp_param_pop((rtk_bt_gatts_read_resp_param_t *)param);
		break;

	default:
		break;
	}
}

void *bt_gatts_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	case RTK_BT_GATTS_EVT_WRITE_IND:
		if (gatts_ipc_write_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size)) {
			osif_mem_free(dev_msg);
			dev_msg = NULL;
		}
		break;

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_gattc_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_gattc_evt_mem_op_tab[] = {
	{RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND, sizeof(rtk_bt_gattc_discover_ind_t)},
	{RTK_BT_GATTC_EVT_READ_RESULT_IND, sizeof(rtk_bt_gattc_read_ind_t)},
	{RTK_BT_GATTC_EVT_WRITE_RESULT_IND, sizeof(rtk_bt_gattc_write_ind_t)},
	{RTK_BT_GATTC_EVT_CCCD_ENABLE_IND, sizeof(rtk_bt_gattc_cccd_update_ind_t)},
	{RTK_BT_GATTC_EVT_CCCD_DISABLE_IND, sizeof(rtk_bt_gattc_cccd_update_ind_t)},
	{RTK_BT_GATTC_EVT_NOTIFY_IND, sizeof(rtk_bt_gattc_cccd_value_ind_t)},
	{RTK_BT_GATTC_EVT_INDICATE_IND, sizeof(rtk_bt_gattc_cccd_value_ind_t)},
	{RTK_BT_GATTC_EVT_MTU_EXCHANGE, sizeof(rtk_bt_gatt_mtu_exchange_ind_t)},
	{RTK_BT_GATTC_EVT_MAX, 0}
};

uint32_t get_bt_gattc_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gattc_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_gattc_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_gattc_evt_mem_op_tab[i].evt == RTK_BT_GATTC_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

static void gattc_ipc_read_param_pop(rtk_bt_gattc_read_param_t *param)
{
	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		param->multiple.handles = (uint16_t *)((uint8_t *)param + (uint32_t)param->multiple.handles);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		param->multiple_variable.handles = (uint16_t *)((uint8_t *)param + (uint32_t)param->multiple_variable.handles);
		break;

	default:
		break;
	}
}

static void gattc_ipc_write_param_pop(rtk_bt_gattc_write_param_t *param)
{
	param->data = (const void *)((uint8_t *)param + (uint32_t)param->data);
}

void bt_gattc_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_GATTC_ACT_READ:
		gattc_ipc_read_param_pop((rtk_bt_gattc_read_param_t *)param);
		break;

	case RTK_BT_GATTC_ACT_WRITE:
		gattc_ipc_write_param_pop((rtk_bt_gattc_write_param_t *)param);
		break;

	default:
		break;
	}
}

static int gattc_ipc_read_evt_push(uint8_t *buf, rtk_bt_gattc_read_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_BY_HANDLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		border[0] = end;

		size = param->by_handle.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->by_handle.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->by_handle.value = (uint8_t *)(border[0] - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_BY_UUID:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		border[0] = end;

		size = param->by_uuid_per.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->by_uuid_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->by_uuid_per.value = (uint8_t *)(border[0] - buf);
		*actual_size = (uint32_t)(end - buf);
		break;
	
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		border[0] = end;

		size = param->multiple_per.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->multiple_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->multiple_per.value = (uint8_t *)(border[0] - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		border[0] = end;

		size = param->multiple_variable_per.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->multiple_variable_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->multiple_variable_per.value = (uint8_t *)(border[0] - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	default:
		printf("rtk_bt_gattc_read_ind_t type unknown\r\n");
		goto exit;
		break;
	}

	return 0;

exit:
	printf("rtk_bt_gattc_read_ind_t size is over deep copy buf\r\n");
	return -1;
}

static int gatts_ipc_notify_indicate_evt_push(uint8_t *buf, rtk_bt_gattc_cccd_value_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_gattc_cccd_value_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->value, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gattc_cccd_value_ind_t *)buf)->value = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gattc_cccd_value_ind_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_gattc_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		if (gattc_ipc_read_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size)) {
			osif_mem_free(dev_msg);
			dev_msg = NULL;
		}
		break;

	case RTK_BT_GATTC_EVT_NOTIFY_IND:
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		if (gatts_ipc_notify_indicate_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size)) {
			osif_mem_free(dev_msg);
			dev_msg = NULL;
		}
		break;

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_br_gap_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_br_gap_evt_mem_op_tab[] = {
	{RTK_BT_BR_GAP_EVT_MAX, 0}
};

uint32_t get_bt_br_gap_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_br_gap_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_br_gap_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_br_gap_evt_mem_op_tab[i].evt == RTK_BT_BR_GAP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_br_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_a2dp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_a2dp_evt_mem_op_tab[] = {
	{RTK_BT_A2DP_EVT_CONN_IND, sizeof(rtk_bt_a2dp_conn_ind_t)},
	{RTK_BT_A2DP_EVT_CONFIG_CMPL, sizeof(rtk_bt_a2dp_codec_t)},
	{RTK_BT_A2DP_EVT_STREAM_OPEN, 0},
	{RTK_BT_A2DP_EVT_STREAM_START_IND, sizeof(rtk_bt_a2dp_stream_start_t)},
	{RTK_BT_A2DP_EVT_STREAM_START_RSP, sizeof(rtk_bt_a2dp_stream_start_t)},
	{RTK_BT_A2DP_EVT_STREAM_STOP, sizeof(rtk_bt_a2dp_conn_ind_t)},
	{RTK_BT_A2DP_EVT_STREAM_CLOSE, sizeof(rtk_bt_a2dp_conn_ind_t)},
	{RTK_BT_A2DP_EVT_STREAM_DATA_IND, sizeof(rtk_bt_a2dp_stream_data_t)},
	{RTK_BT_A2DP_EVT_MAX, 0}
};

uint32_t get_bt_a2dp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_a2dp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_a2dp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_a2dp_evt_mem_op_tab[i].evt == RTK_BT_A2DP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_a2dp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_avrcp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_avrcp_evt_mem_op_tab[] = {
	{RTK_BT_AVRCP_EVT_CONN_IND, sizeof(rtk_bt_avrcp_conn_ind_t)}, 
	{RTK_BT_AVRCP_EVT_VOLUME_CHANGED, sizeof(rtk_bt_avrcp_volume_t)}, 
	{RTK_BT_AVRCP_EVT_VOLUME_UP, 0}, 
	{RTK_BT_AVRCP_EVT_VOLUME_DOWN, 0}, 
	{RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED, 0}, 
	{RTK_BT_AVRCP_EVT_CONN_CMPL, 0}, 
	{RTK_BT_AVRCP_EVT_PLAY_STATUS_CHANGED, sizeof(rtk_bt_avrcp_sub_event_t)}, 
	{RTK_BT_AVRCP_EVT_MAX, 0}
};

uint32_t get_bt_avrcp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_avrcp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_avrcp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_avrcp_evt_mem_op_tab[i].evt == RTK_BT_AVRCP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_avrcp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_sdp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_sdp_evt_mem_op_tab[] = {
	{RTK_BT_SDP_EVT_MAX, 0}
};

uint32_t get_bt_sdp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_sdp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_sdp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_sdp_evt_mem_op_tab[i].evt == RTK_BT_SDP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_sdp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}
#endif