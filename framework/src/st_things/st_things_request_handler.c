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

#include "st_things_request_handler.h"
#ifdef CONFIG_ST_THINGS_COLLECTION
#include "st_things_request_handler_collection.h"
#endif
#include "st_things_representation.h"
#include "utils/things_malloc.h"
#include "utils/things_util.h"
#include "logging/things_logger.h"

#include "things_api.h"
#include "octypes.h"
#include "ocpayload.h"

#define BOOL_ID 0
#define INT_ID 1
#define DOUBLE_ID 2
#define STRING_ID 3
#define OBJECT_ID 4
#define BYTE_ID 5
#define INT_ARRAY_ID 6
#define DOUBLE_ARRAY_ID 7
#define STRING_ARRAY_ID 8
#define OBJECT_ARRAY_ID 9

#define IS_READABLE(var) (1 & var)
#define IS_WRITABLE(var) (2 & var)

#define PROPERTY_KEY_DELIMITER ';'
#define KEY_VALUE_SEPARATOR '='

#define TAG "[st_things_sdk]"

static st_things_get_request_cb g_handle_get_req_cb = NULL;
static st_things_set_request_cb g_handle_set_req_cb = NULL;

bool get_query_value_internal(const char *query, const char *key, char **value, bool *found)
{
	RET_FALSE_IF_EXPR_IS_TRUE(TAG, (NULL == query || strlen(query) < 1), "Invalid query.");
	RET_FALSE_IF_EXPR_IS_TRUE(TAG, (NULL == key || strlen(key) < 1), "Invalid key.");
	RET_FALSE_IF_PARAM_IS_NULL(TAG, value);

	*value = NULL;
	if (NULL != found) {
		*found = false;
	}

	int query_len = strlen(query);
	int key_len = strlen(key);
	char *p_buff = NULL;
	char *p_origin = NULL;
	char *p_ptr = NULL;

	p_origin = p_buff = (char *)things_malloc(query_len + 1);
	if (NULL == p_origin) {
		THINGS_LOG_E(TAG, "Failed to allocate memory to get a specific value from query.");
		return false;
	}

	memset(p_buff, 0, query_len + 1);
	memcpy(p_buff, query, query_len);

	p_ptr = strtok(p_buff, QUERY_DELIMITER);
	if (NULL == p_ptr) {
		THINGS_LOG_E(TAG, "Failed to tokenize the query.");
		things_free(p_origin);
		return false;
	}

	bool res = false;
	while (p_ptr != NULL) {
		if (strncmp(p_ptr, key, key_len) == 0) {
			THINGS_LOG_D(TAG, "Key(%s) exists in query parameter(%s).", key, query);
			if (NULL != found) {
				*found = true;
			}

			*value = things_clone_string(p_ptr + key_len + 1);
			if (NULL == *value) {
				THINGS_LOG_E(TAG, "Failed to clone the query value.");
				things_free(p_origin);
				return false;
			} else {
				res = true;
			}
			break;
		}

		p_ptr = strtok(NULL, QUERY_DELIMITER);
	}

	if (NULL == p_ptr) {
		THINGS_LOG_D(TAG, "Key(%s) doesn't exist in query(%s).", key, query);
	}

	things_free(p_origin);

	return res;
}

bool get_query_value_for_get_req(struct _st_things_get_request_message *req_msg, const char *key, char **value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_msg);
	return get_query_value_internal(req_msg->query, key, value, NULL);
}

bool get_query_value_for_post_req(struct _st_things_set_request_message *req_msg, const char *key, char **value)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_msg);
	return get_query_value_internal(req_msg->query, key, value, NULL);
}

bool is_property_key_exist(struct _st_things_get_request_message *req_msg, const char *key)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_msg);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_msg->property_key);
	RET_FALSE_IF_EXPR_IS_TRUE(TAG, strlen(req_msg->property_key) < 1, "Property key in request message is empty.");
	RET_FALSE_IF_PARAM_IS_NULL(TAG, key);
	RET_FALSE_IF_EXPR_IS_TRUE(TAG, strlen(key) < 1, "Key is empty.");

	int key_len = strlen(key);
	bool exist = false;
	char *key_ptr = req_msg->property_key;
	while (NULL != key_ptr && !exist) {
		key_ptr = strstr(key_ptr, key);
		if (NULL == key_ptr) {
			break;
		}
		// The following logic ensures that the key is a complete key and not a substring of another key.
		// Check whether a delimeter immediately follows the key.
		if (PROPERTY_KEY_DELIMITER != key_ptr[key_len]) {
			key_ptr += key_len;
			continue;
		}
		// If key is not at the begining, then check whether there is a delimiter immediately before the key.
		if (key_ptr != req_msg->property_key && PROPERTY_KEY_DELIMITER != key_ptr[-1]) {
			key_ptr += key_len;
			continue;
		}

		exist = true;
	}

	THINGS_LOG_D(TAG, "Key(%s) exist?: %s.", key, exist ? "Yes" : "No");
	return exist;
}

static bool get_resource_types(things_resource_s *rsrc, char ***res_types, int *count)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rsrc);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, res_types);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, count);

	int rt_count = rsrc->things_get_num_of_res_types(rsrc);
	THINGS_LOG_D(TAG, "Resource(%s) has %d resource type(s).", rsrc->uri, rt_count);
	RET_FALSE_IF_EXPR_IS_TRUE(TAG, rt_count < 1, "No resource types in resource.");

	char **types = (char **)things_calloc(rt_count, sizeof(char *));
	RET_FALSE_IF_NULL(TAG, types, "Failed to allocate memory for resource types.");

	bool result = true;
	const char *res_type;
	for (int i = 0; i < rt_count; i++) {
		res_type = rsrc->things_get_res_type(rsrc, i);
		if (NULL == res_type || strlen(res_type) < 1) {
			THINGS_LOG_E(TAG, "Resource type at index(%d) is invalid.", i);
			things_free_str_array(types, i);
			result = false;
			break;
		}

		types[i] = things_clone_string(res_type);
		if (NULL == types[i]) {
			THINGS_LOG_E(TAG, "Failed to clone resource type(%s).", res_type);
			things_free_str_array(types, i);
			result = false;
			break;
		}
	}

	if (result) {
		*count = rt_count;
		*res_types = types;
	}

	return result;
}

static bool get_interface_types(things_resource_s *rsrc, char ***if_types, int *count)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rsrc);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, if_types);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, count);

	int if_count = rsrc->things_get_num_of_inf_types(rsrc);
	THINGS_LOG_D(TAG, "Resource(%s) has %d interface type(s).", rsrc->uri, if_count);
	RET_FALSE_IF_EXPR_IS_TRUE(TAG, if_count < 1, "No interface types in resource.");

	char **types = (char **)things_calloc(if_count, sizeof(char *));
	RET_VAL_IF_NULL(TAG, types, "Failed to allocate memory for inteface types.", false);

	bool result = true;
	const char *if_type = NULL;
	for (int i = 0; i < if_count; i++) {
		if_type = rsrc->things_get_inf_type(rsrc, i);
		if (NULL == if_type || strlen(if_type) < 1) {
			THINGS_LOG_E(TAG, "Interface type at index(%d) is invalid.", i);
			things_free_str_array(types, i);
			result = false;
			break;
		}

		types[i] = things_clone_string(if_type);
		if (NULL == types[i]) {
			THINGS_LOG_E(TAG, "Failed to clone inteface type(%s).", if_type);
			things_free_str_array(types, i);
			result = false;
			break;
		}
	}

	if (result) {
		*count = if_count;
		*if_types = types;
	}

	return result;
}

/*
 * Adds the common properties of resource such as 'rt', 'if'.
 * 'links' property will be added in the response payload for collection resources.
 * If it fails for any reason, the resp_payload which is partially updated by this function will not be reset.
 * The caller of this method will have to release the payload and return an error response to the client.
 */
bool add_common_props(things_resource_s *rsrc, bool collection, OCRepPayload *resp_payload)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, rsrc);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, resp_payload);

	// Set resource types.
	int rt_count = 0;
	char **res_types = NULL;
	if (!get_resource_types(rsrc, &res_types, &rt_count)) {
		THINGS_LOG_E(TAG, "Failed to get the resource types of the given resource(%s).", rsrc->uri);
		return false;
	}

	for (int i = 0; i < rt_count; i++) {
		if (!OCRepPayloadAddResourceType(resp_payload, res_types[i])) {
			THINGS_LOG_E(TAG, "Failed to add the resource type in the response payload.");
			// Release memory allocated for resource types.
			things_free_str_array(res_types, rt_count);
			return false;
		}
	}
	things_free_str_array(res_types, rt_count);

	// Set interface types.
	int if_count = 0;
	char **if_types = NULL;
	if (!get_interface_types(rsrc, &if_types, &if_count)) {
		THINGS_LOG_E(TAG, "Failed to get the interface types of the given resource(%s).", rsrc->uri);
		return false;
	}

	for (int i = 0; i < if_count; i++) {
		if (!OCRepPayloadAddInterface(resp_payload, if_types[i])) {
			THINGS_LOG_E(TAG, "Failed to add the interface type in the response payload.");
			// Release memory allocated for interface types.
			things_free_str_array(if_types, if_count);
			return false;
		}
	}
	things_free_str_array(if_types, if_count);
#ifdef CONFIG_ST_THINGS_COLLECTION
	// Set "links"(only for collection).
	if (collection) {
		size_t count = 0;
		OCRepPayload **links = NULL;
		if (!form_collection_links(rsrc, &links, &count)) {
			THINGS_LOG_E(TAG, "Failed to form links for the given collection resource(%s).", rsrc->uri);
			return false;
		}

		THINGS_LOG_D(TAG, "Formed links for collection.");
		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { count, 0, 0 };
		bool result = OCRepPayloadSetPropObjectArray(resp_payload, OC_RSRVD_LINKS, links, dimensions);
		if (!result) {
			THINGS_LOG_E(TAG, "Failed to add the links in the response payload.");
			for (size_t i = 0; i < count && NULL != links[i]; i++) {
				OCRepPayloadDestroy(links[i]);
			}
			things_free(links);
			return false;
		}
	}
#endif

	return true;
}

// To get the properties of a resource based on either resource type or resource uri.
static bool get_supported_properties(const char *res_type, const char *res_uri, int *count, things_attribute_info_s ***properties, bool *destroy_props)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, count);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, properties);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, destroy_props);

	*destroy_props = false;

	// Get the properties based on resource type.
	if (NULL != res_type && strlen(res_type) > 0) {
		int res = things_get_attributes_by_resource_type(res_type, count, properties);
		THINGS_LOG_D(TAG, "Get attributes by resource type(%s): %s.", res_type, (1 == res) ? "Success" : "Failed");
		return res ? true : false;
	}

	char **res_types = NULL;
	int *prop_count = NULL;
	things_attribute_info_s ***props = NULL;

	// Get the properties based on resource uri.
	if (NULL != res_uri && strlen(res_uri) > 0) {
		int type_count = 0;
		// 1. Get the resource types.
		if (!things_get_resource_type(res_uri, &type_count, &res_types)) {
			THINGS_LOG_E(TAG, "Failed to get the resource types based on resource uri(%s).", res_uri);
			return false;
		}

		if (type_count < 1 || NULL == res_types) {
			THINGS_LOG_E(TAG, "No resource types for the given resource(%s).", res_uri);
			return false;
		}

		THINGS_LOG_D(TAG, "Resource(%s) has %d resource type(s).", res_uri, type_count);

		// 2. Get the properties for each resource type.
		props = (things_attribute_info_s ***) things_calloc(type_count, sizeof(things_attribute_info_s **));
		if (NULL == props) {
			THINGS_LOG_E(THINGS_ERROR, TAG, "Failed to allocate memory for resource properties.");
			goto EXIT_WITH_ERROR;
		}
		prop_count = (int *)things_calloc(type_count, sizeof(int));
		if (NULL == prop_count) {
			THINGS_LOG_E(THINGS_ERROR, TAG, "Failed to allocate memory for resource properties.");
			goto EXIT_WITH_ERROR;
		}

		for (int index = 0; index < type_count; index++) {
			if (!things_get_attributes_by_resource_type(res_types[index], &prop_count[index], &props[index])) {
				THINGS_LOG_V(THINGS_ERROR, TAG, "Failed to get the properties of resource type (%s).", res_types[index]);
				goto EXIT_WITH_ERROR;
			}
			THINGS_LOG_D(TAG, "Number of properties of resource type(%s): %d.", res_types[index], prop_count[index]);
			*count += prop_count[index];
		}

		THINGS_LOG_D(TAG, "Total number of properties: %d.", *count);

		*properties = (things_attribute_info_s **) things_calloc(*count, sizeof(things_attribute_info_s *));
		if (NULL == *properties) {
			THINGS_LOG_E(THINGS_ERROR, TAG, "Failed to allocate memory for resource properties.");
			goto EXIT_WITH_ERROR;
		}

		int cur_index = 0;
		for (int index = 0; index < type_count; index++) {
			if (NULL == props[index]) {
				THINGS_LOG_V(THINGS_ERROR, TAG, "Resource type(%s) doesn't have any properties.", res_types[index]);
				goto EXIT_WITH_ERROR;
			}

			for (int sub_index = 0; sub_index < prop_count[index]; sub_index++) {
				things_attribute_info_s *prop = *(props[index] + sub_index);
				if (NULL == prop) {
					THINGS_LOG_E(THINGS_ERROR, TAG, "NULL Property.");
					goto EXIT_WITH_ERROR;
				}
				// If this prop is already added, then ignore it and decrement the total count by 1.
				bool exist = false;
				for (int i = 0; i < cur_index; i++) {
					if (0 == strncmp(((*properties)[i])->key, prop->key, strlen(prop->key))) {
						THINGS_LOG_D(TAG, "Property(%s) is already added in the request message.", prop->key);
						exist = true;
						break;
					}
				}
				if (exist) {
					(*count)--;
				} else {
					(*properties)[cur_index++] = prop;
				}
			}
		}

		*destroy_props = true;
		things_free(prop_count);
		things_free(props);
	} else {
		THINGS_LOG_E(TAG, "Resource type and URI are invalid.");
		return false;
	}

	return true;

EXIT_WITH_ERROR:
	res_types = NULL;
	things_free(prop_count);
	prop_count = NULL;
	things_free(props);
	props = NULL;
	things_free(properties);
	properties = NULL;

	*count = 0;

	return false;
}

static void remove_query_parameter(char *query, char *key)
{
	RET_IF_EXPR_IS_TRUE(TAG, (NULL == query || strlen(query) < 1), "Invalid parameter: query.");
	RET_IF_EXPR_IS_TRUE(TAG, (NULL == key || strlen(key) < 1), "Invalid parameter: key.");

	int key_len = strlen(key);
	char *pos1 = strstr(query, key);
	if (NULL == pos1 || (KEY_VALUE_SEPARATOR != pos1[key_len])) {	// Key should exist and its next char should be '='.
		THINGS_LOG_E(TAG, "Key doesn't exist in query.");
		return;
	}

	char *pos2 = strstr(pos1, QUERY_DELIMITER);
	if (NULL != pos2) {
		pos2++;					// Increment the pointer to make it point the first character in the next key
		while ((*pos1++ = *pos2++)) ;
	} else {					// Given key is the last in the query.
		if (pos1 != query) {
			pos1--;				// Decrement the pointer to remove the leading query delimiter
		}

		*pos1 = '\0';
	}
}

static void add_property_key_in_get_req_msg(st_things_get_request_message_s *req_msg, char *prop_key)
{
	RET_IF_PARAM_IS_NULL(TAG, req_msg);
	RET_IF_PARAM_IS_NULL(TAG, req_msg->property_key);
	RET_IF_PARAM_IS_NULL(TAG, prop_key);

	strncat(req_msg->property_key, prop_key, strlen(prop_key));
	int prop_len = strlen(req_msg->property_key);
	req_msg->property_key[prop_len] = PROPERTY_KEY_DELIMITER;
	req_msg->property_key[prop_len + 1] = '\0';
}

static bool add_property_in_post_req_msg(st_things_set_request_message_s *req_msg, OCRepPayload *req_payload, things_attribute_info_s *prop)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_msg);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_msg->rep);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_msg->rep->payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, prop);

	OCRepPayload *resp_payload = req_msg->rep->payload;

	THINGS_LOG_D(TAG, "Property Key is %s", prop->key);
	THINGS_LOG_D(TAG, "Property type is %d", prop->type);

	// Based on the property type, call appropriate methods to copy
	// the property from request payload to request representation.
	bool result = false;
	switch (prop->type) {
	case BOOL_ID: {
		bool value = false;
		if (OCRepPayloadGetPropBool(req_payload, prop->key, &value)) {
			result = OCRepPayloadSetPropBool(resp_payload, prop->key, value);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the boolean value of '%s' in request message.", prop->key);
			}
		} else {
			THINGS_LOG_E(TAG, "Failed to get the boolean value of '%s' for request message.", prop->key);
		}
	}
	break;
	case INT_ID: {
		int64_t value = 0;
		if (OCRepPayloadGetPropInt(req_payload, prop->key, &value)) {
			result = OCRepPayloadSetPropInt(resp_payload, prop->key, value);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the integer value of '%s' in request message", prop->key);
			}
		} else {
			THINGS_LOG_E(TAG, "Failed to get the integer value of '%s' for request message", prop->key);
		}
	}
	break;
	case DOUBLE_ID: {
		double value = 0.0;
		if (OCRepPayloadGetPropDouble(req_payload, prop->key, &value)) {
			result = OCRepPayloadSetPropDouble(resp_payload, prop->key, value);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the double value of '%s' in request message", prop->key);
			}
		} else {
			THINGS_LOG_E(TAG, "Failed to get the double value of '%s' for request message", prop->key);
		}
	}
	break;
	case STRING_ID: {
		char *value = NULL;
		if (OCRepPayloadGetPropString(req_payload, prop->key, &value)) {
			result = OCRepPayloadSetPropString(resp_payload, prop->key, value);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the string value of '%s' in request message", prop->key);
			}

			things_free(value);
		} else {
			THINGS_LOG_E(TAG, "Failed to get the string value of '%s' for request message", prop->key);
		}
	}
	break;
	case OBJECT_ID: {
		OCRepPayload *value = NULL;
		if (OCRepPayloadGetPropObject(req_payload, prop->key, &value)) {
			result = OCRepPayloadSetPropObject(resp_payload, prop->key, value);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the object value of '%s' in request message", prop->key);
			}

			OCRepPayloadDestroy(value);
		} else {
			THINGS_LOG_E(TAG, "Failed to get the object value of '%s' for request message", prop->key);
		}
	}
	break;
	case BYTE_ID: {
		OCByteString byte_value;
		if (OCRepPayloadGetPropByteString(req_payload, prop->key, &byte_value)) {
			result = OCRepPayloadSetPropByteString(resp_payload, prop->key, byte_value);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the byte string value of '%s' in request message", prop->key);
			}

			things_free(byte_value.bytes);
		} else {
			THINGS_LOG_E(TAG, "Failed to get the byte string value of '%s' for request message", prop->key);
		}
	}
	break;
	case INT_ARRAY_ID: {
		int64_t *value = NULL;
		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
		if (OCRepPayloadGetIntArray(req_payload, prop->key, &value, dimensions)) {
			result = OCRepPayloadSetIntArray(resp_payload, prop->key, value, dimensions);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the integer array value of '%s' in request message", prop->key);
			}

			things_free(value);
		} else {
			THINGS_LOG_E(TAG, "Failed to get the integer array value of '%s' for request message", prop->key);
		}
	}
	break;
	case DOUBLE_ARRAY_ID: {
		double *value = NULL;
		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
		if (OCRepPayloadGetDoubleArray(req_payload, prop->key, &value, dimensions)) {
			result = OCRepPayloadSetDoubleArray(resp_payload, prop->key, value, dimensions);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the double array value of '%s' in request message", prop->key);
			}

			things_free(value);
		} else {
			THINGS_LOG_E(TAG, "Failed to get the double array value of '%s' for request message", prop->key);
		}
	}
	break;
	case STRING_ARRAY_ID: {
		char **value = NULL;
		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
		if (OCRepPayloadGetStringArray(req_payload, prop->key, &value, dimensions)) {
			result = OCRepPayloadSetStringArray(resp_payload, prop->key, value, dimensions);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the string array value of '%s' in request message", prop->key);
			}

			size_t len = calcDimTotal(dimensions);
			things_free_str_array(value, len);
		} else {
			THINGS_LOG_E(TAG, "Failed to get the string array value of '%s' for request message", prop->key);
		}
	}
	break;
	case OBJECT_ARRAY_ID: {
		OCRepPayload **value = NULL;
		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
		if (OCRepPayloadGetPropObjectArray(req_payload, prop->key, &value, dimensions)) {
			result = OCRepPayloadSetPropObjectArray(resp_payload, prop->key, value, dimensions);
			if (!result) {
				THINGS_LOG_E(TAG, "Failed to set the object array value of '%s' in request message", prop->key);
			}

			size_t len = calcDimTotal(dimensions);
			for (size_t index = 0; index < len; index++) {
				OCRepPayloadDestroy(value[index]);
			}
			things_free(value);
		} else {
			THINGS_LOG_E(TAG, "Failed to get the object array value of '%s' for request message", prop->key);
		}
	}
	break;
	default:
		THINGS_LOG_E(TAG, "Invalid property type (%d).", prop->type);
		break;
	}

	return result;
}

static void destroy_req_msg_inst_for_get(st_things_get_request_message_s *req_msg)
{
	RET_IF_PARAM_IS_NULL(TAG, req_msg);

	things_free(req_msg->resource_uri);
	things_free(req_msg->query);
	things_free(req_msg->property_key);

	req_msg->query = NULL;
	req_msg->resource_uri = NULL;
	req_msg->property_key = NULL;
	req_msg->get_query_value = NULL;
	req_msg->has_property_key = NULL;

	things_free(req_msg);
}

static void destroy_req_msg_inst_for_post(st_things_set_request_message_s *req_msg, bool destroy_payload)
{
	RET_IF_PARAM_IS_NULL(TAG, req_msg);

	things_free(req_msg->resource_uri);
	things_free(req_msg->query);

	if (NULL != req_msg->rep) {
		destroy_representation_inst_internal(req_msg->rep, destroy_payload);
	}

	req_msg->resource_uri = NULL;
	req_msg->query = NULL;
	req_msg->rep = NULL;
	req_msg->get_query_value = NULL;

	things_free(req_msg);
}

static st_things_get_request_message_s *create_req_msg_inst_for_get(const char *res_uri, const char *query)
{
	st_things_get_request_message_s *req_msg = (st_things_get_request_message_s *) things_malloc(sizeof(st_things_get_request_message_s));
	RET_VAL_IF_NULL(TAG, req_msg, "Failed to allocate memory for GET request message.", NULL);

	req_msg->resource_uri = things_clone_string(res_uri);
	RET_VAL_IF_NULL(TAG, req_msg->resource_uri, "Failed to clone the resource uri.", NULL);

	if (NULL != query && strlen(query) > 1) {
		req_msg->query = things_clone_string(query);
	} else {
		req_msg->query = NULL;
	}
	req_msg->property_key = NULL;
	req_msg->get_query_value = &get_query_value_for_get_req;
	req_msg->has_property_key = &is_property_key_exist;
	return req_msg;
}

static st_things_set_request_message_s *create_req_msg_inst_for_post(const char *res_uri, const char *query, OCRepPayload *req_payload)
{
	st_things_set_request_message_s *req_msg = (st_things_set_request_message_s *) things_malloc(sizeof(st_things_set_request_message_s));
	RET_VAL_IF_NULL(TAG, req_msg, "Failed to allocate memory for SET request message.", NULL);

	req_msg->resource_uri = things_clone_string(res_uri);
	RET_VAL_IF_NULL(TAG, req_msg->resource_uri, "Failed to clone the resource uri.", NULL);

	if (NULL != query && strlen(query) > 1) {
		req_msg->query = things_clone_string(query);
	} else {
		req_msg->query = NULL;
	}

	req_msg->rep = create_representation_inst_internal(req_payload);
	if (NULL == req_msg->rep) {
		THINGS_LOG_E(TAG, "Failed to create representation for SET request message.");
		destroy_req_msg_inst_for_post(req_msg, false);
		return NULL;
	}

	req_msg->get_query_value = &get_query_value_for_post_req;
	return req_msg;
}

/*
 * Helper method to get the response from application for GET requests.
 * The 'resp_payload' parameter will be used to get the response.
 * Common properties of the resource such as rt, if and links will not be set by this method.
 */
bool handle_get_req_helper(const char *res_uri, const char *query, OCRepPayload *resp_payload)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, resp_payload);
	RET_FALSE_IF_NULL(TAG, g_handle_get_req_cb, "Request callback for GET is NULL.");

	// Setup the request message.
	st_things_get_request_message_s *req_msg = create_req_msg_inst_for_get(res_uri, query);
	RET_FALSE_IF_NULL(TAG, req_msg, "Failed to create request message for GET.");

	/* Based on the resource type of the request (specified in the query parameters like rt=oic.r.switch.binary) and
	   interface type in the query parameter, add proper set of property keys in the request message.
	   Application's response representation should include
	   only those properties which are present in the request message. */
	char *res_type = NULL;
	int count = 0;
	bool destroy_props = false;
	struct things_attribute_info_s **properties = NULL;

	// Get resource type from query parameter
	if (NULL != query && strlen(query) > 0) {
		bool found = false;
		bool result = get_query_value_internal(query, OC_RSRVD_RESOURCE_TYPE, &res_type, &found);
		if (found && !result) {	// If query is present but API returns false.
			THINGS_LOG_E(TAG, "Failed to get the resource type from query parameter(%s).", query);
			destroy_req_msg_inst_for_get(req_msg);
			return false;
		}
	}
	// Get supported set of properties based on resource type query parameter.
	// If resource type is not available, then get the properties based on resource uri.
	bool res = get_supported_properties(res_type, res_uri, &count, &properties, &destroy_props);
	if (!res) {
		THINGS_LOG_E(TAG, "Failed to get the resource properties based on its type or uri.");
		destroy_req_msg_inst_for_get(req_msg);
		things_free(res_type);
		return false;
	}
	// Calculate the length of all property keys
	int total_length_of_prop_keys = 0;
	for (int index = 0; index < count; index++) {
		if (NULL != properties[index]) {
			total_length_of_prop_keys += strlen(properties[index]->key);
		}
	}
	total_length_of_prop_keys += count;	// For delimiter
	total_length_of_prop_keys += 1;	// For null character

	// Allocate memory for holding property keys with delimiters
	req_msg->property_key = (char *)things_calloc(total_length_of_prop_keys, sizeof(char));
	if (!req_msg->property_key) {
		THINGS_LOG_E(TAG, "Failed to allocate memory for property key in GET request message.");
		if (destroy_props) {
			things_free(properties);
		}
		destroy_req_msg_inst_for_get(req_msg);
		things_free(res_type);
		return false;
	}
	// Get interface type from query parameter.
	char *if_type = NULL;
	if (NULL != query && strlen(query) > 0) {
		bool found = false;
		bool result = get_query_value_internal(query, OC_RSRVD_INTERFACE, &if_type, &found);
		if (found && !result) {	// If query is present but API returns false.
			THINGS_LOG_E(TAG, "Failed to get the interface type from query parameter(%s).", query);
			if (destroy_props) {
				things_free(properties);
			}
			destroy_req_msg_inst_for_get(req_msg);
			things_free(res_type);
			return false;
		}
	}

	bool handled = false;
	if (NULL != if_type && strlen(if_type) > 0) {
		THINGS_LOG_D(TAG, "Interface type is (%s).", if_type);
		if (0 == strncmp(if_type, OC_RSRVD_INTERFACE_READ, strlen(OC_RSRVD_INTERFACE_READ))) {
			// Add all the attributes which are readable.
			for (int index = 0; index < count; index++) {
				if (NULL != properties[index] && IS_READABLE(properties[index]->rw)) {
					add_property_key_in_get_req_msg(req_msg, properties[index]->key);
				}
			}
			handled = true;
		} else if (0 == strncmp(if_type, ST_THINGS_RSRVD_INTERFACE_READWRITE, strlen(ST_THINGS_RSRVD_INTERFACE_READWRITE))) {
			// Add all the attributes which are readable and writable.
			for (int index = 0; index < count; index++) {
				if (NULL != properties[index] && IS_READABLE(properties[index]->rw) && IS_WRITABLE(properties[index]->rw)) {
					add_property_key_in_get_req_msg(req_msg, properties[index]->key);
				}
			}
			handled = true;
		}
	}

	if (!handled) {				// For all other cases, add all the property keys.
		for (int index = 0; index < count; index++) {
			if (NULL != properties[index]) {
				add_property_key_in_get_req_msg(req_msg, properties[index]->key);
			}
		}
	}
	// Remove 'rt' and 'if' from query parameters
	if (NULL != query && strlen(query) > 0) {
		if (strstr(req_msg->query, OC_RSRVD_RESOURCE_TYPE) != NULL) {
			remove_query_parameter(req_msg->query, OC_RSRVD_RESOURCE_TYPE);
		}
		if (strstr(req_msg->query, OC_RSRVD_INTERFACE) != NULL) {
			remove_query_parameter(req_msg->query, OC_RSRVD_INTERFACE);
		}
	}
	// Setup the response representation for application. This representation will be handed over to the application.
	st_things_representation_s *things_resp_rep = create_representation_inst_internal(resp_payload);
	if (NULL != things_resp_rep) {
		res = g_handle_get_req_cb(req_msg, things_resp_rep);
		THINGS_LOG_D(TAG, "The result of application's callback : %s.", res ? "true" : "false");
		destroy_representation_inst_internal(things_resp_rep, false);
	} else {
		THINGS_LOG_E(TAG, "Failed to create response representation.");
		res = false;
	}

	destroy_req_msg_inst_for_get(req_msg);
	if (destroy_props) {
		things_free(properties);
	}
	things_free(res_type);
	things_free(if_type);

	return res;
}

static int handle_get_req_on_single_rsrc(things_resource_s *single_rsrc)
{
	RET_VAL_IF_PARAM_IS_NULL(TAG, single_rsrc, 0);

	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_VAL_IF_NULL(TAG, resp_rep, "Failed to create response representation.", 0);

	bool result = OCRepPayloadSetUri(resp_rep->payload, single_rsrc->uri);
	if (!result) {
		THINGS_LOG_E(TAG, "Failed to set the resource uri(%s) in response payload.", single_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return 0;
	}

	THINGS_LOG_D(TAG, "Resource uri(%s) is set in the response payload.", single_rsrc->uri);

	// Set the common properties in the payload (Only for baseline interface).
	char *if_type = NULL;
	if (NULL != single_rsrc->query && strlen(single_rsrc->query) > 0) {
		bool found = false;
		bool result = get_query_value_internal(single_rsrc->query, OC_RSRVD_INTERFACE, &if_type, &found);
		if (found && !result) {	// If query is present but API returns false.
			THINGS_LOG_E(TAG, "Failed to get the interface type from query parameter(%s).", single_rsrc->query);
			things_release_representation_inst(resp_rep);
			return 0;
		}
	}

	if (NULL == if_type || strlen(if_type) < 1 || !strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT))) {
		if (!add_common_props(single_rsrc, false, resp_rep->payload)) {
			THINGS_LOG_E(TAG, "Failed to add the common properties in response payload.");
			things_release_representation_inst(resp_rep);
			things_free(if_type);
			return 0;
		}
		THINGS_LOG_D(TAG, "Added common properties in response payload.");
	}
	// Get the resource's properties from the application.
	result = handle_get_req_helper(single_rsrc->uri, single_rsrc->query, resp_rep->payload);
	if (!result) {
		things_release_representation_inst(resp_rep);
		things_free(if_type);
		return 0;
	}
	// Set the response representation in the resource.
	single_rsrc->things_set_representation(single_rsrc, resp_rep);

	things_free(if_type);
	return 1;
}

/*
 * Helper method to get the response from application for POST requests.
 * The 'resp_payload' parameter will be used to get the response.
 * Common properties of the resource such as rt, if and links will not be set by this method.
 */
bool handle_post_req_helper(const char *res_uri, const char *query, OCRepPayload *req_payload, OCRepPayload *resp_payload)
{
	RET_FALSE_IF_PARAM_IS_NULL(TAG, req_payload);
	RET_FALSE_IF_PARAM_IS_NULL(TAG, resp_payload);
	RET_FALSE_IF_NULL(TAG, g_handle_set_req_cb, "Request callback for SET is NULL.");

	// Setup the response representation for application. This representation will be handed over to the application.
	st_things_representation_s *things_resp_rep = create_representation_inst_internal(resp_payload);
	RET_FALSE_IF_NULL(TAG, things_resp_rep, "Failed to create response representation.");

	// Setup the request message.
	st_things_set_request_message_s *req_msg = create_req_msg_inst_for_post(res_uri, query, NULL);
	if (NULL == req_msg) {
		THINGS_LOG_E(TAG, "Failed to create the request message for SET.");
		destroy_representation_inst_internal(things_resp_rep, false);
		return false;
	}

	/* Based on the resource type of the request (specified in the query parameters like rt=oic.r.switch.binary) and
	   interface type in the query parameter, add proper set of properties in the request representation.
	   Application's response representation should include
	   only the properties present in the request representation. */

	char *res_type = NULL;
	int count = 0;
	bool destroy_props = false;
	struct things_attribute_info_s **properties = NULL;

	// Get resource type from query parameter
	if (NULL != query && strlen(query) > 0) {
		bool found = false;
		bool result = get_query_value_internal(query, OC_RSRVD_RESOURCE_TYPE, &res_type, &found);
		if (found && !result) {	// If query is present but API returns false.
			THINGS_LOG_E(TAG, "Failed to get the resource type from query parameter(%s).", query);
			destroy_representation_inst_internal(things_resp_rep, false);
			destroy_req_msg_inst_for_post(req_msg, true);
			return false;
		}
	}
	// Get necessary set of properties.
	bool res = get_supported_properties(res_type, res_uri, &count, &properties, &destroy_props);
	if (!res) {
		THINGS_LOG_E(TAG, "Failed to get the resource properties based on its type or uri.");
		destroy_representation_inst_internal(things_resp_rep, false);
		destroy_req_msg_inst_for_post(req_msg, true);
		things_free(res_type);
		return false;
	}

	THINGS_LOG_D(TAG, "Total number of supported properties: %d.", count);

	res = true;
	// Check whether the request has all mandatory properties.
	for (int index = 0; index < count; index++) {
		if (NULL == properties[index]) {
			THINGS_LOG_E(TAG, "Property at index(%d) is NULL.", index);
			res = false;
			break;
		}

		if (!IS_WRITABLE(properties[index]->rw)) {
			if (!OCRepPayloadIsNull(req_payload, properties[index]->key)) {
				THINGS_LOG_E(TAG, "(%s) is present in the request.", properties[index]->key);
				res = false;
				break;
			}
		}
	}

	if (!res) {
		if (destroy_props) {
			things_free(properties);
		}
		destroy_representation_inst_internal(things_resp_rep, false);
		destroy_req_msg_inst_for_post(req_msg, true);
		things_free(res_type);
		return false;
	}

	for (int index = 0; index < count && NULL != properties[index]; index++) {
		bool exist = (false == OCRepPayloadIsNull(req_payload, properties[index]->key));
		THINGS_LOG_D(TAG, "Is property(%s) present in request payload?: %s", properties[index]->key, exist ? "Yes" : "No");
		if (exist && IS_WRITABLE(properties[index]->rw)) {
			res = add_property_in_post_req_msg(req_msg, req_payload, properties[index]);
			if (!res) {
				break;
			}
		}
	}

	if (res) {
		// Remove 'rt' and 'if' from query parameters.
		if (strstr(req_msg->query, OC_RSRVD_RESOURCE_TYPE) != NULL) {
			remove_query_parameter(req_msg->query, OC_RSRVD_RESOURCE_TYPE);
		}
		if (strstr(req_msg->query, OC_RSRVD_INTERFACE) != NULL) {
			remove_query_parameter(req_msg->query, OC_RSRVD_INTERFACE);
		}
		res = g_handle_set_req_cb(req_msg, things_resp_rep);
		THINGS_LOG_D(TAG, "The result of application's callback : %s", res ? "true" : "false");
	} else {
		THINGS_LOG_E(TAG, "Failed to add properties in request message.");
	}

	if (destroy_props) {
		things_free(properties);
	}
	destroy_req_msg_inst_for_post(req_msg, true);
	destroy_representation_inst_internal(things_resp_rep, false);
	things_free(res_type);
	return true;
}

static int handle_post_req_on_single_rsrc(things_resource_s *single_rsrc)
{
	RET_VAL_IF_PARAM_IS_NULL(TAG, single_rsrc, 0);

	// Retrieve the request representation. This representation will hold all the input properties of post request.
	// Payload in this representation will be used to form the request message which will be given to the application.
	struct things_representation_s *req_rep = NULL;
	bool rep_exist = single_rsrc->things_get_representation(single_rsrc, &req_rep);
	if (!rep_exist || NULL == req_rep || NULL == req_rep->payload) {
		THINGS_LOG_E(TAG, "Empty payload in POST request.");
		return 0;				// TODO: When a post request comes with empty payload, how do we handle?
	}
	// Setup the response representation. This representation will be handed over to the underlying stack.
	// The payload which is passed as a parameter will be updated with the common properties.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_VAL_IF_NULL(TAG, resp_rep, "Failed to create response representation.", 0);

	if (!OCRepPayloadSetUri(resp_rep->payload, single_rsrc->uri)) {
		THINGS_LOG_E(TAG, "Failed to set the resource uri(%s) in response payload.", single_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return 0;
	}

	THINGS_LOG_D(TAG, "Resource uri(%s) is set in the response payload.", single_rsrc->uri);

	// Get interface type from query parameter
	char *if_type = NULL;
	if (NULL != single_rsrc->query && strlen(single_rsrc->query) > 0) {
		bool found = false;
		bool result = get_query_value_internal(single_rsrc->query, OC_RSRVD_INTERFACE, &if_type, &found);
		if (found && !result) {	// If query is present but API returns false.
			THINGS_LOG_E(TAG, "Failed to get the interface type from query parameter(%s).", single_rsrc->query);
			things_release_representation_inst(resp_rep);
			return 0;
		}
	}
	// Set the common properties in the payload (Only for baseline interface).
	// The payload which is passed as a parameter will be updated with the common properties.
	if (NULL == if_type || !strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT))) {
		if (!add_common_props(single_rsrc, false, resp_rep->payload)) {
			THINGS_LOG_E(TAG, "Failed to add the common properties in response payload.");
			things_release_representation_inst(resp_rep);
			things_free(if_type);
			return 0;
		}
		THINGS_LOG_D(TAG, "Added common properties in response payload.");
	}
	// Give the request properties to application and get the response back.
	bool res = handle_post_req_helper(single_rsrc->uri, single_rsrc->query, req_rep->payload, resp_rep->payload);
	if (res) {
		// Set the response representation in the resource.
		single_rsrc->things_set_representation(single_rsrc, resp_rep);
	} else {
		things_release_representation_inst(resp_rep);
	}
	things_free(if_type);
	return res ? 1 : 0;
}

int handle_get_request_cb(struct things_resource_s *resource)
{
	THINGS_LOG_D(TAG, "Received a GET request callback");

	RET_VAL_IF_PARAM_IS_NULL(TAG, resource, 0);
	RET_VAL_IF_NULL(TAG, g_handle_get_req_cb, "Request callback for GET is NULL.", 0);
	RET_VAL_IF_EXPR_IS_TRUE(TAG, (NULL == resource->uri || strlen(resource->uri) < 1), "Resource URI is invalid", 0);

	int result = 0;
#ifdef CONFIG_ST_THINGS_COLLECTION
	bool collection = things_is_collection_resource(resource->uri);
#else
	bool collection = false;
#endif

	THINGS_LOG_D(TAG, "Resource URI : %s (%s resource)", resource->uri, collection ? "collection" : "single");
	THINGS_LOG_D(TAG, "Query Parameter: %s", resource->query);

	if (collection) {
#ifdef CONFIG_ST_THINGS_COLLECTION
		result = handle_get_req_on_collection_rsrc(resource);
#endif
	} else {
		result = handle_get_req_on_single_rsrc(resource);
	}

	if (result) {
		THINGS_LOG_D(TAG, "Handled GET request for %s resource successfully.", collection ? "collection" : "single");
	} else {
		THINGS_LOG_D(TAG, "Failed to handle the GET request for %s resource.", collection ? "collection" : "single");
	}

	return result;
}

int handle_set_request_cb(struct things_resource_s *resource)
{
	THINGS_LOG_D(TAG, "Received a SET request callback");

	RET_VAL_IF_PARAM_IS_NULL(TAG, resource, 0);
	RET_VAL_IF_NULL(TAG, g_handle_set_req_cb, "Request callback for SET is NULL", 0);
	RET_VAL_IF_EXPR_IS_TRUE(TAG, (NULL == resource->uri || strlen(resource->uri) < 1), "Resource URI is invalid", 0);

	int result = 0;
#ifdef CONFIG_ST_THINGS_COLLECTION
	bool collection = things_is_collection_resource(resource->uri);
#else
	bool collection = false;
#endif

	THINGS_LOG_D(TAG, "Resource URI : %s (%s resource)", resource->uri, collection ? "collection" : "single");
	THINGS_LOG_D(TAG, "Query Parameter: %s", resource->query);

	if (collection) {
#ifdef CONFIG_ST_THINGS_COLLECTION
		result = handle_post_req_on_collection_rsrc(resource);
#endif
	} else {
		result = handle_post_req_on_single_rsrc(resource);
	}

	if (result) {
		THINGS_LOG_D(TAG, "Handled SET request for %s resource successfully.", collection ? "collection" : "single");
	} else {
		THINGS_LOG_D(TAG, "Failed to handle the SET request for %s resource.", collection ? "collection" : "single");
	}

	return result;
}

int register_request_handler_cb(st_things_get_request_cb get_cb, st_things_set_request_cb set_cb)
{
	if (NULL == get_cb) {
		THINGS_LOG_E(TAG, "Request callback for GET is NULL.");
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	if (NULL == set_cb) {
		THINGS_LOG_E(TAG, "Request callback for SET is NULL.");
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	g_handle_get_req_cb = get_cb;
	g_handle_set_req_cb = set_cb;

	return ST_THINGS_ERROR_NONE;
}
