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

// UNSUPPORTED: c++98, c++03

// <string>

// basic_string& replace(const_iterator i1, const_iterator i2, initializer_list<charT> il);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_strings_string_replace_iter_iter_initializer_list(void)
{
    {
        std::string s("123def456");
        s.replace(s.cbegin() + 3, s.cbegin() + 6, {'a', 'b', 'c'});
        TC_ASSERT_EXPR(s == "123abc456");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
