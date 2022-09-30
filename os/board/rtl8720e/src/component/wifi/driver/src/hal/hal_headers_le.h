#ifndef __HAL_HEADERS_LE_H__
#define __HAL_HEADERS_LE_H__

/*this file is shared for 11ax submodules*/
#include <drv_types.h>
#include "hal_data.h"

/*==================phl_types.h of g6 driver=====================*/
#define _os_dbgdump printf
typedef _timer _os_timer;

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/*==================pltfm_ops_linux.h of g6 driver==================*/
static inline void _os_init_timer(void *d, _os_timer *timer,
								  void (*call_back_func)(void *context), void *context,
								  const char *sz_id)
{
	(void) d;
	rtw_init_timer(timer, NULL, call_back_func, context, sz_id);
}

static inline void _os_set_timer(void *d, _os_timer *timer, u32 ms_delay)
{
	(void) d;
	rtw_set_timer(timer, ms_delay);
}

static inline void _os_cancel_timer(void *d, _os_timer *timer)
{
	(void) d;
	rtw_cancel_timer(timer);
}

static inline void _os_release_timer(void *d, _os_timer *timer)
{
	(void) d;
	rtw_del_timer(timer);
}

#define _os_mem_alloc(d, sz)  rtw_zmalloc(sz)
#define _os_mem_free(d, buf, sz)  rtw_mfree(buf, sz)
#define _os_mem_set(d, buf, val, sz)  rtw_memset(buf, val, sz)
#define _os_mem_cpy(d, dst, src, sz)  rtw_memcpy(dst, src, sz)
#define _os_mem_cmp(d, dst, src, sz)  rtw_memcmp(dst, src, sz)

#define hal_mem_set(hal, buf, val, sz) rtw_memset(buf, val, sz)
#define hal_mem_cpy(hal, dst, src, sz) rtw_memcpy(dst, src, sz)

#define _os_delay_ms(d, ms)  rtw_mdelay_os(ms)
#define _os_delay_us(d, us)  rtw_udelay_os(us)

#define _os_get_cur_time_us() rtw_get_current_time() //AX_TODO: need find a us timer

#define _os_snprintf(s, sz, fmt, ...) snprintf(s, sz, fmt, ##__VA_ARGS__)

#define _os_strcmp(s1, s2) strcmp(s1, s2)
#define _os_strcpy(dest, src) strcpy(dest, src)
#define _os_sscanf(buf, fmt, ...) sscanf(buf, fmt, ##__VA_ARGS__)
#define _os_strsep(s, ct) strsep(s, ct)
#define _os_strlen(buf) strlen(buf)

#define hal_read32(hal, addr) HAL_READ32(WIFI_REG_BASE, addr)
#define hal_read16(hal, addr) HAL_READ16(WIFI_REG_BASE, addr)
#define hal_read8(hal, addr) HAL_READ8(WIFI_REG_BASE, addr)
#define hal_write32(hal, addr, val) HAL_WRITE32(WIFI_REG_BASE, addr, val)
#define hal_write16(hal, addr, val) HAL_WRITE16(WIFI_REG_BASE, addr, val)
#define hal_write8(hal, addr, val) HAL_WRITE8(WIFI_REG_BASE, addr, val)

#define phl_is_mp_mode(_phl_com)	(_phl_com->drv_mode == RTW_DRV_MODE_MP)

/*==================hal_headers_le.h of g6 driver==================*/
static inline void *hal_mem_alloc(struct rtw_hal_com_t *h, u32 buf_sz)
{
	(void)h;
	return rtw_zmalloc(buf_sz);
}

static inline void hal_mem_free(struct rtw_hal_com_t *h, void *buf, u32 buf_sz)
{
	(void)h;
	rtw_mfree(buf, buf_sz);
}

#endif