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

// int compare(const charT *s) const;

#include <string>
#include <cassert>


int sign(int x)
{
    if (x == 0)
        return 0;
    if (x < 0)
        return -1;
    return 1;
}

template <class S>
void
test(const S& s, const typename S::value_type* str, int x)
{
    assert(sign(s.compare(str)) == sign(x));
}

int main()
{
    {
    typedef std::string S;
    test(S(""), "", 0);
    test(S(""), "abcde", -5);
    test(S(""), "abcdefghij", -10);
    test(S(""), "abcdefghijklmnopqrst", -20);
    test(S("abcde"), "", 5);
    test(S("abcde"), "abcde", 0);
    test(S("abcde"), "abcdefghij", -5);
    test(S("abcde"), "abcdefghijklmnopqrst", -15);
    test(S("abcdefghij"), "", 10);
    test(S("abcdefghij"), "abcde", 5);
    test(S("abcdefghij"), "abcdefghij", 0);
    test(S("abcdefghij"), "abcdefghijklmnopqrst", -10);
    test(S("abcdefghijklmnopqrst"), "", 20);
    test(S("abcdefghijklmnopqrst"), "abcde", 15);
    test(S("abcdefghijklmnopqrst"), "abcdefghij", 10);
    test(S("abcdefghijklmnopqrst"), "abcdefghijklmnopqrst", 0);
    }
}
