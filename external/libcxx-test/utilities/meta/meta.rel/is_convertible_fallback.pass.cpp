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
// UNSUPPORTED: c++03

// ADDITIONAL_COMPILE_FLAGS: -D _LIBCPP_USE_IS_CONVERTIBLE_FALLBACK

// UNSUPPORTED: gcc-13

// type_traits

// is_convertible

// Test the fallback implementation.

// libc++ provides a fallback implementation of the compiler trait
// `__is_convertible` with the same name when clang doesn't.
// Because this test forces the use of the fallback even when clang provides
// it causing a keyword incompatibility.

#include "test_macros.h"
TEST_CLANG_DIAGNOSTIC_IGNORED("-Wkeyword-compat")

#include "is_convertible.pass.cpp"
#include "libcxx_tc_common.h"
