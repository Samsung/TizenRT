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

// template<class T, class U> bool operator==(const shared_ptr<T>& a, const shared_ptr<U>& b);
// template<class T, class U> bool operator!=(const shared_ptr<T>& a, const shared_ptr<U>& b);
// template<class T, class U> bool operator< (const shared_ptr<T>& a, const shared_ptr<U>& b);
// template<class T, class U> bool operator<=(const shared_ptr<T>& a, const shared_ptr<U>& b);
// template<class T, class U> bool operator> (const shared_ptr<T>& a, const shared_ptr<U>& b);
// template<class T, class U> bool operator>=(const shared_ptr<T>& a, const shared_ptr<U>& b);
// template<class T1, class D1, class T2, class D2>
//   requires three_way_comparable_with<typename unique_ptr<T1, D1>::pointer,
//                                      typename unique_ptr<T2, D2>::pointer>
//   compare_three_way_result_t<typename unique_ptr<T1, D1>::pointer,
//                              typename unique_ptr<T2, D2>::pointer>
//     operator<=>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "test_comparisons.h"
#include "libcxx_tc_common.h"

void do_nothing(int*) {}

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cmp_cmp(void) {
  AssertComparisonsAreNoexcept<std::shared_ptr<int> >();
  AssertComparisonsReturnBool<std::shared_ptr<int> >();
#if TEST_STD_VER > 17
  AssertOrderAreNoexcept<std::shared_ptr<int>>();
  AssertOrderReturn<std::strong_ordering, std::shared_ptr<int>>();
#endif

  int* ptr1(new int);
  int* ptr2(new int);
  const std::shared_ptr<int> p1(ptr1);
  const std::shared_ptr<int> p2(ptr2);

  TC_ASSERT_EXPR(!(p1 == p2));
  TC_ASSERT_EXPR(p1 != p2);
  TC_ASSERT_EXPR((p1 < p2) == (ptr1 < ptr2));
  TC_ASSERT_EXPR((p1 <= p2) == (ptr1 <= ptr2));
  TC_ASSERT_EXPR((p1 > p2) == (ptr1 > ptr2));
  TC_ASSERT_EXPR((p1 >= p2) == (ptr1 >= ptr2));
#if TEST_STD_VER > 17
  TC_ASSERT_EXPR((p1 <=> p2) != std::strong_ordering::equal);
  TC_ASSERT_EXPR((p1 <=> p2) == (ptr1 <=> ptr2));
#endif

  // The deleter does not influence the comparisons
  // of the `shared_ptr`
  const std::shared_ptr<int> p3(ptr2, do_nothing);
  TC_ASSERT_EXPR(p2 == p3);
  TC_ASSERT_EXPR(!(p1 == p3));
  TC_ASSERT_EXPR(!(p2 != p3));
  TC_ASSERT_EXPR(p1 != p3);
  TC_ASSERT_EXPR((p1 < p3) == (ptr1 < ptr2));
  TC_ASSERT_EXPR((p1 <= p3) == (ptr1 <= ptr2));
  TC_ASSERT_EXPR((p1 > p3) == (ptr1 > ptr2));
  TC_ASSERT_EXPR((p1 >= p3) == (ptr1 >= ptr2));
#if TEST_STD_VER > 17
  TC_ASSERT_EXPR((p2 <=> p3) == std::strong_ordering::equal);
  TC_ASSERT_EXPR((p1 <=> p3) != std::strong_ordering::equal);
  TC_ASSERT_EXPR((p1 <=> p3) == (ptr1 <=> ptr2));
#endif

  return 0;
}
