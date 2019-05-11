/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef _SECURITY_API_AUTH_H__
#define _SECURITY_API_AUTH_H__

#include "security_common.h"

/**
 * Authenticate
 */

int auth_generate_random(security_handle hnd, unsigned int size, security_data *random);

int auth_generate_certificate(security_handle hnd, const char *cert_name, security_csr *csr, security_data *cert);

int auth_set_certificate(security_handle hnd, const char *cert_name, security_data *cert);

int auth_get_certificate(security_handle hnd, const char *cert_name, security_data *cert);

int auth_remove_certificate(security_handle hnd, const char *cert_name);

int auth_get_rsa_signature(security_handle hnd, security_rsa_param mode, const char *key_name, security_data *hash, security_data *sign);

int auth_verify_rsa_signature(security_handle hnd, security_rsa_param mode, const char *key_name, security_data *hash, security_data *sign);

int auth_get_ecdsa_signature(security_handle hnd, security_ecdsa_param mode, const char *key_name, security_data *hash, security_data *sign);

int auth_verify_ecdsa_signature(security_handle hnd, security_ecdsa_param mode, const char *key_name, security_data *hash, security_data *sign);

int auth_get_hash(security_handle hnd, security_hash_mode mode, security_data *data, security_data *hash);

int auth_get_hmac(security_handle hnd, security_hmac_mode mode, const char *key_name, security_data *data, security_data *hmac);

int auth_generate_dhparams(security_handle hnd, const char *dh_name, security_dh_param *params);

int auth_compute_dhparams(security_handle hnd, const char *dh_name, security_dh_param *params, security_data *secret);

int auth_generate_ecdhkey(security_handle hnd, const char *ecdh_name, security_ecdh_param *params);

int auth_compute_ecdhkey(security_handle hnd, const char *ecdh_name, security_ecdh_param *params, security_data *secret);

#endif //  _SECURITY_API_AUTH_H__
