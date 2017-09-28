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
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *      Query execution functions for AQL.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "db_debug.h"
#include "storage.h"
#include "relation.h"
#include "result.h"
#include "aql.h"

/****************************************************************************
* Private Functions
****************************************************************************/
db_result_t aql_init_handle(db_handle_t **handle)
{
	*handle = NULL;
	*handle = (db_handle_t *)malloc(sizeof(db_handle_t));
	if (*handle == NULL) {
		return DB_ALLOCATION_ERROR;
	}
	memset((*handle), 0, sizeof(db_handle_t));
	DB_LOG_D("init handle!\n");
	return DB_OK;
}

db_result_t aql_deinit_handle(db_handle_t **handle)
{
	db_result_t res;
	if (handle == NULL || *handle == NULL) {
		return DB_ARGUMENT_ERROR;
	}
	res = DB_OK;
	if ((*handle)->rel != NULL) {
		res = relation_release((*handle)->rel);
		if (DB_ERROR(res)) {
			return res;
		}
	}
	if ((*handle)->result_rel != NULL) {
		res = relation_release((*handle)->result_rel);
		if (DB_ERROR(res)) {
			return res;
		}
	}
	if ((*handle)->tuple != NULL) {
		free((*handle)->tuple);
		(*handle)->tuple = NULL;
	}
	if ((*handle)->lvm_instance != NULL) {
		free((*handle)->lvm_instance);
		(*handle)->lvm_instance = NULL;
	}
	if ((*handle)->attr_map != NULL) {
		free((*handle)->attr_map);
		(*handle)->attr_map = NULL;
	}
	free(*handle);
	*handle = NULL;
	DB_LOG_D("deinit handle!\n");
	return res;
}

/****************************************************************************
* Public Functions
****************************************************************************/
db_result_t aql_get_parse_result(char *format, aql_adt_t *adt)
{
	if (format == NULL) {
		return DB_ARGUMENT_ERROR;
	}
	if (AQL_ERROR(aql_parse(adt, format))) {
		return DB_PARSING_ERROR;
	}
	return DB_OK;

}

relation_t *aql_get_relation(aql_adt_t *adt)
{
	int first_rel_arg;

	/* If the ASSIGN flag is set, the first relation in the array is
	   the desired result relation. */
	first_rel_arg = ! !(adt->flags & AQL_FLAG_ASSIGN);
	return relation_load(adt->relations[first_rel_arg]);
}

db_result_t db_exec(char *format)
{
	db_result_t res;
	aql_adt_t adt;
	relation_t *rel = NULL;
	aql_attribute_t *attr;
	attribute_t *relattr = NULL;
	uint32_t optype;
	res = aql_get_parse_result(format, &adt);

	if (DB_ERROR(res)) {
		DB_LOG_E("DB : Parsing Error in db_create : %d\n", res);
		return DB_PARSING_ERROR;
	}

	optype = AQL_GET_OP_TYPE(AQL_GET_TYPE(&adt));
	if (optype == AQL_OP_TYPE_QUERY) {
		DB_LOG_E("DB : AQL OP TYPE Error \n");
		return DB_ARGUMENT_ERROR;
	}

	optype = AQL_GET_EXEC_TYPE(AQL_GET_TYPE(&adt));
	if (optype != AQL_TYPE_CREATE_RELATION) {
		rel = aql_get_relation(&adt);
		if (rel == NULL) {
			DB_LOG_E("DB : get relation Failed\n");
			return DB_RELATIONAL_ERROR;
		}
	}

	res = DB_RELATIONAL_ERROR;

	switch (optype) {
	case AQL_TYPE_CREATE_ATTRIBUTE:
		attr = &(adt.attributes[0]);
		if (relation_attribute_add(rel, DB_STORAGE, attr->name, attr->domain, attr->element_size) != NULL) {
			res = DB_OK;
		}
		break;
	case AQL_TYPE_CREATE_INDEX:
		relattr = relation_attribute_get(rel, adt.attributes[0].name);
		if (relattr == NULL) {
			res = DB_NAME_ERROR;
			break;
		}
		res = index_create(AQL_GET_INDEX_TYPE(&adt), rel, relattr);
		break;
	case AQL_TYPE_CREATE_RELATION:
		if (relation_create(adt.relations[0], DB_STORAGE) != NULL) {
			res = DB_OK;
		}
		break;
	case AQL_TYPE_INSERT:
		if (relation_cardinality(rel) < DB_TUPLE_LIMIT) {
			res = relation_insert(rel, adt.values);
			if (DB_SUCCESS(res)) {
				res = DB_OK;
			}
		} else {
			res = DB_LIMIT_ERROR;
		}
		break;
	case AQL_TYPE_REMOVE_ATTRIBUTE:
		res = relation_attribute_remove(rel, adt.attributes[0].name);
		break;
	case AQL_TYPE_REMOVE_INDEX:
		relattr = relation_attribute_get(rel, adt.attributes[0].name);
		if (relattr != NULL) {
			index_load(rel, relattr);
			if (relattr->index != NULL) {
				res = index_destroy(relattr->index);
			} else {
				res = DB_OK;
			}
		} else {
			res = DB_NAME_ERROR;
		}
		break;
	case AQL_TYPE_REMOVE_RELATION:
		res = relation_remove(rel, 1);
		break;
	default:
		break;
	}
	if (rel != NULL) {
		relation_release(rel);
	}
	return res;
}

db_cursor_t *db_query(char *format)
{
	aql_adt_t adt;
	relation_t *rel;
	uint32_t optype;
	db_handle_t *handler;
	attribute_t *attr_ptr;
	db_cursor_t *cursor;

	handler = NULL;
	cursor = NULL;

	if (DB_ERROR(aql_get_parse_result(format, &adt))) {
		DB_LOG_E("DB : Parsing Error in db_create : %d\n");
		return NULL;
	}
	optype = AQL_GET_OP_TYPE(AQL_GET_TYPE(&adt));
	if (optype != AQL_OP_TYPE_QUERY) {
		DB_LOG_E("DB : AQL OP TYPE Error \n");
		return NULL;
	}
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
	if (DB_SUCCESS(storage_flush_insert_buffer())) {
		DB_LOG_D("DB : flush insert buffer!!\n");
	}
#endif

	rel = aql_get_relation(&adt);
	if (rel == NULL) {
		return NULL;
	}

	optype = AQL_GET_EXEC_TYPE(AQL_GET_TYPE(&adt));
	switch (optype) {
	case AQL_TYPE_REMOVE_TUPLES:
		/* Overwrite the attribute array with a full copy of the original
		   relation's attributes. */
		adt.attribute_count = 0;
		for (attr_ptr = list_head(rel->attributes); attr_ptr != NULL; attr_ptr = attr_ptr->next) {
			AQL_ADD_ATTRIBUTE(&adt, attr_ptr->name, DOMAIN_UNSPECIFIED, 0);
		}
	/* FALLTHROUGH */
	case AQL_TYPE_SELECT:
		if (DB_ERROR(aql_init_handle(&handler))) {
			DB_LOG_E("DB: Init handle failed\n");
			goto errout;
		}
		if (DB_ERROR(relation_select(&handler, rel, &adt))) {
			DB_LOG_E("DB: Failed relation_select\n");
			goto errout;
		}
		cursor = relation_process_result(handler);
		if (cursor == NULL) {
			DB_LOG_E("DB: Failed to process cursor tuples\n");
			goto errout;
		}
		break;
	case AQL_TYPE_FLUSH:
	//TODO flush operation will be implemented later
	default:
		break;
	}

	if (rel != NULL) {
		if (handler == NULL || !(handler->flags & DB_HANDLE_FLAG_PROCESSING)) {
			relation_release(rel);
		}
	}
	aql_deinit_handle(&handler);

	return cursor;

errout:
	if (rel != NULL) {
		relation_release(rel);
	}

	aql_deinit_handle(&handler);

	return NULL;
}
