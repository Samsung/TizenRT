/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "region_defs.h"
/* Enumeration that is used by the create_img.py script
 * for correct binary generation when nested macros are used
 */
enum image_attributes {
    RE_BL1_2_BIN_SIZE = BL1_2_CODE_SIZE,
};
