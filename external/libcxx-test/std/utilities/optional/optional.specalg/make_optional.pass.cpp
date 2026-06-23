//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_optional_access-missing && !no-exceptions

// <optional>
//
// template <class T>
//   constexpr optional<decay_t<T>> make_optional(T&& v);

#include <optional>
#include <string>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_optional_optional_specalg_make_optional(void) {
    {
        int arr[10];
        auto opt = std::make_optional(arr);
        ASSERT_SAME_TYPE(decltype(opt), std::optional<int*>);
        TC_ASSERT_EXPR(*opt == arr);
    }
    {
        constexpr auto opt = std::make_optional(2);
        ASSERT_SAME_TYPE(decltype(opt), const std::optional<int>);
        static_assert(opt.value() == 2);
    }
    {
        auto opt = std::make_optional(2);
        ASSERT_SAME_TYPE(decltype(opt), std::optional<int>);
        TC_ASSERT_EXPR(*opt == 2);
    }
    {
        const std::string s = "123";
        auto opt = std::make_optional(s);
        ASSERT_SAME_TYPE(decltype(opt), std::optional<std::string>);
        TC_ASSERT_EXPR(*opt == "123");
    }
    {
        std::unique_ptr<int> s = std::make_unique<int>(3);
        auto opt = std::make_optional(std::move(s));
        ASSERT_SAME_TYPE(decltype(opt), std::optional<std::unique_ptr<int>>);
        TC_ASSERT_EXPR(**opt == 3);
        TC_ASSERT_EXPR(s == nullptr);
    }

    TC_SUCCESS_RESULT();
    return 0;
}
