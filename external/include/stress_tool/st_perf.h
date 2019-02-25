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

#ifndef __ST_PERF_H__
#define __ST_PERF_H__

#include <sys/time.h>

#define COLOR_SMOKE			"\e[33m"
#define COLOR_RESULT		"\e[36m"
#define COLOR_WHITE			"\e[m"

/*
 * Initializer Macro
 */
#define ST_PERF_INITIALIZER {{0, 0}, {0, 0}, 0, 0, 0, 0, 0, STRESS_TC_PASS}
#define ST_STAB_INITIALIZER {0, 0, 0, 0, STRESS_TC_PASS}

/*
 * Test case Macro
 */
#define TEST_SETUP(tc_name)								\
	static st_tc_result tc_##tc_name##_setup(void *arg)

#define TEST_TEARDOWN(tc_name)								\
	static st_tc_result tc_##tc_name##_teardown(void *arg)

#define TEST_F(tc_name)							\
	static st_tc_result tc_##tc_name(void *arg)

#define ST_SET_FUNC(tc_desc, tc_name)									\
	static st_func g_##tc_name = {tc_desc, tc_##tc_name##_setup, tc_##tc_name##_teardown, tc_##tc_name}

#define ST_SET_PERF(expect, tc_name)									\
	static st_performance g_perf_##tc_name = {expect, ST_PERF_INITIALIZER}

#define ST_SET_STAB(tc_name)										\
	static st_stability g_stab_##tc_name = {ST_STAB_INITIALIZER}

#define ST_GET_SMOKE(tc_name) g_smoke_##tc_name

#define ST_SET_SMOKE_UNIT(repeat, tc_name, tc_next)						\
	static st_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), &(g_smoke_##tc_next)}

#define ST_SET_SMOKE_TAIL_UNIT(repeat, tc_name)							\
	static st_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), NULL}

#define ST_SET_SMOKE(repeat, expect, tc_desc, tc_name, tc_next)			\
	ST_SET_FUNC(tc_desc, tc_name);										\
	ST_SET_PERF(expect, tc_name);										\
	ST_SET_STAB(tc_name);												\
	static st_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), &(g_smoke_##tc_next)}

#define ST_SET_SMOKE_TAIL(repeat, expect, tc_desc, tc_name)				\
	ST_SET_FUNC(tc_desc, tc_name);										\
	ST_SET_PERF(expect, tc_name);										\
	ST_SET_STAB(tc_name);												\
	static st_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), NULL}

#define ST_SET_PACK(fixture, entry)							\
	static st_pack g_pack_##fixture = {&g_smoke_##entry}

#define ST_GET_PACK(fixture) g_pack_##fixture

#define ST_RUN_TEST(fixture) perf_run(&g_pack_##fixture)

#define ST_RESULT_TEST(fixture) perf_print_result(&g_pack_##fixture)
/*
 * Log
 */
#define ST_ERROR														\
	do {																\
		printf(COLOR_SMOKE "[ERROR] F:%s\tFile:%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
		printf(COLOR_WHITE);											\
	} while (0)

#define ST_START_LOG													\
	do {																\
		printf(COLOR_WHITE "[INFO] Start F:%s\tFile:%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
	} while (0)

#define ST_END_LOG														\
	do {																\
		printf(COLOR_WHITE "[INFO] End F:%s\tFile:%s:%d\n\n", __FUNCTION__, __FILE__, __LINE__); \
	} while (0)

/*
 * Expect
 */
#define ST_EXPECT(val, exp)						\
	do {										\
		if (exp != val) {						\
			ST_ERROR;							\
			res = STRESS_TC_FAIL;				\
		}										\
	} while (0)

#define ST_EXPECT_2(val1, val2, exp)			\
	do {										\
		int ret = exp;							\
		if (ret != val1 && ret != val2) {		\
			ST_ERROR;							\
			res = STRESS_TC_FAIL;				\
		}										\
	} while (0)

#define ST_EXPECT_NEQ(val, exp)					\
	do {										\
		if ((exp) == val) {						\
			ST_ERROR;							\
			res = STRESS_TC_FAIL;				\
		}										\
	} while (0)

#define ST_EXPECT_LT(val, exp)					\
	do {										\
		if ((exp) >= val) {						\
			ST_ERROR;							\
			res = STRESS_TC_FAIL;				\
		}										\
	} while (0)

#define ST_EXPECT_LE(val, exp)					\
	do {										\
		if (exp > val) {						\
			ST_ERROR;							\
			res = STRESS_TC_FAIL;				\
		}										\
	} while (0)

#define ST_EXPECT_GT(val, exp)					\
	do {										\
		if (exp <= val) {						\
			ST_ERROR;							\
			res = STRESS_TC_FAIL;				\
		}										\
	} while (0)

#define ST_EXPECT_GE(val, exp)					\
	do {										\
		if (exp < val) {						\
			ST_ERROR;							\
			res = STRESS_TC_FAIL;				\
		}										\
	} while (0)

#define ST_START_TEST									\
	st_tc_result res = STRESS_TC_PASS;					\
	struct timeval start, end;							\
	st_elapsed_time *timer = (st_elapsed_time *)arg;	\
	do {												\
		ST_START_LOG;									\
		if (timer) {									\
			gettimeofday(&start, NULL);					\
		}												\
	} while (0)

#define ST_END_TEST								\
	do {										\
		if (timer) {							\
			gettimeofday(&end, NULL);			\
			timer->start.second = start.tv_sec;	\
			timer->start.micro = start.tv_usec;	\
			timer->end.second = end.tv_sec;		\
			timer->end.micro = end.tv_usec;		\
		}										\
			ST_END_LOG;							\
			return res;							\
	} while (0)


/*
 * Structures
 */
typedef enum {
	STRESS_TC_PASS,
	STRESS_TC_FAIL,
	STRESS_TC_SKIP,
	STRESS_TC_PERF_FAIL
} st_tc_result;

typedef st_tc_result(*st_unit_tc)(void *arg);

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
	unsigned int max;	// milli second
	unsigned int min;	// milli second
	unsigned int sum;	// milli second
	unsigned int fail;  // the number of performance fails
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
	unsigned int expect;	// performance limit: milli second
	st_performance_stat stat;
} st_performance;


typedef struct _st_stability {
	st_stability_stat stat;
} st_stability;



typedef struct _st_func {
	char *tc_name;
	st_unit_tc setup;
	st_unit_tc teardown;
	st_unit_tc tc;
	void *arg;
} st_func;


typedef struct _st_smoke {
	int repeat_size;
	st_performance *performance;
	st_stability *stability;
	st_func *func;
	struct _st_smoke *next;
} st_smoke;


typedef struct _st_pack {
	st_smoke *head;
//	st_smoke *tail;
} st_pack;


/*
 * Functions
 */
void perf_run(st_pack *pack);
void perf_print_result(st_pack *pack);
void perf_add_expect_performance(st_smoke *smoke, unsigned int usec);

#endif //__ST_PERF_H__
