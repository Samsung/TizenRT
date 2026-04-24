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
// <algorithm>

// template<BidirectionalIterator Iter, Predicate<auto, Iter::value_type> Pred>
//   requires ShuffleIterator<Iter>
//         && CopyConstructible<Pred>
//   constexpr Iter  // constexpr in C++20
//   partition(Iter first, Iter last, Pred pred);

#include <algorithm>
#include <cassert>


#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct is_odd
{
    TEST_CONSTEXPR bool operator()(const int& i) const {return i & 1;}
};

template <class Iter>
TEST_CONSTEXPR_CXX20 bool
test()
{
    // check mixed
    int ia[] = {1, 2, 3, 4, 5, 6, 7, 8 ,9};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    Iter r = std::partition(Iter(ia), Iter(ia + sa), is_odd());
    TC_ASSERT_EXPR(base(r) == ia + 5);
    for (int* i = ia; i < base(r); ++i)
        TC_ASSERT_EXPR(is_odd()(*i));
    for (int* i = base(r); i < ia+sa; ++i)
        TC_ASSERT_EXPR(!is_odd()(*i));
    // check empty
    r = std::partition(Iter(ia), Iter(ia), is_odd());
    TC_ASSERT_EXPR(base(r) == ia);
    // check all false
    for (unsigned i = 0; i < sa; ++i)
        ia[i] = 2*i;
    r = std::partition(Iter(ia), Iter(ia+sa), is_odd());
    TC_ASSERT_EXPR(base(r) == ia);
    // check all true
    for (unsigned i = 0; i < sa; ++i)
        ia[i] = 2*i+1;
    r = std::partition(Iter(ia), Iter(ia+sa), is_odd());
    TC_ASSERT_EXPR(base(r) == ia+sa);
    // check all true but last
    for (unsigned i = 0; i < sa; ++i)
        ia[i] = 2*i+1;
    ia[sa-1] = 10;
    r = std::partition(Iter(ia), Iter(ia+sa), is_odd());
    TC_ASSERT_EXPR(base(r) == ia+sa-1);
    for (int* i = ia; i < base(r); ++i)
        TC_ASSERT_EXPR(is_odd()(*i));
    for (int* i = base(r); i < ia+sa; ++i)
        TC_ASSERT_EXPR(!is_odd()(*i));
    // check all true but first
    for (unsigned i = 0; i < sa; ++i)
        ia[i] = 2*i+1;
    ia[0] = 10;
    r = std::partition(Iter(ia), Iter(ia+sa), is_odd());
    TC_ASSERT_EXPR(base(r) == ia+sa-1);
    for (int* i = ia; i < base(r); ++i)
        TC_ASSERT_EXPR(is_odd()(*i));
    for (int* i = base(r); i < ia+sa; ++i)
        TC_ASSERT_EXPR(!is_odd()(*i));
    // check all false but last
    for (unsigned i = 0; i < sa; ++i)
        ia[i] = 2*i;
    ia[sa-1] = 11;
    r = std::partition(Iter(ia), Iter(ia+sa), is_odd());
    TC_ASSERT_EXPR(base(r) == ia+1);
    for (int* i = ia; i < base(r); ++i)
        TC_ASSERT_EXPR(is_odd()(*i));
    for (int* i = base(r); i < ia+sa; ++i)
        TC_ASSERT_EXPR(!is_odd()(*i));
    // check all false but first
    for (unsigned i = 0; i < sa; ++i)
        ia[i] = 2*i;
    ia[0] = 11;
    r = std::partition(Iter(ia), Iter(ia+sa), is_odd());
    TC_ASSERT_EXPR(base(r) == ia+1);
    for (int* i = ia; i < base(r); ++i)
        TC_ASSERT_EXPR(is_odd()(*i));
    for (int* i = base(r); i < ia+sa; ++i)
        TC_ASSERT_EXPR(!is_odd()(*i));

    return true;
}

int tc_algorithms_alg_modifying_operations_alg_partitions_partition(void) {
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER >= 20
    static_assert(test<bidirectional_iterator<int*>>());
    static_assert(test<random_access_iterator<int*>>());
    static_assert(test<int*>());
#endif

    return 0;
}
