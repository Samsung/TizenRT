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

// <memory_resource>

// template <class T> class polymorphic_allocator

// polymorphic_allocator<T>::polymorphic_allocator(polymorphic_allocator const &);

#include <memory_resource>
#include <cassert>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_ctor_copy(void) {
  typedef std::pmr::polymorphic_allocator<void> A1;
  {
    static_assert(std::is_copy_constructible<A1>::value, "");
    static_assert(std::is_move_constructible<A1>::value, "");
  }
  // copy
  {
    A1 const a((std::pmr::memory_resource*)42);
    A1 const a2(a);
    TC_ASSERT_EXPR(a.resource() == a2.resource());
  }
  // move
  {
    A1 a((std::pmr::memory_resource*)42);
    A1 a2(std::move(a));
    TC_ASSERT_EXPR(a.resource() == a2.resource());
    TC_ASSERT_EXPR(a2.resource() == (std::pmr::memory_resource*)42);
  }

  return 0;
}
