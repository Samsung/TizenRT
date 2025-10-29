#include <stdlib.h>
#include "cli.h"

#if defined(CONFIG_BUTTON)
#include "key_main.h"
static void gpio_key_short_press_cb()
{
	CLI_LOGI("SHORTDemo: GPIO Key Short Press\r\n");
}
static void gpio_key_double_press_cb()
{
	CLI_LOGI("DOUBLE Demo: GPIO Key Double Press\r\n");
}
static void gpio_key_long_press_cb()
{
	CLI_LOGI("LONG Demo: GPIO Key Long Press\r\n");
}
static void gpio_key_hold_press_cb()
{
	CLI_LOGI("HOLD Demo: GPIO Key Hold Press\r\n");
}
static void cli_gpio_key_demo_test(char * pcWriteBuffer, int xWriteBufferLen, int argc, char * *argv)
{

	if (os_strcmp(argv[1], "init") == 0) {
		key_initialization();
		CLI_LOGI("gpio_key Init\n");
	} else if(os_strcmp(argv[1], "deinit") == 0) {
		key_uninitialization();
		CLI_LOGI("gpio_key Deinit\n");
	} else if(os_strcmp(argv[1], "configure") == 0) {
		uint32_t gpio_id = SOC_GPIO_NUM;
		uint32_t ret = 0;
		uint8_t avtive_level = LOW_LEVEL_TRIGGER;

		if (argc >= 2)
			gpio_id = os_strtoul(argv[2], NULL, 10);

		if (argc >= 3) {
			if(os_strcmp(argv[3], "low_trigger") == 0) {
				avtive_level = LOW_LEVEL_TRIGGER;
			} else if(os_strcmp(argv[3], "high_trigger") == 0) {
				avtive_level = HIGH_LEVEL_TRIGGER;
			} else {
				CLI_LOGI("gpio_key Please fill in the correct information\n");
			}
		}
		ret = key_item_configure(gpio_id,avtive_level,gpio_key_short_press_cb,gpio_key_double_press_cb,gpio_key_long_press_cb,gpio_key_hold_press_cb);
		CLI_LOGI("gpio_key Configure gpio:%d ret:%d\n", gpio_id, ret);
	} else if(os_strcmp(argv[1], "unconfigure") == 0) {
		uint32_t gpio_id = SOC_GPIO_NUM;

		if (argc >= 2)
			gpio_id = os_strtoul(argv[2], NULL, 10);
		
		key_item_unconfigure(gpio_id);
		CLI_LOGI("gpio_key UNconfigure gpio:%d\n", gpio_id);
	}else {
		return;
	}
}
#endif

#if defined(CONFIG_ADC_KEY)
#include "adc_key_main.h"
static void adc_key_short_press_cb()
{
	CLI_LOGI("SHORTDemo: ADC Key Short Press\r\n");
}
static void adc_key_double_press_cb()
{
	CLI_LOGI("DOUBLE Demo: ADC Key Double Press\r\n");
}
static void adc_key_long_press_cb()
{
	CLI_LOGI("LONG Demo: ADC Key Long Press\r\n");
}
static void adc_key_hold_press_cb()
{
	CLI_LOGI("HOLD Demo: ADC Key Hold Press\r\n");
}


static void cli_adc_key_op(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		CLI_LOGI("cli_adc_key_op please init/deinit");
		return;
	}

	ADCKEY_INDEX index = ADCKEY_NULL;
	uint32_t gpio_id = 0;
	uint32_t adc_id = 0;

	if (os_strcmp(argv[1], "init") == 0) {
		if (argc > 2)
			gpio_id = os_strtoul(argv[3], NULL, 10);
		else
			gpio_id = GPIO_28;

		if (argc > 3)
			adc_id = os_strtoul(argv[4], NULL, 10);
		else
			adc_id = 4;

		bk_adc_key_init(gpio_id, adc_id);
		CLI_LOGI("adc_key init\n");
	} else if(os_strcmp(argv[1], "deinit") == 0) {
		bk_adc_key_deinit();
		CLI_LOGI("adc_key deinit\n");
	} else if(os_strcmp(argv[1], "configure") == 0) {
		if (argc < 4) {
			CLI_LOGI("Configure More parameters required");
			return;
		}
		adckey_configure_t config;

		if(os_strcmp(argv[2], "PEV") == 0) {
			config.user_index = ADCKEY_PEV;
		} else if(os_strcmp(argv[2], "NEXT") == 0) {
			config.user_index = ADCKEY_NEXT;
		} else if(os_strcmp(argv[2], "MENU") == 0) {
			config.user_index = ADCKEY_MENU;
		} else if(os_strcmp(argv[2], "PLAY_PAUSE") == 0) {
			config.user_index = ADCKEY_PLAY_PAUSE;
		}else {
			CLI_LOGI("adc_key Configure fail STRING\n");
			return;
		}

		config.lowest_level = os_strtoul(argv[3], NULL, 10);
		config.highest_level = os_strtoul(argv[4], NULL, 10);

		config.short_press_cb =(void *)adc_key_short_press_cb;
		config.double_press_cb = (void *)adc_key_double_press_cb;
		config.long_press_cb = (void *)adc_key_long_press_cb;
		config.hold_press_cb = (void *)adc_key_hold_press_cb;

		bk_adckey_item_configure(&config);
		CLI_LOGI("adc_key Configure\n");
	} else if(os_strcmp(argv[1], "unconfigure") == 0) {
		if(os_strcmp(argv[2], "PEV") == 0) {
			index = ADCKEY_PEV;
		} else if(os_strcmp(argv[2], "NEXT") == 0) {
			index = ADCKEY_NEXT;
		} else if(os_strcmp(argv[2], "MENU") == 0) {
			index = ADCKEY_MENU;
		} else if(os_strcmp(argv[2], "PLAY_PAUSE") == 0) {
			index = ADCKEY_PLAY_PAUSE;
		}else
			return;
		bk_adckey_item_unconfigure(index);
		CLI_LOGI("adc_key Unconfigure\n");
	} else {
		return;
	}
}
#endif

#define KD_CMD_CNT              (sizeof(s_kd_commands) / sizeof(struct cli_command))
static const struct cli_command s_kd_commands[] =
{
#if defined(CONFIG_BUTTON)
    {"gpio_key", "gpio_key init/deinit/configure/unconfigure", cli_gpio_key_demo_test},
#endif

#if defined(CONFIG_ADC_KEY)
    {"adc_key", "adc_key init/deinit/configure/unconfigure", cli_adc_key_op},
#endif
};

int cli_key_demo_init(void)
{
    return cli_register_commands(s_kd_commands, KD_CMD_CNT);
}


