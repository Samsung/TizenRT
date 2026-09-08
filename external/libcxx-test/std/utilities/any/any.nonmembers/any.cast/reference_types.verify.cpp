//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <any>

// Test: any_cast pointer overload does not accept reference types

#include <any>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test_reference_types_verify() {
    // Verify that any_cast pointer overload does not accept reference types
    // The pointer overload takes a pointer to any and returns pointer to ValueType
    // Reference types should not be allowed as ValueType
    
    constexpr bool is_lref = std::is_reference<int&>::value;
    constexpr bool is_rref = std::is_reference<int&&>::value;
    constexpr bool is_clref = std::is_reference<const int&>::value;
    constexpr bool is_crref = std::is_reference<const int&&>::value;
    TC_ASSERT_MSG(is_lref, "int& is a reference type - any_cast<int&>(&a) should be ill-formed");
    TC_ASSERT_MSG(is_rref, "int&& is a reference type - any_cast<int&&>(&a) should be ill-formed");
    TC_ASSERT_MSG(is_clref, "const int& is a reference type - any_cast<const int&>(&a) should be ill-formed");
    TC_ASSERT_MSG(is_crref, "const int&& is a reference type - any_cast<const int&&>(&a) should be ill-formed");
    
    return true;
}

int tc_any_cast_reference_types_verify(void) {
    test_reference_types_verify();
#if TEST_STD_VER > 17
    static_assert(test_reference_types_verify());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
