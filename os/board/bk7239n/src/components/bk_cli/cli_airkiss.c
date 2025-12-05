#include "cli.h"

#if defined(CONFIG_AIRKISS_TEST)
u32 airkiss_process(u8 start);
uint32_t bk_airkiss_process(uint32_t start);
#endif

#if defined(CONFIG_AIRKISS_TEST)
void cli_wifi_airkiss_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	u8 is_fail = 0;
	u8 start = 0;
	char *in_str, *end_ptr;

	if (argc != 2) {
		CLI_LOGI("need 2 parameters: airkiss 1(start), 0(stop)\r\n");
		return;
	}

	in_str = argv[1];
	start = strtol(in_str, &end_ptr, 0);
	if (end_ptr == in_str)
		is_fail = 1;
	else if ('\0' == *end_ptr) {
	} else
		is_fail = 1;

	if (!((0 == start) || (1 == start)))
		is_fail = 1;

	if (is_fail)
		CLI_LOGI("check the cli parameter, thx\r\n");
	else
		airkiss_process(start);
}

void cli_airkiss_v2_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	u8 is_fail = 0;
	u8 start = 0;
	char *in_str, *end_ptr;

	if (argc != 2) {
		CLI_LOGI("need 2 parameters: airkiss_v2 1(start), 0(stop)\r\n");
		return;
	}

	in_str = argv[1];
	start = strtol(in_str, &end_ptr, 0);
	if (end_ptr == in_str)
		is_fail = 1;
	else if ('\0' == *end_ptr) {
	} else
		is_fail = 1;

	if (!((0 == start) || (1 == start)))
		is_fail = 1;

	if (is_fail)
		CLI_LOGI("check the cli parameter, thx\r\n");
	else
		bk_airkiss_process(start);
}
#endif

#define AIRKISS_CMD_CNT (sizeof(s_airkiss_commands) / sizeof(struct cli_command))
static const struct cli_command s_airkiss_commands[] = {
#if defined(CONFIG_AIRKISS_TEST)
	{"airkiss", "airkiss {0|1}", cli_wifi_airkiss_cmd},
	{"airkiss_v2", "ak {0|1}", cli_airkiss_v2_cmd},
#endif
};

int cli_airkiss_init(void)
{
	return cli_register_commands(s_airkiss_commands, AIRKISS_CMD_CNT);
}
