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
// TESTING void * memory_resource::deallocate(void *, size_t, size_t = max_align)
//
// Concerns:
//  A) 'memory_resource' contains a member 'deallocate' with the required
//     signature, including the default alignment parameter.
//  B) The return type of 'deallocate' is 'void'.
//  C) 'deallocate' is not marked as 'noexcept'.
//  D) Invoking 'deallocate' invokes 'do_deallocate' with the same arguments.

#include <memory_resource>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_mem_res_public_deallocate(void) {
  NullResource R(42);
  auto& P                      = R.getController();
  std::pmr::memory_resource& M = R;
  {
    ASSERT_SAME_TYPE(decltype(M.deallocate(std::declval<int*>(), 0, 0)), void);
    ASSERT_SAME_TYPE(decltype(M.deallocate(std::declval<int*>(), 0)), void);
  }
  {
    static_assert(!noexcept(M.deallocate(std::declval<int*>(), 0, 0)));
    static_assert(!noexcept(M.deallocate(std::declval<int*>(), 0)));
  }
  {
    int s   = 100;
    int a   = 64;
    void* p = reinterpret_cast<void*>(640);
    M.deallocate(p, s, a);
    TC_ASSERT_EXPR(P.dealloc_count == 1);
    TC_ASSERT_EXPR(P.checkDealloc(p, s, a));

    s = 128;
    a = alignof(std::max_align_t);
    p = reinterpret_cast<void*>(12800);
    M.deallocate(p, s);
    TC_ASSERT_EXPR(P.dealloc_count == 2);
    TC_ASSERT_EXPR(P.checkDealloc(p, s, a));
  }

  return 0;
}
