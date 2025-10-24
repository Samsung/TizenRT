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
#include "cli.h"
#include <driver/trng.h>

static void cli_trng_help(void)
{
	CLI_LOGI("trng_driver init\r\n");
	CLI_LOGI("trng_driver deinit\r\n");
	CLI_LOGI("trng start\r\n");
	CLI_LOGI("trng stop\r\n");
	CLI_LOGI("trng get\r\n");
}

static void cli_trng_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_trng_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_trng_driver_init());
		CLI_LOGI("trng driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_trng_driver_deinit());
		CLI_LOGI("trng driver deinit\n");
	} else {
		cli_trng_help();
		return;
	}
}

static void cli_trng_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_trng_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		BK_LOG_ON_ERR(bk_trng_start());
		CLI_LOGI("trng start\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		BK_LOG_ON_ERR(bk_trng_stop());
		CLI_LOGI("trng stop\r\n");
	}else if (os_strcmp(argv[1], "get") == 0) {
		int random_data = bk_rand();
		CLI_LOGI("trng get random data:%u\r\n", random_data);
	} else {
		cli_trng_help();
		return;
	}
}

#define TRNG_CMD_CNT (sizeof(s_trng_commands) / sizeof(struct cli_command))
static const struct cli_command s_trng_commands[] = {
	{"trng_driver", "{init|deinit}", cli_trng_driver_cmd},
	{"trng", "trng {start|stop|get}", cli_trng_cmd}
};

int cli_trng_init(void)
{
	BK_LOG_ON_ERR(bk_trng_driver_init());
	return cli_register_commands(s_trng_commands, TRNG_CMD_CNT);
}


