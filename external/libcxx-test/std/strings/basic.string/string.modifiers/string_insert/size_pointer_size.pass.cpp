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
//   insert(size_type pos, const charT* s, size_type n);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos, const typename S::value_type* str,
     typename S::size_type n, S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos <= old_size)
    {
        s.insert(pos, str, n);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.insert(pos, str, n);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos > old_size);
            TC_ASSERT_EXPR(s == s0);
        }
    }
#endif
    return 0;
}

int tc_libcxx_strings_string_insert_size_pointer_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 0, "", 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345", 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345", 1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345", 2, S("12"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345", 4, S("1234"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345", 5, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, "1234567890", 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, "1234567890", 1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, "1234567890", 5, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, "1234567890", 9, S("123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, "1234567890", 10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345678901234567890", 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345678901234567890", 1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345678901234567890", 10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345678901234567890", 19, S("1234567890123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345678901234567890", 20, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S(""), 1, "", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345", 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345", 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "1234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "1234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "1234567890", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "1234567890", 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "1234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345678901234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345678901234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345678901234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345678901234567890", 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345678901234567890", 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345", 1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345", 2, S("12abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345", 4, S("1234abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345", 5, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "1234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "1234567890", 1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "1234567890", 5, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "1234567890", 9, S("123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "1234567890", 10, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345678901234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345678901234567890", 1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345678901234567890", 10, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345678901234567890", 19, S("1234567890123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345678901234567890", 20, S("12345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345", 1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345", 2, S("a12bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345", 4, S("a1234bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345", 5, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "1234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "1234567890", 1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "1234567890", 5, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "1234567890", 9, S("a123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "1234567890", 10, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345678901234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345678901234567890", 1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345678901234567890", 10, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345678901234567890", 19, S("a1234567890123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345678901234567890", 20, S("a12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345", 1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345", 2, S("ab12cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345", 4, S("ab1234cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345", 5, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "1234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "1234567890", 1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "1234567890", 5, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "1234567890", 9, S("ab123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "1234567890", 10, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345678901234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345678901234567890", 1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345678901234567890", 10, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345678901234567890", 19, S("ab1234567890123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345678901234567890", 20, S("ab12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345", 1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345", 2, S("abcd12e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345", 4, S("abcd1234e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345", 5, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "1234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "1234567890", 1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "1234567890", 5, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "1234567890", 9, S("abcd123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "1234567890", 10, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345678901234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345678901234567890", 1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345678901234567890", 10, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345678901234567890", 19, S("abcd1234567890123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345678901234567890", 20, S("abcd12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345", 1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345", 2, S("abcde12"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345", 4, S("abcde1234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345", 5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "1234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "1234567890", 1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "1234567890", 5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "1234567890", 9, S("abcde123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "1234567890", 10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345678901234567890", 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345678901234567890", 1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345678901234567890", 10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345678901234567890", 19, S("abcde1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345678901234567890", 20, S("abcde12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345", 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345", 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "1234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "1234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "1234567890", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "1234567890", 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "1234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345678901234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345678901234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345678901234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345678901234567890", 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345678901234567890", 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345", 1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345", 2, S("12abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345", 4, S("1234abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345", 5, S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "1234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "1234567890", 1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "1234567890", 5, S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "1234567890", 9, S("123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "1234567890", 10, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345678901234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345678901234567890", 1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345678901234567890", 10, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345678901234567890", 19, S("1234567890123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345678901234567890", 20, S("12345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345", 1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345", 2, S("a12bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345", 4, S("a1234bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345", 5, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "1234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "1234567890", 1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "1234567890", 5, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "1234567890", 9, S("a123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "1234567890", 10, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345678901234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345678901234567890", 1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345678901234567890", 10, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345678901234567890", 19, S("a1234567890123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345678901234567890", 20, S("a12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345", 1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345", 2, S("abcde12fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345", 4, S("abcde1234fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345", 5, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "1234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "1234567890", 1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "1234567890", 5, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "1234567890", 9, S("abcde123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "1234567890", 10, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345678901234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345678901234567890", 1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345678901234567890", 10, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345678901234567890", 19, S("abcde1234567890123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345678901234567890", 20, S("abcde12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345", 1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345", 2, S("abcdefghi12j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345", 4, S("abcdefghi1234j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345", 5, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "1234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "1234567890", 1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "1234567890", 5, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "1234567890", 9, S("abcdefghi123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "1234567890", 10, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345678901234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345678901234567890", 1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345678901234567890", 10, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345678901234567890", 19, S("abcdefghi1234567890123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345678901234567890", 20, S("abcdefghi12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345", 1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345", 2, S("abcdefghij12"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345", 4, S("abcdefghij1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345", 5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "1234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "1234567890", 1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "1234567890", 5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "1234567890", 9, S("abcdefghij123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "1234567890", 10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345678901234567890", 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345678901234567890", 1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345678901234567890", 10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345678901234567890", 19, S("abcdefghij1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345678901234567890", 20, S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345", 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345", 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "1234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "1234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "1234567890", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "1234567890", 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "1234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345678901234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345678901234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345678901234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345678901234567890", 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345678901234567890", 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345", 1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345", 2, S("12abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345", 4, S("1234abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345", 5, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "1234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "1234567890", 1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "1234567890", 5, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "1234567890", 9, S("123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "1234567890", 10, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345678901234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345678901234567890", 1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345678901234567890", 10, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345678901234567890", 19, S("1234567890123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345678901234567890", 20, S("12345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345", 1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345", 2, S("a12bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345", 4, S("a1234bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345", 5, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "1234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "1234567890", 1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "1234567890", 5, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "1234567890", 9, S("a123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "1234567890", 10, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345678901234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345678901234567890", 1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345678901234567890", 10, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345678901234567890", 19, S("a1234567890123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345678901234567890", 20, S("a12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345", 1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345", 2, S("abcdefghij12klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345", 4, S("abcdefghij1234klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345", 5, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "1234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "1234567890", 1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "1234567890", 5, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "1234567890", 9, S("abcdefghij123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "1234567890", 10, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345678901234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345678901234567890", 1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345678901234567890", 10, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345678901234567890", 19, S("abcdefghij1234567890123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345678901234567890", 20, S("abcdefghij12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345", 1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345", 2, S("abcdefghijklmnopqrs12t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345", 4, S("abcdefghijklmnopqrs1234t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345", 5, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "1234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "1234567890", 1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "1234567890", 5, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "1234567890", 9, S("abcdefghijklmnopqrs123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "1234567890", 10, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345678901234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345678901234567890", 1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345678901234567890", 10, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345678901234567890", 19, S("abcdefghijklmnopqrs1234567890123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345678901234567890", 20, S("abcdefghijklmnopqrs12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345", 1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345", 2, S("abcdefghijklmnopqrst12"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345", 4, S("abcdefghijklmnopqrst1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345", 5, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "1234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "1234567890", 1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "1234567890", 5, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "1234567890", 9, S("abcdefghijklmnopqrst123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "1234567890", 10, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345678901234567890", 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345678901234567890", 1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345678901234567890", 10, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345678901234567890", 19, S("abcdefghijklmnopqrst1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345678901234567890", 20, S("abcdefghijklmnopqrst12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345", 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345", 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "1234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "1234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "1234567890", 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "1234567890", 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "1234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345678901234567890", 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345678901234567890", 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345678901234567890", 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345678901234567890", 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345678901234567890", 20, S("can't happen"))));
    }

    { // test inserting into self
    typedef std::string S;
    S s_short = "123/";
    S s_long  = "Lorem ipsum dolor sit amet, consectetur/";

    s_short.insert(0, s_short.data(), s_short.size());
    TC_ASSERT_EXPR(s_short == "123/123/");
    s_short.insert(0, s_short.data(), s_short.size());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/");
    s_short.insert(0, s_short.data(), s_short.size());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/123/123/123/123/");

    s_long.insert(0, s_long.data(), s_long.size());
    TC_ASSERT_EXPR(s_long == "Lorem ipsum dolor sit amet, consectetur/Lorem ipsum dolor sit amet, consectetur/");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
