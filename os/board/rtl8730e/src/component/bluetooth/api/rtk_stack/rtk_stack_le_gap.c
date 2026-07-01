/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <dlist.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_common.h>
#include <rtk_stack_config.h>
#include <rtk_stack_internal.h>
#include <bt_api_config.h>

#include <gap.h>
#include <gap_le.h>
#include <gap_msg.h>
#include <gap_adv.h>
#include <gap_ext_scan.h>
#include <gap_pa_adv.h>
#include <gap_pa_sync.h>
#include <gap_past_recipient.h>
#include <gap_past_sender.h>
#include <gap_conn_le.h>
#include <gap_bond_le.h>
#include <gap_scan.h>
#include <gap_callback_le.h>
#include <gap_config.h>
#include <gap_privacy.h>
#include <gap_le_rf.h>
#include <gap_customer.h>
#include <gap_vendor.h>
#include <gatt_builtin_services.h>
#include <gap_dtm.h>

#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
#include <ble_audio.h>
#endif

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
#include <ble_mgr.h>
#endif

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
#include <ble_ext_adv.h>
#else
#include <gap_ext_adv.h>
#endif

#define RTK_GAP_INVALID_ADV_HANDLE  0xFF

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <trace_app.h>
#include <mesh_api.h>
#include <rtk_bt_device.h>
#include <rtk_stack_vendor.h>
#include <rtk_stack_mesh_internal.h>
bool rtk_ble_mesh_scan_enable_flag = false;
#endif

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
#include <gap_aox.h>
#include <gap_aox_conn.h>
#include <gap_aox_connless_receiver.h>
#include <gap_aox_connless_transmitter.h>
#endif

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
#include <gap_credit_based_conn.h>
#endif

#define GAP_CONN_STATE_SLAVE_CONNECTING   0xFF

typedef struct {
	uint8_t is_pairing_initiator;
	uint8_t is_active;
	uint8_t conn_state; /* 0x0-0x3: ref T_GAP_CONN_STATE;
                           0xFF: ref GAP_CONN_STATE_SLAVE_CONNECTING, specific used for slave connecting.  */
	T_GAP_REMOTE_ADDR_TYPE bd_type;
	uint8_t peer_addr[GAP_BD_ADDR_LEN];
	T_GAP_ROLE role;
	uint16_t mtu_size;
} bt_stack_le_link_info_t;

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
typedef struct {
	bool used;
	uint8_t adv_handle;
#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	T_BLE_EXT_ADV_MGR_STATE ext_adv_state;
#else   /* use gap_ext_adv.h */
	T_GAP_EXT_ADV_STATE ext_adv_state;
#endif
#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
	T_GAP_PA_ADV_STATE pa_state;
#endif
} bt_stack_ext_adv_info_t;
#endif

#if (defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && (defined(F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT) && F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
typedef struct {
	uint16_t sync_handle;
	uint16_t service_data;
} bt_stack_le_past_recv_info_t;
#endif

extern void bt_stack_gatts_evt_indicate_mtu_exchange(uint8_t conn_id, uint16_t mtu);
extern void bt_stack_gattc_evt_indicate_mtu_exchange(uint8_t conn_id, uint16_t mtu);

static T_GAP_DEV_STATE le_gap_dev_state = {0};
static bt_stack_le_link_info_t bt_stack_le_link_tbl[RTK_BLE_GAP_MAX_LINKS] = {0};
static uint16_t bt_stack_le_conn_handle[RTK_BLE_GAP_MAX_LINKS] = {0};
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
static bt_stack_ext_adv_info_t bt_stack_ext_adv_tbl[GAP_MAX_EXT_ADV_SETS] = {0};
static uint8_t bt_stack_le_legacy_adv_hdl = RTK_GAP_INVALID_ADV_HANDLE;
#endif
#if (defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT) && (defined(F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SUPPORT)
static T_GAP_PA_SYNC_DEV_STATE le_gap_pa_sync_dev_state = {0};
#if (defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && (defined(F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT) && F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
static bt_stack_le_past_recv_info_t le_past_recv_info[RTK_BLE_GAP_MAX_LINKS] = {0};
#endif
#endif

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
typedef enum {
	PRIVACY_RESOLVING_LIST_IDLE,
	PRIVACY_RESOLVING_LIST_ADD_PENDING,
	PRIVACY_RESOLVING_LIST_REMOVE_PENDING,
} T_PRIVACY_MODIFY_STATE;

typedef enum {
	PRIVACY_RESOLVING_ADDED_IDLE,
	PRIVACY_RESOLVING_ADDED_PENDING,
	PRIVACY_RESOLVING_ADDED,
} T_PRIVACY_ADDED_STATE;

typedef struct {
	bool is_used;
	T_PRIVACY_ADDED_STATE  is_added;
	T_PRIVACY_MODIFY_STATE state;
	bool device_mode;
	T_GAP_IDENT_ADDR_TYPE  remote_bd_type;
	uint8_t  addr[6];
} T_LE_PRIVACY_ENTRY, *P_LE_PRIVACY_ENTRY;

T_LE_PRIVACY_ENTRY *privacy_table = NULL;
bool privacy_clear_pending = false;
bool privacy_modify_resolv_list_pending = false;
uint8_t privacy_modify_resolv_list_idx = 0xff;
T_LE_PRIVACY_STATE privacy_resolution_status = LE_PRIVACY_RESOLUTION_DISABLED;
bool privacy_enable = false;
bool privacy_whitelist = true;
static void privacy_handle_resolv_list(bool indicate);
static void privacy_handle_bond_modify_msg(T_LE_BOND_MODIFY_TYPE type, T_LE_KEY_ENTRY *p_entry);
#endif

static rtk_bt_le_link_role_t convert_rtk_link_role(T_GAP_ROLE role)
{
	switch (role) {
	case GAP_LINK_ROLE_MASTER:
		return RTK_BT_LE_ROLE_MASTER;
	case GAP_LINK_ROLE_SLAVE:
		return RTK_BT_LE_ROLE_SLAVE;
	default:
		return RTK_BT_LE_ROLE_UNKNOWN;
	}
}

static void bt_stack_le_gap_handle_scan_state_evt(uint8_t pre_state, uint8_t new_state, uint16_t cause)
{
	rtk_bt_le_scan_start_ind_t *p_scan_start_ind = NULL;
	rtk_bt_le_scan_stop_ind_t *p_scan_stop_ind = NULL;
	rtk_bt_evt_t *p_evt = NULL;

	BT_LOGD("bt_stack_le_gap_handle_scan_state_evt new_state: 0x%x, pre_state: 0x%x, cause: 0x%x\r\n", new_state, pre_state, cause);

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
	privacy_handle_resolv_list(false);
#endif

	/* scan state change (central) */
	if (pre_state != new_state) {
		if (GAP_SCAN_STATE_START == pre_state) {
			if (GAP_SCAN_STATE_IDLE == new_state) {
				/* Scan started failed by user API */
				BT_LOGD("[scan_state_change]: Scan started failed by user API, cause: 0x%x\r\n", cause);
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_SCAN_START_IND,
											sizeof(rtk_bt_le_scan_start_ind_t));
				if (!p_evt) {
					return;
				}

				p_scan_start_ind = (rtk_bt_le_scan_start_ind_t *)p_evt->data;
				p_scan_start_ind->err = cause;
				rtk_bt_evt_indicate(p_evt, NULL);
			} else if (GAP_SCAN_STATE_SCANNING == new_state) {
				/* Scan started success by user API */
				BT_LOGD("[scan_state_change]: Scan started success by user API\r\n");
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_SCAN_START_IND,
											sizeof(rtk_bt_le_scan_start_ind_t));
				if (!p_evt) {
					return;
				}

				p_scan_start_ind = (rtk_bt_le_scan_start_ind_t *)p_evt->data;
				p_scan_start_ind->err = 0;
				le_scan_get_param(GAP_PARAM_SCAN_MODE, &p_scan_start_ind->scan_type);
				rtk_bt_evt_indicate(p_evt, NULL);
			}
		} else if (GAP_SCAN_STATE_STOP == pre_state) {
			if (GAP_SCAN_STATE_IDLE == new_state) {
				/* Scan stopped success by user API */
				BT_LOGD("[scan_state_change]: Scan stopped success by user API\r\n");
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_SCAN_STOP_IND,
											sizeof(rtk_bt_le_scan_stop_ind_t));
				if (!p_evt) {
					return;
				}

				p_scan_stop_ind = (rtk_bt_le_scan_stop_ind_t *)p_evt->data;
				p_scan_stop_ind->err = 0;
				p_scan_stop_ind->stop_reason = RTK_BT_LE_SCAN_STOP_BY_HOST;
				rtk_bt_evt_indicate(p_evt, NULL);
			} else if (GAP_SCAN_STATE_SCANNING == new_state) {
				/* Scan stopped failed by user API */
				BT_LOGD("[scan_state_change]: Scan stopped failed by user API, cause: 0x%04x\r\n", cause);
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_SCAN_STOP_IND,
											sizeof(rtk_bt_le_scan_stop_ind_t));
				if (!p_evt) {
					return;
				}

				p_scan_stop_ind = (rtk_bt_le_scan_stop_ind_t *)p_evt->data;
				p_scan_stop_ind->err = cause;
				rtk_bt_evt_indicate(p_evt, NULL);
			}
		} else if (GAP_SCAN_STATE_SCANNING == pre_state && GAP_SCAN_STATE_IDLE == new_state) {
			/* Scan stop success due to duration timeout */
			BT_LOGD("[scan_state_change]: Scan stopped due to duration timeout, cause: %d\r\n", cause);
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_SCAN_STOP_IND,
										sizeof(rtk_bt_le_scan_stop_ind_t));
			if (!p_evt) {
				return;
			}

			p_scan_stop_ind = (rtk_bt_le_scan_stop_ind_t *)p_evt->data;
			p_scan_stop_ind->err = 0;
			p_scan_stop_ind->stop_reason = RTK_BT_LE_SCAN_STOP_BY_DURATION;
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
}

#if (defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT) && (defined(F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SUPPORT)
static void bt_stack_le_gap_handle_pa_sync_dev_state_evt(T_GAP_PA_SYNC_DEV_STATE new_state, uint16_t cause)
{
	(void)cause;
	T_GAP_PA_SYNC_DEV_STATE pre_state = le_gap_pa_sync_dev_state;
	le_gap_pa_sync_dev_state = new_state;

	if (pre_state.gap_terminate_sync_state != new_state.gap_terminate_sync_state) {
		if (new_state.gap_terminate_sync_state == GAP_PA_TERMINATE_SYNC_DEV_STATE_IDLE) {
			BT_LOGD("[pa_sync_dev_state_change]: terminate IDLE, cause: 0x%04x\r\n", cause);
		} else if (new_state.gap_terminate_sync_state == GAP_PA_TERMINATE_SYNC_DEV_STATE_TERMINATING) {
			BT_LOGD("[pa_sync_dev_state_change]: terminate TERMINATING, cause: 0x%04x\r\n", cause);
		}
	}

	if (pre_state.gap_create_sync_state != new_state.gap_create_sync_state) {
		if (new_state.gap_create_sync_state == GAP_PA_CREATE_SYNC_DEV_STATE_IDLE) {
			BT_LOGD("[pa_sync_dev_state_change]: create IDLE, cause: 0x%04x\r\n", cause);
		} else if (new_state.gap_create_sync_state == GAP_PA_CREATE_SYNC_DEV_STATE_SYNCHRONIZING) {
			BT_LOGD("[pa_sync_dev_state_change]: create SYNCHRONIZING, cause: 0x%04x\r\n", cause);
		}
	}

	if (pre_state.gap_periodic_adv_receive_enable_state != new_state.gap_periodic_adv_receive_enable_state) {
		if (new_state.gap_periodic_adv_receive_enable_state == GAP_PA_RECEIVE_ENABLE_DEV_STATE_IDLE) {
			BT_LOGD("[pa_sync_dev_state_change]: receive enable IDLE, cause: 0x%04x\r\n", cause);
		} else if (new_state.gap_periodic_adv_receive_enable_state == GAP_PA_RECEIVE_ENABLE_DEV_STATE_ENABLING) {
			BT_LOGD("[pa_sync_dev_state_change]: receive enable ENABLING, cause: 0x%04x\r\n", cause);
		}
	}
}

static void bt_stack_le_gap_handle_pa_sync_state_evt(uint8_t sync_id, uint16_t sync_handle,
													 T_GAP_PA_SYNC_STATE new_state,
													 bool sync_transfer_received_flag,
													 uint16_t cause)
{
	bool indicate = false;
	rtk_bt_le_pa_sync_ind_t *p_ind;
	rtk_bt_evt_t *p_evt;

	BT_LOGD("[pa_sync_state_evt]: sync_id: %d, sync_handle: 0x%x, new_state: %d, sync_transfer_received_flag: %d, cause: 0x%04x\r\n",
			sync_id, sync_handle, new_state, sync_transfer_received_flag, cause);

	switch (new_state) {
	case GAP_PA_SYNC_STATE_TERMINATED:
#if defined(F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
	case GAP_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING:
#endif
	case GAP_PA_SYNC_STATE_SYNCHRONIZING:
	case GAP_PA_SYNC_STATE_SYNCHRONIZED: {
		indicate = true;
		break;
	}

	default:
		break;
	}

	if (!indicate) {
		return;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND,
								sizeof(rtk_bt_le_pa_sync_ind_t));
	if (!p_evt) {
		return;
	}

	p_ind = (rtk_bt_le_pa_sync_ind_t *)p_evt->data;
	p_ind->cause = cause;
	p_ind->sync_id = sync_id;
	p_ind->state = (rtk_bt_le_pa_sync_state_t)new_state;

	if (new_state == GAP_PA_SYNC_STATE_SYNCHRONIZED) {
		T_GAP_PA_SYNC_COMMON_SYNC_PARAM param;
		if (le_pa_sync_get_param(PA_SYNC_PARAM_SYNCHRONIZED_PARAM, &param, sync_id) != GAP_CAUSE_SUCCESS) {
			rtk_bt_event_free(p_evt);
			return;
		}

		p_ind->info.past_received = sync_transfer_received_flag;
		p_ind->info.sync_handle = sync_handle;
		p_ind->info.adv_sid = param.adv_sid;
		p_ind->info.addr.type = (rtk_bt_le_addr_type_t)param.adv_addr_type;
		memcpy(p_ind->info.addr.addr_val, param.adv_addr, GAP_BD_ADDR_LEN);
		p_ind->info.adv_phy = (rtk_bt_le_phy_type_t)param.adv_phy;
		p_ind->info.adv_clock_accuracy = param.adv_clock_accuracy;
		p_ind->info.periodic_adv_interval = param.periodic_adv_interval;
#if defined(RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT) && RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT
		// p_ind->info.num_subevents = param.num_subevents;
		// p_ind->info.subevent_interval = param.subevent_interval;
		// p_ind->info.response_slot_delay = param.response_slot_delay;
		// p_ind->info.response_slot_spacing = param.response_slot_spacing;
#endif
	}

#if (defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && (defined(F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT) && F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
	int i;
	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		if (le_past_recv_info[i].sync_handle == sync_handle) {
			if (new_state == GAP_PA_SYNC_STATE_TERMINATED) {
				memset(&le_past_recv_info[i], 0, sizeof(bt_stack_le_past_recv_info_t));
			} else if (new_state == GAP_PA_SYNC_STATE_SYNCHRONIZED && sync_transfer_received_flag) {
				p_ind->info.conn_handle = bt_stack_le_conn_handle[i];
				p_ind->info.service_data = le_past_recv_info[i].service_data;
			}
		}
	}
#endif

	rtk_bt_evt_indicate(p_evt, NULL);
}
#endif

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
    (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT) && \
    (!defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV)
static void bt_stack_le_gap_handle_ext_adv_state_evt(uint8_t adv_handle, T_GAP_EXT_ADV_STATE new_state, uint16_t cause)
{
	rtk_bt_le_ext_adv_ind_t *p_ext_adv_ind = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_cmd_t *p_cmd = NULL;
	T_GAP_EXT_ADV_STATE pre_state;
	uint8_t idx;

	for (idx = 0; idx < GAP_MAX_EXT_ADV_SETS; idx++) {
		if (bt_stack_ext_adv_tbl[idx].used &&
			bt_stack_ext_adv_tbl[idx].adv_handle == adv_handle) {
			pre_state = bt_stack_ext_adv_tbl[idx].ext_adv_state;
			bt_stack_ext_adv_tbl[idx].ext_adv_state = new_state;
			break;
		}
	}

	if (idx == GAP_MAX_EXT_ADV_SETS) {
		return;
	}

	BT_LOGD("app_handle_ext_adv_state_evt: adv_handle = %d cause = 0x%x oldState = %d newState = %d\r\n",
			bt_stack_ext_adv_tbl[idx].adv_handle, cause, pre_state, new_state);

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
	privacy_handle_resolv_list(false);
#endif

	if (pre_state == new_state || new_state == EXT_ADV_STATE_START || new_state == EXT_ADV_STATE_STOP) {
		return;
	}

	if (new_state == EXT_ADV_STATE_IDLE) {
		p_cmd = bt_stack_pending_cmd_search(GAP_MSG_LE_EXT_ADV_STATE_CHANGE_INFO);
		if (p_cmd && (adv_handle == *((uint8_t *)p_cmd->param))) {
			BT_LOGD("app_handle_ext_adv_state_evt: async_cmd complete.\r\n");
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = 0;
			osif_sem_give(p_cmd->psem);
		}
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_EXT_ADV_IND,
								sizeof(rtk_bt_le_ext_adv_ind_t));
	if (!p_evt) {
		return;
	}

	p_ext_adv_ind = (rtk_bt_le_ext_adv_ind_t *)p_evt->data;
	p_ext_adv_ind->adv_handle = adv_handle;
	p_ext_adv_ind->is_start = true;
	p_ext_adv_ind->err = cause;
	p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_UNKNOWN;
	BT_LOGD("[ext_adv_state_change] new_state: 0x%d, cause: 0x%x\r\n", new_state, cause);
	if (EXT_ADV_STATE_IDLE == new_state) {
		if (EXT_ADV_STATE_START == pre_state) {
			BT_LOGD("[ext_adv_state_change]: Ext ADV started failed, cause: 0x%04x\r\n", cause);
		} else {
			if (cause == (HCI_ERR | HCI_ERR_OPERATION_CANCELLED_BY_HOST)) {
				p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_HOST;
				p_ext_adv_ind->err = 0;
			} else if (cause == (HCI_ERR | HCI_ERR_DIRECTED_ADV_TIMEOUT) ||
					   cause == (HCI_ERR | HCI_ERR_LIMIT_REACHED)) {
				p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_DURATION;
				p_ext_adv_ind->err = 0;
			} else if (cause == 0) {
				p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_CONN;
			}

			p_ext_adv_ind->is_start = false;
		}
	} else { /*if (EXT_ADV_STATE_ADVERTISING == new_state)*/
		if (EXT_ADV_STATE_START == pre_state) {
			BT_LOGD("[ext_adv_state_change]: Ext ADV started success\r\n");
		} else if (EXT_ADV_STATE_STOP == pre_state) {
			BT_LOGD("[ext_adv_state_change]: Ext ADV stopped failed, cause: 0x%04x\r\n", cause);
			p_ext_adv_ind->is_start = false;
		}
	}
	rtk_bt_evt_indicate(p_evt, NULL);
}
#endif

#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
static void bt_stack_le_gap_handle_pa_state_evt(uint8_t adv_handle, T_GAP_PA_ADV_STATE new_state, uint16_t cause)
{
	rtk_bt_le_pa_ind_t *p_pa_ind = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	T_GAP_PA_ADV_STATE pre_state;
	uint8_t idx;

	for (idx = 0; idx < GAP_MAX_EXT_ADV_SETS; idx++) {
		if (bt_stack_ext_adv_tbl[idx].used &&
			bt_stack_ext_adv_tbl[idx].adv_handle == adv_handle) {
			pre_state = bt_stack_ext_adv_tbl[idx].pa_state;
			bt_stack_ext_adv_tbl[idx].pa_state = new_state;
			break;
		}
	}

	if (idx == GAP_MAX_EXT_ADV_SETS) {
		return;
	}

	BT_LOGD("bt_stack_le_gap_handle_pa_state_evt: adv_handle = %d cause = 0x%x oldState = %d newState = %d\r\n",
			bt_stack_ext_adv_tbl[idx].adv_handle, cause, pre_state, new_state);

	if (pre_state == new_state || new_state == PA_ADV_STATE_START ||
		new_state == PA_ADV_STATE_START_EXT_ADV_STATE_IDLE ||
		new_state == PA_ADV_STATE_STOP) { /* state not change or enter temp state */
		return;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_PA_IND,
								sizeof(rtk_bt_le_pa_ind_t));
	if (!p_evt) {
		return;
	}

	p_pa_ind = (rtk_bt_le_pa_ind_t *)p_evt->data;
	p_pa_ind->adv_handle = adv_handle;
	p_pa_ind->cause = 0;

	if (PA_ADV_STATE_IDLE == new_state) {
		p_pa_ind->state = RTK_BT_LE_PA_STATE_IDLE;
		if (PA_ADV_STATE_START == pre_state || PA_ADV_STATE_START_EXT_ADV_STATE_IDLE == pre_state) {
			BT_LOGD("[pa_state_change]: Periodic ADV started failed, cause: 0x%04x\r\n", cause);
			p_pa_ind->cause = cause;
		} else if (GAP_ADV_STATE_STOP == pre_state) {
			if (cause == (HCI_ERR | HCI_ERR_OPERATION_CANCELLED_BY_HOST)) {
				BT_LOGD("[pa_state_change]: Periodic ADV stopped success by user API\r\n");
			} else {
				BT_LOGD("[pa_state_change]: Periodic ADV stopped due to connection establish\r\n");
			}
		}
	} else if (PA_ADV_STATE_ADVERTISING == new_state) {
		p_pa_ind->state = RTK_BT_LE_PA_STATE_ADVERTISING;
		if (PA_ADV_STATE_STOP == pre_state) {
			BT_LOGD("[pa_state_change]: Periodic ADV stopped failed, cause: 0x%04x\r\n", cause);
			p_pa_ind->cause = cause;
		} else {
			BT_LOGD("[pa_state_change]: Periodic ADV started success\r\n");
		}
	} else { /*if (PA_ADV_STATE_WAIT_EXT_ADV_STATE_ADVERTISING == new_state)*/
		BT_LOGD("[pa_state_change]: Periodic ADV is waiting for ext ADV start\r\n");
		p_pa_ind->state = RTK_BT_LE_PA_STATE_WAIT_EXT_ADV_ADVERTISING;
		p_pa_ind->cause = cause;
	}

	rtk_bt_evt_indicate(p_evt, NULL);
}
#endif

static T_APP_RESULT bt_stack_le_gap_callback(uint8_t type, void *data)
{
	T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)data;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_cmd_t *p_cmd = NULL;
	BT_LOGD("bt_stack_le_gap_callback: type = 0x%x \r\n", type);

#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
	ble_audio_handle_gap_cb(type, p_data);
#endif

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	ble_mgr_handle_gap_cb(type, p_data);
#endif

	switch (type) {
	case GAP_MSG_LE_MODIFY_WHITE_LIST: {
		BT_LOGD("[GAP_MSG_LE_MODIFY_WHITE_LIST]: operation: %d, cause: 0x%x\r\n",
				p_data->p_le_modify_white_list_rsp->operation,
				p_data->p_le_modify_white_list_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			rtk_bt_le_modify_wl_param_t *wl_op = (rtk_bt_le_modify_wl_param_t *)p_cmd->param;
			bt_stack_pending_cmd_delete(p_cmd);
			if ((uint8_t)wl_op->op == (uint8_t)p_data->p_le_modify_white_list_rsp->operation) {
				p_cmd->ret = p_data->p_le_modify_white_list_rsp->cause;
				osif_sem_give(p_cmd->psem);
			} else {
				BT_LOGE("[%s] GAP_MSG_LE_MODIFY_WHITE_LIST: api operation mismatch with callback \r\n", __func__);
			}
		} else {
			BT_LOGD("[%s] GAP_MSG_LE_MODIFY_WHITE_LIST: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_SET_RAND_ADDR:
		BT_LOGD("GAP_MSG_LE_SET_RAND_ADDR: \r\n");
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_set_rand_addr_rsp->cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_SET_RAND_ADDR: find no pending command \r\n", __func__);
		}
		break;

	case GAP_MSG_LE_SET_HOST_CHANN_CLASSIF:
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_set_host_chann_classif_rsp->cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_SET_HOST_CHANN_CLASSIF: find no pending command \r\n", __func__);
		}
		break;

	case GAP_MSG_LE_READ_RSSI: {
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			rtk_bt_le_read_rssi_param_t *read_rssi =
				(rtk_bt_le_read_rssi_param_t *)p_cmd->param;
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_read_rssi_rsp->cause;
			*read_rssi->p_rssi = p_data->p_le_read_rssi_rsp->rssi;
			// BT_LOGA("RSSI IS: %d\r\n", p_data->p_le_read_rssi_rsp->rssi);
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_READ_RSSI: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_SCAN_INFO: {
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
		if (!rtk_bt_mesh_is_enable() || rtk_ble_mesh_scan_enable_flag)
#endif
		{
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
										RTK_BT_LE_GAP_EVT_SCAN_RES_IND,
										sizeof(rtk_bt_le_scan_res_ind_t));
			rtk_bt_le_scan_res_ind_t *scan_res = (rtk_bt_le_scan_res_ind_t *)p_evt->data;
			scan_res->num_report = 1;
			scan_res->adv_report.evt_type =
				(rtk_bt_le_adv_report_type_t)p_data->p_le_scan_info->adv_type;
			scan_res->adv_report.addr.type =
				(rtk_bt_le_addr_type_t)p_data->p_le_scan_info->remote_addr_type;
			memcpy(scan_res->adv_report.addr.addr_val, p_data->p_le_scan_info->bd_addr,
				   RTK_BD_ADDR_LEN);
			scan_res->adv_report.len = p_data->p_le_scan_info->data_len;
			memcpy(scan_res->adv_report.data, p_data->p_le_scan_info->data,
				   RTK_BT_LE_MAX_ADV_DATA_LEN);
			scan_res->adv_report.rssi = p_data->p_le_scan_info->rssi;
			rtk_bt_evt_indicate(p_evt, NULL);
		}
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
		if (rtk_bt_mesh_is_enable()) {
			gap_sched_handle_adv_report(p_data->p_le_scan_info);
		}
#endif
		break;
	}

	case GAP_MSG_LE_ADV_UPDATE_PARAM: {
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
		if (rtk_bt_mesh_is_enable()) {
			gap_sched_adv_params_set_done();
		}
#endif
		break;
	}

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_SCAN_SUPPORT) && F_BT_LE_5_0_AE_SCAN_SUPPORT)
	case GAP_MSG_LE_EXT_ADV_REPORT_INFO: {
		T_LE_EXT_ADV_REPORT_INFO *p_info = p_data->p_le_ext_adv_report_info;
		BT_LOGD("GAP_MSG_LE_EXT_ADV_REPORT_INFO:connectable %d, scannable %d, direct %d, scan response %d, legacy %d, data status 0x%x\r\n",
				p_info->event_type & GAP_EXT_ADV_REPORT_BIT_CONNECTABLE_ADV,
				p_info->event_type & GAP_EXT_ADV_REPORT_BIT_SCANNABLE_ADV,
				p_info->event_type & GAP_EXT_ADV_REPORT_BIT_DIRECTED_ADV,
				p_info->event_type & GAP_EXT_ADV_REPORT_BIT_SCAN_RESPONSE,
				p_info->event_type & GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV,
				p_info->data_status);
		BT_LOGD("GAP_MSG_LE_EXT_ADV_REPORT_INFO:event_type 0x%x, bd_addr %02x:%02x:%02x:%02x:%02x:%02x, addr_type %d, rssi %d, data_len %d\r\n",
				p_info->event_type,
				p_info->bd_addr[5], p_info->bd_addr[4],
				p_info->bd_addr[3], p_info->bd_addr[2],
				p_info->bd_addr[1], p_info->bd_addr[0],
				p_info->addr_type,
				p_info->rssi,
				p_info->data_len);
		if ((p_info->event_type & GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV) == 0) {
			BT_LOGD("GAP_MSG_LE_EXT_ADV_REPORT_INFO:primary_phy %d, secondary_phy %d, adv_sid %d, tx_power %d, peri_adv_interval %d\r\n",
					p_info->primary_phy,
					p_info->secondary_phy,
					p_info->adv_sid,
					p_info->tx_power,
					p_info->peri_adv_interval);
		}
		if (p_info->event_type & GAP_EXT_ADV_REPORT_BIT_DIRECTED_ADV) {
			BT_LOGD("GAP_MSG_LE_EXT_ADV_REPORT_INFO:direct_addr_type 0x%x, direct_addr %02x:%02x:%02x:%02x:%02x:%02x\r\n",
					p_info->direct_addr_type,
					p_info->direct_addr[5], p_info->direct_addr[4],
					p_info->direct_addr[3], p_info->direct_addr[2],
					p_info->direct_addr[1], p_info->direct_addr[0]);
		}

#if 1
		if ((p_info->event_type & GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV) == 0) {
			BT_LOGD("ext adv data content:\r\n");
			BT_DUMPD("", p_info->p_data, p_info->data_len);
		}
#endif

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
		if (!rtk_bt_mesh_is_enable() || rtk_ble_mesh_scan_enable_flag)
#endif
		{
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
										RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND,
										sizeof(rtk_bt_le_ext_scan_res_ind_t) + p_info->data_len);
			rtk_bt_le_ext_scan_res_ind_t *scan_res = (rtk_bt_le_ext_scan_res_ind_t *)p_evt->data;
			scan_res->data = (uint8_t *)scan_res + sizeof(rtk_bt_le_ext_scan_res_ind_t);
			scan_res->evt_type = p_info->event_type;
			scan_res->addr.type = (rtk_bt_le_addr_type_t)p_info->addr_type;
			memcpy(scan_res->addr.addr_val, p_info->bd_addr, RTK_BD_ADDR_LEN);
			scan_res->direct_addr.type = (rtk_bt_le_addr_type_t)p_info->direct_addr_type;
			memcpy(scan_res->direct_addr.addr_val, p_info->direct_addr, RTK_BD_ADDR_LEN);
			scan_res->data_status = p_info->data_status;
			scan_res->len = p_info->data_len;
			memcpy(scan_res->data, p_info->p_data, p_info->data_len);
			scan_res->rssi = p_info->rssi;
			scan_res->primary_phy = p_info->primary_phy;
			scan_res->secondary_phy = p_info->secondary_phy;
			scan_res->adv_sid = p_info->adv_sid;
			scan_res->tx_power = p_info->tx_power;
			scan_res->peri_adv_interval = p_info->peri_adv_interval;
			rtk_bt_evt_indicate(p_evt, NULL);
		}
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
		if (rtk_bt_mesh_is_enable()) {
			// APP_PRINT_INFO5("GAP_MSG_LE_EXT_ADV_REPORT_INFO:event_type 0x%x, bd_addr %s, addr_type %d, rssi %d, data_len %d",
			//              p_data->p_le_ext_adv_report_info->event_type,
			//              TRACE_BDADDR(p_data->p_le_ext_adv_report_info->bd_addr),
			//              p_data->p_le_ext_adv_report_info->addr_type,
			//              p_data->p_le_ext_adv_report_info->rssi,
			//              p_data->p_le_ext_adv_report_info->data_len);
			gap_sched_handle_ext_adv_report(p_data->p_le_ext_adv_report_info);
		}
#endif
		break;
	}
#endif

	case GAP_MSG_LE_CONN_UPDATE_IND: {
		/* Connection parameters update request by remote */
		T_APP_RESULT app_res = (T_APP_RESULT)0;
		uint8_t cb_ret = 0;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
									RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND,
									sizeof(rtk_bt_le_remote_conn_update_req_ind_t));
		rtk_bt_le_remote_conn_update_req_ind_t *update_ind =
			(rtk_bt_le_remote_conn_update_req_ind_t *)p_evt->data;
		update_ind->conn_handle = le_get_conn_handle(p_data->p_le_conn_update_ind->conn_id);
		update_ind->conn_interval_min = p_data->p_le_conn_update_ind->conn_interval_min;
		update_ind->conn_interval_max = p_data->p_le_conn_update_ind->conn_interval_max;
		update_ind->conn_latency = p_data->p_le_conn_update_ind->conn_latency;
		update_ind->supv_timeout = p_data->p_le_conn_update_ind->supervision_timeout;

		BT_LOGD("[GAP_MSG_LE_CONN_UPDATE_IND]: remote device request conn param update, conn_id: %d, conn_interval_min: 0x%x, conn_interval_max: 0x%d, conn_latency: 0x%x, timeout: 0x%x\r\n",
				update_ind->conn_handle, update_ind->conn_interval_min,
				update_ind->conn_interval_max, update_ind->conn_latency,
				update_ind->supv_timeout);
		rtk_bt_evt_indicate(p_evt, &cb_ret);
		if (RTK_BT_EVT_CB_ACCEPT == cb_ret) {
			BT_LOGD("[GAP_MSG_LE_CONN_UPDATE_IND]: Host stack accepted the request!\r\n");
			app_res = APP_RESULT_ACCEPT;
		} else {
			BT_LOGD("[GAP_MSG_LE_CONN_UPDATE_IND]: Host stack rejected the request!\r\n");
			app_res = APP_RESULT_REJECT;
		}

		return app_res;
	}

#if defined(F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT) && F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
	case GAP_MSG_LE_SET_DATA_LEN: {
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_set_data_len_rsp->cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_SET_DATA_LEN: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_DATA_LEN_CHANGE_INFO: {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
									RTK_BT_LE_GAP_EVT_DATA_LEN_CHANGE_IND,
									sizeof(rtk_bt_le_data_len_change_ind_t));
		rtk_bt_le_data_len_change_ind_t *data_len_change =
			(rtk_bt_le_data_len_change_ind_t *)p_evt->data;
		data_len_change->conn_handle = le_get_conn_handle(p_data->p_le_data_len_change_info->conn_id);
		data_len_change->max_tx_octets = p_data->p_le_data_len_change_info->max_tx_octets;
		data_len_change->max_tx_time = p_data->p_le_data_len_change_info->max_tx_time;
		data_len_change->max_rx_octets = p_data->p_le_data_len_change_info->max_rx_octets;
		data_len_change->max_rx_time = p_data->p_le_data_len_change_info->max_rx_time;
		BT_LOGD("[GAP_MSG_LE_DATA_LEN_CHANGE_INFO]: conn_handle: %d, max_tx_octets: 0x%x, "
				"max_tx_time: 0x%x, max_rx_octets: 0x%x, max_rx_time: 0x%x\r\n",
				data_len_change->conn_handle,
				data_len_change->max_tx_octets,
				data_len_change->max_tx_time,
				data_len_change->max_rx_octets,
				data_len_change->max_rx_time);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
#endif  /* F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT */

#if defined(F_BT_LE_5_0_SET_PHYS_SUPPORT) && F_BT_LE_5_0_SET_PHYS_SUPPORT
	case GAP_MSG_LE_PHY_UPDATE_INFO: {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
									RTK_BT_LE_GAP_EVT_PHY_UPDATE_IND,
									sizeof(rtk_bt_le_phy_update_ind_t));
		rtk_bt_le_phy_update_ind_t *phy_update =
			(rtk_bt_le_phy_update_ind_t *)p_evt->data;
		phy_update->err = p_data->p_le_phy_update_info->cause;
		phy_update->conn_handle = le_get_conn_handle(p_data->p_le_phy_update_info->conn_id);
		phy_update->tx_phy = p_data->p_le_phy_update_info->tx_phy;
		phy_update->rx_phy = p_data->p_le_phy_update_info->rx_phy;
		BT_LOGD("[GAP_MSG_LE_PHY_UPDATE_INFO]: conn_handle: %d, tx_phy: %d, rx_phy: %d\r\n",
				phy_update->conn_handle,
				phy_update->tx_phy,
				phy_update->rx_phy);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
#endif  /* F_BT_LE_5_0_SET_PHYS_SUPPORT */


#if defined(F_BT_LE_READ_REMOTE_FEATS) && F_BT_LE_READ_REMOTE_FEATS
	case GAP_MSG_LE_REMOTE_FEATS_INFO: {
		uint8_t remote_feats[8];
		if (p_data->p_le_remote_feats_info->cause == GAP_SUCCESS) {
			memcpy(remote_feats, p_data->p_le_remote_feats_info->remote_feats, 8);
			if (remote_feats[LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1] & LE_SUPPORT_FEATURES_LE_2M_MASK_BIT) {
				BT_LOGD("[GAP_MSG_LE_REMOTE_FEATS_INFO]: support 2M\r\n");
			}
			if (remote_feats[LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1] & LE_SUPPORT_FEATURES_LE_CODED_PHY_MASK_BIT) {
				BT_LOGD("[GAP_MSG_LE_REMOTE_FEATS_INFO]: support CODED\r\n");
			}
		}
	}
	break;
#endif

#if defined(F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT) && F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
	case GAP_MSG_LE_READ_REMOTE_VERSION: {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
									RTK_BT_LE_GAP_EVT_READ_REMOTE_VERSION_IND,
									sizeof(rtk_bt_le_read_remote_version_ind_t));
		rtk_bt_le_read_remote_version_ind_t *rmt_ver = (rtk_bt_le_read_remote_version_ind_t *)p_evt->data;
		rmt_ver->err = p_data->p_le_read_remote_version_rsp->cause;
		rmt_ver->conn_handle = le_get_conn_handle(p_data->p_le_read_remote_version_rsp->conn_id);
		rmt_ver->version = p_data->p_le_read_remote_version_rsp->version;
		rmt_ver->company_id = p_data->p_le_read_remote_version_rsp->manufacturer_name;
		rmt_ver->subversion = p_data->p_le_read_remote_version_rsp->subversion;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
#endif

	case GAP_MSG_LE_BOND_MODIFY_INFO: {
		T_LE_BOND_MODIFY_TYPE operation = p_data->p_le_bond_modify_info->type;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
									RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND,
									sizeof(rtk_bt_le_bond_modify_ind_t));
		rtk_bt_le_bond_modify_ind_t *bond_modify =
			(rtk_bt_le_bond_modify_ind_t *)p_evt->data;
		bond_modify->op = (rtk_bt_le_bond_op_t)operation;
		if (RTK_BT_LE_BOND_ADD == bond_modify->op || RTK_BT_LE_BOND_DELETE == bond_modify->op) {
			bond_modify->remote_addr.type = (rtk_bt_le_addr_type_t)p_data->p_le_bond_modify_info->p_entry->remote_bd.remote_bd_type;
			memcpy(bond_modify->remote_addr.addr_val, p_data->p_le_bond_modify_info->p_entry->remote_bd.addr,
				   sizeof(bond_modify->remote_addr.addr_val));
			bond_modify->ident_addr.type = (rtk_bt_le_ident_addr_type_t)p_data->p_le_bond_modify_info->p_entry->resolved_remote_bd.remote_bd_type;
			memcpy(bond_modify->ident_addr.addr_val, p_data->p_le_bond_modify_info->p_entry->resolved_remote_bd.addr,
				   sizeof(bond_modify->ident_addr.addr_val));
		}
		rtk_bt_evt_indicate(p_evt, NULL);

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
		privacy_handle_bond_modify_msg(operation, p_data->p_le_bond_modify_info->p_entry);
#endif
		break;
	}

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_SCAN_SUPPORT) && F_BT_LE_5_0_AE_SCAN_SUPPORT) && \
(defined(F_BT_LE_GAP_MSG_INFO_WAY) && F_BT_LE_GAP_MSG_INFO_WAY)
	case GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO: {
		uint8_t pre_state = le_gap_dev_state.gap_scan_state;
		le_gap_dev_state.gap_scan_state = p_data->p_le_ext_scan_state_change_info->state;
		BT_LOGD("GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO:state 0x%x, cause 0x%x\r\n",
				p_data->p_le_ext_scan_state_change_info->state,
				p_data->p_le_ext_scan_state_change_info->cause);

		bt_stack_le_gap_handle_scan_state_evt(pre_state, p_data->p_le_ext_scan_state_change_info->state,
											  p_data->p_le_ext_scan_state_change_info->cause);
		break;
	}
#endif

#if (defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT) && (defined(F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SUPPORT)
	case GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST: {
		BT_LOGD("GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST: operation %d, cause 0x%x\r\n",
				p_data->p_le_pa_sync_modify_periodic_adv_list_rsp->operation,
				p_data->p_le_pa_sync_modify_periodic_adv_list_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			rtk_bt_le_pa_sync_modify_advlist_t *ptr =
				(rtk_bt_le_pa_sync_modify_advlist_t *)p_cmd->param;
			bt_stack_pending_cmd_delete(p_cmd);
			if (ptr->operation == (rtk_bt_le_pa_sync_advlist_op_t)p_data->p_le_pa_sync_modify_periodic_adv_list_rsp->operation) {
				p_cmd->ret = p_data->p_le_pa_sync_modify_periodic_adv_list_rsp->cause;
				osif_sem_give(p_cmd->psem);
			} else {
				BT_LOGE("[%s] GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST: api operation mismatch with callback \r\n", __func__);
			}
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST: find no pending command \r\n", __func__);
		}
		break;
	}
	case GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO: {
		T_LE_PERIODIC_ADV_REPORT_INFO *p_info = p_data->p_le_periodic_adv_report_info;
		BT_LOGD("GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO: sync_id %d, sync_handle 0x%x, tx_power %d, rssi %d, cte_type %d, data_status 0x%x, data_len %d\r\n",
				p_info->sync_id, p_info->sync_handle, p_info->tx_power, p_info->rssi,
				p_info->cte_type, p_info->data_status, p_info->data_len);
		/* adv data is contained in  p_data->p_le_periodic_adv_report_info->data */
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
									RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND,
									sizeof(rtk_bt_le_pa_adv_report_ind_t) + p_info->data_len);
		rtk_bt_le_pa_adv_report_ind_t *pa_report =
			(rtk_bt_le_pa_adv_report_ind_t *)p_evt->data;
		pa_report->sync_id = p_info->sync_id;
		pa_report->sync_handle = p_info->sync_handle;
		pa_report->tx_power = p_info->tx_power;
		pa_report->rssi = p_info->rssi;
		pa_report->data_status = p_info->data_status;
		pa_report->data_len = p_info->data_len;
		pa_report->p_data = (uint8_t *)pa_report + sizeof(rtk_bt_le_pa_adv_report_ind_t);
		memcpy(pa_report->p_data, p_info->p_data, p_info->data_len);
		if (p_info->cte_type == GAP_PERIODIC_ADV_REPORT_CTE_TYPE_NO_CTE) {
			pa_report->cte_type = RTK_BT_LE_GAP_CTE_TYPE_NONE;
		} else {
			pa_report->cte_type = BIT(p_info->cte_type);
		}

		rtk_bt_evt_indicate(p_evt, NULL);

		break;
	}
	case GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO: {
		bt_stack_le_gap_handle_pa_sync_dev_state_evt(p_data->p_le_pa_sync_dev_state_change_info->state,
													 p_data->p_le_pa_sync_dev_state_change_info->cause);
		break;
	}
	case GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO: {
		bt_stack_le_gap_handle_pa_sync_state_evt(p_data->p_le_pa_sync_state_change_info->sync_id,
												 p_data->p_le_pa_sync_state_change_info->sync_handle,
												 (T_GAP_PA_SYNC_STATE)p_data->p_le_pa_sync_state_change_info->state,
												 p_data->p_le_pa_sync_state_change_info->sync_transfer_received_flag,
												 p_data->p_le_pa_sync_state_change_info->cause);
		break;
	}
#endif

#if (defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && (defined(F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT) && F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
	case GAP_MSG_LE_PA_SYNC_SET_PERIODIC_ADV_RECEIVE_ENABLE: {
		break;
	}

	case GAP_MSG_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS: {
		BT_LOGD("GAP_MSG_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS: cause 0x%x\r\n",
				p_data->p_le_past_recipient_set_default_periodic_adv_sync_transfer_params_rsp->cause);
		break;
	}

	case GAP_MSG_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS: {
		BT_LOGD("GAP_MSG_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS: cause 0x%x, conn_id %d\r\n",
				p_data->p_le_past_recipient_set_periodic_adv_sync_transfer_params_rsp->cause,
				p_data->p_le_past_recipient_set_periodic_adv_sync_transfer_params_rsp->conn_id);
		break;
	}

	case GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO: {
		T_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO *p_info = p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info;
		BT_LOGD("GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO: ");
		BT_LOGD("cause 0x%x, conn_id %d, sync_id %d, sync_handle 0x%x, service_data 0x%x, ",
				p_info->cause, p_info->conn_id, p_info->sync_id, p_info->sync_handle, p_info->service_data);
		BT_LOGD(" adv_sid 0x%x, adv_addr_type %d, adv_addr %02x:%02x:%02x:%02x:%02x:%02x ",
				p_info->adv_sid, p_info->adv_addr_type, p_info->adv_addr[5], p_info->adv_addr[4],
				p_info->adv_addr[3], p_info->adv_addr[2], p_info->adv_addr[1], p_info->adv_addr[0]);
		BT_LOGD("adv_phy %d, periodic_adv_interval 0x%x, adv_clock_accuracy %d",
				p_info->adv_phy, p_info->periodic_adv_interval, p_info->adv_clock_accuracy);

		le_past_recv_info[p_info->conn_id].sync_handle = p_info->sync_handle;
		le_past_recv_info[p_info->conn_id].service_data = p_info->service_data;
		break;
	}
#endif

#if (defined(RTK_BLE_5_1_PAST_SENDER_SUPPORT) && RTK_BLE_5_1_PAST_SENDER_SUPPORT) && (defined(F_BT_LE_5_1_PAST_SENDER_SUPPORT) && F_BT_LE_5_1_PAST_SENDER_SUPPORT)
	case GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER: {
		BT_LOGD("GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER: cause 0x%x, conn_id 0x%x",
				p_data->p_le_past_sender_periodic_adv_sync_transfer_rsp->cause,
				p_data->p_le_past_sender_periodic_adv_sync_transfer_rsp->conn_id);
		break;
	}

	case GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER: {
		BT_LOGD("GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER: cause 0x%x, conn_id 0x%x",
				p_data->p_le_past_sender_periodic_adv_set_info_transfer_rsp->cause,
				p_data->p_le_past_sender_periodic_adv_set_info_transfer_rsp->conn_id);
		break;
	}

#endif

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
(defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT) && \
(!defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV)
	case GAP_MSG_LE_EXT_ADV_START_SETTING: {
		BT_LOGD("GAP_MSG_LE_EXT_ADV_START_SETTING:cause 0x%x, flag 0x%x, adv_handle %d\r\n",
				p_data->p_le_ext_adv_start_setting_rsp->cause, p_data->p_le_ext_adv_start_setting_rsp->flag,
				p_data->p_le_ext_adv_start_setting_rsp->adv_handle);

		/* start_setting is called by bt_stack_le_gap_start_ext_adv */
		p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_GAP_ACT_START_EXT_ADV << 8);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			if (p_data->p_le_ext_adv_start_setting_rsp->cause == GAP_SUCCESS) {
				p_cmd->user_data = GAP_MSG_LE_EXT_ADV_ENABLE;
				bt_stack_pending_cmd_insert(p_cmd);
				le_ext_adv_enable(1, &p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
			} else {
				p_cmd->ret = p_data->p_le_ext_adv_start_setting_rsp->cause;
				osif_sem_give(p_cmd->psem);
			}
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
			if (rtk_bt_mesh_is_enable()) {
				// APP_PRINT_INFO3("GAP_MSG_LE_EXT_ADV_START_SETTING:cause 0x%x, flag 0x%x, adv_handle %d",
				//              p_data->p_le_ext_adv_start_setting_rsp->cause, p_data->p_le_ext_adv_start_setting_rsp->flag,
				//              p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
				/* Enable one advertising set */
				gap_sched_ext_adv_step(p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
			}
#endif
			break;
		}

		/* start_setting is called by _ext_adv_param_take_effect */
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_ext_adv_start_setting_rsp->cause;
			osif_sem_give(p_cmd->psem);
		}

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
		if (rtk_bt_mesh_is_enable()) {
			// APP_PRINT_INFO3("GAP_MSG_LE_EXT_ADV_START_SETTING:cause 0x%x, flag 0x%x, adv_handle %d",
			//              p_data->p_le_ext_adv_start_setting_rsp->cause, p_data->p_le_ext_adv_start_setting_rsp->flag,
			//              p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
			/* Enable one advertising set */
			gap_sched_ext_adv_step(p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
		}
#endif

		break;
	}

	case GAP_MSG_LE_EXT_ADV_REMOVE_SET: {
		uint16_t cause = p_data->p_le_ext_adv_remove_set_rsp->cause;
		uint8_t handle = p_data->p_le_ext_adv_remove_set_rsp->adv_handle;
		BT_LOGD("GAP_MSG_LE_EXT_ADV_REMOVE_SET:cause 0x%x, adv_handle %d", cause, handle);

		if (cause == GAP_SUCCESS) {
			int idx;
			if (bt_stack_le_legacy_adv_hdl == handle) {
				bt_stack_le_legacy_adv_hdl = RTK_GAP_INVALID_ADV_HANDLE;
			}

			for (idx = 0; idx < GAP_MAX_EXT_ADV_SETS; idx++) {
				if (bt_stack_ext_adv_tbl[idx].used &&
					bt_stack_ext_adv_tbl[idx].adv_handle == handle) {
					bt_stack_ext_adv_tbl[idx].used = false;
					bt_stack_ext_adv_tbl[idx].adv_handle = RTK_GAP_INVALID_ADV_HANDLE;
					bt_stack_ext_adv_tbl[idx].ext_adv_state = EXT_ADV_STATE_IDLE;
#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
					bt_stack_ext_adv_tbl[idx].pa_state = PA_ADV_STATE_IDLE;
#endif
				}
			}
		}

		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = cause;
			osif_sem_give(p_cmd->psem);
		}
		break;
	}

	case GAP_MSG_LE_EXT_ADV_ENABLE: {
		BT_LOGD("GAP_MSG_LE_EXT_ADV_ENABLE:cause 0x%x\r\n", p_data->le_cause.cause);

		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			// For rtk stack extended ADV:
			// le_ext_adv_set_adv_enable_param --> le_ext_adv_start_setting --> wait GAP_MSG_LE_EXT_ADV_START_SETTING --> le_ext_adv_enable --> wait GAP_MSG_LE_EXT_ADV_ENABLE
			// For rtk coded phy mesh stack extended ADV:
			// le_ext_adv_set_adv_enable_param --> gap_sched_ext_adv_set_param_flag --> gap_sched_ext_adv_start --> auto enter GAP_MSG_LE_EXT_ADV_START_SETTING GAP_MSG_LE_EXT_ADV_ENABLE
			// So when using mesh coded phy, will auto enter this case
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
			if (!rtk_bt_mesh_is_enable()) {
				BT_LOGE("[%s] GAP_MSG_LE_EXT_ADV_ENABLE: find no pending command \r\n", __func__);
			}
#else
			BT_LOGE("[%s] GAP_MSG_LE_EXT_ADV_ENABLE: find no pending command \r\n", __func__);
#endif
		}
		break;
	}

#if (defined(F_BT_LE_GAP_MSG_INFO_WAY) && F_BT_LE_GAP_MSG_INFO_WAY) && \
(!defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV)
	case GAP_MSG_LE_EXT_ADV_STATE_CHANGE_INFO: {
		BT_LOGD("GAP_MSG_LE_EXT_ADV_STATE_CHANGE_INFO \r\n");
		bt_stack_le_gap_handle_ext_adv_state_evt(p_data->p_le_ext_adv_state_change_info->adv_handle,
												 (T_GAP_EXT_ADV_STATE)p_data->p_le_ext_adv_state_change_info->state,
												 p_data->p_le_ext_adv_state_change_info->cause);
		break;
	}
#endif

#endif

#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
	case GAP_MSG_LE_PA_ADV_START_SETTING: {
		BT_LOGD("GAP_MSG_LE_PA_ADV_START_SETTING: cause 0x%x, adv_handle 0x%x, flag 0x%x\r\n",
				p_data->p_le_pa_adv_start_setting_rsp->cause,
				p_data->p_le_pa_adv_start_setting_rsp->adv_handle,
				p_data->p_le_pa_adv_start_setting_rsp->flag);

		p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_GAP_ACT_START_PA << 8);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			if (p_data->p_le_pa_adv_start_setting_rsp->cause == GAP_SUCCESS) {
				p_cmd->user_data = GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE;
				bt_stack_pending_cmd_insert(p_cmd);
				le_pa_adv_set_periodic_adv_enable(p_data->p_le_pa_adv_start_setting_rsp->adv_handle,
												  PA_ADV_ENABLE_ENABLE_PERIODIC_ADVERTISING | PA_ADV_ENABLE_INCLUDE_ADI);
			} else {
				p_cmd->ret = p_data->p_le_pa_adv_start_setting_rsp->cause;
				osif_sem_give(p_cmd->psem);
			}
			break;
		}

		p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_GAP_ACT_UPDATE_PA << 8);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_pa_adv_start_setting_rsp->cause;
			osif_sem_give(p_cmd->psem);
		}

		break;
	}

	case GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE: {
		BT_LOGD("GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE: cause 0x%x, adv_handle 0x%x, value of enable param 0x%x, enable periodic advertising %d, include ADI %d\r\n",
				p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->cause,
				p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->adv_handle,
				p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->enable,
				p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->enable &
				PA_ADV_ENABLE_ENABLE_PERIODIC_ADVERTISING,
				p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->enable & PA_ADV_ENABLE_INCLUDE_ADI);

		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_PA_ADV_STATE_CHANGE_INFO: {
		BT_LOGD("GAP_MSG_LE_PA_ADV_STATE_CHANGE_INFO \r\n");
		bt_stack_le_gap_handle_pa_state_evt(p_data->p_le_pa_adv_state_change_info->adv_handle,
											(T_GAP_PA_ADV_STATE)p_data->p_le_pa_adv_state_change_info->state,
											p_data->p_le_pa_adv_state_change_info->cause);
		break;
	}
#endif

	case GAP_MSG_LE_RF_INFO: {
		BT_LOGD("GAP_MSG_LE_RF_INFO\r\n");
		T_GAP_LE_RF_CB *p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

		switch (p_le_rf_cb->opcode) {
#if (defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && (defined(F_BT_LE_5_2_POWER_CONTROL_SUPPORT) && F_BT_LE_5_2_POWER_CONTROL_SUPPORT)
		case GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL: {
			T_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL_RSP *rsp = p_le_rf_cb->data.p_le_rf_enhanced_read_transmit_power_level_rsp;
			BT_LOGD("GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL:conn id %d, cause 0x%x, phy %d, cur trans power level %d, max trans power level %d\r\n",
					rsp->conn_id, rsp->cause, rsp->phy, rsp->cur_tx_power_level, rsp->max_tx_power_level);
			p_cmd = bt_stack_pending_cmd_search(p_le_rf_cb->opcode | (type << 8));
			if (p_cmd) {
				rtk_bt_le_txpower_read_local_t *param = p_cmd->param;
				if (param->conn_handle == le_get_conn_handle(rsp->conn_id) && param->phy == (rtk_bt_le_txpower_phy_t)rsp->phy) {
					*param->cur_txpower = rsp->cur_tx_power_level;
					*param->max_txpower = rsp->max_tx_power_level;
				}
				p_cmd->ret = rsp->cause;
				bt_stack_pending_cmd_delete(p_cmd);
				osif_sem_give(p_cmd->psem);
			} else {
				BT_LOGE("[%s] GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL: find no pending command \r\n", __func__);
			}
			break;
		}

		case GAP_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL: {
			T_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL_RSP *rsp = p_le_rf_cb->data.p_le_rf_read_remote_transmit_power_level_rsp;
			BT_LOGD("GAP_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL:conn id %d, cause 0x%x\r\n", rsp->conn_id, rsp->cause);
			p_cmd = bt_stack_pending_cmd_search(p_le_rf_cb->opcode | (type << 8));
			if (p_cmd) {
				rtk_bt_le_txpower_read_remote_t *param = p_cmd->param;
				if (param->conn_handle == le_get_conn_handle(rsp->conn_id)) {
					p_cmd->ret = rsp->cause;
					bt_stack_pending_cmd_delete(p_cmd);
					osif_sem_give(p_cmd->psem);
				}
			} else {
				BT_LOGE("[%s] GAP_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL: find no pending command \r\n", __func__);
			}
			break;
		}

		case GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE: {
			T_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE_RSP *rsp = p_le_rf_cb->data.p_le_rf_set_transmit_power_reporting_enable_rsp;
			BT_LOGD("GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE:conn id %d, cause 0x%x\r\n", rsp->conn_id, rsp->cause);
			p_cmd = bt_stack_pending_cmd_search(p_le_rf_cb->opcode | (type << 8));
			if (p_cmd) {
				rtk_bt_le_txpower_report_set_t *param = p_cmd->param;
				if (param->conn_handle == le_get_conn_handle(rsp->conn_id)) {
					p_cmd->ret = rsp->cause;
					bt_stack_pending_cmd_delete(p_cmd);
					osif_sem_give(p_cmd->psem);
				}
			} else {
				BT_LOGE("[%s] GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE: find no pending command \r\n", __func__);
			}
			break;
		}

		case GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO: {
			T_LE_RF_TRANSMIT_POWER_REPORTING_INFO *report = p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info;
			BT_LOGD("GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO:conn id %d, cause 0x%x, reason 0x%x, phy %d, transmit power level %d, transmit power level flag %d, delta %d\r\n",
					report->conn_id, report->cause, report->reason, report->phy, report->tx_power_level, report->tx_power_level_flag, report->delta);

			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
										RTK_BT_LE_GAP_EVT_TXPOWER_REPORT_IND,
										sizeof(rtk_bt_le_txpower_ind_t));
			rtk_bt_le_txpower_ind_t *p_ind = (rtk_bt_le_txpower_ind_t *)p_evt->data;
			p_ind->type = report->reason;
			p_ind->conn_handle = le_get_conn_handle(report->conn_id);
			p_ind->phy = (rtk_bt_le_txpower_phy_t)report->phy;
			p_ind->txpower = report->tx_power_level;
			p_ind->flag = report->tx_power_level_flag;
			rtk_bt_evt_indicate(p_evt, NULL);
			break;
		}
#endif
		default:
			break;
		}
		break;
	}

#if defined(F_BT_LE_4_0_DTM_SUPPORT) && F_BT_LE_4_0_DTM_SUPPORT
	case GAP_MSG_LE_DTM_RECEIVER_TEST: {
		BT_LOGD("GAP_MSG_LE_DTM_RECEIVER_TEST: cause 0x%x\r\n", p_data->le_cause.cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_RECEIVER_TEST: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_DTM_TRANSMITTER_TEST: {
		BT_LOGD("GAP_MSG_LE_DTM_TRANSMITTER_TEST: cause 0x%x\r\n", p_data->le_cause.cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_TRANSMITTER_TEST: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_DTM_TEST_END: {
		BT_LOGD("GAP_MSG_LE_DTM_TEST_END: cause 0x%x\r\n", p_data->p_le_dtm_test_end_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->p_le_dtm_test_end_rsp->cause;
			*((uint16_t *)p_cmd->param) = p_data->p_le_dtm_test_end_rsp->num_pkts;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_TRANSMITTER_TEST: find no pending command \r\n", __func__);
		}
		break;
	}

#if defined(F_BT_LE_5_0_DTM_SUPPORT) && F_BT_LE_5_0_DTM_SUPPORT
	case GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST: {
		BT_LOGD("GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST: cause 0x%x\r\n", p_data->le_cause.cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST: {
		BT_LOGD("GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST: cause 0x%x\r\n", p_data->le_cause.cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST: find no pending command \r\n", __func__);
		}
		break;
	}
#endif

#if defined(F_BT_LE_5_1_DTM_SUPPORT) && F_BT_LE_5_1_DTM_SUPPORT
	case GAP_MSG_LE_DTM_RECEIVER_TEST_V3: {
		BT_LOGD("GAP_MSG_LE_DTM_RECEIVER_TEST_V3: cause 0x%x\r\n", p_data->le_cause.cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_RECEIVER_TEST_V3: find no pending command \r\n", __func__);
		}
		break;
	}

	case GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3: {
		BT_LOGD("GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3: cause 0x%x\r\n", p_data->le_cause.cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3: find no pending command \r\n", __func__);
		}
		break;
	}
#endif

#if defined(F_BT_LE_5_2_DTM_SUPPORT) && F_BT_LE_5_2_DTM_SUPPORT
	case GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4: {
		BT_LOGD("GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4: cause 0x%x\r\n", p_data->le_cause.cause);
		p_cmd = bt_stack_pending_cmd_search(type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = p_data->le_cause.cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4: find no pending command \r\n", __func__);
		}
		break;
	}
#endif
#endif /* F_BT_LE_4_0_DTM_SUPPORT */

	case GAP_MSG_LE_GAP_STATE_MSG: {
		T_IO_MSG *io_msg = (T_IO_MSG *)p_data->p_gap_state_msg;
		bt_stack_le_gap_handle_io_msg(io_msg->subtype, &io_msg->u.param);
		break;
	}
	default:
		break;
	}

	return APP_RESULT_SUCCESS;
}

void bt_stack_le_gap_set_config(void *app_conf)
{
	rtk_bt_app_conf_t *papp_conf = (rtk_bt_app_conf_t *)app_conf;
	uint8_t master_init_mtu_req = (uint8_t)papp_conf->master_init_mtu_req;
	uint8_t slave_init_mtu_req = (uint8_t)papp_conf->slave_init_mtu_req;

	/* le_set_gap_param() shall be after bte_init() */
	le_set_gap_param(GAP_PARAM_MASTER_INIT_GATT_MTU_REQ, sizeof(master_init_mtu_req), &master_init_mtu_req);
	le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req), &slave_init_mtu_req);
#if defined(F_BT_LE_5_0_SET_PHYS_SUPPORT) && F_BT_LE_5_0_SET_PHYS_SUPPORT
	le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(papp_conf->prefer_all_phy), &papp_conf->prefer_all_phy);
	le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(papp_conf->prefer_tx_phy), &papp_conf->prefer_tx_phy);
	le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(papp_conf->prefer_rx_phy), &papp_conf->prefer_rx_phy);
#endif

#if defined(F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT) && F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
	le_set_gap_param(GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_OCTETS, sizeof(papp_conf->max_tx_octets), &papp_conf->max_tx_octets);
	le_set_gap_param(GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_TIME, sizeof(papp_conf->max_tx_time), &papp_conf->max_tx_time);
#endif

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
	uint8_t irk_auto_gen = (uint8_t)papp_conf->irk_auto_gen;
	if (irk_auto_gen) {
		le_bond_set_param(GAP_PARAM_BOND_GEN_LOCAL_IRK_AUTO, sizeof(irk_auto_gen), &irk_auto_gen);
	} else {
		le_bond_set_param(GAP_PARAM_BOND_SET_LOCAL_IRK, GAP_KEY_LEN, papp_conf->irk);
	}
#endif
	if (papp_conf->min_enc_key_size) {
		le_bond_set_param(GAP_PARAM_BOND_MIN_KEY_SIZE, sizeof(uint8_t), &papp_conf->min_enc_key_size);
	}
}

void bt_stack_le_gap_ext_adv_init(void)
{
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
    (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT) && \
    (!defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV)
	uint8_t use_ext_adv = true;
	le_set_gap_param(GAP_PARAM_USE_EXTENDED_ADV, sizeof(use_ext_adv), &use_ext_adv);
	le_ext_adv_init(GAP_MAX_EXT_ADV_SETS);
#endif

#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
	le_ext_adv_gap_msg_info_way(false);
	le_pa_adv_init(GAP_MAX_PA_ADV_SETS);
#endif

#if (defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT) && (defined(F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SUPPORT)
	le_ext_scan_gap_msg_info_way(false);
	le_pa_sync_init(GAP_MAX_PA_SYNC_HANDLES);
#endif
}

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT

static uint8_t _get_hci_cte_type(rtk_bt_le_gap_cte_type_t type)
{
	switch (type) {
	case RTK_BT_LE_GAP_CTE_TYPE_AOA:
		return GAP_CTE_TYPE_AOA;
	case RTK_BT_LE_GAP_CTE_TYPE_AOD_1US:
		return GAP_CTE_TYPE_AOD_1US_SLOT;
	case RTK_BT_LE_GAP_CTE_TYPE_AOD_2US:
		return GAP_CTE_TYPE_AOD_2US_SLOT;
	default:
		return 0xFF;
	}
}

static void bt_stack_le_gap_handle_read_antenna_evt(uint8_t type, void *data)
{
	rtk_bt_cmd_t *p_cmd = NULL;
	rtk_bt_le_gap_antenna_info_t *antenna = NULL;
	T_LE_AOX_READ_ANTENNA_INFORMATION_RSP   *p_rsp;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}

	p_rsp = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_read_antenna_information_rsp;
	BT_LOGD("GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION: cause 0x%x, supported_switching_sampling_rates 0x%x, "  \
			"num_antennae %d, max_switching_pattern_length %d, "  \
			"max_cte_length %d\r\n",
			p_rsp->cause,
			p_rsp->supported_switching_sampling_rates,
			p_rsp->num_antennae,
			p_rsp->max_switching_pattern_length,
			p_rsp->max_cte_length);

	p_cmd = bt_stack_pending_cmd_search(type);
	antenna = (rtk_bt_le_gap_antenna_info_t *)p_cmd->param;
	if (p_cmd) {
		bt_stack_pending_cmd_delete(p_cmd);
		p_cmd->ret = p_rsp->cause;
		antenna->supported_switching_sampling_rates = p_rsp->supported_switching_sampling_rates;
		antenna->num_antennae = p_rsp->num_antennae;
		antenna->max_switching_pattern_length = p_rsp->max_switching_pattern_length;
		antenna->max_cte_length = p_rsp->max_cte_length;
		osif_sem_give(p_cmd->psem);
	} else {
		BT_LOGE("GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION: find no pending command \r\n");
	}

}

static void bt_stack_le_gap_handle_connless_cte_rx_enable_evt(uint8_t type, void *data)
{
	rtk_bt_cmd_t *p_cmd = NULL;
	T_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE_RSP *p_rsp;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}

	p_rsp = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_connless_receiver_set_iq_sampling_enable_rsp;
	BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE: "   \
			"cause 0x%x, sync_id %d, sync_handle 0x%x, sampling_enable 0x%x\r\n",
			p_rsp->cause,
			p_rsp->sync_id,
			p_rsp->sync_handle,
			p_rsp->sampling_enable);

	p_cmd = bt_stack_pending_cmd_search(type);
	if (p_cmd) {
		bt_stack_pending_cmd_delete(p_cmd);
		p_cmd->ret = p_rsp->cause;
		osif_sem_give(p_cmd->psem);
	} else {
		BT_LOGE("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE: find no pending command \r\n");
	}
}

static uint16_t bt_stack_le_gap_conn_cte_req_enable(uint8_t conn_id, void *param)
{
	rtk_bt_le_gap_conn_cte_rx_t *rx = (rtk_bt_le_gap_conn_cte_rx_t *)param;
	T_GAP_CAUSE cause;

	if (!param) {
		BT_LOGE("%s: invalid param\r\n", __func__);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	if (rx->enable) {
		T_GAP_CTE_TYPE cte_type = _get_hci_cte_type(rx->p_rx_param->req_cte_type);
		cause = le_aox_conn_cte_request_enable(conn_id,
											   rx->enable,
											   rx->p_rx_param->cte_req_interval,
											   rx->p_rx_param->req_cte_len,
											   cte_type);
		BT_LOGD("le_aox_conn_cte_request_enable conn_id=%u, enable=%u, "  \
				"cte_request_interval=%u, requested_cte_length=%u, "      \
				"requested_cte_type=%u\r\n",
				conn_id,
				rx->enable,
				rx->p_rx_param->cte_req_interval,
				rx->p_rx_param->req_cte_len,
				cte_type);
	} else {
		cause = le_aox_conn_cte_request_enable(conn_id,
											   rx->enable,
											   0,
											   0,
											   0);
		BT_LOGD("le_aox_conn_cte_request_enable conn_id=%u, enable=%u\r\n",
				conn_id,
				rx->enable);
	}

	if (cause) {
		BT_LOGE("[LE GAP] connection cte request error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

static void bt_stack_le_gap_handle_conn_cte_set_rx_params_evt(uint8_t type, void *data)
{
	uint16_t ret;
	rtk_bt_cmd_t *p_cmd = NULL;
	T_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS_RSP *p_rsp;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}

	p_rsp = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_set_conn_cte_receive_params_rsp;
	BT_LOGD("GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS: cause 0x%x, conn_id %d\r\n",
			p_rsp->cause, p_rsp->conn_id);

	p_cmd = bt_stack_pending_cmd_search(type);
	if (p_cmd) {
		bt_stack_pending_cmd_delete(p_cmd);
		if (p_rsp->cause == GAP_SUCCESS) {
			p_cmd->user_data = GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE;
			bt_stack_pending_cmd_insert(p_cmd);
			ret = bt_stack_le_gap_conn_cte_req_enable(p_rsp->conn_id, p_cmd->param);
			if (ret) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = ret;
				osif_sem_give(p_cmd->psem);
			}
		} else {
			p_cmd->ret = p_rsp->cause;
			osif_sem_give(p_cmd->psem);
		}
	} else {
		BT_LOGE("GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS: find no pending command\r\n");
	}

}

static void bt_stack_le_gap_handle_conn_cte_req_evt(uint8_t type, void *data)
{
	rtk_bt_cmd_t *p_cmd = NULL;
	T_LE_AOX_CONN_CTE_REQUEST_ENABLE_RSP *p_rsp;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}

	p_rsp = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_conn_cte_request_enable_rsp;
	BT_LOGD("GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE: cause 0x%x, conn_id %d\r\n", p_rsp->cause, p_rsp->conn_id);

	p_cmd = bt_stack_pending_cmd_search(type);
	if (p_cmd) {
		bt_stack_pending_cmd_delete(p_cmd);
		p_cmd->ret = p_rsp->cause;
		osif_sem_give(p_cmd->psem);
	} else {
		BT_LOGE("GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE: find no pending command \r\n");
	}
}

static uint16_t bt_stack_le_gap_conn_cte_rsp_enable(uint8_t conn_id)
{
	T_GAP_CAUSE cause = le_aox_conn_cte_response_enable(conn_id, GAP_AOX_CTE_RESPONSE_ENABLE);

	BT_LOGD("le_aox_conn_cte_response_enable ret=%u, conn_id=%u, enable=%u\r\n",
			cause, conn_id, GAP_AOX_CTE_RESPONSE_ENABLE);

	if (cause) {
		BT_LOGE("[LE GAP] Connection cte request error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static void bt_stack_le_gap_handle_conn_cte_set_tx_param_evt(uint8_t type, void *data)
{
	uint16_t ret;
	rtk_bt_cmd_t *p_cmd = NULL;
	T_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS_RSP *p_rsp;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}
	p_rsp = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_set_conn_cte_transmit_params_rsp;
	BT_LOGD("GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS: cause 0x%x, conn_id %d\r\n", p_rsp->cause, p_rsp->conn_id);

	p_cmd = bt_stack_pending_cmd_search(type);
	if (p_cmd) {
		bt_stack_pending_cmd_delete(p_cmd);
		if (p_rsp->cause == GAP_SUCCESS) {
			p_cmd->user_data = GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE;
			bt_stack_pending_cmd_insert(p_cmd);
			ret = bt_stack_le_gap_conn_cte_rsp_enable(p_rsp->conn_id);
			if (ret) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = ret;
				osif_sem_give(p_cmd->psem);
			}
		} else {
			p_cmd->ret = p_rsp->cause;
			osif_sem_give(p_cmd->psem);
		}
	} else {
		BT_LOGE("GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS: find no pending command \r\n");
	}
}

static void bt_stack_le_gap_handle_conn_cte_rsp_enable_evt(uint8_t type, void *data)
{
	rtk_bt_cmd_t *p_cmd = NULL;
	T_LE_AOX_CONN_CTE_RESPONSE_ENABLE_RSP *p_rsp;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}
	p_rsp = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_conn_cte_response_enable_rsp;
	BT_LOGD("GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE: cause 0x%x, conn_id %d\r\n", p_rsp->cause, p_rsp->conn_id);

	p_cmd = bt_stack_pending_cmd_search(type);
	if (p_cmd) {
		bt_stack_pending_cmd_delete(p_cmd);
		p_cmd->ret = p_rsp->cause;
		osif_sem_give(p_cmd->psem);
	} else {
		BT_LOGE("GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE: find no pending command \r\n");
	}
}

static void bt_stack_le_gap_handle_connless_cte_tx_set_param_evt(uint8_t type, void *data)
{
	rtk_bt_cmd_t *p_cmd = NULL;
	T_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS_RSP *p_rsp;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}
	p_rsp = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_connless_transmitter_set_cte_transmit_params_rsp;
	BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS adv_handle=%u cause=%u\r\n",
			p_rsp->adv_handle, p_rsp->cause);

	p_cmd = bt_stack_pending_cmd_search(type);
	if (p_cmd) {
		bt_stack_pending_cmd_delete(p_cmd);
		p_cmd->ret = p_rsp->cause;
		if (p_rsp->cause == GAP_SUCCESS) {
			// Here only to enable connectionless cte tx. Disable connectionless cte tx used in function bt_stack_le_gap_connless_cte_tx_stop().
			p_cmd->ret = le_aox_connless_transmitter_set_cte_transmit_enable(p_rsp->adv_handle,
																			 AOX_CONNLESS_TRANSMITTER_CTE_ENABLE_ADV_WITH_CTE_ENABLED);
		}
		osif_sem_give(p_cmd->psem);
	} else {
		BT_LOGE("GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS: find no pending command \r\n");
	}

}

static void bt_stack_le_gap_handle_connless_iq_report_evt(void *data)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_le_gap_connless_iq_report_ind_t *p_ind = NULL;
	T_LE_AOX_CONNLESS_RECEIVER_CONNECTIONLESS_IQ_REPORT_INFO *p_info;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}
	p_info = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_connless_receiver_connectionless_iq_report_info;

	BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO: sync_id %d, sync_handle 0x%x, "    \
			"channel_index %d, rssi %d, rssi_antenna_id %d, "     \
			"cte_type %d, slot_durations %d, packet_status %d\r\n",
			p_info->sync_id,
			p_info->sync_handle,
			p_info->channel_index,
			p_info->rssi,
			p_info->rssi_antenna_id,
			p_info->cte_type,
			p_info->slot_durations,
			p_info->packet_status);
	BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO: sync_id %d, sync_handle 0x%x, "    \
			"periodic_event_counter %d, sample_count %d\r\n",
			p_info->sync_id,
			p_info->sync_handle,
			p_info->periodic_event_counter,
			p_info->sample_count);

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
								RTK_BT_LE_GAP_EVT_CONNLESS_IQ_REPORT_IND,
								sizeof(rtk_bt_le_gap_connless_iq_report_ind_t));
	if (!p_evt) {
		return;
	}

	p_ind = (rtk_bt_le_gap_connless_iq_report_ind_t *)p_evt->data;
	p_ind->sync_id                  = p_info->sync_id;
	p_ind->sync_handle              = p_info->sync_handle;
	p_ind->channel_index            = p_info->channel_index;
	p_ind->rssi                     = p_info->rssi;
	p_ind->rssi_antenna_id          = p_info->rssi_antenna_id;
	p_ind->cte_type                 = BIT(p_info->cte_type);
	p_ind->slot_durations           = (rtk_bt_le_gap_slot_durations_e)p_info->slot_durations;
	p_ind->packet_status            = (rtk_bt_le_gap_cte_packet_status_type_e)p_info->packet_status;
	p_ind->periodic_event_counter   = p_info->periodic_event_counter;
	p_ind->sample_count             = p_info->sample_count;
	for (uint8_t i = 0; i < p_ind->sample_count * 2; ++i) {
		p_ind->iq_sample[i] = p_info->p_iq_sample[i];
	}

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_gap_handle_conn_iq_report_evt(void *data)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_le_gap_conn_iq_report_ind_t *p_ind = NULL;
	T_LE_AOX_CONN_IQ_REPORT_INFO *p_info;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}

	p_info = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_conn_iq_report_info;
	BT_LOGD("GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO: conn_id %d, rx_phy %d, "     \
			"data_chan_index %d, rssi %d, rssi_antenna_id %d, "               \
			"cte_type %d, slot_durations %d, packet_status %d\r\n",
			p_info->conn_id,
			p_info->rx_phy,
			p_info->data_chan_index,
			p_info->rssi,
			p_info->rssi_antenna_id,
			p_info->cte_type,
			p_info->slot_durations,
			p_info->packet_status);
	BT_LOGD("GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO: connection_event_counter %d, sample_count %d\r\n",
			p_info->connection_event_counter,
			p_info->sample_count);

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
								RTK_BT_LE_GAP_EVT_CONN_IQ_REPORT_IND,
								sizeof(rtk_bt_le_gap_conn_iq_report_ind_t));
	if (!p_evt) {
		return;
	}

	p_ind = (rtk_bt_le_gap_conn_iq_report_ind_t *)p_evt->data;
	p_ind->conn_handle              = le_get_conn_handle(p_info->conn_id);
	p_ind->rx_phy                   = p_info->rx_phy;
	p_ind->data_chan_index          = p_info->data_chan_index;
	p_ind->rssi                     = p_info->rssi;
	p_ind->rssi_antenna_id          = p_info->rssi_antenna_id;
	p_ind->cte_type                 = BIT(p_info->cte_type);
	p_ind->slot_durations           = (rtk_bt_le_gap_slot_durations_e)p_info->slot_durations;
	p_ind->packet_status            = (rtk_bt_le_gap_cte_packet_status_type_e)p_info->packet_status;
	p_ind->connection_event_counter = p_info->connection_event_counter;
	p_ind->sample_count             = p_info->sample_count;
	for (uint8_t i = 0; i < p_ind->sample_count * 2; ++i) {
		p_ind->iq_sample[i] = p_info->p_iq_sample[i];
	}

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_gap_handle_cte_req_fail_evt(void *data)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_le_gap_cte_req_fail_ind_t *p_ind = NULL;
	T_LE_AOX_CTE_REQUEST_FAILED_INFO *p_info;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}

	p_info = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_cte_request_failed_info;
	BT_LOGD("GAP_MSG_LE_AOX_CTE_REQUEST_FAILED_INFO: cause 0x%x, conn_id %d\r\n",
			p_info->cause,
			p_info->conn_id);

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
								RTK_BT_LE_GAP_EVT_CONN_CTE_REQ_FAIL_IND,
								sizeof(rtk_bt_le_gap_cte_req_fail_ind_t));
	if (!p_evt) {
		return;
	}

	p_ind = (rtk_bt_le_gap_cte_req_fail_ind_t *)p_evt->data;
	p_ind->conn_handle = le_get_conn_handle(p_info->conn_id);
	p_ind->cause = p_info->cause;

	rtk_bt_evt_indicate(p_evt, NULL);
}

void bt_stack_le_gap_handle_connless_tx_state_evt(void *data)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_le_gap_connless_cte_tx_ind_t *p_ind = NULL;
	T_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO *p_info;
	rtk_bt_le_gap_connless_cte_tx_state_e state;

	if (!data) {
		BT_LOGE("%s: invalid data\r\n", __func__);
		return;
	}

	p_info = ((T_LE_AOX_CB_DATA *)data)->p_le_aox_connless_transmitter_state_change_info;

	if (p_info->state == AOX_CONNLESS_TRANSMITTER_STATE_IDLE &&
		p_info->cause == (HCI_ERR | HCI_ERR_OPERATION_CANCELLED_BY_HOST)) {
		BT_LOGD("%s: operation cancelled by host\r\n", __func__);
		return;
	}

	switch (p_info->state) {
	case AOX_CONNLESS_TRANSMITTER_STATE_IDLE:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_IDLE\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_IDLE;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_PA_ADV_STATE_IDLE:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_PA_ADV_STATE_IDLE\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_IDLE;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_EXT_ADV;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_PA_ADV_STATE_IDLE:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_PA_ADV_STATE_IDLE\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_PA_ADV;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_WAIT_PA_ADV_STATE_ADVERTISING:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_WAIT_PA_ADV_STATE_ADVERTISING\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_PA_ADV;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_IDLE:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_IDLE\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_EXT_ADV_IDLE;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_ADVERTISING:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_ADVERTISING\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_EXT_ADV;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_ENABLING\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_ENABLING;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_TRANSMITTING:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_TRANSMITTING\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_TRANSMITTING;
		break;
	case AOX_CONNLESS_TRANSMITTER_STATE_DISABLING:
		BT_LOGD("AOX_CONNLESS_TRANSMITTER_STATE_DISABLING\r\n");
		state = RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_IDLE;
		break;
	default:
		BT_LOGE("Unsupport AOX_CONNLESS_TRANSMITTER_STATE\r\n");
		break;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
								RTK_BT_LE_GAP_EVT_CONNLESS_CTE_TX_STATE_IND,
								sizeof(rtk_bt_le_gap_connless_cte_tx_ind_t));
	if (!p_evt) {
		return;
	}

	p_ind = (rtk_bt_le_gap_connless_cte_tx_ind_t *)p_evt->data;
	p_ind->adv_handle = p_info->adv_handle;
	p_ind->cause = p_info->cause;
	p_ind->state = state;

	rtk_bt_evt_indicate(p_evt, NULL);

}

static T_APP_RESULT bt_stack_le_gap_cte_callback(uint8_t type, void *data)
{
	switch (type) {
	case GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION:
		BT_LOGD("GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION\r\n");
		bt_stack_le_gap_handle_read_antenna_evt(type, data);
		break;

	case GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE:
		BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE\r\n");
		bt_stack_le_gap_handle_connless_cte_rx_enable_evt(type, data);
		break;

	case GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS:
		BT_LOGD("GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS\r\n");
		bt_stack_le_gap_handle_conn_cte_set_rx_params_evt(type, data);
		break;

	case GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS:
		BT_LOGD("GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS\r\n");
		bt_stack_le_gap_handle_conn_cte_set_tx_param_evt(type, data);
		break;

	case GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE:
		BT_LOGD("GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE\r\n");
		bt_stack_le_gap_handle_conn_cte_rsp_enable_evt(type, data);
		break;

	case GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE:
		BT_LOGD("GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE\r\n");
		bt_stack_le_gap_handle_conn_cte_req_evt(type, data);
		break;

	case GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO:
		BT_LOGD("GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO\r\n");
		bt_stack_le_gap_handle_conn_iq_report_evt(data);
		break;

	case GAP_MSG_LE_AOX_CTE_REQUEST_FAILED_INFO:
		BT_LOGD("GAP_MSG_LE_AOX_CTE_REQUEST_FAILED_INFO\r\n");
		bt_stack_le_gap_handle_cte_req_fail_evt(data);
		break;

	case GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS:
		BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS\r\n");
		bt_stack_le_gap_handle_connless_cte_tx_set_param_evt(type, data);
		break;

	case GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO:
		BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO\r\n");
		bt_stack_le_gap_handle_connless_tx_state_evt(data);
		break;

	case GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO:
		BT_LOGD("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO\r\n");
		bt_stack_le_gap_handle_connless_iq_report_evt(data);
		break;

	default:
		BT_LOGE("Unsupport CTE callback type\r\n");
		break;
	}

	return APP_RESULT_SUCCESS;

}

static bool bt_stack_le_gap_cte_init(void)
{
	le_register_aox_cb(bt_stack_le_gap_cte_callback);

	T_GAP_CAUSE cause = le_aox_connless_transmitter_init(GAP_MAX_PA_ADV_SETS);

	if (GAP_CAUSE_SUCCESS != cause) {
		BT_LOGE("[gap cte init]: aox connless transmitterinit fail, cause: 0x%04x\r\n", cause);
		return false;
	}

	return true;
}

#endif /* RTK_BLE_5_1_CTE_SUPPORT */

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
static T_APP_RESULT bt_stack_le_coc_callback(uint8_t coc_msg_type, void *coc_data)
{
	rtk_bt_cmd_t *p_cmd = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	uint16_t conn_handle;

	switch (coc_msg_type) {
	case GAP_COC_MSG_LE_CHANN_STATE: {
		T_LE_COC_CHANN_STATE *chan_state = ((T_LE_COC_DATA *)coc_data)->p_le_chann_state;
		uint8_t evt_id = 0;
		conn_handle = le_get_conn_handle(chan_state->conn_id);
		BT_LOGD("[GAP_COC_MSG_LE_CHANN_STATE] conn_hdl: %d, cid: 0x%x, conn_state: %d, cause: 0x%x\r\n",
				conn_handle, chan_state->cid, chan_state->conn_state, chan_state->cause);

		if ((GAP_CHANN_STATE_CONNECTED == chan_state->conn_state && !chan_state->cause) ||
			(GAP_CHANN_STATE_DISCONNECTED == chan_state->conn_state && chan_state->cause)) {
			/* When host send or receive connection_req, the state is as follows:
			 * idle(disconnected) ======> connecting ======> connected      | connect OK, cause=0 (both coc conn intiator and receiver)
			 * idle(disconnected) ======> connecting ======> disconnected   | connect fail, cause!=0 (only coc conn intiator)
			 */
			evt_id = RTK_BT_LE_GAP_EVT_COC_CONNECT_IND;
		} else if ((GAP_CHANN_STATE_DISCONNECTED == chan_state->conn_state && !chan_state->cause) ||
				   (GAP_CHANN_STATE_CONNECTED == chan_state->conn_state && chan_state->cause)) {
			/* When host send or receive disconnection_req:
			 * connected ======> idle(disconnected)   | disconnect OK, cause=0 (both coc conn intiator and receiver)
			 * connected ======> connected            | disconnect fail? cause!=0 (only coc disconn intiator, almost never happens)
			 */
			evt_id = RTK_BT_LE_GAP_EVT_COC_DISCONNECT_IND;
		} else {
			break;
		}
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, evt_id, sizeof(rtk_bt_le_coc_conn_state_ind_t));
		rtk_bt_le_coc_conn_state_ind_t *state_ind = (rtk_bt_le_coc_conn_state_ind_t *)p_evt->data;
		state_ind->conn_handle = conn_handle;
		state_ind->cid = chan_state->cid;
		state_ind->err = chan_state->cause;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GAP_COC_MSG_LE_REG_PSM: {
		T_LE_COC_CREDIT_BASED_PSM_REG_RSP *reg_psm = ((T_LE_COC_DATA *)coc_data)->p_le_reg_psm_rsp;
		BT_LOGD("[GAP_COC_MSG_LE_REG_PSM] le_psm: 0x%x, cause: 0x%x\r\n", reg_psm->le_psm, reg_psm->cause);
		p_cmd = bt_stack_pending_cmd_search(coc_msg_type);
		if (p_cmd) {
			rtk_bt_le_coc_psm_reg_param_t *reg_param = (rtk_bt_le_coc_psm_reg_param_t *)p_cmd->param;
			if (reg_param->le_psm == reg_psm->le_psm) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = reg_psm->cause;
				osif_sem_give(p_cmd->psem);
			} else {
				BT_LOGE("[GAP_COC_MSG_LE_REG_PSM] Error: le_psm mismatched\r\n");
			}
		}
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_COC_REG_PSM_IND,
									sizeof(rtk_bt_le_coc_send_data_res_ind_t));
		rtk_bt_le_coc_reg_psm_ind_t *reg_psm_ind = (rtk_bt_le_coc_reg_psm_ind_t *)p_evt->data;
		reg_psm_ind->le_psm = reg_psm->le_psm;
		reg_psm_ind->err = reg_psm->cause;
		rtk_bt_evt_indicate(p_evt, NULL);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}
	case GAP_COC_MSG_LE_SET_PSM_SECURITY: {
		T_LE_COC_CREDIT_BASED_SECURITY_REG_RSP *sec_reg = ((T_LE_COC_DATA *)coc_data)->p_le_set_psm_security_rsp;
		BT_LOGD("[GAP_COC_MSG_LE_SET_PSM_SECURITY] cause: 0x%x\r\n", sec_reg->cause);
		p_cmd = bt_stack_pending_cmd_search(coc_msg_type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = sec_reg->cause;
			osif_sem_give(p_cmd->psem);
		}
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_COC_SET_SEC_IND,
									sizeof(rtk_bt_le_coc_send_data_res_ind_t));
		rtk_bt_le_coc_set_sec_ind_t *set_sec_ind = (rtk_bt_le_coc_set_sec_ind_t *)p_evt->data;
		set_sec_ind->err = sec_reg->cause;
		rtk_bt_evt_indicate(p_evt, NULL);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}
	case GAP_COC_MSG_LE_SEND_DATA: {
		T_LE_COC_SEND_DATA *send_data = ((T_LE_COC_DATA *)coc_data)->p_le_send_data;
		conn_handle = le_get_conn_handle(send_data->conn_id);
		BT_LOGD("[GAP_COC_MSG_LE_SEND_DATA] conn_handle: %d, cid: 0x%x, cause: 0x%x, credit: %d\r\n",
				conn_handle, send_data->cid, send_data->cause, send_data->credit);
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_COC_SEND_DATA_RESULT_IND,
									sizeof(rtk_bt_le_coc_send_data_res_ind_t));
		rtk_bt_le_coc_send_data_res_ind_t *res_ind = (rtk_bt_le_coc_send_data_res_ind_t *)p_evt->data;
		res_ind->conn_handle = conn_handle;
		res_ind->cid = send_data->cid;
		res_ind->err = send_data->cause;
		res_ind->credit = send_data->credit;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GAP_COC_MSG_LE_RECEIVE_DATA: {
		T_LE_COC_RECEIVE_DATA *recv_data = ((T_LE_COC_DATA *)coc_data)->p_le_receive_data;
		conn_handle = le_get_conn_handle(recv_data->conn_id);
		BT_LOGD("[GAP_COC_MSG_LE_RECEIVE_DATA] conn_handle: %d, cid: 0x%x, value_len:%d \r\n",
				conn_handle, recv_data->cid, recv_data->value_len);
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_COC_RECEIVE_DATA_IND,
									(sizeof(rtk_bt_le_coc_receive_data_ind_t) + recv_data->value_len));
		rtk_bt_le_coc_receive_data_ind_t *data_ind = (rtk_bt_le_coc_receive_data_ind_t *)p_evt->data;
		data_ind->conn_handle = conn_handle;
		data_ind->cid = recv_data->cid;
		data_ind->len = recv_data->value_len;
		data_ind->data = (uint8_t *)data_ind + sizeof(rtk_bt_le_coc_receive_data_ind_t);
		memcpy(data_ind->data, recv_data->p_data, recv_data->value_len);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	default:
		break;
	}

	return APP_RESULT_SUCCESS;
}

bool bt_stack_le_coc_init(void)
{
	/* the chann_num set in le_coc_init shall less than gap_config_le_l2c_chann_num */
	if (!le_coc_init(GAP_MAX_LE_COC_CHANN_NUM)) {
		return false;
	}
	le_coc_register_app_cb(bt_stack_le_coc_callback);
	return true;
}
#endif /* RTK_BLE_COC_SUPPORT */

uint16_t bt_stack_le_gap_init(void *gap_conf)
{
	if (false == le_gap_init(RTK_BLE_GAP_MAX_LINKS)) {
		return RTK_BT_ERR_NO_RESOURCE;
	}

	le_gap_msg_info_way(false);
	bt_stack_le_gap_ext_adv_init();

	bt_stack_le_gap_set_config(gap_conf);
	le_register_app_cb(bt_stack_le_gap_callback);

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
	if (false == bt_stack_le_gap_cte_init()) {
		return RTK_BT_FAIL;
	}
#endif

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
	if (false == bt_stack_le_coc_init()) {
		return RTK_BT_FAIL;
	}
#endif

	return 0;
}

uint16_t bt_stack_le_gap_wait_ready(void)
{
	T_GAP_DEV_STATE new_state;
	do {
		osif_delay(100);
		le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);
	} while (GAP_INIT_STATE_STACK_READY != new_state.gap_init_state);

	return 0;
}

static void bt_stack_le_gap_handle_dev_state_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_DEV_STATE *new_state = &(p_gap_msg->msg_data.gap_dev_state_change.new_state);
	rtk_bt_le_adv_start_ind_t *p_le_adv_start_ind = NULL;
	rtk_bt_le_adv_stop_ind_t *p_le_adv_stop_ind = NULL;
	uint16_t cause = p_gap_msg->msg_data.gap_dev_state_change.cause;
	/* get the last time dev state */
	uint8_t pre_adv_state = le_gap_dev_state.gap_adv_state;
	uint8_t new_adv_state = new_state->gap_adv_state;
	uint8_t pre_scan_state = le_gap_dev_state.gap_scan_state;
	uint8_t new_scan_state = new_state->gap_scan_state;
	rtk_bt_evt_t *p_evt = NULL;

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
	privacy_handle_resolv_list(false);
#endif

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	if (le_gap_dev_state.gap_init_state != new_state->gap_init_state) {
		if (new_state->gap_init_state == GAP_INIT_STATE_STACK_READY) {
			if (rtk_bt_mesh_is_enable()) {
				BT_LOGD("GAP stack ready\r\n");
				rtk_bt_mesh_set_device_uuid();
			}
		}
	}
#endif

	/* always update dev state */
	le_gap_dev_state = *new_state;
	// BT_LOGD("----------------------> bt_stack_le_gap_handle_dev_state_evt \r\n");
	if (pre_adv_state != new_adv_state) {
		if (GAP_ADV_STATE_IDLE == new_adv_state) {
			if (GAP_ADV_STATE_START == pre_adv_state) {
				/* ADV started failed by user API */
				BT_LOGD("[dev_state_change]: ADV started failed by user API, cause: 0x%04x\r\n", cause);
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_ADV_START_IND,
											sizeof(rtk_bt_le_adv_start_ind_t));
				if (!p_evt) {
					return;
				}

				p_le_adv_start_ind = (rtk_bt_le_adv_start_ind_t *)p_evt->data;
				p_le_adv_start_ind->err = cause;
				rtk_bt_evt_indicate(p_evt, NULL);
			} else if (GAP_ADV_STATE_STOP == pre_adv_state) {
				/* ADV stopped success by user API*/
				BT_LOGD("[dev_state_change]: ADV stopped success by user API\r\n");
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_ADV_STOP_IND,
											sizeof(rtk_bt_le_adv_stop_ind_t));
				if (!p_evt) {
					return;
				}

				p_le_adv_stop_ind = (rtk_bt_le_adv_stop_ind_t *)p_evt->data;
				p_le_adv_stop_ind->err = 0;
				rtk_bt_evt_indicate(p_evt, NULL);
			} else if (GAP_ADV_TO_IDLE_CAUSE_CONN == new_state->gap_adv_sub_state) {
				/* ADV stopped success due to connection establish */
				BT_LOGD("[dev_state_change]: ADV stopped success due to connection establish\r\n");
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_ADV_STOP_IND,
											sizeof(rtk_bt_le_adv_stop_ind_t));
				if (!p_evt) {
					return;
				}

				p_le_adv_stop_ind = (rtk_bt_le_adv_stop_ind_t *)p_evt->data;
				p_le_adv_stop_ind->err = 0;
				p_le_adv_stop_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_CONN;
				rtk_bt_evt_indicate(p_evt, NULL);
			} else if (GAP_ADV_TO_IDLE_CAUSE_STOP == new_state->gap_adv_sub_state) {
				/* ADV stopped success due to high duty adv timeout */
				BT_LOGD("[dev_state_change]: ADV stopped success due to high duty adv timeout\r\n");
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_ADV_STOP_IND,
											sizeof(rtk_bt_le_adv_stop_ind_t));
				if (!p_evt) {
					return;
				}

				p_le_adv_stop_ind = (rtk_bt_le_adv_stop_ind_t *)p_evt->data;
				p_le_adv_stop_ind->err = 0;
				p_le_adv_stop_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_DURATION;
				rtk_bt_evt_indicate(p_evt, NULL);
			}
		} else if (GAP_ADV_STATE_ADVERTISING == new_adv_state) {
			if (GAP_ADV_STATE_START == pre_adv_state) {
				/* ADV started success by user API */
				BT_LOGD("[dev_state_change]: ADV started success by user API\r\n");
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_ADV_START_IND,
											sizeof(rtk_bt_le_adv_start_ind_t));
				if (!p_evt) {
					return;
				}

				p_le_adv_start_ind = (rtk_bt_le_adv_start_ind_t *)p_evt->data;
				p_le_adv_start_ind->err = 0;
				le_adv_get_param(GAP_PARAM_ADV_EVENT_TYPE, &(p_le_adv_start_ind->adv_type));
				rtk_bt_evt_indicate(p_evt, NULL);
			} else if (GAP_ADV_STATE_STOP == pre_adv_state) {
				/* ADV stopped failed by user API */
				BT_LOGD("[dev_state_change]: ADV stopped failed by user API, cause: 0x%x\r\n", cause);
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_ADV_STOP_IND,
											sizeof(rtk_bt_le_adv_stop_ind_t));
				if (!p_evt) {
					return;
				}

				p_le_adv_stop_ind = (rtk_bt_le_adv_stop_ind_t *)p_evt->data;
				p_le_adv_stop_ind->err = cause;
				rtk_bt_evt_indicate(p_evt, NULL);
			}
		}
	}

	bt_stack_le_gap_handle_scan_state_evt(pre_scan_state, new_scan_state, cause);
}

static void bt_stack_le_gap_handle_conn_state_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_CONN_STATE_CHANGE *conn_state = &(p_gap_msg->msg_data.gap_conn_state_change);
	uint8_t i;
	uint8_t conn_id = conn_state->conn_id;
	uint8_t new_state = conn_state->new_state;
	/* get the last time conn state */
	uint8_t prev_state = bt_stack_le_link_tbl[conn_id].conn_state;
	uint16_t disc_cause = conn_state->disc_cause;
	uint16_t conn_handle;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_le_conn_ind_t *p_conn_ind;
	rtk_bt_le_disconn_ind_t *p_disconn_ind;
	T_GAP_CONN_INFO conn_info = {(T_GAP_CONN_STATE)0, (T_GAP_ROLE)0, {0}, 0};
	T_GAP_DEV_STATE dev_state;
	T_GAP_CAUSE cause;

	/* always update conn_state */
	bt_stack_le_link_tbl[conn_id].conn_state = new_state;

	// BT_LOGD("----------------------------> bt_stack_le_gap_handle_conn_state_evt \r\n");
	switch (new_state) {
	case GAP_CONN_STATE_DISCONNECTED:
		if (GAP_CONN_STATE_DISCONNECTING == prev_state ||
			GAP_CONN_STATE_CONNECTED == prev_state ||
			GAP_CONN_STATE_SLAVE_CONNECTING == prev_state) {
			BT_LOGD("[conn_state_evt]: disconnected, conn_id: %d, disconnect reason: 0x%x\r\n",
					conn_id, disc_cause);

			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_DISCONN_IND,
										sizeof(rtk_bt_le_disconn_ind_t));
			if (!p_evt) {
				return;
			}

			p_disconn_ind = (rtk_bt_le_disconn_ind_t *)p_evt->data;
			p_disconn_ind->reason = disc_cause;
			if (GAP_CONN_STATE_SLAVE_CONNECTING == prev_state) {
				p_disconn_ind->conn_handle = le_get_conn_handle(conn_id);
				if (!le_get_conn_info(conn_id, &conn_info)) {
					BT_LOGE("le_get_conn_info failed\r\n");
				}
				p_disconn_ind->role = convert_rtk_link_role(conn_info.role);
				p_disconn_ind->peer_addr.type = (rtk_bt_le_addr_type_t)conn_info.remote_bd_type;
				memcpy(p_disconn_ind->peer_addr.addr_val, conn_info.remote_bd, RTK_BD_ADDR_LEN);
			} else {
				p_disconn_ind->conn_handle = bt_stack_le_conn_handle[conn_id];
				p_disconn_ind->role = convert_rtk_link_role(bt_stack_le_link_tbl[conn_id].role);
				p_disconn_ind->peer_addr.type = (rtk_bt_le_addr_type_t)bt_stack_le_link_tbl[conn_id].bd_type;
				memcpy(p_disconn_ind->peer_addr.addr_val, &bt_stack_le_link_tbl[conn_id].peer_addr, RTK_BD_ADDR_LEN);
			}

			memset(&bt_stack_le_link_tbl[conn_id], 0, sizeof(bt_stack_le_link_info_t));
			if (RTK_BT_LE_ROLE_SLAVE == p_disconn_ind->role && bt_stack_profile_check(RTK_BT_PROFILE_GATTS)) {
				bt_stack_gatts_disconnect_queue_clear(conn_id);
			}

			rtk_bt_evt_indicate(p_evt, NULL);
		} else if (GAP_CONN_STATE_CONNECTING == prev_state) {
			/* connect failed by user API(only reported in central), and no conn_id because it's failed,
			connection fail event will only be reported by controller to host that
			initate the connection(central), spec5.0 p1190 */
			BT_LOGD("[conn_state_evt]: connection establish failed as central, reason: 0x%x\r\n", disc_cause);

			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_CONNECT_IND,
										sizeof(rtk_bt_le_conn_ind_t));
			if (!p_evt) {
				return;
			}

			p_conn_ind = (rtk_bt_le_conn_ind_t *)p_evt->data;
			/*  when create connection hci cmd is issued, but cannot establish connection before
			    <rtk_bt_le_create_conn_param_t::scan_timeout>, here will report an timeout err */
			if (GAP_ERR_TOUT  == disc_cause) {
				p_conn_ind->err = RTK_BT_ERR_CREATE_CONN_TIMEOUT;
			} else {
				p_conn_ind->err = disc_cause;
			}
			rtk_bt_evt_indicate(p_evt, NULL);
		}
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(VENDOR_CMD_SET_MESH_INFO_SUPPORT) && VENDOR_CMD_SET_MESH_INFO_SUPPORT
		if (rtk_bt_mesh_is_enable()) {
			T_GAP_LE_MESH_PACKET_PRIORITY mesh_prio = GAP_LE_MESH_PACKET_PRIORITY_LOW;
			le_vendor_set_mesh_packet_priority(mesh_prio);
		}
#endif
		break;

	case GAP_CONN_STATE_CONNECTED:
		BT_LOGD("[conn_state_evt]: connected, conn_id: %d\r\n", conn_id);
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_CONNECT_IND,
									sizeof(rtk_bt_le_conn_ind_t));
		if (!p_evt) {
			return;
		}
		p_conn_ind = (rtk_bt_le_conn_ind_t *)p_evt->data;

		conn_handle = le_get_conn_handle(conn_id);
		if (0xFFFF == conn_handle) {
			/* Here, conn_handle shall be recorded even if it's 0xFFFF, if not recorded,
			bt_stack_le_conn_handle[conn_id] will be regarded as 0 in disconnected event */
			bt_stack_le_conn_handle[conn_id] = conn_handle;
			p_conn_ind->err = RTK_BT_ERR_PARAM_INVALID;
			goto exit;
		}
		/* when conn_handle 16 first comes as conn_id 0, then disconnct; conn_handle 16 may
		comes as conn_id 1 next time. So both bt_stack_le_conn_handle[] of 0 and 1 may record
		the handle 16, cause the mismatch in bt_stack_le_gap_get_conn_id. Clear the old record here.*/
		for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
			if (bt_stack_le_conn_handle[i] == conn_handle) {
				bt_stack_le_conn_handle[i] = 0;
			}
		}
		bt_stack_le_conn_handle[conn_id] = conn_handle;

		if (!le_get_conn_info(conn_id, &conn_info)) {
			p_conn_ind->err = RTK_BT_ERR_PARAM_INVALID;
			goto exit;
		}
		bt_stack_le_link_tbl[conn_id].is_active = 1;
		bt_stack_le_link_tbl[conn_id].role = conn_info.role;
		bt_stack_le_link_tbl[conn_id].is_pairing_initiator = 0;
		bt_stack_le_link_tbl[conn_id].bd_type = (T_GAP_REMOTE_ADDR_TYPE)conn_info.remote_bd_type;
		memcpy(bt_stack_le_link_tbl[conn_id].peer_addr, conn_info.remote_bd, GAP_BD_ADDR_LEN);

		BT_LOGD("GAP_CONN_STATE_CONNECTED:remote_bd %02x:%02x:%02x:%02x:%02x:%02x, remote_addr_type %d\r\n",
				bt_stack_le_link_tbl[conn_id].peer_addr[5],
				bt_stack_le_link_tbl[conn_id].peer_addr[4],
				bt_stack_le_link_tbl[conn_id].peer_addr[3],
				bt_stack_le_link_tbl[conn_id].peer_addr[2],
				bt_stack_le_link_tbl[conn_id].peer_addr[1],
				bt_stack_le_link_tbl[conn_id].peer_addr[0],
				bt_stack_le_link_tbl[conn_id].bd_type);

		p_conn_ind->conn_handle = conn_handle;
		p_conn_ind->role = convert_rtk_link_role(conn_info.role);
		p_conn_ind->peer_addr.type = (rtk_bt_le_addr_type_t)bt_stack_le_link_tbl[conn_id].bd_type;
		memcpy(p_conn_ind->peer_addr.addr_val, bt_stack_le_link_tbl[conn_id].peer_addr, RTK_BD_ADDR_LEN);
		if (le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &p_conn_ind->conn_interval, conn_id) ||
			le_get_conn_param(GAP_PARAM_CONN_LATENCY, &p_conn_ind->conn_latency, conn_id) ||
			le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &p_conn_ind->supv_timeout, conn_id)) {
			p_conn_ind->err = RTK_BT_ERR_PARAM_INVALID;
			goto exit;
		}

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(VENDOR_CMD_SET_MESH_INFO_SUPPORT) && VENDOR_CMD_SET_MESH_INFO_SUPPORT
		if (rtk_bt_mesh_is_enable() && GAP_LINK_ROLE_SLAVE == conn_info.role) {
			T_GAP_LE_MESH_PACKET_PRIORITY mesh_prio = GAP_LE_MESH_PACKET_PRIORITY_HIGH;
			le_vendor_set_mesh_packet_priority(mesh_prio);
		}
#endif

exit:
		rtk_bt_evt_indicate(p_evt, NULL);
		break;

	case GAP_CONN_STATE_CONNECTING:
		BT_LOGD("[conn_state_evt]: connecting \r\n");
		cause = le_get_gap_param(GAP_PARAM_DEV_STATE, &dev_state);
		if (cause) {
			BT_LOGE("GAP get dev state failed, cause: %d\r\n", cause);
		}
		if (dev_state.gap_conn_state != GAP_CONN_DEV_STATE_INITIATING) {
			/* The connecting state is not caused by master create connection, when slave device
			was connected, it will also report the connecting state. */
			bt_stack_le_link_tbl[conn_id].conn_state = GAP_CONN_STATE_SLAVE_CONNECTING;
		}
		break;

	case GAP_CONN_STATE_DISCONNECTING:
		BT_LOGD("[conn_state_evt]: disconnecting \r\n");
		break;

	default:
		BT_LOGE("[conn_state_evt]: unknown conn state: 0x%x \r\n", new_state);
		break;
	}

}

static void bt_stack_le_gap_handle_conn_param_update_evt(T_LE_GAP_MSG *gap_msg)
{
	T_GAP_CONN_PARAM_UPDATE *param_update = &(gap_msg->msg_data.gap_conn_param_update);
	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
											  RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND,
											  sizeof(rtk_bt_le_conn_update_ind_t));
	rtk_bt_le_conn_update_ind_t *update_conn = (rtk_bt_le_conn_update_ind_t *)p_evt->data;
	update_conn->conn_handle = le_get_conn_handle(param_update->conn_id);

	if (GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS == param_update->status) {
		update_conn->err = 0;
		le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &update_conn->conn_interval,
						  param_update->conn_id);
		le_get_conn_param(GAP_PARAM_CONN_LATENCY, &update_conn->conn_latency,
						  param_update->conn_id);
		le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &update_conn->supv_timeout,
						  param_update->conn_id);
	} else if (GAP_CONN_PARAM_UPDATE_STATUS_FAIL == param_update->status) {
		update_conn->err = param_update->cause;
	} else {
		rtk_bt_event_free(p_evt);
		return;
	}

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_gap_handle_conn_mtu_info_evt(T_LE_GAP_MSG *p_gap_msg)
{
	uint8_t conn_id = p_gap_msg->msg_data.gap_conn_mtu_info.conn_id;
	uint16_t conn_handle = le_get_conn_handle(conn_id);
	uint16_t mtu_size = p_gap_msg->msg_data.gap_conn_mtu_info.mtu_size;
	T_GAP_CONN_INFO conn_info = {(T_GAP_CONN_STATE)0, (T_GAP_ROLE)0, {0}, 0};
	bt_stack_le_link_tbl[conn_id].mtu_size = mtu_size;

	if (le_get_conn_info(conn_id, &conn_info)) {
		if (conn_info.role == GAP_LINK_ROLE_MASTER) {
			BT_LOGD("bt_stack_le_gap_handle_conn_mtu_info_evt: MASTER case \r\n");
			bt_stack_gattc_evt_indicate_mtu_exchange(conn_handle, mtu_size);
		} else if (conn_info.role == GAP_LINK_ROLE_SLAVE) {
			BT_LOGD("bt_stack_le_gap_handle_conn_mtu_info_evt: SLAVE case \r\n");
			bt_stack_gatts_evt_indicate_mtu_exchange(conn_handle, mtu_size);
		} else {
			BT_LOGE("bt_stack_le_gap_handle_conn_mtu_info_evt: unknown role %d\r\n", conn_info.role);
		}

	}

}

static bool bt_stack_le_sm_pairing_get_ltk(uint8_t conn_id, uint8_t *dev_ltk_length, uint8_t *dev_ltk)
{
	T_GAP_REMOTE_ADDR_TYPE remote_addr_type = (T_GAP_REMOTE_ADDR_TYPE)0;
	T_LE_KEY_ENTRY *p_entry = NULL;
	T_GAP_CONN_INFO conn_info = {(T_GAP_CONN_STATE)0, (T_GAP_ROLE)0, {0}, 0};
	uint8_t bd_addr[6] = {0};
	uint8_t i = 0;
	bool remote = false;

	if (le_get_conn_info(conn_id, &conn_info)) {
		if (conn_info.role == GAP_LINK_ROLE_MASTER) {
			remote = true;//for central, LTK is remote ley;
		} else if (conn_info.role == GAP_LINK_ROLE_SLAVE) {
			remote = false;//for peripheral, LTK is local key;
		}
	} else {
		BT_LOGE("%s: le_get_conn_info fail\r\n", __func__);
		return false;
	}

	if (le_get_conn_addr(conn_id, bd_addr, (uint8_t *)&remote_addr_type)) {
		p_entry = le_find_key_entry(bd_addr, remote_addr_type);
		if (p_entry) {
			if (le_get_dev_ltk(p_entry, remote, dev_ltk_length, dev_ltk)) {
				BT_LOGD("long term key is 0x");
				for (i = 1; i <= *dev_ltk_length; i++) {
					BT_LOGD("%02x", dev_ltk[*dev_ltk_length - i]); //End size conversion
				}
				BT_LOGD("\r\n");
				return true;
			}
		}
	}
	return false;
}

static rtk_bt_le_sec_level_t convert_security_level(T_GAP_SEC_LEVEL sec_level)
{
	switch (sec_level) {
	case GAP_SEC_LEVEL_UNAUTHEN:
		__attribute__((fallthrough));
	case GAP_SEC_LEVEL_SC_UNAUTHEN:
		return RTK_BT_LE_SEC_LEVEL_UNAUTHEN;
	case GAP_SEC_LEVEL_AUTHEN:
		return RTK_BT_LE_SEC_LEVEL_AUTHEN;
	case GAP_SEC_LEVEL_SC_AUTHEN:
		return RTK_BT_LE_SEC_LEVEL_SC_AUTHEN_128;
	case GAP_SEC_LEVEL_NO:
		__attribute__((fallthrough));
	default:
		return RTK_BT_LE_SEC_LEVEL_NONE;
	}
}

static void bt_stack_le_gap_handle_authen_state_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_AUTHEN_STATE *authen_state = &p_gap_msg->msg_data.gap_authen_state;
	T_GAP_SEC_LEVEL sec_level;
	uint8_t conn_id = authen_state->conn_id;
	uint16_t status = authen_state->status;
	uint8_t ltk_length = 0;
	uint8_t ltk[32] = {0};

	if (authen_state->new_state != GAP_AUTHEN_STATE_COMPLETE) {
		return;
	}

	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND,
											  sizeof(rtk_bt_le_auth_complete_ind_t));
	rtk_bt_le_auth_complete_ind_t *auth_ind = (rtk_bt_le_auth_complete_ind_t *)p_evt->data;
	auth_ind->conn_handle = le_get_conn_handle(conn_id);

	if (GAP_SUCCESS == status) {
		auth_ind->err = 0;
		BT_LOGD("[authen_state_evt]: Auth state change succeed\r\n");
		if (bt_stack_le_sm_pairing_get_ltk(conn_id, &ltk_length, ltk) == false || ltk_length > 32) {
			BT_LOGD("[authen_state_evt]: bt_stack_le_sm_pairing_get_ltk fail\r\n");
		} else {
			auth_ind->dev_ltk_length = ltk_length;
			memcpy(auth_ind->dev_ltk, ltk, ltk_length);
		}
		if (GAP_CAUSE_SUCCESS == le_bond_get_sec_level(conn_id, &sec_level)) {
			auth_ind->sec_level = convert_security_level(sec_level);
		}
	} else {
		auth_ind->err = status;
		bt_stack_le_link_tbl[conn_id].is_pairing_initiator = 0;
		BT_LOGD("[authen_state_change]: Auth state change failed(reason: 0x%x)\r\n", status);
	}

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_gap_handle_auth_passkey_display_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_BOND_PASSKEY_DISPLAY *passkey_display = &p_gap_msg->msg_data.gap_bond_passkey_display;
	uint32_t passkey_value = 0;
	uint8_t conn_id = passkey_display->conn_id;

	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND,
											  sizeof(rtk_bt_le_auth_key_display_ind_t));
	rtk_bt_le_auth_key_display_ind_t *key_disp_ind =
		(rtk_bt_le_auth_key_display_ind_t *)p_evt->data;
	key_disp_ind->conn_handle = le_get_conn_handle(conn_id);
	le_bond_get_display_key(conn_id, &passkey_value);
	key_disp_ind->passkey = passkey_value;
	/* NOTICE: if don't call le_bond_passkey_display_confirm the pairing
	    will failed for timeout */
	le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
	BT_LOGD("[auth_passkey_display]: need app to display passkey, conn_id: %d, passkey: %u\r\n", conn_id, passkey_value);

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_gap_handle_auth_passkey_input_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_BOND_PASSKEY_INPUT *passkey_input =
		&p_gap_msg->msg_data.gap_bond_passkey_input;
	uint8_t conn_id = passkey_input->conn_id;

	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
											  RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND,
											  sizeof(rtk_bt_le_auth_key_input_ind_t));
	rtk_bt_le_auth_key_input_ind_t *key_input_ind =
		(rtk_bt_le_auth_key_input_ind_t *)p_evt->data;
	key_input_ind->conn_handle = le_get_conn_handle(conn_id);
	BT_LOGD("[auth_passkey_input]: need user to input auth passkey, conn_id: %d\r\n", conn_id);

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_gap_handle_auth_oob_input_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_BOND_OOB_INPUT *oob_input = &p_gap_msg->msg_data.gap_bond_oob_input;
	uint8_t conn_id = oob_input->conn_id;

	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
											  RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND,
											  sizeof(rtk_bt_le_auth_oob_input_ind_t));
	rtk_bt_le_auth_oob_input_ind_t *oob_input_ind =
		(rtk_bt_le_auth_oob_input_ind_t *)p_evt->data;
	oob_input_ind->conn_handle = le_get_conn_handle(conn_id);
	BT_LOGD("[auth_oob_input]: need user to input oob tk, conn_id: %d\r\n", conn_id);

	rtk_bt_evt_indicate(p_evt, NULL);
}

/* both side display a passkey, and both indicate app layer to confirm if they are same */
static void bt_stack_le_gap_handle_auth_passkey_confirm_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_BOND_USER_CONF *user_conf = &p_gap_msg->msg_data.gap_bond_user_conf;
	uint32_t passkey_value = 0;
	uint8_t conn_id = user_conf->conn_id;

	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
											  RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND,
											  sizeof(rtk_bt_le_auth_key_cfm_ind_t));
	rtk_bt_le_auth_key_cfm_ind_t *key_cfm_ind =
		(rtk_bt_le_auth_key_cfm_ind_t *)p_evt->data;
	key_cfm_ind->conn_handle = le_get_conn_handle(conn_id);
	le_bond_get_display_key(conn_id, &passkey_value);
	key_cfm_ind->passkey = passkey_value;
	BT_LOGD("[auth_passkey_confirmation]: need user to compare passkey, conn_id: %d, passkey: %u\r\n", conn_id, passkey_value);

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_gap_handle_auth_just_work_evt(T_LE_GAP_MSG *p_gap_msg)
{
	T_GAP_BOND_JUST_WORK_CONF *just_work_conf =
		&p_gap_msg->msg_data.gap_bond_just_work_conf;
	uint8_t conn_id = just_work_conf->conn_id;

	/* if the device is pairing initiator，then confirm directly without indicate app layer */
	if (bt_stack_le_link_tbl[conn_id].is_pairing_initiator) {
		le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
		BT_LOGD("[auth_just_work]: device is pair initiator, auto accept just work pairing, conn_id: %d\r\n", conn_id);
		return;
	}

	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP,
											  RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND,
											  sizeof(rtk_bt_le_auth_pair_cfm_ind_t));
	rtk_bt_le_auth_pair_cfm_ind_t *pair_cfm_ind =
		(rtk_bt_le_auth_pair_cfm_ind_t *)p_evt->data;
	pair_cfm_ind->conn_handle = le_get_conn_handle(conn_id);
	BT_LOGD("[auth_just_work]: device is not pair initiator, need user to confirm whether go on pairing, conn_id: %d\r\n", conn_id);

	rtk_bt_evt_indicate(p_evt, NULL);
}

void bt_stack_le_gap_handle_io_msg(uint16_t gap_type, void *gap_msg)
{
	T_LE_GAP_MSG *p_gap_msg = (T_LE_GAP_MSG *)gap_msg;
	BT_LOGD("-----------------> bt_stack_le_gap_handle_io_msg: gap_type = %d \r\n", gap_type);
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	if (rtk_bt_mesh_is_enable()) {
		mesh_inner_msg_t mesh_inner_msg;
		mesh_inner_msg.type = MESH_BT_STATUS_UPDATE;
		mesh_inner_msg.sub_type = gap_type;
		mesh_inner_msg.parm = *(uint32_t *)gap_msg;
		gap_sched_handle_bt_status_msg(&mesh_inner_msg);
	}
#endif

#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
	ble_audio_handle_gap_msg(gap_type, *p_gap_msg);
#endif

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	ble_mgr_handle_gap_msg(gap_type, p_gap_msg);
#endif

	switch (gap_type) {
	case GAP_MSG_LE_DEV_STATE_CHANGE:
		BT_LOGD("GAP_MSG_LE_DEV_STATE_CHANGE \r\n");
		bt_stack_le_gap_handle_dev_state_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_CONN_STATE_CHANGE:
		BT_LOGD("GAP_MSG_LE_CONN_STATE_CHANGE \r\n");
		bt_stack_le_gap_handle_conn_state_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_CONN_PARAM_UPDATE:
		BT_LOGD("GAP_MSG_LE_CONN_PARAM_UPDATE \r\n");
		bt_stack_le_gap_handle_conn_param_update_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_CONN_MTU_INFO:
		BT_LOGD("GAP_MSG_LE_CONN_MTU_INFO \r\n");
		bt_stack_le_gap_handle_conn_mtu_info_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
		BT_LOGD("GAP_MSG_LE_AUTHEN_STATE_CHANGE \r\n");
		bt_stack_le_gap_handle_authen_state_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
		BT_LOGD("GAP_MSG_LE_BOND_PASSKEY_DISPLAY \r\n");
		bt_stack_le_gap_handle_auth_passkey_display_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_BOND_PASSKEY_INPUT:
		BT_LOGD("GAP_MSG_LE_BOND_PASSKEY_INPUT \r\n");
		bt_stack_le_gap_handle_auth_passkey_input_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_BOND_OOB_INPUT:
		BT_LOGD("GAP_MSG_LE_BOND_OOB_INPUT \r\n");
		bt_stack_le_gap_handle_auth_oob_input_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_BOND_USER_CONFIRMATION:
		BT_LOGD("GAP_MSG_LE_BOND_USER_CONFIRMATION \r\n");
		bt_stack_le_gap_handle_auth_passkey_confirm_evt(p_gap_msg);
		break;
	case GAP_MSG_LE_BOND_JUST_WORK:
		BT_LOGD("GAP_MSG_LE_BOND_JUST_WORK \r\n");
		bt_stack_le_gap_handle_auth_just_work_evt(p_gap_msg);
		break;
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
    (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT) && \
    (!defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV)
	case GAP_MSG_LE_EXT_ADV_STATE_CHANGE:
		BT_LOGD("GAP_MSG_LE_EXT_ADV_STATE_CHANGE \r\n");
		bt_stack_le_gap_handle_ext_adv_state_evt(p_gap_msg->msg_data.gap_ext_adv_state_change.adv_handle,
												 (T_GAP_EXT_ADV_STATE)p_gap_msg->msg_data.gap_ext_adv_state_change.new_state,
												 p_gap_msg->msg_data.gap_ext_adv_state_change.cause);
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
		if (rtk_bt_mesh_is_enable()) {
			gap_sched_handle_ext_adv_state_evt(p_gap_msg->msg_data.gap_ext_adv_state_change.adv_handle,
											   (T_GAP_EXT_ADV_STATE)p_gap_msg->msg_data.gap_ext_adv_state_change.new_state,
											   p_gap_msg->msg_data.gap_ext_adv_state_change.cause);
		}
#endif
		break;
#endif
	default:
		BT_LOGD("unknown GAP msg type: 0x%x \r\n", gap_type);
		break;
	}

}

uint16_t bt_stack_le_gap_deinit(void)
{
	memset((void *)&le_gap_dev_state, 0, sizeof(le_gap_dev_state));
	memset((void *)bt_stack_le_link_tbl, 0, sizeof(bt_stack_le_link_info_t) * RTK_BLE_GAP_MAX_LINKS);
	memset((void *)bt_stack_le_conn_handle, 0, sizeof(uint16_t)*RTK_BLE_GAP_MAX_LINKS);

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
	memset((void *)bt_stack_ext_adv_tbl, 0, sizeof(bt_stack_ext_adv_info_t) * GAP_MAX_EXT_ADV_SETS);
	bt_stack_le_legacy_adv_hdl = RTK_GAP_INVALID_ADV_HANDLE;
#endif

#if (defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT) && (defined(F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SUPPORT)
	memset((void *)&le_gap_pa_sync_dev_state, 0, sizeof(T_GAP_PA_SYNC_DEV_STATE));
#if (defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && (defined(F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT) && F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
	memset((void *)le_past_recv_info, 0, sizeof(bt_stack_le_past_recv_info_t) * RTK_BLE_GAP_MAX_LINKS);
#endif
#endif

	return 0;
}

static uint16_t bt_stack_le_gap_get_version(void *param)
{
	bool ret = false;
	rtk_bt_le_version_info_t *version = (rtk_bt_le_version_info_t *)param;
	T_BT_VERSION_INFO rtk_version = {0};

	ret = gap_get_bt_version_info(&rtk_version);
	if (ret != true) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	version->hci_revision = rtk_version.hci_revision;
	version->lmp_subversion = rtk_version.lmp_subversion;
	version->btgap_revision = rtk_version.btgap_revision;
	version->btgap_buildnum = rtk_version.btgap_buildnum;

	return 0;
}

static uint16_t bt_stack_le_gap_get_device_addr(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_addr_t *paddr = (rtk_bt_le_addr_t *)param;

	paddr->type = RTK_BT_LE_ADDR_TYPE_PUBLIC;
	cause = gap_get_param(GAP_PARAM_BD_ADDR, paddr->addr_val);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_get_device_addr: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_set_device_name(void *param)
{
	T_GAP_CAUSE cause;
	char *pname = (char *)param;

	cause = le_set_gap_param(GAP_PARAM_DEVICE_NAME, strlen((const char *)pname), (void *)pname);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_set_device_name: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_set_apperance(void *param)
{
	T_GAP_CAUSE cause;
	uint16_t *paprc = (uint16_t *)param;
	cause = le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(*paprc), paprc);

	if (cause) {
		BT_LOGE("bt_stack_le_gap_set_apperance: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_set_preferred_conn_param(void *param)
{
	uint16_t conn_interval_min = 0, conn_interval_max = 0, slave_latency = 0, supervision_timeout = 0;

	if (param == NULL) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	rtk_bt_le_preferred_conn_param_t *p_preferred_conn_param = (rtk_bt_le_preferred_conn_param_t *)param;
	conn_interval_min = p_preferred_conn_param->conn_interval_min;
	conn_interval_max = p_preferred_conn_param->conn_interval_max;
	slave_latency = p_preferred_conn_param->slave_latency;
	supervision_timeout = p_preferred_conn_param->supv_timeout;

	gaps_set_peripheral_preferred_conn_param(conn_interval_min, conn_interval_max, slave_latency, supervision_timeout);

	return 0;
}

static rtk_bt_le_rand_addr_type_t bt_stack_rand_addr_type(uint8_t *addr_val)
{
	uint8_t flag_bits = ((addr_val[5] >> 6) & 0x3);

	if (3 == flag_bits) {
		return RTK_BT_LE_RAND_ADDR_STATIC;
	} else if (0 == flag_bits) {
		return RTK_BT_LE_RAND_ADDR_NON_RESOLVABLE;
	} else if (1 == flag_bits) {
		return RTK_BT_LE_RAND_ADDR_RESOLVABLE;
	} else {
		return (rtk_bt_le_rand_addr_type_t) 0xFF;
	}
}

static uint16_t bt_stack_le_gap_set_rand_addr(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_set_rand_addr_t *p_param = (rtk_bt_le_set_rand_addr_t *)param;

	if (p_param->auto_generate) {
		cause = le_gen_rand_addr((T_GAP_RAND_ADDR_TYPE)p_param->type, p_param->addr_val);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
	}

	cause = le_set_rand_addr(p_param->addr_val);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	/*  if set static random addr as local addr, it should also be set as identity addr,
	    otherwise, public addr will be distributed when pairing, may cause problem */
	if (RTK_BT_LE_RAND_ADDR_STATIC == bt_stack_rand_addr_type(p_param->addr_val)) {
		cause = le_cfg_local_identity_address(p_param->addr_val, GAP_IDENT_ADDR_RAND);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
	}

	return 0;
}

static uint16_t bt_stack_le_gap_gen_rand_addr(void *param)
{
	rtk_bt_le_gen_rand_addr_t *gen_rand = (rtk_bt_le_gen_rand_addr_t *)param;
	T_GAP_CAUSE cause;

	cause = le_gen_rand_addr((T_GAP_RAND_ADDR_TYPE)gen_rand->type, gen_rand->p_addr);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_cfg_local_ident_addr(void *param)
{
	rtk_bt_le_ident_addr_t *ident_addr = (rtk_bt_le_ident_addr_t *)param;
	T_GAP_CAUSE cause;

	cause = le_cfg_local_identity_address(ident_addr->addr_val, (T_GAP_IDENT_ADDR_TYPE)ident_addr->type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
static bool bt_stack_le_gap_ext_adv_handle_valid(uint8_t handle)
{
	int idx;

	for (idx = 0; idx < GAP_MAX_EXT_ADV_SETS; idx++) {
		if (bt_stack_ext_adv_tbl[idx].used &&
			bt_stack_ext_adv_tbl[idx].adv_handle == handle) {
			return true;
		}
	}

	return false;
}

static uint16_t bt_stack_le_gap_set_ext_adv_data(void *param)
{
	rtk_bt_le_ext_adv_data_t *padv_data = (rtk_bt_le_ext_adv_data_t *)param;
	T_GAP_CAUSE cause;

	if (!bt_stack_le_gap_ext_adv_handle_valid(padv_data->adv_handle)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	BT_LOGD("bt_stack_le_gap_set_ext_adv_data: handle = %d, len = %d\r\n", padv_data->adv_handle, padv_data->len);

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	cause = ble_ext_adv_mgr_set_adv_data(padv_data->adv_handle, padv_data->len, padv_data->pdata);
#else
	cause = le_ext_adv_set_adv_data(padv_data->adv_handle, padv_data->len, padv_data->pdata);
#endif

	if (cause) {
		BT_LOGE("bt_stack_le_gap_set_ext_adv_data: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_set_ext_scan_rsp_data(void *param)
{
	rtk_bt_le_ext_adv_data_t *pscan_rsp = (rtk_bt_le_ext_adv_data_t *)param;
	T_GAP_CAUSE cause;

	if (!bt_stack_le_gap_ext_adv_handle_valid(pscan_rsp->adv_handle)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	BT_LOGD("bt_stack_le_gap_set_ext_scan_rsp_data: handle = %d, len = %d\r\n", pscan_rsp->adv_handle, pscan_rsp->len);

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	cause = ble_ext_adv_mgr_set_scan_response_data(pscan_rsp->adv_handle, pscan_rsp->len, pscan_rsp->pdata);
#else
	cause = le_ext_adv_set_scan_response_data(pscan_rsp->adv_handle, pscan_rsp->len, pscan_rsp->pdata);
#endif

	if (cause) {
		BT_LOGE("bt_stack_le_gap_set_ext_scan_rsp_data: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
static void bt_stack_ble_ext_adv_callback(uint8_t cb_type, void *p_cb_data)
{
	T_BLE_EXT_ADV_CB_DATA *p_data = (T_BLE_EXT_ADV_CB_DATA *)p_cb_data;
	switch (cb_type) {
	case BLE_EXT_ADV_STATE_CHANGE: {
		T_BLE_EXT_ADV_STATE_CHANGE *p_info = p_data->p_ble_state_change;
		uint8_t adv_handle = p_info->adv_handle;
		rtk_bt_le_ext_adv_ind_t *p_ext_adv_ind = NULL;
		rtk_bt_evt_t *p_evt = NULL;
		T_BLE_EXT_ADV_MGR_STATE new_state = p_info->state;
		uint8_t idx;

		for (idx = 0; idx < GAP_MAX_EXT_ADV_SETS; idx++) {
			if (bt_stack_ext_adv_tbl[idx].used &&
				bt_stack_ext_adv_tbl[idx].adv_handle == adv_handle) {
				bt_stack_ext_adv_tbl[idx].ext_adv_state = new_state;
				break;
			}
		}

		if (idx == GAP_MAX_EXT_ADV_SETS) {
			return;
		}

		BT_LOGD("[BLE_EXT_ADV_STATE_CHANGE]: adv_handle = %d newState = %d\r\n",
				bt_stack_ext_adv_tbl[idx].adv_handle, new_state);

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
		privacy_handle_resolv_list(false);
#endif

		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_EXT_ADV_IND,
									sizeof(rtk_bt_le_ext_adv_ind_t));
		if (!p_evt) {
			return;
		}

		p_ext_adv_ind = (rtk_bt_le_ext_adv_ind_t *)p_evt->data;
		p_ext_adv_ind->adv_handle = adv_handle;
		p_ext_adv_ind->err = 0;
		if (BLE_EXT_ADV_MGR_ADV_DISABLED == new_state) {
			BT_LOGD("[BLE_EXT_ADV_STATE_CHANGE]: Ext ADV stopped, stop_cause: 0x%04x\r\n", p_info->stop_cause);
			if (p_info->stop_cause == BLE_EXT_ADV_STOP_CAUSE_ENABLE_FAILED) {
				BT_LOGD("[BLE_EXT_ADV_STATE_CHANGE]: Ext ADV start fail\r\n");
				p_ext_adv_ind->err = 1;
				p_ext_adv_ind->is_start = true;
			} else {
				p_ext_adv_ind->is_start = false;
				if (p_info->stop_cause == BLE_EXT_ADV_STOP_CAUSE_APP) {
					p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_HOST;
				} else if (p_info->stop_cause == BLE_EXT_ADV_STOP_CAUSE_CONN) {
					p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_CONN;
				} else if (p_info->stop_cause == BLE_EXT_ADV_STOP_CAUSE_TIMEOUT) {
					p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_BY_DURATION;
				} else {
					p_ext_adv_ind->stop_reason = RTK_BT_LE_ADV_STOP_UNKNOWN;
				}
			}
		} else { /*if (BLE_EXT_ADV_MGR_ADV_ENABLED == new_state)*/
			BT_LOGD("[BLE_EXT_ADV_STATE_CHANGE]: Ext ADV started\r\n");
			p_ext_adv_ind->err = 0;
			p_ext_adv_ind->is_start = true;
		}
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case BLE_EXT_ADV_SET_CONN_INFO: {
		// T_BLE_EXT_ADV_SET_CONN_INFO *p_info = p_data->p_ble_conn_info;
		// BT_LOGD("[BLE_EXT_ADV_SET_CONN_INFO]: adv_handle = %d, conn_id = %d\r\n", p_info->adv_handle, p_info->conn_id);
		break;
	}
	default:
		break;
	}
}
#endif

#if !defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV
/* Call this function after set EA parameters to make parametes take effect. */
static uint16_t _ext_adv_param_take_effect(void *param)
{
	rtk_bt_le_ext_adv_create_t *p_create = (rtk_bt_le_ext_adv_create_t *)param;
	rtk_bt_le_ext_adv_param_t *padv_param = p_create->p_adv_param;
	T_GAP_CAUSE cause;
	uint8_t flags = EXT_ADV_SET_ADV_PARAS;

	if (padv_param->own_addr.type == RTK_BT_LE_ADDR_TYPE_RANDOM ||
		padv_param->own_addr.type == RTK_BT_LE_ADDR_TYPE_RPA_RANDOM) {
		flags |= EXT_ADV_SET_RANDOM_ADDR;
	}

	cause = le_ext_adv_start_setting(*p_create->p_adv_handle, flags);
	if (cause) {
		BT_LOGE("le_ext_adv_start_setting cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

static uint16_t bt_stack_le_gap_create_ext_adv(void *param)
{
	rtk_bt_le_ext_adv_create_t *p_create = (rtk_bt_le_ext_adv_create_t *)param;
	rtk_bt_le_ext_adv_param_t *padv_param = p_create->p_adv_param;
	T_GAP_CAUSE cause;
	int idx;
	uint8_t adv_handle;
	uint8_t *random_address = NULL;

	BT_LOGD("adv_event_prop=0x%x\r\n", padv_param->adv_event_prop);
	BT_LOGD("primary_adv_interval_min=%d\r\n", padv_param->primary_adv_interval_min);
	BT_LOGD("primary_adv_interval_max=%d\r\n", padv_param->primary_adv_interval_max);
	BT_LOGD("primary_adv_channel_map=%d\r\n", padv_param->primary_adv_channel_map);
	BT_LOGD("own_address_type=%d\r\n", padv_param->own_addr.type);
	BT_LOGD("own_address=%02x:%02x:%02x:%02x:%02x:%02x\r\n", padv_param->own_addr.addr_val[5], padv_param->own_addr.addr_val[4], padv_param->own_addr.addr_val[3],
			padv_param->own_addr.addr_val[2], padv_param->own_addr.addr_val[1], padv_param->own_addr.addr_val[0]);
	BT_LOGD("peer_address_type=%d\r\n", padv_param->peer_addr.type);
	BT_LOGD("peer_address=%02x:%02x:%02x:%02x:%02x:%02x\r\n", padv_param->peer_addr.addr_val[5], padv_param->peer_addr.addr_val[4],
			padv_param->peer_addr.addr_val[3], padv_param->peer_addr.addr_val[2], padv_param->peer_addr.addr_val[1], padv_param->peer_addr.addr_val[0]);
	BT_LOGD("filter_policy=%d\r\n", padv_param->filter_policy);
	BT_LOGD("tx_power=%d\r\n", padv_param->tx_power);
	BT_LOGD("primary_adv_phy=%d\r\n", padv_param->primary_adv_phy);
	BT_LOGD("secondary_adv_max_skip=%d\r\n", padv_param->secondary_adv_max_skip);
	BT_LOGD("secondary_adv_phy=%d\r\n", padv_param->secondary_adv_phy);
	BT_LOGD("adv_sid=%d\r\n", padv_param->adv_sid);

	for (idx = 0; idx < GAP_MAX_EXT_ADV_SETS; idx++) {
		if (!bt_stack_ext_adv_tbl[idx].used) {
			break;
		}
	}

	if (idx == GAP_MAX_EXT_ADV_SETS) {
		return RTK_BT_ERR_NO_RESOURCE;
	}

	if (padv_param->own_addr.type == RTK_BT_LE_ADDR_TYPE_RANDOM ||
		padv_param->own_addr.type == RTK_BT_LE_ADDR_TYPE_RPA_RANDOM) {
		random_address = padv_param->own_addr.addr_val;
	}

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	cause = ble_ext_adv_mgr_init_adv_params_all(&adv_handle, (uint16_t)padv_param->adv_event_prop,
												padv_param->adv_sid, (uint8_t)padv_param->tx_power,
												padv_param->primary_adv_interval_min, padv_param->primary_adv_interval_max,
												padv_param->primary_adv_channel_map, (T_GAP_PHYS_PRIM_ADV_TYPE)padv_param->primary_adv_phy,
												padv_param->secondary_adv_max_skip, (T_GAP_PHYS_TYPE)padv_param->secondary_adv_phy,
												(T_GAP_LOCAL_ADDR_TYPE)padv_param->own_addr.type,
												(T_GAP_REMOTE_ADDR_TYPE)padv_param->peer_addr.type, padv_param->peer_addr.addr_val,
												(T_GAP_ADV_FILTER_POLICY)padv_param->filter_policy, 0, NULL,
												0, NULL, random_address);
	if (cause) {
		BT_LOGE("ble_ext_adv_mgr_init_adv_params_all cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = ble_ext_adv_mgr_register_callback(bt_stack_ble_ext_adv_callback, adv_handle);
	if (cause) {
		BT_LOGE("ble_ext_adv_mgr_register_callback cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#else
	adv_handle = le_ext_adv_create_adv_handle();
	if (adv_handle == RTK_GAP_INVALID_ADV_HANDLE) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_adv_set_adv_param(adv_handle,
									 padv_param->adv_event_prop,
									 padv_param->primary_adv_interval_min,
									 padv_param->primary_adv_interval_max,
									 padv_param->primary_adv_channel_map,
									 (T_GAP_LOCAL_ADDR_TYPE)padv_param->own_addr.type,
									 (T_GAP_REMOTE_ADDR_TYPE)padv_param->peer_addr.type,
									 padv_param->peer_addr.addr_val,
									 (T_GAP_ADV_FILTER_POLICY)padv_param->filter_policy,
									 (uint8_t)padv_param->tx_power,
									 (T_GAP_PHYS_PRIM_ADV_TYPE)padv_param->primary_adv_phy,
									 padv_param->secondary_adv_max_skip,
									 (T_GAP_PHYS_TYPE)padv_param->secondary_adv_phy,
									 padv_param->adv_sid,
									 0);
	if (cause) {
		BT_LOGE("le_ext_adv_set_adv_param cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	if (random_address) {
		cause = le_ext_adv_set_random(adv_handle, random_address);
		if (cause) {
			BT_LOGE("le_ext_adv_set_random cause = %x \r\n", cause);
			return RTK_BT_ERR_LOWER_STACK_API;
		}
	}

#endif

	bt_stack_ext_adv_tbl[idx].used = true;
	bt_stack_ext_adv_tbl[idx].adv_handle = adv_handle;
#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	bt_stack_ext_adv_tbl[idx].ext_adv_state = BLE_EXT_ADV_MGR_ADV_DISABLED;
#else
	bt_stack_ext_adv_tbl[idx].ext_adv_state = EXT_ADV_STATE_IDLE;
#endif
#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
	bt_stack_ext_adv_tbl[idx].pa_state = PA_ADV_STATE_IDLE;
#endif

	*p_create->p_adv_handle = adv_handle;
	return 0;
}

static uint16_t bt_stack_le_gap_start_ext_adv(void *param)
{
	rtk_bt_le_ext_adv_start_t *p_start = (rtk_bt_le_ext_adv_start_t *)param;
	T_GAP_CAUSE cause;
	uint8_t adv_handle = p_start->adv_handle;
	uint16_t duration = p_start->duration;
	uint8_t num_events = p_start->num_events;

	if (!bt_stack_le_gap_ext_adv_handle_valid(adv_handle)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	if (num_events) {
		BT_LOGE("ble_ext_adv_mgr_enable not support num_events\r\n");
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = ble_ext_adv_mgr_enable(adv_handle, duration);
	if (cause) {
		BT_LOGE("ble_ext_adv_mgr_enable cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#else /* #if RTK_BLE_MGR_LIB_EADV */

	cause = le_ext_adv_set_adv_enable_param(adv_handle, duration, num_events);

	if (cause) {
		BT_LOGE("le_ext_adv_set_adv_enable_param cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
	if (rtk_bt_mesh_is_enable()) {
		if (true == gap_sched_ext_adv_set_param_flag(adv_handle, EXT_ADV_SET_AUTO)) {
			cause = GAP_CAUSE_SUCCESS;
		} else {
			cause = GAP_CAUSE_ERROR_UNKNOWN;
		}
	} else {
		cause = le_ext_adv_start_setting(adv_handle, EXT_ADV_SET_AUTO);
	}
#else  // RTK_BLE_MESH_SUPPORT && RTK_BLE_MESH_BASED_ON_CODED_PHY
	cause = le_ext_adv_start_setting(adv_handle, EXT_ADV_SET_AUTO);
#endif
	if (cause) {
		BT_LOGE("bt_stack_le_gap_start_ext_adv: start_setting cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
	if (rtk_bt_mesh_is_enable()) {
		uint8_t cause;
		cause = gap_sched_ext_adv_start(adv_handle);
		if (cause) {
			BT_LOGE("[%s] Call mesh stack adv start api, return cause:0x%x.\r\n", __func__, cause);
			return RTK_BT_ERR_LOWER_STACK_API;
		}
	}
#endif
#endif /* #if RTK_BLE_MGR_LIB_EADV */
	return 0;
}

static uint16_t bt_stack_le_gap_stop_ext_adv(void *param)
{
	uint8_t handle = *((uint8_t *)param);
	T_GAP_CAUSE cause;
	int idx;

	if (!bt_stack_le_gap_ext_adv_handle_valid(handle)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	for (idx = 0; idx < GAP_MAX_EXT_ADV_SETS; idx++) {
		if (bt_stack_ext_adv_tbl[idx].used &&
			bt_stack_ext_adv_tbl[idx].adv_handle == handle) {
#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
			if (bt_stack_ext_adv_tbl[idx].ext_adv_state == BLE_EXT_ADV_MGR_ADV_DISABLED)
#else
			if (bt_stack_ext_adv_tbl[idx].ext_adv_state == EXT_ADV_STATE_IDLE)
#endif
			{
				return RTK_BT_ERR_ALREADY_DONE;
			}
		}
	}

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	cause = ble_ext_adv_mgr_disable(handle, 0);
#else
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
	if (rtk_bt_mesh_is_enable()) {
		cause = gap_sched_ext_adv_stop(handle);
	} else {
		cause = le_ext_adv_disable(1, &handle);
	}
#else  // RTK_BLE_MESH_SUPPORT
	cause = le_ext_adv_disable(1, &handle);
#endif
#endif
	if (cause) {
		BT_LOGE("bt_stack_le_gap_stop_ext_adv: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return 0;
}

static uint16_t bt_stack_le_gap_remove_ext_adv(void *param)
{
#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;//ble_mgr.a has no remove API.
#else
	uint8_t handle = *((uint8_t *)param);
	T_GAP_CAUSE cause;

	if (!bt_stack_le_gap_ext_adv_handle_valid(handle)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_ext_adv_remove_set(handle);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_remove_ext_adv: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#endif
}

static uint16_t bt_stack_le_gap_get_ext_adv_handle_by_conn_handle(void *param)
{
	rtk_bt_le_get_eadv_by_conn_handle_param_t *get_eadv_hdl = (rtk_bt_le_get_eadv_by_conn_handle_param_t *)param;
	uint8_t conn_id = 0;
	uint8_t adv_handle = 0;

	if (!le_get_conn_id_by_handle(get_eadv_hdl->conn_handle, &conn_id)) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	adv_handle = le_ext_adv_get_adv_handle_by_conn_id(conn_id);
	if (0xFF == adv_handle) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	*(get_eadv_hdl->adv_handle) = adv_handle;

	return 0;
}
#endif /* RTK_BLE_5_0_USE_EXTENDED_ADV && F_BT_LE_5_0_AE_ADV_SUPPORT */

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
static uint16_t bt_stack_le_gap_ext_connect(void *param)
{
	rtk_bt_le_ext_create_conn_param_t *p_ext_conn_param =
		(rtk_bt_le_ext_create_conn_param_t *)param;
	T_GAP_CAUSE cause;
	T_GAP_LE_CONN_REQ_PARAM conn_req_param = {0};
	T_GAP_CONN_PARAM_TYPE phy_type = 0;
	uint8_t init_conn_phys = 0;
	uint8_t *peer_addr_val = NULL;
	uint32_t i = 0;

	for (i = 0; i < 3; i++) {
		if (0 == i) {
			phy_type = GAP_CONN_PARAM_1M;
		} else if (1 == i) {
			phy_type = GAP_CONN_PARAM_2M;
		} else if (2 == i) {
			phy_type = GAP_CONN_PARAM_CODED;
		}

		if (p_ext_conn_param->init_phys[i]) {
			init_conn_phys |= 1 << i;
			conn_req_param.scan_interval = p_ext_conn_param->scan_interval[i];
			conn_req_param.scan_window = p_ext_conn_param->scan_window[i];
			conn_req_param.conn_interval_min = p_ext_conn_param->conn_interval_min[i];
			conn_req_param.conn_interval_max = p_ext_conn_param->conn_interval_max[i];
			conn_req_param.conn_latency = p_ext_conn_param->conn_latency[i];
			conn_req_param.supv_tout = p_ext_conn_param->supv_timeout[i];
			conn_req_param.ce_len_min = 2 * (p_ext_conn_param->conn_interval_min[i] - 1);
			conn_req_param.ce_len_max = 2 * (p_ext_conn_param->conn_interval_max[i] - 1);
			BT_LOGD("PHY[%d], scan_interval: 0x%x, scan_window: 0x%x, conn_interval_min: 0x%x, conn_interval_max: 0x%x"
					" conn_latency: 0x%x, supv_tout: 0x%x, ce_len_min: 0x%x, ce_len_max: 0x%x\r\n",
					i, conn_req_param.scan_interval, conn_req_param.scan_window,
					conn_req_param.conn_interval_min, conn_req_param.conn_interval_max,
					conn_req_param.conn_latency, conn_req_param.supv_tout,
					conn_req_param.ce_len_min, conn_req_param.ce_len_max);
			cause = le_set_conn_param(phy_type, &conn_req_param);
			if (cause) {
				return RTK_BT_ERR_LOWER_STACK_API;
			}
		}
	}

	if (RTK_BT_LE_CONN_FILTER_WITHOUT_WHITELIST == p_ext_conn_param->filter_policy) {
		peer_addr_val = (uint8_t *)p_ext_conn_param->peer_addr.addr_val;
	} else {
		peer_addr_val = NULL;
	}
	cause = le_connect(init_conn_phys, peer_addr_val,
					   (T_GAP_REMOTE_ADDR_TYPE)(p_ext_conn_param->peer_addr.type),
					   (T_GAP_LOCAL_ADDR_TYPE)(p_ext_conn_param->own_addr_type),
					   p_ext_conn_param->scan_timeout);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
rtk_bt_le_ext_adv_param_t legacy_adv_ext_param = {
	.adv_event_prop = RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED,
	.primary_adv_interval_min = 200,
	.primary_adv_interval_max = 250,
	.primary_adv_channel_map = RTK_BT_LE_ADV_CHNL_ALL,
	.own_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0, 0, 0, 0, 0, 0}},
	.peer_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0, 0, 0, 0, 0, 0}},
	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
	.tx_power = 0x7F,
	.primary_adv_phy = RTK_BT_LE_PHYS_PRIM_ADV_1M,
	.secondary_adv_phy = RTK_BT_LE_PHYS_1M,
};

rtk_bt_le_ext_adv_create_t legacy_adv_ext_create = {
	.p_adv_param = &legacy_adv_ext_param,
	.p_adv_handle = &bt_stack_le_legacy_adv_hdl
};
#endif

static uint16_t bt_stack_le_gap_set_adv_data(void *param, uint32_t param_len)
{
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
	uint16_t ret;

	if (bt_stack_le_legacy_adv_hdl == RTK_GAP_INVALID_ADV_HANDLE) {
		ret = bt_stack_le_gap_create_ext_adv(&legacy_adv_ext_create);
		if (ret) {
			BT_LOGE("bt_stack_le_gap_create_ext_adv: ret = %x \r\n", ret);
			return ret;
		}
	}

	rtk_bt_le_ext_adv_data_t adv_data = {
		.adv_handle = bt_stack_le_legacy_adv_hdl,
		.pdata = (uint8_t *)param,
		.len = param_len,
	};

	ret = bt_stack_le_gap_set_ext_adv_data(&adv_data);
	if (ret) {
		BT_LOGE("bt_stack_le_gap_set_ext_adv_data: ret = %x \r\n", ret);
		return ret;
	}

	return 0;

#else
	T_GAP_CAUSE cause;
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	extern uint8_t rtk_bt_mesh_stack_set_adv_data(uint8_t *p_data, uint32_t len);
	if (rtk_bt_mesh_is_enable()) {
		cause = rtk_bt_mesh_stack_set_adv_data(param, param_len);
	} else
#endif
	{
		cause = le_adv_set_param(GAP_PARAM_ADV_DATA, param_len, param);
	}

	if (cause) {
		BT_LOGE("bt_stack_le_gap_set_adv_data: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#endif
}

static uint16_t bt_stack_le_gap_set_scan_rsp_data(void *param, uint32_t param_len)
{
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
	uint16_t ret;

	if (bt_stack_le_legacy_adv_hdl == RTK_GAP_INVALID_ADV_HANDLE) {
		ret = bt_stack_le_gap_create_ext_adv(&legacy_adv_ext_create);
		if (ret) {
			BT_LOGE("bt_stack_le_gap_create_ext_adv: ret = %x \r\n", ret);
			return ret;
		}
	}

	rtk_bt_le_ext_adv_data_t scan_rsp = {
		.adv_handle = bt_stack_le_legacy_adv_hdl,
		.pdata = (uint8_t *)param,
		.len = param_len,
	};

	ret = bt_stack_le_gap_set_ext_scan_rsp_data(&scan_rsp);
	if (ret) {
		BT_LOGE("bt_stack_le_gap_set_ext_scan_rsp_data: ret = %x \r\n", ret);
		return ret;
	}

	return 0;
#else
	T_GAP_CAUSE cause;
	cause = le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, param_len, param);

	if (cause) {
		BT_LOGE("bt_stack_le_gap_set_scan_rsp_data: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#endif
}

static uint16_t bt_stack_le_gap_start_adv(void *param)
{
	rtk_bt_le_adv_param_t *padv_param = (rtk_bt_le_adv_param_t *)param;
	T_GAP_CAUSE cause;

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
	uint16_t ret;
	rtk_bt_le_adv_event_prop_t adv_event_prop;
	rtk_bt_le_ext_adv_start_t start_param;

	if (bt_stack_le_legacy_adv_hdl == RTK_GAP_INVALID_ADV_HANDLE) {
		ret = bt_stack_le_gap_create_ext_adv(&legacy_adv_ext_create);
		if (ret) {
			BT_LOGE("bt_stack_le_gap_create_ext_adv: ret = %x \r\n", ret);
			return ret;
		}
	}

	if (padv_param->type == RTK_BT_LE_ADV_TYPE_IND) {
		adv_event_prop = RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED;
	} else if (padv_param->type == RTK_BT_LE_ADV_TYPE_DIRECT_IND_HIGH) {
		adv_event_prop = RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED;
	} else if (padv_param->type == RTK_BT_LE_ADV_TYPE_DIRECT_IND_LOW) {
		adv_event_prop = RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED;
	} else if (padv_param->type == RTK_BT_LE_ADV_TYPE_SCAN_IND) {
		adv_event_prop = RTK_BT_LE_EXT_ADV_LEGACY_ADV_SCAN_UNDIRECTED;
	} else if (padv_param->type == RTK_BT_LE_ADV_TYPE_NONCONN_IND) {
		adv_event_prop = RTK_BT_LE_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED;
	} else {
		return RTK_BT_ERR_PARAM_INVALID;
	}

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	cause = ble_ext_adv_mgr_change_adv_event_prop(bt_stack_le_legacy_adv_hdl, adv_event_prop);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = ble_ext_adv_mgr_change_adv_interval(bt_stack_le_legacy_adv_hdl, (padv_param->interval_min + padv_param->interval_max) / 2);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = ble_ext_adv_mgr_change_peer_addr(bt_stack_le_legacy_adv_hdl,
											 (T_GAP_REMOTE_ADDR_TYPE)padv_param->peer_addr.type,
											 padv_param->peer_addr.addr_val);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = ble_ext_adv_mgr_change_channel_map(bt_stack_le_legacy_adv_hdl, (uint8_t)padv_param->channel_map);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = ble_ext_adv_mgr_change_filter_policy(bt_stack_le_legacy_adv_hdl, (T_GAP_ADV_FILTER_POLICY)padv_param->filter_policy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#else

	cause = le_ext_adv_set_adv_param(bt_stack_le_legacy_adv_hdl, adv_event_prop,
									 padv_param->interval_min, padv_param->interval_max,
									 (uint8_t)padv_param->channel_map, (T_GAP_LOCAL_ADDR_TYPE)padv_param->own_addr_type,
									 (T_GAP_REMOTE_ADDR_TYPE)padv_param->peer_addr.type, padv_param->peer_addr.addr_val,
									 (T_GAP_ADV_FILTER_POLICY)padv_param->filter_policy, 0x7F,
									 GAP_PHYS_PRIM_ADV_1M, 0, GAP_PHYS_1M, 0, false);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	if (adv_event_prop == RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED) {
		start_param.duration = 128;
	} else {
		start_param.duration = 0;
	}

	start_param.adv_handle = bt_stack_le_legacy_adv_hdl;
	start_param.num_events = 0;

	ret = bt_stack_le_gap_start_ext_adv(&start_param);
	if (ret) {
		BT_LOGE("bt_stack_le_gap_start_ext_adv: ret = %x \r\n", ret);
		return ret;
	}

#else
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	extern uint8_t rtk_bt_mesh_stack_start_adv(rtk_bt_le_adv_param_t *adv_param);
	if (rtk_bt_mesh_is_enable()) {
		cause = rtk_bt_mesh_stack_start_adv(padv_param);
	} else
#endif
	{
		cause = le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(padv_param->interval_min),
								 &padv_param->interval_min);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(padv_param->interval_max),
								 &padv_param->interval_max);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof((uint8_t)padv_param->type),
								 &padv_param->type);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_set_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, sizeof((uint8_t)padv_param->own_addr_type),
								 &padv_param->own_addr_type);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof((uint8_t)padv_param->peer_addr.type),
								 &padv_param->peer_addr.type);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(padv_param->peer_addr.addr_val),
								 &padv_param->peer_addr.addr_val);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof((uint8_t)padv_param->channel_map),
								 &padv_param->channel_map);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof((uint8_t)padv_param->filter_policy),
								 &padv_param->filter_policy);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_adv_start();
	}
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	return 0;
}

static uint16_t bt_stack_le_gap_stop_adv(void)
{
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
	uint16_t ret;

	if (bt_stack_le_legacy_adv_hdl == RTK_GAP_INVALID_ADV_HANDLE) {
		return RTK_BT_ERR_STATE_INVALID;
	}

	ret = bt_stack_le_gap_stop_ext_adv(&bt_stack_le_legacy_adv_hdl);
	if (ret) {
		BT_LOGE("bt_stack_le_gap_stop_ext_adv: ret = %x \r\n", ret);
		return ret;
	}

	return 0;
#else
	T_GAP_CAUSE cause;
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	extern uint8_t rtk_bt_mesh_stack_stop_adv(void);
	if (rtk_bt_mesh_is_enable()) {
		cause = rtk_bt_mesh_stack_stop_adv();
	} else
#endif
	{
		cause = le_adv_stop();
	}
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#endif
}

static uint16_t bt_stack_le_gap_get_adv_param(void *param)
{
#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;
#else
	rtk_bt_le_adv_param_t *padv_param = (rtk_bt_le_adv_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_adv_get_param(GAP_PARAM_ADV_INTERVAL_MIN, &padv_param->interval_min);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_adv_get_param(GAP_PARAM_ADV_INTERVAL_MAX, &padv_param->interval_max);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_adv_get_param(GAP_PARAM_ADV_EVENT_TYPE, &padv_param->type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_adv_get_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, &padv_param->own_addr_type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_adv_get_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, &(padv_param->peer_addr.type));
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_adv_get_param(GAP_PARAM_ADV_DIRECT_ADDR, &(padv_param->peer_addr.addr_val));
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_adv_get_param(GAP_PARAM_ADV_CHANNEL_MAP, &padv_param->channel_map);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_adv_get_param(GAP_PARAM_ADV_FILTER_POLICY, &padv_param->filter_policy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#endif
}

static bool bt_stack_le_gap_adv_is_idle(void)
{
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)

#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
	return !ble_ext_adv_is_ongoing();
#else
	return le_ext_adv_check_adv_idle();
#endif

#else

	T_GAP_DEV_STATE dev_state;

	le_get_gap_param(GAP_PARAM_DEV_STATE, &dev_state);
	if (dev_state.gap_adv_state == GAP_ADV_STATE_IDLE) {
		return true;
	}

	return false;

#endif
}

#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
static uint16_t bt_stack_le_gap_start_pa(void *p_param)
{
	rtk_bt_le_pa_param_t *param = (rtk_bt_le_pa_param_t *)p_param;
	T_GAP_CAUSE cause;

	if (!bt_stack_le_gap_ext_adv_handle_valid(param->adv_handle)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	BT_LOGD("le_pa_adv_set_periodic_adv_param: adv_handle = %x,pa_interval_min = %x,pa_interval_max = %x,pa_prop = %x\r\n", param->adv_handle,
			param->pa_interval_min, param->pa_interval_max, param->pa_prop);

	cause = le_pa_adv_set_periodic_adv_param(param->adv_handle, param->pa_interval_min,
											 param->pa_interval_max, param->pa_prop);
	if (cause) {
		BT_LOGE("le_pa_adv_set_periodic_adv_param: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_pa_adv_set_periodic_adv_data(param->adv_handle, param->pa_len, param->pa_data, false);
	if (cause) {
		BT_LOGE("le_pa_adv_set_periodic_adv_data: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_pa_adv_start_setting(param->adv_handle, PA_ADV_SET_PERIODIC_ADV_PARAS | PA_ADV_SET_PERIODIC_ADV_DATA);
	if (cause) {
		BT_LOGE("le_pa_adv_start_setting: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_stop_pa(void *param)
{
	uint8_t handle = *((uint8_t *)param);
	T_GAP_CAUSE cause;

	cause = le_pa_adv_set_periodic_adv_enable(handle, 0);
	if (cause) {
		BT_LOGE("le_pa_adv_set_periodic_adv_enable: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_update_pa(void *param)
{
	rtk_bt_le_pa_update_t *p_param = (rtk_bt_le_pa_update_t *)param;
	T_GAP_CAUSE cause;
	uint8_t *data = p_param->pa_data;
	uint16_t len = p_param->pa_len;

	if (!bt_stack_le_gap_ext_adv_handle_valid(p_param->adv_handle)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_param->update_did_only) {
		if (bt_stack_ext_adv_tbl[p_param->adv_handle].pa_state != PA_ADV_STATE_WAIT_EXT_ADV_STATE_ADVERTISING &&
			bt_stack_ext_adv_tbl[p_param->adv_handle].pa_state != PA_ADV_STATE_ADVERTISING) {
			return RTK_BT_ERR_STATE_INVALID;
		}

		data = NULL;
		len = 0;
	}

	cause = le_pa_adv_set_periodic_adv_data(p_param->adv_handle, len, data, p_param->update_did_only);
	if (cause) {
		BT_LOGE("le_pa_adv_set_periodic_adv_data: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_pa_adv_start_setting(p_param->adv_handle, PA_ADV_SET_PERIODIC_ADV_DATA);
	if (cause) {
		BT_LOGE("le_pa_adv_start_setting: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_SCAN_SUPPORT) && F_BT_LE_5_0_AE_SCAN_SUPPORT)
static uint16_t bt_stack_le_gap_ext_scan_set_param(void *param)
{
	rtk_bt_le_ext_scan_param_t *p_param = (rtk_bt_le_ext_scan_param_t *)param;
	T_GAP_LE_EXT_SCAN_PARAM extended_scan_param[GAP_EXT_SCAN_MAX_PHYS_NUM];
	uint8_t scan_phys = 0;
	T_GAP_CAUSE cause;

	if (p_param->phys[0] == false && p_param->phys[1] == false) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_param->phys[0]) {
		scan_phys |= GAP_EXT_SCAN_PHYS_1M_BIT;
		extended_scan_param[0].scan_type = (T_GAP_SCAN_MODE)p_param->type[0];
		extended_scan_param[0].scan_interval = p_param->interval[0];
		extended_scan_param[0].scan_window = p_param->window[0];
	}

	if (p_param->phys[1]) {
		scan_phys |= GAP_EXT_SCAN_PHYS_CODED_BIT;
		extended_scan_param[1].scan_type = (T_GAP_SCAN_MODE)p_param->type[1];
		extended_scan_param[1].scan_interval = p_param->interval[1];
		extended_scan_param[1].scan_window = p_param->window[1];
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_LOCAL_ADDR_TYPE, sizeof(p_param->own_addr_type), &p_param->own_addr_type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_POLICY, sizeof(p_param->filter_policy), &p_param->filter_policy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_DUPLICATES, sizeof(p_param->duplicate_opt), &p_param->duplicate_opt);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_DURATION, sizeof(p_param->duration), &p_param->duration);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PERIOD, sizeof(p_param->period), &p_param->period);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PHYS, sizeof(scan_phys), &scan_phys);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	if (p_param->phys[0]) {
		le_ext_scan_set_phy_param(LE_SCAN_PHY_LE_1M, &extended_scan_param[0]);
	}

	if (p_param->phys[1]) {
		le_ext_scan_set_phy_param(LE_SCAN_PHY_LE_CODED, &extended_scan_param[1]);
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_ext_scan_start(void)
{
	T_GAP_CAUSE cause;
	cause = le_ext_scan_start();
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_ext_scan_stop(void)
{
	T_GAP_CAUSE cause;
	cause = le_ext_scan_stop();
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

#if (defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT) && (defined(F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SUPPORT)
static uint16_t bt_stack_le_gap_pa_sync_modify_adv_list(void *param)
{
	rtk_bt_le_pa_sync_modify_advlist_t *ptr = (rtk_bt_le_pa_sync_modify_advlist_t *)param;
	T_GAP_CAUSE cause;

	cause = le_pa_sync_modify_periodic_adv_list((T_GAP_PA_SYNC_PERIODIC_ADV_LIST_OP)ptr->operation,
												ptr->adv_addr.addr_val,
												(T_GAP_PA_SYNC_ADV_ADDR_TYPE)ptr->adv_addr.type,
												ptr->adv_sid);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_pa_sync_modify_adv_list: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_pa_sync_create(void *param)
{
	rtk_bt_le_pa_sync_create_t *ptr = (rtk_bt_le_pa_sync_create_t *)param;
	T_GAP_CAUSE cause;

	T_GAP_PA_SYNC_CREATE_SYNC_PARAM p_param;

	p_param.options = ptr->options;
	p_param.sync_cte_type = ptr->sync_cte_type;
	p_param.adv_sid = ptr->adv_sid;
	p_param.adv_addr_type = (T_GAP_PA_SYNC_ADV_ADDR_TYPE)ptr->adv_addr.type;
	memcpy(p_param.adv_addr, ptr->adv_addr.addr_val, RTK_BD_ADDR_LEN);
	p_param.skip = ptr->skip;
	p_param.sync_timeout = ptr->sync_timeout;

	BT_LOGD("bt_stack_le_gap_pa_sync_create: options = %x, sync_cte_type = %x, adv_sid = %x, adv_addr_type = %x, adv_addr[0] = %x\r\n",
			p_param.options, p_param.sync_cte_type, p_param.adv_sid, p_param.adv_addr_type, p_param.adv_addr[0]);
	cause = le_pa_sync_create_sync(&p_param, ptr->p_sync_id);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_pa_sync_create fail: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_pa_sync_terminate(void *param)
{
	uint8_t sync_id = *(uint8_t *)param;
	T_GAP_CAUSE cause;

	cause = le_pa_sync_terminate_sync(sync_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

#if (defined(RTK_BLE_5_1_PAST_SENDER_SUPPORT) && RTK_BLE_5_1_PAST_SENDER_SUPPORT) && (defined(F_BT_LE_5_1_PAST_SENDER_SUPPORT) && F_BT_LE_5_1_PAST_SENDER_SUPPORT)
static uint16_t bt_stack_le_gap_past_send(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_past_send_t *p_param = (rtk_bt_le_past_send_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_param->use_sync_id) {
		cause = le_past_sender_periodic_adv_sync_transfer(conn_id, p_param->service_data,
														  p_param->idx);
	} else {
		cause = le_past_sender_periodic_adv_set_info_transfer(conn_id, p_param->service_data,
															  p_param->idx);
	}

	if (cause) {
		BT_LOGE("bt_stack_le_gap_past_send: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

#if (defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && (defined(F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT) && F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
static uint16_t bt_stack_le_gap_past_recv_set(void *param)
{
	T_GAP_CAUSE cause;
	uint8_t conn_id;
	rtk_bt_le_past_recv_param_t *p_param = (rtk_bt_le_past_recv_param_t *)param;
	T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM transfer = {
		.mode = (T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_MODE)p_param->mode,
		.cte_type = p_param->sync_cte_type,
		.skip = p_param->skip,
		.sync_timeout = p_param->sync_timeout
	};

	if (!le_get_conn_id_by_handle(p_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_past_recipient_set_periodic_adv_sync_transfer_params(conn_id, &transfer);

	if (cause) {
		BT_LOGE("bt_stack_le_gap_past_recv_set: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_default_past_recv_set(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_past_recv_param_t *p_param = (rtk_bt_le_past_recv_param_t *)param;
	T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM transfer = {
		.mode = (T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_MODE)p_param->mode,
		.cte_type = p_param->sync_cte_type,
		.skip = p_param->skip,
		.sync_timeout = p_param->sync_timeout
	};

	cause = le_past_recipient_set_default_periodic_adv_sync_transfer_params(&transfer);

	if (cause) {
		BT_LOGE("bt_stack_le_gap_default_past_recv_set: cause = %x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

static uint16_t bt_stack_le_gap_scan_info_filter(void *param)
{
	rtk_bt_le_scan_info_filter_param_t *p_scan_filter =
		(rtk_bt_le_scan_info_filter_param_t *)param;
	bool ret;

	ret = le_scan_info_filter(p_scan_filter->enable, p_scan_filter->offset,
							  p_scan_filter->len, p_scan_filter->p_filter);
	if (ret != true) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_set_scan_param(void *param)
{
	rtk_bt_le_scan_param_t *p_gap_scan_param = (rtk_bt_le_scan_param_t *)param;
	T_GAP_CAUSE cause;

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	T_GAP_LE_EXT_SCAN_PARAM extended_scan_param = {0};
	uint8_t scan_phys = GAP_EXT_SCAN_PHYS_1M_BIT;

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_LOCAL_ADDR_TYPE, sizeof(p_gap_scan_param->own_addr_type), &p_gap_scan_param->own_addr_type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_POLICY, sizeof(p_gap_scan_param->filter_policy), &p_gap_scan_param->filter_policy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_DUPLICATES, sizeof(p_gap_scan_param->duplicate_opt), &p_gap_scan_param->duplicate_opt);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PHYS, sizeof(scan_phys), &scan_phys);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	extended_scan_param.scan_type = (T_GAP_SCAN_MODE)p_gap_scan_param->type;
	extended_scan_param.scan_interval = p_gap_scan_param->interval;
	extended_scan_param.scan_window = p_gap_scan_param->window;
	le_ext_scan_set_phy_param(LE_SCAN_PHY_LE_1M, &extended_scan_param);

#else
	cause = le_scan_set_param(GAP_PARAM_SCAN_MODE, sizeof(p_gap_scan_param->type), &p_gap_scan_param->type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(p_gap_scan_param->interval), &p_gap_scan_param->interval);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_set_param(GAP_PARAM_SCAN_WINDOW, sizeof(p_gap_scan_param->window), &p_gap_scan_param->window);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_set_param(GAP_PARAM_SCAN_LOCAL_ADDR_TYPE, sizeof(p_gap_scan_param->own_addr_type), &p_gap_scan_param->own_addr_type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(p_gap_scan_param->filter_policy), &p_gap_scan_param->filter_policy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(p_gap_scan_param->duplicate_opt), &p_gap_scan_param->duplicate_opt);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_get_scan_param(void *param)
{
#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;
#else
	rtk_bt_le_scan_param_t *pscan_param = (rtk_bt_le_scan_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_scan_get_param(GAP_PARAM_SCAN_MODE, &pscan_param->type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_get_param(GAP_PARAM_SCAN_INTERVAL, &pscan_param->interval);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_get_param(GAP_PARAM_SCAN_WINDOW, &pscan_param->window);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_get_param(GAP_PARAM_SCAN_LOCAL_ADDR_TYPE, &pscan_param->own_addr_type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_get_param(GAP_PARAM_SCAN_FILTER_POLICY, &pscan_param->filter_policy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_scan_get_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, &pscan_param->duplicate_opt);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#endif
}

static uint16_t bt_stack_le_gap_start_scan(void)
{
	T_GAP_CAUSE cause;
#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	cause = le_ext_scan_start();
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#else
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	extern uint8_t rtk_bt_mesh_stack_set_scan_switch(bool scan_switch);
	if (rtk_bt_mesh_is_enable()) {
		cause = rtk_bt_mesh_stack_set_scan_switch(true);
	} else
#endif
	{
		cause = le_scan_start();
	}
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	return 0;
}

static uint16_t bt_stack_le_gap_stop_scan(void)
{
	T_GAP_CAUSE cause;
#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	cause = le_ext_scan_stop();
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#else
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	extern uint8_t rtk_bt_mesh_stack_set_scan_switch(bool scan_switch);
	if (rtk_bt_mesh_is_enable()) {
		cause = rtk_bt_mesh_stack_set_scan_switch(false);
	} else
#endif
	{
		cause = le_scan_stop();
	}
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	return 0;
}

static uint16_t bt_stack_le_gap_connect(void *param)
{
	rtk_bt_le_create_conn_param_t *p_conn_param = (rtk_bt_le_create_conn_param_t *)param;
	T_GAP_CAUSE cause;
	T_GAP_LE_CONN_REQ_PARAM conn_req_param;
	T_GAP_LOCAL_ADDR_TYPE local_addr_type = (T_GAP_LOCAL_ADDR_TYPE)p_conn_param->own_addr_type;
	uint8_t *peer_addr_val = NULL;
	uint8_t init_phys = 0;

	conn_req_param.scan_interval = p_conn_param->scan_interval;
	conn_req_param.scan_window = p_conn_param->scan_window;
	conn_req_param.conn_interval_min = p_conn_param->conn_interval_min;
	conn_req_param.conn_interval_max = p_conn_param->conn_interval_max;
	conn_req_param.conn_latency = p_conn_param->conn_latency;
	conn_req_param.supv_tout = p_conn_param->supv_timeout;
	conn_req_param.ce_len_min = 2 * (p_conn_param->conn_interval_min - 1);
	conn_req_param.ce_len_max = 2 * (p_conn_param->conn_interval_max - 1);
	BT_LOGD("Legacy conn, scan_interval: 0x%x, scan_window: 0x%x, conn_interval_min: 0x%x, conn_interval_max: 0x%x"
			" conn_latency: 0x%x, supv_tout: 0x%x, ce_len_min: 0x%x, ce_len_max: 0x%x\r\n",
			conn_req_param.scan_interval, conn_req_param.scan_window,
			conn_req_param.conn_interval_min, conn_req_param.conn_interval_max,
			conn_req_param.conn_latency, conn_req_param.supv_tout,
			conn_req_param.ce_len_min, conn_req_param.ce_len_max);
	cause = le_set_conn_param(GAP_CONN_PARAM_1M, &conn_req_param);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	if (RTK_BT_LE_CONN_FILTER_WITHOUT_WHITELIST == p_conn_param->filter_policy) {
		peer_addr_val = (uint8_t *)(p_conn_param->peer_addr.addr_val);
	} else {
		peer_addr_val = NULL;
	}

	/* In rtk stack, if extend adv enabled, legacy connection (le_connect(0,...)) will not work */
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
	init_phys = GAP_PHYS_CONN_INIT_1M_BIT;
#else
	init_phys = 0;
#endif
	cause = le_connect(init_phys, peer_addr_val,
					   (T_GAP_REMOTE_ADDR_TYPE)(p_conn_param->peer_addr.type),
					   local_addr_type, p_conn_param->scan_timeout);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_connnect_cancel(void *param)
{
	rtk_bt_le_addr_t *conn_cancel_param = (rtk_bt_le_addr_t *)param;
	uint8_t conn_id = 0xFF;
	T_GAP_CAUSE cause;

	if (!le_get_conn_id(conn_cancel_param->addr_val, conn_cancel_param->type, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_disconnect(conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_disconnect(void *param)
{
	uint16_t conn_handle = *(uint16_t *)param;
	uint8_t conn_id;
	T_GAP_CAUSE cause;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_disconnect(conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_disconnect_with_reason(void *param)
{
	rtk_bt_le_disconn_with_reason_param_t *disconn_with_reason = (rtk_bt_le_disconn_with_reason_param_t *)param;
	T_GAP_CAUSE cause;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(disconn_with_reason->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_disconnect_with_reason(conn_id, disconn_with_reason->reason);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_update_conn_param(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_update_conn_param_t *p_update_conn_param = (rtk_bt_le_update_conn_param_t *)param;
	uint16_t ce_len_min = 2 * (p_update_conn_param->conn_interval_min - 1);
	uint16_t ce_len_max = 2 * (p_update_conn_param->conn_interval_max - 1);
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_update_conn_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_update_conn_param(conn_id,
								 p_update_conn_param->conn_interval_min,
								 p_update_conn_param->conn_interval_max,
								 p_update_conn_param->conn_latency,
								 p_update_conn_param->supv_timeout,
								 ce_len_min, ce_len_max);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_modify_whitelist(void *param)
{
	uint16_t err = 0;
	T_GAP_CAUSE cause;
	rtk_bt_le_modify_wl_param_t *p_wl_op_param = (rtk_bt_le_modify_wl_param_t *)param;

	cause = le_modify_white_list((T_GAP_WHITE_LIST_OP)p_wl_op_param->op,
								 (uint8_t *)p_wl_op_param->addr.addr_val,
								 (T_GAP_REMOTE_ADDR_TYPE)p_wl_op_param->addr.type);
	if (cause) {
		err = RTK_BT_ERR_LOWER_STACK_API;
	}

	return err;
}

static uint16_t bt_stack_le_gap_read_rssi(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_read_rssi_param_t *read_rssi = (rtk_bt_le_read_rssi_param_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(read_rssi->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_read_rssi(conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

#if defined(F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT) && F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
static uint16_t bt_stack_le_gap_read_remote_version(void *param)
{
	T_GAP_CAUSE cause;
	uint8_t conn_id;
	uint16_t conn_handle = *((uint16_t *)param);

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_read_remote_version(conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

static uint16_t bt_stack_le_gap_get_dev_state(void *param)
{
	T_GAP_CAUSE cause;
	T_GAP_DEV_STATE dev_state;
	rtk_bt_le_gap_dev_state_t *gap_state = (rtk_bt_le_gap_dev_state_t *)param;

	cause = le_get_gap_param(GAP_PARAM_DEV_STATE, &dev_state);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	gap_state->gap_init_state = dev_state.gap_init_state;
	gap_state->gap_adv_state =  dev_state.gap_adv_state;
	gap_state->gap_scan_state = dev_state.gap_scan_state;
	gap_state->gap_conn_state = dev_state.gap_conn_state;

	return 0;
}

static uint16_t bt_stack_le_gap_get_active_conn(void *param)
{
	uint16_t active_num;
	uint16_t i = 0, j = 0;
	rtk_bt_le_get_active_conn_t *p_active_conn =
		(rtk_bt_le_get_active_conn_t *)param;
	active_num = le_get_active_link_num();

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		if (bt_stack_le_link_tbl[i].is_active) {
			p_active_conn->conn_handle[j] = le_get_conn_handle(i);
			j++;
		}
	}
	p_active_conn->conn_num = j;
	if (j != active_num) {
		BT_LOGE("Error: Active conn num get from api mismatch with from rtk stack\r\n");
		return RTK_BT_ERR_MISMATCH;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_get_conn_handle_by_addr(void *param)
{
	rtk_bt_le_get_conn_handle_by_addr_param_t *p_conn_addr =
		(rtk_bt_le_get_conn_handle_by_addr_param_t *)param;
	rtk_bt_le_addr_t addr = p_conn_addr->addr;
	uint16_t *p_conn_handle = p_conn_addr->p_conn_handle;
	uint8_t conn_id = 0;

	if (true != le_get_conn_id(addr.addr_val, addr.type, &conn_id)) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	*p_conn_handle = le_get_conn_handle(conn_id);

	return 0;
}

static uint16_t bt_stack_le_gap_get_conn_info(void *param)
{
	T_GAP_CAUSE cause;
	T_GAP_CONN_INFO stack_conn_info;

	rtk_bt_le_get_conn_info_param_t *get_info =
		(rtk_bt_le_get_conn_info_param_t *)param;
	uint8_t conn_id;
	rtk_bt_le_conn_info_t *info = get_info->p_conn_info;

	if (!le_get_conn_id_by_handle(get_info->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (false == le_get_conn_info(conn_id, &stack_conn_info)) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	memset(info, 0, sizeof(rtk_bt_le_conn_info_t));
	info->role = convert_rtk_link_role(stack_conn_info.role);
	info->remote.type = (rtk_bt_le_addr_type_t)stack_conn_info.remote_bd_type;
	memcpy(info->remote.addr_val, stack_conn_info.remote_bd,
		   sizeof(stack_conn_info.remote_bd));
	cause = le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &info->interval, conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_get_conn_param(GAP_PARAM_CONN_LATENCY, &info->latency, conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &info->supv_timeout, conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#if defined(F_BT_LE_5_0_SET_PHYS_SUPPORT) && F_BT_LE_5_0_SET_PHYS_SUPPORT
	cause = le_get_conn_param(GAP_PARAM_CONN_TX_PHY_TYPE, &info->tx_phy, conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_get_conn_param(GAP_PARAM_CONN_RX_PHY_TYPE, &info->rx_phy, conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#else /* the default phy is 1M uncoded */
	info->tx_phy = 1;
	info->rx_phy = 1;
#endif

	return 0;
}

static uint16_t bt_stack_le_gap_get_mtu_size(void *param)
{
	uint16_t mtu_size = 0;
	rtk_bt_le_get_mtu_size_param_t *get_mtu =
		(rtk_bt_le_get_mtu_size_param_t *)param;
	uint8_t conn_id;
	uint16_t *p_mtu_size = get_mtu->p_mtu_size;

	if (!le_get_conn_id_by_handle(get_mtu->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!bt_stack_le_link_tbl[conn_id].is_active) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	mtu_size = bt_stack_le_link_tbl[conn_id].mtu_size;
	*p_mtu_size = mtu_size ? mtu_size : RTK_BT_LE_DEFAULT_ATT_MTU_SIZE;

	return 0;
}

static uint16_t bt_stack_le_gap_set_max_mtu_size(void *param)
{
	T_GAP_CAUSE cause;
	T_GAP_DEV_STATE dev_state;
	uint8_t active_conn_num = 0;
	uint16_t mtu_size = *(uint16_t *)param;

	cause = le_get_gap_param(GAP_PARAM_DEV_STATE, &dev_state);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	active_conn_num = le_get_active_link_num();

	if ((GAP_CONN_DEV_STATE_INITIATING == dev_state.gap_conn_state) || active_conn_num) {
		return RTK_BT_ERR_STATE_INVALID;
	}

	gap_config_max_mtu_size(mtu_size);

	return 0;
}

static uint16_t bt_stack_le_gap_set_channels(void *param)
{
	T_GAP_CAUSE cause;
	uint8_t *chan_map = (uint8_t *)param;

	cause = le_set_host_chann_classif((uint8_t *)chan_map);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_set_data_len(void *param)
{
#if defined(F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT) && F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
	T_GAP_CAUSE cause;
	rtk_bt_le_set_datalen_param_t *set_datalen = (rtk_bt_le_set_datalen_param_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(set_datalen->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_set_data_len(conn_id, set_datalen->max_tx_octets,
							set_datalen->max_tx_time);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#else
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;
#endif
}

static uint16_t bt_stack_le_gap_set_phy(void *param)
{
#if defined(F_BT_LE_5_0_SET_PHYS_SUPPORT) && F_BT_LE_5_0_SET_PHYS_SUPPORT
	T_GAP_CAUSE cause;
	rtk_bt_le_set_phy_param_t *set_phy = (rtk_bt_le_set_phy_param_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(set_phy->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_set_phy(conn_id, set_phy->all_phys, set_phy->tx_phys,
					   set_phy->rx_phys, (T_GAP_PHYS_OPTIONS)set_phy->phy_options);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#else
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;
#endif
}

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
static void privacy_handle_le_privacy_resolution_status_info(T_LE_PRIVACY_RESOLUTION_STATUS_INFO
															 resolv_status)
{
	BT_LOGD("[PRIVACY] privacy_handle_le_privacy_resolution_status_info: status 0x%x\r\n",
			resolv_status.status);
	privacy_resolution_status = resolv_status.status;
}

static void privacy_handle_le_privacy_modify_resolv_list_rsp(T_LE_PRIVACY_MODIFY_RESOLV_LIST_RSP *p_rsp)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_le_modify_resolv_list_ind_t *p_ind = NULL;
	uint8_t bond_storage_num = le_get_max_le_paired_device_num();
	BT_LOGD("[PRIVACY] privacy_handle_le_privacy_modify_resolv_list_rsp: operation  0x%x, casue 0x%x\r\n",
			p_rsp->operation, p_rsp->cause);
	privacy_modify_resolv_list_pending = false;

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_RESOLV_LIST_MODIFY_IND,
								sizeof(rtk_bt_le_modify_resolv_list_ind_t));
	if (p_evt) {
		p_ind = (rtk_bt_le_modify_resolv_list_ind_t *)p_evt->data;
		memset(p_ind, 0, sizeof(rtk_bt_le_modify_resolv_list_ind_t));
		p_ind->op = (rtk_bt_le_resolv_list_op_t)p_rsp->operation;
		p_ind->err = p_rsp->cause;

		if ((p_rsp->operation == GAP_RESOLV_LIST_OP_ADD ||
			 p_rsp->operation == GAP_RESOLV_LIST_OP_REMOVE) &&
			(privacy_modify_resolv_list_idx < bond_storage_num)) {
			p_ind->entry.device_mode = privacy_table[privacy_modify_resolv_list_idx].device_mode;
			p_ind->entry.addr_type = (rtk_bt_le_ident_addr_type_t)privacy_table[privacy_modify_resolv_list_idx].remote_bd_type;
			memcpy(p_ind->entry.addr, privacy_table[privacy_modify_resolv_list_idx].addr, RTK_BD_ADDR_LEN);
		}
		rtk_bt_evt_indicate(p_evt, NULL);
	}

	if (p_rsp->cause != GAP_SUCCESS) {
		if (p_rsp->cause == (HCI_ERR | HCI_ERR_UNKNOWN_CONN_ID)
			|| p_rsp->cause == (HCI_ERR | HCI_ERR_MEMORY_FULL)
			|| p_rsp->cause == (HCI_ERR | HCI_ERR_INVALID_PARAM)) {
			memset(&privacy_table[privacy_modify_resolv_list_idx], 0, sizeof(T_LE_PRIVACY_ENTRY));
		}
		privacy_modify_resolv_list_idx = 0xff;
	} else {
		if (p_rsp->operation == GAP_RESOLV_LIST_OP_CLEAR) {
			privacy_clear_pending = false;
			memset(privacy_table, 0, bond_storage_num * sizeof(T_LE_PRIVACY_ENTRY));
			/* 0 resolving list need to be added to controller again when resolving list is cleared,
			because RPA adv need it */
			uint8_t peer_addr[RTK_BD_ADDR_LEN] = {0};
			le_privacy_add_resolv_list(GAP_IDENT_ADDR_PUBLIC, peer_addr, NULL, true);
		} else if (p_rsp->operation == GAP_RESOLV_LIST_OP_ADD) {
			if (privacy_modify_resolv_list_idx < bond_storage_num) {
				privacy_table[privacy_modify_resolv_list_idx].is_added = PRIVACY_RESOLVING_ADDED;
				privacy_table[privacy_modify_resolv_list_idx].state = PRIVACY_RESOLVING_LIST_IDLE;
				privacy_modify_resolv_list_idx = 0xff;
			}
		} else {
			if (privacy_modify_resolv_list_idx < bond_storage_num) {
				memset(&privacy_table[privacy_modify_resolv_list_idx], 0, sizeof(T_LE_PRIVACY_ENTRY));
				privacy_modify_resolv_list_idx = 0xff;
			}
		}
		privacy_handle_resolv_list(true);
	}
}

static T_APP_RESULT privacy_msg_callback(uint8_t msg_type, T_LE_PRIVACY_CB_DATA msg_data)
{
	T_APP_RESULT result = APP_RESULT_SUCCESS;
	rtk_bt_cmd_t *p_cmd = NULL;
	BT_LOGD("[PRIVACY] privacy_msg_callback: msg_type  %d\r\n", msg_type);

	switch (msg_type) {
	case GAP_MSG_LE_PRIVACY_RESOLUTION_STATUS_INFO:
		privacy_handle_le_privacy_resolution_status_info(msg_data.le_privacy_resolution_status_info);
		break;

	case GAP_MSG_LE_PRIVACY_MODIFY_RESOLV_LIST:
		privacy_handle_le_privacy_modify_resolv_list_rsp(msg_data.p_le_privacy_modify_resolv_list_rsp);
		break;

	case GAP_MSG_LE_PRIVACY_SET_MODE:
		BT_LOGD("[PRIVACY] GAP_MSG_LE_PRIVACY_SET_MODE: cause 0x%x\r\n",
				msg_data.p_le_privacy_set_mode_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(msg_type);
		if (p_cmd) {
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = msg_data.p_le_privacy_set_mode_rsp->cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGD("[%s] GAP_MSG_LE_PRIVACY_SET_MODE: find no pending command \r\n", __func__);
		}
		break;

	case GAP_MSG_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT:
		BT_LOGD("[PRIVACY] GAP_MSG_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT: cause 0x%x\r\n",
				msg_data.p_le_privacy_set_resolv_priv_addr_timeout_rsp->cause);
		break;

	case GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR:
		BT_LOGD("[PRIVACY] GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR: cause 0x%x\r\n",
				msg_data.p_le_privacy_read_peer_resolv_addr_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(msg_type);
		if (p_cmd) {
			rtk_bt_le_read_peer_rpa_param_t *read_peer = (rtk_bt_le_read_peer_rpa_param_t *)p_cmd->param;
			bt_stack_pending_cmd_delete(p_cmd);
			memcpy(read_peer->peer_rpa, msg_data.p_le_privacy_read_peer_resolv_addr_rsp->peer_resolv_addr, 6);
			p_cmd->ret = msg_data.p_le_privacy_read_peer_resolv_addr_rsp->cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR: find no pending command \r\n", __func__);
		}
		break;

	case GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR:
		BT_LOGD("[PRIVACY] GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR: cause 0x%x\r\n",
				msg_data.p_le_privacy_read_local_resolv_addr_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(msg_type);
		if (p_cmd) {
			rtk_bt_le_read_local_rpa_param_t *read_rpa = (rtk_bt_le_read_local_rpa_param_t *)p_cmd->param;
			bt_stack_pending_cmd_delete(p_cmd);
			memcpy(read_rpa->local_rpa,
				   msg_data.p_le_privacy_read_local_resolv_addr_rsp->local_resolv_addr, 6);
			p_cmd->ret = msg_data.p_le_privacy_read_local_resolv_addr_rsp->cause;
			osif_sem_give(p_cmd->psem);
		} else {
			BT_LOGE("[%s] GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR: find no pending command \r\n", __func__);
		}
		break;

	default:
		break;
	}
	return result;
}

static void privacy_handle_resolv_list_int(void)
{
	uint8_t i;
	T_GAP_CAUSE cause;
	uint8_t bond_storage_num = le_get_max_le_paired_device_num();

	if (privacy_modify_resolv_list_pending) {
		BT_LOGD("[PRIVACY] wait rsp\r\n");
		return;
	}
	if (privacy_clear_pending) {
		BT_LOGD("[PRIVACY] clear\r\n");
		cause = le_privacy_modify_resolv_list(GAP_RESOLV_LIST_OP_CLEAR, GAP_IDENT_ADDR_PUBLIC, NULL);
		if (cause == GAP_CAUSE_SUCCESS) {
			if (privacy_whitelist) {
				le_modify_white_list(GAP_WHITE_LIST_OP_CLEAR, NULL, GAP_REMOTE_ADDR_LE_PUBLIC);
			}
			privacy_modify_resolv_list_pending = true;
			return;
		} else {
			BT_LOGE("[PRIVACY] clear failed: cause %d\r\n", cause);
			return;
		}
	}
	for (i = 0; i < bond_storage_num; i++) {
		if (privacy_table[i].is_used && privacy_table[i].state != PRIVACY_RESOLVING_LIST_IDLE) {
			if (privacy_table[i].state == PRIVACY_RESOLVING_LIST_ADD_PENDING) {
				BT_LOGD("[PRIVACY] Add: i %d, BD %02x:%02x:%02x:%02x:%02x:%02x, type %d\r\n", i,
						privacy_table[i].addr[5], privacy_table[i].addr[4],
						privacy_table[i].addr[3], privacy_table[i].addr[2],
						privacy_table[i].addr[1], privacy_table[i].addr[0],
						privacy_table[i].remote_bd_type);
				cause = le_privacy_modify_resolv_list(GAP_RESOLV_LIST_OP_ADD,
													  privacy_table[i].remote_bd_type,
													  privacy_table[i].addr);
				if (cause == GAP_CAUSE_SUCCESS) {
					privacy_modify_resolv_list_pending = true;
					privacy_modify_resolv_list_idx = i;
					privacy_table[i].is_added = PRIVACY_RESOLVING_ADDED_PENDING;
					if (privacy_whitelist) {
						le_modify_white_list(GAP_WHITE_LIST_OP_ADD, privacy_table[i].addr,
											 (T_GAP_REMOTE_ADDR_TYPE)privacy_table[i].remote_bd_type);
					}
					if (privacy_table[i].device_mode) {
						le_privacy_set_mode(privacy_table[i].remote_bd_type, privacy_table[i].addr,
											GAP_PRIVACY_MODE_DEVICE);
					}
					return;
				} else if (cause != GAP_CAUSE_INVALID_STATE) {
					BT_LOGE("[PRIVACY] Add failed: cause %d, remove i=%d from pend add\r\n", cause, i);
					memset(&privacy_table[i], 0, sizeof(T_LE_PRIVACY_ENTRY));
				} else {
					BT_LOGE("[PRIVACY] Add failed: invalid state");
					return;
				}
			} else {
				BT_LOGD("[PRIVACY] Remove: i %d, BD %02x:%02x:%02x:%02x:%02x:%02x, type %d\r\n", i,
						privacy_table[i].addr[5], privacy_table[i].addr[4],
						privacy_table[i].addr[3], privacy_table[i].addr[2],
						privacy_table[i].addr[1], privacy_table[i].addr[0],
						privacy_table[i].remote_bd_type);
				cause = le_privacy_modify_resolv_list(GAP_RESOLV_LIST_OP_REMOVE,
													  privacy_table[i].remote_bd_type,
													  privacy_table[i].addr);
				if (cause == GAP_CAUSE_SUCCESS) {
					privacy_modify_resolv_list_pending = true;
					privacy_modify_resolv_list_idx = i;
					if (privacy_whitelist) {
						le_modify_white_list(GAP_WHITE_LIST_OP_REMOVE, privacy_table[i].addr,
											 (T_GAP_REMOTE_ADDR_TYPE)privacy_table[i].remote_bd_type);
					}
					return;
				} else if (cause != GAP_CAUSE_INVALID_STATE) {
					BT_LOGE("[PRIVACY] Remove failed: cause %d, remove i=%d from pend add\r\n", cause, i);
					memset(&privacy_table[i], 0, sizeof(T_LE_PRIVACY_ENTRY));
				} else {
					BT_LOGE("[PRIVACY] Remove failed: invalid state\r\n");
					return;
				}
			}
		}
	}
	BT_LOGD("[PRIVACY] privacy_handle_resolv_list_int: idle\r\n");
}

static void privacy_handle_resolv_list(bool indicate)
{
	bool adv_idle = false;
	T_GAP_DEV_STATE dev_state;

	if (!privacy_enable) {
		return;
	}

	le_get_gap_param(GAP_PARAM_DEV_STATE, &dev_state);

	adv_idle = bt_stack_le_gap_adv_is_idle();

	if (privacy_resolution_status == LE_PRIVACY_RESOLUTION_DISABLED
		|| (adv_idle && (dev_state.gap_conn_state == GAP_CONN_DEV_STATE_IDLE) &&
			dev_state.gap_scan_state == GAP_SCAN_STATE_IDLE)) {
		privacy_handle_resolv_list_int();
		return;
	}

	if (indicate) { /* Indicate APP if there is modification pending. */
		int i;
		bool pending = false;
		uint8_t bond_storage_num;

		if (privacy_clear_pending) {
			pending = true;
		}

		if (pending == false) {
			bond_storage_num = le_get_max_le_paired_device_num();
			for (i = 0; i < bond_storage_num; i++) {
				if (privacy_table[i].is_used && privacy_table[i].state != PRIVACY_RESOLVING_LIST_IDLE) {
					pending = true;
					break;
				}
			}
		}

		if (pending == false) {
			return;
		}

		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_le_resolv_list_pending_ind_t *p_ind = NULL;

		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_EVT_RESOLV_LIST_PENDING_IND,
									sizeof(rtk_bt_le_resolv_list_pending_ind_t));
		if (p_evt) {
			p_ind = (rtk_bt_le_resolv_list_pending_ind_t *)p_evt->data;
			p_ind->reason = 0;
			if (!adv_idle) {
				p_ind->reason |= RTK_BT_LE_RESOLV_LIST_PEND_BY_ADV;
			}
			if (dev_state.gap_conn_state != GAP_CONN_DEV_STATE_IDLE) {
				p_ind->reason |= RTK_BT_LE_RESOLV_LIST_PEND_BY_CONNECT;
			}
			if (dev_state.gap_scan_state != GAP_SCAN_STATE_IDLE) {
				p_ind->reason |= RTK_BT_LE_RESOLV_LIST_PEND_BY_SCAN;
			}

			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
}

static void privacy_modify_resolving_list(T_GAP_RESOLV_LIST_OP op, T_GAP_IDENT_ADDR_TYPE addr_type,
										  uint8_t *addr, bool device_mode)
{
	uint8_t bond_storage_num = le_get_max_le_paired_device_num();
	BT_LOGD("[PRIVACY] privacy_modify_resolving_list op = %d\r\n", op);
	switch (op) {
	case GAP_RESOLV_LIST_OP_CLEAR:
		privacy_clear_pending = true;
		break;

	case GAP_RESOLV_LIST_OP_ADD: {
		uint8_t i;
		BT_LOGD("[PRIVACY] privacy_modify_resolving_list add: addr %02x:%02x:%02x:%02x:%02x:%02x, addr type %d\r\n",
				addr[5], addr[4], addr[3], addr[2], addr[1], addr[0], addr_type);
		for (i = 0; i < bond_storage_num; i++) {
			if (privacy_table[i].is_used) {
				if ((privacy_table[i].remote_bd_type == addr_type)
					&& (memcmp(privacy_table[i].addr, addr, 6) == 0)) {
					BT_LOGE("[PRIVACY] privacy_modify_resolving_list add: failed, exist\r\n");
					return;
				}
			}
		}
		for (i = 0; i < bond_storage_num; i++) {
			if (!privacy_table[i].is_used) {
				privacy_table[i].is_used = true;
				privacy_table[i].remote_bd_type = addr_type;
				memcpy(privacy_table[i].addr, addr, 6);
				privacy_table[i].state = PRIVACY_RESOLVING_LIST_ADD_PENDING;
				privacy_table[i].device_mode = device_mode;
				return;
			}
		}
		BT_LOGE("[PRIVACY] privacy_add_device add: failed, no free entry\r\n");
	}
	break;

	case GAP_RESOLV_LIST_OP_REMOVE: {
		uint8_t i;
		BT_LOGD("[PRIVACY] privacy_add_device remove: addr %02x:%02x:%02x:%02x:%02x:%02x, addr type %d\r\n",
				addr[5], addr[4], addr[3], addr[2], addr[1], addr[0], addr_type);
		for (i = 0; i < bond_storage_num; i++) {
			if (privacy_table[i].is_used) {
				if ((privacy_table[i].remote_bd_type == addr_type)
					&& (memcmp(privacy_table[i].addr, addr, 6) == 0)) {
					if (privacy_table[i].is_added != PRIVACY_RESOLVING_ADDED_IDLE) {
						privacy_table[i].state = PRIVACY_RESOLVING_LIST_REMOVE_PENDING;
					} else {
						memset(&privacy_table[i], 0, sizeof(T_LE_PRIVACY_ENTRY));
						BT_LOGE("[PRIVACY] privacy_add_device remove: not added\r\n");
					}
					break;
				}
			}
		}
		BT_LOGE("[PRIVACY] privacy_add_device remove: failed, no find\r\n");
	}
	break;

	default:
		break;
	}
}

static bool privacy_add_device(T_LE_KEY_ENTRY *p_entry)
{
	if (p_entry != NULL && p_entry->is_used) {
		bool device_mode = true;
		T_LE_PRIVACY_INFO privacy_info;
		if (le_get_privacy_info(p_entry, &privacy_info)) {
			BT_LOGA("le_get_privacy_info(): %d %d %d\r\n", privacy_info.is_discov, privacy_info.central_addr_resolv, privacy_info.resolv_addr_only);
			if (privacy_info.is_discov && privacy_info.resolv_addr_only) {
				device_mode = false;
			}
		}
		if ((p_entry->remote_bd.remote_bd_type == GAP_REMOTE_ADDR_LE_PUBLIC) ||
			((p_entry->remote_bd.remote_bd_type == GAP_REMOTE_ADDR_LE_RANDOM) &&
			 ((p_entry->remote_bd.addr[5] & RANDOM_ADDR_MASK) == RANDOM_ADDR_MASK_STATIC))
		   ) {
			privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_ADD,
										  (T_GAP_IDENT_ADDR_TYPE)p_entry->remote_bd.remote_bd_type,
										  p_entry->remote_bd.addr, device_mode);
			return true;
		} else {
			if (p_entry->flags & LE_KEY_STORE_REMOTE_IRK_BIT) {
				privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_ADD,
											  (T_GAP_IDENT_ADDR_TYPE)p_entry->resolved_remote_bd.remote_bd_type,
											  p_entry->resolved_remote_bd.addr, device_mode);
				return true;
			} else {
				BT_LOGE("[PRIVACY] privacy_add_device: failed, idx %d\r\n", p_entry->idx);
			}
		}
	}
	return false;
}

static void privacy_handle_bond_modify_msg(T_LE_BOND_MODIFY_TYPE type, T_LE_KEY_ENTRY *p_entry)
{
	if (!privacy_enable) {
		return;
	}

	BT_LOGD("[PRIVACY] privacy_handle_bond_modify_msg: type 0x%x\r\n", type);

	if (type == LE_BOND_CLEAR) {
		privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_CLEAR, GAP_IDENT_ADDR_PUBLIC, NULL, false);
	} else if (type == LE_BOND_DELETE) {
		if (p_entry->flags & LE_KEY_STORE_REMOTE_IRK_BIT) {
			privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_REMOVE,
										  (T_GAP_IDENT_ADDR_TYPE)p_entry->resolved_remote_bd.remote_bd_type,
										  p_entry->resolved_remote_bd.addr, false);
		} else {
			privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_REMOVE,
										  (T_GAP_IDENT_ADDR_TYPE)p_entry->remote_bd.remote_bd_type,
										  p_entry->remote_bd.addr, false);
		}
	} else if (type == LE_BOND_ADD) {
		privacy_add_device(p_entry);
	}

	privacy_handle_resolv_list(true);
}

static uint16_t bt_stack_le_privacy_init(void *param)
{
	T_GAP_CAUSE cause;
	uint8_t i;
	uint16_t size = 0;
	T_LE_KEY_ENTRY *p_entry;
	uint8_t peer_addr[RTK_BD_ADDR_LEN] = {0};
	uint8_t bond_storage_num = le_get_max_le_paired_device_num();
	bool whitelist = *((bool *)param);

	privacy_whitelist = whitelist;
	BT_LOGD("[PRIVACY] privacy_init: whitelist %d\r\n", privacy_whitelist);
	if (privacy_table == NULL) {
		size = bond_storage_num * sizeof(T_LE_PRIVACY_ENTRY);
		privacy_table = osif_mem_alloc(RAM_TYPE_DATA_ON, size);
		if (!privacy_table) {
			return RTK_BT_ERR_NO_MEMORY;
		}
		le_privacy_register_cb(privacy_msg_callback);
	}

	/* Add a dummy resolving list whose peer addr and irk value is 0, use this way
	to start adv with RPA when local device has not bond with others. */
	cause = le_privacy_add_resolv_list(GAP_IDENT_ADDR_PUBLIC, peer_addr, NULL, true);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	memset(privacy_table, 0, size);
	for (i = 0; i < bond_storage_num; i++) {
		p_entry = le_find_key_entry_by_idx(i);
		privacy_add_device(p_entry);
	}

	privacy_enable = true;
	privacy_handle_resolv_list(true);

	cause = le_privacy_set_addr_resolution(true);
	if (cause) {
		BT_LOGE("bt_stack_le_privacy_set_addr_resolution: cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_set_privacy_mode(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_set_privacy_mode_param_t *set_privacy_param = (rtk_bt_le_set_privacy_mode_param_t *)param;

	if (!privacy_enable) {
		return RTK_BT_ERR_UNSUPPORTED;
	}
	cause = le_privacy_set_mode((T_GAP_IDENT_ADDR_TYPE)set_privacy_param->peer_ident_addr_type,
								set_privacy_param->peer_addr, (T_GAP_PRIVACY_MODE)set_privacy_param->privacy_mode);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_set_privacy_mode: cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_read_local_resolv_addr(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_read_local_rpa_param_t *rpa_param = (rtk_bt_le_read_local_rpa_param_t *)param;

	if (!privacy_enable) {
		return RTK_BT_ERR_UNSUPPORTED;
	}
	cause = le_privacy_read_local_resolv_addr((T_GAP_IDENT_ADDR_TYPE)rpa_param->peer_ident_addr_type,
											  rpa_param->peer_addr);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_read_local_resolv_addr: cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_read_peer_resolv_addr(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_read_peer_rpa_param_t *rpa_param = (rtk_bt_le_read_peer_rpa_param_t *)param;

	if (!privacy_enable) {
		return RTK_BT_ERR_UNSUPPORTED;
	}
	cause = le_privacy_read_peer_resolv_addr((T_GAP_IDENT_ADDR_TYPE)rpa_param->peer_ident_addr_type,
											 rpa_param->peer_addr);
	if (cause) {
		BT_LOGE("bt_stack_le_gap_read_peer_resolv_addr: cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return 0;
}
#endif

static uint16_t bt_stack_le_sm_set_pairing_mode(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_pairing_mode_t pairing_mode = *(rtk_bt_le_pairing_mode_t *)param;

	cause = gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(uint8_t), &pairing_mode);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = gap_set_pairable_mode();
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_sm_set_security_param(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_security_param_t *p_sec_param = (rtk_bt_le_security_param_t *)param;
	uint16_t auth_flags = 0;
	uint16_t auth_sec_req_flags = 0;

	cause = gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(uint8_t),
						  &p_sec_param->io_cap);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#if defined(F_BT_OOB_SUPPORT) && F_BT_OOB_SUPPORT
	cause = gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(uint8_t),
						  &p_sec_param->oob_data_flag);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	auth_flags = (!!p_sec_param->bond_flag) | (!!p_sec_param->mitm_flag) << 2 |
				 (!!p_sec_param->sec_pair_flag) << 3 | (!!p_sec_param->sec_pair_only_flag) << 9;
	cause = gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(uint16_t),
						  &auth_flags);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(uint8_t),
							  &p_sec_param->auto_sec_req);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	auth_sec_req_flags = auth_flags;
	cause = le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(uint16_t),
							  &auth_sec_req_flags);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	if (p_sec_param->use_fixed_key) {
		cause = le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(uint8_t),
								  &p_sec_param->use_fixed_key);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

		cause = le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(uint32_t),
								  &p_sec_param->fixed_key);
		if (cause) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
	}

	cause = gap_set_pairable_mode();
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_bond_set_param(GAP_PARAM_BOND_SIGN_KEY_FLAG, sizeof(uint8_t),
							  &p_sec_param->sign_key_flag);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_sm_get_security_param(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_security_param_t *p_sec_param = (rtk_bt_le_security_param_t *)param;
	uint16_t auth_flags = 0;

	cause = gap_get_param(GAP_PARAM_BOND_IO_CAPABILITIES, &p_sec_param->io_cap);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#if defined(F_BT_OOB_SUPPORT) && F_BT_OOB_SUPPORT
	cause = gap_get_param(GAP_PARAM_BOND_OOB_ENABLED, &p_sec_param->oob_data_flag);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	cause = gap_get_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, &auth_flags);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	p_sec_param->bond_flag = auth_flags & 0x1;
	p_sec_param->mitm_flag = (auth_flags >> 2) & 0x1;
	p_sec_param->sec_pair_flag = (auth_flags >> 3) & 0x1;
	p_sec_param->sec_pair_only_flag = (auth_flags >> 9) & 0x1;

	cause = le_bond_get_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, &p_sec_param->auto_sec_req);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_bond_get_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, &p_sec_param->use_fixed_key);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_bond_get_param(GAP_PARAM_BOND_FIXED_PASSKEY, &p_sec_param->fixed_key);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_sm_start_security(void *param)
{
	T_GAP_CAUSE cause;
	uint16_t conn_handle = *((uint16_t *)param);
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_bond_pair(conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	bt_stack_le_link_tbl[conn_id].is_pairing_initiator = 1;
	return 0;
}

static uint16_t bt_stack_le_sm_pairing_confirm(void *param)
{
	T_GAP_CAUSE cause;
	uint8_t conn_id;
	rtk_bt_le_pair_cfm_t *pair_cfm = (rtk_bt_le_pair_cfm_t *)param;
	T_GAP_CFM_CAUSE confirm =
		(pair_cfm->confirm ? GAP_CFM_CAUSE_ACCEPT : GAP_CFM_CAUSE_REJECT);

	if (!le_get_conn_id_by_handle(pair_cfm->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_bond_just_work_confirm(conn_id, confirm);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_sm_passkey_entry(void *param)
{
	T_GAP_CAUSE cause;
	T_GAP_CFM_CAUSE confirm = GAP_CFM_CAUSE_ACCEPT;
	rtk_bt_le_auth_key_input_t *key_input = (rtk_bt_le_auth_key_input_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(key_input->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_bond_passkey_input_confirm(conn_id,
										  key_input->passkey, confirm);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_sm_passkey_confirm(void *param)
{
	T_GAP_CAUSE cause;
	T_GAP_CFM_CAUSE confirm = GAP_CFM_CAUSE_ACCEPT;
	rtk_bt_le_auth_key_confirm_t *key_cfm = (rtk_bt_le_auth_key_confirm_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(key_cfm->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

#ifdef CONFIG_PLATFORM_TIZENRT_OS
	if (key_cfm->confirm == 1) {
		confirm = GAP_CFM_CAUSE_ACCEPT;
	} else if (key_cfm->confirm == 0) {
		confirm = GAP_CFM_CAUSE_REJECT;
	} else {
		return RTK_BT_ERR_PARAM_INVALID;
	}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

	cause = le_bond_user_confirm(conn_id, confirm);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_sm_set_oob_tk(void *param)
{
#if defined(F_BT_LE_SMP_OOB_SUPPORT) && F_BT_LE_SMP_OOB_SUPPORT
	T_GAP_CAUSE cause;
	rtk_bt_le_set_oob_key_t *set_oob_key = (rtk_bt_le_set_oob_key_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(set_oob_key->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, sizeof(set_oob_key->oob_key),
							  set_oob_key->oob_key);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	cause = le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#else
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;
#endif
}

static uint16_t bt_stack_le_sm_get_sc_local_oob(void *param)
{
#if defined(F_BT_LE_4_2_SC_OOB_SUPPORT) && F_BT_LE_4_2_SC_OOB_SUPPORT
	T_GAP_CAUSE cause;
	rtk_bt_le_sc_local_oob_data_t *oob_data = (rtk_bt_le_sc_local_oob_data_t *)param;
	uint8_t rand[16] = {0};
	T_GAP_LE_LOCAL_OOB_DATA local_oob = {0};

	osif_rand(rand, 16);
	cause = le_bond_sc_local_oob_init(NULL, rand, &local_oob);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	memcpy(oob_data->rand, local_oob.rand, sizeof(oob_data->rand));
	memcpy(oob_data->confirm, local_oob.confirm, sizeof(oob_data->confirm));

	return 0;
#else
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;
#endif
}

static uint16_t bt_stack_le_sm_input_sc_peer_oob(void *param)
{
#if defined(F_BT_LE_4_2_SC_OOB_SUPPORT) && F_BT_LE_4_2_SC_OOB_SUPPORT
	T_GAP_CAUSE cause;
	uint8_t conn_id = 0xFF;
	rtk_bt_le_sc_peer_oob_data_t *oob_data = (rtk_bt_le_sc_peer_oob_data_t *)param;
	T_GAP_LE_PEER_OOB_DATA peer_oob = {0};

	if (!le_get_conn_id(oob_data->addr, GAP_REMOTE_ADDR_LE_PUBLIC, &conn_id) &&
		!le_get_conn_id(oob_data->addr, GAP_REMOTE_ADDR_LE_RANDOM, &conn_id)) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	peer_oob.present = 1;
	memcpy(peer_oob.bd_addr_from, oob_data->addr, 6);
	memcpy(peer_oob.rand, oob_data->rand, sizeof(peer_oob.rand));
	memcpy(peer_oob.confirm, oob_data->confirm, sizeof(peer_oob.confirm));
	cause = le_bond_sc_peer_oob_init(&peer_oob);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
#else
	(void)param;
	return RTK_BT_ERR_UNSUPPORTED;
#endif
}

static uint16_t bt_stack_le_sm_get_bond_num(void *param)
{
	uint8_t *bond_num = (uint8_t *)param;
	*bond_num = le_get_bond_dev_num();

	return 0;
}

static uint16_t bt_stack_le_sm_get_bond_info(void *param)
{
	T_LE_KEY_ENTRY *key_entry;
	uint8_t i, max_pair_num, writed_size = 0;
	rtk_bt_le_bond_info_t *info =
		((rtk_bt_le_get_bond_info_param_t *)param)->p_bond_info;
	uint8_t *size = ((rtk_bt_le_get_bond_info_param_t *)param)->p_size;
	uint8_t allocated_size = *size; /* num of elements of rtk_bt_le_bond_info_t */

	max_pair_num = le_get_max_le_paired_device_num();
	for (i = 0; i < max_pair_num; i++) {
		key_entry = le_find_key_entry_by_idx(i);
		if (key_entry) {
			info[writed_size].remote_addr.type =
				(rtk_bt_le_addr_type_t)key_entry->remote_bd.remote_bd_type;
			memcpy(info[writed_size].remote_addr.addr_val, key_entry->remote_bd.addr,
				   RTK_BD_ADDR_LEN);
			info[writed_size].ident_addr.type =
				(rtk_bt_le_ident_addr_type_t)key_entry->resolved_remote_bd.remote_bd_type;
			memcpy(info[writed_size].ident_addr.addr_val,
				   key_entry->resolved_remote_bd.addr, RTK_BD_ADDR_LEN);
			writed_size++;
		}
		if (writed_size >= allocated_size) {
			break;
		}
	}
	*size = writed_size;

	return 0;
}

static uint16_t bt_stack_le_sm_delete_bond_device(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_addr_t *addr = (rtk_bt_le_addr_t *)param;

	cause = le_bond_delete_by_bd((uint8_t *)(addr->addr_val), (T_GAP_REMOTE_ADDR_TYPE)addr->type);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_sm_clear_bond_list(void)
{
	le_bond_clear_all_keys();
	return 0;
}

static uint16_t bt_stack_le_gap_get_tx_pending_num(void *param)
{
	(void)param;

	uint16_t gatts_tx_pending_num = 0, gattc_tx_pending_num = 0;
	rtk_bt_le_get_tx_pending_num_param_t *get_tx_pending_num_param = (rtk_bt_le_get_tx_pending_num_param_t *)param;

	//get gatts queue number
	bt_stack_le_gatts_get_tx_pending_num(get_tx_pending_num_param->conn_handle, &gatts_tx_pending_num);

#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	//get gattc queue number
	bt_stack_le_gattc_get_tx_pending_num(get_tx_pending_num_param->conn_handle, &gattc_tx_pending_num);
#endif

	*get_tx_pending_num_param->p_tx_pending_num = gatts_tx_pending_num + gattc_tx_pending_num;

	BT_LOGD("%s:gatts_tx_pending_num=%d,gattc_tx_pending_num=%d,tx_pending_num=%d\r\n", __func__, gatts_tx_pending_num, gattc_tx_pending_num,
			*get_tx_pending_num_param->p_tx_pending_num);


	return 0;
}

#if (defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && (defined(F_BT_LE_5_2_POWER_CONTROL_SUPPORT) && F_BT_LE_5_2_POWER_CONTROL_SUPPORT)
uint16_t bt_stack_le_gap_read_local_tx_power(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_txpower_read_local_t *p_param = (rtk_bt_le_txpower_read_local_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_rf_enhanced_read_transmit_power_level(conn_id,
													 (T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE)p_param->phy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

uint16_t bt_stack_le_gap_read_remote_tx_power(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_txpower_read_remote_t *p_param = (rtk_bt_le_txpower_read_remote_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_rf_read_remote_transmit_power_level(conn_id,
												   (T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE)p_param->phy);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

uint16_t bt_stack_le_gap_tx_power_report_set(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_txpower_report_set_t *p_param = (rtk_bt_le_txpower_report_set_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_rf_set_transmit_power_reporting_enable(conn_id, p_param->local_enable,
													  p_param->remote_enable);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

uint16_t bt_stack_le_gap_get_conn_id(uint16_t conn_handle, uint8_t *p_conn_id)
{
	uint8_t conn_id;
	int i;

	if (le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		*p_conn_id = conn_id;
		return 0;
	}

	/* disconnected device */
	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		if (bt_stack_le_conn_handle[i] == conn_handle) {
			*p_conn_id = i;
			return 0;
		}
	}

	return RTK_BT_ERR_PARAM_INVALID;
}

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT

static uint16_t bt_stack_le_gap_get_antenna_info(void)
{
	T_GAP_CAUSE cause = le_aox_read_antenna_information();

	if (cause) {
		BT_LOGE("[LE GAP] CTE get antenna info error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_connless_cte_rx_start(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_gap_connless_cte_rx_start_t *p_start;

	if (!param) {
		BT_LOGE("%s: invalid param\r\n", __func__);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_start = (rtk_bt_le_gap_connless_cte_rx_start_t *)param;
	if (!RTK_BLE_GAP_CTE_NUM_ANT_IDS_VALUE_IN_RANGE(p_start->param->num_ant_ids)) {
		/* To solve the lowerstack requirement switching_pattern_length > 0 .
		 * AoD RX use single antenna.
		 * Connectionless RX params don't include cte_type
		 */
		uint8_t ant_ids[2] = {0, 0};
		cause = le_aox_connless_receiver_set_iq_sampling_enable(
					p_start->sync_id,
					AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_ENABLED,
					p_start->param->slot_durations,
					p_start->param->max_sampled_ctes,
					2,
					ant_ids);
		BT_LOGD("[LE GAP] Connectionless cte rx start slot_durations %u, "    \
				"max_sampled_ctes %u, num_ant_ids 2, ant_ids[0] %u ant_ids[1] %u\r\n",
				p_start->param->slot_durations,
				p_start->param->max_sampled_ctes,
				ant_ids[0],
				ant_ids[1]);

	} else {
		cause = le_aox_connless_receiver_set_iq_sampling_enable(
					p_start->sync_id,
					AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_ENABLED,
					p_start->param->slot_durations,
					p_start->param->max_sampled_ctes,
					p_start->param->num_ant_ids,
					p_start->param->ant_ids);
		BT_LOGD("[LE GAP] Connectionless cte rx start slot_durations %u, "    \
				"max_sampled_ctes %u, num_ant_ids %u, ant_ids 0x%08x\r\n",
				p_start->param->slot_durations,
				p_start->param->max_sampled_ctes,
				p_start->param->num_ant_ids,
				p_start->param->ant_ids);
	}

	if (cause) {
		BT_LOGE("[LE GAP] Connectionless cte rx start error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return RTK_BT_OK;

}

static uint16_t bt_stack_le_gap_connless_cte_rx_stop(void *param)
{
	uint8_t sync_id;

	if (!param) {
		BT_LOGE("%s: invalid param\r\n", __func__);
		return RTK_BT_ERR_PARAM_INVALID;
	}
	sync_id = *((uint8_t *)param);

	T_GAP_CAUSE cause = le_aox_connless_receiver_set_iq_sampling_enable(
							sync_id,
							AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_DISABLED,
							0,
							0,
							0,
							NULL);

	if (cause) {
		BT_LOGE("[LE GAP] Connectionless cte rx stop error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_conn_cte_rx_start(void *param)
{
	uint8_t conn_id;
	uint8_t aod;
	T_GAP_CAUSE cause;
	rtk_bt_le_gap_conn_cte_rx_t *p_start = (rtk_bt_le_gap_conn_cte_rx_t *)param;

	if (!p_start || !p_start->p_rx_param) {
		BT_LOGE("[LE GAP] CTE rx start invalid parameter\r\n");
		return RTK_BT_ERR_PARAM_INVALID;
	}
	if (!le_get_conn_id_by_handle(p_start->conn_handle, &conn_id)) {
		BT_LOGE("[LE GAP] CTE rx start invalid conn_handle %u\r\n", p_start->conn_handle);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	aod = (p_start->p_rx_param->req_cte_type == RTK_BT_LE_GAP_CTE_TYPE_AOD_1US) ||
		  (p_start->p_rx_param->req_cte_type == RTK_BT_LE_GAP_CTE_TYPE_AOD_2US);
	if (aod && !RTK_BLE_GAP_CTE_NUM_ANT_IDS_VALUE_IN_RANGE(p_start->p_rx_param->num_ant_ids)) {
		/* To solve the lowerstack requirement switching_pattern_length > 0 .
		 * AoD RX use single antenna.
		 */
		uint8_t ant_ids[2] = {0, 0};
		cause = le_aox_set_conn_cte_receive_params(conn_id,
												   GAP_AOX_SAMPLING_ENABLE,
												   p_start->p_rx_param->slot_durations,
												   2,
												   ant_ids);
		BT_LOGD("[LE GAP] Connection cte rx start conn_id %u, slot_durations %u, num_ant_ids 2, ant_ids[0] %u, ant_ids[1] %u \r\n",
				conn_id, p_start->p_rx_param->slot_durations,
				ant_ids[0], ant_ids[1]);


	} else {
		cause = le_aox_set_conn_cte_receive_params(conn_id,
												   GAP_AOX_SAMPLING_ENABLE,
												   p_start->p_rx_param->slot_durations,
												   p_start->p_rx_param->num_ant_ids,
												   p_start->p_rx_param->ant_ids);
		BT_LOGD("[LE GAP] Connection cte rx start conn_id %u, slot_durations %u, num_ant_ids %u, ant_ids 0x%08x \r\n",
				conn_id, p_start->p_rx_param->slot_durations,
				p_start->p_rx_param->num_ant_ids, p_start->p_rx_param->ant_ids);
	}

	if (cause) {
		BT_LOGE("[LE GAP] Connection cte rx start error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_conn_cte_rx_stop(void *param)
{
	uint8_t conn_id;
	T_GAP_CAUSE cause;
	rtk_bt_le_gap_conn_cte_rx_t *p_stop;

	if (!param) {
		BT_LOGE("%s: invalid param\r\n", __func__);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_stop = (rtk_bt_le_gap_conn_cte_rx_t *)param;
	if (!le_get_conn_id_by_handle(p_stop->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_aox_set_conn_cte_receive_params(conn_id,
											   GAP_AOX_SAMPLING_DISABLE,
											   0,
											   0,
											   NULL);

	if (cause) {
		BT_LOGE("[LE GAP] Connection cte rx stop error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_conn_cte_tx_start(void *param)
{
	uint8_t conn_id;
	T_GAP_CAUSE cause;

	rtk_bt_le_gap_conn_cte_tx_start_t *p_start = (rtk_bt_le_gap_conn_cte_tx_start_t *)param;

	if (!p_start || !p_start->param) {
		BT_LOGE("[LE GAP] CTE tx start invalid parameter\r\n");
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!le_get_conn_id_by_handle(p_start->conn_handle, &conn_id)) {
		BT_LOGE("[LE GAP] CTE tx start invalid conn_handle %u\r\n", p_start->conn_handle);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_aox_set_conn_cte_transmit_params(conn_id,
												p_start->param->cte_types,
												p_start->param->num_ant_ids,
												p_start->param->ant_ids);
	BT_LOGD("[LE GAP] Connection cte tx start conn_id %u, cte_types %u, num_ant_ids %u, ant_ids 0x%08x \r\n",
			conn_id, p_start->param->cte_types, p_start->param->num_ant_ids, p_start->param->ant_ids);

	if (cause) {
		BT_LOGE("[LE GAP] Connection cte tx start error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_conn_cte_tx_stop(void *param)
{
	uint8_t conn_id;
	T_GAP_CAUSE cause;
	uint16_t conn_handle;

	if (!param) {
		BT_LOGE("%s: invalid param\r\n", __func__);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	conn_handle = *((uint16_t *)param);
	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = le_aox_conn_cte_response_enable(conn_id, GAP_AOX_CTE_RESPONSE_DISABLE);
	if (cause) {
		BT_LOGE("[LE GAP] Connection cte tx stop error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return RTK_BT_OK;
}

#if ((defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
    (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT))

static uint16_t bt_stack_le_gap_connless_cte_tx_start(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_le_gap_connless_cte_tx_start_t *p_start = (rtk_bt_le_gap_connless_cte_tx_start_t *)param;
	T_GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES cte_type;

	if (!param) {
		BT_LOGE("%s: invalid param\r\n", __func__);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cte_type = _get_hci_cte_type(p_start->param->cte_type);
	if (cte_type == GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES_AOA) {
		cause = le_aox_connless_transmitter_set_cte_transmit_params(p_start->adv_handle,
																	p_start->param->cte_len,
																	cte_type,
																	p_start->param->cte_count,
																	0,
																	NULL);
	} else {
		cause = le_aox_connless_transmitter_set_cte_transmit_params(p_start->adv_handle,
																	p_start->param->cte_len,
																	cte_type,
																	p_start->param->cte_count,
																	p_start->param->num_ant_ids,
																	p_start->param->ant_ids);
	}
	BT_LOGD("[LE GAP] Connectionless cte tx start adv_handle=%u, cte_len=%u, cte_type=%u, "   \
			"cte_count=%u, num_ant_ids=%u, ant_ids=%08x\r\n",
			p_start->adv_handle, p_start->param->cte_len, p_start->param->cte_type,
			p_start->param->cte_count, p_start->param->num_ant_ids, p_start->param->ant_ids);

	if (cause) {
		BT_LOGE("[LE GAP] Connectionless cte tx start error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return RTK_BT_OK;
}

static uint16_t bt_stack_le_gap_connless_cte_tx_stop(void *param)
{
	T_GAP_CAUSE cause;
	uint8_t adv_handle;

	if (!param) {
		BT_LOGE("%s: invalid param\r\n", __func__);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	adv_handle = *((uint8_t *)param);
	cause = le_aox_connless_transmitter_set_cte_transmit_enable(adv_handle,
																AOX_CONNLESS_TRANSMITTER_CTE_ENABLE_ADV_WITH_CTE_DISABLED);

	if (cause) {
		BT_LOGE("[LE GAP] Connectionless cte tx stop error, cause: 0x%x\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	return RTK_BT_OK;
}
#endif /* RTK_BLE_5_0_USE_EXTENDED_ADV && RTK_BLE_5_0_PA_ADV_SUPPORT */

#endif /* RTK_BLE_5_1_CTE_SUPPORT */

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
static uint16_t bt_stack_le_gap_coc_register_psm(void *param)
{
	rtk_bt_le_coc_psm_reg_param_t *reg_param = (rtk_bt_le_coc_psm_reg_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_coc_reg_psm(reg_param->le_psm, reg_param->is_register);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_coc_set_psm_security(void *param)
{
	rtk_bt_le_coc_set_psm_sec_param_t *sec_param = (rtk_bt_le_coc_set_psm_sec_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_coc_set_psm_security(sec_param->le_psm, (bool)sec_param->active,
									(T_LE_COC_SECURITY_MODE)sec_param->sec_mode,
									sec_param->key_size);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_coc_set_param(void *param)
{
	rtk_bt_le_coc_param_set_t *param_set = (rtk_bt_le_coc_param_set_t *)param;
	T_GAP_CAUSE cause;

	cause = le_coc_set_param((T_LE_COC_PARAM_TYPE)(param_set->param_type + COC_PARAM_CREDITS_THRESHOLD),
							 2, (void *)&param_set->value);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_coc_get_chan_param(void *param)
{
	rtk_bt_le_coc_chan_param_get_t *param_get = (rtk_bt_le_coc_chan_param_get_t *)param;
	T_GAP_CAUSE cause;

	cause = le_coc_get_chann_param((T_LE_COC_CHANN_PARAM_TYPE)(param_get->param_type + COC_CHANN_PARAM_CUR_CREDITS),
								   (void *)param_get->value, param_get->cid);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_coc_connect(void *param)
{
	rtk_bt_le_coc_connect_param_t *coc_conn = (rtk_bt_le_coc_connect_param_t *)param;
	T_GAP_CAUSE cause;
	uint8_t conn_id = 0;

	le_get_conn_id_by_handle(coc_conn->conn_handle, &conn_id);

	cause = le_coc_create(conn_id, coc_conn->le_psm);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_coc_disconnect(void *param)
{
	uint16_t cid = *(uint16_t *)param;
	T_GAP_CAUSE cause;

	cause = le_coc_disc(cid);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_coc_send_data(void *param)
{
	rtk_bt_le_coc_send_data_param_t *send_data = (rtk_bt_le_coc_send_data_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_coc_send_data(send_data->cid, send_data->data, send_data->len);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif /* RTK_BLE_COC_SUPPORT */

#if defined(F_BT_LE_4_0_DTM_SUPPORT) && F_BT_LE_4_0_DTM_SUPPORT
static uint16_t bt_stack_le_gap_dtm_rx_test_v1(void *param)
{
	rtk_bt_le_dtm_rx_param_t *p_rx_param = (rtk_bt_le_dtm_rx_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_dtm_receiver_test(p_rx_param->rx_channel);
	if (cause) {
		BT_LOGE("le_dtm_receiver_test cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_dtm_tx_test_v1(void *param)
{
	rtk_bt_le_dtm_tx_param_t *p_tx_param = (rtk_bt_le_dtm_tx_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_dtm_transmitter_test(p_tx_param->tx_channel,
									p_tx_param->data_len,
									(uint8_t)p_tx_param->pkt_pl);
	if (cause) {
		BT_LOGE("le_dtm_transmitter_test cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_dtm_end_test(void)
{
	T_GAP_CAUSE cause;

	cause = le_dtm_test_end();
	if (cause) {
		BT_LOGE("le_dtm_test_end cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

#if defined(F_BT_LE_5_0_DTM_SUPPORT) && F_BT_LE_5_0_DTM_SUPPORT
static uint16_t bt_stack_le_gap_dtm_rx_test_v2(void *param)
{
	rtk_bt_le_dtm_rx_param_t *p_rx_param = (rtk_bt_le_dtm_rx_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_dtm_enhanced_receiver_test(p_rx_param->rx_channel,
										  (uint8_t)p_rx_param->phy,
										  (uint8_t)p_rx_param->mod_idx);
	if (cause) {
		BT_LOGE("le_dtm_enhanced_receiver_test cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_dtm_tx_test_v2(void *param)
{
	rtk_bt_le_dtm_tx_param_t *p_tx_param = (rtk_bt_le_dtm_tx_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_dtm_enhanced_transmitter_test(p_tx_param->tx_channel,
											 p_tx_param->data_len,
											 (uint8_t)p_tx_param->pkt_pl,
											 (uint8_t)p_tx_param->phy);
	if (cause) {
		BT_LOGE("le_dtm_enhanced_transmitter_test cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

#if defined(F_BT_LE_5_1_DTM_SUPPORT) && F_BT_LE_5_1_DTM_SUPPORT
static uint16_t bt_stack_le_gap_dtm_rx_test_v3(void *param)
{
	rtk_bt_le_dtm_rx_param_t *p_rx_param = (rtk_bt_le_dtm_rx_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_dtm_receiver_test_v3(p_rx_param->rx_channel,
									(T_GAP_PHYS_TYPE)p_rx_param->phy,
									(T_GAP_DTM_MODULATION_INDEX_TYPE)p_rx_param->mod_idx,
									p_rx_param->exp_cte_len,
									(T_GAP_CTE_TYPE)p_rx_param->exp_cte_type,
									(T_GAP_SLOT_DUATIONS_TYPE)p_rx_param->slot_durations,
									p_rx_param->sw_pattern_len,
									p_rx_param->p_antenna_ids);
	if (cause) {
		BT_LOGE("le_dtm_receiver_test_v3 cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_le_gap_dtm_tx_test_v3(void *param)
{
	rtk_bt_le_dtm_tx_param_t *p_tx_param = (rtk_bt_le_dtm_tx_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_dtm_transmitter_test_v3(p_tx_param->tx_channel,
									   p_tx_param->data_len,
									   (T_GAP_DTM_PACKET_PAYLOAD_TYPE)p_tx_param->pkt_pl,
									   (T_GAP_DTM_PHYS_TYPE)p_tx_param->phy,
									   p_tx_param->cte_len,
									   (T_GAP_CTE_TYPE)p_tx_param->cte_type,
									   p_tx_param->sw_pattern_len,
									   p_tx_param->p_antenna_ids);
	if (cause) {
		BT_LOGE("le_dtm_transmitter_test_v3 cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

#if defined(F_BT_LE_5_2_DTM_SUPPORT) && F_BT_LE_5_2_DTM_SUPPORT
static uint16_t bt_stack_le_gap_dtm_tx_test_v4(void *param)
{
	rtk_bt_le_dtm_tx_param_t *p_tx_param = (rtk_bt_le_dtm_tx_param_t *)param;
	T_GAP_CAUSE cause;

	cause = le_dtm_transmitter_test_v4(p_tx_param->tx_channel,
									   p_tx_param->data_len,
									   (T_GAP_DTM_PACKET_PAYLOAD_TYPE)p_tx_param->pkt_pl,
									   (T_GAP_DTM_PHYS_TYPE)p_tx_param->phy,
									   p_tx_param->cte_len,
									   (T_GAP_CTE_TYPE)p_tx_param->cte_type,
									   p_tx_param->sw_pattern_len,
									   p_tx_param->p_antenna_ids,
									   p_tx_param->tx_power_level);
	if (cause) {
		BT_LOGE("le_dtm_transmitter_test_v4 cause = 0x%x \r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif
#endif /* F_BT_LE_4_0_DTM_SUPPORT */

uint16_t bt_stack_le_gap_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;
	BT_LOGD("bt_stack_le_gap_act_handle: act = %d \r\n", p_cmd->act);
	switch (p_cmd->act) {
	case RTK_BT_LE_GAP_ACT_GET_VERSION:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_VERSION \r\n");
		ret = bt_stack_le_gap_get_version(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR \r\n");
		ret = bt_stack_le_gap_get_device_addr(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_DEVICE_NAME:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_DEVICE_NAME \r\n");
		ret = bt_stack_le_gap_set_device_name(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_APPEARANCE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_APPEARANCE \r\n");
		ret = bt_stack_le_gap_set_apperance(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_PREFERRED_CONN_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_PREFERRED_CONN_PARAM \r\n");
		ret = bt_stack_le_gap_set_preferred_conn_param(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_RAND_ADDR:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_RAND_ADDR \r\n");
		p_cmd->user_data = GAP_MSG_LE_SET_RAND_ADDR;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_set_rand_addr(p_cmd->param);
		goto async_handle;
		break;
	case RTK_BT_LE_GAP_ACT_GEN_RAND_ADDR:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GEN_RAND_ADDR \r\n");
		ret = bt_stack_le_gap_gen_rand_addr(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_CFG_LOCAL_IDENT_ADDR:
		BT_LOGD("RTK_BT_LE_GAP_ACT_CFG_LOCAL_IDENT_ADDR");
		ret = bt_stack_le_gap_cfg_local_ident_addr(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_ADV_DATA:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_ADV_DATA \r\n");
		ret = bt_stack_le_gap_set_adv_data(p_cmd->param, p_cmd->param_len);
		break;
	case RTK_BT_LE_GAP_ACT_SET_SCAN_RSP_DATA:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_SCAN_RSP_DATA \r\n");
		ret = bt_stack_le_gap_set_scan_rsp_data(p_cmd->param, p_cmd->param_len);
		break;
	case RTK_BT_LE_GAP_ACT_START_ADV:
		BT_LOGD("RTK_BT_LE_GAP_ACT_START_ADV \r\n");
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
    (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT) && \
    (!defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV)
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
		if (rtk_bt_mesh_is_enable()) {
			ret = bt_stack_le_gap_start_adv(p_cmd->param);
		} else
#endif  // RTK_BLE_MESH_SUPPORT && RTK_BLE_MESH_BASED_ON_CODED_PHY
		{
			p_cmd->user_data = (RTK_BT_LE_GAP_ACT_START_EXT_ADV << 8);
			bt_stack_pending_cmd_insert(p_cmd);
			ret = bt_stack_le_gap_start_adv(p_cmd->param);
			goto async_handle;
		}
#else
		ret = bt_stack_le_gap_start_adv(p_cmd->param);
#endif
		break;
	case RTK_BT_LE_GAP_ACT_STOP_ADV:
		BT_LOGD("RTK_BT_LE_GAP_ACT_STOP_ADV \r\n");
		ret = bt_stack_le_gap_stop_adv();
		break;
	case RTK_BT_LE_GAP_ACT_GET_ADV_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_ADV_PARAM \r\n");
		ret = bt_stack_le_gap_get_adv_param(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_ADV_STATE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_ADV_STATE \r\n");
		ret = (uint16_t)bt_stack_le_gap_adv_is_idle();
		break;
#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_ADV_SUPPORT) && F_BT_LE_5_0_AE_ADV_SUPPORT)
	case RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA \r\n");
		ret = bt_stack_le_gap_set_ext_adv_data(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA \r\n");
		ret = bt_stack_le_gap_set_ext_scan_rsp_data(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV:
		BT_LOGD("RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV \r\n");
		ret = bt_stack_le_gap_create_ext_adv(p_cmd->param);
#if !defined(RTK_BLE_MGR_LIB_EADV) || !RTK_BLE_MGR_LIB_EADV
		if (!ret) {
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
			if (!rtk_bt_mesh_is_enable())
#endif  // RTK_BLE_MESH_SUPPORT && RTK_BLE_MESH_BASED_ON_CODED_PHY
			{
				p_cmd->user_data = GAP_MSG_LE_EXT_ADV_START_SETTING;
				bt_stack_pending_cmd_insert(p_cmd);
				ret = _ext_adv_param_take_effect(p_cmd->param); /* Some of PA param, like AoA/AoD, must set after EA param takes effect. */
				goto async_handle;
			}
		}
#endif
		break;
	case RTK_BT_LE_GAP_ACT_START_EXT_ADV:
		BT_LOGD("RTK_BT_LE_GAP_ACT_START_EXT_ADV \r\n");
#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
		ret = bt_stack_le_gap_start_ext_adv(p_cmd->param);
#else
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
		if (rtk_bt_mesh_is_enable()) {
			ret = bt_stack_le_gap_start_ext_adv(p_cmd->param);
		} else
#endif  // RTK_BLE_MESH_SUPPORT && RTK_BLE_MESH_BASED_ON_CODED_PHY
		{
			p_cmd->user_data = (RTK_BT_LE_GAP_ACT_START_EXT_ADV << 8);
			bt_stack_pending_cmd_insert(p_cmd);
			ret = bt_stack_le_gap_start_ext_adv(p_cmd->param);
			goto async_handle;
		}
#endif
		break;
	case RTK_BT_LE_GAP_ACT_STOP_EXT_ADV:
		BT_LOGD("RTK_BT_LE_GAP_ACT_STOP_EXT_ADV \r\n");
#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
		ret = bt_stack_le_gap_stop_ext_adv(p_cmd->param);
#else
		p_cmd->user_data = GAP_MSG_LE_EXT_ADV_STATE_CHANGE_INFO;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_stop_ext_adv(p_cmd->param);
		goto async_handle;
#endif
		break;
	case RTK_BT_LE_GAP_ACT_REMOVE_EXT_ADV:
		BT_LOGD("RTK_BT_LE_GAP_ACT_REMOVE_EXT_ADV \r\n");
#if defined(RTK_BLE_MGR_LIB_EADV) && RTK_BLE_MGR_LIB_EADV
		ret = bt_stack_le_gap_remove_ext_adv(p_cmd->param);
#else
		p_cmd->user_data = GAP_MSG_LE_EXT_ADV_REMOVE_SET;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_remove_ext_adv(p_cmd->param);
		goto async_handle;
#endif
		break;
	case RTK_BT_LE_GAP_ACT_GET_EXT_ADV_HANDLE_BY_CONN_HANDLE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_EXT_ADV_HANDLE_BY_CONN_HANDLE \r\n");
		ret = bt_stack_le_gap_get_ext_adv_handle_by_conn_handle(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_EXT_CONN:
		BT_LOGD("RTK_BT_LE_GAP_ACT_EXT_CONN \r\n");
		ret = bt_stack_le_gap_ext_connect(p_cmd->param);
		break;
#endif

#if (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT) && (defined(F_BT_LE_5_0_PA_ADV_SUPPORT) && F_BT_LE_5_0_PA_ADV_SUPPORT)
	case RTK_BT_LE_GAP_ACT_START_PA:
		BT_LOGD("RTK_BT_LE_GAP_ACT_START_PA \r\n");
		p_cmd->user_data = (RTK_BT_LE_GAP_ACT_START_PA << 8);
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_start_pa(p_cmd->param);
		goto async_handle;
		break;
	case RTK_BT_LE_GAP_ACT_STOP_PA:
		BT_LOGD("RTK_BT_LE_GAP_ACT_STOP_PA \r\n");
		p_cmd->user_data = GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_stop_pa(p_cmd->param);
		goto async_handle;
		break;
	case RTK_BT_LE_GAP_ACT_UPDATE_PA:
		BT_LOGD("RTK_BT_LE_GAP_ACT_UPDATE_PA \r\n");
		p_cmd->user_data = (RTK_BT_LE_GAP_ACT_UPDATE_PA << 8);
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_update_pa(p_cmd->param);
		goto async_handle;
		break;
#endif

#if (defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT) && (defined(F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_0_PA_SYNC_SUPPORT)
	case RTK_BT_LE_GAP_ACT_PA_SYNC_MODIFY_ADV_LIST:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PA_SYNC_MODIFY_ADV_LIST \r\n");
		p_cmd->user_data = GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_pa_sync_modify_adv_list(p_cmd->param);
		goto async_handle;
		break;
	case RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE \r\n");
		ret = bt_stack_le_gap_pa_sync_create(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_PA_SYNC_TERMINATE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PA_SYNC_TERMINATE \r\n");
		ret = bt_stack_le_gap_pa_sync_terminate(p_cmd->param);
		break;
#endif

#if (defined(RTK_BLE_5_1_PAST_SENDER_SUPPORT) && RTK_BLE_5_1_PAST_SENDER_SUPPORT) && (defined(F_BT_LE_5_1_PAST_SENDER_SUPPORT) && F_BT_LE_5_1_PAST_SENDER_SUPPORT)
	case RTK_BT_LE_GAP_ACT_PAST_TRANSFER:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PAST_TRANSFER \r\n");
		ret = bt_stack_le_gap_past_send(p_cmd->param);
		break;
#endif

#if (defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && (defined(F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT) && F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
	case RTK_BT_LE_GAP_ACT_PAST_RECV_SET:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PAST_RECV_SET \r\n");
		ret = bt_stack_le_gap_past_recv_set(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_DEFAULT_PAST_RECV_SET:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PAST_RECV_SET \r\n");
		ret = bt_stack_le_gap_default_past_recv_set(p_cmd->param);
		break;
#endif

	case RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER \r\n");
		ret = bt_stack_le_gap_scan_info_filter(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_SCAN_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_SCAN_PARAM \r\n");
		ret = bt_stack_le_gap_set_scan_param(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_GET_SCAN_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_SCAN_PARAM \r\n");
		ret = bt_stack_le_gap_get_scan_param(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_START_SCAN:
		BT_LOGD("RTK_BT_LE_GAP_ACT_START_SCAN \r\n");
		ret = bt_stack_le_gap_start_scan();
		break;
	case RTK_BT_LE_GAP_ACT_STOP_SCAN:
		BT_LOGD("RTK_BT_LE_GAP_ACT_STOP_SCAN \r\n");
		ret = bt_stack_le_gap_stop_scan();
		break;

#if (defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && (defined(F_BT_LE_5_0_AE_SCAN_SUPPORT) && F_BT_LE_5_0_AE_SCAN_SUPPORT)
	case RTK_BT_LE_GAP_ACT_EXT_SCAN_SET_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_EXT_SCAN_SET_PARAM \r\n");
		ret = bt_stack_le_gap_ext_scan_set_param(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_EXT_SCAN_START:
		BT_LOGD("RTK_BT_LE_GAP_ACT_EXT_SCAN_START \r\n");
		ret = bt_stack_le_gap_ext_scan_start();
		break;
	case RTK_BT_LE_GAP_ACT_EXT_SCAN_STOP:
		BT_LOGD("RTK_BT_LE_GAP_ACT_EXT_SCAN_STOP \r\n");
		ret = bt_stack_le_gap_ext_scan_stop();
		break;
#endif

	case RTK_BT_LE_GAP_ACT_CONN:
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONN \r\n");
		ret = bt_stack_le_gap_connect(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_CONN_CANCEL:
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONN_CANCEL \r\n");
		ret = bt_stack_le_gap_connnect_cancel(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_DISCONN:
		BT_LOGD("RTK_BT_LE_GAP_ACT_DISCONN \r\n");
		ret = bt_stack_le_gap_disconnect(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_DISCONN_WITH_REASON:
		BT_LOGD("RTK_BT_LE_GAP_ACT_DISCONN_WITH_REASON \r\n");
		ret = bt_stack_le_gap_disconnect_with_reason(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_UPDATE_CONN_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_UPDATE_CONN_PARAM \r\n");
		ret = bt_stack_le_gap_update_conn_param(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_READ_RSSI:
		BT_LOGD("RTK_BT_LE_GAP_ACT_READ_RSSI \r\n");
		p_cmd->user_data = GAP_MSG_LE_READ_RSSI;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_read_rssi(p_cmd->param);
		goto async_handle;
		break;

#if defined(F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT) && F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
	case RTK_BT_LE_GAP_ACT_READ_REMOTE_VERSION:
		BT_LOGD("RTK_BT_LE_GAP_ACT_READ_REMOTE_VERSION \r\n");
		ret = bt_stack_le_gap_read_remote_version(p_cmd->param);
		break;
#endif

	case RTK_BT_LE_GAP_ACT_MODIFY_WHITELIST:
		BT_LOGD("RTK_BT_LE_GAP_ACT_MODIFY_WHITELIST \r\n");
		p_cmd->user_data = GAP_MSG_LE_MODIFY_WHITE_LIST;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_modify_whitelist(p_cmd->param);
		goto async_handle;
		break;

	case RTK_BT_LE_GAP_ACT_GET_CONN_INFO:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_CONN_INFO \r\n");
		ret = bt_stack_le_gap_get_conn_info(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_DEV_STATE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_DEV_STATE \r\n");
		ret = bt_stack_le_gap_get_dev_state(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN \r\n");
		ret = bt_stack_le_gap_get_active_conn(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR \r\n");
		ret = bt_stack_le_gap_get_conn_handle_by_addr(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_MTU_SIZE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_MTU_SIZE \r\n");
		ret = bt_stack_le_gap_get_mtu_size(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_SET_MAX_MTU_SIZE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_MAX_MTU_SIZE \r\n");
		ret = bt_stack_le_gap_set_max_mtu_size(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_SET_CHANNELS:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_CHANNELS \r\n");
		p_cmd->user_data = GAP_MSG_LE_SET_HOST_CHANN_CLASSIF;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_set_channels(p_cmd->param);
		goto async_handle;
		break;

#if defined(F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT) && F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
	case RTK_BT_LE_GAP_ACT_SET_DATA_LEN:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_DATA_LEN \r\n");
		p_cmd->user_data = GAP_MSG_LE_SET_DATA_LEN;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_set_data_len(p_cmd->param);
		goto async_handle;
		break;
#endif

	case RTK_BT_LE_GAP_ACT_SET_PHY:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_PHY \r\n");
		ret = bt_stack_le_gap_set_phy(p_cmd->param);
		break;

#if (defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT) && (defined(F_BT_LE_PRIVACY_SUPPORT) && F_BT_LE_PRIVACY_SUPPORT)
	case RTK_BT_LE_GAP_ACT_PRIVACY_INIT:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PRIVACY_INIT \r\n");
		ret = bt_stack_le_privacy_init(p_cmd->param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_PRIVACY_MODE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_PRIVACY_MODE \r\n");
		p_cmd->user_data = GAP_MSG_LE_PRIVACY_SET_MODE;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_set_privacy_mode(p_cmd->param);
		goto async_handle;
		break;
	case RTK_BT_LE_GAP_ACT_READ_LOCAL_RESOLV_ADDR:
		BT_LOGD("RTK_BT_LE_GAP_ACT_READ_LOCAL_RESOLV_ADDR \r\n");
		p_cmd->user_data = GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_read_local_resolv_addr(p_cmd->param);
		goto async_handle;
		break;
	case RTK_BT_LE_GAP_ACT_READ_PEER_RESOLV_ADDR:
		BT_LOGD("RTK_BT_LE_GAP_ACT_READ_PEER_RESOLV_ADDR \r\n");
		p_cmd->user_data = GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_read_peer_resolv_addr(p_cmd->param);
		goto async_handle;
		break;
#endif

	case RTK_BT_LE_GAP_ACT_SET_PAIRING_MODE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_PAIRING_MODE \r\n");
		ret = bt_stack_le_sm_set_pairing_mode(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_SET_SEC_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_SET_SEC_PARAM \r\n");
		ret = bt_stack_le_sm_set_security_param(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_SEC_PARAM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_SEC_PARAM \r\n");
		ret = bt_stack_le_sm_get_security_param(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_START_SECURITY:
		BT_LOGD("RTK_BT_LE_GAP_ACT_START_SECURITY \r\n");
		ret = bt_stack_le_sm_start_security(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_PAIRING_CONFIRM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PAIRING_CONFIRM \r\n");
		ret = bt_stack_le_sm_pairing_confirm(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_PASSKEY_ENTRY:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PASSKEY_ENTRY \r\n");
		ret = bt_stack_le_sm_passkey_entry(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_PASSKEY_CONFIRM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_PASSKEY_CONFIRM \r\n");
		ret = bt_stack_le_sm_passkey_confirm(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_OOB_KEY_INPUT:
		BT_LOGD("RTK_BT_LE_GAP_ACT_OOB_KEY_INPUT \r\n");
		ret = bt_stack_le_sm_set_oob_tk(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_SC_LOCAL_OOB:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_SC_LOCAL_OOB \r\n");
		ret = bt_stack_le_sm_get_sc_local_oob(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_INPUT_SC_PEER_OOB:
		BT_LOGD("RTK_BT_LE_GAP_ACT_INPUT_SC_PEER_OOB \r\n");
		ret = bt_stack_le_sm_input_sc_peer_oob(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_BOND_NUM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_BOND_NUM \r\n");
		ret = bt_stack_le_sm_get_bond_num(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_GET_BOND_INFO:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_BOND_INFO \r\n");
		ret = bt_stack_le_sm_get_bond_info(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_DELETE_BOND_DEVICE:
		BT_LOGD("RTK_BT_LE_GAP_ACT_DELETE_BOND_DEVICE \r\n");
		ret = bt_stack_le_sm_delete_bond_device(p_cmd->param);
		break;

	case RTK_BT_LE_GAP_ACT_CLEAR_BOND_LIST:
		BT_LOGD("RTK_BT_LE_GAP_ACT_CLEAR_BOND_LIST \r\n");
		ret = bt_stack_le_sm_clear_bond_list();
		break;

	case RTK_BT_LE_GAP_ACT_GET_TX_PENDING_NUM:
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_TX_PENDING_NUM \r\n");
		ret = bt_stack_le_gap_get_tx_pending_num(p_cmd->param);
		break;

#if (defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && (defined(F_BT_LE_5_2_POWER_CONTROL_SUPPORT) && F_BT_LE_5_2_POWER_CONTROL_SUPPORT)
	case RTK_BT_LE_GAP_ACT_READ_LOCAL_TX_POWER:
		BT_LOGD("RTK_BT_LE_GAP_ACT_READ_LOCAL_TX_POWER \r\n");
		p_cmd->user_data = GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL | (GAP_MSG_LE_RF_INFO << 8);
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_read_local_tx_power(p_cmd->param);
		goto async_handle;
		break;

	case RTK_BT_LE_GAP_ACT_READ_REMOTE_TX_POWER:
		BT_LOGD("RTK_BT_LE_GAP_ACT_READ_REMOTE_TX_POWER \r\n");
		p_cmd->user_data = GAP_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL | (GAP_MSG_LE_RF_INFO << 8);
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_read_remote_tx_power(p_cmd->param);
		goto async_handle;
		break;

	case RTK_BT_LE_GAP_ACT_TX_POWER_REPORT_SET:
		BT_LOGD("RTK_BT_LE_GAP_ACT_TX_POWER_REPORT_SET \r\n");
		p_cmd->user_data = GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE | (GAP_MSG_LE_RF_INFO << 8);
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_tx_power_report_set(p_cmd->param);
		goto async_handle;
		break;
#endif

	case RTK_BT_LE_GAP_ACT_GET_CONN_ID: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_CONN_ID \r\n");
		rtk_bt_le_get_conn_id_t *p_param = (rtk_bt_le_get_conn_id_t *)p_cmd->param;
		ret = bt_stack_le_gap_get_conn_id(p_param->conn_handle, p_param->p_conn_id);
		break;
	}

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
	case RTK_BT_LE_GAP_ACT_GET_ANTENNA_INFO: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_GET_ANTENNA_INFO \r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_get_antenna_info();
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_CONNLESS_CTE_RX_START: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONNLESS_CTE_RX_START\r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_connless_cte_rx_start(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_CONNLESS_CTE_RX_STOP: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONNLESS_CTE_RX_STOP\r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_connless_cte_rx_stop(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_CONN_CTE_RX_START: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONN_CTE_RX_START\r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_conn_cte_rx_start(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_CONN_CTE_RX_STOP: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONN_CTE_RX_STOP\r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_conn_cte_rx_stop(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_CONN_CTE_TX_START: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONN_CTE_TX_START\r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_conn_cte_tx_start(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_CONN_CTE_TX_STOP: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONN_CTE_TX_STOP\r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_conn_cte_tx_stop(p_cmd->param);
		goto async_handle;
		break;
	}

#if ((defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
(defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT))

	case RTK_BT_LE_GAP_ACT_CONNLESS_CTE_TX_START: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONNLESS_CTE_TX_START\r\n");
		p_cmd->user_data = GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_connless_cte_tx_start(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_CONNLESS_CTE_TX_STOP: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_CONNLESS_CTE_TX_STOP\r\n");
		ret = bt_stack_le_gap_connless_cte_tx_stop(p_cmd->param);
		break;
	}
#endif /* RTK_BLE_5_0_USE_EXTENDED_ADV && RTK_BLE_5_0_PA_ADV_SUPPORT */

#endif /* RTK_BLE_5_1_CTE_SUPPORT */

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
	case RTK_BT_LE_GAP_ACT_COC_REG_PSM: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_COC_REG_PSM \r\n");
		p_cmd->user_data = GAP_COC_MSG_LE_REG_PSM;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_coc_register_psm(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_COC_SET_PSM_SECURITY: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_COC_SET_PSM_SECURITY \r\n");
		p_cmd->user_data = GAP_COC_MSG_LE_SET_PSM_SECURITY;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_coc_set_psm_security(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_COC_SET_PARAM: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_COC_SET_PARAM \r\n");
		ret = bt_stack_le_gap_coc_set_param(p_cmd->param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_COC_GET_CHAN_PARAM: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_COC_GET_CHAN_PARAM \r\n");
		ret = bt_stack_le_gap_coc_get_chan_param(p_cmd->param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_COC_CONN: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_COC_CONN \r\n");
		ret = bt_stack_le_gap_coc_connect(p_cmd->param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_COC_DISCONN: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_COC_DISCONN \r\n");
		ret = bt_stack_le_gap_coc_disconnect(p_cmd->param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_COC_SEND_DATA: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_COC_SEND_DATA \r\n");
		ret = bt_stack_le_gap_coc_send_data(p_cmd->param);
		break;
	}
#endif /* RTK_BLE_COC_SUPPORT */

#if defined(F_BT_LE_4_0_DTM_SUPPORT) && F_BT_LE_4_0_DTM_SUPPORT
	case RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V1: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V1 \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_RECEIVER_TEST;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_rx_test_v1(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V1: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V1 \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_TRANSMITTER_TEST;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_tx_test_v1(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_DTM_END_TEST: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_END_TEST \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_TEST_END;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_end_test();
		goto async_handle;
		break;
	}

#if defined(F_BT_LE_5_0_DTM_SUPPORT) && F_BT_LE_5_0_DTM_SUPPORT
	case RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V2: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V2 \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_rx_test_v2(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V2: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V2 \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_tx_test_v2(p_cmd->param);
		goto async_handle;
		break;
	}
#endif

#if defined(F_BT_LE_5_1_DTM_SUPPORT) && F_BT_LE_5_1_DTM_SUPPORT
	case RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V3: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V3 \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_RECEIVER_TEST_V3;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_rx_test_v3(p_cmd->param);
		goto async_handle;
		break;
	}

	case RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V3: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V3 \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_tx_test_v3(p_cmd->param);
		goto async_handle;
		break;
	}
#endif

#if defined(F_BT_LE_5_2_DTM_SUPPORT) && F_BT_LE_5_2_DTM_SUPPORT
	case RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V4: {
		BT_LOGD("RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V4 \r\n");
		p_cmd->user_data = GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_le_gap_dtm_tx_test_v4(p_cmd->param);
		goto async_handle;
		break;
	}
#endif
#endif /* F_BT_LE_4_0_DTM_SUPPORT */
	default:
		BT_LOGE("bt_stack_le_act_handle: unknown act: %d \r\n", p_cmd->act);
		ret = RTK_BT_ERR_NO_CASE_ELEMENT;
		break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;

async_handle:
	if (ret) {
		/*  if bt_stack_xxx failed, shall not wait for callback, and the pending p_cmd
		    should be deleted here */
		bt_stack_pending_cmd_delete(p_cmd);
		p_cmd->ret = ret;
		osif_sem_give(p_cmd->psem);
	}
	return ret;
}
