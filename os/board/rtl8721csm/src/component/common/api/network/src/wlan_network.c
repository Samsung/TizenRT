/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved. 
  *
******************************************************************************/
#if 0
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" 

#include "main.h"
#include "main_test.h"
#if CONFIG_WLAN
#include "wifi_conf.h"
#include "wlan_intf.h"
#include "wifi_constants.h"
#endif
#include "lwip_netconf.h"
#include <platform/platform_stdlib.h>
#include "osdep_service.h"

#ifndef CONFIG_INIT_NET
#define CONFIG_INIT_NET             1
#endif
#ifndef CONFIG_INTERACTIVE_MODE
#define CONFIG_INTERACTIVE_MODE     1
#endif

#define STACKSIZE                   (512 + 768)

xSemaphoreHandle uart_rx_interrupt_sema = NULL;

void init_thread(void *param)
{
#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
	rtw_create_secure_context(configMINIMAL_SECURE_STACK_SIZE);
#endif
	/* To avoid gcc warnings */
	( void ) param;
#if CONFIG_INIT_NET
#if CONFIG_LWIP_LAYER
	/* Initilaize the LwIP stack */
	LwIP_Init();
#endif
#endif
#if CONFIG_WIFI_IND_USE_THREAD
	wifi_manager_init();
#endif
#if CONFIG_WLAN
	wifi_on(RTW_MODE_STA);
#if CONFIG_AUTO_RECONNECT
	//setup reconnection flag
	wifi_set_autoreconnect(1);
#endif
	printf("\n\r%s(%d), Available heap 0x%x", __FUNCTION__, __LINE__, xPortGetFreeHeapSize());	
#endif

#if CONFIG_INTERACTIVE_MODE
 	/* Initial uart rx swmaphore*/
	vSemaphoreCreateBinary(uart_rx_interrupt_sema);
	xSemaphoreTake(uart_rx_interrupt_sema, 1/portTICK_RATE_MS);
	start_interactive_mode();
#endif	

	/* Kill init thread after all init tasks done */
	vTaskDelete(NULL);
}

void wlan_network()
{
	if(xTaskCreate(init_thread, ((const char*)"init"), STACKSIZE, NULL, tskIDLE_PRIORITY + 3 + PRIORITIE_OFFSET, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);
}
#else
#include "wifi_conf.h"
#include "wlan_intf.h"
#include "wifi_constants.h"
#include <platform/platform_stdlib.h>
#include "osdep_service.h"
struct task_struct task;

static void print_scan_result( rtw_scan_result_t* record )
{
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
    at_printf("%s,%d,%s,%d,"MAC_FMT"", record->SSID.val, record->channel,
    		( record->security == RTW_SECURITY_OPEN ) ? "Open" :
            ( record->security == RTW_SECURITY_WEP_PSK ) ? "WEP" :
            ( record->security == RTW_SECURITY_WPA_TKIP_PSK ) ? "WPA TKIP" :
            ( record->security == RTW_SECURITY_WPA_AES_PSK ) ? "WPA AES" :
            ( record->security == RTW_SECURITY_WPA2_AES_PSK ) ? "WPA2 AES" :
            ( record->security == RTW_SECURITY_WPA2_TKIP_PSK ) ? "WPA2 TKIP" :
            ( record->security == RTW_SECURITY_WPA2_MIXED_PSK ) ? "WPA2 Mixed" :
            ( record->security == RTW_SECURITY_WPA_WPA2_MIXED ) ? "WPA/WPA2 AES" : "Unknown",
            record->signal_strength, MAC_ARG(record->BSSID.octet)   );
#else
    RTW_API_INFO("%s\t ", ( record->bss_type == RTW_BSS_TYPE_ADHOC ) ? "Adhoc" : "Infra");
    RTW_API_INFO(MAC_FMT, MAC_ARG(record->BSSID.octet));
    RTW_API_INFO(" %d\t ", record->signal_strength);
    RTW_API_INFO(" %d\t  ", record->channel);
    RTW_API_INFO(" %d\t  ", record->wps_type);
    RTW_API_INFO("%s\t\t ", ( record->security == RTW_SECURITY_OPEN ) ? "Open" :
                                 ( record->security == RTW_SECURITY_WEP_PSK ) ? "WEP" :
                                 ( record->security == RTW_SECURITY_WPA_TKIP_PSK ) ? "WPA TKIP" :
                                 ( record->security == RTW_SECURITY_WPA_AES_PSK ) ? "WPA AES" :
                                 ( record->security == RTW_SECURITY_WPA2_AES_PSK ) ? "WPA2 AES" :
                                 ( record->security == RTW_SECURITY_WPA2_TKIP_PSK ) ? "WPA2 TKIP" :
                                 ( record->security == RTW_SECURITY_WPA2_MIXED_PSK ) ? "WPA2 Mixed" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_MIXED ) ? "WPA/WPA2 AES" :
#ifdef CONFIG_SAE_SUPPORT
								 ( record->security == RTW_SECURITY_WPA3_AES_PSK) ? "WP3-SAE AES" :
#endif
                                 "Unknown");

    RTW_API_INFO(" %s ", record->SSID.val);
    RTW_API_INFO("\r\n");
#endif
}

static rtw_result_t app_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
	static int ApNum = 0;

	if (malloced_scan_result->scan_complete != RTW_TRUE) {
		rtw_scan_result_t* record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
		at_printf("\r\nAP : %d,", ++ApNum);
#else
		RTW_API_INFO("%d\t ", ++ApNum);
#endif
		print_scan_result(record);
#if defined(CONFIG_INIC_CMD_RSP) && CONFIG_INIC_CMD_RSP
		if(malloced_scan_result->user_data)
			memcpy((void *)((char *)malloced_scan_result->user_data+(ApNum-1)*sizeof(rtw_scan_result_t)), (char *)record, sizeof(rtw_scan_result_t));
#endif
	} else{
#if defined(CONFIG_INIC_CMD_RSP) && CONFIG_INIC_CMD_RSP
		inic_c2h_msg("ATWS", RTW_SUCCESS, (char *)malloced_scan_result->user_data, ApNum*sizeof(rtw_scan_result_t));
		if(malloced_scan_result->user_data)
			rtw_free(malloced_scan_result->user_data);
		inic_c2h_msg("ATWS", RTW_SUCCESS, NULL, 0);
#endif
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
		at_printf("\r\n[ATWS] OK");
		at_printf(STR_END_OF_ATCMD_RET);
#endif
		ApNum = 0;
	}
	return RTW_SUCCESS;
}

_timer hello_timer;
void _helloworld_timer_handlder (void *FunctionContext)
{
	_timer *adapter = (_timer *)FunctionContext;

	printf("hello world\r\n");
	
	rtw_set_timer(adapter, 2000);
}

static void init_thread(void *param)
{
	int ret;

	wifi_on(RTW_MODE_STA);
	rtw_init_timer(&(hello_timer), &(hello_timer), _helloworld_timer_handlder, &(hello_timer), "dynamic_chk_timer");
	rtw_set_timer(&(hello_timer), 2000);
	if((ret = wifi_scan_networks(app_scan_result_handler, NULL )) != RTW_SUCCESS){
		printf("wifi scan failed\n\r");
	}

	/* Kill init thread after all init tasks done */
	rtw_thread_exit();
}

void wlan_network(void)
{
	int ret = rtw_create_task(&task, "wlan_init", 1600, 10, init_thread, NULL);
	if (ret != _SUCCESS) {
		DiagPrintf("wlan init thread fail");
	}
}
#endif
