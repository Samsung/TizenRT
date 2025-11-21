/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRITICAL_SECTION_H__ /* TFM prefix to avoid clash */
#define __TFM_CRITICAL_SECTION_H__

#include <stdint.h>
#include "tfm_arch.h"

struct critical_section_t {
    uint32_t   state;
};

#define CRITICAL_SECTION_STATIC_INIT   {.state = 0,}
#define CRITICAL_SECTION_INIT(cs)      (cs).state = (0)
#define CRITICAL_SECTION_ENTER(cs)     (cs).state = __save_disable_irq()
#define CRITICAL_SECTION_LEAVE(cs)     __restore_irq((cs).state)

#endif /* __TFM_CRITICAL_SECTION_H__ */
