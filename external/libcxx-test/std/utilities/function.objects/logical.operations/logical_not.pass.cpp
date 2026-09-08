//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <functional>

// logical_not

#include <functional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_logical_operations_logical_not(void) {
    typedef std::logical_not<int> F;
    const F f = F();
#if TEST_STD_VER <= 17
    static_assert((std::is_same<F::argument_type, int>::value), "" );
    static_assert((std::is_same<F::result_type, bool>::value), "" );
#endif
    TC_ASSERT_EXPR(!f(36));
    TC_ASSERT_EXPR(f(0));
#if TEST_STD_VER > 11
    typedef std::logical_not<> F2;
    const F2 f2 = F2();
    TC_ASSERT_EXPR(!f2(36));
    TC_ASSERT_EXPR( f2(0));
    TC_ASSERT_EXPR(!f2(36L));
    TC_ASSERT_EXPR( f2(0L));

    constexpr bool foo = std::logical_not<int> () (36);
    static_assert ( !foo, "" );

    constexpr bool bar = std::logical_not<> () (36);
    static_assert ( !bar, "" );
#endif

  return 0;
}
