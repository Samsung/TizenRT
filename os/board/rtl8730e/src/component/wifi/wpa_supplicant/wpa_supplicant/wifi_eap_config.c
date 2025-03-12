#include "main.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <osdep_service.h>
#include "utils/os.h"
#include "wifi_conf.h"
#include "wifi_ind.h"

#define WLAN0_NAME "wlan0"
#ifndef ENABLE
#define ENABLE	(1)
#endif
#ifndef DISABLE
#define	DISABLE (0)
#endif

u8 eap_phase = 0;
u8 eap_method = 0;

// eap config arguments
char *eap_target_ssid = NULL;
char *eap_identity = NULL;
char *eap_password = NULL;
// if set eap_ca_cert and defined(EAP_SSL_VERIFY_SERVER), client will verify server's cert
const unsigned char *eap_ca_cert = NULL;
int eap_ca_cert_len = 0;
// if set eap_client_cert, eap_client_key, and defined(EAP_SSL_VERIFY_CLIENT), client will send its cert to server
const unsigned char *eap_client_cert = NULL;
const unsigned char *eap_client_key = NULL;
int eap_client_cert_len = 0;
int eap_client_key_len = 0;
char *eap_client_key_pwd = NULL;

void set_eap_phase(unsigned char is_trigger_eap);
int get_eap_phase(void);
int get_eap_ctx_method(void);
int set_eap_peap_method(void);
int set_eap_tls_method(void);
int set_eap_ttls_method(void);

void eap_eapol_recvd_hdl(char *buf, int buf_len, int flags, void *handler_user_data);
void eap_eapol_start_hdl(char *buf, int buf_len, int flags, void *handler_user_data);
int connect_by_open_system(char *target_ssid);

int eap_start(char *method);

void set_eap_phase(unsigned char is_trigger_eap)
{
	eap_phase = is_trigger_eap;
	wifi_set_eap_phase(is_trigger_eap);
}

int get_eap_phase(void)
{
	return eap_phase;
}

int get_eap_method(void)
{
	return eap_method;
}

void reset_config(void)
{
	eap_target_ssid = NULL;
	eap_identity = NULL;
	eap_password = NULL;
	eap_ca_cert = NULL;
	eap_client_cert = NULL;
	eap_client_key = NULL;
	eap_client_key_pwd = NULL;
}

void judge_station_disconnect(void)
{
	rtw_wifi_setting_t setting = {RTW_MODE_NONE, {0}, {0}, 0, RTW_SECURITY_OPEN, {0}, 0, 0, 0, 0, 0};

	wifi_get_setting(STA_WLAN_INDEX, &setting);

	/* disconnect first if already connected*/
	if (strlen((char *)setting.ssid) > 0) {
		wifi_disconnect();
	}
}

extern void eap_peer_unregister_methods(void);
extern void eap_sm_deinit(void);
void eap_disconnected_hdl(char *buf, int buf_len, int flags, void *handler_user_data)
{
	/* To avoid gcc warnings */
	(void) buf;
	(void) buf_len;
	(void) flags;
	(void) handler_user_data;

//	printf("disconnected\n");
#if (RTL8192E_SUPPORT == 0)//devin_li rtl8192es_temp_mask	
	wifi_unreg_event_handler(WIFI_EVENT_WPA_EAPOL_RECVD, eap_eapol_recvd_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_DISCONNECT, eap_disconnected_hdl);
	//eap_peer_unregister_methods();
	eap_sm_deinit();
	//reset_config();
#endif
}

int eap_start(char *method)
{
	/* To avoid gcc warnings */
	(void) method;
#ifdef CONFIG_ENABLE_EAP
	int ret = -1;

	//unsigned long tick1 = xTaskGetTickCount();
	//unsigned long tick2;
	while (!(wifi_is_running(STA_WLAN_INDEX))) {
		vTaskDelay(1000 / portTICK_RATE_MS);
	}

	if (wifi_is_running(WLAN1_IDX)) {
		printf("\n\rNot support con-current mode!\n\r");
		return -1;
	}

	judge_station_disconnect();

#if CONFIG_ENABLE_PEAP
	if (strcmp(method, "peap") == 0) {
		ret = set_eap_peap_method();
	}
#endif

#if CONFIG_ENABLE_TLS
	if (strcmp(method, "tls") == 0) {
		ret = set_eap_tls_method();
	}
#endif

#if CONFIG_ENABLE_TTLS
	if (strcmp(method, "ttls") == 0) {
		ret = set_eap_ttls_method();
	}
#endif

	if (ret == -1) {
		printf("\r\neap method %s not supported\r\n", method);
		return -1;
	}

	eap_method = get_eap_ctx_method();

	printf("\n==================== %s_start ====================\n", method);

	//eap_config();

	set_eap_phase(ENABLE);
	wifi_reg_event_handler(WIFI_EVENT_WPA_EAPOL_START, eap_eapol_start_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_WPA_EAPOL_RECVD, eap_eapol_recvd_hdl, NULL);



	ret = connect_by_open_system(eap_target_ssid);

#if CONFIG_LWIP_LAYER
	/* Start DHCPClient */
	if (ret == 0) {
		LwIP_DHCP(0, DHCP_START);
	}
#endif

	wifi_unreg_event_handler(WIFI_EVENT_WPA_EAPOL_START, eap_eapol_start_hdl);

	// for re-authentication when session timeout
	wifi_reg_event_handler(WIFI_EVENT_DISCONNECT, eap_disconnected_hdl, NULL);
	//wifi_unreg_event_handler(WIFI_EVENT_WPA_EAPOL_RECVD, eap_eapol_recvd_hdl);

	set_eap_phase(DISABLE);

	// eap failed, disconnect
	if (ret != 0) {
		judge_station_disconnect();
		eap_disconnected_hdl(NULL, 0, 0, NULL);
		rtw_msleep_os(200);	//wait handler done
		printf("\r\nERROR: connect to AP by %s failed\n", method);
	}

	eap_sm_deinit();
	printf("\n==================== %s_finish ====================\n", method);

	//tick2 = xTaskGetTickCount();
	//printf("\r\nConnected after %dms.\n", (tick2-tick1));

	return ret;
#else
	return -1;
#endif
}

int connect_by_open_system(char *target_ssid)
{
	int ret;
	rtw_network_info_t connect_param = {0};
	if (target_ssid != NULL) {
		memcpy(connect_param.ssid.val, target_ssid, strlen(target_ssid));
		connect_param.ssid.len = strlen(target_ssid);
		connect_param.security_type = RTW_SECURITY_OPEN;
		ret = wifi_connect(&connect_param, 1);
		if (ret == RTW_SUCCESS) {
			//printf("\r\n[EAP]Associate with AP success\n");
			return 0;
		} else {
			return -1;
		}
	} else {
		printf("\r\n[EAP]Target SSID is NULL\n");
		return -1;
	}
}

void eap_autoreconnect_thread(void *method)
{
	eap_start((char *)method);
	vTaskDelete(NULL);
}

void eap_autoreconnect_hdl(u8 method_id)
{
	/* To avoid gcc warnings */
	(void) method_id;
#ifdef CONFIG_ENABLE_EAP
	char *method;
	switch (method_id) {
	case 25: // EAP_TYPE_PEAP
		method = "peap";
		break;
	case 13: // EAP_TYPE_TLS
		method = "tls";
		break;
	case 21: // EAP_TYPE_TTLS
		method = "ttls";
		break;
	default:
		printf("invalid eap method\n");
		return;
	}
	if (xTaskCreate(eap_autoreconnect_thread, ((const char *)"eap_autoreconnect_thread"), 1024, (void *) method, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate failed\n", __FUNCTION__);
	}
#endif
}

#include <mbedtls/platform.h>
#include <mbedtls/ssl.h>

#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03000000)
#include "mbedtls/build_info.h"
#include <mbedtls/ssl_misc.h>
#define MBEDTLS_SSL_COMPRESSION_ADD 0
int max_buf_bio_in_size = MBEDTLS_SSL_IN_BUFFER_LEN;
int max_buf_bio_out_size = MBEDTLS_SSL_OUT_BUFFER_LEN;
#elif (MBEDTLS_VERSION >= MBEDTLS_VERSION_CONVERT(2,16,9))
#include <mbedtls/config.h>
#include <mbedtls/ssl_internal.h>
int max_buf_bio_in_size = MBEDTLS_SSL_IN_BUFFER_LEN;
int max_buf_bio_out_size = MBEDTLS_SSL_OUT_BUFFER_LEN;
#else
#include <mbedtls/ssl_internal.h>
int max_buf_bio_size = MBEDTLS_SSL_BUFFER_LEN;
#endif

struct eap_tls {
	void *ssl;
	void *conf;
	void *fd;
};

#if (defined(ENABLE_EAP_SSL_VERIFY_CLIENT) && ENABLE_EAP_SSL_VERIFY_CLIENT)
static mbedtls_x509_crt *_cli_crt = NULL;
static mbedtls_pk_context *_clikey_rsa = NULL;
#endif

#if (defined(ENABLE_EAP_SSL_VERIFY_SERVER) && ENABLE_EAP_SSL_VERIFY_SERVER)
static mbedtls_x509_crt *_ca_crt = NULL;

static int eap_verify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{

	//char buf[1024];
	((void) data);

	printf("\nVerify requested for (Depth %d):\n", depth);
	//mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);
	//printf("%s", buf);

	if (((*flags) & MBEDTLS_X509_BADCERT_EXPIRED) != 0) {
		printf("server certificate has expired\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_REVOKED) != 0) {
		printf("  ! server certificate has been revoked\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_CN_MISMATCH) != 0) {
		printf("  ! CN mismatch\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_NOT_TRUSTED) != 0) {
		printf("  ! self-signed or not signed by a trusted CA\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCRL_NOT_TRUSTED) != 0) {
		printf("  ! CRL not trusted\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCRL_EXPIRED) != 0) {
		printf("  ! CRL expired\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_OTHER) != 0) {
		printf("  ! other (unknown) flag\n");
	}

	if ((*flags) == 0) {
		printf("  Certificate verified without error flags\n");
	}

	return (0);
}
#endif

int eap_cert_init(void)
{
#if (defined(ENABLE_EAP_SSL_VERIFY_CLIENT) && ENABLE_EAP_SSL_VERIFY_CLIENT)
	if (eap_client_cert != NULL && eap_client_key != NULL) {
		_cli_crt = mbedtls_calloc(1, sizeof(mbedtls_x509_crt));

		if (_cli_crt) {
			mbedtls_x509_crt_init(_cli_crt);
		} else {
			return -1;
		}

		_clikey_rsa = mbedtls_calloc(1, sizeof(mbedtls_pk_context));

		if (_clikey_rsa) {
			mbedtls_pk_init(_clikey_rsa);
		} else {
			return -1;
		}
	}
#endif
#if (defined(ENABLE_EAP_SSL_VERIFY_SERVER) && ENABLE_EAP_SSL_VERIFY_SERVER)
	if (eap_ca_cert != NULL) {
		_ca_crt = mbedtls_calloc(1, sizeof(mbedtls_x509_crt));

		if (_ca_crt) {
			mbedtls_x509_crt_init(_ca_crt);
		} else {
			return -1;
		}
	}
#endif
	return 0;
}

void eap_client_cert_free(void)
{
#if (defined(ENABLE_EAP_SSL_VERIFY_CLIENT) && ENABLE_EAP_SSL_VERIFY_CLIENT)
	if (eap_client_cert != NULL && eap_client_key != NULL) {
		if (_cli_crt) {
			mbedtls_x509_crt_free(_cli_crt);
			mbedtls_free(_cli_crt);
			_cli_crt = NULL;
		}

		if (_clikey_rsa) {
			mbedtls_pk_free(_clikey_rsa);
			mbedtls_free(_clikey_rsa);
			_clikey_rsa = NULL;
		}
	}
#endif
}

void eap_server_cert_free(void)
{
#if (defined(ENABLE_EAP_SSL_VERIFY_SERVER) && ENABLE_EAP_SSL_VERIFY_SERVER)
	if (eap_ca_cert != NULL) {
		if (_ca_crt) {
			mbedtls_x509_crt_free(_ca_crt);
			mbedtls_free(_ca_crt);
			_ca_crt = NULL;
		}
	}
#endif
}

int eap_cert_setup(struct eap_tls *tls_context)
{
	/* To avoid gcc warnings */
	(void) tls_context;
#if (defined(ENABLE_EAP_SSL_VERIFY_CLIENT) && ENABLE_EAP_SSL_VERIFY_CLIENT)
	if (eap_client_cert != NULL && eap_client_key != NULL) {
		if (mbedtls_x509_crt_parse(_cli_crt, eap_client_cert, eap_client_cert_len) != 0) {
			return -1;
		}
		if (eap_client_key_pwd) {
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
			if (mbedtls_pk_parse_key(_clikey_rsa, eap_client_key, eap_client_key_len, eap_client_key_pwd, strlen(eap_client_key_pwd), rtw_get_random_bytes_f_rng,
									 (void *)1) != 0)
#else
			if (mbedtls_pk_parse_key(_clikey_rsa, eap_client_key, eap_client_key_len, eap_client_key_pwd, strlen(eap_client_key_pwd)) != 0)
#endif
				return -1;
		} else {
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER>=0x03010000)
			if (mbedtls_pk_parse_key(_clikey_rsa, eap_client_key, eap_client_key_len, eap_client_key_pwd, 0, rtw_get_random_bytes_f_rng, (void *)1) != 0)
#else
			if (mbedtls_pk_parse_key(_clikey_rsa, eap_client_key, eap_client_key_len, eap_client_key_pwd, 0) != 0)
#endif
				return -1;
		}

		mbedtls_ssl_conf_own_cert(tls_context->conf, _cli_crt, _clikey_rsa);
	}
#endif
#if (defined(ENABLE_EAP_SSL_VERIFY_SERVER) && ENABLE_EAP_SSL_VERIFY_SERVER)
	if (eap_ca_cert != NULL) {
		if (mbedtls_x509_crt_parse(_ca_crt, eap_ca_cert, eap_ca_cert_len) != 0) {
			return -1;
		}
		mbedtls_ssl_conf_ca_chain(tls_context->conf, _ca_crt, NULL);
		mbedtls_ssl_conf_authmode(tls_context->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
		mbedtls_ssl_conf_verify(tls_context->conf, eap_verify, NULL);
	}
#endif
	return 0;
}

//#endif
