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
Copyright (c) 2009-2014 Roger Light <roger@atchoo.org>

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

#include <assert.h>

#include <mosquitto.h>
#include <logging_mosq.h>
#include <memory_mosq.h>
#include <net_mosq.h>
#include <read_handle.h>

int _mosquitto_handle_connack(struct mosquitto *mosq)
{
	uint8_t byte;
	uint8_t result;
	int rc;

	assert(mosq);
	_mosquitto_log_printf(mosq, MOSQ_LOG_DEBUG, "Client %s received CONNACK", mosq->id);
	rc = _mosquitto_read_byte(&mosq->in_packet, &byte);	// Reserved byte, not used
	if (rc) {
		return rc;
	}
	rc = _mosquitto_read_byte(&mosq->in_packet, &result);
	if (rc) {
		return rc;
	}
	pthread_mutex_lock(&mosq->callback_mutex);
	if (mosq->on_connect) {
		mosq->in_callback = true;
		mosq->on_connect(mosq, mosq->userdata, result);
		mosq->in_callback = false;
	}
	pthread_mutex_unlock(&mosq->callback_mutex);
	switch (result) {
	case 0:
		if (mosq->state != mosq_cs_disconnecting) {
			mosq->state = mosq_cs_connected;
		}
		return MOSQ_ERR_SUCCESS;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		return MOSQ_ERR_CONN_REFUSED;
	default:
		return MOSQ_ERR_PROTOCOL;
	}
}
