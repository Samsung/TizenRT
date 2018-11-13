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
 *      Main functions for Antelope, a DBMS for sensor devices.
 *
 *      Antelope is described and evaluated in the paper A Database in
 *      Every Sensor, N. Tsiftes and A. Dunkels, in Proceedings of
 *      ACM SenSys 2011.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>

#include "db_debug.h"
#include "result.h"
#include "aql.h"
#include <arastorage/arastorage.h>

/****************************************************************************
* Public Functions
****************************************************************************/
db_result_t db_init(void)
{
	db_result_t res;
	res = relation_init();
	if (res != DB_OK) {
		return res;
	}
	res = index_init();
	if (res != DB_OK) {
		return res;
	}
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
	res = storage_write_buffer_init();
	if (res != DB_OK) {
		return res;
	}
#endif
	return res;
}

db_result_t db_deinit()
{
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
	storage_write_buffer_deinit();
#endif
	relation_deinit();
	index_deinit();
	return DB_OK;
}

void db_set_output_function(db_output_function_t f)
{
	output = f;
}

const char *db_get_result_message(db_result_t code)
{
	switch (code) {
	case DB_FINISHED:
		return DB_FINISHED_MSG;
	case DB_OK:
		return DB_OK_MSG;
	case DB_LIMIT_ERROR:
		return DB_LIMIT_ERROR_MSG;
	case DB_ALLOCATION_ERROR:
		return DB_ALLOCATION_ERROR_MSG;
	case DB_STORAGE_ERROR:
		return DB_STORAGE_ERROR_MSG;
	case DB_PARSING_ERROR:
		return DB_PARSING_ERROR_MSG;
	case DB_NAME_ERROR:
		return DB_NAME_ERROR_MSG;
	case DB_RELATIONAL_ERROR:
		return DB_RELATIONAL_ERROR_MSG;
	case DB_TYPE_ERROR:
		return DB_TYPE_ERROR_MSG;
	case DB_IMPLEMENTATION_ERROR:
		return DB_IMPLEMENTATION_ERROR_MSG;
	case DB_INDEX_ERROR:
		return DB_INDEX_ERROR_MSG;
	case DB_BUSY_ERROR:
		return DB_BUSY_ERROR_MSG;
	case DB_INCONSISTENCY_ERROR:
		return DB_INCONSISTENCY_ERROR_MSG;
	case DB_ARGUMENT_ERROR:
		return DB_ARGUMENT_ERROR_MSG;
	case DB_FULL_ERROR:
		return DB_FULL_ERROR_MSG;
	case DB_CURSOR_ERROR:
		return DB_CURSOR_ERROR_MSG;
	case DB_EMPTY_CURSOR_ERROR:
		return DB_EMPTY_CURSOR_ERROR_MSG;
	default:
		return DB_UNKNOWN_ERROR_MSG;
	};
}

/* Print the related information which will be printed : relation, attribute name */
db_result_t db_print_header(db_cursor_t *cursor)
{

	int column;
	if (cursor == NULL) {
		return DB_ARGUMENT_ERROR;
	}
	output("[relation = %s, attributes = (", cursor->rel_name);
	for (column = 0; column < cursor->attribute_count; column++) {
		output("%s%s", column > 0 ? ", " : "", cursor->attr_map[column].name);
	}
	output(")]\n");
	return DB_OK;

}

db_result_t db_cursor_free(db_cursor_t *cursor)
{
	return cursor_deinit(cursor);
}


/* Print tuple value */
db_result_t db_print_tuple(db_cursor_t *cursor)
{
	int column;
	attribute_value_t value;

	if (IS_EMPTY_CURSOR(cursor)) {
		return DB_CURSOR_ERROR;
	}

	if (IS_INVALID_CURSOR_ROW(cursor)) {
		DB_LOG_E("invalid cursor row id\n");
		return DB_CURSOR_ERROR;
	}

	output("Row %lu:\t", (unsigned long)cursor->current_cursor_row);

	for (column = 0; column < cursor->attribute_count; column++) {
		if (DB_ERROR(cursor_get_value_storage(&value, cursor, column))) {
			return DB_CURSOR_ERROR;
		}
		switch (value.domain) {
		case DOMAIN_STRING:
			output("\"%s\"\t", VALUE_STRING(&value));
			break;
		case DOMAIN_INT:
			output("%d\t", (int)VALUE_INT(&value));
			break;
		case DOMAIN_LONG:
			output("%ld\t", (long)VALUE_LONG(&value));
			break;
		case DOMAIN_DOUBLE:
			output("%.3f\t", (double)VALUE_DOUBLE(&value));
			break;
		default:
			output("\nUnrecognized domain: %d\n", value.domain);
			return DB_IMPLEMENTATION_ERROR;
		}
	}
	output("\n");
	return DB_OK;
}

/* print (index)th attribute value of current row */
db_result_t db_print_value(db_cursor_t *cursor, int attr_index)
{
	attribute_value_t value;

	if (IS_EMPTY_CURSOR(cursor)) {
		return DB_CURSOR_ERROR;
	}

	if (IS_INVALID_CURSOR_ROW(cursor)) {
		DB_LOG_E("invalid cursor row id\n");
		return DB_CURSOR_ERROR;
	}

	output("Row %lu:\t", (unsigned long)cursor->current_cursor_row);

	if (DB_ERROR(cursor_get_value_storage(&value, cursor, attr_index))) {
		return DB_CURSOR_ERROR;
	}

	switch (value.domain) {
	case DOMAIN_STRING:
		output("\"%s\"\t", VALUE_STRING(&value));
		break;
	case DOMAIN_INT:
		output("%d\t", (int)VALUE_INT(&value));
		break;
	case DOMAIN_LONG:
		output("%ld\t", (long)VALUE_LONG(&value));
		break;
	case DOMAIN_DOUBLE:
		output("%.3f\t", (double)VALUE_DOUBLE(&value));
		break;
	default:
		output("\nUnrecognized domain: %d\n", value.domain);
		return DB_IMPLEMENTATION_ERROR;
	}

	output("\n");
	return DB_OK;
}
