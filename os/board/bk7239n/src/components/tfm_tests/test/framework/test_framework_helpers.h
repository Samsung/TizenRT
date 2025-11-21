/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TEST_FRAMEWORK_HELPERS_H__
#define __TEST_FRAMEWORK_HELPERS_H__

#include <stdint.h>
#include "test_framework.h"

#ifdef __cplusplus
extern "C" {
#endif

enum serial_color_t {
    DEFAULT = 0,
    BLACK   = 30,
    RED     = 31,
    GREEN   = 32,
    YELLOW  = 33,
    BLUE    = 34,
    MAGENTA = 35,
    CYAN    = 36,
    WHITE   = 37,
};

/**
 * \brief Sets the the text color in the serial port.
 *
 * \param[in] color_id  Serial foreground color.
 *
 * \return 0 if the message is send successfully, 1 otherwise.
 */
void printf_set_color(enum serial_color_t color_id);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_FRAMEWORK_HELPERS_H__ */
