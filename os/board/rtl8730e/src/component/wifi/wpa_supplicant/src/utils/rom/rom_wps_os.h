/*
 * OS specific functions
 * Copyright (c) 2005-2009, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef ROM_WPS_OS_H
#define ROM_WPS_OS_H

#include "rtw_autoconf.h"

#if defined(CONFIG_AMEBASMART) || defined(CONFIG_AMEBADPLUS)
#ifndef CONFIG_WHC_HOST
#include <wifi_intf_ram_to_rom.h>
#define os_malloc(sz) p_wifi_rom_func_map->zmalloc(sz, 1)
#define os_free(p, sz) p_wifi_rom_func_map->mfree(p)
#else
#include <os_wrapper.h>
#define os_malloc(sz) rtos_mem_malloc(sz)
#define os_free(p, sz) rtos_mem_free(((uint8_t*)(p)))
#endif
#endif

extern uint8_t *WPS_realloc(uint8_t *old_buf, uint32_t old_sz, uint32_t new_sz);
#define os_realloc(p, os, ns) WPS_realloc(((uint8_t*)(p)),(os),(ns))

#endif /* ROM_WPS_OS_H */
