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
 *      .
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

#ifndef RELATION_H
#define RELATION_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arastorage/arastorage.h>
#include "db_options.h"
#include "list.h"
#include "attribute.h"

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/

/* This decides index in int array */
#define GET_INDEX(a) ((a) / (sizeof(uint32_t) * 8))

/* This decides index in a int value of array */
#define GET_POS(a) ((a) % (sizeof(uint32_t) * 8))

/* This decides int array size to present all tuples in relation */
#define GET_CURSOR_DATA_ARR_SIZE(a) (((a) / (sizeof(uint32_t) * 8)) + 1)

/* Set a bit to 1 */
#define BIT_SET(a, b) ((a) |= (1u << (b)))

/* Clear a bit to 0 */
#define BIT_CLEAR(a, b) ((a) &= ~(1u << (b)))

/* Reverse a bit */
#define BIT_FLIP(a, b) ((a) ^= (1u << (b)))

/* Check a bit whether 1 or 0 */
#define BIT_CHECK(a, b) ((a) & (1u << (b)))

/* Check cursor is empty or not */
#define IS_EMPTY_CURSOR(a) ((a) == NULL || (a)->total_rows <= 0 || (a)->cursor_rows <= 0)

/* check current cursor row is valid or invalid*/
#define IS_INVALID_CURSOR_ROW(a) ((a) == NULL || ((a)->current_cursor_row >= (a)->cursor_rows))

/* check current storage row is valid or invalid*/
#define IS_INVALID_STORAGE_ROW(a) ((a) == NULL || ((a)->current_storage_row >= (a)->total_rows) || (a)->current_storage_row >= DB_CURSOR_RESULT_ENTRY)

#define RELATION_HAS_TUPLES(rel) ((rel)->tuple_storage >= 0)

/* Specific API will return Below if cursor value is something wrong */
#define INVALID_CURSOR_VALUE -1

/****************************************************************************
* Public Type Definitions
****************************************************************************/
enum db_direction_e {
	DB_MEMORY = 0,
	DB_STORAGE = 1
};
typedef enum db_direction_e db_direction_t;

enum db_value_type_e {
	NORMAL_VALUE = 0,
	AGGREGATE_VALUE = 1
};
typedef enum db_value_type_e db_value_type_t;

/*
 * A relation consists of a name, a set of domains, a set of indexes,
 * and a set of keys. Each relation must have a primary key.
 */
struct relation_s {
	struct relation_s *next;
	LIST_STRUCT(attributes);
	attribute_t *primary_key;
	size_t row_length;
	attribute_id_t attribute_count;
	tuple_id_t cardinality;
	tuple_id_t next_row;
	db_storage_id_t tuple_storage;
	db_direction_t dir;
	uint8_t references;
	char name[RELATION_NAME_LENGTH + 1];
	char tuple_filename[TUPLE_NAME_LENGTH + 1];
};

typedef struct relation_s relation_t;

/* A structure for reading data from storage */
struct cursor_data_map_s {
	char name[ATTRIBUTE_NAME_LENGTH + 1];
	domain_t domain;
	db_value_type_t valuetype;
	unsigned data_size;
	unsigned offset;
};
typedef struct cursor_data_map_s cursor_data_map_t;

/* A structure for cursor in SELECT operation */
struct _db_cursor_s {
	tuple_id_t current_cursor_row;
	tuple_id_t current_storage_row;
	tuple_id_t cursor_rows;
	tuple_id_t total_rows;
	attribute_id_t attribute_count;
	size_t storage_row_length;
	uint32_t *row_arr;
	unsigned char tuple[DB_MAX_ELEMENT_SIZE + 1];
	char name[TUPLE_NAME_LENGTH + 1];
	char rel_name[RELATION_NAME_LENGTH + 1];
	cursor_data_map_t attr_map[AQL_ATTRIBUTE_LIMIT];
};

/****************************************************************************
 * Internal function prototypes
 ****************************************************************************/
/* Operations for cursor processing */
db_result_t cursor_init(db_cursor_t **cursor, relation_t *rel);
db_result_t cursor_load(db_cursor_t **target, db_cursor_t *src);
db_result_t cursor_data_add(db_cursor_t *cursor, tuple_id_t tuple_id);
db_result_t cursor_deinit(db_cursor_t *cursor);


/* API for relations. */
db_result_t relation_init(void);
db_result_t relation_deinit(void);
db_result_t relation_process_remove(db_handle_t **, db_cursor_t *);
db_result_t relation_process_select(db_handle_t **, db_cursor_t *);
db_cursor_t *relation_process_result(db_handle_t *);
relation_t *relation_load(char *);
db_result_t relation_release(relation_t *);
relation_t *relation_create(char *, db_direction_t);
db_result_t relation_rename(char *, char *);
attribute_t *relation_attribute_add(relation_t *, db_direction_t, char *, domain_t, size_t);
attribute_t *relation_attribute_get(relation_t *, char *);
db_result_t relation_get_value(relation_t *, attribute_t *, unsigned char *, attribute_value_t *);
db_result_t relation_attribute_remove(relation_t *, char *);
db_result_t relation_set_primary_key(relation_t *, char *);
db_result_t relation_remove(relation_t *, int);
db_result_t relation_insert(relation_t *, attribute_value_t *);
db_result_t relation_select(db_handle_t **, relation_t *, void *);
tuple_id_t relation_cardinality(relation_t *);

#endif              /* RELATION_H */
