/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa_proxy_shared_mem_mngr.h"
#include "region_defs.h"
#include "psa/service.h"
#ifdef PSA_PROXY_ADDR_TRANSLATION
#include "tfm_plat_psa_proxy_addr_trans.h"
#endif

/* If a dedicated region used for memory sharing maximum buffer size calculated
 * here. Otherwise the buffer size must be defined.
 */
#ifdef PSA_PROXY_SHARED_MEMORY_SIZE
#define SHARED_BUFFER_SIZE (PSA_PROXY_SHARED_MEMORY_SIZE - \
                           sizeof(struct ns_mailbox_queue_t) - \
                           (sizeof(psa_invec) * PSA_MAX_IOVEC)- \
                           (sizeof(psa_outvec) * PSA_MAX_IOVEC))
#else
#ifndef SHARED_BUFFER_SIZE
#error "PSA_PROXY_SHARED_MEMORY_SIZE or SHARED_BUFFER_SIZE should be defined"
#endif
#endif

struct shared_mem_t {
    struct ns_mailbox_queue_t ns_mailbox_queue;
    psa_invec in_vec[PSA_MAX_IOVEC];
    psa_outvec out_vec[PSA_MAX_IOVEC];
    uint8_t buffer[SHARED_BUFFER_SIZE];
};

#ifdef PSA_PROXY_SHARED_MEMORY_BASE
/* If a dedicated region used for memory sharing the shared_mem variable must
 * be allocated into it.
 * If compiled with gcc the whole section zero initialized, even if the
 * variable initialized here with some other value.
 */
__attribute__((section("PSA_PROXY_SHARED_MEMORY_SECTION")))
#endif
struct shared_mem_t shared_mem;

uint32_t shared_mem_buffer_actual_size = 0;

static psa_status_t write_input_param_into_shared_mem(uint32_t param_num,
                                                      const psa_msg_t *msg)
{
    const void *buff_input_ptr;

    if (shared_mem_buffer_actual_size + msg->in_size[param_num] <=
        SHARED_BUFFER_SIZE) {
        buff_input_ptr = &(shared_mem.buffer[shared_mem_buffer_actual_size]);

        psa_read(msg->handle,
                 param_num,
                 (void *) buff_input_ptr,
                 msg->in_size[param_num]);
        shared_mem_buffer_actual_size += msg->in_size[param_num];

        shared_mem.in_vec[param_num].base = buff_input_ptr;
        shared_mem.in_vec[param_num].len = msg->in_size[param_num];

        return PSA_SUCCESS;
    } else {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }
}

static psa_status_t allocate_output_param_in_shared_mem(uint32_t param_num,
                                                        const psa_msg_t *msg)
{
    void * buff_output_ptr;

    if (shared_mem_buffer_actual_size + msg->out_size[param_num] <=
        SHARED_BUFFER_SIZE) {
        buff_output_ptr = &(shared_mem.buffer[shared_mem_buffer_actual_size]);

        shared_mem_buffer_actual_size += msg->out_size[param_num];

        shared_mem.out_vec[param_num].base = buff_output_ptr;
        shared_mem.out_vec[param_num].len = msg->out_size[param_num];

        return PSA_SUCCESS;
    } else {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }
}

static void clear_shared_mem_buffer(void)
{
    int32_t i;

    shared_mem_buffer_actual_size = 0;

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
        shared_mem.in_vec[i].base = NULL;
        shared_mem.in_vec[i].len = 0;
        shared_mem.out_vec[i].base = NULL;
        shared_mem.out_vec[i].len = 0;
    }
}

#ifdef PSA_PROXY_ADDR_TRANSLATION
static void translate_shared_mem_addrs_to_send_msg(
        struct psa_client_params_t* forward_params)
{
    int32_t i;

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
        shared_mem.in_vec[i].base = translate_addr_from_host_to_se(
                                            (void*)shared_mem.in_vec[i].base);
        shared_mem.out_vec[i].base = translate_addr_from_host_to_se(
                                            shared_mem.out_vec[i].base);
    }

    forward_params->psa_call_params.in_vec = translate_addr_from_host_to_se(
                                                        shared_mem.in_vec);
    forward_params->psa_call_params.out_vec = translate_addr_from_host_to_se(
                                                        shared_mem.out_vec);
}

static void translate_shared_mem_addrs_to_write_back_results(void)
{
    int32_t i;

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
        shared_mem.in_vec[i].base = translate_addr_from_se_to_host(
                                            (void*)shared_mem.in_vec[i].base);
        shared_mem.out_vec[i].base = translate_addr_from_se_to_host(
                                            shared_mem.out_vec[i].base);
    }

}
#endif

struct ns_mailbox_queue_t * psa_proxy_get_ns_mailbox_queue(void)
{
    return &(shared_mem.ns_mailbox_queue);
}

psa_status_t psa_proxy_put_msg_into_shared_mem(
        const psa_msg_t* msg,
        struct psa_client_params_t* forward_params)
{
    psa_status_t status;
    uint32_t i;
    size_t in_vec_len = 0;
    size_t out_vec_len = 0;

    clear_shared_mem_buffer();

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
        if (msg->in_size[i] > 0) {
            status = write_input_param_into_shared_mem(i, msg);
            if ( status != PSA_SUCCESS ) {
                return status;
            }
            in_vec_len = i + 1;
        }
    }

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
        if (msg->out_size[i] > 0) {
            status = allocate_output_param_in_shared_mem(i, msg);
            if ( status != PSA_SUCCESS ) {
                return status;
            }
            out_vec_len = i + 1;
        }
    }

    forward_params->psa_call_params.in_vec = shared_mem.in_vec;
    forward_params->psa_call_params.in_len = in_vec_len;
    forward_params->psa_call_params.out_vec = shared_mem.out_vec;
    forward_params->psa_call_params.out_len = out_vec_len;

#ifdef PSA_PROXY_ADDR_TRANSLATION
    translate_shared_mem_addrs_to_send_msg(forward_params);
#endif

    return PSA_SUCCESS;
}

void psa_proxy_write_back_results_from_shared_mem(const psa_msg_t* msg)
{
    uint32_t i;

#ifdef PSA_PROXY_ADDR_TRANSLATION
    translate_shared_mem_addrs_to_write_back_results();
#endif

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
       if (shared_mem.out_vec[i].len > 0) {
           psa_write(msg->handle,
                     i,
                     shared_mem.out_vec[i].base,
                     shared_mem.out_vec[i].len);
       }
   }
}
