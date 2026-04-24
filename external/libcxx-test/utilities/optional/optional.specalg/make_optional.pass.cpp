/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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

  return 0;
}
