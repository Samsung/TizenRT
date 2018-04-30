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
//   insert(size_type pos, const charT* s);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos, const typename S::value_type* str, S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos <= old_size)
    {
        s.insert(pos, str);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.insert(pos, str);
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

int tc_libcxx_strings_string_insert_size_pointer(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 0, "", S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345", S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, "1234567890", S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, "12345678901234567890", S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S(""), 1, "", S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345", S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "1234567890", S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, "12345678901234567890", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "", S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345", S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "1234567890", S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, "12345678901234567890", S("12345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "", S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345", S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "1234567890", S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, "12345678901234567890", S("a12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "", S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345", S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "1234567890", S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, "12345678901234567890", S("ab12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "", S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345", S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "1234567890", S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, "12345678901234567890", S("abcd12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "", S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345", S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "1234567890", S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, "12345678901234567890", S("abcde12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "1234567890", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, "12345678901234567890", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "", S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345", S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "1234567890", S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, "12345678901234567890", S("12345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "", S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345", S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "1234567890", S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, "12345678901234567890", S("a12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "", S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345", S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "1234567890", S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, "12345678901234567890", S("abcde12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "", S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345", S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "1234567890", S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, "12345678901234567890", S("abcdefghi12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "", S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345", S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "1234567890", S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, "12345678901234567890", S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "1234567890", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, "12345678901234567890", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "", S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345", S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "1234567890", S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, "12345678901234567890", S("12345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "", S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345", S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "1234567890", S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, "12345678901234567890", S("a12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "", S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345", S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "1234567890", S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, "12345678901234567890", S("abcdefghij12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "", S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345", S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "1234567890", S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, "12345678901234567890", S("abcdefghijklmnopqrs12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "", S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345", S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "1234567890", S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, "12345678901234567890", S("abcdefghijklmnopqrst12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "1234567890", S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, "12345678901234567890", S("can't happen"))));
    }

    { // test inserting into self
    typedef std::string S;
    S s_short = "123/";
    S s_long  = "Lorem ipsum dolor sit amet, consectetur/";

    s_short.insert(0, s_short.c_str());
    TC_ASSERT_EXPR(s_short == "123/123/");
    s_short.insert(0, s_short.c_str());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/");
    s_short.insert(0, s_short.c_str());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/123/123/123/123/");

    s_long.insert(0, s_long.c_str());
    TC_ASSERT_EXPR(s_long == "Lorem ipsum dolor sit amet, consectetur/Lorem ipsum dolor sit amet, consectetur/");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
