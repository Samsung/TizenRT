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

// template<CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);    // constexpr since C++20
// template<Returnable R, CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);    // constexpr since C++20

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class R, class F>
TEST_CONSTEXPR_CXX20
void test(F f, R expected) {
    TC_ASSERT_EXPR(f() == expected);
}

template <class R, class F>
TEST_CONSTEXPR_CXX20
void test_const(const F& f, R expected) {
    TC_ASSERT_EXPR(f() == expected);
}

TEST_CONSTEXPR_CXX20 int f() {return 1;}

struct A_int_0 {
    TEST_CONSTEXPR_CXX20 int operator()() {return 4;}
    TEST_CONSTEXPR_CXX20 int operator()() const {return 5;}
};

TEST_CONSTEXPR_CXX20 bool test_all() {
    test(std::bind(f), 1);
    test(std::bind(&f), 1);
    test(std::bind(A_int_0()), 4);
    test_const(std::bind(A_int_0()), 5);

    test(std::bind<int>(f), 1);
    test(std::bind<int>(&f), 1);
    test(std::bind<int>(A_int_0()), 4);
    test_const(std::bind<int>(A_int_0()), 5);
    return true;
}

int tc_utilities_function_objects_bind_func_bind_func_bind_bind_invoke_int_0(void) {
    test_all();
#if TEST_STD_VER >= 20
    static_assert(test_all());
#endif

    return 0;
}
