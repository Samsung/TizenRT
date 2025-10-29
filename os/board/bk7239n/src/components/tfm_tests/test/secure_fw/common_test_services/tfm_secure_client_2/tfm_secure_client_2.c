/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_secure_client_2_api.h"
#include "psa/internal_trusted_storage.h"
#include "psa/crypto.h"

#ifdef TFM_PSA_API
#include "psa/service.h"
#include "psa_manifest/tfm_secure_client_2.h"
#else
#include "psa/client.h"
#endif

#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
/* Define the global variable for the TFM_SECURE_CLIENT_2_SID service. */
uint8_t secure_client_2_data;
uint8_t *secure_client_2_data_p = &secure_client_2_data;
#endif

#if defined(TFM_PARTITION_CRYPTO) || defined(FORWARD_PROT_MSG)
/**
 * \brief Tests calling psa_destroy_key() with the supplied key handle.
 *
 * \param[in] arg      Pointer to key handle
 * \param[in] arg_len  Length of arg in bytes
 *
 * \return Returns test result as specified in \ref psa_status_t
 */
static psa_status_t secure_client_2_test_crypto_access_ctrl(const void *arg,
                                                            size_t arg_len)
{
    psa_key_handle_t key_handle;

    if (arg_len != sizeof(key_handle)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    key_handle = *((psa_key_handle_t *)arg);

    /* Attempt to destroy the key handle */
    return psa_destroy_key(key_handle);
}
#endif /* defined(TFM_PARTITION_CRYPTO) || defined(FORWARD_PROT_MSG) */

#if defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) || defined(FORWARD_PROT_MSG)
/**
 * \brief Tests calling psa_its_get() with the supplied uid.
 *
 * \param[in] arg      Pointer to uid
 * \param[in] arg_len  Length of arg in bytes
 *
 * \return Returns test result as specified in \ref psa_status_t
 */
static psa_status_t secure_client_2_test_its_access_ctrl(const void *arg,
                                                         size_t arg_len)
{
    psa_storage_uid_t uid;
    size_t p_data_length;
    uint8_t data[1];

    if (arg_len != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    uid = *((psa_storage_uid_t *)arg);

    /* Attempt to get one byte from the UID and return the resulting status */
    return psa_its_get(uid, 0, sizeof(data), data, &p_data_length);
}
#endif /* defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) || defined(FORWARD_PROT_MSG) */

#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
/**
 * \brief Tests calling psa_write() to write a pointer to outvec.
 *
 * \param[in] msg    Pointer to the message of the test function
 */
static void secure_client_2_test_retrieve_app_mem(const psa_msg_t *msg)
{
    if (msg->out_size[0] != 0) {
        /*
         * Write a pointer to outvec. The pointer points to a RW memory
         * address in IPC service partition.
         */
        psa_write(msg->handle, 0, &secure_client_2_data_p,
                  sizeof(secure_client_2_data_p));
    }
}
#endif /* TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM */

/**
 * \brief Calls the test function with the supplied ID and returns the result
 *        from the test function.
 *
 * \param[in] id       The ID of the test function
 * \param[in] arg      Pointer to argument to pass to test function
 * \param[in] arg_len  Length of argument in bytes
 *
 * \return Returns test result as specified in \ref psa_status_t
 */
static psa_status_t secure_client_2_dispatch(int32_t id, const void *arg,
                                             size_t arg_len)
{
    switch (id) {
#if defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) || defined(FORWARD_PROT_MSG)
    case TFM_SECURE_CLIENT_2_ID_ITS_ACCESS_CTRL:
        return secure_client_2_test_its_access_ctrl(arg, arg_len);
#endif
#if defined(TFM_PARTITION_CRYPTO) || defined(FORWARD_PROT_MSG)
    case TFM_SECURE_CLIENT_2_ID_CRYPTO_ACCESS_CTRL:
        return secure_client_2_test_crypto_access_ctrl(arg, arg_len);
#endif
    default:
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
}

#ifdef TFM_PSA_API
#define SECURE_CLIENT_2_MAX_ARG_LEN 8U

static psa_status_t tfm_secure_client_2_handle_msg(const psa_msg_t *msg)
{
    psa_status_t status = PSA_ERROR_PROGRAMMER_ERROR;
    size_t len;
    char arg[SECURE_CLIENT_2_MAX_ARG_LEN] __attribute__((__aligned__(8)));

    /* Decode the message */
    switch (msg->type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        status = PSA_SUCCESS;
        break;
#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
    case TFM_SECURE_CLIENT_2_ID_RETRIEVE_APP_MEM:
        secure_client_2_test_retrieve_app_mem(msg);
        status = PSA_SUCCESS;
        break;
#endif
    default:
        len = psa_read(msg->handle, 0, arg, SECURE_CLIENT_2_MAX_ARG_LEN);
        status = secure_client_2_dispatch(msg->type, arg, len);
        break;
    }
    return status;
}

#if (TFM_SP_SECURE_CLIENT_2_MODEL_IPC == 1)
void tfm_secure_client_2_init(void)
{
    psa_msg_t msg;

    while (1) {
        (void)psa_wait(TFM_SECURE_CLIENT_2_SIGNAL, PSA_BLOCK);
        if (psa_get(TFM_SECURE_CLIENT_2_SIGNAL, &msg) != PSA_SUCCESS) {
            continue;
        }
        psa_reply(msg.handle, tfm_secure_client_2_handle_msg(&msg));
    }
}

#elif (TFM_SP_SECURE_CLIENT_2_MODEL_SFN == 1)
psa_status_t tfm_secure_client_2_sfn(const psa_msg_t* msg)
{
    return tfm_secure_client_2_handle_msg(msg);
}

#endif /* TFM_SP_SECURE_CLIENT_2_MODEL_IPC == 1 */

#else /* TFM_PSA_API */
psa_status_t tfm_secure_client_2_init(void)
{
    return PSA_SUCCESS;
}

psa_status_t tfm_secure_client_2_call(psa_invec *in_vec, size_t in_len,
                                      psa_outvec *out_vec, size_t out_len)
{
    int32_t id;

    (void)out_vec;

    if (in_len != 2 || out_len != 0 || in_vec[0].len != sizeof(id)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    id = *((int32_t *)in_vec[0].base);

    return secure_client_2_dispatch(id, in_vec[1].base, in_vec[1].len);
}
#endif /* TFM_PSA_API */
