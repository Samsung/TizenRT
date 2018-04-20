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

// basic_string<charT,traits,Allocator>& operator=(charT c);

#include <string>
#include <cassert>

#include "test_macros.h"

template <class S>
void
test(S s1, typename S::value_type s2)
{
    typedef typename S::traits_type T;
    s1 = s2;
    LIBCPP_ASSERT(s1.__invariants());
    assert(s1.size() == 1);
    assert(T::eq(s1[0], s2));
    assert(s1.capacity() >= s1.size());
}

int main()
{
    {
    typedef std::string S;
    test(S(), 'a');
    test(S("1"), 'a');
    test(S("123456789"), 'a');
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890"), 'a');
    }
}
