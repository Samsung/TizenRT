//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11
#include <memory>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX23 bool test() {
  {
    std::unique_ptr<int> p1 = std::make_unique<int>(1);
    TC_ASSERT_EXPR(*p1 == 1);
    p1 = std::make_unique<int>();
    TC_ASSERT_EXPR(*p1 == 0);
  }

  {
    std::unique_ptr<std::string> p2 = std::make_unique<std::string>("Meow!");
    TC_ASSERT_EXPR(*p2 == "Meow!");
    p2 = std::make_unique<std::string>();
    TC_ASSERT_EXPR(*p2 == "");
    p2 = std::make_unique<std::string>(6, 'z');
    TC_ASSERT_EXPR(*p2 == "zzzzzz");
  }

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_single(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
