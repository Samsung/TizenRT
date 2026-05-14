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
// <tuple>

// template <class... Types> class tuple;

// explicit tuple(const T&...);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class ...>
struct never {
    enum { value = 0 };
};

struct NoValueCtor
{
    NoValueCtor() : id(++count) {}
    NoValueCtor(NoValueCtor const & other) : id(other.id) { ++count; }

    // The constexpr is required to make is_constructible instantiate this template.
    // The explicit is needed to test-around a similar bug with is_convertible.
    template <class T>
    constexpr explicit NoValueCtor(T)
    { static_assert(never<T>::value, "This should not be instantiated"); }

    static int count;
    int id;
};

int NoValueCtor::count = 0;


struct NoValueCtorEmpty
{
    NoValueCtorEmpty() {}
    NoValueCtorEmpty(NoValueCtorEmpty const &) {}

    template <class T>
    constexpr explicit NoValueCtorEmpty(T)
    { static_assert(never<T>::value, "This should not be instantiated"); }
};


struct ImplicitCopy {
  explicit ImplicitCopy(int) {}
  ImplicitCopy(ImplicitCopy const&) {}
};

// Test that tuple(std::allocator_arg, Alloc, Types const&...) allows implicit
// copy conversions in return value expressions.
std::tuple<ImplicitCopy> testImplicitCopy1() {
    ImplicitCopy i(42);
    return {i};
}

std::tuple<ImplicitCopy> testImplicitCopy2() {
    const ImplicitCopy i(42);
    return {i};
}

std::tuple<ImplicitCopy> testImplicitCopy3() {
    const ImplicitCopy i(42);
    return i;
}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_const_Types(void) {
    {
        // check that the literal '0' can implicitly initialize a stored pointer.
        std::tuple<int*> t = 0;
        TC_ASSERT_EXPR(std::get<0>(t) == nullptr);
    }
    {
        std::tuple<int> t(2);
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
    }
#if TEST_STD_VER > 11
    {
        constexpr std::tuple<int> t(2);
        static_assert(std::get<0>(t) == 2, "");
    }
    {
        constexpr std::tuple<int> t;
        static_assert(std::get<0>(t) == 0, "");
    }
#endif
    {
        std::tuple<int, char*> t(2, 0);
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
        TC_ASSERT_EXPR(std::get<1>(t) == nullptr);
    }
#if TEST_STD_VER > 11
    {
        constexpr std::tuple<int, char*> t(2, nullptr);
        static_assert(std::get<0>(t) == 2, "");
        static_assert(std::get<1>(t) == nullptr, "");
    }
#endif
    {
        std::tuple<int, char*> t(2, nullptr);
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
        TC_ASSERT_EXPR(std::get<1>(t) == nullptr);
    }
    {
        std::tuple<int, char*, std::string> t(2, nullptr, "text");
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
        TC_ASSERT_EXPR(std::get<1>(t) == nullptr);
        TC_ASSERT_EXPR(std::get<2>(t) == "text");
    }
    // __tuple_leaf<T> uses is_constructible<T, U> to disable its explicit converting
    // constructor overload __tuple_leaf(U &&). Evaluating is_constructible can cause a compile error.
    // This overload is evaluated when __tuple_leafs copy or move ctor is called.
    // This checks that is_constructible is not evaluated when U == __tuple_leaf.
    {
        std::tuple<int, NoValueCtor, int, int> t(1, NoValueCtor(), 2, 3);
        TC_ASSERT_EXPR(std::get<0>(t) == 1);
        TC_ASSERT_EXPR(std::get<1>(t).id == 1);
        TC_ASSERT_EXPR(std::get<2>(t) == 2);
        TC_ASSERT_EXPR(std::get<3>(t) == 3);
    }
    {
        std::tuple<int, NoValueCtorEmpty, int, int> t(1, NoValueCtorEmpty(), 2, 3);
        TC_ASSERT_EXPR(std::get<0>(t) == 1);
        TC_ASSERT_EXPR(std::get<2>(t) == 2);
        TC_ASSERT_EXPR(std::get<3>(t) == 3);
    }

  return 0;
}
