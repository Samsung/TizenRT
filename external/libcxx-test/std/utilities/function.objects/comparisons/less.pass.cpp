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

// less

#include <functional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "pointer_comparison_test_helper.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_comparisons_less(void) {
    typedef std::less<int> F;
    const F f = F();
#if TEST_STD_VER <= 17
    static_assert((std::is_same<int, F::first_argument_type>::value), "" );
    static_assert((std::is_same<int, F::second_argument_type>::value), "" );
    static_assert((std::is_same<bool, F::result_type>::value), "" );
#endif
    TC_ASSERT_EXPR(!f(36, 36));
    TC_ASSERT_EXPR(!f(36, 6));
    TC_ASSERT_EXPR(f(6, 36));
    {
        // test total ordering of int* for less<int*> and less<void>.
        do_pointer_comparison_test<std::less>();
    }
#if TEST_STD_VER > 11
    typedef std::less<> F2;
    const F2 f2 = F2();
    TC_ASSERT_EXPR(!f2(36, 36));
    TC_ASSERT_EXPR(!f2(36, 6));
    TC_ASSERT_EXPR( f2(6, 36));
    TC_ASSERT_EXPR(!f2(36, 6.0));
    TC_ASSERT_EXPR(!f2(36.0, 6));
    TC_ASSERT_EXPR( f2(6, 36.0));
    TC_ASSERT_EXPR( f2(6.0, 36));

    constexpr bool foo = std::less<int> () (36, 36);
    static_assert ( !foo, "" );

    constexpr bool bar = std::less<> () (36.0, 36);
    static_assert ( !bar, "" );
#endif

  return 0;
}
