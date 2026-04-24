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

// bool operator!=(memory_resource const &, memory_resource const &) noexcept;

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_mem_res_eq_not_equal(void) {
  // check return types
  {
    const std::pmr::memory_resource* mr1 = nullptr;
    const std::pmr::memory_resource* mr2 = nullptr;
    ASSERT_SAME_TYPE(decltype(*mr1 != *mr2), bool);
    ASSERT_NOEXCEPT(*mr1 != *mr2);
  }
  // not equal
  {
    TestResource r1(1);
    TestResource r2(2);
    const std::pmr::memory_resource& mr1 = r1;
    const std::pmr::memory_resource& mr2 = r2;

    TC_ASSERT_EXPR(mr1 != mr2);
    TC_ASSERT_EXPR(r1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(r2.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(mr2 != mr1);
    TC_ASSERT_EXPR(r1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(r2.checkIsEqualCalledEq(1));
  }
  // equal
  {
    TestResource r1(1);
    TestResource r2(1);
    const std::pmr::memory_resource& mr1 = r1;
    const std::pmr::memory_resource& mr2 = r2;

    TC_ASSERT_EXPR(!(mr1 != mr2));
    TC_ASSERT_EXPR(r1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(r2.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(!(mr2 != mr1));
    TC_ASSERT_EXPR(r1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(r2.checkIsEqualCalledEq(1));
  }
  // equal same object
  {
    TestResource r1(1);
    const std::pmr::memory_resource& mr1 = r1;
    const std::pmr::memory_resource& mr2 = r1;

    TC_ASSERT_EXPR(!(mr1 != mr2));
    TC_ASSERT_EXPR(r1.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(!(mr2 != mr1));
    TC_ASSERT_EXPR(r1.checkIsEqualCalledEq(0));
  }

  return 0;
}
