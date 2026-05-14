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
// UNSUPPORTED: c++03, c++11
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <functional>

// bit_not

#include <functional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_bitwise_operations_bit_not(void) {
    typedef std::bit_not<int> F;
    const F f = F();
#if TEST_STD_VER <= 17
    static_assert((std::is_same<F::argument_type, int>::value), "" );
    static_assert((std::is_same<F::result_type, int>::value), "" );
#endif
    TC_ASSERT_EXPR((f(0xEA95) & 0xFFFF ) == 0x156A);
    TC_ASSERT_EXPR((f(0x58D3) & 0xFFFF ) == 0xA72C);
    TC_ASSERT_EXPR((f(0)      & 0xFFFF ) == 0xFFFF);
    TC_ASSERT_EXPR((f(0xFFFF) & 0xFFFF ) == 0);

    typedef std::bit_not<> F2;
    const F2 f2 = F2();
    TC_ASSERT_EXPR((f2(0xEA95)  & 0xFFFF ) == 0x156A);
    TC_ASSERT_EXPR((f2(0xEA95L) & 0xFFFF ) == 0x156A);
    TC_ASSERT_EXPR((f2(0x58D3)  & 0xFFFF ) == 0xA72C);
    TC_ASSERT_EXPR((f2(0x58D3L) & 0xFFFF ) == 0xA72C);
    TC_ASSERT_EXPR((f2(0)       & 0xFFFF ) == 0xFFFF);
    TC_ASSERT_EXPR((f2(0L)      & 0xFFFF ) == 0xFFFF);
    TC_ASSERT_EXPR((f2(0xFFFF)  & 0xFFFF ) == 0);
    TC_ASSERT_EXPR((f2(0xFFFFL)  & 0xFFFF ) == 0);

    constexpr int foo = std::bit_not<int> () (0xEA95) & 0xFFFF;
    static_assert ( foo == 0x156A, "" );

    constexpr int bar = std::bit_not<> () (0xEA95) & 0xFFFF;
    static_assert ( bar == 0x156A, "" );

  return 0;
}
