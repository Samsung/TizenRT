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

// rank

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, unsigned A>
void test_rank()
{
    static_assert( std::rank<T>::value == A, "");
    static_assert( std::rank<const T>::value == A, "");
    static_assert( std::rank<volatile T>::value == A, "");
    static_assert( std::rank<const volatile T>::value == A, "");
#if TEST_STD_VER > 14
    static_assert( std::rank_v<T> == A, "");
    static_assert( std::rank_v<const T> == A, "");
    static_assert( std::rank_v<volatile T> == A, "");
    static_assert( std::rank_v<const volatile T> == A, "");
#endif
}

class Class
{
public:
    ~Class();
};

int tc_utilities_meta_meta_unary_prop_query_rank(void) {
    test_rank<void, 0>();
    test_rank<int&, 0>();
    test_rank<Class, 0>();
    test_rank<int*, 0>();
    test_rank<const int*, 0>();
    test_rank<int, 0>();
    test_rank<double, 0>();
    test_rank<bool, 0>();
    test_rank<unsigned, 0>();

    test_rank<char[3], 1>();
    test_rank<char[][3], 2>();
    test_rank<char[][4][3], 3>();

  return 0;
}
