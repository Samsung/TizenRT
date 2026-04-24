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
// UNSUPPORTED: c++03, c++11, c++14

// template<class T> class weak_ptr

// weak_ptr(shared_ptr<T>) -> weak_ptr<T>

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_shared_ptr_deduction(void) {
  std::shared_ptr<A> s(new A);
  auto w = std::weak_ptr(s);
  ASSERT_SAME_TYPE(decltype(w), std::weak_ptr<A>);
  TC_ASSERT_EXPR(!w.expired());
  TC_ASSERT_EXPR(w.use_count() == 1);
  TC_ASSERT_EXPR(w.lock().get() == s.get());

  return 0;
}
