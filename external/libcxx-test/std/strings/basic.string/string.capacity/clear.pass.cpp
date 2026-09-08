//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// static int clear();

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class S>
static int
test(S s)
{
    s.clear();
    TC_ASSERT_EXPR(s.size() == 0);
    return 0;
}

int tc_libcxx_strings_string_capacity_clear(void)
{
    {
    typedef std::string S;
    S s;
    TC_ASSERT_FUNC((test(s)));

    s.assign(10, 'a');
    s.erase(5);
    TC_ASSERT_FUNC((test(s)));

    s.assign(100, 'a');
    s.erase(50);
    TC_ASSERT_FUNC((test(s)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
