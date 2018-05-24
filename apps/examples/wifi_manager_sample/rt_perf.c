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

#include "rt_perf.h"

int
_calc_performance(rt_performance *p, rt_elapsed_time *duration)
{
	rt_performance_time *start = &duration->start;
	rt_performance_time *end = &duration->end;
	unsigned int start_time = start->second*1000000 + start->micro;
	unsigned int end_time = end->second*1000000 + end->micro;
	rt_performance_stat *stat = &p->stat;
	if (stat->count == 0) {
		stat->start.second = start->second;
		stat->start.micro = start->micro;
	}
	stat->count++;
	stat->end.second = end->second;
	stat->end.micro = end->micro;
	unsigned int elapsed = (end->second - start->second) * 1000000 + (end->micro - start->micro);
	printf("start %d end %d, elapsed %d us\n", start_time, end_time, elapsed);
	
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
		return -1;
	}
	return 0;
}

void
_calc_stability(rt_stability *stab, rt_tc_result r)
{
	rt_stability_stat *s = &stab->stat;
	s->count++;
	switch (r) {
	case DA_TC_PASS:
		s->pass++;
		break;
	case DA_TC_FAIL:
		s->fail++;
		break;
	case DA_TC_SKIP:
		s->skip++;
		break;
	default:
		RT_ERROR;
		break;
	}
}


void
_run_smoke(rt_smoke *smoke)
{
	if (!smoke) {
		return;
	}
	int cnt = 0;
	rt_tc_result ret;
	int perf_result;
	rt_func *unit = smoke->func;
	for (; cnt < smoke->repeat_size; cnt++) {
		if (unit->setup) {
			ret = unit->setup(NULL);
			// update rt performance, stability
			if (ret != DA_TC_PASS) {
				// reset
				// if teardown fails then remained testcase could be affected.
				// so remained procedures could be useless
				// so stopping running smoke would be better
				continue;
			}
		}
		rt_elapsed_time duration;
		ret = unit->tc(&duration);

		perf_result = _calc_performance(smoke->performance, &duration);
		_calc_stability(smoke->stability, ret);

		if (unit->teardown) {
			ret = unit->teardown(NULL);
			if (ret != DA_TC_PASS) {
				// same to the setup procedure
				break;
			}
		}
		printf("\n"); // pkbuild
	}
}


void
_print_stability(rt_stability *stab)
{
	rt_stability_stat *s = &stab->stat;
	printf("             %-11d%-8d%-8d%-7d:%-d\n", s->count, s->pass, s->fail, s->skip, s->result);
}


void
_print_performance(rt_performance *perf)
{
	rt_performance_stat *p = &perf->stat;
	printf("             %-11d%-8d%-8d%-8d%-8d         :%-d\n", p->count, p->max/1000, p->min/1000, p->sum/1000, p->fail, p->result);
}


void
_print_smoke(rt_smoke *smoke)
{
	printf("TESTCASE: %s\n", smoke->func->tc_name);
	_print_performance(smoke->performance);
	_print_stability(smoke->stability);
	printf("----------------------------------------------------------------------------\n");
}


void
perf_run(rt_pack *pack)
{
	if (!pack) {
		return;
	}
	rt_smoke *smoke = pack->head;
	for (; smoke != NULL; smoke = smoke->next) {
		_run_smoke(smoke);
	}
}


void
perf_print_result(rt_pack *pack) {
	if (!pack) {
		return;
	}
	rt_smoke *smoke = pack->head;
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
