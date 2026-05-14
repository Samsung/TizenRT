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
// template <class T>
// using list =
//     ::std::list<T, polymorphic_allocator<T>>
//
// } // namespace std::pmr

#include <list>
#include <memory_resource>
#include <type_traits>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_aliases_header_list_synop(void) {
  using StdList = std::list<int, std::pmr::polymorphic_allocator<int>>;
  using PmrList = std::pmr::list<int>;
  static_assert(std::is_same<StdList, PmrList>::value, "");
  PmrList d;
  TC_ASSERT_EXPR(d.get_allocator().resource() == std::pmr::get_default_resource());

  return 0;
}
