/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "ameba.h"
#include "os_wrapper.h"

/* Static implementation only in FreeRTOS */
int rtos_mutex_create_static(rtos_mutex_t *pp_handle)
{
	return rtos_mutex_create(pp_handle);
}

int rtos_mutex_delete_static(rtos_mutex_t p_handle)
{
	return rtos_mutex_delete(p_handle);
}

int rtos_mutex_recursive_create_static(rtos_mutex_t *pp_handle)
{
	return rtos_mutex_recursive_create(pp_handle);
}

int rtos_mutex_recursive_delete_static(rtos_mutex_t p_handle)
{
	return rtos_mutex_delete_static(p_handle);
}

int rtos_mutex_create(rtos_mutex_t *pp_handle)
{
	sem_t *sem;

	if (!pp_handle) {
		dbg("pp_handle is NULL\n");
		return FAIL;
	}

	sem = (sem_t *)kmm_zalloc(sizeof(sem_t));
	if (sem == NULL) {
		dbg("alloc sem_t fail\n");
		return FAIL;
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	DEBUG_SET_CALLER_ADDR(sem);
#endif

	if (sem_init(sem, 0, 1) != OK) {
		kmm_free((void *)sem);
		dbg("sem init fail\n");
		return FAIL;
	}

	*pp_handle = sem;

	return SUCCESS;
}

int rtos_mutex_delete(rtos_mutex_t p_handle)
{
	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	if (sem_destroy((sem_t *)p_handle) != OK) {
		dbg("sema destroy fail\n");
		return FAIL;
	}

	kmm_free((void *)p_handle);

	return SUCCESS;
}

int rtos_mutex_take(rtos_mutex_t p_handle, uint32_t wait_ms)
{
	if (p_handle == NULL) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	if (wait_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (sem_timedwait((sem_t *) p_handle, &ts) != OK) {
			dbg("sema wait 0x%x ms fail\n", wait_ms);
			return FAIL;
		}
	} else {
		if (sem_wait((sem_t *) p_handle) != OK) {
			dbg("sema wait fail\n");
			return FAIL;
		}
	}

	return SUCCESS;
}

int rtos_mutex_give(rtos_mutex_t p_handle)
{
	if (!p_handle) {
		dbg("pp_handle is NULL\n");
		return FAIL;
	}

	if (sem_post((sem_t *) p_handle) != OK) {
		dbg("sema post fail\n");
		return FAIL;
	}

	return SUCCESS;
}

int rtos_mutex_recursive_create(rtos_mutex_t *pp_handle)
{
	return rtos_mutex_create(pp_handle);
}

int rtos_mutex_recursive_delete(rtos_mutex_t p_handle)
{
	return rtos_mutex_delete(p_handle);
}

int rtos_mutex_recursive_take(rtos_mutex_t p_handle, uint32_t wait_ms)
{
	return rtos_mutex_take(p_handle, wait_ms);
}

int rtos_mutex_recursive_give(rtos_mutex_t p_handle)
{
	return rtos_mutex_give(p_handle);
}
