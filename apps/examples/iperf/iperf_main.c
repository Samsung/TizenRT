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
/*
 * iperf, Copyright (c) 2014, 2015, The Regents of the University of
 * California, through Lawrence Berkeley National Laboratory (subject
 * to receipt of any required approvals from the U.S. Dept. of
 * Energy).  All rights reserved.
 *
 * If you have questions about your rights to use or distribute this
 * software, please contact Berkeley Lab's Technology Transfer
 * Department at TTD@lbl.gov.
 *
 * NOTICE.  This software is owned by the U.S. Department of Energy.
 * As such, the U.S. Government has been granted for itself and others
 * acting on its behalf a paid-up, nonexclusive, irrevocable,
 * worldwide license in the Software to reproduce, prepare derivative
 * works, and perform publicly and display publicly.  Beginning five
 * (5) years after the date permission to assert copyright is obtained
 * from the U.S. Department of Energy, and subject to any subsequent
 * five (5) year renewals, the U.S. Government is granted for itself
 * and others acting on its behalf a paid-up, nonexclusive,
 * irrevocable, worldwide license in the Software to reproduce,
 * prepare derivative works, distribute copies to the public, perform
 * publicly and display publicly, and to permit others to do so.
 *
 * This code is distributed under a BSD style license, see the LICENSE
 * file for complete information.
 */
#include "iperf_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

//#include <netinet/tcp.h>

#include "iperf.h"
#include "iperf_api.h"
#include "iperf_units.h"
#include "iperf_locale.h"
#include "iperf_net.h"
#include <apps/shell/tash.h>

static int run(struct iperf_test *test);

/**************************************************************************/

int iperf_main(int argc, char **argv)
{
	struct iperf_test *test;

	// XXX: Setting the process affinity requires root on most systems.
	//      Is this a feature we really need?
#ifdef TEST_PROC_AFFINITY
	/* didnt seem to work.... */
	/*
	 * increasing the priority of the process to minimise packet generation
	 * delay
	 */
	int rc = setpriority(PRIO_PROCESS, 0, -15);

	if (rc < 0) {
		perror("setpriority:");
		fprintf(stderr, "setting priority to valid level\n");
		rc = setpriority(PRIO_PROCESS, 0, 0);
	}

	/* setting the affinity of the process */
	cpu_set_t cpu_set;
	int affinity = -1;
	int ncores = 1;

	sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set);
	if (errno) {
		perror("couldn't get affinity:");
	}

	if ((ncores = sysconf(_SC_NPROCESSORS_CONF)) <= 0) {
		err("sysconf: couldn't get _SC_NPROCESSORS_CONF");
	}

	CPU_ZERO(&cpu_set);
	CPU_SET(affinity, &cpu_set);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set) != 0) {
		err("couldn't change CPU affinity");
	}
#endif

	test = iperf_new_test();
	if (!test) {
		iperf_errexit(NULL, "create new test error - %s", iperf_strerror(i_errno));
	}
	iperf_defaults(test);		/* sets defaults */

	if (iperf_parse_arguments(test, argc, argv) < 0) {
		printf("\nparameter error - %s\n", iperf_strerror(i_errno));
		goto main_exit;
	}


	if (run(test) < 0) {
		printf("error - %s\n", iperf_strerror(i_errno));
		goto main_exit;
	}

main_exit:
	iperf_free_test(test);

	return 0;
}

/**************************************************************************/
static int run(struct iperf_test *test)
{
	switch (test->role) {
	case 's':
		for (;;) {
			int rc;
			rc = iperf_run_server(test);
			if (rc < 0) {
				iperf_err(test, "error - %s", iperf_strerror(i_errno));
				if (rc < -1) {
					iperf_errexit(test, "exiting");
					break;
				}
			}
			iperf_reset_test(test);
			if (iperf_get_test_one_off(test)) {
				break;
			}
		}
		iperf_delete_pidfile(test);
		break;
	case 'c':
		if (iperf_run_client(test) < 0) {
			struct iperf_stream *sp;

			iperf_set_send_state(test, IPERF_DONE);
			close(test->ctrl_sck);
			SLIST_FOREACH(sp, &test->streams, streams) {
				close(sp->socket);
			}
			iperf_free_test(test);
			printf("error - %s\n", iperf_strerror(i_errno));
			exit(1);
		}
		break;
	default:
		iperf_usage_long();
		break;
	}

	return 0;
}
