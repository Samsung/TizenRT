/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SP_LOG_H__
#define __TFM_SP_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* The Secure Partition log levels */
#define TFM_PARTITION_LOG_LEVEL_DEBUG     3  /* All log APIs output */
#define TFM_PARTITION_LOG_LEVEL_INFO      2  /*
                                              * All log APIs output except
                                              * LOG_DBGFMT
                                              */
#define TFM_PARTITION_LOG_LEVEL_ERROR     1  /*
                                              * Only LOG_ERRFMT APIs output.
                                              */
#define TFM_PARTITION_LOG_LEVEL_SILENCE   0  /* All log APIs are suppressed */

#ifndef TFM_PARTITION_LOG_LEVEL
#error "TFM_PARTITION_LOG_LEVEL not defined!"
#endif

#if (TFM_PARTITION_LOG_LEVEL > TFM_PARTITION_LOG_LEVEL_DEBUG || \
     TFM_PARTITION_LOG_LEVEL < TFM_PARTITION_LOG_LEVEL_SILENCE)
#error "Incorrect TFM_PARTITION_LOG_LEVEL value!"
#endif

#if (TFM_PARTITION_LOG_LEVEL == TFM_PARTITION_LOG_LEVEL_DEBUG)
#define LOG_DBGFMT(...) tfm_sp_log_printf(__VA_ARGS__)
#else
#define LOG_DBGFMT(...)
#endif

#if (TFM_PARTITION_LOG_LEVEL >= TFM_PARTITION_LOG_LEVEL_INFO)
#define LOG_INFFMT(...) tfm_sp_log_printf(__VA_ARGS__)
#else
#define LOG_INFFMT(...)
#endif

#if (TFM_PARTITION_LOG_LEVEL >= TFM_PARTITION_LOG_LEVEL_ERROR)
#define LOG_ERRFMT(...) tfm_sp_log_printf(__VA_ARGS__)
#else
#define LOG_ERRFMT(...)
#endif

/**
 * \brief Print log messages
 *
 * \param[in]   fmt     Formatted string
 * \param[in]   ...     Variable length argument
 *
 * \retval >= 0         Number of chars printed
 * \retval < 0          TF-M HAL error code
 *
 * \note                This function has the similar input argument format as
 *                      the 'printf' function. But it supports only some basic
 *                      formats like 'sdicpuxX' and '%'. It will output
 *                      "[Unsupported Tag]" when none of the above formats match
 *
 * \details             The following output formats are supported.
 *                      %s - string
 *                      %d - decimal signed integer (same for %i)
 *                      %u - decimal unsigned integer
 *                      %x - hex in lowercase
 *                      %X - hex in uppercase
 *                      %p - hex address of a pointer in lowercase
 *                      %c - character
 *                      %% - the '%' symbol
 */
int tfm_sp_log_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_SP_LOG_H__ */
