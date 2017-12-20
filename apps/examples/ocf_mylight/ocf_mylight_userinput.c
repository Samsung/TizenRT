/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/boardctl.h>

#include "ocf_mylight.h"

enum btn_status {
	BTN_RELEASED = 0,
	BTN_PRESSED = 1
};

static pthread_t _tid;

void ocf_mylight_recovery_default(void)
{
	ocf_mylight_reset_svr();
	ocf_mylight_reset_introspection();
}

void ocf_mylight_remove_db(void)
{
	if (unlink(SVR_DB_PATH) == 0)
		MSG("Removed SVR DB");
	else
		DBG("Failed to remove SVR DB");

	if (unlink(DEVICE_DB_PATH) == 0)
		MSG("Removed DEVICE PROPERTIES DB");
	else
		DBG("Failed to remove DEVICE PROPERTIES DB");

	if (unlink(INTROSPECTION_DB_PATH) == 0)
		MSG("Removed DEVICE PROPERTIES DB");
	else
		DBG("Failed to remove DEVICE PROPERTIES DB");
}

static void toggle_light_status(void)
{
	bool status = 0;

	if (ocf_mylight_light_get_status(&status) == 0)
		ocf_mylight_light_set_status(!status);
	else
		DBG("Failed to get light status");
}

#ifdef CONFIG_EXAMPLES_OCFMYLIGHT_DIMMER
static void change_dimm_level(void)
{
	int64_t cur = 0;

	ocf_mylight_dimm_get_level(&cur);
	ocf_mylight_dimm_set_level(cur + 1);
}
#endif

static void *loop_input(void *user_data)
{
	struct pollfd fds[2];
	int buttons[2];
	int timeout = -1;
	int nfds;
	char buf[4];
	int longpress_fd = -1;
	enum btn_status prev_status[2] = { BTN_RELEASED, BTN_RELEASED };

	/* SW702 */
	fds[0].fd = open("/dev/gpio42", O_RDWR);
	fds[0].events = POLLIN;

	/* SW703 */
	fds[1].fd = open("/dev/gpio44", O_RDWR);
	fds[1].events = POLLIN;

	while (1) {
		nfds = poll(fds, 2, timeout);
		if (nfds == -1) {
			DBG("poll() failed. (%s)", strerror(errno));
			break;
		}

		/* poll timeout */
		if (nfds == 0) {
			DBG("longpress timeout");

			if (longpress_fd == fds[0].fd) {
#ifdef CONFIG_EXAMPLES_OCFMYLIGHT_DIMMER
				MSG("Change dimmer value");
				change_dimm_level();
#endif
				/* prevent button released event */
				prev_status[0] = BTN_RELEASED;
			} else if (longpress_fd == fds[1].fd) {
				ocf_mylight_remove_db();
				ocf_mylight_recovery_default();

				MSG("Reboot board");
				boardctl(BOARDIOC_RESET, EXIT_SUCCESS);

				/* prevent button released event */
				prev_status[1] = BTN_RELEASED;
			} else {
				DBG("invalid state (fd=%d)", longpress_fd);
			}

			timeout = -1;
			longpress_fd = -1;
			continue;
		} else {
			if (timeout != -1) {
				DBG("- wait longpress: canceled");
				timeout = -1;
				longpress_fd = -1;
			}
		}

		if (fds[0].revents & POLLIN) {
			lseek(fds[0].fd, 0, SEEK_SET);
			read(fds[0].fd, buf, sizeof(buf));

			if (buf[0] == '0') {
				DBG("SW702 Button pressed");

				if (prev_status[0] == BTN_RELEASED) {
					longpress_fd = fds[0].fd;
					timeout = 2000;
				}

				prev_status[0] = BTN_PRESSED;
				DBG("- wait longpress for dimmer (%d msecs)",
					timeout);
			} else if (buf[0] == '1') {
				DBG("SW702 Button released (short click)");

				if (prev_status[0] == BTN_PRESSED)
					toggle_light_status();

				prev_status[0] = BTN_RELEASED;
			}
		}

		if (fds[1].revents & POLLIN) {
			lseek(fds[1].fd, 0, SEEK_SET);
			read(fds[1].fd, buf, sizeof(buf));

			if (buf[0] == '0') {
				DBG("SW703 Button pressed");

				if (prev_status[1] == BTN_RELEASED) {
					longpress_fd = fds[1].fd;
					timeout = 3000;
				}

				prev_status[1] = BTN_PRESSED;
				DBG("- wait longpress for recovery (%d msecs)",
					timeout);
			} else if (buf[0] == '1') {
				DBG("SW703 Button released (short click)");

				if (prev_status[1] == BTN_PRESSED) {
					DBG("- iotivity will stop");
					ocf_mylight_set_quit_flag(1);
				}

				prev_status[1] = BTN_RELEASED;
				break;
			}
		}
	}

	close(fds[0].fd);
	close(fds[1].fd);

	return NULL;
}

int ocf_mylight_userinput_init(void)
{
	pthread_create(&_tid, NULL, loop_input, NULL);
	pthread_setname_np(_tid, "ocf_mylight_userinput");

	return 0;
}
