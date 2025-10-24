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
#include "tfm_int_target_nsc.h"

static void cli_int_target_help(void)
{
	CLI_LOGI("target {inttype} {s|ns}\r\n");
}

static void cli_int_target_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_int_target_help();
		return;
	}

	uint32_t inttype = os_strtoul(argv[1], NULL, 10);
	bool target_to_s = false;

	if (os_strcmp(argv[2], "s") == 0) {
		target_to_s = true;
	} else if (os_strcmp(argv[2], "ns") == 0) {
		target_to_s = false;
	} else {
		cli_int_target_help();
		return;
	}

	psa_int_target(inttype, target_to_s);
}

#define INT_TARGET_CMD_CNT (sizeof(s_int_target_commands) / sizeof(struct cli_command))
static const struct cli_command s_int_target_commands[] = {
	{"int_target", "int_target {inttype} {s|ns}", cli_int_target_driver_cmd},
};

int cli_int_target_init(void)
{
	return cli_register_commands(s_int_target_commands, INT_TARGET_CMD_CNT);
}


