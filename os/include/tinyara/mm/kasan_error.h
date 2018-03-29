/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __MM_KASAN_KASAN_ERROR_H
#define __MM_KASAN_KASAN_ERROR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>

#ifdef CONFIG_TC_KERNEL_KASAN

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#define KASAN_HEAP_WRITE           0x01
#define KASAN_HEAP_READ            0x02
#define KASAN_HEAP_UAF_WRITE       0x03
#define KASAN_HEAP_UAF_READ        0x04
#define KASAN_GLOBAL_WRITE         0x05
#define KASAN_GLOBAL_READ          0x06
#define KASAN_STACK_WRITE          0x07
#define KASAN_STACK_READ           0x08
#define KASAN_STACK_UAS_WRITE      0x09
#define KASAN_STACK_UAS_READ       0x10
#define KASAN_UNKNOWN              0x11

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: kasan_set_error
 *
 * Description:
 *  Set 'kasan_last_error' variable to 'write_err' or 'read_err' type depending
 *  on is this write or read access error. For possible error types see
 *  'kasan.h' file.
 *
 * Input Parameters:
 *  write     - true or false value denotes write or read access respectively
 *  write_err - error type for write access
 *  read_err  - error type for read access
 *
 * Returned Value:
 *  None
 *
 * Assumption:
 *  None
 *
 ****************************************************************************/
void kasan_set_error(bool write, int write_err, int read_err);

/****************************************************************************
 * Name: kasan_reset_error
 *
 * Description:
 *  Reset 'kasan_last_error' to default value.
 *
 * Input Parameters:
 *  None
 *
 * Returned Value:
 *  None
 *
 * Assumption:
 *  None
 *
 ****************************************************************************/
void kasan_reset_error(void);

/****************************************************************************
 * Name: kasan_get_error
 *
 * Description:
 *  Return last error type.
 *
 * Input Parameters:
 *  None
 *
 * Returned Value:
 *  unsigned int value that denotes last error type. For possible error types
 *  see 'kasan.h' file.
 *
 * Assumption:
 *  None
 *
 ****************************************************************************/
unsigned int kasan_get_error(void);

#else  /* CONFIG_TC_KERNEL_KASAN */

#define kasan_set_error(a, b, c)
#define kasan_reset_error()
#define kasan_get_error()

#endif	/* CONFIG_TC_KERNEL_KASAN */
#endif	/* __MM_KASAN_KASAN_ERROR_H */
