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

#ifndef _THINGS_RESOURCE_H_
#define _THINGS_RESOURCE_H_

#ifdef __cplusplus
extern "C" {
#endif							// __cplusplus

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <octypes.h>

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define QUERY_DELIMITER ";"		// or "#"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
/* Structure for Array Attribute */
typedef struct things_representation_s {
	void *payload;			// 8 bytes
	void **children_payload;	// 8 bytes
	int64_t num_children;	// 8 bytes
	struct things_representation_s **children;	// 8 * num_children bytes

	/**
	 * @brief API for setting value with a key
	 * @param key : Attribute Key which will represent the value
	 * @param value : Attribute value
	 */
	void (*things_set_value)(struct things_representation_s *rep, char *key, char *);
	void (*things_set_bool_value)(struct things_representation_s *rep, char *key, bool);
	void (*things_set_int_value)(struct things_representation_s *rep, char *key, int64_t);
	void (*things_set_double_value)(struct things_representation_s *rep, char *key, double);
	void (*things_set_byte_value)(struct things_representation_s *rep, char *key, uint8_t *value, size_t size);
	bool(*things_set_object_value)(struct things_representation_s *mother, char *key, struct things_representation_s *child);
	/**
	 * @brief API for setting/getting object array type of attribute.
	 * @param key : Attribute Key which will represent the array type of value
	 * @param length : Length of value(representation) array
	 * @param reps : Array type (representation) values
	 */
	bool(*things_set_arrayvalue)(struct things_representation_s *, char *key, int length, struct things_representation_s **reps);

	bool(*things_get_arrayvalue)(struct things_representation_s *, char *key, int *length, struct things_representation_s ***reps);

	/**
	 * @brief API for setting/getting string array type of attribute.
	 * @param key : Attribute Key which will represent the array type of value
	 * @param length : Length of value(string) array
	 * @param array : Array type string values
	 */
	void (*things_set_string_arrayvalue)(struct things_representation_s *mother, char *key, int length, char **array);

	bool(*things_get_string_arrayvalue)(struct things_representation_s *mother, char *key, int *length, char ***array);

	/**
	 * @brief API for setting/getting double array type of attribute.
	 * @param key : Attribute Key which will represent the array type of value
	 * @param length : Length of value(double) array
	 * @param array : Array type double values
	 */
	void (*things_set_double_arrayvalue)(struct things_representation_s *mother, char *key, int length, double *array);

	bool(*things_get_double_arrayvalue)(struct things_representation_s *mother, char *key, int *length, double **array);

	/**
	* @brief API for inserting int array type of attribute.
	* @param key : Attribute Key which will represent the array type of value
	* @param length : Length of value(int) array
	* @param array : Array type int values
	*/
	void (*things_set_int_arrayvalue)(struct things_representation_s *mother, char *key, int length, int64_t *array);

	bool(*things_get_int_arrayvalue)(struct things_representation_s *mother, char *key, int *length, int64_t **array);
	/**
	 * @brief API for getting value with a key
	 * @param key   : Attribute Key which represents the value
	 * @param value : Reference of the variable to where value will be copied
	 * @return      : true(value exist), false(value not exist)
	 */
	bool(*things_get_value)(struct things_representation_s *rep, char *key, char **);
	bool(*things_get_bool_value)(struct things_representation_s *rep, char *key, bool *);
	bool(*things_get_int_value)(struct things_representation_s *rep, char *key, int64_t *);
	bool(*things_get_double_value)(struct things_representation_s *rep, char *key, double *);
	bool(*things_get_byte_value)(struct things_representation_s *rep, char *key, uint8_t **value, size_t *size);
	bool(*things_get_object_value)(struct things_representation_s *mother, char *key, struct things_representation_s *child);

} things_representation_s;

/* Structure for Things resource */
typedef struct things_resource_s {
	char *query;			// 4 bytes
	char *uri;				// 4 Bytes
	char *res_type;			// 4 Bytes
	char *interface_type;	// 4 Bytes

	OCRequestHandle request_handle;
	OCResourceHandle resource_handle;	// 4 Bytes, OCResourceHandle handle;
	int req_type;			// 4 Bytes
	double error;			// 8 Bytes

	struct things_resource_s *next;	// 4 Bytes
	struct things_representation_s *rep;	// 4 Bytes

	size_t size;			// 4 bytes
	char *cmd_id;			// 4 Bytes

	void *dev_addr;			//  sizeof(OCDevAddr) ????

	void (*things_set_dev_addr)(struct things_resource_s *, void *OCDevAddr);
	void *(*things_get_dev_addr)(struct things_resource_s *);

	/**
	* @brief API for setting URI.
	* @param things_resource_s * : Instance of target resource
	* @param const char *  : URI string value
	*/
	void (*things_set_uri)(struct things_resource_s *, const char *uri);
	/**
	* @brief API for setting error when returning request handling result
	* @param things_resource_s * : Instance of target resource
	* @param double        : error code
	*/
	void (*things_set_error)(struct things_resource_s *, double errorCode);

	/**
	* @brief API for getting query value
	* @param things_resource_s * : Instance of target resource
	* @param char* key     : key value to look up in the query(ex: query -> key=value?key=value?...)
	* @param char* value   : value of the key in the query
	* @return              : true(query exist), false(query not exist)
	*/
	bool(*things_get_query)(struct things_resource_s *, char *key, char **value);

	/**
	* @brief API for getting URI
	* @param things_resource_s * : Instance of target resource
	* @param char** uri    : reference of the char* variable to where uri will be copied
	*/
	void (*things_get_uri)(struct things_resource_s *, char **uri);

	/**
	* @brief API for getting # of supporting resource types
	* @param things_resource_s * : Instance of target resource
	* @return              : Number of supporting resource types
	*/
	int (*things_get_num_of_res_types)(struct things_resource_s *);
	int (*things_get_num_of_inf_types)(struct things_resource_s *);
	/**
	* @brief API for getting # of supporting resource type with index
	* @param things_resource_s * : Instance of target resource
	* @param things_resource_s * : Index of supported resource type list
	* @return              : Supporting resource type
	*/
	const char *(*things_get_res_type)(struct things_resource_s *, int index);
	const char *(*things_get_inf_type)(struct things_resource_s *, int index);
	/**
	* @brief API for getting childrens of target resource
	* @param things_resource_s * : Instance of target resource
	* @return              : Instance of children resource
	*                        (Children resource are managed with Linked List)
	*/
	struct things_resource_s *(*things_get_children)(struct things_resource_s *);

	/**
	* @brief API for setting representation of the target resource
	* @param things_resource_s *       : Instance of target resource
	* @param things_representation_s * : Instance of the representation to set
	*/
	void (*things_set_representation)(struct things_resource_s *, struct things_representation_s *rep);

	/**
	* @brief API for getting representation of the target resource
	* @param things_resource_s *       : Instance of target resource
	* @param things_representation_s * : Instance of the representation to set
	* @return                    : true(Representation exist), false(Representation not exist)
	*/
	bool(*things_get_representation)(struct things_resource_s *, struct things_representation_s **rep);

	/**
	* @brief API for getting payload (Internal use only, NOT API)
	* @param things_resource_s *       : Instance of target resource
	* @return                    : Instance OCRepPayload data structure defined by IoTivity stack
	*/
	void *(*things_get_rep_payload)(struct things_resource_s *);

	/**
	* @brief API for creating instance of payload (Internal use only, NOT API)
	* @param things_resource_s *       : Instance of target resource
	* @return                    : New instance OCRepPayload data structure defined by IoTivity stack
	*/
	void *(*things_create_payload)(struct things_resource_s *, char *);

	/**
	* @brief API for checking whether given interface type is supporting one or not
	* @param things_resource_s *   : Instance of target resource
	* @param iftype          : Interface type to check
	* @return                : true (supporting interface type),false(not supporting)
	*/
	bool(*things_is_supporting_interface_type)(struct things_resource_s *, char *ifType);

	/**
	* @brief API for checking whether given resource type is supporting one or not
	* @param things_resource_s *   : Instance of target resource
	* @param iftype          : Resource type to check
	* @return                : true (supporting resource type),false(not supporting)
	*/
	bool(*things_is_supporting_resource_type)(struct things_resource_s *, char *res_type);

	/**
	* @brief API for setting frameid
	* @brief Frameid will be used to match the response delivered from st_things SW with received request
	* @param things_resource_s *   : Instance of target resource
	* @param cmd_id           : Frame ID
	*/
	void (*things_set_command_id)(struct things_resource_s *res, char *cmd_id);

	/**
	* @brief API for getting children resource instances
	* @param things_resource_s *   : Instance of target resource
	* @return                : Instance of children resource(s)
	*/
	struct things_resource_s *(*things_get_next)(struct things_resource_s *);

	/**
	* @brief API for adding children resource instances
	* @param things_resource_s *   : Instance of target resource
	* @param things_resource_s *   : Instance of children resource
	*/
	void (*things_add_child)(struct things_resource_s *, struct things_resource_s *);

} things_resource_s;

/**
* @brief API for creating instance of payload
* @param rep_payload            : Instance of target resource
* @return things_representation_s *  : Instance of resource repsentation
*/
things_representation_s *things_create_representation_inst(void *rep_payload);

/**
* @brief API for creating instance of resource
* @param void*      : Instance of request handle
* @param void*      : Instance of resource handle
* @param void*      : Query in the request message
* @param void*      : Instance of payload in the request
* @return things_resource_s *  : Instance of resource repsentation
*/
things_resource_s *things_create_resource_inst(OCRequestHandle, OCResourceHandle, void *, void *);

/**
* @brief API for cloning instance of given resource
* @param things_resource_s *   : Instance of resource to clone
* @return things_resource_s *  : Instance of cloned resource
*/
things_resource_s *things_clone_resource_inst(things_resource_s *);
void things_clone_resource_inst2(things_resource_s *pori, things_resource_s **pclone);

/**
* @brief API for releasing memory allocated for the instance of given representation
* @param things_representation_s *   : Instance of representation to release
*/
void things_release_representation_inst(things_representation_s *);

/**
* @brief API for releasing memory allocated for the instance of given resource
* @param things_representation_s *   : Instance of resource to release
*/
void things_release_resource_inst(things_resource_s *);

#ifdef __cplusplus
}
#endif							// __cplusplus
#endif							// _THINGS_RESOURCE_H_
