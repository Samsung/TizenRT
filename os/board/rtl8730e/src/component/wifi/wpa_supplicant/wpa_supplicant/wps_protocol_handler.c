#include "rom_rtw_core.h"
#include "wps_protocol_handler.h"
#include "wifi_api.h"
#include "wifi_intf_drv_to_app_internal.h"
#include "rtw_timer.h"

#ifdef CONFIG_WPS

#if 0
#define DEFAULT_DEVICE_NAME		"REALTEKDEV"
#define DEFAULT_MANUFACTURER	"by realtek"
#define DEFAULT_MODEL_NAME		"realtek"
#define DEFAULT_MODEL_NUMBER	"v1.0"
#define DEFAULT_SERIAL_NUMBER	"8"
#else
char g_device_name[33] = "REALTEKDEV";
char g_manufacturer[65] = "by realtek";
char g_model_name[33] = "realtek";
char g_model_number[33] = "v1.0";
char g_serial_number[33] = "8";
u8 g_vendor_ext_m1_ie[64];
u8 g_vendor_ext_m1_len = 0;
#endif
u8 g_ap_device_type[WPS_DEV_TYPE_LEN] = {0x00, 0x06, 0x00, 0x50, 0xf2, 0x04, 0x00, 0x01}; //ap
static const u8 g_sta_device_type[8] = {0x00, 0xFF, 0x00, 0x50, 0xF2, 0x04, 0x00, 0x00}; //sta
static const u8 g_os_version[4] = {0x1, 0x2, 0x3, 0x4};

static unsigned short g_config_methods = WPS_CONFIG_LABEL | WPS_CONFIG_PUSHBUTTON;
#ifndef CONFIG_NO_STDOUT_DEBUG
static unsigned char wlan_idx = SOFTAP_WLAN_INDEX;
#endif
struct rtw_wps_context g_wps_context = {0};

extern struct eap_method wsc_eap;
extern void _wifi_p2p_wps_success(const u8 *peer_addr, int registrar);
extern void _wifi_p2p_wps_failed(void);
extern void wifi_p2p_wps_success(const u8 *peer_addr, int registrar);
extern void wifi_p2p_wps_failed(void);
extern void wpas_wsc_ap_send_eap_reqidentity(void *priv, u8 *rx_buf);
extern void wpas_wsc_ap_check_eap_rspidentity(void *priv, u8 *rx_buf);
extern void wpas_wsc_registrar_send_eap_fail(void *priv);
extern void wpas_wsc_registrar_handle_recvd(void *priv, u8 *rx_buf);
extern void *eap_wsc_server_process_hdl(void *priv, void *req, u8 id);
extern void eap_wsc_server_reset(void *priv);

static u16  rtw_encode_to_wps_auth(u32 security)
{
	u16 authtype = WPS_AUTH_OPEN;

	switch (security) {
	case RTW_SECURITY_OPEN:
		authtype = WPS_AUTH_OPEN;
		break;
	case RTW_SECURITY_WEP_PSK:
		authtype = WPS_AUTH_OPEN;
		break;
	case RTW_SECURITY_WPA_TKIP_PSK:
		authtype = WPS_AUTH_WPAPSK;
		break;
	case RTW_SECURITY_WPA2_AES_PSK:
		authtype = WPS_AUTH_WPA2PSK;
		break;
	}

	return authtype;
}

static u16 rtw_encode_to_wps_encr(u32 security)
{
	u16 encrp_type = 0;
	switch (security) {
	case RTW_SECURITY_OPEN:
		encrp_type = WPS_ENCR_NONE;
		break;
	case RTW_SECURITY_WEP_PSK:
		encrp_type = WPS_ENCR_WEP;
		break;
	case RTW_SECURITY_WPA_TKIP_PSK:
		encrp_type = WPS_ENCR_TKIP;
		break;
	case RTW_SECURITY_WPA2_AES_PSK:
		encrp_type = WPS_ENCR_AES;
		break;
	}

	return encrp_type;
}


static u16 wps_fix_config_methods(u16 config_methods)
{
#ifdef CONFIG_WPS2
	if ((config_methods &
		 (WPS_CONFIG_DISPLAY | WPS_CONFIG_VIRT_DISPLAY |
		  WPS_CONFIG_PHY_DISPLAY)) == WPS_CONFIG_DISPLAY) {
		wpa_printf(MSG_INFO, "WPS: Converting display to "
				   "virtual_display for WPS 2.0 compliance");
		config_methods |= WPS_CONFIG_VIRT_DISPLAY;
	}
	if ((config_methods &
		 (WPS_CONFIG_PUSHBUTTON | WPS_CONFIG_VIRT_PUSHBUTTON |
		  WPS_CONFIG_PHY_PUSHBUTTON)) == WPS_CONFIG_PUSHBUTTON) {
		wpa_printf(MSG_INFO, "WPS: Converting push_button to "
				   "virtual_push_button for WPS 2.0 compliance");
		config_methods |= WPS_CONFIG_VIRT_PUSHBUTTON;
	}
#endif /* CONFIG_WPS2 */

	return config_methods;
}

static void uuid_gen_mac_addr(const u8 *mac_addr, u8 *uuid)
{
	SHA1Context ctx;
	const u8 *addr[2];
	size_t len[2];
	u8 hash[20];
	u8 nsid[16] = {
		0x52, 0x64, 0x80, 0xf8,
		0xc9, 0x9b,
		0x4b, 0xe5,
		0xa6, 0x55,
		0x58, 0xed, 0x5f, 0x5d, 0x60, 0x84
	};

	addr[0] = nsid;
	len[0] = sizeof(nsid);
	addr[1] = mac_addr;
	len[1] = 6;

	// use wlan sha1 rom code to reduce memory usage
	//sha1_vector(2, addr, len, hash);
	rt_sha1_init(&ctx);
	rt_sha1_update(&ctx, addr[0], len[0]);
	rt_sha1_update(&ctx, addr[1], len[1]);
	rt_sha1_finish(&ctx, hash);

	os_memcpy(uuid, hash, 16);
	wpa_printf(MSG_DEBUG, "uuid_gen_mac_addr,mac: "
			   MACSTR, MAC2STR(mac_addr));
	/* Version: 5 = named-based version using SHA-1 */
	uuid[6] = (5 << 4) | (uuid[6] & 0x0f);

	/* Variant specified in RFC 4122 */
	uuid[8] = 0x80 | (uuid[8] & 0x3f);
	wpa_hexdump_key(MSG_DEBUG, "WPS: create UUID", uuid,
					WPS_UUID_LEN);
}

void wpas_get_wifi_mac(u8 *dev_mac)
{
	int i = 0;
	struct rtw_mac mac;

	wifi_get_mac_address(0, &mac, 0);
	for (i = 0; i < ETH_ALEN; i++) {
		dev_mac[i] = mac.octet[i];
	}
	wpa_printf(MSG_INFO, "WPS: Get Device MAC "MAC_FMT, MAC_ARG(dev_mac));
}

static void *wpas_eap_wsc_init(void)
{
	eap_peer_wsc_register();
	return wsc_eap.init(NULL);
}

static int  wpas_set_wps_ie(void *ctx, struct wpabuf *beacon_ie,
							struct wpabuf *probe_resp_ie)
{
	/* To avoid gcc warnings */
	(void) ctx;

	wpa_printf(MSG_DEBUG, "[wpas_set_wps_ie] set wps ie for wlan%d", wlan_idx);

	wifi_set_gen_ie(SOFTAP_WLAN_INDEX, (char *)probe_resp_ie->buf, probe_resp_ie->used, P2PWPS_PROBE_RSP_IE);
	wifi_set_gen_ie(SOFTAP_WLAN_INDEX, (char *)beacon_ie->buf, beacon_ie->used, P2PWPS_BEACON_IE);

	wpabuf_free(beacon_ie);
	wpabuf_free(probe_resp_ie);
	wpa_printf(MSG_DEBUG, "end [wpas_set_wps_ie] set wps ie for wlan%d", wlan_idx);
	return 0;
}

static void wpas_notify_wps_sucess(void *ctx, const u8 *mac_addr,
								   const u8 *uuid_e, const u8 *dev_pw,
								   size_t dev_pw_len)
{
	/* To avoid gcc warnings */
	(void) ctx;
	(void) uuid_e;
	(void) dev_pw;
	(void) dev_pw_len;

	wpa_printf(MSG_DEBUG, "wpas_notify_wps_sucess");
	g_wps_context.clientWpsDone = 1;
	wifi_p2p_wps_success(mac_addr, 1);
}

__attribute__((noinline)) void wpas_wsc_notify_wps_finish(void)
{
	if (g_wps_context.clientWpsProcessing == 1) {
		if (queue_for_credential != NULL) {
			u8 zero_mac[ETH_ALEN] = {0};
			os_xqueue_send(queue_for_credential, &zero_mac, 0);
		}
		rtos_critical_enter(RTOS_CRITICAL_WIFI);
		wsc_eap.deinit(NULL, g_wps_context.wsc_data);
		g_wps_context.wsc_data = NULL;
		rtos_critical_exit(RTOS_CRITICAL_WIFI);
		if (g_wps_context.clientWpsDone == 1) {
			g_wps_context.clientWpsDone = 0;
		}
		g_wps_context.clientWpsProcessing = 0;
	}
}

void wpas_wsc_send_packet(struct rtw_wps_context *rtw_wps, struct wpabuf *sendData)
{
	struct lib1x_eapol *eapol = NULL;
	struct wlan_ethhdr_t *eth_hdr = NULL;
	struct _LIB1X_EAPOL_WSC	*Eapol_wsc_to_send = NULL;
	struct eap_wsc_data *wsc_data = rtw_wps->wsc_data;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN + sendData->used;
	u8 wlan_idx = rtw_wps->wps->ap ? SOFTAP_WLAN_INDEX : STA_WLAN_INDEX;

	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send failed!\n");
		return;
	}

	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(eth_hdr->daddr, wsc_data->wps->peer_dev.mac_addr, ETH_ALEN);
	os_memcpy(eth_hdr->saddr, rtw_wps->wps->dev.mac_addr, ETH_ALEN);
	eth_hdr->type = _htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((unsigned char *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = LIB1X_EAPOL_VER;
	eapol->packet_type = LIB1X_EAPOL_EAPPKT;
	eapol->packet_body_length = _htons(sendData->used);

	Eapol_wsc_to_send = (struct _LIB1X_EAPOL_WSC *)((unsigned char *)eapol + LIB1X_EAPOL_HDRLEN);
	os_memcpy(Eapol_wsc_to_send, sendData->buf, sendData->used);

	wpa_hexdump_key(MSG_MSGDUMP, "TX-EAPOL", buf, length);
	wifi_if_send_eapol(wlan_idx, (char *)buf, length, 0);

	wpabuf_free(sendData);
	os_free(buf, length);

	return;
}

/*------------------------------------------------------------------------------------------*/
//STA handle, the role of registrar or enrollee will be determined after identify req/rsp.
void wpas_wsc_sta_send_eapol_start(struct rtw_wps_context *rtw_wps, u8 *peer_mac)
{
	/* To avoid gcc warnings */
	(void) rtw_wps;

	u8 dev_mac[ETH_ALEN];
	struct wlan_ethhdr_t *eth_hdr;
	struct lib1x_eapol *eapol;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN; //18

	wpa_printf(MSG_INFO, "WPS: Send EAPOL-Start!");

	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send eapol start failed!\n");
		return;
	}
	wpas_get_wifi_mac(dev_mac);
	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(&eth_hdr->daddr, peer_mac, ETH_ALEN);
	os_memcpy(&eth_hdr->saddr, dev_mac, ETH_ALEN);
	eth_hdr->type = _htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((unsigned char *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = (LIB1X_EAPOL_VER);
	eapol->packet_type = (LIB1X_EAPOL_START);
	eapol->packet_body_length = 0;

	wpa_hexdump_key(MSG_MSGDUMP, "EAPOL-START", buf, length);

	wifi_if_send_eapol(STA_WLAN_INDEX, (char *)buf, length, 0);

	// reset the WPS state machine
	g_wps_context.clientWpsDone = 0;
	g_wps_context.clientWpsProcessing = 0;
	g_wps_context.record_last_message_type = 0;
	g_wps_context.last_message_has_more_frag = 0;

	os_free(buf, length);
	return;
}

static void wpas_wsc_sta_send_eap_rspidentity(struct rtw_wps_context *rtw_wps, u8 *rx_buf)
{
	/* To avoid gcc warnings */
	(void) rtw_wps;

	struct wlan_ethhdr_t *eth_hdr;
	struct lib1x_eapol *eapol;
	struct lib1x_eapol_message_hdr *received_eapol_identity_payload_hdr;
	struct lib1x_eapol_message_hdr *eapol_identity_respose;
	char *identity = "WFA-SimpleConfig-Enrollee-1-0";
	unsigned char *temp = NULL;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN + 34; //52

	wpa_printf(MSG_INFO, "WPS: Send EAP RSP Identity!");

	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send eap fail failed!\n");
		return;
	}

	// get the sending address which we want to put data into(EAPOL packet)
	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(eth_hdr->daddr, (rx_buf + 6), ETH_ALEN);
	os_memcpy(eth_hdr->saddr, rx_buf, ETH_ALEN);
	eth_hdr->type = _htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((u8 *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = LIB1X_EAPOL_VER;
	eapol->packet_type = LIB1X_EAPOL_EAPPKT;
	eapol->packet_body_length = _htons(34);

	// translate received packet to EAP message form in order to fill it
	received_eapol_identity_payload_hdr = (struct lib1x_eapol_message_hdr *)(rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);

	// get the sending address which we want to put data into(EAPOL packet)
	eapol_identity_respose = (struct lib1x_eapol_message_hdr *)((u8 *)eapol + LIB1X_EAPOL_HDRLEN);

	// EAP Code
	eapol_identity_respose->Code = LIB1X_EAP_CODE_RESPONSE;

	// EAP Identifier
	wpa_printf(MSG_DEBUG, "EAP Identifier = 0x%x", received_eapol_identity_payload_hdr->Identifier);
	eapol_identity_respose->Identifier = received_eapol_identity_payload_hdr->Identifier;

	// EAP Length
	eapol_identity_respose->Length = _htons(34);

	// EAP Type
	eapol_identity_respose->Type = LIB1X_EAP_TYPE_IDENTITY;

	temp = ((unsigned char *)eapol_identity_respose + LIB1X_EAPOL_MESSAGE_HDRLEN);
	os_memcpy(temp, identity, 29);

	wpa_hexdump_key(MSG_MSGDUMP, "IDENTIFY-RSP", buf, length);
	wifi_if_send_eapol(STA_WLAN_INDEX, (char *)buf, length, 0);

	os_free(buf, length);
	return;
}

/*------------------------------------------------------------------------------------------*/
//Enrollee handle
static void wpas_wsc_enrollee_handle_eap_fail(struct rtw_wps_context *rtw_wps)
{
	struct rtw_wifi_setting setting = {0};
	u8 *peer_mac = rtw_wps->wsc_data->wps->peer_dev.mac_addr;

	if (rtw_wps->clientWpsProcessing == 0) { //handle unexcepted eap fail
		memset(&rtw_wps->wsc_data->wps->cred, 0, 32);
		rtos_time_delay_ms(1000); //let the upper can get wifi connect info.
	}

	wifi_get_setting(STA_WLAN_INDEX, &setting);
	if (setting.mode == RTW_MODE_STA) {
		wpa_printf(MSG_INFO, "WPS: Receive EAPOL-FAIL packet from "MAC_FMT".", MAC_ARG(peer_mac));
		wifi_p2p_wps_success(peer_mac, 0);
		wpas_wsc_notify_wps_finish();
	} else {
	}


}

struct wpabuf *wpas_wsc_enrollee_process_hdl(struct rtw_wps_context *rtw_wps, struct wpabuf *reqData)
{
	int is_ap = rtw_wps->wps->ap;
	//AP as enrollee
	if (is_ap) {
		return eap_wsc_server_process_hdl((void *)rtw_wps->wsc_data, (void *)reqData, g_wps_context.identifier++);
	} else {
		return wsc_eap.process(NULL, (void *)rtw_wps->wsc_data, NULL, reqData);
	}
}

void wpas_wsc_enrollee_send_eapol(struct rtw_wps_context *rtw_wps,
								  unsigned char ReceiveMessageType, u8 *rx_buf)
{
	u8 mac[ETH_ALEN];
	struct _LIB1X_EAPOL_WSC *eapol_wsc = (struct _LIB1X_EAPOL_WSC *)((u8 *)rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);
	struct wpabuf *sendData = NULL;
	struct wpabuf reqData;
	memset(&reqData, 0, sizeof(struct wpabuf));

	wpas_get_wifi_mac(mac);

	// last fragmented pkt, handle in type switch case
	if (!(eapol_wsc->Flags & WSC_FLAGS_MF) && rtw_wps->last_message_has_more_frag) {
		ReceiveMessageType = rtw_wps->record_last_message_type;
	}

	/* main wsc handler */
	// for fragmented pkt except the last one
	if ((eapol_wsc->Flags & WSC_FLAGS_MF)) {
		wpa_printf(MSG_INFO, "Receive fragmented msg\n");

		reqData.buf = (unsigned char *)eapol_wsc;
		sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
		if (sendData == NULL) {
			wpa_printf(MSG_INFO, "[WPS-Default]SendData is NULL!!!");
			return;
		}
		wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);
		wpas_wsc_send_packet(rtw_wps, sendData);
		rtw_wps->last_message_has_more_frag = 1;
	} else {
		switch (ReceiveMessageType) {
		case WPS_START:
			wpa_printf(MSG_INFO, "Receive WSC_START -> WSC M1\n");
			if (rtw_wps->wps == NULL) {
				wpas_wps_dev_config(mac, 0);
			} else if (rtw_wps->wps->registrar) {
				wps_registrar_deinit((void *)rtw_wps->wps->registrar);
				rtw_wps->wps->registrar = NULL;
			} else {}
			wpas_wps_init(STA_WLAN_INDEX);
			rtw_wps->wsc_data = (struct eap_wsc_data *)wpas_eap_wsc_init();
			rtw_wps->wsc_data->wps->wps = rtw_wps->wps; //wps_context
			os_memcpy(rtw_wps->wsc_data->wps->peer_dev.mac_addr, (rx_buf + 6), ETH_ALEN);

			reqData.buf = (unsigned char *)eapol_wsc;
			sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
			if (sendData == NULL) {
				wpa_printf(MSG_WARNING, "[WPS-M1]SendData is NULL!!!");
				if (rtw_wps->clientWpsProcessing == 1) {
					rtw_wps->clientWpsProcessing = 0;
				}
				return;
			}
			wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);

			wpas_wsc_send_packet(rtw_wps, sendData);
			wpa_printf(MSG_DEBUG, "Sending WSC M1 Complete\n");
			break;
		case WPS_M2:
			wpa_printf(MSG_INFO, "Receive WSC M2 -> WSC M3\n");

			reqData.buf = (unsigned char *)eapol_wsc;
			sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
			if (sendData == NULL) {
				wpa_printf(MSG_INFO, "[WPS-M3]SendData is NULL!!!");
				return;
			}
			wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);
			wpas_wsc_send_packet(rtw_wps, sendData);

			wpa_printf(MSG_DEBUG, "\r\nSending WPS_M3 Complete\n");
			break;


		case WPS_M2D:
			wpa_printf(MSG_INFO, "\r\nReceive WSC M2D -> WSC ACK\n");

			reqData.buf = (unsigned char *)eapol_wsc;

			sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
			if (sendData == NULL) {
				wpa_printf(MSG_WARNING, "[WPS-ACK]SendData is NULL!!!");
				//clientWpsDone = 1;
				//clientWpsProcessing = 0;
				return;
			}
			wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);

			wpas_wsc_send_packet(rtw_wps, sendData);
			wpa_printf(MSG_DEBUG, "Sending WPS_ACK Complete\n");
			rtw_wps->clientWpsDone = 1;
			break;
		case WPS_M4:
			wpa_printf(MSG_INFO, "Receive WSC M4 -> WSC M5\n");

			reqData.buf = (unsigned char *)eapol_wsc;

			sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
			if (sendData == NULL) {
				wpa_printf(MSG_WARNING, "[WPS-M5]SendData is NULL!!!");
				return;
			}
			wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);
			wpas_wsc_send_packet(rtw_wps, sendData);
			wpa_printf(MSG_DEBUG, "Sending WPS_M5 Complete\n");
#if 1
			if (rtw_wps->wsc_data->wps->state == SEND_WSC_NACK) {
				//rtos_time_delay_ms(5);
				//because some ap can't send eap fail when receving eap nack,we force wps end, by lawrence_wang.
				wpa_printf(MSG_DEBUG, "[WPS-M5]WRONG PIN CODE!send wsc nack.set done = 1!");
				//os_xqueue_send(queue_for_credential, &wps_context.wsc_data->wps->cred, 0);
				rtw_wps->clientWpsDone = 1;
				wpas_wsc_enrollee_handle_eap_fail(rtw_wps);
			}
#endif
			break;
		case WPS_M6:
			wpa_printf(MSG_INFO, "Receive WSC M6 -> WSC M7\n");

			reqData.buf = (unsigned char *)eapol_wsc;

			sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
			if (sendData == NULL) {
				wpa_printf(MSG_WARNING, "[WPS-M7]SendData is NULL!!!");
				return;
			}

			wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);
			wpas_wsc_send_packet(rtw_wps, sendData);
			wpa_printf(MSG_DEBUG, "Sending WPS_M7 Complete\n");
			break;
		case WPS_M8:
			wpa_printf(MSG_INFO, "Receive WSC M8 -> WSC Done\n");

			reqData.buf = (unsigned char *)eapol_wsc;

			sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
			if (sendData == NULL) {
				wpa_printf(MSG_WARNING, "[WPS-DONE]SendData is NULL!!!");
				return;
			}
			wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);
			wpas_wsc_send_packet(rtw_wps, sendData);

			wpa_printf(MSG_DEBUG, "Sending WPS Done Complete\n");
			rtw_wps->clientWpsDone = 1;
			break;
		case WPS_WSC_NACK:
			wpa_printf(MSG_INFO, "Receive WSC NACK -> WSC NACK\n");

			reqData.buf = (unsigned char *)eapol_wsc;
			sendData = wpas_wsc_enrollee_process_hdl(rtw_wps, &reqData);
			if (sendData == NULL) {
				if (rtw_wps->wps->registrar && rtw_wps->wsc_data->state == FAIL) {
					wpas_wsc_registrar_send_eap_fail((void *)rtw_wps);
					rtw_wps->clientWpsProcessing = 0;
					rtw_wps->identifier = 0;
					rtw_wps->clientWpsDone = 0;
					//here the WPS has end.
					os_xqueue_send(queue_for_credential, rtw_wps->wsc_data->wps->peer_dev.mac_addr, 0);
				}
				wpa_printf(MSG_WARNING, "[WPS-FAIL]SendData is NULL!!!");
				return;
			}
			wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);
			wpas_wsc_send_packet(rtw_wps, sendData);
			wpa_printf(MSG_DEBUG, "Sending WPS NACK Complete\n");
			rtw_wps->clientWpsDone = 1;

			if (!rtw_wps->wps->registrar) {
				// Because some ap can't send eap fail when receiving eap nack, we force wps end.
				wpa_printf(MSG_INFO, "[WPS-FAIL]Recv NACK, force to end");
				wpas_wsc_enrollee_handle_eap_fail(rtw_wps);
			}
			break;
		default:
			wpa_printf(MSG_INFO, "Receive unknown type packet\n");
		}
		rtw_wps->last_message_has_more_frag = 0;
	}
}

static void wpas_wsc_enrollee_handle_recvd(struct rtw_wps_context *rtw_wps, u8 *rx_buf)
{
	unsigned char  Receive_message_type;
	int msg_type_offset;
	struct _LIB1X_EAPOL_WSC *eapol_wsc = (struct _LIB1X_EAPOL_WSC *)((u8 *)rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);

	if (eapol_wsc->Code != LIB1X_EAP_CODE_REQUEST) {
		wpa_printf(MSG_WARNING, "WPS Enrollee: Just handle eap request!");
		return;
	}

	if (eapol_wsc->Flags & WSC_FLAGS_LF) {
		msg_type_offset = 25;
	} else {
		msg_type_offset = 23;
	}
	Receive_message_type = (*((unsigned char *)eapol_wsc + msg_type_offset));
	switch (eapol_wsc->OpCode) {
	case LIB1X_EAP_OPCODE_WSC_START:
		if (rtw_wps->clientWpsProcessing == 0) {
			rtw_wps->clientWpsProcessing = 1;
			wpas_wsc_enrollee_send_eapol(rtw_wps, WPS_START, rx_buf);
		}
		break;
	case LIB1X_EAP_OPCODE_WSC_MSG:
		if ((rtw_wps->record_last_message_type != Receive_message_type) ||
			((rtw_wps->record_last_message_type == Receive_message_type) && (Receive_message_type == WPS_M2D))) {

			wpas_wsc_enrollee_send_eapol(rtw_wps, Receive_message_type, rx_buf);
			// if is ordinary pkt or first fragmented pkt -> keep last message type
			if ((eapol_wsc->Flags & WSC_FLAGS_LF) || !(rtw_wps->last_message_has_more_frag)) {
				rtw_wps->record_last_message_type = Receive_message_type;
			}
		} else {
			wpa_printf(MSG_DEBUG, "\r\nreceive duplicate WPS MSG = 0x%x\n", Receive_message_type);
		}
		break;
	case LIB1X_EAP_OPCODE_WSC_NACK:
	case LIB1X_EAP_OPCODE_WSC_ACK:
		wpa_printf(MSG_DEBUG, "\r\nReceive_message_type = 0x%x\n", Receive_message_type);
		wpas_wsc_enrollee_send_eapol(rtw_wps, Receive_message_type, rx_buf);
		break;
	default:
		wpa_printf(MSG_DEBUG, "EAP-WSC: Unexpected OpCode %d in "
				   "registrar handle", eapol_wsc->OpCode);

	}
}


/*******************************************************************/
/*	Called by applcation Start*/
/*******************************************************************/
void wpas_wsc_eapol_recvd_hdl(u8 *buf, s32 buf_len)
{
	/* To avoid gcc warnings */
	(void) buf_len;

	// get base address of the received EAPOL packet payload
	struct lib1x_eapol *rx_eapol = (struct lib1x_eapol *)(buf + ETH_HLEN);
	struct _LIB1X_EAPOL_WSC *eapol_wsc = (struct _LIB1X_EAPOL_WSC *)(buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);
	int wireless_mode;
	struct rtw_wifi_setting setting = {0};
	struct rtw_wps_context *rtw_wps = &g_wps_context;
	u8 wlan_idx = rtw_wps->wps->ap ? SOFTAP_WLAN_INDEX : STA_WLAN_INDEX;

	wifi_get_setting(wlan_idx, &setting);
	wireless_mode = setting.mode;
	wpa_printf(MSG_DEBUG, "[WPS]wpas_wsc_eapol_recvd:clientWpsDone = %d,clientWpsProcessing = %d",
			   rtw_wps->clientWpsDone, rtw_wps->clientWpsProcessing);

	wpa_hexdump_key(MSG_MSGDUMP, "RX-EAPOL", buf, buf_len);

	//ap recv eap start
	if (rx_eapol->packet_type == LIB1X_EAPOL_START && wireless_mode == RTW_MODE_AP) {
		wpa_printf(MSG_INFO, "WPS: Received EAPOL_START");
		wpas_wsc_ap_send_eap_reqidentity((void *)rtw_wps, buf);
		return;
	}

	if ((LIB1X_EAP_CODE_REQUEST == eapol_wsc->Code || LIB1X_EAP_CODE_RESPONSE == eapol_wsc->Code)
		&& LIB1X_EAP_TYPE_IDENTITY == eapol_wsc->Type) {
		if (wireless_mode == RTW_MODE_AP) {
			//APcheckEapRspIdentity(padapter, psta, skb);
			wpas_wsc_ap_check_eap_rspidentity((void *)rtw_wps, buf);
		} else {
			rtw_del_timer(&rtw_wps->wps_start_timer);
			wpas_wsc_sta_send_eap_rspidentity(rtw_wps, buf);
		}
	} else if (eapol_wsc->Code == LIB1X_EAP_CODE_FAIL) {
		if (g_wps_context.wsc_data && !g_wps_context.wsc_data->wps->registrar) {
			wpas_wsc_enrollee_handle_eap_fail(rtw_wps);
		}
	} else if (LIB1X_EAP_TYPE_EXPANDED == eapol_wsc->Type) {
		// record base address of the received EAPOL-WSC message(i.e WSC EAP packet)
		wpa_printf(MSG_DEBUG, "eapol_wsc->OpCode = 0x%x,code:%x", eapol_wsc->OpCode, eapol_wsc->Code);
		if (eapol_wsc->OpCode == LIB1X_EAP_OPCODE_WSC_START || rtw_wps->wsc_data->registrar == 0) {
			wpas_wsc_enrollee_handle_recvd(rtw_wps, buf);
		} else {
			wpas_wsc_registrar_handle_recvd((void *)rtw_wps, buf);
		}
	}
}

void wpas_wsc_sta_wps_start_timer(void *peer_mac)
{
	wpas_wsc_sta_send_eapol_start(&g_wps_context, (u8 *) peer_mac);
	rtw_set_timer(&g_wps_context.wps_start_timer, WPS_START_RESEND_TIME);
}

void wpas_wsc_sta_wps_start_hdl(u8 *evt_info)
{
	struct rtw_event_wpa_sta_wps_start *wps_start = (struct rtw_event_wpa_sta_wps_start *)evt_info;
#if 0
	u8 *peer_mac = (u8 *)buf;
	wpas_wsc_sta_send_eapol_start(&g_wps_context, peer_mac);
#else
	static u8 peer_mac[ETH_ALEN] = {0};

	if (evt_info) {
		os_memcpy(peer_mac, wps_start->peer_mac, ETH_ALEN);
		rtw_del_timer(&g_wps_context.wps_start_timer);
		rtw_init_timer(&g_wps_context.wps_start_timer, NULL, wpas_wsc_sta_wps_start_timer, peer_mac, "wps_start_timer");
		rtw_set_timer(&g_wps_context.wps_start_timer, WPS_START_DELAY_TIME);
	}
#endif
}

void wpas_wsc_wps_finish_hdl(u8 *evt_info)
{
	/* To avoid gcc warnings */
	(void) evt_info;

	wpas_wsc_notify_wps_finish();
}

void wpas_wsc_server_wps_finish_hdl(u8 *buf, s32 buf_len)
{
	/* To avoid gcc warnings */
	(void) buf;
	(void) buf_len;

	u8 zero_mac[ETH_ALEN] = {0};
	wpa_printf(MSG_INFO, "WPS: AP WPS finished.");
	if (g_wps_context.clientWpsProcessing == 1) {
		struct wps_data *wps = NULL;

		if (queue_for_credential) {
			os_xqueue_send(queue_for_credential, &zero_mac, 0);
		}

		wps = g_wps_context.wsc_data->wps;
		if (wps) {
			if (wps->state == RECV_DONE) {
				_wifi_p2p_wps_success(wps->peer_dev.mac_addr, 1);
			} else {
				_wifi_p2p_wps_failed();
			}
		}
	}
}

__weak int wpas_wps_init(unsigned char wlan_idx)
{
	struct wps_context *wps;
	struct wps_registrar_config rcfg;
	struct rtw_wifi_setting setting = {0};

	wps = g_wps_context.wps;
	if (wps == NULL) {
		return -1;
	}

	if (wifi_get_setting(wlan_idx, &setting) < 0) {
		return -1;
	}

	wps->auth_types = rtw_encode_to_wps_auth((u32)setting.security_type);//WPS_AUTH_WPA2PSK;
	wps->encr_types = rtw_encode_to_wps_encr((u32)setting.security_type);//WPS_ENCR_AES;
	wpa_printf(MSG_INFO, "wps->auth_types:%x,wps->encr_types:%x", wps->auth_types, wps->encr_types);
	//ssid is used by registrar
	os_memcpy(wps->ssid, setting.ssid, os_strlen((char *)setting.ssid));
	wps->ssid_len = os_strlen((char *)setting.ssid);
	wpa_printf(MSG_DEBUG, "get ssid:%s", wps->ssid);

	if (setting.mode == RTW_MODE_AP) {
		os_memcpy(wps->dev.pri_dev_type, g_ap_device_type, WPS_DEV_TYPE_LEN);
		wps->ap = 1;
	}

	os_memset(&rcfg, 0, sizeof(rcfg));
	rcfg.cb_ctx = NULL;
	rcfg.set_ie_cb = wpas_set_wps_ie;
	rcfg.reg_success_cb = wpas_notify_wps_sucess;
	rcfg.force_per_enrollee_psk = 0;

	wps_registrar_init((void *)wps, (void *)&rcfg);
#if 0
	wps->registrar = wps_registrar_init(wps, &rcfg);
	if (wps->registrar == NULL) {
		wpa_printf(MSG_ERROR, "Failed to initialize WPS Registrar");
		os_free(wps, 0);
		return -1;
	}
#endif
	return 0;
}

__weak void wpas_wps_deinit(void)
{
	struct wps_context *wps = g_wps_context.wps;

	rtw_del_timer(&g_wps_context.wps_start_timer);

	if (wps) {
		wpabuf_free(wps->dev.vendor_ext_m1);
		wps->dev.vendor_ext_m1 = NULL;
		if (wps->registrar) {
			wps_registrar_deinit((void *)wps->registrar);
		} else if (wsc_eap.deinit) {
			// for Enrollee
			rtos_critical_enter(RTOS_CRITICAL_WIFI);
			wsc_eap.deinit(NULL, g_wps_context.wsc_data);
			g_wps_context.wsc_data = NULL;
			rtos_critical_exit(RTOS_CRITICAL_WIFI);
		}
		os_free(wps, 0);
		g_wps_context.wps = NULL;
	} else if (wsc_eap.deinit) {
		// for Enrollee
		rtos_critical_enter(RTOS_CRITICAL_WIFI);
		wsc_eap.deinit(NULL, g_wps_context.wsc_data);
		g_wps_context.wsc_data = NULL;
		rtos_critical_exit(RTOS_CRITICAL_WIFI);
	}
}

int wpas_wps_dev_config(u8 *dev_addr, u8 bregistrar)
{
	struct wps_context *wps;
	u8 band_type;

	if (g_wps_context.wps) {
		return 0;
	}

	wps = (struct wps_context *)os_zalloc(sizeof(*wps));
	if (wps == NULL) {
		return -1;
	}

	wps->wps_state = WPS_STATE_CONFIGURED;
	wps->dev.device_name = g_device_name;
	wps->dev.manufacturer = g_manufacturer;
	wps->dev.model_name = g_model_name;
	wps->dev.model_number = g_model_number;
	wps->dev.serial_number = g_serial_number;
	/*config vendor extension ie in m1 frame*/
	wpabuf_free(wps->dev.vendor_ext_m1);
	wps->dev.vendor_ext_m1 = NULL;

	wps->config_methods = g_config_methods;
	if ((wps->config_methods & (WPS_CONFIG_DISPLAY | WPS_CONFIG_LABEL)) ==
		(WPS_CONFIG_DISPLAY | WPS_CONFIG_LABEL)) {
		wpa_printf(MSG_ERROR, "WPS: Both Label and Display config "
				   "methods are not allowed at the same time");
		os_free(wps, 0);
		return -1;
	}
	wps->config_methods = wps_fix_config_methods(wps->config_methods);
	wps->dev.config_methods = wps->config_methods;
	os_memcpy(wps->dev.pri_dev_type, g_sta_device_type, WPS_DEV_TYPE_LEN);

	wps->dev.num_sec_dev_types = 0;
	os_memset(wps->dev.sec_dev_type, 0, WPS_DEV_TYPE_LEN * WPS_SEC_DEVICE_TYPES);

	//wpas_wps_set_vendor_ext_m1(wpa_s, wps);

	wps->dev.os_version = WPA_GET_BE32(g_os_version);
	wifi_get_band_type(&band_type);
#ifndef SUPPORT_5G_CHANNEL
	wps->dev.rf_bands = WPS_RF_24GHZ;
#else
	if (band_type == RTW_SUPPORT_BAND_2_4G_5G_BOTH) {
		wps->dev.rf_bands = WPS_RF_24GHZ | WPS_RF_50GHZ;
	} else if (band_type == RTW_SUPPORT_BAND_5G) {
		wps->dev.rf_bands = WPS_RF_50GHZ;
	} else {
		wps->dev.rf_bands = WPS_RF_24GHZ;
	}
#endif
	os_memcpy(wps->dev.mac_addr, dev_addr, ETH_ALEN);
	uuid_gen_mac_addr(wps->dev.mac_addr, wps->uuid);
	wpa_hexdump_key(MSG_DEBUG, "WPS: get UUID", wps->uuid,
					WPS_UUID_LEN);

	wps->auth_types = WPS_AUTH_WPA2PSK;
	wps->encr_types = WPS_ENCR_AES;
	if (bregistrar) {
		wps->registrar = (struct wps_registrar *)wps_registrar_alloc();
		if (NULL == wps->registrar) {
			os_free(wps, 0);
			return -1;
		}
	}
	g_wps_context.wps = wps;

	return 0;
}

int wpas_wps_registrar_button_pushed(void)
{
	struct wps_registrar *reg = g_wps_context.wps->registrar;

	g_wps_context.clientWpsDone = 0;
	return wps_registrar_button_pushed((void *)reg, NULL);
}

int wpas_wps_registrar_add_pin(const u8 *pin, size_t pin_len)
{
	struct wps_registrar *reg = g_wps_context.wps->registrar;

	g_wps_context.clientWpsDone = 0;
	return wps_registrar_add_pin((void *)reg, NULL, NULL, pin, pin_len, 0);
}

int wpas_wps_registrar_check_done(void)
{
	return g_wps_context.clientWpsDone;
}

int wpas_wps_registrar_wps_cancel()
{
	struct wps_registrar *reg = g_wps_context.wps->registrar;
	wifi_p2p_wps_failed();

	wps_registrar_wps_cancel((void *)reg);
	g_wps_context.clientWpsDone = 0;
	g_wps_context.clientWpsProcessing = 0;
	eap_wsc_server_reset((void *)g_wps_context.wsc_data);
	g_wps_context.wsc_data = NULL;
	return 0;
}

int wpas_wps_enrollee_init_assoc_ie(void)
{
	struct wpabuf *assoc_ie = wps_build_assoc_req_ie(WPS_REQ_ENROLLEE);

	if (assoc_ie) {
		wifi_set_gen_ie(STA_WLAN_INDEX, (char *) wpabuf_head(assoc_ie), wpabuf_len(assoc_ie), P2PWPS_ASSOC_REQ_IE);
		wpabuf_free(assoc_ie);
	}

	return 0;
}

int wpas_wps_enrollee_init_probe_ie(u16 config_methods)
{
	struct wpabuf *probe_ie = NULL;

	if (g_wps_context.wps == NULL) {
		u8 mac[ETH_ALEN];
		wpas_get_wifi_mac(mac);
		wpas_wps_dev_config(mac, 0);
	}

	probe_ie = wps_build_probe_req_ie((config_methods == WPS_CONFIG_PUSHBUTTON) ? DEV_PW_PUSHBUTTON : DEV_PW_DEFAULT,
									  &(g_wps_context.wps->dev), g_wps_context.wps->uuid, WPS_REQ_ENROLLEE, 0, NULL);

	if (probe_ie) {
		wifi_set_gen_ie(STA_WLAN_INDEX, (char *) wpabuf_head(probe_ie), wpabuf_len(probe_ie), P2PWPS_PROBE_REQ_IE);
		wpabuf_free(probe_ie);
	}

	return 0;
}

/*******************************************************************/
/*	Called by applcation End*/
/*******************************************************************/
#endif //CONFIG_WPS
