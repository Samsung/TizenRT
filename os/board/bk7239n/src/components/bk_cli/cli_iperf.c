#include "cli.h"

#if defined(CONFIG_IPERF_TEST)
extern void iperf(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define IPERF_CMD_CNT (sizeof(s_iperf_commands) / sizeof(struct cli_command))
static const struct cli_command s_iperf_commands[] = {
	{"iperf", "iperf help", iperf },
};

int cli_iperf_init(void)
{
	return cli_register_commands(s_iperf_commands, IPERF_CMD_CNT);
}
#endif
