/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "tfm_platform_api.h"
#include "tfm_ns_interface.h"
#include "tfm_veneers.h"

enum tfm_platform_err_t tfm_platform_system_reset(void)
{
    return (enum tfm_platform_err_t) tfm_ns_interface_dispatch(
                                (veneer_fn)tfm_platform_sp_system_reset_veneer,
                                0,
                                0,
                                0,
                                0);
}

enum tfm_platform_err_t
tfm_platform_ioctl(tfm_platform_ioctl_req_t request,
                   psa_invec *input, psa_outvec *output)
{
    tfm_platform_ioctl_req_t req = request;
    struct psa_invec in_vec[2];
    size_t inlen, outlen;

    in_vec[0].base = &req;
    in_vec[0].len = sizeof(req);
    if (input != NULL) {
        in_vec[1].base = input->base;
        in_vec[1].len = input->len;
        inlen = 2;
    } else {
        inlen = 1;
    }

    if (output != NULL) {
        outlen = 1;
    } else {
        outlen = 0;
    }

    return (enum tfm_platform_err_t) tfm_ns_interface_dispatch(
                                (veneer_fn)tfm_platform_sp_ioctl_veneer,
                                (uint32_t)in_vec, (uint32_t)inlen,
                                (uint32_t)output, (uint32_t)outlen);
}

