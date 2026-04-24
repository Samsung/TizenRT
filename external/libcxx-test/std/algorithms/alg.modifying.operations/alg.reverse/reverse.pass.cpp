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
 ****************************************************************************///
//
// <algorithm>

// template<BidirectionalIterator Iter>
//   requires HasSwap<Iter::reference, Iter::reference>
//   constexpr void  // constexpr in C++20
//   reverse(Iter first, Iter last);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

namespace {
template <class Iter>
TEST_CONSTEXPR_CXX20 bool
test()
{
    int ia[] = {0};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    std::reverse(Iter(ia), Iter(ia));
    TC_ASSERT_EXPR(ia[0] == 0);
    std::reverse(Iter(ia), Iter(ia+sa));
    TC_ASSERT_EXPR(ia[0] == 0);

    int ib[] = {0, 1};
    const unsigned sb = sizeof(ib)/sizeof(ib[0]);
    std::reverse(Iter(ib), Iter(ib+sb));
    TC_ASSERT_EXPR(ib[0] == 1);
    TC_ASSERT_EXPR(ib[1] == 0);

    int ic[] = {0, 1, 2};
    const unsigned sc = sizeof(ic)/sizeof(ic[0]);
    std::reverse(Iter(ic), Iter(ic+sc));
    TC_ASSERT_EXPR(ic[0] == 2);
    TC_ASSERT_EXPR(ic[1] == 1);
    TC_ASSERT_EXPR(ic[2] == 0);

    int id[] = {0, 1, 2, 3};
    const unsigned sd = sizeof(id)/sizeof(id[0]);
    std::reverse(Iter(id), Iter(id+sd));
    TC_ASSERT_EXPR(id[0] == 3);
    TC_ASSERT_EXPR(id[1] == 2);
    TC_ASSERT_EXPR(id[2] == 1);
    TC_ASSERT_EXPR(id[3] == 0);

    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_reverse_reverse(void) {
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER >= 20
    static_assert(test<bidirectional_iterator<int*>>());
    static_assert(test<random_access_iterator<int*>>());
    static_assert(test<int*>());
#endif

    TC_SUCCESS_RESULT();


    return 0;
}
