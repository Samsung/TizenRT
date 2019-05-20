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
 * include/debug.h
 *
 *   Copyright (C) 2007-2011, 2014 Gregory Nutt. All rights reserved.
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
/**
 * @defgroup DEBUG_KERNEL DEBUG
 * @brief Provides APIs for Debug
 * @ingroup KERNEL
 *
 * @{
 */

/// @file debug.h
/// @brief Debug APIs

#ifndef __INCLUDE_DEBUG_H
#define __INCLUDE_DEBUG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>
#include <tinyara/logm.h>

#include <syslog.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Debug macros to runtime filter the debug messages sent to the console.  In
 * general, there are four forms of the debug macros:
 *
 * [a-z]dbg() -- Outputs messages to the console similar to printf() except
 *    that the output is not buffered.  The first character indicates the
 *    system system (e.g., n=network, f=filesystem, etc.).  If the first
 *    character is missing (i.e., dbg()), then it is common.  The common
 *    dbg() macro is enabled by CONFIG_DEBUG.  Subsystem debug requires an
 *    additional configuration setting to enable it (e.g., CONFIG_DEBUG_NET
 *    for the network, CONFIG_DEBUG_FS for the file system, etc).
 *
 *    In general, error messages and output of importance use [a-z]dbg().
 *    [a-z]dbg() is implementation dependent but usually uses file descriptors.
 *    (that is a problem only because the interrupt task may have re-
 *    directed stdout).  Therefore [a-z]dbg() should not be used in interrupt
 *    handlers.
 *
 * [a-z]vdbg() -- Identical to [a-z]dbg() except that it also requires that
 *    CONFIG_DEBUG_VERBOSE be defined.  This is intended for general debug
 *    output that you would normally want to suppress.
 *
 * [a-z]lldbg() -- Identical to [a-z]dbg() except this is uses special
 *    interfaces provided by architecture-specific logic to talk directly
 *    to the underlying console hardware.  If the architecture provides such
 *    logic, it should define CONFIG_ARCH_LOWPUTC.
 *
 *    [a-z]lldbg() should not be used in normal code because the implementation
 *    probably disables interrupts and does things that are not consistent with
 *    good real-time performance.  However, [a-z]lldbg() is particularly useful
 *    in low-level code where it is inappropriate to use file descriptors.  For
 *    example, only [a-z]lldbg() should be used in interrupt handlers.
 *
 * [a-z]llvdbg() -- Identical to [a-z]lldbg() except that it also requires that
 *    CONFIG_DEBUG_VERBOSE be defined.  This is intended for general debug
 *    output that you would normally want to suppress.
 */

#ifdef CONFIG_HAVE_FUNCTIONNAME
#define EXTRA_FMT "%s: "
#define EXTRA_ARG , __FUNCTION__
#else
#define EXTRA_FMT
#define EXTRA_ARG
#endif

/* Debug macros will differ depending upon if the toolchain supports
 * macros with a variable number of arguments or not.
 */

#ifdef CONFIG_CPP_HAVE_VARARGS

/* C-99 style variadic macros are supported */

#ifdef CONFIG_DEBUG
/* Temporary LOGM macros to route all dbg messages.
 * Once LOGM is approved, each module should have its own index
 */
#define LOGM_IDX (0)

#ifdef CONFIG_DEBUG_ERROR
#ifdef CONFIG_LOGM
#define dbg(format, ...) \
	logm(LOGM_NORMAL, LOGM_IDX, LOGM_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#define dbg_noarg(format, ...) \
	logm(LOGM_NORMAL, LOGM_IDX, LOGM_ERR, format, ##__VA_ARGS__)

#define lldbg(format, ...) \
	logm(LOGM_LOWPUT, LOGM_IDX, LOGM_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#else
/**
 * @brief  Error debug
 * @details @b #include <debug.h>
 * @since TizenRT v1.0
 */
#define dbg(format, ...) \
	syslog(LOG_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#define dbg_noarg(format, ...) \
	syslog(LOG_ERR, format, ##__VA_ARGS__)

#ifdef CONFIG_ARCH_LOWPUTC
/**
 * @brief  Error debug for low-level
 * @details @b #include <debug.h>
 * @since TizenRT v1.0
 */
#define lldbg(format, ...) \
	lowsyslog(LOG_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define lldbg(...)
#endif
#endif

#else
#define dbg(...)
#define lldbg(...)
#endif

#ifdef CONFIG_DEBUG_WARN
#ifdef CONFIG_LOGM
#define wdbg(format, ...) \
	logm(LOGM_NORMAL, LOGM_IDX, LOGM_WRN, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#define llwdbg(format, ...) \
	logm(LOGM_LOWPUT, LOGM_IDX, LOGM_WRN, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#else
/**
 * @brief  Warning debug
 * @details @b #include <debug.h>
 * @since TizenRT v1.0
 */
#define wdbg(format, ...) \
	syslog(LOG_WARNING, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#ifdef CONFIG_ARCH_LOWPUTC
/**
 * @brief  Warning debug for low-level
 * @details @b #include <debug.h>
 * @since TizenRT v1.0
 */
#define llwdbg(format, ...) \
	lowsyslog(LOG_WARNING, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define llwdbg(...)
#endif
#endif

#else
#define wdbg(...)
#define llwdbg(...)
#endif

#ifdef CONFIG_DEBUG_VERBOSE
#ifdef CONFIG_LOGM
#define vdbg(format, ...) \
	logm(LOGM_NORMAL, LOGM_IDX, LOGM_INF, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#define llvdbg(format, ...) \
	logm(LOGM_LOWPUT, LOGM_IDX, LOGM_INF, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#else
/**
 * @brief  Informational(Verbose) debug
 * @details @b #include <debug.h>
 * @since TizenRT v1.0
 */
#define vdbg(format, ...) \
	syslog(LOG_INFO, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#ifdef CONFIG_ARCH_LOWPUTC
/**
 * @brief  Informational(Verbose) debug for low-level
 * @details @b #include <debug.h>
 * @since TizenRT v1.0
 */
#define llvdbg(format, ...) \
	lowsyslog(LOG_INFO, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#else
#define llvdbg(...)
#endif
#endif

#else
#define vdbg(...)
#define llvdbg(...)
#endif

#else							/* CONFIG_DEBUG */

#define dbg(...)
#define lldbg(...)
#define wdbg(...)
#define llwdbg(...)
#define vdbg(...)
#define llvdbg(...)

#endif							/* CONFIG_DEBUG */

/****************************************/
/*        Subsystem specific debug      */
/****************************************/

#ifdef CONFIG_DEBUG_AUDIO_ERROR
#define auddbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define audlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define auddbg(...)
#define audlldbg(...)
#endif

#ifdef CONFIG_DEBUG_AUDIO_WARN
#define audwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define audllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define audwdbg(...)
#define audllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_AUDIO_INFO
#define audvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define audllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define audvdbg(...)
#define audllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_ERR_REPORT_ERROR
#define nwerrdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define nwerrlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define nwerrdbg(...)
#define nwerrlldbg(...)
#endif

#ifdef CONFIG_DEBUG_ERR_REPORT_WARN
#define nwerr_wdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define nwerr_llwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define nwerr_wdbg(...)
#define nwerr_llwdbg(...)
#endif

#ifdef CONFIG_DEBUG_ERR_REPORT_INFO
#define nwerr_vdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define nwerr_llvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define nwerr_vdbg(...)
#define nwerr_llvdbg(...)
#endif

#ifdef CONFIG_DEBUG_FS_ERROR
#define fdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define flldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define fdbg(...)
#define flldbg(...)
#endif

#ifdef CONFIG_DEBUG_FS_WARN
#define fwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define fllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define fwdbg(...)
#define fllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_FS_INFO
#define fvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define fsdbg(format, ...)   dbg_noarg(format, ##__VA_ARGS__)
#define fllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define fvdbg(...)
#define fsdbg(format, ...)
#define fllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_LIB_ERROR
#define ldbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define llldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define ldbg(...)
#define llldbg(...)
#endif

#ifdef CONFIG_DEBUG_LIB_WARN
#define lwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define lllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define lwdbg(...)
#define lllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_LIB_INFO
#define lvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define lllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define lvdbg(...)
#define lllvdbg(...)
#endif

#ifdef CONFIG_NET_LWIP_DEBUG
#define lwipdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define lwiplldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define lwipdbg(...)
#define lwiplldbg(...)
#endif

#ifdef CONFIG_DEBUG_MM_ERROR
#define mdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define mlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define mdbg(...)
#define mlldbg(...)
#endif

#ifdef CONFIG_DEBUG_MM_WARN
#define mwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define mllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define mwdbg(...)
#define mllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_MM_INFO
#define mvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define mllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define mvdbg(...)
#define mllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_NET_ERROR
#define ndbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define nlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define ndbg(...)
#define nlldbg(...)
#endif

#ifdef CONFIG_DEBUG_NET_WARN
#define nwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define nllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define nwdbg(...)
#define nllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_NET_INFO
#define nvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define nllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define nvdbg(...)
#define nllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_SCHED_ERROR
#define sdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define slldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define sdbg(...)
#define slldbg(...)
#endif

#ifdef CONFIG_DEBUG_SCHED_WARN
#define swdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define sllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define swdbg(...)
#define sllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_SCHED_INFO
#define svdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define sllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define svdbg(...)
#define sllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_PM_ERROR
#define pmdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define pmlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define pmdbg(...)
#define pmlldbg(...)
#endif

#ifdef CONFIG_DEBUG_PM_WARN
#define pmwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define pmllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define pmwdbg(...)
#define pmllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_PM_INFO
#define pmvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define pmllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define pmvdbg(...)
#define pmllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_SECURE_ELEMENT_ERROR
#define sedbg(format, ...)     dbg(format, ##__VA_ARGS__)
#define selldbg(format, ...)   lldbg(format, ##__VA_ARGS__)
#else
#define sedbg(...)
#define selldbg(...)
#endif

#ifdef CONFIG_DEBUG_SECURE_ELEMENT_INFO
#define sevdbg(format, ...)     vdbg(format, ##__VA_ARGS__)
#define sellvdbg(format, ...)   llvdbg(format, ##__VA_ARGS__)
#else
#define sevdbg(...)
#define sellvdbg(...)
#endif

/****************************************/
/*        Framework specific debug      */
/****************************************/

#ifdef CONFIG_DEBUG_DM_ERROR
#define dmdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define dmlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define dmdbg(...)
#define dmlldbg(...)
#endif

#ifdef CONFIG_DEBUG_DM_WARN
#define dmwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define dmllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define dmwdbg(...)
#define dmllwdbg(...)
#endif

#ifdef  CONFIG_DEBUG_DM_INFO
#define dmvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define dmllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define dmvdbg(...)
#define dmllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_EVENTLOOP_ERROR
#define eldbg(format, ...)      dbg(format, ##__VA_ARGS__)
#define ellldbg(format, ...)    lldbg(format, ##__VA_ARGS__)
#else
#define eldbg(...)
#define ellldbg(...)
#endif

#ifdef CONFIG_DEBUG_EVENTLOOP_INFO
#define elvdbg(format, ...)     vdbg(format, ##__VA_ARGS__)
#define elllvdbg(format, ...)   llvdbg(format, ##__VA_ARGS__)
#else
#define elvdbg(...)
#define elllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_MEDIA_ERROR
#define meddbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define medlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define meddbg(...)
#define medlldbg(...)
#endif

#ifdef CONFIG_DEBUG_MEDIA_WARN
#define medwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define medllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define medwdbg(...)
#define medllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_MEDIA_INFO
#define medvdbg(format, ...)    vdbg(format, ##__VA_ARGS__)
#define medllvdbg(format, ...)  llvdbg(format, ##__VA_ARGS__)
#else
#define medvdbg(...)
#define medllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_SECURITY_FRAMEWORK_ERROR
#define sfdbg(format, ...)     dbg(format, ##__VA_ARGS__)
#define sflldbg(format, ...)   lldbg(format, ##__VA_ARGS__)
#else
#define sfdbg(...)
#define sflldbg(...)
#endif

#ifdef CONFIG_DEBUG_SECURITY_FRAMEWORK_INFO
#define sfvdbg(format, ...)     vdbg(format, ##__VA_ARGS__)
#define sfllvdbg(format, ...)   llvdbg(format, ##__VA_ARGS__)
#else
#define sfvdbg(...)
#define sfllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_TASK_MANAGER_ERROR
#define tmdbg(format, ...)      dbg(format, ##__VA_ARGS__)
#define tmlldbg(format, ...)    lldbg(format, ##__VA_ARGS__)
#else
#define tmdbg(...)
#define tmlldbg(...)
#endif
#ifdef CONFIG_DEBUG_TASK_MANAGER_INFO
#define tmvdbg(format, ...)     vdbg(format, ##__VA_ARGS__)
#define tmllvdbg(format, ...)   llvdbg(format, ##__VA_ARGS__)
#else
#define tmvdbg(...)
#define tmllvdbg(...)
#endif

/******************************************/
/*        OS Function specific debug      */
/******************************************/

#ifdef CONFIG_DEBUG_DMA_ERROR
#define dmadbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define dmalldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define dmadbg(...)
#define dmalldbg(...)
#endif

#ifdef CONFIG_DEBUG_DMA_WARN
#define dmawdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define dmallwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define dmawdbg(...)
#define dmallwdbg(...)
#endif

#ifdef CONFIG_DEBUG_DMA_INFO
#define dmavdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define dmallvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define dmavdbg(...)
#define dmallvdbg(...)
#endif

#ifdef CONFIG_DEBUG_PAGING_ERROR
#define pgdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define pglldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define pgdbg(...)
#define pglldbg(...)
#endif

#ifdef CONFIG_DEBUG_PAGING_WARN
#define pgwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define pgllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define pgwdbg(...)
#define pgllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_PAGING_INFO
#define pgvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define pgllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define pgvdbg(...)
#define pgllvdbg(...)
#endif

/*************************************/
/*        Driver specific debug      */
/*************************************/

#ifdef CONFIG_DEBUG_ANALOG_ERROR
#define adbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define alldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define adbg(...)
#define alldbg(...)
#endif

#ifdef CONFIG_DEBUG_ANALOG_WARN
#define awdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define allwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define awdbg(...)
#define allwdbg(...)
#endif

#ifdef CONFIG_DEBUG_ANALOG_INFO
#define avdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define allvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define avdbg(...)
#define allvdbg(...)
#endif

#ifdef CONFIG_DEBUG_GRAPHICS_ERROR
#define gdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define glldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define gdbg(...)
#define glldbg(...)
#endif

#ifdef CONFIG_DEBUG_GRAPHICS_WARN
#define gwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define gllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define gwdbg(...)
#define gllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_GRAPHICS_INFO
#define gvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define gllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define gvdbg(...)
#define gllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_I2C_ERROR
#define i2cerr(format, ...)    dbg(format, ##__VA_ARGS__)
#define i2clldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define i2cerr(...)
#define i2clldbg(...)
#endif

#ifdef CONFIG_DEBUG_I2C_WARN
#define i2cwarn(format, ...)    wdbg(format, ##__VA_ARGS__)
#define i2cllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define i2cwarn(...)
#define i2cllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_I2C_INFO
#define i2cinfo(format, ...)   vdbg(format, ##__VA_ARGS__)
#define i2cllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define i2cinfo(...)
#define i2cllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_I2S_ERROR
#define i2serr(format, ...)    dbg(format, ##__VA_ARGS__)
#define i2slldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define i2serr(...)
#define i2slldbg(...)
#endif

#ifdef CONFIG_DEBUG_I2S_WARN
#define i2swarn(format, ...)    wdbg(format, ##__VA_ARGS__)
#define i2sllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define i2swarn(...)
#define i2sllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_I2S_INFO
#define i2sinfo(format, ...)   vdbg(format, ##__VA_ARGS__)
#define i2sllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define i2sinfo(...)
#define i2sllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_INPUT_ERROR
#define idbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define illdbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define idbg(...)
#define illdbg(...)
#endif

#ifdef CONFIG_DEBUG_INPUT_WARN
#define iwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define illwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define iwdbg(...)
#define illwdbg(...)
#endif

#ifdef CONFIG_DEBUG_INPUT_INFO
#define ivdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define illvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define ivdbg(...)
#define illvdbg(...)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_ERROR
#define nldbg(format, ...)      dbg(format, ##__VA_ARGS__)
#define nllldbg(format, ...)    lldbg(format, ##__VA_ARGS__)
#else
#define nldbg(...)
#define nllldbg(...)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_INFO
#define nlvdbg(format, ...)     vdbg(format, ##__VA_ARGS__)
#define nlllvdbg(format, ...)   llvdbg(format, ##__VA_ARGS__)
#else
#define nlvdbg(...)
#define nlllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_VENDOR_DRV_ERROR
#define vddbg(format, ...)      dbg(format, ##__VA_ARGS__)
#define vdlldbg(format, ...)    lldbg(format, ##__VA_ARGS__)
#else
#define vddbg(...)
#define vdlldbg(...)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_VENDER_DRV_INFO
#define vdvdbg(format, ...)     vdbg(format, ##__VA_ARGS__)
#define vdllvdbg(format, ...)   llvdbg(format, ##__VA_ARGS__)
#else
#define vdvdbg(...)
#define vdllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_SENSORS_ERROR
#define sndbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define snlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define sndbg(...)
#define snlldbg(...)
#endif

#ifdef CONFIG_DEBUG_SENSORS_WARN
#define snwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define snllwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define snwdbg(...)
#define snllwdbg(...)
#endif

#ifdef CONFIG_DEBUG_SENSORS_INFO
#define snvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define snllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define snvdbg(...)
#define snllvdbg(...)
#endif

#ifdef CONFIG_DEBUG_SPI_ERROR
#define spidbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define spilldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define spidbg(...)
#define spilldbg(...)
#endif

#ifdef CONFIG_DEBUG_SPI_WARN
#define spiwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define spillwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define spiwdbg(...)
#define spillwdbg(...)
#endif

#ifdef CONFIG_DEBUG_SPI_INFO
#define spivdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define spillvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define spivdbg(...)
#define spillvdbg(...)
#endif

#ifdef CONFIG_DEBUG_TIMER_ERROR
#define tmrdbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define tmrlldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define tmrdbg(format, ...)
#define tmrlldbg(format, ...)
#endif

#ifdef CONFIG_DEBUG_TIMER_INFO
#define tmrvdbg(format, ...)    vdbg(format, ##__VA_ARGS__)
#define tmrllvdbg(format, ...)  llvdbg(format, ##__VA_ARGS__)
#else
#define tmrvdbg(format, ...)
#define tmrllvdbg(format, ...)
#endif

#ifdef CONFIG_DEBUG_TTRACE
#define ttdbg(format, ...) dbg(format, ##__VA_ARGS__)
#else
#define ttdbg(format, ...)
#endif

#ifdef CONFIG_DEBUG_USB_ERROR
#define udbg(format, ...)    dbg(format, ##__VA_ARGS__)
#define ulldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
#else
#define udbg(...)
#define ulldbg(...)
#endif

#ifdef CONFIG_DEBUG_USB_WARN
#define uwdbg(format, ...)    wdbg(format, ##__VA_ARGS__)
#define ullwdbg(format, ...)  llwdbg(format, ##__VA_ARGS__)
#else
#define uwdbg(...)
#define ullwdbg(...)
#endif

#ifdef CONFIG_DEBUG_USB_INFO
#define uvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
#define ullvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
#define uvdbg(...)
#define ullvdbg(...)
#endif


#else							/* CONFIG_CPP_HAVE_VARARGS */

/* Variadic macros NOT supported */

#ifdef CONFIG_DEBUG
#ifndef CONFIG_DEBUG_ERROR
#define dbg        (void)
#define lldbg     (void)
#else
#ifndef CONFIG_ARCH_LOWPUTC
#define lldbg      (void)
#endif
#endif
#ifndef CONFIG_DEBUG_WARN
#define wdbg        (void)
#define llwdbg     (void)
#else
#ifndef CONFIG_ARCH_LOWPUTC
#define llwdbg      (void)
#endif
#endif
#ifndef CONFIG_DEBUG_VERBOSE
#define vdbg       (void)
#define llvdbg     (void)
#else
#ifndef CONFIG_ARCH_LOWPUTC
#define llvdbg   (void)
#endif
#endif
#else
#define dbg         (void)
#define lldbg       (void)
#define wdbg        (void)
#define llwdbg      (void)
#define vdbg        (void)
#define llvdbg      (void)
#endif


/****************************************/
/*        Subsystem specific debug      */
/****************************************/

#ifdef CONFIG_DEBUG_AUDIO_ERROR
#define auddbg      dbg
#define audlldbg    lldbg
#else
#define auddbg      (void)
#define audlldbg    (void)
#endif

#ifdef CONFIG_DEBUG_AUDIO_WARN
#define audwdbg     wdbg
#define audllwdbg   llwdbg
#else
#define audwdbg     (void)
#define audllwdbg   (void)
#endif

#ifdef CONFIG_DEBUG_AUDIO_INFO
#define audvdbg     vdbg
#define audllvdbg   llvdbg
#else
#define audvdbg     (void)
#define audllvdbg   (void)
#endif

#ifdef CONFIG_DEBUG_ERR_REPORT_ERROR
#define nwerrdbg    dbg
#define nwerrlldbg  lldbg
#else
#define nwerrdbg	(void)
#define nwerrlldbg	(void)
#endif

#ifdef CONFIG_DEBUG_ERR_REPORT_WARN
#define nwerr_wdbg    wdbg
#define nwerr_llwdbg  llwdbg
#else
#define nwerr_wdbg   (void)
#define nwerr_llwdbg (void)
#endif

#ifdef CONFIG_DEBUG_ERR_REPORT_INFO
#define nwerr_vdbg   vdbg
#define nwerr_llvdbg llvdbg
#else
#define nwerr_vdbg   (void)
#define nwerr_llvdbg (void)
#endif

#ifdef CONFIG_DEBUG_FS_ERROR
#define fdbg        dbg
#define flldbg      lldbg
#else
#define fdbg        (void)
#define flldbg      (void)
#endif

#ifdef CONFIG_DEBUG_FS_WARN
#define fwdbg       wdbg
#define fllwdbg     llwdbg
#else
#define fwdbg       (void)
#define fllwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_FS_INFO
#define fvdbg       vdbg
#define fllvdbg     llvdbg
#else
#define fvdbg       (void)
#define fllvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_LIB_ERROR
#define ldbg        dbg
#define llldbg      lldbg
#else
#define ldbg        (void)
#define llldbg      (void)
#endif

#ifdef CONFIG_DEBUG_LIB_WARN
#define lwdbg       wdbg
#define lllwdbg     llwdbg
#else
#define lwdbg       (void)
#define lllwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_LIB_INFO
#define lvdbg       vdbg
#define lllvdbg     llvdbg
#else
#define lvdbg       (void)
#define lllvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_MM_ERROR
#define mdbg        dbg
#define mlldbg      lldbg
#else
#define mdbg        (void)
#define mlldbg      (void)
#endif

#ifdef CONFIG_DEBUG_MM_WARN
#define mwdbg       wdbg
#define mllwdbg     llwdbg
#else
#define mwdbg       (void)
#define mlwldbg     (void)
#endif

#ifdef CONFIG_DEBUG_MM_INFO
#define mvdbg       vdbg
#define mllvdbg     llvdbg
#else
#define mvdbg       (void)
#define mllvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_NET_ERROR
#define ndbg        dbg
#define nlldbg      lldbg
#else
#define ndbg        (void)
#define nlldbg      (void)
#endif

#ifdef CONFIG_DEBUG_NET_WARN
#define nwdbg       wdbg
#define nllwdbg     llwdbg
#else
#define nwdbg       (void)
#define nllwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_NET_INFO
#define nvdbg       vdbg
#define nllvdbg     llvdbg
#else
#define nvdbg       (void)
#define nllvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_SCHED_ERROR
#define sdbg        dbg
#define slldbg      lldbg
#else
#define sdbg        (void)
#define slldbg      (void)
#endif

#ifdef CONFIG_DEBUG_SCHED_WARN
#define swdbg       wdbg
#define sllwdbg     llwdbg
#else
#define swdbg       (void)
#define sllwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_SCHED_INFO
#define svdbg       vdbg
#define sllvdbg     llvdbg
#else
#define svdbg       (void)
#define sllvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_SECURE_ELEMENT_ERROR
#define sedbg      dbg
#define selldbg    lldbg
#else
#define sedbg      (void)
#define selldbg    (void)
#endif

#ifdef CONFIG_DEBUG_SECURE_ELEMENT_INFO
#define sevdbg     vdbg
#define sellvdbg   llvdbg
#else
#define sevdbg     (void)
#define sellvdbg   (void)
#endif

/****************************************/
/*        Framework specific debug      */
/****************************************/

#ifdef CONFIG_DEBUG_EVENTLOOP_ERROR
#define eldbg      dbg
#define ellldbg    lldbg
#else
#define eldbg      (void)
#define ellldbg    (void)
#endif

#ifdef CONFIG_DEBUG_EVENTLOOP_INFO
#define elvdbg     vdbg
#define elllvdbg   llvdbg
#else
#define elvdbg     (void)
#define elllvdbg   (void)
#endif

#ifdef CONFIG_DEBUG_MEDIA_ERROR
#define meddbg      dbg
#define medlldbg    lldbg
#else
#define meddbg      (void)
#define medlldbg    (void)
#endif

#ifdef CONFIG_DEBUG_MEDIA_WARN
#define medwdbg     wdbg
#define medllwdbg   llwdbg
#else
#define medwdbg     (void)
#define medllwdbg   (void)
#endif

#ifdef CONFIG_DEBUG_MEDIA_INFO
#define medvdbg     vdbg
#define medllvdbg   llvdbg
#else
#define medllvdbg   (void)
#define medllvdbg   (...)
#endif

#ifdef CONFIG_DEBUG_SECURITY_FRAMEWORK_ERROR
#define sfdbg      dbg
#define sflldbg    lldbg
#else
#define sfdbg      (void)
#define sflldbg    (void)
#endif

#ifdef CONFIG_DEBUG_SECURITY_FRAMEWORK_INFO
#define sfvdbg     vdbg
#define sfllvdbg   llvdbg
#else
#define sfvdbg     (void)
#define sfllvdbg   (void)
#endif

#ifdef CONFIG_DEBUG_TASK_MANAGER_ERROR
#define tmdbg       dbg
#define tmlldbg     lldbg
#else
#define tmdbg       (void)
#define tmlldbg     (void)
#endif

#ifdef CONFIG_DEBUG_TASK_MANAGER_INFO
#define tmvdbg      vdbg
#define tmllvdbg    llvdbg
#else
#define tmvdbg      (void)
#define tmllvdbg    (void)
#endif


/******************************************/
/*        OS Function specific debug      */
/******************************************/

#ifdef CONFIG_DEBUG_DMA_ERROR
#define dmadbg      dbg
#define dmalldbg    lldbg
#else
#define dmadbg      (void)
#define dmalldbg    (void)
#endif

#ifdef CONFIG_DEBUG_DMA_WARN
#define dmawdbg     wdbg
#define dmallwdbg   llwdbg
#else
#define dmawdbg     (void)
#define dmallwdbg   (void)
#endif

#ifdef CONFIG_DEBUG_DMA_INFO
#define dmavdbg     vdbg
#define dmallvdbg   llvdbg
#else
#define dmavdbg     (void)
#define dmallvdbg   (void)
#endif

#ifdef CONFIG_DEBUG_PAGING_ERROR
#define pgdbg       dbg
#define pglldbg     lldbg
#else
#define pgdbg       (void)
#define pglldbg     (void)
#endif

#ifdef CONFIG_DEBUG_PAGING_WARN
#define pgwdbg      wdbg
#define pgllwdbg    llwdbg
#else
#define pgwdbg      (void)
#define pgllwdbg    (void)
#endif

#ifdef CONFIG_DEBUG_PAGING_INFO
#define pgvdbg      vdbg
#define pgllvdbg    llvdbg
#else
#define pgvdbg      (void)
#define pgllvdbg    (void)
#endif

/*************************************/
/*        Driver specific debug      */
/*************************************/

#ifdef CONFIG_DEBUG_ANALOG_ERROR
#define adbg        dbg
#define alldbg      lldbg
#else
#define adbg        (void)
#define alldbg      (void)
#endif

#ifdef CONFIG_DEBUG_ANALOG_WARN
#define awdbg       wdbg
#define allwdbg     llwdbg
#else
#define awdbg       (void)
#define allwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_ANALOG_INFO
#define avdbg       vdbg
#define allvdbg     llvdbg
#else
#define avdbg       (void)
#define allvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_GRAPHICS_ERROR
#define gdbg        dbg
#define glldbg      lldbg
#else
#define gdbg        (void)
#define glldbg      (void)
#endif

#ifdef CONFIG_DEBUG_GRAPHICS_WARN
#define gwdbg       wdbg
#define gllwdbg     llwdbg
#else
#define gwdbg       (void)
#define gllwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_GRAPHICS_INFO
#define gvdbg       vdbg
#define gllvdbg     llvdbg
#else
#define gvdbg       (void)
#define gllvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_INPUT_ERROR
#define idbg        dbg
#define illdbg      lldbg
#else
#define idbg        (void)
#define illdbg      (void)
#endif

#ifdef CONFIG_DEBUG_INPUT_WARN
#define iwdbg       wdbg
#define illwdbg     llwdbg
#else
#define iwdbg       (void)
#define illwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_INPUT_INFO
#define ivdbg       vdbg
#define illvdbg     llvdbg
#else
#define ivdbg       (void)
#define illvdbg     (void)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_ERROR
#define nldbg      dbg
#define nllldbg    lldbg
#else
#define nldbg      (void)
#define nllldbg    (void)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_INFO
#define nlvdbg     vdbg
#define nlllvdbg   llvdbg
#else
#define nlvdbg     (void)
#define nlllvdbg   (void)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_VENDER_DRV_ERROR
#define vddbg      dbg
#define vdlldbg    lldbg
#else
#define vddbg      (void)
#define vdlldbg    (void)
#endif

#ifdef CONFIG_DEBUG_LWNL80211_VENDOR_DRV_INFO
#define vdvdbg     vdbg
#define vdllvdbg   llvdbg
#else
#define vdvdbg     (void)
#define vdllvdbg   (void)
#endif

#ifdef CONFIG_DEBUG_SENSORS_ERROR
#define sndbg       dbg
#define snlldbg     lldbg
#else
#define sndbg       (void)
#define snlldbg     (void)
#endif

#ifdef CONFIG_DEBUG_SENSORS_WARN
#define snwdbg      wdbg
#define snllwdbg    llwdbg
#else
#define snwdbg      (void)
#define snllwdbg    (void)
#endif

#ifdef CONFIG_DEBUG_SENSORS_INFO
#define snvdbg      vdbg
#define snllvdbg    llvdbg
#else
#define snvdbg      (void)
#define snllvdbg    (void)
#endif

#ifdef CONFIG_DEBUG_TIMER_ERROR
#define tmrdbg      dbg
#define tmrlldbg    lldbg
#else
#define tmrdbg      (void)
#define tmrlldbg    (void)
#endif

#ifdef CONFIG_DEBUG_TIMER_INFO
#define tmrvdbg     vdbg
#define tmrllvdbg   llvdbg
#else
#define tmrvdbg     (void)
#define tmrllvdbg   (void)
#endif

#ifdef CONFIG_DEBUG_USB_ERROR
#define udbg        dbg
#define ulldbg      lldbg
#else
#define udbg        (void)
#define ulldbg      (void)
#endif

#ifdef CONFIG_DEBUG_USB_WARN
#define uwdbg       wdbg
#define ullwdbg     llwdbg
#else
#define uwdbg       (void)
#define ullwdbg     (void)
#endif

#ifdef CONFIG_DEBUG_USB_INFO
#define uvdbg       vdbg
#define ullvdbg     llvdbg
#else
#define uvdbg       (void)
#define ullvdbg     (void)
#endif

#endif							/* CONFIG_CPP_HAVE_VARARGS */

/* Buffer dumping macros do not depend on varargs */

#ifdef CONFIG_DEBUG
#ifdef CONFIG_DEBUG_ERROR
#define dbgdumpbuffer(m, b, n) lib_dumpbuffer(m, b, n)
#else
#define dbgdumpbuffer(m, b, n)
#endif
#ifdef CONFIG_DEBUG_WARN
#define wdbgdumpbuffer(m, b, n) lib_dumpbuffer(m, b, n)
#else
#define wdbgdumpbuffer(m, b, n)
#endif
#ifdef CONFIG_DEBUG_VERBOSE
#define vdbgdumpbuffer(m, b, n) lib_dumpbuffer(m, b, n)
#else
#define vdbgdumpbuffer(m, b, n)
#endif
#else
#define dbgdumpbuffer(m, b, n)
#define wdbgdumpbuffer(m, b, n)
#define vdbgdumpbuffer(m, b, n)
#endif

/* Subsystem specific debug */

#ifdef CONFIG_DEBUG_MM
#define mdbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define mvdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define mdbgdumpbuffer(m, b, n)
#define mvdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_SCHED
#define sdbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define svdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define sdbgdumpbuffer(m, b, n)
#define svdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_PAGING
#define pgdbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define pgvdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define pgdbgdumpbuffer(m, b, n)
#define pgvdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_DMA
#define dmadbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define dmavdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define dmadbgdumpbuffer(m, b, n)
#define dmavdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_NET
#define ndbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define nvdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define ndbgdumpbuffer(m, b, n)
#define nvdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_USB
#define udbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define uvdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define udbgdumpbuffer(m, b, n)
#define uvdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_FS
#define fdbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define fvdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define fdbgdumpbuffer(m, b, n)
#define fvdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_INPUT
#define idbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define ivdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define idbgdumpbuffer(m, b, n)
#define ivdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_GRAPHICS
#define gdbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define gvdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define gdbgdumpbuffer(m, b, n)
#define gvdbgdumpbuffer(m, b, n)
#endif

#ifdef CONFIG_DEBUG_LIB
#define ldbgdumpbuffer(m, b, n)  dbgdumpbuffer(m, b, n)
#define lvdbgdumpbuffer(m, b, n) vdbgdumpbuffer(m, b, n)
#else
#define ldbgdumpbuffer(m, b, n)
#define lvdbgdumpbuffer(m, b, n)
#endif

/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @ingroup DEBUG_KERNEL
 * @brief  Dump a buffer of data
 *
 * @details @b #include <debug.h>
 * @param[in] msg message for buffer dump
 * @param[in] buffer buffer
 * @param[in] buflen length for buffer
 * @return void
 * @since TizenRT v1.1
 */

void lib_dumpbuffer(FAR const char *msg, FAR const uint8_t *buffer, unsigned int buflen);

/* The system logging interfaces are normally accessed via the macros
 * provided above.  If the cross-compiler's C pre-processor supports a
 * variable number of macro arguments, then those macros below will map all
 * debug statements to the logging interfaces declared in syslog.h.
 *
 * If the cross-compiler's pre-processor does not support variable length
 * arguments, then these additional APIs will be built.
 */

#ifndef CONFIG_CPP_HAVE_VARARGS
#ifdef CONFIG_DEBUG

#ifdef CONFIG_DEBUG_ERROR
int dbg(const char *format, ...);

#ifdef CONFIG_ARCH_LOWPUTC
int lldbg(const char *format, ...);
#endif
#endif

#ifdef CONFIG_DEBUG_WARN
int wdbg(const char *format, ...);

#ifdef CONFIG_ARCH_LOWPUTC
int llwdbg(const char *format, ...);
#endif
#endif

#ifdef CONFIG_DEBUG_VERBOSE
int vdbg(const char *format, ...);

#ifdef CONFIG_ARCH_LOWPUTC
int llvdbg(const char *format, ...);
#endif
#endif

#endif							/* CONFIG_DEBUG */
#endif							/* CONFIG_CPP_HAVE_VARARGS */

#if defined(__cplusplus)
}
#endif
#endif							/* __INCLUDE_DEBUG_H */
/**
 * @}
 */
