//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// constexpr explicit optional<T>::operator bool() const noexcept;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_optional_optional_object_optional_object_observe_bool(void) {
    using std::optional;
    {
        const optional<int> opt; ((void)opt);
        ASSERT_NOEXCEPT(bool(opt));
        static_assert(!std::is_convertible<optional<int>, bool>::value, "");
    }
    {
        constexpr optional<int> opt;
        static_assert(!opt, "");
    }
    {
        constexpr optional<int> opt(0);
        static_assert(opt, "");
    }

    TC_SUCCESS_RESULT();
    return 0;
}
