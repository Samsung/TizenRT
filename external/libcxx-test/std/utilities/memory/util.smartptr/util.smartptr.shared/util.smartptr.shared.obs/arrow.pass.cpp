//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// T* operator->() const;

#include <memory>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow(void) {
    const std::shared_ptr<std::pair<int, int> > p(new std::pair<int, int>(3, 4));
    TC_ASSERT_EXPR(p->first == 3);
    TC_ASSERT_EXPR(p->second == 4);
    p->first = 5;
    p->second = 6;
    TC_ASSERT_EXPR(p->first == 5);
    TC_ASSERT_EXPR(p->second == 6);

  return 0;
}
