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

// template <class T, class Alloc> struct uses_allocator;

#include <memory>
#include <vector>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
};

struct B
{
    typedef int allocator_type;
};

struct C {
  static int allocator_type;
};

struct D {
  static int allocator_type() { return 0; }
};

struct E {
private:
  typedef int allocator_type;
};

template <bool Expected, class T, class A>
void
test()
{
    static_assert((std::uses_allocator<T, A>::value == Expected), "");
    static_assert(std::is_base_of<std::integral_constant<bool, Expected>, std::uses_allocator<T, A> >::value, "");
#if TEST_STD_VER > 14
    ASSERT_SAME_TYPE(decltype(std::uses_allocator_v<T, A>), const bool);
    static_assert((std::uses_allocator_v<T, A> == Expected), "");
#endif
}

int tc_utilities_memory_allocator_uses_allocator_uses_trait_uses_allocator(void) {
    test<false, int, std::allocator<int> >();
    test<true, std::vector<int>, std::allocator<int> >();
    test<false, A, std::allocator<int> >();
    test<false, B, std::allocator<int> >();
    test<true, B, double>();
    test<false, C, decltype(C::allocator_type)>();
    test<false, D, decltype(D::allocator_type)>();
#if TEST_STD_VER >= 11
    test<false, E, int>();
#endif


//     static_assert((!std::uses_allocator<int, std::allocator<int> >::value), "");
//     static_assert(( std::uses_allocator<std::vector<int>, std::allocator<int> >::value), "");
//     static_assert((!std::uses_allocator<A, std::allocator<int> >::value), "");
//     static_assert((!std::uses_allocator<B, std::allocator<int> >::value), "");
//     static_assert(( std::uses_allocator<B, double>::value), "");
//     static_assert((!std::uses_allocator<C, decltype(C::allocator_type)>::value), "");
//     static_assert((!std::uses_allocator<D, decltype(D::allocator_type)>::value), "");
// #if TEST_STD_VER >= 11
//     static_assert((!std::uses_allocator<E, int>::value), "");
// #endif

  return 0;
}
