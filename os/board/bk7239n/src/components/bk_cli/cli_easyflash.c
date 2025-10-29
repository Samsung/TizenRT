#include <os/os.h>
#include "cli.h"
#include "bk_ef.h"

#if defined(CONFIG_EASY_FLASH)

#define EF_KEY1                  "key1"
#define EF_KEY2                  "key2"
#define EF_KEY3                  "key3"
#define EF_MAX_VALUE_SIZE        256

typedef struct{
    int m_a;
    unsigned short m_b;
    char m_c[32];
}test_data_t;

int g_ef_value1 = 0;
char g_ef_value2[EF_MAX_VALUE_SIZE] = {0};
test_data_t g_ef_value3;

static void cli_easyflash_help(void)
{
    CLI_LOGI("easyflashtest read\r\n");
    CLI_LOGI("easyflashtest write\r\n");
    CLI_LOGI("easyflashtest del [key]\r\n");
}

static void cli_easyflash_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int ret = 0;
    int tmp_value = 0;
    char tmp_string[EF_MAX_VALUE_SIZE];
    static int data_c = 'A';
    test_data_t ef_value3;
    
    if (argc < 2) {
        cli_easyflash_help();
        return;
    }
    
#if defined(CONFIG_EASY_FLASH_V4)
    if (os_strcmp(argv[1], "read") == 0) {
        ret = bk_get_env_enhance(EF_KEY1, &g_ef_value1, sizeof(g_ef_value1));
        if(ret > 0){
            CLI_LOGI("key1 value:%d\r\n", g_ef_value1);
        }
        
        ret = bk_get_env_enhance(EF_KEY2, (void *)&g_ef_value2, sizeof(g_ef_value2));
        if(ret > 0){
            CLI_LOGI("key2 value:%d, %s\r\n", strlen(g_ef_value2), g_ef_value2);
        }

        ret = bk_get_env_enhance(EF_KEY3, (void *)&g_ef_value3, sizeof(g_ef_value3));
        if(ret > 0){
            CLI_LOGI("key3 value:%d, %d, %s\r\n", g_ef_value3.m_a, g_ef_value3.m_b, g_ef_value3.m_c);
        }
    } 
    else if (os_strcmp(argv[1], "write") == 0) {
        tmp_value = g_ef_value1 + 10;
        ret = bk_set_env_enhance(EF_KEY1, (const void *)&tmp_value, sizeof(tmp_value));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key1 set to value:%d\r\n", tmp_value);
        }
        else{
            CLI_LOGI("key1 set fail:%d\r\n", ret);
        }

        memset(tmp_string, 0, sizeof(tmp_string));
        if(tmp_value > EF_MAX_VALUE_SIZE-1)
            memset(tmp_string, data_c++, EF_MAX_VALUE_SIZE-1);
        else
            memset(tmp_string, data_c++, tmp_value);
        ret = bk_set_env_enhance(EF_KEY2, (const void *)tmp_string, sizeof(tmp_string));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key2 value:%d, %s\r\n", strlen(tmp_string), tmp_string);
        }
        else{
            CLI_LOGI("key2 set fail:%d\r\n", ret);
        }

        ef_value3.m_a = tmp_value + 1;
        ef_value3.m_b = tmp_value + 2;
        memset(ef_value3.m_c, 0, sizeof(ef_value3.m_c));
        memset(ef_value3.m_c, data_c, sizeof(ef_value3.m_c)-1);
        ret = bk_set_env_enhance(EF_KEY3, (const void *)&ef_value3, sizeof(ef_value3));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key3 set value:%d, %d, %s\r\n", ef_value3.m_a, ef_value3.m_b, ef_value3.m_c);
        }
        else{
            CLI_LOGI("key3 set fail:%d\r\n", ret);
        }
    }
    else if (os_strcmp(argv[1], "del") == 0) {
        if((argc == 2)
            || (argc > 2 && 0 == os_memcmp(argv[2], EF_KEY1, strlen(EF_KEY1)))){
            if(EF_NO_ERR == bk_set_env_enhance(EF_KEY1, NULL, 0))
                CLI_LOGI("key1 del success\r\n");
            else
                CLI_LOGI("key1 del fail\r\n");
        }
        
        if((argc == 2)
            || (argc > 2 && 0 == os_memcmp(argv[2], EF_KEY2, strlen(EF_KEY2)))){
            if(EF_NO_ERR == bk_set_env_enhance(EF_KEY2, NULL, 0))
                CLI_LOGI("key2 del success\r\n");
            else
                CLI_LOGI("key2 del fail\r\n");
        }

        if((argc == 2)
            || (argc > 2 && 0 == os_memcmp(argv[2], EF_KEY3, strlen(EF_KEY3)))){
            if(EF_NO_ERR == bk_set_env_enhance(EF_KEY3, NULL, 0))
                CLI_LOGI("key3 del success\r\n");
            else
                CLI_LOGI("key3 del fail\r\n");
        }
    }
#elif defined(CONFIG_EASY_FLASH_V3)
    if (os_strcmp(argv[1], "read") == 0) {
        ret = bk_get_buf_env(EF_KEY1, (const char *)&g_ef_value1, sizeof(g_ef_value1));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key1 value:%d\r\n", g_ef_value1);
        }
        
        ret = bk_get_buf_env(EF_KEY2, (const char *)&g_ef_value2, sizeof(g_ef_value2));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key2 value:%s\r\n", g_ef_value2);
        }

        ret = bk_get_buf_env(EF_KEY3, (const char *)&g_ef_value3, sizeof(g_ef_value3));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key3 value:%d, %d, %s\r\n", g_ef_value3.m_a, g_ef_value3.m_b, g_ef_value3.m_c);
        }
    } 
    else if (os_strcmp(argv[1], "write") == 0) {
        tmp_value = g_ef_value1 + 10;
        ret = bk_set_buf_env(EF_KEY1, (const char *)&tmp_value, sizeof(tmp_value));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key1 set to value:%d\r\n", tmp_value);
        }
        else{
            CLI_LOGI("key1 set fail:%d\r\n", ret);
        }

        memset(tmp_string, 0, sizeof(tmp_string));
        if(tmp_value > EF_MAX_VALUE_SIZE-1)
            memset(tmp_string, data_c++, EF_MAX_VALUE_SIZE-1);
        else
            memset(tmp_string, data_c++, tmp_value);
        ret = bk_set_buf_env(EF_KEY2, tmp_string, sizeof(tmp_string));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key2 set to value:%s\r\n", tmp_string);
        }
        else{
            CLI_LOGI("key2 set fail:%d\r\n", ret);
        }

        ef_value3.m_a = tmp_value + 1;
        ef_value3.m_b = tmp_value + 2;
        memset(ef_value3.m_c, 0, sizeof(ef_value3.m_c));
        memset(ef_value3.m_c, data_c, sizeof(ef_value3.m_c)-1);
        ret = bk_set_buf_env(EF_KEY3, (const char *)&ef_value3, sizeof(ef_value3));
        if(EF_NO_ERR == ret){
            CLI_LOGI("key3 set value:%d, %d, %s\r\n", ef_value3.m_a, ef_value3.m_b, ef_value3.m_c);
        }
        else{
            CLI_LOGI("key3 set fail:%d\r\n", ret);
        }
    }
    else if (os_strcmp(argv[1], "del") == 0) {
        if((argc == 2)
            || (argc > 2 && 0 == os_memcmp(argv[2], EF_KEY1, strlen(EF_KEY1)))){
            if(EF_NO_ERR == bk_set_env(EF_KEY1, NULL))
                CLI_LOGI("key1 del success\r\n");
            else
                CLI_LOGI("key1 del fail\r\n");
        }
        
        if((argc == 2)
            || (argc > 2 && 0 == os_memcmp(argv[2], EF_KEY2, strlen(EF_KEY2)))){
            if(EF_NO_ERR == bk_set_env(EF_KEY2, NULL))
                CLI_LOGI("key2 del success\r\n");
            else
                CLI_LOGI("key2 del fail\r\n");
        }

        if((argc == 2)
            || (argc > 2 && 0 == os_memcmp(argv[2], EF_KEY3, strlen(EF_KEY3)))){
            if(EF_NO_ERR == bk_set_env(EF_KEY3, NULL))
                CLI_LOGI("key3 del success\r\n");
            else
                CLI_LOGI("key3 del fail\r\n");
        }
    }
    else if (os_strcmp(argv[1], "save") == 0){
        CLI_LOGI("save env\r\n");
        bk_save_env();
    }
#endif
    else {
        cli_easyflash_help();
        return;
    }
}

#define EASYFLASH_CMD_CNT (sizeof(s_easyflash_commands) / sizeof(struct cli_command))
static const struct cli_command s_easyflash_commands[] = {
    {"easyflashtest", "easyflashtest {write|read|del}", cli_easyflash_cmd}
};

int cli_easyflash_init(void)
{
    BK_LOG_ON_ERR(easyflash_init());
    return cli_register_commands(s_easyflash_commands, EASYFLASH_CMD_CNT);
}

#endif
