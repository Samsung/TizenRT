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

// <iterator>

// move_iterator

// explicit move_iterator(Iter i);
//
//  constexpr in C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
void
test(It i)
{
    std::move_iterator<It> r(i);
    assert(r.base() == i);
}

int main()
{
    char s[] = "123";
    test(input_iterator<char*>(s));
    test(forward_iterator<char*>(s));
    test(bidirectional_iterator<char*>(s));
    test(random_access_iterator<char*>(s));
    test(s);

#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    constexpr std::move_iterator<const char *> it(p);
    static_assert(it.base() == p);
    }
#endif
}
