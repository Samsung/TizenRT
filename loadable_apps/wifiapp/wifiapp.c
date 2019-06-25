/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "wifiapp_internal.h"
/****************************************************************************
 * Public Functions
 ****************************************************************************/
static void display_test_scenario(void)
{
	printf("\nSelect Test Scenario.\n");
#ifdef CONFIG_EXAMPLES_MESSAGING_TEST
	printf("\t-Press M or m : Messaging F/W Test\n");	
#endif
#ifdef CONFIG_EXAMPLES_RECOVERY_TEST
	printf("\t-Press R or r : Recovery Test\n");
#endif
	printf("\t-Press X or x : Terminate Tests.\n");
}

extern int preapp_start(int argc, char **argv);

int main(int argc, char **argv)
{
	char ch;
	bool is_testing = true;

#ifdef CONFIG_SYSTEM_PREAPP_INIT
	preapp_start(argc, argv);
#endif

	printf("This is WIFI App\n");
#ifndef CONFIG_ENABLE_RECOVERY_AGING_TEST
	while (is_testing) {
		display_test_scenario();
		ch = getchar();
		switch (ch) {
#ifdef CONFIG_EXAMPLES_MESSAGING_TEST
		case 'M':
		case 'm':
			messaging_test();
			break;
#endif
#ifdef CONFIG_EXAMPLES_RECOVERY_TEST
		case 'R':
		case 'r':
			recovery_test();
			is_testing = false;
			break;
#endif
		case 'X':
		case 'x':
			printf("Test will be finished.\n");
			is_testing = false;
			break;
		default:
			printf("Invalid Scenario.\n");
			break;
		}
		sleep(1);
		fflush(stdout);
	}
#else
	recovery_test();
#endif
	while (1) {
		sleep(10);
		printf("[%d] WIFI ALIVE\n", getpid());
	}
	return 0;
}
