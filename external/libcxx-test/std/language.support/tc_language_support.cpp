//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "libcxx_tc_common.h"
#include "tc_language_support.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern "C"
{
	void tc_language_support_runtime()
	{
		{ tc_libcxx_language_support_support_runtime_cstdbool(); }
	}

	int tc_language_support_main(void)
	{
		printf("\nStarting [LANGUAGE_SUPPORT] Tests\n");
		fflush(stdout);

		tc_language_support_runtime();
		usleep(1000);

		return 0;
	}

	// Menu function for LANGUAGE_SUPPORT sub-tests (called from utc_libcxx.c)
	void tc_language_support_menu(int argc, char *argv[])
	{
		// No arguments: run all language support tests
		if (argc <= 1) {
			tc_language_support_main();
			return;
		}

		if (strcmp(argv[1], "runtime") == 0) {
			printf("\nRunning support.runtime tests...\n");
			tc_language_support_runtime();
		}
		else {
			printf("\nUnknown language_support sub-category: %s\n", argv[1]);
			printf("Available: runtime\n");
		}
	}
}
