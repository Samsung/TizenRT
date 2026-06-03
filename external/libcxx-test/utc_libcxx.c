/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include "utc_libcxx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Forward declarations for test category functions
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS
void tc_algorithms_menu(int argc, char *argv[]);
#endif
#ifdef CONFIG_LIBCXX_UTC_CONTAINERS
void tc_containers_menu(int argc, char *argv[]);
#endif
#ifdef CONFIG_LIBCXX_UTC_ITERATORS
void tc_iterators_menu(int argc, char *argv[]);
#endif
#ifdef CONFIG_LIBCXX_UTC_STRINGS
void tc_strings_menu(int argc, char *argv[]);
#endif
#ifdef CONFIG_LIBCXX_UTC_UTILITIES
void tc_utilities_menu(int argc, char *argv[]);
#endif
#ifdef CONFIG_LIBCXX_UTC_LANGUAGE_SUPPORT
void tc_language_support_menu(int argc, char *argv[]);
#endif
#ifdef CONFIG_LIBCXX_UTC_DIAGNOSTICS
int tc_diagnostics_main(void);
#endif
#ifdef CONFIG_LIBCXX_UTC_ATOMICS
int tc_atomics_main(void);
#endif

static void show_usage(void)
{
	printf("\n========== Libc++ Test Commands ==========\n");
	printf("libcxx_utc                    Run all tests\n");
	printf("libcxx_utc help               Show this help\n");
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS
	printf("libcxx_utc algorithms         Run all algorithms tests\n");
	printf("libcxx_utc algorithms mod     Run modifying operations tests\n");
	printf("libcxx_utc algorithms nonmod  Run non-modifying operations tests\n");
	printf("libcxx_utc algorithms sort    Run all sorting tests\n");
	printf("libcxx_utc algorithms sort binarysearch\n");
	printf("libcxx_utc algorithms sort clamp\n");
	printf("libcxx_utc algorithms sort heap\n");
	printf("libcxx_utc algorithms sort lexcompare\n");
	printf("libcxx_utc algorithms sort merge\n");
	printf("libcxx_utc algorithms sort minmax\n");
	printf("libcxx_utc algorithms sort nthelement\n");
	printf("libcxx_utc algorithms sort permutation\n");
	printf("libcxx_utc algorithms sort setops\n");
	printf("libcxx_utc algorithms sort sort\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_CONTAINERS
	printf("libcxx_utc containers         Run all containers tests\n");
	printf("libcxx_utc containers forwardlist\n");
	printf("libcxx_utc containers list\n");
	printf("libcxx_utc containers map\n");
	printf("libcxx_utc containers multimap\n");
	printf("libcxx_utc containers unordmap\n");
	printf("libcxx_utc containers unordmultimap\n");
	printf("libcxx_utc containers vector\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_STRINGS
	printf("libcxx_utc strings            Run all strings tests\n");
	printf("libcxx_utc strings chartraits\n");
	printf("libcxx_utc strings basicstring\n");
	printf("libcxx_utc strings cstrings\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_ITERATORS
	printf("libcxx_utc iterators          Run all iterators tests\n");
	printf("libcxx_utc iterators primitives\n");
	printf("libcxx_utc iterators operations\n");
	printf("libcxx_utc iterators adaptors\n");
	printf("libcxx_utc iterators stream\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_UTILITIES
	printf("libcxx_utc utilities          Run all utilities tests\n");
	printf("libcxx_utc utilities pair\n");
	printf("libcxx_utc utilities smartptr\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_NUMERICS
	printf("libcxx_utc numerics           Run numerics tests\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_THREAD
	printf("libcxx_utc thread             Run thread tests\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_LANGUAGE_SUPPORT
	printf("libcxx_utc languagesupport    Run language support tests\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_DIAGNOSTICS
	printf("libcxx_utc diagnostics        Run diagnostics tests\n");
#endif
#ifdef CONFIG_LIBCXX_UTC_ATOMICS
	printf("libcxx_utc atomics            Run atomics tests\n");
#endif
	printf("==========================================\n");
}

static void run_all_tests(void)
{
	printf("\n########## Running All Tests ##########");
	
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS
	tc_algorithms_menu(0, NULL);
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_CONTAINERS
	tc_containers_menu(0, NULL);
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_UTILITIES
	tc_utilities_menu(0, NULL);
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_NUMERICS
	tc_numerics_main();
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_THREAD
	tc_thread_main();
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_ITERATORS
	tc_iterators_menu(0, NULL);
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_STRINGS
	tc_strings_menu(0, NULL);
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_LANGUAGE_SUPPORT
	tc_language_support_menu(0, NULL);
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_DIAGNOSTICS
	tc_diagnostics_main();
	usleep(1000);
#endif

#ifdef CONFIG_LIBCXX_UTC_ATOMICS
	tc_atomics_main();
	usleep(1000);
#endif
}

int utc_libcxx_main(int argc, char *argv[])
{
	if (testcase_state_handler(TC_START, "Libc++ TC") == ERROR) {
		return ERROR;
	}
	
	printf("\n########## Libc++ TC Start ##########\n");
	fflush(stdout);

	// No arguments: run all tests
	if (argc <= 1) {
		run_all_tests();
		goto end_tests;
	}

	// Help
	if (strcmp(argv[1], "help") == 0) {
		show_usage();
		goto end_tests;
	}

	// Category-based selection
	if (strcmp(argv[1], "algorithms") == 0) {
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS
		tc_algorithms_menu(argc - 1, &argv[1]);
#else
		printf("\nALGORITHMS tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "containers") == 0) {
#ifdef CONFIG_LIBCXX_UTC_CONTAINERS
		tc_containers_menu(argc - 1, &argv[1]);
#else
		printf("\nCONTAINERS tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "utilities") == 0) {
#ifdef CONFIG_LIBCXX_UTC_UTILITIES
		tc_utilities_menu(argc - 1, &argv[1]);
#else
		printf("\nUTILITIES tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "numerics") == 0) {
#ifdef CONFIG_LIBCXX_UTC_NUMERICS
		tc_numerics_main();
#else
		printf("\nNUMERICS tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "thread") == 0) {
#ifdef CONFIG_LIBCXX_UTC_THREAD
		tc_thread_main();
#else
		printf("\nTHREAD tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "iterators") == 0) {
#ifdef CONFIG_LIBCXX_UTC_ITERATORS
		tc_iterators_menu(argc - 1, &argv[1]);
#else
		printf("\nITERATORS tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "strings") == 0) {
#ifdef CONFIG_LIBCXX_UTC_STRINGS
		tc_strings_menu(argc - 1, &argv[1]);
#else
		printf("\nSTRINGS tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "languagesupport") == 0) {
#ifdef CONFIG_LIBCXX_UTC_LANGUAGE_SUPPORT
		tc_language_support_menu(argc - 1, &argv[1]);
#else
		printf("\nLANGUAGE_SUPPORT tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "diagnostics") == 0) {
#ifdef CONFIG_LIBCXX_UTC_DIAGNOSTICS
		tc_diagnostics_main();
#else
		printf("\nDIAGNOSTICS tests not enabled in config\n");
#endif
	}
	else if (strcmp(argv[1], "atomics") == 0) {
#ifdef CONFIG_LIBCXX_UTC_ATOMICS
		tc_atomics_main();
#else
		printf("\nATOMICS tests not enabled in config\n");
#endif
	}
	else {
		printf("\nUnknown category: %s\n", argv[1]);
		show_usage();
	}

end_tests:
	printf("\nEnding Tests\n");
	fflush(stdout);
	testcase_state_handler(TC_END, "Libc++ TC");

	return OK;
}
