#include "cli.h"
#include "bk_sensor_internal.h"
#include "driver/adc.h"
#include "temp_detect.h"

void cli_temp_detect_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t adc_code = 0;
	float  temperature;

	if (argc == 1) {
		BK_LOG_ON_ERR(temp_detect_get_temperature(&adc_code));
		temperature = bk_adc_data_calculate(adc_code, ADC_TEMP_SENSOR_CHANNEL);
		CLI_LOGI("current temperature is %.1f, raw code is %d\n", temperature, adc_code);
		return;
	}

#if defined(CONFIG_TEMP_DETECT)

	if (os_strcmp(argv[1], "init") == 0) {
		uint32_t init_temperature = 0;

		if (argc == 3) 
			init_temperature = os_strtoul(argv[2], NULL, 10);

		BK_LOG_ON_ERR(temp_detect_init(init_temperature));
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(temp_detect_deinit());
	} else if (os_strcmp(argv[1], "start") == 0) {
		BK_LOG_ON_ERR(temp_detect_start());
	} else if (os_strcmp(argv[1], "stop") == 0) {
		BK_LOG_ON_ERR(temp_detect_stop());
	} else if (os_strcmp(argv[1], "update") == 0) {
		//TODO by Frank!!
	}
#endif
}

#define TEMP_DETECT_CMD_CNT (sizeof(s_temp_detect_commands) / sizeof(struct cli_command))
static const struct cli_command s_temp_detect_commands[] = {
	{"tempd", "tempd [init|deinit|stop|start|update]", cli_temp_detect_cmd},
};

int cli_temp_detect_init(void)
{
	return cli_register_commands(s_temp_detect_commands, TEMP_DETECT_CMD_CNT);
}
