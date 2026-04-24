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
// <vector>

// void assign(size_type n, const_reference v);

#include <vector>
#include <algorithm>
#include <cassert>
#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "test_iterators.h"
#if TEST_STD_VER >= 11
#include "emplace_constructible.h"
#include "container_test_types.h"
#include "libcxx_tc_common.h"
#endif


TEST_CONSTEXPR_CXX20 bool test() {
#if TEST_STD_VER >= 11
  int arr1[] = {42};
  int arr2[] = {1, 101, 42};
  {
    using T = EmplaceConstructibleMoveableAndAssignable<int>;
    using It = forward_iterator<int*>;
    {
      std::vector<T> v;
      v.assign(It(arr1), It(std::end(arr1)));
      TC_ASSERT_EXPR(v[0].value == 42);
    }
    {
      std::vector<T> v;
      v.assign(It(arr2), It(std::end(arr2)));
      TC_ASSERT_EXPR(v[0].value == 1);
      TC_ASSERT_EXPR(v[1].value == 101);
      TC_ASSERT_EXPR(v[2].value == 42);
    }
  }
  {
    using T = EmplaceConstructibleMoveableAndAssignable<int>;
    using It = cpp17_input_iterator<int*>;
    {
      std::vector<T> v;
      v.assign(It(arr1), It(std::end(arr1)));
      TC_ASSERT_EXPR(v[0].copied == 0);
      TC_ASSERT_EXPR(v[0].value == 42);
    }
    {
      std::vector<T> v;
      v.assign(It(arr2), It(std::end(arr2)));
      //TC_ASSERT_EXPR(v[0].copied == 0);
      TC_ASSERT_EXPR(v[0].value == 1);
      //TC_ASSERT_EXPR(v[1].copied == 0);
      TC_ASSERT_EXPR(v[1].value == 101);
      TC_ASSERT_EXPR(v[2].copied == 0);
      TC_ASSERT_EXPR(v[2].value == 42);
    }
  }
#endif

  // Test with a number of elements in the source range that is greater than capacity
  {
    typedef forward_iterator<int*> It;

    std::vector<int> dst(10);

    std::size_t n = dst.capacity() * 2;
    std::vector<int> src(n);

    dst.assign(It(src.data()), It(src.data() + src.size()));
    TC_ASSERT_EXPR(dst == src);
  }

  return true;
}

int tc_containers_sequences_vector_vector_cons_assign_iter_iter(void) {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif
  return 0;
}
