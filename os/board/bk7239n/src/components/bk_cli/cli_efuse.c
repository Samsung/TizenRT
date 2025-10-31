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
#include <driver/efuse.h>

static void cli_efuse_help(void)
{
	CLI_LOGI("efuse_driver init\r\n");
	CLI_LOGI("efuse_driver deinit\r\n");
	CLI_LOGI("efuse_test write [addr] [data]\r\n");
	CLI_LOGI("efuse_test read [addr]\r\n");
}

static void cli_efuse_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_efuse_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_efuse_driver_init());
		CLI_LOGI("efuse driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_efuse_driver_deinit());
		CLI_LOGI("efuse driver deinit\n");
	} else {
		cli_efuse_help();
		return;
	}
}

static void cli_efuse_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_efuse_help();
		return;
	}

	uint8_t addr, data;

	if (os_strcmp(argv[1], "write") == 0) {
		addr = os_strtoul(argv[2], NULL, 16);
		data = os_strtoul(argv[3], NULL, 16);
		BK_LOG_ON_ERR(bk_efuse_write_byte(addr, data));
		CLI_LOGW("efuse write addr:0x%02x, data:0x%02x\r\n", addr, data);
	} else if (os_strcmp(argv[1], "read") == 0) {
		addr = os_strtoul(argv[2], NULL, 16);
		data = 0;
		BK_LOG_ON_ERR(bk_efuse_read_byte(addr, &data));
		CLI_LOGW("efuse read addr:0x%02x, data:0x%02x\r\n", addr, data);
	} else {
		cli_efuse_help();
		return;
	}
}



static void cli_get_efuse_value(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t addr = 0;
	uint8_t data = 0;

	BK_LOG_ON_ERR(bk_efuse_read_byte(addr, &data));
	if((data &= 1) == 1){
		CLI_LOGW("enable all security \r\n");
	}
	else{
		CLI_LOGW("enable half security \r\n");
	}
}

#define EFUSE_CMD_CNT (sizeof(s_efuse_commands) / sizeof(struct cli_command))
static const struct cli_command s_efuse_commands[] = {
	{"efuse_driver", "efuse_driver {init|deinit}", cli_efuse_driver_cmd},
	{"efuse_test", "efuse_test {write|read}", cli_efuse_cmd},
	{"get_efuse_value", NULL,cli_get_efuse_value },
};

int cli_efuse_init(void)
{
	BK_LOG_ON_ERR(bk_efuse_driver_init());
	return cli_register_commands(s_efuse_commands, EFUSE_CMD_CNT);
}
