#include "basic_types.h"
#include "platform_autoconf.h"

#ifndef CONFIG_WIFI_P2P_ENABLE
u8 wifi_p2p_check_go(void)
{
	return 0;
}

void wpas_wps_deinit(void)
{
}

int wpas_wps_init(unsigned char wlan_idx)
{
	UNUSED(wlan_idx);
	return 0;
}
#endif

