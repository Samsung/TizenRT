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

// const_reference operator[](size_type pos) const;
//       reference operator[](size_type pos);

#ifdef _LIBCPP_DEBUG
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>


int main()
{
    {
    typedef std::string S;
    S s("0123456789");
    const S& cs = s;
    for (S::size_type i = 0; i < cs.size(); ++i)
    {
        assert(s[i] == static_cast<char>('0' + i));
        assert(cs[i] == s[i]);
    }
    assert(cs[cs.size()] == '\0');
    const S s2 = S();
    assert(s2[0] == '\0');
    }
#ifdef _LIBCPP_DEBUG
    {
        std::string s;
        char c = s[0];
        assert(c == '\0');
        c = s[1];
        assert(false);
    }
#endif
}
