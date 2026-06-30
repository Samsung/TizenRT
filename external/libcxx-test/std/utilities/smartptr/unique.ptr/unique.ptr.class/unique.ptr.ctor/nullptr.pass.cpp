//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// unique_ptr

// constexpr unique_ptr(nullptr_t);  // constexpr since C++23

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 11
TEST_CONSTINIT std::unique_ptr<int> global_static_unique_ptr_single(nullptr);
TEST_CONSTINIT std::unique_ptr<int[]> global_static_unique_ptr_runtime(nullptr);

struct NonDefaultDeleter {
  NonDefaultDeleter() = delete;
  void operator()(void*) const {}
};
#endif

template <class VT>
TEST_CONSTEXPR_CXX23 void test_basic() {
#if TEST_STD_VER >= 11
  {
    using U1 = std::unique_ptr<VT>;
    using U2 = std::unique_ptr<VT, Deleter<VT> >;
    static_assert(std::is_nothrow_constructible<U1, decltype(nullptr)>::value, "");
    static_assert(std::is_nothrow_constructible<U2, decltype(nullptr)>::value, "");
  }
#endif
  {
    std::unique_ptr<VT> p(nullptr);
    TC_ASSERT_EXPR(p.get() == 0);
  }
  {
    std::unique_ptr<VT, NCDeleter<VT> > p(nullptr);
    TC_ASSERT_EXPR(p.get() == 0);
    TC_ASSERT_EXPR(p.get_deleter().state() == 0);
  }
  {
    std::unique_ptr<VT, DefaultCtorDeleter<VT> > p(nullptr);
    TC_ASSERT_EXPR(p.get() == 0);
    TC_ASSERT_EXPR(p.get_deleter().state() == 0);
  }
}

template <class VT>
TEST_CONSTEXPR_CXX23 void test_sfinae() {
#if TEST_STD_VER >= 11
  { // the constructor does not participate in overload resolution when
    // the deleter is a pointer type
    using U = std::unique_ptr<VT, void (*)(void*)>;
    static_assert(!std::is_constructible<U, decltype(nullptr)>::value, "");
  }
  { // the constructor does not participate in overload resolution when
    // the deleter is not default constructible
    using Del = CDeleter<VT>;
    using U1  = std::unique_ptr<VT, NonDefaultDeleter>;
    using U2  = std::unique_ptr<VT, Del&>;
    using U3  = std::unique_ptr<VT, Del const&>;
    static_assert(!std::is_constructible<U1, decltype(nullptr)>::value, "");
    static_assert(!std::is_constructible<U2, decltype(nullptr)>::value, "");
    static_assert(!std::is_constructible<U3, decltype(nullptr)>::value, "");
  }
#endif
}

DEFINE_AND_RUN_IS_INCOMPLETE_TEST({
  { doIncompleteTypeTest(0, nullptr); }
  checkNumIncompleteTypeAlive(0);
  { doIncompleteTypeTest<IncompleteType, NCDeleter<IncompleteType> >(0, nullptr); }
  checkNumIncompleteTypeAlive(0);
  { doIncompleteTypeTest<IncompleteType[]>(0, nullptr); }
  checkNumIncompleteTypeAlive(0);
  { doIncompleteTypeTest<IncompleteType[], NCDeleter<IncompleteType[]> >(0, nullptr); }
  checkNumIncompleteTypeAlive(0);
})

TEST_CONSTEXPR_CXX23 bool test() {
  {
    test_basic<int>();
    test_sfinae<int>();
  }
  {
    test_basic<int[]>();
    test_sfinae<int[]>();
  }

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_ctor_nullptr(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
