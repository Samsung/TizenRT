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
// <functional>

// reference_wrapper

// template <ObjectType T> reference_wrapper<T> ref(reference_wrapper<T> t);

#include <functional>
#include <cassert>

#include "counting_predicates.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"

bool is5 ( int i ) { return i == 5; }

template <typename T>
bool call_pred ( T pred ) { return pred(5); }

namespace adl {
  struct A {};
  void ref(A) {}
}

TEST_CONSTEXPR_CXX20 bool test()
{
  {
    int i = 0;
    std::reference_wrapper<int> r1 = std::ref(i);
    std::reference_wrapper<int> r2 = std::ref(r1);
    TC_ASSERT_EXPR(&r2.get() == &i);
  }
  {
    adl::A a;
    std::reference_wrapper<adl::A> a1 = std::ref(a);
    std::reference_wrapper<adl::A> a2 = std::ref(a1);
    TC_ASSERT_EXPR(&a2.get() == &a);
  }
  return true;
}

int tc_utilities_function_objects_refwrap_refwrap_helpers_ref_2(void) {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  {
    unary_counting_predicate<bool(*)(int), int> cp(is5);
    TC_ASSERT_EXPR(!cp(6));
    TC_ASSERT_EXPR(cp.count() == 1);
    TC_ASSERT_EXPR(call_pred(cp));
    TC_ASSERT_EXPR(cp.count() == 1);
    TC_ASSERT_EXPR(call_pred(std::ref(cp)));
    TC_ASSERT_EXPR(cp.count() == 2);
  }

  return 0;
}
