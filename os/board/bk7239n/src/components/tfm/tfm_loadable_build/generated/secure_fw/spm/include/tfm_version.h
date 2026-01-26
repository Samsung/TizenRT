/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_VERSION_H__
#define __TFM_VERSION_H__

/*
 * Defines for TFM version.
 */
#define TFM_VERSION        0.0.0
#define TFM_VERSION_FULL   unknown

#define VERSTR(x)          #x
#define VERCON(x)          VERSTR(x)

#define VERSION_STR        VERCON(TFM_VERSION)
#define VERSION_FULLSTR    VERCON(TFM_VERSION_FULL)

#endif /* __TFM_VERSION_H__ */
