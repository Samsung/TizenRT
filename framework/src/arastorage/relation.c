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
 *      Logic for relational databases.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <tinyara/config.h>
#include <sys/types.h>

#include "index.h"
#include "lvm.h"
#include "storage.h"
#include "db_options.h"
#include "db_debug.h"
#include "list.h"
#include "aql.h"
#include "relation.h"

/****************************************************************************
* Global Function Prototypes
****************************************************************************/

LIST(relations);
// TODO: SIZE limitation not applied
//MEMB(relations_memb, relation_t, DB_RELATION_POOL_SIZE);
//MEMB(attributes_memb, attribute_t, DB_ATTRIBUTE_POOL_SIZE);
static relation_t *relation_find(char *);
static attribute_t *attribute_find(relation_t *, char *);
static int get_attribute_value_offset(relation_t *, attribute_t *);
static void attribute_free(relation_t *, attribute_t *);
static void purge_relations(void);
static void relation_clear(relation_t *);
static relation_t *relation_allocate(void);
static void relation_free(relation_t *);

/****************************************************************************
* Public Functions
****************************************************************************/
static relation_t *relation_find(char *name)
{
	relation_t *rel;
	rel = list_head(relations);
	while (rel != NULL) {
		if (strcmp(rel->name, name) == 0) {
			return rel;
		}
		rel = rel->next;
	}
	return NULL;
}

static attribute_t *attribute_find(relation_t *rel, char *name)
{
	attribute_t *attr;
	attr = list_head(rel->attributes);
	while (attr != NULL) {
		if (strcmp(attr->name, name) == 0) {
			return attr;
		}
		attr = attr->next;
	}
	return NULL;
}

static int get_attribute_value_offset(relation_t *rel, attribute_t *attr)
{
	attribute_t *ptr;
	int offset;

	offset = 0;
	ptr = list_head(rel->attributes);
	while (ptr != NULL) {
		if (ptr == attr) {
			return offset;
		}
		offset += ptr->element_size;
		ptr = ptr->next;
	}
	return -1;
}

static void attribute_free(relation_t *rel, attribute_t *attr)
{
	if (attr->index != NULL) {
		index_release(attr->index);
	}
	free(attr);
	rel->attribute_count--;
}

static void purge_relations(void)
{
	relation_t *rel;
	relation_t *next;
	rel = list_head(relations);

	while (rel != NULL) {
		next = rel->next;
		if (rel->references == 0) {
			relation_free(rel);
		}
		rel = next;
	}
}

static void relation_clear(relation_t *rel)
{
	memset(rel, 0, sizeof(*rel));
	rel->tuple_storage = -1;
	rel->cardinality = INVALID_TUPLE;
	rel->dir = DB_STORAGE;
	LIST_STRUCT_INIT(rel, attributes);
}

static relation_t *relation_allocate(void)
{
	relation_t *rel;

	rel = malloc(sizeof(relation_t));
	if (rel == NULL) {
		purge_relations();
		rel = malloc(sizeof(relation_t));
		if (rel == NULL) {
			DB_LOG_E("DB: Failed to allocate a relation\n");
			return NULL;
		}
	}

	relation_clear(rel);
	return rel;
}

static void relation_free(relation_t *rel)
{
	attribute_t *attr;

	do {
		attr = list_pop(rel->attributes);
		if (attr != NULL) {
			attribute_free(rel, attr);
		}
	} while (attr != NULL);

	list_remove(relations, rel);
	free(rel);
}

db_result_t relation_init(void)
{
	list_init(relations);
	if (list_head(relations) != NULL) {
		return DB_RELATIONAL_ERROR;
	}

	return DB_OK;
}

db_result_t relation_deinit(void)
{
	relation_t *rel;
	relation_t *next;

	rel = list_head(relations);
	while (rel != NULL) {
		next = rel->next;
		relation_free(rel);
		rel = next;
	}

	return DB_OK;
}

relation_t *relation_load(char *name)
{
	relation_t *rel;

	rel = relation_find(name);
	if (rel != NULL) {
		rel->references++;
		goto end;
	}

	rel = relation_allocate();
	if (rel == NULL) {
		return NULL;
	}

	if (DB_ERROR(storage_get_relation(rel, name))) {
		free(rel);
		return NULL;
	}

	memcpy(rel->name, name, sizeof(rel->name));
	rel->name[sizeof(rel->name) - 1] = '\0';
	rel->references = 1;
	list_add(relations, rel);

end:
	if (rel->dir == DB_STORAGE && DB_ERROR(storage_load(rel))) {
		relation_release(rel);
		return NULL;
	}

	rel->cardinality = relation_cardinality(rel);
	DB_LOG_D("DB: Rel %s, Cardinality %d\n", rel->name, rel->cardinality);

	return rel;
}

db_result_t relation_release(relation_t *rel)
{
	db_result_t res;
	if (rel->references > 0) {
		rel->references--;
	}

	if (rel->references == 0) {
		res = storage_unload(rel);
		if (DB_ERROR(res)) {
			return res;
		}
	}
	return DB_OK;
}

relation_t *relation_create(char *name, db_direction_t dir)
{
	relation_t *rel;
	
	if (*name != '\0') {
		rel = relation_allocate();
		if (rel == NULL) {
			return NULL;
		}
		relation_clear(rel);

		if (storage_get_relation(rel, name) == DB_OK) {
			/* Reject a creation request if the relation already exists. */
			DB_LOG_E("DB: Attempted to create a relation that already exists (%s)\n", name);
			relation_free(rel);
			return NULL;
		}

		relation_clear(rel);
		rel->cardinality = 0;
		strncpy(rel->name, name, sizeof(rel->name) - 1);

		rel->name[sizeof(rel->name) - 1] = '\0';
		rel->dir = dir;
		if (dir == DB_STORAGE) {
			storage_drop_relation(rel, 1);
			if (storage_put_relation(rel) == DB_OK) {
				list_add(relations, rel);
				return rel;
			}
			free(rel);
		} else {
			list_add(relations, rel);
			return rel;
		}
	}
	return NULL;
}

db_result_t relation_rename(char *old_name, char *new_name)
{
	relation_t *rel = relation_load(new_name);

	if (DB_ERROR(relation_remove(rel, 1)) || DB_ERROR(storage_rename_relation(old_name, new_name))) {
		return DB_STORAGE_ERROR;
	}

	return DB_OK;
}

attribute_t *relation_attribute_add(relation_t *rel, db_direction_t dir, char *name, domain_t domain, size_t element_size)
{
	attribute_t *attribute;
	tuple_id_t cardinality;

	cardinality = relation_cardinality(rel);
	if (cardinality != INVALID_TUPLE && cardinality > 0) {
		DB_LOG_E("DB: Attempt to create an attribute in a non-empty relation\n");
		return NULL;
	}

	if (element_size == 0 || element_size > DB_MAX_ELEMENT_SIZE) {
		DB_LOG_E("DB: Unacceptable element size: %u\n", element_size);
		return NULL;
	}

	attribute = malloc(sizeof(attribute_t));
	if (attribute == NULL) {
		DB_LOG_E("DB: Failed to allocate attribute \"%s\"!\n", name);
		return NULL;
	}

	strncpy(attribute->name, name, sizeof(attribute->name) - 1);
	attribute->name[sizeof(attribute->name) - 1] = '\0';
	attribute->domain = domain;
	attribute->element_size = element_size;
	attribute->aggregator = 0;
	attribute->index = NULL;
	attribute->flags = 0 /*ATTRIBUTE_FLAG_UNIQUE */ ;

	if (dir == DB_STORAGE) {
		if (DB_ERROR(storage_put_attribute(rel, attribute))) {
			DB_LOG_E("DB: Failed to store attribute %s\n", attribute->name);
			free(attribute);
			return NULL;
		}
	}
	
	rel->row_length += element_size;
	list_add(rel->attributes, attribute);
	rel->attribute_count++;

	return attribute;
}

attribute_t *relation_attribute_get(relation_t *rel, char *name)
{
	attribute_t *attr;

	attr = attribute_find(rel, name);
	if (attr != NULL && !(attr->flags & ATTRIBUTE_FLAG_INVALID)) {
		return attr;
	}

	return NULL;
}

db_result_t relation_attribute_remove(relation_t *rel, char *name)
{
	/* Not implemented completely. */
	return DB_IMPLEMENTATION_ERROR;
}

db_result_t relation_get_value(relation_t *rel, attribute_t *attr, unsigned char *row_ptr, attribute_value_t *value)
{
	int offset;
	unsigned char *from_ptr;

	offset = get_attribute_value_offset(rel, attr);
	if (offset < 0) {
		return DB_IMPLEMENTATION_ERROR;
	}
	from_ptr = row_ptr + offset;

	return db_phy_to_value(value, attr, from_ptr);
}

db_result_t relation_set_primary_key(relation_t *rel, char *name)
{
	attribute_t *attribute;

	attribute = relation_attribute_get(rel, name);
	if (attribute == NULL) {
		return DB_NAME_ERROR;
	}

	attribute->flags = ATTRIBUTE_FLAG_PRIMARY_KEY;
	DB_LOG_D("DB: New primary key: %s\n", attribute->name);

	return DB_OK;
}

db_result_t relation_remove(relation_t *rel, int remove_tuples)
{
	db_result_t result;
	attribute_t *attr;
	int len;
	char *filename;

	if (rel == NULL) {
		/*
		 * Attempt to remove an inexistent relation. To allow for this
		 * operation to be used for setting up repeatable tests and
		 * experiments, we do not signal an error.
		 */
		return DB_OK;
	}

	if (rel->references > 1) {
		return DB_BUSY_ERROR;
	}
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
	/* Flush insert buffer to make sure of writing tuples before removing relation */
	if (DB_SUCCESS(storage_flush_insert_buffer())) {
		DB_LOG_D("DB : flush insert buffer!!\n");
	}
#endif
	attr = list_head(rel->attributes);

	while (attr != NULL) {
		if (DB_SUCCESS(index_load(rel, attr))) {
			result = index_destroy(attr->index);
			if (DB_ERROR(result)) {
				DB_LOG_E("DB: Index %s.%s destroy failed\n", rel->name, attr->name);
			}
		}
		attr = list_item_next(attr);
	}

	len = strlen(rel->name) + strlen(INDEX_NAME_SUFFIX) + 1;
	filename = malloc(sizeof(char) * len);
	if (filename == NULL) {
		return DB_STORAGE_ERROR;
	}
	snprintf(filename, len, "%s%s\0", rel->name, INDEX_NAME_SUFFIX);
	result = storage_remove(filename);
	free(filename);
	if (DB_ERROR(result)) {
		DB_LOG_E("DB: Index file unlinking failed\n");
	}

	result = storage_drop_relation(rel, remove_tuples);
	relation_free(rel);
	return result;
}

db_result_t relation_insert(relation_t *rel, attribute_value_t *values)
{
	attribute_t *attr;
	unsigned char record[rel->row_length];
	unsigned char *ptr;
	attribute_value_t *value;
	db_result_t result;

	value = values;

	DB_LOG_D("DB: Relation %s has a record size of %u bytes\n", rel->name, (unsigned)rel->row_length);
	ptr = record;

	DB_LOG_V("DB: Insert (");

	attr = list_head(rel->attributes);
	while (attr != NULL) {
		/* Verify that the value is in the expected domain. An exception
		   to this rule is that INT may be promoted to LONG. */
		if (attr->domain != value->domain && !(attr->domain == DOMAIN_LONG && value->domain == DOMAIN_INT)) {
			DB_LOG_E("DB: The value domain %d does not match the domain %d of attribute %s\n", value->domain, attr->domain, attr->name);
			return DB_RELATIONAL_ERROR;
		}

		/* Set the data area for removed attributes to 0. */
		if (attr->flags & ATTRIBUTE_FLAG_INVALID) {
			memset(ptr, 0, attr->element_size);
			ptr += attr->element_size;
			continue;
		}

		result = db_value_to_phy((unsigned char *)ptr, attr, value);
		if (DB_ERROR(result)) {
			return result;
		}
#if DEBUG
		switch (attr->domain) {
		case DOMAIN_INT:
			DB_LOG_V("%s=%d", attr->name, VALUE_INT(value));
			break;
		case DOMAIN_LONG:
			DB_LOG_V("%s=%ld", attr->name, VALUE_LONG(value));
			break;
		case DOMAIN_STRING:
			DB_LOG_V("%s='%s", attr->name, VALUE_STRING(value));
			break;
		default:
			DB_LOG_V(")\nDB: Unhandled attribute domain: %d\n", attr->domain);
			return DB_TYPE_ERROR;
		}

		if (attr->next != NULL) {
			DB_LOG_V(", ");
		}
#endif              /* DEBUG */
		if (attr->index == NULL) {
			index_load(rel, attr);
		}
		ptr += attr->element_size;
		if (attr->index != NULL) {
			if (DB_ERROR(index_insert(attr->index, value, rel->next_row))) {
				return DB_INDEX_ERROR;
			}
		}
		attr = attr->next;
		value++;
	}

	DB_LOG_V(")\n");

	return storage_put_row(rel, record, FALSE);
}

/*
 * Update aggregation value whenever each tuple is read.
 */
static db_result_t aggregate(attribute_t *attr, attribute_value_t *value, tuple_id_t count)
{
	long long_value;
	double sum_value;

	switch (value->domain) {
	case DOMAIN_INT:
		long_value = VALUE_INT(value);
		break;
	case DOMAIN_LONG:
		long_value = VALUE_LONG(value);
		break;
	default:
		return DB_TYPE_ERROR;
	}

	sum_value = 0;
	switch (attr->aggregator) {
	case AQL_COUNT:
		attr->aggregation_value++;
		break;
	case AQL_SUM:
		attr->aggregation_value += (double)long_value;
		break;
	case AQL_MEAN:
		if (count == 1) {
			attr->aggregation_value = (double)long_value;
		} else {
			sum_value = (double) (attr->aggregation_value * (count - 1));
			attr->aggregation_value = (double) ((sum_value + long_value) / count);
		}
		break;
	case AQL_MAX:
		if (long_value > attr->aggregation_value) {
			attr->aggregation_value = (double)long_value;
		}
		break;
	case AQL_MIN:
		if (long_value < attr->aggregation_value) {
			attr->aggregation_value = (double)long_value;
		}
		break;
	default:
		return DB_TYPE_ERROR;
		break;
	}

	DB_LOG_D("DB: aggregation value of attribute %s is %f.\n", attr->name, attr->aggregation_value);

	return DB_OK;
}

static db_result_t generate_attribute_map(source_dest_map_t *attr_map, unsigned attribute_count, relation_t *from_rel, relation_t *to_rel)
{
	attribute_t *from_attr;
	attribute_t *to_attr;
	unsigned size_sum;
	source_dest_map_t *attr_map_ptr;
	int offset;

	attr_map_ptr = attr_map;
	size_sum = 0;
	to_attr = list_head(to_rel->attributes);
	while (to_attr != NULL) {
		from_attr = attribute_find(from_rel, to_attr->name);
		if (from_attr == NULL) {
			DB_LOG_E("DB: Invalid attribute in the result relation: %s\n", to_attr->name);
			return DB_NAME_ERROR;
		}

		attr_map_ptr->from_attr = from_attr;
		attr_map_ptr->to_attr = to_attr;

		offset = get_attribute_value_offset(from_rel, from_attr);
		if (offset < 0) {
			return DB_IMPLEMENTATION_ERROR;
		}

		attr_map_ptr->from_offset = offset;
		attr_map_ptr->to_offset = size_sum;
		size_sum += to_attr->element_size;

		if (to_attr->aggregator != 0) {
			attr_map_ptr->valuetype = AGGREGATE_VALUE;
		} else {
			attr_map_ptr->valuetype = NORMAL_VALUE;
		}

		attr_map_ptr++;
		to_attr = to_attr->next;
	}
	return DB_OK;
}

static void select_index(db_handle_t **handle)
{
	index_t *index;
	attribute_t *attr;
	operand_value_t min;
	operand_value_t max;
	attribute_value_t av_min;
	attribute_value_t av_max;
	long range;
	unsigned long min_range;
	index = NULL;
	min_range = ULONG_MAX;

	/* Find all indexed and derived attributes, and select the index of
	   the attribute with the smallest range. */
	attr = list_head((*handle)->rel->attributes);
	while (attr != NULL) {
		if (attr->index != NULL && !LVM_ERROR(lvm_get_derived_range((*handle)->lvm_instance, attr->name, &min, &max))) {
			range = (unsigned long)max.l - (unsigned long)min.l;
			DB_LOG_D("DB: The search range for attribute \"%s\" comprises %ld values\n", attr->name, range + 1);
			if (range <= min_range) {
				index = attr->index;
				av_min.domain = av_max.domain = DOMAIN_INT;
				VALUE_LONG(&av_min) = min.l;
				VALUE_LONG(&av_max) = max.l;
			}
		}
		attr = attr->next;
	}

	if (index != NULL) {
		/* We found a suitable index; get an iterator for it. */
		if (index_get_iterator(&((*handle)->index_iterator), index, &av_min, &av_max) == DB_OK) {
			(*handle)->flags |= DB_HANDLE_FLAG_SEARCH_INDEX;
		}
	} else {
		(*handle)->flags = DB_HANDLE_FLAG_INVALID;
	}
}

static void relation_index_clear(relation_t *rel)
{
	char *filename;
	attribute_t *attr_ptr;
	int len;

	len = strlen(rel->name) + strlen(INDEX_NAME_SUFFIX) + 1;
	filename = (char *)malloc(sizeof(char) * len);
	if (filename == NULL) {
		return;
	}
	snprintf(filename, len, "%s%s\0", rel->name, INDEX_NAME_SUFFIX);
	storage_remove(filename);
	attr_ptr = list_head(rel->attributes);
	while (attr_ptr != NULL) {
		if (attr_ptr->index != NULL) {
			attr_ptr->index = NULL;
		}
		attr_ptr = attr_ptr->next;
	}
	free(filename);
}

static db_result_t generate_selection_result(db_handle_t **handle, relation_t *rel)
{
	relation_t *result_rel;
	unsigned attribute_count;
	attribute_t *attr;

	result_rel = (*handle)->result_rel;

	(*handle)->current_row = 0;
	(*handle)->ncolumns = 0;
	(*handle)->tuple_id = -1;

	for (attr = list_head(result_rel->attributes); attr != NULL; attr = attr->next) {
		if (attr->flags & ATTRIBUTE_FLAG_NO_STORE) {
			continue;
		}
		(*handle)->ncolumns++;
	}

	attribute_count = result_rel->attribute_count;

	/* Allocate attribute map and tuple row which is used in select operation */
	(*handle)->attr_map = (source_dest_map_t *)malloc(sizeof(source_dest_map_t) * attribute_count);
	if ((*handle)->attr_map == NULL) {
		DB_LOG_E("DB: Failed to malloc attr_map\n");
		return DB_ALLOCATION_ERROR;
	}

	if (DB_ERROR(generate_attribute_map((*handle)->attr_map, attribute_count, rel, result_rel))) {
		free((*handle)->attr_map);
		return DB_IMPLEMENTATION_ERROR;
	}

	if ((*handle)->lvm_instance != NULL) {
		/* Try to establish acceptable ranges for the attribute values. */
		if (!LVM_ERROR(lvm_derive((*handle)->lvm_instance))) {
			select_index(handle);
		}
	}

	(*handle)->tuple = (tuple_t)malloc(sizeof(char) * result_rel->row_length + 1);
	if ((*handle)->tuple == NULL) {
		DB_LOG_E("DB: Failed to malloc tuple row\n");
		free((*handle)->attr_map);
		return DB_ALLOCATION_ERROR;
	}

	/* Set flag to process tuples which need to be read */
	(*handle)->flags |= DB_HANDLE_FLAG_PROCESSING;

	return DB_OK;
}

db_result_t relation_process(db_handle_t **handle, db_cursor_t *cursor)
{
	uint32_t optype;
	if (handle == NULL || *handle == NULL) {
		return DB_ARGUMENT_ERROR;
	}
	optype = AQL_GET_EXEC_TYPE((*handle)->optype);
	switch (optype) {
	case AQL_TYPE_REMOVE_TUPLES:
		return relation_process_remove(handle, cursor);
	case AQL_TYPE_SELECT:
		return relation_process_select(handle, cursor);
	default:
		DB_LOG_E("DB: Invalid operation type: %d\n", optype);
		return DB_INCONSISTENCY_ERROR;
	}
}

/* Process tuples iteratively tuple by tuple  */
int db_processing_status(db_handle_t *handle)
{
	if (handle == NULL) {
		return DB_HANDLE_FLAG_INVALID;
	}
	return handle->flags & DB_HANDLE_FLAG_PROCESSING;
}

db_result_t relation_process_select(db_handle_t **handle, db_cursor_t *cursor)
{
	db_result_t result;
	unsigned attribute_count;
	source_dest_map_t *attr_map_ptr, *attr_map_end;
	attribute_t *result_attr, *from_attr;
	unsigned char *from_ptr, *to_ptr;
	char aggr_buf[8];
	attribute_value_t value;
	storage_row_t row = NULL;
	tuple_t result_row;

	if (cursor == NULL) {
		return DB_CURSOR_ERROR;
	}
	if ((*handle)->tuple == NULL) {
		return DB_ALLOCATION_ERROR;
	}

	result_row = (*handle)->tuple;
	attribute_count = (*handle)->result_rel->attribute_count;
	attr_map_end = (*handle)->attr_map + attribute_count;

	if ((*handle)->flags & DB_HANDLE_FLAG_SEARCH_INDEX) {
		(*handle)->tuple_id = index_get_next(&((*handle)->index_iterator), TRUE);
		if ((*handle)->tuple_id == INVALID_TUPLE) {
			DB_LOG_E("DB: An attribute value could not be found in the index\n");
			if ((*handle)->index_iterator.next_item_no == 0) {
				return DB_INDEX_ERROR;
			}
			if ((*handle)->adt_flags & AQL_FLAG_AGGREGATE) {
				result = DB_FINISHED;
				goto processing_aggregation;
			}
			return DB_FINISHED;
		}
	} else {
		(*handle)->tuple_id++;
	}

	row = (storage_row_t)malloc(sizeof(char) * (*handle)->rel->row_length + 1);
	if (row == NULL) {
		DB_LOG_E("DB: Failed to allocate row\n");
		return DB_ALLOCATION_ERROR;
	}

	/* Put the tuples fulfilling the- given condition into a new relation.
	   The tuples may be projected. */
	result = storage_get_row((*handle)->rel, &((*handle)->tuple_id), row);
	if (DB_ERROR(result)) {
		DB_LOG_E("DB: Failed to get a row in relation %s!\n", (*handle)->rel->name);
		goto errout;
	} else if (result == DB_FINISHED) {
		if ((*handle)->adt_flags & AQL_FLAG_AGGREGATE) {
			goto processing_aggregation;
		}
		if (row != NULL) {
			free(row);
		}
		return DB_FINISHED;
	}

	/* Process the attributes in the result relation. */
	for (attr_map_ptr = (*handle)->attr_map; attr_map_ptr < attr_map_end; attr_map_ptr++) {
		from_ptr = row + attr_map_ptr->from_offset;
		from_attr = attr_map_ptr->from_attr;

		if (from_attr->domain == DOMAIN_INT || from_attr->domain == DOMAIN_LONG) {
			lvm_set_operand_value((*handle)->lvm_instance, from_attr, from_ptr);
		}

		if (from_attr->flags & ATTRIBUTE_FLAG_NO_STORE) {
			/* The attribute is used just for the predicate,
			   so do not copy the current value into the result. */
			continue;
		}

		if (!((*handle)->adt_flags & AQL_FLAG_AGGREGATE)) {
			/* No aggregators. Copy the original value into the resulting tuple. */
			memcpy(result_row + attr_map_ptr->to_offset, from_ptr, from_attr->element_size);
		}
	}

	/* Check whether the given predicate is true for this tuple. */
	if ((*handle)->lvm_instance == NULL || lvm_execute((*handle)->lvm_instance) == TRUE) {
		(*handle)->current_row++;

		if ((*handle)->adt_flags & AQL_FLAG_AGGREGATE) {
			for (attr_map_ptr = (*handle)->attr_map; attr_map_ptr < attr_map_end; attr_map_ptr++) {
				from_ptr = row + attr_map_ptr->from_offset;
				result = db_phy_to_value(&value, attr_map_ptr->from_attr, from_ptr);
				if (DB_ERROR(result)) {
					goto errout;
				}

				result = aggregate(attr_map_ptr->to_attr, &value, (*handle)->current_row);
				if (DB_ERROR(result)) {
					goto errout;
				}
			}
		} else {
			result = cursor_data_add(cursor, (*handle)->tuple_id);
			if (DB_ERROR(result)) {
				goto errout;
			}
		}
	}

	if (row != NULL) {
		free(row);
	}
	return DB_OK;

processing_aggregation:
	/* Generate aggregated result if requested. */
	for (attr_map_ptr = (*handle)->attr_map; attr_map_ptr < attr_map_end; attr_map_ptr++) {
		result_attr = attr_map_ptr->to_attr;
		to_ptr = result_row + attr_map_ptr->to_offset;

		snprintf(aggr_buf, sizeof(aggr_buf), "%f", result_attr->aggregation_value);
		from_ptr = (unsigned char *)aggr_buf;
		memcpy(to_ptr, from_ptr, sizeof(aggr_buf));
	}

	/* Copy aggregated result to tuple in cursor */
	memcpy(cursor->tuple, result_row, sizeof(cursor->tuple));

	(*handle)->current_row = 0;
	(*handle)->adt_flags &= ~AQL_FLAG_AGGREGATE; /* Stop the aggregation. */

	result = cursor_data_add(cursor, (*handle)->current_row);
	if (DB_ERROR(result)) {
		goto errout;
	}
	cursor->total_rows = 1;

	if (row != NULL) {
		free(row);
	}
	return DB_FINISHED;

errout:
	if (row != NULL) {
		free(row);
	}

	return result;
}

db_result_t relation_process_remove(db_handle_t **handle, db_cursor_t *cursor)
{
	db_result_t result;
	unsigned attribute_count;
	unsigned char *from_ptr;
	attribute_t *from_attr;
	storage_row_t row = NULL;
	tuple_t result_row;
	source_dest_map_t *attr_map_ptr, *attr_map_end;
	char name[RELATION_NAME_LENGTH + 1];
	int i;

	if ((*handle)->tuple == NULL) {
		return DB_ALLOCATION_ERROR;
	}

	result_row = (*handle)->tuple;
	attribute_count = (*handle)->result_rel->attribute_count;
	attr_map_end = (*handle)->attr_map + attribute_count;

	/* Search all tuples sequentially without index. */
	(*handle)->tuple_id++;

	row = (storage_row_t)malloc(sizeof(char) * (*handle)->rel->row_length + 1);
	if (row == NULL) {
		DB_LOG_E("DB: Failed to allocate row\n");
		return DB_ALLOCATION_ERROR;
	}

	/* Put the tuples fulfilling the- given condition into a new relation.
	   The tuples may be projected. */
	result = storage_get_row((*handle)->rel, &((*handle)->tuple_id), row);
	if (DB_ERROR(result)) {
		DB_LOG_E("DB: Failed to get a row in relation %s!\n", (*handle)->rel->name);
		goto errout;
	} else if (result == DB_FINISHED) {
		goto end_removal;
	}

	/* Process the attributes in the result relation. */
	for (attr_map_ptr = (*handle)->attr_map; attr_map_ptr < attr_map_end; attr_map_ptr++) {
		from_ptr = row + attr_map_ptr->from_offset;
		from_attr = attr_map_ptr->from_attr;

		if (from_attr->domain == DOMAIN_INT || from_attr->domain == DOMAIN_LONG) {
			lvm_set_operand_value((*handle)->lvm_instance, from_attr, from_ptr);
		}

		if (from_attr->flags & ATTRIBUTE_FLAG_NO_STORE) {
			/* The attribute is used just for the predicate,
			   so do not copy the current value into the result. */
			continue;
		}

		memcpy(result_row + attr_map_ptr->to_offset, from_ptr, from_attr->element_size);
	}

	/* Check whether the given predicate is true for this tuple. */
	if ((*handle)->lvm_instance == NULL || lvm_execute((*handle)->lvm_instance) == FALSE) {
		result = storage_put_row((*handle)->result_rel, result_row, TRUE);
		if (DB_ERROR(result)) {
			DB_LOG_E("DB: Failed to store a row in the result relation!\n");
			goto errout;
		}

		(*handle)->current_row++;
		if (row != NULL) {
			free(row);
		}
		return DB_GOT_ROW;
	}

	if (row != NULL) {
		free(row);
	}
	return DB_OK;

end_removal:
	DB_LOG_E("DB: Finished removing tuples. Result relation has %d tuples\n", (*handle)->result_rel->cardinality);
	memcpy(name, (*handle)->rel->name, sizeof(name));
	relation_remove((*handle)->rel, 1);

	/* Rename the name of new relation to old relation */
	result = relation_rename((*handle)->result_rel->name, name);
	if (DB_ERROR(result)) {
		DB_LOG_E("DB: Failed to rename newly created relation\n");
		goto errout;
	}
	memcpy((*handle)->result_rel->name, (*handle)->rel->name, sizeof((*handle)->result_rel->name));

	/* Process finished, we allocate cursor for result of remove */
	if (DB_ERROR(cursor_init(&cursor, (*handle)->result_rel)) || DB_ERROR(cursor_data_set(cursor, (*handle)->attr_map, (*handle)->result_rel->attribute_count))) {
		DB_LOG_E("DB: Failed to init cursor and set cursor data\n");
		goto errout;
	}

	/* Add cursor tuple data to new relation created by remove operation for update. */
	for (i = 0; i < (*handle)->result_rel->cardinality; i++) {
		if (DB_ERROR(cursor_data_add(cursor, i))) {
			DB_LOG_E("DB: Failed to add cursor tuple data into new relation\n");
			goto errout;
		}
	}

	if (row != NULL) {
		free(row);
	}

	return DB_FINISHED;

errout:
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
	storage_write_buffer_clean();
#endif

	if (row != NULL) {
		free(row);
	}

	return result;
}

db_cursor_t *relation_process_result(db_handle_t *handler)
{
	db_result_t res;
	db_cursor_t *cursor;

	cursor = (db_cursor_t *)malloc(sizeof(db_cursor_t));
	if (cursor == NULL) {
		DB_LOG_E("DB: Failed to malloc cursor\n");
		return NULL;
	}
	memset(cursor, 0, sizeof(db_cursor_t));

	/* when SELECT, cursor row data is set in processing tuple by tuple.
	   So we need to initialize cursor and make cursor data before processing tuples */
	if (handler->optype == AQL_TYPE_SELECT) {
		if (DB_ERROR(cursor_init(&cursor, handler->rel)) || DB_ERROR(cursor_data_set(cursor, handler->attr_map, handler->result_rel->attribute_count))) {
			DB_LOG_E("DB: Failed to init cursor and set cursor data\n");
			cursor_deinit(cursor);
			return NULL;
		}
	}

	res = DB_ARGUMENT_ERROR;
	while (db_processing_status(handler)) {
		res = relation_process(&handler, cursor);
		if (DB_ERROR(res)) {
			DB_LOG_E("DB: Failed to process tuples : %d\n", res);
			cursor_deinit(cursor);
			return NULL;
		}
		switch (res) {
		case DB_FINISHED:
			DB_LOG_V("DB: Processing tuples is done!\n");
			return cursor;
		case DB_OK:
			continue;
		case DB_GOT_ROW:
			break;
		default:
			DB_LOG_E("[%d]\n", res);
			break;
		}
	}
	cursor_deinit(cursor);
	return NULL;
}

db_result_t relation_select(db_handle_t **handle, relation_t *rel, void *adt_ptr)
{
	aql_adt_t *adt;
	char *name;
	db_direction_t dir;
	char *attribute_name;
	attribute_t *attr, *attr_ptr;
	relation_t * res_rel;
	int i;
	int normal_attributes = 0;
	adt = (aql_adt_t *)adt_ptr;
	(*handle)->rel = rel;
	(*handle)->optype = AQL_GET_TYPE(adt);
	DB_LOG_D("relation_select... optype = %d\n", (*handle)->optype);
	(*handle)->adt_flags = AQL_GET_FLAGS(adt);
	(*handle)->lvm_instance = (lvm_instance_t *)adt->lvm_instance;

	if (AQL_GET_FLAGS(adt) & AQL_FLAG_ASSIGN) {
		name = adt->relations[0];
		dir = DB_STORAGE;
	} else {
		name = RESULT_RELATION;
		dir = DB_MEMORY;
	}

	res_rel = relation_load(name);
	relation_remove(res_rel, 1);
	relation_create(name, dir);
	(*handle)->result_rel = relation_load(name);

	if ((*handle)->result_rel == NULL) {
		DB_LOG_E("DB: Failed to load a relation for the query result\n");
		return DB_ALLOCATION_ERROR;
	}

	if (AQL_GET_FLAGS(adt) & AQL_FLAG_SELECT_ALL) {
		attr_ptr = list_head(rel->attributes);
		while (attr_ptr != NULL) {
			if (attr_ptr->flags & ATTRIBUTE_FLAG_INVALID) {
				DB_LOG_E("DB: Failed to add a result attribute\n");
				relation_release((*handle)->result_rel);
				return DB_ALLOCATION_ERROR;
			} else {
				index_load(rel, attr_ptr);
			}

			attr = relation_attribute_add((*handle)->result_rel, dir, attr_ptr->name, attr_ptr->domain, attr_ptr->element_size);
			if (attr == NULL) {
				DB_LOG_E("DB: Failed to add a result attribute\n");
				relation_release((*handle)->result_rel);
				return DB_ALLOCATION_ERROR;
			}
			attr_ptr = attr_ptr->next;
		}
	} else {
		for (i = normal_attributes; i < AQL_ATTRIBUTE_COUNT(adt); i++) {
			attribute_name = adt->attributes[i].name;

			attr = relation_attribute_get(rel, attribute_name);
			if (attr == NULL) {
				DB_LOG_E("DB: Select for invalid attribute %s in relation %s!\n", attribute_name, rel->name);
				return DB_NAME_ERROR;
			} else {
				index_load(rel, attr);
			}

			DB_LOG_D("DB: Found attribute %s in relation %s\n", attribute_name, rel->name);

			attr = relation_attribute_add((*handle)->result_rel, dir, attribute_name, adt->aggregators[i] ? DOMAIN_DOUBLE : attr->domain, adt->aggregators[i] ? sizeof(double) : attr->element_size);

			if (attr == NULL) {
				DB_LOG_E("DB: Failed to add a result attribute\n");
				relation_release((*handle)->result_rel);
				return DB_ALLOCATION_ERROR;
			}
			attr->aggregator = adt->aggregators[i];
			switch (attr->aggregator) {
			case AQL_NONE:
				if (!(adt->attributes[i].flags & ATTRIBUTE_FLAG_NO_STORE)) {
					/* Only count attributes projected into the result set. */
					normal_attributes++;
				}
				break;
			case AQL_MAX:
				attr->aggregation_value = LONG_MIN;
				break;
			case AQL_MIN:
				attr->aggregation_value = LONG_MAX;
				break;
			default:
				attr->aggregation_value = 0;
				break;
			}
			attr->flags = adt->attributes[i].flags;
		}
	}
	/* Preclude mixes of normal attributes and aggregated ones in
	   selection results. */
	if (normal_attributes > 0 && (*handle)->result_rel->attribute_count > normal_attributes) {
		return DB_RELATIONAL_ERROR;
	}

	return generate_selection_result(handle, rel);
}

tuple_id_t relation_cardinality(relation_t *rel)
{
	tuple_id_t tuple_id;

	if (rel->cardinality != INVALID_TUPLE) {
		return rel->cardinality;
	}

	if (!RELATION_HAS_TUPLES(rel)) {
		return 0;
	}

	if (DB_ERROR(storage_get_row_amount(rel, &tuple_id))) {
		return INVALID_TUPLE;
	}

	rel->cardinality = tuple_id;

	DB_LOG_D("DB: Relation %s has cardinality %lu\n", rel->name, (unsigned long)tuple_id);

	return tuple_id;
}
