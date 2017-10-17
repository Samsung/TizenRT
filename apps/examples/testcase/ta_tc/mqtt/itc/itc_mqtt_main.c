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
#include <apps/shell/tash.h>
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

#define ITC_MQTT_WAIT_SIGNAL \
	do {\
		pthread_mutex_lock(&g_ocfmutex);\
		pthread_cond_wait(&g_ocfcond, &g_ocfmutex);\
		pthread_mutex_unlock(&g_ocfmutex);\
	} while (0)

#define ITC_MQTT_SEND_SIGNAL\
	do {\
		pthread_mutex_lock(&g_ocfmutex);\
		pthread_cond_signal(&g_ocfcond);\
		pthread_mutex_unlock(&g_ocfmutex);\
	} while (0)

static mqtt_client_t *g_mqtt_client_handle;
static pthread_mutex_t g_ocfmutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t g_ocfcond;
extern int working_tc;
extern sem_t tc_sem;
static char g_mqtt_msg[] = ITC_MQTT_MESSAGE;

/****************************************************************************************
 *Private Functions
 ****************************************************************************************/

static int _itc_mqtt_init(void)
{
	g_mqtt_client_handle = NULL;

	int res = pthread_mutex_init(&g_ocfmutex, NULL);
	if (res != 0) {
		ITC_MQTT_LOGE;
		return -1;
	}
	res = pthread_cond_init(&g_ocfcond, NULL);
	if (res != 0) {
		ITC_MQTT_LOGE;
		return -1;
	}
	return 0;
}

static void _itc_mqtt_deinit(void)
{
	int res = pthread_mutex_destroy(&g_ocfmutex);
	if (res != 0) {
		ITC_MQTT_LOGE;
	}
	res = pthread_cond_destroy(&g_ocfcond);
	if (res != 0) {
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
 * @testcase             itc_mqtt_init_client_p
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

/*
 * @testcase             itc_mqtt_connect_disconnect_p
 * @brief                To check mqtt_connect
 * @scenario             If used api returns not 0
 * @apicovered           mqtt_connect & mqtt_disconnect
 * @precondition         initialize g_mqt_client_config and initialize
 * @postcondition        freeResources
*/

void itc_mqtt_connect_disconnect_p(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);

	// MQTT Connect
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_EXAMPLES_TESTCASE_ITC_MQTT_BROKER_ADDR, CONFIG_EXAMPLES_TESTCASE_ITC_MQTT_BROKER_PORT, 0);
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

/*
 * @testcase             itc_mqtt_publish_p
 * @brief                To check mqtt_connect
 * @scenario             If used api returns not 0
 * @apicovered           mqtt_publish
 * @precondition         mqtt_init_client and mqtt_connect
 * @postcondition        freeResources
*/

void itc_mqtt_publish_p(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_EXAMPLES_TESTCASE_ITC_MQTT_BROKER_ADDR, CONFIG_EXAMPLES_TESTCASE_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ("mqtt_connect", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_publish(g_mqtt_client_handle, ITC_MQTT_TOPIC, g_mqtt_msg, sizeof(g_mqtt_msg), 0, 0);
	TC_ASSERT_EQ("mqtt_publish", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_disconnect", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_mqtt_subscribe_unsubscribe_p
 * @brief            stop subscribing from a broker
 * @scenario         call mqtt_unsubscribe
 * @apicovered       mqtt_unsubscribe
 * @precondition     mqtt_subscribe
 * @postcondition    none
 */

void itc_mqtt_subscribe_unsubscribe_p(void)
{
	int res;
	g_mqtt_client_handle = mqtt_init_client(&g_mqtt_client_config);
	TC_ASSERT_NEQ("mqtt_init_client", g_mqtt_client_handle, NULL);
	res = mqtt_connect(g_mqtt_client_handle, CONFIG_EXAMPLES_TESTCASE_ITC_MQTT_BROKER_ADDR, CONFIG_EXAMPLES_TESTCASE_ITC_MQTT_BROKER_PORT, 0);
	TC_ASSERT_EQ("mqtt_connect", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_subscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC, 0);
	TC_ASSERT_EQ("mqtt_subscribe", res, 0);

	res = mqtt_unsubscribe(g_mqtt_client_handle, ITC_MQTT_TOPIC);
	TC_ASSERT_EQ("mqtt_unsubscribe", res, 0);

	res = mqtt_disconnect(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_disconnect", res, 0);
	ITC_MQTT_WAIT_SIGNAL;

	res = mqtt_deinit_client(g_mqtt_client_handle);
	TC_ASSERT_EQ("mqtt_deinit_client", res, 0);
	TC_SUCCESS_RESULT();
}

static int mqtt_itc(int arc, FAR char *argv[])
{
	sem_wait(&tc_sem);
	working_tc++;

	total_fail = total_pass = 0;

	printf("=== TIZENRT MQTT ITC START! ===\n");
	int res = _itc_mqtt_init();

	if (res < 0) {
		ITC_MQTT_LOGE;
	} else {
		itc_mqtt_init_deinit_client_p();
		itc_mqtt_connect_disconnect_p();
		itc_mqtt_publish_p();
		itc_mqtt_subscribe_unsubscribe_p();
		_itc_mqtt_deinit();
	}
	printf("\n=== TIZENRT MQTT ITC COMPLETE ===\n");
	printf("\t\tTotal pass : %d\n\t\tTotal fail : %d\n", total_pass, total_fail);

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_mqtt_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TASH
	tash_cmd_install("mqtt_itc", mqtt_itc, TASH_EXECMD_SYNC);
#else
	mqtt_itc(argc, argv);
#endif
	return 0;
}

