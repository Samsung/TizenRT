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
// <set>

// class multiset

//       iterator begin();
// const_iterator begin() const;
//       iterator end();
// const_iterator end()   const;
//
//       reverse_iterator rbegin();
// const_reverse_iterator rbegin() const;
//       reverse_iterator rend();
// const_reverse_iterator rend()   const;
//
// const_iterator         cbegin()  const;
// const_iterator         cend()    const;
// const_reverse_iterator crbegin() const;
// const_reverse_iterator crend()   const;

#include <set>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_iterator(void) {
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3,
            4,
            4,
            4,
            5,
            5,
            5,
            6,
            6,
            6,
            7,
            7,
            7,
            8,
            8,
            8
        };
        std::multiset<int> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.rbegin(), m.rend())) == m.size());
        std::multiset<int>::iterator i;
        i = m.begin();
        std::multiset<int>::const_iterator k = i;
        TC_ASSERT_EXPR(i == k);
        for (int j = 1; j <= 8; ++j)
            for (int n = 0; n < 3; ++n, ++i)
                TC_ASSERT_EXPR(*i == j);
        TC_ASSERT_EXPR(i == m.end());
        for (int j = 8; j >= 1; --j)
            for (int n = 0; n < 3; ++n) {
                --i;
                TC_ASSERT_EXPR(*i == j);
            }
        TC_ASSERT_EXPR(i == m.begin());
    }
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3,
            4,
            4,
            4,
            5,
            5,
            5,
            6,
            6,
            6,
            7,
            7,
            7,
            8,
            8,
            8
        };
        const std::multiset<int> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.cbegin(), m.cend())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.rbegin(), m.rend())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.crbegin(), m.crend())) == m.size());
        std::multiset<int>::const_iterator i;
        i = m.begin();
        for (int j = 1; j <= 8; ++j)
            for (int k = 0; k < 3; ++k, ++i)
                TC_ASSERT_EXPR(*i == j);
        TC_ASSERT_EXPR(i == m.end());
        for (int j = 8; j >= 1; --j)
            for (int k = 0; k < 3; ++k) {
                --i;
                TC_ASSERT_EXPR(*i == j);
            }
        TC_ASSERT_EXPR(i == m.begin());
    }
#if TEST_STD_VER >= 11
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3,
            4,
            4,
            4,
            5,
            5,
            5,
            6,
            6,
            6,
            7,
            7,
            7,
            8,
            8,
            8
        };
        std::multiset<int, std::less<int>, min_allocator<int>> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.rbegin(), m.rend())) == m.size());
        std::multiset<int, std::less<int>, min_allocator<int>>::iterator i;
        i = m.begin();
        std::multiset<int, std::less<int>, min_allocator<int>>::const_iterator k = i;
        TC_ASSERT_EXPR(i == k);
        for (int j = 1; j <= 8; ++j)
            for (int n = 0; n < 3; ++n, ++i)
                TC_ASSERT_EXPR(*i == j);
        TC_ASSERT_EXPR(i == m.end());
        for (int j = 8; j >= 1; --j)
            for (int n = 0; n < 3; ++n) {
                --i;
                TC_ASSERT_EXPR(*i == j);
            }
        TC_ASSERT_EXPR(i == m.begin());
    }
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3,
            4,
            4,
            4,
            5,
            5,
            5,
            6,
            6,
            6,
            7,
            7,
            7,
            8,
            8,
            8
        };
        const std::multiset<int, std::less<int>, min_allocator<int>> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.cbegin(), m.cend())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.rbegin(), m.rend())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.crbegin(), m.crend())) == m.size());
        std::multiset<int, std::less<int>, min_allocator<int>>::const_iterator i;
        i = m.begin();
        for (int j = 1; j <= 8; ++j)
            for (int k = 0; k < 3; ++k, ++i)
                TC_ASSERT_EXPR(*i == j);
        TC_ASSERT_EXPR(i == m.end());
        for (int j = 8; j >= 1; --j)
            for (int k = 0; k < 3; ++k) {
                --i;
                TC_ASSERT_EXPR(*i == j);
            }
        TC_ASSERT_EXPR(i == m.begin());
    }
#endif
#if TEST_STD_VER > 11
    { // N3644 testing
        typedef std::multiset<int> C;
        C::iterator ii1{}, ii2{};
        C::iterator ii4 = ii1;
        C::const_iterator cii{};
        TC_ASSERT_EXPR( ii1 == ii2 );
        TC_ASSERT_EXPR( ii1 == ii4 );

        TC_ASSERT_EXPR(!(ii1 != ii2 ));

        TC_ASSERT_EXPR( (ii1 == cii ));
        TC_ASSERT_EXPR( (cii == ii1 ));
        TC_ASSERT_EXPR(!(ii1 != cii ));
        TC_ASSERT_EXPR(!(cii != ii1 ));
    }
#endif

  return 0;
}
