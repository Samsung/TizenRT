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
 * drivers/crypto/dev_urandom.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/* This random number generator is simple, fast and portable.
 *      Ref:  https://en.wikipedia.org/wiki/Xorshift
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/param.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>

#include <tinyara/lib/lib.h>
#include <tinyara/lib/xorshift128.h>
#include <tinyara/semaphore.h>
#include <tinyara/fs/fs.h>
#include <tinyara/drivers/drivers.h>

#if defined(CONFIG_DEV_URANDOM) && !defined(CONFIG_DEV_URANDOM_ARCH)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if !defined(CONFIG_DEV_URANDOM_CONGRUENTIAL) && \
    !defined(CONFIG_DEV_URANDOM_XORSHIFT128)
#  define CONFIG_DEV_URANDOM_XORSHIFT128 1
#endif

#ifdef CONFIG_DEV_URANDOM_XORSHIFT128
#  define PRNG() do_xorshift128()
#elif defined(CONFIG_DEV_URANDOM_CONGRUENTIAL)
#  define PRNG() do_congruential()
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

union xorshift128_state_u {
	struct xorshift128_state_s state;
	uint8_t u[16];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static ssize_t devurand_read(FAR struct file *filep, FAR char *buffer,
                             size_t buflen);
static ssize_t devurand_write(FAR struct file *filep, FAR const char *buffer,
                              size_t buflen);
static int devurand_poll(FAR struct file *filep, FAR struct pollfd *fds,
                         bool setup);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_urand_fops =
{
  NULL,                         /* open */
  NULL,                         /* close */
  devurand_read,                /* read */
  devurand_write,               /* write */
  NULL,                         /* seek */
  NULL,                         /* ioctl */
  NULL,                         /* mmap */
  NULL,                         /* truncate */
  devurand_poll                 /* poll */
};

#ifdef CONFIG_DEV_URANDOM_XORSHIFT128
static union xorshift128_state_u g_prng;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: do_xorshift128
 ****************************************************************************/

#ifdef CONFIG_DEV_URANDOM_XORSHIFT128
static inline uint32_t do_xorshift128(void)
{
	return xorshift128(&g_prng.state);
}
#endif

/****************************************************************************
 * Name: do_congruential
 ****************************************************************************/

#ifdef CONFIG_DEV_URANDOM_CONGRUENTIAL
static inline uint32_t do_congruential(void)
{
	/* REVISIT:  We could probably generate a 32-bit value with a single
	 * call to nrand().
	 */

	return (uint32_t)nrand(65536L) << (uint32_t)nrand(65536L);
}
#endif

/****************************************************************************
 * Name: devurand_read
 ****************************************************************************/

static ssize_t devurand_read(FAR struct file *filep, FAR char *buffer,
                             size_t len)
{
	size_t n;
	uint32_t rnd;

	n = len;

	/* Align buffer pointer to 4-byte boundary */

	if (((uintptr_t)buffer & 0x03) != 0) {
		/* Generate a pseudo random number */

		rnd = PRNG();

		while (((uintptr_t)buffer & 0x03) != 0) {
			if (n <= 0) {
				return len;
			}

			*buffer++ = rnd & 0xff;
			rnd >>= 8;
			--n;
		}
	}

	/* Stuff buffer with PRNGs 4 bytes at a time */

	while (n >= 4) {
		*(FAR uint32_t *)buffer = PRNG();
		buffer += 4;
		n -= 4;
	}

	/* Stuff remaining 1, 2, or 3 bytes */

	if (n > 0) {
		/* Generate a pseudo random number */

		rnd = PRNG();

		do {
			*buffer++ = rnd & 0xff;
			rnd >>= 8;
		} while (--n > 0);
	}

	return len;
}

/****************************************************************************
 * Name: devurand_write
 ****************************************************************************/

static ssize_t devurand_write(FAR struct file *filep, FAR const char *buffer,
                              size_t len)
{
	/* Write can be used to re-seed the PRNG state. */

#ifdef CONFIG_DEV_URANDOM_CONGRUENTIAL
	unsigned int seed = 0;

	len = MIN(len, sizeof(unsigned int));
	memcpy(&seed, buffer, len);
	srand(seed);
	return len;

#else
	len = MIN(len, sizeof(g_prng.u));
	memcpy(&g_prng.u, buffer, len);
	return len;
#endif
}

/****************************************************************************
 * Name: devurand_poll
 ****************************************************************************/

static int devurand_poll(FAR struct file *filep, FAR struct pollfd *fds,
                         bool setup)
{
	if (setup) {
		poll_notify(&fds, 1, POLLIN | POLLOUT);
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: devurandom_register
 *
 * Description:
 *   Register /dev/urandom
 *
 ****************************************************************************/

void devurandom_register(void)
{
	/* Seed the PRNG */

#ifdef CONFIG_DEV_URANDOM_CONGRUENTIAL
	srand(10197);
#else
	g_prng.state.w = 97;
	g_prng.state.x = 101;
	g_prng.state.y = g_prng.state.w << 17;
	g_prng.state.z = g_prng.state.x << 25;
#endif

	register_driver("/dev/urandom", &g_urand_fops, 0666, NULL);
}

#endif /* CONFIG_DEV_URANDOM && CONFIG_DEV_URANDOM_ARCH */
