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
// This code triggers https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104568
// UNSUPPORTED: msvc

// Test the fix for https://llvm.org/PR54100

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {
  int m[0];
};
static_assert(sizeof(A) == 0, "");  // an extension supported by GCC and Clang

int tc_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_sizezero(void) {
  {
    std::unique_ptr<A> p = std::unique_ptr<A>(new A);
    TC_ASSERT_EXPR(p != nullptr);
  }
  {
    std::unique_ptr<A[]> p = std::unique_ptr<A[]>(new A[1]);
    TC_ASSERT_EXPR(p != nullptr);
  }
#if TEST_STD_VER > 11
  {
    std::unique_ptr<A> p = std::make_unique<A>();
    TC_ASSERT_EXPR(p != nullptr);
  }
  {
    std::unique_ptr<A[]> p = std::make_unique<A[]>(1);
    TC_ASSERT_EXPR(p != nullptr);
  }
#endif

  return 0;
}
