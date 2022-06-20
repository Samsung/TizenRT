#include "ameba_soc.h"

#include "inic_ipc.h"
#include "inic_ipc_api.h"

#ifdef CONFIG_AS_AP
#include "wifi_conf.h"
//#include "wifi_fast_connect.h"
//#if CONFIG_LWIP_LAYER
//#include "lwip_netconf.h"
//#endif

#if defined(CONFIG_WIFI_EN) && CONFIG_WIFI_EN
extern VOID wlan_network(VOID);
extern u32 GlobalDebugEnable;
#endif

struct task_struct wlan_init_task;

/*void app_mbedtls_init(void)
{
	rom_ssl_ram_map.use_hw_crypto_func = 1; //amebad2 todo
}*/

void _init_thread(void *param)
{
	/* To avoid gcc warnings */
	(void) param;

#if 0//CONFIG_LWIP_LAYER
	/* Initilaize the LwIP stack */
	LwIP_Init();
#endif

#if CONFIG_WLAN
	/* wait for inic_ipc_device ready, after that send WIFI_ON ipc msg to device */
	while (!HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_WIFI_CFG)) {
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_WIFI_CFG, 0);
	wifi_on(RTW_MODE_STA);

#endif

	/* Kill init thread after all init tasks done */
	rtw_delete_task(&wlan_init_task);
}

void _wlan_network(void)
{
	if (rtw_create_task(&wlan_init_task, ((const char *)"init"), (512 + 768), 2, _init_thread, NULL) != 1) {
		printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);
	}
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

#elif defined(CONFIG_AS_NP)

void wlan_initialize(void)
{
#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
#if defined(CONFIG_NETWORK) && defined(CONFIG_WIFI_EN) //inic_wlan
	inic_ipc_init();
	inic_ipc_api_init_dev();

	/* set 0x4100C084 =1 to indicate inic_ipc_device is ready */
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_WIFI_CFG, 1);
#endif
#endif
}

#endif	/*CONFIG_AS_AP*/
