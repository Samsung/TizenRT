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
// class unordered_multimap

// static int swap(unordered_multimap& __u);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"
#include "test_compare.h"
#include "test_hash.h"
#include "test_allocator.h"

int tc_libcxx_containers_unord_multimap_swap_swap_non_member(void)
{
    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef test_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        C c1(0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        swap(c1, c2);

        LIBCPP_ASSERT(c1.bucket_count() == 0);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        LIBCPP_ASSERT(c2.bucket_count() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator().get_id() == 2);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef test_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        typedef std::pair<int, std::string> P;
        P a2[] =
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
        C c1(0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        swap(c1, c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(c1.find(10)->second == "ten");
        TC_ASSERT_EXPR(c1.find(20)->second == "twenty");
        TC_ASSERT_EXPR(c1.find(30)->second == "thirty");
        TC_ASSERT_EXPR(c1.find(40)->second == "forty");
        TC_ASSERT_EXPR(c1.find(50)->second == "fifty");
        TC_ASSERT_EXPR(c1.find(60)->second == "sixty");
        TC_ASSERT_EXPR(c1.find(70)->second == "seventy");
        TC_ASSERT_EXPR(c1.find(80)->second == "eighty");
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        LIBCPP_ASSERT(c2.bucket_count() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator().get_id() == 2);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef test_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        typedef std::pair<int, std::string> P;
        P a1[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        swap(c1, c2);

        LIBCPP_ASSERT(c1.bucket_count() == 0);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.find(1)->second == "one");
        TC_ASSERT_EXPR(next(c2.find(1))->second == "four");
        TC_ASSERT_EXPR(c2.find(2)->second == "two");
        TC_ASSERT_EXPR(next(c2.find(2))->second == "four");
        TC_ASSERT_EXPR(c2.find(3)->second == "three");
        TC_ASSERT_EXPR(c2.find(4)->second == "four");
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator().get_id() == 2);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef test_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        typedef std::pair<int, std::string> P;
        P a1[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        P a2[] =
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
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        swap(c1, c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(c1.find(10)->second == "ten");
        TC_ASSERT_EXPR(c1.find(20)->second == "twenty");
        TC_ASSERT_EXPR(c1.find(30)->second == "thirty");
        TC_ASSERT_EXPR(c1.find(40)->second == "forty");
        TC_ASSERT_EXPR(c1.find(50)->second == "fifty");
        TC_ASSERT_EXPR(c1.find(60)->second == "sixty");
        TC_ASSERT_EXPR(c1.find(70)->second == "seventy");
        TC_ASSERT_EXPR(c1.find(80)->second == "eighty");
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.find(1)->second == "one");
        TC_ASSERT_EXPR(next(c2.find(1))->second == "four");
        TC_ASSERT_EXPR(c2.find(2)->second == "two");
        TC_ASSERT_EXPR(next(c2.find(2))->second == "four");
        TC_ASSERT_EXPR(c2.find(3)->second == "three");
        TC_ASSERT_EXPR(c2.find(4)->second == "four");
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator().get_id() == 2);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }

    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef other_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        C c1(0, Hash(1), Compare(1), Alloc(1));
        C c2(0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        swap(c1, c2);

        LIBCPP_ASSERT(c1.bucket_count() == 0);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc(2));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        LIBCPP_ASSERT(c2.bucket_count() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc(1));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef other_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        typedef std::pair<int, std::string> P;
        P a2[] =
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
        C c1(0, Hash(1), Compare(1), Alloc(1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        swap(c1, c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(c1.find(10)->second == "ten");
        TC_ASSERT_EXPR(c1.find(20)->second == "twenty");
        TC_ASSERT_EXPR(c1.find(30)->second == "thirty");
        TC_ASSERT_EXPR(c1.find(40)->second == "forty");
        TC_ASSERT_EXPR(c1.find(50)->second == "fifty");
        TC_ASSERT_EXPR(c1.find(60)->second == "sixty");
        TC_ASSERT_EXPR(c1.find(70)->second == "seventy");
        TC_ASSERT_EXPR(c1.find(80)->second == "eighty");
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc(2));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        LIBCPP_ASSERT(c2.bucket_count() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc(1));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef other_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        typedef std::pair<int, std::string> P;
        P a1[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1));
        C c2(0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        swap(c1, c2);

        LIBCPP_ASSERT(c1.bucket_count() == 0);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc(2));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.find(1)->second == "one");
        TC_ASSERT_EXPR(next(c2.find(1))->second == "four");
        TC_ASSERT_EXPR(c2.find(2)->second == "two");
        TC_ASSERT_EXPR(next(c2.find(2))->second == "four");
        TC_ASSERT_EXPR(c2.find(3)->second == "three");
        TC_ASSERT_EXPR(c2.find(4)->second == "four");
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc(1));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<std::hash<int> > Hash;
        typedef test_compare<std::equal_to<int> > Compare;
        typedef other_allocator<std::pair<const int, std::string> > Alloc;
        typedef std::unordered_multimap<int, std::string, Hash, Compare, Alloc> C;
        typedef std::pair<int, std::string> P;
        P a1[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        P a2[] =
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
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        swap(c1, c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(c1.find(10)->second == "ten");
        TC_ASSERT_EXPR(c1.find(20)->second == "twenty");
        TC_ASSERT_EXPR(c1.find(30)->second == "thirty");
        TC_ASSERT_EXPR(c1.find(40)->second == "forty");
        TC_ASSERT_EXPR(c1.find(50)->second == "fifty");
        TC_ASSERT_EXPR(c1.find(60)->second == "sixty");
        TC_ASSERT_EXPR(c1.find(70)->second == "seventy");
        TC_ASSERT_EXPR(c1.find(80)->second == "eighty");
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc(2));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.find(1)->second == "one");
        TC_ASSERT_EXPR(next(c2.find(1))->second == "four");
        TC_ASSERT_EXPR(c2.find(2)->second == "two");
        TC_ASSERT_EXPR(next(c2.find(2))->second == "four");
        TC_ASSERT_EXPR(c2.find(3)->second == "three");
        TC_ASSERT_EXPR(c2.find(4)->second == "four");
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc(1));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
