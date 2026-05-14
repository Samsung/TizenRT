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

// <set>

// namespace std::pmr {
// template <class V, class Compare = less<V> >
// using set =
//     ::std::set<V, Compare, polymorphic_allocator<V>>
//
// template <class V, class Compare = less<V> >
// using multiset =
//     ::std::multiset<V, Compare, polymorphic_allocator<V>>
//
// } // namespace std::pmr

#include <set>
#include <memory_resource>
#include <type_traits>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_aliases_header_set_synop(void) {
  using V  = char;
  using DC = std::less<V>;
  using OC = std::greater<V>;
  {
    using StdSet = std::set<V, DC, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::set<V>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    using StdSet = std::set<V, OC, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::set<V, OC>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    std::pmr::set<int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }
  {
    using StdSet = std::multiset<V, DC, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::multiset<V>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    using StdSet = std::multiset<V, OC, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::multiset<V, OC>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    std::pmr::multiset<int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }

  return 0;
}
