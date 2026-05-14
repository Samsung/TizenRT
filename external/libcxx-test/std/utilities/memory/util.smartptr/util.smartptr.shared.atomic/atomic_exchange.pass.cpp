//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <memory>

// shared_ptr

// template <class T>
// shared_ptr<T>
// atomic_exchange(shared_ptr<T>* p, shared_ptr<T> r)

// UNSUPPORTED: c++03

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_exchange(void) {
    {
        std::shared_ptr<int> p(new int(4));
        std::shared_ptr<int> r(new int(3));
        r = std::atomic_exchange(&p, r);
        TC_ASSERT_EXPR(*p == 3);
        TC_ASSERT_EXPR(*r == 4);
    }

  return 0;
}
