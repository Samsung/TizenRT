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

// template<class U, class V> pair(U&& x, V&& y);


#include <utility>
#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"

#include "archetypes.hpp"
#include "test_convertible.hpp"
using namespace ImplicitTypes; // Get implicitly archetypes

template <class T1, class T1Arg,
          bool CanCopy = true, bool CanConvert = CanCopy>
static int test_sfinae() {
    using P1 = std::pair<T1, int>;
    using P2 = std::pair<int, T1>;
    using T2 = int const&;
    static_assert(std::is_constructible<P1, T1Arg, T2>::value == CanCopy, "");
    static_assert(test_convertible<P1,   T1Arg, T2>() == CanConvert, "");
    static_assert(std::is_constructible<P2, T2,   T1Arg>::value == CanCopy, "");
    static_assert(test_convertible<P2,   T2,   T1Arg>() == CanConvert, "");
    return 0;
}

struct ExplicitT {
  constexpr explicit ExplicitT(int x) : value(x) {}
  int value;
};

struct ImplicitT {
  constexpr ImplicitT(int x) : value(x) {}
  int value;
};


int tc_libcxx_utilities_pairs_pair_U_V(void)
{
    {
        typedef std::pair<std::unique_ptr<int>, short*> P;
        P p(std::unique_ptr<int>(new int(3)), nullptr);
        TC_ASSERT_EXPR(*p.first == 3);
        TC_ASSERT_EXPR(p.second == nullptr);
    }
    {
        // Test non-const lvalue and rvalue types
    TC_ASSERT_FUNC((test_sfinae<AllCtors, AllCtors&>()));
    TC_ASSERT_FUNC((test_sfinae<AllCtors, AllCtors&&>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<CopyOnly, CopyOnly&>()));
    TC_ASSERT_FUNC((test_sfinae<CopyOnly, CopyOnly&&>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<MoveOnly, MoveOnly&, false>()));
    TC_ASSERT_FUNC((test_sfinae<MoveOnly, MoveOnly&&>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<NonCopyable, NonCopyable&, false>()));
    TC_ASSERT_FUNC((test_sfinae<NonCopyable, NonCopyable&&, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::NonCopyable, ExplicitTypes::NonCopyable&, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::NonCopyable, ExplicitTypes::NonCopyable&&, false>()));
    }
    {
        // Test converting types
    TC_ASSERT_FUNC((test_sfinae<ConvertingType, int&>()));
    TC_ASSERT_FUNC((test_sfinae<ConvertingType, const int&>()));
    TC_ASSERT_FUNC((test_sfinae<ConvertingType, int&&>()));
    TC_ASSERT_FUNC((test_sfinae<ConvertingType, const int&&>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::ConvertingType, int&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::ConvertingType, const int&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::ConvertingType, int&&, true, false>()));
    TC_ASSERT_FUNC((test_sfinae<ExplicitTypes::ConvertingType, const int&&, true, false>()));
    }
#if TEST_STD_VER > 11
    { // explicit constexpr test
        constexpr std::pair<ExplicitT, ExplicitT> p(42, 43);
        static_assert(p.first.value == 42, "");
        static_assert(p.second.value == 43, "");
    }
    { // implicit constexpr test
        constexpr std::pair<ImplicitT, ImplicitT> p = {42, 43};
        static_assert(p.first.value == 42, "");
        static_assert(p.second.value == 43, "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
