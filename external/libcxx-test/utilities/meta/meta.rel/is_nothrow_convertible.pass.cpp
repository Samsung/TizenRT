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
//

// <type_traits>
// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};
struct B {
public:
    operator A() { return a; } A a;
};

class C { };
class D {
public:
    operator C() noexcept { return c; } C c;
};

int tc_utilities_meta_meta_rel_is_nothrow_convertible(void) {
    static_assert((std::is_nothrow_convertible<int, double>::value), "");
    static_assert(!(std::is_nothrow_convertible<int, char*>::value), "");

    static_assert(!(std::is_nothrow_convertible<A, B>::value), "");
    static_assert((std::is_nothrow_convertible<D, C>::value), "");

    static_assert((std::is_nothrow_convertible_v<int, double>), "");
    static_assert(!(std::is_nothrow_convertible_v<int, char*>), "");

    static_assert(!(std::is_nothrow_convertible_v<A, B>), "");
    static_assert((std::is_nothrow_convertible_v<D, C>), "");

    static_assert((std::is_nothrow_convertible_v<const void, void>), "");
    static_assert((std::is_nothrow_convertible_v<volatile void, void>), "");
    static_assert((std::is_nothrow_convertible_v<void, const void>), "");
    static_assert((std::is_nothrow_convertible_v<void, volatile void>), "");

    static_assert(!(std::is_nothrow_convertible_v<int[], double[]>), "");
    static_assert(!(std::is_nothrow_convertible_v<int[], int[]>), "");
    static_assert(!(std::is_nothrow_convertible_v<int[10], int[10]>), "");
    static_assert(!(std::is_nothrow_convertible_v<int[10], double[10]>), "");
    static_assert(!(std::is_nothrow_convertible_v<int[5], double[10]>), "");
    static_assert(!(std::is_nothrow_convertible_v<int[10], A[10]>), "");

    typedef void V();
    typedef int I();
    static_assert(!(std::is_nothrow_convertible_v<V, V>), "");
    static_assert(!(std::is_nothrow_convertible_v<V, I>), "");

    return 0;
}
