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

#include "lwip_check.h"

#include "udp/test_udp.h"
#include "tcp/test_tcp.h"
#include "tcp/test_tcp_oos.h"
#include "core/test_mem.h"
#include "etharp/test_etharp.h"

#include <net/lwip/init.h>

int main()
{
	int number_failed;
	SRunner *sr;
	size_t i;
	suite_getter_fn *suites[] = {
		udp_suite,
		tcp_suite,
		tcp_oos_suite,
		mem_suite,
		etharp_suite
	};
	size_t num = sizeof(suites) / sizeof(void *);
	LWIP_ASSERT("No suites defined", num > 0);

	lwip_init();

	sr = srunner_create((suites[0])());
	for (i = 1; i < num; i++) {
		srunner_add_suite(sr, ((suite_getter_fn *) suites[i])());
	}

#ifdef LWIP_UNITTESTS_NOFORK
	srunner_set_fork_status(sr, CK_NOFORK);
#endif
#ifdef LWIP_UNITTESTS_FORK
	srunner_set_fork_status(sr, CK_FORK);
#endif

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
