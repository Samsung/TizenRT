#include <stdlib.h>
#include "cli.h"
#include "bk_gpio.h"
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"

static void cli_gpio_help(void)
{
	CLI_LOGI("gpio_driver [init/deinit] \r\n");

	CLI_LOGI("gpio    [set_config/input_pulldown/input_pullup////]    only in set_sonfig:[io_mode]    [pull mode] \r\n");
	CLI_LOGI("gpio    [output/input]    [gpio_pin]    [pullup/pulldown] \r\n");
	CLI_LOGI("gpio    [output_high/output_low/input_get]    [gpio_pin] \r\n");
	CLI_LOGI("gpio_map    [sdio_map/spi_map]     [mode]\r\n");
	CLI_LOGI("gpio_int    [index]    [inttype/start/stop]    [low/high_level/rising/falling edge]\r\n");
#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
	CLI_LOGI("gpio_wake    [index][low/high_level/rising/falling edge][enable/disable wakeup]\r\n");
	CLI_LOGI("gpio_low_power    [simulate][param]\r\n");
	CLI_LOGI("gpio_kpsta [register/unregister][index][io_mode][pull_mode][func_mode]\r\n");
#endif
#if defined(CONFIG_GPIO_SIMULATE_UART_WRITE)
	CLI_LOGI("gpio_uart_write    [index][div(baud_rate=1Mbps/(1+div))][string(len < 8)]\r\n");
#endif
}

static void cli_gpio_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2)
	{
		cli_gpio_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_gpio_driver_init());
		CLI_LOGI("gpio init\n");
	} else if(os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_gpio_driver_deinit());
		CLI_LOGI("gpio deinit\n");
	} else {
		cli_gpio_help();
		return;
	}
}



static void cli_gpio_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_gpio_help();
		return;
	}

	 if (os_strcmp(argv[1], "set_config") == 0) {
		gpio_id_t id;
		id = os_strtoul(argv[2], NULL, 10);

		gpio_config_t mode;

		mode.io_mode = os_strtoul(argv[3], NULL, 10);
		mode.pull_mode = os_strtoul(argv[4], NULL, 10);
		BK_LOG_ON_ERR(bk_gpio_set_config(id, &mode));

		CLI_LOGI("gpio io(output/disable/input): %x ,  pull(disable/down/up) : %x\n", mode.io_mode, mode.pull_mode);

	} else if (os_strcmp(argv[1], "output") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		BK_LOG_ON_ERR(bk_gpio_disable_input(id));
		BK_LOG_ON_ERR(bk_gpio_enable_output(id));

		if (os_strcmp(argv[3], "pull_up") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_up(id));
		}else if (os_strcmp(argv[3], "pull_down") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_down(id));
		} else {
			BK_LOG_ON_ERR(bk_gpio_disable_pull(id));
		}

		CLI_LOGI("gpio output test: %x \n", id);

	}else if (os_strcmp(argv[1], "input") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		BK_LOG_ON_ERR(bk_gpio_disable_output(id));
		BK_LOG_ON_ERR(bk_gpio_enable_input(id));

		if (os_strcmp(argv[3], "pull_up") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_up(id));
		}else if (os_strcmp(argv[3], "pull_down") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_down(id));
		} else {
			BK_LOG_ON_ERR(bk_gpio_disable_pull(id));
		}

		CLI_LOGI("gpio input test: %d \n", id);

	}else if (os_strcmp(argv[1], "output_high") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		// must set gpio mode before gpio output test
		BK_LOG_ON_ERR(bk_gpio_set_output_high(id));

		CLI_LOGI("gpio output hgih\n");
	}else if (os_strcmp(argv[1], "output_low") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		// must set gpio mode before gpio output test
		BK_LOG_ON_ERR(bk_gpio_set_output_low(id));

		CLI_LOGI("gpio output low\n");
	} else if (os_strcmp(argv[1], "input_get") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		// must set gpio mode before gpio output test
		uint8_t input_value = bk_gpio_get_input(id);

		CLI_LOGI("gpio input value is %x\r\n", input_value);
	} else {
		cli_gpio_help();
		return;
	}
}


static void cli_gpio_map_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_gpio_help();
		return;
	}
	 uint8 mode = 0;

	 if (os_strcmp(argv[1], "devs") == 0) {
		gpio_id_t id = 0;

		id = os_strtoul(argv[2], NULL, 10);
		mode = os_strtoul(argv[3], NULL, 10);

		gpio_dev_unmap(id);
		gpio_dev_map(id, mode);
	} else if (os_strcmp(argv[1], "jtag_map") == 0) {
		gpio_id_t id = 0;

		id = os_strtoul(argv[2], NULL, 10);
		gpio_dev_unmap(id);

		if (os_strcmp(argv[3], "tck") == 0) {
			CLI_LOGI("gpio set JTAG_TCK\r\n");
			gpio_dev_map(id, GPIO_DEV_JTAG_TCK);
		} else if(os_strcmp(argv[3], "tms") == 0) {
			CLI_LOGI("gpio set JTAG_TMS\r\n");
			gpio_dev_map(id, GPIO_DEV_JTAG_TMS);
		} else if(os_strcmp(argv[3], "tdi") == 0) {
			CLI_LOGI("gpio set JTAG_TDI\r\n");
			gpio_dev_map(id, GPIO_DEV_JTAG_TDI);
		} else if(os_strcmp(argv[3], "tdo") == 0) {
			CLI_LOGI("gpio set JTAG_TDO\r\n");
			gpio_dev_map(id, GPIO_DEV_JTAG_TDO);
		} else {
			cli_gpio_help();
			return;
		}
	}
#if ((defined(CONFIG_SOC_BK7271)) ||(defined(CONFIG_SOC_BK7251)) )
	 else if (os_strcmp(argv[1], "sdio_map") == 0) {

		mode = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(gpio_sdio_sel(mode));
	 } else if (os_strcmp(argv[1], "spi_map") == 0){
		 mode = os_strtoul(argv[2], NULL, 10);
		 BK_LOG_ON_ERR(gpio_spi_sel(mode));
	 }
#if (defined(CONFIG_SOC_BK7271))
	else if (os_strcmp(argv[1], "uart2_map") == 0) {

		mode = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(gpio_uart2_sel(mode));
	 } else if (os_strcmp(argv[1], "pwms_map") == 0){
		 mode = os_strtoul(argv[2], NULL, 10);
		 uint32 channel = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(gpio_pwms_sel(channel,mode));
	 }

#endif
#endif
	else {
		cli_gpio_help();
		return;
	}
}

#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
static void cli_gpio_set_wake_source_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 4) {
		cli_gpio_help();
		return;
	}

	gpio_id_t id = 0;
	uint32_t mode = 0;

	id = os_strtoul(argv[2], NULL, 10);
	mode = os_strtoul(argv[3], NULL, 10);

	if(os_strcmp(argv[1], "register") == 0)
		bk_gpio_register_wakeup_source(id, mode);
	else if(os_strcmp(argv[1], "unregister") == 0)
		bk_gpio_unregister_wakeup_source(id);
	else if(os_strcmp(argv[1], "get_id") == 0)
		CLI_LOGI("GET wakeup gpio id: %d\r\n", bk_gpio_get_wakeup_gpio_id());
	else
		return cli_gpio_help();
}

static void cli_gpio_set_keep_status_config(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_gpio_help();
		return;
	}

	gpio_id_t gpio_id = 0;
	gpio_config_t config;

	gpio_id = os_strtoul(argv[2], NULL, 10);

	if(argc > 2)
		config.io_mode = os_strtoul(argv[3], NULL, 10);
	else
		config.io_mode = GPIO_IO_DISABLE;

	if(argc > 3)
		config.pull_mode = os_strtoul(argv[4], NULL, 10);
	else
		config.pull_mode = GPIO_PULL_DISABLE;

	if(argc > 4)
		config.func_mode = os_strtoul(argv[5], NULL, 10);
	else
		config.func_mode = GPIO_SECOND_FUNC_DISABLE;


	if(os_strcmp(argv[1], "register") == 0) {
		CLI_LOGI("cli_gpio_set_keep_status_config register gpio_id: %d\r\n", gpio_id);
		bk_gpio_register_lowpower_keep_status(gpio_id, &config);
	} else if(os_strcmp(argv[1], "unregister") == 0) {
		CLI_LOGI("cli_gpio_set_keep_status_config unregister gpio_id: %d\r\n", gpio_id);
		bk_gpio_unregister_lowpower_keep_status(gpio_id);
	} else
		return cli_gpio_help();



}

static void cli_gpio_simulate_low_power_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_gpio_help();
		return;
	}

	uint32_t param = 0;

	param = os_strtoul(argv[2], NULL, 10);

	if(os_strcmp(argv[1], "simulate") == 0)
	{
		uint32_t int_level = 0;

		//if param == 0x534b4950 == "SKIP" == 1,397,442,896
		//means not switch GPIO to low power status just do save and restore
		CLI_LOGD("mode:%d,0x%x\n", param, param);
		int_level = rtos_enter_critical();
		gpio_enter_low_power((void *)param);	
		rtos_exit_critical(int_level);

		//TODO:simulate sytem entry low voltage.

		int_level = rtos_enter_critical();
		gpio_exit_low_power((void *)param);
		rtos_exit_critical(int_level);
		return;
	}
	else
		return cli_gpio_help();
}
#endif


#if defined(CONFIG_GPIO_SIMULATE_UART_WRITE)
static void cli_gpio_simulate_uart_write_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t id = 0, div = 0;
	uint32_t len = 8;
	
	if (argc < 4) {
		cli_gpio_help();
		return;
	}

	id = os_strtoul(argv[1], NULL, 10);
	div = os_strtoul(argv[2], NULL, 10);
	if(len > strlen(argv[3]))
		len = strlen(argv[3]);
	gpio_simulate_uart_write((uint8_t *)argv[3], len, id, div);
}
#endif

static void cli_gpio_int_isr(gpio_id_t id)
{
	CLI_LOGI("gpio isr index:%d\n",id);
	bk_gpio_clear_interrupt(id);
}

static void cli_gpio_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t id;
	gpio_config_t cfg;

	cfg.io_mode =GPIO_INPUT_ENABLE;

	if (argc < 2) {
		cli_gpio_help();
		return;
	}

	id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "intype") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		gpio_int_type_t int_type = 0;
		if (os_strcmp(argv[3], "low_level") == 0) {
			int_type = GPIO_INT_TYPE_LOW_LEVEL;
			cfg.pull_mode = GPIO_PULL_UP_EN;
			bk_gpio_set_config(id, &cfg);
		} else if (os_strcmp(argv[3], "high_level") == 0) {
			int_type = GPIO_INT_TYPE_HIGH_LEVEL;
			cfg.pull_mode = GPIO_PULL_DOWN_EN;
			bk_gpio_set_config(id, &cfg);
		}else if (os_strcmp(argv[3], "rising_edge") == 0) {
			int_type = GPIO_INT_TYPE_RISING_EDGE;
			cfg.pull_mode = GPIO_PULL_DOWN_EN;
			bk_gpio_set_config(id, &cfg);
		} else if (os_strcmp(argv[3], "falling_edge") == 0) {
			int_type = GPIO_INT_TYPE_FALLING_EDGE;
			cfg.pull_mode = GPIO_PULL_UP_EN;
			bk_gpio_set_config(id, &cfg);
		} else {
			cli_gpio_help();
			return;
		}
		bk_gpio_register_isr(id ,cli_gpio_int_isr);

		BK_LOG_ON_ERR(bk_gpio_set_interrupt_type(id, int_type));
		CLI_LOGI("gpio[%d] set int type:%x\n", id, int_type);
	} else if (os_strcmp(argv[2], "start") == 0) {
		BK_LOG_ON_ERR(bk_gpio_enable_interrupt(id));
	} else if (os_strcmp(argv[2], "stop") == 0) {
		BK_LOG_ON_ERR(bk_gpio_disable_interrupt(id));
		CLI_LOGI("gpio[%d] int  stop \r\n", id);

	} else {
		cli_gpio_help();
		return;
	}
}

#define GPIO_CMD_CNT (sizeof(s_gpio_commands) / sizeof(struct cli_command))
static const struct cli_command s_gpio_commands[] = {
	{"gpio_int", "gpio_int    [index]     [inttype/start/stop]     [low/high_level/rising/falling edge]", cli_gpio_int_cmd},
	{"gpio", "gpio     [set_mode/output_low/output_high/input/spi_mode]      [id]     [mode]", cli_gpio_cmd},
	{"gpio_driver", "gpio_driver    [init/deinit]}", cli_gpio_driver_cmd},
	{"gpio_map", "gpio_map     [sdio_map/spi_map]",cli_gpio_map_cmd},
#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
	{"gpio_wake", "gpio_wake [index][low/high_level/rising/falling edge][enable/disable wakeup]", cli_gpio_set_wake_source_cmd},
	{"gpio_kpsta", "gpio_kpsta [register/unregister][index][io_mode][pull_mode][func_mode]", cli_gpio_set_keep_status_config},
	{"gpio_low_power", "gpio_low_power [simulate][param]", cli_gpio_simulate_low_power_cmd},
#endif
#if defined(CONFIG_GPIO_SIMULATE_UART_WRITE)
	{"gpio_uart_write", "[index][div(baud_rate=1Mbps/(1+div))][string]", cli_gpio_simulate_uart_write_cmd}
#endif

};

int cli_gpio_init(void)
{
	BK_LOG_ON_ERR(bk_gpio_driver_init());
	return cli_register_commands(s_gpio_commands, GPIO_CMD_CNT);
}
