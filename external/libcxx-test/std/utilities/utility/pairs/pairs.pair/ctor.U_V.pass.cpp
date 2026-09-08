//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <utility>

// template <class T1, class T2> struct pair

// template<class U = T1, class V = T2> pair(U&&, V&&);

#include <utility>
#include <cassert>
#include <memory>

#include "archetypes.h"
#include "test_convertible.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"
using namespace ImplicitTypes; // Get implicitly archetypes

template <class T1, class T1Arg,
          bool CanCopy = true, bool CanConvert = CanCopy>
void test_sfinae() {
    using P1 = std::pair<T1, int>;
    using P2 = std::pair<int, T1>;
    using T2 = int const&;
    static_assert(std::is_constructible<P1, T1Arg, T2>::value == CanCopy, "");
    static_assert(test_convertible<P1,   T1Arg, T2>() == CanConvert, "");
    static_assert(std::is_constructible<P2, T2,   T1Arg>::value == CanCopy, "");
    static_assert(test_convertible<P2,   T2,   T1Arg>() == CanConvert, "");
}

struct ExplicitT {
  constexpr explicit ExplicitT(int x) : value(x) {}
  int value;
};

struct ImplicitT {
  constexpr ImplicitT(int x) : value(x) {}
  int value;
};

int tc_utilities_utility_pairs_pairs_pair_ctor_U_V(void) {
    {
        typedef std::pair<std::unique_ptr<int>, short*> P;
        P p(std::unique_ptr<int>(new int(3)), nullptr);
        TC_ASSERT_EXPR(*p.first == 3);
        TC_ASSERT_EXPR(p.second == nullptr);
    }
    {
        // Test non-const lvalue and rvalue types
        test_sfinae<AllCtors, AllCtors&>();
        test_sfinae<AllCtors, AllCtors&&>();
        test_sfinae<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&, true, false>();
        test_sfinae<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&&, true, false>();
        test_sfinae<CopyOnly, CopyOnly&>();
        test_sfinae<CopyOnly, CopyOnly&&>();
        test_sfinae<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&, true, false>();
        test_sfinae<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&&, true, false>();
        test_sfinae<MoveOnly, MoveOnly&, false>();
        test_sfinae<MoveOnly, MoveOnly&&>();
        test_sfinae<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&, false>();
        test_sfinae<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&&, true, false>();
        test_sfinae<NonCopyable, NonCopyable&, false>();
        test_sfinae<NonCopyable, NonCopyable&&, false>();
        test_sfinae<ExplicitTypes::NonCopyable, ExplicitTypes::NonCopyable&, false>();
        test_sfinae<ExplicitTypes::NonCopyable, ExplicitTypes::NonCopyable&&, false>();
    }
    {
        // Test converting types
        test_sfinae<ConvertingType, int&>();
        test_sfinae<ConvertingType, const int&>();
        test_sfinae<ConvertingType, int&&>();
        test_sfinae<ConvertingType, const int&&>();
        test_sfinae<ExplicitTypes::ConvertingType, int&, true, false>();
        test_sfinae<ExplicitTypes::ConvertingType, const int&, true, false>();
        test_sfinae<ExplicitTypes::ConvertingType, int&&, true, false>();
        test_sfinae<ExplicitTypes::ConvertingType, const int&&, true, false>();
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

    // Test support for http://wg21.link/P1951, default arguments for pair's constructor.
    // Basically, this turns copies for brace initialization into moves.
#if TEST_STD_VER > 20
    {
        struct TrackInit {
            TrackInit() = default;
            constexpr TrackInit(TrackInit const& other) : wasMoveInit(other.wasMoveInit), wasCopyInit(true) { }
            constexpr TrackInit(TrackInit&& other) : wasMoveInit(true), wasCopyInit(other.wasCopyInit) { }
            bool wasMoveInit = false;
            bool wasCopyInit = false;
        };

        // Explicit constructor
        {
            {
                std::pair<TrackInit, int> p({}, 3);
                TC_ASSERT_EXPR( p.first.wasMoveInit);
                TC_ASSERT_EXPR(!p.first.wasCopyInit);
            }
            {
                std::pair<int, TrackInit> p(3, {});
                TC_ASSERT_EXPR( p.second.wasMoveInit);
                TC_ASSERT_EXPR(!p.second.wasCopyInit);
            }
        }

        // Implicit constructor
        {
            {
                std::pair<TrackInit, int> p = {{}, 3};
                TC_ASSERT_EXPR( p.first.wasMoveInit);
                TC_ASSERT_EXPR(!p.first.wasCopyInit);
            }
            {
                std::pair<int, TrackInit> p = {3, {}};
                TC_ASSERT_EXPR( p.second.wasMoveInit);
                TC_ASSERT_EXPR(!p.second.wasCopyInit);
            }
        }
    }
#endif // TEST_STD_VER > 20

    TC_SUCCESS_RESULT();
    return 0;
}
