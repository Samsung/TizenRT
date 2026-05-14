//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// <any>

// any::has_value() noexcept

#include <any>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"
#include "libcxx_tc_common.h"

int tc_utilities_any_any_class_any_observers_has_value(void) {
    {
        std::any a;
        ASSERT_NOEXCEPT(a.has_value());
    }
    // empty
    {
        std::any a;
        TC_ASSERT_EXPR(!a.has_value());

        a.reset();
        TC_ASSERT_EXPR(!a.has_value());

        a = 42;
        TC_ASSERT_EXPR(a.has_value());
    }
    // small object
    {
        std::any a = small(1);
        TC_ASSERT_EXPR(a.has_value());

        a.reset();
        TC_ASSERT_EXPR(!a.has_value());

        a = small(1);
        TC_ASSERT_EXPR(a.has_value());
    }
    // large object
    {
        std::any a = large(1);
        TC_ASSERT_EXPR(a.has_value());

        a.reset();
        TC_ASSERT_EXPR(!a.has_value());

        a = large(1);
        TC_ASSERT_EXPR(a.has_value());
    }

  return 0;
}
