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

// template <ObjectType T> reference_wrapper<const T> cref(const T& t);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test()
{
  int i = 0;
  std::reference_wrapper<const int> r = std::cref(i);
  TC_ASSERT_EXPR(&r.get() == &i);
  return true;
}

int tc_utilities_function_objects_refwrap_refwrap_helpers_cref_1(void) {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return 0;
}
