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

#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/stat.h>

#include "adb.h"
#include "file.h"

#define SYNC_DATA_MAX   4096 /* 65536 */

static sem_t event;

void adb_initfile(void)
{
	mkfifo(ADB_FSYNC_IN_PATH, 0666);
	mkfifo(ADB_FSYNC_OUT_PATH, 0666);

	sem_init(&event, 0, 0);

	sem_setprotocol(&event, SEM_PRIO_NONE);

	task_create("adb_fsync",
				SCHED_PRIORITY_DEFAULT,
				CONFIG_IDLETHREAD_STACKSIZE,
				adb_fsync,
				NULL);
}

static int fail_message(int s, const char *reason)
{
	syncmsg msg;
	int len = strlen(reason);

	D("sync: failure: %s\n", reason);

	msg.data.id = ID_FAIL;
	msg.data.size = htoll(len);
	if (write(s, &msg.data, sizeof(msg.data) < 0) ||
		write(s, reason, len) < 0) {
		return -1;
	} else {
		return 0;
	}
}

static int fail_errno(int s)
{
	return fail_message(s, (const char *)strerror(errno));
}

static int handle_send_file(int rfd, int wfd, char *path, mode_t mode, char *buffer)
{
	syncmsg msg;
	unsigned int timestamp = 0;
	int fd;
	int max;
	int r = 0;
	fd_set rfds;
	int cur;
	int size;
	int n;

	fd = open(path, O_WRONLY | O_CREAT | O_EXCL);
	if (fd < 0 && errno == ENOENT) {
		mkdir(path, 0777);
		fd = open(path, O_WRONLY | O_CREAT | O_EXCL);
	}
	if (fd < 0 && errno == EEXIST) {
		fd = open(path, O_WRONLY);
	}
	if (fd < 0) {
		if (fail_errno(wfd))
			return -1;
		fd = -1;
	}

	for (;;) {
		unsigned int len;

		FD_ZERO(&rfds);
		FD_SET(rfd, &rfds);

		n = select(rfd + 1, &rfds, NULL, NULL, NULL);

		if (n > 0) {
			if (FD_ISSET(rfd, &rfds)) {
				if ((r = read(rfd, &msg.data, sizeof(msg.data))) <= 0) {
					goto fail;
				}
			}
		}

		if (msg.data.id != ID_DATA) {
			if (msg.data.id == ID_DONE) {
				timestamp = ltohl(msg.data.size);
				break;
			}
			fail_message(wfd, "invalid data message");
			goto fail;
		}
		len = ltohl(msg.data.size);
		cur = 0;
		size = len;

		while (len) {
			max = len > SYNC_DATA_MAX ? SYNC_DATA_MAX : len;
			FD_ZERO(&rfds);
			FD_SET(rfd, &rfds);

			n = select(rfd + 1, &rfds, NULL, NULL, NULL);

			if (n > 0) {
				if (FD_ISSET(rfd, &rfds)) {
					r = read(rfd, buffer, max);

					D("recv=(%d)\n", r);
					if (write(fd, buffer, r) < 0) {
						int saved_errno = errno;

						close(fd);
						fd = -1;
						errno = saved_errno;
						if (fail_errno(wfd)) {
							return -1;
						}
					}
				}

				len -= r;
				cur += r;
				D("r=%d, [%d:%d]\n", r, cur, size);
			}
		}
	}

	D("Send OK\n");

	if (fd >= 0) {
		close(fd);

		msg.status.id = ID_OKAY;
		msg.status.msglen = 0;
		if (write(wfd, &msg.status, sizeof(msg.status)) < 0)
			return -1;
	}
	return 0;

fail:
	D("r = %d\n", r);
	if (fd >= 0)
		close(fd);
	return -1;
}

static int do_stat(int rfd, int wfd, const char *path)
{
	syncmsg msg;
	struct stat st;

	msg.stat.id = ID_STAT;

	if (stat(path, &st)) {
		msg.stat.mode = 0;
		msg.stat.size = 0;
		msg.stat.time = 0;
	} else {
		msg.stat.mode = htoll(st.st_mode);
		msg.stat.size = htoll(st.st_size);
		msg.stat.time = htoll(st.st_mtime);
	}

	return write(wfd, &msg.stat, sizeof(msg.stat));
}

static int do_recv(int wfd, const char *path, char *buffer)
{
	syncmsg msg;
	int fd, r;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		if (fail_errno(wfd)) {
			return -1;
		}
		return 0;
	}

	msg.data.id = ID_DATA;
	for (;;) {
		r = read(fd, buffer, SYNC_DATA_MAX);
		if (r <= 0) {
			if (r == 0) {
				break;
			}
			if (errno == EINTR) {
				continue;
			}
			r = fail_errno(wfd);
			close(fd);
			return r;
		}
		msg.data.size = htoll(r);
		if (write(wfd, &msg.data, sizeof(msg.data)) <= 0 ||
			write(wfd, buffer, r) <= 0) {
			close(fd);
			return -1;
		}
	}

	close(fd);

	usleep(100000);

	msg.data.id = ID_DONE;
	msg.data.size = 0;
	if (write(wfd, &msg.data, sizeof(msg.data)) <= 0) {
		return -1;
	}

	return 0;
}

static int do_send(int rfd, int wfd, char *path, char *buffer)
{
	char *tmp;
	mode_t mode;
	int ret;

	D("call\n");

	tmp = (char *)strrchr(path, ',');

	if (tmp) {
		*tmp = 0;
		errno = 0;
		mode = strtoul(tmp + 1, NULL, 0);
		mode &= 0777;
	}
	if (!tmp || errno) {
		mode = 0644;
	}

	/* copy user permission bits to "group" and "other" permissions */
	mode |= ((mode >> 3) & 0070);
	mode |= ((mode >> 3) & 0007);

	ret = handle_send_file(rfd, wfd, path, mode, buffer);

	return ret;
}

static void sync_quit(int fd)
{
	syncmsg msg;

	msg.req.id = ID_QUIT;
	msg.req.namelen = 0;

	write(fd, &msg.req, sizeof(msg.req));
}

void execute_fsync(const char *data, int len)
{
	int r;
	int fd;
	int max;
	int cur;
	char *p;
	int pipe = CONFIG_DEV_PIPE_SIZE / 2;

	fd = open("/dev/adb_fsync_in", O_WRONLY);

	D("send command fd=%d, len=%d\n", fd, len);

	cur = 0;

	while (len) {
		p = (char *)(data + cur);

		max = (len >= pipe) ? pipe : len;

		r = write(fd, p, max);

		len -= r;
		cur += r;

		D("fd=%d, r=%d remain=%d\n", fd, r, len);
	}

	close(fd);
}

int adb_fsync(int argc, char **argv)
{
	int rfd = 0;
	int wfd = 0;
	syncmsg msg;
	char name[CONFIG_NAME_MAX + 1];
	unsigned namelen;
	fd_set rfds;

	char *buffer = (char *)malloc(SYNC_DATA_MAX);

	for (;;) {
		rfd = open(ADB_FSYNC_IN_PATH,  O_RDONLY);
		D("file(in) <- arbiter fd=%d\n", rfd);

		wfd = open(ADB_FSYNC_OUT_PATH, O_WRONLY);
		D("file(out) <- arbiter fd=%d\n", wfd);

		memset((void *)&msg, 0, sizeof(syncmsg));

		sem_wait(&event);

		while (true) {
			FD_ZERO(&rfds);
			FD_SET(rfd, &rfds);

			D("sync: waiting for command\n");

			int n = select(rfd + 1, &rfds, NULL, NULL, NULL);

			if (n > 0) {
				if (FD_ISSET(rfd, &rfds)) {
					if (read(rfd, &msg.req, sizeof(msg.req)) <= 0) {
						fail_message(wfd, "command read failure");
						break;
					}
				}
			}
			D("got msg\n");

			if (msg.req.id == ID_QUIT) {
				sync_quit(wfd);
				break;
			}

			namelen = ltohl(msg.req.namelen);
			if (namelen > CONFIG_NAME_MAX) {
				fail_message(wfd, "invalid namelen");
				break;
			}

			D("namelen=%d\n", namelen);

			n = select(rfd + 1, &rfds, NULL, NULL, NULL);

			if (n > 0) {
				if (FD_ISSET(rfd, &rfds)) {
					if (read(rfd, name, namelen) <= 0) {
						fail_message(wfd, "filename read failure");
						break;
					}
				}
			}

			name[namelen] = 0;

			msg.req.namelen = 0;
			D("sync: '%s' '%s'\n", (char *) &msg.req, name);

			switch (msg.req.id) {
			case ID_STAT:
				if (do_stat(rfd, wfd, name) < 0)
					break;
				break;
			/* case ID_LIST: break; */
			case ID_SEND:
				if (do_send(rfd, wfd, name, buffer) < 0)
					break;
				break;
			case ID_RECV:
				if (do_recv(wfd, name, buffer))
					break;
				break;
			case ID_QUIT:
				break;
			default:
				fail_message(wfd, "unknown command");
				break;
			}
		}

		D("sync: done\n");

		close(rfd);
		close(wfd);
	}

	close(rfd);
	close(wfd);

	return 0;
}

void fsync_wakeup(void)
{
	sem_post(&event);
}
