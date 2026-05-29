/*
 * wpa_supplicant/hostapd / Debug prints
 * Copyright (c) 2002-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPA_DEBUG_H
#define WPA_DEBUG_H

#include "utils/wpabuf.h"
/* Debugging function - conditional DiagPrintf and hex dump. Driver wrappers can
 * use these for debugging purposes. */

enum {
	MSG_EXCESSIVE, MSG_MSGDUMP, MSG_DEBUG, MSG_INFO, MSG_ALWAYS, MSG_WARNING, MSG_ERROR
};


#define wpa_debug_print_timestamp() do { } while (0)
#define wpa_hexdump(l,t,b,le) do { } while (0)
#define wpa_hexdump_buf_key(l,t,b) do { } while (0)
#define wpa_hexdump_ascii_key(l,t,b,le) do { } while (0)
#define wpa_debug_open_file(p) do { } while (0)
#define wpa_debug_close_file() do { } while (0)
#define wpa_dbg(args...) do { } while (0)
#define wpa_msg_ctrl(args...) do { } while (0)
#define wpa_msg_register_cb(f) do { } while (0)
#define wpa_msg_register_ifname_cb(f) do { } while (0)
static inline int wpa_debug_reopen_file(void)
{
	return 0;
}

#ifdef CONFIG_NO_STDOUT_DEBUG
#define wpa_printf(args...) do { } while (0)
#define wpa_hexdump_buf(l,t,b) do { } while (0)
#define wpa_hexdump_key(l,t,b,le) do { } while (0)
#define wpa_hexdump_ascii(l,t,b,le) do { } while (0)
#define wpa_msg(args...) do { } while (0)
#define p2p_dbg(args...) do { } while (0)
#define p2p_info(args...) do { } while (0)
#define p2p_err(args...) do { } while (0)
#else
#define wpa_printf(level, fmt, arg...)     \
	do {\
		if (level >= MSG_INFO) {\
			{\
				RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, fmt, ##arg);\
				RTK_LOGS(NOTAG, RTK_LOG_ALWAYS,"\n\r");\
			} \
		}\
	}while(0)
#define wpa_msg(ctx,level,fmt,arg...) wpa_printf((level),(fmt), ##arg)
#define p2p_dbg(ctx, fmt, arg...) wpa_printf(MSG_DEBUG, (fmt), ##arg)
#define p2p_info(ctx, fmt, arg...) wpa_printf(MSG_INFO, (fmt), ##arg)
#define p2p_err(ctx, fmt, arg...) wpa_printf(MSG_ERROR, (fmt), ##arg)

void wpa_hexdump_key(int level, const char *title, const void *buf, size_t len);
void wpa_hexdump_buf(int level, const char *title,
					 const struct wpabuf *buf);
void wpa_hexdump_ascii(int level, const char *title, const void *buf,
					   size_t len);

#ifdef EAPOL_TEST
#define WPA_ASSERT(a)						       \
	do {							       \
		if (!(a)) {					       \
			DiagPrintf("WPA_ASSERT FAILED '" #a "' "	       \
			       "%s %s:%d\n",			       \
			       __FUNCTION__, __FILE__, __LINE__);      \
			exit(1);				       \
		}						       \
	} while (0)
#else
#define WPA_ASSERT(a) do { } while (0)
#endif
#endif //CONFIG_NO_STDOUT_DEBUG

#endif /* WPA_DEBUG_H */
