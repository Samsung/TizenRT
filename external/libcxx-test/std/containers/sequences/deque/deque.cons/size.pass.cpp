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

// explicit deque(size_type n);

#include "asan_testing.h"
#include <deque>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class T, class Allocator>
void
test2(unsigned n)
{
#if TEST_STD_VER > 11
    typedef std::deque<T, Allocator> C;
    typedef typename C::const_iterator const_iterator;
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
    C d(n, Allocator());
    TC_ASSERT_EXPR(static_cast<unsigned>(DefaultOnly::count) == n);
    TC_ASSERT_EXPR(d.size() == n);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == d.size());
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    for (const_iterator i = d.begin(), e = d.end(); i != e; ++i)
        TC_ASSERT_EXPR(*i == T());
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
#else
    ((void)n);
#endif
}

template <class T, class Allocator>
void
test1(unsigned n)
{
    typedef std::deque<T, Allocator> C;
    typedef typename C::const_iterator const_iterator;
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
    C d(n);
    TC_ASSERT_EXPR(static_cast<unsigned>(DefaultOnly::count) == n);
    TC_ASSERT_EXPR(d.size() == n);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == d.size());
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
#if TEST_STD_VER >= 11
    for (const_iterator i = d.begin(), e = d.end(); i != e; ++i)
        TC_ASSERT_EXPR(*i == T());
#endif
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
}

template <class T, class Allocator>
void
test3(unsigned n, Allocator const &alloc = Allocator())
{
#if TEST_STD_VER > 11
    typedef std::deque<T, Allocator> C;
    {
    C d(n, alloc);
    TC_ASSERT_EXPR(d.size() == n);
    TC_ASSERT_EXPR(d.get_allocator() == alloc);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    }
#else
    ((void)n);
    ((void)alloc);
#endif
}

template <class T, class Allocator>
void
test(unsigned n)
{
    test1<T, Allocator> ( n );
    test2<T, Allocator> ( n );
}

int tc_containers_sequences_deque_deque_cons_size(void) {
    test<DefaultOnly, std::allocator<DefaultOnly> >(0);
    test<DefaultOnly, std::allocator<DefaultOnly> >(1);
    test<DefaultOnly, std::allocator<DefaultOnly> >(10);
    test<DefaultOnly, std::allocator<DefaultOnly> >(1023);
    test<DefaultOnly, std::allocator<DefaultOnly> >(1024);
    test<DefaultOnly, std::allocator<DefaultOnly> >(1025);
    test<DefaultOnly, std::allocator<DefaultOnly> >(2047);
    test<DefaultOnly, std::allocator<DefaultOnly> >(2048);
    test<DefaultOnly, std::allocator<DefaultOnly> >(2049);
    test<DefaultOnly, std::allocator<DefaultOnly> >(4095);
    test<DefaultOnly, std::allocator<DefaultOnly> >(4096);
    test<DefaultOnly, std::allocator<DefaultOnly> >(4097);

    LIBCPP_ONLY(test1<DefaultOnly, limited_allocator<DefaultOnly, 4096> >(4095));

#if TEST_STD_VER >= 11
    test<DefaultOnly, min_allocator<DefaultOnly> >(4095);
#endif

#if TEST_STD_VER > 11
    test3<DefaultOnly, std::allocator<DefaultOnly>> (1023);
    test3<int, std::allocator<int>>(1);
    test3<int, min_allocator<int>> (3);
#endif


  return 0;
}
