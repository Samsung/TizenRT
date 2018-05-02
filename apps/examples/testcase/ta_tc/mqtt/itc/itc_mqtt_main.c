/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <netdb.h>
#include <network/mqtt/mqtt_api.h>
#include "tc_common.h"

#define ITC_MQTT_TOPIC "topic_itc"
#define ITC_SUB_PUB_ID "itc_mqtt_pub/02"
#define ITC_MQTT_MESSAGE "itc hello mqtt"
#define ITC_MQTT_LOGE printf("%s:%d\n", __FUNCTION__, __LINE__)
#define ITC_MQTT_DEBUG
#ifdef ITC_MQTT_DEBUG
#define ITC_MQTT_LOGD printf
#else
#define ITC_MQTT_LOGD
#endif
#define ITC_MQTT_LOOP_SIZE 10

#define ITC_MQTT_WAIT_SIGNAL \
	do {\
		sem_wait(&g_mqtt_itc_signal);				\
	} while (0)

#define ITC_MQTT_SEND_SIGNAL\
	do {\
		sem_post(&g_mqtt_itc_signal);\
	} while (0)

static mqtt_client_t *g_mqtt_client_handle;
static sem_t g_mqtt_itc_signal;
static char g_mqtt_msg[] = ITC_MQTT_MESSAGE;

/****************************************************************************************
 *Private Functions
 ****************************************************************************************/

static int _itc_mqtt_init(void)
{
	g_mqtt_client_handle = NULL;

	int ret = sem_init(&g_mqtt_itc_signal, 0, 0);
	if (ret != OK) {
		ITC_MQTT_LOGE;
		return -1;
	}
	return 0;
}

static void _itc_mqtt_deinit(void)
{
	int ret = sem_destroy(&g_mqtt_itc_signal);
	if (ret < 0) {
		ITC_MQTT_LOGE;
	}
	return;
}

static void on_connect(void *client, int result)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (id && id->config) {
		ITC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, result);
	}
	ITC_MQTT_SEND_SIGNAL;
}

static void on_disconnect(void *client, int result)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (id && id->config) {
		ITC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, result);
	}
	ITC_MQTT_SEND_SIGNAL;
}

static void on_publish(void *client, int msg_id)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (id && id->config) {
		ITC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, msg_id);
	}
	ITC_MQTT_SEND_SIGNAL;
}

static void on_message(void *client, mqtt_msg_t *msg)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		return;
	}
	ITC_MQTT_LOGD("%s: %s, %p\n", __FUNCTION__, id->config->client_id, msg);
}

static void on_subscribe(void *client, int msg_id, int qos_count, const int *granted_qos)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		return;
	}
	ITC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, msg_id);
}

static void on_unsubscribe(void *client, int msg_id)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		return;
	}
	ITC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, msg_id);
}

static double diff_time(struct timeval *x, struct timeval *y)
{
	double x_ms;
	double y_ms;
	double diff;
	x_ms = ((double)x->tv_sec * 1000000) + ((double)x->tv_usec);
	y_ms = ((double)y->tv_sec * 1000000) + ((double)y->tv_usec);
	diff = (double)y_ms - (double)x_ms;
	return diff;
}
/****************************************************************************************
 * pre-initialization functions
 ****************************************************************************************/

static mqtt_client_config_t g_mqtt_client_config = {
	ITC_SUB_PUB_ID,
	"", "", 1, 3, 0, 0,
	on_connect,
	on_disconnect,
	on_publish,
	on_message,
	on_subscribe,
	on_unsubscribe
};

/*
 * @testcase             itc_mqtt_init_deinit_client_p
 * @brief                To check  initialize mqtt and de-initialize mqtt
 * @scenario             If used api returns not NULL and 0
 * @apicovered           mqtt_init_client & mqtt_deinit_client
 * @precondition         initialize g_mqt_client_config
 * @postcondition        NA
*/

void itc_mqtt_init_deinit_client_p(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase             itc_mqtt_init_deinit_client_n
 * @brief                To check initialize mqtt and de-initialize mqtt
 * @scenario             If used api returns NULL and not 0
 * @apicovered           mqtt_init_client & mqtt_deinit_client
 * @precondition         initialize g_mqt_client_config
 * @postcondition        NA
*/

void itc_mqtt_init_deinit_client_n(void)
{
	int res;
	static mqtt_client_config_t mqtt_client_config = {
		"", NULL, NULL,
		0, 0, 0, 0,
		on_connect,
		on_disconnect,
		on_publish,
		on_message,
		on_subscribe,
		on_unsubscribe
	};

	mqtt_client_t *mqtt_client_handle;

	mqtt_client_handle = mqtt_init_client(&mqtt_client_config);
	TC_ASSERT_EQ("mqtt_init_client", mqtt_client_handle, NULL);

	res = mqtt_deinit_client(mqtt_client_handle);
	TC_ASSERT_NEQ("mqtt_deinit_client", res, 0);

	TC_SUCCESS_RESULT();
}


/*
 * @testcase             itc_mqtt_connect_disconnect_p
 * @brief                To check mqtt_connect
 * @scenario             If used api returns not 0
 * @apicovered           mqtt_connect & mqtt_disconnect
 * @precondition         initialize g_mqt_client_config and initialize
 * @postcondition        mqtt_deinit_client
*/

void itc_mqtt_connect_disconnect_p(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);

	// MQTT Connect
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR, CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ("mqtt_connect", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	//MQTT Disconnect
	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_disconnect", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase             itc_mqtt_connect_disconnect_n
 * @brief                To check mqtt_disconnect when connect fails
 * @scenario             If mqtt_connect returns not 0 and mqtt_disconnect return 0
 * @apicovered           mqtt_init_client, mqtt_connect, mqtt_disconnect & mqtt_deinit_client
 * @precondition         initialize g_mqt_client_config and initialize
 * @postcondition        mqtt_deinit_client
*/

void itc_mqtt_connect_disconnect_n(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);

	// MQTT Connect
	res = mqtt_connect(g_mqtt_client_handle, NULL, CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ("mqtt_connect", res, -1);

	//MQTT Disconnect
	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}


/*
 * @testcase             itc_mqtt_publish_p
 * @brief                To check mqtt_connect
 * @scenario             If used api returns 0
 * @apicovered           mqtt_publish, mqtt_init_client, mqtt_connect, mqtt_disconnect,
 *                       mqtt_deinit_client
 * @precondition         mqtt_init_client and mqtt_connect
 * @postcondition        mqtt_deinit_client
*/

void itc_mqtt_publish_p(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR, CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_publish(g_mqtt_client_handle, ITC_MQTT_TOPIC, g_mqtt_msg, sizeof(g_mqtt_msg), 0, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_publish", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase             itc_mqtt_publish_n
 * @brief                To check mqtt_connect
 * @scenario             If used api returns not 0
 * @apicovered           mqtt_publish, mqtt_init_client, mqtt_connect, mqtt_disconnect,
 *                       mqtt_deinit_client
 * @precondition         mqtt_init_client and mqtt_connect
 * @postcondition        mqtt_deinit_client
*/

void itc_mqtt_publish_n(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle,
			CONFIG_ITC_MQTT_BROKER_ADDR,
			CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0,
			mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_publish(g_mqtt_client_handle, NULL, g_mqtt_msg, sizeof(g_mqtt_msg), 0, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_publish", res, -1,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_subscribe_unsubscribe_p
 * @brief            stop subscribing from a broker
 * @scenario         call mqtt_subscribe, mqtt_unsubscribe return 0
 * @apicovered       mqtt_init_client, mqtt_connect, mqtt_subscribe, mqtt_unsubscribe, mqtt_disconnect,
 *                   mqtt_deinit_client
 * @precondition     mqtt_init_client, mqtt_connect
 * @postcondition    mqtt_disconnect, mqtt_deinit_client
 */

void itc_mqtt_subscribe_unsubscribe_p(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR, CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_subscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_subscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_unsubscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC);
	TC_ASSERT_EQ_CLEANUP("mqtt_unsubscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_disconnect", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}
/**
 * @testcase             itc_mqtt_deinit_n_redeinit_client
 * @brief                de-initialize mqtt when it's already deinit
 * @scenario             de-initialize mqtt when it's already deinit
 * @apicovered           mqtt_deinit_client, mqtt_deinit_client
 * @precondition         mqtt_deinit_client
 * @postcondition        none
*/

void itc_mqtt_deinit_n_redeinit_client(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	g_mqtt_client_handle = NULL;

	//MQTT re-deinit
	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, -1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase             itc_mqtt_connect_n_reconnect
 * @brief                check mqtt_connect if already connected
 * @scenario             re-connect to server
 * @apicovered           mqtt_init_client, mqtt_connect, mqtt_disconnect, mqtt_deinit_client
 * @precondition         mqtt_init_client, mqtt_connect
 * @postcondition        mqtt_disconnect, mqtt_deinit_client
*/

void itc_mqtt_connect_n_reconnect(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);

	// MQTT Connect
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR,	CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	// MQTT re-Connect
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR,	CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	if (g_mqtt_client_handle != NULL) {
		TC_ASSERT_EQ_CLEANUP("mqtt_connect", g_mqtt_client_handle->state, MQTT_CLIENT_STATE_CONNECTED,
			mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));
	}
	//MQTT Disconnect
	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase             itc_mqtt_disconnect_n_redisconnect
 * @brief                already disconnected from server
 * @scenario             already disconnected from server
 * @apicovered           mqtt_init_client, mqtt_connect, mqtt_disconnect, mqtt_deinit_client
 * @precondition         mqtt_disconnect
 * @postcondition        mqtt_deinit_client
*/

void itc_mqtt_disconnect_n_redisconnect(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);

	// MQTT Connect
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR,	CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	//MQTT Disconnect
	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	//MQTT re-disconnect
	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	if (g_mqtt_client_handle != NULL) {
		TC_ASSERT_EQ("mqtt_disconnect", g_mqtt_client_handle->state, MQTT_CLIENT_STATE_NOT_CONNECTED);
	}
	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_subscribe_p_resubscribe
 * @brief            already subscribe from a broker
 * @scenario         already subscribe from a broker
 * @apicovered       mqtt_init_client, mqtt_connect, mqtt_subscribe, mqtt_unsubscribe, mqtt_disconnect,
 *                   mqtt_deinit_client
 * @precondition     mqtt_subscribe
 * @postcondition    mqtt_deinit_client
*/

void itc_mqtt_subscribe_p_resubscribe(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR, CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_subscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_subscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	//MQTT re-subscribe
	res = mqtt_subscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_subscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_unsubscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC);
	TC_ASSERT_EQ_CLEANUP("mqtt_unsubscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_unsubscribe_p_reunsubscribe
 * @brief            already un-subscribing from a broker
 * @scenario         already un-subscribing from a broker
 * @apicovered       mqtt_init_client, mqtt_connect, mqtt_subscribe, mqtt_unsubscribe, mqtt_disconnect,
 *                   mqtt_deinit_client
 * @precondition     mqtt_unsubscribe
 * @postcondition    mqtt_deinit_client
*/

void itc_mqtt_unsubscribe_p_reunsubscribe(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR, CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_subscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_subscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_unsubscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC);
	TC_ASSERT_EQ_CLEANUP("mqtt_unsubscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	//MQTT re-unsubscribe
	res = mqtt_unsubscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC);
	TC_ASSERT_EQ_CLEANUP("mqtt_unsubscribe", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_api_success_ratio_p
 * @brief            success ratio for all mqtt api
 * @scenario         success ratio for all mqtt api
 * @apicovered       mqtt_init_client, mqtt_connect, mqtt_subscribe, mqtt_unsubscribe, mqtt_disconnect,
 *                   mqtt_deinit_client, mqtt_publish
 * @precondition     none
 * @postcondition    none
*/

void itc_mqtt_api_success_ratio_p(void)
{
	int res;
	int i;
	float init_cnt = 0.0f;
	float s_init_cnt = 0.0f;
	float deinit_cnt = 0.0f;
	float s_deinit_cnt = 0.0f;
	float conn_cnt = 0.0f;
	float s_conn_cnt = 0.0f;
	float disconn_cnt = 0.0f;
	float s_disconn_cnt = 0.0f;
	float subs_cnt = 0.0f;
	float s_subs_cnt = 0.0f;
	float unsubs_cnt = 0.0f;
	float s_unsubs_cnt = 0.0f;
	float publish_cnt = 0.0f;
	float s_publish_cnt = 0.0f;

	for (i = 0; i < ITC_MQTT_LOOP_SIZE; i++) {
		g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
		if (g_mqtt_client_handle != NULL) {
			s_init_cnt++;
		} else {
			init_cnt++;
			continue;
		}
		init_cnt++;

		res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR,	CONFIG_ITC_MQTT_BROKER_PORT, 0);
		if (res == 0) {
			s_conn_cnt++;
		} else {
			conn_cnt++;
			res = mqtt_deinit_client(g_mqtt_client_handle);
			if (res != 0) {
				printf("mqtt_deinit_client failed\n");
			}
			continue;
		}
		conn_cnt++;
		ITC_MQTT_WAIT_SIGNAL;

		res = mqtt_subscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC, 0);
		if (res == 0) {
			s_subs_cnt++;
		}
		subs_cnt++;

		res = mqtt_publish(g_mqtt_client_handle, ITC_MQTT_TOPIC, g_mqtt_msg,
				sizeof(g_mqtt_msg), 0, 0);
		if (res == 0) {
			s_publish_cnt++;
		}
		publish_cnt++;
		ITC_MQTT_WAIT_SIGNAL;

		res = mqtt_unsubscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC);
		if (res == 0) {
			s_unsubs_cnt++;
		}
		unsubs_cnt++;

		res = mqtt_disconnect(g_mqtt_client_handle);
		if (res == 0) {
			s_disconn_cnt++;
		}
		disconn_cnt++;
		ITC_MQTT_WAIT_SIGNAL;

		res = mqtt_deinit_client(g_mqtt_client_handle);
		if (res == 0) {
			s_deinit_cnt++;
		}
		deinit_cnt++;
	}
	printf("\n\t---Success Rate---\n");
	if (init_cnt) {
		TC_ASSERT_EQ("init sucesses rate", s_init_cnt, init_cnt);
		printf("[Initialization MQTT]    Success Rate=%.2f\n", ((s_init_cnt / init_cnt) * 100));
	}
	if (deinit_cnt) {
		TC_ASSERT_EQ("deinit sucesses rate", s_deinit_cnt, deinit_cnt);
		printf("[Deinitialization MQTT]  Success Rate=%.2f\n", ((s_deinit_cnt / deinit_cnt) * 100));
	}
	if (conn_cnt) {
		TC_ASSERT_EQ("connect sucesses rate", s_conn_cnt, conn_cnt);
		printf("[Connect MQTT]           Success Rate=%.2f\n", ((s_conn_cnt / conn_cnt) * 100));
	}
	if (disconn_cnt) {
		TC_ASSERT_EQ("disconnect sucesses rate", s_disconn_cnt, disconn_cnt);
		printf("[Disconnect MQTT]        Success Rate=%.2f\n", ((s_disconn_cnt / disconn_cnt) * 100));
	}
	if (subs_cnt) {
		TC_ASSERT_EQ("subscription sucesses rate", s_subs_cnt, subs_cnt);
		printf("[Subscription MQTT]      Success Rate=%.2f\n", ((s_subs_cnt / subs_cnt) * 100));
	}
	if (unsubs_cnt) {
		TC_ASSERT_EQ("unsubscription sucesses rate", s_unsubs_cnt, unsubs_cnt);
		printf("[Unsubscription MQTT]    Success Rate=%.2f\n", ((s_unsubs_cnt / unsubs_cnt) * 100));
	}
	if (publish_cnt) {
		TC_ASSERT_EQ("publish sucesses rate", s_publish_cnt, publish_cnt);
		printf("[Publish MQTT]           Success Rate=%.2f\n", ((s_publish_cnt / publish_cnt) * 100));
	}
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_publish_p_performance
 * @brief            average performance for mqtt_publish
 * @scenario         average performance for mqtt_publish
 * @apicovered       mqtt_publish, mqtt_init_client, mqtt_connect, mqtt_disconnect,
 *                   mqtt_deinit_client
 * @precondition     mqtt_init_client, mqtt_connect
 * @postcondition    mqtt_disconnect, mqtt_deinit_client
*/

void itc_mqtt_publish_p_performance(void)
{
	double average = 0.0f;
	struct timeval start;
	struct timeval end;
	int res;
	int i;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR,	CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	for (i = 0; i < ITC_MQTT_LOOP_SIZE; i++) {
		if (-1 == gettimeofday(&start, NULL)) {
			printf("gettimeofday() failed\n");
			continue;
		}
		res = mqtt_publish(g_mqtt_client_handle, ITC_MQTT_TOPIC, g_mqtt_msg, sizeof(g_mqtt_msg), 0, 0);
		if (res != 0) {
			printf("mqtt_publish failed cnt: %d\n", i);
			continue;
		}
		if (-1 == gettimeofday(&end, NULL)) {
			printf("gettimeofday() failed\n");
			continue;
		}
		average += diff_time(&start, &end);
		ITC_MQTT_WAIT_SIGNAL;
	}

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	printf("\n[Average publish time: %.2f us]\n", average / ITC_MQTT_LOOP_SIZE);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_subscribe_p_performance
 * @brief            average performance for subscription
 * @scenario         average performance for subscription
 * @apicovered       mqtt_init_client, mqtt_connect, mqtt_subscribe, mqtt_disconnect
 *                   mqtt_deinit_client
 * @precondition     mqtt_init_client, mqtt_connect
 * @postcondition    mqtt_disconnect, mqtt_deinit_client
*/

void itc_mqtt_subscribe_p_performance(void)
{
	double average = 0.0f;
	struct timeval start;
	struct timeval end;
	int res;
	int i;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR,	CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	for (i = 0; i < ITC_MQTT_LOOP_SIZE; i++) {
		if (-1 == gettimeofday(&start, NULL)) {
			printf("gettimeofday() failed\n");
			continue;
		}
		res = mqtt_subscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC, 0);
		if (res != 0) {
			printf("mqtt_subscribe() failed\n");
			continue;
		}
		if (-1 == gettimeofday(&end, NULL)) {
			printf("gettimeofday() failed\n");
			continue;
		}
		average += diff_time(&start, &end);
		res = mqtt_unsubscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC);
		if (res != 0) {
			printf("mqtt_unsubscribe failed cnt: %d\n", i);
		}
	}

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0, mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	printf("\n[Average subscription time: %.2fus]\n",
			average / ITC_MQTT_LOOP_SIZE);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_publish_p_zero_len_msg
 * @brief            sending zero length message
 * @scenario         sending zero length message to publish api
 * @apicovered       mqtt_init_clienti, mqtt_connect, mqtt_publish, mqtt_disconnect,
 *                   mqtt_deinit_client
 * @precondition     mqtt_init_client, mqtt_connect
 * @postcondition    mqtt_disconnect, mqtt_deinit_client
*/

void itc_mqtt_publish_p_zero_len_msg(void)
{
	int res;
	char msg[] = "";
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_ITC_MQTT_BROKER_ADDR, CONFIG_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_connect", res, 0,
			mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_publish(g_mqtt_client_handle, ITC_MQTT_TOPIC, msg, sizeof(msg),
			0, 0);
	TC_ASSERT_EQ_CLEANUP("mqtt_publish", res, 0,
		mqtt_disconnect(g_mqtt_client_handle); mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ_CLEANUP("mqtt_disconnect", res, 0,
			mqtt_deinit_client(g_mqtt_client_handle));
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/*******************************************************************************************/


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_mqtt_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "MQTT ITC") == ERROR) {
		return ERROR;
	}

	int res = _itc_mqtt_init();

	if (res < 0) {
		ITC_MQTT_LOGE;
	} else {
		itc_mqtt_init_deinit_client_p();
		itc_mqtt_init_deinit_client_n();
		itc_mqtt_connect_disconnect_p();
		itc_mqtt_connect_disconnect_n();
		itc_mqtt_publish_p();
		itc_mqtt_publish_n();
		itc_mqtt_subscribe_unsubscribe_p();
		itc_mqtt_subscribe_p_resubscribe();
		itc_mqtt_unsubscribe_p_reunsubscribe();
		itc_mqtt_publish_p_performance();
		itc_mqtt_api_success_ratio_p();
		itc_mqtt_subscribe_p_performance();
		itc_mqtt_publish_p_zero_len_msg();
		itc_mqtt_deinit_n_redeinit_client();
		itc_mqtt_connect_n_reconnect();
		itc_mqtt_disconnect_n_redisconnect();
		_itc_mqtt_deinit();
	}

	(void)tc_handler(TC_END, "MQTT ITC");

	return 0;
}
