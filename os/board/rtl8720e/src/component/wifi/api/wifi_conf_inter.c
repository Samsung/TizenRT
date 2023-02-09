//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include <dhcp/dhcps.h>
#endif
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

#include "freertos/wrapper.h"

extern u8 rtw_pmf_mode;


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
 *               Variables Definitions
 ******************************************************/


/******************************************************
 *               Function Definitions
 ******************************************************/
void wifi_btcoex_set_pta(pta_type_t type)
{
	rltk_bt_set_gnt_bt_with_clk_source(type);
}

int wifi_set_wpa_mode(rtw_wpa_mode wpa_mode)
{
	if (wpa_mode > WPA2_WPA3_MIXED_MODE) {
		return -1;
	} else {
		wifi_user_config.wifi_wpa_mode = wpa_mode;
		return 0;
	}
}

int wifi_set_pmf_mode(u8 pmf_mode)
{
	struct net_device *dev;

	dev = rtw_get_netdev(STA_WLAN_INDEX);
	ASSERT(dev != NULL);

	if ((pmf_mode != 0) && (pmf_mode != 1) && (pmf_mode != 2)) {
		printf("PMF mode not supported!\r\n");
		return -1;
	}

	rtw_pmf_mode = pmf_mode;
	return 0;
}

int wifi_btcoex_bt_rfk(struct bt_rfk_param *rfk_param)
{
	if (rfk_param == NULL) {
		return _FAIL;
	}
	int res = 0;
	res = rltk_coex_bt_rfk(rfk_param);
	return res;
}