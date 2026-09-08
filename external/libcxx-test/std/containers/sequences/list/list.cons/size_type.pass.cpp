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

// <list>

// explicit list(size_type n);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>
#include "test_macros.h"
#include "DefaultOnly.h"
#include "test_allocator.h"

template <class T, class Allocator>
static int
test3(unsigned n, Allocator const &alloc = Allocator())
{
#if TEST_STD_VER > 11
    typedef std::list<T, Allocator> C;
    {
    C d(n, alloc);
    TC_ASSERT_EXPR(d.size() == n);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == n);
    TC_ASSERT_EXPR(d.get_allocator() == alloc);
    }
#else
    ((void)n);
    ((void)alloc);
#endif
    return 0;
}


int tc_libcxx_containers_list_cons_size_type(void)
{
    {
        std::list<int> l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
    }
    {
        // Add 2 for implementations that dynamically allocate a sentinel node and container proxy.
        std::list<int, limited_allocator<int, 3 + 2> > l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
    }
#if TEST_STD_VER > 11
    {
    }
#endif
#if TEST_STD_VER >= 11
    {
        std::list<DefaultOnly> l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
