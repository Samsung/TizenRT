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

#include "cli.h"
#include <sdio_test.h>
#include "sdio_slave_driver.h"

static void cli_sdio_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t main_case = 0, sub_case = 0;
	uint32_t param1 = 0,param2 = 0,param3 = 0,param4 = 0;

	if (argc >= 2)
		main_case = os_strtoul(argv[1], NULL, 10);
	if (argc >= 3)
		sub_case = os_strtoul(argv[2], NULL, 10);
	if (argc >= 4)
		param1 = os_strtoul(argv[3], NULL, 10);
	if (argc >= 5)
		param2 = os_strtoul(argv[4], NULL, 10);
	if (argc >= 6)
		param3 = os_strtoul(argv[5], NULL, 10);
	if (argc >= 7)
		param4 = os_strtoul(argv[6], NULL, 10);

	bk_sdio_set_test_case(main_case, sub_case, param1, param2, param3, param4);
}
static void cli_sdio_slave_repower_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	sdio_slave_hw_reinit();
}

static void cli_sdio_test_init_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_sdio_test_init();
}

#define SDIO_SLAVE_CMD_CNT (sizeof(s_sdio_slave_commands) / sizeof(struct cli_command))
static const struct cli_command s_sdio_slave_commands[] = {
	{"sdio_slave_test", "sdio_slave_test {main case}{sub case}{param1}{param2}{param3}{param4}", cli_sdio_slave_test_cmd},
	{"sdio_slave_repower", "sdio slave re-power up", cli_sdio_slave_repower_cmd},
	{"sdio_slave_test_init", "sdio slave test init", cli_sdio_test_init_cmd},
};

int cli_sdio_slave_init(void)
{
	return cli_register_commands(s_sdio_slave_commands, SDIO_SLAVE_CMD_CNT);
}

