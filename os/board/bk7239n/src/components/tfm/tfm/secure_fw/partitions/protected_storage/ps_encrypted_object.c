/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "ps_encrypted_object.h"

#include <stddef.h>

#include "crypto/ps_crypto_interface.h"
#include "psa/internal_trusted_storage.h"
#include "tfm_memory_utils.h"
#include "ps_object_defs.h"
#include "ps_utils.h"

/* Gets the size of data to encrypt */
#define PS_ENCRYPT_SIZE(plaintext_size) \
    ((plaintext_size) + PS_OBJECT_HEADER_SIZE - sizeof(union ps_crypto_t))

#define PS_OBJECT_START_POSITION  0

/* Buffer to store the maximum encrypted object */
/* FIXME: Do partial encrypt/decrypt to reduce the size of internal buffer */
#define PS_MAX_ENCRYPTED_OBJ_SIZE PS_ENCRYPT_SIZE(PS_MAX_OBJECT_DATA_SIZE)

/* FIXME: add the tag length to the crypto buffer size to account for the tag
 * being appended to the ciphertext by the crypto layer.
 */
#define PS_TAG_IV_LEN_MAX   ((PS_TAG_LEN_BYTES > PS_IV_LEN_BYTES) ? \
                             PS_TAG_LEN_BYTES : PS_IV_LEN_BYTES)
#define PS_CRYPTO_BUF_LEN (PS_MAX_ENCRYPTED_OBJ_SIZE + PS_TAG_IV_LEN_MAX)

static uint8_t ps_crypto_buf[PS_CRYPTO_BUF_LEN];

static psa_status_t fill_key_label(struct ps_object_t *obj, size_t *length)
{
    psa_storage_uid_t uid = obj->header.crypto.ref.uid;
    int32_t client_id = obj->header.crypto.ref.client_id;

    if (PS_CRYPTO_BUF_LEN < (sizeof(client_id) + sizeof(uid))) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    tfm_memcpy(ps_crypto_buf, &client_id, sizeof(client_id));
    tfm_memcpy(ps_crypto_buf + sizeof(client_id), &uid, sizeof(uid));

    *length = sizeof(client_id) + sizeof(uid);

    return PSA_SUCCESS;
}

/**
 * \brief Performs authenticated decryption on object data, with the header as
 *        the associated data.
 *
 * \param[in]  fid       File ID
 * \param[in]  cur_size  Size of the object data to decrypt
 * \param[in,out] obj    Pointer to the object structure to authenticate and
 *                       fill in with the decrypted data. The tag of the object
 *                       is the one stored in the object table for the given
 *                       File ID.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
static psa_status_t ps_object_auth_decrypt(uint32_t fid,
                                           uint32_t cur_size,
                                           struct ps_object_t *obj)
{
    psa_status_t err;
    uint8_t *p_obj_data = (uint8_t *)&obj->header.info;
    size_t out_len, label_length;

    err = fill_key_label(obj, &label_length);
    if (err != PSA_SUCCESS) {
        return err;
    }

    err = ps_crypto_setkey(ps_crypto_buf, label_length);
    if (err != PSA_SUCCESS) {
        return err;
    }

    (void)tfm_memcpy(ps_crypto_buf, p_obj_data, cur_size);

    /* Use File ID as a part of the associated data to authenticate
     * the object in the FS. The tag will be stored in the object table and
     * not as a part of the object's data stored in the FS.
     */

    err = ps_crypto_auth_and_decrypt(&obj->header.crypto,
                                     (const uint8_t *)&fid,
                                     sizeof(fid),
                                     ps_crypto_buf,
                                     cur_size,
                                     p_obj_data,
                                     sizeof(*obj) - sizeof(obj->header.crypto),
                                     &out_len);
    if (err != PSA_SUCCESS || out_len != cur_size) {
        (void)ps_crypto_destroykey();
        return PSA_ERROR_GENERIC_ERROR;
    }

    return ps_crypto_destroykey();
}

/**
 * \brief Performs authenticated encryption on object data, with the header as
 *        the associated data.
 *
 * \param[in]  fid       File ID
 * \param[in]  cur_size  Size of the object data to encrypt
 * \param[out] obj       Pointer to the object structure to authenticate and
 *                       fill in with the encrypted data.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
static psa_status_t ps_object_auth_encrypt(uint32_t fid,
                                           uint32_t cur_size,
                                           struct ps_object_t *obj)
{
    psa_status_t err;
    uint8_t *p_obj_data = (uint8_t *)&obj->header.info;
    size_t out_len, label_length;

    err = fill_key_label(obj, &label_length);
    if (err != PSA_SUCCESS) {
        return err;
    }

    err = ps_crypto_setkey(ps_crypto_buf, label_length);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Get a new IV for each encryption */
    err = ps_crypto_get_iv(&obj->header.crypto);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Use File ID as a part of the associated data to authenticate
     * the object in the FS. The tag will be stored in the object table and
     * not as a part of the object's data stored in the FS.
     */

    err = ps_crypto_encrypt_and_tag(&obj->header.crypto,
                                    (const uint8_t *)&fid,
                                    sizeof(fid),
                                    p_obj_data,
                                    cur_size,
                                    ps_crypto_buf,
                                    sizeof(ps_crypto_buf),
                                    &out_len);
    if (err != PSA_SUCCESS || out_len != cur_size) {
        (void)ps_crypto_destroykey();
        return PSA_ERROR_GENERIC_ERROR;
    }

    (void)tfm_memcpy(p_obj_data, ps_crypto_buf, cur_size);

    return ps_crypto_destroykey();
}

psa_status_t ps_encrypted_object_read(uint32_t fid, struct ps_object_t *obj)
{
    psa_status_t err;
    uint32_t decrypt_size;
    size_t data_length;

    /* Read the encrypted object from the persistent area. The data stored via
     * ITS interface of this `fid` is the encrypted object together with the
     * `IV`.
     * In the psa_its_get, the buffer size is not checked. Check the buffer size
     * here.
     */
    if (sizeof(ps_crypto_buf) < PS_MAX_ENCRYPTED_OBJ_SIZE + PS_IV_LEN_BYTES) {
        return PSA_ERROR_GENERIC_ERROR;
    }
    err = psa_its_get(fid, PS_OBJECT_START_POSITION,
                      PS_MAX_ENCRYPTED_OBJ_SIZE + PS_IV_LEN_BYTES,
                      (void *)ps_crypto_buf,
                      &data_length);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Get the decrypt size. IV is also stored by ITS service. It is at the end
     * of the read out data. Toolchains may add padding byte after iv array in
     * crypto.ref structure. Separate the copies of header.info and iv array to
     * skip the padding byte.
     */
    decrypt_size = data_length - sizeof(obj->header.crypto.ref.iv);
    tfm_memcpy(&obj->header.info, ps_crypto_buf, decrypt_size);
    tfm_memcpy(obj->header.crypto.ref.iv,
               ps_crypto_buf + decrypt_size,
               sizeof(obj->header.crypto.ref.iv));

    /* Decrypt the object data */
    err = ps_object_auth_decrypt(fid, decrypt_size, obj);
    if (err != PSA_SUCCESS) {
        return err;
    }

    return PSA_SUCCESS;
}

psa_status_t ps_encrypted_object_write(uint32_t fid, struct ps_object_t *obj)
{
    psa_status_t err;
    uint32_t wrt_size;

    wrt_size = PS_ENCRYPT_SIZE(obj->header.info.current_size);

    /* Authenticate and encrypt the object */
    err = ps_object_auth_encrypt(fid, wrt_size, obj);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* The IV will also be stored. The encrypted data is stored in ps_crypto_buf
     * now. Append the value of the 'iv' to the end of the encrypted data.
     * Toolchains may add padding byte after iv array in crypto.ref structure.
     * The padding byte shall not be written into the storage area.
     */
    (void)tfm_memcpy(ps_crypto_buf + wrt_size,
                     obj->header.crypto.ref.iv,
                     sizeof(obj->header.crypto.ref.iv));
    wrt_size += sizeof(obj->header.crypto.ref.iv);

    /* Write the encrypted object to the persistent area. The tag values is not
     * copied as it is stored in the object table.
     */
    return psa_its_set(fid, wrt_size, (const void *)ps_crypto_buf,
                       PSA_STORAGE_FLAG_NONE);
}
