//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr(nullptr_t)

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t(void) {
  {
    std::shared_ptr<int> p(nullptr);
    TC_ASSERT_EXPR(p.use_count() == 0);
    TC_ASSERT_EXPR(p.get() == 0);
  }

  {
    std::shared_ptr<int const> p(nullptr);
    TC_ASSERT_EXPR(p.use_count() == 0);
    TC_ASSERT_EXPR(p.get() == 0);
  }

  return 0;
}
