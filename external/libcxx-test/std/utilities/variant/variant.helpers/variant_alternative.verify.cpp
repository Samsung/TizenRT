//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <variant>

// Test: variant_alternative with out-of-bounds index

#include <memory>
#include <type_traits>
#include <variant>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

namespace {
  
#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    using V = std::variant<int, void *, const void *, long double>;
    
    // variant_alternative with index >= variant size should not have a type member
    constexpr bool oob_index = !std::is_constructible<std::variant_alternative_t<4, V>>::value;
    TC_ASSERT_MSG(oob_index, "variant_alternative with out-of-bounds index shall not have a valid type");
    
    return true;
}
}

int tc_utilities_variant_variant_helpers_variant_alternative_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
