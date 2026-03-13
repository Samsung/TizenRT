#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils/os.h"
#ifdef CONFIG_LWIP_LAYER
#include "lwip_netconf.h"
#endif
#include "wifi_api.h"
#include "wifi_intf_drv_to_app_internal.h"
#include "eap_protocol_handler.h"

#ifndef ENABLE
#define ENABLE	(1)
#endif
#ifndef DISABLE
#define	DISABLE (0)
#endif

u8 eap_phase = 0;
u8 eap_method = 0;
u8 eap_event_reg_disconn = 0;

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
int connect_by_open_system(char *target_ssid);

int eap_start(char *method);

void set_eap_phase(unsigned char is_trigger_eap)
{
	eap_phase = is_trigger_eap;
	wifi_set_eap_phase(is_trigger_eap);
}

__weak int get_eap_phase(void)
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
	struct rtw_wifi_setting setting = {0};

	wifi_get_setting(STA_WLAN_INDEX, &setting);

	/* disconnect first if already connected*/
	if (strlen((char *)setting.ssid) > 0) {
		wifi_disconnect();
	}
}

extern void eap_peer_unregister_methods(void);
extern void eap_sm_deinit(void);

__weak void eap_disconnected_hdl(void)
{
#ifdef CONFIG_ENABLE_EAP
	if (eap_event_reg_disconn) {
		eap_event_reg_disconn = 0;
		//eap_peer_unregister_methods();
		eap_sm_deinit();
		//reset_config();
	}
#endif
}

__weak int eap_start(char *method)
{
	/* To avoid gcc warnings */
	(void) method;
#ifdef CONFIG_ENABLE_EAP
	int ret = -1;

	//unsigned long tick1 = rtos_time_get_current_system_time_ms();
	//unsigned long tick2;
	while (!(wifi_is_running(STA_WLAN_INDEX))) {
		rtos_time_delay_ms(1000);
	}

	if (wifi_is_running(SOFTAP_WLAN_INDEX)) {
		DiagPrintf("\n\rNot support con-current mode!\n\r");
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
		DiagPrintf("\r\neap method %s not supported\r\n", method);
		return -1;
	}

	eap_method = get_eap_ctx_method();

	DiagPrintf("\n==================== %s_start ====================\n", method);

	//eap_config();

	set_eap_phase(ENABLE);

	ret = connect_by_open_system(eap_target_ssid);

#ifdef CONFIG_LWIP_LAYER
	/* Start DHCPClient */
	if (ret == 0) {
		LwIP_IP_Address_Request(NETIF_WLAN_STA_INDEX);
	}
#endif

	eap_event_reg_disconn = 1;
	set_eap_phase(DISABLE);

	// eap failed, disconnect
	if (ret != 0) {
		judge_station_disconnect();
		eap_disconnected_hdl();
		rtos_time_delay_ms(200);	//wait handler done
		DiagPrintf("\r\nERROR: connect to AP by %s failed\n", method);
	}

	eap_sm_deinit();
	DiagPrintf("\n==================== %s_finish ====================\n", method);

	//tick2 = rtos_time_get_current_system_time_ms();

	return ret;
#else
	return -1;
#endif
}

int connect_by_open_system(char *target_ssid)
{
	int ret;
	struct rtw_network_info connect_param = {0};
	if (target_ssid != NULL) {
		memcpy(connect_param.ssid.val, target_ssid, strlen(target_ssid));
		connect_param.ssid.len = strlen(target_ssid);
		ret = wifi_connect(&connect_param, 1);
		if (ret == RTK_SUCCESS) {
			return 0;
		} else {
			return -1;
		}
	} else {
		DiagPrintf("\r\n[EAP]Target SSID is NULL\n");
		return -1;
	}
}

void eap_autoreconnect_thread(void *method)
{
	eap_start((char *)method);
	rtos_task_delete(NULL);
}

__weak void eap_autoreconnect_hdl(u8 method_id)
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
		DiagPrintf("invalid eap method\n");
		return;
	}
	if (rtos_task_create(NULL, ((const char *)"eap_autoreconnect_thread"), eap_autoreconnect_thread, (void *) method, 1024 * 4, 1) != RTK_SUCCESS) {
		DiagPrintf("\n\r%s rtos_task_create failed\n", __FUNCTION__);
	}
#endif
}

#include <mbedtls/platform.h>
#include <mbedtls/ssl.h>
#include "mbedtls/build_info.h"
#include <ssl_misc.h>
#define MBEDTLS_SSL_COMPRESSION_ADD 0
int max_buf_bio_in_size = MBEDTLS_SSL_IN_BUFFER_LEN;
int max_buf_bio_out_size = MBEDTLS_SSL_OUT_BUFFER_LEN;

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
	(void) data;
	(void) crt;

	DiagPrintf("\nVerify requested for (Depth %d):\n", depth);
	//mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);

	if (((*flags) & MBEDTLS_X509_BADCERT_EXPIRED) != 0) {
		DiagPrintf("server certificate has expired\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_REVOKED) != 0) {
		DiagPrintf("  ! server certificate has been revoked\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_CN_MISMATCH) != 0) {
		DiagPrintf("  ! CN mismatch\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_NOT_TRUSTED) != 0) {
		DiagPrintf("  ! self-signed or not signed by a trusted CA\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCRL_NOT_TRUSTED) != 0) {
		DiagPrintf("  ! CRL not trusted\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCRL_EXPIRED) != 0) {
		DiagPrintf("  ! CRL expired\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_OTHER) != 0) {
		DiagPrintf("  ! other (unknown) flag\n");
	}

	if ((*flags) == 0) {
		DiagPrintf("  Certificate verified without error flags\n");
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
			if (mbedtls_pk_parse_key(_clikey_rsa, eap_client_key, eap_client_key_len, (const unsigned char *)eap_client_key_pwd, strlen(eap_client_key_pwd),
									 TRNG_get_random_bytes_f_rng,
									 (void *)1) != 0) {
				return -1;
			}
		} else {
			if (mbedtls_pk_parse_key(_clikey_rsa, eap_client_key, eap_client_key_len, (const unsigned char *)eap_client_key_pwd, 0, TRNG_get_random_bytes_f_rng,
									 (void *)1) != 0) {
				return -1;
			}
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
