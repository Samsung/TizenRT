/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2016-2017 Gregory Nutt. All rights reserved.
 *   Author: Alan Carvalho de Assis <acassis@gmail.com>
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
 ****************************************************************************/

#ifndef __INCLUDE_TINYARA_AUDIO_TONE_H
#define __INCLUDE_TINYARA_AUDIO_TONE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <fixedmath.h>

#include <tinyara/drivers/pwm.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_AUDIO_TONE

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: tone_register
 *
 * Description:
 *   This function binds an instance of a "lower half" PWM driver with
 *   the "upper half" Audio Tone device and registers that device so that can
 *   be used by application code.
 *
 *
 * Input parameters:
 *   path - The full path to the driver to be registers in the Tinyara pseudo-
 *     filesystem.  The recommended convention is to name all PWM drivers
 *     as "/dev/tone0", "/dev/tone1", etc.  where the driver path
 *     differs only in the "minor" number at the end of the device name.
 *   channel - The the PWM peripheral supports multiple output channels, then
 *     this value must be provided to indicate the output channel that drives
 *     the tone.
 *   tone - A pointer to an instance of lower half PWM driver tone. This
 *     instance will be bound to the Audio Tone driver and must persists as
 *     long as that driver persists.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int tone_register(FAR const char *path, FAR struct pwm_lowerhalf_s *tone,
#ifdef CONFIG_PWM_MULTICHAN
				  int channel,
#endif
				  FAR struct oneshot_lowerhalf_s *oneshot);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif	/* CONFIG_AUDIO_TONE */
#endif	/* __INCLUDE_TINYARA_AUDIO_TONE_H */
