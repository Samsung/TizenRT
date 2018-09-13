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

#ifndef __EXTERNAL_LIBCXX_TEST_LIBCXX_TC_COMMON_H
#define __EXTERNAL_LIBCXX_TEST_LIBCXX_TC_COMMON_H
#include "tc_common.h"

/* Custom TC macros */

#define TC_ASSERT_EXPR(expr)	\
{\
	if (!(expr)) {\
		printf("\nTC Assertion FAIL at function: %s, file: %s line: %d\n", __func__, (const uint8_t *)__FILE__, (int)__LINE__); \
		total_fail++; \
		return -1; \
	} \
}

#define TC_ASSERT_FUNC(funcret)\
{\
	if (0 != (funcret)) { \
		return -1; \
	} \
}

#endif /* __EXTERNAL_LIBCXX_TEST_LIBCXX_TC_COMMON_H */
