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

// deque(size_type n, const value_type& v);

#include "asan_testing.h"
#include <deque>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class T, class Allocator>
void
test(unsigned n, const T& x)
{
    typedef std::deque<T, Allocator> C;
    typedef typename C::const_iterator const_iterator;
    C d(n, x);
    TC_ASSERT_EXPR(d.size() == n);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == d.size());
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    for (const_iterator i = d.begin(), e = d.end(); i != e; ++i)
        TC_ASSERT_EXPR(*i == x);
}

int tc_containers_sequences_deque_deque_cons_size_value(void) {
    test<int, std::allocator<int> >(0, 5);
    test<int, std::allocator<int> >(1, 10);
    test<int, std::allocator<int> >(10, 11);
    test<int, std::allocator<int> >(1023, -11);
    test<int, std::allocator<int> >(1024, 25);
    test<int, std::allocator<int> >(1025, 0);
    test<int, std::allocator<int> >(2047, 110);
    test<int, std::allocator<int> >(2048, -500);
    test<int, std::allocator<int> >(2049, 654);
    test<int, std::allocator<int> >(4095, 78);
    test<int, std::allocator<int> >(4096, 1165);
    test<int, std::allocator<int> >(4097, 157);
    LIBCPP_ONLY(test<int, limited_allocator<int, 4096> >(4095, 90));
#if TEST_STD_VER >= 11
    test<int, min_allocator<int> >(4095, 90);
#endif

  return 0;
}
