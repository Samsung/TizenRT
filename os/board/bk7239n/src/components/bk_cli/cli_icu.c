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
#include <driver/int.h>
#include "pwm_hal.h"
#include "gpio_hal.h"
#include "gpio_driver_base.h"
#include "icu_driver.h"
#include "clock_driver.h"
#include "power_driver.h"


typedef struct {
	pwm_chan_t chan1;
	pwm_chan_t chan2;
	bool is_valid;
} pwm_group_info_t;
#define PWM_GROUP_NUM (SOC_PWM_CHAN_NUM_PER_UNIT >> 1)

typedef struct {
	pwm_hal_t hal;
	//Important notes: currently no lock for bits
	//Concurrently operation is NOT allowed!!!
	uint32_t chan_init_bits;
	pwm_group_info_t groups[PWM_GROUP_NUM];
} pwm_driver_t;

#if defined(CONFIG_PWM)
extern pwm_driver_t s_pwm;
#endif
extern gpio_driver_t s_gpio;

static void cli_icu_help(void)
{
	CLI_LOGI("icu {init|deinit|get_int_statis}\n");
	CLI_LOGI("icu_control {power_on_pwm| power_down_pwm|enable_pwm_int|disable_pwm_int|\
		set_pwm_26m|set_pwm_dco|enable_pwm_lpo|disable_pwm_lpo|pwm_isr_register\
		|gpio_isr_register   chan}\n");
}

#if defined(CONFIG_PWM)
static void cli_int_service_pwm_isr(void)
{
#if defined(CONFIG_PWM_V1PX)
	CLI_LOGI("TODO:PWM ASIC IP V1PX\r\n");
#else
	pwm_hal_t *hal = &s_pwm.hal;
	uint32_t int_status;
	int chan;

	int_status = pwm_hal_get_interrupt_status(hal);
	pwm_hal_clear_interrupt_status(hal, int_status);
	for (chan = 0; chan < SOC_PWM_CHAN_NUM_PER_UNIT; chan++) {
		if (pwm_hal_is_interrupt_triggered(hal, chan, int_status)) {
			CLI_LOGI("pwm change register isr test :pwm[%d] isr\r\n", chan);
		}
	}
#endif
}
#endif

static void cli_int_service_gpio_isr(void)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;
	int gpio_id;

	gpio_hal_get_interrupt_status(hal, &gpio_status);

	gpio_hal_clear_interrupt_status(hal, &gpio_status);

	for (gpio_id = 0; gpio_id < SOC_GPIO_NUM; gpio_id++) {
		if (gpio_hal_is_interrupt_triggered(hal, gpio_id, &gpio_status)) {
			CLI_LOGI("gpio change register  isr test :gpio[%d] isr\r\n", gpio_id);
		}
	}

}

static void cli_icu_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (argc < 2) {
        cli_icu_help();
        return;
    }

    if (os_strcmp(argv[1], "init") == 0) {
	    BK_LOG_ON_ERR(bk_icu_driver_init());
	    CLI_LOGI("icu intterupt init\n");
    } else if(os_strcmp(argv[1], "deinit") == 0) {
	    BK_LOG_ON_ERR(bk_icu_driver_deinit());
	    CLI_LOGI("icu interrupt deinit\n");
    }
#if defined(CONFIG_INT_STATIS)
    else if(os_strcmp(argv[1], "get_int_statis") == 0) {
		int_statis_t *int_st = {0};
		int_st = bk_get_int_statis();
		BK_LOG_ON_ERR(bk_dump_int_statis(int_st));
		CLI_LOGI("icu get interrupt statistic\n");
    }
#endif
    else if(os_strcmp(argv[1], "pwm_isr_register") == 0) {
#if defined(CONFIG_PWM)
		bk_int_isr_register(INT_SRC_PWM, cli_int_service_pwm_isr, NULL);
		CLI_LOGI("pwm registert isr change test\n");
#else
		CLI_LOGI("pwm does not supported\n");
#endif
	} else if(os_strcmp(argv[1], "gpio_isr_register") == 0) {
		bk_int_isr_register(INT_SRC_GPIO, cli_int_service_gpio_isr, NULL);
		CLI_LOGI("gpio register isr changing test\n");
    } else if(os_strcmp(argv[1], "set_pwm_int_pri") == 0) {
		uint32_t pri = os_strtoul(argv[2], NULL, 10);
		bk_int_set_priority(INT_SRC_PWM, pri);
		CLI_LOGI("pwm int priority changing test\n");
    }else if(os_strcmp(argv[1], "set_gpio_int_pri") == 0) {
		uint32_t pri = os_strtoul(argv[2], NULL, 10);
		bk_int_set_priority(INT_SRC_GPIO, pri);
		CLI_LOGI("gpio int priority changing test\n");
    }else {
	    cli_icu_help();
	    return;
    }

}

static void cli_icu_control_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t chan;

	if (argc < 2)
	{
		cli_icu_help();
		return;
	}

	chan = os_strtoul(argv[2], NULL, 10);

	if (os_strcmp(argv[1], "power_on_pwm") == 0)
	{
		power_pwm_pwr_up(chan);
		CLI_LOGI("pwr up pwm channel %d\n", chan);
	} else if (os_strcmp(argv[1], "power_down_pwm") == 0) {
		power_pwm_pwr_down(chan);
		CLI_LOGI("pwr down pwm channel %d\n", chan);
	} else if (os_strcmp(argv[1], "enable_pwm_int") == 0) {
		icu_enable_pwm_interrupt();
		icu_enable_pwm2_interrupt();
	} else if (os_strcmp(argv[1], "disable_pwm_int") == 0) {
		icu_disable_pwm_interrupt();
		icu_disable_pwm2_interrupt();
	} else if (os_strcmp(argv[1], "set_pwm_26m") == 0) {
		clk_set_pwms_clk_26m();
	} else if (os_strcmp(argv[1], "set_pwm_dco") == 0) {
		clk_set_pwms_clk_dco();
	} else if (os_strcmp(argv[1], "enable_pwm_lpo") == 0) {
		clk_enable_pwm_clk_lpo(chan);
	} else if (os_strcmp(argv[1], "disable_pwm_lpo") == 0) {
		clk_disable_pwm_clk_lpo(chan);
	}else {
		cli_icu_help();
		return;
	}

}


#define ICU_CMD_CNT (sizeof(s_icu_commands) / sizeof(struct cli_command))
static const struct cli_command s_icu_commands[] = {
	{"icu", "icu {init}", cli_icu_int_cmd},
	{"icu_control", "icu {power_on_pwm|power_down_pwm}", cli_icu_control_cmd},
};

int cli_icu_init(void)
{
    BK_LOG_ON_ERR(bk_icu_driver_init());
    return cli_register_commands(s_icu_commands, ICU_CMD_CNT);
}

