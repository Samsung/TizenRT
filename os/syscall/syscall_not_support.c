/****************************************************************************
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
 /****************************************************************************
 * syscall_not_support.c
 *
 * Provides default stub implementations for unsupported system calls.
 * These stubs set errno to ENOTSUP and return standard failure values.
 *
 * Copyright (C) 2025 Samsung Electronics
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <errno.h>
#include <sys/types.h>

void STUB_notsupport_void(void)
{
    set_errno(ENOTSUP);
}

int STUB_notsupport_int(void)
{
    set_errno(ENOTSUP);
    return ERROR;
}

void *STUB_notsupport_ptr(void)
{
    set_errno(ENOTSUP);
    return NULL;
}
