#include "cli.h"
#include "bk_saradc.h"
#include "bk_sensor_internal.h"

void cli_volt_detect_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 adc_code = 0;
	float  voltage;

	if (argc == 1) {
		BK_LOG_ON_ERR(volt_single_get_current_voltage(&adc_code));
		voltage = bk_adc_data_calculate((UINT16)adc_code, 0);
		CLI_LOGI("current voltage is %.3f V, raw code is %d\n", voltage, adc_code);
		return;
	}

#if defined(CONFIG_VOLT_DETECT) && defined(CONFIG_TEMP_DETECT)
	if (os_strcmp(argv[1], "start") == 0) {
		BK_LOG_ON_ERR(volt_detect_start());
	} else if (os_strcmp(argv[1], "stop") == 0) {
		BK_LOG_ON_ERR(volt_detect_stop());
	}
#endif
}

#define VOLT_DETECT_CMD_CNT (sizeof(s_volt_detect_commands) / sizeof(struct cli_command))
static const struct cli_command s_volt_detect_commands[] = {
	{"voltd", "voltd [stop|start]", cli_volt_detect_cmd},
};

int cli_volt_detect_init(void)
{
	return cli_register_commands(s_volt_detect_commands, VOLT_DETECT_CMD_CNT);
}
