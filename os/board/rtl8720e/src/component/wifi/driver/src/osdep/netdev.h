/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __NETDEV_H_
#define __NETDEV_H_

/* Define compilor specific symbol */
//
// inline function
//

#if defined ( __ICCARM__ )
#define __inline__                      inline
#define __inline                        inline
#define __inline_definition			//In dialect C99, inline means that a function's definition is provided 
//only for inlining, and that there is another definition
//(without inline) somewhere else in the program.
//That means that this program is incomplete, because if
//add isn't inlined (for example, when compiling without optimization),
//then main will have an unresolved reference to that other definition.

// Do not inline function is the function body is defined .c file and this
// function will be called somewhere else, otherwise there is compile error
#elif defined ( __CC_ARM   )
#define __inline__			__inline	//__linine__ is not supported in keil compilor, use __inline instead
#define inline				__inline
#define __inline_definition			// for dialect C99
#elif defined   (  __GNUC__  )
#define __inline__                      inline
#define __inline                        inline
#define __inline_definition	inline
#endif

#include <stdio.h>
#include <drv_conf.h>
#include <osdep_service.h>


// rtl8195a uses receive_tasklet for wps
// 8189em uses interrupt_thread for wps
#if defined(CONFIG_WPS)
#define RECV_STACK_FOR_WPS 448//512//384 //Change to 512 for WPS (IAR STM32) stack overflow
#else
#define RECV_STACK_FOR_WPS 0
#endif

#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBADPLUS)
#define LITTLE_STACKSIZE		256
#else
#define LITTLE_STACKSIZE		(512 + 256 + 128 + RECV_STACK_FOR_WPS)
#endif // defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBALITE)


#define RECV_TASKLET_STACKSIZE	(1024 + RECV_STACK_FOR_WPS)

#define DRIVER_RESUME_TASK_STACKSIZE 256

#define rtw_netdev_priv(netdev) (netdev_priv(netdev))

#define ADPT_FMT "wlan%d"
#define ADPT_ARG(adapter) adapter->iface_type
#define FUNC_NDEV_FMT "%s"
#define FUNC_NDEV_ARG(ndev) __func__
#define FUNC_ADPT_FMT "%s(%s)"
#define FUNC_ADPT_ARG(adapter) __func__, adapter->iface_type

#include "wifi_constants.h"
#include "wifi_structures.h"
int rtw_if_wifi_thread(char *name);
#endif	//#ifndef __OSDEP_SERVICE_H_
