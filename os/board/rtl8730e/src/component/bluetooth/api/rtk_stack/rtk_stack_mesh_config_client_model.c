#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdio.h>
#include <stdint.h>
#include <osif.h>

#include <rtk_bt_common.h>
#include <rtk_bt_def.h>

#include <mesh_api.h>

#include <rtk_bt_mesh_config_client_model.h>
#include <rtk_bt_mesh_def.h>

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
uint8_t rtk_bt_mesh_config_client_model_indicate_event(uint32_t event_code, void *pdata, uint32_t len)
{
	rtk_bt_evt_t *p_evt = NULL;
	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_CONFIG_CLIENT_MODEL, event_code, len);
	if (len) {
		if (event_code == RTK_BT_MESH_CONFIG_MODEL_COMPO_DATA_STAT) {
			memcpy(p_evt->data, pdata, 10);
			rtk_bt_mesh_cfg_compo_data_stat_call_t *data_test = (rtk_bt_mesh_cfg_compo_data_stat_call_t *) pdata;
			memcpy((uint8_t *)(p_evt->data) + 10, data_test->data, len - 10);
		} else {
			memcpy(p_evt->data, pdata, len);
		}
	}
	return rtk_bt_evt_indicate(p_evt, NULL);
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_config_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_CONFIG_MODEL_ADD_APP_KEY: {
		rtk_bt_mesh_config_client_add_app_key_t *add_app_key = (rtk_bt_mesh_config_client_add_app_key_t *)p_cmd->param;
		ret = cfg_app_key_add(add_app_key->unicast_addr, mesh_node.net_key_list[add_app_key->net_key_index].net_key_index_g,
							  mesh_node.app_key_list[add_app_key->app_key_index].app_key_index_g,
							  mesh_node.app_key_list[add_app_key->app_key_index].papp_key[key_state_to_new_loop(mesh_node.app_key_list[add_app_key->app_key_index].key_state)]->app_key);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_BIND: {
		rtk_bt_mesh_config_client_model_app_bind_t *model_app_bind = (rtk_bt_mesh_config_client_model_app_bind_t *)p_cmd->param;
		ret = cfg_model_app_bind(model_app_bind->dst, model_app_bind->element_addr, mesh_node.app_key_list[model_app_bind->app_key_index].app_key_index_g,
								 model_app_bind->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_ADD: {
		rtk_bt_mesh_config_client_model_sub_add_t *model_sub_add = (rtk_bt_mesh_config_client_model_sub_add_t *)p_cmd->param;
		ret = cfg_model_sub_add(model_sub_add->dst, model_sub_add->element_addr, model_sub_add->va_flag, (uint8_t *) & (model_sub_add->group_addr),
								model_sub_add->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_DEFAULT_TTL_GET: {
		rtk_bt_mesh_config_client_model_default_ttl_get_t *ttl_get = (rtk_bt_mesh_config_client_model_default_ttl_get_t *)p_cmd->param;
		ret = cfg_default_ttl_get(ttl_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_DEFAULT_TTL_SET: {
		rtk_bt_mesh_config_client_model_default_ttl_set_t *ttl_set = (rtk_bt_mesh_config_client_model_default_ttl_set_t *)p_cmd->param;
		ret = cfg_default_ttl_set(ttl_set->dst, ttl_set->ttl);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_RELAY_GET: {
		rtk_bt_mesh_config_client_model_relay_get_t *relay_get = (rtk_bt_mesh_config_client_model_relay_get_t *)p_cmd->param;
		ret = cfg_relay_get(relay_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_RELAY_SET: {
		rtk_bt_mesh_config_client_model_relay_set_t *relay_set = (rtk_bt_mesh_config_client_model_relay_set_t *)p_cmd->param;
		ret = cfg_relay_set(relay_set->dst, relay_set->state, relay_set->count, relay_set->steps);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NET_TRANSMIT_GET: {
		rtk_bt_mesh_config_client_model_net_transmit_get_t *net_transmit_get;
		net_transmit_get = (rtk_bt_mesh_config_client_model_net_transmit_get_t *)p_cmd->param;
		ret = cfg_net_transmit_get(net_transmit_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NET_TRANSMIT_SET: {
		rtk_bt_mesh_config_client_model_net_transmit_set_t *net_transmit_set;
		net_transmit_set = (rtk_bt_mesh_config_client_model_net_transmit_set_t *)p_cmd->param;
		ret = cfg_net_transmit_set(net_transmit_set->dst, net_transmit_set->count, net_transmit_set->steps);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NODE_RESET: {
		rtk_bt_mesh_config_client_model_node_reset_t *node_reset;
		node_reset = (rtk_bt_mesh_config_client_model_node_reset_t *)p_cmd->param;
		ret = cfg_node_reset(node_reset->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_COMPO_DATA_GET: {
		rtk_bt_mesh_config_client_model_compo_data_get_t *compo_data_get;
		compo_data_get = (rtk_bt_mesh_config_client_model_compo_data_get_t *)p_cmd->param;
		ret = cfg_compo_data_get(compo_data_get->dst, compo_data_get->page);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_BEACON_GET: {
		rtk_bt_mesh_config_client_model_beacon_get_t *beacon_get;
		beacon_get = (rtk_bt_mesh_config_client_model_beacon_get_t *)p_cmd->param;
		ret = cfg_beacon_get(beacon_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_BEACON_SET: {
		rtk_bt_mesh_config_client_model_beacon_set_t *beacon_set;
		beacon_set = (rtk_bt_mesh_config_client_model_beacon_set_t *)p_cmd->param;
		ret = cfg_beacon_set(beacon_set->dst, beacon_set->state);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_PROXY_GET: {
		rtk_bt_mesh_config_client_model_proxy_get_t *proxy_get;
		proxy_get = (rtk_bt_mesh_config_client_model_proxy_get_t *)p_cmd->param;
		ret = cfg_proxy_get(proxy_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_PROXY_SET: {
		rtk_bt_mesh_config_client_model_proxy_set_t *proxy_set;
		proxy_set = (rtk_bt_mesh_config_client_model_proxy_set_t *)p_cmd->param;
		ret = cfg_proxy_set(proxy_set->dst, proxy_set->state);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_PUB_GET: {
		rtk_bt_mesh_config_client_model_model_pub_get_t *pub_get;
		pub_get = (rtk_bt_mesh_config_client_model_model_pub_get_t *)p_cmd->param;
		ret = cfg_model_pub_get(pub_get->dst, pub_get->element_addr, pub_get->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_PUB_SET: {
		rtk_bt_mesh_config_client_model_model_pub_set_t *pub_set;
		pub_set = (rtk_bt_mesh_config_client_model_model_pub_set_t *)p_cmd->param;
		pub_key_info_t pub_key_info;
		memcpy(&pub_key_info, &(pub_set->pub_key_info), sizeof(pub_key_info_t));
		pub_period_t pub_period;
		memcpy(&pub_period, &(pub_set->pub_period), sizeof(pub_period_t));
		pub_retrans_info_t pub_retrans_info;
		memcpy(&pub_retrans_info, &(pub_set->pub_retrans_info), sizeof(pub_retrans_info_t));
		ret = cfg_model_pub_set(pub_set->dst, pub_set->element_addr, pub_set->va_flag, /*pub_set->pub_addr*/(uint8_t *) & (pub_set->pub_addr),
								pub_key_info,
								pub_set->pub_ttl, pub_period, pub_retrans_info, pub_set->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_DELETE: {
		rtk_bt_mesh_config_client_model_model_sub_delete_t *sub_delete;
		sub_delete = (rtk_bt_mesh_config_client_model_model_sub_delete_t *)p_cmd->param;
		ret = cfg_model_sub_delete(sub_delete->dst, sub_delete->element_addr, sub_delete->va_flag, /*sub_delete->group_addr*/(uint8_t *) & (sub_delete->group_addr),
								   sub_delete->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_DELETE_ALL: {
		rtk_bt_mesh_config_client_model_model_sub_delete_all_t *sub_delete_all;
		sub_delete_all = (rtk_bt_mesh_config_client_model_model_sub_delete_all_t *)p_cmd->param;
		ret = cfg_model_sub_delete_all(sub_delete_all->dst, sub_delete_all->element_addr, sub_delete_all->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_OVERWRITE: {
		rtk_bt_mesh_config_client_model_model_sub_overwrite_t *sub_overwrite;
		sub_overwrite = (rtk_bt_mesh_config_client_model_model_sub_overwrite_t *)p_cmd->param;
		ret = cfg_model_sub_overwrite(sub_overwrite->dst, sub_overwrite->element_addr,
									  sub_overwrite->va_flag, /*sub_overwrite->group_addr*/(uint8_t *) & (sub_overwrite->group_addr), sub_overwrite->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_GET: {
		rtk_bt_mesh_config_client_model_model_sub_get_t *sub_get;
		sub_get = (rtk_bt_mesh_config_client_model_model_sub_get_t *)p_cmd->param;
		ret = cfg_model_sub_get(sub_get->dst, sub_get->element_addr, sub_get->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NET_KEY_ADD: {
		rtk_bt_mesh_config_client_model_net_key_add_t *net_key_add;
		net_key_add = (rtk_bt_mesh_config_client_model_net_key_add_t *)p_cmd->param;
		ret = cfg_net_key_add(net_key_add->dst,
							  mesh_node.net_key_list[net_key_add->net_key_index].net_key_index_g,
							  mesh_node.net_key_list[net_key_add->net_key_index].pnet_key[key_state_to_new_loop(mesh_node.net_key_list[net_key_add->net_key_index].key_state)]->net_key);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NET_KEY_UPDATE: {
		rtk_bt_mesh_config_client_model_net_key_update_t *net_key_update;
		net_key_update = (rtk_bt_mesh_config_client_model_net_key_update_t *)p_cmd->param;
		ret = cfg_net_key_update(net_key_update->dst,
								 mesh_node.net_key_list[net_key_update->net_key_index].net_key_index_g,
								 net_key_update->net_key);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NET_KEY_DELETE: {
		rtk_bt_mesh_config_client_model_net_key_delete_t *net_key_delete;
		net_key_delete = (rtk_bt_mesh_config_client_model_net_key_delete_t *)p_cmd->param;
		ret = cfg_net_key_delete(net_key_delete->dst, mesh_node.net_key_list[net_key_delete->net_key_index].net_key_index_g);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NET_KEY_GET: {
		rtk_bt_mesh_config_client_model_net_key_get_t *net_key_get;
		net_key_get = (rtk_bt_mesh_config_client_model_net_key_get_t *)p_cmd->param;
		ret = cfg_net_key_get(net_key_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_APP_KEY_UPDATE: {
		rtk_bt_mesh_config_client_model_app_key_update_t *app_key_update;
		app_key_update = (rtk_bt_mesh_config_client_model_app_key_update_t *)p_cmd->param;
		ret = cfg_app_key_update(app_key_update->dst,
								 mesh_node.net_key_list[app_key_update->net_key_index].net_key_index_g,
								 mesh_node.app_key_list[app_key_update->app_key_index].app_key_index_g,
								 app_key_update->app_key);
		printf("ret:%d", ret);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_APP_KEY_DELETE: {
		rtk_bt_mesh_config_client_model_app_key_delete_t *app_key_delete;
		app_key_delete = (rtk_bt_mesh_config_client_model_app_key_delete_t *)p_cmd->param;
		ret = cfg_app_key_delete(app_key_delete->dst,
								 mesh_node.net_key_list[app_key_delete->net_key_index].net_key_index_g,
								 mesh_node.app_key_list[app_key_delete->app_key_index].app_key_index_g);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_APP_KEY_GET: {
		rtk_bt_mesh_config_client_model_app_key_get_t *app_key_get;
		app_key_get = (rtk_bt_mesh_config_client_model_app_key_get_t *)p_cmd->param;
		ret = cfg_app_key_get(app_key_get->dst, mesh_node.net_key_list[app_key_get->net_key_index].net_key_index_g);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NODE_IDENTITY_GET: {
		rtk_bt_mesh_config_client_model_node_identity_get_t *node_identity_get;
		node_identity_get = (rtk_bt_mesh_config_client_model_node_identity_get_t *)p_cmd->param;
		ret = cfg_node_identity_get(node_identity_get->dst, node_identity_get->net_key_index);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_NODE_IDENTITY_SET: {
		rtk_bt_mesh_config_client_model_node_identity_set_t *node_identity_set;
		node_identity_set = (rtk_bt_mesh_config_client_model_node_identity_set_t *)p_cmd->param;
		ret = cfg_node_identity_set(node_identity_set->dst, node_identity_set->net_key_index, node_identity_set->identity);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_UNBIND: {
		rtk_bt_mesh_config_client_model_model_app_unbind_t *model_app_unbind;
		model_app_unbind = (rtk_bt_mesh_config_client_model_model_app_unbind_t *)p_cmd->param;
		ret = cfg_model_app_unbind(model_app_unbind->dst, model_app_unbind->element_addr, model_app_unbind->app_key_index, model_app_unbind->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_GET: {
		rtk_bt_mesh_config_client_model_model_app_get_t *model_app_get;
		model_app_get = (rtk_bt_mesh_config_client_model_model_app_get_t *)p_cmd->param;
		ret = cfg_model_app_get(model_app_get->dst, model_app_get->element_addr, model_app_get->model_id);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_FRND_GET: {
		rtk_bt_mesh_config_client_model_frnd_get_t *frnd_get;
		frnd_get = (rtk_bt_mesh_config_client_model_frnd_get_t *)p_cmd->param;
		ret = cfg_frnd_get(frnd_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_FRND_SET: {
		rtk_bt_mesh_config_client_model_frnd_set_t *frnd_set;
		frnd_set = (rtk_bt_mesh_config_client_model_frnd_set_t *)p_cmd->param;
		ret = cfg_frnd_set(frnd_set->dst, frnd_set->state);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_LPN_POLL_TIMEOUT_GET: {
		rtk_bt_mesh_config_client_model_lpn_poll_timeout_get_t *lpn_poll_timeout_get;
		lpn_poll_timeout_get = (rtk_bt_mesh_config_client_model_lpn_poll_timeout_get_t *)p_cmd->param;
		ret = cfg_lpn_poll_timeout_get(lpn_poll_timeout_get->dst, lpn_poll_timeout_get->lpn_addr);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_KEY_REFRESH_PHASE_GET: {
		rtk_bt_mesh_config_client_model_key_refresh_phase_get_t *key_refresh_phase_get;
		key_refresh_phase_get = (rtk_bt_mesh_config_client_model_key_refresh_phase_get_t *)p_cmd->param;
		ret = cfg_key_refresh_phase_get(key_refresh_phase_get->dst,
										mesh_node.net_key_list[key_refresh_phase_get->net_key_index].net_key_index_g);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_KEY_REFRESH_PHASE_SET: {
		rtk_bt_mesh_config_client_model_key_refresh_phase_set_t *key_refresh_phase_set;
		key_refresh_phase_set = (rtk_bt_mesh_config_client_model_key_refresh_phase_set_t *)p_cmd->param;
		ret = cfg_key_refresh_phase_set(key_refresh_phase_set->dst, mesh_node.net_key_list[key_refresh_phase_set->net_key_index].net_key_index_g,
										key_refresh_phase_set->state);
		printf("ret:%d", ret);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_HB_PUB_GET: {
		rtk_bt_mesh_config_client_model_hb_pub_get_t *hb_pub_get;
		hb_pub_get = (rtk_bt_mesh_config_client_model_hb_pub_get_t *)p_cmd->param;
		ret = cfg_hb_pub_get(hb_pub_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_HB_PUB_SET: {
		rtk_bt_mesh_config_client_model_hb_pub_set_t *hb_pub_set;
		hb_pub_set = (rtk_bt_mesh_config_client_model_hb_pub_set_t *)p_cmd->param;
		hb_pub_features_t features;
		memcpy(&features, &(hb_pub_set->features), sizeof(hb_pub_features_t));
		ret = cfg_hb_pub_set(hb_pub_set->dst, hb_pub_set->dst_pub, hb_pub_set->count_log, hb_pub_set->period_log, hb_pub_set->ttl, features,
							 hb_pub_set->net_key_index);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_HB_SUB_GET: {
		rtk_bt_mesh_config_client_model_hb_sub_get_t *hb_sub_get;
		hb_sub_get = (rtk_bt_mesh_config_client_model_hb_sub_get_t *)p_cmd->param;
		ret = cfg_hb_sub_get(hb_sub_get->dst);
		break;
	}
	case RTK_BT_MESH_CONFIG_MODEL_HB_SUB_SET: {
		rtk_bt_mesh_config_client_model_hb_sub_set_t *hb_sub_set;
		hb_sub_set = (rtk_bt_mesh_config_client_model_hb_sub_set_t *)p_cmd->param;
		ret = cfg_hb_sub_set(hb_sub_set->dst, hb_sub_set->src, hb_sub_set->dst_set, hb_sub_set->period_log);
		break;
	}

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
#endif // end of RTK_BLE_MESH_PROVISIONER_SUPPORT
#endif // end of RTK_BLE_MESH_SUPPORT
