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

#include <os/os.h>
#include <stdbool.h>

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

#include "debug.h"

extern volatile uint32_t g_system_log_ready;

static uint8_t s_printf_enable = 1;

#ifdef CONFIG_ASYNC_IRQ_LOG
/* Async IRQ log configuration */
#ifndef CONFIG_ASYNC_IRQ_LOG_BUFFER_SIZE
#define CONFIG_ASYNC_IRQ_LOG_BUFFER_SIZE    2048
#endif

#ifndef CONFIG_ASYNC_IRQ_LOG_MAX_MSG_LEN
#define CONFIG_ASYNC_IRQ_LOG_MAX_MSG_LEN    128
#endif

#ifndef CONFIG_ASYNC_IRQ_LOG_TASK_PRIORITY
#define CONFIG_ASYNC_IRQ_LOG_TASK_PRIORITY  7
#endif

#define BK_ASYNC_IRQ_LOG_TASK_STACK     2048

/* Async IRQ log structure */
typedef struct {
    uint8_t *buffer;                  /* Ring buffer pointer (malloc allocated) */
    uint16_t buffer_size;             /* Buffer size */
    volatile uint16_t write_pos;      /* Write position (ISR context) */
    uint16_t read_pos;                /* Read position (Task context) */
    volatile uint16_t available;      /* Available space */
    beken_semaphore_t sem;            /* Semaphore for task wakeup */
    volatile uint32_t drop_count;     /* Dropped log count */
    volatile bool initialized;        /* Initialization flag */
    beken_thread_t task_thread;       /* Task thread handle */
} async_irq_log_t;

/* Null terminator size for each log entry (1 byte) */
#define ASYNC_IRQ_LOG_NULL_SIZE  1

static async_irq_log_t g_async_irq_log;

/* Forward declaration */
static void async_irq_log_task(void *arg);
static int async_irq_log_write(const char *fmt, va_list args);
static bool async_irq_log_find_null_terminator(uint16_t read_pos, uint16_t write_pos,
                                               uint8_t *buffer, uint16_t buffer_size,
                                               uint16_t *null_pos);
int async_irq_log_init(void);
int async_irq_log_deinit(void);
uint32_t async_irq_log_get_drop_count(void);
void async_irq_log_reset_drop_count(void);
#endif /* CONFIG_ASYNC_IRQ_LOG */
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

#ifdef CONFIG_ASYNC_IRQ_LOG
	/* Initialize async IRQ log system */
	// if (async_irq_log_init() != BK_OK) {
	// 	return BK_FAIL;
	// }
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

	// char string[CONFIG_PRINTF_BUF_SIZE];
	// os_snprintf(&string[0], CONFIG_PRINTF_BUF_SIZE, "[SYNC]:%s", prefix);
	// string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	// int   data_len = strlen(string);
	// vsnprintf(&string[data_len], CONFIG_PRINTF_BUF_SIZE - data_len, fmt, args);

    char string[CONFIG_PRINTF_BUF_SIZE];
	vsnprintf(&string[0], sizeof(string), fmt, args);

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
	int cpu_id = 0;//rtos_get_core_id();
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

void bk_dump_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    char string[CONFIG_PRINTF_BUF_SIZE];

	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;

	vsnprintf(&string[0], CONFIG_PRINTF_BUF_SIZE , fmt, args);

	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), string);

    va_end(args);
}


void bk_printf(const char *fmt, ...)
{
	va_list args;
    int ret = 0;

	va_start(args, fmt);

    if (g_system_log_ready == 0 || rtos_is_in_interrupt_context())
    {
		if (g_system_log_ready == 1) {
			#ifdef CONFIG_ASYNC_IRQ_LOG
				async_irq_log_write(fmt, args);
			#else
				bk_printf_sync_port(NULL, fmt, args);
			#endif
		} else {
			bk_printf_sync_port(NULL, fmt, args);
		}
    }
    else 
    {
#ifdef CONFIG_LOGM
	ret = logm_internal(LOGM_NORMAL, LOGM_IDX, LOGM_INF, fmt, args);
#else
	ret = vsyslog(LOG_INFO, fmt, args);
#endif
    }
	va_end(args);

	return;
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

void bk_printf_ext(int level, char *tag, const char *fmt, ...)
{
	va_list args;

    int ret = 0;
    int priority;

    char new_fmt[CONFIG_PRINTF_BUF_SIZE] = {0};
    const char *final_fmt = fmt;
    if (tag && tag[0] != '\0') {
        snprintf(new_fmt, sizeof(new_fmt), "[%s]%s", tag, fmt);
        final_fmt = new_fmt;
    }

	va_start(args, fmt);

    if (g_system_log_ready == 0 || rtos_is_in_interrupt_context())
    {
		if (g_system_log_ready == 1) {
			#ifdef CONFIG_ASYNC_IRQ_LOG
				async_irq_log_write(final_fmt, args);
			#else
				bk_printf_sync_port(NULL, final_fmt, args);
			#endif
		} else {
			bk_printf_sync_port(NULL, final_fmt, args);
		}
    }
    else 
    {
#ifdef CONFIG_LOGM
	ret = logm_internal(LOGM_NORMAL, LOGM_IDX, LOGM_INF, final_fmt, args);
#else
    if (level >= BK_LOG_ERROR)
    {
        priority = LOG_ERR;
    } 
    else if (level >= BK_LOG_WARN)
    {
        priority = LOG_WARNING;
    }
    else if (level >= BK_LOG_INFO)
    {
        priority = LOG_INFO;
    }
    else {
        priority = LOG_DEBUG;
    }
	ret = vsyslog(priority, final_fmt, args);
#endif
    }
	va_end(args);

	return;
}

void bk_vprintf_ext(int level, char *tag, const char *fmt, va_list args)
{
	// va_list args;

    int ret = 0;
    int priority;

    char new_fmt[CONFIG_PRINTF_BUF_SIZE] = {0};
    const char *final_fmt = fmt;
    if (tag && tag[0] != '\0') {
        snprintf(new_fmt, sizeof(new_fmt), "[%s]%s", tag, fmt);
        final_fmt = new_fmt;
    }

	// va_start(args, fmt);

    if (g_system_log_ready == 0 || rtos_is_in_interrupt_context())
    {
		if (g_system_log_ready == 1) {
			#ifdef CONFIG_ASYNC_IRQ_LOG
				async_irq_log_write(final_fmt, args);
			#else
				bk_printf_sync_port(NULL, final_fmt, args);
			#endif
		} else {
			bk_printf_sync_port(NULL, final_fmt, args);
		}
    }
    else 
    {
#ifdef CONFIG_LOGM
	ret = logm_internal(LOGM_NORMAL, LOGM_IDX, LOGM_INF, final_fmt, args);
#else
    if (level >= BK_LOG_ERROR)
    {
        priority = LOG_ERR;
    } 
    else if (level >= BK_LOG_WARN)
    {
        priority = LOG_WARNING;
    }
    else if (level >= BK_LOG_INFO)
    {
        priority = LOG_INFO;
    }
    else {
        priority = LOG_DEBUG;
    }
	ret = vsyslog(priority, final_fmt, args);
#endif
    }
	// va_end(args);

	return;
}

void bk_vprintf_raw(int level, char *tag, const char *fmt, va_list args)
{

    int ret = 0;
    int priority;

    if (g_system_log_ready == 0 || rtos_is_in_interrupt_context())
    {
		if (g_system_log_ready == 1) {
			#ifdef CONFIG_ASYNC_IRQ_LOG
				async_irq_log_write(fmt, args);
			#else
				bk_printf_sync_port(NULL, fmt, args);
			#endif
		} else {
			bk_printf_sync_port(NULL, fmt, args);
		}
    }
    else 
    {
#ifdef CONFIG_LOGM
	ret = logm_internal(LOGM_NORMAL, LOGM_IDX, LOGM_INF, fmt, args);
#else
    if (level >= BK_LOG_ERROR)
    {
        priority = LOG_ERR;
    } 
    else if (level >= BK_LOG_WARN)
    {
        priority = LOG_WARNING;
    }
    else if (level >= BK_LOG_INFO)
    {
        priority = LOG_INFO;
    }
    else {
        priority = LOG_DEBUG;
    }
	ret = vsyslog(LOG_ERR, fmt, args);
#endif
    }

	return;
}

void bk_printf_raw(int level, char *tag, const char *fmt, ...)
{
	va_list args;

    int ret = 0;
    int priority;
	va_start(args, fmt);

    if (g_system_log_ready == 0 || rtos_is_in_interrupt_context())
    {
		if (g_system_log_ready == 1) {
			#ifdef CONFIG_ASYNC_IRQ_LOG
				async_irq_log_write(fmt, args);
			#else
				bk_printf_sync_port(NULL, fmt, args);
			#endif
		} else {
			bk_printf_sync_port(NULL, fmt, args);
		}
    }
    else 
    {
#ifdef CONFIG_LOGM
	ret = logm_internal(LOGM_NORMAL, LOGM_IDX, LOGM_INF, fmt, args);
#else
    if (level >= BK_LOG_ERROR)
    {
        priority = LOG_ERR;
    } 
    else if (level >= BK_LOG_WARN)
    {
        priority = LOG_WARNING;
    }
    else if (level >= BK_LOG_INFO)
    {
        priority = LOG_INFO;
    }
    else {
        priority = LOG_DEBUG;
    }
	ret = vsyslog(LOG_ERR, fmt, args);
#endif
    }
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

static uint8_t s_ble_log_level = BK_LOG_INFO;
static uint8_t s_wifi_log_level = BK_LOG_INFO;
void bk_ble_printf_ext(int level, char *tag, const char *fmt, ...)
{
	if (level > s_ble_log_level) {
		return;
	}
	va_list args;
	va_start(args, fmt);
	bk_vprintf_ext(level, tag, fmt, args);
	va_end(args);
}

void bk_ble_printf_raw(int level, char *tag, const char *fmt, ...)
{
	if (level > s_ble_log_level) {
		return;
	}
	va_list args;
	va_start(args, fmt);
	bk_vprintf_raw(level, tag, fmt, args);
	va_end(args);
}

void bk_wifi_printf_ext(int level, char *tag, const char *fmt, ...)
{
	if (level > s_wifi_log_level) {
		return;
	}
	va_list args;
	va_start(args, fmt);
	bk_vprintf_ext(level, tag, fmt, args);
	va_end(args);
}

void bk_wifi_printf_raw(int level, char *tag, const char *fmt, ...)
{
	if (level > s_wifi_log_level) {
		return;
	}
	va_list args;
	va_start(args, fmt);
	bk_vprintf_raw(level, tag, fmt, args);
	va_end(args);
}

void bk_set_ble_log_level(uint8_t level)
{
	if (level > BK_LOG_ALL) {
		return;
	}
	s_ble_log_level = level;
}

void bk_set_wifi_log_level(uint8_t level)
{
	if (level > BK_LOG_ALL) {
		return;
	}
	s_wifi_log_level = level;
}

#ifdef CONFIG_ASYNC_IRQ_LOG
/****************************************************************************
 * Name: async_irq_log_init
 *
 * Description:
 *   Initialize the asynchronous IRQ log system
 *
 * Return Value:
 *   BK_OK on success, BK_FAIL on failure
 *
 ****************************************************************************/
int async_irq_log_init(void)
{
	int ret;
	uint32_t flags;

	/* Check if already initialized */
	if (g_async_irq_log.initialized) {
		return BK_OK;
	}

	/* Initialize buffer structure */
	flags = rtos_enter_critical();
	memset(&g_async_irq_log, 0, sizeof(async_irq_log_t));
	rtos_exit_critical(flags);

	/* Allocate ring buffer memory */
	g_async_irq_log.buffer = (uint8_t *)psram_malloc(CONFIG_ASYNC_IRQ_LOG_BUFFER_SIZE);
	if (g_async_irq_log.buffer == NULL) {
		return BK_FAIL;
	}

	/* Initialize buffer size */
	g_async_irq_log.buffer_size = CONFIG_ASYNC_IRQ_LOG_BUFFER_SIZE;

	/* Initialize semaphore (initial value = 0) */
	ret = rtos_init_semaphore(&g_async_irq_log.sem, 0);
	if (ret != BK_OK) {
		psram_free(g_async_irq_log.buffer);
		g_async_irq_log.buffer = NULL;
		return BK_FAIL;
	}

	/* Initialize positions */
	flags = rtos_enter_critical();
	g_async_irq_log.write_pos = 0;
	g_async_irq_log.read_pos = 0;
	g_async_irq_log.available = CONFIG_ASYNC_IRQ_LOG_BUFFER_SIZE;
	g_async_irq_log.drop_count = 0;
	g_async_irq_log.task_thread = NULL;
	rtos_exit_critical(flags);

	/* Create kernel thread */
	ret = rtos_create_thread(
		&g_async_irq_log.task_thread,
		CONFIG_ASYNC_IRQ_LOG_TASK_PRIORITY,
		"bk_irq_log",
		async_irq_log_task,
		BK_ASYNC_IRQ_LOG_TASK_STACK,
		NULL
	);

	if (ret != BK_OK) {
		rtos_deinit_semaphore(&g_async_irq_log.sem);
		psram_free(g_async_irq_log.buffer);
		g_async_irq_log.buffer = NULL;
		return BK_FAIL;
	}

	/* Mark as initialized */
	flags = rtos_enter_critical();
	g_async_irq_log.initialized = true;
	rtos_exit_critical(flags);

	return BK_OK;
}

/****************************************************************************
 * Name: async_irq_log_deinit
 *
 * Description:
 *   Deinitialize the asynchronous IRQ log system
 *
 * Return Value:
 *   BK_OK on success
 *
 ****************************************************************************/
int async_irq_log_deinit(void)
{
	uint32_t flags;

	if (!g_async_irq_log.initialized) {
		return BK_OK;
	}

	/* Mark as uninitialized */
	flags = rtos_enter_critical();
	g_async_irq_log.initialized = false;
	rtos_exit_critical(flags);

	/* Destroy semaphore */
	rtos_deinit_semaphore(&g_async_irq_log.sem);

	/* Free ring buffer memory */
	if (g_async_irq_log.buffer != NULL) {
		psram_free(g_async_irq_log.buffer);
		g_async_irq_log.buffer = NULL;
		g_async_irq_log.buffer_size = 0;
	}

	return BK_OK;
}

/****************************************************************************
 * Name: async_irq_log_write
 *
 * Description:
 *   Write log message to ring buffer (called from interrupt context)
 *
 * Input Parameters:
 *   fmt - Format string
 *   args - Variable argument list
 *
 * Return Value:
 *   BK_OK on success, BK_FAIL on failure
 *
 ****************************************************************************/
static int async_irq_log_write(const char *fmt, va_list args)
{
	char log_msg[CONFIG_ASYNC_IRQ_LOG_MAX_MSG_LEN + 1];
	int msg_len;
	uint16_t write_pos;
	uint16_t read_pos;
	uint16_t available;
	uint16_t write_len;
	uint32_t flags;

	/* Check if initialized */
	if (!g_async_irq_log.initialized) {
		return BK_FAIL;
	}

	/* Check if buffer is allocated */
	if (g_async_irq_log.buffer == NULL) {
		return BK_FAIL;
	}

	/* Format the log message */
	msg_len = vsnprintf(log_msg, sizeof(log_msg), fmt, args);
	if (msg_len < 0) {
		return BK_FAIL;
	}

	/* Limit message length */
	if (msg_len > CONFIG_ASYNC_IRQ_LOG_MAX_MSG_LEN) {
		msg_len = CONFIG_ASYNC_IRQ_LOG_MAX_MSG_LEN;
		log_msg[msg_len] = '\0';
	}


	/* Get current positions atomically */
	flags = rtos_enter_critical();
	write_pos = g_async_irq_log.write_pos;
	read_pos = g_async_irq_log.read_pos;
	available = g_async_irq_log.available;

	/* Calculate total size needed (message + null terminator) */
	uint16_t total_size = (uint16_t)msg_len + ASYNC_IRQ_LOG_NULL_SIZE;

	/* Check if there's enough space */
	if (available < total_size) {
		/* Not enough space, drop the log */
		g_async_irq_log.drop_count++;
		rtos_exit_critical(flags);
		return BK_FAIL;
	}

	write_pos = g_async_irq_log.write_pos;
	read_pos = g_async_irq_log.read_pos;

	/* Write message data + null terminator */
	if (write_pos + total_size <= g_async_irq_log.buffer_size) {
		/* No wrap-around */
		memcpy(&g_async_irq_log.buffer[write_pos], log_msg, msg_len);
		g_async_irq_log.buffer[write_pos + msg_len] = '\0';
		write_pos = (write_pos + total_size) % g_async_irq_log.buffer_size;
	} else {
		/* Wrap-around case */
		uint16_t first_part = g_async_irq_log.buffer_size - write_pos;
		uint16_t second_part = total_size - first_part;

		if (first_part > msg_len) {
			/* Message fits in first part, null terminator wraps */
			memcpy(&g_async_irq_log.buffer[write_pos], log_msg, msg_len);
			g_async_irq_log.buffer[0] = '\0';
			write_pos = 1;
		} else {
			/* Message wraps around */
			memcpy(&g_async_irq_log.buffer[write_pos], log_msg, first_part);
			uint16_t msg_second_part = msg_len - first_part;
			memcpy(&g_async_irq_log.buffer[0], &log_msg[first_part], msg_second_part);
			g_async_irq_log.buffer[msg_second_part] = '\0';
			write_pos = msg_second_part + 1;
		}
	}

	/* Update write position and available space */
	g_async_irq_log.write_pos = write_pos;
	g_async_irq_log.available -= total_size;
	rtos_exit_critical(flags);

	/* Post semaphore to wake up task */
	rtos_set_semaphore(&g_async_irq_log.sem);

	return BK_OK;
}

/****************************************************************************
 * Name: async_irq_log_find_null_terminator
 *
 * Description:
 *   Search for null terminator in ring buffer to find log boundary
 *
 * Input Parameters:
 *   read_pos  - Current read position
 *   write_pos - Current write position
 *   buffer    - Ring buffer pointer
 *   buffer_size - Buffer size
 *   null_pos  - Output parameter: position of null terminator if found
 *
 * Return Value:
 *   true if null terminator found, false otherwise
 *
 ****************************************************************************/
static bool async_irq_log_find_null_terminator(uint16_t read_pos, uint16_t write_pos,
                                               uint8_t *buffer, uint16_t buffer_size,
                                               uint16_t *null_pos)
{
	uint8_t *null_ptr = NULL;

	if (write_pos > read_pos) {
		/* No wrap-around, search linearly */
		null_ptr = (uint8_t *)memchr(&buffer[read_pos], '\0', write_pos - read_pos);
		if (null_ptr != NULL) {
			*null_pos = null_ptr - buffer;
			return true;
		}
	} else {
		/* Wrap-around case: search in two parts */
		/* First part: from read_pos to end of buffer */
		null_ptr = (uint8_t *)memchr(&buffer[read_pos], '\0', 
		                             buffer_size - read_pos);
		if (null_ptr != NULL) {
			*null_pos = null_ptr - buffer;
			return true;
		}

		/* Second part: from start to write_pos */
		if (write_pos > 0) {
			null_ptr = (uint8_t *)memchr(&buffer[0], '\0', write_pos);
			if (null_ptr != NULL) {
				*null_pos = null_ptr - buffer;
				return true;
			}
		}
	}

	return false;
}

/****************************************************************************
 * Name: async_irq_log_task
 *
 * Description:
 *   Task that processes log messages from ring buffer
 *
 * Input Parameters:
 *   arg - Argument (unused)
 ****************************************************************************/
static void async_irq_log_task(void *arg)
{
	char log_msg[CONFIG_ASYNC_IRQ_LOG_MAX_MSG_LEN + 1];
	uint16_t msg_len;
	uint16_t read_pos;
	uint16_t write_pos;
	uint16_t available;
	bool has_data;
	uint32_t flags;

	(void)arg;

	while (1) {
		/* Wait for semaphore (blocking) */
		if (rtos_get_semaphore(&g_async_irq_log.sem, BEKEN_WAIT_FOREVER) != BK_OK) {
			/* Handle error, but continue loop */
			rtos_delay_milliseconds(1000); /* Sleep 1ms on error */
			continue;
		}
		/* Check if buffer is allocated */
		if (g_async_irq_log.buffer == NULL) {
			break;
		}

		/* Process all available logs in buffer */
		do {
			has_data = false;

			/* Get current positions atomically */
			flags = rtos_enter_critical();
			read_pos = g_async_irq_log.read_pos;
			write_pos = g_async_irq_log.write_pos;
			available = g_async_irq_log.available;
			rtos_exit_critical(flags);

			/* Check if there's data to read */
			/* In ring buffer, read_pos == write_pos can mean either EMPTY or FULL */
			/* We use 'available' field to distinguish:
			 *   - read_pos == write_pos && available == buffer_size: EMPTY (no data)
			 *   - read_pos == write_pos && available == 0: FULL (has data, but buffer is full)
			 *   - read_pos != write_pos: has data
			 * So we check: read_pos != write_pos OR available < buffer_size */
			if (read_pos != write_pos || available < g_async_irq_log.buffer_size) {
				uint16_t current_read_pos = read_pos;
				uint16_t null_pos = 0;
				bool found_null;

				/* Search for null terminator to find log boundary */
				found_null = async_irq_log_find_null_terminator(
					current_read_pos,
					write_pos,
					g_async_irq_log.buffer,
					g_async_irq_log.buffer_size,
					&null_pos);

				if (!found_null) {
					/* No null terminator found yet, wait for more data */
					break;
				}

				/* Calculate message length */
				if (null_pos >= current_read_pos) {
					msg_len = null_pos - current_read_pos;
				} else {
					/* Wrap-around case */
					msg_len = (g_async_irq_log.buffer_size - current_read_pos) + null_pos;
				}

				/* Validate message length */
				/* Assert if message length exceeds maximum (should never happen) */
				BK_ASSERT(msg_len <= CONFIG_ASYNC_IRQ_LOG_MAX_MSG_LEN);
				
				if (msg_len == 0) {
					/* Empty message, skip this entry (skip null terminator) */
					flags = rtos_enter_critical();
					g_async_irq_log.read_pos = (null_pos + 1) % g_async_irq_log.buffer_size;
					g_async_irq_log.available += ASYNC_IRQ_LOG_NULL_SIZE;
					rtos_exit_critical(flags);
					continue;
				}

				has_data = true;

				/* Copy message from buffer (excluding null terminator) */
				if (null_pos >= current_read_pos) {
					/* No wrap-around */
					memcpy(log_msg, &g_async_irq_log.buffer[current_read_pos], msg_len);
					current_read_pos = (null_pos + 1) % g_async_irq_log.buffer_size;
				} else {
					/* Wrap-around case */
					uint16_t first_part = g_async_irq_log.buffer_size - current_read_pos;
					memcpy(log_msg, &g_async_irq_log.buffer[current_read_pos], first_part);
					memcpy(&log_msg[first_part], &g_async_irq_log.buffer[0],
					       msg_len - first_part);
					current_read_pos = (null_pos + 1) % g_async_irq_log.buffer_size;
				}
				log_msg[msg_len] = '\0';

				/* Update read position and available space */
				flags = rtos_enter_critical();
				g_async_irq_log.read_pos = current_read_pos;
				g_async_irq_log.available += (msg_len + ASYNC_IRQ_LOG_NULL_SIZE);
				rtos_exit_critical(flags);

				/* Output log message */
				bk_printf_ext(0, NULL, "%s", log_msg);
			}
		} while (has_data);
	}

	return 0; /* Should never reach here */
}

/****************************************************************************
 * Name: async_irq_log_get_drop_count
 *
 * Description:
 *   Get the count of dropped log messages
 *
 * Return Value:
 *   Number of dropped messages
 *
 ****************************************************************************/
uint32_t async_irq_log_get_drop_count(void)
{
	return g_async_irq_log.drop_count;
}

/****************************************************************************
 * Name: async_irq_log_reset_drop_count
 *
 * Description:
 *   Reset the dropped log count
 *
 ****************************************************************************/
void async_irq_log_reset_drop_count(void)
{
	uint32_t flags;

	flags = rtos_enter_critical();
	g_async_irq_log.drop_count = 0;
	rtos_exit_critical(flags);
}
#endif /* CONFIG_ASYNC_IRQ_LOG */
