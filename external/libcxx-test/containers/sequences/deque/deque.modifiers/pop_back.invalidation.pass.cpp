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
// <deque>

// void pop_back()

//  Erasing items from the beginning or the end of a deque shall not invalidate iterators
//  to items that were not erased.

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename C>
void test(C c)
{
    typename C::iterator it1 = c.begin();
    typename C::iterator it2 = c.end() - 2;

    c.pop_back();

    typename C::iterator it3 = c.begin();
    typename C::iterator it4 = c.end() - 1;
    TC_ASSERT_EXPR(  it1 ==   it3);
    TC_ASSERT_EXPR( *it1 ==  *it3);
    TC_ASSERT_EXPR(&*it1 == &*it3);
    TC_ASSERT_EXPR(  it2 ==   it4);
    TC_ASSERT_EXPR( *it2 ==  *it4);
    TC_ASSERT_EXPR(&*it2 == &*it4);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
}

int tc_containers_sequences_deque_deque_modifiers_pop_back_invalidation(void) {
    std::deque<int> queue;
    for (int i = 0; i < 4098; ++i)
        queue.push_back(i);

    while (queue.size() > 1)
    {
        test(queue);
        queue.pop_back();
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(queue));
    }

  return 0;
}
