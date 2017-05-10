#ifndef _WIFI_API_H_
#define _WIFI_API_H_
#include "apps/netutils/wifi/slsi_wifi_api.h"

enum wifi_state {
    WIFI_CONNECTED,
    WIFI_DISCONNECTED,
};

int get_wifiState(void);
int wifi_start_station(void);
int wifi_start_ap(uint8_t *ssid, char *security, char *password, int ch);
int wifi_join(uint8_t *ssid, char *security, char *passwd);
int wifi_leave(void);
#ifdef CONFIG_SLSI_WIFI_SANITY
int dhcpc_start(void);
int dhcpserver_start(void);
int stop_dhcp(int interface);
#endif
int wifi_securitymode_check(char *sec_type);
#endif
