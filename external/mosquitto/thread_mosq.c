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
/*
Copyright (c) 2011-2014 Roger Light <roger@atchoo.org>

All rights reserved. This program and the accompanying materials
are made available under the terms of the Eclipse Public License v1.0
and Eclipse Distribution License v1.0 which accompany this distribution.

The Eclipse Public License is available at
   http://www.eclipse.org/legal/epl-v10.html
and the Eclipse Distribution License is available at
  http://www.eclipse.org/org/documents/edl-v10.php.

Contributors:
   Roger Light - initial implementation and documentation.
*/

#include <config.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include <mosquitto_internal.h>
#include <net_mosq.h>

#if defined(__TINYARA__)
#define TINYARA_MQTT_PTHREAD_STACK_SIZE		(8 * 1024)
#endif

void *_mosquitto_thread_main(void *obj);

int mosquitto_loop_start(struct mosquitto *mosq)
{
#ifdef WITH_THREADING
	if (!mosq || mosq->threaded != mosq_ts_none) {
		return MOSQ_ERR_INVAL;
	}

#if defined(__TINYARA__)
	char threadname[40];
	pthread_attr_t attr;

	mosq->threaded = mosq_ts_self;
	if (pthread_attr_init(&attr) != 0) {
		return MOSQ_ERR_ERRNO;
	}

	if (pthread_attr_setstacksize(&attr, TINYARA_MQTT_PTHREAD_STACK_SIZE) != 0) {
		return MOSQ_ERR_ERRNO;
	}

	if (pthread_create(&mosq->thread_id, &attr, _mosquitto_thread_main, mosq) != 0) {
		return MOSQ_ERR_ERRNO;
	}

	snprintf(threadname, sizeof(threadname), "%s-%s", "MQTT", mosq->id);
	pthread_setname_np(mosq->thread_id, threadname);
	pthread_detach(mosq->thread_id);

	return MOSQ_ERR_SUCCESS;

#else
	mosq->threaded = mosq_ts_self;
	if (!pthread_create(&mosq->thread_id, NULL, _mosquitto_thread_main, mosq)) {
		return MOSQ_ERR_SUCCESS;
	} else {
		return MOSQ_ERR_ERRNO;
	}
#endif

#else
	return MOSQ_ERR_NOT_SUPPORTED;
#endif
}

int mosquitto_loop_stop(struct mosquitto *mosq, bool force)
{
#ifdef WITH_THREADING
#	ifndef WITH_BROKER
	char sockpair_data = 0;
#	endif

	if (!mosq || mosq->threaded != mosq_ts_self) {
		return MOSQ_ERR_INVAL;
	}

	/* Write a single byte to sockpairW (connected to sockpairR) to break out
	 * of select() if in threaded mode. */
	if (mosq->sockpairW != INVALID_SOCKET) {
#ifndef WIN32
		if (write(mosq->sockpairW, &sockpair_data, 1)) {
		}
#else
		send(mosq->sockpairW, &sockpair_data, 1, 0);
#endif
	}

	if (force) {
		pthread_cancel(mosq->thread_id);
	}
	pthread_join(mosq->thread_id, NULL);
	mosq->thread_id = pthread_self();
	mosq->threaded = mosq_ts_none;

	return MOSQ_ERR_SUCCESS;
#else
	return MOSQ_ERR_NOT_SUPPORTED;
#endif
}

#ifdef WITH_THREADING
void *_mosquitto_thread_main(void *obj)
{
	struct mosquitto *mosq = obj;

	if (!mosq) {
		return NULL;
	}

	pthread_mutex_lock(&mosq->state_mutex);
	if (mosq->state == mosq_cs_connect_async) {
		pthread_mutex_unlock(&mosq->state_mutex);
		mosquitto_reconnect(mosq);
	} else {
		pthread_mutex_unlock(&mosq->state_mutex);
	}

	if (!mosq->keepalive) {
		/* Sleep for a day if keepalive disabled. */
		mosquitto_loop_forever(mosq, 1000 * 86400, 1);
	} else {
		/* Sleep for our keepalive value. publish() etc. will wake us up. */
		mosquitto_loop_forever(mosq, mosq->keepalive * 1000, 1);
	}

#if defined(__TINYARA__)
	mosq->threaded = mosq_ts_none;
#endif

	return obj;
}
#endif

int mosquitto_threaded_set(struct mosquitto *mosq, bool threaded)
{
	if (!mosq) {
		return MOSQ_ERR_INVAL;
	}

	if (threaded) {
		mosq->threaded = mosq_ts_external;
	} else {
		mosq->threaded = mosq_ts_none;
	}

	return MOSQ_ERR_SUCCESS;
}
