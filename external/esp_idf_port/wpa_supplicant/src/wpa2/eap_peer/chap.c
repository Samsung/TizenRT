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
 * CHAP-MD5
 *
 */
#ifdef CHAP_MD5

#include "wpa/includes.h"
#include "wpa/common.h"
#include "crypto/crypto.h"
#include "wpa2/eap_peer/chap.h"

int chap_md5(u8 id, const u8 *secret, size_t secret_len, const u8 *challenge,
	     size_t challenge_len, u8 *response)
{
	const u8 *addr[3];
	size_t len[3];

	addr[0] = &id;
	len[0] = 1;
	addr[1] = secret;
	len[1] = secret_len;
	addr[2] = challenge;
	len[2] = challenge_len;
	return md5_vector(3, addr, len, response);
}

#endif /* CHAP_MD5 */
