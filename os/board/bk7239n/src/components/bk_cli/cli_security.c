#include "cli.h"

#if (defined(CONFIG_SOC_BK7251))
#include "bk_security.h"
extern void sec_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

#define SECURITY_CMD_CNT (sizeof(s_security_commands) / sizeof(struct cli_command))
static const struct cli_command s_security_commands[] = {
#if (defined(CONFIG_SOC_BK7251))
	{"sec", "sec help", sec_Command },
#endif
};

int cli_security_init(void)
{
	return cli_register_commands(s_security_commands, SECURITY_CMD_CNT);
}
