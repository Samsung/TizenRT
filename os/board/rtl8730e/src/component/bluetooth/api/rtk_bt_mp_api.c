/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <hci_if_rtk.h>
#include <platform_opts_bt.h>
#include <rtk_bt_vendor.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_profile_config.h>
#endif

/* ---------------------------------- define -------------------------------*/

/* ------------------------------ Global Variables -------------------------*/
uint8_t need_bt_power_on = 1;		// For download BT MP patch only once

/* -------------------------------- Functions ------------------------------*/
extern void bt_power_off(void);
extern bool bte_init(void);
extern void bte_deinit(void);

/**
 * @brief     Init BT power on for MP test.
 * @param     None
 * @return    None
 */
void rtk_bt_mp_power_on(void)
{
	if (need_bt_power_on) {
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
		int *ret = NULL;

		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_MP_API, RTK_BT_MP_POWER_ON, 
											NULL, 0);
		osif_mem_free(ret);
#else
		bte_init();
		hci_if_wait_patch_download();
		printf("After download patch, deinit HCI driver & HCI uart!\r\n");
		bte_deinit();
#endif
		need_bt_power_on = 0;
	} else {
		printf("No need to download patch again!\r\n");
	}
}

/**
 * @brief     Init BT power off for MP test.
 * @param     None
 * @return    None
 */
void rtk_bt_mp_power_off(void)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_MP_API, RTK_BT_MP_POWER_OFF, 
										NULL, 0);
	osif_mem_free(ret);
#else
	bt_power_off();
#endif
	need_bt_power_on = 1;
}

/**
 * @brief     set bt rf path.
 * @param[in] ant_switch:patch switch (0 for S0, 1 for S1)
 * @return    None
 * This api is only useable for AmebaSmart Platform
 */
void rtk_bt_set_ant(uint8_t ant_switch)
{
	rtk_bt_set_bt_antenna(ant_switch);
}