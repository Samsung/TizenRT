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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <utility>

// template <class T1, class T2> struct pair

// template <class U, class V> pair(pair<U, V>&& p);

#include <utility>
#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"

#include "archetypes.hpp"
#include "test_convertible.hpp"
using namespace ImplicitTypes; // Get implicitly archetypes

template <class T1, class U1,
          bool CanCopy = true, bool CanConvert = CanCopy>
static int test_pair_rv()
{
    using P1 = std::pair<T1, int>;
    using P2 = std::pair<int, T1>;
    using UP1 = std::pair<U1, int>&&;
    using UP2 = std::pair<int, U1>&&;
    static_assert(std::is_constructible<P1, UP1>::value == CanCopy, "");
    static_assert(test_convertible<P1, UP1>() == CanConvert, "");
    static_assert(std::is_constructible<P2, UP2>::value == CanCopy, "");
    static_assert(test_convertible<P2,  UP2>() == CanConvert, "");
    return 0;
}

struct Base
{
    virtual ~Base() {}
};

struct Derived
    : public Base
{
};


template <class T, class U>
struct DPair : public std::pair<T, U> {
  using Base = std::pair<T, U>;
  using Base::Base;
};

struct ExplicitT {
  constexpr explicit ExplicitT(int x) : value(x) {}
  int value;
};

struct ImplicitT {
  constexpr ImplicitT(int x) : value(x) {}
  int value;
};

int tc_libcxx_utilities_pairs_pair_rv_pair_U_V(void)
{
    {
        typedef std::pair<std::unique_ptr<Derived>, int> P1;
        typedef std::pair<std::unique_ptr<Base>, long> P2;
        P1 p1(std::unique_ptr<Derived>(), 4);
        P2 p2 = std::move(p1);
        TC_ASSERT_EXPR(p2.first == nullptr);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    {
        // We allow derived types to use this constructor
        using P1 = DPair<long, long>;
        using P2 = std::pair<int, int>;
        P1 p1(42, 101);
        P2 p2(std::move(p1));
        TC_ASSERT_EXPR(p2.first == 42);
        TC_ASSERT_EXPR(p2.second == 101);
    }
    {
    TC_ASSERT_FUNC((test_pair_rv<AllCtors, AllCtors>()));
    TC_ASSERT_FUNC((test_pair_rv<AllCtors, AllCtors&>()));
    TC_ASSERT_FUNC((test_pair_rv<AllCtors, AllCtors&&>()));
    TC_ASSERT_FUNC((test_pair_rv<AllCtors, const AllCtors&>()));
    TC_ASSERT_FUNC((test_pair_rv<AllCtors, const AllCtors&&>()));

    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::AllCtors, const ExplicitTypes::AllCtors&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::AllCtors, const ExplicitTypes::AllCtors&&, true, false>()));

    TC_ASSERT_FUNC((test_pair_rv<MoveOnly, MoveOnly>()));
    TC_ASSERT_FUNC((test_pair_rv<MoveOnly, MoveOnly&, false>()));
    TC_ASSERT_FUNC((test_pair_rv<MoveOnly, MoveOnly&&>()));

        test_pair_rv<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly>(); // copy construction
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&&, true, false>()));

    TC_ASSERT_FUNC((test_pair_rv<CopyOnly, CopyOnly>()));
    TC_ASSERT_FUNC((test_pair_rv<CopyOnly, CopyOnly&>()));
    TC_ASSERT_FUNC((test_pair_rv<CopyOnly, CopyOnly&&>()));

    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&&, true, false>()));

    TC_ASSERT_FUNC((test_pair_rv<NonCopyable, NonCopyable, false>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable, NonCopyable&, false>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable, NonCopyable&&, false>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable, const NonCopyable&, false>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable, const NonCopyable&&, false>()));
    }
    { // Test construction of references
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable&, NonCopyable&>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable&, NonCopyable&&>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable&, NonCopyable const&, false>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable const&, NonCopyable&&>()));
    TC_ASSERT_FUNC((test_pair_rv<NonCopyable&&, NonCopyable&&>()));

    TC_ASSERT_FUNC((test_pair_rv<ConvertingType&, int, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType&, int, false>()));
        // Unfortunately the below conversions are allowed and create dangling
        // references.
        //test_pair_rv<ConvertingType&&, int>();
        //test_pair_rv<ConvertingType const&, int>();
        //test_pair_rv<ConvertingType const&&, int>();
        // But these are not because the converting constructor is explicit.
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType&&, int, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType const&, int, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType const&&, int, false>()));
    }
    {
    TC_ASSERT_FUNC((test_pair_rv<AllCtors, int, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::AllCtors, int, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ConvertingType, int>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, int, true, false>()));

    TC_ASSERT_FUNC((test_pair_rv<ConvertingType, int>()));
    TC_ASSERT_FUNC((test_pair_rv<ConvertingType, ConvertingType>()));
    TC_ASSERT_FUNC((test_pair_rv<ConvertingType, ConvertingType const&>()));
    TC_ASSERT_FUNC((test_pair_rv<ConvertingType, ConvertingType&>()));
    TC_ASSERT_FUNC((test_pair_rv<ConvertingType, ConvertingType&&>()));

    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, int, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, int&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, const int&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, int&&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, const int&&, true, false>()));

    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType const&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType&, true, false>()));
    TC_ASSERT_FUNC((test_pair_rv<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType&&, true, false>()));
    }
#if TEST_STD_VER > 11
    { // explicit constexpr test
        constexpr std::pair<int, int> p1(42, 43);
        constexpr std::pair<ExplicitT, ExplicitT> p2(std::move(p1));
        static_assert(p2.first.value == 42, "");
        static_assert(p2.second.value == 43, "");
    }
    { // implicit constexpr test
        constexpr std::pair<int, int> p1(42, 43);
        constexpr std::pair<ImplicitT, ImplicitT> p2 = std::move(p1);
        static_assert(p2.first.value == 42, "");
        static_assert(p2.second.value == 43, "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
