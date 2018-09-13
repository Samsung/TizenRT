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

#include <stdio.h>
#include <sys/time.h>

#include <stress_tool/st_perf.h>

int
_calc_performance(st_performance *p, st_elapsed_time *duration)
{
	st_performance_time *start = &duration->start;
	st_performance_time *end = &duration->end;

	unsigned int start_time = start->second*1000000 + start->micro;
	unsigned int end_time = end->second*1000000 + end->micro;
	unsigned int elapsed = end_time - start_time;

	st_performance_stat *stat = &p->stat;
	if (stat->count == 0) {
		stat->start.second = start->second;
		stat->start.micro = start->micro;
	}
	stat->count++;
	stat->end.second = end->second;
	stat->end.micro = end->micro;

	printf("TEST#%d\tstart %u end %u, elapsed %u us\n", stat->count, start_time, end_time, elapsed);
	
	stat->sum += elapsed;
	if (stat->count == 1) {
		stat->max = elapsed;
		stat->min = elapsed;
	} else {
		if (elapsed > stat->max) {
			stat->max = elapsed;
		}
		if (elapsed < stat->min) {
			stat->min = elapsed;
		}
	}
	if (p->expect != 0 && p->expect < elapsed) {
		printf("timeout\n");
		stat->fail++;
		stat->result = STRESS_TC_FAIL;
		return -1;
	}
	return 0;
}

void
_calc_stability(st_stability *stab, st_tc_result r)
{
	st_stability_stat *s = &stab->stat;
	s->count++;
	switch (r) {
	case STRESS_TC_PASS:
		s->pass++;
		break;
	case STRESS_TC_FAIL:
		s->fail++;
		s->result = STRESS_TC_FAIL;
		break;
	case STRESS_TC_SKIP:
		s->skip++;
		s->result = STRESS_TC_SKIP;
		break;
	default:
		ST_ERROR;
		break;
	}
}


void
_run_smoke(st_smoke *smoke)
{
	if (!smoke) {
		return;
	}
	int cnt = 0;
	st_tc_result ret;
	int perf_result;
	st_func *unit = smoke->func;
	printf("Repeat size: %d\n", smoke->repeat_size);
	for (; cnt < smoke->repeat_size; cnt++) {
		if (unit->setup) {
			ret = unit->setup(NULL);
			// update rt performance, stability
			if (ret != STRESS_TC_PASS) {
				// reset
				// if teardown fails then remained testcase could be affected.
				// so remained procedures could be useless
				// so stopping running smoke would be better
				continue;
			}
		}
		st_elapsed_time duration;
		ret = unit->tc(&duration);
		perf_result = _calc_performance(smoke->performance, &duration);
		_calc_stability(smoke->stability, ret);

		if (unit->teardown) {
			ret = unit->teardown(NULL);
			if (ret != STRESS_TC_PASS) {
				// same to the setup procedure
				break;
			}
		}
		printf("\n"); // pkbuild
	}
}


void
_print_stability(st_stability *stab)
{
	st_stability_stat *s = &stab->stat;
	if (s->result == STRESS_TC_PASS) {
		printf("             %-11d%-8d%-8d%-7d         :SUCCESS\n", s->count, s->pass, s->fail, s->skip);
	} else {
		printf("             %-11d%-8d%-8d%-7d         :FAILURE\n", s->count, s->pass, s->fail, s->skip);
	}
}


void
_print_performance(st_performance *perf)
{
	st_performance_stat *p = &perf->stat;
	if (p->result == STRESS_TC_PASS) {
		printf("             %-11d%-8d%-8d%-8d%-8d:SUCCESS\n", p->count, p->max/1000, p->min/1000, p->sum/1000, p->fail);
	} else {
		printf("             %-11d%-8d%-8d%-8d%-8d:FAILURE\n", p->count, p->max/1000, p->min/1000, p->sum/1000, p->fail);
	}
}


void
_print_smoke(st_smoke *smoke)
{
	printf("TESTCASE: %s\n", smoke->func->tc_name);
	_print_performance(smoke->performance);
	_print_stability(smoke->stability);
	printf("----------------------------------------------------------------------------\n");
}


void
perf_run(st_pack *pack)
{
	if (!pack) {
		return;
	}
	st_smoke *smoke = pack->head;
	for (; smoke != NULL; smoke = smoke->next) {
		_run_smoke(smoke);
	}
}


void
perf_print_result(st_pack *pack) {
	if (!pack) {
		return;
	}
	st_smoke *smoke = pack->head;
	printf(COLOR_RESULT);
	printf("============================================================================\n");
	printf("PERFORMANCE: count\tmax\tmin\tsum\tfail\tresult\n");
	printf("STABILITY  : count\tpass\tfail\tskip\t        result\n");
	printf("----------------------------------------------------------------------------\n");
	for (; smoke != NULL; smoke = smoke->next) {
		_print_smoke(smoke);
	}
	printf("============================================================================\n");
	printf(COLOR_WHITE);
}
