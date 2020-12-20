/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <errno.h>

#include <tinyara/arch.h>
#ifdef CONFIG_AMEBAD_TRUSTZONE
#include <arch/chip/amebad_nsc.h>
#endif

/****************************************************************************
 * Private Function
 ****************************************************************************/
#ifdef CONFIG_AMEBAD_TRUSTZONE
static uint32_t no_protection_func(uint32_t data)
{
	uint32_t result;

	result = (data / 5) + 3;
	result *= 2;
	result += 8;

	return result;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/************************************************************************************
 * Name: test_tz
 *
 * Description:  This function tests the trustzone functionality
 *
 ************************************************************************************/

int test_tz(void)
{
#ifdef CONFIG_AMEBAD_TRUSTZONE
	int i = 0;
	uint32_t rdp_result;
	uint32_t no_rdp_result;

	/*
	 * Tasks are not created with a secure context.
	 * Any task that is going to call secure functions must call up_allocate_secure_context()
	 * to allocate itself a secure context before it calls any secure function
	 */
	up_allocate_secure_context(1024);
	for (i = 0; i < 32; i++) {
		rdp_result = rdp_protection_entry(i);
		no_rdp_result = rdp_no_protection_call(no_protection_func, i);

		if (rdp_result != no_rdp_result) {
			dbg("rdp call fail!\n");
			dbg("rdp_result = 0x%x, no_rdp_result=0x%x\n", rdp_result, no_rdp_result);
			break;
		}
	}
	up_free_secure_context();

	if (i == 32) {
		dbg("rdp demo call succeed!\n");
		return OK;
	}

	dbg("rdp demo call FAILED\n");
	return -EINTR;
#else
	return -EPERM;
#endif
}

