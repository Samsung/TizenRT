/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/**
 * @addtogroup HTTP_SERVER
 * @{
 */

/**
 * @file protocols/webserver/http_keyvalue_list.h
 * @brief APIs for key/value in HTTP Server.
 */

#ifndef __http_keyvalue_list_h__
#define __http_keyvalue_list_h__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <protocols/webserver/http_server.h>

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/*****************************************************************************
 * Public Types
 ****************************************************************************/

/**
 * @brief HTTP keyvalue structure.
 */
struct http_keyvalue_t {
	char key[HTTP_CONF_MAX_KEY_LENGTH];
	char value[HTTP_CONF_MAX_VALUE_LENGTH];

	struct http_keyvalue_t *prev;
	struct http_keyvalue_t *next;
};

/**
 * @brief HTTP keyvalue linked list structure.
 */
struct http_keyvalue_list_t {
	struct http_keyvalue_t *head;
	struct http_keyvalue_t *tail;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief http_keyvalue_list_init() allocates list's head and tail.
 *
 * @param[in] list the keyvalue list to be initialized.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int   http_keyvalue_list_init(struct http_keyvalue_list_t *list);

/**
 * @brief http_keyvalue_list_release() frees list.
 *
 * @param[in] list the keyvalue list to be freed.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int   http_keyvalue_list_release(struct http_keyvalue_list_t *list);

/**
 * @brief http_keyvalue_list_add() adds keyvalue to list.
 *
 * @param[in] list the keyvalue list to be freed.
 * @param[in] key the string to be a key.
 * @param[in] value the string to be a value.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int   http_keyvalue_list_add(struct http_keyvalue_list_t *list, const char *key, const char *value);

/**
 * @brief http_keyvalue_list_delete_tail() deletes keyvalue to list
 *        where in list's tail.
 *
 * @param[in] list the list that has keyvalue to be deleted.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int   http_keyvalue_list_delete_tail(struct http_keyvalue_list_t *list);

/**
 * @brief http_keyvalue_list_find() finds a value with a specific key.
 *
 * @param[in] list the keyvalue list to be found a value.
 * @param[in] key the key to find a value.
 * @return On success, a value is returned.
 *         On failure, "(null)" is returned.
 * @since TizenRT v1.0
 */
char *http_keyvalue_list_find(struct http_keyvalue_list_t *list, const char *key);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif
/** @} */
