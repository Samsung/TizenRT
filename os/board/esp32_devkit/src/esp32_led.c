/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include "esp32_ledc.h"

FAR struct ledc_lowerhalf_s *esp32_ledcinitialize(int i);

int board_ledc_setup(void)
{
#ifdef CONFIG_LEDC
	FAR struct ledc_lowerhalf_s *ledc;
	char path[10];
	int ret = 0;
	int i;
	for (i = 0; i < 1; i++) {
		ledc = esp32_ledcinitialize(i);
		if (!ledc) {
			lldbg("Failed to get the PWM lower half\n");
			return -ENODEV;
		}

		snprintf(path, sizeof(path), "/dev/ledc%d", i);

		ret = ledc_register(path, ledc);
		if (ret < 0) {
			lldbg("ledc_register failed: %d\n", ret);
			return ret;
		}
	}
#endif
	return OK;
}
