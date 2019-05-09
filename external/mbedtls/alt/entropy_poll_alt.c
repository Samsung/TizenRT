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
#include <tinyara/security_hal.h>
#include <tinyara/seclink.h>

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
	sl_ctx shnd;

	hal_data random = {data, len, NULL, 0};
	hal_result_e hres = HAL_FAIL;
	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	ret = sl_generate_random(shnd, len, &random, &hres);

	if ((ret != SECLINK_OK) || (hres != HAL_SUCCESS)) {
		sl_deinit(shnd);
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	sl_deinit(shnd);

	return HAL_SUCCESS;
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
