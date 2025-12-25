/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/**
 * @file
 * Hardware checksum port implementation for lwIP
 * 
 * This file implements the registration mechanism for vendor-specific
 * hardware checksum functions and provides the default checksum function
 * that can be used by LWIP_CHKSUM macro.
 */

#include "lwip/opt.h"
#include "lwip/port/hw_chksum_port.h"
#include "lwip/inet_chksum.h"
#include "lwip/def.h"
#include <string.h>

/* Registered hardware checksum function (global registration only) */
static lwip_hw_chksum_func_t hw_chksum_func = NULL;

/* Default checksum algorithm (from lwipopts.h) */
#ifndef LWIP_CHKSUM_ALGORITHM
#define LWIP_CHKSUM_ALGORITHM 2
#endif

/* Forward declaration */
u16_t lwip_standard_chksum(const void *dataptr, int len);

/**
 * Get hardware checksum function
 * 
 * @return: function pointer or NULL if not registered
 */
lwip_hw_chksum_func_t lwip_get_hw_chksum_func(void)
{
	return hw_chksum_func;
}

/**
 * Register hardware checksum function
 * 
 * @param chksum_func: hardware checksum function pointer
 * @return: 0 on success, -1 on failure
 */
int lwip_register_hw_chksum(lwip_hw_chksum_func_t chksum_func)
{
	if (chksum_func == NULL) {
		return -1;
	}
	
	hw_chksum_func = chksum_func;
	return 0;
}

/**
 * Unregister hardware checksum function
 * 
 * @return: 0 on success, -1 on failure
 */
int lwip_unregister_hw_chksum(void)
{
	if (hw_chksum_func == NULL) {
		return -1;
	}
	
	hw_chksum_func = NULL;
	return 0;
}

/**
 * lwIP checksum function: uses software implementation by default,
 * and automatically uses hardware checksum if registered by vendor.
 * This function is used by LWIP_CHKSUM macro when the port layer is enabled.
 *
 * @param dataptr: data pointer
 * @param len: data length
 * @return: checksum value
 */
u16_t lwip_chksum(const void *dataptr, int len)
{
	if (hw_chksum_func != NULL) {
		/* Use hardware checksum */
		return hw_chksum_func(dataptr, len);
	}
	
	/* Fall back to software checksum */
	return lwip_standard_chksum(dataptr, len);
}
