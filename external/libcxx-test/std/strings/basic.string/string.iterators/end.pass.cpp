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

//       iterator end();
// const_iterator end() const;

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
    typename S::iterator e = s.end();
    typename S::const_iterator ce = cs.end();
    if (s.empty())
    {
        TC_ASSERT_EXPR(e == s.begin());
        TC_ASSERT_EXPR(ce == cs.begin());
    }
    TC_ASSERT_EXPR(static_cast<std::size_t>(e - s.begin()) == s.size());
    TC_ASSERT_EXPR(static_cast<std::size_t>(ce - cs.begin()) == cs.size());
    return 0;
}

int tc_libcxx_strings_string_iterators_end(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S())));
    TC_ASSERT_FUNC((test(S("123"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
