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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <arastorage/arastorage.h>
#include <apps/shell/tash.h>
#include <tinyara/fs/fs_utils.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#ifndef CONFIG_MOUNT_POINT
#define CONFIG_MOUNT_POINT "/mnt"
#endif

#define RELATION_NAME "rel"
#define RELATION_INDEX "bplustree"
#define QUERY_LENGTH 128

#define DATA_SET_NUM 10
/****************************************************************************
 *  Global Variables
 ****************************************************************************/

static db_cursor_t *g_cursor;

static int g_arastorage_tc_count;

static int g_arastorage_tc_fail_count;


const static char *g_attribute_set[] = {"id", "date", "fruit", "weight"};

struct arastorage_data_type_s {
	int int_value;
	long long_value;
	char *string_value;
	double double_value;
};

typedef struct arastorage_data_type_s arastorage_data_t;

const static arastorage_data_t g_arastorage_data_set[DATA_SET_NUM] = {
	{ 1,        20160101,       "apple",           1.0001},
	{ 2,        20160102,       "banana",          2.0002},
	{ 3,        20160103,       "mango",           3.0003},
	{ 4,        20160104,       "orange",          4.0004},
	{ 5,        20160105,       "melon",           5.0005},
	{ 6,        20160106,       "kiwi",            6.0006},
	{ 7,        20160107,       "peach",           7.0007},
	{ 8,        20160108,       "cherry",          8.0008},
	{ 9,        20160109,       "strawberry",      9.0009},
	{10,        20160110,       "watermelon",     10.0010}
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void utc_arastorage_db_init_tc_p(void)
{
	db_result_t res;

	printf("%d. db_init Positive Unit Test started. Please wait...\n", g_arastorage_tc_count++);
	res = db_init();
	if (DB_ERROR(res)) {
		printf("db_init Failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS!\n");
}

void utc_arastorage_db_deinit_tc_p(void)
{
	db_result_t res;

	printf("%d. db_deinit Positive Unit Test started. Please wait...\n", g_arastorage_tc_count++);
	res = db_deinit();
	if (DB_ERROR(res)) {
		printf("db_deinit Failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS!\n");
}

void utc_arastorage_db_exec_tc_p(void)
{
	db_result_t res;
	char query[QUERY_LENGTH];
	int i;

	printf("%d. db_exec Positive Unit Test started. Please wait...\n", g_arastorage_tc_count++);
	sleep(2);
	snprintf(query, QUERY_LENGTH, "CREATE RELATION %s;", RELATION_NAME);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Create relation) res : \n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	memset(query, 0, QUERY_LENGTH);
	snprintf(query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN int IN %s;", g_attribute_set[0],
			 RELATION_NAME);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Create Attribute) Name : %s Type : Int\n",
			   g_attribute_set[0]);
		g_arastorage_tc_fail_count++;
		return;
	}

	memset(query, 0, QUERY_LENGTH);
	snprintf(query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN long IN %s;", g_attribute_set[1],
			 RELATION_NAME);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Create Attribute) Name : %s Type : LONG\n",
			   g_attribute_set[1]);
		g_arastorage_tc_fail_count++;
		return;
	}

	memset(query, 0, QUERY_LENGTH);
	snprintf(query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN string(32) IN %s;",
			 g_attribute_set[2],
			 RELATION_NAME);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Create Attribute) Name : %s Type = STRING\n",
			   g_attribute_set[2]);
		g_arastorage_tc_fail_count++;
		return;
	}

	memset(query, 0, QUERY_LENGTH);
	sprintf(query, "CREATE INDEX %s.%s TYPE %s;", RELATION_NAME, g_attribute_set[0], RELATION_INDEX);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Create Index) Type : %s Attribute = %s\n", RELATION_INDEX, g_attribute_set[0]);
		g_arastorage_tc_fail_count++;
		return;
	}

#ifdef CONFIG_ARCH_FLOAT_H
	memset(query, 0, QUERY_LENGTH);
	snprintf(query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN double IN %s;", g_attribute_set[3],
			 RELATION_NAME);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Create Attribute) Name : %s Type : DOUBLE\n",
			   g_attribute_set[1]);
		g_arastorage_tc_fail_count++;
		return;
	}

	for (i = 0; i < DATA_SET_NUM; i++) {
		memset(query, 0, QUERY_LENGTH);
		snprintf(query, "INSERT (%d, %ld, \'%s\', %f) INTO %s;", QUERY_LENGTH,
				 g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				 g_arastorage_data_set[i].string_value, g_arastorage_data_set[i].double_value,
				 RELATION_NAME);
		res = db_exec(query);
		if (DB_ERROR(res)) {
			printf("db_exec Failed(Insert Data) Data : %d %d %s %lf Tuple Row : %d\n",
				   g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				   g_arastorage_data_set[i].string_value, g_arastorage_data_set[i].double_value,
				   i);
			g_arastorage_tc_fail_count++;
			return;
		}
	}
#else
	for (i = 0; i < DATA_SET_NUM; i++) {
		memset(query, 0, QUERY_LENGTH);
		snprintf(query, QUERY_LENGTH, "INSERT (%d, %ld, \'%s\') INTO %s;",
				 g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				 g_arastorage_data_set[i].string_value, RELATION_NAME);
		res = db_exec(query);
		if (DB_ERROR(res)) {
			printf("db_exec Failed(Insert Data) Data : %d %d %s Tuple Row : %d\n",
				   g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				   g_arastorage_data_set[i].string_value, i);
			g_arastorage_tc_fail_count++;
			return;
		}
	}
#endif



	memset(query, 0, QUERY_LENGTH);
	sprintf(query, "REMOVE INDEX %s.%s ;", RELATION_NAME, g_attribute_set[0],RELATION_NAME);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Remove Index) Type : %s Attribute = %s\n", RELATION_INDEX, g_attribute_set[0]);
		g_arastorage_tc_fail_count++;
		return;
	}



#ifdef CONFIG_ARCH_FLOAT_H
	memset(query, 0, QUERY_LENGTH);
	snprintf(query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN double IN %s;", g_attribute_set[3],
			 RELATION_NAME);
	res = db_exec(query);
	if (DB_ERROR(res)) {
		printf("db_exec Failed(Create Attribute) Name : %s Type : DOUBLE\n",
			   g_attribute_set[1]);
		g_arastorage_tc_fail_count++;
		return;
	}

	for (i = 0; i < DATA_SET_NUM; i++) {
		memset(query, 0, QUERY_LENGTH);
		snprintf(query, "INSERT (%d, %ld, \'%s\', %f) INTO %s;", QUERY_LENGTH,
				 g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				 g_arastorage_data_set[i].string_value, g_arastorage_data_set[i].double_value,
				 RELATION_NAME);
		res = db_exec(query);
		if (DB_ERROR(res)) {
			printf("db_exec Failed(Insert Data) Data : %d %d %s %lf Tuple Row : %d\n",
				   g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				   g_arastorage_data_set[i].string_value, g_arastorage_data_set[i].double_value,
				   i);
			g_arastorage_tc_fail_count++;
			return;
		}
	}
#else
	for (i = 0; i < DATA_SET_NUM; i++) {
		memset(query, 0, QUERY_LENGTH);
		snprintf(query, QUERY_LENGTH, "INSERT (%d, %ld, \'%s\') INTO %s;",
				 g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				 g_arastorage_data_set[i].string_value, RELATION_NAME);
		res = db_exec(query);
		if (DB_ERROR(res)) {
			printf("db_exec Failed(Insert Data) Data : %d %d %s Tuple Row : %d\n",
				   g_arastorage_data_set[i].int_value, g_arastorage_data_set[i].long_value,
				   g_arastorage_data_set[i].string_value, i);
			g_arastorage_tc_fail_count++;
			return;
		}
	}
#endif

	printf("PASS!\n");
}

void utc_arastorage_db_exec_tc_n(void)
{
	db_result_t res;
	char query[QUERY_LENGTH];

	printf("%d. db_exec Negative Unit Test started. Please wait...\n", g_arastorage_tc_count++);

	/* Try to create existing relation */
	memset(query, 0, QUERY_LENGTH);
	snprintf(query, QUERY_LENGTH, "CREATE RELATION %s;", RELATION_NAME);
	res = db_exec(query);
	if (DB_SUCCESS(res)) {
		printf("db_exec Failed : existing relation %s\n", RELATION_NAME);
		g_arastorage_tc_fail_count++;
		return;
	}

	/* Try to create existing attribute */
	memset(query, 0, QUERY_LENGTH);
	snprintf(query, QUERY_LENGTH, "CREATE ATTRIBUTE %s DOMAIN int IN %s;", g_attribute_set[0],
			 RELATION_NAME);
	res = db_exec(query);
	if (DB_SUCCESS(res)) {
		printf("db_exec Failed : existing attribute %s in %s\n", g_attribute_set[0], RELATION_NAME);
		g_arastorage_tc_fail_count++;
		return;
	}

	/* Try to excute db with NULL value */
	res = db_exec(NULL);
	if (DB_SUCCESS(res)) {
		printf("db_exec Failed with NULL value\n");
		g_arastorage_tc_fail_count++;
		return;
	}

	printf("PASS\n");
}

void utc_arastorage_db_query_tc_p(void)
{
	char query[QUERY_LENGTH];
	printf("%d. db_query Positive Unit Test started. Please wait...\n", g_arastorage_tc_count++);
	sleep(2);
#ifdef CONFIG_ARCH_FLOAT_H
	snprintf(query, QUERY_LENGTH, "SELECT %s, %s, %s, %s FROM %s WHERE %s > 0;",
			 g_attribute_set[0],
			 g_attribute_set[1], g_attribute_set[2], g_attribute_set[3], RELATION_NAME,
			 g_attribute_set[0]);

#else
	snprintf(query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 0;", g_attribute_set[0],
			 g_attribute_set[1], g_attribute_set[2], RELATION_NAME, g_attribute_set[0]);
#endif
	g_cursor = db_query(query);
	if (g_cursor == NULL) {
		printf("db_query Failed\n");
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS!\n");
}

void utc_arastorage_db_query_tc_n(void)
{
	char query[QUERY_LENGTH];
	char *name = "BAD_RELATION";
	printf("%d. db_query Negative Unit Test started. Please wait...\n", g_arastorage_tc_count++);

	snprintf(query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 5;", g_attribute_set[0],
			 g_attribute_set[1], g_attribute_set[2], name, g_attribute_set[0]);

	g_cursor = db_query(query);
	if (g_cursor != NULL) {
		printf("db_query Failed with wrong relation name\n");
		g_arastorage_tc_fail_count++;
		return;
	}

	g_cursor = db_query(NULL);
	if (g_cursor != NULL) {
		printf("db_query Failed with NULL value\n");
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS!\n");
}

void utc_arastorage_db_get_result_message_tc_p(void)
{
	db_result_t res;

	printf("%d. db_get_result_message Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_FINISHED)) == 0) {
		res = DB_FINISHED;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_OK)) == 0) {
		res = DB_OK;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_LIMIT_ERROR)) == 0) {
		res = DB_LIMIT_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG,
			   db_get_result_message(DB_ALLOCATION_ERROR)) == 0) {
		res = DB_ALLOCATION_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG,
			   db_get_result_message(DB_STORAGE_ERROR)) == 0) {
		res = DB_STORAGE_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG,
			   db_get_result_message(DB_RELATIONAL_ERROR)) == 0) {
		res = DB_RELATIONAL_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_TYPE_ERROR)) == 0) {
		res = DB_TYPE_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG,
			   db_get_result_message(DB_IMPLEMENTATION_ERROR)) == 0) {
		res = DB_IMPLEMENTATION_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_INDEX_ERROR)) == 0) {
		res = DB_INDEX_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_NAME_ERROR)) == 0) {
		res = DB_NAME_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_BUSY_ERROR)) == 0) {
		res = DB_BUSY_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG,
			   db_get_result_message(DB_INCONSISTENCY_ERROR)) == 0) {
		res = DB_INCONSISTENCY_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG,
			   db_get_result_message(DB_ARGUMENT_ERROR)) == 0) {
		res = DB_ARGUMENT_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_FULL_ERROR)) == 0) {
		res = DB_FULL_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(DB_CURSOR_ERROR)) == 0) {
		res = DB_CURSOR_ERROR;
		goto errout;
	}

	if (strcmp(DB_UNKNOWN_ERROR_MSG,
			   db_get_result_message(DB_EMPTY_CURSOR_ERROR)) == 0) {
		res = DB_EMPTY_CURSOR_ERROR;
		goto errout;
	}

	printf("PASS\n");
	return;
errout:
	printf("db_get_result_message Failed : %d\n", res);
	g_arastorage_tc_fail_count++;
}

void utc_arastorage_db_get_result_message_tc_n(void)
{
	int num;

	printf("%d. db_get_result_message Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	srand(18);
	num = rand();
	num -= 15;
	if (strcmp(DB_UNKNOWN_ERROR_MSG, db_get_result_message(num)) != 0) {
		printf("db_get_result_message Failed : %d\n", num);
		return;
	}
	printf("PASS\n");
	return;
}

void utc_arastorage_db_print_header_tc_p(void)
{
	db_result_t res;

	printf("%d. db_print_header Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);

	res = db_print_header(g_cursor);
	if (DB_ERROR(res)) {
		printf("db_print_header failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_db_print_header_tc_n(void)
{
	db_result_t res;

	printf("%d. db_print_header Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);

	res = db_print_header(NULL);
	if (DB_SUCCESS(res)) {
		printf("db_print_header failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_db_print_tuple_tc_p(void)
{
	db_result_t res;
	printf("%d. db_print_tuple Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);

	if (DB_SUCCESS(cursor_move_first(g_cursor))) {
		do {
			res = db_print_tuple(g_cursor);
			if (DB_ERROR(res)) {
				printf("db_print_tuple Failed : %d\n", res);
				g_arastorage_tc_fail_count++;
				return;
			}
		} while (DB_SUCCESS(cursor_move_next(g_cursor)));
	}
	printf("PASS\n");
}

void utc_arastorage_db_print_tuple_tc_n(void)
{
	db_result_t res;

	printf("%d. db_print_tuple Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	if (DB_SUCCESS(cursor_move_first(g_cursor))) {
		do {
			res = db_print_tuple(NULL);
			if (DB_SUCCESS(res)) {
				printf("db_print_tuple Failed : %d\n", res);
				g_arastorage_tc_fail_count++;
				return;
			}
		} while (DB_SUCCESS(cursor_move_next(g_cursor)));
	}
	printf("PASS\n");
}

void utc_arastorage_db_print_value_tc_p(void)
{
	db_result_t res;

	printf("%d. db_print_value Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = db_print_value(g_cursor, 2);
	if (DB_ERROR(res)) {
		printf("db_print_value Failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_db_print_value_tc_n(void)
{
	db_result_t res;

	printf("%d. db_print_value Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = db_print_value(NULL, 2);
	if (DB_SUCCESS(res)) {
		printf("db_print_value Failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}

	res = db_print_value(g_cursor, 4);
	if (DB_SUCCESS(res)) {
		printf("db_print_value Failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}


void utc_arastorage_db_cursor_free_tc_p(void)
{
	db_result_t res;
	printf("%d. db_cursor_free Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = db_cursor_free(g_cursor);
	if (DB_ERROR(res)) {
		printf("db_cursor_free failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	g_cursor = NULL;
	printf("PASS\n");
}

void utc_arastorage_db_cursor_free_tc_n(void)
{
	db_result_t res;
	printf("%d. db_cursor_free Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = db_cursor_free(NULL);
	if (DB_SUCCESS(res)) {
		printf("db_cursor_free with negative value failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	/* free cursor properly here to prevent memory leak */
	res = db_cursor_free(g_cursor);
	if (DB_ERROR(res)) {
		printf("db_cursor_free with correct data failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	g_cursor = NULL;
	printf("PASS\n");
}

void utc_arastorage_cursor_get_count_tc_p(void)
{
	tuple_id_t count;

	printf("%d. cursor_get_count Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	count = cursor_get_count(g_cursor);
	printf("cursor count : %d\n", count);
	if (count < 0) {
		printf("cursor_get_count Failed : count %d\n", count);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_get_count_tc_n(void)
{
	tuple_id_t count;
	printf("%d. cursor_get_count Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	count = cursor_get_count(NULL);
	printf("cursor count : %d\n", count);
	if (count != INVALID_CURSOR_VALUE) {
		printf("cursor_get_count Failed : count %d\n", count);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("cursor_get_count = [%d]\n", cursor_get_count(g_cursor));
	printf("PASS\n");
}

void utc_arastorage_cursor_move_first_tc_p(void)
{
	db_result_t res;

	printf("%d. cursor_move_first Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_first(g_cursor);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_ERROR(res) || cursor_get_row(g_cursor) != 0) {
		printf("cursor_move_first Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_first_tc_n(void)
{
	db_result_t res;
	printf("%d. cursor_move_first Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_first(NULL);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_SUCCESS(res)) {
		printf("cursor_move_first Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_next_tc_p(void)
{
	db_result_t res;

	printf("%d. cursor_move_next Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_next(g_cursor);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_ERROR(res) || cursor_get_row(g_cursor) != 1) {
		printf("cursor_move_next Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_next_tc_n(void)
{
	db_result_t res;

	printf("%d. cursor_move_next Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_next(NULL);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_SUCCESS(res)) {
		printf("cursor_move_next Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_prev_tc_p(void)
{
	db_result_t res;

	printf("%d. cursor_move_previous Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_prev(g_cursor);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_ERROR(res) || cursor_get_row(g_cursor) != 0) {
		printf("cursor_move_previous Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_prev_tc_n(void)
{
	db_result_t res;

	printf("%d. cursor_move_previous Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_prev(NULL);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_SUCCESS(res)) {
		printf("cursor_move_previous Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_last_tc_p(void)
{
	db_result_t res;

	printf("%d. cursor_move_last Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_last(g_cursor);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_ERROR(res) || cursor_get_row(g_cursor) != DATA_SET_NUM + DATA_SET_NUM - 1) {
		printf("cursor_move_last Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_last_tc_n(void)
{
	db_result_t res;

	printf("%d. cursor_move_last Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_last(NULL);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_SUCCESS(res)) {
		printf("cursor_move_last Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_to_tc_p(void)
{
	db_result_t res;
	tuple_id_t row = 3;
	printf("%d. cursor_move_to Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_to(g_cursor, row);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_ERROR(res) || cursor_get_row(g_cursor) != row) {
		printf("cursor_move_to Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_move_to_tc_n(void)
{
	db_result_t res;
	tuple_id_t row = 3;
	printf("%d. cursor_move_to Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_to(g_cursor, -1);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_SUCCESS(res) || cursor_get_row(g_cursor) == row) {
		printf("cursor_move_to Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	res = cursor_move_to(NULL, 0);
	printf("cursor row = %d\n", cursor_get_row(g_cursor));
	if (DB_SUCCESS(res)) {
		printf("cursor_move_to Failed : res %d row %d\n", res,
			   cursor_get_row(g_cursor));
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_get_row_tc_p(void)
{
	cursor_row_t row;
	tuple_id_t id = 3;
	printf("%d. cursor_get_row Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	row = cursor_get_row(g_cursor);
	if (row != id) {
		printf("cursor_get_row Failed : row %d id %d\n", row, id);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_get_row_tc_n(void)
{
	cursor_row_t row;
	tuple_id_t id = 3;
	printf("%d. cursor_get_row Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	row = cursor_get_row(g_cursor);
	if (row == id) {
		printf("cursor_get_row Failed : row %d id %d\n", row, id);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_is_first_row_tc_p(void)
{
	db_result_t res;

	printf("%d. cursor_is_first_row Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);

	res = cursor_move_first(g_cursor);
	if (DB_ERROR(res)) {
		printf("cursor_move_first Failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	if (!cursor_is_first_row(g_cursor)) {
		printf("cursor_is_first_row Failed\n");
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_is_first_row_tc_n(void)
{
	db_result_t res;
	char query[QUERY_LENGTH];

	printf("%d. cursor_is_first_row Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);

	/* We need to get valid cursor for this test */
	snprintf(query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 0;", g_attribute_set[0],
			 g_attribute_set[1], g_attribute_set[2], RELATION_NAME, g_attribute_set[0]);
	g_cursor = db_query(query);
	if (g_cursor == NULL) {
		printf("cursor_is_first_row Failed : db_query Fail\n");
		g_arastorage_tc_fail_count++;
		return;
	}
	res = cursor_move_last(g_cursor);
	if (DB_ERROR(res)) {
		printf("cursor_is_first_row Failed : res = %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}

	/* Cursor is valid, but it isn't pointing first row */
	if (cursor_is_first_row(g_cursor)) {
		printf("cursor_is_first_row Failed\n");
		g_arastorage_tc_fail_count++;
		return;
	}

	if (cursor_is_first_row(NULL)) {
		printf("cursor_is_first_row Failed\n");
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_is_last_row_tc_p(void)
{
	db_result_t res;

	printf("%d. cursor_is_last_row Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	res = cursor_move_last(g_cursor);
	if (DB_ERROR(res)) {
		printf("cursor_move_last Failed : %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}
	if (!cursor_is_last_row(g_cursor)) {
		printf("cursor_is_last_row Failed\n");
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_is_last_row_tc_n(void)
{
	db_result_t res;
	char query[QUERY_LENGTH];

	printf("%d. cursor_is_last_row Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);

	/* We need to get valid cursor for this test */
	snprintf(query, QUERY_LENGTH, "SELECT %s, %s, %s FROM %s WHERE %s > 0;", g_attribute_set[0],
			 g_attribute_set[1], g_attribute_set[2], RELATION_NAME, g_attribute_set[0]);
	g_cursor = db_query(query);
	if (g_cursor == NULL) {
		printf("cursor_is_last_row Failed : db_query Fail\n");
		g_arastorage_tc_fail_count++;
		return;
	}

	res = cursor_move_first(g_cursor);
	if (DB_ERROR(res)) {
		printf("cursor_is_last_row Failed : res = %d\n", res);
		g_arastorage_tc_fail_count++;
		return;
	}

	/* Cursor is valid, but it isn't pointing last row */
	if (cursor_is_last_row(NULL)) {
		printf("cursor_is_last_row Failed\n");
		g_arastorage_tc_fail_count++;
		return;
	}

	if (cursor_is_last_row(NULL)) {
		printf("cursor_is_last_row Failed\n");
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_get_attr_type_tc_p(void)
{
	domain_t domain;

	printf("%d. cursor_get_attr_type Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	domain = cursor_get_attr_type(g_cursor, 0);
	if (domain != DOMAIN_INT) {
		printf("cursor_get_attr_type Failed : %d\n", domain);
		g_arastorage_tc_fail_count++;
		return;
	}

	domain = cursor_get_attr_type(g_cursor, 1);
	if (domain != DOMAIN_LONG) {
		printf("cursor_get_attr_type Failed : %d\n", domain);
		g_arastorage_tc_fail_count++;
		return;
	}

	domain = cursor_get_attr_type(g_cursor, 2);
	if (domain != DOMAIN_STRING) {
		printf("cursor_get_attr_type Failed : %d\n", domain);
		g_arastorage_tc_fail_count++;
		return;
	}
#ifdef CONFIG_ARCH_FLOAT_H
	domain = cursor_get_attr_type(g_cursor, 3);
	if (domain != DOMAIN_DOUBLE) {
		printf("cursor_get_attr_type Failed : %d\n", domain);
		g_arastorage_tc_fail_count++;
		return;
	}

#endif
	printf("PASS\n");
}

void utc_arastorage_cursor_get_attr_type_tc_n(void)
{
	domain_t domain;

	printf("%d. cursor_get_attr_type Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	domain = cursor_get_attr_type(NULL, 0);
	if ((domain == DOMAIN_INT) || (domain == DOMAIN_LONG) || (domain == DOMAIN_STRING))
#ifdef CONFIG_ARCH_FLOAT_H
		if ((domain == DOMAIN_INT) || (domain == DOMAIN_LONG) || (domain == DOMAIN_STRING) || (domain == DOMAIN_DOUBLE))
#endif
		{
			printf("cursor_get_attr_type Failed : %d\n", domain);
			g_arastorage_tc_fail_count++;
			return;
		}

	domain = cursor_get_attr_type(g_cursor, 6);
	if ((domain == DOMAIN_INT) || (domain == DOMAIN_LONG) || (domain == DOMAIN_STRING))
#ifdef CONFIG_ARCH_FLOAT_H
		if ((domain == DOMAIN_INT) || (domain == DOMAIN_LONG) || (domain == DOMAIN_STRING) || (domain == DOMAIN_DOUBLE))
#endif
		{
			printf("cursor_get_attr_type Failed : %d\n", domain);
			g_arastorage_tc_fail_count++;
			return;
		}
	printf("PASS\n");
}

void utc_arastorage_cursor_get_attr_name_tc_p(void)
{
	char *attr_name;

	printf("%d. cursor_get_attr_name Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	attr_name = cursor_get_attr_name(g_cursor, 0);
	printf("attr name index 0 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[0]) != 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}

	attr_name = cursor_get_attr_name(g_cursor, 1);
	printf("attr name index 1 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[1]) != 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}

	attr_name = cursor_get_attr_name(g_cursor, 2);
	printf("attr name index 2 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[2]) != 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}
#ifdef CONFIG_ARCH_FLOAT_H
	attr_name = cursor_get_attr_name(g_cursor, 3);
	printf("attr name index 3 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[3]) != 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}
#endif
	printf("PASS\n");
}

void utc_arastorage_cursor_get_attr_name_tc_n(void)
{
	char *attr_name;

	printf("%d. cursor_get_attr_name Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	attr_name = cursor_get_attr_name(NULL, 0);
	printf("attr name index 0 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[0]) == 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}

	attr_name = cursor_get_attr_name(NULL, 1);
	printf("attr name index 1 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[1]) == 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}

	attr_name = cursor_get_attr_name(NULL, 2);
	printf("attr name index 2 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[2]) == 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}
	attr_name = cursor_get_attr_name(g_cursor, 3);
	printf("attr name index 3 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[3]) == 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}
#ifdef CONFIG_ARCH_FLOAT_H
	attr_name = cursor_get_attr_name(NULL, 3);
	printf("attr name index 3 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[3]) == 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}
	attr_name = cursor_get_attr_name(g_cursor, 4);
	printf("attr name index 3 : %s\n", attr_name);
	if (strcmp((const char *)attr_name, g_attribute_set[3]) == 0) {
		printf("cursor_get_attr_name Failed : %s\n", attr_name);
		g_arastorage_tc_fail_count++;
		return;
	}
#endif
	printf("PASS\n");
}

void utc_arastorage_cursor_get_attr_index_tc_p(void)
{
	attribute_id_t index;

	printf("%d. cursor_get_attr_index Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	index = cursor_get_attr_index(g_cursor, g_attribute_set[0]);
	printf("attr name %s index : %d\n", g_attribute_set[0], index);
	if (index != 0) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}

	index = cursor_get_attr_index(g_cursor, g_attribute_set[1]);
	printf("attr name %s index : %d\n", g_attribute_set[1], index);
	if (index != 1) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}

	index = cursor_get_attr_index(g_cursor, g_attribute_set[2]);
	printf("attr name %s index : %d\n", g_attribute_set[2], index);
	if (index != 2) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}

#ifdef CONFIG_ARCH_FLOAT_H
	index = cursor_get_attr_index(g_cursor, g_attribute_set[3]);
	printf("attr name %s index : %d\n", g_attribute_set[3], index);
	if (index != INVALID_CURSOR_VALUE) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}
#endif
	printf("PASS\n");
}

void utc_arastorage_cursor_get_attr_index_tc_n(void)
{
	attribute_id_t index;
	printf("%d. cursor_get_attr_index Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	index = cursor_get_attr_index(NULL, g_attribute_set[0]);
	printf("attr name %s index : %d\n", g_attribute_set[0], index);
	if (index == 0) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}

	index = cursor_get_attr_index(NULL, g_attribute_set[1]);
	printf("attr name %s index : %d\n", g_attribute_set[1], index);
	if (index == 1) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}

	index = cursor_get_attr_index(NULL, g_attribute_set[2]);
	printf("attr name %s index : %d\n", g_attribute_set[2], index);
	if (index == 2) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}

#ifdef CONFIG_ARCH_FLOAT_H
	index = cursor_get_attr_index(NULL, g_attribute_set[3]);
	printf("attr name %s index : %d\n", g_attribute_set[3], index);
	if (index == 3) {
		printf("cursor_get_attr_index Failed : %d\n", index);
		g_arastorage_tc_fail_count++;
		return;
	}
#endif
	printf("PASS\n");
}
void utc_arastorage_cursor_get_int_value_tc_p(void)
{
	int value;

	printf("%d. cursor_get_int_value Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_int_value(g_cursor, 0);
	if (value < 0) {
		printf("cursor_get_int_value of index 0 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %d\n", value);

	value = cursor_get_int_value(g_cursor, 1);
	if (value >= 0) {
		printf("cursor_get_int_value of index 1 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}

	printf("PASS\n");
}

void utc_arastorage_cursor_get_int_value_tc_n(void)
{
	int value;

	printf("%d. cursor_get_int_value Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_int_value(NULL, 0);
	if (value >= 0) {
		printf("cursor_get_int_value of index 0 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %d\n", value);

	value = cursor_get_int_value(g_cursor, 1);
	if (value >= 0) {
		printf("cursor_get_int_value of index 1 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}

	printf("PASS\n");
}

void utc_arastorage_cursor_get_long_value_tc_p(void)
{
	long value;

	printf("%d. cursor_get_long_value Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_long_value(g_cursor, 1);
	if (value < 0) {
		printf("cursor_get_long_value of index 1 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %ld\n", value);

	value = cursor_get_long_value(g_cursor, 2);
	if (value >= 0) {
		printf("cursor_get_long_value of index 2 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}

	printf("PASS\n");
}

void utc_arastorage_cursor_get_long_value_tc_n(void)
{
	long value;

	printf("%d. cursor_get_long_value Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_long_value(NULL, 1);
	if (value >= 0) {
		printf("cursor_get_long_value of index 1 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %ld\n", value);

	value = cursor_get_long_value(g_cursor, 2);
	if (value >= 0) {
		printf("cursor_get_long_value of index 2 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}

	printf("PASS\n");
}


#ifdef CONFIG_ARCH_FLOAT_H
void utc_arastorage_cursor_get_double_value_tc_p(void)
{
	double value;

	printf("%d. cursor_get_double_value Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_double_value(g_cursor, 3);
	if (value < 0) {
		printf("cursor_get_double_value of index 3 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %f\n", value);

	value = cursor_get_double_value(g_cursor, 2);
	if (value >= 0) {
		printf("cursor_get_double_value of index 2 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}

	printf("PASS\n");
}


void utc_arastorage_cursor_get_double_value_tc_n(void)
{
	double value;

	printf("%d. cursor_get_double_value Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_double_value(NULL, 3);
	if (value >= 0) {
		printf("cursor_get_double_value of index 3 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %f\n", value);

	value = cursor_get_double_value(g_cursor, 2);
	if (value >= 0) {
		printf("cursor_get_double_value of index 2 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}

	printf("PASS\n");
}
#endif

void utc_arastorage_cursor_get_string_value_tc_p(void)
{
	unsigned char *value;

	printf("%d. cursor_get_string_value Positive Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_string_value(g_cursor, 2);
	if (value == NULL) {
		printf("cursor_get_string_value of index 2 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %s\n", value);

	value = cursor_get_string_value(g_cursor, 0);
	if (value != NULL) {
		printf("cursor_get_string_value of index 0 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

void utc_arastorage_cursor_get_string_value_tc_n(void)
{
	unsigned char *value;

	printf("%d. cursor_get_string_value Negative Unit Test started. Please wait...\n",
		   g_arastorage_tc_count++);
	value = cursor_get_string_value(NULL, 2);
	if (value != NULL) {
		printf("cursor_get_string_value of index 2 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("value = %s\n", value);

	value = cursor_get_string_value(g_cursor, 0);
	if (value != NULL) {
		printf("cursor_get_string_value of index 0 Failed : %d\n", value);
		g_arastorage_tc_fail_count++;
		return;
	}
	printf("PASS\n");
}

int arastorage_sample_launcher(int argc, FAR char *argv[])
{

#ifdef CONFIG_FS_SMARTFS
	if (fs_erase("/dev/smart1") != OK) {
		printf("Error erasing file system, STOP TEST!!\n");
		return ERROR;
	}

	if (fs_initiate("/dev/smart1", "smartfs") != OK) {
		printf("Error initiating file system, STOP TEST!!\n");
		return ERROR;
	}
	printf("File system initiated!! START TEST!!\n");
#endif
	g_arastorage_tc_count = 1;
	g_arastorage_tc_fail_count = 0;

	utc_arastorage_db_init_tc_p();
	utc_arastorage_db_exec_tc_p();
	utc_arastorage_db_query_tc_p();
	utc_arastorage_db_get_result_message_tc_p();
	utc_arastorage_db_print_header_tc_p();
	utc_arastorage_db_print_tuple_tc_p();
	utc_arastorage_db_print_value_tc_p();
	utc_arastorage_cursor_get_count_tc_p();
	utc_arastorage_cursor_move_first_tc_p();
	utc_arastorage_cursor_move_next_tc_p();
	utc_arastorage_cursor_move_prev_tc_p();
	utc_arastorage_cursor_move_last_tc_p();
	utc_arastorage_cursor_move_to_tc_p();
	utc_arastorage_cursor_get_row_tc_p();
	utc_arastorage_cursor_is_first_row_tc_p();
	utc_arastorage_cursor_is_last_row_tc_p();
	utc_arastorage_cursor_get_attr_type_tc_p();
	utc_arastorage_cursor_get_attr_name_tc_p();
	utc_arastorage_cursor_get_attr_index_tc_p();
	utc_arastorage_cursor_get_int_value_tc_p();
	utc_arastorage_cursor_get_long_value_tc_p();
#ifdef CONFIG_ARCH_FLOAT_H
	utc_arastorage_cursor_get_double_value_tc_p();
#endif
	utc_arastorage_cursor_get_string_value_tc_p();
	utc_arastorage_db_cursor_free_tc_p();
	utc_arastorage_db_deinit_tc_p();

	printf("#########################################\n");
	printf("         Arastorage Positive TC Result            \n");
	printf("         Total TC : %d              \n", --g_arastorage_tc_count);
	printf("         PASS : %d FAIL : %d        \n",
		   (g_arastorage_tc_count - g_arastorage_tc_fail_count),
		   g_arastorage_tc_fail_count);
	printf("#########################################\n");

	g_arastorage_tc_count = 1;
	g_arastorage_tc_fail_count = 0;
	db_init();
	utc_arastorage_db_exec_tc_n();
	utc_arastorage_db_query_tc_n();
	utc_arastorage_db_get_result_message_tc_n();
	utc_arastorage_db_print_header_tc_n();
	utc_arastorage_db_print_tuple_tc_n();
	utc_arastorage_db_print_value_tc_n();
	utc_arastorage_cursor_get_count_tc_n();
	utc_arastorage_cursor_move_first_tc_n();
	utc_arastorage_cursor_move_next_tc_n();
	utc_arastorage_cursor_move_prev_tc_n();
	utc_arastorage_cursor_move_last_tc_n();
	utc_arastorage_cursor_move_to_tc_n();
	utc_arastorage_cursor_get_row_tc_n();
	utc_arastorage_cursor_is_first_row_tc_n();
	utc_arastorage_cursor_is_last_row_tc_n();
	utc_arastorage_cursor_get_attr_type_tc_n();
	utc_arastorage_cursor_get_attr_name_tc_n();
	utc_arastorage_cursor_get_attr_index_tc_n();
	utc_arastorage_cursor_get_int_value_tc_n();
	utc_arastorage_cursor_get_long_value_tc_n();
#ifdef CONFIG_ARCH_FLOAT_H
	utc_arastorage_cursor_get_double_value_tc_n();
#endif
	utc_arastorage_cursor_get_string_value_tc_n();
	utc_arastorage_db_cursor_free_tc_n();
	db_deinit();


	printf("#########################################\n");
	printf("         Arastorage Negative TC Result            \n");
	printf("         Total TC : %d              \n", --g_arastorage_tc_count);
	printf("         PASS : %d FAIL : %d        \n",
		   (g_arastorage_tc_count - g_arastorage_tc_fail_count),
		   g_arastorage_tc_fail_count);
	printf("#########################################\n");
	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_arastorage_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TASH
	tash_cmd_install("arastorage_utc", arastorage_sample_launcher, TASH_EXECMD_SYNC);
#else
	arastorage_sample_launcher(argc, argv);
#endif
	return 0;

}
