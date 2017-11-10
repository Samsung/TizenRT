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
** getXXent.h        Function definition file for getXXent() type calls
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
**    RETOBJTYPE
**    FUNCOBJENT
**    NSWENTRY
**
**    NSWPREFIX
**    STDPREFIX
**
**    CONCAT(a,b)
*/

#define GETPREFIX CONCAT(STDPREFIX,get)

RETOBJTYPE CONCAT(CONCAT(NSWPREFIX, get), FUNCOBJENT)(void)
{
	RETOBJTYPE robj = NULL;
	static int CONCAT(recur_, FUNCNAME) = 0;

	if (nswp == NULL) {
		nswp = getnswbyname(NSWENTRY);
		setptr = 0;
	}

	/* We are already executing in this function, or no /etc/nsswitch.conf */
	if (CONCAT(recur_, FUNCNAME) == 1 || nswp == NULL) {
		return CONCAT(GETPREFIX, FUNCOBJENT)();
	}

	if (setptr >= nswp->orderc) {
		return NULL;
	}

	CONCAT(recur_, FUNCNAME) = 1;

	errno = 0;
	do {
		if (nswp->orderl[setptr] == NSWO_RETURN && errno == 0) {
			CONCAT(recur_, FUNCNAME) = 0;
			return NULL;
		}

		if (setflag && setptr == setflag) {
			CONCAT(_nsw_end, FUNCOBJENT)(setptr - 1);
			CONCAT(_nsw_set, FUNCOBJENT)(setptr);
			++setflag;
		}

		switch (nswp->orderl[setptr]) {
		case NSWO_RETURN:
			break;

		case NSWO_FILES:
			robj = CONCAT(GETPREFIX, FUNCOBJENT)();
			break;

#ifdef ENABLE_NIS
		case NSWO_NISPLUS:
			robj = CONCAT(_nis_get, FUNCOBJENT)();
			break;
#endif
#ifdef ENABLE_YP
		case NSWO_NIS:
			robj = CONCAT(_yp_get, FUNCOBJENT)();
			break;
#endif
#ifdef ENABLE_DNS
		case NSWO_DNS:
			robj = CONCAT(_dns_get, FUNCOBJENT)();
			break;
#endif
#ifdef ENABLE_DBM
		case NSWO_DBM:
			robj = CONCAT(_dbm_get, FUNCOBJENT)();
			break;
#endif
		}
	} while (robj == NULL && (++setptr < nswp->orderc));

	CONCAT(recur_, FUNCNAME) = 0;
	return robj;
}
