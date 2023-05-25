/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/prctl.h>

#include <tinyara/preference.h>

#include "preference_internal.h"

/****************************************************************************
 * Set Functions
 ****************************************************************************/
int preference_set_int(const char *key, int value)
{
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_INT;
	data.attr.len = sizeof(int);
	data.value = &value;

	/* Set preference with prctl */
	return prctl(PR_SET_PREFERENCE, &data);
}

int preference_set_double(const char *key, double value)
{
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_DOUBLE;
	data.attr.len = sizeof(double);
	data.value = &value;

	/* Set preference with prctl */
	return prctl(PR_SET_PREFERENCE, &data);
}

int preference_set_bool(const char *key, bool value)
{
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_BOOL;
	data.attr.len = sizeof(bool);
	data.value = &value;

	/* Set preference with prctl */
	return prctl(PR_SET_PREFERENCE, &data);
}

int preference_set_string(const char *key, char *value)
{
	preference_data_t data;

	if (key == NULL || value == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_STRING;
	data.attr.len = strlen(value) + 1;
	data.value = value;

	/* Set preference with prctl */
	return prctl(PR_SET_PREFERENCE, &data);
}

int preference_set_binary(const char *key, void *value, int len)
{
	preference_data_t data;

	if (key == NULL || value == NULL || len <= 0) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_BINARY;
	data.attr.len = len;
	data.value = value;

	/* Set preference with prctl */
	return prctl(PR_SET_PREFERENCE, &data);
}

/****************************************************************************
 * Get Functions
 ****************************************************************************/
int preference_get_int(const char *key, int *value)
{
	int ret;
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_INT;

	/* Get preference with prctl */
	ret = prctl(PR_GET_PREFERENCE, &data);
	if (ret == OK) {
		*value = *(int *)data.value;
		PREFERENCE_FREE(data.value);
	}

	return ret;
}

int preference_get_bool(const char *key, bool *value)
{
	int ret;
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_BOOL;

	/* Get preference with prctl */
	ret = prctl(PR_GET_PREFERENCE, &data);
	if (ret == OK) {
		*value = *(bool *)data.value;
		PREFERENCE_FREE(data.value);
	}

	return ret;
}

int preference_get_double(const char *key, double *value)
{
	int ret;
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_DOUBLE;

	/* Get preference with prctl */
	ret = prctl(PR_GET_PREFERENCE, &data);
	if (ret == OK) {
		*value = *(double *)data.value;
		PREFERENCE_FREE(data.value);
	}

	return ret;
}

int preference_get_string(const char *key, char **value)
{
	int ret;
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_STRING;

	/* Get preference with prctl */
	ret = prctl(PR_GET_PREFERENCE, &data);
	if (ret == OK) {
		*value = (char *)data.value;
	}

	return ret;
}

int preference_get_binary(const char *key, void **value, int *len)
{
	int ret;
	preference_data_t data;

	if (key == NULL || value == NULL || len == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.attr.type = PREFERENCE_TYPE_BINARY;

	/* Get preference with prctl */
	ret = prctl(PR_GET_PREFERENCE, &data);
	if (ret == OK) {
		*value = (void *)data.value;
		*len = data.attr.len;
	}

	return ret;
}

/****************************************************************************
 * Check Functions
 ****************************************************************************/
int preference_is_existing(const char *key, bool *existing)
{
	if (key == NULL) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	/* Check preference with prctl */
	return prctl(PR_CHECK_PREFERENCE, PRIVATE_PREFERENCE, key, existing);
}

/****************************************************************************
 * Remove Functions
 ****************************************************************************/
int preference_remove(const char *key)
{
	if (key == NULL) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	/* Remove preference with prctl */
	return prctl(PR_REMOVE_PREFERENCE, PRIVATE_PREFERENCE, key);
}

int preference_remove_all(void)
{
	/* Remove preference with prctl */
	return prctl(PR_REMOVEALL_PREFERENCE, PRIVATE_PREFERENCE, NULL);
}

/****************************************************************************
 * Callback Functions
 ****************************************************************************/
int preference_set_changed_cb(char *key, preference_changed_cb callback, void *user_data)
{
	int ret;
	struct sigaction act;
	preference_callback_t data;

	if (key == NULL || callback == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	/* Register signal handler */
	act.sa_handler = (_sa_handler_t)preference_signal_cb;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIG_PREFERENCE);
	
	ret = sigaction(SIG_PREFERENCE, &act, NULL);
	if (ret != OK) {
		prefdbg("sigaction fail, errno %d", errno);
		return PREFERENCE_OPERATION_FAIL;
	}

	data.key = key;
	data.type = PRIVATE_PREFERENCE;
	data.cb_func = callback;
	data.cb_data = user_data;

	/* Set callback with prctl */
	return prctl(PR_SET_PREFERENCE_CB, &data);
}

int preference_unset_changed_cb(char *key)
{
	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	/* Unset callback with prctl */
	return prctl(PR_UNSET_PREFERENCE_CB, PRIVATE_PREFERENCE, key);
}
