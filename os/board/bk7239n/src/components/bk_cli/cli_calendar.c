// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <os/os.h>
#include <driver/calendar.h>
#include "cli.h"

static void cli_calendar_help(void)
{
	CLI_LOGI("calendar_driver init\r\n");
	CLI_LOGI("calendar_driver deinit\r\n");
	CLI_LOGI("calendar get_time_us\r\n");
}

static void cli_calendar_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_calendar_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_calendar_driver_init());
		CLI_LOGI("calendar driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_calendar_driver_deinit());
		CLI_LOGI("calendar driver deinit\n");
	} else {
		cli_calendar_help();
		return;
	}
}

static void cli_calendar_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_calendar_help();
		return;
	}

	if (os_strcmp(argv[1], "get_time_us") == 0) {
		uint64_t time_us = bk_calendar_get_time_us();
		CLI_LOGI("calendar get time_us:%d\r\n", time_us);
	} else {
		cli_calendar_help();
		return;
	}
}

#define CALENDAR_CMD_CNT (sizeof(s_calendar_commands) / sizeof(struct cli_command))
static const struct cli_command s_calendar_commands[] = {
	{"calendar_driver", "calendar_driver {init|deinit}", cli_calendar_driver_cmd},
	{"calendar", "calendar get_time_us", cli_calendar_cmd}
};

int cli_calendar_init(void)
{
	BK_LOG_ON_ERR(bk_calendar_driver_init());
	return cli_register_commands(s_calendar_commands, CALENDAR_CMD_CNT);
}

