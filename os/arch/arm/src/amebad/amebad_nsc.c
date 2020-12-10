/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>

#include <arch/chip/amebad_nsc.h>

/****************************************************************************
 * Context Management for RTL8721D AmebaD TrustZone
 ****************************************************************************/

/* Realtek RTL8721D API hooks for tz context */

uint32_t TZ_InitContextSystem_S(void)
{
	/* De-prioritize the non-secure exceptions so that the
	 * non-secure pendSV runs at the lowest priority */
	SecureInit_DePrioritizeNSExceptions();

	SecureContext_Init();
	return PASS;
}

TZ_MemoryId_t TZ_AllocModuleContext_S(TZ_ModuleId_t module)
{
	TZ_MemoryId_t ret = 0;
	ret = (TZ_MemoryId_t) SecureContext_AllocateContext((uint32_t)module);
	return ret;
}

uint32_t TZ_FreeModuleContext_S(TZ_MemoryId_t id)
{
	SecureContext_FreeContext((SecureContextHandle_t)id);
	/* return execution status (1: success, 0: error) */
	return PASS;
}

uint32_t TZ_LoadContext_S(TZ_MemoryId_t id)
{
	SecureContext_LoadContext((SecureContextHandle_t)id);
	/* return execution status (1: success, 0: error) */
	return PASS;
}

uint32_t TZ_StoreContext_S(TZ_MemoryId_t id)
{
	SecureContext_SaveContext((SecureContextHandle_t)id);
	/* return execution status (1: success, 0: error) */
	return PASS;
}
