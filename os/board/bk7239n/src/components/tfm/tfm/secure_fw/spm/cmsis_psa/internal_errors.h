/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __INTERNAL_ERRORS_H__
#define __INTERNAL_ERRORS_H__

#include <stdint.h>
#include "ffm/spm_error_base.h"

/* Code for 'no error'. */
#ifndef SPM_SUCCESS
#define SPM_SUCCESS                0
#endif

/* Implementation internal error codes */
#define SPM_ERROR_BAD_PARAMETERS   ((int32_t)(SPM_ERROR_BASE) + 1)
#define SPM_ERROR_SHORT_BUFFER     ((int32_t)(SPM_ERROR_BASE) + 2)
#define SPM_ERROR_VERSION          ((int32_t)(SPM_ERROR_BASE) + 3)
#define SPM_ERROR_MEMORY_CHECK     ((int32_t)(SPM_ERROR_BASE) + 4)
#define SPM_ERROR_GENERIC          ((int32_t)(SPM_ERROR_BASE) + 5)

#endif /* __INTERNAL_ERRORS_H__ */
