/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __PLATFORM_STDLIB_H__
#define __PLATFORM_STDLIB_H__





#if defined(CONFIG_PLATFORM_8711B)+\
		defined(CONFIG_PLATFORM_8721D)+\
		defined(CONFIG_PLATFORM_8195BHP)+\
		defined(CONFIG_PLATFORM_8735B)+\
		defined(USE_STM322xG_EVAL)+\
		defined(USE_STM324xG_EVAL)+\
		defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)+\
		defined(STM32F10X_XL) > 1
#error "Cannot define two or more platform at one time"
#endif


#if defined (CONFIG_PLATFORM_8711B)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_rtl8711b.h"
#ifdef __cplusplus
}
#endif
#elif defined (CONFIG_PLATFORM_8721D)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_rtl8721d.h"
#ifdef __cplusplus
}
#endif
#elif defined (CONFIG_PLATFORM_AMEBAD2)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_amebad2.h"
#ifdef __cplusplus
}
#endif

#elif defined (CONFIG_PLATFORM_AMEBALITE)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_amebalite.h"
#ifdef __cplusplus
}
#endif
#elif defined (CONFIG_PLATFORM_AMEBADPLUS)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_amebadplus.h"
#ifdef __cplusplus
}
#endif
#elif defined(CONFIG_PLATFORM_8195BHP)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_rtl8195bhp.h"
#ifdef __cplusplus
}
#endif
#elif defined(CONFIG_PLATFORM_8735B)
#include "platform_stdlib_rtl8735b.h"
#elif defined(USE_STM322xG_EVAL) || defined(USE_STM324xG_EVAL)  || defined(STM32F10X_XL)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_stm32.h"
#ifdef __cplusplus
}
#endif
#elif defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_customer.h"
#ifdef __cplusplus
}
#endif
#elif defined (CONFIG_PLATFORM_8710C)
#ifdef __cplusplus
extern "C" {
#endif
#include "platform_stdlib_rtl8710c.h"
#ifdef __cplusplus
}
#endif
#else
#error "Undefined Platform stdlib"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_PLATFORM_AMEBA_X) && (CONFIG_PLATFORM_AMEBA_X == 0)
#ifndef CONFIG_PLATFOMR_CUSTOMER_RTOS
#include "basic_types.h"
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif //__PLATFORM_STDLIB_H__

