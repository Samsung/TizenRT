/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __RUNTIME_DEFS_H__
#define __RUNTIME_DEFS_H__

#include <stdint.h>

#include "psa/error.h"
#include "psa/service.h"

/* SFN defs */
typedef psa_status_t (*service_fn_t)(psa_msg_t *msg);
typedef psa_status_t (*sfn_init_fn_t)(void);

struct runtime_metadata_t {
    uintptr_t       entry;          /* Entry function invoked by sprt_main */
    uint32_t        n_sfn;          /* Number of Secure FuNctions */
    service_fn_t    sfn_table[];    /* Secure FuNctions Table */
};

#endif /* __RUNTIME_DEFS_H__ */
