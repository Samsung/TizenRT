/* ****************************************************************
*
* Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include "st_things_sample.h"

#define CMD_STOP "stop"

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int st_things_sample_main(int argc, char *argv[])
#endif
{
	printf("st_things_sample!!\n");

	if (argc == 2 && strncmp(argv[1], CMD_STOP, strlen(argv[1])) == 0) {
		ess_process(true);
	} else if (argc == 1) {
		ess_process(false);
	} else {
		printf("Usage: st_things_sample\n   or: st_things_sample stop\n Start, or Stop st_things sample\n");
	}

	return 0;
}
