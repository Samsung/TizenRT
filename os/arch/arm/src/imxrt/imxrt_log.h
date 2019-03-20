/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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

#ifndef __ARCH_ARM_SRC_IMXRT_IMXRT_LOG_H
#define __ARCH_ARM_SRC_IMXRT_IMXRT_LOG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "up_internal.h"
#include "chip.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define IMXLOG(buf) imxrt_log_write_kernelbuffer((const uint8_t *)buf, (const uint32_t)strlen(buf))

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_log_write_kernelbuffer
 *
 * Description:
 *   Called when kernel log can be checked
 *
 ****************************************************************************/
void imxrt_log_write_kernelbuffer(const uint8_t *log_string, const uint32_t log_length);

/****************************************************************************
 * Name: imxrt_log_print_kernelbuffer
 *
 * Description:
 *   Called when kernel log can be printed
 *
 ****************************************************************************/
void imxrt_log_print_kernelbuffer(void);

#endif							/* __ARCH_ARM_SRC_IMXRT_IMXRT_LOG_H */
