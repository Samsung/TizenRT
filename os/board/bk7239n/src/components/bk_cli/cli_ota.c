#include "cli.h"
#include "modules/ota.h"
#if defined(CONFIG_SECURITY_OTA)
#include "_ota.h"
#endif
#include "modules/ota_security.h"
#define DEBUG_TEST_CODE          (1)

#if defined(CONFIG_OTA_TFTP)
extern void tftp_start(void);
extern void string_to_ip(char *s);
static void tftp_ota_get_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	short len = 0, i;
	extern char     BootFile[] ;

	if (argc > 3) {
		CLI_LOGI("ota server_ip ota_file\r\n");
		return;
	}

	CLI_LOGI("%s\r\n", argv[1]);

	os_strcpy(BootFile, argv[2]);
	CLI_LOGI("%s\r\n", BootFile);
	string_to_ip(argv[1]);


	tftp_start();

	return;

}
#endif

#if defined(CONFIG_HTTP_AB_PARTITION)
void get_http_ab_version(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	exec_flag ret_partition = 0;
#if defined(CONFIG_OTA_POSITION_INDEPENDENT_AB)
	ret_partition = bk_ota_get_current_partition();
	if(ret_partition == 0x0)
	{
    	CLI_LOGI("partition A\r\n");
    }
	else
	{
    	CLI_LOGI("partition B\r\n");
    }
#else
	ret_partition = bk_ota_get_current_partition();
	if((ret_partition == 0xFF) ||(ret_partition == EXEX_A_PART))
	{
    	CLI_LOGI("partition A\r\n");
    }
	else
	{
    	CLI_LOGI("partition B\r\n");
    }
#endif
}
#endif

#if defined(CONFIG_DIRECT_XIP) && defined(CONFIG_SECURITY_OTA)
void get_http_ab_version(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	extern uint32_t flash_get_excute_enable();
	uint32_t id = flash_get_excute_enable();
	if(id == 0){
		CLI_LOGW("partition A\r\n");
	} else if (id == 1){
		CLI_LOGW("partition B\r\n");
	}
}

void cli_get_bl2_current_exec_partition(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8 val = ota_bootloader_get_boot_flag_value();
	CLI_LOGW("val :%d \r\n",val);
	if(val== OTA_BOOTLOADER_BOOT_FLAG_PRIMARY){
		CLI_LOGW("bl2 partA\r\n");
	} else if (val == OTA_BOOTLOADER_BOOT_FLAG_SECONDARY){
		CLI_LOGW("bl2 partB\r\n");
	}else{
		CLI_LOGW("default bl2 partA\r\n");
	}
}

void cli_swap_bl2_exec_partition(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t curr_fw_idx = 0;
    uint32_t target_fw_idx = 0;

    curr_fw_idx = bk_ota_security_get_curr_bl2_fw_idx();
    if(curr_fw_idx == 1)   //curr on boot_A
    {
        target_fw_idx = 2;
    }
    else //curr on boot_B
    {
        target_fw_idx = 1;
    }

	int ret = bk_ota_security_swap_bl2_fw(target_fw_idx);
	if(ret != 0){
		CLI_LOGW("swap bl2 fail!!!ret :%d\r\n",ret);
	}else{
		CLI_LOGW("swap bl2 success and rebooting \r\n");
		bk_reboot();
	}
}

void cli_swap_bl2_with_input_param(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	uint32_t target_fw_idx    = 0;

	target_fw_idx      = os_strtoul(argv[1], NULL, 16);
	ret = bk_ota_security_swap_bl2_fw(target_fw_idx);
	if(ret !=  0){
		CLI_LOGW("swap bl2 fail!!!ret :%d\r\n",ret);
	}else{
		CLI_LOGW("swap bl2 success and rebooting \r\n");
		bk_reboot();
	}
}

void cli_set_bl2_version_number (char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int ret = -1;
    char input_version[ALLOCATED_VERSION_LEN] = {0};
	uint32_t target_fw_idx    = 0;

    target_fw_idx  = os_strtoul(argv[1], NULL, 16);
    strcpy(input_version, argv[2]);
 	CLI_LOGW("target_fw_idx :%d,input_version :%s\r\n",target_fw_idx, input_version);
#if DEBUG_TEST_CODE
    extern int bk_ota_security_set_target_bl2_ver(uint8_t target_bl2_idx, uint8_t* input_ver);
    ret = bk_ota_security_set_target_bl2_ver(target_fw_idx, (uint8_t*)input_version);
#endif

    if(ret !=0 )
    {
	    CLI_LOGW("set bl2 ver_number fail!!!\r\n");
    }
    else
    {
        CLI_LOGW("set bl2 ver_number success \r\n");
        bk_reboot();
    }
}

void cli_get_bl2_version_number(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char* p_curr_version = NULL;
	uint32_t target_fw_idx    = 0;

	target_fw_idx      = os_strtoul(argv[1], NULL, 16);
	p_curr_version =  bk_ota_security_get_current_bl2_ver(target_fw_idx);

    if(p_curr_version == NULL )
    {
	    CLI_LOGW("get bl2 ver_number fail!!!\r\n");
    }
    CLI_LOGW("p_curr_version :%s \r\n",p_curr_version);
}

void cli_ota_get_xip_status(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t read_write       = 0;
	uint32_t area_id          = 0;
	uint32_t operate_type     = 0;
	uint32_t return_value     = 0;
	uint32_t write_value      = 0;

	read_write      = os_strtoul(argv[1], NULL, 16);
	area_id         = os_strtoul(argv[2], NULL, 16);
	operate_type    = os_strtoul(argv[3], NULL, 16);
	write_value     = os_strtoul(argv[4], NULL, 16);

	extern uint32_t flash_get_excute_enable();
	uint32_t id = flash_get_excute_enable();
	if(id == 0){
		CLI_LOGW("Current area is A\r\n");
	} else if (id == 1){
		CLI_LOGW("Current area is B\r\n");
	}

	if(read_write == 0)//read
	{
		return_value = bk_ota_security_read_xip_status(area_id, operate_type);
	}
	else if(read_write == 1)//write
	{
		bk_ota_security_write_xip_status(area_id, operate_type, write_value);
	}
	else
	{

	}
	CLI_LOGW("xip status[read_write:%d][area_id:%d][operate_type:%d][write_value:0x%x][return_value:0x%x]\r\n",read_write,area_id,operate_type,write_value,return_value);

}
void cli_swap_app(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	uint32_t switch_fw_id    = 0;
	switch_fw_id      = os_strtoul(argv[1], NULL, 16);
	ret = bk_ota_security_excute_fw_switch(switch_fw_id);
	if(ret == 0)
	{
		if(switch_fw_id == 0x1)
		{
			CLI_LOGW("App swap area A ok[ret:%d].\r\n",ret);
		}
		else if(switch_fw_id == 0x2)
		{
			CLI_LOGW("App swap area B ok[ret:%d].\r\n",ret);
		}
	}
	else
	{
		if(switch_fw_id == 0x1)
		{
			CLI_LOGW("App swap area A fail[ret:%d].\r\n",ret);
		}
		else if(switch_fw_id == 0x2)
		{
			CLI_LOGW("App swap area B fail[ret:%d].\r\n",ret);
		}
	}
}
#endif

#if defined(CONFIG_OTA_HTTP)
static void http_ota_thread(char *arg)
{
	int ret;
	ret = bk_http_ota_download(arg);
	
	if (0 != ret)
		CLI_LOGI("http_ota download failed.");

    free(arg);

	rtos_delete_thread(NULL);
}

void http_ota_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int ret = 0;
    if (argc != 2) {
        CLI_LOGI("Usage:http_ota [url:]\r\n");
        return;
    }

    char *url = (char *)os_malloc(strlen(argv[1]) + 1);
    if (url == NULL) {
        CLI_LOGI("%s,Memory allocation failed\r\n",__func__);
        return;
    }

    CLI_LOGI("Thread creation failed\r\n");
    strcpy(url, argv[1]);

	ret = rtos_create_thread(NULL, 4, "http_ota",
						(beken_thread_function_t)http_ota_thread,
						4*1024,
						(void*)url);

    if (ret != 0) {
        free(url);
        CLI_LOGI("Thread creation failed\r\n");
    }

}
#endif

extern uint32_t  move_bootloader_image(void);

void http_move_bootloader_image(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;

	os_printf("%s enter!!! \r\n",__FUNCTION__);
	ret = move_bootloader_image();

	if (0 != ret)
		os_printf("%s failed.",__FUNCTION__);


    
	return;
}


#if defined(CONFIG_OTA_HTTPS)
char *https_url = NULL;
int bk_https_ota_download(const char *url);
void bk_https_start_download(beken_thread_arg_t arg) {
	int ret;
	ret = bk_https_ota_download(https_url);
	if(ret != BK_OK) {
		CLI_LOGI("%s download fail, ret:%d\r\n", __func__, ret);
	}
	rtos_delete_thread(NULL);
}

void https_ota_start(void)
{
	UINT32 ret;

	CLI_LOGI("https_ota_start\r\n");
	ret = rtos_create_thread(NULL, BEKEN_APPLICATION_PRIORITY,
							 "https_ota",
							 (beken_thread_function_t)bk_https_start_download,
							 5120,
							 0);

	if (kNoErr != ret)
		CLI_LOGI("https_ota_start failed\r\n");

}

void https_ota_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

	if (argc != 2)
		goto HTTP_CMD_ERR;

	https_url = argv[1];
	https_ota_start();
	return;

HTTP_CMD_ERR:
	CLI_LOGI("%s,Usage:http_ota [url:]\r\n",__func__);
}
#endif

#define OTA_CMD_CNT (sizeof(s_ota_commands) / sizeof(struct cli_command))
static const struct cli_command s_ota_commands[] = {

#if defined(CONFIG_OTA_TFTP)
	{"tftpota", "tftpota [ip] [file]", tftp_ota_get_Command},
#endif
#if  defined(CONFIG_DEBUG_FIRMWARE)
#if defined(CONFIG_OTA_HTTP)
	{"http_ota", "http_ota url", http_ota_Command},
#endif
#if defined(CONFIG_OTA_HTTPS)
	{"https_ota", "ip [sta|ap][{ip}{mask}{gate}{dns}]", https_ota_Command},
#endif
#if defined(CONFIG_HTTP_AB_PARTITION)
	{"ab_version", NULL, get_http_ab_version},
#endif
#if defined(CONFIG_DIRECT_XIP) && defined(CONFIG_SECURITY_OTA)
	{"bl2_swap_test", "input parameter", cli_swap_bl2_with_input_param},
	{"xip_status", NULL, cli_ota_get_xip_status},
	{"set_bl2_ver_number","set_bl2_ver_number [target-idx] [input version]", cli_set_bl2_version_number},
	{"get_bl2_ver_number","get_bl2_ver_number [target-idx]", cli_get_bl2_version_number},
#endif
#endif
#if defined(CONFIG_DIRECT_XIP) && defined(CONFIG_SECURITY_OTA)
	{"ab_version", NULL, get_http_ab_version},
	{"bl2_version", NULL, cli_get_bl2_current_exec_partition},
	{"bl2_swap", NULL, cli_swap_bl2_exec_partition},
	{"app_swap", NULL, cli_swap_app},
#endif
#if defined(CONFIG_INJECT_KEY)
    {"insert_key_start", "NULL", http_move_bootloader_image},
#endif
};

#if (defined(CONFIG_TFM_FWU))
extern int32_t ns_interface_lock_init(void);
#endif
int cli_ota_init(void)
{
#if (defined(CONFIG_TFM_FWU))
	ns_interface_lock_init();
#endif
	return cli_register_commands(s_ota_commands, OTA_CMD_CNT);
}
