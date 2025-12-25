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
 * Hardware checksum port interface for lwIP
 * 
 * This file provides registration interface for vendor-specific hardware
 * checksum implementations.
 */

#ifndef LWIP_HDR_PORT_HW_CHKSUM_H
#define LWIP_HDR_PORT_HW_CHKSUM_H

#include "lwip/opt.h"
#include <tinyara/config.h>

#ifdef CONFIG_NET_LWIP_HW_CHKSUM_PORT

#include "lwip/arch.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Hardware checksum function type
 * @param dataptr points to start of data to be summed
 * @param len length of data to be summed
 * @return checksum value (host order, non-inverted Internet sum)
 */
typedef uint16_t (*lwip_hw_chksum_func_t)(const void *dataptr, int len);

/**
 * Register a hardware checksum function
 * 
 * @param chksum_func hardware checksum function pointer
 * @return 0 on success, -1 on failure
 */
int lwip_register_hw_chksum(lwip_hw_chksum_func_t chksum_func);

/**
 * Unregister a hardware checksum function
 * 
 * @return 0 on success, -1 on failure
 */
int lwip_unregister_hw_chksum(void);

/**
 * Get hardware checksum function (internal use)
 * If no hardware function is registered, returns NULL and software
 * implementation will be used.
 * 
 * @return hardware checksum function pointer, or NULL if not registered
 */
lwip_hw_chksum_func_t lwip_get_hw_chksum_func(void);

/**
 * lwIP checksum function: uses software implementation by default,
 * and automatically uses hardware checksum if registered by vendor.
 */
u16_t lwip_chksum(const void *dataptr, int len);

/* Define LWIP_CHKSUM if not already defined */
#ifndef LWIP_CHKSUM
#define LWIP_CHKSUM lwip_chksum
#ifndef LWIP_CHKSUM_ALGORITHM
#define LWIP_CHKSUM_ALGORITHM 2
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_NET_LWIP_HW_CHKSUM_PORT */

#endif /* LWIP_HDR_PORT_HW_CHKSUM_H */
