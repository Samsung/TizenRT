//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class multiset

// size_type erase(const key_type& k);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_erase_key(void) {
    {
        typedef std::multiset<int> M;
        typedef int V;
        typedef M::size_type I;
        V ar[] =
        {
            3,
            3,
            3,
            5,
            5,
            5,
            7,
            7,
            7
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 9);
        I i = m.erase(6);
        TC_ASSERT_EXPR(m.size() == 9);
        TC_ASSERT_EXPR(i == 0);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 8) == 7);

        i = m.erase(5);
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 7);

        i = m.erase(3);
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(i == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 7);

        i = m.erase(7);
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == 3);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::multiset<int, std::less<int>, min_allocator<int>> M;
        typedef int V;
        typedef M::size_type I;
        V ar[] =
        {
            3,
            3,
            3,
            5,
            5,
            5,
            7,
            7,
            7
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 9);
        I i = m.erase(6);
        TC_ASSERT_EXPR(m.size() == 9);
        TC_ASSERT_EXPR(i == 0);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 8) == 7);

        i = m.erase(5);
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 7);

        i = m.erase(3);
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(i == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 7);

        i = m.erase(7);
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == 3);
    }
#endif

  return 0;
}
