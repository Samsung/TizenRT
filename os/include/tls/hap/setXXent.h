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
** setXXent.h        Function definition file for setXXent() type calls
**
** Copyright (c) 1993 Signum Support AB, Sweden
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
**
** Macros that should be defined before including this file:
**
**    RETOBJTYPE
**    FUNCOBJENT
**    NSWENTRY
**
**    SETARGTYPE   (optional, define only if setXXent() takes an argument)
**
**    NSWPREFIX
**    STDPREFIX
**
**    CONCAT(a,b)
*/

#define SETPREFIX CONCAT(STDPREFIX,set)

#ifdef SETARGTYPE
static SETARGTYPE saved_setarg;
#endif

static void CONCAT(_nsw_set, FUNCOBJENT)(int ptr)
{
	switch (nswp->orderl[ptr]) {
	case NSWO_RETURN:
		break;

	case NSWO_FILES:
#ifdef SETARGTYPE
		CONCAT(SETPREFIX, FUNCOBJENT)(saved_setarg);
#else
		CONCAT(SETPREFIX, FUNCOBJENT)();
#endif
		break;

#ifdef ENABLE_NIS
	case NSWO_NISPLUS:
#ifdef SETARGTYPE
		CONCAT(_nis_set, FUNCOBJENT)(saved_setarg);
#else
		CONCAT(_nis_set, FUNCOBJENT)();
#endif
		break;
#endif
#ifdef ENABLE_YP
	case NSWO_NIS:
#ifdef SETARGTYPE
		CONCAT(_yp_set, FUNCOBJENT)(saved_setarg);
#else
		CONCAT(_yp_set, FUNCOBJENT)();
#endif
		break;
#endif
#ifdef ENABLE_DNS
	case NSWO_DNS:
#ifdef SETARGTYPE
		CONCAT(_dns_set, FUNCOBJENT)(saved_setarg);
#else
		CONCAT(_dns_set, FUNCOBJENT)();
#endif
		break;
#endif
#ifdef ENABLE_DBM
	case NSWO_DBM:
#ifdef SETARGTYPE
		CONCAT(_dbm_set, FUNCOBJENT)(saved_setarg);
#else
		CONCAT(_dbm_set, FUNCOBJENT)();
#endif
		break;
#endif
	}
}

void CONCAT(CONCAT(NSWPREFIX, set), FUNCOBJENT)
#ifdef SETARGTYPE
(SETARGTYPE arg)
#else
(void)
#endif
{
	static int CONCAT(recur_, FUNCOBJENT) = 0;

	setflag = 1;
	setptr = 0;

	if (nswp == NULL) {
		nswp = getnswbyname(NSWENTRY);
	}

	if (CONCAT(recur_, FUNCOBJENT) == 1 || nswp == NULL) {
#ifdef SETARGTYPE
		CONCAT(SETPREFIX, FUNCOBJENT)(arg);
#else
		CONCAT(SETPREFIX, FUNCOBJENT)();
#endif
		return;
	}

	CONCAT(recur_, FUNCOBJENT) = 1;

#ifdef SETARGTYPE
#ifdef SETARGDUP
	saved_setarg = SETARGDUP(arg);
#else
	saved_setarg = arg;
#endif
#endif

	CONCAT(_nsw_set, FUNCOBJENT)(setptr);

	CONCAT(recur_, FUNCOBJENT) = 0;
}
