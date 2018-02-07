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
/**
 * @ingroup SmartThings
 * @{
 */

/**
 * @file st_things/st_things_types.h
 * @brief Provides structure definition for SmartThings Things SDK
 */
#ifndef __ST_THINGS_TYPES_H__
#define __ST_THINGS_TYPES_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Enumeration for ST Things error code.
 * @since TizenRT v1.1
 */
typedef enum {
	ST_THINGS_ERROR_NONE = 0,						   /**< Successful */
	ST_THINGS_ERROR_INVALID_PARAMETER = -1,			   /**< Invalid parameter (If parameter is null or empty)*/
	ST_THINGS_ERROR_OPERATION_FAILED = -2,			   /**< Operation Failed */
	ST_THINGS_ERROR_STACK_NOT_INITIALIZED = -3,		   /**< Stack is not yet initialized*/
	ST_THINGS_ERROR_STACK_ALREADY_INITIALIZED = -4,	   /**< Stack is already initialized*/
	ST_THINGS_ERROR_STACK_NOT_STARTED = -5,			   /**< Stack is not yet started*/
	ST_THINGS_ERROR_STACK_RUNNING = -6,				   /**< Stack is currently running*/
} st_things_error_e;

/**
 * @brief Enumeration for ST Things status.
 * @since TizenRT v1.1
 */
typedef enum {
	ST_THINGS_STATUS_INIT = 0,						   /**< Initial state of ST Things */
	ST_THINGS_STATUS_ES_STARTED,					   /**< Easy-setup is started */
	ST_THINGS_STATUS_ES_DONE,						   /**< Easy-setup is done */
	ST_THINGS_STATUS_ES_FAILED_ON_OWNERSHIP_TRANSFER,  /**< Easy-setup failed due to Ownership-Transfer failure */
	ST_THINGS_STATUS_CONNECTING_TO_AP,				   /**< Connecting to target Wi-Fi access point */
	ST_THINGS_STATUS_CONNECTED_TO_AP,				   /**< Connected to target Wi-Fi access point */
	ST_THINGS_STATUS_CONNECTING_TO_AP_FAILED,		   /**< Failed to connect to target Wi-Fi access point */
	ST_THINGS_STATUS_REGISTERING_TO_CLOUD,			   /**< Trying to Sign-up/Sign-in/Publish-Resource(s) to Cloud */
	ST_THINGS_STATUS_REGISTERED_TO_CLOUD,			   /**< Publish resource(s) to cloud is complete. Now the Thing is ready to be controlled via Cloud */
	ST_THINGS_STATUS_REGISTERING_FAILED_ON_SIGN_IN,	   /**< Failed to sign-in to Cloud */
	ST_THINGS_STATUS_REGISTERING_FAILED_ON_PUB_RES	   /**< Failed to publish resources to Cloud */
} st_things_status_e;

/**
 * @brief Structure for Representation.
 * @since TizenRT v1.1
 */
typedef struct _st_things_representation {
	void *payload;	 /**< Payload of representation */

	/**
	* @brief API for getting the value of string type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will return deep-copied string value as out parameter, so application must free it after use.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[out] value String value
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_str_value)(struct _st_things_representation *rep, const char *key, char **value);

	/**
	* @brief API for getting the value of boolean type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[out] value Bool value
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_bool_value)(struct _st_things_representation *rep, const char *key, bool *value);

	/**
	* @brief API for getting the value of integer type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[out] value Integer value
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_int_value)(struct _st_things_representation *rep, const char *key, int64_t *value);

	/**
	* @brief API for getting the value of double type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[out] value Double value
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_double_value)(struct _st_things_representation *rep, const char *key, double *value);

	/**
	* @brief API for getting the value of byte array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will return deep-copied byte value as out parameter, so application must free it after use.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[out] value Byte value
	* @param[out] size Size of Byte value
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_byte_value)(struct _st_things_representation *rep, const char *key, uint8_t **value, size_t *size);

	/**
	* @brief API for getting the value of object type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will return deep-copied object value as out parameter, so application must free it after use.\n
	*          To free an object, st_things_destroy_representation_inst() in st_things.h should be used.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[out] value Object value
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_object_value)(struct _st_things_representation *rep, const char *key, struct _st_things_representation **value);

	/**
	* @brief API for setting the value of string type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will deep-copy the string value inside, so application still has an ownership of memory for the string value.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the value.
	* @param[in]  value String value.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_str_value)(struct _st_things_representation *rep, const char *key, const char *value);

	/**
	* @brief API for setting the value of boolean type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the value.
	* @param[in]  value Bool value.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_bool_value)(struct _st_things_representation *rep, const char *key, bool value);

	/**
	* @brief API for setting the value of integer type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the value.
	* @param[in]  value Integer value.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_int_value)(struct _st_things_representation *rep, const char *key, int64_t value);

	/**
	* @brief API for setting the value of double type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the value.
	* @param[in]  value Double value.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_double_value)(struct _st_things_representation *rep, const char *key, double value);

	/**
	* @brief API for setting the value of byte array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will deep-copy the byte value inside, so application still has an ownership of memory for the byte value.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the value.
	* @param[in]  value Byte value.
	* @param[in]  size Size of Byte value.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_byte_value)(struct _st_things_representation *rep, const char *key, const uint8_t *value, size_t size);

	/**
	* @brief API for setting the value of object type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will deep-copy the object value inside, so application still has an ownership of memory for the object value.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the value.
	* @param[in]  value Object value.
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_object_value)(struct _st_things_representation *rep, const char *key, const struct _st_things_representation *value);

	/**
	* @brief API for getting the value of string array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will return deep-copied array value as out parameter, so application must free it after use.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the array type of value.
	* @param[out] array Reference of the string array to where the value will be copied.
	* @param[out] length Total number of elements in the array.
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_str_array_value)(struct _st_things_representation *rep, const char *key, char ***array, size_t *length);

	/**
	* @brief API for getting the value of integer array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will return deep-copied array value as out parameter, so application must free it after use.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the array type of value.
	* @param[out] array Reference of the integer array where the value will be copied.
	* @param[out] length Total number of elements in the array.
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_int_array_value)(struct _st_things_representation *rep, const char *key, int64_t **array, size_t *length);

	/**
	* @brief API for getting the value of double array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will return deep-copied array value as out parameter, so application must free it after use.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which will represent the array type of value.
	* @param[out] array Reference of the double array where the value will be copied.
	* @param[out] length Total number of elements in the array.
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_double_array_value)(struct _st_things_representation *rep, const char *key, double **array, size_t *length);

	/**
	* @brief API for getting the value of object array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will return deep-copied array value as out parameter, so application must free it after use.\n
	*          To free each object in array, st_things_destroy_representation_inst() in st_things.h should be used.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the array type of value.
	* @param[out] array Reference of the object array where the value will be copied.
	* @param[out] length Total number of elements in the array.
	* @return @c true if value exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_object_array_value)(struct _st_things_representation *rep, const char *key, struct _st_things_representation ***array, size_t *length);

	/**
	* @brief API for setting the value of string array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will deep-copy the array value inside, so application still has an ownership of memory for the array value.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[in]  array String array type value.
	* @param[in]  length Total number of elements in the array.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_str_array_value)(struct _st_things_representation *rep, const char *key, const char **array, size_t length);

	/**
	* @brief API for setting the value of integer array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will deep-copy the array value inside, so application still has an ownership of memory for the array value.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[in]  array Integer array type value.
	* @param[in]  length Total number of elements in the array.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_int_array_value)(struct _st_things_representation *rep, const char *key, const int64_t *array, size_t length);

	/**
	* @brief API for setting the value of double array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will deep-copy the array value inside, so application still has an ownership of memory for the array value.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[in]  array Double array type value.
	* @param[in]  length Total number of elements in the array.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_double_array_value)(struct _st_things_representation *rep, const char *key, const double *array, size_t length);

	/**
	* @brief API for setting the value of object array type property with a key.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @remarks This API will deep-copy the array value inside, so application still has an ownership of memory for the array value.
	* @param[in]  rep Instance of Representation.
	* @param[in]  key Property Name which represents the value.
	* @param[in]  array Object array type value.
	* @param[in]  length Total number of elements in the array.
	* @return @c true if setting value is successful, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*set_object_array_value)(struct _st_things_representation *rep, const char *key, const struct _st_things_representation **array, size_t length);

} st_things_representation_s;

/**
 * @brief Structure for representing the Get Request Message.
 * @since TizenRT v1.1
 */
typedef struct _st_things_get_request_message {
	char *resource_uri;									/**< Resource URI */
	char *query;										/**< One or more query parameters of the request message. Ex: key1=value1;key2=value2;... */
	char *property_key;									/**< One or more property key that application needs to set a value for response. Ex: key1;key2;... */

	/**
	* @brief API for getting the value of a specific query from the query parameters of the request.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  req_msg Instance of get request message.
	* @param[in]  key Name of the query.(ex: key1, key2, etc)
	* @param[out] value Value of the query.(value1, value2, etc)
	* @return @c true if query exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_query_value)(struct _st_things_get_request_message *req_msg, const char *key, char **value);

	/**
	* @brief API for checking whether the request has a specific property key or not.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  req_msg Instance of get request message.
	* @param[in]  key Name of the property.
	* @return @c true if the property key exists, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*has_property_key)(struct _st_things_get_request_message *req_msg, const char *key);

} st_things_get_request_message_s;

/**
 * @brief Structure for representing the Set Request Message.
 * @since TizenRT v1.1
 */
typedef struct _st_things_set_request_message {
	char *resource_uri;									/**< Resource URI */
	char *query;										/**< One or more query parameters of the request message. Ex: key1=value1?key2=value2?... */
	struct _st_things_representation *rep;				/**< Representation of the set request message */

	/**
	* @brief API for getting the value of a specific query from the query parameters of the request.
	*
	* @details @b #include <st_things/st_things_types.h>
	* @param[in]  req_msg Instance of request message.
	* @param[in]  key Name of the query.(ex: key1, key2, etc)
	* @param[out] value Value of the query.(value1, value2, etc)
	* @return @c true if query exist, otherwise @c false
	* @since TizenRT v1.1
	*/
	bool (*get_query_value)(struct _st_things_set_request_message *req_msg, const char *key, char **value);

} st_things_set_request_message_s;

#endif							/* __ST_THINGS_TYPES_H__ */
/** @} */// end of SmartThings group
