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
// <functional>

// UNSUPPORTED: c++03, c++11, c++14

// default searcher
// template<class _ForwardIterator, class _BinaryPredicate = equal_to<>>
// class default_searcher {
// public:
//     default_searcher(_ForwardIterator __f, _ForwardIterator __l,
//                             _BinaryPredicate __p = _BinaryPredicate())
//         : __first_(__f), __last_(__l), __pred_(__p) {}
//
//     template <typename _ForwardIterator2>
//     pair<_ForwardIterator2, _ForwardIterator2>
//     operator () (_ForwardIterator2 __f, _ForwardIterator2 __l) const {
//         return std::search(__f, __l, __first_, __last_, __pred_);
//         }
//
// private:
//     _ForwardIterator __first_;
//     _ForwardIterator __last_;
//     _BinaryPredicate __pred_;
//     };


#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct count_equal
{
    int *count;

    template <class T>
    TEST_CONSTEXPR_CXX14 bool operator()(const T& x, const T& y) const
        {++*count; return x == y;}
};

template <typename Iter1, typename Iter2>
TEST_CONSTEXPR_CXX20
void do_search(Iter1 b1, Iter1 e1, Iter2 b2, Iter2 e2, Iter1 result) {
    int count = 0;
    std::default_searcher<Iter2, count_equal> s{b2, e2, count_equal{&count}};
    TC_ASSERT_EXPR(result == std::search(b1, e1, s));
    auto d1 = std::distance(b1, e1);
    auto d2 = std::distance(b2, e2);
    TC_ASSERT_EXPR((count >= 1) || (d2 == 0) || (d1 < d2));
    TC_ASSERT_EXPR((d1 < d2) || count <= d1 * (d1 - d2 + 1));
}

template <class Iter1, class Iter2>
TEST_CONSTEXPR_CXX20
bool test()
{
    int ia[] = {0, 1, 2, 3, 4, 5};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia),      Iter2(ia),    Iter1(ia));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia),      Iter2(ia+1),  Iter1(ia));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia+1),    Iter2(ia+2),  Iter1(ia+1));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia+2),    Iter2(ia+2),  Iter1(ia));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia+2),    Iter2(ia+3),  Iter1(ia+2));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia+2),    Iter2(ia+3),  Iter1(ia+2));
    do_search(Iter1(ia), Iter1(ia),      Iter2(ia+2),    Iter2(ia+3),  Iter1(ia));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia+sa-1), Iter2(ia+sa), Iter1(ia+sa-1));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia+sa-3), Iter2(ia+sa), Iter1(ia+sa-3));
    do_search(Iter1(ia), Iter1(ia+sa),   Iter2(ia),      Iter2(ia+sa), Iter1(ia));
    do_search(Iter1(ia), Iter1(ia+sa-1), Iter2(ia),      Iter2(ia+sa), Iter1(ia+sa-1));
    do_search(Iter1(ia), Iter1(ia+1),    Iter2(ia),      Iter2(ia+sa), Iter1(ia+1));
    int ib[] = {0, 1, 2, 0, 1, 2, 3, 0, 1, 2, 3, 4};
    const unsigned sb = sizeof(ib)/sizeof(ib[0]);
    int ic[] = {1};
    do_search(Iter1(ib), Iter1(ib+sb), Iter2(ic), Iter2(ic+1), Iter1(ib+1));
    int id[] = {1, 2};
    do_search(Iter1(ib), Iter1(ib+sb), Iter2(id), Iter2(id+2), Iter1(ib+1));
    int ie[] = {1, 2, 3};
    do_search(Iter1(ib), Iter1(ib+sb), Iter2(ie), Iter2(ie+3), Iter1(ib+4));
    int ig[] = {1, 2, 3, 4};
    do_search(Iter1(ib), Iter1(ib+sb), Iter2(ig), Iter2(ig+4), Iter1(ib+8));
    int ih[] = {0, 1, 1, 1, 1, 2, 3, 0, 1, 2, 3, 4};
    const unsigned sh = sizeof(ih)/sizeof(ih[0]);
    int ii[] = {1, 1, 2};
    do_search(Iter1(ih), Iter1(ih+sh), Iter2(ii), Iter2(ii+3), Iter1(ih+3));

    return true;
}

int tc_utilities_function_objects_func_search_func_search_default_default_pred(void) {
    test<forward_iterator<const int*>, forward_iterator<const int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<const int*> >();
    test<forward_iterator<const int*>, random_access_iterator<const int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<const int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<const int*> >();
    test<random_access_iterator<const int*>, forward_iterator<const int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<const int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<const int*> >();

#if TEST_STD_VER >= 20
    static_assert(test<forward_iterator<const int*>, forward_iterator<const int*>>());
    static_assert(test<forward_iterator<const int*>, bidirectional_iterator<const int*>>());
    static_assert(test<forward_iterator<const int*>, random_access_iterator<const int*>>());
    static_assert(test<bidirectional_iterator<const int*>, forward_iterator<const int*>>());
    static_assert(test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*>>());
    static_assert(test<bidirectional_iterator<const int*>, random_access_iterator<const int*>>());
    static_assert(test<random_access_iterator<const int*>, forward_iterator<const int*>>());
    static_assert(test<random_access_iterator<const int*>, bidirectional_iterator<const int*>>());
    static_assert(test<random_access_iterator<const int*>, random_access_iterator<const int*>>());
#endif

    return 0;
}
