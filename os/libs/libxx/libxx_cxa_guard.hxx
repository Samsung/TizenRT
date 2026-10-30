/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/compiler.h>

//***************************************************************************
// Pre-processor Definitions
//***************************************************************************

//***************************************************************************
// Private Types
//***************************************************************************

#ifdef __ARM_EABI__
// The 32-bit ARM C++ ABI specifies that the guard is a 32-bit
// variable and the least significant bit contains 0 prior to
// initialization, and 1 after.

typedef int __guard;

#else
// The "standard" C++ ABI specifies that the guard is a 64-bit
// variable and the first byte contains 0 prior to initialization, and
// 1 after.

__extension__ typedef int __guard __attribute__((mode(__DI__)));
#endif
