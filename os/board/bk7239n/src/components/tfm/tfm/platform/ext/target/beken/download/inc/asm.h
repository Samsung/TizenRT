/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __ASM_H__
#define __ASM_H__

#define _DO_CONCAT(x, y) x##y
#define _CONCAT(x, y) _DO_CONCAT(x, y)

#define _STRINGIFY(x) #x
#define STRINGIFY(s) _STRINGIFY(s)

#define __GENERIC_SECTION(s) __attribute__((section(STRINGIFY(s))))
#define _GENERIC_SECTION(s) __GENERIC_SECTION(s)

/* global func/data */
#define GFUNCTION(x) .global x; .type x,STT_FUNC; x:
#define GDATA(x) .global x; .type x,STT_OBJECT; x:

/* local func/data */
#define LFUNCTION(x) .type x,STT_FUNC; x:
#define LDATA(x) .type x,STT_OBJECT; x:

#endif /* __ASM_H__ */

