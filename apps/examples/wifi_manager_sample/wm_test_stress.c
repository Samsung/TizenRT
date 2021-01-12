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

#include <tinyara/config.h>
#include <stdint.h>
#include <wifi_manager/wifi_manager.h>
#include "wm_test.h"

extern void wm_run_stress_test1(struct options *opt);
extern void wm_run_stress_test2(struct options *opt);

void wm_run_stress_test(void *arg)
{
	struct options *opt = (struct options *)arg;
	if (opt->stress_tc_idx == 1) {
		wm_run_stress_test1(opt);
	} else if (opt->stress_tc_idx == 2) {
		wm_run_stress_test2(opt);
	}
}
