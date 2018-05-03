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

// template <class U, class V> EXPLICIT constexpr pair(const pair<U, V>& p);

#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "archetypes.hpp"
#include "test_convertible.hpp"
using namespace ImplicitTypes; // Get implicitly archetypes

template <class T1, class U1,
          bool CanCopy = true, bool CanConvert = CanCopy>
static int test_pair_const()
{
    using P1 = std::pair<T1, int>;
    using P2 = std::pair<int, T1>;
    using UP1 = std::pair<U1, int> const&;
    using UP2 = std::pair<int, U1> const&;
    static_assert(std::is_constructible<P1, UP1>::value == CanCopy, "");
    static_assert(test_convertible<P1, UP1>() == CanConvert, "");
    static_assert(std::is_constructible<P2, UP2>::value == CanCopy, "");
    static_assert(test_convertible<P2,  UP2>() == CanConvert, "");
    return 0;
}

template <class T, class U>
struct DPair : public std::pair<T, U> {
  using Base = std::pair<T, U>;
  using Base::Base;
};

struct ExplicitT {
  constexpr explicit ExplicitT(int x) : value(x) {}
  constexpr explicit ExplicitT(ExplicitT const& o) : value(o.value) {}
  int value;
};

struct ImplicitT {
  constexpr ImplicitT(int x) : value(x) {}
  constexpr ImplicitT(ImplicitT const& o) : value(o.value) {}
  int value;
};

int tc_libcxx_utilities_pairs_pair_const_pair_U_V(void)
{
    {
        typedef std::pair<int, int> P1;
        typedef std::pair<double, long> P2;
        const P1 p1(3, 4);
        const P2 p2 = p1;
        TC_ASSERT_EXPR(p2.first == 3);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    {
        // We allow derived types to use this constructor
        using P1 = DPair<long, long>;
        using P2 = std::pair<int, int>;
        P1 p1(42, 101);
        P2 p2(p1);
        TC_ASSERT_EXPR(p2.first == 42);
        TC_ASSERT_EXPR(p2.second == 101);
    }
    {
        test_pair_const<AllCtors, AllCtors>(); // copy construction
    TC_ASSERT_FUNC((test_pair_const<AllCtors, AllCtors&>()));
    TC_ASSERT_FUNC((test_pair_const<AllCtors, AllCtors&&>()));
    TC_ASSERT_FUNC((test_pair_const<AllCtors, const AllCtors&>()));
    TC_ASSERT_FUNC((test_pair_const<AllCtors, const AllCtors&&>()));

        test_pair_const<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors>(); // copy construction
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::AllCtors, const ExplicitTypes::AllCtors&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::AllCtors, const ExplicitTypes::AllCtors&&, true, false>()));

        test_pair_const<MoveOnly, MoveOnly, false>(); // copy construction
    TC_ASSERT_FUNC((test_pair_const<MoveOnly, MoveOnly&, false>()));
    TC_ASSERT_FUNC((test_pair_const<MoveOnly, MoveOnly&&, false>()));

        test_pair_const<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly, false>(); // copy construction
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&&, false>()));

    TC_ASSERT_FUNC((test_pair_const<CopyOnly, CopyOnly>()));
    TC_ASSERT_FUNC((test_pair_const<CopyOnly, CopyOnly&>()));
    TC_ASSERT_FUNC((test_pair_const<CopyOnly, CopyOnly&&>()));

    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&&, true, false>()));

    TC_ASSERT_FUNC((test_pair_const<NonCopyable, NonCopyable, false>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable, NonCopyable&, false>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable, NonCopyable&&, false>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable, const NonCopyable&, false>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable, const NonCopyable&&, false>()));
    }

    { // Test construction of references
    TC_ASSERT_FUNC((test_pair_const<NonCopyable&, NonCopyable&>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable&, NonCopyable&&>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable&, NonCopyable const&, false>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable const&, NonCopyable&&>()));
    TC_ASSERT_FUNC((test_pair_const<NonCopyable&&, NonCopyable&&, false>()));

    TC_ASSERT_FUNC((test_pair_const<ConvertingType&, int, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType&, int, false>()));
        // Unfortunately the below conversions are allowed and create dangling
        // references.
        //test_pair_const<ConvertingType&&, int>();
        //test_pair_const<ConvertingType const&, int>();
        //test_pair_const<ConvertingType const&&, int>();
        // But these are not because the converting constructor is explicit.
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType&&, int, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType const&, int, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType const&&, int, false>()));

    }
    {
    TC_ASSERT_FUNC((test_pair_const<AllCtors, int, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::AllCtors, int, false>()));
    TC_ASSERT_FUNC((test_pair_const<ConvertingType, int>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, int, true, false>()));

    TC_ASSERT_FUNC((test_pair_const<ConvertingType, int>()));
    TC_ASSERT_FUNC((test_pair_const<ConvertingType, ConvertingType>()));
    TC_ASSERT_FUNC((test_pair_const<ConvertingType, ConvertingType const&>()));
    TC_ASSERT_FUNC((test_pair_const<ConvertingType, ConvertingType&>()));
    TC_ASSERT_FUNC((test_pair_const<ConvertingType, ConvertingType&&>()));

    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, int, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, int&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, const int&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, int&&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, const int&&, true, false>()));

    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType const&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType&, true, false>()));
    TC_ASSERT_FUNC((test_pair_const<ExplicitTypes::ConvertingType, ExplicitTypes::ConvertingType&&, true, false>()));
    }
#if TEST_STD_VER > 11
    {
        typedef std::pair<int, int> P1;
        typedef std::pair<double, long> P2;
        constexpr P1 p1(3, 4);
        constexpr P2 p2 = p1;
        static_assert(p2.first == 3, "");
        static_assert(p2.second == 4, "");
    }
    {
        using P1 = std::pair<int, int>;
        using P2 = std::pair<ExplicitT, ExplicitT>;
        constexpr P1 p1(42, 101);
        constexpr P2 p2(p1);
        static_assert(p2.first.value == 42, "");
        static_assert(p2.second.value == 101, "");
    }
    {
        using P1 = std::pair<int, int>;
        using P2 = std::pair<ImplicitT, ImplicitT>;
        constexpr P1 p1(42, 101);
        constexpr P2 p2 = p1;
        static_assert(p2.first.value == 42, "");
        static_assert(p2.second.value == 101, "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
