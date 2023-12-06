#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <provision_provisioner.h>
#include <platform_utils.h>
#include <health.h>
#include <mesh_cmd.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_stack_internal.h>
#include <rtk_stack_mesh_internal.h>
#include <rtk_bt_mesh_common.h>
#include <rtk_bt_mesh_generic_onoff_model.h>
#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_light_model.h>
#include <rtk_bt_mesh_time_model.h>
#include <rtk_bt_mesh_scheduler_model.h>
#include <rtk_bt_mesh_scene_model.h>
#include <rtk_bt_mesh_generic_default_transition_time.h>
#include <rtk_bt_mesh_generic_model.h>
#include <rtk_bt_mesh_remote_prov_model.h>

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
#include <provision_service.h>
#include <provision_client.h>
#include <proxy_client.h>
#include <remote_provisioning.h>
#endif

static bool dev_info_switch_flag = false;
static uint8_t rtk_bt_mesh_uuid_user[16] = {0};
extern bool rtk_ble_mesh_scan_enable_flag;
static mesh_role_t mesh_role = 0xff;

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
static bool prov_manual = false;
extern uint8_t proxy_client_conn_id;
#endif

/******************************************************************
 * @fn      generic_random_value_for_authentication
 * @brief   Generic a random value for oob in Authentication.
 *
 * @param   size  -  oob size(bytes in memory) for random value
 * @return  the random value
 */
static uint32_t generic_random_value_for_authentication(uint8_t size)
{
    uint8_t i;
    uint32_t max = 0;
	if(size > 4) {
		printf("[%s] size(%d) > 4, only generic 4 bytes random number", __func__, size);
		size = 4;
	}
    for(i=0; i<size; i++) {
        max = max*0xff + 0xff;
    }
    return platform_random(max);
}

/******************************************************************
 * @fn      store_data_use_big_endian
 * @brief   Store the random value use big endian for oob in Authentication.
 *
 * @param   num  -  the random number
 * @param   p_data  -  pointer for store random number
 * @param   size  -  len of p_data to store number
 * @return  bool
 */
static bool store_data_use_big_endian(uint32_t num, uint8_t *p_data, uint8_t size)
{
    uint8_t i,*p;
	if(size > sizeof(uint32_t)) {
		printf("[%s] The size %d exceed max capability %d, fail\r\n", __func__, size, sizeof(uint32_t));
		return false;
	}
    p = (uint8_t *)&num;
    for(i=0; i<size; i++)
    {
        p_data[i] = p[size-i-1];
    }
	return true;
}

/******************************************************************
 * @fn      prov_cb
 * @brief   Provisioning callbacks are handled in this function.
 *
 * @param   cb_data  -  @ref prov_cb_data_t
 * @return  the operation result
 */
static bool prov_cb(prov_cb_type_t cb_type, prov_cb_data_t cb_data)
{
	switch (cb_type) {
	case PROV_CB_TYPE_PROV: {
		/* Notify stack ready */
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_stack_evt_provisioned_device_t *provisioned_dev;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_PROVISIONED_DEVICE, sizeof(rtk_bt_mesh_stack_evt_provisioned_device_t));
		provisioned_dev = (rtk_bt_mesh_stack_evt_provisioned_device_t *)p_evt->data;
		provisioned_dev->unicast_addr = mesh_node.unicast_addr;
		rtk_bt_evt_indicate(p_evt, NULL);
#endif
		break;
	}
	case PROV_CB_TYPE_PB_ADV_LINK_STATE: {
		rtk_bt_evt_t *p_evt = NULL;
		prov_generic_cb_type_t *pb_adv_link_state;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_PB_ADV_LINK_STATE, sizeof(prov_generic_cb_type_t));
		pb_adv_link_state = (prov_generic_cb_type_t *)p_evt->data;
		*pb_adv_link_state = cb_data.pb_generic_cb_type;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case PROV_CB_TYPE_COMPLETE: {
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_stack_evt_prov_complete_t *prov_complete;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_PROV_COMPLETE, sizeof(rtk_bt_mesh_stack_evt_prov_complete_t));
		prov_complete = (rtk_bt_mesh_stack_evt_prov_complete_t *)p_evt->data;
		prov_complete->unicast_addr = cb_data.pprov_data->unicast_address;
		rtk_bt_evt_indicate(p_evt, NULL);
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
		if (MESH_ROLE_PROVISIONER == mesh_role) {
			/* the spec requires to disconnect, but you can remove it as you like! :) */
#if defined(BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL) && BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL
			if (rmt_prov_client_link_state() == RTK_BT_MESH_RMT_PROV_LINK_STATE_OUTBOUND_PKT_TRANS)
			{
				rmt_prov_link_close(RTK_BT_MESH_RMT_PROV_LINK_CLOSE_SUCCESS);
			}
			else 
#endif
			{
				prov_disconnect(PB_ADV_LINK_CLOSE_SUCCESS);
			}
		}
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
		if (MESH_ROLE_DEVICE == mesh_role) {
			mesh_node.iv_timer_count = MESH_IV_INDEX_48W;
		}
#endif
	}
	break;
	case PROV_CB_TYPE_FAIL: {
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_stack_evt_prov_fail_t *prov_fail;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_PROV_FAIL, sizeof(rtk_bt_mesh_stack_evt_prov_fail_t));
		prov_fail = (rtk_bt_mesh_stack_evt_prov_fail_t *)p_evt->data;
		prov_fail->fail_reason = (rtk_bt_mesh_stack_prov_cb_fail_type_t)cb_data.prov_fail.fail_type;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	case PROV_CB_TYPE_PATH_CHOOSE: {
		if (prov_manual) {
			/* use cmd "authpath" to select oob/no oob public key and no oob/static oob/input oob/output oob auth data according to the device capabilities */
			prov_capabilities_p pprov_capabilities = cb_data.pprov_capabilities;
			// printf("prov capabilities: en-%d al-%d pk-%d so-%d os-%d oa-%d is-%d ia-%d\r\n",
			// 	   pprov_capabilities->element_num, pprov_capabilities->algorithm,
			// 	   pprov_capabilities->public_key, pprov_capabilities->static_oob,
			// 	   pprov_capabilities->output_oob_size, pprov_capabilities->output_oob_action,
			// 	   pprov_capabilities->input_oob_size, pprov_capabilities->input_oob_action);
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_NOTIFY_CAPABILITY_OF_UDB, sizeof(rtk_bt_mesh_stack_prov_capabilities_t));
			memcpy(p_evt->data, pprov_capabilities, sizeof(rtk_bt_mesh_stack_prov_capabilities_t));
			rtk_bt_evt_indicate(p_evt, NULL);
			prov_manual = false;
		} else {
			/* select no oob public key and no oob auth data as default provision method */
			prov_start_t prov_start;
			memset(&prov_start, 0, sizeof(prov_start_t));
			prov_path_choose(&prov_start);
		}
	}
	break;
#if MESH_PROV_WO_AUTH_VALUE
	case PROV_CB_TYPE_CONF_CHECK: {
		/* check confirmation value when using OOB */
		uint8_t rand[16] = {0};
		uint8_t confirmation[16] = {0};
		prov_check_conf_t prov_check_conf = cb_data.prov_check_conf;

		memcpy((void *)rand, (void *)prov_check_conf.rand, sizeof(rand));
		memcpy((void *)confirmation, (void *)prov_check_conf.conf, sizeof(confirmation));
		printf("confimation %02x %02x %02x %02x %02x %02x \r\n", confirmation[0], confirmation[1], confirmation[2], confirmation[3], confirmation[4], confirmation[5]);
		printf("rand %02x %02x %02x %02x %02x %02x \r\n", rand[0], rand[1], rand[2], rand[3], rand[4], rand[5]);

		/* if confirmation from device is correct invoke prov_send_prov_data() */
		/* if confirmation from device is wrong invoke prov_reject() */
		// if (success) {
		//     prov_send_prov_data();
		// } else {
		//     prov_reject();
		// }
	}
	break;
#endif
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	case PROV_CB_TYPE_UNPROV: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_UNPROVISIONED_DEVICE, 0);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case PROV_CB_TYPE_START: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_DEVICE_START_PROVED, 0);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
#endif
	case PROV_CB_TYPE_PUBLIC_KEY: {
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
		printf("[%s] Get the public key from the device\r\n", __func__);
		uint8_t public_key[64] = {0xf4, 0x65, 0xe4, 0x3f, 0xf2, 0x3d, 0x3f, 0x1b, 0x9d, 0xc7, 0xdf, 0xc0, 0x4d, 0xa8, 0x75, 0x81, 0x84, 0xdb, 0xc9, 0x66, 0x20, 0x47, 0x96, 0xec, 0xcf, 0x0d, 0x6c, 0xf5, 0xe1, 0x65, 0x00, 0xcc, 0x02, 0x01, 0xd0, 0x48, 0xbc, 0xbb, 0xd8, 0x99, 0xee, 0xef, 0xc4, 0x24, 0x16, 0x4e, 0x33, 0xc2, 0x01, 0xc2, 0xb0, 0x10, 0xca, 0x6b, 0x4d, 0x43, 0xa8, 0xa1, 0x55, 0xca, 0xd8, 0xec, 0xb2, 0x79};
		prov_device_public_key_set(public_key);
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
		printf("[%s] Set the public key\r\n", __func__);
		uint8_t public_key[64] = {0xf4, 0x65, 0xe4, 0x3f, 0xf2, 0x3d, 0x3f, 0x1b, 0x9d, 0xc7, 0xdf, 0xc0, 0x4d, 0xa8, 0x75, 0x81, 0x84, 0xdb, 0xc9, 0x66, 0x20, 0x47, 0x96, 0xec, 0xcf, 0x0d, 0x6c, 0xf5, 0xe1, 0x65, 0x00, 0xcc, 0x02, 0x01, 0xd0, 0x48, 0xbc, 0xbb, 0xd8, 0x99, 0xee, 0xef, 0xc4, 0x24, 0x16, 0x4e, 0x33, 0xc2, 0x01, 0xc2, 0xb0, 0x10, 0xca, 0x6b, 0x4d, 0x43, 0xa8, 0xa1, 0x55, 0xca, 0xd8, 0xec, 0xb2, 0x79};
		uint8_t private_key[32] = {0x52, 0x9a, 0xa0, 0x67, 0x0d, 0x72, 0xcd, 0x64, 0x97, 0x50, 0x2e, 0xd4, 0x73, 0x50, 0x2b, 0x03, 0x7e, 0x88, 0x03, 0xb5, 0xc6, 0x08, 0x29, 0xa5, 0xa3, 0xca, 0xa2, 0x19, 0x50, 0x55, 0x30, 0xba};
		prov_params_set(PROV_PARAMS_PUBLIC_KEY, public_key, sizeof(public_key));
		prov_params_set(PROV_PARAMS_PRIVATE_KEY, private_key, sizeof(private_key));
#endif
	}
	break;
	// For Authentication process of provisioning when using oob method
	case PROV_CB_TYPE_AUTH_DATA: {
		prov_start_p pprov_start = cb_data.pprov_start;
		// prov_auth_value_type_t prov_auth_value_type = prov_auth_value_type_get(pprov_start);
		// /* use cmd to set auth data */
		// printf("auth method=%d[nsoi] action=%d size=%d type=%d[nbNa]\r\n>",
		// 	   pprov_start->auth_method,
		// 	   pprov_start->auth_action, pprov_start->auth_size, prov_auth_value_type);
		uint8_t auth_data[16] = STATIC_OOB_VALUE_FOR_AUTHENTICATION;
		switch (pprov_start->auth_method) {
		case PROV_AUTH_METHOD_STATIC_OOB:
		{
			rtk_bt_evt_t *p_evt = NULL;
			rtk_bt_mesh_stack_set_auth_value_for_static_oob *static_oob;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_SET_AUTH_VALUE_FOR_STATIC_OOB, sizeof(rtk_bt_mesh_stack_set_auth_value_for_static_oob));
			static_oob = (rtk_bt_mesh_stack_set_auth_value_for_static_oob *)p_evt->data;
			if(!prov_auth_value_set(auth_data, sizeof(auth_data))) {
				static_oob->status = false;
			}
			else{
				static_oob->status = true;
				memcpy(static_oob->data, auth_data, 16);
			}
			rtk_bt_evt_indicate(p_evt, NULL);
			break;
		}
		case PROV_AUTH_METHOD_OUTPUT_OOB:
			switch (pprov_start->auth_action.output_oob_action) {
				case PROV_START_OUTPUT_OOB_ACTION_OUTPUT_NUMERIC:
				{
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
					rtk_bt_evt_t *p_evt = NULL;
					p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_NOTIFY_FOR_OUTPUT_OOB_VALUE, 0);
					rtk_bt_evt_indicate(p_evt, NULL);
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
					uint32_t random;
					rtk_bt_evt_t *p_evt = NULL;
					rtk_bt_mesh_stack_set_auth_value_for_oob_data *oob_data;
					p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_SET_AUTH_VALUE_FOR_OUTPUT_OOB, sizeof(rtk_bt_mesh_stack_set_auth_value_for_oob_data));
					oob_data = (rtk_bt_mesh_stack_set_auth_value_for_oob_data *)p_evt->data;
					random = generic_random_value_for_authentication(pprov_start->auth_size.oob_size);
					store_data_use_big_endian(random, auth_data, pprov_start->auth_size.oob_size);
					if(!prov_auth_value_set(auth_data, pprov_start->auth_size.oob_size)) {
						oob_data->status = false;
					}
					else {
						oob_data->status = true;
						oob_data->random = random;
					}
					rtk_bt_evt_indicate(p_evt, NULL);
#endif
					break;
				}
				default:
					printf("[%s] pprov_start->auth_action.output_oob_action:%d can not support\r\n", __func__, pprov_start->auth_action.output_oob_action);
					break;
			}
			//prov_auth_value_set(auth_data, pprov_start->auth_size.output_oob_size);
			// APP_PRINT_INFO2("prov_cb: Please input the oob data provided by the device, output size = %d, action = %d",
			//                 pprov_start->auth_size.output_oob_size, pprov_start->auth_action.output_oob_action);
			break;
		case PROV_AUTH_METHOD_INPUT_OOB:
			switch (pprov_start->auth_action.input_oob_action) {
				case PROV_START_INPUT_OOB_ACTION_INPUT_NUMERIC:
				{
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
					uint32_t random;
					rtk_bt_evt_t *p_evt = NULL;
					rtk_bt_mesh_stack_set_auth_value_for_oob_data *oob_data;
					p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_SET_AUTH_VALUE_FOR_INPUT_OOB, sizeof(rtk_bt_mesh_stack_set_auth_value_for_oob_data));
					oob_data = (rtk_bt_mesh_stack_set_auth_value_for_oob_data *)p_evt->data;
					random = generic_random_value_for_authentication(pprov_start->auth_size.oob_size);
					store_data_use_big_endian(random, auth_data, pprov_start->auth_size.oob_size);
					if(!prov_auth_value_set(auth_data, pprov_start->auth_size.oob_size)) {
						oob_data->status = false;
					}
					else {
						oob_data->status = true;
						oob_data->random = random;
					}
					rtk_bt_evt_indicate(p_evt, NULL);
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
					rtk_bt_evt_t *p_evt = NULL;
					p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_NOTIFY_FOR_INPUT_OOB_VALUE, 0);
					rtk_bt_evt_indicate(p_evt, NULL);
#endif
					break;
				}
				default:
					printf("[%s] pprov_start->auth_action.input_oob_action:%d can not support\r\n", __func__, pprov_start->auth_action.input_oob_action);
					break;
			}
			//prov_auth_value_set(auth_data, pprov_start->auth_size.input_oob_size);
			// APP_PRINT_INFO2("prov_cb: Please output the oob data to the device, input size = %d, action = %d",
			//                 pprov_start->auth_size.input_oob_size, pprov_start->auth_action.input_oob_action);
			break;
		default:
			break;
		}
	}
	break;
	case PROV_CB_TYPE_RANDOM: {
		// Notified when receive remote random value
		break;
	}
	default:
		printf("[%s] Unknown cb_type:%d\r\n", __func__, cb_type);
		break;
	}
	return true;
}

static void device_info_cb(uint8_t bt_addr[6], uint8_t bt_addr_type, int8_t rssi, device_info_t *pinfo)
{
	if (!dev_info_switch_flag) {
		return;
	}
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_mesh_stack_device_info_common_data *common_data;
	switch (pinfo->type) {
	case DEVICE_INFO_UDB:
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_DEVICE_INFO_UDB_DISPLAY, sizeof(rtk_bt_mesh_stack_evt_dev_info_udb_t));
		rtk_bt_mesh_stack_evt_dev_info_udb_t *device_info_udb;
		device_info_udb = (rtk_bt_mesh_stack_evt_dev_info_udb_t *)p_evt->data;
		memcpy(device_info_udb->dev_uuid, pinfo->pbeacon_udb->dev_uuid, 16);
		break;
	case DEVICE_INFO_PROV_ADV:
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_DEVICE_INFO_PROV_DISPLAY, sizeof(rtk_bt_mesh_stack_evt_dev_info_provision_adv_t));
		rtk_bt_mesh_stack_evt_dev_info_provision_adv_t *device_info_prov;
		device_info_prov = (rtk_bt_mesh_stack_evt_dev_info_provision_adv_t *)p_evt->data;
		memcpy(device_info_prov->dev_uuid, pinfo->pservice_data->provision.dev_uuid, 16);
		break;
	case DEVICE_INFO_PROXY_ADV:
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_DEVICE_INFO_PROXY_DISPLAY, sizeof(rtk_bt_mesh_stack_evt_dev_info_proxy_adv_t));
		rtk_bt_mesh_stack_evt_dev_info_proxy_adv_t *device_info_proxy;
		device_info_proxy = (rtk_bt_mesh_stack_evt_dev_info_proxy_adv_t *)p_evt->data;
		device_info_proxy->len = pinfo->len;
		memcpy((uint8_t *)&device_info_proxy->proxy, (uint8_t *)&pinfo->pservice_data->proxy, device_info_proxy->len);
		break;
	default:
		printf("[%s] Unknown adv type:%d\r\n", __func__, pinfo->type);
		return ;
		break;
	}
	common_data = (rtk_bt_mesh_stack_device_info_common_data *)p_evt->data;
	memcpy(common_data->bt_addr, bt_addr, 6);
	common_data->bt_addr_type = bt_addr_type;
	common_data->rssi = rssi;
	rtk_bt_evt_indicate(p_evt, NULL);

}

static void hb_cb(hb_data_type_t type, void *pargs)
{
	rtk_bt_evt_t *p_evt = NULL;
    switch (type)
    {
    case HB_DATA_PUB_TIMER_STATE:
        {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_HB_PUB_TIMER_STATE, sizeof(rtk_bt_mesh_stack_hb_data_timer_state_t));
			memcpy(p_evt->data, pargs, sizeof(rtk_bt_mesh_stack_hb_data_timer_state_t));
			rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case HB_DATA_SUB_TIMER_STATE:
        {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_HB_SUB_TIMER_STATE, sizeof(rtk_bt_mesh_stack_hb_data_timer_state_t));
			memcpy(p_evt->data, pargs, sizeof(rtk_bt_mesh_stack_hb_data_timer_state_t));
			rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case HB_DATA_PUB_COUNT_UPDATE:
        {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_HB_PUB_COUNT_UPDATE, sizeof(rtk_bt_mesh_stack_hb_pub_count_update_t));
			memcpy(p_evt->data, pargs, sizeof(rtk_bt_mesh_stack_hb_pub_count_update_t));
			rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case HB_DATA_SUB_PERIOD_UPDATE:
        {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_HB_SUB_PERIOD_UPDATE, sizeof(rtk_bt_mesh_stack_hb_sub_period_update_t));
			memcpy(p_evt->data, pargs, sizeof(rtk_bt_mesh_stack_hb_sub_period_update_t));
			rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case HB_DATA_SUB_RECEIVE:
        {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_HB_SUB_RECEIVE, sizeof(rtk_bt_mesh_stack_hb_data_sub_receive_t));
			memcpy(p_evt->data, pargs, sizeof(rtk_bt_mesh_stack_hb_data_sub_receive_t));
			rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
		printf("[%s] Unknown type:%d\r\n", __func__, type);
        break;
    }
}

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
T_APP_RESULT bt_stack_mesh_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
	(void)conn_id;
	T_APP_RESULT  result = APP_RESULT_SUCCESS;
	if (client_id == prov_client_id) {
		prov_client_cb_data_t *pcb_data = (prov_client_cb_data_t *)p_data;
		rtk_bt_mesh_stack_prov_dis_result dis_res;
		rtk_bt_mesh_stack_evt_prov_dis_t *dis_event = NULL;
		switch (pcb_data->cb_type) {
		case PROV_CLIENT_CB_TYPE_DISC_STATE:
			switch (pcb_data->cb_content.disc_state) {
			case PROV_DISC_DONE:
				/* Discovery Simple BLE service procedure successfully done. */
				dis_res = RTK_BT_MESH_PROV_DISC_DONE;
				break;
			case PROV_DISC_FAIL:
				/* Discovery Request failed. */
				dis_res = RTK_BT_MESH_PROV_DISC_FAIL;
				break;
			case PROV_DISC_NOT_FOUND:
				/* Discovery Request failed. */
				dis_res = RTK_BT_MESH_PROV_DISC_NOT_FOUND;
				break;
			default:
				break;
			}
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_PROV_SERVICE_DIS_RESULT, sizeof(rtk_bt_mesh_stack_evt_prov_dis_t));
			dis_event = (rtk_bt_mesh_stack_evt_prov_dis_t *)p_evt->data;
			dis_event->dis_result = dis_res;
			rtk_bt_evt_indicate(p_evt, NULL);
			break;
		case PROV_CLIENT_CB_TYPE_READ_RESULT:
			break;
		case PROV_CLIENT_CB_TYPE_WRITE_RESULT:
			break;
		default:
			break;
		}
	} else if (client_id == proxy_client_id) {
		proxy_client_cb_data_t *pcb_data = (proxy_client_cb_data_t *)p_data;
		rtk_bt_mesh_stack_proxy_dis_result dis_res;
		rtk_bt_mesh_stack_evt_proxy_dis_t *dis_event = NULL;
		switch (pcb_data->cb_type) {
		case PROXY_CLIENT_CB_TYPE_DISC_STATE:
			switch (pcb_data->cb_content.disc_state) {
			case PROXY_DISC_DONE:
				/* Discovery Simple BLE service procedure successfully done. */
				dis_res = RTK_BT_MESH_PROXY_DISC_DONE;
				break;
			case PROXY_DISC_FAIL:
				/* Discovery Request failed. */
				dis_res = RTK_BT_MESH_PROXY_DISC_FAIL;
				break;
			case PROXY_DISC_NOT_FOUND:
				/* Discovery Request failed. */
				dis_res = RTK_BT_MESH_PROXY_DISC_NOT_FOUND;
				break;
			default:
				break;
			}
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_PROXY_SERVICE_DIS_RESULT, sizeof(rtk_bt_mesh_stack_evt_proxy_dis_t));
			dis_event = (rtk_bt_mesh_stack_evt_proxy_dis_t *)p_evt->data;
			dis_event->dis_result = dis_res;
			rtk_bt_evt_indicate(p_evt, NULL);
			break;
		case PROXY_CLIENT_CB_TYPE_READ_RESULT:
			break;
		case PROXY_CLIENT_CB_TYPE_WRITE_RESULT:
			break;
		default:
			break;
		}
	}
	return result;
}

static void client_models_init(void)
{
	cfg_client_reg();
	health_client_model_init();
#if defined(BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL) && BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL
	remote_prov_client_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL
	generic_on_off_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL
	generic_level_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL
	generic_default_transition_time_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL
	generic_power_on_off_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL
	generic_power_level_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL
	generic_battery_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL
	generic_location_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL
	generic_property_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_SENSOR_CLIENT_MODEL) && BT_MESH_ENABLE_SENSOR_CLIENT_MODEL
	sensor_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_TIME_CLIENT_MODEL) && BT_MESH_ENABLE_TIME_CLIENT_MODEL
	time_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_SCENE_CLIENT_MODEL) && BT_MESH_ENABLE_SCENE_CLIENT_MODEL
	scene_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL) && BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL
	scheduler_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL
	light_lightness_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL
	light_ctl_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL
	light_hsl_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL
	light_xyl_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL
	light_lc_client_model_init();
#endif
#if defined(BT_MESH_ENABLE_DATATRANS_MODEL) && BT_MESH_ENABLE_DATATRANS_MODEL
	datatrans_model_init();
#endif
}
#endif

#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
static void server_models_init(void)
{
	health_server_model_init();
#if defined(BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL
	generic_on_off_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL
	generic_level_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL
	generic_default_transition_time_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL
	generic_power_on_off_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL
	generic_power_on_off_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL
	generic_power_level_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL
	generic_power_level_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL
	generic_battery_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL
	generic_location_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL
	generic_location_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL
	generic_admin_property_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL
	generic_manu_property_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL
	generic_user_property_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL
	generic_client_property_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_SENSOR_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SERVER_MODEL
	sensor_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL
	sensor_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_TIME_SERVER_MODEL) && BT_MESH_ENABLE_TIME_SERVER_MODEL
	time_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL
	time_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_SCENE_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SERVER_MODEL
	scene_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL
	scene_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL) && BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL
	scheduler_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL
	scheduler_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL
	light_lightness_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL
	light_lightness_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL
	light_ctl_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_CTL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_SETUP_SERVER_MODEL
	light_ctl_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL
	light_ctl_temperature_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL
	light_hsl_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_HSL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SETUP_SERVER_MODEL
	light_hsl_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL
	light_hsl_hue_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL
	light_hsl_saturation_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL
	light_xyl_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_XYL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_SETUP_SERVER_MODEL
	light_xyl_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL
	light_lc_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL
	light_lc_setup_server_model_init();
#endif
#if defined(BT_MESH_ENABLE_DATATRANS_MODEL) && BT_MESH_ENABLE_DATATRANS_MODEL
	datatrans_model_init();
#endif
}
#endif

static void rtk_bt_mesh_gap_init(void)
{
	uint16_t scan_window = 0x100; /* 160ms */
	uint16_t scan_interval = 0x120; /* 180ms */
	gap_sched_params_set(GAP_SCHED_PARAMS_INTERWAVE_SCAN_WINDOW, &scan_window, sizeof(scan_window));
	gap_sched_params_set(GAP_SCHED_PARAMS_INTERWAVE_SCAN_INTERVAL, &scan_interval, sizeof(scan_interval));
	gap_sched_params_set(GAP_SCHED_PARAMS_SCAN_WINDOW, &scan_window, sizeof(scan_window));
	gap_sched_params_set(GAP_SCHED_PARAMS_SCAN_INTERVAL, &scan_interval, sizeof(scan_interval));

	uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
	char *dev_name = NULL;
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	if (MESH_ROLE_PROVISIONER == mesh_role) {
		dev_name = "Mesh Provisioner";
	}
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	if (MESH_ROLE_DEVICE == mesh_role) {
		dev_name = "Mesh Device";
	}
#endif
	gap_sched_params_set(GAP_SCHED_PARAMS_DEVICE_NAME, dev_name, GAP_DEVICE_NAME_LEN);
	gap_sched_params_set(GAP_SCHED_PARAMS_APPEARANCE, &appearance, sizeof(appearance));
}

extern void proxy_server_support_prov_on_proxy(bool);
static void rtk_bt_mesh_stack_init(void *data)
{
	rtk_bt_mesh_app_conf_t *mesh_app_conf = (rtk_bt_mesh_app_conf_t *)data;

	/** set mesh stack log level, default all on, disable the log of level LEVEL_TRACE */
	uint32_t module_bitmap[MESH_LOG_LEVEL_SIZE] = {0};
	diag_level_set(TRACE_LEVEL_TRACE, module_bitmap);

	/** mesh stack needs rand seed */
	plt_srand(platform_random(0xffffffff));

#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	if (MESH_ROLE_DEVICE == mesh_role) {
		/** configure provisioning parameters */
		prov_capabilities_t prov_capabilities = {
			.algorithm = PROV_CAP_ALGO_FIPS_P256_ELLIPTIC_CURVE,
			.public_key = 0,
			.static_oob = PROV_SUPPORT_STATIC_OOB,
			.output_oob_size = PROV_SUPPORT_OUTPUT_OOB_SIZE,
			.output_oob_action = PROV_SUPPORT_OUTPUT_OOB_ACTION,
			.input_oob_size = PROV_SUPPORT_INPUT_OOB_SIZE,
			.input_oob_action = PROV_SUPPORT_INPUT_OOB_ACTION
		};
		prov_params_set(PROV_PARAMS_CAPABILITIES, &prov_capabilities, sizeof(prov_capabilities_t));
	}
#endif
	/** configure provisioning parameters */
	prov_params_set(PROV_PARAMS_CALLBACK_FUN, (void *)prov_cb, sizeof(prov_cb_pf));

	/** config node parameters */
	mesh_node_features_t features = {
		.role = mesh_role,
		.relay = BT_MESH_CONFIG_RELAY_IS_ENABL,
		.proxy = 1,
		.fn = 1,
		.prov = 1,
		.snb = 1,
		.bg_scan = 1,
		.flash = 1,
		.flash_rpl = 1
	};

	mesh_node_cfg_t node_cfg = {
		.net_key_num = 10,
		.app_key_num = 3,
		.vir_addr_num = 3,
		.rpl_num = 20,
		.sub_addr_num = 5,
		.proxy_num = 1,
		.proxy_interval = 5
	};
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	if (MESH_ROLE_PROVISIONER == mesh_role) {
		features.lpn = 2;
		features.udb = 0;
		if (mesh_app_conf->bt_mesh_dev_key_num) {
			// Use the user device key num
			node_cfg.dev_key_num = mesh_app_conf->bt_mesh_dev_key_num;
		} else {
			// Use the default device key num
			node_cfg.dev_key_num = 20;
		}
	}
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	if (MESH_ROLE_DEVICE == mesh_role) {
		features.lpn = 1;
		features.udb = 1;
		node_cfg.dev_key_num = 2;
		node_cfg.prov_interval = 2;
		node_cfg.udb_interval = 2;
	}
#endif

	if (mesh_app_conf->bt_mesh_rpl_num) {
		// Set user rpl num
		node_cfg.rpl_num = mesh_app_conf->bt_mesh_rpl_num;
	}

	mesh_node_cfg(features, &node_cfg);
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	if (MESH_ROLE_DEVICE == mesh_role) {
		// Enable proxy service support Provisioning PDU for nrf mesh APP
		proxy_server_support_prov_on_proxy(true);
	}
#endif

	// Set default mesh retrans params
	mesh_node.net_trans_count = BT_MESH_CONFIG_NET_TRANS_COUNTS;
	mesh_node.net_trans_steps = BT_MESH_CONFIG_NET_TRANS_STEPS;
	mesh_node.relay_retrans_count = BT_MESH_CONFIG_RELAY_RETRANS_COUNTS;
	mesh_node.relay_retrans_steps = BT_MESH_CONFIG_RELAY_RETRANS_STEPS;
	mesh_node.trans_retrans_count = BT_MESH_CONFIG_TRANS_RETRANS_COUNTS;
	mesh_node.ttl = BT_MESH_CONFIG_MSG_TTL;

	// Set user mesh retrans params
	if (mesh_app_conf->trans_retrans_count) {
		mesh_node.trans_retrans_count = mesh_app_conf->trans_retrans_count;
	}
	if (mesh_app_conf->net_trans_count) {
		mesh_node.net_trans_count = mesh_app_conf->net_trans_count;
	}
	if (mesh_app_conf->net_trans_interval_step) {
		mesh_node.net_trans_steps = mesh_app_conf->net_trans_interval_step;
	}
	if (mesh_app_conf->relay_retrans_count) {
		mesh_node.relay_retrans_count = mesh_app_conf->relay_retrans_count;
	}
	if (mesh_app_conf->relay_retrans_interval_step) {
		mesh_node.relay_retrans_steps = mesh_app_conf->relay_retrans_interval_step;
	}
	if (mesh_app_conf->ttl) {
		mesh_node.ttl = mesh_app_conf->ttl;
	}

	// Set mesh FTL size for user,if not,use the default 1800 bytes
	if (mesh_app_conf->bt_mesh_flash_size) {
		mesh_node.flash_size = mesh_app_conf->bt_mesh_flash_size;
	}

	/** create elements and register models */
	mesh_element_create(GATT_NS_DESC_UNKNOWN);
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	if (MESH_ROLE_PROVISIONER == mesh_role) {
		client_models_init();
	}
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	if (MESH_ROLE_DEVICE == mesh_role) {
		server_models_init();
	}
#endif
	compo_data_page0_header_t compo_data_page0_header = {COMPANY_ID, PRODUCT_ID, VERSION_ID};
	compo_data_page0_gen(&compo_data_page0_header);

	/** init mesh stack */
	mesh_init();
	device_info_cb_reg(device_info_cb);
    hb_init(hb_cb);
}

void rtk_bt_mesh_set_device_uuid(void)
{
	uint8_t i, flag = 0;
	for (i = 0; i < 16; i++) {
		if (rtk_bt_mesh_uuid_user[i] != 0) {
			flag = 1;
			break;
		}
	}
	if (flag) {
		device_uuid_set(rtk_bt_mesh_uuid_user);
	} else {
		uint8_t bt_addr[6];
		uint8_t dev_uuid[16] = MESH_DEVICE_UUID;
		gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
		memcpy(dev_uuid, bt_addr, sizeof(bt_addr));
		device_uuid_set(dev_uuid);
	}
}

static uint8_t rtk_bt_mesh_save_info(void *data)
{
	rtk_bt_mesh_app_conf_t *mesh_app_conf = (rtk_bt_mesh_app_conf_t *)data;
	memcpy(rtk_bt_mesh_uuid_user, mesh_app_conf->bt_mesh_uuid, 16);
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	if (RTK_BT_MESH_ROLE_PROVISIONER == mesh_app_conf->bt_mesh_role) {
		mesh_role = MESH_ROLE_PROVISIONER;
	}
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	if (RTK_BT_MESH_ROLE_DEVICE == mesh_app_conf->bt_mesh_role) {
		mesh_role = MESH_ROLE_DEVICE;
	}
#endif
	if ((RTK_BT_MESH_ROLE_PROVISIONER != mesh_app_conf->bt_mesh_role) && (RTK_BT_MESH_ROLE_DEVICE != mesh_app_conf->bt_mesh_role)) {
		printf("[%s] Unkown mesh role:%d\r\n", __func__, mesh_app_conf->bt_mesh_role);
		return RTK_BT_MESH_STACK_API_FAIL;
	}
	return 0;
}

/* =============================================== bt mesh one shot ADV releate API functions ======================================= */
static rtk_bt_mesh_stack_act_send_adv_t default_adv;
static uint8_t is_advertising_flag = 0;
static void *mesh_one_shot_adv_timer_handle = NULL;

static uint16_t rtk_stack_send_one_shot_adv(rtk_bt_mesh_stack_act_send_adv_t *adv_param);
void ble_mesh_handle_io_msg(T_IO_MSG *io_msg)
{
	uint16_t subtype = io_msg->subtype;
	switch (subtype){
		case RTK_BT_MESH_IO_MSG_SUBTYPE_ADV:
			if (is_advertising_flag) {
				rtk_stack_send_one_shot_adv(&default_adv);
			}
			break;
		default:
			break;
	}
}

extern uint16_t bt_stack_msg_send(uint16_t type, uint16_t subtype, void *msg);
static void one_shot_adv_timer_func(void *param)
{
	(void)param;
	if (is_advertising_flag) {
		bt_stack_msg_send(IO_MSG_TYPE_LE_MESH, RTK_BT_MESH_IO_MSG_SUBTYPE_ADV, NULL);
	}
}

static bool rtk_bt_mesh_one_shot_adv_init(void)
{
	is_advertising_flag = 0;
	memset(&default_adv, 0, sizeof(rtk_bt_mesh_stack_act_send_adv_t));
	return osif_timer_create(&mesh_one_shot_adv_timer_handle, "mesh one shot timer", 0, 0xffffffff, 1, one_shot_adv_timer_func);
}

static bool rtk_bt_mesh_one_shot_adv_deinit(void)
{
	is_advertising_flag = 0;
	memset(&default_adv, 0, sizeof(rtk_bt_mesh_stack_act_send_adv_t));
	if (!osif_timer_delete(&mesh_one_shot_adv_timer_handle)) {
		printf("[%s] Delete one shot adv timer fail\r\n", __func__);
		return false;
	}
	mesh_one_shot_adv_timer_handle = NULL;
	return true;
}

/* =============================================== bt mesh stack releate API functions ======================================= */
static void rtk_stack_set_device_info_display(rkt_bt_mesh_stack_act_device_info_set_t *p_data)
{
	if (p_data->dev_info_dis) {
		dev_info_switch_flag = true;
	} else {
		dev_info_switch_flag = false;
	}
}

uint8_t rtk_bt_mesh_stack_set_scan_switch(bool scan_switch)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_mesh_stack_evt_scan_switch_t *p_scan_switch;
	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_SCAN_SWITCH,
								sizeof(rtk_bt_mesh_stack_evt_scan_switch_t));
	p_scan_switch = (rtk_bt_mesh_stack_evt_scan_switch_t *)p_evt->data;
	p_scan_switch->scan_enable = scan_switch;
	rtk_bt_evt_indicate(p_evt, NULL);
	rtk_ble_mesh_scan_enable_flag = scan_switch;
	return RTK_BT_MESH_STACK_API_SUCCESS;
}

uint8_t rtk_bt_mesh_stack_set_adv_data(uint8_t *p_data, uint32_t len)
{
	memcpy(default_adv.adv_raw_data, p_data, len);
	default_adv.adv_data_len = len;
	return RTK_BT_MESH_STACK_API_SUCCESS;
}

uint8_t rtk_bt_mesh_stack_start_adv(rtk_bt_le_adv_param_t *adv_param)
{
	uint16_t ret;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_mesh_stack_evt_start_adv_t *p_start_adv;
	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_START_ADV,
								sizeof(rtk_bt_mesh_stack_evt_start_adv_t));
	p_start_adv = (rtk_bt_mesh_stack_evt_start_adv_t *)p_evt->data;
	default_adv.adv_type = adv_param->type;
	if(adv_param->own_addr_type > RTK_BT_LE_ADDR_TYPE_RANDOM) {
		printf("[%s] Addr type %d for mesh adv is not support, use the default addr type.\r\n", __func__, default_adv.addr_type);
	}
	else {
		default_adv.addr_type = adv_param->own_addr_type;
	}
	is_advertising_flag = 1;
	if (osif_timer_restart(&mesh_one_shot_adv_timer_handle, ((adv_param->interval_min + adv_param->interval_max) * 625) / 2 / 1000)) {
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		is_advertising_flag = 0;
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	p_start_adv->result = ret;
	rtk_bt_evt_indicate(p_evt, NULL);
	return ret;
}

uint8_t rtk_bt_mesh_stack_stop_adv(void)
{
	uint16_t ret;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_mesh_stack_evt_stop_adv_t *p_stop_adv;
	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_STOP_ADV,
								sizeof(rtk_bt_mesh_stack_evt_stop_adv_t));
	p_stop_adv = (rtk_bt_mesh_stack_evt_stop_adv_t *)p_evt->data;
	is_advertising_flag = 0;
	if (osif_timer_stop(&mesh_one_shot_adv_timer_handle)) {
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		printf("[%s] Stop one shot adv timer fail\r\n", __func__);
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	p_stop_adv->result = ret;
	rtk_bt_evt_indicate(p_evt, NULL);
	return ret;
}

static uint16_t rtk_stack_send_one_shot_adv(rtk_bt_mesh_stack_act_send_adv_t *adv_param)
{
	uint8_t *padv_data = gap_sched_task_get();
	if (NULL == padv_data) {
		printf("[%s] Allocate padv_data fail ! \n\r", __func__);
		return RTK_BT_MESH_STACK_API_FAIL;
	}
	memcpy(padv_data, adv_param->adv_raw_data, adv_param->adv_data_len);

	gap_sched_task_p ptask = CONTAINER_OF(padv_data, gap_sched_task_t, adv_data);
	ptask->adv_len += adv_param->adv_data_len;
	ptask->adv_type = (gap_sched_adv_type_t)adv_param->adv_type;
	switch (adv_param->addr_type) {
		case RTK_BT_LE_ADDR_TYPE_PUBLIC:
			ptask->adv_addr_type = GAP_SCHED_ADV_ADDR_TYPE_PUBLIC;
			break;
		case RTK_BT_LE_ADDR_TYPE_RANDOM:
			ptask->adv_addr_type = GAP_SCHED_ADV_ADDR_TYPE_RANDOM;
			break;
		default:
			break;
	}
	gap_sched_try(ptask);
	return RTK_BT_MESH_STACK_API_SUCCESS;
}

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
static uint8_t output_oob_size_of_udb = 0;
#endif
static uint16_t rtk_stack_set_random_value_for_authentication(rtk_bt_mesh_stack_act_set_random_value_t *value)
{
	uint8_t oob_size,auth_data[16]={0};
	uint32_t random;
	random = value->random;
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	oob_size = output_oob_size_of_udb;
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	oob_size = PROV_SUPPORT_INPUT_OOB_SIZE;
#endif
	store_data_use_big_endian(random, auth_data, oob_size);
	if(prov_auth_value_set(auth_data, oob_size)) {
		return RTK_BT_MESH_STACK_API_SUCCESS;
	}
	else {
		return RTK_BT_MESH_STACK_API_FAIL;
	}
}

static uint16_t rtk_stack_get_heartbeat_subscribe_result(rtk_bt_mesh_hb_sub_t *hb_sub)
{
    *(hb_sub_t *)hb_sub = hb_subscription_get();
	return RTK_BT_MESH_STACK_API_SUCCESS;
}

static uint16_t rtk_stack_set_model_subscribe(rtk_bt_mesh_set_model_subscribe_t *model_sub)
{
    mesh_element_p p_element = mesh_element_get(model_sub->element_index);
    mesh_model_p pmodel = mesh_model_get_by_model_id(p_element, model_sub->model_id);
    if (pmodel == NULL || MESH_NOT_SUBSCRIBE_ADDR(model_sub->sub_addr))
    {
		printf("[%s] Can not get model info or sub addr(0x%x) is not correct!\r\n", __func__, model_sub->sub_addr);
        return RTK_BT_MESH_STACK_API_FAIL;
    }
    else
    {
        if(mesh_model_sub(pmodel, model_sub->sub_addr)) {
            return RTK_BT_MESH_STACK_API_SUCCESS;
        }
		else {
			printf("[%s] Set model subscribe fail!\r\n", __func__);
			return RTK_BT_MESH_STACK_API_FAIL;
		}
    }
}

#if defined(RTK_BLE_MESH_FN_SUPPORT) && RTK_BLE_MESH_FN_SUPPORT
static void friendship_fn_callback(uint8_t frnd_index, fn_cb_type_t type, uint16_t lpn_addr)
{
	(void)frnd_index;
	rtk_bt_mesh_stack_evt_fn_t *fn_cb;
	rtk_bt_evt_t *p_evt = NULL;
	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_FN_CB, sizeof(rtk_bt_mesh_stack_evt_fn_t));
	fn_cb = (rtk_bt_mesh_stack_evt_fn_t *)p_evt->data;
	fn_cb->lpn_addr = lpn_addr;
	switch (type) {
	case FN_CB_TYPE_ESTABLISHING:
		fn_cb->cb_type = RTK_BT_MESH_FN_CB_TYPE_ESTABLISHING;
		break;
	case FN_CB_TYPE_ESTABLISH_FAIL_NO_POLL:
		fn_cb->cb_type = RTK_BT_MESH_FN_CB_TYPE_ESTABLISH_FAIL_NO_POLL;
		break;
	case FN_CB_TYPE_ESTABLISH_SUCCESS:
		fn_cb->cb_type = RTK_BT_MESH_FN_CB_TYPE_ESTABLISH_SUCCESS;
		break;
	case FN_CB_TYPE_FRND_LOST:
		fn_cb->cb_type = RTK_BT_MESH_FN_CB_TYPE_FRND_LOST;
		break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, type);
		break;
	}
	rtk_bt_evt_indicate(p_evt, NULL);
}

static bool rtk_stack_fn_init(rtk_bt_mesh_stack_act_fn_init_t *friend_init)
{
	fn_params_t fn_params;
	fn_params.queue_size = friend_init->queue_size;
	mesh_node.frnd_rx_window = friend_init->rx_window;
	return fn_init(friend_init->lpn_num, &fn_params, friendship_fn_callback);
}

static void rtk_stack_fn_deinit(void)
{
	fn_deinit();
}
#endif // end of RTK_BLE_MESH_FN_SUPPORT

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
static void rtk_stack_provisioner_init_setting(rtk_bt_mesh_stack_act_provisioner_init_setting_t *p_data)
{
	uint16_t net_key_index;
	mesh_node.node_state = PROV_NODE;
	mesh_node.unicast_addr = p_data->unicast_addr;
	net_key_index = net_key_add(0, p_data->net_key);
	app_key_add(net_key_index, 0, p_data->app_key);
	mesh_model_bind_all_key();
}

static uint16_t rtk_stack_pb_adv_con(rtk_bt_mesh_stack_act_pb_adv_con_t *pbadvcon)
{
	uint16_t ret;
	if (pb_adv_link_open(0, pbadvcon->uuid)) {
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	return ret;
}

static uint16_t rtk_stack_pb_adv_discon(void)
{
	uint16_t ret;
	if (pb_adv_link_close(0, PB_ADV_LINK_CLOSE_SUCCESS)) {
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	return ret;
}

static uint16_t rtk_stack_start_provisioning(rtk_bt_mesh_stack_act_start_prov_t *start_prov)
{
	uint16_t ret;
	prov_manual = start_prov->prov_manual;  // Use the provisioning method whitch device supported
	if (prov_invite(start_prov->attn_dur)) {
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	return ret;
}

static uint16_t rtk_stack_method_choose_for_prov(rtk_bt_mesh_stack_prov_start_t *method)
{
	prov_start_t prov_start;
	memcpy(&prov_start, method, sizeof(prov_start_t));
	if(RTK_BT_MESH_PROV_AUTH_METHOD_OUTPUT_OOB==method->auth_method && \
		RTK_BT_MESH_PROV_START_OUTPUT_OOB_ACTION_OUTPUT_NUMERIC==method->auth_action.oob_action) {
		output_oob_size_of_udb = method->auth_size.oob_size;
	}
	if(prov_path_choose(&prov_start)) {
		return RTK_BT_MESH_STACK_API_SUCCESS;
	}
	else {
		return RTK_BT_MESH_STACK_API_FAIL;
	}
}

static uint16_t rtk_stack_prov_service_discovery(rtk_bt_mesh_stack_act_prov_dis_t *prov_dis)
{
	uint16_t ret;
	uint8_t conn_id;
	if(bt_stack_le_gap_get_conn_id(prov_dis->conn_handle, &conn_id)) {
		printf("[%s] Get connection id fail\r\n", __func__);
		return RTK_BT_MESH_STACK_API_FAIL;
	}
	if (prov_client_start_discovery(conn_id)) {
		prov_client_conn_id = conn_id;  // Save the conn id for use of notify of prov service later
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	return ret;
}

static uint16_t rtk_stack_prov_service_set_notify(rtk_bt_mesh_stack_act_prov_set_notify_t *prov_notify)
{
	uint16_t ret;
	if (!prov_client_data_out_cccd_set(prov_client_conn_id, prov_notify->is_enable)) {
		ret = RTK_BT_MESH_STACK_API_FAIL;
		goto end;
	}
	if (!proxy_ctx_set_link(prov_proxy_ctx_id, prov_client_conn_id)) {
		ret = RTK_BT_MESH_STACK_API_FAIL;
		goto end;
	}
	ret = RTK_BT_MESH_STACK_API_SUCCESS;
end:
	return ret;
}

static uint16_t rtk_stack_proxy_service_discovery(rtk_bt_mesh_stack_act_proxy_dis_t *proxy_dis)
{
	uint16_t ret;
	uint8_t conn_id;
	if(bt_stack_le_gap_get_conn_id(proxy_dis->conn_handle, &conn_id)) {
		printf("[%s] Get connection id fail\r\n", __func__);
		return RTK_BT_MESH_STACK_API_FAIL;
	}
	if (proxy_client_start_discovery(conn_id)) {
		proxy_client_conn_id = conn_id;  // Save the conn id for use of notify of proxy service later
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	return ret;
}

static uint16_t rtk_stack_proxy_service_set_notify(rtk_bt_mesh_stack_act_proxy_set_notify_t *proxy_notify)
{
	uint16_t ret;
	if (proxy_client_data_out_cccd_set(proxy_client_conn_id, proxy_notify->is_enable)) {
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
	} else {
		ret = RTK_BT_MESH_STACK_API_FAIL;
	}
	return ret;
}

#endif // end of RTK_BLE_MESH_PROVISIONER_SUPPORT

#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
#if defined(RTK_BLE_MESH_LPN_SUPPORT) && RTK_BLE_MESH_LPN_SUPPORT

static uint8_t lpn_have_establish_friendship_flag = 0;
static void friendship_lpn_callback(uint8_t frnd_index, lpn_cb_type_t type, uint16_t fn_addr)
{
	(void)frnd_index;
	rtk_bt_mesh_stack_evt_lpn_t *lpn_cb;
	rtk_bt_evt_t *p_evt = NULL;
	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_STACK, RTK_BT_MESH_STACK_EVT_LPN_CB, sizeof(rtk_bt_mesh_stack_evt_lpn_t));
	lpn_cb = (rtk_bt_mesh_stack_evt_lpn_t *)p_evt->data;
	lpn_cb->fn_addr = fn_addr;
	switch (type) {
	case LPN_CB_TYPE_ESTABLISH_SUCCESS:
		lpn_cb->cb_type = RTK_BT_MESH_LPN_CB_TYPE_ESTABLISH_SUCCESS;
		// Have establish friendship, disable scan for low power.
		gap_sched_scan(false);
		// Disable service adv to avoid establish GATT link as proxy server
		// If LPN establish friendship as proxy server, it could not relay message for proxy client between every poll
		mesh_service_adv_stop();
		lpn_have_establish_friendship_flag = 1;
		break;
	case LPN_CB_TYPE_ESTABLISH_FAIL_NO_OFFER:
		lpn_cb->cb_type = RTK_BT_MESH_LPN_CB_TYPE_ESTABLISH_FAIL_NO_OFFER;
		break;
	case LPN_CB_TYPE_ESTABLISH_FAIL_NO_UPDATE:
		lpn_cb->cb_type = RTK_BT_MESH_LPN_CB_TYPE_ESTABLISH_FAIL_NO_UPDATE;
		break;
	case LPN_CB_TYPE_FRIENDSHIP_LOST:
		lpn_cb->cb_type = RTK_BT_MESH_LPN_CB_TYPE_FRIENDSHIP_LOST;
		if (lpn_have_establish_friendship_flag) {
			// Enable scan and service adv after friendship lost
			gap_sched_scan(true);
			mesh_service_adv_start();
			lpn_have_establish_friendship_flag = 0;
		}
		break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, type);
		lpn_cb->cb_type = RTK_BT_MESH_LPN_CB_TYPE_UNKNOWN;
		break;
	}
	rtk_bt_evt_indicate(p_evt, NULL);
}

static bool rtk_stack_lpn_init(rtk_bt_mesh_stack_act_lpn_init_t *lpn)
{
	return lpn_init(lpn->fn_num, friendship_lpn_callback);
}

static rtk_bt_mesh_stack_lpn_req_result_type rtk_stack_lpn_req(rtk_bt_mesh_stack_act_lpn_req_t *lpn)
{
	lpn_req_reason_t req_result;
	rtk_bt_mesh_stack_lpn_req_result_type rtk_req_result;
	lpn_req_params_t req_params = {50, 100, {1, 0, 0, 0}, NULL};
	uint8_t fn_index = lpn->fn_index;
	uint16_t net_key_index = lpn->net_key_index;
	if (le_get_active_link_num() != 0) {
		// If the device is proxy server, do not enable establish friendship as LPN role;
		// The LPN will disable scan between every poll request with FN node;
		// If LPN establish friendship as proxy server, it could not relay message for proxy client between every poll
		printf("[%s] Have BLE active link, cannot establish friendship\r\n", __func__);
		return RTK_BT_MESH_LPN_REQ_REASON_PROXY_SERVER_DO_NOT_SUPPORT_LPN;
	}
	mesh_node.frnd_rx_delay = lpn->frnd_rx_delay;
	mesh_node.frnd_rx_widen = lpn->frnd_rx_widen;
	req_params.poll_interval = lpn->poll_interval;
	req_params.poll_timeout = lpn->poll_timeout;
	req_result = lpn_req(fn_index, net_key_index, &req_params);
	switch (req_result) {
	case LPN_REQ_REASON_SUCCESS:
		rtk_req_result = RTK_BT_MESH_LPN_REQ_REASON_SUCCESS;
		break;
	case LPN_REQ_REASON_REQ_PARAMS_INVALID:
		rtk_req_result = RTK_BT_MESH_LPN_REQ_REASON_REQ_PARAMS_INVALID;
		break;
	case LPN_REQ_REASON_NET_KEY_INDEX_INVALID:
		rtk_req_result = RTK_BT_MESH_LPN_REQ_REASON_NET_KEY_INDEX_INVALID;
		break;
	case LPN_REQ_REASON_FRND_ON_SAME_NET_KEY_INDEX:
		rtk_req_result = RTK_BT_MESH_LPN_REQ_REASON_FRND_ON_SAME_NET_KEY_INDEX;
		break;
	case LPN_REQ_REASON_FRND_INDEX_INVALID:
		rtk_req_result = RTK_BT_MESH_LPN_REQ_REASON_FRND_INDEX_INVALID;
		break;
	case LPN_REQ_REASON_FRND_ALREADY_EXIST_OR_ESTABLISING:
		rtk_req_result = RTK_BT_MESH_LPN_REQ_REASON_FRND_ALREADY_EXIST_OR_ESTABLISING;
		break;
	case LPN_REQ_REASON_RESOURCE_INSUFFICIENT:
		rtk_req_result = RTK_BT_MESH_LPN_REQ_REASON_RESOURCE_INSUFFICIENT;
		break;
	default:
		printf("[%s] Unknown req result:%d\r\n", __func__, req_result);
		break;
	}
	return rtk_req_result;
}

static void rtk_stack_lpn_sub(rtk_bt_mesh_stack_act_lpn_sub_t *lpn_sub)
{
	frnd_sub_list_add_rm(lpn_sub->fn_index, &lpn_sub->addr, 1, lpn_sub->add_rm);
}

static bool rtk_stack_lpn_clear(rtk_bt_mesh_stack_act_lpn_clear_t *clear_friendship)
{
	return lpn_clear(clear_friendship->fn_index);
}

static void rtk_stack_lpn_deinit(void)
{
	lpn_deinit();
	if (lpn_have_establish_friendship_flag) {
		// Enable scan and service adv after deinit lpn
		gap_sched_scan(true);
		mesh_service_adv_start();
		lpn_have_establish_friendship_flag = 0;
	}
}

#endif // end of RTK_BLE_MESH_LPN_SUPPORT
#endif // end of RTK_BLE_MESH_DEVICE_SUPPORT

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_stack_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_STACK_API_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_STACK_ACT_NODE_RESET:
		mesh_node_reset();
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
		break;
	case RTK_BT_MESH_STACK_ACT_DEV_INFO_SWITCH:
		rtk_stack_set_device_info_display(p_cmd->param);
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
		break;
	case RTK_BT_MESH_STACK_ACT_USER_LIST_INFO:
		user_cmd_list(NULL);
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
		break;
	case RTK_BT_MESH_STACK_ACT_SEND_ONE_SHOT_ADV:
		ret = rtk_stack_send_one_shot_adv(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_SET_RANDOM_VALUE:
		ret = rtk_stack_set_random_value_for_authentication(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_GET_HB_SUB_RESULT:
		ret = rtk_stack_get_heartbeat_subscribe_result(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_SET_MODEL_SUB:
		ret = rtk_stack_set_model_subscribe(p_cmd->param);
		break;
#if defined(RTK_BLE_MESH_FN_SUPPORT) && RTK_BLE_MESH_FN_SUPPORT
	case RTK_BT_MESH_STACK_ACT_FN_INIT:
		if (rtk_stack_fn_init(p_cmd->param)) {
			ret = RTK_BT_MESH_STACK_API_SUCCESS;
		} else {
			ret = RTK_BT_MESH_STACK_API_FAIL;
		}
		break;
	case RTK_BT_MESH_STACK_ACT_FN_DEINIT:
		rtk_stack_fn_deinit();
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
		break;
#endif // end of RTK_BLE_MESH_FN_SUPPORT
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	case RTK_BT_MESH_STACK_ACT_PROVISIONER_INIT_SETTING:
		rtk_stack_provisioner_init_setting(p_cmd->param);
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
		break;
	case RTK_BT_MESH_STACK_ACT_PB_ADV_CON:
		ret = rtk_stack_pb_adv_con(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_PB_ADV_DISCON:
		ret = rtk_stack_pb_adv_discon();
		break;
	case RTK_BT_MESH_STACK_ACT_START_PROV:
		ret = rtk_stack_start_provisioning(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_METHOD_CHOOSE_FOR_PROV:
		ret = rtk_stack_method_choose_for_prov(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_PROV_SERVICE_DISCOVERY:
		ret = rtk_stack_prov_service_discovery(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_PROV_SERVICE_SET_NOTIFY:
		ret = rtk_stack_prov_service_set_notify(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_PROXY_SERVICE_DISCOVERY:
		ret = rtk_stack_proxy_service_discovery(p_cmd->param);
		break;
	case RTK_BT_MESH_STACK_ACT_PROXY_SERVICE_SET_NOTIFY:
		ret = rtk_stack_proxy_service_set_notify(p_cmd->param);
		break;
#endif // end of RTK_BLE_MESH_PROVISIONER_SUPPORT
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
#if defined(RTK_BLE_MESH_LPN_SUPPORT) && RTK_BLE_MESH_LPN_SUPPORT
	case RTK_BT_MESH_STACK_ACT_LPN_INIT:
		if (rtk_stack_lpn_init(p_cmd->param)) {
			ret = RTK_BT_MESH_STACK_API_SUCCESS;
		} else {
			ret = RTK_BT_MESH_STACK_API_FAIL;
		}
		break;
	case RTK_BT_MESH_STACK_ACT_LPN_REQ: {
		rtk_bt_mesh_stack_lpn_req_result_type req_result;
		req_result = rtk_stack_lpn_req(p_cmd->param);
		ret = req_result;
		break;
	}
	case RTK_BT_MESH_STACK_ACT_LPN_SUB: {
		rtk_stack_lpn_sub(p_cmd->param);
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
		break;
	}
	case RTK_BT_MESH_STACK_ACT_LPN_CLEAR: {
		if (rtk_stack_lpn_clear(p_cmd->param)) {
			ret = RTK_BT_MESH_STACK_API_SUCCESS;
		} else {
			ret = RTK_BT_MESH_STACK_API_FAIL;
		}
		break;
	}
	case RTK_BT_MESH_STACK_ACT_LPN_DEINIT:
		rtk_stack_lpn_deinit();
		ret = RTK_BT_MESH_STACK_API_SUCCESS;
		break;
#endif // end of RTK_BLE_MESH_LPN_SUPPORT
#endif // end of RTK_BLE_MESH_DEVICE_SUPPORT
	default:
		printf("[%s] Unknown p_cmd->act:%d\r\n", __func__, p_cmd->act);
		break;
	}
	ret = ret | RTK_BT_STACK_MESH_ERROR_FLAG;
end:
	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;
}

uint16_t bt_stack_mesh_init(void *data)
{
	rtk_bt_mesh_app_conf_t *mesh_app_conf = &((rtk_bt_app_conf_t *)data)->bt_mesh_app_conf;
	if (rtk_bt_mesh_save_info(mesh_app_conf)) {
		return RTK_BT_MESH_STACK_API_FAIL;
	}
	rtk_bt_mesh_gap_init();
	rtk_bt_mesh_stack_init(mesh_app_conf);
	rtk_bt_mesh_one_shot_adv_init();
	return RTK_BT_MESH_STACK_API_SUCCESS;
}

void bt_stack_mesh_deinit(void)
{
	mesh_deinit();
	memset(rtk_bt_mesh_uuid_user, 0, 16);
	rtk_bt_mesh_one_shot_adv_deinit();
}

#endif // end of RTK_BLE_MESH_SUPPORT
