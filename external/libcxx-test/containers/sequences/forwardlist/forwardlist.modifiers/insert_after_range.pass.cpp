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
// <forward_list>

// template <class InputIterator>
//     iterator insert_after(const_iterator p,
//                           InputIterator first, InputIterator last);

#include <forward_list>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_forwardlist_forwardlist_modifiers_insert_after_range(void) {
    {
        typedef int T;
        typedef std::forward_list<T> C;
        typedef C::iterator I;
        typedef cpp17_input_iterator<const T*> J;
        C c;
        const T t[] = {0, 1, 2, 3, 4};
        I i = c.insert_after(c.cbefore_begin(), J(t), J(t));
        TC_ASSERT_EXPR(i == c.before_begin());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);

        i = c.insert_after(c.cbefore_begin(), J(t), J(t+3));
        TC_ASSERT_EXPR(i == std::next(c.before_begin(), 3));
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 3);
        TC_ASSERT_EXPR(*std::next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*std::next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(c.begin(), 2) == 2);

        i = c.insert_after(c.begin(), J(t+3), J(t+5));
        TC_ASSERT_EXPR(i == std::next(c.begin(), 2));
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 5);
        TC_ASSERT_EXPR(*std::next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*std::next(c.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(c.begin(), 2) == 4);
        TC_ASSERT_EXPR(*std::next(c.begin(), 3) == 1);
        TC_ASSERT_EXPR(*std::next(c.begin(), 4) == 2);
    }
#if TEST_STD_VER >= 11
    {
        typedef int T;
        typedef std::forward_list<T, min_allocator<T>> C;
        typedef C::iterator I;
        typedef cpp17_input_iterator<const T*> J;
        C c;
        const T t[] = {0, 1, 2, 3, 4};
        I i = c.insert_after(c.cbefore_begin(), J(t), J(t));
        TC_ASSERT_EXPR(i == c.before_begin());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);

        i = c.insert_after(c.cbefore_begin(), J(t), J(t+3));
        TC_ASSERT_EXPR(i == std::next(c.before_begin(), 3));
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 3);
        TC_ASSERT_EXPR(*std::next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*std::next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(c.begin(), 2) == 2);

        i = c.insert_after(c.begin(), J(t+3), J(t+5));
        TC_ASSERT_EXPR(i == std::next(c.begin(), 2));
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 5);
        TC_ASSERT_EXPR(*std::next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*std::next(c.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(c.begin(), 2) == 4);
        TC_ASSERT_EXPR(*std::next(c.begin(), 3) == 1);
        TC_ASSERT_EXPR(*std::next(c.begin(), 4) == 2);
    }
#endif

  return 0;
}
