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

// void push_back(charT c)

#include <string>
#include <cassert>

#include "test_macros.h"

struct veryLarge
{
  long long a;
  char b;
};

template <class S>
void
test(S s, typename S::value_type c, S expected)
{
    s.push_back(c);
    LIBCPP_ASSERT(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef std::string S;
    test(S(), 'a', S(1, 'a'));
    test(S("12345"), 'a', S("12345a"));
    test(S("12345678901234567890"), 'a', S("12345678901234567890a"));
    }

    {
// https://bugs.llvm.org/show_bug.cgi?id=31454
    std::basic_string<veryLarge> s;
    veryLarge vl = {};
    s.push_back(vl);
    s.push_back(vl);
    s.push_back(vl);
    }
}
