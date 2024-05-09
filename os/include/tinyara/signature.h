/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_SIGNATURE_H
#define __INCLUDE_TINYARA_SIGNATURE_H

#include <tinyara/config.h>
#include <stdint.h>

/* The size of prepended binary signing data which is board-specific value */
#ifdef CONFIG_USER_SIGN_PREPEND_SIZE
#define USER_SIGN_PREPEND_SIZE     CONFIG_USER_SIGN_PREPEND_SIZE
#else
#define USER_SIGN_PREPEND_SIZE     0
#endif

#define SIGNATURE_VAILD      0
#define SIGNATURE_INVALID     1

int up_verify_kernelsignature(uint32_t address);

#ifdef CONFIG_APP_BINARY_SEPARATION
int up_verify_usersignature(uint32_t address);
#endif

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_SIGNATURE_H */
