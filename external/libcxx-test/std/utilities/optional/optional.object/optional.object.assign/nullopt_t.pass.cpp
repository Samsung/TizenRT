//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// optional<T>& operator=(nullopt_t) noexcept;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

using std::optional;
using std::nullopt_t;
using std::nullopt;

TEST_CONSTEXPR_CXX20 bool test()
{
    enum class State { inactive, constructed, destroyed };
    State state = State::inactive;

    struct StateTracker {
      TEST_CONSTEXPR_CXX20 StateTracker(State& s)
      : state_(&s)
      {
        s = State::constructed;
      }
      TEST_CONSTEXPR_CXX20 ~StateTracker() { *state_ = State::destroyed; }

      State* state_;
    };
    {
        optional<int> opt;
        static_assert(noexcept(opt = nullopt) == true, "");
        opt = nullopt;
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    {
        optional<int> opt(3);
        opt = nullopt;
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    {
        optional<StateTracker> opt;
        opt = nullopt;
        TC_ASSERT_EXPR(state == State::inactive);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    {
        optional<StateTracker> opt(state);
        TC_ASSERT_EXPR(state == State::constructed);
        opt = nullopt;
        TC_ASSERT_EXPR(state == State::destroyed);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    return true;
}


int tc_utilities_optional_optional_object_optional_object_assign_nullopt_t(void) {
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    test();
    using TT = TestTypes::TestType;
    TT::reset();
    {
        optional<TT> opt;
        static_assert(noexcept(opt = nullopt) == true, "");
        TC_ASSERT_EXPR(TT::destroyed == 0);
        opt = nullopt;
        TC_ASSERT_EXPR(TT::constructed == 0);
        TC_ASSERT_EXPR(TT::alive == 0);
        TC_ASSERT_EXPR(TT::destroyed == 0);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    TC_ASSERT_EXPR(TT::alive == 0);
    TC_ASSERT_EXPR(TT::destroyed == 0);
    TT::reset();
    {
        optional<TT> opt(42);
        TC_ASSERT_EXPR(TT::destroyed == 0);
        TT::reset_constructors();
        opt = nullopt;
        TC_ASSERT_EXPR(TT::constructed == 0);
        TC_ASSERT_EXPR(TT::alive == 0);
        TC_ASSERT_EXPR(TT::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    TC_ASSERT_EXPR(TT::alive == 0);
    TC_ASSERT_EXPR(TT::destroyed == 1);
    TT::reset();

    TC_SUCCESS_RESULT();
    return 0;
}
