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

// template<class T> class shared_ptr
// {
// public:
//     typedef T element_type; // until C++17
//     typedef remove_extent_t<T> element_type; // since C++17
//     typedef weak_ptr<T> weak_type; // C++17
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 14
template <typename T, typename = std::void_t<> >
struct has_less : std::false_type {};

template <typename T>
struct has_less<T,
                std::void_t<decltype(std::declval<T>() < std::declval<T>())> >
    : std::true_type {};
#endif

struct A;  // purposefully incomplete
struct B {
  int x;
  B() = default;
};

template <class T>
void test() {
  ASSERT_SAME_TYPE(typename std::shared_ptr<T>::element_type, T);
#if TEST_STD_VER > 14
  ASSERT_SAME_TYPE(typename std::shared_ptr<T>::weak_type, std::weak_ptr<T>);
  static_assert(std::is_copy_constructible<std::shared_ptr<T> >::value, "");
  static_assert(std::is_copy_assignable<std::shared_ptr<T> >::value, "");
  static_assert(has_less<std::shared_ptr<T> >::value);
  ASSERT_SAME_TYPE(typename std::shared_ptr<T[]>::element_type, T);
  ASSERT_SAME_TYPE(typename std::shared_ptr<T[8]>::element_type, T);
#endif
}

int tc_utilities_memory_util_smartptr_util_smartptr_shared_types(void) {
  test<A>();
  test<B>();
  test<int>();
  test<char*>();

#if TEST_STD_VER > 14
  ASSERT_SAME_TYPE(typename std::shared_ptr<int[][2]>::element_type, int[2]);
#endif

  return 0;
}
