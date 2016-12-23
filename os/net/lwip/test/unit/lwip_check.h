/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __LWIP_CHECK_H__
#define __LWIP_CHECK_H__

/* Common header file for lwIP unit tests using the check framework */

#include <config.h>
#include <check.h>
#include <stdlib.h>

#define FAIL_RET() do { fail(); return; } while (0)
#define EXPECT(x) fail_unless(x)
#define EXPECT_RET(x) do { fail_unless(x); if (!(x)) { return; } } while (0)
#define EXPECT_RETX(x, y) do { fail_unless(x); if (!(x)) { return y; } } while (0)
#define EXPECT_RETNULL(x) EXPECT_RETX(x, NULL)

/** typedef for a function returning a test suite */
typedef Suite *(suite_getter_fn)(void);

/** Create a test suite */
static Suite *create_suite(const char *name, TFun *tests, size_t num_tests, SFun setup, SFun teardown)
{
	size_t i;
	Suite *s = suite_create(name);

	for (i = 0; i < num_tests; i++) {
		/* Core test case */
		TCase *tc_core = tcase_create("Core");
		if ((setup != NULL) || (teardown != NULL)) {
			tcase_add_checked_fixture(tc_core, setup, teardown);
		}
		tcase_add_test(tc_core, tests[i]);
		suite_add_tcase(s, tc_core);
	}
	return s;
}

#endif							/* __LWIP_CHECK_H__ */
