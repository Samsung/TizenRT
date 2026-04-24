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

// void swap(tuple& rhs);

// UNSUPPORTED: c++03

#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20
bool test()
{
    {
        typedef std::tuple<> T;
        T t0;
        T t1;
        t0.swap(t1);
    }
    {
        typedef std::tuple<MoveOnly> T;
        T t0(MoveOnly(0));
        T t1(MoveOnly(1));
        t0.swap(t1);
        TC_ASSERT_EXPR(std::get<0>(t0) == 1);
        TC_ASSERT_EXPR(std::get<0>(t1) == 0);
    }
    {
        typedef std::tuple<MoveOnly, MoveOnly> T;
        T t0(MoveOnly(0), MoveOnly(1));
        T t1(MoveOnly(2), MoveOnly(3));
        t0.swap(t1);
        TC_ASSERT_EXPR(std::get<0>(t0) == 2);
        TC_ASSERT_EXPR(std::get<1>(t0) == 3);
        TC_ASSERT_EXPR(std::get<0>(t1) == 0);
        TC_ASSERT_EXPR(std::get<1>(t1) == 1);
    }
    {
        typedef std::tuple<MoveOnly, MoveOnly, MoveOnly> T;
        T t0(MoveOnly(0), MoveOnly(1), MoveOnly(2));
        T t1(MoveOnly(3), MoveOnly(4), MoveOnly(5));
        t0.swap(t1);
        TC_ASSERT_EXPR(std::get<0>(t0) == 3);
        TC_ASSERT_EXPR(std::get<1>(t0) == 4);
        TC_ASSERT_EXPR(std::get<2>(t0) == 5);
        TC_ASSERT_EXPR(std::get<0>(t1) == 0);
        TC_ASSERT_EXPR(std::get<1>(t1) == 1);
        TC_ASSERT_EXPR(std::get<2>(t1) == 2);
    }
    return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_swap_member_swap(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return 0;
}
