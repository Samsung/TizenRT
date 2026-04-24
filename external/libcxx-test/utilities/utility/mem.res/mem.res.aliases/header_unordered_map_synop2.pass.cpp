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
// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// <unordered_map>

// namespace std::pmr {
//
// typedef ... unordered_map
//
// } // namespace std::pmr

#include <unordered_map>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop2(void) {
  {
    // Check that std::pmr::unordered_map is usable without <memory_resource>.
    std::pmr::unordered_map<int, int> m;
    std::pmr::unordered_multimap<int, int> mm;
  }

  return 0;
}
