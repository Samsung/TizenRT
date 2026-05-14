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

// template<ForwardIterator Iter1, ForwardIterator Iter2>
//   requires HasSwap<Iter1::reference, Iter2::reference>
//   Iter2
//   swap_ranges(Iter1 first1, Iter1 last1, Iter2 first2);

#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template<class Iter1, class Iter2>
void
test()
{
    int i[3] = {1, 2, 3};
    int j[3] = {4, 5, 6};
    Iter2 r = std::swap_ranges(Iter1(i), Iter1(i+3), Iter2(j));
    TC_ASSERT_EXPR(base(r) == j+3);
    TC_ASSERT_EXPR(i[0] == 4);
    TC_ASSERT_EXPR(i[1] == 5);
    TC_ASSERT_EXPR(i[2] == 6);
    TC_ASSERT_EXPR(j[0] == 1);
    TC_ASSERT_EXPR(j[1] == 2);
    TC_ASSERT_EXPR(j[2] == 3);
}

#if TEST_STD_VER >= 11
template<class Iter1, class Iter2>
void
test1()
{
    std::unique_ptr<int> i[3];
    for (int k = 0; k < 3; ++k)
        i[k].reset(new int(k+1));
    std::unique_ptr<int> j[3];
    for (int k = 0; k < 3; ++k)
        j[k].reset(new int(k+4));
    Iter2 r = std::swap_ranges(Iter1(i), Iter1(i+3), Iter2(j));
    TC_ASSERT_EXPR(base(r) == j+3);
    TC_ASSERT_EXPR(*i[0] == 4);
    TC_ASSERT_EXPR(*i[1] == 5);
    TC_ASSERT_EXPR(*i[2] == 6);
    TC_ASSERT_EXPR(*j[0] == 1);
    TC_ASSERT_EXPR(*j[1] == 2);
    TC_ASSERT_EXPR(*j[2] == 3);
}
#endif // TEST_STD_VER >= 11

void test2()
{
    {
    int src[2][2]      = {{0, 1}, {2, 3}};
    decltype(src) dest = {{9, 8}, {7, 6}};

    std::swap(src, dest);

    TC_ASSERT_EXPR( src[0][0] == 9 );
    TC_ASSERT_EXPR( src[0][1] == 8 );
    TC_ASSERT_EXPR( src[1][0] == 7 );
    TC_ASSERT_EXPR( src[1][1] == 6 );

    TC_ASSERT_EXPR( dest[0][0] == 0 );
    TC_ASSERT_EXPR( dest[0][1] == 1 );
    TC_ASSERT_EXPR( dest[1][0] == 2 );
    TC_ASSERT_EXPR( dest[1][1] == 3 );
    }

    {
    int src[3][3]      = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
    decltype(src) dest = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};

    std::swap(src, dest);

    TC_ASSERT_EXPR( src[0][0] == 9 );
    TC_ASSERT_EXPR( src[0][1] == 8 );
    TC_ASSERT_EXPR( src[0][2] == 7 );
    TC_ASSERT_EXPR( src[1][0] == 6 );
    TC_ASSERT_EXPR( src[1][1] == 5 );
    TC_ASSERT_EXPR( src[1][2] == 4 );
    TC_ASSERT_EXPR( src[2][0] == 3 );
    TC_ASSERT_EXPR( src[2][1] == 2 );
    TC_ASSERT_EXPR( src[2][2] == 1 );

    TC_ASSERT_EXPR( dest[0][0] == 0 );
    TC_ASSERT_EXPR( dest[0][1] == 1 );
    TC_ASSERT_EXPR( dest[0][2] == 2 );
    TC_ASSERT_EXPR( dest[1][0] == 3 );
    TC_ASSERT_EXPR( dest[1][1] == 4 );
    TC_ASSERT_EXPR( dest[1][2] == 5 );
    TC_ASSERT_EXPR( dest[2][0] == 6 );
    TC_ASSERT_EXPR( dest[2][1] == 7 );
    TC_ASSERT_EXPR( dest[2][2] == 8 );
    }
}

#if TEST_STD_VER > 17
constexpr bool test_swap_constexpr()
{
    int i[3] = {1, 2, 3};
    int j[3] = {4, 5, 6};
    std::swap_ranges(i, i+3, j);
    return i[0] == 4 &&
           i[1] == 5 &&
           i[2] == 6 &&
           j[0] == 1 &&
           j[1] == 2 &&
           j[2] == 3;
}
#endif // TEST_STD_VER > 17

int tc_algorithms_alg_modifying_operations_alg_swap_swap_ranges(void) {
    test<forward_iterator<int*>, forward_iterator<int*> >();
    test<forward_iterator<int*>, bidirectional_iterator<int*> >();
    test<forward_iterator<int*>, random_access_iterator<int*> >();
    test<forward_iterator<int*>, int*>();

    test<bidirectional_iterator<int*>, forward_iterator<int*> >();
    test<bidirectional_iterator<int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<int*>, random_access_iterator<int*> >();
    test<bidirectional_iterator<int*>, int*>();

    test<random_access_iterator<int*>, forward_iterator<int*> >();
    test<random_access_iterator<int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<int*>, random_access_iterator<int*> >();
    test<random_access_iterator<int*>, int*>();

    test<int*, forward_iterator<int*> >();
    test<int*, bidirectional_iterator<int*> >();
    test<int*, random_access_iterator<int*> >();
    test<int*, int*>();

#if TEST_STD_VER >= 11
    test1<forward_iterator<std::unique_ptr<int>*>, forward_iterator<std::unique_ptr<int>*> >();
    test1<forward_iterator<std::unique_ptr<int>*>, bidirectional_iterator<std::unique_ptr<int>*> >();
    test1<forward_iterator<std::unique_ptr<int>*>, random_access_iterator<std::unique_ptr<int>*> >();
    test1<forward_iterator<std::unique_ptr<int>*>, std::unique_ptr<int>*>();

    test1<bidirectional_iterator<std::unique_ptr<int>*>, forward_iterator<std::unique_ptr<int>*> >();
    test1<bidirectional_iterator<std::unique_ptr<int>*>, bidirectional_iterator<std::unique_ptr<int>*> >();
    test1<bidirectional_iterator<std::unique_ptr<int>*>, random_access_iterator<std::unique_ptr<int>*> >();
    test1<bidirectional_iterator<std::unique_ptr<int>*>, std::unique_ptr<int>*>();

    test1<random_access_iterator<std::unique_ptr<int>*>, forward_iterator<std::unique_ptr<int>*> >();
    test1<random_access_iterator<std::unique_ptr<int>*>, bidirectional_iterator<std::unique_ptr<int>*> >();
    test1<random_access_iterator<std::unique_ptr<int>*>, random_access_iterator<std::unique_ptr<int>*> >();
    test1<random_access_iterator<std::unique_ptr<int>*>, std::unique_ptr<int>*>();

    test1<std::unique_ptr<int>*, forward_iterator<std::unique_ptr<int>*> >();
    test1<std::unique_ptr<int>*, bidirectional_iterator<std::unique_ptr<int>*> >();
    test1<std::unique_ptr<int>*, random_access_iterator<std::unique_ptr<int>*> >();
    test1<std::unique_ptr<int>*, std::unique_ptr<int>*>();
#endif // TEST_STD_VER >= 11

#if TEST_STD_VER > 17
    static_assert(test_swap_constexpr());
#endif // TEST_STD_VER > 17

    test2();

  return 0;
}
