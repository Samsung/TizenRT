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

// template<class ForwardIterator, class Size, class T>
//   constexpr ForwardIterator     // constexpr after C++17
//   search_n(ForwardIterator first, ForwardIterator last, Size count,
//            const T& value);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "user_defined_integral.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {0, 0, 1, 1, 2, 2};
    return    (std::search_n(std::begin(ia), std::end(ia), 1, 0) == ia)
           && (std::search_n(std::begin(ia), std::end(ia), 2, 1) == ia+2)
           && (std::search_n(std::begin(ia), std::end(ia), 1, 3) == std::end(ia))
           ;
    }
#endif

template <class Iter>
void
test()
{
    int ia[] = {0, 1, 2, 3, 4, 5};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 0, 0) == Iter(ia));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 1, 0) == Iter(ia+0));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 2, 0) == Iter(ia+sa));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), sa, 0) == Iter(ia+sa));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 0, 3) == Iter(ia));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 1, 3) == Iter(ia+3));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 2, 3) == Iter(ia+sa));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), sa, 3) == Iter(ia+sa));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 0, 5) == Iter(ia));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 1, 5) == Iter(ia+5));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), 2, 5) == Iter(ia+sa));
    TC_ASSERT_EXPR(std::search_n(Iter(ia), Iter(ia+sa), sa, 5) == Iter(ia+sa));

    int ib[] = {0, 0, 1, 1, 2, 2};
    const unsigned sb = sizeof(ib)/sizeof(ib[0]);
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 0, 0) == Iter(ib));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 1, 0) == Iter(ib+0));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 2, 0) == Iter(ib+0));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 3, 0) == Iter(ib+sb));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), sb, 0) == Iter(ib+sb));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 0, 1) == Iter(ib));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 1, 1) == Iter(ib+2));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 2, 1) == Iter(ib+2));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 3, 1) == Iter(ib+sb));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), sb, 1) == Iter(ib+sb));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 0, 2) == Iter(ib));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 1, 2) == Iter(ib+4));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 2, 2) == Iter(ib+4));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), 3, 2) == Iter(ib+sb));
    TC_ASSERT_EXPR(std::search_n(Iter(ib), Iter(ib+sb), sb, 2) == Iter(ib+sb));

    int ic[] = {0, 0, 0};
    const unsigned sc = sizeof(ic)/sizeof(ic[0]);
    TC_ASSERT_EXPR(std::search_n(Iter(ic), Iter(ic+sc), 0, 0) == Iter(ic));
    TC_ASSERT_EXPR(std::search_n(Iter(ic), Iter(ic+sc), 1, 0) == Iter(ic));
    TC_ASSERT_EXPR(std::search_n(Iter(ic), Iter(ic+sc), 2, 0) == Iter(ic));
    TC_ASSERT_EXPR(std::search_n(Iter(ic), Iter(ic+sc), 3, 0) == Iter(ic));
    TC_ASSERT_EXPR(std::search_n(Iter(ic), Iter(ic+sc), 4, 0) == Iter(ic+sc));

    // Check that we properly convert the size argument to an integral.
    (void)std::search_n(Iter(ic), Iter(ic+sc), UserDefinedIntegral<unsigned>(0), 0);
}

int tc_algorithms_alg_nonmodifying_alg_search_search_n(void) {
    test<forward_iterator<const int*> >();
    test<bidirectional_iterator<const int*> >();
    test<random_access_iterator<const int*> >();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
