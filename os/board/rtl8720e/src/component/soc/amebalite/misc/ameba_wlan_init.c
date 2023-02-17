#include "ameba_soc.h"
#ifdef CONFIG_WLAN
#include "inic_ipc.h"
#include "inic_ipc_api.h"
#endif

#if defined(CONFIG_AS_INIC_AP)
#include "wifi_conf.h"
//#include "wifi_fast_connect.h"

struct task_struct wlan_init_task;

/*void app_mbedtls_init(void)
{
	rom_ssl_ram_map.use_hw_crypto_func = 1; //amebad2 todo
}
*/

void _init_thread(void *param)
{
	/* To avoid gcc warnings */
	(void) param;

#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
#if CONFIG_LWIP_LAYER
	/* Initilaize the LwIP stack */
	LwIP_Init();
#endif
#endif

	/* wait for inic_ipc_device ready, after that send WIFI_ON ipc msg to device */
	while (!HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_WIFI_CFG)) {
		//todo add 1msec delay
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_WIFI_CFG, 0);
	wifi_on(RTW_MODE_STA);

	/* Kill init thread after all init tasks done */
	rtw_delete_task(&wlan_init_task);
}

void _wlan_network(void)
{
	if (rtw_create_task(&wlan_init_task, ((const char *)"init"), (512 + 768), 2, _init_thread, NULL) != 1) {
		printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);
	}
}

static void _wlan_network_deinit_task(void)
{
	rtw_delete_task(&wlan_init_task);
}

void wlan_initialize(void)
{
	// app_mbedtls_init();

	inic_ipc_init_host();
	inic_ipc_api_init_host();

	//wifi_fast_connect_enable(1);
	_wlan_network();

	/* set 0x4100C084 =1 to indicate inic_ipc_device is ready */
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_WIFI_CFG, 1);
}

void wlan_deinitialize(void)
{
	inic_ipc_host_deinit_priv();
	inic_ipc_msgQ_wlan_task_deinit();
	inic_ipc_api_deinit_host();
	_wlan_network_deinit_task();
}
#elif defined(CONFIG_AS_INIC_NP)

void wlan_initialize(void)
{
	inic_ipc_init();
	inic_ipc_api_init_dev();

	/* set 0x4100C084 =1 to indicate inic_ipc_device is ready */
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_WIFI_CFG, 1);
}
#endif
