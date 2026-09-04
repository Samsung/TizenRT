//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <memory>

// unique_ptr

// FIXME(EricWF): This test contains tests for constructing a unique_ptr from NULL.
// The behavior demonstrated in this test is not meant to be standard; It simply
// tests the current status quo in libc++.

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

template <class VT>
TEST_CONSTEXPR_CXX23 void test_pointer_ctor() {
  {
    std::unique_ptr<VT> p(0);
    TC_ASSERT_EXPR(p.get() == 0);
  }
  {
    std::unique_ptr<VT, Deleter<VT> > p(0);
    TC_ASSERT_EXPR(p.get() == 0);
    TC_ASSERT_EXPR(p.get_deleter().state() == 0);
  }
}

template <class VT>
TEST_CONSTEXPR_CXX23 void test_pointer_deleter_ctor() {
  {
    std::default_delete<VT> d;
    std::unique_ptr<VT> p(0, d);
    TC_ASSERT_EXPR(p.get() == 0);
  }
  {
    std::unique_ptr<VT, Deleter<VT> > p(0, Deleter<VT>(5));
    TC_ASSERT_EXPR(p.get() == 0);
    TC_ASSERT_EXPR(p.get_deleter().state() == 5);
  }
  {
    NCDeleter<VT> d(5);
    std::unique_ptr<VT, NCDeleter<VT>&> p(0, d);
    TC_ASSERT_EXPR(p.get() == 0);
    TC_ASSERT_EXPR(p.get_deleter().state() == 5);
  }
  {
    NCConstDeleter<VT> d(5);
    std::unique_ptr<VT, NCConstDeleter<VT> const&> p(0, d);
    TC_ASSERT_EXPR(p.get() == 0);
    TC_ASSERT_EXPR(p.get_deleter().state() == 5);
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  {
    // test_pointer_ctor<int>();
    test_pointer_deleter_ctor<int>();
  }
  {
    test_pointer_ctor<int[]>();
    test_pointer_deleter_ctor<int[]>();
  }

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_ctor_null(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
