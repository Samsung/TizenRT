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
** config.h                 Option control file for the NYS project
**
** Copyright (c) 1993 Signum Support AB
**
** This file is part of the NYS Library.
**
** The NYS Library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public License as
** published by the Free Software Foundation; either version 2 of the
** License, or (at your option) any later version.
**
** The NYS Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with the NYS Library; see the file COPYING.LIB.  If
** not, write to the Free Software Foundation, Inc., 675 Mass Ave,
** Cambridge, MA 02139, USA.
*/

#ifndef __NYS_CONFIG_H__
#define __NYS_CONFIG_H__

/*
** Define this when/if you want to produce a final
** version of these libraries (for inclusion into the
** normal libc). You must also rename the old functions
** in libc to "_" + the old name (Ie, "getpwnam" -> "_getpwnam")
** if you do this.
*/
#define FINALOUT 1

/*
** Build options, use these to select what you want to include
** in the Name Service Switch handler.
*/
#if 0							/* These should be selected in config.h by "configure" */
#undef ENABLE_YP				/* NSSwitch YP (NIS v2) support   */
#undef ENABLE_NIS				/* NSSwitch NIS+ (NIS v3) support */
#undef ENABLE_DNS				/* NSSwitch DNS/Hesiod support    */
#undef ENABLE_DBM				/* NSSwitch DBM support           */
#endif							/* 0 */

#if defined(ENABLE_YP) || defined(ENABLE_NIS) || defined(ENABLE_DNS) || defined(ENABLE_DBM)
#define ENABLE_NSW	1
#endif

/*
** Low level support options, use these to enable emulation code
** if you don't have local support for them.
*/
#define ENABLE_YPEMU	1		/* YP client side emulation   */
#define ENABLE_NISEMU	1		/* NIS+ client side emulation */

/*
** Define this if you are using an 'yp.x' with buggy ypresp_key_val
** definition to generate the yp_clnt.c and yp_xdr.c files (like the
** one included in SUNRPC 4.0). See yp/src/yp_if.c for more information.
** The one included here has been fixed.
*/
/*#define HAVE_BUGGY_YP_X 1*/

#endif
