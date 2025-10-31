#include <common/bk_include.h>
#include <common/sys_config.h>

#if defined(CONFIG_UART_DEBUG)
#include "udebug.h"
#include "bk_uart_debug.h"
#include "bk_uart.h"
#include <os/mem.h>
#include "bk_drv_model.h"
#include "command_line.h"

char udebug_buf[CONFIG_SYS_CBSIZE];
UINT32 udebug_last_cnt = 0;
UINT32 udebug_cnt = 0;

UINT32 debug_pri_level = DEBUG_PRI_LEVEL_1;
UINT32 debug_chunk_count = DEBUG_CHUNK_DEFAULT_COUNT;

UINT32 uart_debug_init(void)
{
	return 0;
}

void udebug_update_chunk_cnt(void)
{
	debug_chunk_count = debug_pri_level * DEBUG_CHUNK_DEFAULT_COUNT;
}

void udebug_set_pri_level(UINT32 level)
{
	if (level >= DEBUG_PRI_LEVEL_MAX)
		level = DEBUG_PRI_LEVEL_6;
	else if (level <= DEBUG_PRI_LEVEL_MIN)
		level = DEBUG_PRI_LEVEL_1;

	debug_pri_level = level;
}

UINT32 udebug_get_pri_level(void)
{
	return debug_pri_level;
}

UINT32 udebug_has_ctrlc(void)
{
	char val;
	UINT32 ret = 0;
	UINT32 status;
	DD_HANDLE uart_hdl;
	uart_hdl = ddev_open(DD_DEV_TYPE_UART2, &status, 0);
	BK_ASSERT(DRV_FAILURE != uart_hdl);

	ret = ddev_read(uart_hdl, &val, sizeof(val), 0);
	if (ret) {
		if (0x03 == val) { /* ^C control c*/
			ddev_close(uart_hdl);
			return 1;
		}
	}

	ddev_close(uart_hdl);

	return 0;
}

UINT32 udebug_wait_ctrlc_exit(void)
{
	char val;
	UINT32 ret = 0;
	UINT32 status;
	DD_HANDLE uart_hdl;

	uart_hdl = ddev_open(DD_DEV_TYPE_UART2, &status, 0);
	BK_ASSERT(DRV_FAILURE != uart_hdl);

	while (1) {
		ret = ddev_read(uart_hdl, &val, sizeof(val), 0);
		if (ret) {
			if (0x03 == val) /* ^C control c*/
				break;
		}
	}

	ddev_close(uart_hdl);

	return 0;
}

UINT32 udebug_handler(void)
{
	char val;
	UINT32 ret;
	UINT32 hit = 0;
	UINT32 status;
	DD_HANDLE uart_hdl;
	UART_PEEK_RX_T peek;

	char uart_peek[UART_PEEK_LEN];

#if defined(CONFIG_BACKGROUND_PRINT)
	uart_hdl = ddev_open(DD_DEV_TYPE_UART2, &status, 0);
	BK_ASSERT(DRV_FAILURE != uart_hdl);

	ddev_control(uart_hdl, CMD_SEND_BACKGROUND, 0);
	ddev_close(uart_hdl);
#endif // CONFIG_BACKGROUND_PRINT

	uart_hdl = ddev_open(DD_DEV_TYPE_UART2, &status, 0);
	BK_ASSERT(DRV_FAILURE != uart_hdl);

	while (1) {
		do {
			ret = ddev_control(uart_hdl, CMD_RX_COUNT, 0);
			if (ret < BKREG_MIN_LEN)
				break;

			peek.sig = URX_PEEK_SIG;
			peek.ptr = &uart_peek[0];
			peek.len = UART_PEEK_LEN;
			ret = ddev_control(uart_hdl, CMD_RX_PEEK, &peek);

#ifdef UD_SUPPORT_UPKEY
			if ((UP_KEY_COUNT == ret)
				&& (0x1B == uart_peek[0])
				&& (0x4F == uart_peek[1])
				&& (0x41 == uart_peek[2])) {
				ret = ddev_read(uart_hdl, uart_peek, sizeof(uart_peek), 0);
				BK_ASSERT(UP_KEY_COUNT == ret);

				if (0 == udebug_cnt) {
					udebug_cnt = udebug_last_cnt;

					UD_LOGI("%s", udebug_buf);
					UD_LOGI("\r\n");

					run_command(udebug_buf, 0);

					UD_LOGI(UDEBUG_PROMPT);

					udebug_cnt = 0;
					os_memset(uart_peek, 0, sizeof(uart_peek));

					break;
				}
			} else
#endif   // UD_SUPPORT_UPKEY
			{
#if defined(CONFIG_BKREG)
				if ((ret > BKREG_MIN_LEN)
					&& (BKREG_MAGIC_WORD0 == uart_peek[0])
					&& (BKREG_MAGIC_WORD1 == uart_peek[1])
					&& (BKREG_MAGIC_WORD2 == uart_peek[2])) {
					ret = ddev_read(uart_hdl, uart_peek, sizeof(uart_peek), 0);
					BK_ASSERT(ret > BKREG_MIN_LEN);

					bkreg_run_command(uart_peek, ret);
					os_memset(uart_peek, 0, sizeof(uart_peek));

					break;
				}
#endif  // CONFIG_BKREG
			}
		} while (0);

		ret = ddev_read(uart_hdl, &val, sizeof(val), 0);
		if (0 == ret)
			break;

#ifdef CONSOLE_NO_LOCAL_ECHO
		UD_LOGI("%c", val);
#endif

		if (('\n' == val)
			|| ('\n' == val)
			|| ('\r' == val)
			|| (sizeof(udebug_buf) == udebug_cnt)) {
			hit = 1;
			break;
		}

		if (0x08 == val) { /* 0x08 bs backspace*/
			if (udebug_cnt >= 1)
				udebug_cnt = udebug_cnt - 1;
		} else {
			if (0 == udebug_cnt)
				os_memset(udebug_buf, 0, sizeof(udebug_buf));

			udebug_buf[udebug_cnt] = val;
			udebug_cnt ++;
		}
	}

	if (hit) {
		if (udebug_cnt)
			run_command(udebug_buf, 0);

#ifdef UD_SUPPORT_UPKEY
		udebug_last_cnt = udebug_cnt;
#else
		os_memset(udebug_buf, 0, sizeof(udebug_buf));
#endif

		udebug_cnt = 0;

		UD_LOGI("\r\n");
		UD_LOGI(UDEBUG_PROMPT);

		return 0;
	} else
		return 1;
}
#endif

// eof

