//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// template <class T, class... Args>
//   constexpr optional<T> make_optional(Args&&... args);

#include <optional>
#include <string>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 17
namespace {
TEST_CONSTEXPR_CXX20 bool test() {
    {
        constexpr auto opt = std::make_optional<int>('a');
        static_assert(*opt == int('a'));
    }
    {
        std::string s = "123";
        auto opt = std::make_optional<std::string>(s);
        TC_ASSERT_EXPR(*opt == "123");
    }
    {
        std::unique_ptr<int> s = std::make_unique<int>(3);
        auto opt = std::make_optional<std::unique_ptr<int>>(std::move(s));
        TC_ASSERT_EXPR(**opt == 3);
        TC_ASSERT_EXPR(s == nullptr);
    }
    {
        auto opt = std::make_optional<std::string>(4u, 'X');
        TC_ASSERT_EXPR(*opt == "XXXX");
    }
    return true;
}
} // namespace

int tc_utilities_optional_optional_specalg_make_optional_explicit_pass(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
