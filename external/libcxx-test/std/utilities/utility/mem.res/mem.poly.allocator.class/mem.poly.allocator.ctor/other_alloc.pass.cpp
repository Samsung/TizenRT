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

// template <class U>
// polymorphic_allocator<T>::polymorphic_allocator(polymorphic_allocator<U> const &);

#include <memory_resource>
#include <cassert>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_ctor_other_alloc(void) {
  typedef std::pmr::polymorphic_allocator<void> A1;
  typedef std::pmr::polymorphic_allocator<char> A2;
  { // Test that the conversion is implicit and noexcept.
    static_assert(std::is_convertible<A1 const&, A2>::value, "");
    static_assert(std::is_convertible<A2 const&, A1>::value, "");
    static_assert(std::is_nothrow_constructible<A1, A2 const&>::value, "");
    static_assert(std::is_nothrow_constructible<A2, A1 const&>::value, "");
  }
  // copy other type
  {
    A1 const a((std::pmr::memory_resource*)42);
    A2 const a2(a);
    TC_ASSERT_EXPR(a.resource() == a2.resource());
    TC_ASSERT_EXPR(a2.resource() == (std::pmr::memory_resource*)42);
  }
  {
    A1 a((std::pmr::memory_resource*)42);
    A2 const a2(std::move(a));
    TC_ASSERT_EXPR(a.resource() == a2.resource());
    TC_ASSERT_EXPR(a2.resource() == (std::pmr::memory_resource*)42);
  }

  return 0;
}
