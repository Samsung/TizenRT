/*
 * wpa_supplicant/hostapd / Debug prints
 * Copyright (c) 2002-2013, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "includes.h"

#include "common.h"

int wpa_debug_level = MSG_INFO;
int wpa_debug_show_keys = 0;
int wpa_debug_timestamp = 0;

void wpa_set_debug_level(int level)
{
	if (level >= 0 && level <= MSG_ERROR) {
		wpa_debug_level = level;
		WPA_LOGI("set wpa debug level to %d\r\n", wpa_debug_level);
	}
}

#ifndef CONFIG_NO_STDOUT_DEBUG
void wpa_debug_print_timestamp(void)
{
#if 0
	struct os_time tv;

	if (!wpa_debug_timestamp)
		return;

	os_get_time(&tv);
	WPA_LOGI("%ld.%06u: ", (long) tv.sec, (unsigned int) tv.usec);
#endif
}

/**
 * wpa_printf - conditional printf
 * @level: priority level (MSG_*) of the message
 * @fmt: printf format string, followed by optional arguments
 *
 * This function is used to print conditional debugging and error messages. The
 * output may be directed to stdout, stderr, and/or syslog based on
 * configuration.
 *
 * Note: New line '\n' is added to the end of the text when printing to stdout.
 */
void wpa_dbg(void *ctx, int level, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;

	if (level >= wpa_debug_level) {
		va_start(ap, fmt);
		buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
		va_end(ap);

		buf = os_malloc(buflen);
		if (buf == NULL) {
			WPA_LOGE("wpa_msg: Failed to allocate message "
				   "buffer");
			return;
		}
		va_start(ap, fmt);

		vsnprintf(buf, buflen, fmt, ap);
		va_end(ap);
		WPA_LOGI("%s\n", buf);
		os_free(buf);
	}
}

static void _wpa_hexdump(int level, const char *title, const u8 *buf,
			 size_t len, int show)
{
	size_t i;

	if (level < wpa_debug_level)
		return;
	wpa_debug_print_timestamp();
	WPA_LOGI("%s - hexdump(len=%lu):", title, (unsigned long) len);
	if (buf == NULL) {
		WPA_LOGI(" [NULL]");
	} else if (show) {
		for (i = 0; i < len; i++)
			bk_printf(" %02x", buf[i]);
	} else {
		WPA_LOGI(" [REMOVED]");
	}
	WPA_LOGI("\r\n");
}

void wpa_hexdump(int level, const char *title, const void *buf, size_t len)
{
	_wpa_hexdump(level, title, buf, len, 1);
}

void wpa_hexdump_key(int level, const char *title, const void *buf, size_t len)
{
	_wpa_hexdump(level, title, buf, len, wpa_debug_show_keys);
}

static void _wpa_hexdump_ascii(int level, const char *title, const void *buf,
			       size_t len, int show)
{
	size_t i, llen;
	const u8 *pos = buf;
	const size_t line_len = 16;

	if (level < wpa_debug_level)
		return;
	wpa_debug_print_timestamp();
	if (!show) {
		WPA_LOGI("%s - hexdump_ascii(len=%lu): [REMOVED]\r\n",
		       title, (unsigned long) len);
		return;
	}
	if (buf == NULL) {
		WPA_LOGI("%s - hexdump_ascii(len=%lu): [NULL]\r\n",
		       title, (unsigned long) len);
		return;
	}
	WPA_LOGI("%s - hexdump_ascii(len=%lu):\r\n", title, (unsigned long) len);
	while (len) {
		llen = len > line_len ? line_len : len;
		WPA_LOGI("    ");
		for (i = 0; i < llen; i++)
			bk_printf(" %02x", pos[i]);
		for (i = llen; i < line_len; i++)
			bk_printf("   ");
		bk_printf("   ");
		for (i = 0; i < llen; i++) {
			if (isprint(pos[i]))
				bk_printf("%c", pos[i]);
			else
				bk_printf("_");
		}
		for (i = llen; i < line_len; i++)
			bk_printf(" ");
		WPA_LOGI("\r\n");
		pos += llen;
		len -= llen;
	}
}

void wpa_hexdump_ascii(int level, const char *title, const void *buf,
		       size_t len)
{
	_wpa_hexdump_ascii(level, title, buf, len, 1);
}

void wpa_hexdump_ascii_key(int level, const char *title, const void *buf,
			   size_t len)
{
	_wpa_hexdump_ascii(level, title, buf, len, wpa_debug_show_keys);
}

int wpa_debug_reopen_file(void)
{
	return 0;
}

int wpa_debug_open_file(const char *path)
{
	(void)path;
	return 0;
}

void wpa_debug_close_file(void)
{
}

void wpa_debug_setup_stdout(void)
{
}

#endif /* CONFIG_NO_STDOUT_DEBUG */


#ifndef CONFIG_NO_WPA_MSG
static wpa_msg_cb_func wpa_msg_cb = NULL;

void wpa_msg_register_cb(wpa_msg_cb_func func)
{
	wpa_msg_cb = func;
}

static wpa_msg_get_ifname_func wpa_msg_ifname_cb = NULL;

void wpa_msg_register_ifname_cb(wpa_msg_get_ifname_func func)
{
	wpa_msg_ifname_cb = func;
}

void wpa_msg(void *ctx, int level, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;
	int len;
	char prefix[130];

	va_start(ap, fmt);
	buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);

	buf = os_malloc(buflen);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR, "wpa_msg: Failed to allocate message "
			   "buffer");
		return;
	}
	va_start(ap, fmt);
	prefix[0] = '\0';
	if (wpa_msg_ifname_cb) {
		const char *ifname = wpa_msg_ifname_cb(ctx);
		if (ifname) {
			int res = os_snprintf(prefix, sizeof(prefix), "%s: ",
					      ifname);
			if (os_snprintf_error(sizeof(prefix), res))
				prefix[0] = '\0';
		}
	}
	len = os_vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);

#ifndef CONFIG_NO_STDOUT_DEBUG
	WPA_LOGI("%s\r\n", buf);
	//wpa_printf(level, "%s%s", prefix, buf);
#endif

	if (wpa_msg_cb)
		wpa_msg_cb(ctx, level, WPA_MSG_PER_INTERFACE, buf, len);	//wpa_supplicant_ctrl_iface_msg_cb
	bin_clear_free(buf, buflen);
}


void wpa_msg_ctrl(void *ctx, int level, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;
	int len;

	if (!wpa_msg_cb)
		return;

	va_start(ap, fmt);
	buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);

	buf = os_malloc(buflen);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR, "wpa_msg_ctrl: Failed to allocate "
			   "message buffer");
		return;
	}
	va_start(ap, fmt);
	len = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
	wpa_msg_cb(ctx, level, WPA_MSG_PER_INTERFACE, buf, len);
	bin_clear_free(buf, buflen);
}


void wpa_msg_global(void *ctx, int level, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;
	int len;

	va_start(ap, fmt);
	buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);

	buf = os_malloc(buflen);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR, "wpa_msg_global: Failed to allocate "
			   "message buffer");
		return;
	}
	va_start(ap, fmt);
	len = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
	wpa_printf(level, "%s", buf);
	if (wpa_msg_cb)
		wpa_msg_cb(ctx, level, WPA_MSG_GLOBAL, buf, len);
	bin_clear_free(buf, buflen);
}


void wpa_msg_global_ctrl(void *ctx, int level, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;
	int len;

	if (!wpa_msg_cb)
		return;

	va_start(ap, fmt);
	buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);

	buf = os_malloc(buflen);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR,
			   "wpa_msg_global_ctrl: Failed to allocate message buffer");
		return;
	}
	va_start(ap, fmt);
	len = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
	wpa_msg_cb(ctx, level, WPA_MSG_GLOBAL, buf, len);
	bin_clear_free(buf, buflen);
}


void wpa_msg_no_global(void *ctx, int level, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;
	int len;

	va_start(ap, fmt);
	buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);

	buf = os_malloc(buflen);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR, "wpa_msg_no_global: Failed to allocate "
			   "message buffer");
		return;
	}
	va_start(ap, fmt);
	len = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
	wpa_printf(level, "%s", buf);
	if (wpa_msg_cb)
		wpa_msg_cb(ctx, level, WPA_MSG_NO_GLOBAL, buf, len);
	bin_clear_free(buf, buflen);
}


void wpa_msg_global_only(void *ctx, int level, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;
	int len;

	va_start(ap, fmt);
	buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);

	buf = os_malloc(buflen);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR, "%s: Failed to allocate message buffer",
			   __func__);
		return;
	}
	va_start(ap, fmt);
	len = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
	wpa_printf(level, "%s", buf);
	if (wpa_msg_cb)
		wpa_msg_cb(ctx, level, WPA_MSG_ONLY_GLOBAL, buf, len);
	os_free(buf);
}

#endif /* CONFIG_NO_WPA_MSG */


#ifndef CONFIG_NO_HOSTAPD_LOGGER
static hostapd_logger_cb_func hostapd_logger_cb = NULL;

void hostapd_logger_register_cb(hostapd_logger_cb_func func)
{
	hostapd_logger_cb = func;
}


void hostapd_logger(void *ctx, const u8 *addr, unsigned int module, int level,
		    const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int buflen;
	int len;

	va_start(ap, fmt);
	buflen = vsnprintf(NULL, 0, fmt, ap) + 1;
	va_end(ap);

	buf = os_malloc(buflen);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR, "hostapd_logger: Failed to allocate "
			   "message buffer");
		return;
	}
	va_start(ap, fmt);
	len = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
	if (hostapd_logger_cb)
		hostapd_logger_cb(ctx, addr, module, level, buf, len);
	else if (addr)
		wpa_printf(MSG_DEBUG, "hostapd_logger: STA " MACSTR " - %s",
			   MAC2STR(addr), buf);
	else
		wpa_printf(MSG_DEBUG, "hostapd_logger: %s", buf);
	bin_clear_free(buf, buflen);
}
#endif /* CONFIG_NO_HOSTAPD_LOGGER */


const char * debug_level_str(int level)
{
	switch (level) {
	case MSG_EXCESSIVE:
		return "EXCESSIVE";
	case MSG_MSGDUMP:
		return "MSGDUMP";
	case MSG_DEBUG:
		return "DEBUG";
	case MSG_INFO:
		return "INFO";
	case MSG_WARNING:
		return "WARNING";
	case MSG_ERROR:
		return "ERROR";
	default:
		return "?";
	}
}


int str_to_debug_level(const char *s)
{
	if (os_strcasecmp(s, "EXCESSIVE") == 0)
		return MSG_EXCESSIVE;
	if (os_strcasecmp(s, "MSGDUMP") == 0)
		return MSG_MSGDUMP;
	if (os_strcasecmp(s, "DEBUG") == 0)
		return MSG_DEBUG;
	if (os_strcasecmp(s, "INFO") == 0)
		return MSG_INFO;
	if (os_strcasecmp(s, "WARNING") == 0)
		return MSG_WARNING;
	if (os_strcasecmp(s, "ERROR") == 0)
		return MSG_ERROR;
	return -1;
}
