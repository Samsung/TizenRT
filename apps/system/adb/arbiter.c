/* Copyright 2018 Byoungtae Cho <bt.cho@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

#include "adb.h"
#include "arbiter.h"

#define ADB_ARBITER_BUFSIZE		(CONFIG_DEV_PIPE_SIZE)

static pthread_t tid;
static pthread_attr_t attr;

void stop_arbiter(void)
{
	int status = 0;

	pthread_join(tid, (void **)&status);
	pthread_attr_destroy(&attr);
}

static pthread_addr_t arbiter_thread(void *param)
{
	int fd = 0;
	int cfd = 0;
	int rfd = 0;
	int sfd = 0;
	int maxfd = 0;
	int r;
	fd_set rfds;
	char *buf;
	struct timeval tv;
	aarbiter *arbiter = (aarbiter *)param;

	atransport  *t = arbiter->t;
	aconnection *conn = arbiter->conn;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	D("created!\n");

	buf = (char *)malloc(ADB_ARBITER_BUFSIZE);

	fd = open(ADB_ARBITER_PATH, O_RDWR);
	D("adb <-> arbiter fd = (%d)\n", fd);

	if (conn->status != ADB_FILE) {
		cfd = open(ADB_SHELL_IN_PATH, O_WRONLY);
		D("arbiter -> shell (cmd) fd = (%d)\n", cfd);

		rfd = open(ADB_SHELL_OUT_PATH, O_RDONLY);
		D("arbiter <- shell (result) fd = (%d)\n", rfd);

		maxfd = (rfd > fd) ? rfd : fd;
	} else {
		sfd = open(ADB_FSYNC_OUT_PATH, O_RDONLY);
		D("arbiter <- file fd = (%d)\n", sfd);

		maxfd = (sfd > fd) ? sfd : fd;
	}

	while (conn->arbiter) {
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		if (conn->status != ADB_FILE) {
			FD_SET(rfd, &rfds);
		} else {
			FD_SET(sfd, &rfds);
		}

		int n = select(maxfd + 1, &rfds, NULL, NULL, &tv);

		if (n > 0) {
			if (FD_ISSET(fd, &rfds)) {
				r = read(fd, buf, ADB_ARBITER_BUFSIZE);

				D("from adb, r = %d\n", r);

				/* send to service */
				write(cfd, buf, r);
			}
			if (conn->status != ADB_FILE) {
				if (FD_ISSET(rfd, &rfds)) {
					r = read(rfd, buf, ADB_ARBITER_BUFSIZE);

					D("from shell, r = %d\n", r);

					/* send to ADB */
					send_msg(conn, t, (uint8_t *)buf, r);

					if (conn->status == ADB_CLOSED
						|| conn->status == ADB_COMMAND) {
						send_close(conn, t);
					}
				}
			} else {
				if (FD_ISSET(sfd, &rfds)) {
					r = read(sfd, buf, ADB_ARBITER_BUFSIZE);

					D("from file, r = %d\n", r);

					/* send to ADB */
					send_msg(conn, t, (uint8_t *)buf, r);
				}
			}
		}
	}

	free(buf);
	free(arbiter);

	close(fd);

	if (conn->status == ADB_FILE) {
		close(sfd);
	} else {
		close(cfd);
		close(rfd);
	}

	return 0;
}

void execute_command(const char *cmd, int len)
{
	int fd;

	fd = open(ADB_ARBITER_PATH, O_WRONLY);

	D("send command fd=%d, len=%d\n", fd, len);
	write(fd, cmd, strlen(cmd));

	close(fd);
}

void adb_initarbiter(void)
{
	D("Initialize\n");

	mkfifo(ADB_ARBITER_PATH, 0666);
}

int run_arbiter(aconnection *conn, atransport *t)
{
	int status = 0;
	aarbiter *arbiter;

	arbiter = (aarbiter *)malloc(sizeof(arbiter));

	arbiter->conn = conn;
	arbiter->t = t;

	pthread_attr_init(&attr);
	status = pthread_attr_setstacksize(&attr, CONFIG_IDLETHREAD_STACKSIZE);
	pthread_create(&tid, &attr, arbiter_thread, (void *)arbiter);
	if (status)
		goto exit;

	pthread_setname_np(tid, "adb_arbiter");

exit:
	return status;
}
