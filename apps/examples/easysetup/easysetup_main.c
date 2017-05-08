/****************************************************************************
 * examples/eaysetup/easysetup.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <sys/socket.h>
#include <apps/netutils/webserver/http_server.h>
#include <apps/netutils/webserver/http_keyvalue_list.h>
#include <apps/netutils/cJSON.h>
#include "wifi_api.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define WPA_MAX_SSID_LEN	(4 * 32 + 1)
#define MODE_STRING_MAX		100

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
struct ap_info_t {
	char ssid[32];
	char security[32];
	char pwd[64];
};

const char *soft_ap_ssid = "IoT_SoftAP10";
const char *soft_ap_passphase = "12345678";
const char *soft_ap_security = "wpa2_aes";
static int soft_ap_channel = 1;
static struct ap_info_t ap_info;
sem_t get_netinfo;
sem_t finish_scan;
sem_t get_ctrl;
static int g_stop_easysetup = 0;
const char *root_uri = "/";
const char *network_uri = "/network";
const char *control_uri = "/control";
static struct http_server_t *https_server = NULL;
static const char g_httpcontype[]  = "Content-type";
static const char g_httpconhtml[]  = "text/html";
static const char g_httpcontsize[] = "Content-Length";
static const char g_httpconnect[]  = "Connection";
static const char g_httpcnlost[]   = "close";
static struct timeval rt;

#ifdef CONFIG_HW_RSA
#include "tls/sss_key.h"
#include "tls/see_api.h"

#define EASY_CA_KEY_INDEX		1
#define EASY_DEV_KEY_INDEX		2
#define EASY_CA_CERT_INDEX		1
#define EASY_DEV_CERT_INDEX		2

#else
static const char ca_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIDhzCCAm+gAwIBAgIBADANBgkqhkiG9w0BAQUFADA7MQswCQYDVQQGEwJOTDER\r\n"
	"MA8GA1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwHhcN\r\n"
	"MTEwMjEyMTQ0NDAwWhcNMjEwMjEyMTQ0NDAwWjA7MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwggEiMA0G\r\n"
	"CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDA3zf8F7vglp0/ht6WMn1EpRagzSHx\r\n"
	"mdTs6st8GFgIlKXsm8WL3xoemTiZhx57wI053zhdcHgH057Zk+i5clHFzqMwUqny\r\n"
	"50BwFMtEonILwuVA+T7lpg6z+exKY8C4KQB0nFc7qKUEkHHxvYPZP9al4jwqj+8n\r\n"
	"YMPGn8u67GB9t+aEMr5P+1gmIgNb1LTV+/Xjli5wwOQuvfwu7uJBVcA0Ln0kcmnL\r\n"
	"R7EUQIN9Z/SG9jGr8XmksrUuEvmEF/Bibyc+E1ixVA0hmnM3oTDPb5Lc9un8rNsu\r\n"
	"KNF+AksjoBXyOGVkCeoMbo4bF6BxyLObyavpw/LPh5aPgAIynplYb6LVAgMBAAGj\r\n"
	"gZUwgZIwDAYDVR0TBAUwAwEB/zAdBgNVHQ4EFgQUtFrkpbPe0lL2udWmlQ/rPrzH\r\n"
	"/f8wYwYDVR0jBFwwWoAUtFrkpbPe0lL2udWmlQ/rPrzH/f+hP6Q9MDsxCzAJBgNV\r\n"
	"BAYTAk5MMREwDwYDVQQKEwhQb2xhclNTTDEZMBcGA1UEAxMQUG9sYXJTU0wgVGVz\r\n"
	"dCBDQYIBADANBgkqhkiG9w0BAQUFAAOCAQEAuP1U2ABUkIslsCfdlc2i94QHHYeJ\r\n"
	"SsR4EdgHtdciUI5I62J6Mom+Y0dT/7a+8S6MVMCZP6C5NyNyXw1GWY/YR82XTJ8H\r\n"
	"DBJiCTok5DbZ6SzaONBzdWHXwWwmi5vg1dxn7YxrM9d0IjxM27WNKs4sDQhZBQkF\r\n"
	"pjmfs2cb4oPl4Y9T9meTx/lvdkRYEug61Jfn6cA+qHpyPYdTH+UshITnmp5/Ztkf\r\n"
	"m/UTSLBNFNHesiTZeH31NcxYGdHSme9Nc/gfidRa0FLOCfWxRlFqAI47zG9jAQCZ\r\n"
	"7Z2mCGDNMhjQc+BYcdnl0lPXjdDK6V0qCg1dVewhUBcW5gZKzV7e9+DpVA==\r\n"
	"-----END CERTIFICATE-----\r\n";

static const char srv_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIDNzCCAh+gAwIBAgIBAjANBgkqhkiG9w0BAQUFADA7MQswCQYDVQQGEwJOTDER\r\n"
	"MA8GA1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwHhcN\r\n"
	"MTEwMjEyMTQ0NDA2WhcNMjEwMjEyMTQ0NDA2WjA0MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxEjAQBgNVBAMTCWxvY2FsaG9zdDCCASIwDQYJKoZIhvcN\r\n"
	"AQEBBQADggEPADCCAQoCggEBAMFNo93nzR3RBNdJcriZrA545Do8Ss86ExbQWuTN\r\n"
	"owCIp+4ea5anUrSQ7y1yej4kmvy2NKwk9XfgJmSMnLAofaHa6ozmyRyWvP7BBFKz\r\n"
	"NtSj+uGxdtiQwWG0ZlI2oiZTqqt0Xgd9GYLbKtgfoNkNHC1JZvdbJXNG6AuKT2kM\r\n"
	"tQCQ4dqCEGZ9rlQri2V5kaHiYcPNQEkI7mgM8YuG0ka/0LiqEQMef1aoGh5EGA8P\r\n"
	"hYvai0Re4hjGYi/HZo36Xdh98yeJKQHFkA4/J/EwyEoO79bex8cna8cFPXrEAjya\r\n"
	"HT4P6DSYW8tzS1KW2BGiLICIaTla0w+w3lkvEcf36hIBMJcCAwEAAaNNMEswCQYD\r\n"
	"VR0TBAIwADAdBgNVHQ4EFgQUpQXoZLjc32APUBJNYKhkr02LQ5MwHwYDVR0jBBgw\r\n"
	"FoAUtFrkpbPe0lL2udWmlQ/rPrzH/f8wDQYJKoZIhvcNAQEFBQADggEBAJxnXClY\r\n"
	"oHkbp70cqBrsGXLybA74czbO5RdLEgFs7rHVS9r+c293luS/KdliLScZqAzYVylw\r\n"
	"UfRWvKMoWhHYKp3dEIS4xTXk6/5zXxhv9Rw8SGc8qn6vITHk1S1mPevtekgasY5Y\r\n"
	"iWQuM3h4YVlRH3HHEMAD1TnAexfXHHDFQGe+Bd1iAbz1/sH9H8l4StwX6egvTK3M\r\n"
	"wXRwkKkvjKaEDA9ATbZx0mI8LGsxSuCqe9r9dyjmttd47J1p1Rulz3CLzaRcVIuS\r\n"
	"RRQfaD8neM9c1S/iJ/amTVqJxA1KOdOS5780WhPfSArA+g4qAmSjelc3p4wWpha8\r\n"
	"zhuYwjVuX6JHG0c=\r\n"
	"-----END CERTIFICATE-----\r\n";

static const char srv_key_rsa[] =
	"-----BEGIN RSA PRIVATE KEY-----\r\n"
	"MIIEpAIBAAKCAQEAwU2j3efNHdEE10lyuJmsDnjkOjxKzzoTFtBa5M2jAIin7h5r\r\n"
	"lqdStJDvLXJ6PiSa/LY0rCT1d+AmZIycsCh9odrqjObJHJa8/sEEUrM21KP64bF2\r\n"
	"2JDBYbRmUjaiJlOqq3ReB30Zgtsq2B+g2Q0cLUlm91slc0boC4pPaQy1AJDh2oIQ\r\n"
	"Zn2uVCuLZXmRoeJhw81ASQjuaAzxi4bSRr/QuKoRAx5/VqgaHkQYDw+Fi9qLRF7i\r\n"
	"GMZiL8dmjfpd2H3zJ4kpAcWQDj8n8TDISg7v1t7HxydrxwU9esQCPJodPg/oNJhb\r\n"
	"y3NLUpbYEaIsgIhpOVrTD7DeWS8Rx/fqEgEwlwIDAQABAoIBAQCXR0S8EIHFGORZ\r\n"
	"++AtOg6eENxD+xVs0f1IeGz57Tjo3QnXX7VBZNdj+p1ECvhCE/G7XnkgU5hLZX+G\r\n"
	"Z0jkz/tqJOI0vRSdLBbipHnWouyBQ4e/A1yIJdlBtqXxJ1KE/ituHRbNc4j4kL8Z\r\n"
	"/r6pvwnTI0PSx2Eqs048YdS92LT6qAv4flbNDxMn2uY7s4ycS4Q8w1JXnCeaAnYm\r\n"
	"WYI5wxO+bvRELR2Mcz5DmVnL8jRyml6l6582bSv5oufReFIbyPZbQWlXgYnpu6He\r\n"
	"GTc7E1zKYQGG/9+DQUl/1vQuCPqQwny0tQoX2w5tdYpdMdVm+zkLtbajzdTviJJa\r\n"
	"TWzL6lt5AoGBAN86+SVeJDcmQJcv4Eq6UhtRr4QGMiQMz0Sod6ettYxYzMgxtw28\r\n"
	"CIrgpozCc+UaZJLo7UxvC6an85r1b2nKPCLQFaggJ0H4Q0J/sZOhBIXaoBzWxveK\r\n"
	"nupceKdVxGsFi8CDy86DBfiyFivfBj+47BbaQzPBj7C4rK7UlLjab2rDAoGBAN2u\r\n"
	"AM2gchoFiu4v1HFL8D7lweEpi6ZnMJjnEu/dEgGQJFjwdpLnPbsj4c75odQ4Gz8g\r\n"
	"sw9lao9VVzbusoRE/JGI4aTdO0pATXyG7eG1Qu+5Yc1YGXcCrliA2xM9xx+d7f+s\r\n"
	"mPzN+WIEg5GJDYZDjAzHG5BNvi/FfM1C9dOtjv2dAoGAF0t5KmwbjWHBhcVqO4Ic\r\n"
	"BVvN3BIlc1ue2YRXEDlxY5b0r8N4XceMgKmW18OHApZxfl8uPDauWZLXOgl4uepv\r\n"
	"whZC3EuWrSyyICNhLY21Ah7hbIEBPF3L3ZsOwC+UErL+dXWLdB56Jgy3gZaBeW7b\r\n"
	"vDrEnocJbqCm7IukhXHOBK8CgYEAwqdHB0hqyNSzIOGY7v9abzB6pUdA3BZiQvEs\r\n"
	"3LjHVd4HPJ2x0N8CgrBIWOE0q8+0hSMmeE96WW/7jD3fPWwCR5zlXknxBQsfv0gP\r\n"
	"3BC5PR0Qdypz+d+9zfMf625kyit4T/hzwhDveZUzHnk1Cf+IG7Q+TOEnLnWAWBED\r\n"
	"ISOWmrUCgYAFEmRxgwAc/u+D6t0syCwAYh6POtscq9Y0i9GyWk89NzgC4NdwwbBH\r\n"
	"4AgahOxIxXx2gxJnq3yfkJfIjwf0s2DyP0kY2y6Ua1OeomPeY9mrIS4tCuDQ6LrE\r\n"
	"TB6l9VGoxJL4fyHnZb8L5gGvnB1bbD8cL6YPaDiOhcRseC9vBiEuVg==\r\n"
	"-----END RSA PRIVATE KEY-----\r\n";
#endif /* CONFIG_HW_RSA */

void print_laptime(void)
{
	struct timeval ct;
	gettimeofday(&ct, NULL);

	while (ct.tv_usec < rt.tv_usec) {
		ct.tv_usec += 1000000;
		ct.tv_sec-- ;
	}

	printf("%d sec %d msecs\n",
			ct.tv_sec - rt.tv_sec,
			(ct.tv_usec - rt.tv_usec)/1000);
	rt.tv_sec = ct.tv_sec;
	rt.tv_usec = ct.tv_usec;

}

static void http_get_cb(struct http_client_t *client, struct http_req_message *msg)
{
	struct http_keyvalue_list_t response_headers;
	const char *rsp_msg = "This is a root page";
	char contlen[6] = {0,};

	http_keyvalue_list_init(&response_headers);

	http_keyvalue_list_add(&response_headers, g_httpcontype, g_httpconhtml);
	sprintf(contlen, "%d", strlen(rsp_msg));
	http_keyvalue_list_add(&response_headers, g_httpcontsize, contlen);
	http_keyvalue_list_add(&response_headers, g_httpconnect, g_httpcnlost);

	printf(">get_root\n");
	http_send_response(client, 200, rsp_msg, &response_headers);
	http_keyvalue_list_release(&response_headers);
}

static void http_post_cb(struct http_client_t *client, struct http_req_message *msg)
{
	cJSON *tmp;
	static bool valid_info = false;

	if (msg->entity != NULL) {
		cJSON *request_json = cJSON_Parse(msg->entity);
		valid_info = true;
		if (request_json != NULL) {
			memset(&ap_info, 0, sizeof(struct ap_info_t));

			tmp = cJSON_GetObjectItem(request_json, "ssid");

			if (tmp != NULL) {
				printf("ssid %s\n", tmp->valuestring);
				memcpy(ap_info.ssid, tmp->valuestring, strlen(tmp->valuestring));
			} else {
				valid_info = false;
			}

			tmp = cJSON_GetObjectItem(request_json, "password");
			if (tmp != NULL) {
				printf("password %s\n", tmp->valuestring);
				memcpy(ap_info.pwd, tmp->valuestring, strlen(tmp->valuestring));
			} else {
				valid_info = false;
			}

			tmp = cJSON_GetObjectItem(request_json, "security");
			if (tmp != NULL) {
				printf("security %s\n", tmp->valuestring);
				if (wifi_securitymode_check(tmp->valuestring) != 0) {

					valid_info = false;
					printf("wrong argunets\n");
				} else {
					memcpy(ap_info.security, tmp->valuestring, strlen(tmp->valuestring));
				}
			} else {
				valid_info = false;
			}
		} else {
			valid_info = false;
		}

		cJSON_Delete(request_json);

		if (msg->encoding == HTTP_CHUNKED_ENCODING && msg->entity[0] != '\0') {
			return;
		}
	}

	http_send_response(client, 200, NULL, NULL);

	if (valid_info) {
		sem_post(&get_netinfo);
		valid_info = false;
	} else {
		printf("AP information is not valid\n");
	}
}

static void http_post_ctrl_cb(struct http_client_t *client, struct http_req_message *msg)
{
	cJSON *tmp;
	int stop_val = 0;

	if (msg->entity != NULL) {
		cJSON *request_json = cJSON_Parse(msg->entity);
		if (request_json != NULL) {
			printf(">>>> post /control \n");
			memset(&ap_info, 0, sizeof(struct ap_info_t));

			tmp = cJSON_GetObjectItem(request_json, "stop");
			if (tmp != NULL) {
				printf("stop %d \n", tmp->valueint);
				stop_val = tmp->valueint;
			} else {
				stop_val = 0;
			}
		}
		cJSON_Delete(request_json);
	}

	if (msg->encoding == HTTP_CHUNKED_ENCODING && msg->entity[0] != '\0') {
		g_stop_easysetup = stop_val;
		return;
	}

	http_send_response(client, 200, NULL, NULL);
	sem_post(&get_ctrl);
}

static int start_webserver(void)
{
	int https_port = 443;
	struct ssl_config_t ssl_config;

	https_server = http_server_init(https_port);
	if (https_server == NULL) {
		printf("Error: Cannot allocate server structure!!\n");
		return -1;
	}

#if defined(CONFIG_HW_RSA)
	int ret;

	see_init();

	/* Setup post key */
	/* THIS CODE SHOULD BE REMOVED AFTER USING SSS KEY AND CERT */
	if ((ret = see_setup_key(sss_da_rsa_ca, sizeof(sss_da_rsa_ca),
					SECURE_STORAGE_TYPE_KEY_RSA, EASY_CA_KEY_INDEX)) != 0) {
		printf("Error: see_setup_key ca 0x%x\n\n", ret);
		return -1;
	}

	if ((ret = see_setup_key(sss_da_rsa_dev, sizeof(sss_da_rsa_dev),
					SECURE_STORAGE_TYPE_KEY_RSA, EASY_DEV_KEY_INDEX)) != 0) {
		printf("Error: see_setup_key dev 0x%x\n\n", ret);
		return -1;
	}

	if ((ret = see_set_certificate(sss_ca_crt, sizeof(sss_ca_crt),
					EASY_CA_CERT_INDEX, CERT_PEM)) != 0) {
		printf("Error: set_cert fail %d\n", ret);
		return -1;
	}

	if ((ret = see_set_certificate(sss_dev_crt, sizeof(sss_dev_crt),
					EASY_DEV_CERT_INDEX, CERT_PEM)) != 0) {
		printf("Error: set_cert fail %d\n", ret);
		return -1;
	}

	ssl_config.ca_key_index    = EASY_CA_KEY_INDEX;
	ssl_config.dev_key_index   = EASY_DEV_KEY_INDEX;
	ssl_config.ca_cert_index   = EASY_CA_CERT_INDEX;
	ssl_config.dev_cert_index  = EASY_DEV_CERT_INDEX;
	ssl_config.auth_mode       = MBEDTLS_SSL_VERIFY_REQUIRED;
#else
	ssl_config.root_ca = (char *)ca_crt_rsa;
	ssl_config.root_ca_len = sizeof(ca_crt_rsa);
	ssl_config.dev_cert = (char *)srv_crt_rsa;
	ssl_config.dev_cert_len = sizeof(srv_crt_rsa);
	ssl_config.private_key = (char *)srv_key_rsa;
	ssl_config.private_key_len = sizeof(srv_key_rsa);
	ssl_config.auth_mode = MBEDTLS_SSL_VERIFY_REQUIRED;
#endif /* CONFIG_HW_RSA */

	if (http_tls_init(https_server, &ssl_config) != 0) {
		printf("ssl config Error\n");
		return -1;
	}

	http_server_register_cb(https_server, HTTP_METHOD_GET, root_uri, http_get_cb);
	http_server_register_cb(https_server, HTTP_METHOD_POST, network_uri, http_post_cb);
	http_server_register_cb(https_server, HTTP_METHOD_POST, control_uri, http_post_ctrl_cb);

	http_server_start(https_server);

	return 0;
}

static int stop_webserver(void)
{
	http_server_deregister_cb(https_server, HTTP_METHOD_GET, root_uri);
	http_server_deregister_cb(https_server, HTTP_METHOD_POST, network_uri);
	http_server_deregister_cb(https_server, HTTP_METHOD_POST, control_uri);
	http_server_stop(https_server);
	http_server_release(&https_server);

#if defined(CONFIG_HW_RSA)
	see_free();
#endif
	return 0;
}
/* function is used from multiple places so dont rely on local static vars in this file */
static void getSecurityModeString(slsi_security_config_t *sec, uint8_t count, char* modestring)
{
	memset(modestring, 0, MODE_STRING_MAX);
	char *pos = modestring;
	int x = 0;

	if (count == 0) {
		strncat(pos, "[NONE]", 6);
	} else {
		for (x = 0; x < count; x++) {
			strncat(pos, "[", 1);
			pos += 1;
			if (sec->secmode == SLSI_SEC_MODE_WEP) {
				strncat(pos, "WEP", 3);
				pos += 3;
			} else if (sec->secmode == SLSI_SEC_MODE_WEP_SHARED) {
				strncat(pos, "WEP_SHARED", 10);
				pos += 10;
			} else if (sec->secmode == (SLSI_SEC_MODE_WEP | SLSI_SEC_MODE_WEP_SHARED)) {
				strncat(pos, "WEP_UNKNOWN", 11);
				pos += 11;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA_TKIP) {
				strncat(pos, "WPA-PSK+TKIP", 12);
				pos += 12;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA_CCMP) {
				strncat(pos, "WPA-PSK+AES", 11);
				pos += 11;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA_MIXED) {
				strncat(pos, "WPA-PSK+AES+TKIP", 16);
				pos += 16;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA2_TKIP) {
				strncat(pos, "WPA2-PSK+TKIP", 13);
				pos += 13;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA2_CCMP) {
				strncat(pos, "WPA2-PSK+AES", 12);
				pos += 12;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA2_MIXED) {
				strncat(pos, "WPA2-PSK+AES+TKIP", 17);
				pos += 17;
			} else if (sec->secmode == SLSI_SEC_MODE_EAP) {
				strncat(pos, "WPA-EAP", 7);
				pos += 7;
			}

			strncat(pos, "]", 1);
			pos += 1;
			sec++; //go to next structure
		}
	}
}

static void print_scan_result(slsi_scan_info_t *list)
{
	int count = 0;
	slsi_scan_info_t *current_element = list;
	printf("Scan Result - networks:\n");
	printf("          %-20.20s %-8.8s %-40.40s %-6.6s %s\n", "BSSID", "RSSI", "SECURITY", "CH", "SSID");

	while (current_element) {
		char ssid[WPA_MAX_SSID_LEN];
		char modestring[MODE_STRING_MAX];
		count++;
		getSecurityModeString(current_element->sec_modes, current_element->num_sec_modes, modestring);
		printf_encode(ssid, WPA_MAX_SSID_LEN, current_element->ssid, current_element->ssid_len);
		printf("     %3.3d) %-20.20s %-8.8d %-40.40s %-6.6d %.32s \n",
				count, current_element->bssid, current_element->rssi, modestring,
				current_element->channel, ssid);
		current_element = current_element->next;
	}
}

static int8_t scan_result_handler(slsi_reason_t* reason)
{
	if (reason->reason_code != 0) {
		printf("Scan failed reason: %d, locally_generated: %d\n", reason->reason_code, reason->locally_generated);
	} else {
		slsi_scan_info_t *scan_list_head;
		WiFiGetScanResults(&scan_list_head);
		print_scan_result(scan_list_head);
		WiFiFreeScanResults(&scan_list_head);
		sem_post(&finish_scan);
	}

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int easysetup_main(int argc, char **argv)
#endif
{
	int easysetup_cnt = 0;
_START_:
	sem_init(&get_netinfo, 0, 0);
	sem_init(&finish_scan, 0, 0);
	sem_init(&get_ctrl, 0, 0);

	gettimeofday(&rt, NULL);
	printf("=======================================\n");
	printf("EASYSETUP start [%d] ::", easysetup_cnt);
	print_laptime();
	if (wifi_start_ap((uint8_t *)soft_ap_ssid, (char *)soft_ap_security, (char *)soft_ap_passphase, soft_ap_channel) != 0) {
		printf("Error start softap\n");
		return 0;
	}

	printf("Start SoftAP ::");
	print_laptime();

#ifdef CONFIG_SLSI_WIFI_SANITY
	dhcpserver_start();
	printf("Start DHCP Server Start ::");
	print_laptime();
#endif

	if (start_webserver() != 0) {
		printf("webserver start fail\n");
	}

	printf("Start Webserver ::");
	print_laptime();

	sem_wait(&get_netinfo);
	printf("Get AP information ::");
	print_laptime();
	WiFiRegisterScanCallback(&scan_result_handler);
	WiFiScanNetwork();
	sem_wait(&finish_scan);
	printf("Scan finish ::");
	print_laptime();

	stop_webserver();
	printf("Stop Webserver ::");
	print_laptime();

#ifdef CONFIG_SLSI_WIFI_SANITY
	stop_dhcp(SLSI_WIFI_SOFT_AP_IF);
	printf("Stop DHCP Server ::");
	print_laptime();
#endif

	if (wifi_start_station() < 0) {
		printf("start station error \n");
		sem_destroy(&get_netinfo);
		sem_destroy(&finish_scan);
		return 0;
	}

	while (wifi_join((uint8_t *)ap_info.ssid, ap_info.security, ap_info.pwd) != 0) {
		printf("Retry to Join\n");
		sleep(1);
	}

	while (get_wifiState() == WIFI_DISCONNECTED) {
		printf("wait connecting to AP\n");
		usleep(300000);
	}

	printf("AP join ::");
	print_laptime();

#ifdef CONFIG_SLSI_WIFI_SANITY
	while (dhcpc_start() != 0) {
		printf("Get IP address Fail\n");
	}
#endif

	printf("DHCP Client Start ::");
	print_laptime();
	g_stop_easysetup = 0;
	if (start_webserver() != 0) {
		printf("webserver start fail\n");
		return -1;
	}

	printf("Start Webserver ::\n");
	print_laptime();

	sem_wait(&get_ctrl);

	sleep(1);

	printf("Stop\n");
	sem_destroy(&get_netinfo);
	sem_destroy(&finish_scan);
	sem_destroy(&get_ctrl);
	stop_webserver();
#ifdef CONFIG_SLSI_WIFI_SANITY
	stop_dhcp(SLSI_WIFI_STATION_IF);
#endif

	easysetup_cnt++;

	printf("=======================================\n");
	if (g_stop_easysetup == 2) {
		goto _START_;
	}

	if (WiFiStop() != 0) {
		return -1;
	}

	return 0;
}
