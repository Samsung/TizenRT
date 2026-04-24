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

// divides

#include <functional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_arithmetic_operations_divides(void) {
    typedef std::divides<int> F;
    const F f = F();
#if TEST_STD_VER <= 17
    static_assert((std::is_same<int, F::first_argument_type>::value), "" );
    static_assert((std::is_same<int, F::second_argument_type>::value), "" );
    static_assert((std::is_same<int, F::result_type>::value), "" );
#endif
    TC_ASSERT_EXPR(f(36, 4) == 9);
#if TEST_STD_VER > 11
    typedef std::divides<> F2;
    const F2 f2 = F2();
    TC_ASSERT_EXPR(f2(36, 4) == 9);
    TC_ASSERT_EXPR(f2(36.0, 4) == 9);
    TC_ASSERT_EXPR(f2(18, 4.0) == 4.5); // exact in binary

    constexpr int foo = std::divides<int> () (3, 2);
    static_assert ( foo == 1, "" );

    constexpr double bar = std::divides<> () (3.0, 2);
    static_assert ( bar == 1.5, "" ); // exact in binary
#endif

  return 0;
}
