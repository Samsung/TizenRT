//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// Test: optional instantiation with nullopt_t is ill-formed

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify that optional cannot be instantiated with nullopt_t
    constexpr bool opt_nullopt = !std::is_constructible<std::optional<std::nullopt_t>>::value;
    constexpr bool opt_c_nullopt = !std::is_constructible<std::optional<const std::nullopt_t>>::value;
    constexpr bool opt_v_nullopt = !std::is_constructible<std::optional<volatile std::nullopt_t>>::value;
    constexpr bool opt_cv_nullopt = !std::is_constructible<std::optional<const volatile std::nullopt_t>>::value;
    TC_ASSERT_MSG(opt_nullopt, "optional<nullopt_t> shall not be constructible");
    TC_ASSERT_MSG(opt_c_nullopt, "optional<const nullopt_t> shall not be constructible");
    TC_ASSERT_MSG(opt_v_nullopt, "optional<volatile nullopt_t> shall not be constructible");
    TC_ASSERT_MSG(opt_cv_nullopt, "optional<const volatile nullopt_t> shall not be constructible");
    
    return true;
}

int tc_utilities_optional_optional_syn_optional_nullopt_t_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
