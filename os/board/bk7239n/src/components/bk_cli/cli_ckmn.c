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
#include <driver/ckmn.h>
#include <ckmn_hal.h>
#include "sys_driver.h"

#define CHECK32K_CYCLE		(1023)
#define CLK_26M		(26 * 1000 * 1000)

static uint32_t g_target_26m;
static uint32_t g_target_32k;

static void cli_ckmn_help(void)
{
	CLI_LOGI("ckmn_driver {init|deinit}\r\n");
	CLI_LOGI("ckmn ckeck32k {32|other}\r\n");
	CLI_LOGI("ckmncorr {26m|32k} {0|1|2|3}\r\n");
	CLI_LOGI("ckmnautosw {26m|32k}\r\n");
	CLI_LOGI("ckmn show_reg\r\n");
}

static void cli_ckmn_show_reg(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	ckmn_struct_dump();
}

static void  cli_ckmn_ckeck32k_isr(void)
{
	double ckeck_32k = 0.0;
	int rc26m_count = 0;
	int rc32k_count = 0;

	BK_LOG_ON_ERR(bk_ckmn_ckest_disable());
	rc26m_count = bk_ckmn_get_rc26m_count();
	rc32k_count = bk_ckmn_get_rc32k_count();

	if (rc26m_count <= 0 || rc32k_count <= 0) {
		CLI_LOGI("ckmn get rc26m cycle failed!\n");
		return;
	}
	ckeck_32k = CLK_26M * (rc32k_count / ((double)rc26m_count));
	CLI_LOGI("get check 32k clock:%7.2f\n", ckeck_32k);
}

static void cli_ckmn_corr26m_isr(void)
{
	bk_ckmn_cor26m_intr_disable();
	bk_ckmn_disable_corr_26m();
	CLI_LOGI("ckmnautosw 26m interrupter happened,target:%d\r\n", g_target_26m);
}

static void cli_ckmn_corr32k_isr(void)
{
	bk_ckmn_cor32k_intr_disable();
	bk_ckmn_disable_corr_32k();
	CLI_LOGI("ckmnautosw 32k interrupter happened,target:%d\r\n", g_target_32k);
}

static void cli_ckmn_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_ckmn_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_ckmn_driver_init());
		CLI_LOGI("ckmn driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_ckmn_driver_deinit());
		CLI_LOGI("ckmn driver deinit\n");
	} else {
		cli_ckmn_help();
		return;
	}
}

static void cli_ckmn_ckeck32K(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t count;
	if (argc < 3) {
		cli_ckmn_help();
		return;
	}
	BK_LOG_ON_ERR(bk_ckmn_driver_init());
	count = os_strtoul(argv[2], NULL, 10);
	if (os_strcmp(argv[1], "ckeck32k") == 0) {
		//bk_ckmn_driver_rc32k_prog(count);
		BK_LOG_ON_ERR(bk_ckmn_set_rc32k_count(count));
		BK_LOG_ON_ERR(bk_ckmn_register_isr(CKMN_INT_CKEST, cli_ckmn_ckeck32k_isr));
		BK_LOG_ON_ERR(bk_ckmn_ckest_enable());
		CLI_LOGI("ckmn ckeck32k start\r\n");
	} else {
		cli_ckmn_help();
		return;
	}
}

static void cli_ckmn_ckmncorr(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_ckmn_help();
		return;
	}
	BK_LOG_ON_ERR(bk_ckmn_driver_init());

	if (os_strcmp(argv[1], "26m") == 0) {
		g_target_26m = os_strtoul(argv[2], NULL, 10);
#if 0
		//clock_dco_cali();
		UINT32 reg_val;
		reg_val = sys_drv_analog_get(ANALOG_REG1);
		reg_val &= ~(0x7 << 27);
		reg_val |= (0x6 << 27);
		sys_drv_analog_set(ANALOG_REG1, reg_val);
#endif
		BK_LOG_ON_ERR(bk_ckmn_set_26m_target(g_target_26m));
		BK_LOG_ON_ERR(bk_ckmn_register_isr(CKMN_INT_26M, cli_ckmn_corr26m_isr));
		BK_LOG_ON_ERR(bk_ckmn_enable_corr_26m());
		BK_LOG_ON_ERR(bk_ckmn_cor26m_intr_enable());
		CLI_LOGI("ckmnautosw 26m start\r\n");
	} else if (os_strcmp(argv[1], "32k") == 0) {
		g_target_32k = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_ckmn_set_32k_target(g_target_32k));
		BK_LOG_ON_ERR(bk_ckmn_register_isr(CKMN_INT_32K, cli_ckmn_corr32k_isr));
		BK_LOG_ON_ERR(bk_ckmn_enable_corr_32k());
		BK_LOG_ON_ERR(bk_ckmn_cor32k_intr_enable());
		CLI_LOGI("ckmnautosw 32k start\r\n");
	} else {
		cli_ckmn_help();
		return;
	}
}
static void cli_ckmn_ckmnautosw(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_ckmn_help();
		return;
	}

	if (os_strcmp(argv[1], "26m") == 0) {
		bk_ckmn_enable_autosw_26m();
		CLI_LOGI("ckmnautosw 26m start\r\n");
	} else if (os_strcmp(argv[1], "32k") == 0) {
		bk_ckmn_enable_autosw_32k();
		CLI_LOGI("ckmnautosw 32k start\r\n");
	} else {
		cli_ckmn_help();
		return;
	}
}

#define CKMN_CMD_CNT (sizeof(s_ckmn_commands) / sizeof(struct cli_command))
static const struct cli_command s_ckmn_commands[] = {
	{"ckmn_driver", "ckmn_driver {init|deinit}", cli_ckmn_driver_cmd},
	{"ckmn", "ckmn ckeck32k {32|other}", cli_ckmn_ckeck32K},
	{"ckmncorr", "ckmncorr {26m|32k} {0|1|2|3}", cli_ckmn_ckmncorr},
	{"ckmnautosw", "ckmnautosw {26m|32k}", cli_ckmn_ckmnautosw},
	{"ckmn_show_reg", "ckmn show_reg", cli_ckmn_show_reg},
};

int cli_ckmn_init(void)
{
	BK_LOG_ON_ERR(bk_ckmn_driver_init());
	return cli_register_commands(s_ckmn_commands, CKMN_CMD_CNT);
}


