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
//   assign(const basic_string<charT,traits>& str);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
static int
test(S s, S str, S expected)
{
    s.assign(str);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

template <class S>
static int
testAlloc(S s, S str, const typename S::allocator_type& a)
{
    s.assign(str);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == str);
    TC_ASSERT_EXPR(s.get_allocator() == a);
    return 0;
}

int tc_libcxx_strings_string_assign_string(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), S(), S())));
    TC_ASSERT_FUNC((test(S(), S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S(), S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S(), S("12345678901234567890"), S("12345678901234567890"))));

    TC_ASSERT_FUNC((test(S("12345"), S(), S())));
    TC_ASSERT_FUNC((test(S("12345"), S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("12345"), S("12345678901234567890"), S("12345678901234567890"))));

    TC_ASSERT_FUNC((test(S("1234567890"), S(), S())));
    TC_ASSERT_FUNC((test(S("1234567890"), S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S("1234567890"), S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), S("12345678901234567890"), S("12345678901234567890"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), S(), S())));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), S("1234567890"), S("1234567890"))));
    test(S("12345678901234567890"), S("12345678901234567890"),
         S("12345678901234567890"));

    TC_ASSERT_FUNC((testAlloc(S(), S(), std::allocator<char>())));
    TC_ASSERT_FUNC((testAlloc(S(), S("12345"), std::allocator<char>())));
    TC_ASSERT_FUNC((testAlloc(S(), S("1234567890"), std::allocator<char>())));
    TC_ASSERT_FUNC((testAlloc(S(), S("12345678901234567890"), std::allocator<char>())));
    }

    { //  LWG#5579 make sure assign takes the allocators where appropriate
    typedef other_allocator<char> A;  // has POCCA --> true
    typedef std::basic_string<char, std::char_traits<char>, A> S;
    TC_ASSERT_FUNC((testAlloc(S(A(5)), S(A(3)), A(3))));
    TC_ASSERT_FUNC((testAlloc(S(A(5)), S("1"), A())));
    TC_ASSERT_FUNC((testAlloc(S(A(5)), S("1", A(7)), A(7))));
    TC_ASSERT_FUNC((testAlloc(S(A(5)), S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), A(7))));
    }

#if TEST_STD_VER > 14
    {
    typedef std::string S;
    static_assert(noexcept(S().assign(S())), "");  // LWG#2063
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
