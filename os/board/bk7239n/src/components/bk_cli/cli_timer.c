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
#include <driver/timer.h>
#include "bk_misc.h"

static void cli_timer_isr(timer_id_t chan)
{
    CLI_LOGI("[TIMER][ISR] chan:%d\r\n", chan);
}

static void cli_delay_us(int argc, char **argv)
{
	uint32_t us = 0;

	if (argc < 3) {
		CLI_LOGI("timer delay [count]\r\n");
		return;
	}

	us = os_strtoul(argv[2], NULL, 10);
	uint32_t level = rtos_enter_critical();
	delay_us(us);
	rtos_exit_critical(level);
	CLI_LOGI("delayed %u us\r\n", us);
}

static void cli_timer_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int ret = 0;
    uint8_t channel = 0;
    uint32_t time_ms = 0, read_cnt = 0;

    if (os_strcmp(argv[1], "delay") == 0) {
        cli_delay_us(argc, argv);
    } else {
        channel = os_strtoul(argv[1], NULL, 10);
    }

    if (os_strcmp(argv[2], "start") == 0) {
        time_ms = os_strtoul(argv[3], NULL, 10);
        CLI_LOGI("[TIMER][START] channel:%d, time_ms:%d\r\n", channel, time_ms);
        ret = bk_timer_start(channel, time_ms, cli_timer_isr);
        if (ret != BK_OK) {
            CLI_LOGW("[TIMER][START] start failed, error code:%x\n", ret);
        }
    } else if (os_strcmp(argv[2], "stop") == 0) {
        CLI_LOGI("[TIMER][STOP] channel:%d\r\n", channel);
        bk_timer_stop(channel);
    } else if (os_strcmp(argv[2], "read") == 0) {
        read_cnt = bk_timer_get_cnt(channel);
        CLI_LOGI("[TIMER][READ] read cnt:%x\r\n", read_cnt);
    } else if (os_strcmp(argv[2], "enable") == 0) {
        bk_timer_enable(channel);
        CLI_LOGI("[TIMER][ENABLE] channel:%x\r\n", channel);
    } else if (os_strcmp(argv[2], "disable") == 0) {
        bk_timer_disable(channel);
        CLI_LOGI("[TIMER][DISABLE] channel:%x\r\n", channel);
    } else if (os_strcmp(argv[2], "get_period") == 0) {
        read_cnt = bk_timer_get_period(channel);
        CLI_LOGI("[TIMER][GET][PERIOD] period value:%x\r\n", read_cnt);
    } else {
        CLI_LOGW("timer {chan} {start|stop|read} [...]\r\n");
    }
}

#define TIMER_CMD_CNT (sizeof(s_timer_commands) / sizeof(struct cli_command))
static const struct cli_command s_timer_commands[] = {
    {"timer", "timer {chan} {start|stop|read} [...]", cli_timer_cmd},
};

int cli_timer_init(void)
{
    BK_LOG_ON_ERR(bk_timer_driver_init());
    return cli_register_commands(s_timer_commands, TIMER_CMD_CNT);
}

