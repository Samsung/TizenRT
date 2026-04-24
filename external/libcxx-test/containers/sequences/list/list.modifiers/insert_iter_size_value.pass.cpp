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

// iterator insert(const_iterator position, size_type n, const value_type& x);

// UNSUPPORTED: sanitizer-new-delete

#include <list>
#include <cstdlib>
#include <cassert>

#include "min_allocator.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class List>
void test() {
    int a1[] = {1, 2, 3};
    int a2[] = {1, 4, 4, 4, 4, 4, 2, 3};
    List l1(a1, a1+3);
    typename List::iterator i = l1.insert(std::next(l1.cbegin()), 5, 4);
    TC_ASSERT_EXPR(i == std::next(l1.begin()));
    TC_ASSERT_EXPR(l1 == List(a2, a2+8));
#ifndef TEST_HAS_NO_EXCEPTIONS
    globalMemCounter.throw_after = 4;
    int save_count = globalMemCounter.outstanding_new;
    try
    {
        i = l1.insert(i, 5, 5);
        TC_ASSERT_EXPR(false);
    }
    catch (...)
    {
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(save_count));
    TC_ASSERT_EXPR(l1 == List(a2, a2+8));
#endif
}

int tc_containers_sequences_list_list_modifiers_insert_iter_size_value(void) {
    test<std::list<int> >();
#if TEST_STD_VER >= 11
    test<std::list<int, min_allocator<int>>>();
#endif

  return 0;
}
