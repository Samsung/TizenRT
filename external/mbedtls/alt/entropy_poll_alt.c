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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ENTROPY_C)

#include "mbedtls/entropy.h"
#include "mbedtls/entropy_poll.h"

#include "mbedtls/alt/common.h"

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT) && defined(CONFIG_HW_RNG)
static int mbedtls_generate_random_alt( unsigned int *data, unsigned int len )
{
	int r;

	if (data == NULL || len > MBEDTLS_MAX_RANDOM_SIZE) {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	/* Change length to word number */
	if (len & 0x3) {
		len = len + 4 - (len & 0x3);
	}

	ISP_CHECKBUSY();
	if ((r = isp_generate_random(data, len / 4)) != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	return( 0 );
}

int mbedtls_hardware_poll( void *data,
                           unsigned char *output, size_t len, size_t *olen )
{
	unsigned int inlen = MBEDTLS_MAX_RANDOM_SIZE;
	unsigned int inbuf[MBEDTLS_MAX_RANDOM_SIZE];

	((void) data);

	if (mbedtls_generate_random_alt( inbuf, inlen ) < 0) {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	if (len < inlen) {
		inlen = len;
	}

	memcpy(output, inbuf, inlen);
	*olen = inlen;

	return( 0 );
}
#endif /* MBEDTLS_ENTROPY_HARDWARE_ALT & CONFIG_HW_RNG */

#endif /* MBEDTLS_ENTROPY_C */
