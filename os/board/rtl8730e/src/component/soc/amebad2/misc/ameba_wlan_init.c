
#include "ameba_soc.h"
#ifdef CONFIG_WLAN

#include "inic_ipc.h"
#include "inic_ipc_api.h"
#include "wifi_conf.h"
#if CONFIG_LWIP_LAYER
#include "lwip_netconf.h"
#endif

#if defined(CONFIG_AS_INIC_AP)
// #include "wifi_fast_connect.h"

struct task_struct wlan_init_task;

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
	while ((HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_BACKUP3) & BIT(26)) == 0) {
		rtw_mdelay_os(1);
	}
	u32 value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_BACKUP3);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_BACKUP3, value & ~BIT(26));
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

void wlan_initialize(void)
{
	inic_ipc_init_host();
	inic_ipc_api_init_host();

#ifndef CONFIG_MP_INCLUDED
#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
	// wifi_fast_connect_enable(0);
#else
	// wifi_fast_connect_enable(1);
#endif
#endif
	_wlan_network();
}

#elif defined(CONFIG_AS_INIC_NP)
void wlan_initialize(void)
{
	inic_ipc_init();
	inic_ipc_api_init_dev();

	/* set 0x42008268[26]=1 to indicate inic_ipc_device is ready */
	u32 value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_BACKUP3);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_BACKUP3, value | BIT(26));
}

#elif defined(CONFIG_SINGLE_CORE_WIFI)
#include "wifi_fast_connect.h"
void _init_thread(void *param)
{
	/* To avoid gcc warnings */
	(void) param;
#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
	rtw_create_secure_context(configMINIMAL_SECURE_STACK_SIZE);
#endif

#if CONFIG_LWIP_LAYER
	/* Initilaize the LwIP stack */
	LwIP_Init();
#endif

	wifi_on(RTW_MODE_STA);
#if CONFIG_AUTO_RECONNECT
	//setup reconnection flag
	wifi_config_autoreconnect(1);
#endif

	printf("\n\r%s(%d), Available heap 0x%x\n", __FUNCTION__, __LINE__, xPortGetFreeHeapSize());

	/* Kill init thread after all init tasks done */
	vTaskDelete(NULL);
}

void _wlan_network(void)
{
	if (xTaskCreate(_init_thread, ((const char *)"init"), (512 + 768), NULL, tskIDLE_PRIORITY + 5, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);
	}
}

void wlan_initialize(void)
{
#ifndef CONFIG_MP_INCLUDED
#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
	wifi_fast_connect_enable(0);
#else
	wifi_fast_connect_enable(1);
#endif
#endif
	_wlan_network();
}

#endif
#endif
