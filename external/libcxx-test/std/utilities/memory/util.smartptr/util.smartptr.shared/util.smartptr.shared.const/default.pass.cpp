//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr();

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};

template <class T>
void test() {
  std::shared_ptr<T> p;
  TC_ASSERT_EXPR(p.use_count() == 0);
  TC_ASSERT_EXPR(p.get() == 0);
}

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default(void) {
  test<int>();
  test<int const>();
  test<A>();
  test<A const>();
  test<int*>();
  test<int const*>();
  test<int[]>();
  test<int[8]>();

  return 0;
}
