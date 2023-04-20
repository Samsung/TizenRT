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
 * @file mqtt_api.c
 * @brief MQTT API
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <debug.h>
#include <errno.h>

#include "mosquitto.h"
#include "mosquitto_internal.h"
#include "memory_mosq.h"

#include <network/mqtt/mqtt_api.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions Prototype
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static mqtt_client_t *create_mqtt_client(void)
{
	mqtt_client_t *mqtt_client = NULL;
	mqtt_client = (mqtt_client_t *)mosquitto__malloc(sizeof(mqtt_client_t));
	if (mqtt_client == NULL) {
		ndbg("ERROR: mqtt_client is null.\n");
	}

	return mqtt_client;
}

static void destroy_mqtt_client(mqtt_client_t *client)
{
	if (client) {
		if (client->mosq) {
			mosquitto_destroy((struct mosquitto *)client->mosq);
		}
		client->mosq = NULL;

		mosquitto_lib_cleanup();

		mosquitto__free(client);
	}
}

static void on_connect_callback(struct mosquitto *client, void *data, int result)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)data;

	if (mqtt_client) {
		mqtt_client->state = MQTT_CLIENT_STATE_CONNECTED;
		if (mqtt_client->config && mqtt_client->config->on_connect) {
			mqtt_client->config->on_connect(mqtt_client, result);
		}
	}
}

static void on_disconnect_callback(struct mosquitto *client, void *data, int result)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)data;

	if (mqtt_client) {
		mqtt_client->state = MQTT_CLIENT_STATE_NOT_CONNECTED;
		if (mqtt_client->config && mqtt_client->config->on_disconnect) {
			mqtt_client->config->on_disconnect(mqtt_client, result);
		}
	}
}

static void on_message_callback(struct mosquitto *client, void *data, const struct mosquitto_message *msg)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)data;
	mqtt_msg_t *received_msg;

	received_msg = (mqtt_msg_t *)mosquitto__malloc(sizeof(mqtt_msg_t));
	if (!received_msg) {
		ndbg("ERROR: received_msg is null.");
		return;
	}

	received_msg->msg_id = msg->mid;
	received_msg->topic = msg->topic;
	received_msg->payload = msg->payload;
	received_msg->payload_len = msg->payloadlen;
	received_msg->qos = msg->qos;
	received_msg->retain = msg->retain;

	if (mqtt_client && mqtt_client->config && mqtt_client->config->on_message) {
		mqtt_client->config->on_message(mqtt_client, received_msg);
	}

	mosquitto__free(received_msg);
}

static void on_publish_callback(struct mosquitto *client, void *data, int msg_id)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)data;

	if (mqtt_client) {
		if (mqtt_client->config && mqtt_client->config->on_publish) {
			mqtt_client->config->on_publish(mqtt_client, msg_id);
		}
	}
}

static void on_subscribe_callback(struct mosquitto *client, void *data, int msg_id, int qos_count, const int *granted_qos)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)data;

	if (mqtt_client) {
		if (mqtt_client->config && mqtt_client->config->on_subscribe) {
			mqtt_client->config->on_subscribe(mqtt_client, msg_id, qos_count, granted_qos);
		}
	}
}

static void on_unsubscribe_callback(struct mosquitto *client, void *data, int msg_id)
{
	mqtt_client_t *mqtt_client = (mqtt_client_t *)data;

	if (mqtt_client) {
		if (mqtt_client->config && mqtt_client->config->on_unsubscribe) {
			mqtt_client->config->on_unsubscribe(mqtt_client, msg_id);
		}
	}
}

static void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
	printf("%s\n", str);
}

static void get_mqtt_client_state_string(int state, char *result_str)
{
	switch (state) {
	case MQTT_CLIENT_STATE_NOT_CONNECTED:
		snprintf(result_str, 20, "NOT_CONNECTED");
		break;
	case MQTT_CLIENT_STATE_CONNECTED:
		snprintf(result_str, 20, "CONNECTED");
		break;
	case MQTT_CLIENT_STATE_CONNECT_REQUEST:
		snprintf(result_str, 20, "CONNECT_REQUEST");
		break;
	case MQTT_CLIENT_STATE_DISCONNECT_REQUEST:
		snprintf(result_str, 20, "DISCONNECT_REQUEST");
		break;
	default:
		snprintf(result_str, 20, "UNKNOWN");
		break;
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mqtt_init_client
 *
 * Description:
 *	 Initializes MQTT object.
 *
 * Parameters:
 *     config : the information of MQTT client object configuration
 *
 * Returned Value:
 *	 On success, the handle of MQTT client object is returned. On failure, NULL is returned.
 *
 ****************************************************************************/
mqtt_client_t *mqtt_init_client(mqtt_client_config_t *config)
{
	int result = -1;
	mqtt_client_t *mqtt_client = NULL;
	int ret = 0;
	int major, minor, revision;

	if (config == NULL) {
		ndbg("ERROR: mqtt config is null.\n");
		goto done;
	}

	mqtt_client = create_mqtt_client();
	if (!mqtt_client) {
		ndbg("ERROR: mqtt_client is null.\n");
		goto done;
	}

	memset(mqtt_client, 0, sizeof(*mqtt_client));

	mosquitto_lib_version(&major, &minor, &revision);
	mqtt_client->lib_version = major * 1000000 + minor * 1000 + revision;
	mqtt_client->config = config;
	mqtt_client->state = MQTT_CLIENT_STATE_NOT_CONNECTED;
	mosquitto_lib_init();

	mqtt_client->mosq = mosquitto_new(config->client_id, config->clean_session, NULL);
	if (!mqtt_client->mosq) {
		switch (errno) {
		case ENOMEM:
			ndbg("ERROR: mosquitto_new() failed. (reason: out of memory)\n");
			break;
		case EINVAL:
			ndbg("ERROR: mosquitto_new() failed. (reason: invalid id)\n");
			break;
		}
		goto done;
	}

	if (config->user_name != NULL && config->password != NULL) {
		ret = mosquitto_username_pw_set((struct mosquitto *)mqtt_client->mosq, config->user_name, config->password);
		if (ret != MOSQ_ERR_SUCCESS) {
			goto done;
		}
	}

	/* set userdata */
	mosquitto_user_data_set((struct mosquitto *)mqtt_client->mosq, mqtt_client);

	/* set callback */
	if (config->debug) {
		mosquitto_log_callback_set((struct mosquitto *)mqtt_client->mosq, my_log_callback);
	}
	mosquitto_connect_callback_set((struct mosquitto *)mqtt_client->mosq, on_connect_callback);
	mosquitto_disconnect_callback_set((struct mosquitto *)mqtt_client->mosq, on_disconnect_callback);
	mosquitto_publish_callback_set((struct mosquitto *)mqtt_client->mosq, on_publish_callback);
	mosquitto_message_callback_set((struct mosquitto *)mqtt_client->mosq, on_message_callback);
	mosquitto_subscribe_callback_set((struct mosquitto *)mqtt_client->mosq, on_subscribe_callback);
	mosquitto_unsubscribe_callback_set((struct mosquitto *)mqtt_client->mosq, on_unsubscribe_callback);

	/* set protocol version */
	ret = mosquitto_opts_set((struct mosquitto *)mqtt_client->mosq, MOSQ_OPT_PROTOCOL_VERSION, &(config->protocol_version));
	if (ret != MOSQ_ERR_SUCCESS) {
		ndbg("ERROR: fail to set mqtt protocol version.\n");
		goto done;
	}
#if defined(CONFIG_NETUTILS_MQTT_SECURITY)
	if (config->tls) {
		struct mosquitto *tmp = (struct mosquitto *)mqtt_client->mosq;

		tmp->tls_ca_cert_len = config->tls->ca_cert_len;
		tmp->tls_cert_len = config->tls->cert_len;
		tmp->tls_key_len = config->tls->key_len;

		ret = mosquitto_tls_set(tmp, (const char *)config->tls->ca_cert, NULL, (const char *)config->tls->cert, (const char *)config->tls->key, NULL);
		if (ret != 0) {
			ndbg("ERROR: mosquitto_tls_set() failed. (ret: %d)\n", ret);
			goto done;
		}
	}
#else
	if (config->tls) {
		ndbg("this version doesn't support MQTT over TLS.\n");
	}
#endif
	/* result is success */
	result = 0;

done:
	if (result != 0) {
		destroy_mqtt_client(mqtt_client);
		mqtt_client = NULL;
	}
	return mqtt_client;
}

/****************************************************************************
 * Name: mqtt_deinit_client
 *
 * Description:
 *	 De-initializes MQTT object.
 *
 * Parameters:
 *     handle : the handle of MQTT client object
 *
 * Returned Value:
 *	 On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mqtt_deinit_client(mqtt_client_t *handle)
{
	int result = -1;

	if (handle) {
		destroy_mqtt_client(handle);
		result = 0;
	}

	return result;
}

/****************************************************************************
 * Name: mqtt_connect
 *
 * Description:
 *	 Establishes connection with MQTT broker.
 *
 * Parameters:
 *     handle : the handle of MQTT client object
 *     addr : MQTT broker address
 *     port : MQTT broker port
 *     keep_alive : MQTT keep-alive time in second
 *
 * Returned Value:
 *	 On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mqtt_connect(mqtt_client_t *handle, char *addr, int port, int keep_alive)
{
	int result = -1;
	int ret = 0;
	struct mosquitto *mosq = NULL;
	mqtt_client_config_t *mqtt_config = NULL;
	int prev_state = 0;

	if (handle == NULL) {
		ndbg("ERROR: mqtt_client handle is null.\n");
		goto done;
	}

	mosq = (struct mosquitto *)handle->mosq;
	if (mosq == NULL) {
		ndbg("ERROR: mosquitto handle is null.\n");
		goto done;
	}

	if (handle->state == MQTT_CLIENT_STATE_CONNECTED) {
		ndbg("ERROR: mqtt_client is already connected.\n");
		result = 0;
		goto done;
	}

	if (handle->state > MQTT_CLIENT_STATE_CONNECTED) {
		char state_str[20];
		get_mqtt_client_state_string(handle->state, state_str);
		ndbg("ERROR: mqtt_client is busy.(current state: %s)\n", state_str);
		goto done;
	}

	mqtt_config = handle->config;
	if (mqtt_config == NULL) {
		ndbg("ERROR: mqtt_config is null.\n");
		goto done;
	}

	if (addr == NULL) {
		ndbg("ERROR: addr is null.\n");
		goto done;
	}

	if (port == 0) {
		port = MQTT_DEFAULT_BROKER_PORT;
	}

	prev_state = handle->state;
	handle->state = MQTT_CLIENT_STATE_CONNECT_REQUEST;
	ret = mosquitto_connect(mosq, addr, port, keep_alive);
	if (ret != 0) {
		ndbg("ERROR: mosquitto_connect() failed. (ret: %d)\n", ret);
		handle->state = prev_state;
		goto done;
	}

	if (!mosq->threaded) {
		ret = mosquitto_loop_start(mosq);
		if (ret != 0) {
			ndbg("ERROR: mosquitto_loop_start() failed. (ret: %d)\n", ret);
			ret = mosquitto_disconnect(mosq);
			if (ret != 0) {
				ndbg("ERROR: mosquitto_disconnect() failed. (ret: %d)\n", ret);
			}
			goto done;
		}
	}

	/* result is success */
	result = 0;

done:
	return result;
}

/****************************************************************************
 * Name: mqtt_disconnect
 *
 * Description:
 *	 Disconnect from MQTT broker.
 *
 * Parameters:
 *     handle : the handle of MQTT client object
 *
 * Returned Value:
 *	 On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mqtt_disconnect(mqtt_client_t *handle)
{
	int result = -1;
	int ret = 0;
	struct mosquitto *mosq = NULL;
	int prev_state = 0;

	if (handle == NULL) {
		ndbg("ERROR: mqtt_client handle is null.\n");
		goto done;
	}

	mosq = (struct mosquitto *)handle->mosq;
	if (mosq == NULL) {
		ndbg("ERROR: mosquitto handle is null.\n");
		goto done;
	}

	if (handle->state == MQTT_CLIENT_STATE_NOT_CONNECTED) {
		ndbg("ERROR: mqtt_client is already disconnected.\n");
		result = 0;
		goto done;
	}

	if (handle->state == MQTT_CLIENT_STATE_DISCONNECT_REQUEST) {
		char state_str[20];
		get_mqtt_client_state_string(handle->state, state_str);
		ndbg("ERROR: mqtt_client is busy. (current state: %s)\n", state_str);
		goto done;
	}

	prev_state = handle->state;
	handle->state = MQTT_CLIENT_STATE_DISCONNECT_REQUEST;
	ret = mosquitto_disconnect(mosq);
	if (ret != 0) {
		ndbg("ERROR: mosquitto_disconnect() failed. (ret=%d)\n", ret);
		if (ret == MOSQ_ERR_NO_CONN) {
			ndbg("the return value of mosquitto_disconnect() is MOSQ_ERR_NO_CONN.\n");
			handle->state = MQTT_CLIENT_STATE_NOT_CONNECTED;
			result = 0;
		} else {
			handle->state = prev_state;
		}

		goto done;
	}

	/* result is success */
	result = 0;

done:
	return result;
}

/****************************************************************************
 * Name: mqtt_publish
 *
 * Description:
 *	 Publish message to MQTT Broker on the given Topic.
 *
 * Parameters:
 *     handle : the handle of MQTT client object
 *     topic : the topic on which the message to be published
 *     data : the message to publish
 *     data_len : the length of message
 *     qos : the Quality of Service to be used for the message. QoS value should be 0,1 or 2.
 *     retain : the flag to make the message retained
 *
 * Returned Value:
 *	 On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mqtt_publish(mqtt_client_t *handle, char *topic, char *data, uint32_t data_len, uint8_t qos, uint8_t retain)
{
	int result = -1;
	int ret = 0;
	struct mosquitto *mosq = NULL;

	if (handle == NULL) {
		ndbg("ERROR: mqtt_client handle is null.\n");
		goto done;
	}

	mosq = (struct mosquitto *)handle->mosq;
	if (mosq == NULL) {
		ndbg("ERROR: mosquitto handle is null.\n");
		goto done;
	}

	if (handle->state == MQTT_CLIENT_STATE_NOT_CONNECTED) {
		ndbg("ERROR: mqtt_client is disconnected.\n");
		goto done;
	}

	if (handle->state > MQTT_CLIENT_STATE_CONNECTED) {
		char state_str[20];
		get_mqtt_client_state_string(handle->state, state_str);
		ndbg("ERROR: mqtt_client is busy. (current state: %s)\n", state_str);
		goto done;
	}

	if (topic == NULL) {
		ndbg("ERROR: topic is null.\n");
		goto done;
	}

	if (qos > 2) {
		ndbg("ERROR: invalid qos: %d (valid range: 0 ~ 2)\n", qos);
		goto done;
	}

	ret = mosquitto_publish(mosq, NULL, (const char *)topic, data_len, data, qos, retain != 0 ? true : false);
	if (ret != 0) {
		ndbg("ERROR: mosquitto_publish() failed. (ret: %d)\n", ret);
		handle->state = MQTT_CLIENT_STATE_CONNECTED;
		goto done;
	}

	/* result is success */
	result = 0;

done:
	return result;
}

/****************************************************************************
 * Name: mqtt_subscribe
 *
 * Description:
 *	 Subscribe for a topic with MQTT Broker.
 *
 * Parameters:
 *     handle  the handle of MQTT client object
 *     topic  the topic on which the message to be unsubscribed
 *     qos  the Quality of Service for the subscription.  QoS value should be 0,1 or 2.
 *
 * Returned Value:
 *	 On success, the handle of MQTT client object is returned. On failure, NULL is returned.
 *
 ****************************************************************************/
int mqtt_subscribe(mqtt_client_t *handle, char *topic, uint8_t qos)
{
	int result = -1;
	int ret = 0;
	struct mosquitto *mosq = NULL;

	if (handle == NULL) {
		ndbg("ERROR: mqtt_client handle is null.\n");
		goto done;
	}

	mosq = (struct mosquitto *)handle->mosq;
	if (mosq == NULL) {
		ndbg("ERROR: mosquitto handle is null.\n");
		goto done;
	}

	if (handle->state == MQTT_CLIENT_STATE_NOT_CONNECTED) {
		ndbg("ERROR: mqtt_client is disconnected.\n");
		goto done;
	}

	if (handle->state > MQTT_CLIENT_STATE_CONNECTED) {
		char state_str[20];
		get_mqtt_client_state_string(handle->state, state_str);
		ndbg("ERROR: mqtt_client is busy. (current state: %s)\n", state_str);
		goto done;
	}

	if (topic == NULL) {
		ndbg("ERROR: topic is null.\n");
		goto done;
	}

	if (qos > 2) {
		ndbg("ERROR: invalid qos: %d (valid range: 0 ~ 2) \n", qos);
		goto done;
	}

	ret = mosquitto_subscribe(mosq, NULL, (const char *)topic, qos);
	if (ret != 0) {
		ndbg("ERROR: mqtt_subscribe() failed. (ret: %d)\n", ret);
		handle->state = MQTT_CLIENT_STATE_CONNECTED;
		goto done;
	}

	/* result is success */
	result = 0;

done:
	return result;
}

/****************************************************************************
 * Name: mqtt_unsubscribe
 *
 * Description:
 *	 Unsubscribe the topic from MQTT Broker.
 *
 * Parameters:
 *     handle  the handle of MQTT client object
 *     topic  the topic on which the message to be unsubscribed
 *
 * Returned Value:
 *	 On success, the handle of MQTT client object is returned. On failure, NULL is returned.
 *
 ****************************************************************************/
int mqtt_unsubscribe(mqtt_client_t *handle, char *topic)
{
	int result = -1;
	int ret = 0;
	struct mosquitto *mosq = NULL;

	if (handle == NULL) {
		ndbg("ERROR: mqtt_client handle is null.\n");
		goto done;
	}

	mosq = (struct mosquitto *)handle->mosq;
	if (mosq == NULL) {
		ndbg("ERROR: mosquitto handle is null.\n");
		goto done;
	}

	if (handle->state == MQTT_CLIENT_STATE_NOT_CONNECTED) {
		ndbg("ERROR: mqtt_client is disconnected.\n");
		goto done;
	}

	if (handle->state > MQTT_CLIENT_STATE_CONNECTED) {
		char state_str[20];
		get_mqtt_client_state_string(handle->state, state_str);
		ndbg("ERROR: mqtt_client is busy. (current state: %s)\n", state_str);
		goto done;
	}

	if (topic == NULL) {
		ndbg("ERROR: topic is null.\n");
		goto done;
	}

	ret = mosquitto_unsubscribe(mosq, NULL, (const char *)topic);
	if (ret != 0) {
		ndbg("ERROR: mosquitto_unsubscribe() failed. (ret: %d)\n");
		handle->state = MQTT_CLIENT_STATE_CONNECTED;
		goto done;
	}

	/* result is success */
	result = 0;

done:
	return result;
}
