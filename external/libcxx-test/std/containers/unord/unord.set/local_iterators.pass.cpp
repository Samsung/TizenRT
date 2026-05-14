//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// local_iterator       begin (size_type n);
// local_iterator       end   (size_type n);
// const_local_iterator begin (size_type n) const;
// const_local_iterator end   (size_type n) const;
// const_local_iterator cbegin(size_type n) const;
// const_local_iterator cend  (size_type n) const;

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_local_iterators(void) {
    {
        typedef std::unordered_set<int> C;
        typedef int P;
        typedef C::local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        typedef std::unordered_set<int> C;
        typedef int P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        typedef std::unordered_set<int> C;
        typedef int P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        typedef std::unordered_set<int> C;
        typedef int P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_set<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        typedef int P;
        typedef C::local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        typedef std::unordered_set<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        typedef int P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        typedef std::unordered_set<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        typedef int P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        typedef std::unordered_set<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        typedef int P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 1);

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 2);

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 3);

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(*i == 4);
    }
#endif

  return 0;
}
