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

#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "things_def.h"
#include "things_common.h"
#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "ocpayload.h"
#include "things_resource.h"
#include "octypes.h"

#define TAG "[things_resource]"

//#*******************************************************************************//
//#                 Function definitions for the THINGS Resources.
//#*******************************************************************************//

void set_dev_addr(struct things_resource_s *p_res, void *dev_addr)
{
	if (dev_addr == NULL) {
		p_res->dev_addr = NULL;
		return;
	}
	(p_res->dev_addr) = (OCDevAddr *) things_malloc(sizeof(OCDevAddr));
	if (NULL != p_res->dev_addr) {
		memcpy((p_res->dev_addr), dev_addr, sizeof(OCDevAddr));
	}
	// p_res->dev_addr = dev_addr;
}

void *get_dev_addr(struct things_resource_s *p_res)
{
	return (p_res->dev_addr);
}

void set_uri(struct things_resource_s *res, const char *key)
{
	//THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (key == NULL) {
		return;
	}

	if (res->uri != NULL) {
		things_free(res->uri);
		res->uri = NULL;
	}
	res->uri = (char *)things_malloc(sizeof(char) *strlen(key) + 1);
	memset(res->uri, 0, strlen(key) + 1);
	things_strncpy(res->uri, key, strlen(key) + 1);

	if (NULL != res->rep) {
		OCRepPayloadSetUri(res->rep->payload, key);
	} else {
		THINGS_LOG_E(TAG, "Set URI Failed, No Representation Yet");
	}

	//THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

void set_error(struct things_resource_s *res, double error)
{
	res->error = error;
}

void things_set_value(struct things_representation_s *rep, char *key, char *value)
{
#ifdef __ST_THINGS_RTOS__
	if (value != NULL && key != NULL) {
		OCRepPayloadSetPropString(rep->payload, key, value);
	}
#else
	OCRepPayloadSetPropString(rep->payload, key, value);
#endif
}

void things_set_bool_value(struct things_representation_s *rep, char *key, bool value)
{
	OCRepPayloadSetPropBool(rep->payload, key, value);
}

void things_set_int_value(struct things_representation_s *rep, char *key, int64_t value)
{
	OCRepPayloadSetPropInt(rep->payload, key, value);
}

void things_set_double_value(struct things_representation_s *rep, char *key, double value)
{
	OCRepPayloadSetPropDouble(rep->payload, key, value);
}

void things_set_byte_value(struct things_representation_s *rep, char *key, uint8_t *value, size_t size)
{
	OCByteString b_val = { value, size };

	if (b_val.len < 1) {
		THINGS_LOG_E(TAG, "No byte value to Set");
	} else {
		OCRepPayloadSetPropByteString(rep->payload, key, b_val);
	}
}

bool things_set_object_value(struct things_representation_s *mother, char *key, struct things_representation_s *child)
{
	return OCRepPayloadSetPropObject(mother->payload, key, child->payload);
}

static OCRepPayloadValue *things_rep_payload_find_values(const OCRepPayload *payload, const char *name)
{
	if (!payload || !name) {
		return NULL;
	}

	OCRepPayloadValue *val = payload->values;
	while (val) {
		if (0 == strncmp(val->name, name, strlen(name))) {
			return val;
		}
		val = val->next;
	}

	return NULL;
}

bool things_set_arrayvalue(struct things_representation_s *mother, char *key, int length, struct things_representation_s **children)
{
	bool things_set_value = false;
	if (length < 1) {
		THINGS_LOG_E(TAG, "DATA LENGTH IS ZERO!!!!!!");
		return things_set_value;
	}
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };

	OCRepPayload *payloads[length];

	for (int iter = 0; iter < length; iter++) {
		payloads[iter] = children[iter]->payload;
	}
	//  This is codesnippet for creating Array type of value inside the payload..
	things_set_value = OCRepPayloadSetPropObjectArray(mother->payload, key, (const OCRepPayload **)(payloads), dimensions);

	// THINGS_LOG_V(TAG, " Length : %d", length);
	// OCRepPayloadSetPropInt(mother->payload, SEC_ATTRIBUTE_LENGTH, length);

	//  Do we need to have those children representation
	//       with this resource instance????
	return things_set_value;
}

bool things_get_arrayvalue(struct things_representation_s *mother, char *key, int *length, struct things_representation_s ***children)
{
	//THINGS_LOG_E(TAG, "NOt Supported This Function Yet");
	bool find_value = false;
	THINGS_LOG_D(TAG, "There're (%d) Number of children resources in the Payload : %d", mother->num_children);

	// if( OCRepPayloadGetPropInt((OCRepPayload*)(mother->payload), SEC_ATTRIBUTE_LENGTH, &(mother->num_children) ) )
	OCRepPayloadValue *payload_value = things_rep_payload_find_values((OCRepPayload *)(mother->payload), key);
	if (NULL != payload_value) {
		OCRepPayload **payload_values = NULL;
		size_t dimension_size = calcDimTotal(payload_value->arr.dimensions);
		size_t dimensions[3] = { dimension_size, 0, 0 };

		THINGS_LOG_D(TAG, "Dimension size in the Payload : %d", dimension_size);
		//    This is testing code only... will be removed...
		find_value = OCRepPayloadGetPropObjectArray((OCRepPayload *)(mother->payload), key, &payload_values, dimensions);
		THINGS_LOG_D(TAG, "Find Value : %d", find_value);
		if (find_value) {
			*children = (things_representation_s **) things_malloc(sizeof(things_representation_s *) *dimension_size);

			for (int iter = 0; iter < dimension_size; iter++) {
				(*children)[iter] = things_create_representation_inst(payload_values[iter]);
				/*! Added by st_things for memory Leak fix
				 */
				OCRepPayloadDestroy(payload_values[iter]);
			}
			/*! Added by st_things for memory Leak fix
			 */
			things_free(payload_values);
			*length = mother->num_children = dimension_size;
		}
	} else {
		THINGS_LOG_E(TAG, "DATA NOT EXIST~!!!!");
	}

	return find_value;
}

void things_set_string_arrayvalue(struct things_representation_s *mother, char *key, int length, char **array)
{
	if (length < 1) {
		THINGS_LOG_E(TAG, "DATA LENGTH IS ZERO!!!!!!");
		return;
	}
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };

	//    This is codesnippet for creating Array type of value inside the payload..
	OCRepPayloadSetStringArray(mother->payload, key, (const char **)array, dimensions);
}

bool things_get_string_arrayvalue(struct things_representation_s *mother, char *key, int *length, char ***array)
{
	THINGS_LOG_D(TAG, "There're (%d) Number of children resources in the Payload : %d", mother->num_children);

	bool find_value = false;

	OCRepPayloadValue *payload_value = NULL;

	payload_value = things_rep_payload_find_values((OCRepPayload *)(mother->payload), key);

	if (payload_value != NULL) {
		size_t dimension_size = calcDimTotal(payload_value->arr.dimensions);
		size_t dimensions[3] = { dimension_size, 0, 0 };

		find_value = OCRepPayloadGetStringArray((OCRepPayload *)(mother->payload), key, array, dimensions);
		THINGS_LOG_D(TAG, "Find Value : %d", find_value);
		if (find_value) {

			*length = dimension_size;
		}
	} else {
		THINGS_LOG_E(TAG, "DATA NOT EXIST~!!!!");
	}

	return find_value;
}

void things_set_double_arrayvalue(struct things_representation_s *mother, char *key, int length, double *array)
{
	if (length < 1) {
		THINGS_LOG_E(TAG, "DATA LENGTH IS ZERO!!!!!!");
		return;
	}
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };

	OCRepPayloadSetDoubleArray(mother->payload, key, array, dimensions);
}

bool things_get_double_arrayvalue(struct things_representation_s *mother, char *key, int *length, double **array)
{
	bool find_value = false;

	OCRepPayloadValue *payload_value = things_rep_payload_find_values((OCRepPayload *)(mother->payload), key);
	if (NULL != payload_value) {

		size_t dimension_size = calcDimTotal(payload_value->arr.dimensions);
		size_t dimensions[3] = { dimension_size, 0, 0 };

		THINGS_LOG_D(TAG, "Dimension size in the Payload : %d", dimension_size);

		find_value = OCRepPayloadGetDoubleArray((OCRepPayload *)(mother->payload), key, array, dimensions);

		THINGS_LOG_D(TAG, "Find Value : %d", find_value);
		if (find_value) {
			*length = dimension_size;
		}
	} else {
		THINGS_LOG_E(TAG, "DATA NOT EXIST~!!!!");
	}

	return find_value;
}

void things_set_int_arrayvalue(struct things_representation_s *mother, char *key, int length, int64_t *array)
{
	if (length < 1) {
		THINGS_LOG_E(TAG, "DATA LENGTH IS ZERO!!!!!!");
		return;
	}
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { length, 0, 0 };

	OCRepPayloadSetIntArray(mother->payload, key, array, dimensions);
}

bool things_get_int_arrayvalue(struct things_representation_s *mother, char *key, int *length, int64_t **array)
{
	bool find_value = false;

	OCRepPayloadValue *payload_value = things_rep_payload_find_values((OCRepPayload *)(mother->payload), key);

	if (NULL != payload_value) {
		size_t dimension_size = calcDimTotal(payload_value->arr.dimensions);
		size_t dimensions[3] = { dimension_size, 0, 0 };

		THINGS_LOG_D(TAG, "Dimension size in the Payload : %d", dimension_size);

		find_value = OCRepPayloadGetIntArray((OCRepPayload *)(mother->payload), key, array, dimensions);

		THINGS_LOG_D(TAG, "Find Value : %d", find_value);
		if (find_value) {
			*length = dimension_size;
		}
	} else {
		THINGS_LOG_E(TAG, "DATA NOT EXIST~!!!!");
	}
	return find_value;
}

void set_representation(struct things_resource_s *res, struct things_representation_s *rep)
{
	if (res->rep) {
		things_release_representation_inst(res->rep);
		res->rep = NULL;
	}
	res->rep = rep;
}

//   Getter should be refactored to deliver the request value as its return
//      not through the inserted param.
void get_uri(struct things_resource_s *res, char **value)
{
	if (res->uri != NULL) {
		*value = (char *)things_malloc(sizeof(char) *strlen(res->uri) + 1);
		memset(*value, 0, strlen(res->uri) + 1);
		things_strncpy(*value, res->uri, strlen(res->uri) + 1);
	}
}

int get_num_of_res_types(struct things_resource_s *res)
{
	uint8_t num = 0;
	OCGetNumberOfResourceTypes((OCResourceHandle) res->resource_handle, &num);
	return (int)num;
}

const char *get_res_type(struct things_resource_s *res, int index)
{
	const char *resourcType = OCGetResourceTypeName((OCResourceHandle) res->resource_handle, index);
	THINGS_LOG_D(TAG, "=====>  RH : %x cnt : %d", res->resource_handle, index);
	THINGS_LOG_D(TAG, "=====>  RT : %s ", resourcType);

	return resourcType;
}

int get_num_of_inf_types(struct things_resource_s *res)
{
	uint8_t num = 0;
	OCGetNumberOfResourceInterfaces((OCResourceHandle) res->resource_handle, &num);
	return (int)num;
}

const char *get_inf_type(struct things_resource_s *res, int index)
{
	const char *interface_type = OCGetResourceInterfaceName((OCResourceHandle) res->resource_handle, index);
	THINGS_LOG_D(TAG, "=====>  RH : %x cnt : %d", res->resource_handle, index);
	THINGS_LOG_D(TAG, "=====>  IT : %s ", interface_type);

	return interface_type;
}

bool things_get_value(struct things_representation_s *rep, char *key, char **value)
{
	return OCRepPayloadGetPropString((OCRepPayload *) rep->payload, key, (char **)value);
}

bool things_get_bool_value(struct things_representation_s *rep, char *key, bool *value)
{
	return OCRepPayloadGetPropBool((OCRepPayload *) rep->payload, key, value);
}

bool things_get_int_value(struct things_representation_s *rep, char *key, int64_t *value)
{
	return OCRepPayloadGetPropInt((OCRepPayload *) rep->payload, key, value);
}

bool things_get_double_value(struct things_representation_s *rep, char *key, double *value)
{
	return OCRepPayloadGetPropDouble((OCRepPayload *) rep->payload, key, value);
}

bool things_get_byte_value(struct things_representation_s *rep, char *key, uint8_t **value, size_t *size)
{
	OCByteString b_val = { NULL, 0 };
	bool ret = OCRepPayloadGetPropByteString((OCRepPayload *) rep->payload, key, &b_val);

	if (true == ret) {
		(*size) = b_val.len;
		(*value) = (uint8_t *) things_malloc((*size) + 1);
		memcpy((*value), b_val.bytes, b_val.len);

		if (b_val.bytes != NULL) {
			things_free(b_val.bytes);
			b_val.bytes = NULL;
		}
	}

	return ret;
}

bool things_get_object_value(struct things_representation_s *mother, char *key, struct things_representation_s *child)
{
	OCRepPayload *payload = NULL;
	bool ret = OCRepPayloadGetPropObject(mother->payload, key, &payload);

	if (true == ret && NULL != payload) {
		if (child->payload) {
			OCRepPayloadDestroy(child->payload);
			child->payload = NULL;
		}
		child->payload = payload;
	}
	return ret;
}

bool get_query(struct things_resource_s *res, char *key, char **value)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (NULL == key) {
		return 0;
	} else if (NULL == res) {
		return 0;
	} else if (NULL == res->query) {
		return 0;
	} else if (strlen(res->query) < 1) {
		return 0;
	}

	THINGS_LOG_D(TAG, "Query is %s.", res->query);

	*value = NULL;

	char *p_buff = NULL, *p_origin = NULL;
	char *p_ptr = NULL;
	char *p_ptr2 = NULL;

	p_origin = p_buff = (char *)things_malloc(strlen(res->query) + 1);

	if (NULL == p_buff || NULL == p_origin) {
		return 0;
	}

	memset(p_buff, 0, strlen(res->query) + 1);
	THINGS_LOG_D(TAG, "p_buff is initialized by 0.");
	memcpy(p_buff, res->query, strlen(res->query) + 1);
	THINGS_LOG_D(TAG, "p_buff is Initialized as", res->query);

	p_ptr = strtok(p_buff, QUERY_DELIMITER);
	if (p_ptr != NULL) {
		p_ptr2 = p_ptr;
	} else {
		things_free(p_origin);
		return 0;
	}
	//while (p_ptr != NULL)
	while (p_ptr2 != NULL) {
		if (strncmp(p_ptr2, key, strlen(key)) == 0) {
			THINGS_LOG_D(TAG, "\tFind Query : %s", p_ptr2 + strlen(key) + 1);

			things_string_duplicate(p_ptr2 + strlen(key) + 1, value);
			if (NULL == *value) {
				things_free(p_origin);
				return 1;
			}
			THINGS_LOG_D(TAG, "\tRESULT : %s", *value);
			break;
		}

		p_ptr2 = strtok(NULL, QUERY_DELIMITER);
	}

	things_free(p_origin);

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return 1;
}

struct things_resource_s *get_children(struct things_resource_s *pParent)
{
	return pParent->next;
}

bool get_representation(struct things_resource_s *res, struct things_representation_s **rep)
{
	if (res->rep != NULL) {
		*rep = res->rep;
		return true;
	}

	return false;
}

void /*OCRepPayload */ *create_payload(struct things_resource_s *res, char *query)
{
	//    To provide identical pattern for handling the result
	//       allocating new memory for the payload to delete later
	OCRepPayload *payload = OCRepPayloadClone((OCRepPayload *)(res->rep->payload));
	THINGS_LOG_D(TAG, "Query : %s", query);

	if (query == NULL || (query != NULL && strlen(query) < 1)
		|| strstr(query, OIC_INTERFACE_ACTUATOR) != NULL || strstr(query, OIC_INTERFACE_SENSOR) != NULL) {
		THINGS_LOG_D(TAG, "Including properties & its values only");
		// Do nothing..
	} else if (strstr(query, OIC_INTERFACE_BASELINE)) {
		THINGS_LOG_D(TAG, "Including all the properties & its values");

		uint8_t index = 0;
		uint8_t number_of_interfaces = 0;
		uint8_t number_of_resource_type = 0;

		OCGetNumberOfResourceInterfaces((OCResourceHandle) res->resource_handle, &number_of_interfaces);

		THINGS_LOG_D(TAG, "@@  IF # : %d ", number_of_interfaces);
		//2.a Find interface type(s) & insert it/them into payload
		for (index = 0; index < number_of_interfaces; index++) {
			const char *interface = OCGetResourceInterfaceName((OCResourceHandle) res->resource_handle, index);
			THINGS_LOG_D(TAG, "=====>  IF : %s ", interface);
			OCRepPayloadAddInterface(payload, interface);
		}

		//3.a Find resource type & insert it into payload
		OCGetNumberOfResourceTypes((OCResourceHandle) res->resource_handle, &number_of_resource_type);
		THINGS_LOG_D(TAG, "@@  RT # : %d ", number_of_resource_type);
		for (index = 0; index < number_of_resource_type; index++) {
			const char *rt = OCGetResourceTypeName((OCResourceHandle) res->resource_handle, index);
			THINGS_LOG_D(TAG, "=====>  RT : %s ", rt);
			OCRepPayloadAddResourceType(payload, rt);
		}
	} else if (strstr(query, OC_RSRVD_INTERFACE_BATCH)) {
		THINGS_LOG_D(TAG, "Batch only supported by Collection Resource");
	} else if (strstr(query, OC_RSRVD_INTERFACE_LL)) {
		THINGS_LOG_D(TAG, "Link-List only supported by Collection Resource");
	} else {
		THINGS_LOG_D(TAG, "Not supporting Interface type : %s", query);
	}

	return payload;
}

void /*OCRepPayload */ *get_rep_payload(struct things_resource_s *res)
{
	if (res->rep != NULL) {
		// Create Payload for the mother resource
		// It applies not only single resource but also the collection resource..
		things_resource_s *p_temp;
		OCRepPayload *rep_payload;

		if (NULL != res->things_get_children(res) && (NULL != strstr(res->uri, URI_DEVICE_COL)) && (strstr(res->query, OIC_INTERFACE_BATCH) != NULL)) {
			p_temp = res->next;
			rep_payload = p_temp->things_create_payload(p_temp, p_temp->query);
			p_temp = p_temp->next;
		}

		else {
			rep_payload = res->things_create_payload(res, res->query);
			// Create payload for the children resource(s)
			p_temp = res->things_get_children(res);
		}

		while (NULL != p_temp) {
			if (NULL != p_temp->rep) {
				OCRepPayloadAppend(rep_payload, p_temp->things_create_payload(p_temp, p_temp->query));
			}
			p_temp = p_temp->next;
		}

		return rep_payload;
	} else {
		THINGS_LOG_E(TAG, "ROOT(Parent) Paylaod is NULL.");
		return NULL;
	}
}

bool is_supporting_interface_type(struct things_resource_s *res, char *query)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	bool result = false;
	uint8_t number_of_interfaces = 0;

	OCGetNumberOfResourceInterfaces((OCResourceHandle) res->resource_handle, &number_of_interfaces);

	THINGS_LOG_D(TAG, "@@  IF # : %d ", number_of_interfaces);
	//2.a Find interface type(s) & insert it/them into payload
	for (uint8_t index = 0; index < number_of_interfaces; index++) {
		const char *interface = OCGetResourceInterfaceName((OCResourceHandle) res->resource_handle, index);

		// THINGS_LOG_D(TAG, "Supporting Interface :: %s(%s)", query, interface);
		if (NULL != query) {
			if (strstr(query, interface) != NULL) {
				// THINGS_LOG_D(TAG, "Confirm Interface.");
				result = true;
				break;
			}
		}
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return result;
}

bool is_supporting_resource_type(struct things_resource_s *res, char *query)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	bool result = false;
	uint8_t number_of_resource_type = 0;

	OCGetNumberOfResourceTypes((OCResourceHandle) res->resource_handle, &number_of_resource_type);

	THINGS_LOG_D(TAG, "@@  RT # : %d ", number_of_resource_type);
	//2.a Find resource type(s) & insert it/them into payload
	for (uint8_t index = 0; index < number_of_resource_type; index++) {
		const char *rtype = OCGetResourceTypeName((OCResourceHandle) res->resource_handle, index);

		// THINGS_LOG_D(TAG, "Supporting rtype :: %s(%s)", query, rtype);
		if (strstr(query, rtype) != NULL) {
			// THINGS_LOG_D(TAG, "Confirm rtype.");
			result = true;
			break;
		}
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return result;
}

void set_command_id(things_resource_s *res, char *cmd_id)
{
	things_string_duplicate(cmd_id, &res->cmd_id);
}

things_resource_s *get_next(things_resource_s *res)
{
	if (NULL != res) {
		if (NULL != res->next) {
			return res->next;
		}
	}

	return NULL;
}

void add_child(things_resource_s *root, things_resource_s *child)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	things_resource_s *p_node = NULL;

	p_node = root;
	while (NULL != p_node->things_get_next(p_node)) {
		p_node = p_node->things_get_next(p_node);
	}

	p_node->next = child;
	root->size++;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

things_representation_s *things_create_representation_inst(void *rep_payload)
{
	things_representation_s *rep = (things_representation_s *) things_malloc(sizeof(things_representation_s));
	if (NULL == rep) {
		THINGS_LOG_E(TAG, THINGS_MEMORY_ERROR);
		return NULL;
	}

	rep->things_set_value = &things_set_value;
	rep->things_set_bool_value = &things_set_bool_value;
	rep->things_set_double_value = &things_set_double_value;
	rep->things_set_int_value = &things_set_int_value;
	rep->things_set_byte_value = &things_set_byte_value;
	rep->things_set_object_value = &things_set_object_value;

	rep->things_set_arrayvalue = &things_set_arrayvalue;
	rep->things_get_arrayvalue = &things_get_arrayvalue;
	rep->things_set_string_arrayvalue = &things_set_string_arrayvalue;
	rep->things_get_string_arrayvalue = &things_get_string_arrayvalue;

	rep->things_set_double_arrayvalue = &things_set_double_arrayvalue;
	rep->things_get_double_arrayvalue = &things_get_double_arrayvalue;
	rep->things_set_int_arrayvalue = &things_set_int_arrayvalue;
	rep->things_get_int_arrayvalue = &things_get_int_arrayvalue;

	rep->things_get_value = &things_get_value;
	rep->things_get_bool_value = &things_get_bool_value;
	rep->things_get_int_value = &things_get_int_value;
	rep->things_get_double_value = &things_get_double_value;
	rep->things_get_byte_value = &things_get_byte_value;
	rep->things_get_object_value = &things_get_object_value;

	rep->payload = NULL;
	rep->children_payload = NULL;
	rep->children = NULL;
	rep->num_children = 0;

	if (rep_payload != NULL) {
		rep->payload = OCRepPayloadClone((OCRepPayload *) rep_payload);
	} else {
		rep->payload = OCRepPayloadCreate();
	}

	return rep;
}

things_resource_s *create_resource_inst_impl(void *requesthd, void *resourcehd, void *query, void *rep_payload)
{
	things_resource_s *res = (things_resource_s *) things_malloc(sizeof(things_resource_s));
	if (NULL == res) {
		THINGS_LOG_E(TAG, THINGS_MEMORY_ERROR);
		return NULL;
	}

	res->error = 0;				// OC_EH_OK;

	res->dev_addr = NULL;

	res->things_set_dev_addr = &set_dev_addr;
	res->things_get_dev_addr = &get_dev_addr;

	res->things_set_uri = &set_uri;
	res->things_set_error = &set_error;

	res->things_set_command_id = &set_command_id;

	res->things_add_child = &add_child;
	res->things_get_children = &get_children;

	res->things_get_uri = &get_uri;
	res->things_get_res_type = &get_res_type;
	res->things_get_num_of_res_types = &get_num_of_res_types;
	res->things_get_inf_type = &get_inf_type;
	res->things_get_num_of_inf_types = &get_num_of_inf_types;
	res->things_set_representation = &set_representation;
	res->things_get_representation = &get_representation;

	res->things_get_query = &get_query;
	res->things_get_rep_payload = &get_rep_payload;
	res->things_create_payload = &create_payload;
	res->things_is_supporting_interface_type = &is_supporting_interface_type;
	res->things_is_supporting_resource_type = &is_supporting_resource_type;

	res->next = NULL;
	res->things_get_next = &get_next;

	res->resource_handle = resourcehd;
	res->request_handle = requesthd;

	res->uri = NULL;
	const char *uri = OCGetResourceUri(resourcehd);
	if (uri != NULL && strlen(uri) > 0) {
		res->uri = (char *)things_malloc(sizeof(char) * strlen(uri) + 1);
		memset(res->uri, 0, strlen(uri) + 1);
		things_strncpy(res->uri, uri, strlen(uri));
	}

	res->res_type = NULL;
	res->interface_type = NULL;
	res->req_type = 0;
	res->cmd_id = NULL;
	res->rep = NULL;
	res->query = NULL;
	res->size = 1;

	if (NULL != query) {
		things_string_duplicate((char *)query, &(res->query));
	}

	if (NULL != rep_payload) {
		res->rep = things_create_representation_inst(rep_payload);
	}
	// else
	// {
	//     THINGS_LOG_D(TAG, "Representation not created!!");
	// }

	return res;
}

things_resource_s *things_create_resource_inst(OCRequestHandle requesthd, OCResourceHandle resourcehd, void *query, void *rep_payload)
{
	things_resource_s *res = create_resource_inst_impl(requesthd,
							 resourcehd,
							 query,
							 rep_payload);

	if (NULL != rep_payload) {
		THINGS_LOG_D(TAG, "Representation Inserted to Clone~!!!");
		OCRepPayload *pl = ((OCRepPayload *) rep_payload)->next;

		things_resource_s *pTempRes = NULL;
		while (pl) {
			if (NULL != pl->uri) {
				pTempRes = create_resource_inst_impl(requesthd, resourcehd, NULL, pl);

				THINGS_LOG_D(TAG, "Child Resource URI : %s", pl->uri);
				pTempRes->things_set_uri(pTempRes, pl->uri);
				res->things_add_child(res, pTempRes);
				//res->size++;
			} else {
				THINGS_LOG_D(TAG, "No Uri in this payload for child, this payload was ignored.");
			}

			pl = pl->next;
		}
		THINGS_LOG_D(TAG, "\t Child Count : %d", res->size);
	} else {
		THINGS_LOG_D(TAG, "No Representation to Clone : %x", rep_payload);
	}

	return res;
}

things_resource_s *clone_resource_inst(things_resource_s *pori)
{
	if (pori == NULL) {
		return NULL;
	}

	things_resource_s *pclone = create_resource_inst_impl(pori->request_handle,
								pori->resource_handle,
								pori->query,
								(pori->rep == NULL) ? NULL : pori->rep->payload);
	pclone->things_set_uri(pclone, pori->uri);
	THINGS_LOG_D(TAG, "@@@@@@@@@@@@@@@ URI  %s", pclone->uri);

	things_resource_s *p_temp = pori->next;
	while (p_temp) {
		things_resource_s *p_new = create_resource_inst_impl(p_temp->request_handle,
								   p_temp->resource_handle,
								   p_temp->query,
								   ((p_temp->rep == NULL) ? NULL : p_temp->rep->payload));

		p_new->things_set_uri(p_new, p_temp->uri);
		p_temp->things_add_child(p_temp, p_new);
		p_temp = p_temp->next;
	}

	THINGS_LOG_D(TAG, "@@@@@@@@@@@@@@@ Address of cloned %x", pclone);
	return pclone;
}

void things_clone_resource_inst2(things_resource_s *pori, things_resource_s **pclone)
{
	if (pori == NULL) {
		return;
	}

	*pclone = create_resource_inst_impl(pori->request_handle, pori->resource_handle, pori->query, ((pori->rep == NULL) ? NULL : pori->rep->payload));

	(*pclone)->things_set_uri(*pclone, pori->uri);
	THINGS_LOG_D(TAG, "@@@@@@@@@@@@@@@ URI  %s", (*pclone)->uri);

	return;
}

void things_release_representation_inst(things_representation_s *rep)
{
	if (rep != NULL) {
		if (rep->children_payload != NULL) {
			THINGS_LOG_D(TAG, "Representation has %lld Children.", rep->num_children);
			for (int64_t iter = 0; iter < rep->num_children; iter++) {
				if ((rep->children_payload[iter]) != NULL) {
					THINGS_LOG_D(TAG, "\t RELEASED.");
					OCPayloadDestroy((OCPayload *)(rep->children_payload[iter]));
					rep->children_payload[iter] = NULL;
				}
			}
		}
		if (rep->payload != NULL) {
			OCPayloadDestroy((OCPayload *)(rep->payload));
			rep->payload = NULL;
		}

		things_free(rep);
		rep = NULL;
	}
}

void release_resource_inst_impl(things_resource_s *res)
{
	if (res != NULL) {
		if (res->uri != NULL) {
			things_free(res->uri);
			res->uri = NULL;
		}
		if (res->res_type != NULL) {
			things_free(res->res_type);
			res->res_type = NULL;
		}
		if (res->interface_type != NULL) {
			things_free(res->interface_type);
			res->interface_type = NULL;
		}
		if (res->cmd_id != NULL) {
			things_free(res->cmd_id);
			res->cmd_id = NULL;
		}
		if (res->query != NULL) {
			things_free(res->query);
			res->query = NULL;
		}
		if (res->rep != NULL) {
			things_release_representation_inst(res->rep);
		}
		if (res->dev_addr != NULL) {
			things_free(res->dev_addr);
		}
		things_free(res);
		res = NULL;
	}
}

void things_release_resource_inst(things_resource_s *res)
{
	if (res != NULL) {
		if (NULL != res->next) {
			things_resource_s *p_temp = res->next;
			things_resource_s *pDel = NULL;
			while (NULL != p_temp) {
				pDel = p_temp;
				p_temp = p_temp->next;

				release_resource_inst_impl(pDel);
				pDel = NULL;
			}
		}

		release_resource_inst_impl(res);
		res = NULL;
	}
}
