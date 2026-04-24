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
// <list>

// template <InputIterator Iter>
//   iterator insert(const_iterator position, Iter first, Iter last);

#include <list>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "count_new.h"
#include "libcxx_tc_common.h"

template <class List>
void test() {
    int a1[] = {1, 2, 3};
    List l1;
    typename List::iterator i = l1.insert(l1.begin(), a1, a1+3);
    TC_ASSERT_EXPR(i == l1.begin());
    TC_ASSERT_EXPR(l1.size() == 3);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 3);
    i = l1.begin();
    TC_ASSERT_EXPR(*i == 1);
    ++i;
    TC_ASSERT_EXPR(*i == 2);
    ++i;
    TC_ASSERT_EXPR(*i == 3);
    int a2[] = {4, 5, 6};
    i = l1.insert(i, a2, a2+3);
    TC_ASSERT_EXPR(*i == 4);
    TC_ASSERT_EXPR(l1.size() == 6);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 6);
    i = l1.begin();
    TC_ASSERT_EXPR(*i == 1);
    ++i;
    TC_ASSERT_EXPR(*i == 2);
    ++i;
    TC_ASSERT_EXPR(*i == 4);
    ++i;
    TC_ASSERT_EXPR(*i == 5);
    ++i;
    TC_ASSERT_EXPR(*i == 6);
    ++i;
    TC_ASSERT_EXPR(*i == 3);

#if !defined(TEST_HAS_NO_EXCEPTIONS) && !defined(DISABLE_NEW_COUNT)
    globalMemCounter.throw_after = 2;
    int save_count = globalMemCounter.outstanding_new;
    try
    {
        i = l1.insert(i, a2, a2+3);
        TC_ASSERT_EXPR(false);
    }
    catch (...)
    {
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(save_count));
    TC_ASSERT_EXPR(l1.size() == 6);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 6);
    i = l1.begin();
    TC_ASSERT_EXPR(*i == 1);
    ++i;
    TC_ASSERT_EXPR(*i == 2);
    ++i;
    TC_ASSERT_EXPR(*i == 4);
    ++i;
    TC_ASSERT_EXPR(*i == 5);
    ++i;
    TC_ASSERT_EXPR(*i == 6);
    ++i;
    TC_ASSERT_EXPR(*i == 3);
#endif
}

int tc_containers_sequences_list_list_modifiers_insert_iter_iter_iter(void) {
    test<std::list<int> >();
#if TEST_STD_VER >= 11
    test<std::list<int, min_allocator<int>>>();
#endif

  return 0;
}
