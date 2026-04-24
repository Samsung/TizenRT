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

// <list>

// namespace std::pmr {
//
// typedef ... list
//
// } // namespace std::pmr

#include <list>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_aliases_header_list_synop2(void) {
  {
    // Check that std::pmr::list is usable without <memory_resource>.
    std::pmr::list<int> l;
  }

  return 0;
}
