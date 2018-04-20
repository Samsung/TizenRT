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
//   bool operator>(const basic_string<charT,traits,Allocator>& lhs,
//                  const basic_string<charT,traits,Allocator>& rhs);

#include <string>
#include <cassert>


template <class S>
void
test(const S& lhs, const S& rhs, bool x)
{
    assert((lhs > rhs) == x);
}

int main()
{
    {
    typedef std::string S;
    test(S(""), S(""), false);
    test(S(""), S("abcde"), false);
    test(S(""), S("abcdefghij"), false);
    test(S(""), S("abcdefghijklmnopqrst"), false);
    test(S("abcde"), S(""), true);
    test(S("abcde"), S("abcde"), false);
    test(S("abcde"), S("abcdefghij"), false);
    test(S("abcde"), S("abcdefghijklmnopqrst"), false);
    test(S("abcdefghij"), S(""), true);
    test(S("abcdefghij"), S("abcde"), true);
    test(S("abcdefghij"), S("abcdefghij"), false);
    test(S("abcdefghij"), S("abcdefghijklmnopqrst"), false);
    test(S("abcdefghijklmnopqrst"), S(""), true);
    test(S("abcdefghijklmnopqrst"), S("abcde"), true);
    test(S("abcdefghijklmnopqrst"), S("abcdefghij"), true);
    test(S("abcdefghijklmnopqrst"), S("abcdefghijklmnopqrst"), false);
    }
}
