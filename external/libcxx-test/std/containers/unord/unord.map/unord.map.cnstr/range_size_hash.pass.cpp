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

// template <class InputIterator>
//     unordered_map(InputIterator first, InputIterator last, size_type n,
//                   const hasher& hf);

#include <unordered_map>
#include <string>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"
#include "NotConstructible.h"
#include "test_compare.h"
#include "test_hash.h"
#include "test_allocator.h"

int main()
{
    {
        typedef std::unordered_map<int, std::string,
                                   test_hash<std::hash<int> >,
                                   test_compare<std::equal_to<int> >,
                                   test_allocator<std::pair<const int, std::string> >
                                   > C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(input_iterator<P*>(a), input_iterator<P*>(a + sizeof(a)/sizeof(a[0])),
            7,
            test_hash<std::hash<int> >(8)
           );
        LIBCPP_ASSERT(c.bucket_count() == 7);
        assert(c.size() == 4);
        assert(c.at(1) == "one");
        assert(c.at(2) == "two");
        assert(c.at(3) == "three");
        assert(c.at(4) == "four");
        assert(c.hash_function() == test_hash<std::hash<int> >(8));
        assert(c.key_eq() == test_compare<std::equal_to<int> >());
        assert(c.get_allocator() ==
               (test_allocator<std::pair<const int, std::string> >()));
        assert(!c.empty());
        assert(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        assert(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        assert(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        assert(c.max_load_factor() == 1);
    }
}
