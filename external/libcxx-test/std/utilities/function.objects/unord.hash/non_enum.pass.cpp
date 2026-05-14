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

// <functional>

//  Hashing a struct w/o a defined hash should *not* fail, but it should
// create a type that is not constructible and not callable.
// See also: https://cplusplus.github.io/LWG/lwg-defects.html#2543

#include <functional>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct X {};

int tc_utilities_function_objects_unord_hash_non_enum(void) {
    using H = std::hash<X>;
    static_assert(!std::is_default_constructible<H>::value, "");
    static_assert(!std::is_copy_constructible<H>::value, "");
    static_assert(!std::is_move_constructible<H>::value, "");
    static_assert(!std::is_copy_assignable<H>::value, "");
    static_assert(!std::is_move_assignable<H>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_invocable<H, X&>::value, "");
    static_assert(!std::is_invocable<H, X const&>::value, "");
#endif

  return 0;
}
