/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#ifndef CONFIG_DISABLE_POLL
#include <poll.h>
#endif
#include <errno.h>
#include <sys/types.h>

#define KEY_CODE_SIZE	32

enum key_event_type {
	KEY_EVENT_TYPE_PRESS = 0,
	KEY_EVENT_TYPE_RELEASE =1
};

struct key_event {
	char key_code[KEY_CODE_SIZE];
	enum key_event_type type;
};

static void virtkey_send(void)
{
	/* send 10 events via virtkey driver */

	int ret;

	int fd = open("/dev/virtkey", O_WRONLY);

	if (fd < 0) {
		printf("Failed to open /dev/virtkey, errno : %d\n", get_errno());
		return NULL;
	}

	struct key_event key;

	for (int i = 0; i < 100; i++) {

		int k = i % 10;

		key.type = k % 2;
		
		memset(key.key_code, '0' + k, KEY_CODE_SIZE - 1);
		
		key.key_code[KEY_CODE_SIZE - 1] = '\0';

		while (true) {
			ret = write(fd, &key, sizeof(struct key_event));
			
			if (ret == 0) {
				/* buffer may be full, try again after sometime
				 * TODO here we have not added poll as we are not
				 * sure regarding the requirement, may be if write
				 * fails, we may need to drop the data and previous
				 * data is still not processed, sleep and retry was
				 * added just for the case of this example */
				usleep(20);
			} else {
				printf("\nsender[%d] : sent event %s %d, ret : %d\n", i, key.key_code, key.type, ret);
				break;
			}
		}
	}
	
	close(fd);

	return;
}

static void virtkey_receive(void)
{
	/* read first 10 events */
	char c;
	int ret;

	int fd = open("/dev/virtkey", O_RDONLY);

	if (fd < 0) {
		printf("Failed to open /dev/virtkey, errno : %d\n", get_errno());
		return NULL;
	}

	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;

	struct key_event key;

	for (int i = 0; i < 100; i++) {

		while (true) {
			poll(fds, 1, -1);
			if (fds[0].revents & POLLIN) {
				break;
			}
		}	
		/* since we are the only reader, no need to check failure case
		 * as we are garunteed that there is data to read by POLLIN */
		ret = read(fd, &key, sizeof(struct key_event));
		printf("\nreceiver[%d] : received event %s %d\n", i, key.key_code, key.type);
	}

	close(fd);

	return 0;
}

/****************************************************************************
 * virtkey_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int virtkey_main(int argc, char *argv[])
#endif
{
	pid_t send;
	pid_t recv;

	send = task_create("vkeysend", SCHED_PRIORITY_DEFAULT, 1024, virtkey_send, NULL);

	if (send < 0) {
		printf("failed to create sender, error : %d\n", get_errno());
		return 0;
	}

	recv = task_create("vkeyrcv", SCHED_PRIORITY_DEFAULT, 1024, virtkey_receive, NULL);

	if (recv < 0) {
		printf("failed to create receiver, error : %d\n", get_errno());
		task_delete(send);
		return 0;
	}

	return 0;
}
