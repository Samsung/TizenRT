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

// template<InputIterator Iter1, InputIterator Iter2>
//   requires HasLess<Iter1::value_type, Iter2::value_type>
//         && HasLess<Iter2::value_type, Iter1::value_type>
//   constexpr bool             // constexpr after C++17
//   includes(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template <class Iter1, class Iter2>
TEST_CONSTEXPR_CXX20
void test()
{
    int ia[] = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    int ib[] = {2, 4};
    const unsigned sb = sizeof(ib)/sizeof(ib[0]);
    int ic[] = {1, 2};
    const unsigned sc = sizeof(ic)/sizeof(ic[0]); ((void)sc);
    int id[] = {3, 3, 3, 3};
    const unsigned sd = sizeof(id)/sizeof(id[0]); ((void)sd);

    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia), Iter2(ib), Iter2(ib)));
    TC_ASSERT_EXPR(!std::includes(Iter1(ia), Iter1(ia), Iter2(ib), Iter2(ib+1)));
    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia+1), Iter2(ib), Iter2(ib)));
    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia+sa), Iter2(ia), Iter2(ia+sa)));

    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia+sa), Iter2(ib), Iter2(ib+sb)));
    TC_ASSERT_EXPR(!std::includes(Iter1(ib), Iter1(ib+sb), Iter2(ia), Iter2(ia+sa)));

    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia+2), Iter2(ic), Iter2(ic+2)));
    TC_ASSERT_EXPR(!std::includes(Iter1(ia), Iter1(ia+2), Iter2(ib), Iter2(ib+2)));

    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia+sa), Iter2(id), Iter2(id+1)));
    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia+sa), Iter2(id), Iter2(id+2)));
    TC_ASSERT_EXPR(std::includes(Iter1(ia), Iter1(ia+sa), Iter2(id), Iter2(id+3)));
    TC_ASSERT_EXPR(!std::includes(Iter1(ia), Iter1(ia+sa), Iter2(id), Iter2(id+4)));
}

TEST_CONSTEXPR_CXX20
bool do_tests()
{
    test<cpp17_input_iterator<const int*>, cpp17_input_iterator<const int*> >();
    test<cpp17_input_iterator<const int*>, forward_iterator<const int*> >();
    test<cpp17_input_iterator<const int*>, bidirectional_iterator<const int*> >();
    test<cpp17_input_iterator<const int*>, random_access_iterator<const int*> >();
    test<cpp17_input_iterator<const int*>, const int*>();

    test<forward_iterator<const int*>, cpp17_input_iterator<const int*> >();
    test<forward_iterator<const int*>, forward_iterator<const int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<const int*> >();
    test<forward_iterator<const int*>, random_access_iterator<const int*> >();
    test<forward_iterator<const int*>, const int*>();

    test<bidirectional_iterator<const int*>, cpp17_input_iterator<const int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<const int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<const int*> >();
    test<bidirectional_iterator<const int*>, const int*>();

    test<random_access_iterator<const int*>, cpp17_input_iterator<const int*> >();
    test<random_access_iterator<const int*>, forward_iterator<const int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<const int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<const int*> >();
    test<random_access_iterator<const int*>, const int*>();

    test<const int*, cpp17_input_iterator<const int*> >();
    test<const int*, forward_iterator<const int*> >();
    test<const int*, bidirectional_iterator<const int*> >();
    test<const int*, random_access_iterator<const int*> >();
    test<const int*, const int*>();

    return true;
}

int tc_algorithms_alg_sorting_alg_set_operations_includes_includes(void) {
    do_tests();
#if TEST_STD_VER > 17
    static_assert(do_tests());
#endif
    return 0;
}
