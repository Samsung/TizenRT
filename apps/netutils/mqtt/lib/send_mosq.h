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
Copyright (c) 2010-2014 Roger Light <roger@atchoo.org>

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
#ifndef _SEND_MOSQ_H_
#define _SEND_MOSQ_H_

#include <mosquitto.h>

int _mosquitto_send_simple_command(struct mosquitto *mosq, uint8_t command);
int _mosquitto_send_command_with_mid(struct mosquitto *mosq, uint8_t command, uint16_t mid, bool dup);
int _mosquitto_send_real_publish(struct mosquitto *mosq, uint16_t mid, const char *topic, uint32_t payloadlen, const void *payload, int qos, bool retain, bool dup);

int _mosquitto_send_connect(struct mosquitto *mosq, uint16_t keepalive, bool clean_session);
int _mosquitto_send_disconnect(struct mosquitto *mosq);
int _mosquitto_send_pingreq(struct mosquitto *mosq);
int _mosquitto_send_pingresp(struct mosquitto *mosq);
int _mosquitto_send_puback(struct mosquitto *mosq, uint16_t mid);
int _mosquitto_send_pubcomp(struct mosquitto *mosq, uint16_t mid);
int _mosquitto_send_publish(struct mosquitto *mosq, uint16_t mid, const char *topic, uint32_t payloadlen, const void *payload, int qos, bool retain, bool dup);
int _mosquitto_send_pubrec(struct mosquitto *mosq, uint16_t mid);
int _mosquitto_send_pubrel(struct mosquitto *mosq, uint16_t mid);
int _mosquitto_send_subscribe(struct mosquitto *mosq, int *mid, const char *topic, uint8_t topic_qos);
int _mosquitto_send_unsubscribe(struct mosquitto *mosq, int *mid, const char *topic);

#endif
