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
// UNSUPPORTED: c++03 && !stdlib=libc++

// <utility>

// template <class T>
//     typename conditional
//     <
//         !is_nothrow_move_constructible<T>::value && is_copy_constructible<T>::value,
//         const T&,
//         T&&
//     >::type
//     move_if_noexcept(T& x);

#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class A
{
    A(const A&);
    A& operator=(const A&);
public:

    A() {}
    A(A&&) {}
};

struct legacy
{
    legacy() {}
    legacy(const legacy&);
};

int tc_utilities_utility_forward_move_if_noexcept(void) {
    int i = 0;
    const int ci = 0;

    legacy l;
    A a;
    const A ca;

    static_assert((std::is_same<decltype(std::move_if_noexcept(i)), int&&>::value), "");
    static_assert((std::is_same<decltype(std::move_if_noexcept(ci)), const int&&>::value), "");
    static_assert((std::is_same<decltype(std::move_if_noexcept(a)), A&&>::value), "");
    static_assert((std::is_same<decltype(std::move_if_noexcept(ca)), const A&&>::value), "");
    static_assert((std::is_same<decltype(std::move_if_noexcept(l)), const legacy&>::value), "");

#if TEST_STD_VER > 11
    constexpr int i1 = 23;
    constexpr int i2 = std::move_if_noexcept(i1);
    static_assert(i2 == 23, "" );
#endif


  return 0;
}
