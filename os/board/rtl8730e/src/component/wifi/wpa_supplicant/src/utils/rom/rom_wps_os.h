/*
 * OS specific functions
 * Copyright (c) 2005-2009, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef ROM_WPS_OS_H
#define ROM_WPS_OS_H

#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBADPLUS)
#ifndef CONFIG_AS_INIC_AP
#include <wifi_intf_ram_to_rom.h>
#define os_malloc(sz) p_wifi_rom_func_map->zmalloc(sz, 1)
#define os_free(p, sz) p_wifi_rom_func_map->mfree(p)
#else
#include <osdep_service.h>
#define os_malloc(sz) rtw_malloc(sz)
#define os_free(p, sz) rtw_mfree(((uint8_t*)(p)), (sz))
#endif
#endif

extern uint8_t *WPS_realloc(uint8_t *old_buf, uint32_t old_sz, uint32_t new_sz);
#define os_realloc(p, os, ns) WPS_realloc(((uint8_t*)(p)),(os),(ns))

#endif /* ROM_WPS_OS_H */
