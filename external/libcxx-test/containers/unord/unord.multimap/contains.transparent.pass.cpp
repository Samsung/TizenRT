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

// template <typename K>
// bool contains(const K& x) const;

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <unordered_map>
#include "test_transparent_unordered.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multimap_contains_transparent(void) {
    using key_type = StoredType<int>;

    {
      // Make sure conversions don't happen for transparent non-final hasher and key_equal
      using M = unord_map_type<std::unordered_multimap, transparent_hash, std::equal_to<>>;
      test_transparent_contains<M>({{1, 2}, {1, 3}, {2, 3}});
      test_transparent_contains<const M>({{1, 2}, {1, 3}, {2, 3}});
    }

    {
      // Make sure conversions don't happen for transparent final hasher and key_equal
      using M = unord_map_type<std::unordered_multimap, transparent_hash_final, transparent_equal_final>;
      test_transparent_contains<M>({{1, 2}, {1, 3}, {2, 3}});
      test_transparent_contains<const M>({{1, 2}, {1, 3}, {2, 3}});
    }

    {
      // Make sure conversions do happen for non-transparent hasher
      using M = unord_map_type<std::unordered_multimap, non_transparent_hash, std::equal_to<>>;
      test_non_transparent_contains<M>({{1, 2}, {1, 3}, {2, 3}});
      test_non_transparent_contains<const M>({{1, 2}, {1, 3}, {2, 3}});
    }

    {
      // Make sure conversions do happen for non-transparent key_equal
      using M = unord_map_type<std::unordered_multimap, transparent_hash, std::equal_to<key_type>>;
      test_non_transparent_contains<M>({{1, 2}, {1, 3}, {2, 3}});
      test_non_transparent_contains<const M>({{1, 2}, {1, 3}, {2, 3}});
    }

    {
      // Make sure conversions do happen for both non-transparent hasher and key_equal
      using M = unord_map_type<std::unordered_multimap, non_transparent_hash, std::equal_to<key_type>>;
      test_non_transparent_contains<M>({{1, 2}, {1, 3}, {2, 3}});
      test_non_transparent_contains<const M>({{1, 2}, {1, 3}, {2, 3}});
    }

    return 0;
}
