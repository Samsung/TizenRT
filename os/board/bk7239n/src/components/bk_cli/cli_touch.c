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


#include "cli.h"


extern void cli_touch_single_channel_calib_mode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

extern void cli_touch_single_channel_manul_mode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

extern void cli_touch_multi_channel_scan_mode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

extern void cli_touch_single_channel_multi_calib_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

extern void cli_touch_multi_channel_cyclic_calib_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#if defined(CONFIG_SOC_BK7256XX)
extern void cli_touch_adc_mode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

#define TOUCH_CMD_CNT	(sizeof(s_touch_commands) / sizeof(struct cli_command))

static const struct cli_command s_touch_commands[] = {
    {"touch_single_channel_calib_mode_test", "touch_single_channel_calib_mode_test {0|1|...|15} {0|1|2|3}", cli_touch_single_channel_calib_mode_test_cmd},
    {"touch_single_channel_manul_mode_test", "touch_single_channel_manul_mode_test {0|1|...|15} {calibration_value}", cli_touch_single_channel_manul_mode_test_cmd},
    {"touch_multi_channel_scan_mode_test", "touch_multi_channel_scan_mode_test {start|stop} {0|1|2|3}", cli_touch_multi_channel_scan_mode_test_cmd},
    {"touch_single_channel_multi_calib_test", "touch_single_channel_multi_calib_test {0|1|...|15} {0|1|2|3}", cli_touch_single_channel_multi_calib_test_cmd},
    {"touch_multi_channel_cyclic_calib_test", "touch_multi_channel_cyclic_calib_test {start|stop} {0|1|2|3}", cli_touch_multi_channel_cyclic_calib_test_cmd},
#if defined(CONFIG_SOC_BK7256XX)
    {"touch_adc_mode_test", "cali, test", cli_touch_adc_mode_test_cmd},
#endif
};

int cli_touch_init(void)
{
	return cli_register_commands(s_touch_commands, TOUCH_CMD_CNT);
}




