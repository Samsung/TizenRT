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
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#if !defined(CONFIG_DISABLE_POLL)
#include <poll.h>
#endif

#include <tinyara/input/touchscreen.h>

#if defined(CONFIG_TOUCH_POLL)
static bool g_terminated;
#endif

#if defined(CONFIG_TOUCH_CALLBACK) 
static struct touch_set_callback_s touch_set_callback;
#endif

#if defined(CONFIG_TOUCH_POLL)
static void touch_test(void)
{
	/* read first 10 events */
	int ret;

	int fd = open(TOUCH_DEV_PATH, O_RDONLY);
	if (fd < 0) {
		printf("Error: Failed to open /dev/input0, errno : %d\n", get_errno());
		return;
	}

	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;

	struct touch_sample_s buf;
	while (!g_terminated) {
		poll(fds, 1, 5000);
		if (fds[0].revents & POLLIN) {
			ret = read(fd, &buf, sizeof(struct touch_sample_s));
			if (ret != - 1) {
				printf("Total touch points %d\n", buf.npoints);
				for (int i = 0; i < buf.npoints; i++) {
					printf("coordinates id: %d, x : %d y : %d touch type: %d\n", buf.point[i].id, buf.point[i].x, buf.point[i].y, buf.point[i].flags);
					if (buf.point[i].flags == TOUCH_DOWN) {
						printf("Touch press event \n");
					} else if (buf.point[i].flags == TOUCH_MOVE) {
						printf("Touch hold/move event \n");
					} else if (buf.point[i].flags == TOUCH_UP) {
						printf("Touch release event \n");
					}
				}
			}
		}
	}
	close(fd);
}
#elif defined(CONFIG_TOUCH_CALLBACK)
void is_touch_detected(int status)
{
	if (status == OK) {
		printf("Total touch points %d\n", touch_set_callback.touch_points->npoints);
		for (int i = 0; i < touch_set_callback.touch_points->npoints; i++) {
			printf("TOUCH COORDINATES id: %d, x : %d y : %d touch type: %d\n", touch_set_callback.touch_points->point[i].id, touch_set_callback.touch_points->point[i].x, touch_set_callback.touch_points->point[i].y, touch_set_callback.touch_points->point[i].flags);
			if (touch_set_callback.touch_points->point[i].flags == TOUCH_DOWN) {
				printf("Touch press event \n");
			} else if (touch_set_callback.touch_points->point[i].flags == TOUCH_MOVE) {
				printf("Touch hold/move event \n");
			} else if (touch_set_callback.touch_points->point[i].flags == TOUCH_UP) {
				printf("Touch release event \n");
			}
		}
	} else if (status == -EINVAL) {	// -EINVAL Error signifies touch_set_callback.touch_points is NULL
		if (touch_set_callback.touch_points == NULL) {
			touch_set_callback.touch_points = (struct touch_sample_s *)malloc(sizeof(struct touch_sample_s));
			if (!touch_set_callback.touch_points) {
				printf("Error: Touch point buffer memory allocation failed\n");
			}
		}
	} else {
		printf("Error: Touch detection failed\n");
	}
}
#endif

static int touchsceen_test_start(void)
{
	printf("touchscreen test start\n");
#if defined(CONFIG_TOUCH_POLL)
	int touch = task_create("touch", SCHED_PRIORITY_DEFAULT, 8096, (main_t)touch_test, NULL);
	if (touch < 0) {
		printf("Error: Failed to create touch reader, error : %d\n", get_errno());
		return ERROR;
	}
	g_terminated = false;
	return OK;
#elif defined(CONFIG_TOUCH_CALLBACK)
	int touch_fd = open(TOUCH_DEV_PATH, O_RDWR);
	if (touch_fd < 0) {
		printf("Error: Failed to open /dev/touch0, errno : %d\n", get_errno());
		return ERROR;
	}

	touch_set_callback.touch_points = (struct touch_sample_s *)malloc(sizeof(struct touch_sample_s));
	if (!touch_set_callback.touch_points) {
		printf("Error: Touch point buffer memory allocation failed\n");
		close(touch_fd);
		return ERROR;
	}
	touch_set_callback.is_touch_detected = is_touch_detected;

	if (ioctl(touch_fd, TSIOC_SETAPPNOTIFY, &touch_set_callback) != OK) {
		printf("ERROR: Touch callback register from application to touch driver failed\n");
		close(touch_fd);
		free(touch_set_callback.touch_points);
		return ERROR;
	}
	close(touch_fd);
#endif
}

static int touchsceen_test_stop(void)
{
#if defined(CONFIG_TOUCH_POLL)
	g_terminated = true;
#elif defined(CONFIG_TOUCH_CALLBACK)
	free(touch_set_callback.touch_points);
#endif
	printf("touchscreen test stop\n");
	return OK;
}

static int touchsceen_specific_cmd(int argc, char*argv[])
{
	struct touchscreen_cmd_s args;
	int fd = open(TOUCH_DEV_PATH, O_RDWR);
	if (fd < 0) {
		printf("Fail to open %s, errno:%d\n", TOUCH_DEV_PATH, get_errno());
		return ERROR;
	}
	args.argc = argc;
	args.argv = argv;
	if (ioctl(fd, TSIOC_CMD, (unsigned long)&args) != OK) {
		printf("Fail to ioctl %s, errno:%d\n", TOUCH_DEV_PATH, get_errno());
	}

	close(fd);
	return OK;
}


static void show_usage(void)
{
	printf("usage: touchscreen <command #>\n");
	printf("Excute touchscreen testing or controling.\n\n");
	printf("The touchscreen basic test command which printing coordinates and types:\n");
	printf("    start: Start the touchscreen basic test \n");
	printf("    stop : Stop  the touchscreen basic test\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int touchscreen_main(int argc, char *argv[])
#endif
{
	if (argc <= 1 || !strncmp(argv[1], "-h", 2) || !strncmp(argv[1], "--help", 6)) {
		show_usage();
		touchsceen_specific_cmd(0, NULL);
		return OK;
	}

	if (argc == 2) {
		if (!strcmp(argv[1], "start")) {
			return touchsceen_test_start();
		} else if (!strcmp(argv[1], "stop")) {
			return touchsceen_test_stop();
		}
	}

	return touchsceen_specific_cmd(argc, argv);
}
