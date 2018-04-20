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
//   bool operator<(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs);

#include <string>
#include <cassert>


template <class S>
void
test(const S& lhs, const typename S::value_type* rhs, bool x)
{
    assert((lhs < rhs) == x);
}

int main()
{
    {
    typedef std::string S;
    test(S(""), "", false);
    test(S(""), "abcde", true);
    test(S(""), "abcdefghij", true);
    test(S(""), "abcdefghijklmnopqrst", true);
    test(S("abcde"), "", false);
    test(S("abcde"), "abcde", false);
    test(S("abcde"), "abcdefghij", true);
    test(S("abcde"), "abcdefghijklmnopqrst", true);
    test(S("abcdefghij"), "", false);
    test(S("abcdefghij"), "abcde", false);
    test(S("abcdefghij"), "abcdefghij", false);
    test(S("abcdefghij"), "abcdefghijklmnopqrst", true);
    test(S("abcdefghijklmnopqrst"), "", false);
    test(S("abcdefghijklmnopqrst"), "abcde", false);
    test(S("abcdefghijklmnopqrst"), "abcdefghij", false);
    test(S("abcdefghijklmnopqrst"), "abcdefghijklmnopqrst", false);
    }
}
