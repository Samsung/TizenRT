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

// basic_string<charT,traits,Allocator>&
//   operator=(const charT* s);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s1, const typename S::value_type* s2)
{
    typedef typename S::traits_type T;
    s1 = s2;
    LIBCPP_ASSERT(s1.__invariants());
    TC_ASSERT_EXPR(s1.size() == T::length(s2));
    TC_ASSERT_EXPR(T::compare(s1.data(), s2, s1.size()) == 0);
    TC_ASSERT_EXPR(s1.capacity() >= s1.size());
    return 0;
}

int tc_libcxx_strings_string_cons_pointer_assignment(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), "")));
    TC_ASSERT_FUNC((test(S("1"), "")));
    TC_ASSERT_FUNC((test(S(), "1")));
    TC_ASSERT_FUNC((test(S("1"), "2")));
    TC_ASSERT_FUNC((test(S("1"), "2")));

    test(S(),
         "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S("123456789"),
         "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890"),
         "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890"
           "1234567890123456789012345678901234567890123456789012345678901234567890"),
         "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
