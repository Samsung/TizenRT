/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>

#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <queue.h>
#include <stress_tool/st_perf.h>

typedef enum {
	ST_COLOR_DEFAULT,
	ST_COLOR_RED,
	ST_COLOR_GREEN,
	ST_COLOR_YELLOW,
} st_color_e;

static const char *get_color_code(st_color_e color)
{
	switch (color) {
	case ST_COLOR_RED:
		return "\033[0;31m";
	case ST_COLOR_GREEN:
		return "\033[0;32m";
	case ST_COLOR_YELLOW:
		return "\033[0;33m";
	default:
		return "\033[m";
	}
}

static void _color_print(st_color_e color, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	printf("%s", get_color_code(color));
	vprintf(fmt, args);
	printf("\033[m"); // Resets the terminal to default.
	va_end(args);
}

void print_smoke_title(st_smoke *smoke)
{
	st_func *unit = smoke->func;
	_color_print(ST_COLOR_GREEN, "[ RUN        ] ");
	printf("%s repeat: %d\n", unit->tc_name, smoke->repeat_size);
}

void print_smoke_result(st_smoke *smoke)
{
	st_stability *stab = smoke->stability;
	st_performance *perf = smoke->performance;
	if (stab->stat.result == STRESS_TC_PASS) {
		_color_print(ST_COLOR_GREEN, "[        OK  ] ");
	} else {
		_color_print(ST_COLOR_RED, "[  FAILED    ] ");
	}
	
	printf("%s (%u/%d) %u us\n", smoke->func->tc_name, perf->stat.count, smoke->repeat_size, perf->stat.total_elapsed);
}

void print_testsuite_title(st_pack *ts)
{
	_color_print(ST_COLOR_GREEN, "[============] ");
	printf("%s\n", ts->title);
}

void print_testsuite_result(st_pack *ts)
{
	_color_print(ST_COLOR_GREEN, "[============] ");
	printf("%s\n", ts->title);
}

void print_testsuite_setup(st_pack *ts)
{
	_color_print(ST_COLOR_GREEN, "[------------] ");
	printf("%s set-up\n", ts->title);
}

void print_testsuite_teardown(st_pack *ts)
{
	_color_print(ST_COLOR_GREEN, "[------------] ");
	printf("%s tear-down\n", ts->title);
}

void _print_stability(st_stability *stab)
{
	st_stability_stat *s = &stab->stat;
	if (s->result == STRESS_TC_PASS) {
		printf("             %-11d%-8d%-8d%-7d         :SUCCESS\n", s->count, s->pass, s->fail, s->skip);
	} else {
		printf("             %-11d%-8d%-8d%-7d         :FAILURE\n", s->count, s->pass, s->fail, s->skip);
	}
}

void _print_performance(st_performance *perf)
{
	st_performance_stat *p = &perf->stat;
	if (p->result == STRESS_TC_PASS) {
		printf("             %-11d%-8d%-8d%-8d%-8d:SUCCESS\n",
			   p->count, p->max / 1000, p->min / 1000, p->sum / 1000, p->fail);
	} else {
		printf("             %-11d%-8d%-8d%-8d%-8d:FAILURE\n",
			   p->count, p->max / 1000, p->min / 1000, p->sum / 1000, p->fail);
	}
}

void print_smoke_summary(st_smoke *smoke)
{
	printf("TESTCASE: %s\n", smoke->func->tc_name);
	_print_performance(smoke->performance);
	_print_stability(smoke->stability);
	printf("----------------------------------------------------------------------------\n");
}

void print_smoke_summary_title(void)
{
	printf("============================================================================\n");
	printf("PERFORMANCE: count\tmax\tmin\tsum\tfail\tresult\n");
	printf("STABILITY  : count\tpass\tfail\tskip\t        result\n");
	printf("----------------------------------------------------------------------------\n");
}

void print_smoke_summary_end(void)
{
	printf("============================================================================\n");
}
