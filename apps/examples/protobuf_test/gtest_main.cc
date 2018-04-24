/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>
#include <tinyara/init.h>
#include <stdio.h>
#include <apps/platform/cxxinitialize.h>

#include "gtest/gtest.h"

// The following lines pull in the test files.
#include "any_test.cc"
#include "arenastring_unittest.cc"
#include "arena_unittest.cc"
#include "descriptor_database_unittest.cc"
#include "descriptor_unittest.cc"
#include "drop_unknown_fields_test.cc"
#include "dynamic_message_unittest.cc"
#include "extension_set_unittest.cc"
#include "generated_message_reflection_unittest.cc"
#include "map_field_test.cc"
#include "map_test.cc"
#include "message_unittest.cc"
#include "no_field_presence_test.cc"
#include "preserve_unknown_enum_test.cc"
#include "reflection_ops_unittest.cc"
#include "repeated_field_reflection_unittest.cc"
#include "repeated_field_unittest.cc"
#include "unknown_field_set_unittest.cc"
#include "well_known_types_unittest.cc"
#include "wire_format_unittest.cc"

#define PROTOBUF_TEST_STACKSIZE  (409600)

struct pthread_arg {
	int argc;
	char **argv;
};

GTEST_API_ int gtest_run(void *args)
{
	int argc;
	char **argv;

	argc = ((struct pthread_arg *)args)->argc;
	argv = ((struct pthread_arg *)args)->argv;

	/* C++ initialization should be supported to run this unittest.
	All the tests are registered executing this function. */
	up_cxxinitialize();

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/****************************************************************************
 * Name: gtest_main
 ****************************************************************************/

extern "C"
{
	int gtest_main(int argc, char **argv)
	{
		struct pthread_arg args;
		args.argc = argc;
		args.argv = argv;

		pthread_t tid;
		pthread_attr_t attr;
		int ret;

		/* Initialize the attribute variable */
		if ((ret = pthread_attr_init(&attr)) != 0) {
			printf("%s: pthread_attr_init failed, status=%d\n", __func__, ret);
			return ret;
		}

		/* set a stacksize */
		if ((ret = pthread_attr_setstacksize(&attr, PROTOBUF_TEST_STACKSIZE)) != 0) {
			printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, ret);
			return ret;
		}

		/* create pthread with entry function */
		if ((ret = pthread_create(&tid, &attr, (pthread_startroutine_t) gtest_run, (void *)&args)) != 0) {
			printf("%s: pthread_create failed, status=%d\n", __func__, ret);
			return ret;
		}

		/* Wait for the threads to stop */
		pthread_join(tid, NULL);

		return ret;
	}
}
