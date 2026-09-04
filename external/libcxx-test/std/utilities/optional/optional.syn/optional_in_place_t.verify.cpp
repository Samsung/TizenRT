//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// Test: optional instantiation with in_place_t is ill-formed

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify that optional cannot be instantiated with in_place_t
    constexpr bool opt_in_place = !std::is_constructible<std::optional<std::in_place_t>>::value;
    constexpr bool opt_c_in_place = !std::is_constructible<std::optional<const std::in_place_t>>::value;
    constexpr bool opt_v_in_place = !std::is_constructible<std::optional<volatile std::in_place_t>>::value;
    constexpr bool opt_cv_in_place = !std::is_constructible<std::optional<const volatile std::in_place_t>>::value;
    TC_ASSERT_MSG(opt_in_place, "optional<in_place_t> shall not be constructible");
    TC_ASSERT_MSG(opt_c_in_place, "optional<const in_place_t> shall not be constructible");
    TC_ASSERT_MSG(opt_v_in_place, "optional<volatile in_place_t> shall not be constructible");
    TC_ASSERT_MSG(opt_cv_in_place, "optional<const volatile in_place_t> shall not be constructible");
    
    return true;
}

int tc_utilities_optional_optional_syn_optional_in_place_t_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
