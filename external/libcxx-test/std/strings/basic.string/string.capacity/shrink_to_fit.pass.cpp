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

// void shrink_to_fit();

#include <string>
#include <cassert>

#include "test_macros.h"

template <class S>
void
test(S s)
{
    typename S::size_type old_cap = s.capacity();
    S s0 = s;
    s.shrink_to_fit();
    LIBCPP_ASSERT(s.__invariants());
    assert(s == s0);
    assert(s.capacity() <= old_cap);
    assert(s.capacity() >= s.size());
}

int main()
{
    {
    typedef std::string S;
    S s;
    test(s);

    s.assign(10, 'a');
    s.erase(5);
    test(s);

    s.assign(100, 'a');
    s.erase(50);
    test(s);
    }
}
