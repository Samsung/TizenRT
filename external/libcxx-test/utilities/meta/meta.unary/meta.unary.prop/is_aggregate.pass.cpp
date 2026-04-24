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

// <type_traits>

// template <class T> struct is_aggregate;
// template <class T> constexpr bool is_aggregate_v = is_aggregate<T>::value;

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_true()
{
    static_assert( std::is_aggregate<T>::value, "");
    static_assert( std::is_aggregate<const T>::value, "");
    static_assert( std::is_aggregate<volatile T>::value, "");
    static_assert( std::is_aggregate<const volatile T>::value, "");
    static_assert( std::is_aggregate_v<T>, "");
    static_assert( std::is_aggregate_v<const T>, "");
    static_assert( std::is_aggregate_v<volatile T>, "");
    static_assert( std::is_aggregate_v<const volatile T>, "");
}

template <class T>
void test_false()
{
    static_assert(!std::is_aggregate<T>::value, "");
    static_assert(!std::is_aggregate<const T>::value, "");
    static_assert(!std::is_aggregate<volatile T>::value, "");
    static_assert(!std::is_aggregate<const volatile T>::value, "");
    static_assert(!std::is_aggregate_v<T>, "");
    static_assert(!std::is_aggregate_v<const T>, "");
    static_assert(!std::is_aggregate_v<volatile T>, "");
    static_assert(!std::is_aggregate_v<const volatile T>, "");
}

struct Aggregate {};
struct HasCons { HasCons(int); };
struct HasPriv {
  void PreventUnusedPrivateMemberWarning();
private:
  int x;
};
struct Union { int x; void* y; };


int tc_utilities_meta_meta_unary_meta_unary_prop_is_aggregate(void) {
  {
    test_false<void>();
    test_false<int>();
    test_false<void*>();
    test_false<void()>();
    test_false<void() const>();
    test_false<void(Aggregate::*)(int) const>();
    test_false<Aggregate&>();
    test_false<HasCons>();
    test_false<HasPriv>();
  }
  {
    test_true<Aggregate>();
    test_true<Aggregate[]>();
    test_true<Aggregate[42][101]>();
    test_true<Union>();
  }

  return 0;
}
