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

//       reverse_iterator rend();
// const_reverse_iterator rend() const;

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <cstddef>


template <class S>
static int
test(S s)
{
    const S& cs = s;
    typename S::reverse_iterator e = s.rend();
    typename S::const_reverse_iterator ce = cs.rend();
    if (s.empty())
    {
        TC_ASSERT_EXPR(e == s.rbegin());
        TC_ASSERT_EXPR(ce == cs.rbegin());
    }
    TC_ASSERT_EXPR(static_cast<std::size_t>(e - s.rbegin()) == s.size());
    TC_ASSERT_EXPR(static_cast<std::size_t>(ce - cs.rbegin()) == cs.size());
    return 0;
}

int tc_libcxx_strings_string_iterators_rend(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S())));
    TC_ASSERT_FUNC((test(S("123"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
