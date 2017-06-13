/****************************************************************************
*
* Copyright 2016 Samsung Electronics All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
* either express or implied. See the License for the specific
* language governing permissions and limitations under the License.
*
****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <arastorage/arastorage.h>
#include <apps/shell/tash.h>
#include <tinyara/fs/fs_utils.h>
#include "../../../tc_common.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#ifndef CONFIG_MOUNT_POINT
#define CONFIG_MOUNT_POINT "/mnt"
#endif

#define RELATION_NAME "rel"
#define RELATION_NAME_QUERY "relname"
#define RELATION_INDEX "bplustree"
#define QUERY_LENGTH 128

#define DATA_SET_NUM 10

/****************************************************************************
*  Global Variables
 ****************************************************************************/
static db_cursor_t *g_cursor;

const static char *g_attribute_set[] = { "id", "date", "fruit", "weight" };

struct arastorage_data_type_s {
	int int_value;
	long long_value;
	char *string_value;
	double double_value;
};

typedef struct arastorage_data_type_s arastorage_data_t;

const static arastorage_data_t g_arastorage_data_set[DATA_SET_NUM] = {
	{ 1, 20160101, "apple",       1.0001},
	{ 2, 20160102, "banana",      2.0002},
	{ 3, 20160103, "mango",       3.0003},
	{ 4, 20160104, "orange",      4.0004},
	{ 5, 20160105, "melon",       5.0005},
	{ 6, 20160106, "kiwi",        6.0006},
	{ 7, 20160107, "peach",       7.0007},
	{ 8, 20160108, "cherry",      8.0008},
	{ 9, 20160109, "strawberry",  9.0009},
	{10, 20160110, "watermelon", 10.0010}
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

char g_query[QUERY_LENGTH];
bool g_check;
bool g_init;

/**
* @testcase             itc_arastorage_startup_p
* @brief                To initialize database resources
* @scenario             If used api returns DB_OK
* @apicovered           db_init , db_exec
* @precondition         NA
* @postcondition        NA
*/
void itc_arastorage_startup_p(void)
{
	db_result_t ret;
	int index;
	g_check = false;
	g_init = false;
	ret = db_init();
	TC_ASSERT_EQ("db_init", ret, DB_OK);
	g_init = true;

	snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s;", RELATION_NAME);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN int IN %s;", g_attribute_set[0], RELATION_NAME);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN long IN %s;", g_attribute_set[1], RELATION_NAME);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN string(32) IN %s;", g_attribute_set[2], RELATION_NAME);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	memset(g_query, 0, QUERY_LENGTH);
	sprintf(g_query, "CREATE INDEX %s.%s TYPE %s;", RELATION_NAME, g_attribute_set[0], RELATION_INDEX);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

#ifdef CONFIG_ARCH_FLOAT_H
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN double IN %s;", g_attribute_set[3], RELATION_NAME);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, 0);

	for (index = 0; index < DATA_SET_NUM; index++) {
		memset(g_query, 0, QUERY_LENGTH);
		snprintf(g_query, QUERY_LENGTH, "INSERT (%d, %ld, \'%s\', %f) INTO %s;", g_arastorage_data_set[index].int_value,
				   g_arastorage_data_set[index].long_value, g_arastorage_data_set[index].string_value,
				   g_arastorage_data_set[index].double_value, RELATION_NAME);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, 0);
	}
#else
	for (index = 0; index < DATA_SET_NUM; index++) {
		memset(g_query, 0, QUERY_LENGTH);
		snprintf(g_query, QUERY_LENGTH, "INSERT (%d, %ld, \'%s\') INTO %s;", g_arastorage_data_set[index].int_value,
						 g_arastorage_data_set[index].long_value, g_arastorage_data_set[index].string_value,
						 RELATION_NAME);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, DB_OK);
	}
	g_check = true;
	TC_SUCCESS_RESULT();
#endif
}

/**
* @testcase             itc_arastorage_cleanup_p
* @brief                To de-initialize database resources
* @scenario             If used api returns DB_OK
* @apicovered           db_deinit
* @precondition         NA
* @postcondition        NA
*/
void itc_arastorage_cleanup_p(void)
{
	if (g_init) {
		db_deinit();
	}
}

/**
* @testcase             itc_arastorage_db_init_deinit_p
* @brief                To check  initialize and de-initialize database resources
* @scenario             If used api returns DB_OK
* @apicovered           db_deinit  , db_init
* @precondition         NA
* @postcondition        NA
*/
void itc_arastorage_db_init_deinit_p(void)
{
	db_result_t ret;

	ret = db_init();
	TC_ASSERT_EQ("db_init", ret, DB_OK);

	ret = db_deinit();
	TC_ASSERT_EQ("db_deinit", ret, DB_OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_get_result_message_p
* @brief                To check get string of each API's result based on value of db_result_t
* @scenario             If used api returns DB_OK
* @apicovered           db_get_result_message
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_db_get_result_message_p(void)
{
	db_result_t ret;

	ret = strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_FINISHED));
	TC_ASSERT_NEQ("db_get_result_message : DB_FINISHED", ret, 0);

	TC_ASSERT_NEQ("db_get_result_message : DB_OK", ret, 0);

	TC_ASSERT_NEQ("db_get_result_message : DB_LIMIT_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_ALLOCATION_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_STORAGE_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_RELATIONAL_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_TYPE_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_IMPLEMENTATION_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_INDEX_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_NAME_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_BUSY_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_INCONSISTENCY_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_ARGUMENT_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_FULL_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_CURSOR_ERROR", ret, 0);
	TC_ASSERT_NEQ("db_get_result_message : DB_EMPTY_CURSOR_ERROR", ret, 0);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @testcase             itc_arastorage_db_print_header_p
* @brief                To check print of the related information : relation, attribute name
* @scenario             If used api returns DB_OK
* @apicovered           db_print_header , db_query
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_db_print_header_p(void)
{
	db_result_t ret;

	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NOT_NULL("db_query", g_cursor);
	ret = db_print_header(g_cursor);
	TC_ASSERT_EQ_CLEANUP("db_print_header", DB_ERROR(ret), 0, "db_print_header failed\n", db_cursor_free(g_cursor));
	db_cursor_free(g_cursor);
	g_cursor = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_print_tuple_p
* @brief                To check print the tuple data
* @scenario             If used api returns DB_OK
* @apicovered           db_print_tuple , db_query
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_db_print_tuple_p(void)
{
	db_result_t ret;

#ifdef CONFIG_ARCH_FLOAT_H
	snprintf(query, QUERY_LENGTH, "SELECT %s, %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], g_attribute_set[3], RELATION_NAME, g_attribute_set[0]);
#else
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME, g_attribute_set[0]);
#endif
	g_cursor = db_query(g_query);
	TC_ASSERT_NOT_NULL("db_query", g_cursor);

	if (DB_SUCCESS(cursor_move_first(g_cursor))) {
		do {
			ret = db_print_tuple(g_cursor);
			TC_ASSERT_EQ("db_print_tuple", DB_ERROR(ret), 0);
		} while (DB_SUCCESS(cursor_move_next(g_cursor)));
	}
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_print_value_p
* @brief                To check print data value
* @scenario             If used api returns DB_OK
* @apicovered           db_print_value , cursor_move_first
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_db_print_value_p(void)
{
	db_result_t ret;

	if (DB_SUCCESS(cursor_move_first(g_cursor))) {
		do {
			ret = db_print_value(g_cursor, 2);
			TC_ASSERT_EQ("db_print_value", DB_ERROR(ret), 0);
		} while (DB_SUCCESS(cursor_move_next(g_cursor)));
	}
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_count_p
* @brief                To check get count of total row of cursor
* @scenario             If used api returns DB_OK
* @apicovered           cursor_get_count
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_count_p(void)
{
	cursor_row_t count;

	count = cursor_get_count(g_cursor);
	printf("cursor count : %d\n", count);
	TC_ASSERT_GEQ("cursor_get_count", count, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_first_p
* @brief                To check move current position of cursor to first row
* @scenario             If used api returns success
* @apicovered           cursor_move_first , cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_move_first_p(void)
{
	db_result_t ret;
	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_next_p
* @brief                To check move current position of cursor to next row
* @scenario             If used api returns success
* @apicovered           cursor_move_next , cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_move_next_p(void)
{
	db_result_t ret;
	ret = cursor_move_next(g_cursor);
	TC_ASSERT_EQ("cursor_move_next", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", DB_ERROR(ret), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_prev_p
* @brief                To check move current position of cursor to previous row
* @scenario             If used api returns success
* @apicovered           cursor_move_prev , cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_move_prev_p(void)
{
	db_result_t ret;

	ret = cursor_move_prev(g_cursor);
	TC_ASSERT_EQ("cursor_move_previous", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_last_p
* @brief                To check move current position of cursor to last row
* @scenario             If used api returns success
* @apicovered           cursor_move_last , cursor_get_row , cursor_get_count
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_move_last_p(void)
{
	int count;
	db_result_t ret;

	count = cursor_get_count(g_cursor);
	ret = cursor_move_last(g_cursor);
	TC_ASSERT_EQ("cursor_move_last", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", ret, (count - 1));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_to_p
* @brief                To check move current position of cursor to specific row
* @scenario             If used api returns success
* @apicovered           cursor_move_to, cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_move_to_p(void)
{
	db_result_t ret;
	tuple_id_t row = 3;
	ret = cursor_move_to(g_cursor, row);
	TC_ASSERT_EQ("cursor_move_to", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", ret, row);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_is_first_row_p
* @brief                To check current position of cursor is first row
* @scenario             If used api returns success
* @apicovered           cursor_move_first, cursor_is_first_row
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_is_first_row_p(void)
{
	db_result_t ret;

	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	bool bret = cursor_is_first_row(g_cursor);
	TC_ASSERT_EQ("cursor_is_first_row", DB_ERROR(bret), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_is_last_row_p
* @brief                To check current position of cursor is last row
* @scenario             If used api returns success
* @apicovered           cursor_move_last, cursor_is_last_row
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_is_last_row_p(void)
{
	db_result_t ret;

	ret = cursor_move_last(g_cursor);
	TC_ASSERT_EQ("cursor_move_last", DB_ERROR(ret), 0);

	bool bret = cursor_is_last_row(g_cursor);
	TC_ASSERT_EQ("cursor_is_last_row", DB_ERROR(bret), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_attr_type_p
* @brief                To check get type of attribute of specific index in cursor
* @scenario             If used api returns success
* @apicovered           cursor_get_attr_type
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_attr_type_p(void)
{
	domain_t domain;

	domain = cursor_get_attr_type(g_cursor, 0);
	TC_ASSERT_EQ("cursor_get_attr_type", domain, DOMAIN_INT);

	domain = cursor_get_attr_type(g_cursor, 1);
	TC_ASSERT_EQ("cursor_get_attr_type", domain, DOMAIN_LONG);

	domain = cursor_get_attr_type(g_cursor, 2);
	TC_ASSERT_EQ("cursor_get_attr_type", domain, DOMAIN_STRING);
#ifdef CONFIG_ARCH_FLOAT_H
	domain = cursor_get_attr_type(g_cursor, 3);
	TC_ASSERT_EQ("cursor_get_attr_type", domain, DOMAIN_DOUBLE);

#endif
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_attr_name_p
* @brief                To check get name of attribute of specific index in cursor
* @scenario             If used api returns success
* @apicovered           cursor_get_attr_name
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_attr_name_p(void)
{
	char *attr_name;
	int ret = 0;
	attr_name = cursor_get_attr_name(g_cursor, 0);
	ret = strcmp((const char *)attr_name, g_attribute_set[0]);
	TC_ASSERT_EQ("cursor_get_attr_name", ret, 0);

	attr_name = cursor_get_attr_name(g_cursor, 1);
	ret = strcmp((const char *)attr_name, g_attribute_set[1]);
	TC_ASSERT_EQ("cursor_get_attr_name", ret, 0);

	attr_name = cursor_get_attr_name(g_cursor, 2);
	ret = strcmp((const char *)attr_name, g_attribute_set[2]);
	TC_ASSERT_EQ("cursor_get_attr_name", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_attr_index_p
* @brief                To check index number where using specific attribute name
* @scenario             If used api returns success
* @apicovered           cursor_get_attr_index
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_attr_index_p(void)
{
	attribute_id_t index;

	index = cursor_get_attr_index(g_cursor, g_attribute_set[0]);
	TC_ASSERT_EQ("cursor_get_attr_index", index, 0);

	index = cursor_get_attr_index(g_cursor, g_attribute_set[1]);
	TC_ASSERT_EQ("cursor_get_attr_index", index, 1);

	index = cursor_get_attr_index(g_cursor, g_attribute_set[2]);
	TC_ASSERT_EQ("cursor_get_attr_index", index, 2);

#ifdef CONFIG_ARCH_FLOAT_H
	index = cursor_get_attr_index(g_cursor, g_attribute_set[3]);
	TC_ASSERT_EQ("cursor_get_attr_index", index, 3);

#endif
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_int_value_p
* @brief                To check value of current row's data where specific index if it is integer type.
* @scenario             If used api returns success
* @apicovered           cursor_get_int_value
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_int_value_p(void)
{
	int value;

	value = cursor_get_int_value(g_cursor, 0);
	TC_ASSERT_GEQ("cursor_get_int_value", value, 0);

	value = cursor_get_int_value(g_cursor, 1);
	TC_ASSERT_LT("cursor_get_int_value", value, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_long_value_p
* @brief                To check value of current row's data where specific index if it is long type.
* @scenario             If used api returns success
* @apicovered           cursor_get_long_value
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_long_value_p(void)
{
	long value;

	value = cursor_get_long_value(g_cursor, 1);
	TC_ASSERT_GEQ("cursor_get_long_value", value, 0);

	value = cursor_get_long_value(g_cursor, 2);;
	TC_ASSERT_LT("cursor_get_long_value", value, 0)

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_ARCH_FLOAT_H
/**
* @testcase             itc_arastorage_cursor_get_double_value_p
* @brief                To check value of current row's data where specific index if it is double type.
* @scenario             If used api returns success
* @apicovered           cursor_get_double_value
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_double_value_p(void)
{
	double value;

	value = cursor_get_double_value(g_cursor, 3);
	TC_ASSERT_GEQ("cursor_get_double_value", value, 0)

	value = cursor_get_double_value(g_cursor, 2);
	TC_ASSERT_LT("cursor_get_double_value", value, 0)

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase             itc_arastorage_cursor_get_string_value_p
* @brief                To check value of current row's data where specific index if it is string type.
* @scenario             If used api returns success
* @apicovered           cursor_get_string_value
* @precondition         Database resource must be initialized
* @postcondition        NA
*/
void itc_arastorage_cursor_get_string_value_p(void)
{
	unsigned char *value;

	value = cursor_get_string_value(g_cursor, 2);
	TC_ASSERT_NEQ("cursor_get_string_value", value, NULL);

	value = cursor_get_string_value(g_cursor, 0);
	TC_ASSERT_EQ("cursor_get_string_value", value, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_row_tc_p
* @brief            Get current row in cursor
* @scenario         Get current row in cursor
* @apicovered       cursor_get_row
* @precondition     Database resource must be initialized
* @postcondition    none
*/
void itc_arastorage_cursor_get_row_tc_p(void)
{
	cursor_row_t row;
	tuple_id_t id = 3;

	cursor_move_to(g_cursor, id);
	row = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", row, id);

	TC_SUCCESS_RESULT();
}

int itc_arastorage_launcher(int argc, FAR char *argv[])
{
	total_fail = 0;
	total_pass = 0;
	itc_arastorage_db_init_deinit_p();
	itc_arastorage_startup_p();
	if (g_check) {
		itc_arastorage_db_get_result_message_p();
		itc_arastorage_db_print_header_p();
		itc_arastorage_db_print_tuple_p();
		itc_arastorage_db_print_value_p();
		itc_arastorage_cursor_get_count_p();
		itc_arastorage_cursor_move_first_p();
		itc_arastorage_cursor_move_next_p();
		itc_arastorage_cursor_move_prev_p();
		itc_arastorage_cursor_move_last_p();
		itc_arastorage_cursor_move_to_p();
		itc_arastorage_cursor_is_first_row_p();
		itc_arastorage_cursor_is_last_row_p();
		itc_arastorage_cursor_get_attr_type_p();
		itc_arastorage_cursor_get_attr_name_p();
		itc_arastorage_cursor_get_attr_index_p();
		itc_arastorage_cursor_get_int_value_p();
		itc_arastorage_cursor_get_long_value_p();
#ifdef CONFIG_ARCH_FLOAT_H
		itc_arastorage_cursor_get_double_value_p();
#endif
		itc_arastorage_cursor_get_string_value_p();
	} else {
		printf("Startup FAIL\n");
	}
	itc_arastorage_cleanup_p();

	printf("\n#########################################\n");
	printf("         Arastorage ITC Result            \n");
	printf("         Total TC : %d              \n", (total_pass + total_fail));
	printf("         PASS : %d FAIL : %d        \n", total_pass, total_fail);
	printf("#########################################\n");
	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_arastorage_main(int argc, char *argv[])
#endif
{

#ifdef CONFIG_TASH
	tash_cmd_install("arastorage_itc", itc_arastorage_launcher, TASH_EXECMD_SYNC);
#else
	itc_arastorage_launcher(argc, argv);
#endif
	return 0;

}
