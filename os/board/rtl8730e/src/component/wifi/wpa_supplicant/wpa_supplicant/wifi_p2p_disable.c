#include "basic_types.h"
#include "platform_autoconf.h"

#ifndef CONFIG_WIFI_P2P_ENABLE
u8 wifi_p2p_check_role(u8 role)
{
	UNUSED(role);
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

void wpas_wsc_ap_send_eap_reqidentity(void *priv, u8 *rx_buf)
{
	(void) priv;
	(void) rx_buf;
}

void wpas_wsc_ap_check_eap_rspidentity(void *priv, u8 *rx_buf)
{
	(void) priv;
	(void) rx_buf;
}

void wpas_wsc_registrar_send_eap_fail(void *priv)
{
	(void) priv;
}

void wpas_wsc_registrar_handle_recvd(void *priv, u8 *rx_buf)
{
	(void) priv;
	(void) rx_buf;
}

#endif

