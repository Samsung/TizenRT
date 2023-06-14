/****************************************************************************
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_INCLUDE_AMEBASMART_CHIP_H
#define __ARCH_ARM_INCLUDE_AMEBASMART_CHIP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/
#  define AMEBASMART_DRAM_SIZE   (5*1024*1024)   /* Size of the PSRAM for BL33 (5mb) */
// #  define AMEBASMART_L2CACHE_SIZE (1024*1024)  /* 1MB L2 Cache */
#  define AMEBASMART_NXCPUS       2            /* Two CPUs */
// #  define AMEBASMART_HAVE_DDR64   1            /* 64-bit DDR3 */
// #  undef  AMEBASMART_HAVE_DDR32                /* 32-bit DDR3 */
// #  define AMEBASMART_HAVE_DDR32x2 1            /* Two channel 32-bit DDR3 */
/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

#endif							/* __ARCH_ARM_INCLUDE_AMEBASMART_CHIP_H */
