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

// pair& operator=(pair&& p);

#include <utility>
#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"


struct NonAssignable {
  NonAssignable& operator=(NonAssignable const&) = delete;
  NonAssignable& operator=(NonAssignable&&) = delete;
};
struct CopyAssignable {
  CopyAssignable() = default;
  CopyAssignable& operator=(CopyAssignable const&) = default;
  CopyAssignable& operator=(CopyAssignable&&) = delete;
};
struct MoveAssignable {
  MoveAssignable() = default;
  MoveAssignable& operator=(MoveAssignable const&) = delete;
  MoveAssignable& operator=(MoveAssignable&&) = default;
};

struct CountAssign {
  static int copied;
  static int moved;
  static void reset() { copied = moved = 0; }
  CountAssign() = default;
  CountAssign& operator=(CountAssign const&) { ++copied; return *this; }
  CountAssign& operator=(CountAssign&&) { ++moved; return *this; }
};
int CountAssign::copied = 0;
int CountAssign::moved = 0;

int tc_libcxx_utilities_pairs_pair_assign_rv_pair(void)
{
    {
        typedef std::pair<std::unique_ptr<int>, int> P;
        P p1(std::unique_ptr<int>(new int(3)), 4);
        P p2;
        p2 = std::move(p1);
        TC_ASSERT_EXPR(*p2.first == 3);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    {
        using P = std::pair<int&, int&&>;
        int x = 42;
        int y = 101;
        int x2 = -1;
        int y2 = 300;
        P p1(x, std::move(y));
        P p2(x2, std::move(y2));
        p1 = std::move(p2);
        TC_ASSERT_EXPR(p1.first == x2);
        TC_ASSERT_EXPR(p1.second == y2);
    }
    {
        using P = std::pair<int, NonAssignable>;
        static_assert(!std::is_move_assignable<P>::value, "");
    }
    {
        // The move decays to the copy constructor
        CountAssign::reset();
        using P = std::pair<CountAssign, CopyAssignable>;
        static_assert(std::is_move_assignable<P>::value, "");
        P p;
        P p2;
        p = std::move(p2);
        TC_ASSERT_EXPR(CountAssign::moved == 0);
        TC_ASSERT_EXPR(CountAssign::copied == 1);
    }
    {
        CountAssign::reset();
        using P = std::pair<CountAssign, MoveAssignable>;
        static_assert(std::is_move_assignable<P>::value, "");
        P p;
        P p2;
        p = std::move(p2);
        TC_ASSERT_EXPR(CountAssign::moved == 1);
        TC_ASSERT_EXPR(CountAssign::copied == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
