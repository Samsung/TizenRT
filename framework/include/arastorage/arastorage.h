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
/*
 * Copyright (c) 2010, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *      Declarations of the main Antelope functions.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/**
 * @defgroup ARASTORAGE ARASTORAGE
 * @brief Provides APIs for Lightweight Database, Arastorage
 * @ingroup ARASTORAGE
 * @{
 */

/**
 * @file arastorage/arastorage.h
 * @brief APIs for Lightweight Database, Arastorage
 */

#ifndef __ARASTORAGE_ARA_STORAGE_H
#define __ARASTORAGE_ARA_STORAGE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/
#define INVALID_TUPLE   (tuple_id_t)-1
#define INVALID_CURSOR_VALUE -1

#define DB_ERROR(result_code)   ((result_code) < DB_OK)
#define DB_SUCCESS(result_code) !DB_ERROR(result_code)

/* We only accept value in the range of [DB_XXX_MIN + 1, MAX] */
#define DB_INT_MAX INT_MAX
#define DB_INT_MIN INT_MIN + 1

#define DB_LONG_MAX LONG_MAX
#define DB_LONG_MIN LONG_MIN + 1

#ifdef CONFIG_HAVE_DOUBLE
#define DB_DOUBLE_MAX DBL_MAX
#define DB_DOUBLE_MIN DBL_MIN + 1
#else
#define DB_DOUBLE_MAX FLT_MAX
#define DB_DOUBLE_MIN FLT_MIN + 1
#endif

/* We consider that the least value in the range of each type is invalid */
#define DB_INT_ERROR INT_MIN
#define DB_LONG_ERROR LONG_MIN
#ifdef CONFIG_HAVE_DOUBLE
#define DB_DOUBLE_ERROR DBL_MIN
#else
#define DB_DOUBLE_ERROR FLT_MIN
#endif

/****************************************************************************
* Public Type Definitions
****************************************************************************/
#define DB_FINISHED_MSG "Iteration finished"
#define DB_OK_MSG "Operation succeeded"
#define DB_LIMIT_ERROR_MSG "Limit reached"
#define DB_ALLOCATION_ERROR_MSG "Allocation error"
#define DB_STORAGE_ERROR_MSG "Storage error"
#define DB_PARSING_ERROR_MSG "Parsing error"
#define DB_NAME_ERROR_MSG "Invalid name"
#define DB_RELATIONAL_ERROR_MSG "Semantic error"
#define DB_TYPE_ERROR_MSG "Type error"
#define DB_IMPLEMENTATION_ERROR_MSG "Implementation error"
#define DB_INDEX_ERROR_MSG "Index error"
#define DB_BUSY_ERROR_MSG "Busy with processing"
#define DB_INCONSISTENCY_ERROR_MSG "Inconsistent handle"
#define DB_ARGUMENT_ERROR_MSG "Invalid argument"
#define DB_FULL_ERROR_MSG "Tuple limit reached"
#define DB_CURSOR_ERROR_MSG "Cursor Error"
#define DB_UNKNOWN_ERROR_MSG "Unknown result code"
#define DB_EMPTY_CURSOR_ERROR_MSG "Empty Cursor"

enum db_result_e {
	DB_FINISHED = 3,
	DB_GOT_ROW = 2,
	DB_OK = 1,
	DB_LIMIT_ERROR = -1,
	DB_ALLOCATION_ERROR = -2,
	DB_STORAGE_ERROR = -3,
	DB_PARSING_ERROR = -4,
	DB_NAME_ERROR = -5,
	DB_RELATIONAL_ERROR = -6,
	DB_TYPE_ERROR = -7,
	DB_IMPLEMENTATION_ERROR = -8,
	DB_INDEX_ERROR = -9,
	DB_BUSY_ERROR = -10,
	DB_INCONSISTENCY_ERROR = -11,
	DB_ARGUMENT_ERROR = -12,
	DB_FULL_ERROR = -13,
	DB_CURSOR_ERROR = -14,
	DB_EMPTY_CURSOR_ERROR = -15
};

typedef enum db_result_e db_result_t;

enum domain_e {
	DOMAIN_UNSPECIFIED = 0,
	DOMAIN_INT = 1,
	DOMAIN_LONG = 2,
	DOMAIN_STRING = 3,
	DOMAIN_DOUBLE = 4
};

typedef enum domain_e domain_t;


struct _db_handle_s;
typedef struct _db_handle_s db_handle_t;

struct _db_cursor_s;
typedef struct _db_cursor_s db_cursor_t;

typedef int db_storage_id_t;

typedef uint32_t cursor_row_t;

typedef uint32_t tuple_id_t;

typedef uint8_t attribute_id_t;

/****************************************************************************
* Public Variables
****************************************************************************/
typedef int (*db_output_function_t)(const char *, ...);

static db_output_function_t output = printf;

/****************************************************************************
* Global Function Prototypes
****************************************************************************/

/**
* @brief initialize database's resources, it must be called for using arastorage
*
* @details @b #include <arastorage/arastorage.h>
* @param none
* @return On success, DB_OK is returned. On failure, a negative value is returned.
* @since TizenRT v1.0
*/
db_result_t db_init(void);

/**
* @brief de-initialize database's resources, it must be called when terminating arastorage
*
* @details @b #include <arastorage/arastorage.h>
* @param none
* @return On success, DB_OK is returned. On failure, a negative value is returned.
* @since TizenRT v1.0
*/
db_result_t db_deinit(void);

/**
* @brief create or remove relations, attributes and indexes in arastorage
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] format query sentence
* @return On success, DB_OK is returned. On failure, a negative value is returned.
* @since TizenRT v1.0
*
*/
db_result_t db_exec(char *format);

/**
* @brief process query of arastorage
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] format query sentence
* @return On success, a pointer to db_cursor_t is returned. On failure, a NULL is returned.
* @since TizenRT v1.0
*/
db_cursor_t *db_query(char *format);

/**
* @brief free allocated cursor data, it should be called before application terminated
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, DB_OK is returned. On failure, a negative value is returned.
* @since TizenRT v1.0
*/
db_result_t db_cursor_free(db_cursor_t *cursor);

/**
* @brief get string corresponding to each result value of API
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] code result value
* @return string of each result value
* @since TizenRT v1.0
*/
const char *db_get_result_message(db_result_t code);

/**
* @brief print the name of relation and attributes
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, DB_OK is returned. On failure, a negative value is returned.
* @since TizenRT v1.0
*/
db_result_t db_print_header(db_cursor_t *cursor);

/**
* @brief print the tuple data
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, DB_OK is returned. On failure, a negative value is returned.
* @since TizenRT v1.0
*/
db_result_t db_print_tuple(db_cursor_t *cursor);

/**
* @brief print current row's data with specific attribute index
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_index index of attribute(column) in cursor
* @return On success, DB_OK is returned. On failure, a negative value is returned.
* @since TizenRT v1.0
*/
db_result_t db_print_value(db_cursor_t *cursor, int attr_index);

/**
* @brief move current position of cursor to first row
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, DB_OK is returned. On failure, DB_CURSOR_ERROR is returned.
* @since TizenRT v1.0
*/
db_result_t cursor_move_first(db_cursor_t *cursor);

/**
* @brief move current position of cursor to last row
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, DB_OK is returned. On failure, DB_CURSOR_ERROR is returned.
* @since TizenRT v1.0
*/
db_result_t cursor_move_last(db_cursor_t *cursor);

/**
* @brief move current position of cursor to next row
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, DB_OK is returned. On failure, DB_CURSOR_ERROR is returned.
* @since TizenRT v1.0
*/
db_result_t cursor_move_next(db_cursor_t *cursor);

/**
* @brief move current position of cursor to previous row
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, DB_OK is returned. On failure, DB_CURSOR_ERROR is returned.
* @since TizenRT v1.0
*/
db_result_t cursor_move_prev(db_cursor_t *cursor);

/**
* @brief move current position of cursor to specific row
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] row_id index of row
* @return On success, DB_OK is returned. On failure, DB_CURSOR_ERROR is returned.
* @since TizenRT v1.0
*/
db_result_t cursor_move_to(db_cursor_t *cursor, tuple_id_t row_id);

/**
* @brief check whether current position of cursor is first row
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, true is returned. On failure, false is returned.
* @since TizenRT v1.0
*/
bool cursor_is_first_row(db_cursor_t *cursor);

/**
* @brief check whether current position of cursor is last row
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, true is returned. On failure, false is returned.
* @since TizenRT v1.0
*/
bool cursor_is_last_row(db_cursor_t *cursor);

/**
* @brief get current position of cursor
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, the index of current row is returned. On failure, INVALID_CURSOR_VALUE is returned.
* @since TizenRT v1.0
*/
cursor_row_t cursor_get_row(db_cursor_t *cursor);

/**
* @brief get the number of rows of cursor
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @return On success, the number of rows is returned. On failure, INVALID_CURSOR_VALUE is returned.
* @since TizenRT v1.0
*/
cursor_row_t cursor_get_count(db_cursor_t *cursor);

/**
* @brief get type of attribute with specific index in cursor
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_index index of attribute(column) in cursor
* @return On success, positive value is returned. On failure, INVALID_CURSOR_VALUE is returned.
* @since TizenRT v1.0
*/
domain_t cursor_get_attr_type(db_cursor_t *cursor, int attr_index);

/**
* @brief get name of attribute with specific index in cursor
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_index index of attribute(column) in cursor
* @return On success, attribute name is returned. On failure, a NULL is returned.
* @since TizenRT v1.0
*/
char *cursor_get_attr_name(db_cursor_t *cursor, int attr_index);

/**
* @brief get index of attribute corresponding name in cursor
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_name attribute name
* @return On success, non-negative value is returned. On failure, INVALID_CURSOR_VALUE is returned.
* @since TizenRT v1.0
*/
attribute_id_t cursor_get_attr_index(db_cursor_t *cursor, const char *attr_name);

/**
* @brief get value of attribute of which the type is DOMAIN_INT with specific index in current row.
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_index index of attribute(column) in cursor
* @return On success, integer type value of which the range is [DB_INT_MIN+1, DB_INT_MAX] is returned.
*	  On failure, DB_INT_ERROR is returned. These are defined in arastorage.h.
* @since TizenRT v1.0
*/
int cursor_get_int_value(db_cursor_t *cursor, int attr_index);

/**
* @brief get value of attribute of which the type is DOMAIN_LONG with specific index in current row.
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_index index of attribute(column) in cursor
* @return On success, long type value of which the range is [DB_LONG_MIN+1, DB_LONG_MAX] is returned.
*	  On failure, a DB_LONG_ERROR is returned. These are defined arastorage.h.
* @since TizenRT v1.0
*/
long cursor_get_long_value(db_cursor_t *cursor, int attr_index);

#ifdef CONFIG_ARCH_FLOAT_H
/**
* @brief get value of attribute of which the type is DOMAIN_DOUBLE with specific index in current row.
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_index index of attribute(column) in cursor
* @return On success, double value of which the range is [DB_DOUBLE_MIN+1, DB_DOUBLE_MAX] is returned.
*	  On failure, DB_DOUBLE_ERROR is returned. These are defined arastorage.h.
* @since TizenRT v1.0
*/
double cursor_get_double_value(db_cursor_t *cursor, int attr_index);
#endif

/**
* @brief get value of attribute of which the type is DOMAIN_STRING with specific index in current row.
*
* @details @b #include <arastorage/arastorage.h>
* @param[in] cursor a pointer to cursor
* @param[in] attr_index index of attribute(column) in cursor
* @return On success, string value is returned. On failure, a NULL is returned.
* @since TizenRT v1.0
*/
unsigned char *cursor_get_string_value(db_cursor_t *cursor, int attr_index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */ // end of AraStorage group

#endif /* __ARASTORAGE_ARA_STORAGE_H */
