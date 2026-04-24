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

//-----------------------------------------------------------------------------
// TESTING memory_resource * get_default_resource() noexcept;
//         memory_resource * set_default_resource(memory_resource*) noexcept;
//
// Concerns:
//  A) 'get_default_resource()' returns a non-null memory_resource pointer.
//  B) 'get_default_resource()' returns the value set by the last call to
//     'set_default_resource(...)' and 'new_delete_resource()' if no call
//     to 'set_default_resource(...)' has occurred.
//  C) 'set_default_resource(...)' returns the previous value of the default
//     resource.
//  D) 'set_default_resource(T* p)' for a non-null 'p' sets the default resource
//     to be 'p'.
//  E) 'set_default_resource(null)' sets the default resource to
//     'new_delete_resource()'.
//  F) 'get_default_resource' and 'set_default_resource' are noexcept.

#include <memory_resource>
#include <cassert>

#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

using namespace std::pmr;

int tc_utilities_utility_mem_res_mem_res_global_default_resource(void) {
  TestResource R;
  { // Test (A) and (B)
    memory_resource* p = get_default_resource();
    TC_ASSERT_EXPR(p != nullptr);
    TC_ASSERT_EXPR(p == new_delete_resource());
    TC_ASSERT_EXPR(p == get_default_resource());
  }
  { // Test (C) and (D)
    memory_resource* expect = &R;
    memory_resource* old    = set_default_resource(expect);
    TC_ASSERT_EXPR(old != nullptr);
    TC_ASSERT_EXPR(old == new_delete_resource());

    memory_resource* p = get_default_resource();
    TC_ASSERT_EXPR(p != nullptr);
    TC_ASSERT_EXPR(p == expect);
    TC_ASSERT_EXPR(p == get_default_resource());
  }
  { // Test (E)
    memory_resource* old = set_default_resource(nullptr);
    TC_ASSERT_EXPR(old == &R);
    memory_resource* p = get_default_resource();
    TC_ASSERT_EXPR(p != nullptr);
    TC_ASSERT_EXPR(p == new_delete_resource());
    TC_ASSERT_EXPR(p == get_default_resource());
  }
  { // Test (F)
    static_assert(noexcept(get_default_resource()), "get_default_resource() must be noexcept");

    static_assert(noexcept(set_default_resource(nullptr)), "set_default_resource() must be noexcept");
  }

  return 0;
}
