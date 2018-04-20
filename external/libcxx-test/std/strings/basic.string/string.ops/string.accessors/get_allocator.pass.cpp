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

// allocator_type get_allocator() const;

#include <string>
#include <cassert>

#include "test_allocator.h"

template <class S>
void
test(const S& s, const typename S::allocator_type& a)
{
    assert(s.get_allocator() == a);
}

int main()
{
    {
    typedef test_allocator<char> A;
    typedef std::basic_string<char, std::char_traits<char>, A> S;
    test(S(""), A());
    test(S("abcde", A(1)), A(1));
    test(S("abcdefghij", A(2)), A(2));
    test(S("abcdefghijklmnopqrst", A(3)), A(3));
    }
}
