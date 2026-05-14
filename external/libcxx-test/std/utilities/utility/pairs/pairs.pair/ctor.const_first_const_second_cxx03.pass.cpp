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

  return 0;
}
