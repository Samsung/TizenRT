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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <mosquitto_internal.h>
#include <mosquitto.h>
#include <memory_mosq.h>

int _mosquitto_log_printf(struct mosquitto *mosq, int priority, const char *fmt, ...)
{
	va_list va;
	char *s;
	int len;

	assert(mosq);
	assert(fmt);

	pthread_mutex_lock(&mosq->log_callback_mutex);
	if (mosq->on_log) {
		len = strlen(fmt) + 500;
		s = _mosquitto_malloc(len * sizeof(char));
		if (!s) {
			pthread_mutex_unlock(&mosq->log_callback_mutex);
			return MOSQ_ERR_NOMEM;
		}

		va_start(va, fmt);
		vsnprintf(s, len, fmt, va);
		va_end(va);
		s[len - 1] = '\0';		/* Ensure string is null terminated. */

		mosq->on_log(mosq, mosq->userdata, priority, s);

		_mosquitto_free(s);
	}
	pthread_mutex_unlock(&mosq->log_callback_mutex);

	return MOSQ_ERR_SUCCESS;
}
