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
// Copyright 2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

# include "export.h"
# include "randombytes.h"


/* Defining RANDOMBYTES_DEFAULT_IMPLEMENTATION here allows us to compile with the ESP32 hardware
   implementation as the default. No need to call randombytes_set_implementation().

   Doing it in the header like this is easier than passing it via a -D argument to gcc.
*/
#undef RANDOMBYTES_DEFAULT_IMPLEMENTATION

#ifdef CONFIG_ARCH_CHIP_ESP32
SODIUM_EXPORT
extern const struct randombytes_implementation randombytes_esp32_implementation;
#define RANDOMBYTES_DEFAULT_IMPLEMENTATION &randombytes_esp32_implementation
#else
SODIUM_EXPORT
extern const struct randombytes_implementation randombytes_soft_implementation;
#define RANDOMBYTES_DEFAULT_IMPLEMENTATION &randombytes_soft_implementation
#endif
