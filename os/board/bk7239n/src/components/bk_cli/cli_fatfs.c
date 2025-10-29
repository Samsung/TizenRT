#include "cli.h"

#if defined(CONFIG_FATFS)

#include "test_fatfs.h"

static void fatfs_operate(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t cmd;
	char *disk_name[DISK_NUMBER_COUNT] = {"ram", "sdio_sd", "udisk", "flash"};
	DISK_NUMBER drv_num = DISK_NUMBER_SDIO_SD;
	char file_name[64] = {'d', 'e', 'f', 'a', 'u', 'l', 't', 'f', 'i', 'l', 'e', 'n', 'a', 'm', 'e', '.', 't', 'x', 't', 0};
	char write_content[64];
	uint64_t content_len = 0;
	uint32_t test_cnt = 0;

	if (argc >= 3) {
		cmd = argv[1][0];
		drv_num = os_strtoul(argv[2], NULL, 10);
		if(argc >= 4)
		{
			snprintf(&file_name[0], sizeof(file_name) - 1, argv[3]);
			file_name[sizeof(file_name)-1] = 0;

			if(argc >= 5)
			{
				snprintf(&write_content[0], sizeof(write_content) - 1, argv[4]);
				write_content[sizeof(write_content)-1] = 0;

				if(argc >= 6)
				{
					content_len = os_strtoul(argv[5], NULL, 10);
					content_len = content_len > (sizeof(write_content) - 1) ? (sizeof(write_content) - 1) : content_len;
					content_len = content_len > strlen(write_content)? strlen(write_content) : content_len;

					//re-use in autotest
					test_cnt = os_strtoul(argv[5], NULL, 10);
				}
			}
		}
		else
			CLI_LOGD("error file name,use defaultfilename.txt\r\n");

		switch (cmd) {
		case 'M':
			test_mount(drv_num);
			CLI_LOGD("mount:%s\r\n", disk_name[drv_num%DISK_NUMBER_COUNT]);
			break;
		case 'U':
			test_unmount(drv_num);
			CLI_LOGD("unmount:%s\r\n", disk_name[drv_num%DISK_NUMBER_COUNT]);
			break;
		case 'G':
			test_getfree(drv_num);
			CLI_LOGD("getfree:%s\r\n", disk_name[drv_num%DISK_NUMBER_COUNT]);
			break;
		case 'R':
			if (argc >= 5)
				content_len = os_strtoul(argv[4], NULL, 10);
			else
				content_len = 0x0fffffffffffffff;	//read finish
			test_fatfs_read(drv_num, file_name, content_len);
			CLI_LOGD("read %s, len_h = %d, len_l = %d\r\n", file_name, (uint32_t)(content_len>>32), (uint32_t)content_len);
			break;
		case 'W':
			test_fatfs_append_write(drv_num, file_name, write_content, content_len);
			CLI_LOGD("append and write:%s,%s\r\n", file_name, write_content);
			break;
		//fatfstest D dev-num file-name start-addr dump-len
		case 'D':
		{
			uint32_t start_addr = 0;
			if(argc >= 5)
			{
				start_addr = os_strtoul(argv[4], NULL, 10);
			}
			if(argc >= 6)
			{
				content_len = os_strtoul(argv[5], NULL, 10);
			}
			test_fatfs_dump(drv_num, file_name, start_addr, content_len);
			break;
		}

		//fatfstest A dev-num file-name write-len test_cnt start-addr
		//fatfstest A 1 autotest.txt 12487 3
		case 'A':
		{
			if(argc < 4)
			{
				CLI_LOGD("Autotest Fatfs argc < 4! Please input correctly\r\n");
				return;
			}
			uint32_t start_addr = 0;
			if(argc >= 5)
			{
				content_len = os_strtoul(argv[4], NULL, 10);
			}
			if(argc >= 7)
			{
				start_addr = os_strtoul(argv[6], NULL, 16);
			}
			test_fatfs_auto_test(drv_num, file_name, content_len, test_cnt, start_addr);
			break;
		}
		case 'F':
			test_fatfs_format(drv_num);
			CLI_LOGD("format :%d\r\n", drv_num);
			break;
		case 'S':
			scan_file_system(drv_num);
			CLI_LOGD("scan \r\n");
			break;
		default:
			break;
		}
	} else
		CLI_LOGD("cmd param error\r\n");
}

static beken_thread_t idle_fatfs_out_test_handle = NULL;
static bool s_fatfs_idle_test_stop_flag = 1;

static void cli_fatfs_idle_out_test(void)
{
	char *file_name = "1.txt";

	test_mount(1);
	rtos_delay_milliseconds(10);
	test_fatfs_format(1);

	while (1) {
		rtos_delay_milliseconds(50);
		if(s_fatfs_idle_test_stop_flag)
			break;
		else
			test_fatfs_auto_test(1, file_name, 12487, 1, 0);
	}

	rtos_delete_thread(&idle_fatfs_out_test_handle);

}

static void fatfs_idle_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		if (!idle_fatfs_out_test_handle && s_fatfs_idle_test_stop_flag){
			CLI_LOGI("fatfs_idle_test START!\n");
			s_fatfs_idle_test_stop_flag = 0;
			if(rtos_create_thread(&idle_fatfs_out_test_handle, 8, "idle_fatfs_out",
				(beken_thread_function_t) cli_fatfs_idle_out_test, 2048, 0)) {
					CLI_LOGI("fatfs_idle_test rtos_create_thread FAILED!\n");
					return;
			}
		} else
			CLI_LOGI("fatfs idle test WORKING!\n");

	} else if (os_strcmp(argv[1], "stop") == 0) {
		if (idle_fatfs_out_test_handle && !s_fatfs_idle_test_stop_flag) {
			s_fatfs_idle_test_stop_flag = 1;
			CLI_LOGI("fatfs_idle_test STOP! Please clean FLAG!\n");
		} else
			CLI_LOGI("PLEASE start fatfs idle test!\n");

	} else if (os_strcmp(argv[1], "clean") == 0) {
		if (idle_fatfs_out_test_handle && s_fatfs_idle_test_stop_flag)
			idle_fatfs_out_test_handle = NULL;
		else
			CLI_LOGI("PLEASE start->stop->clean, check thread status!\n");
	}
}


#define FATFS_CMD_CNT (sizeof(s_fatfs_commands) / sizeof(struct cli_command))
static const struct cli_command s_fatfs_commands[] = {
	{"fatfstest", "fatfstest <cmd>", fatfs_operate},
	{"fatfs_idle_test", "fatfs_idle_test {start|stop|clean}", fatfs_idle_test},
};

int cli_fatfs_init(void)
{
	return cli_register_commands(s_fatfs_commands, FATFS_CMD_CNT);
}

#endif
