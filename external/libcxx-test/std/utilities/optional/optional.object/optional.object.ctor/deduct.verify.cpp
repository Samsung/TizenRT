//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// Test: optional deduction guides

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "../../../libcxx_tc_common.h"

struct A {};

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Test that optional without template args is not default constructible
    constexpr bool opt_no_arg = !std::is_default_constructible<std::optional<>>::value;
    TC_ASSERT_MSG(opt_no_arg, "optional without template arg shall not be default constructible");
    
    // Test that optional cannot be deduced from nullopt_t
    constexpr bool opt_nullopt = !std::is_constructible<std::optional<std::nullopt_t>, std::nullopt_t>::value;
    TC_ASSERT_MSG(opt_nullopt, "optional<nullopt_t> shall not be constructible from nullopt_t");
    
    return true;
}

int tc_utilities_optional_optional_object_ctor_deduct_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
