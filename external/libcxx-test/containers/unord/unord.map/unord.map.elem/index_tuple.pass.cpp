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
// UNSUPPORTED: c++03

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// mapped_type& operator[](const key_type& k);

// https://llvm.org/PR16542

#include <cstddef>
#include <tuple>
#include <unordered_map>
#include "libcxx_tc_common.h"

struct my_hash {
    std::size_t operator()(const std::tuple<int, int>&) const { return 0; }
};

int tc_containers_unord_unord_map_unord_map_elem_index_tuple(void) {
    std::unordered_map<std::tuple<int, int>, std::size_t, my_hash> m;
    m[std::make_tuple(2, 3)] = 7;

    return 0;
}
