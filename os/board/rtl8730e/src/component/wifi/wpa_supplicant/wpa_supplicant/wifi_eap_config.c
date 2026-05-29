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

extern void eap_sm_deinit(void);

__weak void eap_disconnected_hdl(void)
{
#ifdef CONFIG_ENABLE_EAP
	if (eap_event_reg_disconn) {
		eap_event_reg_disconn = 0;
		eap_sm_deinit();
		//reset_config();
	}
#endif
}

/*
void eap_config(void){
	eap_target_ssid = "Test_eap";
	eap_identity = "guest2";
	eap_password = "test2";

	eap_client_cert = \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIC9zCCAd8CAQMwDQYJKoZIhvcNAQEEBQAwgZMxCzAJBgNVBAYTAkZSMQ8wDQYD\r\n" \
"VQQIEwZSYWRpdXMxEjAQBgNVBAcTCVNvbWV3aGVyZTEVMBMGA1UEChMMRXhhbXBs\r\n" \
"ZSBJbmMuMSAwHgYJKoZIhvcNAQkBFhFhZG1pbkBleGFtcGxlLmNvbTEmMCQGA1UE\r\n" \
"AxMdRXhhbXBsZSBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwHhcNMTYwMzE1MDgwNzEx\r\n" \
"WhcNMTcwMzE1MDgwNzExWjBzMQswCQYDVQQGEwJGUjEPMA0GA1UECBMGUmFkaXVz\r\n" \
"MRUwEwYDVQQKEwxFeGFtcGxlIEluYy4xGjAYBgNVBAMUEXVzZXIyQGV4YW1wbGUu\r\n" \
"Y29tMSAwHgYJKoZIhvcNAQkBFhF1c2VyMkBleGFtcGxlLmNvbTCBnzANBgkqhkiG\r\n" \
"9w0BAQEFAAOBjQAwgYkCgYEAqESlV4OYfBcIgZ+Cs8mWpiBjhvKoa0/kIe7saqhC\r\n" \
"e5q4snox0jdkUpLcc4vOs3vQ7ZGnimqTltA9oF6XNUzTWW4vlJTKEfrCWK085l7c\r\n" \
"DHFvHavH3E6vuP71lI7jq4PLXbo2TvZK+uBul4ozjzVWihaZBtz8eLHq446h/D/p\r\n" \
"kzkCAwEAATANBgkqhkiG9w0BAQQFAAOCAQEAAfhVAIkNdeeUNJud720uUHVnIcxz\r\n" \
"GXWI+Svi1qchuTEnRNhLwXmnE+A0WWSHyfdR6FvzdT3xtz3K50iOif8jY2gCGkSK\r\n" \
"8RjKr97228SwbrGO9y9+dYIjH1uz9cBpoVKcpzdsWpKObrDPDYyReHSWo99jM2+O\r\n" \
"vfJxnBw4PLiBj7Q0/dpd6o4JXyp7Cxa0mB4/+cZqjCzzuKfuK3WP7j6laMCV6mg4\r\n" \
"wRZ528IdwDqB7OOqsDm1PVQM8vzny9PM6ikWUCRTVNQJN8RDLkrHR3FRjy15YLdt\r\n" \
"yOfDqVnT/z0wGBaxnNziSJjqPGHPpRi4bJFGXwXOhtknKmciKzfj9/npoQ==\r\n" \
"-----END CERTIFICATE-----\r\n";

	eap_client_key = \
"-----BEGIN RSA PRIVATE KEY-----\r\n" \
"MIICXQIBAAKBgQCoRKVXg5h8FwiBn4KzyZamIGOG8qhrT+Qh7uxqqEJ7mriyejHS\r\n" \
"N2RSktxzi86ze9DtkaeKapOW0D2gXpc1TNNZbi+UlMoR+sJYrTzmXtwMcW8dq8fc\r\n" \
"Tq+4/vWUjuOrg8tdujZO9kr64G6XijOPNVaKFpkG3Px4serjjqH8P+mTOQIDAQAB\r\n" \
"AoGARI+LyweshssfxSkIKVc3EcNaqi6PHwJzUrw2ChM624AkR1xwllXJg7ehKVdK\r\n" \
"xmjprRLO8CASuL1qjsBb3fTKnBl+sIVxIFS0AI4Y3ri8VUKbangvSsI7pCzAFry7\r\n" \
"p1gmy9WWRV2ZEa+dV8xcrjb3bloT7hcdeLehgBCvExJIQM0CQQDXlSAKdW3AhYyj\r\n" \
"1A+pfyBSGxJbpSwNyyWgwHIHHjxendxmdUbrc8EbAu1eNKbP58TLgdCZsKcMonAv\r\n" \
"MY1Y2/nnAkEAx9CrUaCU8pJqXTRypM5JtexLKnYMJhpnA9uUILBQOq4Oe0eruyF5\r\n" \
"SaSxhyJYXY491ahWYPF0PTb3jkUhoN+l3wJBAJZthjgGDJlEFwjSFkOtYz4nib3N\r\n" \
"GVpeoFj1MBvrazCScpJDz0LIOLzCZCNSFfwIu3dNk+NKMqZMSn+D0h9pD40CQQC5\r\n" \
"K9n4NXaTLbjAU2CC9mE85JPr76XmkcUxwAWQHZTcLH1jJdIyAx1hb+zNLLjzSmRn\r\n" \
"Yi9ae6ibKhtUjyBQ87HFAkA2Bb3z7NUx+AA2g2HZocFZFShBxylACyQkl8FAFZtf\r\n" \
"osudmKdFQHyAWuBMex4tpz/OLTqJ1ecL1JQeC7OvlpEX\r\n" \
"-----END RSA PRIVATE KEY-----\r\n";

	eap_ca_cert = \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIEpzCCA4+gAwIBAgIJAPvZaozpdfjkMA0GCSqGSIb3DQEBCwUAMIGTMQswCQYD\r\n" \
"VQQGEwJGUjEPMA0GA1UECBMGUmFkaXVzMRIwEAYDVQQHEwlTb21ld2hlcmUxFTAT\r\n" \
"BgNVBAoTDEV4YW1wbGUgSW5jLjEgMB4GCSqGSIb3DQEJARYRYWRtaW5AZXhhbXBs\r\n" \
"ZS5jb20xJjAkBgNVBAMTHUV4YW1wbGUgQ2VydGlmaWNhdGUgQXV0aG9yaXR5MB4X\r\n" \
"DTE2MDMxNDExMjU0OVoXDTE2MDQxMzExMjU0OVowgZMxCzAJBgNVBAYTAkZSMQ8w\r\n" \
"DQYDVQQIEwZSYWRpdXMxEjAQBgNVBAcTCVNvbWV3aGVyZTEVMBMGA1UEChMMRXhh\r\n" \
"bXBsZSBJbmMuMSAwHgYJKoZIhvcNAQkBFhFhZG1pbkBleGFtcGxlLmNvbTEmMCQG\r\n" \
"A1UEAxMdRXhhbXBsZSBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwggEiMA0GCSqGSIb3\r\n" \
"DQEBAQUAA4IBDwAwggEKAoIBAQC9pireu0aCDLNfMaGv3vId7RXjUhQwSK0jV2Oc\r\n" \
"SyvlKWH3P/N+5kLrP2iL6SCzyETVDXZ0vOsAMjcBF0zHp16prXV0d51cTUqeWBb0\r\n" \
"I5UnGxleIuuOfSg8zLUJoBWZPqLv++eZ5WgOKHt7SXocjvg7TU5t/TMB0Y8OCz3H\r\n" \
"CW2vJ/XKMgMA9HDUu4g57cJu88i1JPRpyFaz/HIQBc7+UNb9z+q09uTZKWTmEMqi\r\n" \
"E2U0EEIs7EtbxnOze1/8C4XNlmztrEdwvu6UEBU/TFkUoh9M646NkkBK7wP9n9pv\r\n" \
"T0nPQRJiiCrICzVqUtlEi9lIKpbBSMbQ0KzrGF7lGTgm4rz9AgMBAAGjgfswgfgw\r\n" \
"HQYDVR0OBBYEFIVyecka74kvOKIW0BjlTc/B+a2NMIHIBgNVHSMEgcAwgb2AFIVy\r\n" \
"ecka74kvOKIW0BjlTc/B+a2NoYGZpIGWMIGTMQswCQYDVQQGEwJGUjEPMA0GA1UE\r\n" \
"CBMGUmFkaXVzMRIwEAYDVQQHEwlTb21ld2hlcmUxFTATBgNVBAoTDEV4YW1wbGUg\r\n" \
"SW5jLjEgMB4GCSqGSIb3DQEJARYRYWRtaW5AZXhhbXBsZS5jb20xJjAkBgNVBAMT\r\n" \
"HUV4YW1wbGUgQ2VydGlmaWNhdGUgQXV0aG9yaXR5ggkA+9lqjOl1+OQwDAYDVR0T\r\n" \
"BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAZYHM26sxbKOckVqJJ1QY0U2QFlGP\r\n" \
"1GYd8v27znxdnRmSonDvv3GjFfhwoyDk0JUuxkK/33ikCxihrgoO/EQTY9BV2OpW\r\n" \
"qkB1PDtb3i5ZRNvfjmW0pVA4p+GmdTGaEE5pTlcVnorzVrUeFKaZakb+IDFYzmeF\r\n" \
"xp8B3Bb5wvinDligLOaJnSlgS8QeeIab9HZfaVTTuPmVK6zE6D54Y0dJPnykvDdE\r\n" \
"cGN0FC+migfilFjJgkDJ0r78nwes55L8zjoofiZuO03rrHww6ARc3v1jYzAufddk\r\n" \
"QTiZHgjlMQb2XXMmXLn8kBgoDnqkXFNe8j0h8uxIJSrjOoIyn1h1wvX5/w==\r\n" \
"-----END CERTIFICATE-----\r\n";
}
*/
extern void eap_peer_unregister_methods(void);

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
		DiagPrintf("\r\nERROR: connect to AP by %s failed\n", method);
	}

	eap_peer_unregister_methods();  //free malloc when register

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
	while (get_eap_phase()) {
		rtos_time_delay_ms(200);  //wait pre-conn handler done
	}
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
