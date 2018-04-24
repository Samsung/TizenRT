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

/*****************************************************************************
* pppdebug.h - System debugging utilities.
*
* Copyright (c) 2003 by Marc Boucher, Services Informatiques (MBSI) inc.
* portions Copyright (c) 1998 Global Election Systems Inc.
* portions Copyright (c) 2001 by Cognizant Pty Ltd.
*
* The authors hereby grant permission to use, copy, modify, distribute,
* and license this software and its documentation for any purpose, provided
* that existing copyright notices are retained in all copies and that this
* notice and the following disclaimer are included verbatim in any
* distributions. No written agreement, license, or royalty fee is required
* for any of the authorized uses.
*
* THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS *AS IS* AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
* REVISION HISTORY (please don't use tabs!)
*
* 03-01-01 Marc Boucher <marc@mbsi.ca>
*   Ported to lwIP.
* 98-07-29 Guy Lancaster <lancasterg@acm.org>, Global Election Systems Inc.
*   Original.
*
*****************************************************************************
*/
#ifndef PPPDEBUG_H
#define PPPDEBUG_H

#define TRACELCP PPP_DEBUG

/* Trace levels. */
#if PPP_DEBUG

#ifndef LOG_CRITICAL
#define LOG_CRITICAL  (PPP_DEBUG | LWIP_DBG_LEVEL_SEVERE)
#endif
#ifndef LOG_ERR
#define LOG_ERR       (PPP_DEBUG | LWIP_DBG_LEVEL_SEVERE)
#endif
#ifndef LOG_NOTICE
#define LOG_NOTICE    (PPP_DEBUG | LWIP_DBG_LEVEL_WARNING)
#endif
#ifndef LOG_WARNING
#define LOG_WARNING   (PPP_DEBUG | LWIP_DBG_LEVEL_WARNING)
#endif
#ifndef LOG_INFO
#define LOG_INFO      (PPP_DEBUG)
#endif
#ifndef LOG_DETAIL
#define LOG_DETAIL    (PPP_DEBUG)
#endif
#ifndef LOG_DEBUG
#define LOG_DEBUG     (PPP_DEBUG)
#endif

#define AUTHDEBUG(a, b) LWIP_DEBUGF(a, b)
#define IPCPDEBUG(a, b) LWIP_DEBUGF(a, b)
#define UPAPDEBUG(a, b) LWIP_DEBUGF(a, b)
#define LCPDEBUG(a, b)  LWIP_DEBUGF(a, b)
#define FSMDEBUG(a, b)  LWIP_DEBUGF(a, b)
#define CHAPDEBUG(a, b) LWIP_DEBUGF(a, b)
#define PPPDEBUG(a, b)  LWIP_DEBUGF(a, b)

#else							/* PPP_DEBUG */

#define AUTHDEBUG(a, b)
#define IPCPDEBUG(a, b)
#define UPAPDEBUG(a, b)
#define LCPDEBUG(a, b)
#define FSMDEBUG(a, b)
#define CHAPDEBUG(a, b)
#define PPPDEBUG(a, b)

#endif							/* PPP_DEBUG */

#endif							/* PPPDEBUG_H */
