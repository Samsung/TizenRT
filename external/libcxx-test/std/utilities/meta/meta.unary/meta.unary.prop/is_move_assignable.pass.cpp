//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// type_traits

// is_move_assignable

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_move_assignable()
{
    static_assert(( std::is_move_assignable<T>::value), "");
#if TEST_STD_VER > 14
    static_assert(( std::is_move_assignable_v<T>), "");
#endif
}

template <class T>
void test_is_not_move_assignable()
{
    static_assert((!std::is_move_assignable<T>::value), "");
#if TEST_STD_VER > 14
    static_assert((!std::is_move_assignable_v<T>), "");
#endif
}

class Empty
{
};

class NotEmpty
{
public:
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

struct A
{
    A();
};

int tc_utilities_meta_meta_unary_meta_unary_prop_is_move_assignable(void) {
    test_is_move_assignable<int> ();
    test_is_move_assignable<A> ();
    test_is_move_assignable<bit_zero> ();
    test_is_move_assignable<Union> ();
    test_is_move_assignable<NotEmpty> ();
    test_is_move_assignable<Empty> ();

    test_is_not_move_assignable<const int> ();
    test_is_not_move_assignable<int[]> ();
    test_is_not_move_assignable<int[3]> ();

    test_is_not_move_assignable<void> ();

  return 0;
}
