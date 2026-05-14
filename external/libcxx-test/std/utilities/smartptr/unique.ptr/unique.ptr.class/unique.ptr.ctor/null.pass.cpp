/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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
