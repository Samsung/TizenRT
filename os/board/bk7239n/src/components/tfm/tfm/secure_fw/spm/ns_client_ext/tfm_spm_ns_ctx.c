/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_nspm.h"
#include "tfm_ns_ctx.h"
#include "tfm_ns_client_ext.h"

#define DEFAULT_NS_CLIENT_ID ((int32_t)-1)

int32_t tfm_nspm_get_current_client_id(void)
{
#ifdef TFM_NS_MANAGE_NSID
    int32_t client_id;
    client_id = get_nsid_from_active_ns_ctx();
    return (client_id < 0 ? client_id:TFM_NS_CLIENT_INVALID_ID);
#else
    return DEFAULT_NS_CLIENT_ID;
#endif
}

void tfm_nspm_ctx_init(void)
{
#ifdef TFM_NS_MANAGE_NSID
    if (!init_ns_ctx()) {
        tfm_core_panic();
    }
#endif
}
