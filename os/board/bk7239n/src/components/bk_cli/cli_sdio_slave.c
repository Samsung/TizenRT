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

//#include <os/os.h>
#include "cli.h"
#include <sdio_test.h>

#if 0
static void cli_sdio_slave_help(void)
{
	CLI_LOGI("sdio_slave_test {main case}{sub case}{param1}{param2}{param3}{param4}\r\n");
}
#endif

beken_thread_t loop_test_handle = NULL;
uint32_t looptest_flag = 0;

static void loop_test(void *arg) {
	uint32_t delay_time = 5000;
	bk_sdio_set_test_case(1, 0, 0, 1024, 1, 1);  //1 0 0 1024 1
	while (1) {
		bk_sdio_set_test_case(1, 1, 0, 1024, 80, 1); //1 1 0 1024 80   rx, single_packet, chan_id, len, value, packet_cnt
		bk_sdio_set_test_case(0, 0, 0, 1024, 1, 1);  //0 0 0 1024 1
		bk_sdio_set_test_case(0, 1, 0, 1024, 12345678, 1);
		os_printf("======delay %d ms.=======\n", delay_time);
		rtos_delay_milliseconds(delay_time);
	}
	rtos_delete_thread(&loop_test_handle);
}

static void cli_sdio_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t main_case = 0, sub_case = 0;
	uint32_t param1 = 0,param2 = 0,param3 = 0,param4 = 0;

	if (os_strcmp(argv[1], "loopstart") == 0) {
		uint32_t task_prio = 5;
		os_printf("loop_test task start: task_prio = %u.\n", task_prio);
		rtos_create_thread(&loop_test_handle, task_prio,
			"loop_test",
			(beken_thread_function_t) loop_test,
			CONFIG_APP_MAIN_TASK_STACK_SIZE,
			(beken_thread_arg_t)0);
		looptest_flag = 1;
		return;
	} else if (os_strcmp(argv[1], "loopstop") == 0) {
		if (loop_test_handle) {
			rtos_delete_thread(&loop_test_handle);
			loop_test_handle = NULL;
			os_printf("loop_test task stop\n");
		}
		looptest_flag = 0;
		return;
	}

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

#define SDIO_SLAVE_CMD_CNT (sizeof(s_sdio_slave_commands) / sizeof(struct cli_command))
static const struct cli_command s_sdio_slave_commands[] = {
	{"sdio_slave_test", "sdio_slave_test {main case}{sub case}{param1}{param2}{param3}{param4}", cli_sdio_slave_test_cmd},
};

int cli_sdio_slave_init(void)
{
	return cli_register_commands(s_sdio_slave_commands, SDIO_SLAVE_CMD_CNT);
}

