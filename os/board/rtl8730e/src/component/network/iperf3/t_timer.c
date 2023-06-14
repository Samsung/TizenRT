/*
 * iperf, Copyright (c) 2014, The Regents of the University of
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
#include <sys/time.h>

#include "platform_stdlib.h"

#include "timer.h"

#include <FreeRTOS.h>
#include "task.h"

static int flag;


static void
timer_proc(TimerClientData client_data, struct timeval *nowP)
{
	/* To avoid gcc warnings */
	(void) client_data;
	(void) nowP;
	flag = 1;
}


void
timer_thread(void *param)
{
	/* To avoid gcc warnings */
	(void) param;

	vTaskDelay(5000);
	printf("\ntimer test\n");

	Timer *tp;
	int tim = 2.2 * 1000 * 1000;

	flag = 0;
	tp = tmr_create((struct timeval *) 0, timer_proc, JunkClientData, tim, 0);
	if (!tp) {
		printf("failed to create timer\n");
		goto EXIT;
	}

	vTaskDelay(2000);//delay 2s
	tmr_run((struct timeval *) 0);
	if (flag) {
		printf("timer should not have expired\n");
		goto EXIT;
	}

	vTaskDelay(200);//delay 0.2s
	tmr_run((struct timeval *) 0);
	if (!flag) {
		printf("timer should have expired\n");
		goto EXIT;
	}

	tmr_destroy();
EXIT:
	vTaskDelete(NULL);

}

void example_timer(void)
{

	if (xTaskCreate(timer_thread, ((const char *)"timer_thread"), 524, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(timer_thread) failed", __FUNCTION__);
	}
}

