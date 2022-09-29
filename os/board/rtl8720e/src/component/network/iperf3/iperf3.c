/*
 * iperf, Copyright (c) 2014, 2015, 2017, The Regents of the University of
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
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include <getopt.h>
#include <errno.h>

#include "platform_stdlib.h"
#include "lwipconf.h" //realtek add
#include <FreeRTOS.h>
#include "task.h"
#include <osdep_service.h>

#include "iperf.h"
#include "iperf_api.h"
#include "units.h"
#include "iperf_locale.h"
#include "net.h"
#include "timer.h"

struct iperf_test *test;
struct task_struct g_server_task;
struct task_struct g_client_task;
unsigned char g_server_terminate = 0;
unsigned char g_client_terminate = 0;

void server_thread(void *param)
{
	/* To avoid gcc warnings */
	(void) param;
	vTaskDelay(500);
	printf("server test start\n");
	g_server_terminate = 0;
	int rc;
	while (!g_server_terminate) {
		rc = iperf_run_server(test);
		if (rc < 0) {
			iperf_err(test, "error - %s", iperf_strerror(i_errno));
			if (rc < -1) {
				iperf_err(test, "exiting");
			}
		}

		iperf_reset_test(test);
		if (iperf_get_test_one_off(test)) {
			break;
		}
	}

	iperf_free_test(test);
	tmr_destroy();
	printf("server test end\n");
	rtw_delete_task(&g_server_task);
}

void client_thread(void *param)
{
	/* To avoid gcc warnings */
	(void) param;
	g_client_terminate = 0;
	vTaskDelay(500);
	printf("client test start\n");

	if (iperf_run_client(test) < 0) {
		iperf_err(test, "error - %s", iperf_strerror(i_errno));
	}

	iperf_free_test(test);
	tmr_destroy();
	printf("client test end\n");
	rtw_delete_task(&g_client_task);
}

/**************************************************************************/
extern void lwip_setsockrcvevent(int fd, int rcvevent);
extern void lwip_selectevindicate(int fd);
static void indicate_server(void)
{
	if (test->listener >= 0) {
		lwip_setsockrcvevent(test->listener, 1);
		lwip_selectevindicate(test->listener);
	}

	if (test->ctrl_sck >= 0) {
		lwip_setsockrcvevent(test->ctrl_sck, 1);
		lwip_selectevindicate(test->ctrl_sck);
	}

}
/**************************************************************************/


void cmd_iperf3(int argc, char **argv)
{
	if (strcmp(argv[1], "stop") == 0) {
		switch (test->role) {
		case 's':
			g_server_terminate = 1;
			printf("\n\r start terminate server test:%d\n", g_server_terminate);
			if (g_server_task.task) {
				indicate_server();
				vTaskDelay(1000);
			}
			break;
		case 'c':
			g_client_terminate = 1;
			printf("\n\r start terminate client test:%d\n", g_client_terminate);
			if (g_client_task.task) {
				vTaskDelay(1000);
			}
			break;
		}
		goto Exit;
	}

	if (g_server_task.task != NULL && test->role == 's') {
		printf("\n\rserver test is already running.\n");
		goto Exit;
	}
	if (g_client_task.task != NULL && test->role == 'c') {
		printf("\n\rclient test is already running.\n");
		goto Exit;
	}

	test = iperf_new_test();
	if (!test) {
		iperf_err(NULL, "create new test error - %s", iperf_strerror(i_errno));
		goto Exit;
	}

	iperf_defaults(test);	/* sets defaults */
	if (iperf_parse_arguments(test, argc, argv) < 0) {
		iperf_err(test, "parameter error - %s", iperf_strerror(i_errno));
		fprintf(stderr, "\n");
		usage_long(stdout);
		iperf_free_test(test);
		goto Exit;
	}

	switch (test->role) {
	case 's':
		if (rtw_create_task(&g_server_task, ((const char *)"server_thread"), 1024, tskIDLE_PRIORITY + 2 + PRIORITIE_OFFSET, server_thread, NULL) != pdPASS) {
			printf("\n\r%s xTaskCreate(server_thread) failed", __FUNCTION__);
			iperf_free_test(test);
		}
		break;
	case 'c':
		if (rtw_create_task(&g_client_task, ((const char *)"client_thread"), 1024, tskIDLE_PRIORITY + 1 + PRIORITIE_OFFSET, client_thread, NULL) != pdPASS) {
			printf("\n\r%s xTaskCreate(client_thread) failed", __FUNCTION__);
			iperf_free_test(test);
		}
		break;
	default:
		usage();
		break;
	}
Exit:
	return;
}
