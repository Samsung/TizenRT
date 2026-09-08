//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <mdspan>
//
// friend constexpr void swap(mdspan& x, mdspan& y) noexcept;
//
// Effects: Equivalent to:
//   swap(x.ptr_, y.ptr_);
//   swap(x.map_, y.map_);
//   swap(x.acc_, y.acc_);

#include <mdspan>
#include <type_traits>
#include <concepts>
#include <cassert>

#include "test_macros.h"

#include "../MinimalElementType.h"
#include "CustomTestLayouts.h"
#include "libcxx_tc_common.h"

template <class MDS>
constexpr void test_swap(MDS a, MDS b) {
  auto org_a = a;
  auto org_b = b;
  swap(a, b);
  TC_ASSERT_EXPR(a.extents() == org_b.extents());
  TC_ASSERT_EXPR(b.extents() == org_a.extents());
  if constexpr (std::equality_comparable<typename MDS::mapping_type>) {
    TC_ASSERT_EXPR(a.mapping() == org_b.mapping());
    TC_ASSERT_EXPR(b.mapping() == org_a.mapping());
  }
  if constexpr (std::equality_comparable<typename MDS::data_handle_type>) {
    TC_ASSERT_EXPR(a.data_handle() == org_b.data_handle());
    TC_ASSERT_EXPR(b.data_handle() == org_a.data_handle());
  }
  // This check uses a side effect of layout_wrapping_integral::swap to make sure
  // mdspan calls the underlying components' swap via ADL
  if !consteval {
    if constexpr (std::is_same_v<typename MDS::layout_type, layout_wrapping_integral<4>>) {
      TC_ASSERT_EXPR(MDS::mapping_type::swap_counter() > 0);
    }
  }
}

constexpr bool test() {
  using extents_t = std::extents<int, 4, std::dynamic_extent>;
  float data_a[1024];
  float data_b[1024];
  {
    std::mdspan a(data_a, extents_t(12));
    std::mdspan b(data_b, extents_t(5));
    test_swap(a, b);
  }
  {
    layout_wrapping_integral<4>::template mapping<extents_t> map_a(extents_t(12), not_extents_constructible_tag()),
        map_b(extents_t(5), not_extents_constructible_tag());
    std::mdspan a(data_a, map_a);
    std::mdspan b(data_b, map_b);
    test_swap(a, b);
  }
  return true;
}

int tc_containers_views_mdspan_mdspan_swap(void) {
  test();
  static_assert(test());
  return 0;
}
