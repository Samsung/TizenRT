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

// template <class T> class polymorphic_allocator

// polymorphic_allocator<T>::polymorphic_allocator() noexcept

#include <memory_resource>
#include <cassert>
#include <type_traits>

#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_ctor_default(void) {
  {
    static_assert(std::is_nothrow_default_constructible<std::pmr::polymorphic_allocator<void>>::value,
                  "Must me nothrow default constructible");
  }
  {
    // test that the allocator gets its resource from get_default_resource
    TestResource R1(42);
    std::pmr::set_default_resource(&R1);

    typedef std::pmr::polymorphic_allocator<void> A;
    A const a;
    TC_ASSERT_EXPR(a.resource() == &R1);

    std::pmr::set_default_resource(nullptr);
    A const a2;
    TC_ASSERT_EXPR(a.resource() == &R1);
    TC_ASSERT_EXPR(a2.resource() == std::pmr::new_delete_resource());
  }

  return 0;
}
