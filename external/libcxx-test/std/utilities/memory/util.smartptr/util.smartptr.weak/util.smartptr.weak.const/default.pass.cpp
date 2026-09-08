//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template<class T> class weak_ptr

// weak_ptr();

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A;

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_default(void) {
    std::weak_ptr<A> p;
    TC_ASSERT_EXPR(p.use_count() == 0);

  return 0;
}
