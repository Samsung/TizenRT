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
// <functional>

// template <class T>
// struct hash
// {
//     size_t operator()(T val) const;
// };

#include <bitset>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
void
test()
{
    typedef std::bitset<N> T;
    typedef std::hash<T> H;
#if TEST_STD_VER <= 14
    static_assert((std::is_same<typename H::argument_type, T>::value), "" );
    static_assert((std::is_same<typename H::result_type, std::size_t>::value), "" );
#endif
    ASSERT_NOEXCEPT(H()(T()));

    H h;
    T bs(static_cast<unsigned long long>(N));
    const std::size_t result = h(bs);
    LIBCPP_ASSERT(result == N);
    ((void)result); // Prevent unused warning
}

int tc_utilities_template_bitset_bitset_hash_bitset(void) {
    test<0>();
    test<10>();
    test<100>();
    test<1000>();

  return 0;
}
