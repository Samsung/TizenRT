/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <libtuv/uv.h>
#include <eventloop/eventloop.h>

void eventloop_loop_run(void)
{
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int eventloop_loop_stop(void)
{
	if (uv_loop_close(uv_default_loop()) < 0) {
		eldbg("Failed to cloase loop\n");
		return EVENTLOOP_BUSY;
	}

	return OK;
}
