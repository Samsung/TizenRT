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

// iterator erase(const_iterator position);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

struct TemplateConstructor
{
    template<typename T>
    TemplateConstructor (const T&) {}
};

bool operator<(const TemplateConstructor&, const TemplateConstructor&) { return false; }

int tc_containers_associative_multiset_erase_iter(void) {
    {
        typedef std::multiset<int> M;
        typedef int V;
        typedef M::iterator I;
        V ar[] =
        {
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 8);
        I i = m.erase(std::next(m.cbegin(), 3));
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 8);

        i = m.erase(std::next(m.cbegin(), 0));
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 8);

        i = m.erase(std::next(m.cbegin(), 5));
        TC_ASSERT_EXPR(m.size() == 5);
        TC_ASSERT_EXPR(i == m.end());
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 7);

        i = m.erase(std::next(m.cbegin(), 1));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(i == std::next(m.begin()));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 7);

        i = m.erase(std::next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 7);

        i = m.erase(std::next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 5);

        i = m.erase(std::next(m.cbegin(), 0));
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 5);

        i = m.erase(m.cbegin());
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(i == m.end());
    }
#if TEST_STD_VER >= 11
    {
        typedef std::multiset<int, std::less<int>, min_allocator<int>> M;
        typedef int V;
        typedef M::iterator I;
        V ar[] =
        {
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 8);
        I i = m.erase(std::next(m.cbegin(), 3));
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 8);

        i = m.erase(std::next(m.cbegin(), 0));
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 8);

        i = m.erase(std::next(m.cbegin(), 5));
        TC_ASSERT_EXPR(m.size() == 5);
        TC_ASSERT_EXPR(i == m.end());
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 7);

        i = m.erase(std::next(m.cbegin(), 1));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(i == std::next(m.begin()));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 7);

        i = m.erase(std::next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 7);

        i = m.erase(std::next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 5);

        i = m.erase(std::next(m.cbegin(), 0));
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 5);

        i = m.erase(m.cbegin());
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(i == m.end());
    }
#endif
#if TEST_STD_VER >= 14
    {
    //  This is LWG #2059
        typedef TemplateConstructor T;
        typedef std::multiset<T> C;
        typedef C::iterator I;

        C c;
        T a{0};
        I it = c.find(a);
        if (it != c.end())
            c.erase(it);
    }
#endif

  return 0;
}
