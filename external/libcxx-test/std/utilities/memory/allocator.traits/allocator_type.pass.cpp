//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template <class Alloc>
// struct allocator_traits
// {
//     typedef Alloc allocator_type;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
struct A
{
    typedef T value_type;
};

int tc_utilities_memory_allocator_traits_allocator_type(void) {
    static_assert((std::is_same<std::allocator_traits<A<char> >::allocator_type, A<char> >::value), "");

  return 0;
}
