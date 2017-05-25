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

enum watch_io {
	WATCH_IO_IN = (1 << 0),	/**< data input available */
	WATCH_IO_OUT = (1 << 1), /**< data can be written */
	WATCH_IO_PRI = (1 << 2), /**< high priority input available */
	WATCH_IO_ERR = (1 << 3), /**< i/o error */
	WATCH_IO_HUP = (1 << 4), /**< Hung up. device disconnected */
	WATCH_IO_NVAL = (1 << 5)
					/**< invalid request. the file descriptor is not open */
};

typedef void (*timeout_callback)(void *user_data);
typedef int (*watch_callback)(int fd, enum watch_io io, void *user_data);
typedef int (*idle_callback)(void *user_data);

void libtuv_loop_run(void);
void libtuv_loop_quit(void);
int libtuv_add_timeout_callback(unsigned int msec, timeout_callback func, void *user_data);
int libtuv_add_fd_watch(int fd, enum watch_io io, watch_callback func, void *user_data, int *watch_id);
int libtuv_add_idle_callback(idle_callback func, void *user_data);
