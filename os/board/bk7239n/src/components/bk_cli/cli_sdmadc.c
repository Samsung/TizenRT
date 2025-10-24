// Copyright 2022-2023 Beken
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
#include <driver/sdmadc.h>

static void cli_sdmadc_help(void)
{
	CLI_LOGI("sdmadc init/deinit-------------------driver init and sdmadc init\r\n");
	CLI_LOGI("sdmadc single_read {channel}--------------read average with default single mode config\r\n");
	CLI_LOGI("sdmadc continuous_read {channel}{read_size}---read average with default continuous mode config\r\n");
	CLI_LOGI("sdmadc read--------------------------read average with config set by tester\r\n");
	CLI_LOGI("sdmadc set_config  {samp_mode}{samp_numb}{samp_chan}{comp_bpss}{int_enbale}{cali_offset}{cali_gains}\r\n");
	CLI_LOGI("sdmadc set_channel {channel}\r\n");
	CLI_LOGI("sdmadc set_mode    {mode}\r\n");
}

static void cli_sdmadc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_sdmadc_driver_init());
		BK_LOG_ON_ERR(bk_sdmadc_init());
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_sdmadc_deinit());
		BK_LOG_ON_ERR(bk_sdmadc_driver_deinit());
	} else if (os_strcmp(argv[1], "set_mode") == 0) {
		sdmadc_mode_t mode;
		mode = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_sdmadc_set_sample_mode(mode));
	} else if (os_strcmp(argv[1], "set_channel") == 0) {
		uint32_t chan;
		chan = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_sdmadc_set_channel(chan));
	} else if (os_strcmp(argv[1], "single_read") == 0) {
		int16_t result = 0;
		uint16_t channel = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_sdmadc_single_read(&result, channel));
		os_printf("average = %d\r\n",result);
	} else if (os_strcmp(argv[1], "continuous_read") == 0) {
		int16_t result = 0;
		uint16_t channel = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3   ], NULL, 10);
		BK_LOG_ON_ERR(bk_sdmadc_continuous_read(&result, size, channel));
		os_printf("average = %d\r\n",result);
	} else if (os_strcmp(argv[1], "read") == 0) {
		int16_t result = 0;
		uint32_t size = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_sdmadc_read_average(&result, size));
		os_printf("average = %d\r\n",result);
	} else if (os_strcmp(argv[1], "set_config") == 0) {
		sdmadc_config_t config;
		config.samp_mode = os_strtoul(argv[2], NULL, 10);
		config.samp_numb = os_strtoul(argv[3], NULL, 10);
		config.samp_chan = os_strtoul(argv[4], NULL, 10);
		config.comp_bpss = os_strtoul(argv[5], NULL, 10);
		config.cic2_bpss = os_strtoul(argv[6], NULL, 10);
		config.cic2_gain = os_strtoul(argv[7], NULL, 10);
		config.int_enable = os_strtoul(argv[8], NULL, 10);
		config.cali_offset = os_strtoul(argv[9], NULL, 10);
		config.cali_gains = os_strtoul(argv[10], NULL, 10);
		BK_LOG_ON_ERR(bk_sdmadc_set_cfg(&config));
	} else {
		cli_sdmadc_help();
		return;
	}
}

#define SDMADC_CMD_CNT (sizeof(s_sdmadc_commands) / sizeof(struct cli_command))
static const struct cli_command s_sdmadc_commands[] = {
	{"sdmadc", "sdmadc_test", cli_sdmadc_cmd}
};

int cli_sdmadc_init(void)
{
	return cli_register_commands(s_sdmadc_commands, SDMADC_CMD_CNT);
}