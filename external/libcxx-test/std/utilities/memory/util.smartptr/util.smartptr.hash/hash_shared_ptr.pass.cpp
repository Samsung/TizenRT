//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template <class T>
// struct hash<shared_ptr<T>>
// {
//     typedef shared_ptr<T>    argument_type;
//     typedef size_t           result_type;
//     size_t operator()(const shared_ptr<T>& p) const;
// };

#include <memory>

#include <cassert>
#include <functional>

#include "test_macros.h"

#if TEST_STD_VER >= 11
#include "poisoned_hash_helper.h"
#include "libcxx_tc_common.h"

struct A {};
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_hash_hash_shared_ptr(void) {
  {
    int* ptr = new int;
    std::shared_ptr<int> p(ptr);
    std::hash<std::shared_ptr<int> > f;
    std::size_t h = f(p);
    TC_ASSERT_EXPR(h == std::hash<int*>()(ptr));
  }
#if TEST_STD_VER >= 11
  {
    test_hash_enabled_for_type<std::shared_ptr<int>>();
    test_hash_enabled_for_type<std::shared_ptr<A>>();
  }
#endif

  return 0;
}
