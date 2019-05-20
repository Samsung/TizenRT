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

#ifndef _SECURITY_API_CRYPTO_H__
#define _SECURITY_API_CRYPTO_H__

#include "security_common.h"
/**
 * Crypto
 */

security_error crypto_aes_encryption(security_handle hnd, security_aes_param *param, const char *key_name, security_data *input, security_data *output);
security_error crypto_aes_decryption(security_handle hnd, security_aes_param *param, const char *key_name, security_data *input, security_data *output);
security_error crypto_rsa_encryption(security_handle hnd, security_rsa_param *param, const char *key_name, security_data *input, security_data *output);
security_error crypto_rsa_decryption(security_handle hnd, security_rsa_param *param, const char *key_name, security_data *input, security_data *output);

#endif // _SECURITY_API_CRYPTO_H__
