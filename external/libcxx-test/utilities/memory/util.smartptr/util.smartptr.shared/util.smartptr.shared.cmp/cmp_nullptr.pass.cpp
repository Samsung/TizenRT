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

// template <class T>
//     bool operator==(const shared_ptr<T>& x, nullptr_t) noexcept;
// template <class T>
//     bool operator==(nullptr_t, const shared_ptr<T>& y) noexcept;
// template <class T>
//     bool operator!=(const shared_ptr<T>& x, nullptr_t) noexcept;
// template <class T>
//     bool operator!=(nullptr_t, const shared_ptr<T>& y) noexcept;
// template <class T>
//     bool operator<(const shared_ptr<T>& x, nullptr_t) noexcept;
// template <class T>
//     bool operator<(nullptr_t, const shared_ptr<T>& y) noexcept;
// template <class T>
//     bool operator<=(const shared_ptr<T>& x, nullptr_t) noexcept;
// template <class T>
//     bool operator<=(nullptr_t, const shared_ptr<T>& y) noexcept;
// template <class T>
//     bool operator>(const shared_ptr<T>& x, nullptr_t) noexcept;
// template <class T>
//     bool operator>(nullptr_t, const shared_ptr<T>& y) noexcept;
// template <class T>
//     bool operator>=(const shared_ptr<T>& x, nullptr_t) noexcept;
// template <class T>
//     bool operator>=(nullptr_t, const shared_ptr<T>& y) noexcept;
// template<class T>
//     strong_ordering operator<=>(shared_ptr<T> const& x, nullptr_t) noexcept;   // C++20

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "test_comparisons.h"
#include "libcxx_tc_common.h"

void do_nothing(int*) {}

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cmp_cmp_nullptr(void) {
  AssertComparisonsAreNoexcept<std::shared_ptr<int>, nullptr_t>();
  AssertComparisonsAreNoexcept<nullptr_t, std::shared_ptr<int> >();
  AssertComparisonsReturnBool<std::shared_ptr<int>, nullptr_t>();
  AssertComparisonsReturnBool<nullptr_t, std::shared_ptr<int> >();
#if TEST_STD_VER > 17
  AssertOrderAreNoexcept<std::shared_ptr<int>>();
  AssertOrderReturn<std::strong_ordering, std::shared_ptr<int>>();
#endif

  const std::shared_ptr<int> p1(new int(1));
  TC_ASSERT_EXPR(!(p1 == nullptr));
  TC_ASSERT_EXPR(!(nullptr == p1));
  TC_ASSERT_EXPR(!(p1 < nullptr));
  TC_ASSERT_EXPR((nullptr < p1));
  TC_ASSERT_EXPR(!(p1 <= nullptr));
  TC_ASSERT_EXPR((nullptr <= p1));
  TC_ASSERT_EXPR((p1 > nullptr));
  TC_ASSERT_EXPR(!(nullptr > p1));
  TC_ASSERT_EXPR((p1 >= nullptr));
  TC_ASSERT_EXPR(!(nullptr >= p1));
#if TEST_STD_VER > 17
  TC_ASSERT_EXPR((nullptr <=> p1) == std::strong_ordering::less);
  TC_ASSERT_EXPR((p1 <=> nullptr) == std::strong_ordering::greater);
#endif

  const std::shared_ptr<int> p2;
  TC_ASSERT_EXPR((p2 == nullptr));
  TC_ASSERT_EXPR((nullptr == p2));
  TC_ASSERT_EXPR(!(p2 < nullptr));
  TC_ASSERT_EXPR(!(nullptr < p2));
  TC_ASSERT_EXPR((p2 <= nullptr));
  TC_ASSERT_EXPR((nullptr <= p2));
  TC_ASSERT_EXPR(!(p2 > nullptr));
  TC_ASSERT_EXPR(!(nullptr > p2));
  TC_ASSERT_EXPR((p2 >= nullptr));
  TC_ASSERT_EXPR((nullptr >= p2));
#if TEST_STD_VER > 17
  TC_ASSERT_EXPR((nullptr <=> p2) == std::strong_ordering::equivalent);
#endif

  return 0;
}
