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
 * Copyright (c) 2010--2011, Olaf Bergmann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  o Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  o Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 ****************************************************************************/
/* debug.h -- debug utilities
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#ifndef _COAP_DEBUG_H_
#define _COAP_DEBUG_H_

#include "config.h"

#ifndef COAP_DEBUG_FD
#define COAP_DEBUG_FD stdout
#endif

#ifndef COAP_ERR_FD
#define COAP_ERR_FD stderr
#endif

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
typedef short coap_log_t;
#else
/** Pre-defined log levels akin to what is used in \b syslog. */
typedef enum { LOG_EMERG = 0, LOG_ALERT, LOG_CRIT, LOG_WARNING,
			   LOG_NOTICE, LOG_INFO, LOG_DEBUG
			 } coap_log_t;
#endif

/** Returns the current log level. */
coap_log_t coap_get_log_level(void);

/** Sets the log level to the specified value. */
void coap_set_log_level(coap_log_t level);

/**
 * Writes the given text to @c COAP_ERR_FD (for @p level <= @c
 * LOG_CRIT) or @c COAP_DEBUG_FD (for @p level >= @c LOG_WARNING). The
 * text is output only when @p level is below or equal to the log
 * level that set by coap_set_log_level().
 */
void coap_log_impl(coap_log_t level, const char *format, ...);

#ifndef coap_log
#define coap_log(...) coap_log_impl(__VA_ARGS__)
#endif

#ifndef NDEBUG

/* A set of convenience macros for common log levels. */
#define info(...) coap_log(LOG_INFO, __VA_ARGS__)
#define warn(...) coap_log(LOG_WARNING, __VA_ARGS__)
#define debug(...) coap_log(LOG_DEBUG, __VA_ARGS__)

#include "pdu.h"
void coap_show_pdu(const coap_pdu_t *);

struct coap_address_t;
size_t coap_print_addr(const struct coap_address_t *, unsigned char *, size_t);

#else

#define debug(...)
#define info(...)
#define warn(...)

#define coap_show_pdu(x)
#define coap_print_addr(...)

#endif

#endif							/* _COAP_DEBUG_H_ */
