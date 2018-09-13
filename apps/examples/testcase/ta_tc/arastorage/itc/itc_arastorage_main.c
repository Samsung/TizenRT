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
#include <tinyara/fs/fs_utils.h>
#include "tc_common.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#ifndef CONFIG_MOUNT_POINT
#define CONFIG_MOUNT_POINT "/mnt"
#endif

#define RELATION_NAME1 "rel1"
#define RELATION_NAME2 "rel2"
#define RELATION_INDEX "bplustree"
#define QUERY_LENGTH 128
#define DATA_SET_NUM 10

/****************************************************************************
*  Global Variables
 ****************************************************************************/

static char g_query[QUERY_LENGTH];
static bool g_check;
static bool g_init;
static bool g_flag;

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
	{ 1, 20160101, "apple", 1.0001 },
	{ 2, 20160102, "banana", 2.0002 },
	{ 3, 20160103, "mango", 3.0003 },
	{ 4, 20160104, "orange", 4.0004 },
	{ 5, 20160105, "melon", 5.0005 },
	{ 6, 20160106, "kiwi", 6.0006 },
	{ 7, 20160107, "peach", 7.0007 },
	{ 8, 20160108, "cherry", 8.0008 },
	{ 9, 20160109, "strawberry", 9.0009 },
	{10, 20160110, "watermelon", 10.0010 }
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
* @testcase             deinit_db
* @brief                To de-initialize database resources
* @scenario             de-initialize database resources
* @apicovered           db_deinit
* @precondition         none
* @postcondition        none
*/
static void deinit_db(void)
{
	if (g_init) {
		db_deinit();
	}
}

/**
* @testcase             db_cleanup
* @brief                To de-initialize database resources and to remove relation name from DB
* @scenario             If used api returns DB_OK
* @apicovered           db_exec
* @precondition         none
* @postcondition        none
*/
static void db_cleanup(const char *rel)
{
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "REMOVE RELATION %s;", rel);
	int ret = db_exec(g_query);
	if (ret != DB_OK) {
		printf("\ndb_create_attribute : db_exec FAIL\n");
		db_deinit();
		return;
	}
}

/**
* @testcase             error_exec
* @brief                To clean up resource on exec fail
* @scenario             clean up resource on exec fail
* @apicovered           db_deinit
* @precondition         none
* @postcondition        none
*/
static void error_exec(const char *rel_name)
{
	g_flag = false;
	printf("\nerror_exec : db_exec FAIL\n");
	db_cleanup(rel_name);
	db_deinit();
	return;
}

/**
* @testcase             error_query
* @brief                To clean up resource on query fail
* @scenario             clean up resource on query fail
* @apicovered           db_deinit
* @precondition         none
* @postcondition        none
*/
static void error_query(const char *rel_name)
{
	g_flag = false;
	printf("\ndb_query FAIL\n");
	db_cleanup(rel_name);
	db_deinit();
	return;
}

/**
* @testcase             db_create_attribute
* @brief                To create db attribute into db
* @scenario             create db attribute into db
* @apicovered           db_exec
* @precondition         none
* @postcondition        none
*/
static bool db_create_attribute(const char *domain, const char *attribute, const char *rel_name)
{
	g_flag = true;
	db_result_t ret;
	char str[QUERY_LENGTH] = "CREATE ATTRIBUTE %s DOMAIN %s IN %s;";

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, str, attribute, domain, rel_name);

	ret = db_exec(g_query);
	if (ret != DB_OK) {
		g_flag = false;
		printf("\ndb_create_attribute : db_exec FAIL\n");
		db_deinit();
		return false;
	}
	return true;
}

/**
* @testcase             db_insert
* @brief                To insert data into db
* @scenario             insert data into db
* @apicovered           db_exec
* @precondition         none
* @postcondition        none
*/
static void db_insert(const char *rel_name)
{
	g_flag = true;
	db_result_t ret;

	ret = db_init();
	if (ret != DB_OK) {
		g_flag = false;
		printf("\ndb_init FAIL\n");
		return;
	}

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s;", rel_name);
	ret = db_exec(g_query);
	if (ret != DB_OK) {
		g_flag = false;
		printf("\ndb_insert : db_exec FAIL\n");
		db_deinit();
		return;
	}

	if (db_create_attribute("int", g_attribute_set[0], rel_name) == false) {
		return;
	}
	if (db_create_attribute("long", g_attribute_set[1], rel_name) == false) {
		return;
	}
	if (db_create_attribute("string(32)", g_attribute_set[2], rel_name) == false) {
		return;
	}

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE INDEX %s.%s TYPE %s;", rel_name, g_attribute_set[0], RELATION_INDEX);
	ret = db_exec(g_query);
	if (ret != DB_OK) {
		error_exec(rel_name);
		return;
	}

	int index;
	for (index = 0; index < DATA_SET_NUM; index++) {
		memset(g_query, 0, QUERY_LENGTH);
		snprintf(g_query, QUERY_LENGTH, "INSERT (%d, %ld, \'%s\') INTO %s;", g_arastorage_data_set[index].int_value,
			g_arastorage_data_set[index].long_value, g_arastorage_data_set[index].string_value,
			rel_name);
		ret = db_exec(g_query);
		if (ret != DB_OK) {
			error_exec(rel_name);
			return;
		}
	}

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 2;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], rel_name, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	if (g_cursor == NULL) {
		error_query(rel_name);
		return;
	}

	ret = db_deinit();
	if (ret != DB_OK) {
		g_flag = false;
		printf("\ndb_query FAIL\n");
		db_cleanup(rel_name);
		return;
	}

	return;
}

/**
* @testcase             itc_arastorage_db_init_deinit_p
* @brief                To check initialization and deinitialization database resources
* @scenario             check initialization and deinitialization database operation returns DB_OK
* @apicovered           db_deinit, db_init
* @precondition         none
* @postcondition        none
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
* @testcase             itc_arastorage_db_init_deinit_p_multi_time
* @brief                To check initialization and deinitialization database resources
* @scenario             check iterative initialization and deinitialization database operation returns DB_OK
* @apicovered           db_deinit, db_init
* @precondition         none
* @postcondition        none
*/
void itc_arastorage_db_init_deinit_p_multi_time(void)
{
	db_result_t ret;
	int iteration = 0;

	for (iteration = 0; iteration < 10; ++iteration) {
		ret = db_init();
		TC_ASSERT_EQ("db_init", ret, DB_OK);

		ret = db_deinit();
		TC_ASSERT_EQ("db_deinit", ret, DB_OK);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_print_tuple_n_after_deinit
* @brief                To check print the tuple data
* @scenario             If used api returns DB_OK
* @apicovered           db_print_tuple, db_query
* @precondition         Database resource must be deinitialized
* @postcondition        none
*/
void itc_arastorage_db_print_tuple_n_after_deinit(void)
{
	const char *rel_name = RELATION_NAME2;
	db_insert(rel_name);
	TC_ASSERT_EQ("db_insert", g_flag, true);

	TC_ASSERT_EQ_CLEANUP("db_print_tuple", db_print_tuple(g_cursor), DB_CURSOR_ERROR, db_cleanup(rel_name));
	db_cleanup(rel_name);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_print_value_n_after_deinit
* @brief                To check print data value
* @scenario             If used api returns DB_OK
* @apicovered           db_print_value, cursor_move_first
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_db_print_value_n_after_deinit(void)
{
	const char *rel_name = RELATION_NAME2;
	db_insert(rel_name);
	TC_ASSERT_EQ("db_insert", g_flag, true);

	TC_ASSERT_EQ_CLEANUP("db_print_value", db_print_value(g_cursor, 2), DB_CURSOR_ERROR, db_cleanup(rel_name));
	db_cleanup(rel_name);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_startup_p
* @brief                To initialize database resources
* @scenario             If used api returns DB_OK
* @apicovered           db_init, db_exec
* @precondition         none
* @postcondition        none
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

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s;", RELATION_NAME1);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN int IN %s;", g_attribute_set[0], RELATION_NAME1);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN long IN %s;", g_attribute_set[1], RELATION_NAME1);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN string(32) IN %s;", g_attribute_set[2], RELATION_NAME1);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE INDEX %s.%s TYPE %s;", RELATION_NAME1, g_attribute_set[0], RELATION_INDEX);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

#ifdef CONFIG_ARCH_FLOAT_H
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN double IN %s;", g_attribute_set[3], RELATION_NAME1);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, 0);

	for (index = 0; index < DATA_SET_NUM; index++) {
		memset(g_query, 0, QUERY_LENGTH);
		snprintf(g_query, QUERY_LENGTH, "INSERT (%d, %ld, \'%s\', %f) INTO %s;", g_arastorage_data_set[index].int_value,
			g_arastorage_data_set[index].long_value, g_arastorage_data_set[index].string_value,
			g_arastorage_data_set[index].double_value, RELATION_NAME1);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, 0);
	}
#else
	for (index = 0; index < DATA_SET_NUM; index++) {
		memset(g_query, 0, QUERY_LENGTH);
		snprintf(g_query, QUERY_LENGTH, "INSERT (%d, %ld, \'%s\') INTO %s;", g_arastorage_data_set[index].int_value,
			g_arastorage_data_set[index].long_value, g_arastorage_data_set[index].string_value,
			RELATION_NAME1);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, DB_OK);
	}
#endif

	g_check = true;
	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_query_p_create_remove_relation
* @brief                To check create  and remove relation in database system
* @scenario             check create  and remove relation in database system returns DB_OK
* @apicovered           db_exec
* @precondition         none
* @postcondition        none
*/
void itc_arastorage_db_query_p_create_remove_relation(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	const char *rel_name = RELATION_NAME2;

	// g_query to create relation
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s;", rel_name);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "REMOVE RELATION %s;", rel_name);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_query_n_recreate_relation
* @brief                To check create  and remove relation in database system
* @scenario             check re create relation in database system
* @apicovered           db_exec
* @precondition         none
* @postcondition        none
*/
void itc_arastorage_db_query_n_recreate_relation(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;
	const char *rel_name = RELATION_NAME2;

	// query to create relation
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s;", rel_name);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	// query to create relation
	ret = db_exec(g_query);
	TC_ASSERT_EQ_CLEANUP("db_exec", ret, DB_RELATIONAL_ERROR, db_cleanup(rel_name));

	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "REMOVE RELATION %s;", rel_name);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_query_n_reremove_relation
* @brief                To check create  and remove relation in database system
* @scenario             check create  and remove relation in database system returns DB_OK
* @apicovered           db_exec
* @precondition         none
* @postcondition        none
*/
void itc_arastorage_db_query_n_reremove_relation(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;
	const char *rel_name = RELATION_NAME2;

	// query to create relation
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s;", rel_name);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	// query to remove relation
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "REMOVE RELATION %s;", rel_name);
	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_OK);

	ret = db_exec(g_query);
	TC_ASSERT_EQ("db_exec", ret, DB_RELATIONAL_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_query_p_create_remove_multi_relation
* @brief                To check create  and remove relation in database system
* @scenario             check create and remove relation in database system returns DB_OK
* @apicovered           db_deinit, db_init, db_exec
* @precondition         none
* @postcondition        none
*/
void itc_arastorage_db_query_p_create_remove_multi_relation(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;
	const char *rel_name = RELATION_NAME2;
	int index = 0;

	for (index = 0; index < 10; ++index) {
		memset(g_query, 0, QUERY_LENGTH);
		//query to create relation
		snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s_%d;", rel_name, index);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, DB_OK);

		memset(g_query, 0, QUERY_LENGTH);
		//query to remove relation
		snprintf(g_query, QUERY_LENGTH, "REMOVE RELATION %s_%d;", rel_name, index);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, DB_OK);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_query_p_create_remove_relation_multi_time
* @brief                To check create  and remove relation in database system
* @scenario             check create  and remove relation in database system returns DB_OK
* @apicovered           db_deinit, db_init, db_exec
* @precondition         none
* @postcondition        none
*/
void itc_arastorage_db_query_p_create_remove_relation_multi_time(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;
	const char *rel_name = RELATION_NAME2;
	int index = 0;

	for (index = 0; index < 10; ++index) {
		memset(g_query, 0, QUERY_LENGTH);
		//query to create relation
		snprintf(g_query, QUERY_LENGTH, "CREATE RELATION %s;", rel_name);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, DB_OK);

		memset(g_query, 0, QUERY_LENGTH);
		//query to remove relation
		snprintf(g_query, QUERY_LENGTH, "REMOVE RELATION %s;", rel_name);
		ret = db_exec(g_query);
		TC_ASSERT_EQ("db_exec", ret, DB_OK);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_get_result_message_p
* @brief                To check get string of each API's result based on value of db_result_t
* @scenario             If used api returns DB_OK
* @apicovered           db_get_result_message
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_db_get_result_message_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	const char *str_err = db_get_result_message(DB_FINISHED);
	TC_ASSERT_EQ("db_get_result_message : DB_FINISHED", strncmp(str_err, DB_FINISHED_MSG, sizeof(DB_FINISHED_MSG)), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_get_result_message_p_all_msgs
* @brief                to check get string of each API's result based on value of db_result_t
* @scenario             error message should be return
* @apicovered           db_get_result_message
* @precondition         none
* @postcondition        none
*/
void itc_arastorage_db_get_result_message_p_all_msgs(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	int count;
	int index = 0;

	struct db_result_error {
		db_result_t code;
		char *str_err;
	};

	struct db_result_error st_error[] = {
		{ DB_FINISHED, DB_FINISHED_MSG },
		{ DB_OK, DB_OK_MSG },
		{ DB_LIMIT_ERROR, DB_LIMIT_ERROR_MSG },
		{ DB_ALLOCATION_ERROR, DB_ALLOCATION_ERROR_MSG },
		{ DB_STORAGE_ERROR, DB_STORAGE_ERROR_MSG },
		{ DB_PARSING_ERROR, DB_PARSING_ERROR_MSG },
		{ DB_NAME_ERROR, DB_NAME_ERROR_MSG },
		{ DB_RELATIONAL_ERROR, DB_RELATIONAL_ERROR_MSG },
		{ DB_TYPE_ERROR, DB_TYPE_ERROR_MSG },
		{ DB_IMPLEMENTATION_ERROR, DB_IMPLEMENTATION_ERROR_MSG },
		{ DB_INDEX_ERROR, DB_INDEX_ERROR_MSG },
		{ DB_BUSY_ERROR, DB_BUSY_ERROR_MSG },
		{ DB_INCONSISTENCY_ERROR, DB_INCONSISTENCY_ERROR_MSG },
		{ DB_ARGUMENT_ERROR, DB_ARGUMENT_ERROR_MSG },
		{ DB_FULL_ERROR, DB_FULL_ERROR_MSG },
		{ DB_CURSOR_ERROR, DB_CURSOR_ERROR_MSG },
		{ DB_EMPTY_CURSOR_ERROR, DB_EMPTY_CURSOR_ERROR_MSG }
	};

	count = sizeof(st_error) / sizeof(st_error[0]);

	for (index = 0; index < count; ++index) {
		printf("Error code %d\n", st_error[index].code);
		TC_ASSERT_EQ("db_get_result_message", strcmp(db_get_result_message(st_error[index].code), st_error[index].str_err), 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_db_get_result_message_n
* @brief            Get database result message with invalid argument
* @scenario         Get database result message with out of range result value
* @apicovered       db_get_result_message
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_db_get_result_message_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);

	TC_ASSERT_EQ("db_get_result_message", strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(200)), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_print_header_p
* @brief                To check print of the related information : relation, attribute name
* @scenario             If used api returns DB_OK
* @apicovered           db_print_header, db_query
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_db_print_header_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);
	ret = db_print_header(g_cursor);
	TC_ASSERT_EQ_CLEANUP("db_print_header", DB_ERROR(ret), 0, db_cursor_free(g_cursor));
	db_cursor_free(g_cursor);
	g_cursor = NULL;

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_db_print_header_n
* @brief            Print a header of query result with invalid argument
* @scenario         Print relation, attribute name of query result with NULL value
* @apicovered       db_print_header
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_db_print_header_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = db_print_header(NULL);
	TC_ASSERT_EQ("db_print_header", ret, DB_ARGUMENT_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_print_tuple_p
* @brief                To check print the tuple data
* @scenario             If used api returns DB_OK
* @apicovered           db_print_tuple, db_query
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_db_print_tuple_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	memset(g_query, 0, QUERY_LENGTH);
#ifdef CONFIG_ARCH_FLOAT_H
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], g_attribute_set[3], RELATION_NAME1, g_attribute_set[0]);
#else
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
#endif
	g_cursor = NULL;
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	if (DB_SUCCESS(cursor_move_first(g_cursor))) {
		do {
			ret = db_print_tuple(g_cursor);
			TC_ASSERT_EQ("db_print_tuple", DB_ERROR(ret), 0);
		} while (DB_SUCCESS(cursor_move_next(g_cursor)));
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_db_print_tuple_n
* @brief            Print tuple pointed by cursor with invalid argument
* @scenario         Print tuple values from the first to the last row with NULL value
* @apicovered       cursor_move_first, db_print_tuple, cursor_move_next
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_db_print_tuple_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = db_print_tuple(NULL);
	TC_ASSERT_EQ("db_print_tuple", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_db_print_value_p
* @brief                To check print data value
* @scenario             If used api returns DB_OK
* @apicovered           db_print_value, cursor_move_first
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_db_print_value_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	if (DB_SUCCESS(cursor_move_first(g_cursor))) {
		do {
			ret = db_print_value(g_cursor, 2);
			TC_ASSERT_EQ("db_print_value", DB_ERROR(ret), 0);
		} while (DB_SUCCESS(cursor_move_next(g_cursor)));
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_db_print_value_n
* @brief            Print a attribute value of tuple pointed by cursor with invalid argument
* @scenario         Print a specific attribute value of tuple pointed by cursor with NULL value
* @apicovered       db_print_value
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_db_print_value_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = db_print_value(NULL, 2);
	TC_ASSERT_EQ("db_print_value", ret, DB_CURSOR_ERROR);

	ret = db_print_value(g_cursor, 5);
	TC_ASSERT_EQ("db_print_value", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_count_p
* @brief                To check get count of total row of cursor
* @scenario             If used api returns DB_OK
* @apicovered           cursor_get_count
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_get_count_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);

	g_cursor = NULL;
	int nCount = -1;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	nCount = cursor_get_count(g_cursor);

	TC_ASSERT_GEQ("cursor_get_count", nCount, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_count_n
* @brief            Get the number of tuples in cursor with invalid argument
* @scenario         Get the number of tuples in cursor with NULL value
* @apicovered       cursor_get_count
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_count_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	tuple_id_t count;

	count = cursor_get_count(NULL);
	TC_ASSERT_EQ("cursor_get_count", count, INVALID_CURSOR_VALUE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_first_p
* @brief                To check move current position of cursor to first row
* @scenario             If used api returns success
* @apicovered           cursor_move_first, cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_move_first_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_move_first_n
* @brief            Move cursor to the first row with invalid argument
* @scenario         Move cursor to the first row with NULL value
* @apicovered       cursor_move_first, cursor_get_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_move_first_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = cursor_move_first(NULL);
	TC_ASSERT_EQ("cursor_move_first", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_next_p
* @brief                To check move current position of cursor to next row
* @scenario             If used api returns success
* @apicovered           cursor_move_next, cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_move_next_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_next(g_cursor);
	TC_ASSERT_EQ("cursor_move_next", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_GEQ("cursor_get_row", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_move_next_n
* @brief            Move cursor to next row with invalid argument
* @scenario         Move cursor to next row with NULL value
* @apicovered       cursor_move_next, cursor_get_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_move_next_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = cursor_move_next(NULL);
	TC_ASSERT_EQ("cursor_move_next", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_prev_p
* @brief                To check move current position of cursor to previous row
* @scenario             If used api returns success
* @apicovered           cursor_move_prev, cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_move_prev_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_last(g_cursor);
	TC_ASSERT_EQ("cursor_move_last", DB_ERROR(ret), 0)

	ret = cursor_move_prev(g_cursor);
	TC_ASSERT_GEQ("cursor_move_previous", ret, 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_GEQ("cursor_get_row", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_move_prev_n
* @brief            Move cursor to previous row with invalid argument
* @scenario         Move cursor to previous row with NULL value
* @apicovered       cursor_move_prev, cursor_get_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_move_prev_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = cursor_move_prev(NULL);
	TC_ASSERT_EQ("cursor_move_prev", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_last_p
* @brief                To check move current position of cursor to last row
* @scenario             If used api returns success
* @apicovered           cursor_move_last, cursor_get_row, cursor_get_count
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_move_last_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	int count;
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	count = cursor_get_count(g_cursor);
	ret = cursor_move_last(g_cursor);
	TC_ASSERT_EQ("cursor_move_last", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", ret, (count - 1));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_move_last_n
* @brief            Move cursor to the last row with invalid argument
* @scenario         Move cursor to the last row with NULL value
* @apicovered       cursor_move_last, cursor_get_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_move_last_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = cursor_move_last(NULL);
	TC_ASSERT_EQ("cursor_move_last", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_move_to_p
* @brief                To check move current position of cursor to specific row
* @scenario             If used api returns success
* @apicovered           cursor_move_to, cursor_get_row
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_move_to_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;
	tuple_id_t row = 3;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_to(g_cursor, row);
	TC_ASSERT_EQ("cursor_move_to", DB_ERROR(ret), 0);

	ret = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", ret, row);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_move_to_n
* @brief            Move cursor to specific row
* @scenario         Move cursor to specific row and check whether current row is set as the specific row
* @apicovered       cursor_move_to, cursor_get_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_move_to_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;
	tuple_id_t row = 3;

	ret = cursor_move_to(NULL, row);
	TC_ASSERT_EQ("cursor_move_to", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_is_first_row_p
* @brief                To check current position of cursor is first row
* @scenario             If used api returns success
* @apicovered           cursor_move_first, cursor_is_first_row
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_is_first_row_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	bool bret = cursor_is_first_row(g_cursor);
	TC_ASSERT_EQ("cursor_is_first_row", DB_ERROR(bret), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_is_first_row_n
* @brief            Check whether cursor points the first row with invalid argument
* @scenario         Move cursor to the last row and check whether current row is set as the first row,
*                   Check whether cursor points the first row with NULL value
* @apicovered       db_query, cursor_move_last, cursor_is_first_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_is_first_row_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	/* We need to get valid cursor for this test */
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 0;", g_attribute_set[0],
		g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);

	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);
	ret = cursor_move_last(g_cursor);
	TC_ASSERT_EQ("cursor_move_last", DB_SUCCESS(ret), true);

	/* Cursor is valid, but it is pointing to last row and not the first row */
	bool bret = cursor_is_first_row(g_cursor);
	TC_ASSERT_EQ("cursor_is_first_row", bret, false);

	/* Invalid argument */
	bret = cursor_is_first_row(NULL);
	TC_ASSERT_EQ("cursor_is_first_row", bret, false);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_is_last_row_p
* @brief                To check current position of cursor is last row
* @scenario             If used api returns success
* @apicovered           cursor_move_last, cursor_is_last_row
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_is_last_row_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_last(g_cursor);
	TC_ASSERT_EQ("cursor_move_last", DB_ERROR(ret), 0);

	bool bret = cursor_is_last_row(g_cursor);
	TC_ASSERT_EQ("cursor_is_last_row", DB_ERROR(bret), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_is_last_row_n
* @brief            Check whether cursor points the last row with invalid argument
* @scenario         Move cursor to the first row and check whether current row is set as the last row,
*                   Check whether cursor points the last row with NULL value
* @apicovered       db_query, cursor_move_first, cursor_is_last_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_is_last_row_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	/* We need to get valid cursor for this test */
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 0;", g_attribute_set[0],
		g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);
	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_SUCCESS(ret), true);

	/* Cursor is valid, but it isn't pointing last row */
	bool bret = cursor_is_last_row(g_cursor);
	TC_ASSERT_EQ("cursor_is_last_row", bret, false);

	/* Invalid argument */
	bret = cursor_is_last_row(NULL);
	TC_ASSERT_EQ("cursor_is_last_row", bret, false);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_attr_type_p
* @brief                To check get type of attribute of specific index in cursor
* @scenario             If used api returns success
* @apicovered           cursor_get_attr_type
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_get_attr_type_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	domain_t domain;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

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
* @testcase         itc_arastorage_cursor_get_attr_type_n
* @brief            Get a domain(type) of specific attribute with invalid argument
* @scenario         Get a domain of specific attribute with NULL value
*                   Get a domain of attribute with out of range attribute index
* @apicovered       cursor_get_attr_type
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_attr_type_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	domain_t domain;

	domain = cursor_get_attr_type(NULL, 0);
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_INT);
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_LONG);
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_STRING);
#ifdef CONFIG_ARCH_FLOAT_H
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_DOUBLE);
#endif

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	domain = cursor_get_attr_type(g_cursor, 6);
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_INT);
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_LONG);
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_STRING);
#ifdef CONFIG_ARCH_FLOAT_H
	TC_ASSERT_NEQ("cursor_get_attr_type", domain, DOMAIN_DOUBLE);
#endif

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_attr_name_p
* @brief                To check get name of attribute of specific index in cursor
* @scenario             If used api returns success
* @apicovered           cursor_get_attr_name
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_get_attr_name_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	char *attr_name = NULL;
	int ret = 0;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	attr_name = cursor_get_attr_name(g_cursor, 0);
	ret = strncmp((const char *)attr_name, g_attribute_set[0], sizeof(attr_name));
	TC_ASSERT_EQ("cursor_get_attr_name", ret, 0);

	attr_name = cursor_get_attr_name(g_cursor, 1);
	ret = strncmp((const char *)attr_name, g_attribute_set[1], sizeof(attr_name));
	TC_ASSERT_EQ("cursor_get_attr_name", ret, 0);

	attr_name = cursor_get_attr_name(g_cursor, 2);
	ret = strncmp((const char *)attr_name, g_attribute_set[2], sizeof(attr_name));
	TC_ASSERT_EQ("cursor_get_attr_name", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_attr_name_n
* @brief            Get a name of specific attribute with invalid argument
* @scenario         Get a name of specific attribute with NULL value
*                   Get a name of attribute with out of range attribute index
* @apicovered       cursor_get_attr_name
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_attr_name_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	char *attr_name;

	attr_name = cursor_get_attr_name(NULL, 0);
	TC_ASSERT_NEQ("cursor_get_attr_name", strcmp((const char *)attr_name, g_attribute_set[0]), 0);

	attr_name = cursor_get_attr_name(NULL, 1);
	TC_ASSERT_NEQ("cursor_get_attr_name", strcmp((const char *)attr_name, g_attribute_set[1]), 0);

	attr_name = cursor_get_attr_name(NULL, 2);
	TC_ASSERT_NEQ("cursor_get_attr_name", strcmp((const char *)attr_name, g_attribute_set[2]), 0);

	attr_name = cursor_get_attr_name(g_cursor, 3);
	TC_ASSERT_NEQ("cursor_get_attr_name", strcmp((const char *)attr_name, g_attribute_set[3]), 0);

#ifdef CONFIG_ARCH_FLOAT_H
	attr_name = cursor_get_attr_name(NULL, 3);
	TC_ASSERT_NEQ("cursor_get_attr_name", strcmp((const char *)attr_name, g_attribute_set[3]), 0);

	attr_name = cursor_get_attr_name(g_cursor, 4);
	TC_ASSERT_NEQ("cursor_get_attr_name", strcmp((const char *)attr_name, g_attribute_set[3]), 0);
#endif

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_attr_index_p
* @brief                To check index number where using specific attribute name
* @scenario             If used api returns success
* @apicovered           cursor_get_attr_index
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_get_attr_index_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	attribute_id_t index;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

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
* @testcase         itc_arastorage_cursor_get_attr_index_n
* @brief            Get a index of specific attribute with invalid argument
* @scenario         Get a index of specific attribute with NULL value
* @apicovered       cursor_get_attr_index
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_attr_index_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	attribute_id_t index;

	index = cursor_get_attr_index(NULL, g_attribute_set[0]);
	TC_ASSERT_NEQ("cursor_get_attr_index", index, 0);

	index = cursor_get_attr_index(NULL, g_attribute_set[1]);
	TC_ASSERT_NEQ("cursor_get_attr_index", index, 1);

	index = cursor_get_attr_index(NULL, g_attribute_set[2]);
	TC_ASSERT_NEQ("cursor_get_attr_index", index, 2);

#ifdef CONFIG_ARCH_FLOAT_H
	index = cursor_get_attr_index(NULL, g_attribute_set[3]);
	TC_ASSERT_NEQ("cursor_get_attr_index", index, 3);
#endif

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_arastorage_cursor_get_int_value_p
* @brief                To check value of current row's data where specific index if it is integer type.
* @scenario             If used api returns success
* @apicovered           cursor_get_int_value
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_get_int_value_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	int value;
	int ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	value = cursor_get_int_value(g_cursor, 0);
	TC_ASSERT_GEQ("cursor_get_int_value", value, 0);

	value = cursor_get_int_value(g_cursor, 1);
	TC_ASSERT_EQ("cursor_get_int_value", value, DB_INT_ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_int_value_n
* @brief            Get a int value of specific attribute with invalid argument
* @scenario         Get a int value of specific attribute with NULL value
*                   Get a int value of attribute which the type is not integer
* @apicovered       cursor_get_int_value
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_int_value_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	int value;

	value = cursor_get_int_value(NULL, 0);
	TC_ASSERT_LT("cursor_get_int_value", value, 0);

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
* @postcondition        none
*/
void itc_arastorage_cursor_get_long_value_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	long value;
	int ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	value = cursor_get_long_value(g_cursor, 1);
	TC_ASSERT_GEQ("cursor_get_long_value", value, 0);

	value = cursor_get_long_value(g_cursor, 2);;
	TC_ASSERT_EQ("cursor_get_long_value", value, DB_LONG_ERROR)

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_long_value_n
* @brief            Get a long value of specific attribute with invalid argument
* @scenario         Get a long value of specific attribute with NULL value
*                   Get a long value of attribute which the type is not long
* @apicovered       cursor_get_long_value
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_long_value_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	long value;

	value = cursor_get_long_value(NULL, 1);
	TC_ASSERT_LT("cursor_get_long_value", value, 0);

	value = cursor_get_long_value(g_cursor, 2);
	TC_ASSERT_LT("cursor_get_long_value", value, 0);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_ARCH_FLOAT_H
/**
* @testcase             itc_arastorage_cursor_get_double_value_p
* @brief                To check value of current row's data where specific index if it is double type.
* @scenario             If used api returns success
* @apicovered           cursor_get_double_value
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_get_double_value_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	double value;
	int ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	value = cursor_get_double_value(g_cursor, 3);
	TC_ASSERT_GEQ("cursor_get_double_value", value, 0)

	value = cursor_get_double_value(g_cursor, 2);
	TC_ASSERT_EQ("cursor_get_double_value", value, DB_DOUBLE_ERROR)

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_double_value_n
* @brief            Get a double value of specific attribute with invalid argument
* @scenario         Get a double value of specific attribute with NULL value
*                   Get a double value of attribute which the type is not double
* @apicovered       cursor_get_double_value
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_double_value_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	double value;

	value = cursor_get_double_value(NULL, 3);
	TC_ASSERT_LT("cursor_get_double_value", value, 0);

	value = cursor_get_double_value(g_cursor, 2);
	TC_ASSERT_LT("cursor_get_double_value", value, 0);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase             itc_arastorage_cursor_get_string_value_p
* @brief                To check value of current row's data where specific index if it is string type.
* @scenario             If used api returns success
* @apicovered           cursor_get_string_value
* @precondition         Database resource must be initialized
* @postcondition        none
*/
void itc_arastorage_cursor_get_string_value_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	unsigned char *value = NULL;
	int ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = cursor_move_first(g_cursor);
	TC_ASSERT_EQ("cursor_move_first", DB_ERROR(ret), 0);

	value = cursor_get_string_value(g_cursor, 2);
	TC_ASSERT_NEQ("cursor_get_string_value", value, NULL);

	value = cursor_get_string_value(g_cursor, 0);
	TC_ASSERT_EQ("cursor_get_string_value", value, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_string_value_n
* @brief            Get a string value of specific attribute with invalid argument
* @scenario         Get a string value of specific attribute with NULL value
*                   Get a string value of attribute which the type is not string
* @apicovered       cursor_get_string_value
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_string_value_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	unsigned char *value;

	value = cursor_get_string_value(NULL, 2);
	TC_ASSERT_EQ("cursor_get_string_value", value, NULL);

	value = cursor_get_string_value(g_cursor, 0);
	TC_ASSERT_EQ("cursor_get_string_value", value, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_row_p
* @brief            Get current row in cursor
* @scenario         Get current row in cursor
* @apicovered       cursor_get_row
* @precondition     Database resource must be initialized
* @postcondition    none
*/
void itc_arastorage_cursor_get_row_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	cursor_row_t row;
	tuple_id_t id = 3;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	int ret = cursor_move_to(g_cursor, id);
	TC_ASSERT_EQ("cursor_move_to", DB_ERROR(ret), 0);

	row = cursor_get_row(g_cursor);
	TC_ASSERT_EQ("cursor_get_row", row, id);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_cursor_get_row_n
* @brief            Get current row in cursor with invalid argument
* @scenario         Get current row in cursor with NULL value
* @apicovered       cursor_get_row
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_cursor_get_row_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	cursor_row_t row;

	row = cursor_get_row(NULL);
	TC_ASSERT_EQ("cursor_get_row", row, INVALID_CURSOR_VALUE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_db_cursor_free_p
* @brief            Free cursor
* @scenario         Free allocated resources for cursor
* @apicovered       db_cursor_free
* @precondition     itc_arastorage_startup_p should be pass
* @postcondition    none
*/
void itc_arastorage_db_cursor_free_p(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	g_cursor = NULL;
	memset(g_query, 0, QUERY_LENGTH);
	snprintf(g_query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0], g_attribute_set[1], g_attribute_set[2], RELATION_NAME1, g_attribute_set[0]);
	g_cursor = db_query(g_query);
	TC_ASSERT_NEQ("db_query", g_cursor, NULL);

	ret = db_cursor_free(g_cursor);
	TC_ASSERT_EQ("db_cursor_free", DB_SUCCESS(ret), true);
	g_cursor = NULL;

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_arastorage_db_cursor_free_n
* @brief            Free cursor with invalid argument
* @scenario         Free allocated resources for cursor with NULL value
* @apicovered       db_cursor_free
* @precondition     none
* @postcondition    none
*/
void itc_arastorage_db_cursor_free_n(void)
{
	TC_ASSERT_EQ("ARASTORAGE_STARTUP", g_check, true);
	db_result_t ret;

	ret = db_cursor_free(NULL);
	TC_ASSERT_EQ("db_cursor_free", ret, DB_CURSOR_ERROR);

	TC_SUCCESS_RESULT();
}

int itc_arastorage_launcher(int argc, FAR char *argv[])
{
	itc_arastorage_startup_p();
	//db_init being called once inside itc_arastorage_startup_p for below ITCs
	itc_arastorage_db_query_p_create_remove_relation();
	itc_arastorage_db_query_n_recreate_relation();
	itc_arastorage_db_query_n_reremove_relation();
	itc_arastorage_db_query_p_create_remove_multi_relation();
	itc_arastorage_db_query_p_create_remove_relation_multi_time();
	itc_arastorage_db_get_result_message_p();
	itc_arastorage_db_get_result_message_p_all_msgs();
	itc_arastorage_db_get_result_message_n();
	itc_arastorage_db_print_header_p();
	itc_arastorage_db_print_header_n();
	itc_arastorage_db_print_tuple_p();
	itc_arastorage_db_print_tuple_n();
	itc_arastorage_db_print_value_p();
	itc_arastorage_db_print_value_n();
	itc_arastorage_cursor_get_count_p();
	itc_arastorage_cursor_get_count_n();
	itc_arastorage_cursor_move_first_p();
	itc_arastorage_cursor_move_first_n();
	itc_arastorage_cursor_move_next_p();
	itc_arastorage_cursor_move_next_n();
	itc_arastorage_cursor_move_prev_p();
	itc_arastorage_cursor_move_prev_n();
	itc_arastorage_cursor_move_last_p();
	itc_arastorage_cursor_move_last_n();
	itc_arastorage_cursor_move_to_p();
	itc_arastorage_cursor_move_to_n();
	itc_arastorage_cursor_is_first_row_p();
	itc_arastorage_cursor_is_first_row_n();
	itc_arastorage_cursor_is_last_row_p();
	itc_arastorage_cursor_is_last_row_n();
	itc_arastorage_cursor_get_attr_type_p();
	itc_arastorage_cursor_get_attr_type_n();
	itc_arastorage_cursor_get_attr_name_p();
	itc_arastorage_cursor_get_attr_name_n();
	itc_arastorage_cursor_get_attr_index_p();
	itc_arastorage_cursor_get_attr_index_n();
	itc_arastorage_cursor_get_int_value_p();
	itc_arastorage_cursor_get_int_value_n();
	itc_arastorage_cursor_get_long_value_p();
	itc_arastorage_cursor_get_long_value_n();
	itc_arastorage_cursor_get_row_p();
	itc_arastorage_cursor_get_row_n();
#ifdef CONFIG_ARCH_FLOAT_H
	itc_arastorage_cursor_get_double_value_p();
	itc_arastorage_cursor_get_double_value_n();
#endif
	itc_arastorage_cursor_get_string_value_p();
	itc_arastorage_cursor_get_string_value_n();
	itc_arastorage_db_cursor_free_p();
	itc_arastorage_db_cursor_free_n();

	db_cleanup(RELATION_NAME1);
	deinit_db();

	//Scenario ITCs called after db_deinit
	itc_arastorage_db_init_deinit_p();
	itc_arastorage_db_init_deinit_p_multi_time();
	itc_arastorage_db_print_tuple_n_after_deinit();
	itc_arastorage_db_print_value_n_after_deinit();

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_arastorage_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "Arastorage ITC") == ERROR) {
		return ERROR;
	}

	itc_arastorage_launcher(argc, argv);

	(void)tc_handler(TC_END, "Arastorage ITC");
	return 0;
}
