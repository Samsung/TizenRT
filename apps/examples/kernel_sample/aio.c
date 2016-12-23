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
/****************************************************************************
 * examples/kernel_sample/aio.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

#include "kernel_sample.h"

#ifdef CONFIG_FS_AIO

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define AIO_RDBUFFER_SIZE  128
#define AIO_WRBUFFER1_SIZE sizeof(g_wrbuffer1)
#define AIO_WRBUFFER2_SIZE sizeof(g_wrbuffer2)

#define AIO_NCTRLBLKS      5
#define AIO_NXFRS          3

#define AIO_FILEPATH CONFIG_EXAMPLES_KERNEL_SAMPLE_AIOPATH "/aio_test.dat"

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* Constant write buffers */

static const char g_wrbuffer1[] = "This is write buffer #1\n";
static const char g_wrbuffer2[] = "This second write buffer is this line\n";
static char g_rdbuffer[AIO_RDBUFFER_SIZE];

/* AIO control blocks:  write, nop, write, NULL, read */


static struct aiocb g_aiocbs[AIO_NCTRLBLKS - 1];
static struct aiocb *g_aiocb[AIO_NCTRLBLKS];

static struct aiocb *const g_aiocb_init[AIO_NCTRLBLKS] = {
	&g_aiocbs[0], &g_aiocbs[1], &g_aiocbs[2], NULL, &g_aiocbs[3]
};


static FAR void *const g_buffers[AIO_NCTRLBLKS] = {
	(FAR void *)g_wrbuffer1,
	(FAR void *)NULL,
	(FAR void *)g_wrbuffer2,
	(FAR void *)NULL,
	(FAR void *)g_rdbuffer
};

static const FAR uint8_t g_offsets[AIO_NCTRLBLKS] = {
	0, 0, AIO_WRBUFFER1_SIZE, 0, 0
};

static const FAR uint8_t g_nbytes[AIO_NCTRLBLKS] = {
	AIO_WRBUFFER1_SIZE, 0, AIO_WRBUFFER2_SIZE, 0, AIO_RDBUFFER_SIZE
};

static const FAR uint8_t g_opcode[AIO_NCTRLBLKS] = {
	LIO_WRITE, LIO_NOP, LIO_WRITE, LIO_NOP, LIO_READ
};

static int g_fildes;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void init_aiocb(bool signal)
{
	FAR struct aiocb *aiocbp;
	int i;

	memset(g_aiocbs, 0xff, (AIO_NCTRLBLKS - 1) * sizeof(struct aiocb));
	memset(g_rdbuffer, 0xff, AIO_RDBUFFER_SIZE);

	for (i = 0; i < AIO_NCTRLBLKS; i++) {
		aiocbp     = g_aiocb_init[i];
		g_aiocb[i] = aiocbp;

		if (aiocbp) {
			aiocbp->aio_sigevent.sigev_notify = signal ? SIGEV_SIGNAL : SIGEV_NONE;
			aiocbp->aio_sigevent.sigev_signo  = SIGUSR1;

			aiocbp->aio_buf        = g_buffers[i];
			aiocbp->aio_offset     = (off_t)g_offsets[i];
			aiocbp->aio_nbytes     = (size_t)g_nbytes[i];
			aiocbp->aio_fildes     = g_fildes;
			aiocbp->aio_reqprio    = 0;
			aiocbp->aio_lio_opcode = g_opcode[i];
		}
	}
}

static int check_done(void)
{
	FAR struct aiocb *aiocbp;
	int i;

	/* Check each entry in the list.  Break out of the loop if any entry
	 * has not completed.
	 */

	for (i = 0; i < AIO_NCTRLBLKS; i++) {
		/* Skip over NULL entries */

		aiocbp = g_aiocb[i];
		if (aiocbp) {
			/* Check if the I/O has completed */

			printf("  list[%d]. result = %d\n", i, aiocbp->aio_result);
			if (aiocbp->aio_lio_opcode == LIO_NOP) {
				printf("     NO operation\n");
			} else if (aiocbp->aio_result == -EINPROGRESS) {
				/* No.. return -EINPROGRESS */

				printf("     NOT finished\n");
				return -EINPROGRESS;
			} else if (aiocbp->aio_result == -ECANCELED) {
				/* No.. return -EINPROGRESS */

				printf("     Cancelled\n");
			}

			/* Check for an I/O error */

			else if (aiocbp->aio_result < 0) {
				printf("     ERROR: Failed I/O transfer\n");
			}

			/* Successful completion r */

			else {
				printf("     Successful completion\n");
			}
		} else {
			printf("  list[%d]. NULL\n", i);
		}
	}

	/* All of the I/Os have completed */

	return OK;
}

static int remove_done(void)
{
	FAR struct aiocb *aiocbp;
	int completed = 0;
	int i;

	/* Check each entry in the list.  Break out of the loop if any entry
	 * has not completed.
	 */

	completed = 0;
	for (i = 0; i < AIO_NCTRLBLKS; i++) {
		/* Skip over NULL entries */

		aiocbp = g_aiocb[i];
		if (aiocbp) {
			/* Check if the I/O has completed */

			printf("  list[%d]. result = %d\n", i, aiocbp->aio_result);
			if (aiocbp->aio_lio_opcode == LIO_NOP) {
				printf("     NO operation\n");
				g_aiocb[i] = NULL;
				completed++;
			} else if (aiocbp->aio_result == -EINPROGRESS) {
				/* No.. return -EINPROGRESS */

				printf("     NOT finished\n");
			} else if (aiocbp->aio_result == -ECANCELED) {
				/* No.. return -EINPROGRESS */

				printf("     Cancelled\n");
				g_aiocb[i] = NULL;
				completed++;
			}

			/* Check for an I/O error */

			else if (aiocbp->aio_result < 0) {
				printf("     ERROR: Failed I/O transfer\n");
				g_aiocb[i] = NULL;
				completed++;
			}

			/* Successful completion r */

			else {
				printf("     Successful completion\n");
				g_aiocb[i] = NULL;
				completed++;
			}
		} else {
			printf("  list[%d]. NULL\n", i);
		}
	}

	/* All of the I/Os have completed */

	return completed;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void aio_test(void)
{
	struct sigevent sig;
	sigset_t oset;
	sigset_t set;
	int completed;
	int total;
	int ret;
	int i;

	/* Block all signals except for SIGUSR1 and SIGALRM (for sleep) */

	sigfillset(&set);
	sigdelset(&set, SIGUSR1);
	sigdelset(&set, SIGALRM);
	(void)sigprocmask(SIG_SETMASK, &set, &oset);

	/* Case 1: Poll for transfer complete */

	printf("AIO test case 1: Poll for transfer complete\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout_with_procmask;
	}

	init_aiocb(false);
	ret = lio_listio(LIO_NOWAIT, g_aiocb, AIO_NCTRLBLKS, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout_with_fildes;
	}

	do {
		usleep(500 * 1000);
		ret = check_done();
	} while (ret < 0);

	close(g_fildes);
	g_fildes = -1;

	/* Case 2: Wait in lio_listio */

	/* Try to assure that this test does not overlap any activity from the
	 * task end of the last test case -- especially the dangling SIGPOLL.
	 */

	usleep(500 * 1000);

	printf("AIO test case 2: Use LIO_WAIT for transfer complete\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout_with_procmask;
	}

	init_aiocb(false);
	ret = lio_listio(LIO_WAIT, g_aiocb, AIO_NCTRLBLKS, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout_with_fildes;
	}

	ret = check_done();
	if (ret < 0) {
		printf("aio_test: ERROR: Not done\n");
		goto errout_with_fildes;
	}

	close(g_fildes);
	g_fildes = -1;

	/* Case 3: Use aio_suspend() until complete */

	/* Try to assure that this test does not overlap any activity from the
	 * task end of the last test case -- especially the dangling SIGPOLL.
	 */

	usleep(500 * 1000);

	printf("AIO test case 3: Use aio_suspend for transfer complete\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout_with_procmask;
	}

	init_aiocb(false);
	ret = lio_listio(LIO_NOWAIT, g_aiocb, AIO_NCTRLBLKS, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout_with_fildes;
	}

	total = 1; /* One entry was initially NULL */
	for (i = 1; i <= AIO_NCTRLBLKS; i++) {
		printf("  Calling aio_suspend #%d\n", i);
		ret = aio_suspend((FAR const struct aiocb * const *)g_aiocb, AIO_NCTRLBLKS, NULL);
		if (ret < 0) {
			printf("aio_test: ERROR: aio_suspend failed: %d\n", errno);
			goto errout_with_fildes;
		}

		completed = remove_done();
		if (completed < 1) {
			printf("aio_test: ERROR: Signalled, but no I/O completed\n");
			goto errout_with_fildes;
		}

		total += completed;
		printf("  Completed=%d\n", total);

		if (total >= AIO_NCTRLBLKS) {
			break;
		}
	}

	if (total != AIO_NCTRLBLKS) {
		printf("aio_test: ERROR: Total is %d, should be %d\n", total, AIO_NCTRLBLKS);
		goto errout_with_fildes;
	}

	close(g_fildes);
	g_fildes = -1;

	/* Case 4: Use individual signals */

	/* Try to assure that this test does not overlap any activity from the
	 * task end of the last test case -- especially the dangling SIGPOLL.
	 */

	usleep(500 * 1000);

	printf("AIO test case 4: Use individual signals for transfer complete\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout_with_procmask;
	}

	init_aiocb(true);

	ret = lio_listio(LIO_NOWAIT, g_aiocb, AIO_NCTRLBLKS, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout_with_fildes;
	}

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);

	do {
		ret = check_done();
		if (ret < 0) {
			int status = sigwaitinfo(&set, NULL);
			if (status < 0) {
				int errcode = errno;
				if (errcode == EINTR) {
					printf("  Interrupted by a signal)\n");
				} else {
					printf("aio_test: ERROR: sigwaitinfo failed: %d\n", errcode);
					goto errout_with_fildes;
				}
			} else {
				printf("  Received signal %d\n", status);
			}
		}
	} while (ret < 0);

	close(g_fildes);
	g_fildes = -1;

	/* Case 5: Use list complete signal */

	/* Try to assure that this test does not overlap any activity from the
	 * task end of the last test case -- especially the dangling SIGPOLL.
	 */

	usleep(500 * 1000);

	printf("AIO test case 5: Use list complete signal for transfer complete\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout_with_procmask;
	}

	init_aiocb(false);

	sig.sigev_notify = SIGEV_SIGNAL;
	sig.sigev_signo = SIGUSR1;
	sig.sigev_value.sival_ptr = NULL;

	ret = lio_listio(LIO_NOWAIT, g_aiocb, AIO_NCTRLBLKS, &sig);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout_with_fildes;
	}

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);

	do {
		ret = check_done();
		if (ret < 0) {
			int status = sigwaitinfo(&set, NULL);
			if (status < 0) {
				int errcode = errno;
				if (errcode == EINTR) {
					printf("aio_test: Interrupted by a signal\n");
				} else {
					printf("aio_test: ERROR: sigwaitinfo failed: %d\n", errcode);
					goto errout_with_fildes;
				}
			}
		}
	} while (ret < 0);

	close(g_fildes);
	g_fildes = -1;

	/* Case 6: Cancel I/O by AIO control block */

	/* Try to assure that this test does not overlap any activity from the
	 * task end of the last test case -- especially the dangling SIGPOLL.
	 */

	usleep(500 * 1000);

	printf("AIO test case 6: Cancel I/O by AIO control block\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout_with_procmask;
	}

	init_aiocb(false);
	ret = lio_listio(LIO_NOWAIT, g_aiocb, AIO_NCTRLBLKS, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout_with_fildes;
	}

	ret = aio_cancel(g_fildes, g_aiocb[2]);
	if (ret < 0) {
		printf("aio_test: ERROR: aio_cancel failed: %d\n", errno);
		goto errout_with_fildes;
	}

	printf("  aio_cancel return %d\n", ret);

	do {
		usleep(500 * 1000);
		ret = check_done();
	} while (ret < 0);

	close(g_fildes);
	g_fildes = -1;

	/* Case 7: Cancel I/O by file descriptor */

	/* Try to assure that this test does not overlap any activity from the
	 * task end of the last test case -- especially the dangling SIGPOLL.
	 */

	usleep(500 * 1000);

	printf("AIO test case 7:Cancel I/O by file descriptor\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout_with_procmask;
	}

	init_aiocb(false);
	ret = lio_listio(LIO_NOWAIT, g_aiocb, AIO_NCTRLBLKS, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout_with_fildes;
	}

	ret = aio_cancel(g_fildes, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: aio_cancel failed: %d\n", errno);
		goto errout_with_fildes;
	}

	printf("  aio_cancel return %d\n", ret);

	do {
		usleep(500 * 1000);
		ret = check_done();
	} while (ret < 0);

	(void)sigprocmask(SIG_SETMASK, &oset, NULL);
	printf("aio_test: Test completed successfully\n");
	return;

errout_with_fildes:
	close(g_fildes);
	g_fildes = -1;
errout_with_procmask:
	(void)sigprocmask(SIG_SETMASK, &oset, NULL);
	printf("aio_test: ERROR: Test aborted\n");
}

#endif /* CONFIG_FS_AIO */
