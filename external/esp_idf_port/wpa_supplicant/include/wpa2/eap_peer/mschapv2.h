/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/*
 * MSCHAPV2
 */


#ifndef MSCHAPV2_H
#define MSCHAPV2_H

#define MSCHAPV2_CHAL_LEN		16
#define MSCHAPV2_NT_RESPONSE_LEN	24
#define MSCHAPV2_AUTH_RESPONSE_LEN	20
#define MSCHAPV2_MASTER_KEY_LEN		16

const u8 * mschapv2_remove_domain(const u8 *username, size_t *len);
int mschapv2_derive_response(const u8 *username, size_t username_len,
			     const u8 *password, size_t password_len,
			     int pwhash,
			     const u8 *auth_challenge,
			     const u8 *peer_challenge,
			     u8 *nt_response, u8 *auth_response,
			     u8 *master_key);
int mschapv2_verify_auth_response(const u8 *auth_response,
				  const u8 *buf, size_t buf_len);
#endif /* MSCHAPV2_H */
