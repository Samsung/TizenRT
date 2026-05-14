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

//       reference operator[](size_type __i);
// const_reference operator[](size_type __i) const;
//
//       reference at(size_type __i);
// const_reference at(size_type __i) const;
//
//       reference front();
// const_reference front() const;
//
//       reference back();
// const_reference back() const;
// libc++ marks these as 'noexcept'

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "min_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class C>
C
make(int size, int start = 0 )
{
    const int b = 4096 / sizeof(int);
    int init = 0;
    if (start > 0)
    {
        init = (start+1) / b + ((start+1) % b != 0);
        init *= b;
        --init;
    }
    C c(init, 0);
    for (int i = 0; i < init-start; ++i)
        c.pop_back();
    for (int i = 0; i < size; ++i)
        c.push_back(i);
    for (int i = 0; i < start; ++i)
        c.pop_front();
    return c;
}

int tc_containers_sequences_deque_deque_capacity_access(void) {
    {
        typedef std::deque<int> C;
        C c = make<std::deque<int> >(10);
        ASSERT_SAME_TYPE(decltype(c[0]), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(   c[0]);
        LIBCPP_ASSERT_NOEXCEPT(   c.front());
        ASSERT_SAME_TYPE(decltype(c.front()), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(   c.back());
        ASSERT_SAME_TYPE(decltype(c.back()), C::reference);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c[i] == i);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c.at(i) == i);
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(c.back() == 9);
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    }
    {
        typedef std::deque<int> C;
        const C c = make<std::deque<int> >(10);
        ASSERT_SAME_TYPE(decltype(c[0]), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(   c[0]);
        LIBCPP_ASSERT_NOEXCEPT(   c.front());
        ASSERT_SAME_TYPE(decltype(c.front()), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(   c.back());
        ASSERT_SAME_TYPE(decltype(c.back()), C::const_reference);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c[i] == i);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c.at(i) == i);
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(c.back() == 9);
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    }
#if TEST_STD_VER >= 11
    {
        typedef std::deque<int, min_allocator<int>> C;
        C c = make<std::deque<int, min_allocator<int>> >(10);
        ASSERT_SAME_TYPE(decltype(c[0]), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(   c[0]);
        LIBCPP_ASSERT_NOEXCEPT(   c.front());
        ASSERT_SAME_TYPE(decltype(c.front()), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(   c.back());
        ASSERT_SAME_TYPE(decltype(c.back()), C::reference);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c[i] == i);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c.at(i) == i);
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(c.back() == 9);
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    }
    {
        typedef std::deque<int, min_allocator<int>> C;
        const C c = make<std::deque<int, min_allocator<int>> >(10);
        ASSERT_SAME_TYPE(decltype(c[0]), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(   c[0]);
        LIBCPP_ASSERT_NOEXCEPT(   c.front());
        ASSERT_SAME_TYPE(decltype(c.front()), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(   c.back());
        ASSERT_SAME_TYPE(decltype(c.back()), C::const_reference);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c[i] == i);
        for (int i = 0; i < 10; ++i)
            TC_ASSERT_EXPR(c.at(i) == i);
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(c.back() == 9);
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    }
#endif

  return 0;
}
