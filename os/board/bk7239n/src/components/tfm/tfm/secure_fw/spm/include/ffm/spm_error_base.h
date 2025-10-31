/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_ERROR_BASE__
#define __SPM_ERROR_BASE__

#include <stdint.h>

/*
 * SPM logs the implementation error code but does not parse it. SPM cares
 * only two status of implementation function: SPM_SUCCESS or !SPM_SUCCESS.
 *
 * SPM_SUCCESS is defined as ZERO. Other error codes MUST be defined by
 * performing arithmetic increasements on SPM_ERROR_BASE.
 */
#define SPM_ERROR_BASE             INT32_MIN

/* The 'no error' code, which is 'success'. */
#define SPM_SUCCESS                0

#endif /* __SPM_ERROR_BASE__ */
