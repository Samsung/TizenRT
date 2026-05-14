//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// template <class U, class... Args>
//     constexpr
//     explicit optional(in_place_t, initializer_list<U> il, Args&&... args);

#include <optional>
#include <type_traits>
#include <memory>
#include <vector>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;
using std::in_place_t;
using std::in_place;

class X
{
    int i_;
    int j_ = 0;
public:
    X() : i_(0) {}
    X(int i) : i_(i) {}
    X(int i, int j) : i_(i), j_(j) {}

    ~X() {}

    friend bool operator==(const X& x, const X& y)
        {return x.i_ == y.i_ && x.j_ == y.j_;}
};

class Y
{
    int i_;
    int j_ = 0;
public:
    constexpr Y() : i_(0) {}
    constexpr Y(int i) : i_(i) {}
    constexpr Y(std::initializer_list<int> il) : i_(il.begin()[0]), j_(il.begin()[1]) {}

    friend constexpr bool operator==(const Y& x, const Y& y)
        {return x.i_ == y.i_ && x.j_ == y.j_;}
};

class Z
{
    int i_;
    int j_ = 0;
public:
    Z() : i_(0) {}
    Z(int i) : i_(i) {}
    Z(std::initializer_list<int> il) : i_(il.begin()[0]), j_(il.begin()[1])
        {TEST_THROW(6);}

    friend bool operator==(const Z& x, const Z& y)
        {return x.i_ == y.i_ && x.j_ == y.j_;}
};

int tc_utilities_optional_optional_object_optional_object_ctor_initializer_list(void) {
    {
        static_assert(!std::is_constructible<X, std::initializer_list<int>&>::value, "");
        static_assert(!std::is_constructible<optional<X>, std::initializer_list<int>&>::value, "");
    }
    {
        optional<std::vector<int>> opt(in_place, {3, 1});
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR((*opt == std::vector<int>{3, 1}));
        TC_ASSERT_EXPR(opt->size() == 2);
    }
    {
        optional<std::vector<int>> opt(in_place, {3, 1}, std::allocator<int>());
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR((*opt == std::vector<int>{3, 1}));
        TC_ASSERT_EXPR(opt->size() == 2);
    }
    {
        static_assert(std::is_constructible<optional<Y>, std::initializer_list<int>&>::value, "");
        constexpr optional<Y> opt(in_place, {3, 1});
        static_assert(static_cast<bool>(opt) == true, "");
        static_assert(*opt == Y{3, 1}, "");

        struct test_constexpr_ctor
            : public optional<Y>
        {
            constexpr test_constexpr_ctor(in_place_t, std::initializer_list<int> i)
                : optional<Y>(in_place, i) {}
        };

    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        static_assert(std::is_constructible<optional<Z>, std::initializer_list<int>&>::value, "");
        try
        {
            optional<Z> opt(in_place, {3, 1});
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 6);
        }
    }
#endif

  return 0;
}
