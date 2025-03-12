/**
  ******************************************************************************
  * @file    rtw_atomic.c
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

#include <os_wrapper.h>
#include <stdio.h>
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include "ameba_tizenrt_pmu.h"
#else
#include <ameba_pmu.h>
#endif //CONFIG_PLATFORM_TIZENRT_OS
#include <rtw_atomic.h>

void ATOMIC_SET(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_store(v, i);
#else
	atomic_set(v, i);
#endif
}

int ATOMIC_READ(ATOMIC_T *v)
{
#if defined(STDATOMIC)
	return atomic_load(v);
#else
	return atomic_read(v);
#endif
}

void ATOMIC_ADD(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_fetch_add(v, i);
#else

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	irqstate_t flags = tizenrt_critical_enter();
#else
	rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	v->counter += i;

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	tizenrt_critical_exit(flags);
#else
	rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

#endif
}

void ATOMIC_SUB(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_fetch_sub(v, i);
#else

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	irqstate_t flags = tizenrt_critical_enter();
#else
	rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	v->counter -= i;

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	tizenrt_critical_exit(flags);
#else
	rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

#endif
}

void ATOMIC_INC(ATOMIC_T *v)
{
	ATOMIC_ADD(v, 1);
}

void ATOMIC_DEC(ATOMIC_T *v)
{
	ATOMIC_SUB(v, 1);
}

int ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_fetch_sub(v, i);
	return atomic_load(v);
#else
	int temp;

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	irqstate_t flags = tizenrt_critical_enter();
#else
	rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	temp = v->counter;
	temp -= i;
	v->counter = temp;

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	tizenrt_critical_exit(flags);
#else
	rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	return temp;
#endif
}

int ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	return ATOMIC_SUB_RETURN(v, 1);
}

int ATOMIC_DEC_AND_TEST(ATOMIC_T *v)
{
	return ATOMIC_DEC_RETURN(v) == 0;
}


