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
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <functional>

// bit_xor

#include <functional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_bitwise_operations_bit_xor(void) {
    {
    typedef std::bit_xor<int> F;
    const F f = F();
#if TEST_STD_VER <= 17
    static_assert((std::is_same<int, F::first_argument_type>::value), "" );
    static_assert((std::is_same<int, F::second_argument_type>::value), "" );
    static_assert((std::is_same<int, F::result_type>::value), "" );
#endif
    TC_ASSERT_EXPR(f(0xEA95, 0xEA95) == 0);
    TC_ASSERT_EXPR(f(0xEA95, 0x58D3) == 0xB246);
    TC_ASSERT_EXPR(f(0x58D3, 0xEA95) == 0xB246);
    TC_ASSERT_EXPR(f(0x58D3, 0) == 0x58D3);
    TC_ASSERT_EXPR(f(0xFFFF, 0x58D3) == 0xA72C);
    }
#if TEST_STD_VER > 11
    {
    typedef std::bit_xor<> F2;
    const F2 f = F2();
    TC_ASSERT_EXPR(f(0xEA95, 0xEA95) == 0);
    TC_ASSERT_EXPR(f(0xEA95L, 0xEA95) == 0);
    TC_ASSERT_EXPR(f(0xEA95, 0xEA95L) == 0);

    TC_ASSERT_EXPR(f(0xEA95, 0x58D3) == 0xB246);
    TC_ASSERT_EXPR(f(0xEA95L, 0x58D3) == 0xB246);
    TC_ASSERT_EXPR(f(0xEA95, 0x58D3L) == 0xB246);

    TC_ASSERT_EXPR(f(0x58D3, 0xEA95) == 0xB246);
    TC_ASSERT_EXPR(f(0x58D3L, 0xEA95) == 0xB246);
    TC_ASSERT_EXPR(f(0x58D3, 0xEA95L) == 0xB246);

    TC_ASSERT_EXPR(f(0x58D3, 0) == 0x58D3);
    TC_ASSERT_EXPR(f(0x58D3L, 0) == 0x58D3);
    TC_ASSERT_EXPR(f(0x58D3, 0L) == 0x58D3);

    TC_ASSERT_EXPR(f(0xFFFF, 0x58D3) == 0xA72C);
    TC_ASSERT_EXPR(f(0xFFFFL, 0x58D3) == 0xA72C);
    TC_ASSERT_EXPR(f(0xFFFF, 0x58D3L) == 0xA72C);

    constexpr int foo = std::bit_xor<int> () (0x58D3, 0xEA95);
    static_assert ( foo == 0xB246, "" );

    constexpr int bar = std::bit_xor<> () (0x58D3L, 0xEA95);
    static_assert ( bar == 0xB246, "" );
    }
#endif

  return 0;
}
