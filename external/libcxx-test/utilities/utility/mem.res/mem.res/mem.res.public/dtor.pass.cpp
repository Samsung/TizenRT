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

// test_memory_resource requires RTTI for dynamic_cast
// UNSUPPORTED: no-rtti

// <memory_resource>

//------------------------------------------------------------------------------
// TESTING virtual ~memory_resource()
//
// Concerns:
//  A) 'memory_resource' is destructible.
//  B) The destructor is implicitly marked noexcept.
//  C) The destructor is marked virtual.

#include <memory_resource>
#include <cassert>
#include <type_traits>

#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_mem_res_public_dtor(void) {
  static_assert(std::has_virtual_destructor_v<std::pmr::memory_resource>);
  static_assert(std::is_nothrow_destructible_v<std::pmr::memory_resource>);
  static_assert(std::is_abstract_v<std::pmr::memory_resource>);

  // Check that the destructor of `TestResource` is called when
  // it is deleted as a pointer to `memory_resource`.
  {
    using TR                     = TestResource;
    std::pmr::memory_resource* M = new TR(42);
    TC_ASSERT_EXPR(TR::resource_alive == 1);
    TC_ASSERT_EXPR(TR::resource_constructed == 1);
    TC_ASSERT_EXPR(TR::resource_destructed == 0);

    delete M;

    TC_ASSERT_EXPR(TR::resource_alive == 0);
    TC_ASSERT_EXPR(TR::resource_constructed == 1);
    TC_ASSERT_EXPR(TR::resource_destructed == 1);
  }

  return 0;
}
