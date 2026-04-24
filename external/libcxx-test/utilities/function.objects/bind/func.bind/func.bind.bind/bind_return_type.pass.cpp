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
//   unspecified bind(Fn, Types...);
// template<Returnable R, CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);

// Check that the call operators have the proper return type and that they
// only SFINAE away when too few arguments are provided. Otherwise they should
// be well formed and should ignore any additional arguments.

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int dummy = 42;

int return_value(int) { return dummy; }
int& return_lvalue(int) { return dummy; }
const int& return_const_lvalue(int) { return dummy; }
int&& return_rvalue(int) { return std::move(dummy); }
const int&& return_const_rvalue(int) { return std::move(dummy); }

template <class Bind, class ...Args>
auto CheckCallImp(int)
    -> decltype((std::declval<Bind>()(std::declval<Args>()...)), std::true_type{});

template <class Bind, class ...>
auto CheckCallImp(long) -> std::false_type;

template <class ...Args>
constexpr bool CheckCall() {
    return decltype(CheckCallImp<Args...>(0))::value;
}

template <class Expect, class Fn>
void do_test(Fn* func) {
    using namespace std::placeholders;
    auto ret = std::bind(func, _1);
    auto ret_r = std::bind<Expect>(func, _1);
    using Bind = decltype(ret);
    using BindR = decltype(ret_r);

    using Ret = decltype(ret(42));
    using Ret2 = decltype(ret(42, 43)); // Test that the extra argument is discarded.
    using RetR = decltype(ret_r(42));
    using RetR2 = decltype(ret_r(42, 43));

    static_assert(std::is_same<Ret, Expect>::value, "");
    static_assert(std::is_same<Ret2, Expect>::value, "");
    static_assert(std::is_same<RetR, Expect>::value, "");
    static_assert(std::is_same<RetR2, Expect>::value, "");

    Expect exp = ret(100); // the input value is ignored. dummy is returned.
    Expect exp2 = ret(101, 102);
    Expect exp_r = ret_r(100);
    Expect exp_r2 = ret_r(101, 102);

    TC_ASSERT_EXPR(exp == 42);
    TC_ASSERT_EXPR(exp2 == 42);
    TC_ASSERT_EXPR(exp_r == 42);
    TC_ASSERT_EXPR(exp_r2 == 42);

    if ((std::is_reference<Expect>::value)) {
        TC_ASSERT_EXPR(&exp == &dummy);
        TC_ASSERT_EXPR(&exp2 == &dummy);
        TC_ASSERT_EXPR(&exp_r == &dummy);
        TC_ASSERT_EXPR(&exp_r2 == &dummy);
    }
    // Check that the call operator SFINAE's away when too few arguments
    // are provided but is well-formed otherwise.
    {
        LIBCPP_STATIC_ASSERT(!CheckCall<Bind>(), "");
        static_assert(CheckCall<Bind, int>(), "");
        static_assert(CheckCall<Bind, int, int>(), "");
        LIBCPP_STATIC_ASSERT(!CheckCall<BindR>(), "");
        static_assert(CheckCall<BindR, int>(), "");
        static_assert(CheckCall<BindR, int, int>(), "");
    }
}


// Test but with an explicit return type which differs from the real one.
template <class Expect, class Fn>
void do_test_r(Fn* func) {
    using namespace std::placeholders;
    auto ret = std::bind<Expect>(func, _1);
    using Bind = decltype(ret);
    using Ret = decltype(ret(42));
    using Ret2 = decltype(ret(42, 43)); // Test that the extra argument is discarded.
    static_assert(std::is_same<Ret, Expect>::value, "");
    static_assert(std::is_same<Ret2, Expect>::value, "");
    Expect exp = ret(100); // the input value is ignored
    Expect exp2 = ret(101, 102);
    TC_ASSERT_EXPR(exp == 42);
    TC_ASSERT_EXPR(exp2 == 42);
    // Check that the call operator SFINAE's away when too few arguments
    // are provided but is well-formed otherwise.
    {
        LIBCPP_STATIC_ASSERT(!CheckCall<Bind>(), "");
        static_assert(CheckCall<Bind, int>(), "");
        static_assert(CheckCall<Bind, int, int>(), "");
    }
}

int tc_utilities_function_objects_bind_func_bind_func_bind_bind_bind_return_type(void) {
    do_test<int>(return_value);
    do_test<int&>(return_lvalue);
    do_test<const int&>(return_const_lvalue);
    do_test<int&&>(return_rvalue);
    do_test<const int&&>(return_const_rvalue);

    do_test_r<long>(return_value);
    do_test_r<long>(return_lvalue);
    do_test_r<long>(return_const_lvalue);
    do_test_r<long>(return_rvalue);
    do_test_r<long>(return_const_rvalue);


  return 0;
}
