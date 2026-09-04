//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// type_traits

// type_identity

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_type_identity()
{
    ASSERT_SAME_TYPE(T, typename std::type_identity<T>::type);
    ASSERT_SAME_TYPE(T,          std::type_identity_t<T>);
}

int tc_utilities_meta_meta_trans_meta_trans_other_type_identity(void) {
    test_type_identity<void>();
    test_type_identity<int>();
    test_type_identity<const volatile int>();
    test_type_identity<int*>();
    test_type_identity<      int[3]>();
    test_type_identity<const int[3]>();

    test_type_identity<void (*)()>();
    test_type_identity<int(int) const>();
    test_type_identity<int(int) volatile>();
    test_type_identity<int(int)  &>();
    test_type_identity<int(int) &&>();

  return 0;
}
