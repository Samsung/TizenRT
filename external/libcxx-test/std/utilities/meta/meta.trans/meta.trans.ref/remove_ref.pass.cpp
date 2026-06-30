//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// type_traits

// remove_reference

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_remove_reference()
{
    ASSERT_SAME_TYPE(U, typename std::remove_reference<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U, std::remove_reference_t<T>);
#endif
}

int tc_utilities_meta_meta_trans_meta_trans_ref_remove_ref(void) {
    test_remove_reference<void, void>();
    test_remove_reference<int, int>();
    test_remove_reference<int[3], int[3]>();
    test_remove_reference<int*, int*>();
    test_remove_reference<const int*, const int*>();

    test_remove_reference<int&, int>();
    test_remove_reference<const int&, const int>();
    test_remove_reference<int(&)[3], int[3]>();
    test_remove_reference<int*&, int*>();
    test_remove_reference<const int*&, const int*>();

    test_remove_reference<int&&, int>();
    test_remove_reference<const int&&, const int>();
    test_remove_reference<int(&&)[3], int[3]>();
    test_remove_reference<int*&&, int*>();
    test_remove_reference<const int*&&, const int*>();

  return 0;
}
