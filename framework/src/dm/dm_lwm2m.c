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
 * @defgroup LWM2M LWM2M
 * @ingroup DM
 * @{
 */

/**
 * @file dm_lwm2m.c
 * @brief device management APIs for DM
 */

#include "lwm2mclient.h"
#include "connectivity_interface.h"
#include "internals.h"
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>

#include <dm/dm_lwm2m.h>
#include <dm/dm_error.h>

static pthread_t wakaama_thread;
static pthread_attr_t wakaama_attr;
static struct sched_param wakaama_sparam;

static struct dm_lwm2m_context_s g_dm_context;

dm_lwm2m_client_state_e dm_state = DM_LWM2M_CLIENT_STOPPED;

static void prv_copy_dm_context(struct dm_lwm2m_context_s *dm_context)
{
	memcpy(&g_dm_context, dm_context, sizeof(struct dm_lwm2m_context_s));
}

static void prv_initiate_bootstrap(void)
{
	client_initiate_bootstrap();
}

/**
 * @brief Start a DM client
 *
 * @param[in] dm_context
 * @return On success, 0 is returned.
 *         On failure, a negative value is returned.
 *         If client is already started, return DM_ERROR_ALREADY_STARTED.
 */
int dm_lwm2m_start_client(struct dm_lwm2m_context_s *dm_context)
{

	int ret;
	int len, i;

	if (dm_state == DM_LWM2M_CLIENT_STARTED) {
		return DM_ERROR_ALREADY_STARTED;
	}
	if (dm_context == NULL) {
		return DM_ERROR_INVALID_PARAMETER;
	}
	/* Set server Ip Address*/
	if (dm_context->server_info.ipAddress != NULL) {
		client_set_serverAddr(dm_context->server_info.ipAddress, dm_context->server_info.isBootstrap);
	} else {
		return DM_ERROR_INVALID_PARAMETER;
	}

	if (dm_context->server_info.port != NULL) {
		/* Set server Port*/
		len = strlen(dm_context->server_info.port);
		for (i = 0; i < len; i++) {
			if ((dm_context->server_info.port[i] < '0') || (dm_context->server_info.port[i] > '9')) {
				printf("Invalid port number\n");
				return DM_ERROR_INVALID_PARAMETER;
			}
		}
		client_set_serverPort(dm_context->server_info.port, dm_context->server_info.isBootstrap);
	} else {
		return DM_ERROR_INVALID_PARAMETER;
	}
	/* Set client lifetime*/
	if (dm_context->server_info.isBootstrap != true) {
		if (dm_context->client_info.lifetime <= 0) {
			printf("Invalid client lifetime\n");
			return DM_ERROR_INVALID_PARAMETER;
		}
		client_set_lifetime(dm_context->client_info.lifetime);
	} else {
		prv_initiate_bootstrap();
	}

	pthread_attr_init(&wakaama_attr);
	wakaama_sparam.sched_priority = SCHED_PRIORITY_DEFAULT;
	(void)pthread_attr_setschedparam(&wakaama_attr, &wakaama_sparam);
	(void)pthread_attr_setstacksize(&wakaama_attr, 4096);
	ret = pthread_create(&wakaama_thread, &wakaama_attr, (pthread_startroutine_t)client_main, NULL);
	pthread_detach(wakaama_thread);
	/* Function return value */
	if (ret != 0) {
		printf(" Start client API fail\n");
		return DM_ERROR_UNKNOWN;
	} else {
		printf(" Start client API success\n");
		dm_state = DM_LWM2M_CLIENT_STARTED;
		prv_copy_dm_context(dm_context);
		return DM_ERROR_NONE;
	}
}

/**
 * @brief Close a DM client
 *
 * @param  None
 * @return On success, 0 is returned.
 *         On failure, a negative value is returned.
 *         If client is already stopped, return DM_ERROR_ALREADY_STOPPED.
 */
int dm_lwm2m_stop_client(void)
{
	if (dm_state == DM_LWM2M_CLIENT_STOPPED) {
		return DM_ERROR_ALREADY_STOPPED;
	}
	client_close();
	memset(&g_dm_context, 0x00, sizeof(struct dm_lwm2m_context_s));
	dm_state = DM_LWM2M_CLIENT_STOPPED;
	return DM_ERROR_NONE;
}

/**
 * @brief Get server IP address
 *
 * @param[out] IP address pointer to memory to store server IP address
 * @return     On success, 0 is returned.
 *             On failure, a negative value is returned.
 */
int dm_lwm2m_get_server_address(char *server_ipAddr)
{
	if (dm_state == DM_LWM2M_CLIENT_STOPPED) {
		return DM_ERROR_NO_DATA;
	}
	if (server_ipAddr == NULL) {
		fprintf(stderr, "%s: Null input\n", __FUNCTION__);
		return DM_ERROR_INVALID_PARAMETER;
	}

	if (g_dm_context.server_info.ipAddress[0] == '\0') {
		fprintf(stderr, "%s: No data\n", __FUNCTION__);
		return DM_ERROR_NO_DATA;
	}
	strncpy(server_ipAddr, g_dm_context.server_info.ipAddress, strlen(g_dm_context.server_info.ipAddress));
	return DM_ERROR_NONE;
}

/**
 * @brief Get server port number
 *
 * @param[out] port pointer to memory to store server port
 * @return     On success, 0 is returned.
 *             On failure, a negative value is returned.
 */
int dm_lwm2m_get_server_port(char *server_port)
{
	if (dm_state == DM_LWM2M_CLIENT_STOPPED) {
		return DM_ERROR_NO_DATA;
	}

	if (server_port == NULL) {
		fprintf(stderr, "%s: Null input\n", __FUNCTION__);
		return DM_ERROR_INVALID_PARAMETER;
	}

	if (g_dm_context.server_info.port[0] == '\0') {
		fprintf(stderr, "%s: No data\n", __FUNCTION__);
		return DM_ERROR_NO_DATA;
	}

	strncpy(server_port, g_dm_context.server_info.port, strlen(g_dm_context.server_info.port));
	return DM_ERROR_NONE;
}

/**
 * @brief Get lifetime for a client
 *
 * @param[out] lifetime pointer to memory to store client lifetime
 * @return     On success, 0 is returned.
 *             On failure, a negative value is returned.
 */
int dm_lwm2m_get_client_lifetime(int *lifetime)
{
	if (dm_state == DM_LWM2M_CLIENT_STOPPED) {
		return DM_ERROR_NO_DATA;
	}

	if (lifetime == NULL) {
		fprintf(stderr, "%s: Null input\n", __FUNCTION__);
		return DM_ERROR_INVALID_PARAMETER;
	}

	*lifetime = g_dm_context.client_info.lifetime;
	return DM_ERROR_NONE;
}

/**
 * @brief Get state for a client
 *
 * @param[out] state pointer to memory to store client state
 * @return     On success, 0 is returned.
 *             On failure, a negative value is returned.
 */
int dm_lwm2m_get_client_state(dm_lwm2m_client_state_e *state)
{
	if (state == NULL) {
		fprintf(stderr, "%s: Null input\n", __FUNCTION__);
		return DM_ERROR_INVALID_PARAMETER;
	}

	*state = dm_state;
	return DM_ERROR_NONE;
}

/**
 * @brief Get client resource value
 *
 * @param[in] buffer pointer to resource URI
 *
 * @return     On success, 0 is returned.
 *             On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_lwm2m_display_client_resource(char *buffer)
{
	if (buffer == NULL) {
		return DM_ERROR_INVALID_PARAMETER;
	} else {
		int ret;
		ret = client_object_dump(buffer);
		if (ret == DM_ERROR_NONE) {
			return ret;
		} else {
			return DM_ERROR_NO_DATA;
		}
	}
}

int dm_lwm2m_change_client_resource(char *buffer)
{
	int ret = DM_ERROR_INVALID_PARAMETER;
	if (buffer != NULL)
		ret = client_change_resource(buffer, NULL);

	return ret;
}
