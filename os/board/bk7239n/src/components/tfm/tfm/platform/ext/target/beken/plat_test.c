/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_retarget.h"
#include "timer_cmsdk.h"
#include "tfm_plat_test.h"
#include  "tfm_platform_system.h"
#include "psa/service.h"
#include "driver/timer.h"

void timer0_handler(void)
{
	while(1);
}

void tfm_plat_test_secure_timer_start(void)
{
	psa_invec in[1];
	int32_t err, timer_id = TIMER_ID0;

	err = tfm_platform_ioctl(PLT_OP_TIMER_DRV_INIT, NULL, NULL);/*bk_timer_driver_init();*/

	in[0].base = &timer_id;
	in[0].len = sizeof(timer_id);
	err = tfm_platform_ioctl(PLT_OP_START_TIMER, in, NULL);/*bk_timer_start(TIMER_ID0, 500, (timer_isr_t)NULL);*/

	(void)err;
}

void tfm_plat_test_secure_timer_clear_intr(void) 
{
	int32_t err;

#if CONFIG_CLEAR_IRQ_VIA_PLATFORM_IDF
	err = tfm_platform_ioctl(PLT_OP_CLEAR_TIMER_ISR_STATUS, NULL, NULL);/*timer_clear_isr_status();*/
#else
	/*FIXME*/
	uint32_t val;

	val = *((volatile uint32_t *)(0x44810000 + 7 * 4));
	do{
		*((volatile uint32_t *)(0x44810000 + 7 * 4)) = val;
	}while(val == *((volatile uint32_t *)(0x44810000 + 7 * 4)));
#endif

	(void)err;
}

void tfm_plat_test_secure_timer_stop(void)
{
	psa_invec in[1];
	int32_t err, timer_id = TIMER_ID0;

	in[0].base = &timer_id;
	in[0].len = sizeof(timer_id);
	err = tfm_platform_ioctl(PLT_OP_STOP_TIMER, in, NULL);/*bk_timer_stop(TIMER_ID2, 500, (timer_isr_t)NULL);*/

	(void)err;
}

void tfm_plat_test_non_secure_timer_start(void)
{
	psa_invec in[1];
	int32_t err, timer_id = TIMER_ID2;

	err = tfm_platform_ioctl(PLT_OP_TIMER_DRV_INIT, NULL, NULL);/*bk_timer_driver_init();*/

	in[0].base = &timer_id;
	in[0].len = sizeof(timer_id);
	err = tfm_platform_ioctl(PLT_OP_START_TIMER, in, NULL);/*bk_timer_start(TIMER_ID2, 500, (timer_isr_t)NULL);*/

	(void)err;
}

void tfm_plat_test_non_secure_timer_stop(void)
{
	psa_invec in[1];
	int32_t err, timer_id = TIMER_ID2;

	in[0].base = &timer_id;
	in[0].len = sizeof(timer_id);
	err = tfm_platform_ioctl(PLT_OP_STOP_TIMER, in, NULL);/*bk_timer_stop(TIMER_ID0, 500, (timer_isr_t)NULL);*/

	(void)err;
}
