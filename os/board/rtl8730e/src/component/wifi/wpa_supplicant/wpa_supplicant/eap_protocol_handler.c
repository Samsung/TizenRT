#include <osdep_service.h>
#include "lwip_netconf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils/os.h"
#include "eap_protocol_handler.h"
#include "wps_protocol_handler.h"
#include "wifi_conf.h"
#include "wifi_ind.h"
#include "crypto/crypto.h"
#include "crypto/tls.h"
#include "eap_peer/eap_config.h"
#include "wpa_lite_intf.h"

struct task_struct eap_recvd_tsk;
struct rtw_eap_context g_eap_context = {0};
const struct eap_method *eap;

// get config arguments from wifi_eap_config.c
extern char *eap_identity;
extern char *eap_password;
extern const unsigned char *eap_ca_cert;
extern const unsigned char *eap_client_cert;
extern const unsigned char *eap_client_key;

extern void eap_server_cert_free(void);
extern void eap_client_cert_free(void);
extern int eap_cert_init(void);
extern int eap_cert_setup(void *tls_context);

void *Rx_handle = NULL;

int get_eap_ctx_method(void)
{
	return g_eap_context.eap_method;
}

int set_eap_peap_method(void)
{
#if CONFIG_PEAP
	int ret = eap_peer_peap_register();
	if (ret != -1) {
		ret = eap_peer_mschapv2_register();
	}
	if (ret != -1) {
		eap = eap_peer_get_eap_method(EAP_VENDOR_IETF, EAP_TYPE_PEAP);
	}
	if (eap == NULL) {
		return -1;
	}
	g_eap_context.eap_method = EAP_TYPE_PEAP;
	wifi_set_eap_method(EAP_TYPE_PEAP);
	return 0;
#else
	return -1;
#endif
}

int set_eap_tls_method(void)
{
#if CONFIG_TLS
	int ret = eap_peer_tls_register();
	if (ret != -1) {
		eap = eap_peer_get_eap_method(EAP_VENDOR_IETF, EAP_TYPE_TLS);
	}
	if (eap == NULL) {
		return -1;
	}
	g_eap_context.eap_method = EAP_TYPE_TLS;
	wifi_set_eap_method(EAP_TYPE_TLS);
	return 0;
#else
	return -1;
#endif
}

int set_eap_ttls_method(void)
{
#if CONFIG_TTLS
	int ret = eap_peer_ttls_register();
	if (ret != -1) {
		eap = eap_peer_get_eap_method(EAP_VENDOR_IETF, EAP_TYPE_TTLS);
	}
	if (eap == NULL) {
		return -1;
	}
	g_eap_context.eap_method = EAP_TYPE_TTLS;
	wifi_set_eap_method(EAP_TYPE_TTLS);
	return 0;
#else
	return -1;
#endif
}

void eap_send_eapol_start(u8 *dst_mac)
{
	u8 *dev_mac = LwIP_GetMAC(0);
	struct wlan_ethhdr_t *eth_hdr;
	struct lib1x_eapol *eapol;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN; //18

	wpa_printf(MSG_INFO, "EAP: Send EAPOL-Start");

	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send eapol start failed!\n");
		return;
	}

	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(&eth_hdr->daddr, dst_mac, ETH_ALEN);
	os_memcpy(&eth_hdr->saddr, dev_mac, ETH_ALEN);
	eth_hdr->type = htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((unsigned char *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = (LIB1X_EAPOL_VER);
	eapol->packet_type = (LIB1X_EAPOL_START);
	eapol->packet_body_length = 0;

	wpa_hexdump_key(MSG_MSGDUMP, "EAPOL-START", buf, length);

	wifi_if_send_eapol(STA_WLAN_INDEX, (char *)buf, length, 0);

	os_free(buf, length);
	return;
}

static void eap_send_eap_rspidentity(u8 *rx_buf)
{
	struct wlan_ethhdr_t *eth_hdr;
	struct lib1x_eapol *eapol;
	struct lib1x_eapol_message_hdr *received_eapol_identity_payload_hdr;
	struct lib1x_eapol_message_hdr *eapol_identity_respose;
	unsigned char *identity = g_eap_context.eap_sm->last_config->identity;
	unsigned char *temp = NULL;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN + 34; //52

	wpa_printf(MSG_INFO, "EAP: Send EAP RSP Identity");

	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send eap fail failed!\n");
		return;
	}

	// get the sending address which we want to put data into(EAPOL packet)
	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(eth_hdr->daddr, (rx_buf + 6), ETH_ALEN);
	os_memcpy(eth_hdr->saddr, rx_buf, ETH_ALEN);
	eth_hdr->type = htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((u8 *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = LIB1X_EAPOL_VER;
	eapol->packet_type = LIB1X_EAPOL_EAPPKT;
	eapol->packet_body_length = htons(5 + strlen((const char *)identity));

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
	eapol_identity_respose->Length = htons(5 + strlen((const char *)identity));

	// EAP Type
	eapol_identity_respose->Type = LIB1X_EAP_TYPE_IDENTITY;

	temp = ((unsigned char *)eapol_identity_respose + LIB1X_EAPOL_MESSAGE_HDRLEN);
	os_memcpy(temp, identity, strlen((const char *)identity));

	wpa_hexdump_key(MSG_MSGDUMP, "IDENTIFY-RSP", buf, length);
	wifi_if_send_eapol(STA_WLAN_INDEX, (char *)buf, length, 0);

	os_free(buf, length);
	return;
}

static void eap_send_packet(u8 *rx_buf, struct wpabuf *sendData)
{
//printf("Owen: send %d data\n", sendData->used);
	struct lib1x_eapol *eapol = NULL;
	struct wlan_ethhdr_t *eth_hdr = NULL;
	struct _LIB1X_EAPOL_WSC	*Eapol_wsc_to_send = NULL;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN + sendData->used;

	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send failed!\n");
		goto exit;
	}

	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(eth_hdr->daddr, (rx_buf + 6), ETH_ALEN);
	os_memcpy(eth_hdr->saddr, rx_buf, ETH_ALEN);
	eth_hdr->type = htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((unsigned char *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = LIB1X_EAPOL_VER;
	eapol->packet_type = LIB1X_EAPOL_EAPPKT;
	eapol->packet_body_length = htons(sendData->used);

	Eapol_wsc_to_send = (struct _LIB1X_EAPOL_WSC *)((unsigned char *)eapol + LIB1X_EAPOL_HDRLEN);
	os_memcpy(Eapol_wsc_to_send, sendData->buf, sendData->used);

	wpa_hexdump_key(MSG_MSGDUMP, "TX-EAPOL", buf, length);
	wifi_if_send_eapol(STA_WLAN_INDEX, (char *)buf, length, 0);

	os_free(buf, length);
exit:
	wpabuf_free(sendData);
	return;
}

void eap_sm_deinit(void)
{
	struct eap_sm *sm = g_eap_context.eap_sm;
	void *data = g_eap_context.eap_data;

	g_eap_context.isKeySet = 0;

	if (sm != NULL) {
		eap->deinit(sm, data);
		tls_deinit(sm->ssl_ctx);
		if (sm->last_config != NULL) {
			os_free(sm->last_config, 0);
		}
		os_free(sm, 0);
		g_eap_context.eap_sm = NULL;
		g_eap_context.eap_data = NULL;
	}

	// unregister eap methods
	// move to outer wifi_eap_config.c
	//eap_peer_unregister_methods();

	eap_client_cert_free();
	eap_server_cert_free();
}

int eap_sm_init(void)
{
	g_eap_context.eapIsProcessing = 1;
	g_eap_context.isKeySet = 0;

	if (eap == NULL) {
		wpa_printf(MSG_INFO, "EAP: eap method null");
		return -1;
	}


	struct eap_sm *sm;
	sm = os_zalloc(sizeof(*sm));
	if (sm == NULL) {
		wpa_printf(MSG_INFO, "EAP: sm zalloc failed");
		return -1;
	}

	sm->ssl_ctx = (void *)tls_init(NULL);
	if (sm->ssl_ctx == NULL) {
		wpa_printf(MSG_INFO, "EAP: tls_init() failed");
		return -1;
	} else {
		wpa_printf(MSG_DEBUG, "EAP: tls_init() success");
	}
	g_eap_context.eap_sm = sm;

	struct eap_peer_config *conf;
	conf = os_zalloc(sizeof(*conf));
	if (conf == NULL) {
		wpa_printf(MSG_INFO, "EAP: conf zalloc failed");
		os_free(sm, 0);
		return -1;
	}

	conf->identity = (u8 *) eap_identity;
	conf->identity_len = strlen((const char *)conf->identity);
	conf->password = (u8 *) eap_password;
	if (conf->password != NULL) {
		conf->password_len = strlen((const char *)conf->password);
	}
	conf->fragment_size = 1398; // default size

	switch (g_eap_context.eap_method) {
	case EAP_TYPE_PEAP:
		//conf->phase1 = "peapver=1 peaplabel=1";
		conf->phase2 = "auth=MSCHAPV2";
		break;
	case EAP_TYPE_TLS:
		// check cert & key
		if (eap_client_cert == NULL || eap_client_key == NULL) {
			wpa_printf(MSG_INFO, "EAP: missing client cert/key for EAP-TLS method");
			eap_sm_deinit();
			return -1;
		}
		break;
	case EAP_TYPE_TTLS:
		//conf->phase2 = "auth=MD5";
		conf->phase2 = "auth=MSCHAPV2";
		break;
	default:
		break;
	}

	g_eap_context.eap_sm->last_config = conf;

	// using hw crypto in this case will fail (enc/dec wrong, can't get rx thread stack data)
	// no further used, can be deleted
	//rom_ssl_ram_map.use_hw_crypto_func = 0;

	g_eap_context.eap_data = eap->init(g_eap_context.eap_sm);
	if (g_eap_context.eap_data == NULL) {
		wpa_printf(MSG_INFO, "EAP: eap_init() failed...");
		eap_sm_deinit();
		return -1;
	}



	// set the cert auth
	if (eap_cert_init() != 0) {
		wpa_printf(MSG_INFO, "EAP: eap_cert_init failed");
		eap_sm_deinit();
		return -1;
	}
	if (eap_cert_setup(sm->ssl_ctx) != 0) {
		wpa_printf(MSG_INFO, "EAP: eap_cert_setup failed");
		eap_sm_deinit();
		return -1;
	}

	return 0;
}

void dump_buf(void *buf, size_t len)
{
	unsigned char *b = buf;
	for (int i = 0; i < len; i++) {
		printf(" %02X", (unsigned int) b[i]);
	}
}

void eap_send_Nak(u8 *rx_buf)
{
	struct wpabuf *sendData = NULL;
	struct lib1x_eapol_message_hdr *received_eapol_payload_hdr;
	int reqId = 0;

	received_eapol_payload_hdr = (struct lib1x_eapol_message_hdr *)(rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);
	reqId = received_eapol_payload_hdr->Identifier;

	//refer to: eap_sm_buildNak(g_eap_context.eap_sm, reqId);
	sendData = eap_msg_alloc(EAP_VENDOR_IETF, EAP_TYPE_NAK,
							 sizeof(struct eap_hdr) + 1 + 1 + 1,
							 EAP_CODE_RESPONSE, reqId);
	if (sendData == NULL) {
		printf("EAP: eap_msg_alloc failed\n");
		return;
	}
	wpabuf_put_u8(sendData, get_eap_ctx_method());
	eap_update_len(sendData);

	eap_send_packet(rx_buf, sendData);
}

void eap_supplicant_handle_recvd(u8 *rx_buf)
{
	struct wpabuf *sendData = NULL;
	struct wpabuf *reqData;

	struct lib1x_eapol_message_hdr *eapol_payload_hdr;
	eapol_payload_hdr = (struct lib1x_eapol_message_hdr *)((u8 *)rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);

	reqData = os_zalloc(sizeof(*reqData));
	if (reqData == NULL) {
		wpa_printf(MSG_INFO, "EAP: reqData zalloc failed");
		return;
	}

	reqData->buf = (unsigned char *)eapol_payload_hdr;
	sendData = eap->process(g_eap_context.eap_sm, g_eap_context.eap_data, NULL, reqData);

	os_free(reqData, 0);

	if (sendData == NULL) {
		wpa_printf(MSG_INFO, "EAP: sendData is null");
		return;
	}

	//dump_buf(sendData->buf, sendData->used);

	// set wpa_global_PSK for incoming 4-way handshake
	if (g_eap_context.isKeySet == 0 && eap->isKeyAvailable(g_eap_context.eap_sm, g_eap_context.eap_data)) {
		int key_len = 0;
		u8 *key = eap->getKey(g_eap_context.eap_sm, g_eap_context.eap_data, (size_t *)&key_len);
		if (key_len < 1 || key == NULL) {
			wpa_printf(MSG_INFO, "[EAP]4-way handshake key setting failed");
		} else {
			rtw_psk_set_pmk_from_eap(key, (u8 *)rx_buf + 6);
			os_free(key, 0);
		}
	}

	eap_send_packet(rx_buf, sendData);
}

//void eap_eapol_recvd(char *buf, int buf_len, int flags, void* handler_user_data )
void eap_eapol_recvd(void *buf)
{
	struct lib1x_eapol_message_hdr *eapol_payload_hdr;
	eapol_payload_hdr = (struct lib1x_eapol_message_hdr *)((u8 *)buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);

	//wpa_hexdump_key(MSG_MSGDUMP, "RX-EAPOL", buf, buf_len);

	if ((EAP_CODE_REQUEST == eapol_payload_hdr->Code)) {
		// Since our crypto would process for a longer time, server might re-send the eap id req. Ignore the resend req (len != 5) to proceed.
		if ((EAP_TYPE_IDENTITY == eapol_payload_hdr->Type) && (htons(5) == eapol_payload_hdr->Length)) {
			wpa_printf(MSG_DEBUG, "[EAP]Recv EAPOL packet: EAP-Request_Identity");
			// current process hang, deinit sm and retry
			if (g_eap_context.eapIsProcessing == 1) {
				eap_sm_deinit();
				wpa_printf(MSG_DEBUG, "[EAP]process retry");
			}
			if (eap_sm_init() != 0) {
				g_eap_context.eapIsProcessing = 0;
				//wpa_printf(MSG_INFO, "[EAP]eap_sm_init failed");
				printf("EAP: eap_sm_init failed\n");
				goto exit;
			}
			eap_send_eap_rspidentity((u8 *)buf);
		} else if (eapol_payload_hdr->Type != get_eap_ctx_method()) {
			//printf("type not match, recv: %d, support: %d\n", eapol_payload_hdr->Type, get_eap_ctx_method());
			eap_send_Nak((u8 *)buf);
		} else {
			eap_supplicant_handle_recvd((u8 *)buf);
		}
	} else if ((EAP_CODE_SUCCESS == eapol_payload_hdr->Code)) {
		wpa_printf(MSG_INFO, "[EAP]Recv EAPOL packet: EAP-Success");

		// set wpa_global_PSK for incoming 4-way handshake
		if (eap->isKeyAvailable(g_eap_context.eap_sm, g_eap_context.eap_data)) {
			int key_len = 0;
			u8 *key = eap->getKey(g_eap_context.eap_sm, g_eap_context.eap_data, (size_t *)&key_len);
			if (key_len < 1 || key == NULL) {
				wpa_printf(MSG_INFO, "[EAP]4-way handshake key setting failed");
			} else {
				rtw_psk_set_pmk_from_eap(key, (u8 *)buf + 6);
				os_free(key, 0);
			}
		}

		eap_sm_deinit();
		g_eap_context.eapIsProcessing = 0;
	} else if ((EAP_CODE_FAILURE == eapol_payload_hdr->Code)) {
		//wpa_printf(MSG_INFO, "[EAP]Recv EAPOL packet: EAP-Failure");
		printf("EAP: Recv EAPOL packet: EAP-Failure\n");
		eap_sm_deinit();
		g_eap_context.eapIsProcessing = 0;
		wifi_indication(WIFI_EVENT_DISCONNECT, NULL, 0, 0);
	}
exit:
	os_free(buf, 0);
	if (Rx_handle != NULL) {
		rtw_resume_task(Rx_handle);
	}
	rtw_delete_task(&eap_recvd_tsk);
}

void eap_eapol_recvd_hdl(char *buf, int buf_len, int flags, void *handler_user_data)
{
//	eap_eapol_recvd(buf, buf_len, flags, handler_user_data);

	char *copy_buf = os_malloc(buf_len);
	memcpy(copy_buf, buf, buf_len);
	if (rtw_create_task(&eap_recvd_tsk, "eap_recvd", 1024, tskIDLE_PRIORITY + 1, (thread_func_t)eap_eapol_recvd, copy_buf) != _PASS) {
		printf("\n\r%s rtw_create_task failed\n", __FUNCTION__);
	} else {
		Rx_handle = (void *)rtw_get_current_TaskHandle();
		rtw_suspend_task(NULL);
	}
}

void eap_eapol_start_hdl(char *buf, int buf_len, int flags, void *handler_user_data)
{
	u8 *dst_mac = (u8 *)buf;
	eap_send_eapol_start(dst_mac);
}
