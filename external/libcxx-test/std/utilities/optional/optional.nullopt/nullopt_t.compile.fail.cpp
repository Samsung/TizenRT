//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// Test: nullopt_t shall not have default constructor or initializer-list constructor

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

// [optional.nullopt]/2: Type nullopt_t shall not have a default constructor
// or an initializer-list constructor, and shall not be an aggregate.

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify nullopt_t is not default constructible
    TC_ASSERT_MSG(!std::is_default_constructible<std::nullopt_t>::value,
                  "nullopt_t shall not be default constructible");
    
    // Verify nullopt_t is not constructible from initializer_list
    constexpr bool nullopt_not_init_list = !std::is_constructible<std::nullopt_t, std::initializer_list<int>>::value;
    TC_ASSERT_MSG(nullopt_not_init_list,
                  "nullopt_t shall not be constructible from initializer_list");
    
    return true;
}

int tc_utilities_optional_optional_nullopt_nullopt_t_compile_fail(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
