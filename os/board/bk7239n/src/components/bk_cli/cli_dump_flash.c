#include "cli.h"
#include <components/system.h>
#include <os/str.h>
#include "sys_driver.h"
#include "sdkconfig.h"
#include <common/bk_assert.h>

void dump_flash_assert(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGW("dump to fash, memset 0\r\n");
	os_write_word(0, 0);
}

void dump_flash_mem_fault(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGW("dump to fash, assert\r\n");
	BK_ASSERT(0);
}

void dump_flash_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGW("dump_flash_init\r\n");
	bk_init_crash_cnt();
}

void dump_flash_enable(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGW("dump_flash_set_enable\r\n");
	if (argc < 1) {
		CLI_LOGW("please input enable-mode.\r\n");
		return;
	}

	char mode_str = *(argv[1]);
	uint32_t mode = mode_str - '0';
	bk_set_dump_to_flash_enable(mode);
}

void dump_flash_get_number(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int num = bk_get_crash_count();
	CLI_LOGW("curr dump num = %d\r\n", num);
}

void dump_flash_read(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGW("dump_flash_read\r\n");
	if (argc < 1) {
		CLI_LOGW("please input read id.\r\n");
		return;
	}
	char id_str = *(argv[1]);
	int dump_id = id_str - '0';
	uint32_t len = 0;
	int ret;
	CLI_LOGW("dump id = %d\r\n", dump_id);
	ret = bk_get_crash_dump_len(dump_id, &len);
	if (ret != 0 || len == 0) {
		CLI_LOGW("get dump fail\r\n");
		return;
	}
	if (len)
	CLI_LOGW("dump len = %d\r\n", len);
	char *ptr = (char *)malloc(len + 1);
	if (ptr == NULL) {
		CLI_LOGW("malloc error\r\n");
		return;
	}
	memset(ptr, 0, len + 1);
	ret = bk_get_crash_dump_ptr(dump_id, ptr, len + 1);
	if (ret != 0) {
		return;
	}
	uint32_t id = 0;
	char buffer[101] = {0};
	CLI_LOGW("dump id: %d, content:\r\n", dump_id);
	while(id < len) {
		strncpy(buffer, (const char *)&ptr[id], 100);
		bk_printf_raw(0, "%s", buffer);
		id += 100;
	}
	free(ptr);
}

#define DUMP_FLASH_CMD_CNT (sizeof(s_dump_flash_commands) / sizeof(struct cli_command))
static const struct cli_command s_dump_flash_commands[] = {
	{"df_memset", NULL, dump_flash_assert},
	{"df_assert", NULL, dump_flash_mem_fault},
	{"df_num", NULL, dump_flash_get_number},
	{"df_read", NULL, dump_flash_read},
	{"df_init", NULL, dump_flash_init},
	{"df_enable", NULL, dump_flash_enable},
};

int cli_dump2flash_init(void)
{
	return cli_register_commands(s_dump_flash_commands, DUMP_FLASH_CMD_CNT);
}
