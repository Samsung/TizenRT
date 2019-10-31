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
#include "randombytes_default.h"
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

static time_t time_v = (time_t)-1;

uint32_t soft_random(void)
{
	if (time_v == (time_t)-1) {
		time_v = time(NULL);
		srand((unsigned int)time_v);
	}

	return (uint32_t)rand();
}

static void randombytes_soft_random_buf(void *const buf, const size_t size)
{
	if (time_v == (time_t)-1) {
		time_v = time(NULL);
		srand((unsigned int)time_v);
	}

	size_t i;
	uint8_t *p = (uint8_t *)buf;
	for (i = 0; i < size; i++) {
		p[i] = rand();
	}
}

static const char *randombytes_soft_implementation_name(void)
{
	return "soft";
}

const struct randombytes_implementation randombytes_soft_implementation = {
	.implementation_name = randombytes_soft_implementation_name,
	.random = soft_random,
	.stir = NULL,
	.uniform = NULL,
	.buf = randombytes_soft_random_buf,
	.close = NULL,
};
