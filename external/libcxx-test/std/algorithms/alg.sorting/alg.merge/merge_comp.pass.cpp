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

// template<InputIterator InIter1, InputIterator InIter2, typename OutIter,
//          CopyConstructible Compare>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && Predicate<Compare, InIter1::value_type, InIter2::value_type>
//         && Predicate<Compare, InIter2::value_type, InIter1::value_type>
//   constexpr OutIter       // constexpr after C++17
//   merge(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2,
//         OutIter result, Compare comp);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "../sortable_helpers.h"
#include "libcxx_tc_common.h"

namespace {
template<class T, class Iter1, class Iter2, class OutIter>
TEST_CONSTEXPR_CXX20 void test4()
{
    const T a[] = {11, 33, 31, 41};
    const T b[] = {22, 32, 43, 42, 52};
    {
        T result[20] = {};
        T expected[] = {11, 22, 33, 31, 32, 41, 43, 42, 52};
        OutIter end = std::merge(Iter1(a), Iter1(a+4), Iter2(b), Iter2(b+5), OutIter(result), typename T::Comparator());
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+9, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
    {
        T result[20] = {};
        T expected[] = {11, 22, 32, 33, 31, 43, 42, 41, 52};
        OutIter end = std::merge(Iter1(b), Iter1(b+5), Iter2(a), Iter2(a+4), OutIter(result), typename T::Comparator());
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+9, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
}

template<class T, class Iter1, class Iter2>
TEST_CONSTEXPR_CXX20 void test3()
{
    test4<T, Iter1, Iter2, cpp17_output_iterator<T*> >();
    test4<T, Iter1, Iter2, forward_iterator<T*> >();
    test4<T, Iter1, Iter2, bidirectional_iterator<T*> >();
    test4<T, Iter1, Iter2, random_access_iterator<T*> >();
    test4<T, Iter1, Iter2, T*>();
}

template<class T, class Iter1>
TEST_CONSTEXPR_CXX20 void test2()
{
    test3<T, Iter1, cpp17_input_iterator<const T*> >();
    test3<T, Iter1, forward_iterator<const T*> >();
    test3<T, Iter1, bidirectional_iterator<const T*> >();
    test3<T, Iter1, random_access_iterator<const T*> >();
    test3<T, Iter1, const T*>();
}

template<class T>
TEST_CONSTEXPR_CXX20 void test1()
{
    test2<T, cpp17_input_iterator<const T*> >();
    test2<T, forward_iterator<const T*> >();
    test2<T, bidirectional_iterator<const T*> >();
    test2<T, random_access_iterator<const T*> >();
    test2<T, const T*>();
}

TEST_CONSTEXPR_CXX20 bool test()
{
    test1<TrivialSortableWithComp>();
    test1<NonTrivialSortableWithComp>();
    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_merge_merge_comp(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

    TC_SUCCESS_RESULT();


    return 0;
}
