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

// constexpr mdspan();
// Constraints:
//   - rank_dynamic() > 0 is true.
//   - is_default_constructible_v<data_handle_type> is true.
//   - is_default_constructible_v<mapping_type> is true.
//   - is_default_constructible_v<accessor_type> is true.
//
// Preconditions: [0, map_.required_span_size()) is an accessible range of ptr_
//               and acc_ for the values of map_ and acc_ after the invocation of this constructor.
//
// Effects: Value-initializes ptr_, map_, and acc_.

#include <mdspan>
#include <type_traits>
#include <concepts>
#include <cassert>

#include "test_macros.h"

#include "../MinimalElementType.h"
#include "CustomTestLayouts.h"
#include "CustomTestAccessors.h"
#include "libcxx_tc_common.h"

template <bool hc, bool mc, bool ac, class H, class M, class A>
constexpr void test_mdspan_types(const H&, const M&, const A&) {
  using MDS = std::mdspan<typename A::element_type, typename M::extents_type, typename M::layout_type, A>;

  static_assert(hc == std::is_default_constructible_v<H>);
  static_assert(mc == std::is_default_constructible_v<M>);
  static_assert(ac == std::is_default_constructible_v<A>);

  if constexpr (MDS::rank_dynamic() > 0 && hc && mc && ac) {
    MDS m;
    static_assert(noexcept(MDS()) == (noexcept(H())&& noexcept(M())&& noexcept(A())));
    TC_ASSERT_EXPR(m.extents() == typename MDS::extents_type());
    if constexpr (std::equality_comparable<H>)
      TC_ASSERT_EXPR(m.data_handle() == H());
    if constexpr (std::equality_comparable<M>)
      TC_ASSERT_EXPR(m.mapping() == M());
    if constexpr (std::equality_comparable<A>)
      TC_ASSERT_EXPR(m.accessor() == A());
  } else {
    static_assert(!std::is_default_constructible_v<MDS>);
  }
}

template <bool hc, bool mc, bool ac, class H, class L, class A>
constexpr void mixin_extents(const H& handle, const L& layout, const A& acc) {
  constexpr size_t D = std::dynamic_extent;
  test_mdspan_types<hc, mc, ac>(handle, construct_mapping(layout, std::extents<int>()), acc);
  test_mdspan_types<hc, mc, ac>(handle, construct_mapping(layout, std::extents<char, D>(7)), acc);
  test_mdspan_types<hc, mc, ac>(handle, construct_mapping(layout, std::extents<unsigned, 7>()), acc);
  test_mdspan_types<hc, mc, ac>(handle, construct_mapping(layout, std::extents<size_t, D, 4, D>(2, 3)), acc);
  test_mdspan_types<hc, mc, ac>(handle, construct_mapping(layout, std::extents<char, D, 7, D>(0, 3)), acc);
  test_mdspan_types<hc, mc, ac>(
      handle, construct_mapping(layout, std::extents<int64_t, D, 7, D, 4, D, D>(1, 2, 3, 2)), acc);
}

template <bool hc, bool ac, class H, class A>
constexpr void mixin_layout(const H& handle, const A& acc) {
  mixin_extents<hc, true, ac>(handle, std::layout_left(), acc);
  mixin_extents<hc, true, ac>(handle, std::layout_right(), acc);

  // Use weird layout, make sure it has the properties we want to test
  constexpr size_t D = std::dynamic_extent;
  static_assert(
      !std::is_default_constructible_v< typename layout_wrapping_integral<4>::template mapping<std::extents<char, D>>>);
  mixin_extents<hc, false, ac>(handle, layout_wrapping_integral<4>(), acc);
}

template <class T>
constexpr void mixin_accessor() {
  ElementPool<T, 1024> elements;
  mixin_layout<true, true>(elements.get_ptr(), std::default_accessor<T>());

  // Using weird accessor/data_handle
  // Make sure they actually got the properties we want to test
  // checked_accessor is not default constructible except for const double, where it is not noexcept
  static_assert(std::is_default_constructible_v<checked_accessor<T>> == std::is_same_v<T, const double>);
  // checked_accessor's data handle type is not default constructible for double
  static_assert(
      std::is_default_constructible_v<typename checked_accessor<T>::data_handle_type> != std::is_same_v<T, double>);
  mixin_layout<!std::is_same_v<T, double>, std::is_same_v<T, const double>>(
      typename checked_accessor<T>::data_handle_type(elements.get_ptr()), checked_accessor<T>(1024));
}

constexpr bool test() {
  mixin_accessor<int>();
  mixin_accessor<const int>();
  mixin_accessor<double>();
  mixin_accessor<const double>();
  mixin_accessor<MinimalElementType>();
  mixin_accessor<const MinimalElementType>();
  return true;
}
int tc_containers_views_mdspan_mdspan_ctor_default(void) {
  test();
  static_assert(test());
  return 0;
}
