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
 ******************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "result.h"
#include "db_debug.h"
#include "storage.h"
#include "relation.h"

/****************************************************************************
* Public Functions
****************************************************************************/

/* Update current cursor id and storage id. */
db_result_t cursor_move_to(db_cursor_t *cursor, tuple_id_t row_id)
{
	if (IS_EMPTY_CURSOR(cursor)) {
		DB_LOG_E("Empty Cursor\n");
		return DB_CURSOR_ERROR;
	}

	if (row_id >= DB_CURSOR_RESULT_ENTRY || row_id >= cursor->cursor_rows) {
		DB_LOG_E("invalid row id\n");
		return DB_CURSOR_ERROR;
	}

	int i, index, pos;
	int cnt = 0;

	/* search (row_id)th set tuple id */
	for (i = 0; i < cursor->total_rows; i++) {
		index = GET_INDEX(i);
		pos = GET_POS(i);

		if (BIT_CHECK(cursor->row_arr[index], pos)) {
			if (cnt == row_id) {
				cursor->current_cursor_row = row_id;
				cursor->current_storage_row = i;
				DB_LOG_D("set current cursor id = %d, storage id = %d\n", cursor->current_cursor_row, cursor->current_storage_row);
				return DB_OK;
			}
			cnt++;
		}
	}
	return DB_CURSOR_ERROR;
}

/* Search the first set tuple id and update storage id corresponding it. */
db_result_t cursor_move_first(db_cursor_t *cursor)
{
	return cursor_move_to(cursor, 0);
}

/* Search the last set tuple id and update storage id corresponding it. */
db_result_t cursor_move_last(db_cursor_t *cursor)
{
	if (!cursor) {
		return DB_CURSOR_ERROR;
	}

	return cursor_move_to(cursor, cursor->cursor_rows - 1);
}

/* Search next set tuple id and update storage id corresponding it. */
db_result_t cursor_move_next(db_cursor_t *cursor)
{
	if (!cursor) {
		return DB_CURSOR_ERROR;
	}

	return cursor_move_to(cursor, cursor->current_cursor_row + 1);
}

/* Search previous set tuple id and update storage id corresponding it. */
db_result_t cursor_move_prev(db_cursor_t *cursor)
{
	if (!cursor) {
		return DB_CURSOR_ERROR;
	}

	return cursor_move_to(cursor, cursor->current_cursor_row - 1);
}

/* get current row of cursor data */
cursor_row_t cursor_get_row(db_cursor_t *cursor)
{
	if (cursor == NULL) {
		return INVALID_CURSOR_VALUE;
	}
	return cursor->current_cursor_row;
}

/* Check whether cursor is pointing the first row*/
bool cursor_is_first_row(db_cursor_t *cursor)
{
	int i, index, pos;

	if (cursor == NULL) {
		return false;
	}
	//check whether pointing cursor id is correct
	if (cursor->current_cursor_row != 0) {
		return false;
	}
	//check whether pointing storage row id is true
	for (i = 0; i < cursor->total_rows; i++) {
		index = GET_INDEX(i);
		pos = GET_POS(i);

		if (BIT_CHECK(cursor->row_arr[index], pos)) {
			if (i == cursor->current_storage_row) {
				return true;
			}
			return false;
		}
	}
	return false;
}

/* Check whether cursor is pointing the last row*/
bool cursor_is_last_row(db_cursor_t *cursor)
{
	if (cursor == NULL) {
		return false;
	}
	//check whether pointing cursor id is correct
	if (cursor->current_cursor_row != cursor->cursor_rows - 1) {
		return false;
	}
	//check whether pointing storage row id is true
	int i, index, pos;

	for (i = cursor->total_rows - 1; i >= 0; i--) {
		index = GET_INDEX(i);
		pos = GET_POS(i);

		if (BIT_CHECK(cursor->row_arr[index], pos)) {
			if (i == cursor->current_storage_row) {
				return true;
			}
			return false;
		}
	}
	return false;
}

db_result_t cursor_data_add(db_cursor_t *cursor, tuple_id_t tuple_id)
{
	if (cursor == NULL) {
		DB_LOG_E("cursor is null\n");
		return DB_CURSOR_ERROR;
	}

	if (tuple_id >= DB_CURSOR_RESULT_ENTRY || tuple_id >= cursor->total_rows) {
		DB_LOG_E("invalid tuple id error\n");
		return DB_CURSOR_ERROR;
	}

	int index = GET_INDEX(tuple_id);
	int pos = GET_POS(tuple_id);

	BIT_SET(cursor->row_arr[index], pos);
	cursor->cursor_rows++;

	DB_LOG_D("cursor data added successfully : id %d, cardinality %d\n", tuple_id, cursor->cursor_rows);

	return DB_OK;
}

/* Get the number of tuples in a cursor */
cursor_row_t cursor_get_count(db_cursor_t *cursor)
{
	if (IS_EMPTY_CURSOR(cursor)) {
		return INVALID_CURSOR_VALUE;
	}

	return cursor->cursor_rows;
}

/* Get a domain of specific attribute(column). */
domain_t cursor_get_attr_type(db_cursor_t *cursor, int attr_index)
{
	if (cursor == NULL) {
		return INVALID_CURSOR_VALUE;
	}

	if (attr_index < 0 || attr_index >= cursor->attribute_count) {
		DB_LOG_E("invalid column index\n");
		return INVALID_CURSOR_VALUE;
	}

	return cursor->attr_map[attr_index].domain;
}

/* Get a name of specific attribute */
char *cursor_get_attr_name(db_cursor_t *cursor, int attr_index)
{
	if (cursor == NULL) {
		return NULL;
	}

	if (attr_index < 0 || attr_index >= cursor->attribute_count) {
		DB_LOG_E("invalid column index!\n");
		return NULL;
	}

	return cursor->attr_map[attr_index].name;
}

/* Get a index of specific attribute. */
attribute_id_t cursor_get_attr_index(db_cursor_t *cursor, const char *attr_name)
{
	int i;

	if (cursor == NULL || attr_name == NULL) {
		return INVALID_CURSOR_VALUE;
	}

	for (i = 0; i < cursor->attribute_count; i++) {
		if (strcmp(cursor->attr_map[i].name, attr_name) == 0) {
			return i;
		}
	}
	return INVALID_CURSOR_VALUE;
}

db_result_t cursor_get_value_storage(attribute_value_t *value, db_cursor_t *cursor, unsigned col)
{
	int fd, offset;
	attribute_t attr;
	unsigned char *buf;

	if (col >= cursor->attribute_count) {
		DB_LOG_E("DB: Requested value (%d) is out of bounds; max = (%d)\n", col, cursor->attribute_count);
		return DB_CURSOR_ERROR;
	}

	if (IS_INVALID_STORAGE_ROW(cursor)) {
		DB_LOG_E("invalid storage row id\n");
		return DB_CURSOR_ERROR;
	}

	buf = cursor->tuple;

	memcpy(attr.name, cursor->attr_map[col].name, sizeof(attr.name));
	attr.domain = cursor->attr_map[col].domain;
	attr.element_size = cursor->attr_map[col].data_size;

	if (cursor->attr_map[col].valuetype == AGGREGATE_VALUE) {
		/* If the type of value is aggregate value, we don't need to read storage.
		 Because aggregate result is already calculated and stored in buffer. */
		buf += cursor->attr_map[col].offset;
	} else {
		/* Otherwise, Read tuple value from storage. */
		offset = cursor->current_storage_row * cursor->storage_row_length + cursor->attr_map[col].offset;
		fd = storage_open(cursor->name, O_RDONLY);
		if (fd < 0) {
			DB_LOG_E("failed to open storage %s\n", cursor->name);
			return DB_CURSOR_ERROR;
		}
		storage_read_from(fd, buf, offset, attr.element_size);
		storage_close(fd);
	}

	return db_phy_to_value(value, &attr, buf);
}

db_result_t cursor_get_value(db_cursor_t *cursor, int attr_index, attribute_value_t *value, domain_t domain)
{
	if (IS_INVALID_CURSOR_ROW(cursor)) {
		DB_LOG_E("invalid cursor row id\n");
		return DB_CURSOR_ERROR;
	}

	if (cursor->attr_map[attr_index].domain != domain) {
		DB_LOG_E("No Matched type of value, domain : %d\n", domain);
		return DB_CURSOR_ERROR;
	}

	if (DB_ERROR(cursor_get_value_storage(value, cursor, attr_index))) {
		DB_LOG_E("Failed to get value from storage\n");
		return DB_CURSOR_ERROR;
	}

	return DB_OK;
}

/* get int value of (index)th attribute */
int cursor_get_int_value(db_cursor_t *cursor, int attr_index)
{
	attribute_value_t value;

	if (DB_ERROR(cursor_get_value(cursor, attr_index, &value, DOMAIN_INT))) {
		DB_LOG_E("Faild to get int value\n");
		return DB_INT_ERROR;
	}

	return (int)VALUE_INT(&value);
}

/* get long value of (index)th attribute */
long cursor_get_long_value(db_cursor_t *cursor, int attr_index)
{
	attribute_value_t value;

	if (DB_ERROR(cursor_get_value(cursor, attr_index, &value, DOMAIN_LONG))) {
		DB_LOG_E("Faild to get long value\n");
		return DB_LONG_ERROR;
	}

	return (long)VALUE_LONG(&value);
}

#ifdef CONFIG_ARCH_FLOAT_H
/* get double value of (index)th attribute */
double cursor_get_double_value(db_cursor_t *cursor, int attr_index)
{
	attribute_value_t value;

	if (DB_ERROR(cursor_get_value(cursor, attr_index, &value, DOMAIN_DOUBLE))) {
		DB_LOG_E("Faild to get double value\n");
		return DB_DOUBLE_ERROR;
	}

	return (double)VALUE_DOUBLE(&value);
}
#endif

/* get string value of (index)th attribute */
unsigned char *cursor_get_string_value(db_cursor_t *cursor, int attr_index)
{
	attribute_value_t value;

	if (DB_ERROR(cursor_get_value(cursor, attr_index, &value, DOMAIN_STRING))) {
		DB_LOG_E("Faild to get string value\n");
		return NULL;
	}

	return (unsigned char *)VALUE_STRING(&value);
}

db_result_t cursor_data_set(db_cursor_t *cursor, source_dest_map_t *attr_map, attribute_id_t attribute_count)
{
	source_dest_map_t *attr_map_ptr;
	int i;

	if (cursor == NULL) {
		DB_LOG_E("DB: Invalid cursor\n");
		return DB_CURSOR_ERROR;
	}

	if (attr_map == NULL) {
		DB_LOG_E("DB: Invalid attribute map\n");
		return DB_ARGUMENT_ERROR;
	}

	attr_map_ptr = attr_map;

	for (i = 0; i < attribute_count; i++) {
		memset(cursor->attr_map[i].name, 0, sizeof(cursor->attr_map[i].name));
		memcpy(cursor->attr_map[i].name, attr_map_ptr->to_attr->name, sizeof(attr_map_ptr->to_attr->name));
		cursor->attr_map[i].domain = attr_map_ptr->to_attr->domain;
		cursor->attr_map[i].valuetype = attr_map_ptr->valuetype;
		if (cursor->attr_map[i].valuetype == AGGREGATE_VALUE) {
			cursor->attr_map[i].data_size = attr_map_ptr->to_attr->element_size;
			cursor->attr_map[i].offset = attr_map_ptr->to_offset;
		} else {
			cursor->attr_map[i].data_size = attr_map_ptr->from_attr->element_size;
			cursor->attr_map[i].offset = attr_map_ptr->from_offset;
		}
		attr_map_ptr++;
	}

	cursor->attribute_count = attribute_count;

	return DB_OK;
}

void cursor_clean_data(db_cursor_t *cursor)
{
	memset(cursor->name, 0, sizeof(cursor->name));
	cursor->current_cursor_row = -1;
	cursor->current_storage_row = -1;
	cursor->cursor_rows = 0;
	cursor->total_rows = 0;
	cursor->attribute_count = 0;
	cursor->storage_row_length = 0;
	if (cursor->row_arr != NULL) {
		free(cursor->row_arr);
	}
	cursor->row_arr = NULL;
}

db_result_t cursor_init(db_cursor_t **cursor, relation_t *rel)
{
	int arr_size;
	if (*cursor == NULL) {
		return DB_CURSOR_ERROR;
	}

	cursor_clean_data(*cursor);

	arr_size = GET_CURSOR_DATA_ARR_SIZE(rel->cardinality);
	if (arr_size > DB_CURSOR_LIMIT) {
		return DB_CURSOR_ERROR;
	}

	(*cursor)->row_arr = (uint32_t *)malloc(sizeof(uint32_t) * arr_size);
	if ((*cursor)->row_arr == NULL) {
		return DB_CURSOR_ERROR;
	}
	memset((*cursor)->row_arr, 0, arr_size * sizeof(uint32_t));
	(*cursor)->total_rows = rel->cardinality;

	(*cursor)->storage_row_length = rel->row_length;
	memcpy((*cursor)->name, rel->tuple_filename, sizeof(rel->tuple_filename));
	memcpy((*cursor)->rel_name, rel->name, sizeof(rel->name));

	return DB_OK;
}

db_result_t cursor_deinit(db_cursor_t *cursor)
{
	if (cursor == NULL) {
		return DB_CURSOR_ERROR;
	}
	if (cursor->row_arr) {
		free(cursor->row_arr);
		cursor->row_arr = NULL;
	}
	free(cursor);
	return DB_OK;
}
