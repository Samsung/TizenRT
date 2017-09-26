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
 * @file mqtt_client_sub.c
 * @brief the program for testing mqtt subscriber
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>

#include <network/mqtt/mqtt_api.h>

#if defined(CONFIG_NETUTILS_MQTT_SECURITY)
#include "mbedtls/x509_crt.h"
#include "mbedtls/pem.h"
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define MQTT_CLIENT_SUB_COMMAND_NAME	"mqtt_sub"
#define MQTT_ID_MAX_LENGTH				23

#define MQTT_SUB_STACK_SIZE				20480
#define MQTT_SUB_SCHED_PRI				100
#define MQTT_SUB_SCHED_POLICY			SCHED_RR

#define MQTT_SUB_DEBUG_PRINT(client_handle, ...) \
		do { \
			if (client_handle && (client_handle)->config && (client_handle)->config->debug) \
				printf(__VA_ARGS__); \
		} while (0);

/****************************************************************************
 * Structure
 ****************************************************************************/
struct mqtt_sub_input {
	int argc;
	char **argv;
};

/****************************************************************************
 * Enumeration
 ****************************************************************************/
typedef enum {
	CHECK_OPTION_RESULT_CHECKED_OK,
	CHECK_OPTION_RESULT_CHECKED_ERROR,
	CHECK_OPTION_RESULT_NOTHING_TODO,
} check_option_result_e;

/****************************************************************************
 * Function Prototype
 ****************************************************************************/
void mqtt_set_srand(void);
char *mqtt_generate_client_id(const char *id_base);
const unsigned char *mqtt_get_ca_certificate(void);
const unsigned char *mqtt_get_client_certificate(void);
const unsigned char *mqtt_get_client_key(void);
int mqtt_get_ca_certificate_size(void);
int mqtt_get_client_certificate_size(void);
int mqtt_get_client_key_size(void);

/****************************************************************************
 * Global Valiables
 ****************************************************************************/
static mqtt_client_t *g_mqtt_client_handle = NULL;
static mqtt_client_config_t g_mqtt_client_config;
static mqtt_msg_t g_subscribe_msg;
#if defined(CONFIG_NETUTILS_MQTT_SECURITY)
static mqtt_tls_param_t g_tls;
#endif

static char *g_id;
static char *g_host_addr;
static int g_port;
static bool g_clean_session;
static char *g_topic;
static int g_qos;
static char *g_username;
static char *g_password;
static int g_keepalive;
static int g_protocol_version;
static int g_stop;
static char *g_sub_topic;
static char *g_unsub_topic;
static bool g_debug;

static const char mqtt_ca_crt_rsa[] =
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

static const char mqtt_cli_crt_rsa[] =
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
	"zhuYwjVuX6JHG0c=\r\n" "-----END CERTIFICATE-----\r\n";

static const char mqtt_cli_key_rsa[] =
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

/****************************************************************************
 * Static Functions
 ****************************************************************************/
static void my_connect_callback(void *client, int result)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)client;
	mqtt_msg_t *mqtt_msg = NULL;

	if (mqtt_client == NULL || mqtt_client->config == NULL) {
		fprintf(stderr, "Error: >>> connect callback: %s is NULL.\n", mqtt_client == NULL ? "mqtt_client" : "mqtt_client->config");
		return;
	}

	if (result == MQTT_CONN_ACCEPTED) {

		MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> connect callback: client_id=%s, connect success!\n", mqtt_client->config->client_id);

		if (mqtt_client->config->user_data) {
			mqtt_msg = (mqtt_msg_t *)mqtt_client->config->user_data;
			if (mqtt_subscribe(mqtt_client, mqtt_msg->topic, mqtt_msg->qos) != 0) {
				fprintf(stderr, "Error: mqtt_subscribe() failed.\n");
			}
		} else {
			fprintf(stderr, "Error: mqtt_client is NULL.\n");
		}
	} else {
		char reason_str[40];
		switch (result) {
		case MQTT_CONN_REFUSED_UNACCEPTABLE_PROTOCOL_VER:
			snprintf(reason_str, sizeof(reason_str), "unacceptable protocol version");
			break;
		case MQTT_CONN_REFUSED_ID_REJECTED:
			snprintf(reason_str, sizeof(reason_str), "identifier rejected");
			break;
		case MQTT_CONN_REFUSED_BAD_USER_NAME_OR_PASSWORD:
			snprintf(reason_str, sizeof(reason_str), "bad user name or password");
			break;
		case MQTT_CONN_REFUSED_NOT_AUTHORIZED:
			snprintf(reason_str, sizeof(reason_str), "not authorized");
			break;
		default:
			snprintf(reason_str, sizeof(reason_str), "unknown");
			break;
		}

		MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> connect callback: client_id=%s, connect failed (reason: %s)\n", mqtt_client->config->client_id, reason_str);
	}
}

static void my_disconnect_callback(void *client, int result)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)client;

	if (mqtt_client == NULL || mqtt_client->config == NULL) {
		fprintf(stderr, "Error: >>> disconnect callback: %s is NULL.\n", mqtt_client == NULL ? "mqtt_client" : "mqtt_client->config");
		return;
	}

	if (result == 0) {
		MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> disconnect callback: client_id=%s, disconnected by mqtt_disconnect()\n", mqtt_client->config->client_id);
	} else {
		MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> disconnect callback: client_id=%s, disconnected by other reason\n", mqtt_client->config->client_id);
	}
}

static void my_message_callback(void *client, mqtt_msg_t *msg)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)client;

	if (mqtt_client == NULL || mqtt_client->config == NULL) {
		fprintf(stderr, "Error: >>> message callback: %s is NULL.\n", mqtt_client == NULL ? "mqtt_client" : "mqtt_client->config");
		return;
	}

	if (msg->payload_len) {
		MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> message callback: ");
		printf("%s %s\n", msg->topic, msg->payload);
	} else {
		MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> message callback: payload_len is 0\n");
	}
}

static void my_subscribe_callback(void *client, int msg_id, int qos_count, const int *granted_qos)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)client;

	if (mqtt_client == NULL || mqtt_client->config == NULL) {
		fprintf(stderr, "Error: >>> subscribe callback: %s is NULL.\n", mqtt_client == NULL ? "mqtt_client" : "mqtt_client->config");
		return;
	}

	MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> subscribe callback: client_id=%s, msg_id=%d, qos_count=%d, granted_qos=%d\n", mqtt_client->config->client_id, msg_id, qos_count, *granted_qos);
}

static void my_unsubscribe_callback(void *client, int msg_id)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)client;

	if (mqtt_client == NULL || mqtt_client->config == NULL) {
		fprintf(stderr, "Error: >>> unsubscribe callback: %s is NULL.\n", mqtt_client == NULL ? "mqtt_client" : "mqtt_client->config");
		return;
	}

	MQTT_SUB_DEBUG_PRINT(mqtt_client, ">>> unsubscribe callback: client_id=%s, msg_id=%d\n", mqtt_client->config->client_id, msg_id);
}

static void print_usage(void)
{
	printf("%s is a simple mqtt client that will subscribe to a single topic and print all messages it receives.\n", MQTT_CLIENT_SUB_COMMAND_NAME);
	printf("Usage: %s [-c] [-k keepalive] [-p port] [-q qos] -h host -t topic\n", MQTT_CLIENT_SUB_COMMAND_NAME);
	printf("                     [-i id]\n");
	printf("                     [-d]\n");
	printf("                     [-u username [-P password]]\n");
	printf("                     [-V protocol_version]\n");
	printf("       %s --sub_topic topic\n", MQTT_CLIENT_SUB_COMMAND_NAME);
	printf("       %s --unsub_topic topic\n", MQTT_CLIENT_SUB_COMMAND_NAME);
	printf("       %s --stop\n", MQTT_CLIENT_SUB_COMMAND_NAME);
	printf("       %s --help\n\n", MQTT_CLIENT_SUB_COMMAND_NAME);
	printf(" -c : disable 'clean session' (store subscription and pending messages when client disconnects).\n");
	printf(" -d : enable debug messages.\n");
	printf(" -h : mqtt host to connect to. Defaults to localhost.\n");
	printf(" -i : id to use for this client. Defaults to mosquitto_pub_ appended with the process id.\n");
	printf(" -k : keep alive in seconds for this client. Defaults to 60.\n");
	printf(" -p : network port to connect to. Defaults to 1883.\n");
	printf(" -P : provide a password (requires MQTT 3.1 broker)\n");
	printf(" -q : quality of service level to use for all messages. Defaults to 0.\n");
	printf(" -t : mqtt topic to subscribe to.\n");
	printf(" -u : provide a username (requires MQTT 3.1 broker)\n");
	printf(" -V : specify the version of the MQTT protocol to use when connecting.\n");
	printf("      Can be mqttv31 or mqttv311. Defaults to mqttv31.\n");
	printf(" --help : display this message.\n");
	printf(" --quiet: don't print error messages.\n");
	printf(" --sub_topic   : add mqtt topic to subscribe.\n");
	printf(" --unsub_topic : mqtt topic to unsubscribe.\n");
	printf(" --stop : stop mqtt subscriber.\n");
}

static void init_variables(void)
{
	g_id = NULL;
	g_host_addr = NULL;
	g_port = MQTT_DEFAULT_BROKER_PORT;
	g_clean_session = true;
	g_topic = NULL;
	g_qos = 0;
	g_username = NULL;
	g_password = NULL;
	g_keepalive = MQTT_DEFAULT_KEEP_ALIVE_TIME;
	g_protocol_version = MQTT_PROTOCOL_VERSION_31;
	g_stop = 0;
	g_sub_topic = NULL;
	g_unsub_topic = NULL;
	g_debug = false;
}

static void deinit_variables(void)
{
	if (g_id) {
		free(g_id);
		g_id = NULL;
	}

	if (g_host_addr) {
		free(g_host_addr);
		g_host_addr = NULL;
	}

	if (g_topic) {
		free(g_topic);
		g_topic = NULL;
	}

	if (g_username) {
		free(g_username);
		g_username = NULL;
	}

	if (g_password) {
		free(g_password);
		g_password = NULL;
	}

	if (g_unsub_topic) {
		free(g_unsub_topic);
		g_unsub_topic = NULL;
	}

	if (g_sub_topic) {
		free(g_sub_topic);
		g_sub_topic = NULL;
	}

}

static int make_client_config(void)
{
	if (g_host_addr == NULL) {
		fprintf(stderr, "Error: broker address is NULL. You can set host address with -h option.\n");
		goto errout;
	}

	if (g_topic == NULL) {
		fprintf(stderr, "Error: topic is NULL. You can set host address with -t option.\n");
		goto errout;
	}

	if (g_id == NULL) {
		if (g_clean_session == false) {
			fprintf(stderr, "Error: You must provide a client id using -i option if you are using the -c option.\n");
			goto errout;
		}

		g_id = mqtt_generate_client_id(MQTT_CLIENT_SUB_COMMAND_NAME);
		if (g_id == NULL) {
			fprintf(stderr, "Error: fail to set a client id.\n");
			goto errout;
		}
	}

	/* set information to subscribe */
	memset(&g_subscribe_msg, 0, sizeof(g_subscribe_msg));
	g_subscribe_msg.topic = strdup(g_topic);
	g_subscribe_msg.qos = g_qos;

#if defined(CONFIG_NETUTILS_MQTT_SECURITY)
	/* set tls parameters */

	/* set ca_cert */
	g_tls.ca_cert = mqtt_get_ca_certificate();	/* the pointer of ca_cert buffer */
	g_tls.ca_cert_len = mqtt_get_ca_certificate_size();	/* the length of ca_cert buffer  */

	/* set cert */
	g_tls.cert = mqtt_get_client_certificate();	/* the pointer of cert buffer */
	g_tls.cert_len = mqtt_get_client_certificate_size();	/* the length of cert buffer */

	/* set key */
	g_tls.key = mqtt_get_client_key();	/* the pointer of key buffer */
	g_tls.key_len = mqtt_get_client_key_size();	/* the length of key buffer */
#endif

	/* set mqtt config */
	memset(&g_mqtt_client_config, 0, sizeof(g_mqtt_client_config));
	g_mqtt_client_config.client_id = strdup(g_id);
	g_mqtt_client_config.user_name = strdup(g_username);
	g_mqtt_client_config.password = strdup(g_password);
	g_mqtt_client_config.clean_session = g_clean_session;
	g_mqtt_client_config.protocol_version = g_protocol_version;
	g_mqtt_client_config.debug = g_debug;
	g_mqtt_client_config.on_connect = my_connect_callback;
	g_mqtt_client_config.on_disconnect = my_disconnect_callback;
	g_mqtt_client_config.on_message = my_message_callback;
	g_mqtt_client_config.on_subscribe = my_subscribe_callback;
	g_mqtt_client_config.on_unsubscribe = my_unsubscribe_callback;
	g_mqtt_client_config.user_data = &g_subscribe_msg;

#if defined(CONFIG_NETUTILS_MQTT_SECURITY)
	if (g_port == MQTT_SECURITY_BROKER_PORT) {
		g_mqtt_client_config.tls = &g_tls;
	} else {
		g_mqtt_client_config.tls = NULL;
	}
#else
	g_mqtt_client_config.tls = NULL;
#endif

	return 0;

errout:
	return -1;
}

static void clean_client_config(void)
{
	/* g_subscribe_msg */
	if (g_subscribe_msg.topic) {
		free(g_subscribe_msg.topic);
		g_subscribe_msg.topic = NULL;
	}

	if (g_subscribe_msg.payload) {
		free(g_subscribe_msg.payload);
		g_subscribe_msg.payload = NULL;
	}

	/* g_mqtt_client_config */
	if (g_mqtt_client_config.client_id) {
		free(g_mqtt_client_config.client_id);
		g_mqtt_client_config.client_id = NULL;
	}

	if (g_mqtt_client_config.user_name) {
		free(g_mqtt_client_config.user_name);
		g_mqtt_client_config.user_name = NULL;
	}

	if (g_mqtt_client_config.password) {
		free(g_mqtt_client_config.password);
		g_mqtt_client_config.password = NULL;
	}
}

static int process_options(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -p argument given but no port specified.\n\n");
				return 1;
			} else {
				g_port = atoi(argv[i + 1]);
				if (g_port < 1 || g_port > 65535) {
					fprintf(stderr, "Error: Invalid port given: %d\n", g_port);
					return 1;
				}
			}
			i++;
		} else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
			g_debug = true;
		} else if (!strcmp(argv[i], "--help")) {
			return 2;
		} else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--host")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -h argument given but no host specified.\n\n");
				return 1;
			} else {
				g_host_addr = strdup(argv[i + 1]);
			}
			i++;
		} else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--id")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -i argument given but no id specified.\n\n");
				return 1;
			} else {
				g_id = strdup(argv[i + 1]);
			}
			i++;
		} else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--keepalive")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -k argument given but no keepalive specified.\n\n");
				return 1;
			} else {
				g_keepalive = atoi(argv[i + 1]);
				if (g_keepalive > 65535) {
					fprintf(stderr, "Error: Invalid keepalive given: %d\n", g_keepalive);
					return 1;
				}
			}
			i++;
		} else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--protocol-version")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: --protocol-version argument given but no version specified.\n\n");
				return 1;
			} else {
				if (!strcmp(argv[i + 1], "mqttv31")) {
					g_protocol_version = MQTT_PROTOCOL_VERSION_31;
				} else if (!strcmp(argv[i + 1], "mqttv311")) {
					g_protocol_version = MQTT_PROTOCOL_VERSION_311;
				} else {
					fprintf(stderr, "Error: Invalid protocol version argument given.\n\n");
					return 1;
				}
				i++;
			}
		} else if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--qos")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -q argument given but no QoS specified.\n\n");
				return 1;
			} else {
				g_qos = atoi(argv[i + 1]);
				if (g_qos < 0 || g_qos > 2) {
					fprintf(stderr, "Error: Invalid QoS given: %d\n", g_qos);
					return 1;
				}
			}
			i++;
		} else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--topic")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -t argument given but no topic specified.\n\n");
				return 1;
			} else {
				g_topic = strdup(argv[i + 1]);
				i++;
			}
		} else if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--username")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -u argument given but no username specified.\n\n");
				return 1;
			} else {
				g_username = strdup(argv[i + 1]);
			}
			i++;
		} else if (!strcmp(argv[i], "-P") || !strcmp(argv[i], "--pw")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -P argument given but no password specified.\n\n");
				return 1;
			} else {
				g_password = strdup(argv[i + 1]);
			}
			i++;
		} else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--disable-clean-session")) {
			g_clean_session = false;
		} else if (!strcmp(argv[i], "--sub_topic")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -t argument given but no sub_topic specified.\n\n");
				return 1;
			} else {
				g_sub_topic = strdup(argv[i + 1]);
				i++;
			}
		} else if (!strcmp(argv[i], "--unsub_topic")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -t argument given but no unsub_topic specified.\n\n");
				return 1;
			} else {
				g_unsub_topic = strdup(argv[i + 1]);
				i++;
			}
		} else if (!strcmp(argv[i], "--stop")) {
			g_stop = 1;
		} else {
			goto unknown_option;
		}
	}

	return 0;

unknown_option:
	fprintf(stderr, "Error: Unknown option '%s'.\n", argv[i]);
	return 1;
}

static int check_option_on_client_running(void)
{
	int result = CHECK_OPTION_RESULT_CHECKED_ERROR;

	if (!g_mqtt_client_handle) {
		if (g_stop || g_sub_topic || g_unsub_topic) {
			printf("Error: MQTT client is not running.\n");
			goto done;
		}

		result = CHECK_OPTION_RESULT_NOTHING_TODO;
		goto done;
	}

	if (g_mqtt_client_handle && !(g_stop || g_sub_topic || g_unsub_topic)) {
		printf("Error: MQTT client is running. You have to stop the mqtt subscriber with --stop\n");
		printf("      in order to start new mqtt subscriber.\n");
		goto done;
	}

	if (g_stop) {
		int disconnect_try_count = 20;

		MQTT_SUB_DEBUG_PRINT(g_mqtt_client_handle, "disconnect from a MQTT broker before stopping MQTT client.\n");
		while ((mqtt_disconnect(g_mqtt_client_handle) != 0) && disconnect_try_count) {
			disconnect_try_count--;
			usleep(500 * 1000);
		}
		if (disconnect_try_count == 0) {
			fprintf(stderr, "Error: mqtt_disconnect() failed.\n");
			goto done;
		}

		MQTT_SUB_DEBUG_PRINT(g_mqtt_client_handle, "deinitialize MQTT client context.\n");
		if (mqtt_deinit_client(g_mqtt_client_handle) != 0) {
			fprintf(stderr, "Error: mqtt_deinit_client() failed.\n");
			goto done;
		}

		g_mqtt_client_handle = NULL;
		clean_client_config();
	} else {
		if (g_sub_topic) {
			MQTT_SUB_DEBUG_PRINT(g_mqtt_client_handle, "subscribe the specified topic.\n");
			if (mqtt_subscribe(g_mqtt_client_handle, g_sub_topic, g_qos) != 0) {
				fprintf(stderr, "Error: mqtt_subscribe() failed.\n");
				goto done;
			}
		}

		if (g_unsub_topic) {
			MQTT_SUB_DEBUG_PRINT(g_mqtt_client_handle, "unsubscribe the specified topic.\n");
			if (mqtt_unsubscribe(g_mqtt_client_handle, g_unsub_topic) != 0) {
				fprintf(stderr, "Error: mqtt_unsubscribe() failed.\n");
				goto done;
			}
		}
	}

	/* result is success */
	result = CHECK_OPTION_RESULT_CHECKED_OK;

done:
	return result;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int mqtt_client_sub_task(void *arg)
{
	int result = -1;
	int ret = 0;
	int argc;
	char **argv;

	argc = ((struct mqtt_sub_input *)arg)->argc;
	argv = ((struct mqtt_sub_input *)arg)->argv;
	if (argc == 1) {
		print_usage();
		return 0;
	}

	/* set  the seed of a new sequence of random values */
	mqtt_set_srand();

	/* check options and set variables */
	init_variables();
	ret = process_options(argc, argv);
	if (ret != 0) {
		if (ret == 2) {
			print_usage();
			result = 0;
		}
		goto done;
	}

	/* check and do options when a client is running */
	ret = check_option_on_client_running();
	if (ret == CHECK_OPTION_RESULT_CHECKED_OK) {
		result = 0;
		goto done;
	} else if (ret == CHECK_OPTION_RESULT_CHECKED_ERROR) {
		goto done;
	}

	/* make mqtt subscriber client config */
	if (make_client_config() != 0) {
		goto done;
	}

	/* create mqtt subscriber client */
	if (g_debug) {
		printf("initialize MQTT client context.\n");
	}
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	if (g_mqtt_client_handle == NULL) {
		fprintf(stderr, "Error: mqtt_init_client() failed.\n");
		clean_client_config();
		goto done;
	}

	/* connect to a mqtt broker */
	if (g_debug) {
		printf("connect to a MQTT broker (%s : %d).\n", g_host_addr, g_port);
	}
	if (mqtt_connect(g_mqtt_client_handle, g_host_addr, g_port, g_keepalive) != 0) {
		fprintf(stderr, "Error: mqtt_connect() failed.\n");

		if (mqtt_deinit_client(g_mqtt_client_handle) != 0) {
			fprintf(stderr, "Error: mqtt_deinit_client() failed.\n");
		} else {
			g_mqtt_client_handle = NULL;
		}
		clean_client_config();
		goto done;
	}

	if (g_debug) {
		printf("MQTT subscriber has started successfully.\n");
	}

	/* result is success */
	result = 0;

done:
	deinit_variables();

	return result;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
#else
int mqtt_client_sub_main(int argc, char *argv[])
#endif
{
	int result = -1;
	int ret;
	pthread_attr_t attr;
	struct sched_param sparam;
	pthread_t tid;
	struct mqtt_sub_input arg;

	ret = pthread_attr_init(&attr);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_init() failed. (ret=%d)\n", ret);
		goto done;
	}

	sparam.sched_priority = MQTT_SUB_SCHED_PRI;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_setschedparam() failed. (ret=%d)\n", ret);
		goto done;
	}

	ret = pthread_attr_setschedpolicy(&attr, MQTT_SUB_SCHED_POLICY);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_setschedpolicy() failed. (ret=%d)\n", ret);
		goto done;
	}

	ret = pthread_attr_setstacksize(&attr, MQTT_SUB_STACK_SIZE);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_setstacksize() failed. (ret=%d)\n", ret);
		goto done;
	}

	arg.argc = argc;
	arg.argv = argv;
	ret = pthread_create(&tid, &attr, (pthread_startroutine_t)mqtt_client_sub_task, &arg);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_create() failed. (ret=%d)\n", ret);
		goto done;
	}

	pthread_setname_np(tid, MQTT_CLIENT_SUB_COMMAND_NAME);

	pthread_join(tid, NULL);

	/* result is success */
	result = 0;

done:
	if (result != 0) {
		fprintf(stderr, "Error: fail to start %s.\n", MQTT_CLIENT_SUB_COMMAND_NAME);
	}

	return result;
}

bool mqtt_client_sub_is_running(void)
{
	return g_mqtt_client_handle ? true : false;
}

void mqtt_set_srand(void)
{
	static int initialzed = 0;
	if (!initialzed) {
		srand(time(NULL));
		initialzed = 1;
	}
}

char *mqtt_generate_client_id(const char *id_base)
{
	int len;
	char *client_id = NULL;

	len = strlen(id_base) + strlen("/") + 5 + 1;
	client_id = malloc(len);
	if (!client_id) {
		fprintf(stderr, "Error: Out of memory.\n");
		return NULL;
	}
	snprintf(client_id, len, "%s/%05d", id_base, rand() % 100000);
	if (strlen(client_id) > MQTT_ID_MAX_LENGTH) {
		/* Enforce maximum client id length of 23 characters */
		client_id[MQTT_ID_MAX_LENGTH] = '\0';
	}

	return client_id;
}

const unsigned char *mqtt_get_ca_certificate(void)
{
	return (const unsigned char *)mqtt_ca_crt_rsa;
}

const unsigned char *mqtt_get_client_certificate(void)
{
	return (const unsigned char *)mqtt_cli_crt_rsa;
}

const unsigned char *mqtt_get_client_key(void)
{
	return (const unsigned char *)mqtt_cli_key_rsa;
}

int mqtt_get_ca_certificate_size(void)
{
	return sizeof(mqtt_ca_crt_rsa);
}

int mqtt_get_client_certificate_size(void)
{
	return sizeof(mqtt_cli_crt_rsa);
}

int mqtt_get_client_key_size(void)
{
	return sizeof(mqtt_cli_key_rsa);
}
