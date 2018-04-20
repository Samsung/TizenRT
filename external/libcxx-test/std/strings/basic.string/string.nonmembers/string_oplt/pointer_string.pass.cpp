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

// template<class charT, class traits, class Allocator>
//   bool operator<(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs);

#include <string>
#include <cassert>


template <class S>
void
test(const typename S::value_type* lhs, const S& rhs, bool x)
{
    assert((lhs < rhs) == x);
}

int main()
{
    {
    typedef std::string S;
    test("", S(""), false);
    test("", S("abcde"), true);
    test("", S("abcdefghij"), true);
    test("", S("abcdefghijklmnopqrst"), true);
    test("abcde", S(""), false);
    test("abcde", S("abcde"), false);
    test("abcde", S("abcdefghij"), true);
    test("abcde", S("abcdefghijklmnopqrst"), true);
    test("abcdefghij", S(""), false);
    test("abcdefghij", S("abcde"), false);
    test("abcdefghij", S("abcdefghij"), false);
    test("abcdefghij", S("abcdefghijklmnopqrst"), true);
    test("abcdefghijklmnopqrst", S(""), false);
    test("abcdefghijklmnopqrst", S("abcde"), false);
    test("abcdefghijklmnopqrst", S("abcdefghij"), false);
    test("abcdefghijklmnopqrst", S("abcdefghijklmnopqrst"), false);
    }
}
