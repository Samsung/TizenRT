//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
