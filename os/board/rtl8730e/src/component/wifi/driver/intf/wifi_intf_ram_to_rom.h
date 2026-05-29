/**
  ******************************************************************************
  * @file    wifi_intf_ram_to_rom.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef ROM_FUNC_MAP_H
#define ROM_FUNC_MAP_H

#include "basic_types.h"

struct _wifi_rom_func_map {
	/* OS interface */
	void *(*zmalloc)(unsigned int, unsigned int);
	void (*mfree)(void *);

	int (*random)(void *, unsigned char *, size_t);
};

extern struct _wifi_rom_func_map wifi_rom_func_map;
extern struct _wifi_rom_func_map *p_wifi_rom_func_map;

#endif	/* ROM_FUNC_MAP_H */
