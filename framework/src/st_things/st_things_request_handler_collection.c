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

#include "st_things_request_handler_collection.h"
#include "st_things_request_handler.h"
#include "st_things_logger.h"
#include "st_things_util.h"

#include "things_api.h"
#include "ocpayload.h"

static bool get_resource_types2(things_resource_info_s *rsrc, char ***res_types, int *count)
{
	RET_FALSE_IF_PARAM_IS_NULL(rsrc);
	RET_FALSE_IF_PARAM_IS_NULL(res_types);
	RET_FALSE_IF_PARAM_IS_NULL(count);

	int rt_count = rsrc->rt_cnt;
	ST_LOG_V(ST_DEBUG, "Resource(%s) has %d resource type(s).", rsrc->uri, rt_count);
	RET_FALSE_IF_EXPR_IS_TRUE(rt_count < 1, "No resource types in resource.");

	char **types = (char **)util_calloc(rt_count, sizeof(char *));
	RET_VAL_IF_NULL(types, "Failed to allocate memory for resource types.", false);

	bool result = true;
	const char *res_type = NULL;
	for (int i = 0; i < rt_count; i++) {
		res_type = rsrc->resource_types[i];
		if (NULL == res_type || strlen(res_type) < 1) {
			ST_LOG_V(ST_ERROR, "Resource type at index(%d) is invalid.", i);
			util_free_str_array(types, i);
			result = false;
			break;
		}

		types[i] = util_clone_string(res_type);
		if (NULL == types[i]) {
			ST_LOG_V(ST_ERROR, "Failed to clone resource type(%s).", res_type);
			util_free_str_array(types, i);
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

static bool get_interface_types2(things_resource_info_s *rsrc, char ***if_types, int *count)
{
	RET_FALSE_IF_PARAM_IS_NULL(rsrc);
	RET_FALSE_IF_PARAM_IS_NULL(if_types);
	RET_FALSE_IF_PARAM_IS_NULL(count);

	int if_count = rsrc->if_cnt;
	ST_LOG_V(ST_DEBUG, "Resource(%s) has %d interface type(s).", rsrc->uri, if_count);
	RET_FALSE_IF_EXPR_IS_TRUE(if_count < 1, "No interface types in resource.");

	char **types = (char **)util_calloc(if_count, sizeof(char *));
	RET_VAL_IF_NULL(types, "Failed to allocate memory for inteface types.", false);

	bool result = true;
	const char *if_type = NULL;
	for (int i = 0; i < if_count; i++) {
		if_type = rsrc->interface_types[i];
		if (NULL == if_type || strlen(if_type) < 1) {
			ST_LOG_V(ST_ERROR, "Interface type at index(%d) is invalid.", i);
			util_free_str_array(types, i);
			result = false;
			break;
		}

		types[i] = util_clone_string(if_type);
		if (NULL == types[i]) {
			ST_LOG_V(ST_ERROR, "Failed to clone inteface type(%s).", if_type);
			util_free_str_array(types, i);
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
 * Sample format of "links" representation:-
 *   [
 *       {
 *           "href": , // Resource URI
 *           "rt": [],
 *           "if": []
 *       },
 *       {
 *           "href": , // Resource URI
 *           "rt": [],
 *           "if": []
 *       }
 *   ]
 */
bool form_collection_links(things_resource_s *collection_rsrc, OCRepPayload *** links, size_t *count)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);
	RET_FALSE_IF_PARAM_IS_NULL(links);
	RET_FALSE_IF_PARAM_IS_NULL(count);

	int num_of_children = 0;
	things_resource_info_s **children = NULL;
	if (!things_get_child_resources(collection_rsrc->uri, &num_of_children, &children)) {
		ST_LOG_V(ST_ERROR, "Failed to get child resources of collection resource(%s).", collection_rsrc->uri);
		return false;
	}

	if (0 == num_of_children || NULL == children) {
		ST_LOG_V(ST_ERROR, "Collection resource(%s) has no children.", collection_rsrc->uri);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Collection resource(%s) has %d children.", collection_rsrc->uri, num_of_children);

	// Allocate memory for the "links" in the response payload.
	OCRepPayload **link_arr = (OCRepPayload **) util_calloc(num_of_children, sizeof(OCRepPayload *));
	if (NULL == link_arr) {
		ST_LOG(ST_ERROR, "Failed to allocate memory for links in collection.");
		return false;
	}

	bool result = true;
	int rt_count = 0;
	int if_count = 0;
	char **res_types = NULL;
	char **if_types = NULL;
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0, 0, 0 };
	for (int index = 0; index < num_of_children; index++) {
		if (NULL == children[index]->uri || strlen(children[index]->uri)) {
			ST_LOG_V(ST_ERROR, "Resource URI of child resource at index(%d) is invalid.", index);
			result = false;
			break;
		}

		link_arr[index] = OCRepPayloadCreate();
		if (NULL == link_arr[index]) {
			ST_LOG(ST_ERROR, "Failed to allocate memory for links in collection.");
			result = false;
			break;
		}
		// Set the resource URI.
		if (!OCRepPayloadSetPropString(link_arr[index], OC_RSRVD_HREF, children[index]->uri)) {
			ST_LOG_V(ST_ERROR, "Failed to set the URI of child(%s) for links in collection(%s).", children[index]->uri, collection_rsrc->uri);
			result = false;
			break;
		}

		ST_LOG_V(ST_DEBUG, "Child resource uri(%s) is set in the response payload.", children[index]->uri);

		// Set the resource types.
		if (!get_resource_types2(children[index], &res_types, &rt_count)) {
			ST_LOG_V(ST_ERROR, "Failed to get the resource types of a child(%s) for links in collection.", children[index]->uri);
			result = false;
			break;
		}
		dimensions[0] = rt_count;
		result = OCRepPayloadSetStringArrayAsOwner(link_arr[index], OC_RSRVD_RESOURCE_TYPE, res_types, dimensions);
		if (!result) {
			ST_LOG_V(ST_ERROR, "Failed to set the resource types of child(%s) for links in collection.", children[index]->uri);
			result = false;
			break;
		}
		res_types = NULL;

		ST_LOG_V(ST_DEBUG, "Resource types of child resource(%s) are set in the response payload.", children[index]->uri);

		// Set the interface types.
		if (!get_interface_types2(children[index], &if_types, &if_count)) {
			ST_LOG_V(ST_ERROR, "Failed to get the interface types of child(%s) for links in collection.", children[index]->uri);
			result = false;
			break;
		}
		dimensions[0] = if_count;
		result = OCRepPayloadSetStringArrayAsOwner(link_arr[index], OC_RSRVD_INTERFACE, if_types, dimensions);
		if (!result) {
			ST_LOG_V(ST_ERROR, "Failed to set the interface types of child(%s) for links in collection.", children[index]->uri);
			result = false;
			break;
		}
		if_types = NULL;

		ST_LOG_V(ST_DEBUG, "Interface types of child resource(%s) are set in the response payload.", children[index]->uri);
	}

	if (result) {
		*count = num_of_children;
		*links = link_arr;
	} else {
		// Release memory allocated for resource types.
		if (res_types != NULL) {
			util_free_str_array(res_types, rt_count);
		}
		// Release memory allocated for interface types.
		if (if_types != NULL) {
			util_free_str_array(if_types, if_count);
		}

		for (int i = 0; i < num_of_children && NULL != link_arr[i]; i++) {
			OCRepPayloadDestroy(link_arr[i]);
		}
		util_free(link_arr);

		*count = 0;
		*links = NULL;
	}

	return result;
}

/*
 * Response for links list can directly be sent without dependency on application
 * because the response will include only the common properties of all the child resources.
 *
 * Sample response representation:-
 *
 *   {
 *       // Child resources' common properties
 *       "links":
 *       [
 *           {
 *               "href": , // Resource URI
 *               "rt": [],
 *               "if": []
 *           },
 *           {
 *               "href": , // Resource URI
 *               "rt": [],
 *               "if": []
 *           }
 *       ]
 *   }
 */
static bool handle_get_req_on_collection_linkslist(things_resource_s *collection_rsrc)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);

	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_FALSE_IF_NULL(resp_rep, "Failed to create response representation.");

	OCRepPayload *payload = resp_rep->payload;
	if (NULL == payload) {
		ST_LOG(ST_ERROR, "Payload in response representation is NULL.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Get the "links".
	size_t count = 0;
	OCRepPayload **links = NULL;
	bool result = false;
	if (form_collection_links(collection_rsrc, &links, &count)) {
		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { count, 0, 0 };
		result = OCRepPayloadSetPropObjectArrayAsOwner(payload, OC_RSRVD_LINKS, links, dimensions);
		if (!result) {
			ST_LOG(ST_ERROR, "Failed to add the links in the response payload.");
			for (size_t i = 0; i < count && NULL != links[i]; i++) {
				OCRepPayloadDestroy(links[i]);
			}
			util_free(links);
		}
	} else {
		ST_LOG(ST_ERROR, "Failed to form links for collection.");
	}

	if (result) {
		collection_rsrc->things_set_representation(collection_rsrc, resp_rep);
	} else {
		things_release_representation_inst(resp_rep);
	}

	return result;
}

/*
 * Sample response representation:-
 *
 *   {
 *       "href": ,
 *
 *       // Collection resource's common properties
 *       "rt":[],
 *       "if":[],
 *       "links":
 *       [
 *           {
 *               "href": , // Resource URI
 *               "rt": [],
 *               "if": []
 *           },
 *           {
 *               "href": , // Resource URI
 *               "rt": [],
 *               "if": []
 *           }
 *       ]
 *
 *       // Collection resource's properties
 *       "ps":,
 *       "lec":,
 *   }
 */
static bool handle_get_req_on_collection_baseline(things_resource_s *collection_rsrc)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);

	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_FALSE_IF_NULL(resp_rep, "Failed to create response representation.");

	OCRepPayload *resp_payload = resp_rep->payload;
	if (NULL == resp_payload) {
		ST_LOG(ST_ERROR, "Payload in response representation is NULL.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Set collection resource's URI.
	if (!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri)) {
		ST_LOG_V(ST_ERROR, "Failed to set the resource uri(%s) in response payload.", collection_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Resource URI(%s) is set in the response payload.", collection_rsrc->uri);

	// Set collection resource's common properties (rt, if & links).
	bool result = add_common_props(collection_rsrc, true, resp_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to add the common properties in response representation.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG(ST_DEBUG, "Common properties are added in the response payload.");

	// Get the properties of the collection resource from application.
	result = handle_get_req_helper(collection_rsrc->uri, collection_rsrc->query, resp_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to get the resource properties from application.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	collection_rsrc->things_set_representation(collection_rsrc, resp_rep);

	return true;
}

/*
 * Linked list of payload(OCRepPayload) of each and every resources.
 * First object in the payload is collection resource.
 *
 * Sample response representation:-
 *
 *   {
 *       "href": ,
 *       "rep":
 *       {
 *           // Collection resource's common properties
 *           "rt":[],
 *           "if":[],
 *           "links":[],
 *
 *           // Collection resource's properties
 *           "ps":,
 *           "lec":,
 *       }
 *   },
 *   {
 *       "href": ,
 *       "rep":
 *       {
 *           // Child resource's common properties
 *           "rt":[],
 *           "if":[],
 *
 *           // Child resource's properties
 *           "prop1":,
 *           "prop2":,
 *       }
 *   }
 */
static bool handle_get_req_on_collection_batch(things_resource_s *collection_rsrc)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);

	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_FALSE_IF_NULL(resp_rep, "Failed to create response representation.");

	OCRepPayload *payload_head = resp_rep->payload;
	if (NULL == payload_head) {
		ST_LOG(ST_ERROR, "Payload in response representation is NULL.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Handle collection resource.
	// Set collection resource's URI.
	if (!OCRepPayloadSetUri(payload_head, collection_rsrc->uri)) {
		ST_LOG_V(ST_ERROR, "Failed to set the resource uri(%s) in response payload.", collection_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Resource URI(%s) is set in the response payload.", collection_rsrc->uri);

	OCRepPayload *rep_payload = OCRepPayloadCreate();
	if (NULL == rep_payload) {
		ST_LOG(ST_ERROR, "Failed to create response payload for 'rep'.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Set collection resource's common properties (rt, if & links).
	bool result = add_common_props(collection_rsrc, true, rep_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to add collection's common properties in response payload.");
		OCRepPayloadDestroy(rep_payload);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG(ST_DEBUG, "Collection's common properties are added in the response payload.");

	// Get the properties of the collection resource from application.
	result = handle_get_req_helper(collection_rsrc->uri, collection_rsrc->query, rep_payload);
	if (!result) {
		ST_LOG_V(ST_DEBUG, "Failed to get the collection resource(%s) properties from application.", collection_rsrc->uri);
		ST_LOG(ST_DEBUG, "Corresponding payload in response will be empty.");
	}

	result = OCRepPayloadSetPropObjectAsOwner(payload_head, OC_RSRVD_REPRESENTATION, rep_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to set 'rep' for collection in response payload.");
		OCRepPayloadDestroy(rep_payload);
		things_release_representation_inst(resp_rep);
		return false;
	}

	rep_payload = NULL;

	// Handle child resources.
	int num_of_children = 0;
	things_resource_info_s **children = NULL;
	if (!things_get_child_resources(collection_rsrc->uri, &num_of_children, &children)) {
		ST_LOG(ST_ERROR, "Failed to get child resources.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	if (0 == num_of_children || NULL == children) {
		ST_LOG(ST_ERROR, "No child resource(s).");
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Collection resource(%s) has %d child resource(s).", collection_rsrc->uri, num_of_children);

	result = true;
	int rt_count = 0;
	int if_count = 0;
	char **res_types = NULL;
	char **if_types = NULL;
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0, 0, 0 };
	OCRepPayload *payload = payload_head;
	OCRepPayload *child_payload = NULL;
	for (int index = 0; index < num_of_children; index++) {
		if (NULL == children[index]) {
			ST_LOG_V(ST_ERROR, "Child at index(%d) is NULL.", index);
			result = false;
			break;
		}

		child_payload = OCRepPayloadCreate();
		if (NULL == child_payload) {
			ST_LOG(ST_ERROR, "Failed to create payload for child.");
			result = false;
			break;
		}
		// Set child resource's URI.
		if (!OCRepPayloadSetUri(child_payload, children[index]->uri)) {
			ST_LOG_V(ST_ERROR, "Failed to set the child resource uri(%s) in response representation.", children[index]->uri);
			result = false;
			break;
		}
		// Set child resource's representation (common properties and resource properties).
		rep_payload = OCRepPayloadCreate();
		if (NULL == rep_payload) {
			ST_LOG(ST_ERROR, "Failed to create payload for child.");
			result = false;
			break;
		}
		// Set the resource types.
		if (!get_resource_types2(children[index], &res_types, &rt_count)) {
			ST_LOG(ST_ERROR, "Failed to get the resource types of child.");
			result = false;
			break;
		}
		dimensions[0] = rt_count;
		if (!OCRepPayloadSetStringArrayAsOwner(rep_payload, OC_RSRVD_RESOURCE_TYPE, res_types, dimensions)) {
			ST_LOG(ST_ERROR, "Failed to set the resource types of child in response representation.");
			result = false;
			break;
		}
		res_types = NULL;

		// Set the interface types.
		if (!get_interface_types2(children[index], &if_types, &if_count)) {
			ST_LOG(ST_ERROR, "Failed to get the interface types of child.");
			result = false;
			break;
		}
		dimensions[0] = if_count;
		if (!OCRepPayloadSetStringArrayAsOwner(rep_payload, OC_RSRVD_INTERFACE, if_types, dimensions)) {
			ST_LOG(ST_ERROR, "Failed to set the inteface types of child in response representation.");
			result = false;
			break;
		}
		if_types = NULL;

		// First interface supported by the resource is the default inteface.
		// Form a query with the first interface.
		char **child_if_types = children[index]->interface_types;
		if (NULL == child_if_types) {
			ST_LOG(ST_ERROR, "Child resource doesn't have any interface.");
			result = false;
			break;
		}
		char *if_type = child_if_types[0];
		if (NULL == if_type) {
			ST_LOG(ST_ERROR, "First interface of child resource is NULL.");
			result = false;
			break;
		}

		char *prefix = "if=";
		char *query = (char *)util_malloc(strlen(if_type) + strlen(prefix) + 1);
		if (NULL == query) {
			ST_LOG(ST_ERROR, "Failed to create query for child.");
			result = false;
			break;
		}

		strncat(query, prefix, strlen(prefix));
		strncat(query, if_type, strlen(if_type));

		// Get the properties of the child resource from application.
		result = handle_get_req_helper(children[index]->uri, query, rep_payload);
		if (!result) {
			ST_LOG_V(ST_DEBUG, "Failed to get the child resource(%s) properties from application.", children[index]->uri);
			ST_LOG(ST_DEBUG, "Corresponding payload in response will be empty.");
		}

		if (!OCRepPayloadSetPropObjectAsOwner(child_payload, OC_RSRVD_REPRESENTATION, rep_payload)) {
			ST_LOG(ST_ERROR, "Failed to set the child representation in response representation.");
			util_free(query);
			result = false;
			break;
		}

		rep_payload = NULL;

		util_free(query);

		payload->next = child_payload;
		payload = child_payload;
	}

	if (!result) {
		if (NULL != res_types) {
			util_free_str_array(res_types, rt_count);
		}

		if (NULL != if_types) {
			util_free_str_array(if_types, if_count);
		}

		OCRepPayloadDestroy(child_payload);
		OCRepPayloadDestroy(rep_payload);
		things_release_representation_inst(resp_rep);
		return false;
	}

	collection_rsrc->things_set_representation(collection_rsrc, resp_rep);

	return true;
}

/*
 * Handles GET request for colletion resources on "read-only", "read-write", "actuator" and "sensor" interfaces.
 * Common properties will not be added.
 */
static bool handle_get_req_on_collection_common(things_resource_s *collection_rsrc)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);

	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_FALSE_IF_NULL(resp_rep, "Failed to create response representation.");

	OCRepPayload *resp_payload = resp_rep->payload;
	if (NULL == resp_payload) {
		ST_LOG(ST_ERROR, "Payload is response representation is NULL.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	if (!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri)) {
		ST_LOG_V(ST_ERROR, "Failed to set the resource uri(%s) in response payload.", collection_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Resource URI(%s) is set in the response payload.", collection_rsrc->uri);

	// Get the resource's properties from the application.
	bool result = handle_get_req_helper(collection_rsrc->uri, collection_rsrc->query, resp_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to get the resource properties from application.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Set the response representation in the resource.
	collection_rsrc->things_set_representation(collection_rsrc, resp_rep);
	return true;
}

int handle_get_req_on_collection_rsrc(things_resource_s *collection_rsrc)
{
	RET_VAL_IF_PARAM_IS_NULL(collection_rsrc, 0);

	// Get interface type from query parameter
	char *if_type = NULL;
	if (NULL != collection_rsrc->query && strlen(collection_rsrc->query) > 0) {
		bool found = false;
		bool result = get_query_value_internal(collection_rsrc->query, OC_RSRVD_INTERFACE, &if_type, &found);
		if (found && !result) {	// If query is present but API returns false.
			ST_LOG_V(ST_ERROR, "Failed to get the interface type from query parameter(%s).", collection_rsrc->query);
			return 0;
		}
	}
	// If interface is not available in the query parameter, then "oic.if.ll" will be taken as the default interface.
	// Reasons:-
	// 1. Default collection request handler in iotivity stack(occollection.c)
	// takes "links list" as the default interface to serve collection resource requests.
	// 2. Underlying stack adds "oic.if.ll" to all collection resources.
	if (NULL == if_type) {
		if_type = util_clone_string(OC_RSRVD_INTERFACE_LL);
		RET_VAL_IF_NULL(if_type, "Failed to clone interface type.", 0);
	}

	ST_LOG_V(ST_DEBUG, "This GET request will be handled on %s inteface.", if_type);

	bool result = false;
	if (0 == strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT))) {
		result = handle_get_req_on_collection_baseline(collection_rsrc);
	} else if (0 == strncmp(if_type, OC_RSRVD_INTERFACE_LL, strlen(OC_RSRVD_INTERFACE_LL))) {
		result = handle_get_req_on_collection_linkslist(collection_rsrc);
	} else if (0 == strncmp(if_type, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {
		result = handle_get_req_on_collection_batch(collection_rsrc);
	} else {
		result = handle_get_req_on_collection_common(collection_rsrc);
	}

	util_free(if_type);

	return result ? 1 : 0;
}

/*
 * POST request and response schema is same as the GET response schema.
 */
static bool handle_post_req_on_collection_baseline(things_resource_s *collection_rsrc)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);

	// Retrieve the request representation. This representation will hold all the input properties of post request.
	// Payload in this representation will be used to form the request message which will be given to the application.
	struct things_representation_s *req_rep = NULL;
	bool rep_exist = collection_rsrc->things_get_representation(collection_rsrc, &req_rep);
	if (!rep_exist || NULL == req_rep || NULL == req_rep->payload) {
		ST_LOG(ST_ERROR, "Empty payload in POST request.");
		return false;			// TODO: When a post request comes with empty payload, how do we handle?
	}
	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_FALSE_IF_NULL(resp_rep, "Failed to create response representation.");

	OCRepPayload *resp_payload = resp_rep->payload;
	if (NULL == resp_payload) {
		ST_LOG(ST_ERROR, "Payload in response representation is NULL.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Set collection resource's URI.
	if (!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri)) {
		ST_LOG_V(ST_ERROR, "Failed to set the resource uri(%s) in response payload.", collection_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Resource URI(%s) is set in the response payload.", collection_rsrc->uri);

	// Set collection resource's common properties (rt, if, & links).
	bool result = add_common_props(collection_rsrc, true, resp_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to add the common properties in response payload.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG(ST_DEBUG, "Common properties are added in the response payload.");

	// Give the properties of the collection resource to application and get the response.
	result = handle_post_req_helper(collection_rsrc->uri, collection_rsrc->query, req_rep->payload, resp_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to set the resource properties.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	collection_rsrc->things_set_representation(collection_rsrc, resp_rep);

	return true;
}

/*
 * POST request and response schema is same as the GET response schema.
 */
static bool handle_post_req_on_collection_batch(things_resource_s *collection_rsrc)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);

	// Retrieve the request representation. This representation will hold all the input properties of post request.
	// Payload in this representation will be used to form the request message which will be given to the application.
	struct things_representation_s *req_rep = NULL;
	bool rep_exist = collection_rsrc->things_get_representation(collection_rsrc, &req_rep);
	if (!rep_exist || NULL == req_rep || NULL == req_rep->payload) {
		ST_LOG(ST_ERROR, "Empty payload in POST request.");
		return false;			// TODO: When a post request comes with empty payload, how do we handle?
	}
	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_FALSE_IF_NULL(resp_rep, "Failed to create response representation.");

	OCRepPayload *payload_head = resp_rep->payload;
	if (NULL == payload_head) {
		ST_LOG(ST_ERROR, "Payload in response representation is NULL.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Handle collection resource.
	// Set collection resource's URI.
	if (!OCRepPayloadSetUri(payload_head, collection_rsrc->uri)) {
		ST_LOG_V(ST_ERROR, "Failed to set the resource uri(%s) in response payload.", collection_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Resource URI(%s) is set in the response payload.", collection_rsrc->uri);

	OCRepPayload *rep_payload = OCRepPayloadCreate();
	if (NULL == rep_payload) {
		ST_LOG(ST_ERROR, "Failed to create payload for response representation.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Set collection resource's common properties (rt, if, & links).
	bool result = add_common_props(collection_rsrc, true, rep_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to add collection's common properties in response payload.");
		OCRepPayloadDestroy(rep_payload);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG(ST_DEBUG, "Collection's common properties are added in the response payload.");

	// Get the properties of the collection resource from application.
	result = handle_post_req_helper(collection_rsrc->uri, collection_rsrc->query, req_rep->payload, rep_payload);
	if (!result) {
		ST_LOG_V(ST_DEBUG, "Failed to get the collection resource(%s) properties from application.", collection_rsrc->uri);
		ST_LOG(ST_DEBUG, "Corresponding payload in response will be empty.");
	}

	result = OCRepPayloadSetPropObjectAsOwner(payload_head, OC_RSRVD_REPRESENTATION, rep_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to set the representation for collection in response payload.");
		OCRepPayloadDestroy(rep_payload);
		things_release_representation_inst(resp_rep);
		return false;
	}

	rep_payload = NULL;

	// Handle child resources.
	result = true;
	int num_of_children = 0;
	things_resource_info_s **children = NULL;
	if (!things_get_child_resources(collection_rsrc->uri, &num_of_children, &children)) {
		ST_LOG(ST_ERROR, "Failed to get child resources.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	if (0 == num_of_children || NULL == children) {
		ST_LOG(ST_ERROR, "No child resource(s).");
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Collection resource(%s) has %d child resource(s).", collection_rsrc->uri, num_of_children);

	int rt_count = 0;
	int if_count = 0;
	char **res_types = NULL;
	char **if_types = NULL;
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0, 0, 0 };
	OCRepPayload *child_head = NULL;
	OCRepPayload *prev_payload = NULL;	// Represents the previous payload in the linked list.
	OCRepPayload *payload = NULL;	// Represents the current payload in loop.
	for (int index = 0; index < num_of_children; index++) {
		if (NULL == children[index]) {
			ST_LOG_V(ST_ERROR, "Child at index(%d) is NULL.", index);
			result = false;
			break;
		}

		payload = OCRepPayloadCreate();
		if (NULL == payload) {
			ST_LOG(ST_ERROR, "Failed to create payload for child.");
			result = false;
			break;
		}
		// Set child resource's URI.
		if (!OCRepPayloadSetUri(payload, children[index]->uri)) {
			ST_LOG_V(ST_ERROR, "Failed to set the child resource uri(%s) in response representation.", children[index]->uri);
			result = false;
			break;
		}
		// Set child resource's representation (common properties and resource properties).
		rep_payload = OCRepPayloadCreate();
		if (NULL == rep_payload) {
			ST_LOG(ST_ERROR, "Failed to create payload for child.");
			result = false;
			break;
		}
		// Set the resource types.
		if (!get_resource_types2(children[index], &res_types, &rt_count)) {
			ST_LOG(ST_ERROR, "Failed to get the resource types of child.");
			result = false;
			break;
		}
		dimensions[0] = rt_count;
		if (!OCRepPayloadSetStringArrayAsOwner(rep_payload, OC_RSRVD_RESOURCE_TYPE, res_types, dimensions)) {
			ST_LOG(ST_ERROR, "Failed to set the resource types of child in response representation.");
			result = false;
			break;
		}
		res_types = NULL;

		// Set the interface types.
		if (!get_interface_types2(children[index], &if_types, &if_count)) {
			ST_LOG(ST_ERROR, "Failed to get the interface types of child.");
			result = false;
			break;
		}
		dimensions[0] = if_count;
		if (!OCRepPayloadSetStringArrayAsOwner(rep_payload, OC_RSRVD_INTERFACE, if_types, dimensions)) {
			ST_LOG(ST_ERROR, "Failed to set the inteface types of child in response representation.");
			result = false;
			break;
		}
		if_types = NULL;

		// First interface supported by the resource is the default inteface.
		// Form a query with the first interface.
		char **child_if_types = children[index]->interface_types;
		if (NULL == child_if_types) {
			ST_LOG(ST_ERROR, "Child resource doesn't have any interface.");
			result = false;
			break;
		}
		char *if_type = child_if_types[0];
		if (NULL == if_type) {
			ST_LOG(ST_ERROR, "First interface of child resource is NULL.");
			result = false;
			break;
		}

		char *prefix = "if=";
		char *query = (char *)util_malloc(strlen(if_type) + strlen(prefix) + 1);
		if (NULL == query) {
			ST_LOG(ST_ERROR, "Failed to create query for child.");
			result = false;
			break;
		}

		strncat(query, prefix, strlen(prefix));
		strncat(query, if_type, strlen(if_type));

		// Get the properties of the child resource from application.
		result = handle_post_req_helper(children[index]->uri, query, req_rep->payload, rep_payload);
		if (!result) {
			ST_LOG_V(ST_DEBUG, "Failed to get the child resource(%s) properties from application.", children[index]->uri);
			ST_LOG(ST_DEBUG, "Corresponding payload in response will be empty.");
		}

		if (!OCRepPayloadSetPropObjectAsOwner(payload, OC_RSRVD_REPRESENTATION, rep_payload)) {
			ST_LOG(ST_ERROR, "Failed to set the child representation in response representation.");
			result = false;
			break;
		}

		rep_payload = NULL;

		if (NULL != prev_payload) {
			prev_payload->next = payload;
		} else {
			child_head = payload;
		}
		prev_payload = payload;
		payload = NULL;

		util_free(query);
	}

	if (!result) {
		if (NULL != res_types) {
			util_free_str_array(res_types, rt_count);
		}

		if (NULL != if_types) {
			util_free_str_array(if_types, if_count);
		}

		OCRepPayloadDestroy(rep_payload);
		OCRepPayloadDestroy(payload);
		OCRepPayloadDestroy(child_head);
		things_release_representation_inst(resp_rep);
		return false;
	}

	payload_head->next = child_head;
	collection_rsrc->things_set_representation(collection_rsrc, resp_rep);
	return true;
}

/*
 * Handles POST request for colletion resources on "read-write" and "actuator" interfaces.
 * Request and response neither include the common properties of collection resource
 * nor the child resources' properties which means both request and response includes only
 * the collection resource's properties.
 */
static bool handle_post_req_on_collection_common(things_resource_s *collection_rsrc)
{
	RET_FALSE_IF_PARAM_IS_NULL(collection_rsrc);

	// Retrieve the request representation. This representation will hold all the input properties of post request.
	// Payload in this representation will be used to form the request message which will be given to the application.
	struct things_representation_s *req_rep = NULL;
	bool rep_exist = collection_rsrc->things_get_representation(collection_rsrc, &req_rep);
	if (!rep_exist || NULL == req_rep || NULL == req_rep->payload) {
		ST_LOG(ST_ERROR, "Empty payload in POST request.");
		return false;			// TODO: When a post request comes with empty payload, how do we handle?
	}
	// Setup the response representation. This representation will be handed over to the underlying stack.
	things_representation_s *resp_rep = things_create_representation_inst(NULL);
	RET_FALSE_IF_NULL(resp_rep, "Failed to create response representation.");

	OCRepPayload *resp_payload = resp_rep->payload;
	if (NULL == resp_payload) {
		ST_LOG(ST_ERROR, "Payload is response representation is NULL.");
		things_release_representation_inst(resp_rep);
		return false;
	}

	if (!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri)) {
		ST_LOG_V(ST_ERROR, "Failed to set the resource uri(%s) in response payload.", collection_rsrc->uri);
		things_release_representation_inst(resp_rep);
		return false;
	}

	ST_LOG_V(ST_DEBUG, "Resource URI(%s) is set in the response payload.", collection_rsrc->uri);

	// Give the properties of the collection resource to application and get the response.
	bool result = handle_post_req_helper(collection_rsrc->uri, collection_rsrc->query,
										 req_rep->payload, resp_payload);
	if (!result) {
		ST_LOG(ST_ERROR, "Failed to set the resource properties.");
		things_release_representation_inst(resp_rep);
		return false;
	}
	// Set the response representation in the resource.
	collection_rsrc->things_set_representation(collection_rsrc, resp_rep);

	return true;
}

int handle_post_req_on_collection_rsrc(things_resource_s *collection_rsrc)
{
	RET_VAL_IF_PARAM_IS_NULL(collection_rsrc, 0);

	// Get interface type from query parameter.
	char *if_type = NULL;
	if (NULL != collection_rsrc->query && strlen(collection_rsrc->query) > 0) {
		bool found = false;
		bool result = get_query_value_internal(collection_rsrc->query, OC_RSRVD_INTERFACE, &if_type, &found);
		if (found && !result) {	// If query is present but API returns false.
			ST_LOG_V(ST_ERROR, "Failed to get the interface type from query parameter(%s).", collection_rsrc->query);
			return 0;
		}
	}
	// If interface is not available in the query parameter, then "oic.if.baseline" will be taken as the default interface.
	if (NULL == if_type) {
		if_type = util_clone_string(OC_RSRVD_INTERFACE_DEFAULT);
		RET_VAL_IF_NULL(if_type, "Failed to clone interface type.", 0);
	}

	ST_LOG_V(ST_DEBUG, "This SET request will be handled on %s inteface.", if_type);

	bool result = false;
	if (0 == strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT))) {
		result = handle_post_req_on_collection_baseline(collection_rsrc);
	} else if (0 == strncmp(if_type, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {
		result = handle_post_req_on_collection_batch(collection_rsrc);
	} else if (0 == strncmp(if_type, ST_THINGS_RSRVD_INTERFACE_READWRITE, strlen(ST_THINGS_RSRVD_INTERFACE_READWRITE))) {
		result = handle_post_req_on_collection_common(collection_rsrc);
	} else if (0 == strncmp(if_type, ST_THINGS_RSRVD_INTERFACE_ACTUATOR, strlen(ST_THINGS_RSRVD_INTERFACE_ACTUATOR))) {
		result = handle_post_req_on_collection_common(collection_rsrc);
	} else {
		result = false;
	}

	util_free(if_type);

	return result ? 1 : 0;
}
