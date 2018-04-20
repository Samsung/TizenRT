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

// iterator insert(const_iterator p, charT c);

#include <string>
#include <stdexcept>
#include <cassert>

#include "test_macros.h"

template <class S>
void
test(S& s, typename S::const_iterator p, typename S::value_type c, S expected)
{
    bool sufficient_cap = s.size() < s.capacity();
    typename S::difference_type pos = p - s.begin();
    typename S::iterator i = s.insert(p, c);
    LIBCPP_ASSERT(s.__invariants());
    assert(s == expected);
    assert(i - s.begin() == pos);
    assert(*i == c);
    if (sufficient_cap)
        assert(i == p);
}

int main()
{
    {
    typedef std::string S;
    S s;
    test(s, s.begin(), '1', S("1"));
    test(s, s.begin(), 'a', S("a1"));
    test(s, s.end(), 'b', S("a1b"));
    test(s, s.end()-1, 'c', S("a1cb"));
    test(s, s.end()-2, 'd', S("a1dcb"));
    test(s, s.end()-3, '2', S("a12dcb"));
    test(s, s.end()-4, '3', S("a132dcb"));
    test(s, s.end()-5, '4', S("a1432dcb"));
    test(s, s.begin()+1, '5', S("a51432dcb"));
    test(s, s.begin()+2, '6', S("a561432dcb"));
    test(s, s.begin()+3, '7', S("a5671432dcb"));
    test(s, s.begin()+4, 'A', S("a567A1432dcb"));
    test(s, s.begin()+5, 'B', S("a567AB1432dcb"));
    test(s, s.begin()+6, 'C', S("a567ABC1432dcb"));
    }
}
