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
// test rel_ops

#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    int data_;

    explicit A(int data = -1) : data_(data) {}
};

inline
bool
operator == (const A& x, const A& y)
{
    return x.data_ == y.data_;
}

inline
bool
operator < (const A& x, const A& y)
{
    return x.data_ < y.data_;
}

int tc_utilities_utility_operators_rel_ops(void) {
    using namespace std::rel_ops;
    A a1(1);
    A a2(2);
    TC_ASSERT_EXPR(a1 == a1);
    TC_ASSERT_EXPR(a1 != a2);
    TC_ASSERT_EXPR(a1 < a2);
    TC_ASSERT_EXPR(a2 > a1);
    TC_ASSERT_EXPR(a1 <= a1);
    TC_ASSERT_EXPR(a1 <= a2);
    TC_ASSERT_EXPR(a2 >= a2);
    TC_ASSERT_EXPR(a2 >= a1);

  return 0;
}
