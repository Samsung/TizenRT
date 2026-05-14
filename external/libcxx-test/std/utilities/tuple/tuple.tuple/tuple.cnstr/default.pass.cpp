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

// explicit(see-below) constexpr tuple();

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "DefaultOnly.h"
#include "libcxx_tc_common.h"

struct NoDefault {
    NoDefault() = delete;
    explicit NoDefault(int) { }
};

struct NoExceptDefault {
    NoExceptDefault() noexcept = default;
};

struct ThrowingDefault {
    ThrowingDefault() { }
};

struct IllFormedDefault {
    IllFormedDefault(int x) : value(x) {}
    template <bool Pred = false>
    constexpr IllFormedDefault() {
        static_assert(Pred,
            "The default constructor should not be instantiated");
    }
    int value;
};

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_default(void) {
    {
        std::tuple<> t;
        (void)t;
    }
    {
        std::tuple<int> t;
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
    }
    {
        std::tuple<int, char*> t;
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
        TC_ASSERT_EXPR(std::get<1>(t) == nullptr);
    }
    {
        std::tuple<int, char*, std::string> t;
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
        TC_ASSERT_EXPR(std::get<1>(t) == nullptr);
        TC_ASSERT_EXPR(std::get<2>(t) == "");
    }
    {
        std::tuple<int, char*, std::string, DefaultOnly> t;
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
        TC_ASSERT_EXPR(std::get<1>(t) == nullptr);
        TC_ASSERT_EXPR(std::get<2>(t) == "");
        TC_ASSERT_EXPR(std::get<3>(t) == DefaultOnly());
    }
    {
        // See bug #21157.
        static_assert(!std::is_default_constructible<std::tuple<NoDefault>>(), "");
        static_assert(!std::is_default_constructible<std::tuple<DefaultOnly, NoDefault>>(), "");
        static_assert(!std::is_default_constructible<std::tuple<NoDefault, DefaultOnly, NoDefault>>(), "");
    }
    {
        static_assert(noexcept(std::tuple<NoExceptDefault>()), "");
        static_assert(noexcept(std::tuple<NoExceptDefault, NoExceptDefault>()), "");

        static_assert(!noexcept(std::tuple<ThrowingDefault, NoExceptDefault>()), "");
        static_assert(!noexcept(std::tuple<NoExceptDefault, ThrowingDefault>()), "");
        static_assert(!noexcept(std::tuple<ThrowingDefault, ThrowingDefault>()), "");
    }
    {
        constexpr std::tuple<> t;
        (void)t;
    }
    {
        constexpr std::tuple<int> t;
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
    }
    {
        constexpr std::tuple<int, char*> t;
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
        TC_ASSERT_EXPR(std::get<1>(t) == nullptr);
    }
    {
    // Check that the SFINAE on the default constructor is not evaluated when
    // it isn't needed. If the default constructor is evaluated then this test
    // should fail to compile.
        IllFormedDefault v(0);
        std::tuple<IllFormedDefault> t(v);
    }
    {
        struct Base { };
        struct Derived : Base { protected: Derived() = default; };
        static_assert(!std::is_default_constructible<std::tuple<Derived, int> >::value, "");
    }

    return 0;
}
