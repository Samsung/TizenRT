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

// template<BidirectionalIterator InIter, OutputIterator<auto, InIter::reference> OutIter>
//   constexpr OutIter          // constexpr after C++17
//   reverse_copy(InIter first, InIter last, OutIter result);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 5, 6};
    int ib[std::size(ia)] = {0};

    auto it = std::reverse_copy(std::begin(ia), std::end(ia), std::begin(ib));

    return std::distance(std::begin(ib), it) == static_cast<int>(std::size(ia))
        && std::equal   (std::begin(ia), std::end(ia), std::rbegin(ib))
           ;
    }
#endif

template <class InIter, class OutIter>
void
test()
{
    const int ia[] = {0};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    int ja[sa] = {-1};
    OutIter r = std::reverse_copy(InIter(ia), InIter(ia), OutIter(ja));
    TC_ASSERT_EXPR(base(r) == ja);
    TC_ASSERT_EXPR(ja[0] == -1);
    r = std::reverse_copy(InIter(ia), InIter(ia+sa), OutIter(ja));
    TC_ASSERT_EXPR(ja[0] == 0);

    const int ib[] = {0, 1};
    const unsigned sb = sizeof(ib)/sizeof(ib[0]);
    int jb[sb] = {-1};
    r = std::reverse_copy(InIter(ib), InIter(ib+sb), OutIter(jb));
    TC_ASSERT_EXPR(base(r) == jb+sb);
    TC_ASSERT_EXPR(jb[0] == 1);
    TC_ASSERT_EXPR(jb[1] == 0);

    const int ic[] = {0, 1, 2};
    const unsigned sc = sizeof(ic)/sizeof(ic[0]);
    int jc[sc] = {-1};
    r = std::reverse_copy(InIter(ic), InIter(ic+sc), OutIter(jc));
    TC_ASSERT_EXPR(base(r) == jc+sc);
    TC_ASSERT_EXPR(jc[0] == 2);
    TC_ASSERT_EXPR(jc[1] == 1);
    TC_ASSERT_EXPR(jc[2] == 0);

    int id[] = {0, 1, 2, 3};
    const unsigned sd = sizeof(id)/sizeof(id[0]);
    int jd[sd] = {-1};
    r = std::reverse_copy(InIter(id), InIter(id+sd), OutIter(jd));
    TC_ASSERT_EXPR(base(r) == jd+sd);
    TC_ASSERT_EXPR(jd[0] == 3);
    TC_ASSERT_EXPR(jd[1] == 2);
    TC_ASSERT_EXPR(jd[2] == 1);
    TC_ASSERT_EXPR(jd[3] == 0);
}

int tc_algorithms_alg_modifying_operations_alg_reverse_reverse_copy(void) {
    test<bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test<bidirectional_iterator<const int*>, int*>();

    test<random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<random_access_iterator<const int*>, forward_iterator<int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<int*> >();
    test<random_access_iterator<const int*>, int*>();

    test<const int*, cpp17_output_iterator<int*> >();
    test<const int*, forward_iterator<int*> >();
    test<const int*, bidirectional_iterator<int*> >();
    test<const int*, random_access_iterator<int*> >();
    test<const int*, int*>();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
