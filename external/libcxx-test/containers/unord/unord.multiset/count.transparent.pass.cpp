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

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multiset

// template <typename K>
// size_type count(const K& k) const;

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <unordered_set>

#include "test_transparent_unordered.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multiset_count_transparent(void) {
    using key_type = StoredType<int>;

    {
      // Make sure conversions don't happen for transparent non-final hasher and key_equal
      using S = unord_set_type<std::unordered_multiset, transparent_hash, std::equal_to<>>;
      test_transparent_count<S>({1, 1, 2});
      test_transparent_count<const S>({1, 1, 2});
    }

    {
      // Make sure conversions don't happen for transparent final hasher and key_equal
      using S = unord_set_type<std::unordered_multiset, transparent_hash_final, transparent_equal_final>;
      test_transparent_count<S>({1, 1, 2});
      test_transparent_count<const S>({1, 1, 2});
    }

    {
      // Make sure conversions do happen for non-transparent hasher
      using S = unord_set_type<std::unordered_multiset, non_transparent_hash, std::equal_to<>>;
      test_non_transparent_count<S>({1, 1, 2});
      test_non_transparent_count<const S>({1, 1, 2});
    }

    {
      // Make sure conversions do happen for non-transparent key_equal
      using S = unord_set_type<std::unordered_multiset, transparent_hash, std::equal_to<key_type>>;
      test_non_transparent_count<S>({1, 1, 2});
      test_non_transparent_count<const S>({1, 1, 2});
    }

    {
      // Make sure conversions do happen for both non-transparent hasher and key_equal
      using S = unord_set_type<std::unordered_multiset, non_transparent_hash, std::equal_to<key_type>>;
      test_non_transparent_count<S>({1, 1, 2});
      test_non_transparent_count<const S>({1, 1, 2});
    }

    return 0;
}
