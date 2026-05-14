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
//
// template <class T>
// class allocator
// {
// public: // All of these are constexpr after C++17
//  allocator() noexcept;
//  allocator(const allocator&) noexcept;
//  template<class U> allocator(const allocator<U>&) noexcept;
// ...
// };

#include <memory>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template<class T>
TEST_CONSTEXPR_CXX20 bool test() {
  typedef std::allocator<T> A1;
  typedef std::allocator<long> A2;

  A1 a1;
  A1 a1_copy = a1; (void)a1_copy;
  A2 a2 = a1; (void)a2;

  return true;
}

int tc_utilities_memory_default_allocator_allocator_ctor(void) {
  test<char>();
  test<char const>();
  test<void>();

#if TEST_STD_VER > 17
  static_assert(test<char>());
  static_assert(test<char const>());
  static_assert(test<void>());
#endif
  return 0;
}
