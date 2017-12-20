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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "ocf_mylight.h"

#define MAX_SLOT 8

struct notify_slot {
	bool active;
	OCObservationId id;
};

/*
 * 0: Light
 * 1: Dimming
 */
static struct notify_slot slots[2][MAX_SLOT];

static pthread_t _notify_tid;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_loop = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static unsigned int changed_rsrc;
static int exit_notify_thread;

static int find_empty_slot(unsigned int rsrc_id)
{
	int i;

	if (rsrc_id > 1) {
		DBG("invalid rsrc_id(%d)", rsrc_id);
		return -1;
	}

	for (i = 0; i < MAX_SLOT; i++) {
		if (slots[rsrc_id][i].active == false)
			return i;
	}

	return -1;
}

static int find_slot(unsigned int rsrc_id, OCObservationId id)
{
	int i;

	if (rsrc_id > 1) {
		DBG("invalid rsrc_id(%d)", rsrc_id);
		return -1;
	}

	for (i = 0; i < MAX_SLOT; i++) {
		if (slots[rsrc_id][i].id == id)
			return i;
	}

	return -1;
}

static void notify(unsigned int rsrc_id)
{
	OCRepPayload *payload = NULL;
	OCObservationId list[MAX_SLOT];
	OCResourceHandle handle;
	int i;
	int list_cnt = 0;

	if (rsrc_id > 1) {
		DBG("invalid rsrc_id(%d)", rsrc_id);
		return;
	}

	pthread_mutex_lock(&lock);
	for (i = 0; i < MAX_SLOT; i++) {
		if (slots[rsrc_id][i].active) {
			list[list_cnt] = slots[rsrc_id][i].id;
			list_cnt++;
		}
	}
	DBG("list_cnt = %d", list_cnt);
	pthread_mutex_unlock(&lock);

	payload = OCRepPayloadCreate();

	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	if (rsrc_id == 0) {
		bool value;

		OCRepPayloadAddResourceType(payload, "oic.r.switch.binary");
		OCRepPayloadSetUri(payload, ocf_mylight_light_peek_uri());

		ocf_mylight_light_get_status(&value);
		OCRepPayloadSetPropBool(payload, "value", value);

		ocf_mylight_light_get_handle(&handle);
	} else if (rsrc_id == 1) {
#ifdef CONFIG_EXAMPLES_OCFMYLIGHT_DIMMER
		int64_t value;

		OCRepPayloadAddResourceType(payload, "oic.r.light.dimming");
		OCRepPayloadSetUri(payload, ocf_mylight_dimm_peek_uri());

		ocf_mylight_dimm_get_level(&value);
		OCRepPayloadSetPropInt(payload, "dimmingSetting", value);

		ocf_mylight_dimm_get_handle(&handle);
#endif
	}

	OCNotifyListOfObservers(handle, list, list_cnt, payload, OC_NA_QOS);

	OCRepPayloadDestroy(payload);
}

static void *loop_notify(void *user_data _UNUSED_)
{
	MSG("Notify Thread ready...");

	while (1) {
		pthread_mutex_lock(&lock_loop);
		pthread_cond_wait(&cond, &lock_loop);

		if (exit_notify_thread) {
			MSG("Exit notify thread");
			pthread_mutex_unlock(&lock_loop);
			break;
		}

		MSG("Do notify! (resource=%d)", changed_rsrc);
		notify(changed_rsrc);

		pthread_mutex_unlock(&lock_loop);
	}

	return NULL;
}

int ocf_mylight_notify_add(unsigned int rsrc_id, OCObservationId obs_id)
{
	int i;

	if (rsrc_id > 1) {
		DBG("invalid rsrc_id(%u)", rsrc_id);
		return -1;
	}

	DBG("Resource%u obs_id = %d", rsrc_id, obs_id);

	pthread_mutex_lock(&lock);

	i = find_empty_slot(rsrc_id);
	if (i == -1) {
		DBG("notify slot is full");
		pthread_mutex_unlock(&lock);
		return -1;
	}

	slots[rsrc_id][i].active = true;
	slots[rsrc_id][i].id = obs_id;

	pthread_mutex_unlock(&lock);

	return 0;
}

int ocf_mylight_notify_del(unsigned int rsrc_id, OCObservationId obs_id)
{
	int i;

	if (rsrc_id > 1) {
		DBG("invalid rsrc_id(%u)", rsrc_id);
		return -1;
	}

	DBG("Resource%u obs_id = %d", rsrc_id, obs_id);

	pthread_mutex_lock(&lock);

	i = find_slot(rsrc_id, obs_id);
	if (i == -1) {
		DBG("can't find obs_id(%d)", obs_id);
		pthread_mutex_unlock(&lock);
		return -1;
	}

	slots[rsrc_id][i].active = false;
	slots[rsrc_id][i].id = 0;

	pthread_mutex_unlock(&lock);

	return 0;
}

int ocf_mylight_notify_emit(unsigned int rsrc_id)
{
	if (rsrc_id > 1) {
		DBG("invalid rsrc_id(%u)", rsrc_id);
		return -1;
	}

	pthread_mutex_lock(&lock_loop);
	MSG("notiry request (resource=%d)", rsrc_id);
	changed_rsrc = rsrc_id;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lock_loop);

	return 0;
}

int ocf_mylight_notify_init(void)
{
	int i;

	for (i = 0; i < MAX_SLOT; i++) {
		slots[0][i].active = false;
		slots[0][i].id = 0;
		slots[1][i].active = false;
		slots[1][i].id = 0;
	}

	exit_notify_thread = 0;
	if (pthread_create(&_notify_tid, NULL, loop_notify, NULL) != 0) {
		DBG("pthread_create failed. errno=%d", errno);
		return -1;
	}
	pthread_setname_np(_notify_tid, "ocf_mylight_notify");

	return 0;
}

void ocf_mylight_notify_exit(void)
{
	exit_notify_thread = 1;
	pthread_cond_signal(&cond);
}
