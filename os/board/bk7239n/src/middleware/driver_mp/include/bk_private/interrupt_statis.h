// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <driver/int_types.h>
#if defined(CONFIG_INT_STATIS)
int_statis_t g_int_statis_num = {0};
#else
#endif

#if defined(CONFIG_INT_STATIS)
#define INT_STATIS_INC((_x)) (_x)++
#else
#define INT_STATIS_INC((_x))
#endif

typedef struct {
        uint32_t irq_int_statis[IRQ_STATIS_COUNT];
        uint32_t fiq_int_statis[FIQ_STATIS_COUNT];
} int_statis_t;

/**
 * @brief     get every unit int numbers form ICU driver after congifg CONFIG_INT_STATIS
 *
 * This API get every unit's int numbers
 *
 * @return
 *    - int_statis_t: an int_statis_t pointer which including all uints' int numbers
 *    - others: other errors.
 */
int_statis_t * bk_get_int_statis(void);

/**
 * @brief     dunmp all units' int numbers form ICU driver
 *
 * This API dunmp all unit's int numbers
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dump_int_statis(int_statis_t *int_statistic);



