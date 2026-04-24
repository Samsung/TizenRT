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

// template<class IndexType, size_t Rank>
//     using dextents = see below;
//
// Result: A type E that is a specialization of extents such that
//         E::rank() == Rank && E::rank() == E::rank_dynamic() is true,
//         and E::index_type denotes IndexType.

#include <mdspan>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class IndexType>
void test_alias_template_dextents() {
  constexpr size_t D = std::dynamic_extent;
  ASSERT_SAME_TYPE(std::dextents<IndexType, 0>, std::extents<IndexType>);
  ASSERT_SAME_TYPE(std::dextents<IndexType, 1>, std::extents<IndexType, D>);
  ASSERT_SAME_TYPE(std::dextents<IndexType, 2>, std::extents<IndexType, D, D>);
  ASSERT_SAME_TYPE(std::dextents<IndexType, 3>, std::extents<IndexType, D, D, D>);
  ASSERT_SAME_TYPE(std::dextents<IndexType, 9>, std::extents<IndexType, D, D, D, D, D, D, D, D, D>);
}

int tc_containers_views_mdspan_extents_dextents(void) {
  test_alias_template_dextents<int>();
  test_alias_template_dextents<unsigned int>();
  test_alias_template_dextents<size_t>();
  return 0;
}
