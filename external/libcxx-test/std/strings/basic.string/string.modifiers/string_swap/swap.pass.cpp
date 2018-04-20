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

// void swap(basic_string& s);

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "test_macros.h"

template <class S>
void
test(S s1, S s2)
{
    S s1_ = s1;
    S s2_ = s2;
    s1.swap(s2);
    LIBCPP_ASSERT(s1.__invariants());
    LIBCPP_ASSERT(s2.__invariants());
    assert(s1 == s2_);
    assert(s2 == s1_);
}

int main()
{
    {
    typedef std::string S;
    test(S(""), S(""));
    test(S(""), S("12345"));
    test(S(""), S("1234567890"));
    test(S(""), S("12345678901234567890"));
    test(S("abcde"), S(""));
    test(S("abcde"), S("12345"));
    test(S("abcde"), S("1234567890"));
    test(S("abcde"), S("12345678901234567890"));
    test(S("abcdefghij"), S(""));
    test(S("abcdefghij"), S("12345"));
    test(S("abcdefghij"), S("1234567890"));
    test(S("abcdefghij"), S("12345678901234567890"));
    test(S("abcdefghijklmnopqrst"), S(""));
    test(S("abcdefghijklmnopqrst"), S("12345"));
    test(S("abcdefghijklmnopqrst"), S("1234567890"));
    test(S("abcdefghijklmnopqrst"), S("12345678901234567890"));
    }
}
