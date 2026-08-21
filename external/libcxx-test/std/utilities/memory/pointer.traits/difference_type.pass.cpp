//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template <class T>
// struct pointer_traits<T*>
// {
//     typedef ptrdiff_t difference_type;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_pointer_traits_difference_type(void) {
    static_assert((std::is_same<std::pointer_traits<double*>::difference_type, std::ptrdiff_t>::value), "");

  return 0;
}
