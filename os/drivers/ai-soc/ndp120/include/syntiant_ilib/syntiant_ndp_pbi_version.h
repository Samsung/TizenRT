/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/
#ifndef SYNTIANT_NDP_PBI_VERSION_H
#define SYNTIANT_NDP_PBI_VERSION_H

/* NDP10xB0 current PBI version number */
#define SYNTIANT_NDP_ILIB_PBI_NDP10XB0_LABEL "NDP10x-B0"
#define SYNTIANT_NDP_ILIB_PBI_NDP10XB0_MAJOR_VERSION    1
#define SYNTIANT_NDP_ILIB_PBI_NDP10XB0_MINOR_VERSION    0
#define SYNTIANT_NDP_ILIB_PBI_NDP10XB0_PATCH_VERSION    0

/* NDP115A0 current PBI version number */
#define SYNTIANT_NDP_ILIB_PBI_NDP115A0_LABEL "NDP115-A0"
#define SYNTIANT_NDP_ILIB_PBI_NDP115A0_MAJOR_VERSION    3
#define SYNTIANT_NDP_ILIB_PBI_NDP115A0_MINOR_VERSION    3
#define SYNTIANT_NDP_ILIB_PBI_NDP115A0_PATCH_VERSION    0

/* NDP120B0 current PBI version number */
#define SYNTIANT_NDP_ILIB_PBI_NDP120B0_LABEL "NDP120-B0"
#define SYNTIANT_NDP_ILIB_PBI_NDP120B0_MAJOR_VERSION    3
#define SYNTIANT_NDP_ILIB_PBI_NDP120B0_MINOR_VERSION    3
#define SYNTIANT_NDP_ILIB_PBI_NDP120B0_PATCH_VERSION    0


#ifndef STR_HELPER
#define STR_HELPER(x)   #x
#define STR(x)          STR_HELPER(x)
#endif

#ifndef EXCLUDE_SYNTIANT_CORE_1
#define SYNTIANT_NDP_ILIB_PBI_VERSION_10X \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP10XB0_MAJOR_VERSION) "." \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP10XB0_MINOR_VERSION) "." \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP10XB0_PATCH_VERSION)
#define SYNTIANT_NDP_ILIB_PBI_VERSION_2_SEPARATOR   ", "
#else
#define SYNTIANT_NDP_ILIB_PBI_VERSION_10X           ""
#define SYNTIANT_NDP_ILIB_PBI_VERSION_2_SEPARATOR   ""
#endif
#define SYNTIANT_NDP_ILIB_PBI_VERSION_1 \
    SYNTIANT_NDP_ILIB_PBI_NDP10XB0_LABEL SYNTIANT_NDP_ILIB_PBI_VERSION_10X

#ifndef EXCLUDE_SYNTIANT_CORE_2
#define SYNTIANT_NDP_ILIB_PBI_VERSION_120 \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP120B0_MAJOR_VERSION) "." \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP120B0_MINOR_VERSION) "." \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP120B0_PATCH_VERSION)
#define SYNTIANT_NDP_ILIB_PBI_VERSION_115 \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP115A0_MAJOR_VERSION) "." \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP115A0_MINOR_VERSION) "." \
    STR(SYNTIANT_NDP_ILIB_PBI_NDP115A0_PATCH_VERSION)
#define SYNTIANT_NDP_ILIB_PBI_VERSION_2 \
    SYNTIANT_NDP_ILIB_PBI_NDP120B0_LABEL SYNTIANT_NDP_ILIB_PBI_VERSION_120 \
    ", " SYNTIANT_NDP_ILIB_PBI_NDP115A0_LABEL SYNTIANT_NDP_ILIB_PBI_VERSION_115
#else
#define SYNTIANT_NDP_ILIB_PBI_VERSION_2             ""
#endif

#define SYNTIANT_NDP_ILIB_PBI_VERSION SYNTIANT_NDP_ILIB_PBI_VERSION_1 \
    SYNTIANT_NDP_ILIB_PBI_VERSION_2_SEPARATOR SYNTIANT_NDP_ILIB_PBI_VERSION_2

#endif

