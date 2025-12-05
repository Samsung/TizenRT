#include "cli.h"

#include <driver/sd_card.h>
#if defined(CONFIG_SDCARD)
/*
sdtest I 0 --
sdtest R secnum
sdtest W secnum
*/
extern uint32_t sdcard_intf_test(void);
extern uint32_t test_sdcard_read(uint32_t blk, uint32_t blk_cnt);
extern uint32_t test_sdcard_write(uint32_t blk, uint32_t blk_cnt, uint32_t wr_val);
extern void sdcard_intf_close(void);

static void sd_operate(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t cmd;
	uint32_t blknum = 0, blkcnt = 1, wr_val = 0x12345678;
	uint32_t ret;
	if (argc > 1) {
		cmd = argv[1][0];
		if (argc > 2)
		{
			blknum = os_strtoul(argv[2], NULL, 10);
			if (argc > 3)
			{
				blkcnt = os_strtoul(argv[3], NULL, 10);
				if (argc > 4)
				{
					wr_val = os_strtoul(argv[4], NULL, 16);
				}
			}
		}
		switch (cmd) {
		case 'I':
			ret = sdcard_intf_test();
			os_printf("init ret=%x\r\n", ret);
			break;
		case 'R':
			ret = test_sdcard_read(blknum, blkcnt);
			os_printf("read ret=%x,blknum=%d,blkcnt=%d\r\n", ret, blknum, blkcnt);
			break;
		case 'W':
			ret = test_sdcard_write(blknum, blkcnt, wr_val);
			os_printf("write ret=%x,blknum=%d,blkcnt=%d, wr_val=0x%08x\r\n", ret, blknum, blkcnt, wr_val);
			break;
		case 'C':
			sdcard_intf_close();
			os_printf("sdtest close \r\n");
			break;
		case 'S':
			bk_sd_card_set_clock(blknum);
			break;
		default:
			break;
		}
	} else
		os_printf("cmd param error\r\n");
}

#endif


#define SD_CMD_CNT (sizeof(s_sd_commands) / sizeof(struct cli_command))
static const struct cli_command s_sd_commands[] = {
#if defined(CONFIG_SDCARD)
	{"sdtest", "sdtest <cmd>", sd_operate},
#endif
};

int cli_sd_init(void)
{
	return cli_register_commands(s_sd_commands, SD_CMD_CNT);
}
