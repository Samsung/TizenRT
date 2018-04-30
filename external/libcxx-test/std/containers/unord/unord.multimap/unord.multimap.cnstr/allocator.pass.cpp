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

// explicit unordered_multimap(const allocator_type& __a);

#include <unordered_map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "NotConstructible.h"
#include "test_compare.h"
#include "test_hash.h"
#include "test_allocator.h"

int tc_libcxx_containers_unord_multimap_cnstr_allocator(void)
{
    {
        typedef std::unordered_multimap<NotConstructible, NotConstructible,
                                   test_hash<std::hash<NotConstructible> >,
                                   test_compare<std::equal_to<NotConstructible> >,
                                   test_allocator<std::pair<const NotConstructible,
                                                                  NotConstructible> >
                                   > C;
        C c(test_allocator<std::pair<const NotConstructible, NotConstructible> >(10));
        LIBCPP_ASSERT(c.bucket_count() == 0);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<std::hash<NotConstructible> >());
        TC_ASSERT_EXPR(c.key_eq() == test_compare<std::equal_to<NotConstructible> >());
        TC_ASSERT_EXPR(c.get_allocator() ==
               (test_allocator<std::pair<const NotConstructible, NotConstructible> >(10)));
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER > 11
    {
        typedef NotConstructible T;
        typedef test_allocator<std::pair<const T, T>> A;
        typedef test_hash<std::hash<T>> HF;
        typedef test_compare<std::equal_to<T>> Comp;
        typedef std::unordered_multimap<T, T, HF, Comp, A> C;

        A a(10);
        C c(2, a);
        LIBCPP_ASSERT(c.bucket_count() == 2);
        TC_ASSERT_EXPR(c.hash_function() == HF());
        TC_ASSERT_EXPR(c.key_eq() == Comp());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef NotConstructible T;
        typedef test_allocator<std::pair<const T, T>> A;
        typedef test_hash<std::hash<T>> HF;
        typedef test_compare<std::equal_to<T>> Comp;
        typedef std::unordered_multimap<T, T, HF, Comp, A> C;

        A a(10);
        HF hf(12);
        C c(2, hf, a);
        LIBCPP_ASSERT(c.bucket_count() == 2);
        TC_ASSERT_EXPR(c.hash_function() == hf);
        TC_ASSERT_EXPR(!(c.hash_function() == HF()));
        TC_ASSERT_EXPR(c.key_eq() == Comp());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
