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
// TESTING virtual bool is_equal(memory_resource const &) const noexcept
//
// Concerns:
//   A) 'memory_resource' provides a function 'is_equal' with the required
//      signature.
//   B) 'is_equal' is noexcept.
//   C) 'do_is_equal' is called using the same arguments passed to 'is_equal'
//      and the resulting value is returned.
//   D) 'do_is_equal' is called on the LHS object and not the RHS object.

#include <memory_resource>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_mem_res_public_is_equal(void) {
  {
    const std::pmr::memory_resource* r1 = nullptr;
    const std::pmr::memory_resource* r2 = nullptr;
    ASSERT_NOEXCEPT(r1->is_equal(*r2));
  }
  {
    TestResource1 R1(1);
    auto& P1                            = R1.getController();
    const std::pmr::memory_resource& M1 = R1;

    TestResource2 R2(1);
    auto& P2                            = R2.getController();
    const std::pmr::memory_resource& M2 = R2;

    TC_ASSERT_EXPR(M1.is_equal(M2) == false);
    TC_ASSERT_EXPR(P1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(P2.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(M2.is_equal(M1) == false);
    TC_ASSERT_EXPR(P2.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(P1.checkIsEqualCalledEq(1));
  }
  {
    TestResource1 R1(1);
    auto& P1                            = R1.getController();
    const std::pmr::memory_resource& M1 = R1;

    TestResource1 R2(2);
    auto& P2                            = R2.getController();
    const std::pmr::memory_resource& M2 = R2;

    TC_ASSERT_EXPR(M1.is_equal(M2) == false);
    TC_ASSERT_EXPR(P1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(P2.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(M2.is_equal(M1) == false);
    TC_ASSERT_EXPR(P2.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(P1.checkIsEqualCalledEq(1));
  }
  {
    TestResource1 R1(1);
    auto& P1                            = R1.getController();
    const std::pmr::memory_resource& M1 = R1;

    TestResource1 R2(1);
    auto& P2                            = R2.getController();
    const std::pmr::memory_resource& M2 = R2;

    TC_ASSERT_EXPR(M1.is_equal(M2) == true);
    TC_ASSERT_EXPR(P1.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(P2.checkIsEqualCalledEq(0));

    TC_ASSERT_EXPR(M2.is_equal(M1) == true);
    TC_ASSERT_EXPR(P2.checkIsEqualCalledEq(1));
    TC_ASSERT_EXPR(P1.checkIsEqualCalledEq(1));
  }

  return 0;
}
