/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef __NON_SECURE_CALLABLE_H__
#define __NON_SECURE_CALLABLE_H__


/*
 * This header file contains function prototypes for the interfaces exposed
 * from Secure world of AmebaD through Non-secure callable gateway
 */

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/tz_context.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define PASS								(1)
#define FAIL								(0)

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* Opaque handle */
struct SecureContext;
typedef struct SecureContext *SecureContextHandle_t;
typedef uint32_t nsFunc(uint32_t);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: SecureContext_Init
 *
 * Description:
 *   Initializes the secure context management system.
 *   PSP is set to NULL and therefore a task must allocate and load a context
 *   before calling any secure side function in the thread mode.
 *
 * Inputs:
 *   None
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void SecureContext_Init(void);

/****************************************************************************
 * Name: SecureContext_AllocateContext
 *
 * Description:
 *   Allocates a context on the secure side.
 *
 * Inputs:
 *   size: Size of the stack to allocate on secure side.
 *
 * Returned Value:
 *   Opaque context handle if context is successfully allocated, NULL
 *   otherwise.
 ****************************************************************************/

SecureContextHandle_t SecureContext_AllocateContext(uint32_t size);

/****************************************************************************
 * Name: SecureContext_FreeContext
 *
 * Description:
 *   Frees the given context
 *
 * Inputs:
 *   xSecureContextHandle: Context handle corresponding to the
 *   context to be freed.
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void SecureContext_FreeContext(SecureContextHandle_t xSecureContextHandle);

/****************************************************************************
 * Name: SecureContext_LoadContext
 *
 * Description:
 *   Loads the given context
 *
 * Inputs:
 *   xSecureContextHandle: Context handle corresponding to the
 *   context to be loaded
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void SecureContext_LoadContext(SecureContextHandle_t xSecureContextHandle);

/****************************************************************************
 * Name: SecureContext_SaveContext
 *
 * Description:
 *   Saves the given context
 *
 * Inputs:
 *   xSecureContextHandle: Context handle corresponding to the
 *   context to be saved
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void SecureContext_SaveContext(SecureContextHandle_t xSecureContextHandle);

/****************************************************************************
 * Name: SecureInit_DePrioritizeNSExceptions
 *
 * Description:
 *   De-prioritizes the non-secure exceptions.
 *
 * Inputs:
 *   None
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void SecureInit_DePrioritizeNSExceptions(void);

/****************************************************************************
 * Name: SecureInit_EnableNSFPUAccess
 *
 * Description:
 *   Sets up the Floating Point Unit (FPU) for Non-Secure access.
 *
 * Inputs:
 *   None
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void SecureInit_EnableNSFPUAccess(void);

/****************************************************************************
 * Name: rdp_protection_entry
 *
 * Description:
 *   Secure entry function and can be called by Non-secure functions.
 *
 * Inputs:
 *   idx: Data
 *
 * Returned Value:
 *   Result after processing data in the Secure world
 ****************************************************************************/

uint32_t rdp_protection_entry(uint32_t idx);

/****************************************************************************
 * Name: rdp_no_protection_call
 *
 * Description:
 *   Secure entry function and can be called by Non-secure functions.
 *   Passes a non-secure function pointer such that Secure world can call
 *   this function pointer to execute the non-secure function.
 *
 * Inputs:
 *   fp: function pointer of non-secure function
 *   idx: Data
 *
 * Returned Value:
 *   Result after processing data in the Non-secure world
 ****************************************************************************/

uint32_t rdp_no_protection_call(nsFunc *fp, uint32_t idx);

/****************************************************************************
 * Name: BOOT_IMG3
 *
 * Description:
 *   Boot secure IMG3
 *
 * Inputs:
 *   None
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void BOOT_IMG3(void);

#endif /* __NON_SECURE_CALLABLE_H__ */
