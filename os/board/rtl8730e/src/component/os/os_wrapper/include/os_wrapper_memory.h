/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OS_WRAPPER_MEMORY_H__
#define __OS_WRAPPER_MEMORY_H__


/**
 * @brief  Define memory alignment settings. This macro will be checked during compilation to ensure
 * that it is consistent with the configuration of the operating system.
 */
#ifdef CONFIG_AMEBASMART
#if (defined ARM_CORE_CM4) || (defined ARM_CORE_CA32)
#define RTOS_MEM_BYTE_ALIGNMENT 64
#else
#define RTOS_MEM_BYTE_ALIGNMENT 32
#endif
#else
#define RTOS_MEM_BYTE_ALIGNMENT 32
#endif

/**
 * @brief  Allocate memory from the heap. For FreeRTOS, map to pvPortMalloc
 *         The buffer value is random
 * @note   The return buffer size/address is cacheline size aligned
 * @param  size: buffer size in byte
 * @retval
 */
void *rtos_mem_malloc(uint32_t size);

/**
 * @brief  Allocate memory from the heap. For FreeRTOS, map to pvPortMalloc
 *         The buffer value is zero
 * @note   The return buffer size/address is cacheline size aligned
 * @param  size: buffer size in byte
 * @retval
 */
void *rtos_mem_zmalloc(uint32_t size);

/**
 * @brief  Allocate memory from the heap. For FreeRTOS, map to pvPortMalloc
 *         The buffer value is zero
 * @note   The return buffer size/address is cacheline size aligned
 * @param  elementNum:  memory size is elementNum*elementSize
 * @param  elementSize:
 * @retval
 */
void *rtos_mem_calloc(uint32_t elementNum, uint32_t elementSize);

/**
 * @brief  Reuse or extend memory previously allocated by the malloc(), calloc(), and realloc() functions
 * @note   The return buffer size/address is cacheline size aligned
 * @param  pbuf: Pointer containing the address
 * @param  size: The number of bytes of memory to be newly allocated.
 * @retval
 */
void *rtos_mem_realloc(void *pbuf, uint32_t size);

/**
 * @brief  For FreeRTOS, map to vPortFree
 * @param  pbuf:
 * @param  size: Optional parameters, the default value is 0. This parameter currently has no effect.
 */
void rtos_mem_free(void *pbuf);

/**
 * @brief  Get free heap size. For FreeRTOS, map to xPortGetFreeHeapSize
 * @retval Free heap size in byte
 */
uint32_t rtos_mem_get_free_heap_size(void);

/**
 * @brief  Get minimum ever free heap size. For FreeRTOS, map to xPortGetMinimumEverFreeHeapSize
 * @retval Minimum ever free heap size in byte
 */
uint32_t rtos_mem_get_minimum_ever_free_heap_size(void);

#endif
