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

// deque(size_type n, const value_type& v, const allocator_type& a);

#include "asan_testing.h"
#include <deque>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class T, class Allocator>
void
test(unsigned n, const T& x, const Allocator& a)
{
    typedef std::deque<T, Allocator> C;
    typedef typename C::const_iterator const_iterator;
    C d(n, x, a);
    TC_ASSERT_EXPR(d.get_allocator() == a);
    TC_ASSERT_EXPR(d.size() == n);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == d.size());
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    for (const_iterator i = d.begin(), e = d.end(); i != e; ++i)
        TC_ASSERT_EXPR(*i == x);
}

int tc_containers_sequences_deque_deque_cons_size_value_alloc(void) {
    {
    std::allocator<int> a;
    test(0, 5, a);
    test(1, 10, a);
    test(10, 11, a);
    test(1023, -11, a);
    test(1024, 25, a);
    test(1025, 0, a);
    test(2047, 110, a);
    test(2048, -500, a);
    test(2049, 654, a);
    test(4095, 78, a);
    test(4096, 1165, a);
    test(4097, 157, a);
    }
#if TEST_STD_VER >= 11
    {
    min_allocator<int> a;
    test(0, 5, a);
    test(1, 10, a);
    test(10, 11, a);
    test(1023, -11, a);
    test(1024, 25, a);
    test(1025, 0, a);
    test(2047, 110, a);
    test(2048, -500, a);
    test(2049, 654, a);
    test(4095, 78, a);
    test(4096, 1165, a);
    test(4097, 157, a);
    }
#endif

  return 0;
}
