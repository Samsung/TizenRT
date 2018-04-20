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

// const charT* data() const;
//       charT* data();   // C++17

#include <string>
#include <cassert>

#include "test_macros.h"

template <class S>
void
test_const(const S& s)
{
    typedef typename S::traits_type T;
    const typename S::value_type* str = s.data();
    if (s.size() > 0)
    {
        assert(T::compare(str, &s[0], s.size()) == 0);
        assert(T::eq(str[s.size()], typename S::value_type()));
    }
    else
        assert(T::eq(str[0], typename S::value_type()));
}

template <class S>
void
test_nonconst(S& s)
{
    typedef typename S::traits_type T;
    typename S::value_type* str = s.data();
    if (s.size() > 0)
    {
        assert(T::compare(str, &s[0], s.size()) == 0);
        assert(T::eq(str[s.size()], typename S::value_type()));
    }
    else
        assert(T::eq(str[0], typename S::value_type()));
}

int main()
{
    {
    typedef std::string S;
    test_const(S(""));
    test_const(S("abcde"));
    test_const(S("abcdefghij"));
    test_const(S("abcdefghijklmnopqrst"));
    }
#if TEST_STD_VER > 14
    {
    typedef std::string S;
    S s1("");                     test_nonconst(s1);
    S s2("abcde");                test_nonconst(s2);
    S s3("abcdefghij");           test_nonconst(s3);
    S s4("abcdefghijklmnopqrst"); test_nonconst(s4);
    }
#endif
}
