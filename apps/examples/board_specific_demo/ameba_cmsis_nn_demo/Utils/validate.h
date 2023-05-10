/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define TEST_ASSERT_EQUAL(exp, val)                                    \
	do {                                                            \
		uint32_t tmp_val = val;                                       \
		if (exp != tmp_val) {                                         \
			printf("[ERROR] val (%d) exp (%d)\n", tmp_val, exp);      \
			printf("%s %s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
		}                                                             \
	} while (0)

#define TEST_ASSERT_TRUE(condition)                                    \
	do {                                                            \
		if (! (condition) ) {                                         \
			printf("%s %s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
		}                                                             \
		else{                                         \
			printf("%s -> OK!\n", __FUNCTION__); \
		}                                                             \
	} while (0)

static inline int validate(int8_t *act, const int8_t *ref, int size)
{
    int test_passed = true;
    int count = 0;
    int total = 0;

    for (int i = 0; i < size; ++i)
    {
        total++;
        if (act[i] != ref[i])
        {
            count++;
            printf("ERROR at pos %d: Act: %d Ref: %d\r\n", i, act[i], ref[i]);
            test_passed = false;
        }
    }

    if (!test_passed)
    {
        printf("%d of %d failed\r\n", count, total);
    }

    return test_passed;
}

static inline int validate_s16(int16_t *act, const int16_t *ref, int size)
{
    int test_passed = true;
    int count = 0;
    int total = 0;

    for (int i = 0; i < size; ++i)
    {
        total++;
        if (act[i] != ref[i])
        {
            count++;
            printf("ERROR at pos %d: Act: %d Ref: %d\r\n", i, act[i], ref[i]);
            test_passed = false;
        }
        else
        {
            // printf("PASS at pos %d: %d\r\n", i, act[i]);
        }
    }

    if (!test_passed)
    {
        printf("%d of %d failed\r\n", count, total);
    }

    return test_passed;
}
