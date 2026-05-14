//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// <stack>

// template<class T, three_way_comparable Container>
//   compare_three_way_result_t<Container>
//     operator<=>(const stack<T, Container>& x, const stack<T, Container>& y);

#include <cassert>
#include <deque>
#include <list>
#include <stack>
#include <vector>

#include "nasty_containers.h"
#include "test_container_comparisons.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_stack_compare_three_way(void) {
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::stack, std::deque>()));
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::stack, std::list>()));
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::stack, std::vector>()));
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::stack, nasty_list>()));
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::stack, nasty_vector>()));
  // `std::stack` is not constexpr, so no `static_assert` test here.
  return 0;
}
