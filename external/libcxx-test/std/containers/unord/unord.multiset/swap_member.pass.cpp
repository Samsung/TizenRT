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
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// void swap(unordered_multiset& __u);

#include <unordered_set>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "../../test_compare.h"
#include "../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multiset_swap_member(void) {
    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef test_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        C c1(0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        c1.swap(c2);

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
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef test_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a2[] =
        {
            P(10),
            P(20),
            P(30),
            P(40),
            P(50),
            P(60),
            P(70),
            P(80)
        };
        C c1(0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        c1.swap(c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(*c1.find(10) == 10);
        TC_ASSERT_EXPR(*c1.find(20) == 20);
        TC_ASSERT_EXPR(*c1.find(30) == 30);
        TC_ASSERT_EXPR(*c1.find(40) == 40);
        TC_ASSERT_EXPR(*c1.find(50) == 50);
        TC_ASSERT_EXPR(*c1.find(60) == 60);
        TC_ASSERT_EXPR(*c1.find(70) == 70);
        TC_ASSERT_EXPR(*c1.find(80) == 80);
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
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef test_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a1[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        c1.swap(c2);

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
        TC_ASSERT_EXPR(c2.count(1) == 2);
        TC_ASSERT_EXPR(c2.count(2) == 2);
        TC_ASSERT_EXPR(c2.count(3) == 1);
        TC_ASSERT_EXPR(c2.count(4) == 1);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator().get_id() == 2);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef test_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a1[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        P a2[] =
        {
            P(10),
            P(20),
            P(30),
            P(40),
            P(50),
            P(60),
            P(70),
            P(80)
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1, 1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(1, 2));
        c2.max_load_factor(2);
        c1.swap(c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(*c1.find(10) == 10);
        TC_ASSERT_EXPR(*c1.find(20) == 20);
        TC_ASSERT_EXPR(*c1.find(30) == 30);
        TC_ASSERT_EXPR(*c1.find(40) == 40);
        TC_ASSERT_EXPR(*c1.find(50) == 50);
        TC_ASSERT_EXPR(*c1.find(60) == 60);
        TC_ASSERT_EXPR(*c1.find(70) == 70);
        TC_ASSERT_EXPR(*c1.find(80) == 80);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.count(1) == 2);
        TC_ASSERT_EXPR(c2.count(2) == 2);
        TC_ASSERT_EXPR(c2.count(3) == 1);
        TC_ASSERT_EXPR(c2.count(4) == 1);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator().get_id() == 2);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }

    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef other_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        C c1(0, Hash(1), Compare(1), Alloc(1));
        C c2(0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        c1.swap(c2);

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
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef other_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a2[] =
        {
            P(10),
            P(20),
            P(30),
            P(40),
            P(50),
            P(60),
            P(70),
            P(80)
        };
        C c1(0, Hash(1), Compare(1), Alloc(1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        c1.swap(c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(*c1.find(10) == 10);
        TC_ASSERT_EXPR(*c1.find(20) == 20);
        TC_ASSERT_EXPR(*c1.find(30) == 30);
        TC_ASSERT_EXPR(*c1.find(40) == 40);
        TC_ASSERT_EXPR(*c1.find(50) == 50);
        TC_ASSERT_EXPR(*c1.find(60) == 60);
        TC_ASSERT_EXPR(*c1.find(70) == 70);
        TC_ASSERT_EXPR(*c1.find(80) == 80);
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
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef other_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a1[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1));
        C c2(0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        c1.swap(c2);

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
        TC_ASSERT_EXPR(c2.count(1) == 2);
        TC_ASSERT_EXPR(c2.count(2) == 2);
        TC_ASSERT_EXPR(c2.count(3) == 1);
        TC_ASSERT_EXPR(c2.count(4) == 1);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc(1));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef other_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a1[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        P a2[] =
        {
            P(10),
            P(20),
            P(30),
            P(40),
            P(50),
            P(60),
            P(70),
            P(80)
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc(1));
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc(2));
        c2.max_load_factor(2);
        c1.swap(c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(*c1.find(10) == 10);
        TC_ASSERT_EXPR(*c1.find(20) == 20);
        TC_ASSERT_EXPR(*c1.find(30) == 30);
        TC_ASSERT_EXPR(*c1.find(40) == 40);
        TC_ASSERT_EXPR(*c1.find(50) == 50);
        TC_ASSERT_EXPR(*c1.find(60) == 60);
        TC_ASSERT_EXPR(*c1.find(70) == 70);
        TC_ASSERT_EXPR(*c1.find(80) == 80);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc(2));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.count(1) == 2);
        TC_ASSERT_EXPR(c2.count(2) == 2);
        TC_ASSERT_EXPR(c2.count(3) == 1);
        TC_ASSERT_EXPR(c2.count(4) == 1);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc(1));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
#if TEST_STD_VER >= 11
    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef min_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        C c1(0, Hash(1), Compare(1), Alloc());
        C c2(0, Hash(2), Compare(2), Alloc());
        c2.max_load_factor(2);
        c1.swap(c2);

        LIBCPP_ASSERT(c1.bucket_count() == 0);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        LIBCPP_ASSERT(c2.bucket_count() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef min_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a2[] =
        {
            P(10),
            P(20),
            P(30),
            P(40),
            P(50),
            P(60),
            P(70),
            P(80)
        };
        C c1(0, Hash(1), Compare(1), Alloc());
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc());
        c2.max_load_factor(2);
        c1.swap(c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(*c1.find(10) == 10);
        TC_ASSERT_EXPR(*c1.find(20) == 20);
        TC_ASSERT_EXPR(*c1.find(30) == 30);
        TC_ASSERT_EXPR(*c1.find(40) == 40);
        TC_ASSERT_EXPR(*c1.find(50) == 50);
        TC_ASSERT_EXPR(*c1.find(60) == 60);
        TC_ASSERT_EXPR(*c1.find(70) == 70);
        TC_ASSERT_EXPR(*c1.find(80) == 80);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        LIBCPP_ASSERT(c2.bucket_count() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef min_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a1[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc());
        C c2(0, Hash(2), Compare(2), Alloc());
        c2.max_load_factor(2);
        c1.swap(c2);

        LIBCPP_ASSERT(c1.bucket_count() == 0);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.count(1) == 2);
        TC_ASSERT_EXPR(c2.count(2) == 2);
        TC_ASSERT_EXPR(c2.count(3) == 1);
        TC_ASSERT_EXPR(c2.count(4) == 1);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
    {
        typedef test_hash<int> Hash;
        typedef test_equal_to<int> Compare;
        typedef min_allocator<int> Alloc;
        typedef std::unordered_multiset<int, Hash, Compare, Alloc> C;
        typedef int P;
        P a1[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        P a2[] =
        {
            P(10),
            P(20),
            P(30),
            P(40),
            P(50),
            P(60),
            P(70),
            P(80)
        };
        C c1(std::begin(a1), std::end(a1), 0, Hash(1), Compare(1), Alloc());
        C c2(std::begin(a2), std::end(a2), 0, Hash(2), Compare(2), Alloc());
        c2.max_load_factor(2);
        c1.swap(c2);

        TC_ASSERT_EXPR(c1.bucket_count() >= 8);
        TC_ASSERT_EXPR(c1.size() == 8);
        TC_ASSERT_EXPR(*c1.find(10) == 10);
        TC_ASSERT_EXPR(*c1.find(20) == 20);
        TC_ASSERT_EXPR(*c1.find(30) == 30);
        TC_ASSERT_EXPR(*c1.find(40) == 40);
        TC_ASSERT_EXPR(*c1.find(50) == 50);
        TC_ASSERT_EXPR(*c1.find(60) == 60);
        TC_ASSERT_EXPR(*c1.find(70) == 70);
        TC_ASSERT_EXPR(*c1.find(80) == 80);
        TC_ASSERT_EXPR(c1.hash_function() == Hash(2));
        TC_ASSERT_EXPR(c1.key_eq() == Compare(2));
        TC_ASSERT_EXPR(c1.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.cbegin(), c1.cend())) == c1.size());
        TC_ASSERT_EXPR(c1.max_load_factor() == 2);

        TC_ASSERT_EXPR(c2.bucket_count() >= 6);
        TC_ASSERT_EXPR(c2.size() == 6);
        TC_ASSERT_EXPR(c2.count(1) == 2);
        TC_ASSERT_EXPR(c2.count(2) == 2);
        TC_ASSERT_EXPR(c2.count(3) == 1);
        TC_ASSERT_EXPR(c2.count(4) == 1);
        TC_ASSERT_EXPR(c2.hash_function() == Hash(1));
        TC_ASSERT_EXPR(c2.key_eq() == Compare(1));
        TC_ASSERT_EXPR(c2.get_allocator() == Alloc());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.begin(), c2.end())) == c2.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c2.cbegin(), c2.cend())) == c2.size());
        TC_ASSERT_EXPR(c2.max_load_factor() == 1);
    }
#endif

  return 0;
}
