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
// UNSUPPORTED: c++03

// <vector>

// iterator insert(const_iterator p, initializer_list<value_type> il);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
int throw_if_zero     = 2;
int constructed_count = 0;

struct ThrowSometimes {
  ThrowSometimes() { ++constructed_count; }
  ThrowSometimes(const ThrowSometimes&) {
    if (--throw_if_zero == 0)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 1;
    ++constructed_count;
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  ThrowSometimes& operator=(const ThrowSometimes&) {
    if (--throw_if_zero == 0)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 1;
    ++constructed_count;
    return *this;
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  ~ThrowSometimes() { --constructed_count; }
};

void test_throwing() {
  std::vector<ThrowSometimes> v;
  v.reserve(4);
  v.emplace_back();
  v.emplace_back();
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    v.insert(v.end(), {ThrowSometimes{}, ThrowSometimes{}});
    TC_ASSERT_EXPR(false);
  } catch (int) {
    TC_ASSERT_EXPR(v.size() == 2);
    TC_ASSERT_EXPR(constructed_count == 2);
  }
#endif // _LIBCPP_NO_EXCEPTIONS
}
#endif // TEST_HAS_NO_EXCEPTIONS

TEST_CONSTEXPR_CXX20 bool tests()
{
  {
    std::vector<int> d(10, 1);
    std::vector<int>::iterator i = d.insert(d.cbegin() + 2, {3, 4, 5, 6});
    TC_ASSERT_EXPR(d.size() == 14);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(d));
    TC_ASSERT_EXPR(i == d.begin() + 2);
    TC_ASSERT_EXPR(d[0] == 1);
    TC_ASSERT_EXPR(d[1] == 1);
    TC_ASSERT_EXPR(d[2] == 3);
    TC_ASSERT_EXPR(d[3] == 4);
    TC_ASSERT_EXPR(d[4] == 5);
    TC_ASSERT_EXPR(d[5] == 6);
    TC_ASSERT_EXPR(d[6] == 1);
    TC_ASSERT_EXPR(d[7] == 1);
    TC_ASSERT_EXPR(d[8] == 1);
    TC_ASSERT_EXPR(d[9] == 1);
    TC_ASSERT_EXPR(d[10] == 1);
    TC_ASSERT_EXPR(d[11] == 1);
    TC_ASSERT_EXPR(d[12] == 1);
    TC_ASSERT_EXPR(d[13] == 1);
  }
  {
    std::vector<int, min_allocator<int>> d(10, 1);
    std::vector<int, min_allocator<int>>::iterator i = d.insert(d.cbegin() + 2, {3, 4, 5, 6});
    TC_ASSERT_EXPR(d.size() == 14);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(d));
    TC_ASSERT_EXPR(i == d.begin() + 2);
    TC_ASSERT_EXPR(d[0] == 1);
    TC_ASSERT_EXPR(d[1] == 1);
    TC_ASSERT_EXPR(d[2] == 3);
    TC_ASSERT_EXPR(d[3] == 4);
    TC_ASSERT_EXPR(d[4] == 5);
    TC_ASSERT_EXPR(d[5] == 6);
    TC_ASSERT_EXPR(d[6] == 1);
    TC_ASSERT_EXPR(d[7] == 1);
    TC_ASSERT_EXPR(d[8] == 1);
    TC_ASSERT_EXPR(d[9] == 1);
    TC_ASSERT_EXPR(d[10] == 1);
    TC_ASSERT_EXPR(d[11] == 1);
    TC_ASSERT_EXPR(d[12] == 1);
    TC_ASSERT_EXPR(d[13] == 1);
  }

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_insert_iter_initializer_list(void) {
#ifndef TEST_HAS_NO_EXCEPTIONS
  test_throwing();
#endif
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return 0;
}
