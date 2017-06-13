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
 *      This component forwards index calls using the generic index
 *      API to specific implementations.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <arastorage/arastorage.h>
#include "attribute.h"
#include "db_options.h"
#include "db_debug.h"
#include "storage.h"
#include "memb.h"
#include "list.h"
#include "index.h"
#include "result.h"

/****************************************************************************
* Private Types
****************************************************************************/
static index_api_t *index_components[] = { &index_inline,
										   &index_bplustree
										 };

pthread_attr_t g_attr;

/****************************************************************************
 * Private function prototypes
 ****************************************************************************/
static index_api_t *find_index_api(index_type_t index_type);
db_result_t db_indexing(relation_t*);
LIST(indices);
MEMB(index_memb, index_t, DB_INDEX_POOL_SIZE);

/****************************************************************************
* Public Functions
****************************************************************************/
db_result_t index_init(void)
{
	int status;
	list_init(indices);

	if (list_head(indices) != NULL) {
		return DB_INDEX_ERROR;
	}
	memb_init(&index_memb);
	status = pthread_attr_init(&g_attr);

	if (status != 0) {
		DB_LOG_E("indexing_test: pthread_attr_init failed, status=%d\n", status);
	}
	return DB_OK;

}

db_result_t index_deinit(void)
{
	index_t *index = list_head(indices);
	while (index != NULL) {
		DB_LOG_D("Releasing index on rel: %s, attr :%s\n", index->rel->name, index->attr->name);
		if (DB_ERROR(index->api->release(index))) {
			return DB_INDEX_ERROR;
		}
		index = index->next;
	}
	if (DB_ERROR(index_init())) {
		return DB_INDEX_ERROR;
	}
	return DB_OK;
}

db_result_t index_create(index_type_t index_type, relation_t *rel, attribute_t *attr)
{
	tuple_id_t cardinality;
	index_t *index;
	index_api_t *api;

	cardinality = relation_cardinality(rel);
	if (cardinality == INVALID_TUPLE) {
		return DB_STORAGE_ERROR;
	}

	if (attr->index != NULL) {
		/* Refuse to overwrite the old index. */
		DB_LOG_E("DB: The attribute %s is already indexed\n", attr->name);
		return DB_INDEX_ERROR;
	}

	if (attr->domain != DOMAIN_INT && attr->domain != DOMAIN_LONG) {
		DB_LOG_E("DB: Cannot create an index for a non-number attribute!\n");
		return DB_INDEX_ERROR;
	}

	api = find_index_api(index_type);
	if (api == NULL) {
		DB_LOG_E("DB: No API for index type %d\n", (int)index_type);
		return DB_INDEX_ERROR;
	}

	index = memb_alloc(&index_memb);
	if (index == NULL) {
		DB_LOG_E("DB: Failed to allocate an index\n");
		return DB_ALLOCATION_ERROR;
	}

	index->rel = rel;
	index->attr = attr;
	index->api = api;
	index->state = INDEX_LOAD_NEEDED;
	index->opaque_data = NULL;
	index->descriptor_file[0] = '\0';
	index->type = index_type;

	if (DB_ERROR(api->create(index))) {
		index_release(index);
		DB_LOG_E("DB: Index-specific creation failed for attribute %s\n", attr->name);
		return DB_INDEX_ERROR;
	}

	attr->index = index;
	list_push(indices, index);

	if (index->descriptor_file[0] != '\0' && DB_ERROR(storage_put_index(index))) {
		index_destroy(index);
		DB_LOG_E("DB: Failed to store index data in file \"%s\"\n", index->descriptor_file);
		return DB_INDEX_ERROR;
	}

	if (!(api->flags & INDEX_API_INLINE) && cardinality > 0) {
		DB_LOG_D("DB: Created an index for an old relation; issuing a load request\n");
		if (DB_ERROR(db_indexing(rel))) {
			index_destroy(index);
			DB_LOG_E("DB: Failed to create index for an old relation %s.\n", rel->name);
			return DB_INDEX_ERROR;
		}
	} else {
		/* Inline indexes (i.e., those using the existing storage of the relation)
		   do not need to be reloaded after restarting the system. */
		DB_LOG_D("DB: Index created for attribute %s\n", attr->name);
	}

	index->state = INDEX_READY;
	return DB_OK;
}

db_result_t index_destroy(index_t *index)
{
	db_result_t res;
	int i;
	index_t *index_iter;
	if (DB_ERROR(index_release(index)) || DB_ERROR(index->api->destroy(index))) {
		return DB_INDEX_ERROR;
	}
	for (i = 0; i < index_memb.num; ++i) {
		if (index_memb.count[i] > 0) {
			index_iter = (index_t *)((char *)index_memb.mem + (i * index_memb.size));
			if ((strcmp(index->rel->name, index_iter->rel->name) == 0) && strcmp(index->attr->name, index_iter->attr->name) == 0) {
				index_memb.count[i] = 0;
				break;
			}
		}
	}
	list_remove(indices, index);
	if (index->descriptor_file[0] != '\0') {
		res = storage_remove(index->descriptor_file);
		if (DB_ERROR(res)) {
			return res;
		}
	}
	if (DB_ERROR(storage_remove_index(index->rel, index->attr))) {
		return DB_STORAGE_ERROR;
	}
	index->rel = NULL;
	index->attr->index = NULL;
	index->attr = NULL;
	return DB_OK;
}

db_result_t index_load(relation_t *rel, attribute_t *attr)
{
	index_t *index;
	index_api_t *api;

	DB_LOG_D("DB: Attempting to load an index over %s.%s\n", rel->name, attr->name);

	int i;
	bool found = false;
	for (i = 0; i < index_memb.num; ++i) {
		if (index_memb.count[i] > 0) {
			/* If this block was unused, we increase the reference count to
			   indicate that it now is used and return a pointer to the
			   memory block. */
			index = (index_t *)((char *)index_memb.mem + (i * index_memb.size));
			if ((strcmp(index->rel->name, rel->name) == 0) && strcmp(index->attr->name, attr->name) == 0) {
				found = true;
				index_memb.count[i]++;
				attr->index = index;
				index->rel = rel;
				index->attr = attr;
				index->state = INDEX_READY;
				break;
			}
		}
	}

	if (!found) {

		index = memb_alloc(&index_memb);
		if (index == NULL) {
			DB_LOG_E("DB: No more index objects available\n");
			return DB_ALLOCATION_ERROR;
		}

		if (DB_ERROR(storage_get_index(index, rel, attr))) {
			DB_LOG_E("DB: Failed load an index descriptor from storage\n");
			memb_free(&index_memb, index);
			return DB_INDEX_ERROR;
		}

		index->rel = rel;
		index->attr = attr;
		index->opaque_data = NULL;

		api = find_index_api(index->type);
		if (api == NULL) {
			DB_LOG_E("DB: No API for index type %d\n", index->type);
			return DB_INDEX_ERROR;
		}

		index->api = api;

		if (DB_ERROR(api->load(index))) {
			DB_LOG_E("DB: Index-specific load failed\n");
			index->rel = NULL;
			index->attr = NULL;
			memb_free(&index_memb, index);
			return DB_INDEX_ERROR;
		}
		list_add(indices, index);
		attr->index = index;
		index->state = INDEX_READY;
	}
	return DB_OK;
}

db_result_t index_release(index_t *index)
{
	memb_free(&index_memb, index);

	return DB_OK;
}

db_result_t index_insert(index_t *index, attribute_value_t *value, tuple_id_t tuple_id)
{
	return index->api->insert(index, value, tuple_id);
}

db_result_t index_delete(index_t *index, attribute_value_t *value)
{
	if (index->state != INDEX_READY) {
		return DB_INDEX_ERROR;
	}

	return index->api->delete(index, value);
}

db_result_t index_get_iterator(index_iterator_t *iterator, index_t *index, attribute_value_t *min_value, attribute_value_t *max_value)
{
	tuple_id_t cardinality;
	unsigned long range;
	unsigned long max_range;
	long max;
	long min;

	cardinality = relation_cardinality(index->rel);
	if (cardinality == INVALID_TUPLE) {
		return DB_STORAGE_ERROR;
	}

	if (index->state != INDEX_READY) {
		return DB_INDEX_ERROR;
	}

	min = db_value_to_long(min_value);
	max = db_value_to_long(max_value);
	DB_LOG_D("DB : Index_get_iterator min = %ld, max = %ld\n", min, max);
	range = (unsigned long)max - min;
	if (range > 0) {
		/*
		 * Index structures that do not have a natural ability to handle
		 * range queries (e.g., a hash index) can nevertheless emulate them.
		 *
		 * The range query emulation attempts to look up the key for each
		 * value in the search range. If the search range is sparse, this
		 * iteration will incur a considerable overhead per found key.
		 *
		 * Hence, the emulation is preferable when an external module wants
		 * to iterate over a narrow range of keys, for which the total
		 * search cost is smaller than that of an iteration over all tuples
		 * in the relation.
		 */
		if (!(index->api->flags & INDEX_API_RANGE_QUERIES)) {
			DB_LOG_D("DB: Range query requested for an index that does not support it\n");
			max_range = cardinality / DB_INDEX_COST;
			DB_LOG_D("DB: Range query requested for an index that does not support it max_range = %d cardinality = %d range = %d\n", max_range, cardinality, range);
			if (range > max_range) {
				return DB_INDEX_ERROR;
			}
			DB_LOG_D("DB: Using the index anyway because the range is small enough (%lu <= %lu)\n", range, max_range);
		}
	}

	iterator->index = index;
	iterator->min_value = *min_value;
	iterator->max_value = *max_value;
	iterator->next_item_no = 0;

	DB_LOG_D("DB: Acquired an index iterator for %s.%s over the range (%ld,%ld)\n", index->rel->name, index->attr->name, min_value->u.long_value, max_value->u.long_value);

	return DB_OK;
}

tuple_id_t index_get_next(index_iterator_t *iterator, uint8_t matched_condition)
{
	long min;
	long max;

	if (iterator->index == NULL) {
		/* This attribute is not indexed. */
		return INVALID_TUPLE;
	}

	if ((iterator->index->attr->flags & ATTRIBUTE_FLAG_UNIQUE) && iterator->next_item_no == 1) {
		min = db_value_to_long(&iterator->min_value);
		max = db_value_to_long(&iterator->max_value);
		if (min == max) {
			/*
			 * We stop if this is an equivalence search on an attribute
			 * whose values are unique, and we already found one item.
			 */
			DB_LOG_E("DB: Equivalence search finished\n");
			return INVALID_TUPLE;
		}
	}

	return iterator->index->api->get_next(iterator, matched_condition);
}

/****************************************************************************
* Private Functions
****************************************************************************/
static index_api_t *find_index_api(index_type_t index_type)
{
	int i;

	for (i = 0; i < sizeof(index_components) / sizeof(index_components[0]); i++) {
		if (index_components[i]->type == index_type) {
			return index_components[i];
		}
	}

	return NULL;
}

static index_t *get_next_index_to_load(void)
{
	index_t *index;

	for (index = list_head(indices); index != NULL; index = index->next) {
		if (index->state == INDEX_LOAD_NEEDED) {
			return index;
		}
	}

	return NULL;
}

db_result_t db_indexing(relation_t *rel)
{
	index_t *index;
	tuple_id_t tuple_id;
	tuple_id_t cardinality;
	storage_row_t row;
	attribute_value_t value;
	attribute_t *attr;
	db_result_t result;
	int offset;
	bool isfound;

	index = get_next_index_to_load();
	if (index == NULL) {
		DB_LOG_E("DB: Request to load an index, but no index is set to be loaded\n");
		return DB_INDEX_ERROR;
	}

	row = NULL;
	row = (storage_row_t)malloc(sizeof(char) * rel->row_length + 1);
	if (row == NULL) {
		DB_LOG_E("DB: Failed to allocate row\n");
		return DB_ALLOCATION_ERROR;
	}
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
	storage_flush_insert_buffer();
#endif
	DB_LOG_D("DB: Loading the index for %s.%s...\n", index->rel->name, index->attr->name);

	offset  = 0;
	isfound = false;

	attr = list_head(rel->attributes);
	while (attr != NULL) {
		if (strcmp(attr->name, index->attr->name) == 0) {
			DB_LOG_D("DB: Found attribute %s in %s\n", index->attr->name, rel->name);
			isfound = true;
			break;
		}
		offset += attr->element_size;
		attr = attr->next;
	}

	if (!isfound) {
		DB_LOG_E("DB: Failed to find attribute %s in %s\n", index->attr->name, rel->name);
		goto errout;
	}

	cardinality = relation_cardinality(rel);

	for (tuple_id = 0; tuple_id < cardinality; tuple_id++) {
		memset(row, 0, sizeof(row));
		DB_LOG_V("DB: Indexing Tuple id %d\n", tuple_id);
		result = storage_get_row(rel, &tuple_id, row);
		if (DB_ERROR(result)) {
			DB_LOG_E("DB: Failed to get a row in relation %s!\n", rel->name);
			goto errout;
		}

		result = db_phy_to_value(&value, index->attr, row + offset);
		if (DB_ERROR(result)) {
			DB_LOG_E("DB: Failed to get value from row\n");
			goto errout;
		}

		if (DB_ERROR(index_insert(index, &value, tuple_id))) {
			DB_LOG_E("DB: Failed to get a row in relation %s!\n", rel->name);
			goto errout;
		}
	}

	free(row);
	DB_LOG_D("DB: Loaded %lu rows into the index\n", cardinality);

	return DB_OK;

errout:
	if (row != NULL) {
		free(row);
	}

	return DB_INDEX_ERROR;
}
