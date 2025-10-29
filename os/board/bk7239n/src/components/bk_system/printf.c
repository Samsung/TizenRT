// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdarg.h>
#include <string.h>
#include <driver/uart.h>
#include "bk_uart.h"
#include <common/sys_config.h>
#include <common/bk_compiler.h>
#include <os/mem.h>
#include "printf_impl.h"

#if defined(CONFIG_SHELL_ASYNCLOG)
#include "bk_api_cli.h"
#endif
#include <os/str.h>

#if defined(CONFIG_SMP_SYNC_PRINTF)
#include "FreeRTOS.h"
#include "spinlock.h"

volatile spinlock_t printf_spin_lock = SPIN_LOCK_INIT;

/*temporarily addressed the issue of printing in process context, and in the interrupt context it cannot*/
#define MULTI_CORE_PRINTF_LOCK()   spin_lock(&printf_spin_lock)
#define MULTI_CORE_PRINTF_UNLOCK() spin_unlock(&printf_spin_lock)
#else
#define MULTI_CORE_PRINTF_LOCK()
#define MULTI_CORE_PRINTF_UNLOCK()
#endif

#ifndef CONFIG_PRINTF_BUF_SIZE
#define CONFIG_PRINTF_BUF_SIZE    (136)
#endif

static uint8_t s_printf_enable = 1;
#if defined(CONFIG_SHELL_ASYNCLOG)
static volatile uint8_t s_printf_sync = 0;

typedef  struct
{
	char	mod_name[11];
	u8		disabled;
} __bk_packed mod_disable_list_t;

static mod_disable_list_t mod_tag_list[6];
static u8 whitelist_enabled = 0;
#endif

int printf_lock_init(void)
{
#if defined(CONFIG_SHELL_ASYNCLOG)
	memset(&mod_tag_list[0], 0, sizeof(mod_tag_list));
	shell_set_log_level(LOG_LEVEL);
#endif

	return BK_OK;
}

int printf_lock_deinit(void)
{
	return BK_OK;
}

static void bk_printf_sync_port(char *prefix, const char *fmt, va_list args)
{
	if(!printf_is_init())
		return;

	char string[CONFIG_PRINTF_BUF_SIZE];

	os_snprintf(&string[0], CONFIG_PRINTF_BUF_SIZE, "[SYNC]:%s", prefix);
	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;

	int   data_len = strlen(string);

	vsnprintf(&string[data_len], CONFIG_PRINTF_BUF_SIZE - data_len, fmt, args);

	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), string);
}

static void bk_printf_raw_sync(const char *fmt, va_list args)
{
	if(!printf_is_init())
		return;

	char string[CONFIG_PRINTF_BUF_SIZE];

	vsnprintf(&string[0], sizeof(string), fmt, args);
	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), string);
}

static const char * level_format[] =
{
	"(%d):",
	"E(%d):",
	"W(%d):",
	"I(%d):",
	"D(%d):",
	"V(%d):",
};

void bk_printf_port_ext(int level, char *tag, const char *fmt, va_list args)
{
#define CPU_STR_LEN		4
#define MAX_TAG_LEN		8
#define MAX_INFO_LEN	14    /* 13 + 1(:) */

	/*                  "cpux" + ":"       "Tag12345" + ":"    "X(1234567890)" + ":"   */
	char    prefix_str[(CPU_STR_LEN + 1) + (MAX_TAG_LEN + 1) + (MAX_INFO_LEN) + 1];
	int     data_len = 0;
#if defined(CONFIG_SHELL_ASYNCLOG)
	int cpu_id = shell_get_cpu_id();
#else
	int cpu_id = rtos_get_core_id();
#endif

	prefix_str[0] = 0;
	if(cpu_id >= 0)
	{
		os_snprintf(&prefix_str[data_len], CPU_STR_LEN + 1, "cpu%d", cpu_id);
		data_len += CPU_STR_LEN;

		prefix_str[data_len++] = ':';
		prefix_str[data_len] = 0;
	}

	if((tag != NULL) && (tag[0] != 0))
	{
		strncpy(&prefix_str[data_len], tag, MAX_TAG_LEN);
		prefix_str[MAX_TAG_LEN + data_len] = 0;

		data_len = strlen(prefix_str);

		prefix_str[data_len] = ':';
		data_len++;
		prefix_str[data_len] = 0;
	}

	os_snprintf(&prefix_str[data_len], MAX_INFO_LEN + 1, level_format[level], rtos_get_time());

#if defined(CONFIG_SHELL_ASYNCLOG)

	if(s_printf_sync == 0)
	{
		shell_log_out_port(level, prefix_str, fmt, args);
	}
	else
	{
		bk_printf_sync_port(prefix_str, fmt, args);
	}

#else
	bk_printf_sync_port(prefix_str, fmt, args);
#endif // #if CONFIG_SHELL_ASYNCLOG
}

void bk_printf_raw_port(int level, const char *fmt, va_list args)
{
#if defined(CONFIG_SHELL_ASYNCLOG)

	if(s_printf_sync == 0)
	{
		shell_log_out_port(level, NULL, fmt, args);
	}
	else
	{
		bk_printf_raw_sync(fmt, args);
	}

#else
	bk_printf_raw_sync(fmt, args);
#endif // #if CONFIG_SHELL_ASYNCLOG
}

void bk_printf(const char *fmt, ...)
{
	int level = 0; // BK_LOG_INFO;
	va_list args;

	if(!printf_is_init())
		return;

	if(!s_printf_enable)
		return;

#if defined(CONFIG_SHELL_ASYNCLOG)
	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;
#endif

	va_start(args, fmt);
	MULTI_CORE_PRINTF_LOCK();

	bk_printf_port_ext(level, NULL, fmt, args);

	MULTI_CORE_PRINTF_UNLOCK();
	va_end(args);

	return;
}

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
void bk_print_dump_hex(const uint8_t *ptr, size_t buflen)
{
    unsigned char *buf = (unsigned char*)ptr;
    int i, j;

    for (i=0; i<buflen; i+=16)
    {
        bk_printf("%08X: ", i);

        for (j=0; j<16; j++)
            if (i+j < buflen)
                bk_printf("%02X ", buf[i+j]);
            else
                bk_printf("   ");
        bk_printf(" ");

        for (j=0; j<16; j++)
            if (i+j < buflen)
                bk_printf("%c", __is_print(buf[i+j]) ? buf[i+j] : '.');
        bk_printf("\n");
    }
}

#if defined(CONFIG_SHELL_ASYNCLOG)
static int bk_mod_printf_disbled(char * tag)
{
	int		i, result;

	for(i = 0; i < ARRAY_SIZE(mod_tag_list); i++)
	{
		if(mod_tag_list[i].disabled)
		{
			result = strncmp(mod_tag_list[i].mod_name, tag, sizeof(mod_tag_list[i].mod_name) - 1);

			if(result == 0)
				return 1;
		}
	}

	return 0;
}

void bk_enable_white_list(int enabled)
{
	if(enabled)
		whitelist_enabled = 1;
	else
		whitelist_enabled = 0;
}

int bk_white_list_state(void)
{
	return whitelist_enabled;
}

#endif // #if CONFIG_SHELL_ASYNCLOG

/*  ========= !! NOTE !! =========  */
/*          Obsoleted  API          */
void bk_buf_printf_sync(char *buf, int buf_len)   /* Obsoleted  API */
{
	return;
}

/*  ========= !! NOTE !! =========  */
/*          Obsoleted  API          */
/* use bk_printf_ext(...) instead.  */
void bk_printf_ex(int level, char *tag, const char *fmt, ...)   /* Obsoleted  API */
{
#if 0
	va_list args;

	if(!printf_is_init())
		return;

#if defined(CONFIG_SHELL_ASYNCLOG)
	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	if(bk_mod_printf_disbled(tag) ^ whitelist_enabled)
		return;
#endif

	va_start(args, fmt);

	bk_printf_port(level, tag, fmt, args);

	va_end(args);

	return;
#endif
}

void bk_printf_ext(int level, char *tag, const char *fmt, ...)
{
	va_list args;

	if(!printf_is_init())
		return;

        if(!s_printf_enable)
                return;

#if defined(CONFIG_SHELL_ASYNCLOG)
	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	if(tag != NULL)
	{
		if(bk_mod_printf_disbled(tag) ^ whitelist_enabled)
			return;
	}
#endif

	va_start(args, fmt);

	bk_printf_port_ext(level, tag, fmt, args);

	va_end(args);

	return;
}

void bk_printf_raw(int level, char *tag, const char *fmt, ...)
{
	va_list args;

	if(!printf_is_init())
		return;

	if(!s_printf_enable)
		return;

#if defined(CONFIG_SHELL_ASYNCLOG)
	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	if(tag != NULL)
	{
		if(bk_mod_printf_disbled(tag) ^ whitelist_enabled)
			return;
	}
#endif

	va_start(args, fmt);

	bk_printf_raw_port(level, fmt, args);

	va_end(args);

	return;
}

uint8_t bk_get_printf_enable(void) {
	return s_printf_enable;
}

void bk_set_printf_enable(uint8_t enable)
{
#if defined(CONFIG_SHELL_ASYNCLOG)
	if(0 == enable) {
		shell_echo_set(0);
		shell_set_log_level(0);
	} else {
		shell_echo_set(1);
		shell_set_log_level(LOG_LEVEL);
	}
#endif
	s_printf_enable = enable;
}

void bk_set_printf_sync(uint8_t enable)
{
#if defined(CONFIG_SHELL_ASYNCLOG)
#if defined(CONFIG_SYS_CPU1)
	s_printf_sync = 0;
#else
	s_printf_sync = enable;
#endif
#endif
}

int bk_get_printf_sync(void)
{
#if defined(CONFIG_SHELL_ASYNCLOG)
	return s_printf_sync;
#endif

	return 1;
}

void bk_disable_mod_printf(char *mod_name, uint8_t disable)
{
#if defined(CONFIG_SHELL_ASYNCLOG)

	int		i, j, result;

	disable = disable ^ whitelist_enabled;

	if(disable == 0)
	{
		for(i = 0; i < ARRAY_SIZE(mod_tag_list); i++)
		{
			if(mod_tag_list[i].disabled)
			{
				result = strncmp(mod_tag_list[i].mod_name, mod_name, sizeof(mod_tag_list[i].mod_name) - 1);

				if(result == 0)
				{
					mod_tag_list[i].disabled = 0;
				}
			}
		}
	}
	else
	{
		j = ARRAY_SIZE(mod_tag_list);

		for(i = 0; i < ARRAY_SIZE(mod_tag_list); i++)
		{
			if(mod_tag_list[i].disabled)
			{
				result = strncmp(mod_tag_list[i].mod_name, mod_name, sizeof(mod_tag_list[i].mod_name) - 1);

				if(result == 0)
				{
					return;
				}
			}
			else
				j = i;
		}

		if(j >= ARRAY_SIZE(mod_tag_list)) /* no free slot to record this module name. */
			return;
		else
		{
			strncpy(mod_tag_list[j].mod_name, mod_name, sizeof(mod_tag_list[j].mod_name) - 1);
			mod_tag_list[j].mod_name[sizeof(mod_tag_list[j].mod_name) - 1] = 0;
			mod_tag_list[j].disabled = 1;
		}
	}

	return;

#endif
}

char * bk_get_disable_mod(int * idx)
{
#if defined(CONFIG_SHELL_ASYNCLOG)

	int 	i;

	for(i = *idx; i < ARRAY_SIZE(mod_tag_list); i++)
	{
		(*idx)++;
		if(mod_tag_list[i].disabled)
		{
			return &mod_tag_list[i].mod_name[0];
		}
	}

	return NULL;

#endif

	return NULL;
}

