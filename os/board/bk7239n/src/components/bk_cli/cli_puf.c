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
#include "sys_hal.h"
#include "bk_misc.h"

static void cli_puf_help(void)
{
	CLI_LOGI("puf {version|enrollment|read_uid}\r\n");
}

static void cli_puf_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_puf_help();
		return;
	}

	/* power on */
	sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(0);
	delay_ms(10);

	if (os_strcmp(argv[1], "version") == 0) {
		uint32_t puf_version = REG_READ(SOC_OTP_APB_BASE + 0x02c0);
		CLI_LOGI("PTM_VERSION:%x\r\n", puf_version);
	} else if (os_strcmp(argv[1], "enrollment") == 0) {
		bool puf_is_busy = true;
		uint32_t loop_cnt = 0;
		REG_WRITE(SOC_OTP_APB_BASE + 0x02e4, 0xa7);

		while (puf_is_busy && (loop_cnt < 1000000)) {
			puf_is_busy = REG_READ(SOC_OTP_APB_BASE + 0x02c4);
			loop_cnt++;
		}

		uint32_t puf_info_block = REG_READ(SOC_OTP_APB_BASE);

		/* PUF offset 0, bit[16:19] */
		uint32_t enrol_state = (puf_info_block >> 16) & 0xf;
		if (enrol_state == 0xf) {
			CLI_LOGI("PUF enrollment done\r\n");
		} else {
			CLI_LOGI("PUF incomplete enrollment\r\n");
		}
	} else if (os_strcmp(argv[1], "read_uid") == 0) {
		uint32_t uid = REG_READ(SOC_OTP_APB_BASE + 0x0300);
		CLI_LOGI("PUF uid:%x\r\n", uid);
	} else {
		cli_puf_help();
		return;
	}
}

#define PUF_CMD_CNT (sizeof(s_puf_commands) / sizeof(struct cli_command))
static const struct cli_command s_puf_commands[] = {
	{"puf", "puf {version|enrollment|read_uid}", cli_puf_cmd},
};

int cli_puf_init(void)
{
	return cli_register_commands(s_puf_commands, PUF_CMD_CNT);
}
