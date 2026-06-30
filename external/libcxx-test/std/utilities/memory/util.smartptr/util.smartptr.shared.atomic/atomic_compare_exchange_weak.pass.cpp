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
// bool
// atomic_compare_exchange_weak(shared_ptr<T>* p, shared_ptr<T>* v,
//                              shared_ptr<T> w);

// UNSUPPORTED: c++03

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_compare_exchange_weak(void) {
    {
        std::shared_ptr<int> p(new int(4));
        std::shared_ptr<int> v(new int(3));
        std::shared_ptr<int> w(new int(2));
        bool b = std::atomic_compare_exchange_weak(&p, &v, w);
        TC_ASSERT_EXPR(b == false);
        TC_ASSERT_EXPR(*p == 4);
        TC_ASSERT_EXPR(*v == 4);
        TC_ASSERT_EXPR(*w == 2);
    }
    {
        std::shared_ptr<int> p(new int(4));
        std::shared_ptr<int> v = p;
        std::shared_ptr<int> w(new int(2));
        bool b = std::atomic_compare_exchange_weak(&p, &v, w);
        TC_ASSERT_EXPR(b == true);
        TC_ASSERT_EXPR(*p == 2);
        TC_ASSERT_EXPR(*v == 4);
        TC_ASSERT_EXPR(*w == 2);
    }

  return 0;
}
