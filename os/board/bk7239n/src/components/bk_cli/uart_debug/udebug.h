#ifndef _UDEBUG_H_
#define _UDEBUG_H_

#include <components/log.h>

#define UD_DEBUG

#ifdef UD_DEBUG
#define UD_TAG "ud"
#define UD_LOGI(...)      BK_LOGI(UD_TAG, ##__VA_ARGS__)
#define UD_LOGW(...)      BK_LOGW(UD_TAG, ##__VA_ARGS__)
#define UD_LOGE(...)      BK_LOGE(UD_TAG, ##__VA_ARGS__)
#define UD_LOGD(...)      BK_LOGD(UD_TAG, ##__VA_ARGS__)
#else
#define UD_LOGI(...)      os_null_printf
#define UD_LOGW(...)      os_null_printf
#endif

#define BKREG_MIN_LEN                     3
#define BKREG_MAGIC_WORD0                 (0x01)
#define BKREG_MAGIC_WORD1                 (0xE0uc)
#define BKREG_MAGIC_WORD2                 (0xFCuc)

// largest one  01 e0 fc 09 03 xx xx xx xx xx xx xx xx
#define UART_PEEK_LEN                     13

//#define CONSOLE_NO_LOCAL_ECHO
#define UD_SUPPORT_UPKEY

#define UP_KEY_COUNT                      3
#define DEBUG_CHUNK_DEFAULT_COUNT         20*5
#define UDEBUG_PROMPT                     "\\BK7211\\UART_DEBUG>"

enum {
	DEBUG_PRI_LEVEL_MIN,
	DEBUG_PRI_LEVEL_1,
	DEBUG_PRI_LEVEL_2,
	DEBUG_PRI_LEVEL_3,
	DEBUG_PRI_LEVEL_4,
	DEBUG_PRI_LEVEL_5,
	DEBUG_PRI_LEVEL_6,
	DEBUG_PRI_LEVEL_MAX
};

#endif // _UDEBUG_H_

// eof
