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
// <array>

// tuple_size<array<T, N> >::value

#include <array>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, std::size_t N>
void test()
{
    {
    typedef std::array<T, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
    }
    {
    typedef std::array<T const, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
    }
    {
    typedef std::array<T volatile, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
    }
    {
    typedef std::array<T const volatile, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
    }
}

int tc_containers_sequences_array_array_tuple_tuple_size(void) {
    test<double, 0>();
    test<double, 3>();
    test<double, 5>();

  return 0;
}
