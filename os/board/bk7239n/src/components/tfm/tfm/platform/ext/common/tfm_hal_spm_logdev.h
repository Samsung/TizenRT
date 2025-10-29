/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_HAL_SPM_LOGDEV_H__
#define __TFM_HAL_SPM_LOGDEV_H__

#include <stdint.h>

/**
 * \brief HAL API for SPM log system
 *
 * \param[in]  str       The string to output
 * \param[in]  len       Length of the string in bytes
 *
 * \retval >=0           Number of chars output.
 * \retval <0            TFM HAL error code.
 */
int32_t tfm_hal_output_spm_log(const char *str, uint32_t len);

#endif /* __TFM_HAL_SPM_LOGDEV_H__ */
