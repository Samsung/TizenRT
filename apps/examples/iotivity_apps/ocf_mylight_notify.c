#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

//#include "ocf_mylight.h"
#include "iotivity_common.h"

#define MAX_SLOT 8

#define _UNUSED_ __attribute__((unused))

struct notify_slot {
	bool active;
	OCObservationId id;
};

static struct notify_slot slots[2][MAX_SLOT];

static pthread_t _notify_tid;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_loop = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static unsigned int changed_light;

static int find_empty_slot(unsigned int light_id)
{
	int i;

	for (i = 0; i < MAX_SLOT; i++) {
		if (slots[light_id][i].active == false)
			return i;
	}

	return -1;
}

static int find_slot(unsigned int light_id, OCObservationId id)
{
	int i;

	for (i = 0; i < MAX_SLOT; i++) {
		if (slots[light_id][i].id == id)
			return i;
	}

	return -1;
}

static void notify(int light_id)
{
	OCRepPayload *payload = NULL;
	OCObservationId list[MAX_SLOT];
	OCResourceHandle handle;
	int i;
	int list_cnt = 0;
	bool value;

	pthread_mutex_lock(&lock);
	for (i = 0; i < MAX_SLOT; i++) {
		if (slots[light_id][i].active) {
			list[list_cnt] = slots[light_id][i].id;
			list_cnt++;
		}
	}
	DBG("list_cnt = %d", list_cnt);
	pthread_mutex_unlock(&lock);

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.r.switch.binary");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	sec_get_current_value(payload, light_id);

	sec_get_current_hadle(light_id, &handle);
	OCNotifyListOfObservers(handle, list, list_cnt, payload, OC_NA_QOS);

	OCRepPayloadDestroy(payload);
}

static void *loop_notify(void *user_data _UNUSED_)
{
	MSG("Notify Thread ready...");

	while (1) {
		pthread_mutex_lock(&lock_loop);
		pthread_cond_wait(&cond, &lock_loop);

		MSG("Do notify! (light=%d)", changed_light);
		notify(changed_light);

		pthread_mutex_unlock(&lock_loop);
	}

	return NULL;
}

int ocf_mylight_notify_add(unsigned int light_id, OCObservationId obs_id)
{
	int i;

	if (light_id > 1) {
		DBG("invalid light_id(%u)", light_id);
		return -1;
	}

	DBG("Light%u obs_id = %d", light_id, obs_id);

	pthread_mutex_lock(&lock);

	i = find_empty_slot(light_id);
	if (i == -1) {
		DBG("notify slot is full");
		pthread_mutex_unlock(&lock);
		return -1;
	}

	slots[light_id][i].active = true;
	slots[light_id][i].id = obs_id;

	pthread_mutex_unlock(&lock);

	return 0;
}

int ocf_mylight_notify_del(unsigned int light_id, OCObservationId obs_id)
{
	int i;

	if (light_id > 1) {
		DBG("invalid light_id(%u)", light_id);
		return -1;
	}

	DBG("Light%u obs_id = %d", light_id, obs_id);

	pthread_mutex_lock(&lock);

	i = find_slot(light_id, obs_id);
	if (i == -1) {
		DBG("invalid obs_id(%d)", obs_id);
		pthread_mutex_unlock(&lock);
		return -1;
	}

	slots[light_id][i].active = false;
	slots[light_id][i].id = 0;

	pthread_mutex_unlock(&lock);

	return 0;
}

int ocf_mylight_notify_emit(unsigned int light_id)
{
	if (light_id > 1) {
		DBG("invalid light_id(%u)", light_id);
		return -1;
	}

	pthread_mutex_lock(&lock_loop);
	MSG("request (light=%d)", light_id);
	changed_light = light_id;
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

	if (pthread_create(&_notify_tid, NULL, loop_notify, NULL) != 0) {
		DBG("pthread_create failed. errno=%d", errno);
		return -1;
	}

	return 0;
}

void ocf_mylight_notify_exit(void)
{
}
