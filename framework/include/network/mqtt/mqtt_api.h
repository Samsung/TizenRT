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
 * @defgroup MQTT MQTT Client
 * @brief Provides APIs for MQTT Client
 * @ingroup NETWORK
 * @{
 */
/**
 * @file network/mqtt/mqtt_api.h
 * @brief APIs for MQTT Client(Subscriber/Publisher)
 */

#ifndef __MQTT_API_H__
#define __MQTT_API_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MQTT_DEFAULT_BROKER_PORT			1883
#define MQTT_SECURITY_BROKER_PORT			8883
#define MQTT_DEFAULT_KEEP_ALIVE_TIME		60
#define MQTT_PROTOCOL_VERSION_31			3
#define MQTT_PROTOCOL_VERSION_311			4

/****************************************************************************
 * Enumeration
 ****************************************************************************/
/**
 * @brief This enumeration describes the state of the MQTT client
 */
enum mqtt_client_state_e {
	MQTT_CLIENT_STATE_NOT_CONNECTED = 0,
	MQTT_CLIENT_STATE_CONNECTED,
	MQTT_CLIENT_STATE_CONNECT_REQUEST,
	MQTT_CLIENT_STATE_DISCONNECT_REQUEST,
};

/**
 * @brief This enumeration describes the state of the MQTT connection result
 */
enum mqtt_connection_result_e {
	MQTT_CONN_ACCEPTED = 0,
	MQTT_CONN_REFUSED_UNACCEPTABLE_PROTOCOL_VER,
	MQTT_CONN_REFUSED_ID_REJECTED,
	MQTT_CONN_REFUSED_BROKER_UNAVAILABLE,
	MQTT_CONN_REFUSED_BAD_USER_NAME_OR_PASSWORD,
	MQTT_CONN_REFUSED_NOT_AUTHORIZED,
};

/****************************************************************************
 * Public Types
 ****************************************************************************/
/**
 * @brief Structure of MQTT message
 */
struct _mqtt_msg_s {
	int msg_id;	/**< message identification */
	char *topic; /**< message topic */
	void *payload; /**< message payload content */
	int payload_len; /**< message pay load length */
	int qos; /**< message qos flag */
	int retain;	/**< message retain flag */
};

typedef struct _mqtt_msg_s mqtt_msg_t;

/**
 * @brief Structure of MQTT security information
 */
struct _mqtt_tls_param_s {
	const unsigned char *ca_cert;	/* CA certificate, common between client and MQTT Broker */
	const unsigned char *cert;	/* Client certificate */
	const unsigned char *key;	/* Client private key */
	int ca_cert_len;			/* the length of CA certificate  */
	int cert_len;				/* the length of Client certificate */
	int key_len;				/* the length of key */
};

typedef struct _mqtt_tls_param_s mqtt_tls_param_t;

/**
 * @brief Structure of MQTT client configuration
 */
struct _mqtt_client_config_s {
	char *client_id; /**< client identification */
	char *user_name; /**< client user name */
	char *password;	/**< password corresponding to the user name */
	bool clean_session;	/**< mqtt clean session flag */
	int protocol_version;	/**< mqtt protocol version */
	bool debug;	/**< mqtt debug flag */
	mqtt_tls_param_t *tls; /**< mqtt tls parameter */

	void (*on_connect)(void *client, int result);
	/**< on_connect call back function */
	void (*on_disconnect)(void *client, int result);
	/**< on_disconnect call back function */
	void (*on_publish)(void *client, int msg_id);
	/**< on_publish call back function */
	void (*on_message)(void *client, mqtt_msg_t *msg);
	/**< on_message call back function */
	void (*on_subscribe)(void *client, int msg_id, int qos_count, const int *granted_qos);
	/**< on_subscribe call back function */
	void (*on_unsubscribe)(void *client, int msg_id);
	/**< on_unsubscribe call back function */

	void *user_data; /**< user defined data */
};

typedef struct _mqtt_client_config_s mqtt_client_config_t;

/**
 * @brief Structure of MQTT client object
 */
struct _mqtt_client_s {
	int lib_version; /**< mqtt library version */
	void *mosq;	/**< mqtt library client pointer */
	mqtt_client_config_t *config; /**< mqtt config */
	int state; /**< mqtt client state */
};

typedef struct _mqtt_client_s mqtt_client_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/**
 * @brief mqtt_init_client() initializes MQTT client
 *
 * @details @b #include <network/mqtt/mqtt_api.h>
 * @param[in] config the information of MQTT client object configuration
 * @return On success, the handle of MQTT client object is returned. On failure, NULL is returned.
 * @since TizenRT v1.1
 */
mqtt_client_t *mqtt_init_client(mqtt_client_config_t *config);

/**
 * @brief mqtt_deinit_client() de-initializes MQTT client
 *
 * @details @b #include <network/mqtt/mqtt_api.h>
 * @param[in] handle the handle of MQTT client object
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.1
 */
int mqtt_deinit_client(mqtt_client_t *handle);

/**
 * @brief mqtt_connect() connects to a MQTT broker
 *
 * @details @b #include <network/mqtt/mqtt_api.h>
 * @param[in] handle the handle of MQTT client object
 * @param[in] addr MQTT broker address
 * @param[in] port MQTT broker port
 * @param[in] keep_alive MQTT keep-alive time in second
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.1
 */
int mqtt_connect(mqtt_client_t *handle, char *addr, int port, int keep_alive);

/**
 * @brief mqtt_disconnect() disconnects from a MQTT broker
 *
 * @details @b #include <network/mqtt/mqtt_api.h>
 * @param[in] handle the handle of MQTT client object
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.1
 */
int mqtt_disconnect(mqtt_client_t *handle);

/**
 * @brief mqtt_publish() pusblishes message to a MQTT broker on the given topic
 *
 * @details @b #include <network/mqtt/mqtt_api.h>
 * @param[in] handle the handle of MQTT client object
 * @param[in] topic the topic on which the message to be published
 * @param[in] data the message to publish
 * @param[in] data_len the length of message
 * @param[in] qos the Quality of Service to be used for the message. QoS value should be 0,1 or 2.
 * @param[in] retain the flag to make the message retained.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.1
 */
int mqtt_publish(mqtt_client_t *handle, char *topic, char *data, uint32_t data_len, uint8_t qos, uint8_t retain);

/**
 * @brief mqtt_subscribe() subscribes for the specified topic with MQTT broker
 *
 * @details @b #include <network/mqtt/mqtt_api.h>
 * @param[in] handle the handle of MQTT client object
 * @param[in] topic the topic on which the message to be unsubscribed
 * @param[in] qos the Quality of Service for the subscription.  QoS value should be 0,1 or 2.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.1
 */
int mqtt_subscribe(mqtt_client_t *handle, char *topic, uint8_t qos);

/**
 * @brief mqtt_unsubscribe() unsubscribes from the specified topic
 *
 * @details @b #include <network/mqtt/mqtt_api.h>
 * @param[in] handle the handle of MQTT client object
 * @param[in] topic the topic on which the message to be unsubscribed
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.1
 */
int mqtt_unsubscribe(mqtt_client_t *handle, char *topic);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __MQTT_API_H__ */

/** @} */// end of MQTT group
