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

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// unordered_multimap(size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a);

#include <unordered_map>
#include <cassert>

#include "test_macros.h"
#include "../../../NotConstructible.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multimap_unord_multimap_cnstr_size_hash_equal_allocator(void) {
    {
        typedef std::unordered_multimap<NotConstructible, NotConstructible,
                                   test_hash<NotConstructible>,
                                   test_equal_to<NotConstructible>,
                                   test_allocator<std::pair<const NotConstructible,
                                                                  NotConstructible> >
                                   > C;
        C c(7,
            test_hash<NotConstructible>(8),
            test_equal_to<NotConstructible>(9),
            test_allocator<std::pair<const NotConstructible, NotConstructible> >(10)
           );
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<NotConstructible>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<NotConstructible>(9));
        TC_ASSERT_EXPR(c.get_allocator() ==
               (test_allocator<std::pair<const NotConstructible, NotConstructible> >(10)));
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multimap<NotConstructible, NotConstructible,
                                   test_hash<NotConstructible>,
                                   test_equal_to<NotConstructible>,
                                   min_allocator<std::pair<const NotConstructible,
                                                                 NotConstructible> >
                                   > C;
        C c(7,
            test_hash<NotConstructible>(8),
            test_equal_to<NotConstructible>(9),
            min_allocator<std::pair<const NotConstructible, NotConstructible> >()
           );
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<NotConstructible>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<NotConstructible>(9));
        TC_ASSERT_EXPR(c.get_allocator() ==
               (min_allocator<std::pair<const NotConstructible, NotConstructible> >()));
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef explicit_allocator<std::pair<const NotConstructible, NotConstructible> > A;
        typedef std::unordered_multimap<NotConstructible, NotConstructible,
                                   test_hash<NotConstructible>,
                                   test_equal_to<NotConstructible>,
                                   A
                                   > C;
        C c(7,
            test_hash<NotConstructible>(8),
            test_equal_to<NotConstructible>(9),
            A{}
           );
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<NotConstructible>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<NotConstructible>(9));
        TC_ASSERT_EXPR(c.get_allocator() == A{});
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#endif

  return 0;
}
