/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
#ifdef CONFIG_SECURITY_LEVEL
extern "C"{
#include <tinyara/security_level.h>
}
#endif

#include <cstdio>
#include <debug.h>
#include <exception>
#include <stdlib.h>
#include <typeinfo>


/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void __global_terminate_handler(void)
{
	printf("C++ terminate handler called! PID: %d\n", getpid());
#ifdef CONFIG_SECURITY_LEVEL
	if (!IS_SECURE_STATE())
#endif
	{
#ifdef CONFIG_LIBCXX_EXCEPTION
		// Check if there's an active exception
		std::exception_ptr exptr = std::current_exception();
		if (exptr) {
			try {
				std::rethrow_exception(exptr);
			} catch (const std::exception& e) {
				try {
#ifdef __cpp_rtti
					printf("Exception Type: %s\n", typeid(e).name());
#endif
					printf("Exception Message: %s\n", e.what());
				} catch (...) {
					printf("Exception info extraction failed\n");
				}
			} catch (...) {
				printf("Unknown exception type\n");
			}
		} else {
			printf("No active exception\n");
		}
#endif /* CONFIG_LIBCXX_EXCEPTION */
	}

	PANIC();
}

//***************************************************************************
// Public Functions
//***************************************************************************

extern "C" void register_cxx_terminate_handler(void)
{
    std::set_terminate(__global_terminate_handler);
}
