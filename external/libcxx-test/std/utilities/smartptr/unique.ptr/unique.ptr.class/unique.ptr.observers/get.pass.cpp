//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// unique_ptr

// test get

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, int[], int>::type VT;
  typedef const VT CVT;
  {
    typedef std::unique_ptr<VT> U;
    int* p = newValue<VT>(1);
    U s(p);
    U const& sc = s;
    ASSERT_SAME_TYPE(decltype(s.get()), int*);
    ASSERT_SAME_TYPE(decltype(sc.get()), int*);
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(sc.get() == s.get());
  }
  {
    typedef std::unique_ptr<CVT> U;
    const int* p = newValue<VT>(1);
    U s(p);
    U const& sc = s;
    ASSERT_SAME_TYPE(decltype(s.get()), const int*);
    ASSERT_SAME_TYPE(decltype(sc.get()), const int*);
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(sc.get() == s.get());
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
