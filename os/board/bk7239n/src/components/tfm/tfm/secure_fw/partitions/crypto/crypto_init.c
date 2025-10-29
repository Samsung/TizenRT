/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_mbedcrypto_include.h"

#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"
#include "tfm_sp_log.h"

/*
 * \brief This Mbed TLS include is needed to initialise the memory allocator
 *        inside the Mbed TLS layer of Mbed Crypto
 */
#include "mbedtls/memory_buffer_alloc.h"

#ifdef CRYPTO_NV_SEED
#include "tfm_plat_crypto_nv_seed.h"
#endif /* CRYPTO_NV_SEED */

#ifndef TFM_PSA_API
#include "tfm_secure_api.h"
#endif

#ifdef CRYPTO_HW_ACCELERATOR
#include "crypto_hw.h"
#endif /* CRYPTO_HW_ACCELERATOR */

#ifdef TFM_PSA_API
#include "psa/service.h"
#include "psa_manifest/tfm_crypto.h"
#include "tfm_memory_utils.h"

extern  int printf(const char *fmt, ...);
/**
 * \brief Table containing all the Uniform Signature API exposed
 *        by the TF-M Crypto partition
 */
static const tfm_crypto_us_t sfid_func_table[TFM_CRYPTO_SID_MAX] = {
#define X(api_name) api_name,
LIST_TFM_CRYPTO_UNIFORM_SIGNATURE_API
#undef X
};

/**
 * \brief Aligns a value x up to an alignment a.
 */
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

/**
 * \brief Maximum alignment required by any iovec parameters to the TF-M Crypto
 *        partition.
 */
#define TFM_CRYPTO_IOVEC_ALIGNMENT (4u)

/**
 * \brief Default size of the internal scratch buffer used for IOVec allocations
 *        in bytes
 */
#ifndef TFM_CRYPTO_IOVEC_BUFFER_SIZE
#error TFM_CRYPTO_IOVEC_BUFFER_SIZE is not defined
#endif

/**
 * \brief Internal scratch used for IOVec allocations
 *
 */
static struct tfm_crypto_scratch {
    __attribute__((__aligned__(TFM_CRYPTO_IOVEC_ALIGNMENT)))
    uint8_t buf[TFM_CRYPTO_IOVEC_BUFFER_SIZE];
    uint32_t alloc_index;
    int32_t owner;
} scratch = {.buf = {0}, .alloc_index = 0};

static psa_status_t tfm_crypto_set_scratch_owner(int32_t id)
{
    scratch.owner = id;
    return PSA_SUCCESS;
}

static psa_status_t tfm_crypto_get_scratch_owner(int32_t *id)
{
    *id = scratch.owner;
    return PSA_SUCCESS;
}

static psa_status_t tfm_crypto_alloc_scratch(size_t requested_size, void **buf)
{
    /* Ensure alloc_index remains aligned to the required iovec alignment */
    requested_size = ALIGN(requested_size, TFM_CRYPTO_IOVEC_ALIGNMENT);

    if (requested_size > (sizeof(scratch.buf) - scratch.alloc_index)) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    /* Compute the pointer to the allocated space */
    *buf = (void *)&scratch.buf[scratch.alloc_index];

    /* Increase the allocated size */
    scratch.alloc_index += requested_size;

    return PSA_SUCCESS;
}

void tfm_crypto_clear_scratch(void)
{
    scratch.owner = 0;
    (void)tfm_memset(scratch.buf, 0, scratch.alloc_index);
    scratch.alloc_index = 0;
}

static psa_status_t tfm_crypto_call_srv(const psa_msg_t *msg)
{
    psa_status_t status = PSA_SUCCESS;
    size_t in_len = PSA_MAX_IOVEC, out_len = PSA_MAX_IOVEC, i;
    psa_invec in_vec[PSA_MAX_IOVEC] = { {NULL, 0} };
    psa_outvec out_vec[PSA_MAX_IOVEC] = { {NULL, 0} };
    struct tfm_crypto_pack_iovec iov = {0};
    void *alloc_buf_ptr = NULL;

    /* Check the number of in_vec filled */
    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0)) {
        in_len--;
    }

    /* There will always be a tfm_crypto_pack_iovec in the first iovec */
    if (in_len < 1) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (psa_read(msg->handle, 0, &iov, sizeof(iov)) != sizeof(iov)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (iov.srv_id >= TFM_CRYPTO_SID_MAX) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Initialise the first iovec with the IOV read when parsing */
    in_vec[0].base = &iov;
    in_vec[0].len = sizeof(struct tfm_crypto_pack_iovec);

    /* Alloc/read from the second element as the first is read when parsing */
    for (i = 1; i < in_len; i++) {
        /* Allocate necessary space in the internal scratch */
        status = tfm_crypto_alloc_scratch(msg->in_size[i], &alloc_buf_ptr);
        if (status != PSA_SUCCESS) {
            tfm_crypto_clear_scratch();
            return status;
        }
        /* Read from the IPC framework inputs into the scratch */
        in_vec[i].len =
                       psa_read(msg->handle, i, alloc_buf_ptr, msg->in_size[i]);
        /* Populate the fields of the input to the secure function */
        in_vec[i].base = alloc_buf_ptr;
    }

    /* Check the number of out_vec filled */
    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0)) {
        out_len--;
    }

    for (i = 0; i < out_len; i++) {
        /* Allocate necessary space for the output in the internal scratch */
        status = tfm_crypto_alloc_scratch(msg->out_size[i], &alloc_buf_ptr);
        if (status != PSA_SUCCESS) {
            tfm_crypto_clear_scratch();
            return status;
        }
        /* Populate the fields of the output to the secure function */
        out_vec[i].base = alloc_buf_ptr;
        out_vec[i].len = msg->out_size[i];
    }

    /* Set the owner of the data in the scratch */
    (void)tfm_crypto_set_scratch_owner(msg->client_id);

    /* Call the uniform signature API */

    //printf("iov.srv_id = %d\r\n",iov.srv_id);
    status = sfid_func_table[iov.srv_id](in_vec, in_len, out_vec, out_len);

    /* Write into the IPC framework outputs from the scratch */
    for (i = 0; i < out_len; i++) {
        psa_write(msg->handle, i, out_vec[i].base, out_vec[i].len);
    }

    /* Clear the allocated internal scratch before returning */
    tfm_crypto_clear_scratch();

    return status;
}

#endif /* TFM_PSA_API */

/**
 * \brief Default value for the size of the static buffer used by Mbed
 *        Crypto for its dynamic allocations
 */
#ifndef TFM_CRYPTO_ENGINE_BUF_SIZE
#error TFM_CRYPTO_ENGINE_BUF_SIZE is not defined
#endif

/**
 * \brief Static buffer to be used by Mbed Crypto for memory allocations
 *
 */
static uint8_t mbedtls_mem_buf[TFM_CRYPTO_ENGINE_BUF_SIZE] = {0};

static psa_status_t tfm_crypto_engine_init(void)
{

#ifdef CRYPTO_NV_SEED
#ifdef TFM_PSA_API
    if (tfm_plat_crypto_provision_entropy_seed() != TFM_CRYPTO_NV_SEED_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    LOG_INFFMT("[Crypto] ");
    LOG_INFFMT("TF-M in library mode uses a dummy NV seed. ");
    LOG_INFFMT("This is not suitable for production! ");
    LOG_INFFMT("This device is NOT SECURE");
    LOG_INFFMT("\r\n");
#endif /* TFM_PSA_API */
#endif /* CRYPTO_NV_SEED */

    /* Initialise the Mbed Crypto memory allocator to use static
     * memory allocation from the provided buffer instead of using
     * the heap
     */
    mbedtls_memory_buffer_alloc_init(mbedtls_mem_buf,
                                     TFM_CRYPTO_ENGINE_BUF_SIZE);

    /* Initialise the crypto accelerator if one is enabled */
#ifdef CRYPTO_HW_ACCELERATOR
    if (crypto_hw_accelerator_init() != 0) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }
#endif /* CRYPTO_HW_ACCELERATOR */

    /* Previous function does not return any value, so just call the
     * initialisation function of the Mbed Crypto layer
     */
    return psa_crypto_init();
}

static psa_status_t tfm_crypto_module_init(void)
{
    /* Init the Alloc module */
    return tfm_crypto_init_alloc();
}

psa_status_t tfm_crypto_get_caller_id(int32_t *id)
{
#ifdef TFM_PSA_API
    return tfm_crypto_get_scratch_owner(id);
#else
    int32_t res;

    res = tfm_core_get_caller_client_id(id);
    if (res != TFM_SUCCESS) {
        return PSA_ERROR_NOT_PERMITTED;
    } else {
        return PSA_SUCCESS;
    }
#endif
}

psa_status_t tfm_crypto_init(void)
{
    psa_status_t status;

    /* Initialise other modules of the service */
    status = tfm_crypto_module_init();
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Initialise the engine layer */
    return tfm_crypto_engine_init();
}

#ifdef TFM_PSA_API
psa_status_t tfm_crypto_sfn(const psa_msg_t *msg)
{
    /* Process the message type */
    switch (msg->type) {
    case PSA_IPC_CALL:
        return tfm_crypto_call_srv(msg);
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_GENERIC_ERROR;
}
#endif

/*
*	For codesize optimization, the functions in  LIST_TFM_CRYPTO_UNIFORM_SIGNATURE_API   have been optimized.
*	If you call the optimized PSA API in  non secure world, UART1 will output log 'please realize this func sid = xx',
*     and restore the function in LIST_TFM_CRYPTO_UNIFORM_SIGNATURE_API based on the sid value.
*/
psa_status_t tfm_crypto_empty_func(psa_invec in_vec[],
                                   size_t in_len,
                                   psa_outvec out_vec[],
                                   size_t out_len)
{
	const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
	uint32_t srv_id     = iov->srv_id;
	psa_algorithm_t alg = iov->alg;
	printf("Err:must realize this func ");
	printf("sid = %u, alg =%u\r\n", srv_id, alg);

	return 0;
}

