/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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

#pragma once

#define ST_NO_TIMELIMIT 0

/*
 * Initializer Macro
 */
#define ST_PERF_INITIALIZER                           \
	{                                                 \
		{0, 0}, {0, 0}, 0, 0, 0, 0, 0, STRESS_TC_PASS \
	}
#define ST_STAB_INITIALIZER        \
	{                              \
		0, 0, 0, 0, STRESS_TC_PASS \
	}

/*
 * Internal Test case Macro
 * API which are used in internal only
 */
#define ST_START_TEST                                \
	st_tc_result st_res = STRESS_TC_PASS;            \
	struct timeval start, end;                       \
	st_elapsed_time *timer = (st_elapsed_time *)arg; \
	do {                                             \
		if (timer) {                                 \
			if (gettimeofday(&start, NULL) == -1) {  \
				st_res = STRESS_TC_PERF_FAIL;        \
				goto STFUNC_OUT;                     \
			}                                        \
		}                                            \
	} while (0)

#define ST_END_TEST                               \
	STFUNC_OUT:                                   \
	do {                                          \
		if (timer) {                              \
			if (gettimeofday(&end, NULL) == -1) { \
				return STRESS_TC_PERF_FAIL;       \
			}                                     \
			timer->start.second = start.tv_sec;   \
			timer->start.micro = start.tv_usec;   \
			timer->end.second = end.tv_sec;       \
			timer->end.micro = end.tv_usec;       \
		}                                         \
		return st_res;                            \
	} while (0)

/* #define ST_SET_FUNC(tc_desc, tc_name) \ */
/* 	static st_func g_##tc_name = {tc_desc, tc_##tc_name##_setup, tc_##tc_name##_teardown, tc_##tc_name} */

/* #define ST_SET_PERF(expect, tc_name) \ */
/* 	static st_performance g_perf_##tc_name = {expect, ST_PERF_INITIALIZER} */

/* #define ST_SET_STAB(tc_name) \ */
/* 	static st_stability g_stab_##tc_name = {ST_STAB_INITIALIZER} */

/* #define ST_GET_SMOKE(tc_name) g_smoke_##tc_name */

/* #define ST_SET_SMOKE_UNIT(repeat, tc_name, tc_next) \ */
/* 	static st_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), &(g_smoke_##tc_next)} */

/* #define ST_SET_SMOKE_TAIL_UNIT(repeat, tc_name) \ */
/* 	static st_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), NULL} */

/*
 * Structures
 */
typedef enum {
	STRESS_TC_PASS,
	STRESS_TC_FAIL,
	STRESS_TC_SKIP,
	STRESS_TC_PERF_FAIL,
	STRESS_TC_SETUP_FAIL,
	STRESS_TC_TEARDOWN_FAIL,
} st_tc_result;

typedef st_tc_result (*st_unit_tc)(void *arg);

typedef struct _st_performance_time {
	unsigned int second;
	unsigned int micro;
} st_performance_time;

typedef struct _st_elapsed_time {
	st_performance_time start;
	st_performance_time end;
} st_elapsed_time;

/*  developer are not supposed to modify it */
typedef struct _st_performance_stat {
	st_performance_time start;
	st_performance_time end;
	unsigned int count;
	unsigned int max;			// milli second
	unsigned int min;			// milli second
	unsigned int sum;			// milli second
	unsigned int fail;			// the number of performance fails
	unsigned int total_elapsed; // total elapsed time during testing count times.
	st_tc_result result;
} st_performance_stat;

/*  developer are not supposed to modify it */
typedef struct _st_stability_stat {
	int count;
	int pass;
	int fail;
	int skip;
	st_tc_result result;
} st_stability_stat;

typedef struct _st_performance {
	unsigned int expect; // performance limit: milli second
	st_performance_stat stat;
} st_performance;

typedef struct _st_stability {
	st_stability_stat stat;
} st_stability;

typedef struct _st_func {
	char *tc_name;
	st_unit_tc setup;
	st_unit_tc teardown;
	st_unit_tc init;
	st_unit_tc deinit;
	st_unit_tc tc;
	void *arg;
} st_func;

typedef struct _st_smoke {
	int repeat_size;
	st_performance *performance;
	st_stability *stability;
	st_func *func;
	sq_entry_t entry;
} st_smoke;

typedef struct _st_pack {
	sq_queue_t queue;
	st_unit_tc setup;
	st_unit_tc teardown;
	char *title;
} st_pack;

/*
 * Functions
 */
void perf_run(st_pack *pack);
void perf_add_expect_performance(st_smoke *smoke, unsigned int usec);

void perf_initialize(st_pack *pack);
void perf_add_item(st_pack *pack, int repeat, char *tc_desc,
				   st_unit_tc func_init, st_unit_tc func_deinit,
				   st_unit_tc func_setup, st_unit_tc func_teardown, st_unit_tc func, unsigned int expect);
void perf_add_global(st_pack *pack, st_unit_tc global_setup, st_unit_tc global_teardown, const char *title);
void perf_set_keeprunning(int enable);
/******************************************
 * API
 *******************************************/
/*
 * Expect?
 */

#define ST_EXPECT_EQ(exp, val)                                    \
	do {                                                            \
		uint32_t tmp_val = val;                                       \
		if (exp != tmp_val) {                                         \
			printf("\t[ERROR] val (%d) exp (%d)\n", tmp_val, exp);      \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			st_res = STRESS_TC_FAIL;                                    \
			goto STFUNC_OUT;                                            \
		}                                                             \
	} while (0)

#define ST_ASSERT_EQ(exp, val)                                    \
	do {                                                            \
		uint32_t tmp_val = val;                                       \
		if (exp != tmp_val) {                                         \
			printf("\t[ERROR] val (%d) exp (%d)\n", tmp_val, exp);      \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			assert(0);                                                  \
		}                                                             \
	} while (0)

#define ST_ASSERT_NEQ(exp, val)                                   \
	do {                                                            \
		uint32_t tmp_val = val;                                       \
		if (exp == tmp_val) {                                         \
			printf("\t[ERROR] val (%d) exp (%d)\n", tmp_val, exp);      \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			assert(0);                                                  \
		}                                                             \
	} while (0)

#define ST_EXPECT_EQ2(val1, val2, exp)                                     \
	do {                                                                     \
		if (exp != val1 && exp != val2) {                                      \
			printf("\t[ERROR] val1 (%d) val2 (%d) exp (%d)\n", val1, val2, exp); \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__);          \
			st_res = STRESS_TC_FAIL;                                             \
			goto STFUNC_OUT;                                                     \
		}                                                                      \
	} while (0)

#define ST_EXPECT_NEQ(val, exp)                                   \
	do {                                                            \
		if ((exp) == val) {                                           \
			printf("\t[ERROR] val (%d) exp (%d)\n", val, exp);          \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			goto STFUNC_OUT;                                            \
		}                                                             \
	} while (0)

#define ST_EXPECT_LT(val, exp)                                    \
	do {                                                            \
		if ((exp) >= val) {                                           \
			printf("\t[ERROR] val (%d) exp (%d)\n", val, exp);          \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			st_res = STRESS_TC_FAIL;                                    \
			goto STFUNC_OUT;                                            \
		}                                                             \
	} while (0)

#define ST_EXPECT_LE(val, exp)                                    \
	do {                                                            \
		if (exp > val) {                                              \
			printf("\t[ERROR] val (%d) exp (%d)\n", val, exp);          \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			st_res = STRESS_TC_FAIL;                                    \
			goto STFUNC_OUT;                                            \
		}                                                             \
	} while (0)

#define ST_EXPECT_GT(val, exp)                                    \
	do {                                                            \
		if (exp <= val) {                                             \
			printf("\t[ERROR] val (%d) exp (%d)\n", val, exp);          \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			st_res = STRESS_TC_FAIL;                                    \
			goto STFUNC_OUT;                                            \
		}                                                             \
	} while (0)

#define ST_EXPECT_GE(val, exp)                                    \
	do {                                                            \
		if (exp < val) {                                              \
			printf("\t[ERROR] val (%d) exp (%d)\n", val, exp);          \
			printf("\t %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
			st_res = STRESS_TC_FAIL;                                    \
			goto STFUNC_OUT;                                            \
		}                                                             \
	} while (0)

/*
 * Testcase definitions
 */

/*
 * Description: Define testsuite & do not set global configuration
 */
#define ST_SET_PACK_GLOBAL(testsuite) \
	static st_pack g_pack_##testsuite;

/*
 * Description: Define testsuite
 */
#define ST_SET_PACK(testsuite)         \
	static st_pack g_pack_##testsuite; \
	perf_add_global(&g_pack_##testsuite, NULL, NULL, #testsuite);

/*
 * Description: Add testcase(smoke) to testsuite(testsuite)
 *
 * Note: testcase is composed to setup, teardown and testcase
 */
#define ST_SET_SMOKE(testsuite, repeat, expect, tc_desc, tc_name)                          \
	perf_add_item(&g_pack_##testsuite, repeat, #tc_name,                                   \
				  NULL, NULL, tc_##tc_name##_setup, tc_##tc_name##_teardown, tc_##tc_name, \
				  expect)

/*
 * Description: Add testcase(smoke) to testsuite(testsuite)
 *
 * Note: testcase is composed to testcase only
 */
#define ST_SET_SMOKE1(testsuite, repeat, expect, tc_desc, tc_name) \
	perf_add_item(&g_pack_##testsuite, repeat, #tc_name,           \
				  NULL, NULL, NULL, NULL, tc_##tc_name,            \
				  expect)

/*
 * Description: Add testcase(smoke) to testsuite(testsuite)
 *
 * Note: testcase is composed to setup, teardown and testcase
 *       reuse_tc is for sharing setup and teardown code among testcases.
 */
#define ST_TC_SET_SMOKE(testsuite, repeat, expect, tc_desc, reuse_tc, tc_name)               \
	perf_add_item(&g_pack_##testsuite, repeat, #tc_name,                                     \
				  NULL, NULL, tc_##reuse_tc##_setup, tc_##reuse_tc##_teardown, tc_##tc_name, \
				  expect)

/*
 * Description: Define setup and teardown for testsuite
 *
 * Note: global_config is setup and teardown for testsuite
 *       this will be called only a time during test.
 */
#define ST_TC_SET_GLOBAL(testsuite, global_config) \
	perf_add_global(&g_pack_##testsuite, tc_##global_config##_setup, tc_##global_config##_teardown, #testsuite)

/*
 * Description: Run testsuite
 */
#define ST_RUN_TEST(testsuite) perf_run(&g_pack_##testsuite)

/*
 * Description: Run testsuite
 *
 * Note: deprecated. ST_RUN_TEST prints the result of testsuite
 */
#define ST_RESULT_TEST(testsuite)

/*
 * Description: Set stress test style
 *
 * Note: Keep stress test running even it failed.
 */
#define ST_SET_KEEP_RUNNING \
	perf_set_keeprunning(1);
#define ST_RESET_KEEP_RUNNING \
	perf_set_keeprunning(0);

/*
 * Description: define setup
 *
 * Notes: deprecated. replaced to TESTCASE_SETUP
 */
#define TEST_SETUP(tc_name) \
	static st_tc_result tc_##tc_name##_setup(void *arg)

/*
 * Description: define teardown.
 *
 * Notes: deprecated. replaced to TESTCASE_TEARDOWN
 */
#define TEST_TEARDOWN(tc_name) \
	static st_tc_result tc_##tc_name##_teardown(void *arg)

/*
 * Description: define testcase
 *
 * Notes: deprecated. use START_TEST_F
 */
#define TEST_F(tc_name) \
	static st_tc_result tc_##tc_name(void *arg)

/*
 * Description: define testcase
 */
#define START_TEST_F(tc_name)                   \
	static st_tc_result tc_##tc_name(void *arg) \
	{                                           \
		ST_START_TEST;

/*
 * Description: define testcase
 */
#define END_TEST_F \
	ST_END_TEST;   \
	}

/*
 * Description: define setup
 */
#define TESTCASE_SETUP(testcase)                         \
	static st_tc_result tc_##testcase##_setup(void *arg) \
	{                                                    \
		ST_START_TEST;

/*
 * Description: define teardown
 */
#define TESTCASE_TEARDOWN(testcase)                         \
	static st_tc_result tc_##testcase##_teardown(void *arg) \
	{                                                       \
		ST_START_TEST;

#define END_TESTCASE \
	ST_END_TEST;     \
	}
