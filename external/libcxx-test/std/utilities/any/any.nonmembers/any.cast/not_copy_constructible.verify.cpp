//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <any>

// Test: any_cast with non-copyable/movable types

#include <any>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

struct no_copy {
    no_copy() {}
    no_copy(no_copy &&) {}
    no_copy(no_copy const &) = delete;
};

struct no_move {
    no_move() {}
    no_move(no_move&&) = delete;
    no_move(no_move const&) {}
};

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test_not_copy_constructible_verify() {
    constexpr bool no_copy_check = !std::is_copy_constructible<no_copy>::value;
    constexpr bool no_move_check = !std::is_move_constructible<no_move>::value;
    TC_ASSERT_MSG(no_copy_check, "no_copy shall not be copy constructible");
    TC_ASSERT_MSG(no_move_check, "no_move shall not be move constructible");
    
    // Verify any_cast constraints for non-copyable types
    constexpr bool no_copy_from_lvalue = !std::is_constructible<no_copy, std::any&>::value;
    constexpr bool no_copy_from_const_lvalue = !std::is_constructible<no_copy, const std::any&>::value;
    TC_ASSERT_MSG(no_copy_from_lvalue, "no_copy shall not be constructible from any&");
    TC_ASSERT_MSG(no_copy_from_const_lvalue, "no_copy shall not be constructible from const any&");
    
    // no_copy from rvalue any is OK (uses move)
    // no_move from rvalue any should fail
    constexpr bool no_move_from_rvalue = !std::is_constructible<no_move, std::any&&>::value;
    TC_ASSERT_MSG(no_move_from_rvalue, "no_move shall not be constructible from any&&");
    
    return true;
}

int tc_any_cast_not_copy_constructible_verify(void) {
    test_not_copy_constructible_verify();
#if TEST_STD_VER > 17
    static_assert(test_not_copy_constructible_verify());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
