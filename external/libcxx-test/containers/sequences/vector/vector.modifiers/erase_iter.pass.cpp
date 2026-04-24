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

// iterator erase(const_iterator position);

#include <vector>
#include <iterator>
#include <cassert>

#include "asan_testing.h"
#include "min_allocator.h"
#include "MoveOnly.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
struct Throws {
  Throws() : v_(0) {}
  Throws(int v) : v_(v) {}
  Throws(const Throws& rhs) : v_(rhs.v_) {
    if (sThrows)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 1;
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  Throws(Throws&& rhs) : v_(rhs.v_) {
    if (sThrows)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 1;
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  Throws& operator=(const Throws& rhs) {
    v_ = rhs.v_;
    return *this;
  }
  Throws& operator=(Throws&& rhs) {
    v_ = rhs.v_;
    return *this;
  }
  int v_;
  static bool sThrows;
};

bool Throws::sThrows = false;
#endif

TEST_CONSTEXPR_CXX20 bool tests() {
  {
    int a1[] = {1, 2, 3, 4, 5};
    std::vector<int> l1(a1, a1 + 5);
    l1.erase(l1.begin());
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    TC_ASSERT_EXPR(l1 == std::vector<int>(a1 + 1, a1 + 5));
  }
  {
    int a1[] = {1, 2, 3, 4, 5};
    int e1[] = {1, 3, 4, 5};
    std::vector<int> l1(a1, a1 + 5);
    l1.erase(l1.begin() + 1);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    TC_ASSERT_EXPR(l1 == std::vector<int>(e1, e1 + 4));
  }
  {
    int a1[] = {1, 2, 3};
    std::vector<int> l1(a1, a1 + 3);
    std::vector<int>::const_iterator i = l1.begin();
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    ++i;
    std::vector<int>::iterator j = l1.erase(i);
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 2);
    TC_ASSERT_EXPR(*j == 3);
    TC_ASSERT_EXPR(*l1.begin() == 1);
    TC_ASSERT_EXPR(*std::next(l1.begin()) == 3);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    j = l1.erase(j);
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 1);
    TC_ASSERT_EXPR(*l1.begin() == 1);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    j = l1.erase(l1.begin());
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 0);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 0);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
  }

  // Make sure vector::erase works with move-only types
  // When non-trivial
  {
    std::vector<MoveOnly> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);
    v.erase(v.begin());
    TC_ASSERT_EXPR(v.size() == 2);
    TC_ASSERT_EXPR(v[0] == MoveOnly(2));
    TC_ASSERT_EXPR(v[1] == MoveOnly(3));
  }
  // When trivial
  {
    std::vector<TrivialMoveOnly> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);
    v.erase(v.begin());
    TC_ASSERT_EXPR(v.size() == 2);
    TC_ASSERT_EXPR(v[0] == TrivialMoveOnly(2));
    TC_ASSERT_EXPR(v[1] == TrivialMoveOnly(3));
  }

#if TEST_STD_VER >= 11
  {
    int a1[] = {1, 2, 3};
    std::vector<int, min_allocator<int>> l1(a1, a1 + 3);
    std::vector<int, min_allocator<int>>::const_iterator i = l1.begin();
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    ++i;
    std::vector<int, min_allocator<int>>::iterator j = l1.erase(i);
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 2);
    TC_ASSERT_EXPR(*j == 3);
    TC_ASSERT_EXPR(*l1.begin() == 1);
    TC_ASSERT_EXPR(*std::next(l1.begin()) == 3);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    j = l1.erase(j);
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 1);
    TC_ASSERT_EXPR(*l1.begin() == 1);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    j = l1.erase(l1.begin());
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 0);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 0);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
  }
#endif

  return true;
}

int tc_containers_sequences_vector_vector_modifiers_erase_iter(void) {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif

#ifndef TEST_HAS_NO_EXCEPTIONS
  // Test for LWG2853:
  // Throws: Nothing unless an exception is thrown by the assignment operator or move assignment operator of T.
  {
    Throws arr[] = {1, 2, 3};
    std::vector<Throws> v(arr, arr + 3);
    Throws::sThrows = true;
    v.erase(v.begin());
    v.erase(--v.end());
    v.erase(v.begin());
    TC_ASSERT_EXPR(v.size() == 0);
  }
#endif

  return 0;
}
