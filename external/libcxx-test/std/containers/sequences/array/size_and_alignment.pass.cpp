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
// <array>

// template <class T, size_t N>
// struct array

// Test the size and alignment matches that of an array of a given type.

// Ignore error about requesting a large alignment not being ABI compatible with older AIX systems.
#if defined(_AIX)
# pragma clang diagnostic ignored "-Waix-compat"
#endif

#include <array>
#include <iterator>
#include <type_traits>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, std::size_t Size>
struct MyArray {
  T elems[Size];
};

template <class T, std::size_t Size>
void test() {
  typedef T CArrayT[Size == 0 ? 1 : Size];
  typedef std::array<T, Size> ArrayT;
  typedef MyArray<T, Size == 0 ? 1 : Size> MyArrayT;
  static_assert(sizeof(ArrayT) == sizeof(CArrayT), "");
  static_assert(sizeof(ArrayT) == sizeof(MyArrayT), "");
  static_assert(TEST_ALIGNOF(ArrayT) == TEST_ALIGNOF(MyArrayT), "");
}

template <class T>
void test_type() {
  test<T, 1>();
  test<T, 42>();
  test<T, 0>();
}

#if TEST_STD_VER >= 11
struct alignas(alignof(std::max_align_t) * 2) TestType1 {

};

struct alignas(alignof(std::max_align_t) * 2) TestType2 {
  char data[1000];
};

struct alignas(alignof(std::max_align_t)) TestType3 {
  char data[1000];
};
#endif

int tc_containers_sequences_array_size_and_alignment(void) {
  test_type<char>();
  test_type<int>();
  test_type<double>();
  test_type<long double>();

#if TEST_STD_VER >= 11
  test_type<std::max_align_t>();
  test_type<TestType1>();
  test_type<TestType2>();
  test_type<TestType3>();
#endif

  return 0;
}
