//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_any_cast-missing && !no-exceptions

// <any>

// any::reset() noexcept

#include <any>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"
#include "libcxx_tc_common.h"

int tc_utilities_any_any_class_any_modifiers_reset(void) {
    // empty
    {
        std::any a;
        ASSERT_NOEXCEPT(a.reset());

        assertEmpty(a);

        a.reset();

        assertEmpty(a);
    }
    // small object
    {
        std::any a = small(1);
        TC_ASSERT_EXPR(small::count == 1);
        assertContains<small>(a, 1);

        a.reset();

        assertEmpty<small>(a);
        TC_ASSERT_EXPR(small::count == 0);
    }
    // large object
    {
        std::any a = large(1);
        TC_ASSERT_EXPR(large::count == 1);
        assertContains<large>(a, 1);

        a.reset();

        assertEmpty<large>(a);
        TC_ASSERT_EXPR(large::count == 0);
    }

  return 0;
}
