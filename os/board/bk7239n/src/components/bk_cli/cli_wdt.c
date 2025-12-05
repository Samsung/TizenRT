// Copyright 2020-2025 Beken
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
#include <driver/wdt.h>
#include <bk_wdt.h>

static void cli_wdt_help(void)
{
	CLI_LOGI("wdt_driver init\n");
	CLI_LOGI("wdt_driver deinit\n");
	CLI_LOGI("wdt start [timeout]\n");
	CLI_LOGI("wdt stop\n");
	CLI_LOGI("wdt feed\n");
}

static void cli_wdt_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_wdt_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_wdt_driver_init());
		CLI_LOGI("wdt driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_wdt_driver_deinit());
		CLI_LOGI("wdt driver deinit\n");
	} else {
		cli_wdt_help();
		return;
	}
}

static void cli_wdt_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_wdt_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		uint32_t timeout = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_wdt_start(timeout));
		CLI_LOGI("wdt start, timeout=%d\n", timeout);
	} else if (os_strcmp(argv[1], "stop") == 0) {
		BK_LOG_ON_ERR(bk_wdt_stop());
#if (defined(CONFIG_TASK_WDT))
		bk_task_wdt_stop();
#endif
		CLI_LOGI("wdt stop\n");
	}else if (os_strcmp(argv[1], "feed") == 0) {
		BK_LOG_ON_ERR(bk_wdt_feed());
		CLI_LOGI("wdt feed\n");
	}else if (os_strcmp(argv[1], "disable") == 0) {
		bk_wdt_stop();
#if (defined(CONFIG_TASK_WDT))
		bk_task_wdt_stop();
#endif
		CLI_LOGI("wdt debug disabled\n");
	}else if (os_strcmp(argv[1], "enable") == 0) {
		extern void wdt_init(void);
		wdt_init();
		CLI_LOGI("wdt debug enabled\n");
	}else if (os_strcmp(argv[1], "while") == 0) {
		GLOBAL_INT_DECLARATION();
		GLOBAL_INT_DISABLE();
		CLI_LOGI("wdt enter while1\n");
		while(1);
		GLOBAL_INT_RESTORE();
	} else {
		cli_wdt_help();
		return;
	}
}

#define WDT_CMD_CNT (sizeof(s_wdt_commands) / sizeof(struct cli_command))
static const struct cli_command s_wdt_commands[] = {
	{"wdt_driver", "{init|deinit}", cli_wdt_driver_cmd},
	{"wdt", "wdt {start|stop|feed} [...]", cli_wdt_cmd}
};

int cli_wdt_init(void)
{
	BK_LOG_ON_ERR(bk_wdt_driver_init());
	return cli_register_commands(s_wdt_commands, WDT_CMD_CNT);
}

