//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// bool unique() const;

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_unique(void) {
    const std::shared_ptr<int> p(new int(32));
    TC_ASSERT_EXPR(p.unique());
    {
    std::shared_ptr<int> p2 = p;
    TC_ASSERT_EXPR(!p.unique());
    }
    TC_ASSERT_EXPR(p.unique());

  return 0;
}
