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
// <unordered_map>

// template <class Key, class T, class Hash, class Pred, class Alloc>
// bool
// operator==(const unordered_map<Key, T, Hash, Pred, Alloc>& x,
//            const unordered_map<Key, T, Hash, Pred, Alloc>& y);
//
// template <class Key, class T, class Hash, class Pred, class Alloc>
// bool
// operator!=(const unordered_map<Key, T, Hash, Pred, Alloc>& x,
//            const unordered_map<Key, T, Hash, Pred, Alloc>& y);

#include <unordered_map>
#include <string>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "min_allocator.h"

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_map_eq(void) {
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        const C c1(std::begin(a), std::end(a));
        const C c2;
        TC_ASSERT_EXPR(testEquality(c1, c2, false));
    }
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        const C c1(std::begin(a), std::end(a));
        const C c2 = c1;
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
    }
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        C c1(std::begin(a), std::end(a));
        C c2 = c1;
        c2.rehash(30);
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
        c2.insert(P(90, "ninety"));
        TC_ASSERT_EXPR(testEquality(c1, c2, false));
        c1.insert(P(90, "ninety"));
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
    }
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        C c1(std::begin(a), std::end(a));
        C c2 = c1;
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
        c1.insert(P(90, "ninety"));
        c2.insert(P(100, "onehundred"));
        TC_ASSERT_EXPR(testEquality(c1, c2, false));
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        const C c1(std::begin(a), std::end(a));
        const C c2;
        TC_ASSERT_EXPR(testEquality(c1, c2, false));
    }
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        const C c1(std::begin(a), std::end(a));
        const C c2 = c1;
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
    }
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        C c1(std::begin(a), std::end(a));
        C c2 = c1;
        c2.rehash(30);
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
        c2.insert(P(90, "ninety"));
        TC_ASSERT_EXPR(testEquality(c1, c2, false));
        c1.insert(P(90, "ninety"));
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
    }
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        C c1(std::begin(a), std::end(a));
        C c2 = c1;
        TC_ASSERT_EXPR(testEquality(c1, c2, true));
        c1.insert(P(90, "ninety"));
        c2.insert(P(100, "onehundred"));
        TC_ASSERT_EXPR(testEquality(c1, c2, false));
    }
#endif

  return 0;
}
