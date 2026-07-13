//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <variant>

// Test: variant cannot contain void types

#include <variant>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify variant cannot be instantiated with void types
    constexpr bool var_void = !std::is_constructible<std::variant<int, void>>::value;
    constexpr bool var_cvoid = !std::is_constructible<std::variant<int, const void>>::value;
    constexpr bool var_cvvoid = !std::is_constructible<std::variant<const volatile void, int>>::value;
    TC_ASSERT_MSG(var_void, "variant<int, void> shall not be constructible");
    TC_ASSERT_MSG(var_cvoid, "variant<int, const void> shall not be constructible");
    TC_ASSERT_MSG(var_cvvoid, "variant<const volatile void, int> shall not be constructible");
    
    return true;
}

int tc_utilities_variant_variant_variant_void_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
