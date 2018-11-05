/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @file artik_demo.c
 */

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <net/if.h>

#include <apps/shell/tash.h>
#include <protocols/dhcpc.h>
#include <netutils/netlib.h>
#include <protocols/webclient.h>
#include <dm/dm_error.h>
#include <dm/dm_connectivity.h>
#include <slsi_wifi/slsi_wifi_api.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#if 0
#define AP_SSID "Gorani"
#define AP_SECURITY "wpa2_aes"
#define AP_PASS "roadkill1#"

#define ARTIK_CLOUD_BEARER "f10e5a3c94c140e8865393393808f688"
#define ARTIK_CLOUD_DID "4a9aa1ed4d3f4d29869f146d75d93e13"
#endif

static const char c_ca_crt_rsa[] = "-----BEGIN CERTIFICATE-----\r\n"
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

static const char c_srv_crt_rsa[] = "-----BEGIN CERTIFICATE-----\r\n"
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

static const char c_srv_key_rsa[] = "-----BEGIN RSA PRIVATE KEY-----\r\n"
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

static const char *tlsname = "araweb_tls_client";

static const char g_http10[] = "HTTP/1.0";
static const char g_http11[] = "HTTP/1.1";
#ifdef CONFIG_WEBCLIENT_GETMIMETYPE
static const char g_httpcontenttype[] = "content-type: ";
#endif
static const char g_httplocation[] = "location: ";

static const char g_http200[] = "200 ";
static const char g_http301[] = "301 ";
static const char g_http302[] = "302 ";
static const char g_http404[] = "404 ";
static const char g_http500[] = "500 ";

#define WEBCLIENT_STATE_STATUSLINE 0
#define WEBCLIENT_STATE_HEADERS    1
#define WEBCLIENT_STATE_DATA       2
#define WEBCLIENT_STATE_CLOSE      3

#define HTTPSTATUS_NONE            0
#define HTTPSTATUS_OK              1
#define HTTPSTATUS_MOVED           2
#define HTTPSTATUS_ERROR           3

#define ISO_nl                     0x0a
#define ISO_cr                     0x0d
#define ISO_space                  0x20

#define CONFIG_WEBCLIENT_MAXHTTPLINE 200
#define CONFIG_WEBCLIENT_MAXHOSTNAME 40
#define CONFIG_WEBCLIENT_MAXFILENAME 100

struct wget_s {
	/* Internal status */

	uint8_t state;
	uint8_t httpstatus;

	/* The port number to use in the connection */
	uint16_t port;

	/*
	 * These describe the just-received buffer of data
	 */
	FAR
	/* user-provided buffer */
	char *buffer;
	/* Length of the user provided buffer */
	int buflen;
	/* Offset to the beginning of interesting data */
	int offset;
	/* Offset+1 to the last valid byte of data in the buffer */
	int datend;

	/*
	 * Buffer HTTP header data and parse line at a time
	 */

	char line[CONFIG_WEBCLIENT_MAXHTTPLINE];
	int ndx;

#ifdef CONFIG_WEBCLIENT_GETMIMETYPE
	char mimetype[CONFIG_WEBCLIENT_MAXMIMESIZE];
#endif
	char hostname[CONFIG_WEBCLIENT_MAXHOSTNAME];
	char filename[CONFIG_WEBCLIENT_MAXFILENAME];
};

/****************************************************************************
 * External Function Prototype
 ****************************************************************************/
extern int artik_demo_main(int argc, char *argv[]);
static conn_cb linkUpEvent(void);
static int wifiAutoConnectInit(void);
static int wifiAutoConnect(void);
static void wget_tls_debug(void *ctx, int level, const char *file, int line,
		const char *str);
static void wget_tls_release(struct http_client_tls_t *client);
static int webclient_tls_init(struct http_client_tls_t *client,
		struct http_client_ssl_config_t *ssl_config);
static void wget_tls_ssl_release(struct http_client_tls_t *client);
static int wget_tls_handshake(struct http_client_tls_t *client);
static int wget_parsestatus(struct wget_s *ws);
static int wget_parseheaders(struct wget_s *ws);
int send_data_to_artik(int *data);
int artik_demo_run(int argc, char *argv[]);

/****************************************************************************
 * Private Variables
 ****************************************************************************/
/*
 * Set Up Type sending to Artik Cloud
 */
const int field_count = 3;
char *field_name[] = { "rssi", "tx_power", "channel" };

/*
 * Check Variable whether Network Connected
 */
static int isConnected = 0;
static char NET_DEVNAME[10];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int app_dhcp_main(void)
{
	int ret;
	struct in_addr ip_check;

	ret = dhcp_client_start(NET_DEVNAME);
	if (ret != OK) {
		return -1;
	}

	ret = netlib_get_ipv4addr(NET_DEVNAME, &ip_check)
	if (ret != OK) {
		return -1;
	}
	printf("IP address get %s ----\n", inet_ntoa(ip_check));
	return 1;
}

static conn_cb linkUpEvent()
{
	isConnected = 1;
	return 0;
}

/*
 * Initiate WiFi auto connection
 */
static int wifiAutoConnectInit()
{
	int8_t ret;
	uint8_t result;

	isConnected = 0;
	/*
	 * Register Link up Callback to check connection
	 */
	dm_conn_register_linkup_cb((conn_cb) linkUpEvent);
	/*
	 * Get Interface Name
	 */
	dm_conn_get_interface(NET_DEVNAME);

	/*
	 * Check WifiIsConnected or not
	 */
	if (WiFiIsConnected(&result, NULL) != SLSI_STATUS_SUCCESS) {
		/* Error : failed to WifiIsConnected */
		printf("failed to WifiIsConnected\n");
		return -1;
	}

	if (result > 0) {
		printf("Wi-Fi status - Connected : %d\n", result);
		isConnected = 1;
		return 1;
	}

	ret = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (ret == SLSI_STATUS_SUCCESS) {
		printf("[AutoConnect]STA mode started\n");
		ret = WiFiNetworkJoin((uint8_t*)CONFIG_AP_SSID, strlen(CONFIG_AP_SSID), NULL,
				(const slsi_security_config_t *)getSecurityConfig(
						CONFIG_AP_SECURITY, CONFIG_AP_PASS, SLSI_WIFI_STATION_IF));
		sleep(1);
		if (ret == SLSI_STATUS_SUCCESS) {
			printf("[AutoConnect]Start to Join with SSID %s\n", CONFIG_AP_SSID);
			return 1;
		} else {
			/* Error : [AutoConnect]Failed to join the network */
			printf("[AutoConnect]Failed to join the network.[%d]\n", ret);
		}
		return -1;
	} else {
		/* Error : [AutoConnect]Failed to start STA mode */
		printf("[AutoConnect]Failed to start STA mode\n");
	}
	return -1;
}

/*
 * Connect WiFi automatically
 */
static int wifiAutoConnect()
{
	int ret;
	if ((ret = wifiAutoConnectInit()) == 1) {
		int waitTime = 10;
		while (waitTime--) {
			if (isConnected == 1) {
				printf("[AutoConnect]WiFi Connected!\n");
				int dhcp_ret = app_dhcp_main();
				if (dhcp_ret == -1) {
					/* Error : [AutoConnect]Timeout fail to get ip address */
					printf("Timeout fail to get ip address\n");
					return dhcp_ret;
				} else if (dhcp_ret == 0) {
					/* Error : No network interface */
					printf("No network interface\n");
					return dhcp_ret;
				}
				printf("DHCP done\n");
				break;
			}
			sleep(1);
		}
		if (waitTime <= 0) {
			printf("[AutoConnect]WiFi is not working. Test Canceled\n");
			return -1;
		}
	}
	return 1;
}

static void wget_tls_debug(void *ctx, int level, const char *file, int line,
		const char *str)
{
	ndbg("%s:%04d: %s", file, line, str);
}

static void wget_tls_release(struct http_client_tls_t *client)
{
	if (client == NULL)
		return;

	mbedtls_x509_crt_free(&(client->tls_clicert));
	mbedtls_pk_free(&(client->tls_pkey));
	mbedtls_ssl_config_free(&(client->tls_conf));
	mbedtls_ctr_drbg_free(&(client->tls_ctr_drbg));
	mbedtls_entropy_free(&(client->tls_entropy));
	mbedtls_ssl_session_free(&(client->tls_session));
}

static int webclient_tls_init(struct http_client_tls_t *client,
		struct http_client_ssl_config_t *ssl_config)
{
	int result = 0;

	mbedtls_ssl_config_init(&(client->tls_conf));
	mbedtls_x509_crt_init(&(client->tls_clicert));
	mbedtls_pk_init(&(client->tls_pkey));
	mbedtls_entropy_init(&(client->tls_entropy));
	mbedtls_ctr_drbg_init(&(client->tls_ctr_drbg));
	mbedtls_ssl_session_init(&(client->tls_session));

	/*
	 * 0. Initialize the RNG
	 */

	ndbg("  . Seeding the random number generator...");

	if ((result = mbedtls_ctr_drbg_seed(&(client->tls_ctr_drbg),
			mbedtls_entropy_func, &(client->tls_entropy),
			(const unsigned char *)tlsname, strlen(tlsname))) != 0) {
		/* Error : mbedtls_ctr_drbg_seed returned */
		ndbg("Error: mbedtls_ctr_drbg_seed returned %d\n", result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

	/*
	 * 1. Load the certificates and private key
	 */

	ndbg("  . Loading the client cert, and key...");

	result = mbedtls_x509_crt_parse(&(client->tls_clicert),
			(const unsigned char *)ssl_config->dev_cert,
			ssl_config->dev_cert_len);
	if (result) {
		/* Error : srv_cert parse fail */
		ndbg("Error: srv_cert parse fail, return %d\n", result);
		goto TLS_INIT_EXIT;
	}

	result = mbedtls_pk_parse_key(&(client->tls_pkey),
			(const unsigned char *)ssl_config->private_key,
			ssl_config->private_key_len, NULL, 0);
	if (result) {
		/* Error : srv_cert parse fail */
		ndbg("Error: srv_key parse fail, return %d\n", result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

	/*
	 * 2. Load the CA certificate
	 */

	ndbg("  . Loading the CA cert...");

	result = mbedtls_x509_crt_parse(&(client->tls_clicert),
			(const unsigned char *)ssl_config->root_ca,
			ssl_config->root_ca_len);
	if (result) {
		/* Error : CA_cert parse fail */
		ndbg("Error: CA_cert parse fail, return %d\n", result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

	/*
	 * 3. Setup ssl stuff
	 */

	ndbg("  . Setting up the SSL data...");

	if ((result = mbedtls_ssl_config_defaults(&(client->tls_conf),
	MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
	MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		/* Error : mbedtls_ssl_config_defaults returned */
		ndbg("Error: mbedtls_ssl_config_defaults returned %d\n", result);
		goto TLS_INIT_EXIT;
	}

	mbedtls_ssl_conf_rng(&(client->tls_conf), mbedtls_ctr_drbg_random,
			&(client->tls_ctr_drbg));
	mbedtls_ssl_conf_dbg(&(client->tls_conf), wget_tls_debug, stdout);
	mbedtls_ssl_conf_ca_chain(&(client->tls_conf), client->tls_clicert.next,
	NULL);

	if ((result = mbedtls_ssl_conf_own_cert(&(client->tls_conf),
			&(client->tls_clicert), &(client->tls_pkey))) != 0) {
		/* Error : mbedtls_ssl_conf_own_cert returned */
		ndbg("Error: mbedtls_ssl_conf_own_cert returned %d\n", result);
		goto TLS_INIT_EXIT;
	}
	ndbg("Ok\n");

	return 0;
	TLS_INIT_EXIT: wget_tls_release(client);
	return result;
}

static void wget_tls_ssl_release(struct http_client_tls_t *client)
{
	if (client == NULL)
		return;

	mbedtls_net_free(&(client->tls_client_fd));
	mbedtls_ssl_session_reset(&(client->tls_ssl));
	mbedtls_ssl_free(&(client->tls_ssl));
}

static int wget_tls_handshake(struct http_client_tls_t *client)
{
	int result = 0;

	/*
	 * Client TLS Init start
	 */
	ndbg("Client TLS Init start..");

	mbedtls_ssl_conf_authmode(&(client->tls_conf),
	MBEDTLS_SSL_VERIFY_OPTIONAL);

	/*
	 * Init TLS
	 */
	mbedtls_net_init(&(client->tls_client_fd));
	mbedtls_ssl_init(&(client->tls_ssl));

	client->tls_client_fd.fd = client->client_fd;

	mbedtls_net_set_block(&(client->tls_client_fd));

	ndbg("Ok\n");

	if ((result = mbedtls_ssl_setup(&(client->tls_ssl), &(client->tls_conf))) != 0) {
		/* Error: mbedtls_ssl_setup returned */
		ndbg("Error: mbedtls_ssl_setup returned %d\n", result);
		goto HANDSHAKE_FAIL;
	}

	if ((result = mbedtls_ssl_set_hostname(&(client->tls_ssl), "localhost")) != 0) {
		/* Error: mbedtls_hostname fail */
		ndbg("Error: mbedtls_hostname fail %d\n", result);
		goto HANDSHAKE_FAIL;
	}
	mbedtls_ssl_set_bio(&(client->tls_ssl), &(client->tls_client_fd),
			mbedtls_net_send, mbedtls_net_recv, NULL);

	/*
	 * Handshake
	 */
	ndbg("  . Performing the SSL/TLS handshake...\n");

	while ((result = mbedtls_ssl_handshake(&(client->tls_ssl))) != 0) {
		if (result != MBEDTLS_ERR_SSL_WANT_READ	&& result != MBEDTLS_ERR_SSL_WANT_WRITE) {
			/* Error : mbedtls_ssl_handshake returned */
			ndbg("Error: mbedtls_ssl_handshake returned %d\n", result);
			goto HANDSHAKE_FAIL;
		}
	}

	return 0;

	/*
	 * Handle Exception Error
	 */
HANDSHAKE_FAIL:
	wget_tls_release(client);
	wget_tls_ssl_release(client);
	return result;
}

static int wget_parsestatus(struct wget_s *ws)
{
	int offset, ndx;
	char *dest;

	offset = ws->offset;
	ndx = ws->ndx;

	while (offset < ws->datend) {
		ws->line[ndx] = ws->buffer[offset];
		if (ws->line[ndx] == ISO_nl) {
			ws->line[ndx] = '\0';
			if ((strncmp(ws->line, g_http10, strlen(g_http10)) == 0) || (strncmp(ws->line, g_http11, strlen(g_http11)) == 0)) {
				dest = &(ws->line[9]);
				ws->httpstatus = HTTPSTATUS_NONE;

				/*
				 * Check for 200 OK
				 */

				if (strncmp(dest, g_http200, strlen(g_http200)) == 0) {
					ws->httpstatus = HTTPSTATUS_OK;
				}

				/*
				 * Check for 301 Moved permanently or 302 Found. Location: header line
				 * will contain the new location.
				 */

				else if (strncmp(dest, g_http301, strlen(g_http301)) == 0 || strncmp(dest, g_http302, strlen(g_http302)) == 0
						|| strncmp(dest, g_http404, strlen(g_http404)) == 0 || strncmp(dest, g_http500, strlen(g_http500)) == 0) {
					ws->state = WEBCLIENT_STATE_DATA;
					break;
				}
			} else {
				return -ECONNABORTED;
			}

			/*
			 * We're done parsing the status line, so start parsing the HTTP headers.
			 */

			ws->state = WEBCLIENT_STATE_HEADERS;
			break;
		} else {
			offset++;
			ndx++;
		}
	}

	ws->offset = offset;
	ws->ndx = ndx;
	return 0;
}

static int wget_parseheaders(struct wget_s *ws)
{
	int offset;
	int ndx;

	offset = ws->offset;
	ndx = ws->ndx;

	while (offset < ws->datend) {
		ws->line[ndx] = ws->buffer[offset];
		if (ws->line[ndx] == ISO_nl) {
			/*
			 * We have an entire HTTP header line in s.line, so
			 * we parse it.
			 */

			if (ndx > 0) {
				/*
				 * Should always be true
				 */
				if (ws->line[0] == ISO_cr) {
					/*
					 * This was the last header line (i.e., and empty "\r\n"), so
					 * we are done with the headers and proceed with the actual
					 * data.
					 */

					ws->state = WEBCLIENT_STATE_DATA;
					goto exit;
				}

				/*
				 * Truncate the trailing \r\n
				 */

				ws->line[ndx - 1] = '\0';

				/*
				 * Check for specific HTTP header fields.
				 */

#ifdef CONFIG_WEBCLIENT_GETMIMETYPE
				if (strncasecmp(ws->line, g_httpcontenttype, strlen(g_httpcontenttype)) == 0) {
					/*
					 * Found Content-type field.
					 */

					char *dest = strchr(ws->line, ';');
					if (dest != NULL) {
						*dest = 0;
					}

					strncpy(ws->mimetype, ws->line + strlen(g_httpcontenttype),	sizeof(ws->mimetype));
				} else
#endif
				if (strncasecmp(ws->line, g_httplocation, strlen(g_httplocation)) == 0) {
					/*
					 * Parse the new HTTP host and filename from the URL.  Note that
					 * the return value is ignored.  In the event of failure, we
					 * retain the current location.
					 */

					(void) netlib_parsehttpurl(ws->line + strlen(g_httplocation), &ws->port, ws->hostname, CONFIG_WEBCLIENT_MAXHOSTNAME, ws->filename, CONFIG_WEBCLIENT_MAXFILENAME);
					nvdbg("New hostname='%s' filename='%s'\n", ws->hostname, ws->filename);
				}
			}

			/*
			 * We're done parsing this line, so we reset the index to the start
			 * of the next line.
			 */

			ndx = 0;
		} else {
			ndx++;
		}

		offset++;
	}

	exit: ws->offset = ++offset;
	ws->ndx = ndx;
	return 0;
}

/*
 * Send date to Artik Cloud
 */
int send_data_to_artik(int *data)
{

	int ret, sockfd, i;
	bool redirected;
	struct sockaddr_in server;
	struct wget_s ws;
	struct http_client_tls_t *client_tls = (struct http_client_tls_t *)malloc(sizeof(struct http_client_tls_t));
	struct http_client_ssl_config_t ssl_config;

	/*
	 * Set Up ssl config
	 */
	ssl_config.root_ca = (char *)c_ca_crt_rsa;
	ssl_config.root_ca_len = sizeof(c_ca_crt_rsa);
	ssl_config.dev_cert = (char *)c_srv_crt_rsa;
	ssl_config.dev_cert_len = sizeof(c_srv_crt_rsa);
	ssl_config.private_key = (char *)c_srv_key_rsa;
	ssl_config.private_key_len = sizeof(c_srv_key_rsa);

	char r_message[1024], w_message[1024];
	char body[1024];
	char body_main[512];
	char data_string[256];
	char req[300], body1[50], body2[150];
	int req_len, body_len;

	ws.buffer = r_message;
	ws.buflen = 1024;

	do {
		ws.httpstatus = HTTPSTATUS_NONE;
		ws.offset = 0;
		ws.datend = 0;
		ws.ndx = 0;

		/*
		 * Try to open socket
		 */
		sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockfd < 0) {
			/* ERROR: socket failed.  It will set the errno appropriately */
			ndbg("ERROR: socket failed.\n");
			free(client_tls);
			return ERROR;
		}

		/*
		 * Set up Server Addr
		 */
		server.sin_family = AF_INET;
		server.sin_port = htons(443);
		server.sin_addr.s_addr = inet_addr("52.86.204.150");

		/*
		 * Try to connect server
		 */
		ret = connect(sockfd, (struct sockaddr *)&server,
				sizeof(struct sockaddr_in));
		if (ret < 0) {
			/* ERROR: Connection Failed */
			ndbg("ERROR: connect failed: %d\n", ret);
			goto error_fd_tls;
		}

		client_tls->client_fd = sockfd;

		if (webclient_tls_init(client_tls, &ssl_config)) {
			/* ERROR: Fail to client tls init */
			ndbg("Fail to client tls init\n");
			goto error_fd_tls;
		} else {
			if (wget_tls_handshake(client_tls)) {
				/* ERROR: Fail to client tls handshake */
				ndbg("Fail to client tls handshake\n");
				goto error_fd_tls;
			}
		}

		/*
		 * Assemble TLS header, body, footer
		 */
		sprintf(req, "POST /v1.1/messages HTTP/1.1\r\nhost:api.artik.cloud\r\nContent-Type: application/json\r\nAuthorization: Bearer %s\r\nContent-Length:", CONFIG_ARTIK_CLOUD_BEARER);
		sprintf(body1, "{\"data\": {");
		sprintf(body2, "},\"sdid\": \"%s\",\"type\": \"message\"}", CONFIG_ARTIK_CLOUD_DID);
		body_main[0] = '\0';

		for (i = 0; i < field_count; i++) {
			if (i == field_count - 1)
				sprintf(data_string, "\"%s\":%d", field_name[i], data[i]);
			else
				sprintf(data_string, "\"%s\":%d,", field_name[i], data[i]);
			strcat(body_main, data_string);
		}
		/*
		 * Checking the body_main to send
		 */
		ndbg("sending data : %s\n", body_main);
		sprintf(body, "%s%s%s", body1, body_main, body2);

		/*
		 * Get Contents Length
		 */
		req_len = strlen(req);
		body_len = strlen(body);

		sprintf(w_message, "%s%d\r\n\r\n%s", req, body_len, body);

		ndbg("send_message : %s\n", w_message);

		ret = mbedtls_ssl_write(&(client_tls->tls_ssl), (unsigned char*)w_message, strlen(w_message));

		if (ret < 0) {
			/* Error Occur */
			ndbg("ERROR: send failed: %d\n", ret);
			goto errout;
		} else {
			/* Send Success */
			ndbg("SEND SUCCESS: send %d bytes\n", ret);
		}

		ws.state = WEBCLIENT_STATE_STATUSLINE;
		redirected = false;

		for (;;) {
			ws.datend = mbedtls_ssl_read(&(client_tls->tls_ssl), (unsigned char*)ws.buffer, ws.buflen);

			if (ws.datend == MBEDTLS_ERR_SSL_CONN_EOF || ws.datend == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
				/* Disconnected */
				nvdbg("Connection lost\n");
				close(sockfd);
				break;
			}

			if (ws.datend < 0) {
				/* Cannot Receive Meg */
				ndbg("ERROR: recv failed: %d\n", ws.datend);
				ret = ws.datend;
				goto errout;
			} else if (ws.datend == 0) {
				/* Disconnected */
				nvdbg("Connection lost\n");
				close(sockfd);
				break;
			}
			/*
			 * Handle initial parsing of the status line
			 */

			ws.offset = 0;
			if (ws.state == WEBCLIENT_STATE_STATUSLINE) {
				ret = wget_parsestatus(&ws);
				if (ret < 0) {
					goto errout;
				}
			}

			/*
			 * Parse the HTTP data
			 */

			if (ws.state == WEBCLIENT_STATE_HEADERS) {
				ret = wget_parseheaders(&ws);
				if (ret < 0) {
					goto errout;
				}
			}

			/*
			 * Dispose of the data payload
			 */

			if (ws.state == WEBCLIENT_STATE_DATA) {
				if (ws.httpstatus != HTTPSTATUS_MOVED) {
					/* Let the client decide what to do with the received file */
					ndbg("Received : %s\n", ws.buffer);
				} else {
					redirected = true;
					close(sockfd);
					break;
				}
			}
		}

	} while (redirected);

	/* Clear Connection */
	wget_tls_release(client_tls);
	wget_tls_ssl_release(client_tls);

	/* Release Memory */
	free(client_tls);

	return 0;

	/* Release memory and close socket if error */
error_fd_tls:
	close(sockfd);
	free(client_tls);

	return ERROR;

	/* Handle Exception */
errout:
	close(sockfd);
	wget_tls_release(client_tls);
	wget_tls_ssl_release(client_tls);

	free(client_tls);

	return 0;
}

int artik_demo_run(int argc, char *argv[])
{
	int ret;
	int field_value[field_count];

	/*
	 * Initialize Artik Network
	 */
	wifiAutoConnect();

	while (1) {
		printf("Sending ... ");
		/*
		 * Get Rssi of network using dm_conn_get_rssi api
		 */
		ret = dm_conn_get_rssi(&field_value[0]);
		printf("%s[%d] : %d / ", field_name[0], ret, field_value[0]);
		/*
		 * Get Tx Power of network using dm_conn_get_tx_power api
		 */
		ret = dm_conn_get_tx_power(&field_value[1]);
		printf("%s[%d] : %d / ", field_name[1], ret, field_value[1]);
		/*
		 * Get Channel of network using dm_conn_get_channel api
		 */
		ret = dm_conn_get_channel(&field_value[2]);
		printf("%s[%d] : %d\n", field_name[2], ret, field_value[2]);

		/*
		 * Sending Data to Artik Cloud
		 */
		send_data_to_artik(field_value);

		sleep(3);
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int artik_demo_main(int argc, char *argv[])
#endif
{
	/*
	 *
	 * This App is a Demo for using DM APIs.
	 *
	 * It tries connecting to Artik Cloud(https://artik.cloud)
	 * and sends data which are get by DM APIs.
	 * The Connection will use tls to send messages or actions.
	 *
	 * Before Start Application, some values have to be set up.
	 *
	 * - AP_SSID : the ssid of ap.
	 *
	 * - AP_PASS : the password of ap.
	 *
	 * - AP_SECURITY : the type of security of ap.
	 *					this is related to wifi driver.
	 *					artik uses 'open', 'wep', 'wep_shared', 'wpa2_tkip', 'wpa2_aes',
     *					'wpa2_mixed', 'wpa_aes', 'wpa_tkip', 'wpa_mixed'
     *
     * - ARTIK_CLOUD_BEARER : the User token of Artik Cloud.
     *							After join to Artik Cloud, user can get it.
     *
     * - ARTIK_CLOUD_DID : the Device ID which is using in Artik Cloud.
     *						Server distinguishes each device by this value.
     *
     *
     *
     * [How To Get ARTIK_Cloud Info]
     *
     * 1. Sign Up in 'https://artik.cloud/'
     *
     * 2. To Set Up data type visit
     *    'https://developer.artik.cloud/dashboard/devicetypes/'
     *
     * 3. Input Device display name, unique name
     *
     * 4. Click New Manifest.
     *
     * 5. You can set any device field, but this is an example for artik demo.
     * 5-1.  Field Name : rssi		Type : double
     * 5-2.  Field Name : tx_power	Type : double
     * 5-3.  Field Name : channel	Type : double
     *
     * 6. Click 'NEXT: DEVICE ACTIONS' button.
     *
     * 7. To skip this page, click 'NEXT: ACTIVATE MANIFEST'
     *
     * 8. Click 'ACTIVATE MANIFEST'
     *
     * 9. To get user id, visit
     *    'https://developer.artik.cloud/api-console/'.
     *
     * 10. Click 'Get Current User Profile'
     *
     * 11. Click 'TRY IT!'
     *
     * 12. Now, you can get '"Authorization": "Bearer ~~~~~~~~~~"'
     *
     * 13. After the word 'Bearer', it is the id of users.
     *
     * 14. Set ARTIK_CLOUD_BEARER, ARTIK_CLOUD_DID in 'artik_demo.c'
     *
     * 15. build and run tinyara.
     *
     * 16. TASH>> artik_demo
     *
     * 17. Demo App will connect wifi automatically and try to send the data of connectivity.
     *
     *
     * [How To Run]
     *
     * 1. tinyara/os$ make menuconfig
     *  (use base configuration with 'sidk_s5jt200/sidk_tash_wlan')
     *
     * 2. External Functions -> DM
     *
     * 3. Application Configuration -> Examples -> Artik API Demo Example
     *
     * 4. Set up AP's SSID, PASSWORD, SECURITY
     *
     * 5. Set up Artik Cloud's bearer, device id
     *
     * 6. Build tinyara.
     *
     * 7. Install tinyara.
     *
     * 8. TASH>> artik_demo
     *
     */

	int pid;

	/*
	 * Create Task For async job
	 */
	pid = task_create("artik_demo_run", SCHED_PRIORITY_DEFAULT, 16384, artik_demo_run, argv);
	if (pid < 0) {
		/*
		 * Error : Can't Create task
		 */
		printf("Artik Demo is not started, err = %d\n", pid);
	}

	return 0;
}

