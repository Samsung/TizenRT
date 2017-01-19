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
 *      Declarations for the result acquisition API.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

#ifndef RESULT_H
#define RESULT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <arastorage/arastorage.h>
#include "index.h"
#include "storage.h"
#include "relation.h"

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/
#define RESULT_TUPLE_INVALID(tuple)     ((tuple) == NULL)
#define RESULT_TUPLE_SIZE(handle)       (handle).rel->row_length

#define DB_HANDLE_FLAG_INDEX_STEP       0x01
#define DB_HANDLE_FLAG_SEARCH_INDEX     0x02
#define DB_HANDLE_FLAG_PROCESSING       0x04
#define DB_HANDLE_FLAG_INVALID          0x00

/****************************************************************************
* Public Type Definitions
****************************************************************************/
typedef unsigned char *tuple_t;

/*
 * The source_dest_map structure is used for mapping the pointers to
 * data in a source row and in the corresponding destination row. The
 * structure is calculated just before processing a relational
 * selection, and then used to improve the performance when processing
 * each row.
*/
struct source_dest_map_s {
	attribute_t *from_attr;
	attribute_t *to_attr;
	unsigned from_offset;
	unsigned to_offset;
	db_value_type_t valuetype;
};
typedef struct source_dest_map_s source_dest_map_t;

struct _db_handle_s {
	index_iterator_t index_iterator;
	tuple_id_t tuple_id;
	tuple_id_t current_row;
	relation_t *rel;
	relation_t *result_rel;
	tuple_t tuple;
	uint32_t optype;
	uint8_t flags;
	uint8_t adt_flags;
	uint8_t ncolumns;
	void *lvm_instance;
	source_dest_map_t *attr_map;
};

/****************************************************************************
* Global Function Prototypes
****************************************************************************/
db_result_t db_get_value(attribute_value_t *value, db_handle_t *handle, unsigned col);
db_result_t db_phy_to_value(attribute_value_t *value, attribute_t *attr, unsigned char *ptr);
db_result_t db_value_to_phy(unsigned char *ptr, attribute_t *attr, attribute_value_t *value);

#endif              /* !RESULT_H */
long db_value_to_long(attribute_value_t *value);
