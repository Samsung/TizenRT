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
#include "tfm_mpc_nsc.h"

static void cli_mpc_help(void)
{
	CLI_LOGI("mpc {dev} {addr} {block_num} {s|ns}\r\n");
}

static void cli_mpc_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc == 1) {
		psa_mpc_dump();
		return;
	} else if (argc < 5) {
		cli_mpc_help();
		return;
	}

	uint32_t dev = os_strtoul(argv[1], NULL, 10);
	uint32_t addr = os_strtoul(argv[2], NULL, 16);
	uint32_t block_num = os_strtoul(argv[3], NULL, 10);
	uint32_t type = 0;

	if (os_strcmp(argv[4], "s") == 0) {
		type = 0;
	} else if (os_strcmp(argv[4], "ns") == 0) {
		type = 1;
	} else {
		cli_mpc_help();
		return;
	}

	psa_mpc_set_secure_attribute(dev, addr, block_num, type);
}

#define MPC_CMD_CNT (sizeof(s_mpc_commands) / sizeof(struct cli_command))
static const struct cli_command s_mpc_commands[] = {
	{"mpc", "mpc {dev} {addr} {block_num} {s|ns}", cli_mpc_driver_cmd},
};

int cli_mpc_init(void)
{
	return cli_register_commands(s_mpc_commands, MPC_CMD_CNT);
}


