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
//   append(const basic_string<charT,traits>& str);

#include <string>
#include <cassert>

#include "test_macros.h"

template <class S>
void
test(S s, S str, S expected)
{
    s.append(str);
    LIBCPP_ASSERT(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef std::string S;
    test(S(), S(), S());
    test(S(), S("12345"), S("12345"));
    test(S(), S("1234567890"), S("1234567890"));
    test(S(), S("12345678901234567890"), S("12345678901234567890"));

    test(S("12345"), S(), S("12345"));
    test(S("12345"), S("12345"), S("1234512345"));
    test(S("12345"), S("1234567890"), S("123451234567890"));
    test(S("12345"), S("12345678901234567890"), S("1234512345678901234567890"));

    test(S("1234567890"), S(), S("1234567890"));
    test(S("1234567890"), S("12345"), S("123456789012345"));
    test(S("1234567890"), S("1234567890"), S("12345678901234567890"));
    test(S("1234567890"), S("12345678901234567890"), S("123456789012345678901234567890"));

    test(S("12345678901234567890"), S(), S("12345678901234567890"));
    test(S("12345678901234567890"), S("12345"), S("1234567890123456789012345"));
    test(S("12345678901234567890"), S("1234567890"), S("123456789012345678901234567890"));
    test(S("12345678901234567890"), S("12345678901234567890"),
         S("1234567890123456789012345678901234567890"));
    }
}
