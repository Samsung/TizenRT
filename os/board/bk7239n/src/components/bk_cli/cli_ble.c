#include "bk_cli.h"
#include <common/sys_config.h>
#include "bk_phy.h"

extern void ble_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);//TODO

#define BLE_CMD_CNT (sizeof(s_ble_commands) / sizeof(struct cli_command))

static const struct cli_command s_ble_commands[] = {
#if defined(CONFIG_ARMINO_BLE) && 1//defined(CONFIG_NON_SIGANL_PHY_ENABLE)
	{"ble", "ble arg1 arg2",  ble_command},
#endif

#if (!defined(CONFIG_WIFI_ENABLE))
	{"rfcali_show_data",     "",                      cmd_rfcali_show_data},
#endif
};

int cli_ble_init(void)
{
	return cli_register_commands(s_ble_commands, BLE_CMD_CNT);
}
