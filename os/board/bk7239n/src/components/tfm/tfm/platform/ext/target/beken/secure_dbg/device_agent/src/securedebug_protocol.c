/**
* Copyright (C), 2018-2023, Arm Technology (China) Co., Ltd.
* All rights reserved
*
* The content of this file or document is CONFIDENTIAL and PROPRIETARY
* to Arm Technology (China) Co., Ltd. It is subject to the terms of a
* License Agreement between Licensee and Arm Technology (China) Co., Ltd
* restricting among other things, the use, reproduction, distribution
* and transfer.  Each of the embodiments, including this information and,,
* any derivative work shall retain this copyright notice.
*/
#include "securedebug_internal.h"
#include "dubhe_otp.h"
#include "dubhe_trng.h"

const static uint8_t _g_rsa_pubkey_e[] = {0x01, 0x00, 0x01};

static hal_ret_t sec_debug_check_protocol_data(uint32_t *data, size_t size)
{
    uint32_t calc_checksum = 0;

    if (_PROTOCOL_MAGIC(data) != SEC_DEBUG_PROTOCOL_MAGIC_VALUE) {
        PAL_LOG_ERR("Invlid protocol magic: 0x%x\n", _PROTOCOL_MAGIC(data));
        return HAL_ERR_GENERIC;
    }
    if (size < (uint8_t *)_PROTOCOL_PAYLOAD_ADDR(data) - (uint8_t *)data) {
        PAL_LOG_ERR("Invlid data length: 0x%x, too small!\n", size);
        return HAL_ERR_GENERIC;
    }
    if ((_PROTOCOL_CMD(data) != SEC_DEBUG_CREATE_HANDSHAKE_REQUEST) &&
        (_PROTOCOL_CMD(data) != SEC_DEBUG_PUBKEY_VERIFY_REQUEST) &&
        (_PROTOCOL_CMD(data) != SEC_DEBUG_SIG_VERIFY_AND_EXCUTE_CMD_REQUEST) &&
        (_PROTOCOL_CMD(data) != SEC_DEBUG_DEVICE_TERMINATE_REQUEST)) {
        PAL_LOG_ERR("Invlid protocol cmd: 0x%x\n", _PROTOCOL_CMD(data));
        return HAL_ERR_GENERIC;
    }
    if (_PROTOCOL_LEN(data) != size) {
        PAL_LOG_ERR(
            "Invlid protocol length: 0x%x, not match with data size: 0x%x\n",
            _PROTOCOL_LEN(data),
            size);
        return HAL_ERR_GENERIC;
    }
    calc_checksum = _check_sum((uint8_t *)(&(_PROTOCOL_LEN(data))),
                               size - sizeof(uint32_t) * 2);
    if (calc_checksum != _PROTOCOL_CHECKSUM(data)) {
        PAL_LOG_ERR("Invlid protocol checksum: 0x%x, not match with: 0x%x\n",
                    _PROTOCOL_CHECKSUM(data),
                    calc_checksum);
        return HAL_ERR_GENERIC;
    }
    PAL_LOG_DEBUG("Valid protocol data, cmd: 0x%x\n", _PROTOCOL_CMD(data));
    return HAL_OK;
}

static hal_ret_t sec_debug_protocol_handshake_process(
    uint32_t *data, size_t data_size, debug_protocol_info_t *protocol_info)
{
    hal_ret_t ret                 = HAL_ERR_GENERIC;
    uint32_t *protocol_write_data = NULL;
    uint32_t magic_number         = 0;

    PAL_ASSERT((NULL != data) && (NULL != protocol_info));

    protocol_write_data =
        (uint32_t *)pal_malloc(SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
    if (!protocol_write_data) {
        PAL_LOG_ERR("Malloc %d out of memory!\n",
                    SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
        return HAL_ERR_OUT_OF_MEMORY;
    }

    magic_number = data[0];
    PAL_LOG_DEBUG("HANDSHAKE MAGIC: 0x%x\n", magic_number);
    if (SEC_DEBUG_HAND_SHAKE_MAGIC_VALUE != magic_number) {
        pal_free(protocol_write_data);
        PAL_LOG_ERR("Verify magic_number failed: 0x%x != 0x%x\n",
                    (unsigned int)magic_number,
                    SEC_DEBUG_HAND_SHAKE_MAGIC_VALUE);
        return HAL_ERR_VRFY_FAILED;
    }
     
    PAL_ASSERT(HAL_OTP_MODEL_ID_SIZE == sizeof(protocol_info->model_id));
    ret = arm_ce_get_otp_info(OTP_INFO_MODEL_ID,
                       (uint8_t *)(&protocol_info->model_id),
                       HAL_OTP_MODEL_ID_SIZE,
                       0);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_otp_read failed!\n");
        goto finish;
    }

    pal_memcpy((uint8_t *)(_PROTOCOL_PAYLOAD_ADDR(protocol_write_data)),
               &protocol_info->model_id,
               sizeof(protocol_info->model_id));
    _protocol_init_header(protocol_write_data,
                          sizeof(protocol_info->model_id),
                          SEC_DEBUG_CREATE_HANDSHAKE_RESPONSE);

    ret = hal_channel_write(g_sec_dbg_ctx.channel,
                            (uint8_t *)protocol_write_data,
                            protocol_write_data[2]);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_channel_write failed!\n");
        goto finish;
    }
finish:
    if (protocol_write_data) {
        pal_free(protocol_write_data);
        protocol_write_data = NULL;
    }
    return ret;
}

static hal_ret_t sec_debug_protocol_pubkey_process(
    uint32_t *data, size_t data_size, debug_protocol_info_t *protocol_info)
{
    hal_ret_t ret                       = HAL_ERR_GENERIC;
    uint32_t *protocol_write_data       = NULL;
    uint32_t dgst_sch                   = 0;
    size_t dgst_size                    = 0;
    sec_debug_dgst_result_t dgst_result = {0};
    sec_debug_dgst_result_t dgst_otp    = {0};

    PAL_ASSERT((NULL != data) && (NULL != protocol_info));

    protocol_write_data =
        (uint32_t *)pal_malloc(SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
    if (!protocol_write_data) {
        PAL_LOG_ERR("Malloc %d out of memory!\n",
                    SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
        return HAL_ERR_OUT_OF_MEMORY;
    }

    dgst_sch                                   = data[0];
    protocol_info->pubkey_info.pubkey_dgst_sch = (sec_debug_dgst_sch_t)dgst_sch;
    PAL_LOG_INFO("pubkey_info->pubkey_dgst_sch: %d \n",
                 protocol_info->pubkey_info.pubkey_dgst_sch);

    protocol_info->pubkey_info.pubkey_size = data[1];

    PAL_LOG_INFO("pubkey_info->pubkey_size: %d \n",
                 (unsigned int)protocol_info->pubkey_info.pubkey_size);

    if (protocol_info->pubkey_info.pubkey_size > SEC_DEBUG_MAX_PUBKEY_SIZE) {
        PAL_LOG_ERR("Bad pubkey size: %d\n",
                    (unsigned int)protocol_info->pubkey_info.pubkey_size);
        ret = HAL_ERR_GENERIC;
        goto finish;
    }

    pal_memcpy(protocol_info->pubkey_info.pubkey,
               &data[2],
               protocol_info->pubkey_info.pubkey_size);

    dgst_size = sizeof(dgst_result);
    ret       = hal_crypto_hash(SWITCH_HASH_SCH(dgst_sch),
                          protocol_info->pubkey_info.pubkey,
                          protocol_info->pubkey_info.pubkey_size,
                          dgst_result,
                          &dgst_size);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_crypto_hash failed!\n");
        goto finish;
    }
    if (dgst_size != _SEC_DEBUG_GET_DGST_SIZE(dgst_sch)) {
        PAL_LOG_ERR("hal_crypto_hash return bad digest size: %d!\n", dgst_size);
        ret = HAL_ERR_GENERIC;
        goto finish;
    }

     
    PAL_ASSERT(sizeof(dgst_otp) == HAL_OTP_SEC_DBG_PUBKEY_HASH_SIZE);
    ret = arm_ce_get_otp_info(OTP_INFO_SEC_DBG_PK_HASH,
                       dgst_otp,
                       HAL_OTP_SEC_DBG_PUBKEY_HASH_SIZE,
                       0);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_otp_read failed!\n");
        goto finish;
    }
     
    if (pal_memcmp(dgst_result, dgst_otp, dgst_size) != 0) {
        PAL_LOG_ERR("Verify pubkey hash failed!\n");
        PAL_LOG_DUMP_BUF("HASH calculated:", dgst_result, dgst_size);
        PAL_LOG_DUMP_BUF("HASH in OTP", dgst_otp, dgst_size);
        ret = HAL_ERR_VRFY_FAILED;
        goto finish;
    }

    PAL_LOG_INFO("Verify pubkey hash SUCCESS\n");
     
    ret = arm_ce_seed_read(protocol_info->challenge_data,
                              SEC_DEBUG_PROTOCOL_CHALLENGE_SIZE);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("Generate ramdom data for challenge failed!\n");
        goto finish;
    }

    PAL_LOG_DUMP_BUF("Challenge_data:",
                     protocol_info->challenge_data,
                     SEC_DEBUG_PROTOCOL_CHALLENGE_SIZE);
     
    PAL_ASSERT(sizeof(protocol_info->device_id) == HAL_OTP_DEVICE_ID_SIZE);
    ret = arm_ce_get_otp_info(OTP_INFO_DEV_ID,
                       (uint8_t *)(&protocol_info->device_id),
                       HAL_OTP_DEVICE_ID_SIZE,
                       0);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_otp_read failed!\n");
        goto finish;
    }
     
    *((uint32_t *)_PROTOCOL_PAYLOAD_ADDR(protocol_write_data)) =
        SEC_DEBUG_PROTOCOL_PUBKEY_VERIFY_SUCCESS;
     
    pal_memcpy((uint8_t *)_PROTOCOL_PAYLOAD_ADDR(protocol_write_data) +
                   sizeof(uint32_t),
               &protocol_info->model_id,
               sizeof(protocol_info->model_id));
     
    pal_memcpy((uint8_t *)_PROTOCOL_PAYLOAD_ADDR(protocol_write_data) +
                   sizeof(uint32_t) + sizeof(protocol_info->model_id),
               &protocol_info->device_id,
               sizeof(protocol_info->device_id));

     
    pal_memcpy((uint8_t *)_PROTOCOL_PAYLOAD_ADDR(protocol_write_data) +
                   sizeof(uint32_t) + sizeof(protocol_info->model_id) +
                   sizeof(protocol_info->device_id),
               protocol_info->challenge_data,
               SEC_DEBUG_PROTOCOL_CHALLENGE_SIZE);
     
    _protocol_init_header(protocol_write_data,
                          sizeof(uint32_t) + sizeof(protocol_info->model_id) +
                              sizeof(protocol_info->device_id) +
                              SEC_DEBUG_PROTOCOL_CHALLENGE_SIZE,
                          SEC_DEBUG_PUBKEY_VERIFY_RESPONSE);

    ret = HAL_OK;

finish:
    if (HAL_OK != ret) {
        if (HAL_ERR_VRFY_FAILED == ret) {
            *((uint32_t *)_PROTOCOL_PAYLOAD_ADDR(protocol_write_data)) =
                SEC_DEBUG_PROTOCOL_PUBKEY_VERIFY_FAILED;
        } else {
            *((uint32_t *)_PROTOCOL_PAYLOAD_ADDR(protocol_write_data)) =
                SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
        }
        _protocol_init_header(protocol_write_data,
                              sizeof(uint32_t),
                              SEC_DEBUG_PUBKEY_VERIFY_RESPONSE);
    }

    if (HAL_OK != hal_channel_write(g_sec_dbg_ctx.channel,
                                    (uint8_t *)protocol_write_data,
                                    protocol_write_data[2])) {
        PAL_LOG_ERR("hal_channel_write failed!\n");
    }
    if (protocol_write_data) {
        pal_free(protocol_write_data);
        protocol_write_data = NULL;
    }

    return ret;
}

static hal_ret_t sec_debug_protocol_sig_and_cmd_process(
    uint32_t *data, size_t data_size, debug_protocol_info_t *protocol_info)
{
    hal_ret_t ret                                     = HAL_ERR_GENERIC;
    uint32_t *protocol_write_data                     = NULL;
    sec_debug_protocol_challenge_t protocol_challenge = {0};
    sec_debug_sig_info_t sig_info                     = {0};
    uint32_t msg_size                                 = 0;
    uint32_t sig_sch                                  = 0;
    uint8_t *msg                                      = NULL;
    uint32_t cmd                                      = 0;
    uint32_t device_id                                = 0;
    uint32_t model_id                                 = 0;
    uint32_t protocol_ret        = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
    sec_debug_dgst_result_t hash = {0};
    hal_crypto_key_t key         = {0};
    size_t hash_len              = 0;
    debug_cmd_t dbg_cmd          = {0};
    PAL_ASSERT((NULL != data) && (NULL != protocol_info));

    protocol_write_data =
        (uint32_t *)pal_malloc(SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
    if (!protocol_write_data) {
        PAL_LOG_ERR("Malloc %d out of memory!\n",
                    SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
        return HAL_ERR_OUT_OF_MEMORY;
    }

    msg      = (uint8_t *)data;
    model_id = data[0];

    if (model_id != (protocol_info->model_id)) {
        ret          = HAL_ERR_GENERIC;
        protocol_ret = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
        PAL_LOG_ERR("Bad Model ID, not match: model_id(0x%x) != "
                    "protocol_info->model_id(0x%x)\n",
                    (unsigned int)model_id,
                    (unsigned int)(protocol_info->model_id));
        goto build_response;
    }

    device_id = data[1];
    if (device_id != protocol_info->device_id) {
        ret          = HAL_ERR_GENERIC;
        protocol_ret = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
        PAL_LOG_ERR("device_id(0x%x) != protocol_info->device_id(0x%x)\n",
                    (unsigned int)device_id,
                    (unsigned int)protocol_info->device_id);
        goto build_response;
    }

    cmd = data[2];

    data[2] = SWAP_ENDIAN(data[2]);
    PAL_LOG_INFO("Debug-cmd: 0x%x \n", (unsigned int)cmd);
    pal_memcpy(
        protocol_challenge, &data[3], sizeof(sec_debug_protocol_challenge_t));
     
    if (pal_memcmp(protocol_challenge,
                   protocol_info->challenge_data,
                   sizeof(sec_debug_protocol_challenge_t)) != 0) {
        ret          = HAL_ERR_GENERIC;
        protocol_ret = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
        PAL_LOG_ERR("Bad protocol challenge, not match!\n");
        PAL_LOG_DUMP_BUF("protocol_challenge",
                         protocol_challenge,
                         sizeof(protocol_challenge));
        PAL_LOG_DUMP_BUF("local challenge",
                         protocol_info->challenge_data,
                         sizeof(protocol_challenge));
        goto build_response;
    }

    msg_size = sizeof(sec_debug_protocol_challenge_t) + 3 * sizeof(uint32_t);
    PAL_LOG_DUMP_BUF("Message:", msg, msg_size);

    pal_memcpy(&sig_sch,
               (uint8_t *)(&data[3]) + sizeof(sec_debug_protocol_challenge_t),
               sizeof(sig_sch));
    sig_info.sig_sch = (sec_debug_sig_sch_t)(sig_sch);
    PAL_LOG_INFO("sig_sch: 0x%x \n", sig_info.sig_sch);

    pal_memcpy(&sig_info.sig_size,
               (uint8_t *)(&data[3]) + sizeof(sec_debug_protocol_challenge_t) +
                   sizeof(sig_sch),
               sizeof(sig_info.sig_size));
    PAL_LOG_INFO("sig_size: 0x%x \n", (unsigned int)sig_info.sig_size);

    sig_info.sig = (uint8_t *)(&data[3]) +
                   sizeof(sec_debug_protocol_challenge_t) + sizeof(sig_sch) +
                   sizeof(sig_info.sig_size);

    pal_memset(hash, 0, sizeof(sec_debug_dgst_result_t));
    hash_len = sizeof(sec_debug_dgst_result_t);
    ret      = hal_crypto_hash(
        _GET_DGST_SCH_FROM_SIG_SCH(sig_sch), msg, msg_size, hash, &hash_len);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_crypto_hash failed!\n");
        protocol_ret = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
        goto build_response;
    }

    if (IS_RSA_SIG_SCH(sig_info.sig_sch)) {

        key.key_type          = _GET_ASYM_KEY_TYPE_FROM_SCH(sig_sch);
        key.rsa_pubkey.n      = protocol_info->pubkey_info.pubkey;
        key.rsa_pubkey.n_size = protocol_info->pubkey_info.pubkey_size;
        key.rsa_pubkey.e      = (uint8_t *)_g_rsa_pubkey_e;
        key.rsa_pubkey.e_size = sizeof(_g_rsa_pubkey_e);

        ret = hal_crypto_rsa_verify_digest(SWITCH_RSA_SIG_SCH(sig_sch),
                                           &key,
                                           hash,
                                           hash_len,
                                           sig_info.sig,
                                           sig_info.sig_size);
        if (HAL_OK != ret) {
            PAL_LOG_ERR("hal_crypto_rsa_verify_digest failed!\n");
            if (HAL_ERR_VRFY_FAILED == ret) {
                protocol_ret = SEC_DEBUG_PROTOCOL_SIG_VERIFY_FAILED;
            } else {
                protocol_ret = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
            }
            goto build_response;
        }
    } else if (IS_ECDSA_SIG_SCH(sig_info.sig_sch)) {

        key.key_type         = _GET_ASYM_KEY_TYPE_FROM_SCH(sig_sch);
        key.ec_pubkey.q      = protocol_info->pubkey_info.pubkey;
        key.ec_pubkey.q_size = protocol_info->pubkey_info.pubkey_size;

        ret = hal_crypto_ecdsa_verify_digest(
            SWITCH_ECDSA_SIG_SCH(sig_sch),
            &key,
            hash,
            hash_len,
            sig_info.sig,
            _GET_ECDSA_SIG_SIZE_FROM_SIG_SCH(sig_sch),
            sig_info.sig + _GET_ECDSA_SIG_SIZE_FROM_SIG_SCH(sig_sch),
            _GET_ECDSA_SIG_SIZE_FROM_SIG_SCH(sig_sch));
        if (HAL_OK != ret) {
            PAL_LOG_ERR("hal_crypto_ecdsa_verify_digest failed!\n");
            if (HAL_ERR_VRFY_FAILED == ret) {
                protocol_ret = SEC_DEBUG_PROTOCOL_SIG_VERIFY_FAILED;
            } else {
                protocol_ret = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
            }
            goto build_response;
        }

    } else {
        PAL_LOG_ERR("Invalid signature scheme!\n");
        protocol_ret = SEC_DEBUG_PROTOCOL_FAILED_INTERNAL;
        ret          = HAL_ERR_GENERIC;
        goto build_response;
    }

     
    PAL_ASSERT(sizeof(debug_cmd_t) == sizeof(cmd));
    pal_memcpy(&dbg_cmd, &cmd, sizeof(debug_cmd_t));
    ret = hal_secure_debug_excute_debug_cmd(dbg_cmd);
    if (HAL_OK == ret) {
        PAL_LOG_INFO("Excute Secure Debug command: 0x%x SUCCESS!\n", cmd);
        protocol_ret = SEC_DEBUG_PROTOCOL_SIG_AND_CMD_SUCCESS;
    } else {
        PAL_LOG_INFO("Excute Secure Debug command: 0x%x FAILED!\n", cmd);
        protocol_ret = SEC_DEBUG_PROTOCOL_EXEC_CMD_FAILED;
    }
 
build_response:
    *((uint32_t *)(_PROTOCOL_PAYLOAD_ADDR(protocol_write_data))) = protocol_ret;
    if (HAL_OK == ret) {
        PAL_LOG_INFO("Signature verify and excute command SUCCESS!\n");
    } else {
        PAL_LOG_INFO("Signature verify or excute command FAILED!\n");
    }

    _protocol_init_header(protocol_write_data,
                          sizeof(uint32_t),
                          SEC_DEBUG_SIG_VERIFY_AND_EXCUTE_CMD_RESPONSE);

    if (HAL_OK != hal_channel_write(g_sec_dbg_ctx.channel,
                                    (uint8_t *)protocol_write_data,
                                    protocol_write_data[2])) {
        PAL_LOG_ERR("hal_channel_write failed!\n");
    }

    pal_free(protocol_write_data);

    return ret;
}

hal_ret_t sec_debug_protocol_process(void)
{
    hal_ret_t ret                               = HAL_ERR_GENERIC;
    uint32_t *protocol_read_data                = NULL;
    size_t read_data_size                       = 0;
    sec_debug_protocol_tag_t tag                = SEC_DEBUG_INVALID;
    sec_debug_protocol_status_t protocol_status = SEC_DEBUG_PROTOCOL_INITIAL;
    debug_protocol_info_t protocol_info         = {0};

    protocol_read_data =
        (uint32_t *)pal_malloc(SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
    if (!protocol_read_data) {
        PAL_LOG_ERR("Malloc %d failed!\n", SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
        return HAL_ERR_GENERIC;
    }
    pal_memset(&protocol_info, 0, sizeof(debug_protocol_info_t));

    while (1) {
        read_data_size = SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE;
        pal_memset(protocol_read_data, 0, SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
        ret = hal_channel_read(g_sec_dbg_ctx.channel,
                               (uint8_t *)protocol_read_data,
                               &read_data_size);
        if (HAL_OK != ret) {
            PAL_LOG_ERR("hal_channel_read failed!\n");

            continue;
        }
        ret = sec_debug_check_protocol_data(protocol_read_data, read_data_size);
        if (HAL_OK != ret) {
            PAL_LOG_ERR("sec_debug_check_protocol_data fail: 0x%x\n", ret);

            continue;
        }

        tag = _PROTOCOL_CMD(protocol_read_data);
        switch (tag) {
        case SEC_DEBUG_DEVICE_TERMINATE_REQUEST: {
            ret = HAL_ERR_GENERIC;
            PAL_LOG_INFO("Device Terminate!\n");
            break;
        }
        case SEC_DEBUG_CREATE_HANDSHAKE_REQUEST:
            if (SEC_DEBUG_PROTOCOL_INITIAL == protocol_status) {
                ret = sec_debug_protocol_handshake_process(
                    _PROTOCOL_PAYLOAD_ADDR(protocol_read_data),
                    _PROTOCOL_PAYLOAD_SIZE(protocol_read_data),
                    &protocol_info);
                if (HAL_OK == ret) {
                    protocol_status =
                        SEC_DEBUG_PROTOCOL_HANDSHAKE_PROCESS_SUCCESS;
                    PAL_LOG_DEBUG("Handshake success!\n");
                } else {
                    protocol_status = SEC_DEBUG_PROTOCOL_FAILED;
                }
            } else {
                ret = HAL_ERR_GENERIC;
            }
            break;
        case SEC_DEBUG_PUBKEY_VERIFY_REQUEST:
            if (SEC_DEBUG_PROTOCOL_HANDSHAKE_PROCESS_SUCCESS ==
                protocol_status) {
                ret = sec_debug_protocol_pubkey_process(
                    _PROTOCOL_PAYLOAD_ADDR(protocol_read_data),
                    _PROTOCOL_PAYLOAD_SIZE(protocol_read_data),
                    &protocol_info);
                if (HAL_OK == ret) {
                    protocol_status = SEC_DEBUG_PROTOCOL_PUBKEY_PROCESS_SUCCESS;
                    PAL_LOG_DEBUG("Verify Pubkey success!\n");
                } else {
                    protocol_status = SEC_DEBUG_PROTOCOL_FAILED;
                }
            } else {
                ret = HAL_ERR_GENERIC;
            }
            break;
        case SEC_DEBUG_SIG_VERIFY_AND_EXCUTE_CMD_REQUEST:
            if (SEC_DEBUG_PROTOCOL_PUBKEY_PROCESS_SUCCESS == protocol_status) {
                ret = sec_debug_protocol_sig_and_cmd_process(
                    _PROTOCOL_PAYLOAD_ADDR(protocol_read_data),
                    _PROTOCOL_PAYLOAD_SIZE(protocol_read_data),
                    &protocol_info);
                if (HAL_OK == ret) {
                    protocol_status =
                        SEC_DEBUG_PROTOCOL_SIG_AND_CMD_PROCESS_SUCCESS;
                    PAL_LOG_DEBUG(
                        "Verify signature and execute command success!\n");
                } else {
                    protocol_status = SEC_DEBUG_PROTOCOL_FAILED;
                }
            } else {
                ret = HAL_ERR_GENERIC;
            }
            break;
        default:
            ret = HAL_ERR_GENERIC;
            break;
        }

        if ((SEC_DEBUG_PROTOCOL_SIG_AND_CMD_PROCESS_SUCCESS ==
             protocol_status) ||
            (SEC_DEBUG_PROTOCOL_FAILED == protocol_status)) {
            break;
        } else {
            if (HAL_OK != ret) {
                break;
            }
        }
    }

    pal_memset((uint8_t *)&protocol_info, 0, sizeof(protocol_info));
    pal_memset(protocol_read_data, 0, SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE);
    pal_free(protocol_read_data);
    return ret;
}
// eof

