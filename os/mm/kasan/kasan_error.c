/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/mm/kasan_error.h>

#ifdef CONFIG_TC_KERNEL_KASAN

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static unsigned int kasan_last_error;

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
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
void kasan_set_error(bool write, int write_err, int read_err)
{
	kasan_last_error = write ? write_err : read_err;
}

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
void kasan_reset_error(void)
{
	kasan_last_error = 0;
}

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
unsigned int kasan_get_error(void)
{
	return kasan_last_error;
}
#endif	/* CONFIG_TC_KERNEL_KASAN */
