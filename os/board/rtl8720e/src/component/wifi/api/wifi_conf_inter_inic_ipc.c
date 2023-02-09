
//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>

#include "main.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
//#include <dhcp/dhcps.h>
#endif

#include <platform_stdlib.h>
#include <wifi_conf_inter.h>
#include <wifi_ind.h>
#include <osdep_service.h>
#include <device_lock.h>

#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
#include "at_cmd/atcmd_wifi.h"
#endif
#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_8735B) || defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBADPLUS)
#include "platform_opts_bt.h"
#endif
#ifdef CONFIG_AS_INIC_AP
#include "inic_ipc_api.h"
#endif
/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *               Variables Declarations
 ******************************************************/


/******************************************************
 *               Variables Declarations
 ******************************************************/

/******************************************************
 *               Variables Definitions
 ******************************************************/


/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN

void wifi_btcoex_set_pta(pta_type_t type)
{
	u32 param_buf[1];

	param_buf[0] = (u32)type;
	inic_ipc_api_host_message_send(IPC_API_WIFI_COEX_SET_PTA, param_buf, 1);
}


int wifi_set_wpa_mode(rtw_wpa_mode wpa_mode)
{
	u32 param_buf[1];
	int ret = 0;

	param_buf[0] = (u32)wpa_mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_WPA_MODE, param_buf, 1);
	return ret;
}

int wifi_set_pmf_mode(u8 pmf_mode)
{
	u32 param_buf[1];
	int ret = 0;

	param_buf[0] = (u32)pmf_mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_PMF_MODE, param_buf, 1);
	return ret;
}

int wifi_btcoex_bt_rfk(struct bt_rfk_param *rfk_param)
{
	if (rfk_param == NULL) {
		return _FAIL;
	}

	int ret = 0;
	u32 param_buf[5];
	param_buf[0] = (u32)rfk_param->type;
	param_buf[1] = (u32)rfk_param->rfk_data1;
	param_buf[2] = (u32)rfk_param->rfk_data2;
	param_buf[3] = (u32)rfk_param->rfk_data3;
	param_buf[4] = (u32)rfk_param->rfk_data4;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_COEX_BT_RFK, param_buf, 5);
	return ret;
}

#endif	//#if CONFIG_WLAN
