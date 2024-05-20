/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_scatternet_app_main.c
   * @brief     Source file for BLE central client project, mainly used for initialize modules
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
#include <ble_tizenrt_scatternet_app_task.h>
#include <trace_app.h>
#include <gap_le_types.h>
#include <gap.h>
#include <gap_config.h>
#include <gap_bond_le.h>
#include <gap_scan.h>
#include <profile_client.h>
#include <gap_msg.h>
#include "ble_tizenrt_app.h"
#include <ble_tizenrt_service.h>
//#include "ble_tizenrt_app_flags.h"
#include <ble_tizenrt_scatternet_app.h>
#include <gcs_client.h>
#include <ble_tizenrt_scatternet_link_mgr.h>
#include "trace_uart.h"
#include <bte.h>
#include "wifi_constants.h"
#include <wifi/wifi_conf.h>
#include "rtk_coex.h"
#include <stdio.h>
#include <gap_adv.h>
#include "ble_tizenrt_scatternet_app_flags.h"

extern bool bt_trace_uninit(void);

/** @defgroup  SCATTERNET_DEMO_MAIN Scatternet Main
    * @brief Main file to initialize hardware and BT stack and start task scheduling
    * @{
    */
void print_no_combo(const char* format, ...) {}
/*============================================================================*
 *                              Constants
 *============================================================================*/
/** @brief Default scan interval (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_INTERVAL     0x520
/** @brief Default scan window (units of 0.625ms, 0x520=820ms) */
#define DEFAULT_SCAN_WINDOW       0x520

/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
 * @brief  Config bt stack related feature
 *
 * NOTE: This function shall be called before @ref bte_init is invoked.
 * @return void
 */
extern trble_client_init_config *client_init_parm;
extern trble_server_init_config server_init_parm;
extern void gap_config_hci_task_secure_context(uint32_t size);
extern void gap_config_deinit_flow(uint8_t deinit_flow);
void ble_tizenrt_scatternet_bt_stack_config_init(void)
{
    gap_config_max_le_link_num(BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS);
    gap_config_max_le_paired_device(BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS);
    if(client_init_parm && client_init_parm->mtu)
        gap_config_max_mtu_size(client_init_parm->mtu);
    else
        debug_print("init mtu is 0, use the default value 247 \n");
    gap_config_hci_task_secure_context(280);
    gap_config_deinit_flow(1);
}

/**
  * @brief  Initialize central and gap bond manager related parameters
  * @return void
  */
void ble_tizenrt_scatternet_app_le_gap_init(void)
{
    /* Device name and device appearance */
    uint8_t  device_name[GAP_DEVICE_NAME_LEN] = "BLE_TIZENRT_SCATTERNET_CLIENT";
    uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
    uint8_t  slave_init_mtu_req = false;

    /* Advertising parameters */
    uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
    uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;

    /* Scan parameters */
    uint8_t  scan_mode = GAP_SCAN_MODE_ACTIVE;
    uint16_t scan_interval = DEFAULT_SCAN_INTERVAL;
    uint16_t scan_window = DEFAULT_SCAN_WINDOW;
    uint8_t  scan_filter_policy = GAP_SCAN_FILTER_ANY;
    uint8_t  scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;

    /* GAP Bond Manager parameters */
    //uint8_t  auth_pair_mode = (server_init_parm.is_secured_connect_allowed) ? GAP_PAIRING_MODE_PAIRABLE : GAP_PAIRING_MODE_NO_PAIRING;
    uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;    //TODO:pair mode is device parameters, client and server share the same parameters
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

    /* Set maximum mtu size, the maximum is 512 */
    gap_config_max_mtu_size(512);

    /* Set advertising parameters */
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);

    /* Set scan parameters */
    le_scan_set_param(GAP_PARAM_SCAN_MODE, sizeof(scan_mode), &scan_mode);
    le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(scan_interval), &scan_interval);
    le_scan_set_param(GAP_PARAM_SCAN_WINDOW, sizeof(scan_window), &scan_window);
    le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(scan_filter_policy),
                      &scan_filter_policy);
    le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(scan_filter_duplicate),
                      &scan_filter_duplicate);

    /* Setup the GAP Bond Manager */
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
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

    /* register gap message callback */
    le_register_app_cb(ble_tizenrt_scatternet_app_gap_callback);
#if F_BT_LE_USE_STATIC_RANDOM_ADDR
	T_APP_STATIC_RANDOM_ADDR random_addr;
	bool gen_addr = true;
	uint8_t local_bd_type = GAP_LOCAL_ADDR_LE_RANDOM;
	if (ble_central_app_load_static_random_address(&random_addr) == 0)
	{
		if (random_addr.is_exist == true)
		{
			gen_addr = false;
		}
	}
	if (gen_addr)
	{
		if (le_gen_rand_addr(GAP_RAND_ADDR_STATIC, random_addr.bd_addr) == GAP_CAUSE_SUCCESS)
		{
			random_addr.is_exist = true;
			ble_central_app_save_static_random_address(&random_addr);
		}
	}
	le_cfg_local_identity_address(random_addr.bd_addr, GAP_IDENT_ADDR_RAND);
	le_set_gap_param(GAP_PARAM_RANDOM_ADDR, 6, random_addr.bd_addr);
	le_scan_set_param(GAP_PARAM_SCAN_LOCAL_ADDR_TYPE, sizeof(local_bd_type), &local_bd_type);
#endif
#if F_BT_LE_5_0_SET_PHY_SUPPORT
	uint8_t  phys_prefer = GAP_PHYS_PREFER_ALL;
	uint8_t  tx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
							  GAP_PHYS_PREFER_CODED_BIT;
	uint8_t  rx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT |
							  GAP_PHYS_PREFER_CODED_BIT;
	le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(phys_prefer), &phys_prefer);
	le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys_prefer), &tx_phys_prefer);
	le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys_prefer), &rx_phys_prefer);
#endif
	vendor_cmd_init(app_vendor_callback);
}

/**
 * @brief  Add GATT clients and register callbacks
 * @return void
 */
extern TIZENERT_SRV_CNT tizenrt_ble_srv_count;
extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];
void ble_tizenrt_scatternet_app_le_profile_init(void)
{
    client_init(1);
    ble_tizenrt_scatternet_gcs_client_id = gcs_add_client(ble_tizenrt_scatternet_gcs_client_callback, 
                        BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS, BLE_TIZENRT_SCATTERNET_APP_MAX_DISCOV_TABLE_NUM);

    parse_service_table(server_init_parm.profile, server_init_parm.profile_count);
    server_init(tizenrt_ble_srv_count);
    for (int i = 0; i < tizenrt_ble_srv_count ; i++)
    {
        tizenrt_ble_srv_database[i].srv_id = tizenrt_add_service((void *)ble_tizenrt_scatternet_app_profile_callback, i);
    }
    server_register_app_cb(ble_tizenrt_scatternet_app_profile_callback);

    debug_print("init success \n");
}


/**
 * @brief    Contains the initialization of all tasks
 * @note     There is only one task in BLE Central Client APP, thus only one APP task is init here
 * @return   void
 */
void ble_tizenrt_scatternet_task_init(void)
{
    ble_tizenrt_scatternet_app_task_init();
    debug_print("init success \n");
}

/**
 * @brief    Entry of APP code
 * @return   int (To avoid compile warning)
 */
int ble_tizenrt_scatternet_app_main(void)
{
    bt_trace_init();
    ble_tizenrt_scatternet_bt_stack_config_init();
    bte_init();
    le_gap_init(BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS);
    ble_tizenrt_scatternet_app_le_gap_init();
    ble_tizenrt_scatternet_app_le_profile_init();
    ble_tizenrt_scatternet_task_init();
    debug_print("init success \n");
    return 0;
}

extern void wifi_btcoex_set_bt_on(void);
int ble_tizenrt_scatternet_app_init(void)
{
	T_GAP_DEV_STATE new_state;

	/* Wait WIFI init complete */
	while(!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE))) {
		os_delay(1000);
	}

	/* judge BLE central is already on */
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY) {
		dbg("BT Stack already on \n");
		return 0;
	}
	else
		ble_tizenrt_scatternet_app_main();
	
	bt_coex_init();

	/* Wait BT init complete */
	do {
		os_delay(100);
		le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	}while(new_state.gap_init_state != GAP_INIT_STATE_STACK_READY);

	/* Start BT WIFI coexistence */
	wifi_btcoex_set_bt_on();
	debug_print("init success \n");
	return 0;

}

extern void gcs_delete_client(void);
void ble_tizenrt_scatternet_app_deinit(void)
{
	ble_tizenrt_scatternet_app_task_deinit();
	T_GAP_DEV_STATE state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &state);
	if (state.gap_init_state != GAP_INIT_STATE_STACK_READY) {
		dbg("BT Stack is not running \n");
	}
#if F_BT_DEINIT
	else {
		gcs_delete_client();
		bte_deinit();
		bt_trace_uninit();
		dbg("BT Stack deinitialized \n");
	}
#endif
    tizenrt_ble_srv_count = 0;
    memset(tizenrt_ble_srv_database, 0, (7 * sizeof(TIZENERT_SRV_DATABASE)));
}
/** @} */ /* End of group SCATTERNET_DEMO_MAIN */
