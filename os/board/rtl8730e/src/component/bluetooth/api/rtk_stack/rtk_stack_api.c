/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_stack_config.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>
#include <rtk_stack_internal.h>
#include <gap.h>
#include <bte.h>
#include <gap_msg.h>
#include <gap_le.h>
#include <app_msg.h>
#include <gap_config.h>
#include <trace_app.h>
#include <bt_api_config.h>
#if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)
#include <rtk_bt_le_iso.h>
#endif
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
#include <rtk_bt_le_audio.h>
#include <ble_audio.h>
#endif
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
#include <mesh_common.h>
#include <rtk_bt_mesh_stack_internal.h>
#include <rtk_bt_mesh_api_stack.h>
#include <rtk_bt_mesh_api_config_client_model.h>
#include <rtk_bt_mesh_api_generic_onoff_model.h>
#include <rtk_bt_mesh_api_datatrans_model.h>
#include <rtk_bt_mesh_api_light_model.h>
#endif
static void *api_task_sem = NULL;
static void *api_task_hdl = NULL;
static void *api_task_io_msg_q = NULL;
static void *api_task_evt_msg_q = NULL;
static uint8_t api_task_running = 0;

static uint16_t bt_stack_act_handler(rtk_bt_cmd_t *p_cmd);

static void bt_stack_api_taskentry(void *ctx)
{
	(void)ctx;
	uint8_t event;
	T_IO_MSG io_msg;
	rtk_bt_cmd_t *api_cmd;

	api_task_running = 1;
	osif_sem_give(api_task_sem);

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
	osif_create_secure_context(configMINIMAL_SECURE_STACK_SIZE + 256);
#endif

	while (api_task_running) {
		if (true == osif_msg_recv(api_task_evt_msg_q, &event, BT_TIMEOUT_FOREVER)) {
			if (EVENT_GROUP_IO == EVENT_GROUP(event)) {
				if (EVENT_IO_TO_APP == event) {
					if (true == osif_msg_recv(api_task_io_msg_q, &io_msg, BT_TIMEOUT_NONE)) {
						switch (io_msg.type) {
						case IO_MSG_TYPE_API_SYS_CALL:
							api_cmd = (rtk_bt_cmd_t *)io_msg.u.buf;

							/* Check if need to exit task*/
							if (RTK_BT_API_TASK_EXIT == api_cmd->group) {
								api_task_running = 0;
								api_cmd->ret = 0;
								osif_sem_give(api_cmd->psem);
								continue;
							}
							bt_stack_act_handler((rtk_bt_cmd_t *)io_msg.u.buf);
							break;

						case IO_MSG_TYPE_BT_STATUS:
							bt_stack_le_gap_handle_io_msg(io_msg.subtype, (void *)&io_msg.u.param);
							break;
#if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)
						case IO_MSG_TYPE_LE_MGR:
							bt_stack_le_iso_handle_io_msg(&io_msg);
							break;
#endif
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
						case IO_MSG_TYPE_LE_AUDIO:
							ble_audio_handle_msg(&io_msg);
							break;
#endif
						default:
							break;
						}
					}
				}
			} else if (EVENT_GROUP_FRAMEWORK == EVENT_GROUP(event)) {
				rtk_stack_framework_event_handler(event);
			}
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
			else if (EVENT_MESH == event) {
				mesh_inner_msg_handle(event);
			}
#endif
			else {
				gap_handle_msg(event);
			}
		}
	}

	API_PRINT("[BT api task] bt api task exit\r\n");
	osif_sem_give(api_task_sem);
	osif_task_delete(NULL);
}

static uint16_t bt_stack_init(void *app_config)
{
	bool b_trace_init_ret = false;
	bool b_bte_init_ret = false;
	uint16_t b_bt_ble_gap_init_ret = 0;
	rtk_bt_app_conf_t *papp_conf = (rtk_bt_app_conf_t *)app_config;
	rtk_bt_app_conf_t default_conf = {0};

	//set default conf
	if (papp_conf != NULL) {
		default_conf.mtu_size = papp_conf->mtu_size;
		default_conf.master_init_mtu_req = papp_conf->master_init_mtu_req;
		default_conf.prefer_all_phy = papp_conf->prefer_all_phy;
		default_conf.prefer_tx_phy = papp_conf->prefer_tx_phy;
		default_conf.prefer_rx_phy = papp_conf->prefer_rx_phy;
		default_conf.max_tx_octets = papp_conf->max_tx_octets;
		default_conf.max_tx_time = papp_conf->max_tx_time;
#if defined(RTK_BLE_SET_TX_QUEUE_NUM) && RTK_BLE_SET_TX_QUEUE_NUM
		if (papp_conf->max_stack_tx_pending_num != 0) {
			default_conf.max_stack_tx_pending_num = papp_conf->max_stack_tx_pending_num;
		} else {
			default_conf.max_stack_tx_pending_num = RTK_BT_GATT_DEFAULT_CREDITS;
		}
#endif
#if RTK_BLE_PRIVACY_SUPPORT
		memcpy(default_conf.irk, papp_conf->irk, RTK_BT_LE_GAP_IRK_LEN);
#endif
	} else {
		default_conf.mtu_size = 180;
		default_conf.master_init_mtu_req = true;
		default_conf.prefer_all_phy = 0;
		default_conf.prefer_tx_phy = 1 | 1 << 1 | 1 << 2;
		default_conf.prefer_rx_phy = 1 | 1 << 1 | 1 << 2;
		default_conf.max_tx_octets = 0x40;
		default_conf.max_tx_time = 0x200;
#if defined(RTK_BLE_SET_TX_QUEUE_NUM) && RTK_BLE_SET_TX_QUEUE_NUM
		default_conf.max_stack_tx_pending_num = RTK_BT_GATT_DEFAULT_CREDITS;
#endif
#if RTK_BLE_PRIVACY_SUPPORT
		memset(default_conf.irk, 0, RTK_BT_LE_GAP_IRK_LEN);
#endif
	}

	//Trace uart init
	b_trace_init_ret = bt_trace_init();
	if (false == b_trace_init_ret) {
		goto failed;
	}

	/* gap_config_xxx() shall be before bte_init() */
	gap_config_max_le_link_num(GAP_MAX_LINKS);
	gap_config_max_le_paired_device(GAP_MAX_LINKS);
	gap_config_max_mtu_size(default_conf.mtu_size);
#if defined(RTK_BLE_SET_TX_QUEUE_NUM) && RTK_BLE_SET_TX_QUEUE_NUM
	if (false == gap_config_credits_num(default_conf.max_stack_tx_pending_num)) {
		printf("%s: gap_config_credits_num fail(max_stack_tx_pending_num is %d)\r\n", __func__, default_conf.max_stack_tx_pending_num);
	}
#endif

#if defined(F_BT_5_2_EATT_SUPPORT) && F_BT_5_2_EATT_SUPPORT
#if RTK_BT_5_2_EATT_SUPPORT
	gap_config_eatt(true);
	/* If BREDR uses EATT at the same, enlarge the paramter to include gatt channel of BREDR */
	gap_config_gatt_chann_num(GAP_MAX_LINKS + GAP_LE_MAX_ECFC_CHANN_NUM);
#else
	gap_config_eatt(false); /*clear GATTS_SERVER_SUPPORTED_FEATURES_EATT_BIT of GATT_UUID_CHAR_SERVER_SUPPORTED_FEATURES*/
#endif
#endif

#if RTK_BT_5_2_L2C_ECFC_SUPPORT && F_BT_5_2_L2C_ECFC_SUPPORT
	gap_config_le_l2c_chann_num(GAP_LE_MAX_ECFC_CHANN_NUM);
	gap_config_le_sec_entry_num(GAP_MAX_ECFC_PROTOCAL_NUM);
#endif

	//BT Stack init
	b_bte_init_ret = bte_init();
	if (false == b_bte_init_ret) {
		goto failed;
	}

	//GAP common initialization
	if (bt_stack_gap_init()) {
		goto failed;
	}

	//LE GAP initialization
	b_bt_ble_gap_init_ret = bt_stack_le_gap_init(&default_conf);
	if (b_bt_ble_gap_init_ret) {
		goto failed;
	}

	//BR/EDR GAP initialization
	if (RTK_BT_FAIL == bt_stack_br_gap_init(api_task_evt_msg_q)) {
		goto failed;
	}

	return 0;

failed:
	if (0 == b_bt_ble_gap_init_ret) {
		bt_stack_le_gap_deinit();
	}

	if (true == b_bte_init_ret) {
		bte_deinit();
	}

	if (true == b_trace_init_ret) {
		bt_trace_deinit();
	}

	return RTK_BT_FAIL;
}


static uint16_t bt_stack_deinit(void)
{
	T_GAP_DEV_STATE state;
	uint16_t ret = 0;

	le_get_gap_param(GAP_PARAM_DEV_STATE, &state);
	if (state.gap_init_state == GAP_INIT_STATE_STACK_READY) {
		bt_stack_le_gap_deinit();
		bte_deinit();
		bt_trace_deinit();
	}
	ret = bt_stack_br_gap_deinit();

	return ret;
}

static uint32_t bt_stack_app_profile_conf = 0;

bool bt_stack_profile_check(rtk_bt_profile_t profile)
{
	if (bt_stack_app_profile_conf & profile) {
		return true;
	}
	return false;
}

static uint16_t bt_stack_profile_init(void *app_conf)
{
	uint16_t ret = 0;
	rtk_bt_app_conf_t *papp_conf = (rtk_bt_app_conf_t *)app_conf;
	uint32_t  app_profile_support = 0;

	if (papp_conf == NULL) {
		app_profile_support = RTK_BT_PROFILE_GATTS | RTK_BT_PROFILE_GATTC;
	} else {
		app_profile_support = papp_conf->app_profile_support;
	}

	if (app_profile_support & RTK_BT_PROFILE_GATTS) {
		API_PRINT("GATTS Profile init  \r\n");
		ret = bt_stack_gatts_init(app_conf);
		if (ret) {
			return ret;
		}
	}
	if (app_profile_support & RTK_BT_PROFILE_GATTC) {
		ret = bt_stack_gattc_init(app_conf);
		if (ret) {
			return ret;
		}
	}
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
	if (app_profile_support & RTK_BT_PROFILE_MESH) {
		ret = bt_mesh_stack_init(papp_conf->bt_mesh_uuid);
		if (ret) {
			return ret;
		}
	}
#endif
	if (app_profile_support & RTK_BT_PROFILE_AVRCP) {
		ret = bt_stack_avrcp_init();
		if (ret) {
			return ret;
		}
	}
	if (app_profile_support & RTK_BT_PROFILE_A2DP) {
		ret = bt_stack_a2dp_init(papp_conf->a2dp_role);
		if (ret) {
			return ret;
		}
	}
	if (app_profile_support & RTK_BT_PROFILE_SPP) {
		ret = bt_stack_spp_init(papp_conf->spp_role);
		if (ret) {
			return ret;
		}
	}
	if (app_profile_support & RTK_BT_PROFILE_SDP) {
		ret = bt_stack_sdp_init();
		if (ret) {
			return ret;
		}
	}
	if (app_profile_support & RTK_BT_PROFILE_HFP) {
		ret = bt_stack_hfp_init(papp_conf->hfp_role);
		if (ret) {
			return ret;
		}
	}
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
	if (app_profile_support & RTK_BT_PROFILE_LEAUDIO) {
		ret = bt_stack_le_audio_init(papp_conf, api_task_io_msg_q, api_task_evt_msg_q);
		if (ret) {
			return ret;
		}
	}
#endif
	bt_stack_app_profile_conf = app_profile_support;

	return 0;
}

static uint16_t bt_stack_profile_deinit(void)
{
	uint32_t profile_conf = bt_stack_app_profile_conf;
	if (profile_conf & RTK_BT_PROFILE_GATTS) {
		bt_stack_gatts_deinit();
	}
	if (profile_conf & RTK_BT_PROFILE_GATTC) {
		bt_stack_gattc_deinit();
	}
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
	if (profile_conf & RTK_BT_PROFILE_MESH) {
		bt_mesh_stack_deinit();
	}
#endif
	if (profile_conf & RTK_BT_PROFILE_AVRCP) {
		bt_stack_avrcp_deinit();
	}
	if (profile_conf & RTK_BT_PROFILE_A2DP) {
		bt_stack_a2dp_deinit();
	}
	if (profile_conf & RTK_BT_PROFILE_SPP) {
		bt_stack_spp_deinit();
	}
	if (profile_conf & RTK_BT_PROFILE_HFP) {
		bt_stack_hfp_deinit();
	}
	if (profile_conf & RTK_BT_PROFILE_SDP) {
		bt_stack_sdp_deinit();
	}
	if (profile_conf & RTK_BT_PROFILE_LEAUDIO) {
		bt_stack_le_audio_deinit();
	}

	bt_stack_app_profile_conf = 0;

	return 0;
}

static uint16_t bt_stack_api_init(void)
{
	if (false == osif_sem_create(&api_task_sem, 0, 1)) {
		goto failed;
	}

	if (false == osif_msg_queue_create(&api_task_io_msg_q, API_TASK_IO_MSG_QUEUE_SIZE, sizeof(T_IO_MSG))) {
		goto failed;
	}

	if (false == osif_msg_queue_create(&api_task_evt_msg_q, API_TASK_EVT_MSG_QUEUE, sizeof(uint8_t))) {
		goto failed;
	}

	api_task_running = 0;
	if (false == osif_task_create(&api_task_hdl, "bt_api_task", bt_stack_api_taskentry, NULL,
								  API_TASK_STACK_SIZE, API_TASK_PRIORITY)) {
		goto failed;
	}

	/* Wait until event task is running */
	if (false == osif_sem_take(api_task_sem, BT_TIMEOUT_FOREVER)) {
		goto failed;
	}

	return 0;

failed:
	if (api_task_hdl) {
		osif_task_delete(api_task_hdl);
	}
	if (api_task_sem) {
		osif_sem_delete(api_task_sem);
	}
	if (api_task_io_msg_q) {
		osif_msg_queue_delete(api_task_io_msg_q);
	}
	if (api_task_evt_msg_q) {
		osif_msg_queue_delete(api_task_evt_msg_q);
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_api_deinit(void)
{
	uint16_t ret = 0;

	/* indicate bt api task to kill itself */
	ret = rtk_bt_send_cmd(RTK_BT_API_TASK_EXIT, 0, NULL, 0);
	if (ret) {
		return ret;
	}

	if (false == osif_sem_take(api_task_sem, BT_TIMEOUT_FOREVER)) {
		return RTK_BT_ERR_OS_OPERATION;
	}

	osif_sem_delete(api_task_sem);
	osif_msg_queue_delete(api_task_io_msg_q);
	osif_msg_queue_delete(api_task_evt_msg_q);
	api_task_sem = NULL;
	api_task_hdl = NULL;
	api_task_io_msg_q = NULL;
	api_task_evt_msg_q = NULL;

	return 0;
}

uint16_t bt_stack_act_handler(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;
	API_PRINT("bt_stack_act_handler: group = %d, act = %d \r\n", p_cmd->group, p_cmd->act);
	switch (p_cmd->group) {
	case RTK_BT_LE_GP_GAP:
		API_PRINT("RTK_BT_LE_GP_GAP group \r\n");
		bt_stack_le_gap_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_GATTS:
		API_PRINT("RTK_BT_LE_GP_GATTS group \r\n");
		bt_stack_gatts_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_GATTC:
		API_PRINT("RTK_BT_LE_GP_GATTC group \r\n");
		bt_stack_gattc_act_handle(p_cmd);
		break;
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
	case RTK_BT_LE_GP_MESH_STACK:
		API_PRINT("RTK_BT_LE_GP_MESH_STACK group");
		bt_mesh_stack_act_handle(p_cmd);
		break;
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	case RTK_BT_LE_GP_MESH_CONFIG_CLIENT_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_CONFIG_CLIENT_MODEL group");
		bt_mesh_config_client_model_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_MESH_GENERIC_ONOFF_CLIENT_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_GENERIC_ONOFF_CLIENT_MODEL group");
		bt_mesh_generic_onoff_client_model_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL group");
		bt_mesh_light_lightness_client_model_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL group");
		bt_mesh_light_ctl_client_model_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL group");
		bt_mesh_light_hsl_client_model_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL group");
		bt_mesh_light_xyl_client_model_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL group");
		bt_mesh_light_lc_client_model_act_handle(p_cmd);
		break;
#endif
	case RTK_BT_LE_GP_MESH_DATATRANS_MODEL:
		API_PRINT("RTK_BT_LE_GP_MESH_DATATRANS_MODEL group");
		bt_mesh_datatrans_model_act_handle(p_cmd);
		break;
#endif
	case RTK_BT_BR_GP_GAP:
		API_PRINT("RTK_BT_BR_GP_GAP group \r\n");
		bt_stack_br_gap_act_handle(p_cmd);
		break;
	case RTK_BT_BR_GP_AVRCP:
		API_PRINT("RTK_BT_BR_GP_GAP group \r\n");
		bt_stack_avrcp_act_handle(p_cmd);
		break;
	case RTK_BT_BR_GP_A2DP:
		API_PRINT("RTK_BT_BR_GP_GAP group \r\n");
		bt_stack_a2dp_act_handle(p_cmd);
		break;
	case RTK_BT_BR_GP_SPP:
		API_PRINT("RTK_BT_BR_GP_GAP group \r\n");
		bt_stack_spp_act_handle(p_cmd);
		break;
	case RTK_BT_BR_GP_HFP:
		API_PRINT("RTK_BT_BR_GP_GAP group \r\n");
		bt_stack_hfp_act_handle(p_cmd);
		break;
	case RTK_BT_BR_GP_SDP:
		API_PRINT("RTK_BT_BR_GP_GAP group \r\n");
		bt_stack_sdp_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_ISO:
		API_PRINT("RTK_BT_LE_GP_ISO group \r\n");
		bt_stack_le_iso_act_handle(p_cmd);
		break;
	case RTK_BT_LE_GP_AUDIO:
		API_PRINT("RTK_BT_LE_GP_AUDIO group \r\n");
		bt_stack_le_audio_act_handle(p_cmd);
		break;
	case RTK_BT_COMMON_GP_GAP:
		API_PRINT("RTK_BT_COMMON_GP_GAP group \r\n");
		bt_stack_gap_act_handle(p_cmd);
		break;
	default:
		API_PRINT("bt_stack_le_act_handle:unknown group: %d \r\n", p_cmd->group);
		break;
	}

	return ret;
}

static bool bt_stack_startup(void *app_conf)
{
	(void)app_conf;
	if (false == gap_start_bt_stack(api_task_evt_msg_q, api_task_io_msg_q, API_TASK_GAP_MSG_QUEUE_SIZE)) {
		return false;
	}
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
	if (((rtk_bt_app_conf_t *)app_conf)->app_profile_support & RTK_BT_PROFILE_MESH) {
		mesh_start(EVENT_MESH, EVENT_IO_TO_APP, api_task_evt_msg_q, api_task_io_msg_q);
	}
#endif
	return true;
}

uint16_t bt_stack_enable(void *app_conf)
{
	uint16_t ret = 0;

	//step 1 initialize APP task
	ret = bt_stack_api_init();
	if (ret) {
		return ret;
	}
	//step 2 initialize GAP
	ret = bt_stack_init(app_conf);
	if (ret) {
		return ret;
	}
	//step3 initialize profile
	ret = bt_stack_profile_init(app_conf);
	if (ret) {
		return ret;
	}
	//step 4 stack enable
	if (false == bt_stack_startup(app_conf)) {
		return RTK_BT_FAIL;
	}

	bt_stack_le_gap_wait_ready();
	bt_stack_br_gap_wait_ready();

	bt_stack_le_gap_get_init_tx_credits();

	return 0;
}

uint16_t bt_stack_disable(void)
{
	uint16_t ret = 0;

	ret = bt_stack_profile_deinit();
	if (ret) {
		return ret;
	}

	ret = bt_stack_deinit();
	if (ret) {
		return ret;
	}

	ret = bt_stack_api_deinit();
	if (ret) {
		return ret;
	}

	return 0;
}

uint16_t bt_stack_api_send(void *pcmd)
{
	uint8_t event = EVENT_IO_TO_APP;
	T_IO_MSG io_msg;

	if (!pcmd) {
		return RTK_BT_ERR_POINTER_INVALID;
	}
	if (!api_task_running) {
		return RTK_BT_ERR_NOT_READY;
	}

	io_msg.type = IO_MSG_TYPE_API_SYS_CALL;
	io_msg.subtype = 0;
	io_msg.u.buf = pcmd;

	if (false == osif_msg_send(api_task_io_msg_q, &io_msg, 0)) {
		return RTK_BT_ERR_OS_OPERATION;
	}
	if (false == osif_msg_send(api_task_evt_msg_q, &event, 0)) {
		return RTK_BT_ERR_OS_OPERATION;
	}

	return 0;
}

uint16_t bt_stack_msg_send(uint16_t type, uint16_t subtype, void *msg)
{
	uint8_t event = EVENT_IO_TO_APP;
	T_IO_MSG io_msg;

	if (!type || !subtype || !msg) {
		return RTK_BT_ERR_POINTER_INVALID;
	}
	if (!api_task_running) {
		return RTK_BT_ERR_NOT_READY;
	}

	io_msg.type = type;
	io_msg.subtype = subtype;
	io_msg.u.buf = msg;

	if (false == osif_msg_send(api_task_io_msg_q, &io_msg, 0)) {
		return RTK_BT_ERR_OS_OPERATION;
	}
	if (false == osif_msg_send(api_task_evt_msg_q, &event, 0)) {
		return RTK_BT_ERR_OS_OPERATION;
	}

	return RTK_BT_OK;
}


#ifdef CONFIG_BT_API_DEBUG
void BT_API_DUMPBUF(uint8_t level, const char *func, uint8_t *buf, uint16_t len)
{
	int i = 0;
	if (level <= BT_API_DEBUG_LEVEL) {
		BT_API_PRINT(level, "%s:buf %p, buf len is %d\r\n", func, buf, len);
		for (i = 0; i < len; i++) {
			printf("%02x ", buf[i]);
			if ((i + 1) % 16 == 0) {
				printf("\r\n");
			}
		}
		printf("\r\n");
	}
}
#endif