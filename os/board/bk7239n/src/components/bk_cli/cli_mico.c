#include "cli.h"

static int mico_debug_enabled;
static void micodebug_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc == 1) {
		os_printf("Usage: micodebug on/off. _BK_ debug is currently %s\r\n",
				  mico_debug_enabled ? "Enabled" : "Disabled");
		return;
	}

	if (!os_strcasecmp(argv[1], "on")) {
		os_printf("Enable _BK_ debug\r\n");
		mico_debug_enabled = 1;
	} else if (!os_strcasecmp(argv[1], "off")) {
		os_printf("Disable _BK_ debug\r\n");
		mico_debug_enabled = 0;
	}
}


#define MICO_CMD_CNT (sizeof(s_mico_commands) / sizeof(struct cli_command))
static const struct cli_command s_mico_commands[] = {
	{"micodebug", "micodebug on/off", micodebug_Command},
};

int cli_mico_init(void)
{
	return cli_register_commands(s_mico_commands, MICO_CMD_CNT);
}
