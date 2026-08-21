//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>
// UNSUPPORTED: c++03, c++11, c++14

// template<class T> class weak_ptr

// weak_ptr(shared_ptr<T>) -> weak_ptr<T>

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_shared_ptr_deduction(void) {
  std::shared_ptr<A> s(new A);
  auto w = std::weak_ptr(s);
  ASSERT_SAME_TYPE(decltype(w), std::weak_ptr<A>);
  TC_ASSERT_EXPR(!w.expired());
  TC_ASSERT_EXPR(w.use_count() == 1);
  TC_ASSERT_EXPR(w.lock().get() == s.get());

  return 0;
}
