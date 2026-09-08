//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
