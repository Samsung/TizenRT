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
//   constexpr OutIter   // constexpr after C++17
//   copy_if(InIter first, InIter last, OutIter result, Pred pred);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct Pred
{
    TEST_CONSTEXPR_CXX14 bool operator()(int i) {return i % 3 == 0;}
};

template <class InIter, class OutIter>
TEST_CONSTEXPR_CXX20 void
test_copy_if()
{
    const unsigned N = 1000;
    int ia[N] = {};
    for (unsigned i = 0; i < N; ++i)
        ia[i] = i;
    int ib[N] = {0};

    OutIter r = std::copy_if(InIter(ia), InIter(ia+N), OutIter(ib), Pred());
    TC_ASSERT_EXPR(base(r) == ib+N/3+1);
    for (unsigned i = 0; i < N/3+1; ++i)
        TC_ASSERT_EXPR(ib[i] % 3 == 0);
}

TEST_CONSTEXPR_CXX20 bool
test()
{
    test_copy_if<cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_if<cpp17_input_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_if<cpp17_input_iterator<const int*>, forward_iterator<int*> >();
    test_copy_if<cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_if<cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_if<cpp17_input_iterator<const int*>, int*>();

    test_copy_if<forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_if<forward_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_if<forward_iterator<const int*>, forward_iterator<int*> >();
    test_copy_if<forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_if<forward_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_if<forward_iterator<const int*>, int*>();

    test_copy_if<bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_if<bidirectional_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_if<bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test_copy_if<bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_if<bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_if<bidirectional_iterator<const int*>, int*>();

    test_copy_if<random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_if<random_access_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_if<random_access_iterator<const int*>, forward_iterator<int*> >();
    test_copy_if<random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_if<random_access_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_if<random_access_iterator<const int*>, int*>();

    test_copy_if<const int*, cpp17_output_iterator<int*> >();
    test_copy_if<const int*, cpp17_input_iterator<int*> >();
    test_copy_if<const int*, forward_iterator<int*> >();
    test_copy_if<const int*, bidirectional_iterator<int*> >();
    test_copy_if<const int*, random_access_iterator<int*> >();
    test_copy_if<const int*, int*>();

  return true;
}

int tc_algorithms_alg_modifying_operations_alg_copy_copy_if(void) {
    test();

#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;
}
