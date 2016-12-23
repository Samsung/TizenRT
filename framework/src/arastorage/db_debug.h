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
/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/**
 * \file
 *         A set of debugging macros.
 *
 * \author Nicolas Tsiftes <nvt@sics.se>
 *         Niclas Finne <nfi@sics.se>
 *         Joakim Eriksson <joakime@sics.se>
 */

#ifndef DB_DEBUG_H
#define DB_DEBUG_H
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <debug.h>
/****************************************************************************
* Pre-processor Definitions
****************************************************************************/
#define DEBUG_NONE      0x00
#define DEBUG_ENABLE    0x01
#define DEBUG_VERBOSE   0x10

#define DEBUG DEBUG_NONE

#if (DEBUG & DEBUG_ENABLE)
#define DB_LOG_D(format, ...)   printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#define DB_LOG_E(format, ...)   printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#define DB_LOG_V(format, ...)

#elif (DEBUG & DEBUG_VERBOSE)
#define DB_LOG_D(format, ...)   printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#define DB_LOG_E(format, ...)   printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#define DB_LOG_V(format, ...)   printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#else
#define DB_LOG_D(...)
#define DB_LOG_E(...)
#define DB_LOG_V(...)
#endif

#endif /* DB_DEBUG_H */
