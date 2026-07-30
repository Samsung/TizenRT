//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// unique_ptr

// The deleter is not called if get() == 0

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class Deleter {
  int state_;

  Deleter(Deleter&);
  Deleter& operator=(Deleter&);

public:
  TEST_CONSTEXPR_CXX23 Deleter() : state_(0) {}

  TEST_CONSTEXPR_CXX23 int state() const { return state_; }

  TEST_CONSTEXPR_CXX23 void operator()(void*) { ++state_; }
};

template <class T>
TEST_CONSTEXPR_CXX23 void test_basic() {
  Deleter d;
  TC_ASSERT_EXPR(d.state() == 0);
  {
    std::unique_ptr<T, Deleter&> p(nullptr, d);
    TC_ASSERT_EXPR(p.get() == nullptr);
    TC_ASSERT_EXPR(&p.get_deleter() == &d);
  }
  TC_ASSERT_EXPR(d.state() == 0);
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic<int>();
  test_basic<int[]>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_dtor_null(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
