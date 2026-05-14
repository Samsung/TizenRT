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
// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// <memory_resource>

// template <class T> class polymorphic_allocator

// template <class U1, class U2>
// void polymorphic_allocator<T>::construct(pair<U1, U2>*)

#include <memory_resource>
#include <cassert>
#include <tuple>
#include <type_traits>
#include <utility>
#include "uses_alloc_types.h"
#include "libcxx_tc_common.h"

int constructed = 0;

template <int>
struct default_constructible {
  default_constructible() : x(42) { ++constructed; }
  int x = 0;
};

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_pair(void) {
  // pair<default_constructible, default_constructible>
  {
    typedef default_constructible<0> T;
    typedef std::pair<T, T> P;
    typedef std::pmr::polymorphic_allocator<void> A;
    alignas(P) char buffer[sizeof(P)];
    P* ptr = reinterpret_cast<P*>(buffer);
    A a;
    constructed = 0;
    a.construct(ptr);
    TC_ASSERT_EXPR(constructed == 2);
    TC_ASSERT_EXPR(ptr->first.x == 42);
    TC_ASSERT_EXPR(ptr->second.x == 42);
  }

  // pair<default_constructible<0>, default_constructible<1>>
  {
    typedef default_constructible<0> T;
    typedef default_constructible<1> U;
    typedef std::pair<T, U> P;
    typedef std::pmr::polymorphic_allocator<void> A;
    alignas(P) char buffer[sizeof(P)];
    P* ptr = reinterpret_cast<P*>(buffer);
    A a;
    constructed = 0;
    a.construct(ptr);
    TC_ASSERT_EXPR(constructed == 2);
    TC_ASSERT_EXPR(ptr->first.x == 42);
    TC_ASSERT_EXPR(ptr->second.x == 42);
  }

  return 0;
}
