/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/security_sdk/inc/security_sdk_test.h
 *
 *   Copyright (C) 2018 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: Youngdae Oh <yd.oh@samsung.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef APPS_EXAMPLES_SECURITY_SDK_SECURITY_SDK_TEST_H_
#define APPS_EXAMPLES_SECURITY_SDK_SECURITY_SDK_TEST_H_

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <artik_module.h>
#include <artik_security.h>

#define see_selfprintf printf
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

typedef enum {
	SECURITY_SDK_OK,
	SECURITY_SDK_ERROR,
} security_sdk_error;

int security_sdk_aes_encrypt_decrypt(void);
int security_sdk_generate_key(void);
int security_sdk_set_compute_dhm_params(void);
int security_sdk_set_compute_ecdh_params(void);
int security_sdk_rsassa_sign_verify(void);
int security_sdk_get_hash(void);
int security_sdk_setup_remove_key(void);
int security_sdk_rsa_encrypt_decrypt(void);
int security_sdk_get_hmac(void);
int security_sdk_ecdsa_sign_verify(void);
int security_sdk_get_publickey(void);
void security_sdk_print_existence(void);
void security_sdk_init_existence(void);
void security_sdk_factory_key_test(int debug_mode);
int security_sdk_get_certificates(void);
int security_sdk_cert_publickey(void);
int security_sdk_cert_serial(void);
int security_sdk_secure_storage(void);
int security_sdk_random(void);

#endif /* APPS_EXAMPLES_SECURITY_SDK_SECURITY_SDK_TEST_H_ */
