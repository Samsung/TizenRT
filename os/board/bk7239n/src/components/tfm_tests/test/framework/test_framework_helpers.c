/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_helpers.h"

#include <stdio.h>

void printf_set_color(enum serial_color_t color_id)
{
    TEST_LOG("\33[%dm", color_id);
}
