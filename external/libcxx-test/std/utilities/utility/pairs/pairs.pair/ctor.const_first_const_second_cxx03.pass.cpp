//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <utility>

// template <class T1, class T2> struct pair

// pair(const T1& x, const T2& y);

#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class A
{
    int data_;
public:
    A(int data) : data_(data) {}

    bool operator==(const A& a) const {return data_ == a.data_;}
};

int tc_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03(void) {
    {
        typedef std::pair<float, short*> P;
        P p(3.5f, 0);
        TC_ASSERT_EXPR(p.first == 3.5f);
        TC_ASSERT_EXPR(p.second == nullptr);
    }
    {
        typedef std::pair<A, int> P;
        P p(1, 2);
        TC_ASSERT_EXPR(p.first == A(1));
        TC_ASSERT_EXPR(p.second == 2);
    }

    TC_SUCCESS_RESULT();
    return 0;
}
