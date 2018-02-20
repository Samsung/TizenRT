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

#include <tinyara/ascii.h>
#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

#include "adb.h"
#include "shell.h"

#define STDOUT_FILENO	   1
#define STDERR_FILENO	   2

#define ADB_SHELL_BUFSIZE 128

static sem_t event;

static int readline(int fd, int wfd, char *buf)
{
	int bufsize = ADB_SHELL_BUFSIZE;
	int pos = 0;
	int nbytes = 0;
	int char_idx = 0;
	fd_set tfd;
	struct timeval stimeout;

	stimeout.tv_sec = 6;
	stimeout.tv_usec = 0;

	do {
		FD_ZERO(&tfd);
		FD_SET(fd, &tfd);

		if ((select(fd + 1, &tfd, NULL, NULL, &stimeout)) && FD_ISSET(fd, &tfd)) {
			/* read characters */
			nbytes = read(fd, &buf[pos], (bufsize - pos));
			if (nbytes < 0) {
				fprintf(stderr, "can not read\n");
				return 0;
			}

			for (char_idx = 0; char_idx < nbytes; char_idx++) {
				/* treat backspace and delete */
				if ((buf[pos] == ASCII_BS) || (buf[pos] == ASCII_DEL)) {
					int valid_char_pos = pos + 1;

					if (pos > 0) {
						pos--;
						/* Update screen */
						if (write(wfd, "\b \b", 3) <= 0) {
							fprintf(stderr, "echo failed (errno = %d)\n", get_errno());
						}
					}

					if ((buf[valid_char_pos] != 0x0) && (valid_char_pos < ADB_SHELL_BUFSIZE)) {
						memmove(&buf[pos], &buf[valid_char_pos], (bufsize - valid_char_pos));
					}
				} else {
					if (buf[pos] == ASCII_CR) {
						buf[pos] = ASCII_LF;
					}

					putchar(buf[pos]);
					fflush(stdout);

					pos++;
					if (pos >= ADB_SHELL_BUFSIZE) {
						fprintf(stderr, "length of input character is too long, maximum length is %d\n", ADB_SHELL_BUFSIZE);
						buf[0] = ASCII_NUL;
						return pos;
					}
				}
			}
		}
	} while (buf[pos - 1] != ASCII_LF);

	buf[pos - 1] = ASCII_NUL;
	return pos;
}

static int adb_shell(int argc, char *argv[])
{
	int r;
	int rfd; /* connect to adb */
	int wfd;
	int fd1, fd2;
	char buf[ADB_SHELL_BUFSIZE];
	aconnection *conn;

	/* prompt should have null ptr */
	char *prompt = "TASH>>\0";

	while (true) {
		rfd = open(ADB_SHELL_IN_PATH, O_RDONLY);
		D("shell(cmd) <- arbiter fd = (%d)\n", rfd);

		wfd = open(ADB_SHELL_OUT_PATH, O_WRONLY);
		D("shell(result) -> arbiter fd = (%d)\n", wfd);

		sem_wait(&event);

		D("shell is running\n");

		conn = getconn();

		fd1 = dup2(wfd, STDOUT_FILENO);
		fd2 = dup2(wfd, STDERR_FILENO);

		while (conn->status != ADB_CLOSED) {
			if (conn->status == ADB_SHELL) {
				write(wfd, prompt, 7);

				readline(rfd, wfd, buf);

				if (strncmp(buf, "exit", 4) == 0) {
					conn->status = ADB_CLOSED;
					printf("\n");

					break;
				}

				tash_execute_cmdline(buf);
			} else if (conn->status == ADB_COMMAND) {
				memset(buf, 0, ADB_SHELL_BUFSIZE);
				r = read(rfd, buf, ADB_SHELL_BUFSIZE);

				tash_execute_cmdline(buf);
				printf("\n");

				break;
			}
		}

		/* restore original stdout/stderr */

		dup2(fd1, STDOUT_FILENO);
		dup2(fd2, STDERR_FILENO);

		D("done\n");

		close(rfd);
		close(wfd);
	}

	return 0;
}

void adb_initshell(void)
{
	D("initialize\n");

	mkfifo(ADB_SHELL_IN_PATH,  0666);
	mkfifo(ADB_SHELL_OUT_PATH, 0666);

	sem_init(&event, 0, 0);

	sem_setprotocol(&event, SEM_PRIO_NONE);

	task_create("adb_shell",
				CONFIG_SYSTEM_ADB_SHELL_PRIORITY,
				CONFIG_SYSTEM_ADB_SHELL_STACKSIZE,
				adb_shell,
				NULL);
}

void shell_wakeup(void)
{
	sem_post(&event);
}
