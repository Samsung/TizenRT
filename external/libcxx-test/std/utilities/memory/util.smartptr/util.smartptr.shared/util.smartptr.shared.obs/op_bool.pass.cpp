//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// explicit operator bool() const;

#include <memory>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {
  int a;
  virtual ~A(){};
};
struct B : A {};

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_op_bool(void) {
    static_assert(std::is_constructible<bool, std::shared_ptr<A> >::value, "");
    static_assert(!std::is_convertible<std::shared_ptr<A>, bool>::value, "");

    {
      const std::shared_ptr<int> p(new int(32));
      TC_ASSERT_EXPR(p);
    }
    {
      const std::shared_ptr<int> p;
      TC_ASSERT_EXPR(!p);
    }
#if !defined(TEST_HAS_NO_RTTI)
    {
      std::shared_ptr<A> basePtr = std::make_shared<B>();
      std::shared_ptr<B> sp = std::dynamic_pointer_cast<B>(basePtr);
      TC_ASSERT_EXPR(sp);
    }
#endif

    return 0;
}
