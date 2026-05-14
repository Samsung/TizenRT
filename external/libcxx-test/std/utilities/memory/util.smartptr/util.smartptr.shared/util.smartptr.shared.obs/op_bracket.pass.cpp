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

// shared_ptr

// element_type& operator[](ptrdiff_t i) const;

#include "test_macros.h"

#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_op_bracket(void) {
  {
    const std::shared_ptr<int[8]> p(new int[8]);

    for (int i = 0; i < 8; ++i)
      p[i] = i;
    for (int i = 0; i < 8; ++i)
      TC_ASSERT_EXPR(p[i] == i);
  }
  {
    int* iptr = new int[8];
    for (int i = 0; i < 8; ++i)
      iptr[i] = i;

    const std::shared_ptr<int[8]> p(iptr);

    for (int i = 0; i < 8; ++i)
      TC_ASSERT_EXPR(p[i] == i);
  }
  {
    const std::shared_ptr<int[]> p(new int[8]);

    for (int i = 0; i < 8; ++i)
      p[i] = i;
    for (int i = 0; i < 8; ++i)
      TC_ASSERT_EXPR(p[i] == i);
  }

  return 0;
}
