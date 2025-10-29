/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PS_TEST_SERVICE_API_H__
#define __TFM_PS_TEST_SERVICE_API_H__

#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Requests the PS Test Service to call ps_system_prepare().
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t tfm_ps_test_system_prepare(void);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PS_TEST_SERVICE_API_H__ */
