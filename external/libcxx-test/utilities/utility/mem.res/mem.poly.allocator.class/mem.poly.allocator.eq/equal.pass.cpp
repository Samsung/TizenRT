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

// template <class T> class polymorphic_allocator;

// template <class T, class U>
// bool operator==(
//      polymorphic_allocator<T> const &
//    , polymorphic_allocator<U> const &) noexcept

#include <memory_resource>
#include <cassert>
#include <type_traits>

#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_eq_equal(void) {
  typedef std::pmr::polymorphic_allocator<void> A1;
  typedef std::pmr::polymorphic_allocator<int> A2;
  // check return types
  {
    A1 const a1;
    A2 const a2;
    static_assert(std::is_same_v<decltype(a1 == a2), bool>);
    static_assert(noexcept(a1 == a2));
  }
  // equal same type (different resource)
  {
    TestResource d1(1);
    TestResource d2(1);
    A1 const a1(&d1);
    A1 const a2(&d2);

    TC_ASSERT_EXPR(a1 == a2);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(0));

    d1.reset();

    TC_ASSERT_EXPR(a2 == a1);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(0));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(1));
  }
  // equal same type (same resource)
  {
    TestResource d1;
    A1 const a1(&d1);
    A1 const a2(&d1);

    TC_ASSERT_EXPR(a1 == a2);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(a2 == a1);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(0));
  }
  // equal different type (different resource)
  {
    TestResource d1(42);
    TestResource d2(42);
    A1 const a1(&d1);
    A2 const a2(&d2);

    TC_ASSERT_EXPR(a1 == a2);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(a2 == a1);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(1));
  }
  // equal different type (same resource)
  {
    TestResource d1(42);
    A1 const a1(&d1);
    A2 const a2(&d1);

    TC_ASSERT_EXPR(a1 == a2);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(a2 == a1);
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(0));
  }
  // not equal same type
  {
    TestResource d1(1);
    TestResource d2(2);
    A1 const a1(&d1);
    A1 const a2(&d2);

    TC_ASSERT_EXPR(!(a1 == a2));
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(0));

    d1.reset();

    TC_ASSERT_EXPR(!(a2 == a1));
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(0));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(1));
  }
  // not equal different types
  {
    TestResource d1;
    TestResource1 d2;
    A1 const a1(&d1);
    A2 const a2(&d2);

    TC_ASSERT_EXPR(!(a1 == a2));
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(0));

    d1.reset();

    TC_ASSERT_EXPR(!(a2 == a1));
    TC_ASSERT_EXPR(d1.checkIsEqualCalledEq(0));
    TC_ASSERT_EXPR(d2.checkIsEqualCalledEq(1));
  }

  return 0;
}
