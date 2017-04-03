/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * arch/arm/src/s5j/s5j_pwm.h
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * The Samsung sample code has a BSD compatible license that requires this
 * copyright notice:
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifndef	__S5J_PWM_H_
#define	__S5J_PWM_H_

/****************************************************************************
 * Pre-proessor Definitions
 ****************************************************************************/
#define __SYSCALL_USER_DEF			(0x10000000)

/* Start user definition ioctl request for timer */
#define __TIMER_USER_DEF			(0x10000000)
//#define PWM_WAIT_INT              (__TIMER_USER_DEF + 0x1)
#define PWM_SET_PRESCALER			(__TIMER_USER_DEF + 0x2)
#define PWM_SET_DIVIDER				(__TIMER_USER_DEF + 0x3)
#define PWM_SET_DEADZONE			(__TIMER_USER_DEF + 0x4)
#define PWM_SET_TCMP				(__TIMER_USER_DEF + 0x5)
//#define PWM_SET_TIME_ORDER            (__TIMER_USER_DEF + 0x6)
#define PWM_SET_DUTY_CYCLE			(__TIMER_USER_DEF + 0x7)

#define PWM_START_ENABLE_TOUT			(__TIMER_USER_DEF + 0x10)
#define PWM_START_ENABLE_ONETIME			(__TIMER_USER_DEF + 0x11)

#define PWM_PCLK    (26000000)

#define TREG_INVAL      0xFFFF

#define TCFG0           0x0000
#define TCFG1           0x0004
#define TCON            0x0008

#define TCNTB(x)        (0x000C + x * 0xC)
#define TCMPB(x)        (x < 4 ? (0x0010 + x * 0xC) : TREG_INVAL)
#define TCNTO(x)        (x < 4 ? (0x0014 + x * 0xC) : 0x0040)

#define TINT_CSTAT      0x0044

#define TIMER_PREDIVID  256

#define TIME_NS     1
#define TIME_MS     0

#define TIMER_SRC_PCLK  0
#define TIMER_SRC_SCLK  1

#define MAX_NO_PWM      4
#define TOTAL_NUMBER_OF_TIMER      8
#define TOTAL_NUMBER_OF_PWMOUT      6

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* sfr offset */
struct s5j_pwm_intdata {
	int pwm_no;
	int64_t int_count;
	int32_t reload;
	int32_t manual_update;
	int32_t start;
	int32_t invert;
	int32_t prescaler;
	int32_t div_mux;
};

/* This structure represents the state of one PWM timer */
struct s5j_pwmtimer_s {
	FAR const struct pwm_ops_s *ops;	/* PWM operations */

	int id;
	void *base;
	int irq;

	/* Add private data structures if it is needed */
	unsigned long long timer_pclk;		/* input clock Hz */
	int pre_scaler;
	int divider;
	int freq;
	int dutycycle;				//0~100
	void *callback;

	struct s5j_pwm_intdata *intdata;
};

static int s5j_pwm_interrupt(struct s5j_pwmtimer_s *priv);
static int s5j_pwm_timer_interrupt(int irq, void *context, void *arg);
static int s5j_pwm_setup(FAR struct pwm_lowerhalf_s *dev);
static int s5j_pwm_shutdown(FAR struct pwm_lowerhalf_s *dev);
#ifdef CONFIG_PWM_PULSECOUNT
static int s5j_pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info, FAR void *handle);
#else
static int s5j_pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info);
#endif
static int s5j_pwm_stop(FAR struct pwm_lowerhalf_s *dev);
static int s5j_pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd, unsigned long arg);
static int s5j_pwm_set_frequency_dutycycle(FAR struct pwm_lowerhalf_s *dev, int32_t frequency, int32_t duty);
FAR struct pwm_lowerhalf_s *s5j_pwminitialize(int timer);

#endif
