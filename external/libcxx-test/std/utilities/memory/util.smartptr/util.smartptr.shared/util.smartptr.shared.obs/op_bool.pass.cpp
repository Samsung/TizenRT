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
