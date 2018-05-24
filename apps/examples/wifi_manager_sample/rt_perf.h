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

#ifndef __RT_PERF_H__
#define __RT_PERF_H__

#include <sys/time.h>

#define COLOR_SMOKE			"\e[33m"
#define COLOR_RESULT		"\e[36m"
#define COLOR_WHITE			"\e[m"

/*
 * Initializer Macro
 */
#define RT_PERF_INITIALIZER {{0, 0}, {0, 0}, 0, 0, 0, 0, 0, DA_TC_SKIP}
#define RT_STAB_INITIALIZER {0, 0, 0, 0, DA_TC_SKIP}

/*
 * Test case Macro
 */
#define TEST_SETUP(tc_name)								\
	static rt_tc_result tc_##tc_name##_setup(void *arg)

#define TEST_TEARDOWN(tc_name)								\
	static rt_tc_result tc_##tc_name##_teardown(void *arg)

#define TEST_F(tc_name)							\
	static rt_tc_result tc_##tc_name(void *arg)

#define RT_SET_FUNC(tc_desc, tc_name)									\
	static rt_func g_##tc_name = {tc_desc, tc_##tc_name##_setup, tc_##tc_name##_teardown, tc_##tc_name}

#define RT_SET_PERF(expect, tc_name)									\
	static rt_performance g_perf_##tc_name = {expect, RT_PERF_INITIALIZER}

#define RT_SET_STAB(tc_name)										\
	static rt_stability g_stab_##tc_name = {RT_STAB_INITIALIZER}

#define RT_GET_SMOKE(tc_name) g_smoke_##tc_name

#define RT_SET_SMOKE_UNIT(repeat, tc_name, tc_next)						\
	static rt_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), &(g_smoke_##tc_next)}

#define RT_SET_SMOKE_TAIL_UNIT(repeat, tc_name)							\
	static rt_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), NULL}

#define RT_SET_SMOKE(repeat, expect, tc_desc, tc_name, tc_next)			\
	RT_SET_FUNC(tc_desc, tc_name);										\
	RT_SET_PERF(expect, tc_name);										\
	RT_SET_STAB(tc_name);												\
	static rt_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), &(g_smoke_##tc_next)}

#define RT_SET_SMOKE_TAIL(repeat, expect, tc_desc, tc_name)				\
	RT_SET_FUNC(tc_desc, tc_name);										\
	RT_SET_PERF(expect, tc_name);										\
	RT_SET_STAB(tc_name);												\
	static rt_smoke g_smoke_##tc_name = {repeat, &(g_perf_##tc_name), &(g_stab_##tc_name), &(g_##tc_name), NULL}

#define RT_SET_PACK(fixture, entry)							\
	static rt_pack g_pack_##fixture = {&g_smoke_##entry}

#define RT_GET_PACK(fixture) g_pack_##fixture

#define RT_RUN_TEST(fixture) perf_run(&g_pack_##fixture)

#define RT_RESULT_TEST(fixture) perf_print_result(&g_pack_##fixture)
/*
 * Log
 */
#define RT_ERROR														\
	do {																\
		printf(COLOR_SMOKE "[ERROR] F:%s\tFile:%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
		printf(COLOR_WHITE);											\
	} while (0)

#define RT_START_LOG													\
	do {																\
		printf(COLOR_WHITE "[INFO] Start F:%s\tFile:%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
	} while (0)

#define RT_END_LOG														\
	do {																\
		printf(COLOR_WHITE "[INFO] End F:%s\tFile:%s:%d\n\n", __FUNCTION__, __FILE__, __LINE__); \
	} while (0)

/*
 * Expect
 */
#define RT_EXPECT(val, exp)						\
	do {										\
		if (exp != val) {						\
			RT_ERROR;							\
			res = DA_TC_FAIL;					\
		}										\
	} while (0)

#define RT_EXPECT_LT(val, exp)					\
	do {										\
		if ((exp) >= val) {						\
			RT_ERROR;							\
			res = DA_TC_FAIL;					\
		}										\
	} while (0)

#define RT_EXPECT_LE(val, exp)					\
	do {										\
		if (exp > val) {						\
			RT_ERROR;							\
			res = DA_TC_FAIL;					\
		}										\
	} while (0)

#define RT_EXPECT_GT(val, exp)					\
	do {										\
		if (exp <= val) {						\
			RT_ERROR;							\
			res = DA_TC_FAIL;					\
		}										\
	} while (0)

#define RT_EXPECT_GE(val, exp)					\
	do {										\
		if (exp < val) {						\
			RT_ERROR;							\
			res = DA_TC_FAIL;					\
		}										\
	} while (0)

#define RT_START_TEST									\
	rt_tc_result res = DA_TC_PASS;						\
	struct timeval start, end;							\
	rt_elapsed_time *timer = (rt_elapsed_time *)arg;	\
	do {												\
		RT_START_LOG;									\
		if (timer) {									\
			gettimeofday(&start, NULL);					\
		}												\
	} while (0)

#define RT_END_TEST								\
	do {										\
		if (timer) {							\
			gettimeofday(&end, NULL);			\
			timer->start.second = start.tv_sec;	\
			timer->start.micro = start.tv_usec;	\
			timer->end.second = end.tv_sec;		\
			timer->end.micro = end.tv_usec;		\
		}										\
		RT_END_LOG;								\
		return res;								\
	} while (0)


/*
 * Structures
 */
typedef enum {
	DA_TC_PASS,
	DA_TC_FAIL,
	DA_TC_SKIP,
	DA_TC_PERF_FAIL
} rt_tc_result;

typedef rt_tc_result(*rt_unit_tc)(void *arg);

typedef struct _rt_performance_time {
	unsigned int second;
	unsigned int micro;
} rt_performance_time;

typedef struct _rt_elapsed_time {
	rt_performance_time start;
	rt_performance_time end;
} rt_elapsed_time;

/*  developer are not supposed to modify it */
typedef struct _rt_performance_stat {
	rt_performance_time start;
	rt_performance_time end;
	unsigned int count;
	unsigned int max;	// milli second
	unsigned int min;	// milli second
	unsigned int sum;	// milli second
	unsigned int fail;  // the number of performance fails
	rt_tc_result result;
} rt_performance_stat;

/*  developer are not supposed to modify it */
typedef struct _rt_stability_stat {
	int count;
	int pass;
	int fail;
	int skip;
	rt_tc_result result;
} rt_stability_stat;


typedef struct _rt_performance {
	unsigned int expect;	// performance limit: milli second
	rt_performance_stat stat;
} rt_performance;


typedef struct _rt_stability {
	rt_stability_stat stat;
} rt_stability;



typedef struct _rt_func {
	char *tc_name;
	rt_unit_tc setup;
	rt_unit_tc teardown;
	rt_unit_tc tc;
	void *arg;
} rt_func;


typedef struct _rt_smoke {
	int repeat_size;
	rt_performance *performance;
	rt_stability *stability;
	rt_func *func;
	struct _rt_smoke *next;
} rt_smoke;


typedef struct _rt_pack {
	rt_smoke *head;
//	rt_smoke *tail;
} rt_pack;


/*
 * Functions
 */
void perf_run(rt_pack *pack);
void perf_print_result(rt_pack *pack);
void perf_add_expect_performance(rt_smoke *smoke, unsigned int usec);

#endif //__RT_PERF_H__
