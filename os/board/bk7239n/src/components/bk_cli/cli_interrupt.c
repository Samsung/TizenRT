// Copyright 2022-2025 Beken
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
// See the License for the interruptcific language governing permissions and
// limitations under the License.

#include <os/os.h>
#include "soc/soc.h"
#include "interrupt_base.h"
#include "cli.h"
#if defined(CONFIG_SOC_BK7239XX)
#include "arch_interrupt.h"
#else
#include "interrupts.h"
#endif
static void cli_interrupt_help(void)
{
	CLI_LOGI("int help - display this message\n");
	CLI_LOGI("int enable {irq} {0|1} - enable irq\n");
	CLI_LOGI("int enable bits {irq_group0} {irq_group1} - enable irq by bits\n");
	CLI_LOGI("int target {irq} {0|1} - target irq\n");
	CLI_LOGI("int target bits {irq_group0} {irq_group1} - target irq by bits\n");
	CLI_LOGI("int state - dump irq enable/target state\n");
	CLI_LOGI("int statis - dump irq statis\n");
}

static void cli_dump_int_state(void)
{
    uint64_t int_ena_st = 0;
	uint64_t int_target_st = 0;

    for (uint32_t irq_type = 0; irq_type < INT_ID_MAX; irq_type++) {
        if (arch_int_get_enable_irq(irq_type)){
                int_ena_st |= BIT(irq_type);
		}

		if (arch_int_get_target_state(irq_type)){
			int_target_st |= BIT(irq_type);
		}
    }

	CLI_LOGI("irq enabled state: "BK_U64_FORMAT"\r\n", BK_U64_TO_U32(int_ena_st));
	CLI_LOGI("irq target state: "BK_U64_FORMAT"\r\n", BK_U64_TO_U32(int_target_st));
}

static void cli_interrupt_enable_irq_by_bits(uint64_t irq_bits)
{
	CLI_LOGI("enable irq by bits: "BK_U64_FORMAT"\r\n", BK_U64_TO_U32(irq_bits));
    for (uint32_t irq_type = 0; irq_type < INT_ID_MAX; irq_type++) {
		if (irq_bits & (BIT(irq_type))) {
        	arch_int_enable_irq(irq_type);
		} else {
        	arch_int_disable_irq(irq_type);
		}
    }
}

static void cli_interrupt_set_target_by_bits(uint64_t target_bits)
{
	CLI_LOGI("set target by bits: "BK_U64_FORMAT"\r\n", BK_U64_TO_U32(target_bits));
    for (uint32_t irq_type = 0; irq_type < INT_ID_MAX; irq_type++) {
		if (target_bits & (BIT(irq_type))) {
        	arch_int_set_target_state(irq_type);
		} else {
        	arch_int_clear_target_state(irq_type);
		}
    }
}

static void cli_interrupt_target_cmd(int argc, char **argv)
{
	if (argc < 4) {
		cli_interrupt_help();
		return;
	}

	if (os_strcmp(argv[2], "bits") == 0) {
		if (argc != 5) {
			cli_interrupt_help();
			return;
		}

		uint64_t int_g0 = os_strtoul(argv[3], NULL, 16);
		uint64_t int_g1 = os_strtoul(argv[4], NULL, 16);
		uint64_t target_bits = int_g0 | (int_g1 << 32);
		cli_interrupt_set_target_by_bits(target_bits);
	} else {
		uint32_t irq = os_strtoul(argv[2], NULL, 10);
		uint32_t state = os_strtoul(argv[3], NULL, 10);
		CLI_LOGI("set irq=%d target=%d\r\n", irq, state);
		if (state)
			arch_int_set_target_state(irq);
		else
			arch_int_clear_target_state(irq);
	}
}

static void cli_interrupt_enable_cmd(int argc, char **argv)
{
	if (argc < 4) {
		cli_interrupt_help();
		return;
	}

	if (os_strcmp(argv[2], "bits") == 0) {
		uint64_t int_g0 = os_strtoul(argv[3], NULL, 16);
		uint64_t int_g1 = os_strtoul(argv[4], NULL, 16);
		uint64_t enable_bits = int_g0 | (int_g1 << 32);
		cli_interrupt_enable_irq_by_bits(enable_bits);
	} else {
		uint32_t irq = os_strtoul(argv[2], NULL, 10);
		uint32_t state = os_strtoul(argv[3], NULL, 10);
		CLI_LOGI("enable irq=%d target=%d\r\n", irq, state);
		if (state)
			arch_int_enable_irq(irq);
		else
			arch_int_disable_irq(irq);
	}
}


static void cli_interrupt_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_interrupt_help();
		return;
	}

	if (os_strcmp(argv[1], "state") == 0) {
		cli_dump_int_state();
	} else if (os_strcmp(argv[1], "statis") == 0) {
		//arch_int_dump_statis();//TODO
	} else if (os_strcmp(argv[1], "help") == 0) {
		cli_interrupt_help();
	} else if (os_strcmp(argv[1], "target") == 0) {
		cli_interrupt_target_cmd(argc, argv);
	} else if (os_strcmp(argv[1], "enable") == 0) {
		cli_interrupt_enable_cmd(argc, argv);
	} else {
		cli_interrupt_help();
	}
}

#define INTERRUPT_CMD_CNT (sizeof(s_interrupt_commands) / sizeof(struct cli_command))
static const struct cli_command s_interrupt_commands[] = {
	{"int", "retarget {int_group0} {int_group1}", cli_interrupt_cmd},
};

int cli_interrupt_init(void)
{
	return cli_register_commands(s_interrupt_commands, INTERRUPT_CMD_CNT);
}

