//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// template <class U> bool owner_before(shared_ptr<U> const& b) const noexcept;

#include <memory>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_owner_before_shared_ptr(void) {
    const std::shared_ptr<int> p1(new int);
    const std::shared_ptr<int> p2 = p1;
    const std::shared_ptr<int> p3(new int);
    TC_ASSERT_EXPR(!p1.owner_before(p2));
    TC_ASSERT_EXPR(!p2.owner_before(p1));
    TC_ASSERT_EXPR(p1.owner_before(p3) || p3.owner_before(p1));
    TC_ASSERT_EXPR(p3.owner_before(p1) == p3.owner_before(p2));
    ASSERT_NOEXCEPT(p1.owner_before(p2));

  return 0;
}
