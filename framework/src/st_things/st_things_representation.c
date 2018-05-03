/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "st_things_representation.h"
#include "utils/things_util.h"
#include "utils/things_malloc.h"
#include "logging/things_logger.h"
#include "ocpayload.h"

#define TAG "[st_things_sdk]"

bool get_str_value(struct _st_things_representation *rep, const char *key, char **value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);

	return OCRepPayloadGetPropString((OCRepPayload *) rep->payload, key, value);
}

bool get_bool_value(struct _st_things_representation *rep, const char *key, bool *value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);

	return OCRepPayloadGetPropBool((OCRepPayload *) rep->payload, key, value);
}

bool get_int_value(struct _st_things_representation *rep, const char *key, int64_t *value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);

	return OCRepPayloadGetPropInt((OCRepPayload *)rep->payload, key, value);
}

bool get_double_value(struct _st_things_representation *rep, const char *key, double *value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);

	return OCRepPayloadGetPropDouble((OCRepPayload *) rep->payload, key, value);
}

bool get_byte_value(struct _st_things_representation *rep, const char *key, uint8_t **value, size_t *size)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, size);

	OCByteString byte_value = { NULL, 0 };
	if (!OCRepPayloadGetPropByteString((OCRepPayload *)rep->payload, key, &byte_value)) {
		return false;
	}

	(*size) = byte_value.len;
	(*value) = byte_value.bytes;

	return true;
}

bool get_object_value(struct _st_things_representation *rep, const char *key, struct _st_things_representation **value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);

	OCRepPayload *payload = NULL;
	bool result = OCRepPayloadGetPropObject(rep->payload, key, &payload);
	if (result) {
		*value = create_representation_inst_internal(payload);
		if (NULL == *value) {
			THINGS_LOG_E(TAG, "Failed to create representation for value.");
			OCRepPayloadDestroy(payload);
			return false;
		}
	}
	return result;
}

bool set_str_value(struct _st_things_representation *rep, const char *key, const char *value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);

	return OCRepPayloadSetPropString(rep->payload, key, value);
}

bool set_bool_value(struct _st_things_representation *rep, const char *key, bool value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);

	return OCRepPayloadSetPropBool(rep->payload, key, value);
}

bool set_int_value(struct _st_things_representation *rep, const char *key, int64_t value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);

	return OCRepPayloadSetPropInt(rep->payload, key, value);
}

bool set_double_value(struct _st_things_representation *rep, const char *key, double value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);

	return OCRepPayloadSetPropDouble(rep->payload, key, value);
}

bool set_byte_value(struct _st_things_representation *rep, const char *key, const uint8_t *value, size_t size)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);
	RET_FALSE_IF_PARAM_EXPR_IS_TRUE(TAG, size < 1);

	OCByteString bVal = { (uint8_t *) /*To resolve a build warning */ value, size };
	return OCRepPayloadSetPropByteString(rep->payload, key, bVal);
}

bool set_object_value(struct _st_things_representation *rep, const char *key, const struct _st_things_representation *value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value->payload);

	return OCRepPayloadSetPropObject(rep->payload, key, value->payload);
}

bool get_str_array_value(struct _st_things_representation *rep, const char *key, char ***array, size_t *length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, length);

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
	bool ret = OCRepPayloadGetStringArray(rep->payload, key, array, dimensions);
	if (true == ret) {
		*length = calcDimTotal(dimensions);
	}

	return ret;
}

bool get_int_array_value(struct _st_things_representation *rep, const char *key, int64_t **array, size_t *length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, length);

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
	bool ret = OCRepPayloadGetIntArray(rep->payload, key, array, dimensions);
	if (true == ret) {
		*length = calcDimTotal(dimensions);
	}

	return ret;
}

bool get_double_array_value(struct _st_things_representation *rep, const char *key, double **array, size_t *length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, length);

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
	bool ret = OCRepPayloadGetDoubleArray(rep->payload, key, array, dimensions);
	if (true == ret) {
		*length = calcDimTotal(dimensions);
	}

	return ret;
}

bool get_object_array_value(struct _st_things_representation *rep, const char *key, struct _st_things_representation ***array, size_t *length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, length);

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
	OCRepPayload **children = NULL;
	bool ret = OCRepPayloadGetPropObjectArray(rep->payload, key, &children, dimensions);
	if (!ret) {
		return false;
	}

	*length = calcDimTotal(dimensions);
	*array = (struct _st_things_representation **)things_calloc((*length), sizeof(struct _st_things_representation *));
	if (NULL == *array) {
		THINGS_LOG_E(TAG, "Failed to allocate memory for object array value.");
		for (size_t index = 0; index < *length; index++) {
			OCPayloadDestroy((OCPayload *) children[index]);
		}
		things_free(children);
		return false;
	}

	for (size_t index = 0; index < *length; index++) {
		if (NULL == children[index]) {
			THINGS_LOG_E(TAG, "Payload at index(%d) is NULL.", index);
		} else {
			(*array)[index] = create_representation_inst_internal(children[index]);
			if (NULL == (*array)[index]) {
				THINGS_LOG_E(TAG, "Failed to create a representation for object array value.");
			}
		}

		if (NULL == (*array)[index]) {
			for (size_t sub_index = 0; sub_index < index; sub_index++) {
				destroy_representation_inst_internal((*array)[sub_index], true);
			}
			things_free(*array);
			*array = NULL;
			*length = 0;

			for (size_t sub_index = index; sub_index < *length; sub_index++) {
				OCPayloadDestroy((OCPayload *) children[sub_index]);
			}
			things_free(children);
			return false;
		}
	}

	things_free(children);
	return true;
}

bool set_str_array_value(struct _st_things_representation *rep, const char *key, const char **array, size_t length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_EXPR_IS_TRUE(TAG, length < 1);

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };
	return OCRepPayloadSetStringArray(rep->payload, key, array, dimensions);
}

bool set_int_array_value(struct _st_things_representation *rep, const char *key, const int64_t *array, size_t length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_EXPR_IS_TRUE(TAG, length < 1);

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };
	return OCRepPayloadSetIntArray(rep->payload, key, array, dimensions);
}

bool set_double_array_value(struct _st_things_representation *rep, const char *key, const double *array, size_t length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_EXPR_IS_TRUE(TAG, length < 1);

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };
	return OCRepPayloadSetDoubleArray(rep->payload, key, array, dimensions);
}

bool set_object_array_value(struct _st_things_representation *rep, const char *key, const struct _st_things_representation **array, size_t length)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, array);
	RET_FALSE_IF_PARAM_EXPR_IS_TRUE(TAG, length < 1);

	OCRepPayload **value_payload = NULL;
	value_payload = (OCRepPayload **) things_malloc(length * sizeof(OCRepPayload *));
	if (NULL == value_payload) {
		THINGS_LOG_E(TAG, "Failed to allocate memory for object array value.");
		return false;
	}

	for (size_t i = 0; i < length; i++) {
		if (NULL == array[i]) {
			THINGS_LOG_E(TAG, "array[%d] is NULL.", i);
			things_free(value_payload);
			return false;
		}
		if (NULL == array[i]->payload) {
			THINGS_LOG_E(TAG, "array[%d]->payload is NULL.", i);
			things_free(value_payload);
			return false;
		}
		value_payload[i] = array[i]->payload;
	}

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };
	bool res = OCRepPayloadSetPropObjectArray(rep->payload, key, (const OCRepPayload **)value_payload, dimensions);
	things_free(value_payload);

	return res;
}

st_things_representation_s *create_representation_inst_internal(OCRepPayload *payload)
{
	st_things_representation_s *rep = (st_things_representation_s *) things_malloc(sizeof(st_things_representation_s));
	RET_VAL_IF_NULL(TAG, rep, "Failed to allocate memory for representation.", NULL);

	rep->get_str_value = &get_str_value;
	rep->get_bool_value = &get_bool_value;
	rep->get_int_value = &get_int_value;
	rep->get_double_value = &get_double_value;
	rep->get_byte_value = &get_byte_value;
	rep->get_object_value = &get_object_value;

	rep->set_str_value = &set_str_value;
	rep->set_bool_value = &set_bool_value;
	rep->set_int_value = &set_int_value;
	rep->set_double_value = &set_double_value;
	rep->set_byte_value = &set_byte_value;
	rep->set_object_value = &set_object_value;

	rep->get_str_array_value = &get_str_array_value;
	rep->get_int_array_value = &get_int_array_value;
	rep->get_double_array_value = &get_double_array_value;
	rep->get_object_array_value = &get_object_array_value;

	rep->set_str_array_value = &set_str_array_value;
	rep->set_int_array_value = &set_int_array_value;
	rep->set_double_array_value = &set_double_array_value;
	rep->set_object_array_value = &set_object_array_value;

	if (NULL != payload) {
		THINGS_LOG_D(TAG, "Setting the given payload in the representation.");
		rep->payload = payload;
	} else {
		THINGS_LOG_D(TAG, "Creating a new payload and setting it in the representation.");
		rep->payload = OCRepPayloadCreate();
		if (NULL == rep->payload) {
			THINGS_LOG_E(TAG, "Failed to create payload for representation.");
			destroy_representation_inst_internal(rep, false);
			return NULL;
		}
	}

	return rep;
}

st_things_representation_s *create_representation_inst(void)
{
	return create_representation_inst_internal(NULL);
}

void destroy_representation_inst_internal(st_things_representation_s *rep, bool destroy_payload)
{
	RET_IF_PARAM_IS_NULL(TAG, rep);

	THINGS_LOG_D(TAG, "Destroy payload: %s.", destroy_payload ? "Yes" : "No");

	// Payload will be de-allocated for the following cases.
	// 1. For request representations(created at this API layer).
	// 2. For all representations created by the application.
	// DA Stack will de-allocate the payload for other cases.
	// 1. For response representations.
	if (destroy_payload) {
		OCPayloadDestroy((OCPayload *)(rep->payload));
	}

	things_free(rep);
}

void destroy_representation_inst(st_things_representation_s *rep)
{
	destroy_representation_inst_internal(rep, true);
}
