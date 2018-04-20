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

// UNSUPPORTED: c++98, c++03

// <string>

// basic_string<charT,traits,Allocator>&
//   operator=(basic_string<charT,traits,Allocator>&& str);

#include <string>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
void
test(S s1, S s2)
{
    S s0 = s2;
    s1 = std::move(s2);
    LIBCPP_ASSERT(s1.__invariants());
    LIBCPP_ASSERT(s2.__invariants());
    assert(s1 == s0);
    assert(s1.capacity() >= s1.size());
}

int main()
{
    {
    typedef std::string S;
    test(S(), S());
    test(S("1"), S());
    test(S(), S("1"));
    test(S("1"), S("2"));
    test(S("1"), S("2"));

    test(S(),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S("123456789"),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890"),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890"
           "1234567890123456789012345678901234567890123456789012345678901234567890"),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    }
}
