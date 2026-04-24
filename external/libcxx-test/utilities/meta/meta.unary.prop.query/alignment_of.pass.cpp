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
// type_traits

// alignment_of

#include <type_traits>
#include <cstdint>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, unsigned A>
void test_alignment_of()
{
    const unsigned AlignofResult = TEST_ALIGNOF(T);
    static_assert( AlignofResult == A, "Golden value does not match result of alignof keyword");
    static_assert( std::alignment_of<T>::value == AlignofResult, "");
    static_assert( std::alignment_of<T>::value == A, "");
    static_assert( std::alignment_of<const T>::value == A, "");
    static_assert( std::alignment_of<volatile T>::value == A, "");
    static_assert( std::alignment_of<const volatile T>::value == A, "");
#if TEST_STD_VER > 14
    static_assert( std::alignment_of_v<T> == A, "");
    static_assert( std::alignment_of_v<const T> == A, "");
    static_assert( std::alignment_of_v<volatile T> == A, "");
    static_assert( std::alignment_of_v<const volatile T> == A, "");
#endif
}

class Class
{
public:
    ~Class();
};

int tc_utilities_meta_meta_unary_prop_query_alignment_of(void) {
    test_alignment_of<int&, 4>();
    test_alignment_of<Class, 1>();
    test_alignment_of<int*, sizeof(std::intptr_t)>();
    test_alignment_of<const int*, sizeof(std::intptr_t)>();
    test_alignment_of<char[3], 1>();
    test_alignment_of<int, 4>();
    test_alignment_of<double, TEST_ALIGNOF(double)>();
    test_alignment_of<bool, 1>();
    test_alignment_of<unsigned, 4>();

  return 0;
}
