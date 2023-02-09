//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#endif
#include <platform_stdlib.h>
#include <wifi_conf.h>
#include <wifi_ind.h>
#include <osdep_service.h>
#include <device_lock.h>


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
#ifdef CONFIG_PROMISC
extern void promisc_init_packet_filter(void);
extern int promisc_add_packet_filter(u8 filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule);
extern int promisc_enable_packet_filter(u8 filter_id);
extern int promisc_disable_packet_filter(u8 filter_id);
extern int promisc_remove_packet_filter(u8 filter_id);
extern int promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used);
extern int promisc_filter_retransmit_pkt(u8 enable, u8 filter_interval_ms);
extern void promisc_filter_by_ap_and_phone_mac(u8 enable, void *ap_mac, void *phone_mac);
extern int promisc_ctrl_packet_rpt(u8 enable);
#endif

#if CONFIG_WLAN

int wifi_set_promisc(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used)
{
	return promisc_set(enabled, callback, len_used);
}


void wifi_enter_promisc_mode()
{
#ifdef CONFIG_PROMISC
	rtw_wifi_setting_t setting = {0};

	/* stop softap fisrt*/
	if (wifi_is_running(SOFTAP_WLAN_INDEX)) {
		wifi_stop_ap();
	}

	/* disconnect to ap if already connected*/
	wifi_get_setting(STA_WLAN_INDEX, &setting);
	if (strlen((const char *)setting.ssid) > 0) {
		wifi_disconnect();
	}
#endif
}

#ifdef CONFIG_PROMISC
void wifi_init_packet_filter()
{
	promisc_init_packet_filter();
}

int wifi_add_packet_filter(unsigned char filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule)
{
	return promisc_add_packet_filter(filter_id, patt, rule);
}

int wifi_enable_packet_filter(unsigned char filter_id)
{
	return promisc_enable_packet_filter(filter_id);
}

int wifi_disable_packet_filter(unsigned char filter_id)
{
	return promisc_disable_packet_filter(filter_id);
}

int wifi_remove_packet_filter(unsigned char filter_id)
{
	return promisc_remove_packet_filter(filter_id);
}

int wifi_retransmit_packet_filter(u8 enable, u8 filter_interval_ms)
{
	return promisc_filter_retransmit_pkt(enable, filter_interval_ms);
}

void wifi_filter_by_ap_and_phone_mac(u8 enable, void *ap_mac, void *phone_mac)
{
	promisc_filter_by_ap_and_phone_mac(enable, ap_mac, phone_mac);
}

int wifi_promisc_ctrl_packet_rpt(u8 enable)
{
	return promisc_ctrl_packet_rpt(enable);
}
#endif

#endif	//#if CONFIG_WLAN
