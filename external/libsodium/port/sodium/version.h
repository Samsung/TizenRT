/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#ifndef sodium_version_H
#define sodium_version_H

#include <sodium/export.h>

// ATTENTION: Update these version manually once the submoudle updated!
#define SODIUM_VERSION_STRING "1.0.12-tizenrt"

#define SODIUM_LIBRARY_VERSION_MAJOR 9
#define SODIUM_LIBRARY_VERSION_MINOR 4

#ifdef __cplusplus
extern "C" {
#endif

SODIUM_EXPORT
const char *sodium_version_string(void);

SODIUM_EXPORT
int sodium_library_version_major(void);

SODIUM_EXPORT
int sodium_library_version_minor(void);

SODIUM_EXPORT
int sodium_library_minimal(void);

#ifdef __cplusplus
}
#endif

#endif
