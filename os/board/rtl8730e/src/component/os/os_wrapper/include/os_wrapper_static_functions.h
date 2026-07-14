/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OS_WRAPPER_STATIC_FUNCTIONS_H__
#define __OS_WRAPPER_STATIC_FUNCTIONS_H__

struct component_status {
	unsigned int mutex_total_num;
	unsigned int mutex_buf_used_num;
	unsigned int mutex_max_buf_used_num;
	unsigned int mutex_dynamic_num;
	unsigned int mutex_poolsize_static;

	unsigned int sema_total_num;
	unsigned int sema_buf_used_num;
	unsigned int sema_max_buf_used_num;
	unsigned int sema_dynamic_num;
	unsigned int sema_poolsize_static;

	unsigned int timer_total_num;
	unsigned int timer_buf_used_num;
	unsigned int timer_max_buf_used_num;
	unsigned int timer_dynamic_num;
	unsigned int timer_poolsize_static;
};

void rtos_static_get_component_status(struct component_status *comp_status);

#endif
