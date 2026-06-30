//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// default_delete

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX23 bool test() {
  std::default_delete<A> d;
  A* p = new A;
  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 1);

  d(p);

  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 0);

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_default(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
