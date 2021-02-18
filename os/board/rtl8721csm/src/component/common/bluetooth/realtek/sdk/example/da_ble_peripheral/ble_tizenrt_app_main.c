/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      main.c
   * @brief     Source file for BLE peripheral project, mainly used for initialize modules
   * @author    jane
   * @date      2017-06-12
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <os_sched.h>
#include <string.h>
#include <trace_app.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include <simple_ble_config.h>
#include <bte.h>
#include <gap_config.h>
#include <bt_flags.h>
#include "ble_tizenrt_app_task.h"
#include "ble_tizenrt_app.h"
#include "ble_tizenrt_app_flags.h"
#include <stdio.h>
#include "wifi_constants.h"
#include <wifi/wifi_conf.h>
#include "rtk_coex.h"
#include <da_ble_server.h>
#include <ble_tizenrt_service.h>
#include <os_mem.h>
/** @defgroup  PERIPH_DEMO_MAIN Peripheral Main
    * @brief Main file to initialize hardware and BT stack and start task scheduling
    * @{
    */

/*============================================================================*
 *                              Constants
 *============================================================================*/


/*============================================================================*
 *                              Variables
 *============================================================================*/
void print_no_server(const char* format, ...) {}

/** @brief  GAP - scan response data (max size = 31 bytes) */
static const uint8_t scan_rsp_data[] =
{
    0x03,                             /* length */
    GAP_ADTYPE_APPEARANCE,            /* type="Appearance" */
    LO_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
    HI_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
};

/** @brief  GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */
static const uint8_t adv_data[] =
{
    /* Flags */
    0x02,             /* length */
    GAP_ADTYPE_FLAGS, /* type="Flags" */
    GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
    /* Service */
    0x03,             /* length */
    GAP_ADTYPE_16BIT_COMPLETE,
    LO_WORD(GATT_UUID_SIMPLE_PROFILE),
    HI_WORD(GATT_UUID_SIMPLE_PROFILE),
    /* Local name */
    0x0C,             /* length */
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,
    'B', 'L', 'E', '_', 'T', 'I', 'Z', 'E', 'N', 'R', 'T',
};

/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
 * @brief  Config bt stack related feature
 *
 * NOTE: This function shall be called before @ref bte_init is invoked.
 * @return void
 */
extern void gap_config_hci_task_secure_context(uint32_t size);
extern void gap_config_deinit_flow(uint8_t deinit_flow);
void ble_tizenrt_stack_config_init(void)
{
    gap_config_max_le_link_num(BLE_TIZENRT_PERIPHERAL_APP_MAX_LINKS);
    gap_config_max_le_paired_device(BLE_TIZENRT_PERIPHERAL_APP_MAX_PAIRED_DEV);
    gap_config_hci_task_secure_context (280);
    gap_config_deinit_flow(1);
}

/**
  * @brief  Initialize peripheral and gap bond manager related parameters
  * @return void
  */
extern da_ble_server_init_parm server_init_parm;
void ble_tizenrt_app_le_gap_init(void)
{
    /* Device name and device appearance */
    uint8_t  device_name[GAP_DEVICE_NAME_LEN] = "BLE_TIZENRT";
    uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
    uint8_t  slave_init_mtu_req = false;


    /* Advertising parameters */
    uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
    uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    uint8_t  adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
    uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
    uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
    uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MAX;

    /* GAP Bond Manager parameters */
    debug_print("\r\n[%s] is_secured_connect_allowed %d", __FUNCTION__, server_init_parm.is_secured_connect_allowed);
    uint8_t  auth_pair_mode = (server_init_parm.is_secured_connect_allowed) ? GAP_PAIRING_MODE_PAIRABLE : GAP_PAIRING_MODE_NO_PAIRING;
    uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG;
    uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
#if F_BT_LE_SMP_OOB_SUPPORT
    uint8_t  auth_oob = false;
#endif
    uint8_t  auth_use_fix_passkey = false;
    uint32_t auth_fix_passkey = 0;
    uint8_t  auth_sec_req_enable = false;
    uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_BONDING_FLAG;

    /* Set device name and device appearance */
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req),
                     &slave_init_mtu_req);

    /* Set advertising parameters */
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
    le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), (void *)adv_data);
    le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);

    /* Setup the GAP Bond Manager */
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    if(server_init_parm.is_secured_connect_allowed)
    {
        gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
        gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
#if F_BT_LE_SMP_OOB_SUPPORT
        gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
#endif
        le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
        le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                        &auth_use_fix_passkey);
        le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
        le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                        &auth_sec_req_flags);
    }
    /* register gap message callback */
    le_register_app_cb(ble_tizenrt_app_gap_callback);
    debug_print("\r\n[%s] Init Done", __FUNCTION__);
}

/**
 * @brief  Add GATT services and register callbacks
 * @return void
 */



extern TIZENERT_SRV_CNT tizenrt_ble_srv_count;
extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];
void ble_tizenrt_app_le_profile_init(void)
{
    parse_service_table(server_init_parm.profile, server_init_parm.profile_count);
    server_init(tizenrt_ble_srv_count);
    for (int i = 0; i < tizenrt_ble_srv_count ; i++)
    {
        tizenrt_ble_srv_database[i].srv_id = tizenrt_add_service((void *)ble_tizenrt_app_profile_callback, i);
    }
    server_register_app_cb(ble_tizenrt_app_profile_callback);
    debug_print("\r\n[%s] Init Done", __FUNCTION__);
}

/**
 * @brief    Entry of APP code
 * @return   int (To avoid compile warning)
 */
int ble_tizenrt_app_main(void)
{
    bt_trace_init();
    ble_tizenrt_stack_config_init();
    bte_init();
    le_gap_init(BLE_TIZENRT_PERIPHERAL_APP_MAX_LINKS);
    ble_tizenrt_app_le_gap_init();
    ble_tizenrt_app_le_profile_init();
    ble_tizenrt_app_task_init();
    debug_print("\r\n[%s] Init Done", __FUNCTION__);
    return 0;
}

extern void wifi_btcoex_set_bt_on(void);
int ble_tizenrt_app_init(void)
{
	T_GAP_DEV_STATE new_state;

	/* judge BLE central is already on */
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY) {
        printf("\r\n[%s] BT Stack already on", __FUNCTION__);
		return 0;
	}
	else
		ble_tizenrt_app_main();

	bt_coex_init();

	/*Wait BT init complete*/
	do {
		os_delay(100);
		le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	}while(new_state.gap_init_state != GAP_INIT_STATE_STACK_READY);

	/*Start BT WIFI coexistence*/
	wifi_btcoex_set_bt_on();
    debug_print("\r\n[%s] Init Done", __FUNCTION__);
	return 0;
}

extern void ble_tizenrt_app_task_deinit(void);
extern bool bt_trace_uninit(void);
extern T_GAP_DEV_STATE ble_tizenrt_gap_dev_state;

void ble_tizenrt_app_deinit(void)
{
	ble_tizenrt_app_task_deinit();
 
	T_GAP_DEV_STATE state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &state);
	if (state.gap_init_state != GAP_INIT_STATE_STACK_READY) {
		printf("[BLE Tizenrt]BT Stack is not running\n\r");
	}
#if F_BT_DEINIT
	else {
		bte_deinit();
		bt_trace_uninit();
		memset(&ble_tizenrt_gap_dev_state, 0, sizeof(T_GAP_DEV_STATE));
		printf("[BLE Tizenrt]BT Stack deinitalized\n\r");
	}
#endif
    tizenrt_ble_srv_count = 0;
    memset(tizenrt_ble_srv_database, 0, (7 * sizeof(TIZENERT_SRV_DATABASE)));
}

/** @} */ /* End of group PERIPH_DEMO_MAIN */
