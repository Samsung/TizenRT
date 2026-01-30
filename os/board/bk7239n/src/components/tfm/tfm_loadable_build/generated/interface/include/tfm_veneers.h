/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __TFM_VENEERS_H__
#define __TFM_VENEERS_H__

#include "tfm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/******** TFM_SP_PS ********/
psa_status_t tfm_ps_set_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_get_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_get_info_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_remove_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_get_support_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);

/******** TFM_SP_ITS ********/
psa_status_t tfm_its_set_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_its_get_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_its_get_info_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_its_remove_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);

/******** TFM_SP_CRYPTO ********/
psa_status_t tfm_crypto_get_key_attributes_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_open_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_close_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_reset_key_attributes_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_import_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_destroy_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_export_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_export_public_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_purge_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_copy_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_compute_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_compare_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_update_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_finish_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_verify_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_abort_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_hash_clone_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_compute_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_verify_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_sign_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_verify_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_update_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_sign_finish_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_verify_finish_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_mac_abort_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_encrypt_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_decrypt_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_encrypt_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_decrypt_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_generate_iv_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_set_iv_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_update_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_finish_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_cipher_abort_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_encrypt_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_decrypt_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_encrypt_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_decrypt_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_generate_nonce_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_set_nonce_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_set_lengths_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_update_ad_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_update_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_finish_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_verify_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_aead_abort_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_sign_message_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_verify_message_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_sign_hash_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_verify_hash_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_asymmetric_encrypt_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_asymmetric_decrypt_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_setup_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_get_capacity_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_set_capacity_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_input_bytes_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_input_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_key_agreement_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_output_bytes_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_output_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_key_derivation_abort_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_raw_key_agreement_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_generate_random_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_crypto_generate_key_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);

/******** TFM_SP_PLATFORM ********/
psa_status_t tfm_platform_sp_system_reset_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_platform_sp_ioctl_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_platform_sp_nv_counter_read_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_platform_sp_nv_counter_increment_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);

/******** TFM_SP_INITIAL_ATTESTATION ********/
psa_status_t tfm_initial_attest_get_token_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_initial_attest_get_token_size_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);

/******** TFM_SP_FWU ********/
psa_status_t tfm_fwu_write_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_install_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_abort_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_query_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_request_reboot_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_accept_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);

/******** TFM_SP_SYSTEM_CLOCK ********/

#ifdef __cplusplus
}
#endif

#endif /* __TFM_VENEERS_H__ */
