/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_SVCALLS_H__
#define __TFM_SVCALLS_H__

#include <stdint.h>

/**
 * \brief The C source of SVCall handlers
 *
 * \param[in] msp               MSP at SVCall entry.
 * \param[in] exc_return        EXC_RETURN value of the SVC.
 * \param[in] psp               PSP at SVCall entry.

 * \returns                     EXC_RETURN value indicates where to return.
 */
uint32_t tfm_core_svc_handler(uint32_t *msp, uint32_t exc_return,
                              uint32_t *psp);

#endif
