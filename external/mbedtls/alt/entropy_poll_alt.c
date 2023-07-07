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

#include <tinyara/config.h>
#include <security/security_common.h>
#include <security/security_auth.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ENTROPY_C)

#include "mbedtls/entropy.h"
#include "mbedtls/entropy_poll.h"

#include "mbedtls/alt/common.h"

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)
static int mbedtls_generate_random_alt(unsigned char *data, unsigned int len)
{
	int ret;
	security_handle shnd;
	security_data random = {data, len};

	ret = security_init(&shnd);
	if (ret != SECURITY_OK) {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	ret = auth_generate_random(shnd, len, &random);
	security_deinit(shnd);
	if (ret != SECURITY_OK) {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}
	return SECURITY_OK;
}

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
	unsigned char inbuf[len];

	if (mbedtls_generate_random_alt(inbuf, len) < 0) {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	memcpy(output, inbuf, len);
	*olen = len;

	return 0;
}
#endif							/* MBEDTLS_ENTROPY_HARDWARE_ALT */

#endif							/* MBEDTLS_ENTROPY_C */
