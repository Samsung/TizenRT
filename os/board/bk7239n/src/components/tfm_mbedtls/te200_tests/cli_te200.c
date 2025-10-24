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
#include "te200_test.h"
#include <os/mem.h>
#include <driver/aon_rtc.h>
#include "sys_hal.h"

#define TE200_SHA256_TEST_CNT    1

static void sys_hal_delay(volatile uint32_t times)
{
        while(times--);
}

#define TE200_SET_CPU_FREQ_TO_160M() do { \
	sys_hal_mclk_div_set(320000000/CONFIG_CPU_FREQ_HZ - 1); \
	sys_hal_delay(10000); \
	sys_hal_mclk_mux_set(0x2); \
} while(0)

static void cli_te200_help(void)
{
	// CLI_LOGI("securityip_driver init/deinit\r\n");
	// CLI_LOGI("securityip_trng cfg/get\r\n");
}

static void cli_te200_sha_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_te200_help();
		return;
	}

	int ret = 0;
	TE200_SET_CPU_FREQ_TO_160M();

	if (os_strcmp(argv[1], "256") == 0) {
		ret = te200_sha256_loop_test(32, TE200_SHA256_TEST_CNT);
		ret = te200_sha256_loop_test(1024, TE200_SHA256_TEST_CNT);
		ret = te200_sha256_loop_test(4096, TE200_SHA256_TEST_CNT);
		ret = te200_sha256_loop_test(32768, TE200_SHA256_TEST_CNT);
	} else {
		cli_te200_help();
	}

	if (0 == ret)
		CLI_LOGI("passed\r\n");
	else
		CLI_LOGE("failed\r\n");
}

static void cli_te200_aes_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_te200_help();
		return;
	}

	int ret = 0;
	TE200_SET_CPU_FREQ_TO_160M();

	if (os_strcmp(argv[1], "ecb") == 0) {
		ret = te200_aes_ecb_test();
	} else if (os_strcmp(argv[1], "cbc") == 0) {
		ret = te200_aes_cbc_test();
	} else if (os_strcmp(argv[1], "ctr") == 0) {
	 	ret = te200_aes_ctr_test();
	} else {
		cli_te200_help();
	}

	if (0 == ret)
		CLI_LOGI("passed\r\n");
	else
		CLI_LOGE("failed\r\n");
}

static void cli_te200_ecdsa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_te200_help();
		return;
	}

	int ret = 0;
	TE200_SET_CPU_FREQ_TO_160M();

	uint32_t loop_cnt = os_strtoul(argv[1], NULL, 10);
	ret = te200_ecdsa_self_test(1, loop_cnt);

	if (0 == ret)
		CLI_LOGI("passed\r\n");
	else
		CLI_LOGE("failed\r\n");
}

static void cli_te200_rsa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	TE200_SET_CPU_FREQ_TO_160M();

	ret = te200_rsa_self_test(0);

	if (0 == ret)
		CLI_LOGI("passed\r\n");
	else
		CLI_LOGE("failed\r\n");
}

#define TE200_CMD_CNT (sizeof(s_te200_commands) / sizeof(struct cli_command))
static const struct cli_command s_te200_commands[] = {
	{"te200_sha", "te200_sha {256|512} {len}", cli_te200_sha_cmd},
	{"te200_aes", "te200_aes {ecb|cbc|ctr}", cli_te200_aes_cmd},
	{"te200_ecdsa", "te200_ecdsa {10}", cli_te200_ecdsa_cmd},
	{"te200_rsa", "te200_rsa {10}", cli_te200_rsa_cmd},
};

int cli_te200_init(void)
{
	return cli_register_commands(s_te200_commands, TE200_CMD_CNT);
}
