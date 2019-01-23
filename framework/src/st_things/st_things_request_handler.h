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

#ifndef __ST_THINGS_REQUEST_HANDLER_H__
#define __ST_THINGS_REQUEST_HANDLER_H__

#include "st_things.h"
#include "things_resource.h"
#include "octypes.h"

// OIC core specification for read-write interface.
#define ST_THINGS_RSRVD_INTERFACE_READWRITE "oic.if.rw"

// OIC core specification for sensor interface.
#define ST_THINGS_RSRVD_INTERFACE_SENSOR "oic.if.s"

// OIC core specification for actuator interface.
#define ST_THINGS_RSRVD_INTERFACE_ACTUATOR "oic.if.a"

//--------------------------------------------------------------------------------
/**
 * @brief API for getting value from query parameter.
 * @param query	: Contain the 'resource type ' or 'interface type'.We get 'rt' or 'if'
 *		  from query parameter.
 * @param key	: Attribute Key which represents the array type of value.
 * @param value	: Reference of the varaible to wheree value will be copied.
 * @param found	: Check the existant of Key in query parameter
 *		  (true = exists, false = Not exists).
 * @return	: true(query value exist), false(query value not exist).
 */
bool get_query_value_internal(const char *query, const char *key, char **value, bool *found);

//--------------------------------------------------------------------------------
/**
 * @brief API for adds the common properties of resource such as 'rt', 'if'.
 * @param things_resource_s * : Instance of target resource.
 * @param collection   : Check the type of resource.
 *			: (true = collection resource, false = Not collection resource).
 * @param OCRepPayload *:Instance of target resource.
 * @return      : true(value is added), false(value not added).
 */
bool add_common_props(things_resource_s *rsrc, bool collection, OCRepPayload *resp_payload);

//--------------------------------------------------------------------------------
/**
 * @brief API for get the response from application for GET requests.
 * @param const char * : URI string value.
 * @param query	       : Contain the 'resource type ' or 'interface type'.We get 'rt' or 'if'
 *			from query parameter.
 * @param OCRepPayload *:Instance of target resource.
 * @return		: true(Response for Get request exist), false(Response for Get request 
 *			  not exist).
 */
bool handle_get_req_helper(const char *res_uri, const char *query, OCRepPayload *resp_payload);

//--------------------------------------------------------------------------------
/**
 * @brief API for get the response from application for POST requests.
 * @param const char * : URI string value.
 * @param query        : Contain the 'resource type ' or 'interface type'.We get 'rt' or 'if'
 *                       from query parameter.
 * @param OCRepPayload *:Instance of target resource.
 * @return		:true(Response for Post request exist), false(Response for Post request not exist).
 */
bool handle_post_req_helper(const char *res_uri, const char *query, OCRepPayload *req_payload, OCRepPayload *resp_payload);

//--------------------------------------------------------------------------------
/**
 * @brief API for handle get request callback.
 * @param things_resource_s *: Instance of target resource.
 * @return		     : 0 in success, otherwise a negative error value.
 */
int handle_get_request_cb(struct things_resource_s *resource);

//--------------------------------------------------------------------------------
/**
 * @brief API for handle set request callback.
 * @param things_resource_s *: Instance of target resource.
 * @return			: 0 in success, otherwise a negative error value.
 */
int handle_set_request_cb(struct things_resource_s *resource);

//--------------------------------------------------------------------------------
/**
 * @brief API for callback registration for handling request message.
 * @param get_cb	: Reference of the callback function to handle GET request.
 * @param_set_cb	: Reference of the callback function to handle SET request.
 * @return		: 0 in success, otherwise a negative error value.
 */
int register_request_handler_cb(st_things_get_request_cb get_cb, st_things_set_request_cb set_cb);

#endif							// __ST_THINGS_REQUEST_HANDLER_H__
