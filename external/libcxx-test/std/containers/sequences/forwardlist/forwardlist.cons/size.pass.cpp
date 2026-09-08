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

// <forward_list>

// explicit forward_list(size_type n);
// explicit forward_list(size_type n, const Alloc& a);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"
#include "DefaultOnly.h"

template <class T, class Allocator>
static int check_allocator(unsigned n, Allocator const &alloc = Allocator())
{
#if TEST_STD_VER > 11
    typedef std::forward_list<T, Allocator> C;
    C d(n, alloc);
    TC_ASSERT_EXPR(d.get_allocator() == alloc);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == n);
#else
    ((void)n);
    ((void)alloc);
#endif
    return 0;
}

int tc_libcxx_containers_forwardlist_cons_size(void)
{
    { // test that the ctor is explicit
      typedef std::forward_list<DefaultOnly> C;
      static_assert((std::is_constructible<C, size_t>::value), "");
      static_assert((!std::is_convertible<size_t, C>::value), "");
    }
    {
        typedef DefaultOnly T;
        typedef std::forward_list<T> C;
        unsigned N = 10;
        C c(N);
        unsigned n = 0;

        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n) {
#if TEST_STD_VER >= 11
            TC_ASSERT_EXPR(*i == T());
#else
            ((static int)0);
#endif
        }
        TC_ASSERT_EXPR(n == N);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
