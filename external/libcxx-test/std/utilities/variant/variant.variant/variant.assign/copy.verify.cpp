//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <variant>

// Test: LWG issue 3024 - variant with non-copyable type is not copyable

#include <variant>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

struct NotCopyConstructible
{
    NotCopyConstructible() = default;
    NotCopyConstructible(NotCopyConstructible const&) = delete;
};

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test_variant_assign_copy_verify() {
    constexpr bool not_copy = !std::is_copy_constructible_v<NotCopyConstructible>;
    TC_ASSERT_MSG(not_copy, "NotCopyConstructible shall not be copy constructible");
    
    // Verify variant with non-copyable type is not copy constructible
    constexpr bool var_not_copy = !std::is_copy_constructible<std::variant<NotCopyConstructible>>::value;
    TC_ASSERT_MSG(var_not_copy, "variant<NotCopyConstructible> shall not be copy constructible");
    
    // Verify variant with non-copyable type is not copy assignable
    constexpr bool var_not_assign = !std::is_copy_assignable<std::variant<NotCopyConstructible>>::value;
    TC_ASSERT_MSG(var_not_assign, "variant<NotCopyConstructible> shall not be copy assignable");
    
    return true;
}

int tc_utilities_variant_assign_copy_verify(void) {
    test_variant_assign_copy_verify();
#if TEST_STD_VER > 17
    static_assert(test_variant_assign_copy_verify());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
