//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// Test: T shall be an object type other than cv in_place_t or cv nullopt_t
// and shall satisfy the Cpp17Destructible requirements.

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "../../../libcxx_tc_common.h"

struct NonDestructible { ~NonDestructible() = delete; };

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify optional cannot be instantiated with reference types
    constexpr bool opt_char_ref = !std::is_constructible<std::optional<char&>>::value;
    constexpr bool opt_non_destruct = !std::is_constructible<std::optional<NonDestructible>>::value;
    constexpr bool opt_array = !std::is_constructible<std::optional<char[20]>>::value;
    TC_ASSERT_MSG(opt_char_ref, "optional<char&> shall not be constructible");
    TC_ASSERT_MSG(opt_non_destruct, "optional<NonDestructible> shall not be constructible");
    TC_ASSERT_MSG(opt_array, "optional<char[20]> shall not be constructible");
    
    // Verify optional cannot be instantiated with in_place_t
    constexpr bool opt_in_place = !std::is_constructible<std::optional<std::in_place_t>>::value;
    constexpr bool opt_c_in_place = !std::is_constructible<std::optional<const std::in_place_t>>::value;
    constexpr bool opt_v_in_place = !std::is_constructible<std::optional<volatile std::in_place_t>>::value;
    constexpr bool opt_cv_in_place = !std::is_constructible<std::optional<const volatile std::in_place_t>>::value;
    TC_ASSERT_MSG(opt_in_place, "optional<in_place_t> shall not be constructible");
    TC_ASSERT_MSG(opt_c_in_place, "optional<const in_place_t> shall not be constructible");
    TC_ASSERT_MSG(opt_v_in_place, "optional<volatile in_place_t> shall not be constructible");
    TC_ASSERT_MSG(opt_cv_in_place, "optional<const volatile in_place_t> shall not be constructible");
    
    // Verify optional cannot be instantiated with nullopt_t
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

int tc_utilities_optional_optional_object_ctor_ctor_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
