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
//   insert(size_type pos, size_type n, charT c);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos, typename S::size_type n,
     typename S::value_type str, S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos <= old_size)
    {
        s.insert(pos, n, str);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.insert(pos, n, str);
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

int tc_libcxx_strings_string_insert_size_size_char(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 0, 0, '1', S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 5, '1', S("11111"))));
    TC_ASSERT_FUNC((test(S(""), 0, 10, '1', S("1111111111"))));
    TC_ASSERT_FUNC((test(S(""), 0, 20, '1', S("11111111111111111111"))));
    TC_ASSERT_FUNC((test(S(""), 1, 0, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, 5, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, 10, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, 20, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, '1', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, '1', S("11111abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 10, '1', S("1111111111abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 20, '1', S("11111111111111111111abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, '1', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, '1', S("a11111bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 10, '1', S("a1111111111bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 20, '1', S("a11111111111111111111bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, '1', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 5, '1', S("ab11111cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 10, '1', S("ab1111111111cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 20, '1', S("ab11111111111111111111cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, '1', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 5, '1', S("abcd11111e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 10, '1', S("abcd1111111111e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 20, '1', S("abcd11111111111111111111e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, '1', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 5, '1', S("abcde11111"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 10, '1', S("abcde1111111111"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 20, '1', S("abcde11111111111111111111"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 5, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 10, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 20, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, '1', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, '1', S("11111abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, '1', S("1111111111abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 20, '1', S("11111111111111111111abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, '1', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 5, '1', S("a11111bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, '1', S("a1111111111bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 20, '1', S("a11111111111111111111bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, '1', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, '1', S("abcde11111fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 10, '1', S("abcde1111111111fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 20, '1', S("abcde11111111111111111111fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, '1', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 5, '1', S("abcdefghi11111j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 10, '1', S("abcdefghi1111111111j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 20, '1', S("abcdefghi11111111111111111111j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, '1', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 5, '1', S("abcdefghij11111"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 10, '1', S("abcdefghij1111111111"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 20, '1', S("abcdefghij11111111111111111111"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 5, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 10, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 20, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, '1', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 5, '1', S("11111abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, '1', S("1111111111abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, '1', S("11111111111111111111abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, '1', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 5, '1', S("a11111bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 10, '1', S("a1111111111bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, '1', S("a11111111111111111111bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, '1', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, '1', S("abcdefghij11111klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, '1', S("abcdefghij1111111111klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 20, '1', S("abcdefghij11111111111111111111klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, '1', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 5, '1', S("abcdefghijklmnopqrs11111t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 10, '1', S("abcdefghijklmnopqrs1111111111t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 20, '1', S("abcdefghijklmnopqrs11111111111111111111t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, '1', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 5, '1', S("abcdefghijklmnopqrst11111"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 10, '1', S("abcdefghijklmnopqrst1111111111"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 20, '1', S("abcdefghijklmnopqrst11111111111111111111"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 5, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 10, '1', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 20, '1', S("can't happen"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
