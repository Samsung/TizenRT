// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include <driver/pwr_clk.h>
#include <driver/aon_rtc.h>

#include <modules/pm.h>
#include "sys_driver.h"

#include <driver/timer.h>
#include <driver/rosc_ppm.h>

/*=====================DEFINE  SECTION  START=====================*/

#define EXTERN32K_STANDARD_VAL (32768)
#define ROSC_STANDARD_VAL      (32000)
#define TIMER_CONVERSION       (1000)

typedef struct
{
	uint32_t    rtc_val;
	uint32_t    std_val;
	uint32_t    flag;
	uint32_t    cycle_count;
	uint32_t    ppm_max;
}rosc_ppm_stats_t;

/*=====================DEFINE  SECTION  END=====================*/

/*=====================VARIABLE  SECTION  START=================*/

static rosc_ppm_stats_t  s_rosc_ppm;

/*=====================VARIABLE  SECTION  END=================*/

/*================FUNCTION DECLARATION  SECTION  START========*/

static void ppm_statistic_callback(timer_id_t timer_id)
{
	uint32_t cur_val;
	uint32_t diff_val = 0;

	cur_val = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);

	if ( cur_val > s_rosc_ppm.rtc_val)
	{
		diff_val = cur_val - s_rosc_ppm.rtc_val;

		if (diff_val > s_rosc_ppm.std_val)
		{
			if (diff_val - s_rosc_ppm.std_val > s_rosc_ppm.ppm_max)
				s_rosc_ppm.ppm_max = diff_val - s_rosc_ppm.std_val;
		}
		else
		{
			if (s_rosc_ppm.std_val - diff_val  > s_rosc_ppm.ppm_max)
				s_rosc_ppm.ppm_max = s_rosc_ppm.std_val - diff_val;
		}
		if (s_rosc_ppm.cycle_count > 0)
		{
			s_rosc_ppm.cycle_count--;
		}
	}
	else
	{
		// timer overflow
	}

	//bk_printf("count = %d ,  cur - record = %d\r\n", s_rosc_ppm.cycle_count+1, diff_val);
	if (s_rosc_ppm.cycle_count == 0)
	{
		bk_timer_stop(timer_id);
		s_rosc_ppm.flag = 0x1;
	}
	s_rosc_ppm.rtc_val = cur_val;

	return ;
}

bk_err_t bk_rosc_ppm_statistic_start(timer_id_t timer_id, uint32_t interval_ms, uint32_t count )
{
	pm_lpo_src_e lpo_src;

	if(timer_id >= TIMER_ID_MAX)
	{
		return BK_FAIL;
	}

	s_rosc_ppm.flag = 0x0;
	s_rosc_ppm.ppm_max = 0x0;
	s_rosc_ppm.cycle_count = count;

	lpo_src = bk_pm_lpo_src_get();

	if (lpo_src == PM_LPO_SRC_X32K)
	{
		s_rosc_ppm.std_val = (interval_ms * EXTERN32K_STANDARD_VAL)/TIMER_CONVERSION;	
	}
	else
	{
		s_rosc_ppm.std_val = (interval_ms * ROSC_STANDARD_VAL)/TIMER_CONVERSION;
	}

	bk_timer_delay_with_callback(timer_id, (uint64_t)interval_ms*TIMER_CONVERSION, ppm_statistic_callback);//TIMER_ID0

	s_rosc_ppm.rtc_val = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);

	return BK_OK;
}

bk_err_t bk_rosc_ppm_statistics_get(int32 *rosc_ppm)
{
	if(s_rosc_ppm.flag == 0x1)
	{
		//ppm = [ (actual_freq - set_freq) / set_freq ] x 1000000
		*rosc_ppm = (int32)(s_rosc_ppm.ppm_max / (double)s_rosc_ppm.std_val * 1e6);
	}
	else
	{
		return BK_FAIL;
	}

	return BK_OK;
}

/*================FUNCTION DECLARATION  SECTION  END========*/

