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

// polymorphic_allocator
// polymorphic_allocator<T>::select_on_container_copy_construction() const

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_select_on_container_copy_construction(void) {
  typedef std::pmr::polymorphic_allocator<void> A;
  {
    A const a;
    ASSERT_SAME_TYPE(decltype(a.select_on_container_copy_construction()), A);
  }
  {
    std::pmr::memory_resource* mptr = (std::pmr::memory_resource*)42;
    A const a(mptr);
    TC_ASSERT_EXPR(a.resource() == mptr);
    A const other = a.select_on_container_copy_construction();
    TC_ASSERT_EXPR(other.resource() == std::pmr::get_default_resource());
    TC_ASSERT_EXPR(a.resource() == mptr);
  }
  {
    std::pmr::memory_resource* mptr = (std::pmr::memory_resource*)42;
    std::pmr::set_default_resource(mptr);
    A const a(nullptr);
    TC_ASSERT_EXPR(a.resource() == nullptr);
    A const other = a.select_on_container_copy_construction();
    TC_ASSERT_EXPR(other.resource() == std::pmr::get_default_resource());
    TC_ASSERT_EXPR(other.resource() == mptr);
    TC_ASSERT_EXPR(a.resource() == nullptr);
  }

  return 0;
}
