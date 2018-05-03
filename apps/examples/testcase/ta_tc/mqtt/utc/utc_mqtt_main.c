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
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <network/mqtt/mqtt_api.h>
#include "tc_common.h"

#define UTC_MQTT_TOPIC "topic_utc"
#define UTC_PUB_CLIENT_ID "utc_mqtt_pub/02"
#define UTC_SUB_CLIENT_ID "utc_mqtt_sub/01"
#define UTC_MQTT_MESSAGE "utc hello mqtt"
#define UTC_MQTT_LOGE printf("%s:%d\n", __FUNCTION__, __LINE__)
#define UTC_MQTT_DEBUG
#ifdef UTC_MQTT_DEBUG
#define UTC_MQTT_LOGD printf
#else
#define UTC_MQTT_LOGD
#endif
#define UTC_MQTT_WAIT_SIGNAL					\
	do {										\
		sem_wait(&g_mqtt_signal);				\
	} while (0)

#define UTC_MQTT_SEND_SIGNAL					\
	do {										\
		sem_post(&g_mqtt_signal);				\
	} while (0)

static char g_mqtt_msg[] = UTC_MQTT_MESSAGE;
static mqtt_client_t *g_mqtt_pub_handle = NULL;
static mqtt_client_t *g_mqtt_sub_handle = NULL;
static sem_t g_mqtt_signal;
/**
 * Private Functions
 */
static int _utc_mqtt_init(void)
{
	g_mqtt_sub_handle = g_mqtt_pub_handle = NULL;

	int ret = sem_init(&g_mqtt_signal, 0, 0);
	if (ret != OK) {
		UTC_MQTT_LOGE;
		return -1;
	}
	return 0;
}

static void _utc_mqtt_deinit(void)
{
	int ret = sem_destroy(&g_mqtt_signal);
	if (ret < 0) {
		UTC_MQTT_LOGE;
	}
	return;
}

static void on_connect(void *client, int result)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		goto send_signal;
	}

	UTC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, result);
send_signal:
	UTC_MQTT_SEND_SIGNAL;
}

static void on_disconnect(void *client, int result)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		goto send_signal;
	}
	UTC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, result);
send_signal:
	UTC_MQTT_SEND_SIGNAL;
}

static void on_publish(void *client, int msg_id)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		goto send_signal;
	}
	UTC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, msg_id);
send_signal:
	UTC_MQTT_SEND_SIGNAL;
}

static void on_message(void *client, mqtt_msg_t *msg)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		return;
	}
	UTC_MQTT_LOGD("%s: %s, %p\n", __FUNCTION__, id->config->client_id, msg);
}

static void on_subscribe(void *client, int msg_id, int qos_count, const int *granted_qos)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		return;
	}
	UTC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, msg_id);
}

static void on_unsubscribe(void *client, int msg_id)
{
	mqtt_client_t *id = (mqtt_client_t *)client;
	if (!id || !id->config) {
		return;
	}
	UTC_MQTT_LOGD("%s: %s, %d\n", __FUNCTION__, id->config->client_id, msg_id);
}


static mqtt_client_config_t g_mqtt_pub_config = {
	UTC_PUB_CLIENT_ID,
	"", "", 1, 3, 0, 0,
	on_connect,
	on_disconnect,
	on_publish,
	on_message,
	on_subscribe,
	on_unsubscribe};

static mqtt_client_config_t g_mqtt_sub_config = {
	UTC_SUB_CLIENT_ID,
	"", "", 1, 3, 0, 0,
	on_connect,
	on_disconnect,
	on_publish,
	on_message,
	on_subscribe,
	on_unsubscribe};


/**
 * @testcase         utc_mqtt_init_client_n
 * @brief            Initialize mqtt stack
 * @scenario         Initialize mqtt stack to connect broker
 * @apicovered       mqtt_init_client
 * @precondition     none
 * @postcondition    none
 */
static void utc_mqtt_init_client_n(void)
{
	mqtt_client_t *res = mqtt_init_client(NULL);
	TC_ASSERT_EQ("mqtt_init_client", res, NULL);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_init_client_p
 * @brief            Initialize mqtt stack
 * @scenario         Initialize mqtt stack to connect broker
 * @apicovered       mqtt_init_client
 * @precondition     none
 * @postcondition    none
 */
static void utc_mqtt_init_client_p(void)
{
	g_mqtt_sub_handle = mqtt_init_client(&g_mqtt_sub_config);
	TC_ASSERT_NEQ("utc_mqtt_init_client_p", g_mqtt_sub_handle, NULL);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_connect_n
 * @brief            connect to broker
 * @scenario         connect to broker not exist, it should return error
 * @apicovered       mqtt_connect
 * @precondition     mqtt_init_client
 * @postcondition    none
 */
static void utc_mqtt_connect_n(void)
{
	int res = mqtt_connect(g_mqtt_sub_handle, "", -1, 0);
	TC_ASSERT_LT("mqtt_connect", res, 0);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_connect_p
 * @brief            connect to broker
 * @scenario         connect to broker not exist, it should return error
 * @apicovered       mqtt_connect
 * @precondition     mqtt_init_client
 * @postcondition    none
 */
static void utc_mqtt_connect_p(void)
{
	int res = mqtt_connect(g_mqtt_sub_handle, CONFIG_EXAMPLES_TESTCASE_MQTT_BROKER_ADDR,
						   CONFIG_EXAMPLES_TESTCASE_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ("mqtt_connect", res, 0);

	UTC_MQTT_WAIT_SIGNAL;
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_subscribe_n
 * @brief            subscribe a message from broker with an invalid input
 * @scenario         subscribe a message
 * @apicovered       mqtt_subscribe
 * @precondition     mqtt_connect
 * @postcondition    none
 */
static void utc_mqtt_subscribe_n(void)
{
	int res = mqtt_subscribe(NULL, UTC_MQTT_TOPIC, 0);
	TC_ASSERT_LT("mqtt_subscribe", res, 0);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_subscribe_p
 * @brief            subscribe a message from broker with a valid input
 * @scenario         subscribe a message
 * @apicovered       mqtt_subscribe
 * @precondition     mqtt_connect
 * @postcondition    none
 */
static void utc_mqtt_subscribe_p(void)
{
	int res = mqtt_subscribe(g_mqtt_sub_handle, UTC_MQTT_TOPIC, 0);
	TC_ASSERT_EQ("mqtt_subscribe", res, 0);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_publish_n
 * @brief            publish a message with an invalid input
 * @scenario         publish a message
 * @apicovered       mqtt_publish
 * @precondition     mqtt_connect
 * @postcondition    none
 */
static void utc_mqtt_publish_n(void)
{
	int res = mqtt_publish(NULL, UTC_MQTT_TOPIC, "", 0, 0, 0);
	TC_ASSERT_LT("mqtt_publish", res, 0);

	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_publish_p
 * @brief            publish a message
 * @scenario         publish a message
 * @apicovered       mqtt_publish
 * @precondition     mqtt_connect
 * @postcondition    none
 */
static void utc_mqtt_publish_p(void)
{
	/* Prepare the publisher*/
	g_mqtt_pub_handle = mqtt_init_client(&g_mqtt_pub_config);
	if (!g_mqtt_pub_handle) {
		UTC_MQTT_LOGE;
		return ;
	}

	int res = mqtt_connect(g_mqtt_pub_handle, CONFIG_EXAMPLES_TESTCASE_MQTT_BROKER_ADDR,
						   CONFIG_EXAMPLES_TESTCASE_MQTT_BROKER_PORT, 0);
	if (res < 0) {
		UTC_MQTT_LOGE;
		return ;
	}

	UTC_MQTT_WAIT_SIGNAL;
	res = mqtt_publish(g_mqtt_pub_handle, UTC_MQTT_TOPIC, g_mqtt_msg, sizeof(g_mqtt_msg), 0, 0);
	TC_ASSERT_EQ("mqtt_publish", res, 0);
	UTC_MQTT_WAIT_SIGNAL;
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_unsubscribe_n
 * @brief            stop subscribing from a broker
 * @scenario         call mqtt_unsubscribe with an invalid input
 * @apicovered       mqtt_unsubscribe
 * @precondition     mqtt_subscribe
 * @postcondition    none
 */
static void utc_mqtt_unsubscribe_n(void)
{
	int res = mqtt_unsubscribe(NULL, NULL);
	TC_ASSERT_LT("mqtt_unsubscribe", res, 0);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_unsubscribe_p
 * @brief            stop subscribing from a broker
 * @scenario         call mqtt_unsubscribe
 * @apicovered       mqtt_unsubscribe
 * @precondition     mqtt_subscribe
 * @postcondition    none
 */
static void utc_mqtt_unsubscribe_p(void)
{
	int res = mqtt_unsubscribe(g_mqtt_sub_handle, UTC_MQTT_TOPIC);
	TC_ASSERT_EQ("mqtt_unsbscribe", res, 0);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_disconnect_n
 * @brief            disconnect to broker
 * @scenario         disconnect to broker using an invalid handle
 * @apicovered       mqtt_disconnect
 * @precondition     mqtt_connect
 * @postcondition    none
 */
static void utc_mqtt_disconnect_n(void)
{
	int res = mqtt_disconnect(NULL);
	TC_ASSERT_LT("mqtt_disconnect", res, 0);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_disconnect_p
 * @brief            disconnect to broker
 * @scenario         disconnect to broker using a valid handle
 * @apicovered       mqtt_disconnect
 * @precondition     mqtt_connect
 * @postcondition    none
 */
static void utc_mqtt_disconnect_p(void)
{
	int res = mqtt_disconnect(g_mqtt_sub_handle);
	TC_ASSERT_EQ("mqtt_disconnect", res, 0);

	UTC_MQTT_WAIT_SIGNAL;
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_deinit_client_n
 * @brief            free all resources used to mqtt stack
 * @scenario         call mqtt_deinit with an invalid input
 * @apicovered       mqtt_deinit_client
 * @precondition     mqtt_init
 * @postcondition    none
 */
static void utc_mqtt_deinit_client_n(void)
{
	int res = mqtt_deinit_client(NULL);
	TC_ASSERT_LT("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_mqtt_deinit_client_p
 * @brief            free all resources used to mqtt stack
 * @scenario         call mqtt_deinit with an invalid input
 * @apicovered       mqtt_deinit_client
 * @precondition     mqtt_init
 * @postcondition    none
 */
static void utc_mqtt_deinit_client_p(void)
{
	int res = mqtt_deinit_client(g_mqtt_pub_handle);
	if (res < 0) {
		UTC_MQTT_LOGE;
	}
	res = mqtt_deinit_client(g_mqtt_sub_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_mqtt_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "MQTT UTC") == ERROR) {
		return ERROR;
	}

	int res = _utc_mqtt_init();
	if (res < 0) {
		UTC_MQTT_LOGE;
		goto exit;
	}
	utc_mqtt_init_client_n();
	utc_mqtt_init_client_p();
	utc_mqtt_connect_n();
	utc_mqtt_connect_p();
	utc_mqtt_subscribe_n();
	utc_mqtt_subscribe_p();
	utc_mqtt_publish_n();
	utc_mqtt_publish_p();
	utc_mqtt_unsubscribe_n();
	utc_mqtt_unsubscribe_p();
	utc_mqtt_disconnect_n();
	utc_mqtt_disconnect_p();
	utc_mqtt_deinit_client_n();
	utc_mqtt_deinit_client_p();

	_utc_mqtt_deinit();
exit:
	(void)tc_handler(TC_END, "MQTT UTC");

	return 0;
}
