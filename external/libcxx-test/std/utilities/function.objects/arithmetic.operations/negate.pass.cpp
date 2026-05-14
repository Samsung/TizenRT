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

// negate

#include <functional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_arithmetic_operations_negate(void) {
    typedef std::negate<int> F;
    const F f = F();
#if TEST_STD_VER <= 17
    static_assert((std::is_same<F::argument_type, int>::value), "" );
    static_assert((std::is_same<F::result_type, int>::value), "" );
#endif
    TC_ASSERT_EXPR(f(36) == -36);
#if TEST_STD_VER > 11
    typedef std::negate<> F2;
    const F2 f2 = F2();
    TC_ASSERT_EXPR(f2(36) == -36);
    TC_ASSERT_EXPR(f2(36L) == -36);
    TC_ASSERT_EXPR(f2(36.0) == -36);

    constexpr int foo = std::negate<int> () (3);
    static_assert ( foo == -3, "" );

    constexpr double bar = std::negate<> () (3.0);
    static_assert ( bar == -3.0, "" );
#endif

  return 0;
}
