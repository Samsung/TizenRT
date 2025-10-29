#ifndef _BK_EASY_FLASH_H_
#define _BK_EASY_FLASH_H_

#include "easyflash.h"

#if defined(CONFIG_EASY_FLASH)

#if !defined(CONFIG_EASY_FLASH_V4)
#error "you shold config CONFIG_EASY_FLASH_V4"
#endif

#if defined(CONFIG_EASY_FLASH_V4)
//!!!new api, Strongly recommended to use
EfErrCode bk_set_env_enhance(const char *key, const void *value, int value_len);
int bk_get_env_enhance(const char *key, void *value, int value_len);
#endif

EfErrCode bk_save_env(void);
char *bk_get_env(const char *key);
EfErrCode bk_set_env(const char *key, const char *value);
EfErrCode bk_set_buf_env(const char *key, const char *buf, int len);
EfErrCode bk_get_buf_env(const char *key, const char *buf, int len);
#endif // CONFIG_EASY_FLASH

#endif // _BK_EASY_FLASH_H_
// eof

