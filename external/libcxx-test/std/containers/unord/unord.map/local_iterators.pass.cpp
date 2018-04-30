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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// local_iterator       begin (size_type n);
// local_iterator       end   (size_type n);
// const_local_iterator begin (size_type n) const;
// const_local_iterator end   (size_type n) const;
// const_local_iterator cbegin(size_type n) const;
// const_local_iterator cend  (size_type n) const;

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_containers_unord_map_local_iterators(void)
{
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
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
        TC_ASSERT_EXPR(i->first == 1);
        TC_ASSERT_EXPR(i->second == "one");

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 2);
        TC_ASSERT_EXPR(i->second == "two");

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");
    }
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
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
        TC_ASSERT_EXPR(i->first == 1);
        TC_ASSERT_EXPR(i->second == "one");

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 2);
        TC_ASSERT_EXPR(i->second == "two");

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");
    }
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
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
        TC_ASSERT_EXPR(i->first == 1);
        TC_ASSERT_EXPR(i->second == "one");

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 2);
        TC_ASSERT_EXPR(i->second == "two");

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");
    }
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
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
        TC_ASSERT_EXPR(i->first == 1);
        TC_ASSERT_EXPR(i->second == "one");

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 2);
        TC_ASSERT_EXPR(i->second == "two");

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
