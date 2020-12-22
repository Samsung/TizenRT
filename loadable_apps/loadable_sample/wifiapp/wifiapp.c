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
#ifdef CONFIG_BINARY_MANAGER
#include <binary_manager/binary_manager.h>
#endif

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
#ifdef CONFIG_EXAMPLES_BINARY_UPDATE_TEST
	printf("\t-Press U or u : Binary Update Test\n");
#endif
	printf("\t-Press X or x : Terminate Tests.\n");
}

extern int preapp_start(int argc, char **argv);

#ifdef CONFIG_EXAMPLES_SMARTFS_POWERCUT
extern int smartfs_powercut_main(int argc, char *argv[]);
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
int main(int argc, char **argv)
#else
int wifiapp_main(int argc, char **argv)
#endif
{
#ifdef CONFIG_EXAMPLES_LOADABLE_MANUAL_TEST
	int ch;
	bool is_testing = true;
#endif

#if defined(CONFIG_SYSTEM_PREAPP_INIT) && defined(CONFIG_APP_BINARY_SEPARATION)
	preapp_start(argc, argv);
#endif

	printf("This is WIFI App\n");

#if defined(CONFIG_BINARY_MANAGER) && !defined(CONFIG_EXAMPLES_MICOM_TIMER_TEST)
	int ret;
	ret = binary_manager_notify_binary_started();
	if (ret < 0) {
		printf("WIFI notify 'START' state FAIL\n");
	}
#endif

#ifdef CONFIG_EXAMPLES_SMARTFS_POWERCUT
	smartfs_powercut_main(0, NULL);
#endif

#ifdef CONFIG_EXAMPLES_LOADABLE_MANUAL_TEST
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
#ifdef CONFIG_EXAMPLES_BINARY_UPDATE_TEST
		case 'U':
		case 'u':
			binary_update_test();
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
	}

#elif defined(CONFIG_EXAMPLES_LOADABLE_AUTOMATIC_TEST)
#ifdef CONFIG_EXAMPLES_RECOVERY_AGING_TEST
	recovery_test();
#elif defined(CONFIG_EXAMPLES_UPDATE_AGING_TEST)
	binary_update_aging_test();
#endif
#endif

	while (1) {
		sleep(300);
		printf("[%d] WIFI ALIVE\n", getpid());
	}
	return 0;
}
