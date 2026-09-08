//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// template<class InputIterator>
//   basic_string(InputIterator begin, InputIterator end,
//   const Allocator& a = Allocator());


#include <string>
#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "../input_iterator.h"

template <class It>
static int
test(It first, It last)
{
    typedef typename std::iterator_traits<It>::value_type charT;
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::allocator_type A;
    S s2(first, last);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == static_cast<std::size_t>(std::distance(first, last)));
    unsigned i = 0;
    for (It it = first; it != last; ++it, ++i)
        TC_ASSERT_EXPR(s2[i] == *it);
    TC_ASSERT_EXPR(s2.get_allocator() == A());
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

template <class It, class A>
static int
test(It first, It last, const A& a)
{
    typedef typename std::iterator_traits<It>::value_type charT;
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    S s2(first, last, a);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == static_cast<std::size_t>(std::distance(first, last)));
    unsigned i = 0;
    for (It it = first; it != last; ++it, ++i)
        TC_ASSERT_EXPR(s2[i] == *it);
    TC_ASSERT_EXPR(s2.get_allocator() == a);
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

int tc_libcxx_strings_string_cons_iter_alloc(void)
{
    {
    typedef test_allocator<char> A;
    const char* s = "12345678901234567890123456789012345678901234567890";

    TC_ASSERT_FUNC((test(s, s)));
    TC_ASSERT_FUNC((test(s, s, A(2))));

    TC_ASSERT_FUNC((test(s, s+1)));
    TC_ASSERT_FUNC((test(s, s+1, A(2))));

    TC_ASSERT_FUNC((test(s, s+10)));
    TC_ASSERT_FUNC((test(s, s+10, A(2))));

    TC_ASSERT_FUNC((test(s, s+50)));
    TC_ASSERT_FUNC((test(s, s+50, A(2))));

    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s))));
    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s), A(2))));

    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+1))));
    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+1), A(2))));

    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+10), A(2))));

    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+50))));
    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+50), A(2))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
