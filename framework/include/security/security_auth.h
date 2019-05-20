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

security_error auth_generate_random(security_handle hnd, unsigned int size, security_data *random);

security_error auth_generate_certificate(security_handle hnd, const char *cert_name, security_csr *csr, security_data *cert);

security_error auth_set_certificate(security_handle hnd, const char *cert_name, security_data *cert);

security_error auth_get_certificate(security_handle hnd, const char *cert_name, security_data *cert);

security_error auth_remove_certificate(security_handle hnd, const char *cert_name);

security_error auth_get_rsa_signature(security_handle hnd, security_rsa_param *param, const char *key_name, security_data *hash, security_data *sign);

security_error auth_verify_rsa_signature(security_handle hnd, security_rsa_param *param, const char *key_name, security_data *hash, security_data *sign);

security_error auth_get_ecdsa_signature(security_handle hnd, security_ecdsa_param *param, const char *key_name, security_data *hash, security_data *sign);

security_error auth_verify_ecdsa_signature(security_handle hnd, security_ecdsa_param *param, const char *key_name, security_data *hash, security_data *sign);

security_error auth_get_hash(security_handle hnd, security_hash_mode mode, security_data *data, security_data *hash);

security_error auth_get_hmac(security_handle hnd, security_hmac_mode mode, const char *key_name, security_data *data, security_data *hmac);

security_error auth_generate_dhparams(security_handle hnd, const char *dh_name, security_dh_param *param);

security_error auth_compute_dhparams(security_handle hnd, const char *dh_name, security_dh_param *param, security_data *secret);

security_error auth_generate_ecdhkey(security_handle hnd, const char *ecdh_name, security_ecdh_param *param);

security_error auth_compute_ecdhkey(security_handle hnd, const char *ecdh_name, security_ecdh_param *param, security_data *secret);

#endif //  _SECURITY_API_AUTH_H__
