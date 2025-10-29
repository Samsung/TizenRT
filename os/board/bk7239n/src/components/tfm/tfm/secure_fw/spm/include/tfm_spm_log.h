/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPM_LOG_H__
#define __TFM_SPM_LOG_H__

#include <stddef.h>
#include <stdint.h>
#include "tfm_hal_defs.h"
#include "tfm_hal_spm_logdev.h"

/* The SPM log levels */
#define TFM_SPM_LOG_LEVEL_DEBUG    3  /* All log APIs output */
#define TFM_SPM_LOG_LEVEL_INFO     2  /*
                                       * All log APIs output except SPMLOG_DBG
                                       * and SPMLOG_DBGMSGVAL
                                       */
#define TFM_SPM_LOG_LEVEL_ERROR    1  /*
                                       * Only SPMLOG_ERRMSG and SPMLOG_ERRMSGVAL
                                       * APIs output.
                                       */
#define TFM_SPM_LOG_LEVEL_SILENCE  0  /* All log APIs are suppressed */

#ifndef TFM_SPM_LOG_LEVEL
#error "TFM_SPM_LOG_LEVEL not defined!"
#endif

#if (TFM_SPM_LOG_LEVEL > TFM_SPM_LOG_LEVEL_DEBUG || \
     TFM_SPM_LOG_LEVEL < TFM_SPM_LOG_LEVEL_SILENCE)
#error "Incorrect TFM_SPM_LOG_LEVEL value!"
#endif

#if (TFM_SPM_LOG_LEVEL == TFM_SPM_LOG_LEVEL_DEBUG)
#define SPMLOG_DBGMSGVAL(msg, val) spm_log_msgval(msg, sizeof(msg), val)
#define SPMLOG_DBGMSG(msg) tfm_hal_output_spm_log(msg, sizeof(msg))
#else
#define SPMLOG_DBGMSGVAL(msg, val)
#define SPMLOG_DBGMSG(msg)
#endif

#if (TFM_SPM_LOG_LEVEL >= TFM_SPM_LOG_LEVEL_INFO)
#define SPMLOG_INFMSGVAL(msg, val) spm_log_msgval(msg, sizeof(msg), val)
#define SPMLOG_INFMSG(msg) tfm_hal_output_spm_log(msg, sizeof(msg))
#else
#define SPMLOG_INFMSGVAL(msg, val)
#define SPMLOG_INFMSG(msg)
#endif

#if (TFM_SPM_LOG_LEVEL >= TFM_SPM_LOG_LEVEL_ERROR)
#define SPMLOG_ERRMSGVAL(msg, val) spm_log_msgval(msg, sizeof(msg), val)
#define SPMLOG_ERRMSG(msg) tfm_hal_output_spm_log(msg, sizeof(msg))
#else
#define SPMLOG_ERRMSGVAL(msg, val)
#define SPMLOG_ERRMSG(msg)
#endif

/**
 * \brief SPM output API to convert digit number into HEX string and call the
 *        HAL API tfm_hal_output_spm_log.
 *
 * \param[in]  msg    A string message
 * \param[in]  len    The length of the message
 * \param[in]  value  A value need to be output
 *
 * \retval >=0        Number of chars output.
 * \retval <0         TFM HAL error code.
 */
int32_t spm_log_msgval(const char *msg, size_t len, uint32_t value);

#endif /* __TFM_SPM_LOG_H__ */
