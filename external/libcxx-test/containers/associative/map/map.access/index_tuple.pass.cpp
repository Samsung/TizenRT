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

// <map>

// class map

// mapped_type& operator[](const key_type& k);

// https://llvm.org/PR16542

#include <map>
#include <tuple>
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_access_index_tuple(void) {
    std::map<std::tuple<int, int>, std::size_t> m;
    m[std::make_tuple(2, 3)] = 7;

    return 0;
}
