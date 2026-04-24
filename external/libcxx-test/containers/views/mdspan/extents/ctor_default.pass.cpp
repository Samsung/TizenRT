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
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <mdspan>

// Test default construction:
//
// constexpr extents() noexcept = default;
//
// Remarks: since the standard uses an exposition only array member, dynamic extents
// need to be zero intialized!

#include <mdspan>
#include <cassert>
#include <array>

#include "../ConvertibleToIntegral.h"
#include "CtorTestCombinations.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct DefaultCtorTest {
  template <class E, class AllExtents, class Extents, size_t... Indices>
  static constexpr void test_construction(AllExtents all_ext, Extents, std::index_sequence<Indices...>) {
    // This function gets called twice: once with Extents being just the dynamic ones, and once with all the extents specified.
    // We only test during the all extent case, since then Indices is the correct number. This allows us to reuse the same
    // testing machinery used in other constructor tests.
    if constexpr (sizeof...(Indices) == E::rank()) {
      ASSERT_NOEXCEPT(E{});
      // Need to construct new expected values, replacing dynamic values with 0
      std::array<typename AllExtents::value_type, E::rank()> expected_exts{
          ((E::static_extent(Indices) == std::dynamic_extent)
               ? typename AllExtents::value_type(0)
               : all_ext[Indices])...};
      test_runtime_observers(E{}, expected_exts);
    }
  }
};

int tc_containers_views_mdspan_extents_ctor_default(void) {
  test_index_type_combo<DefaultCtorTest>();
  static_assert(test_index_type_combo<DefaultCtorTest>());
  return 0;
}
