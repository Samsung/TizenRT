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
 * @file mqtt_client_pub.c
 * @brief the program for testing mqtt publisher
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
#include <semaphore.h>
#include <tinyara/clock.h>
#include <errno.h>

#include <network/mqtt/mqtt_api.h>

#if defined(CONFIG_NETUTILS_MQTT_SECURITY)
#include "mbedtls/x509_crt.h"
#include "mbedtls/pem.h"
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define MQTT_CLIENT_PUB_COMMAND_NAME	"mqtt_pub"
#define MQTT_ID_MAX_LENGTH				23

#define MQTT_PUB_STACK_SIZE				20480
#define MQTT_PUB_SCHED_PRI				100
#define MQTT_PUB_SCHED_POLICY			SCHED_RR

#define MQTT_PUB_DEBUG_PRINT(client_handle, ...) \
		do { \
			if (client_handle && (client_handle)->config && (client_handle)->config->debug) \
				printf(__VA_ARGS__); \
		} while (0);

/****************************************************************************
 * Structure
 ****************************************************************************/
struct mqtt_pub_input {
	int argc;
	char **argv;
};

/****************************************************************************
 * External Function Prototype
 ****************************************************************************/
extern void mqtt_set_srand(void);
extern char *mqtt_generate_client_id(const char *id_base);
extern const unsigned char *mqtt_get_ca_certificate(void);
extern const unsigned char *mqtt_get_client_certificate(void);
extern const unsigned char *mqtt_get_client_key(void);
extern int mqtt_get_ca_certificate_size(void);
extern int mqtt_get_client_certificate_size(void);
extern int mqtt_get_client_key_size(void);

/****************************************************************************
 * Global Valiables
 ****************************************************************************/
static mqtt_client_t *g_mqtt_client_handle;
static mqtt_client_config_t g_mqtt_client_config;
static mqtt_msg_t g_publish_msg;
static sem_t g_mqtt_pub_sem;
#if defined(CONFIG_NETUTILS_MQTT_SECURITY)
static mqtt_tls_param_t g_tls;
#endif

static char *g_id;
static char *g_host_addr;
static int g_port;
static char *g_topic;
static char *g_message;
static int g_qos;
static int g_retain;
static char *g_username;
static char *g_password;
static int g_keepalive;
static int g_protocol_version;
static bool g_nullmsg;
static bool g_debug;

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
		MQTT_PUB_DEBUG_PRINT(mqtt_client, ">>> connect callback: client_id=%s, connect success!\n", mqtt_client->config->client_id);

		if (mqtt_client->config->user_data) {
			mqtt_msg = (mqtt_msg_t *)mqtt_client->config->user_data;
			if (mqtt_publish(mqtt_client, mqtt_msg->topic, mqtt_msg->payload, mqtt_msg->payload_len, mqtt_msg->qos, mqtt_msg->retain) != 0) {
				fprintf(stderr, "Error: mqtt_publish() failed.\n");
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

		MQTT_PUB_DEBUG_PRINT(mqtt_client, ">>> connect callback: client_id=%s, connect failed (reason: %s)\n", mqtt_client->config->client_id, reason_str);
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
		MQTT_PUB_DEBUG_PRINT(mqtt_client, ">>> disconnect callback: client_id=%s, disconnected by mqtt_disconnect()\n", mqtt_client->config->client_id);
	} else {
		MQTT_PUB_DEBUG_PRINT(mqtt_client, ">>> disconnect callback: client_id=%s, disconnected by other reason\n", mqtt_client->config->client_id);
	}

	sem_post(&g_mqtt_pub_sem);
}

static void my_publish_callback(void *client, int msg_id)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)client;

	if (mqtt_client == NULL || mqtt_client->config == NULL) {
		fprintf(stderr, "Error: >>> publish callback: %s is NULL.\n", mqtt_client == NULL ? "mqtt_client" : "mqtt_client->config");
		return;
	}

	MQTT_PUB_DEBUG_PRINT(mqtt_client, ">>> publish callback: client_id=%s, msg_id = %d\n", mqtt_client->config->client_id, msg_id);

	MQTT_PUB_DEBUG_PRINT(mqtt_client, "disconnect from a MQTT broker before stopping MQTT client.\n");
	if (mqtt_disconnect(mqtt_client) != 0) {
		fprintf(stderr, "Error: mqtt_disconnect() failed.\n");
	}
}

static void print_usage(void)
{
	printf("%s is a simple mqtt client that will publish a message on a single topic and exit.\n", MQTT_CLIENT_PUB_COMMAND_NAME);
	printf("Usage: %s [-k keepalive] [-p port] [-q qos] [-r] -h host {-n | -m message} -t topic\n", MQTT_CLIENT_PUB_COMMAND_NAME);
	printf("                     [-i id]\n");
	printf("                     [-d]\n");
	printf("                     [-u username [-P password]]\n");
	printf("                     [-V protocol_version]\n");
	printf("       %s --help\n\n", MQTT_CLIENT_PUB_COMMAND_NAME);
	printf(" -d : enable debug messages.\n");
	printf(" -h : mqtt host to connect to. Defaults to localhost.\n");
	printf(" -i : id to use for this client. Defaults to mosquitto_pub_ appended with the process id.\n");
	printf(" -k : keep alive in seconds for this client. Defaults to 60.\n");
	printf(" -m : message payload to send.\n");
	printf(" -p : network port to connect to. Defaults to 1883.\n");
	printf(" -P : provide a password (requires MQTT 3.1 broker)\n");
	printf(" -n : send a null (zero length) message.\n");
	printf(" -q : quality of service level to use for all messages. Defaults to 0.\n");
	printf(" -r : message should be retained.\n");
	printf(" -t : mqtt topic to publish to.\n");
	printf(" -u : provide a username (requires MQTT 3.1 broker)\n");
	printf(" -V : specify the version of the MQTT protocol to use when connecting.\n");
	printf("      Can be mqttv31 or mqttv311. Defaults to mqttv31.\n");
	printf(" --help : display this message.\n");
}

static void init_variables(void)
{
	g_id = NULL;
	g_host_addr = NULL;
	g_port = MQTT_DEFAULT_BROKER_PORT;
	g_topic = NULL;
	g_message = NULL;
	g_qos = 0;
	g_retain = 0;
	g_username = NULL;
	g_password = NULL;
	g_keepalive = MQTT_DEFAULT_KEEP_ALIVE_TIME;
	g_protocol_version = MQTT_PROTOCOL_VERSION_31;
	g_nullmsg = false;
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

	if (g_message) {
		free(g_message);
		g_message = NULL;
	}

	if (g_username) {
		free(g_username);
		g_username = NULL;
	}

	if (g_password) {
		free(g_password);
		g_password = NULL;
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

	if ((g_message == NULL) && (g_nullmsg == false)) {
		fprintf(stderr, "Error: message is NULL. You can set host address with -m option.\n");
		goto errout;
	}

	if (g_id == NULL) {
		g_id = mqtt_generate_client_id(MQTT_CLIENT_PUB_COMMAND_NAME);
		if (g_id == NULL) {
			fprintf(stderr, "Error: fail to set a client id.\n");
			goto errout;
		}
	}

	/* set information to publish */
	memset(&g_publish_msg, 0, sizeof(g_publish_msg));
	g_publish_msg.topic = strdup(g_topic);
	if (g_nullmsg) {
		g_publish_msg.payload = NULL;
		g_publish_msg.payload_len = 0;
	} else {
		g_publish_msg.payload = strdup(g_message);
		g_publish_msg.payload_len = strlen(g_message);
	}
	g_publish_msg.qos = g_qos;
	g_publish_msg.retain = g_retain;

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
	g_mqtt_client_config.protocol_version = g_protocol_version;
	g_mqtt_client_config.debug = g_debug;
	g_mqtt_client_config.on_connect = my_connect_callback;
	g_mqtt_client_config.on_disconnect = my_disconnect_callback;
	g_mqtt_client_config.on_publish = my_publish_callback;
	g_mqtt_client_config.user_data = &g_publish_msg;

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
	/* g_publish_msg */
	if (g_publish_msg.topic) {
		free(g_publish_msg.topic);
		g_publish_msg.topic = NULL;
	}

	if (g_publish_msg.payload) {
		free(g_publish_msg.payload);
		g_publish_msg.payload = NULL;
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
		} else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--message")) {
			if (i == argc - 1) {
				fprintf(stderr, "Error: -m argument given but no message specified.\n\n");
				return 1;
			} else {
				g_message = strdup(argv[i + 1]);
			}
			i++;
		} else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--null-message")) {
			g_nullmsg = true;
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
		} else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--retain")) {
			g_retain = 1;
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
		} else {
			goto unknown_option;
		}
	}

	return 0;

unknown_option:
	fprintf(stderr, "Error: Unknown option '%s'.\n", argv[i]);
	return 1;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int mqtt_client_pub_task(void *arg)
{
	int result = -1;
	int ret = 0;
	struct timespec abstime;
	const int wait_sec = 30;
	const int wait_nsec = 0;
	int timeout = 0;
	int argc;
	char **argv;

	argc = ((struct mqtt_pub_input *)arg)->argc;
	argv = ((struct mqtt_pub_input *)arg)->argv;
	if (argc == 1) {
		print_usage();
		return 0;
	}

	/* initialize a semaphore for signaling */
	sem_init(&g_mqtt_pub_sem, 0, 0);
	sem_setprotocol(&g_mqtt_pub_sem, SEM_PRIO_NONE);

	/* set the seed of a new sequence of random values */
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

	/* make mqtt publisher client config */
	if (make_client_config() != 0) {
		goto done;
	}

	/* create mqtt publisher client */
	if (g_debug) {
		printf("initialize MQTT client context.\n");
	}
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	if (g_mqtt_client_handle == NULL) {
		fprintf(stderr, "Error: mqtt_init_client() failed.\n");
		goto done;
	}

	/* connect to a mqtt broker */
	if (g_debug) {
		printf("connect to a MQTT broker (%s).\n", g_host_addr);
	}
	if (mqtt_connect(g_mqtt_client_handle, g_host_addr, g_port, g_keepalive) != 0) {
		fprintf(stderr, "Error: mqtt_connect() failed.\n");
		goto done;
	}

	/* wait for finishing mqtt publish */
	(void)clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += wait_sec;
	abstime.tv_nsec += wait_nsec;
	if (abstime.tv_nsec >= NSEC_PER_SEC) {
		abstime.tv_sec++;
		abstime.tv_nsec -= NSEC_PER_SEC;
	}
	while (sem_timedwait(&g_mqtt_pub_sem, &abstime) != 0) {
		int err = get_errno();
		ASSERT(err == EINTR || err == ETIMEDOUT);

		if (err == ETIMEDOUT) {
			timeout = 1;
			break;
		}
	}
	if (timeout) {
		fprintf(stderr, "Error: mqtt_pub timeout!\n");
		goto done;
	}

	/* result is success */
	result = 0;

done:
	if (g_mqtt_client_handle) {
		MQTT_PUB_DEBUG_PRINT(g_mqtt_client_handle, "deinitialize MQTT client context.\n");
		if (mqtt_deinit_client(g_mqtt_client_handle) != 0) {
			fprintf(stderr, "Error: mqtt_deinit_client() failed.\n");
		}

		g_mqtt_client_handle = NULL;
	}

	deinit_variables();
	clean_client_config();
	sem_destroy(&g_mqtt_pub_sem);

	return result;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
#else
int mqtt_client_pub_main(int argc, char *argv[])
#endif
{
	int result = -1;
	int ret;
	pthread_attr_t attr;
	struct sched_param sparam;
	pthread_t tid;
	struct mqtt_pub_input arg;

	ret = pthread_attr_init(&attr);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_init() failed. (ret=%d)\n", ret);
		goto done;
	}

	sparam.sched_priority = MQTT_PUB_SCHED_PRI;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_setschedparam() failed. (ret=%d)\n", ret);
		goto done;
	}

	ret = pthread_attr_setschedpolicy(&attr, MQTT_PUB_SCHED_POLICY);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_setschedpolicy() failed. (ret=%d)\n", ret);
		goto done;
	}

	ret = pthread_attr_setstacksize(&attr, MQTT_PUB_STACK_SIZE);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_attr_setstacksize() failed. (ret=%d)\n", ret);
		goto done;
	}

	arg.argc = argc;
	arg.argv = argv;

	ret = pthread_create(&tid, &attr, (pthread_startroutine_t)mqtt_client_pub_task, &arg);
	if (ret != 0) {
		fprintf(stderr, "Error: pthread_create() failed. (ret=%d)\n", ret);
		goto done;
	}
	pthread_setname_np(tid, MQTT_CLIENT_PUB_COMMAND_NAME);

	pthread_join(tid, NULL);

	/* result is success */
	result = 0;

done:
	if (result != 0) {
		fprintf(stderr, "Error: fail to start %s\n", MQTT_CLIENT_PUB_COMMAND_NAME);
	}

	return result;
}
