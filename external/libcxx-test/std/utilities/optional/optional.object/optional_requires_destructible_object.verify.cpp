//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// Test: T shall be an object type and shall satisfy Destructible requirements

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

using std::optional;

struct X
{
private:
    ~X() {}
};

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify optional cannot be instantiated with reference types
    constexpr bool opt_int_ref = !std::is_constructible<optional<int&>>::value;
    constexpr bool opt_int_rref = !std::is_constructible<optional<int&&>>::value;
    TC_ASSERT_MSG(opt_int_ref, "optional<int&> shall not be constructible");
    TC_ASSERT_MSG(opt_int_rref, "optional<int&&> shall not be constructible");
    
    // Verify optional cannot be instantiated with non-destructible types
    constexpr bool opt_X = !std::is_constructible<optional<X>>::value;
    TC_ASSERT_MSG(opt_X, "optional<X> with non-destructible X shall not be constructible");
    
    // Verify optional cannot be instantiated with function types
    constexpr bool opt_func = !std::is_constructible<optional<void()>>::value;
    TC_ASSERT_MSG(opt_func, "optional<void()> shall not be constructible");
    
    // Verify optional cannot be instantiated with void
    constexpr bool opt_cvoid = !std::is_constructible<optional<const void>>::value;
    TC_ASSERT_MSG(opt_cvoid, "optional<const void> shall not be constructible");
    
    return true;
}

int tc_utilities_optional_optional_object_optional_requires_destructible_object_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
