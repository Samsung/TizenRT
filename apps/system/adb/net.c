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
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <netinet/in.h>

#include "net.h"
#include "adb.h"

int net_init(void)
{
	int fd;
	struct sockaddr_in addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);

	memset((void *)&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(CONFIG_SYSTEM_ADB_PORT);

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		D("bind error");
		return -1;
	}

	if (listen(fd, 1) == -1) {
		D("listen error");
		return -1;
	}

	return fd;
}

int net_wait(int sockfd)
{
	int fd;
	int len;
	struct sockaddr_in addr;

	if ((fd = accept(sockfd, (struct sockaddr *)&addr, &len)) < 0) {
		D("can't accept client\n");
		return -1;
	}

	return fd;
}

int net_read(int fd, uint16_t len, uint8_t *data)
{
	int r;
	unsigned char *p = (unsigned char *)(data);
	int size = len;
	int idx = 0;

	D("about to read (fd=%d, len=%d)\n", fd, len);

	while (true) {
		if ((r = read(fd, (p + idx), size)) <= 0) {
			D("fd(%d) has been closed\n", fd);
			return -1;
		}
		size -= r;
		idx += r;

		if (size == 0) {
			r = len;
			break;
		}
	}

	D("done fd=%d\n", fd);

	return r;
}

int net_write(int fd, uint16_t len, uint8_t *data)
{
	int r;

	D("about to write (fd=%d, len=%d)\n", fd, len);

	r = write(fd, data, len);

	if (r != len) {
		D("ERROR: fd = %d, r = %d, errno = %d (%s)\n",
			fd, r, errno, strerror(errno));
		return -1;
	}

	D("done fd=%d\n", fd);

	return r;
}
