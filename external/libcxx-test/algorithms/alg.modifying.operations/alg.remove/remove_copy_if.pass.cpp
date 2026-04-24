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

// template<InputIterator InIter, OutputIterator<auto, InIter::reference> OutIter,
//          Predicate<auto, InIter::value_type> Pred>
//   requires CopyConstructible<Pred>
//   constexpr OutIter         // constexpr after C++17
//   remove_copy_if(InIter first, InIter last, OutIter result, Pred pred);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR bool equalToTwo(int v) { return v == 2; }

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 5, 6};
    int ib[std::size(ia)] = {0};

    auto it = std::remove_copy_if(std::begin(ia), std::end(ia), std::begin(ib), equalToTwo);

    return std::distance(std::begin(ib), it) == static_cast<int>(std::size(ia) - 1)   // we removed one element
        && std::none_of(std::begin(ib), it, equalToTwo)
        && std::all_of (it, std::end(ib), [](int a) {return a == 0;})
           ;
    }
#endif

template <class InIter, class OutIter>
void
test()
{
    int ia[] = {0, 1, 2, 3, 4, 2, 3, 4, 2};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    int ib[sa];
    OutIter r = std::remove_copy_if(InIter(ia), InIter(ia+sa),
                                    OutIter(ib), equalToTwo);
    TC_ASSERT_EXPR(base(r) == ib + sa-3);
    TC_ASSERT_EXPR(ib[0] == 0);
    TC_ASSERT_EXPR(ib[1] == 1);
    TC_ASSERT_EXPR(ib[2] == 3);
    TC_ASSERT_EXPR(ib[3] == 4);
    TC_ASSERT_EXPR(ib[4] == 3);
    TC_ASSERT_EXPR(ib[5] == 4);
}

int tc_algorithms_alg_modifying_operations_alg_remove_remove_copy_if(void) {
    test<cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, forward_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, int*>();

    test<forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<forward_iterator<const int*>, forward_iterator<int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test<forward_iterator<const int*>, random_access_iterator<int*> >();
    test<forward_iterator<const int*>, int*>();

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
