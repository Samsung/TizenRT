/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

/* Include header for DVFS implementation */
#include <tinyara/pm/pm.h>
#include <modules/pm.h>

#ifdef CONFIG_PM_DVFS
void up_set_dvfs(int div_lvl)
{
	/*
	PM_CPU_FRQ_XTAL = 0, // 0:CPU:XTAL,BUS:XTAL(XTAL:40M)
	PM_CPU_FRQ_60M,      // 1:CPU0:60M,BUS:60M
	PM_CPU_FRQ_80M,      // 2:CPU0:80M,BUS:80M
	PM_CPU_FRQ_120M,     // 3:CPU0:120M,BUS:120M
	PM_CPU_FRQ_160M,     // 4:CPU0:160M,BUS:160M
	PM_CPU_FRQ_240M,     // 5:CPU0:240M,BUS:240M
	*/

	pm_cpu_freq_e cpu_freq = PM_CPU_FRQ_XTAL;
	switch (div_lvl)
	{
	case 0:
		cpu_freq = PM_CPU_FRQ_240M;
		break;
	case 1:
		cpu_freq = PM_CPU_FRQ_160M;
		break;
	case 2:
		cpu_freq = PM_CPU_FRQ_120M;
		break;
	case 3:
		cpu_freq = PM_CPU_FRQ_80M;
		break;
	case 4:
		cpu_freq = PM_CPU_FRQ_60M;
		break;
	case 5:
		cpu_freq = PM_CPU_FRQ_XTAL;
		break;
	default:
		break;
	}
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT, cpu_freq);
}
#endif
