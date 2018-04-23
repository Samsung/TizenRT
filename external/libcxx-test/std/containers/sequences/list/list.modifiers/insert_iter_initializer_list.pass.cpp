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

// UNSUPPORTED: c++98, c++03

// <list>

// iterator insert(const_iterator p, initializer_list<value_type> il);

#include <list>
#include <cassert>


int main()
{
    {
    std::list<int> d(10, 1);
    std::list<int>::iterator i = d.insert(next(d.cbegin(), 2), {3, 4, 5, 6});
    assert(d.size() == 14);
    assert(i == next(d.begin(), 2));
    i = d.begin();
    assert(*i++ == 1);
    assert(*i++ == 1);
    assert(*i++ == 3);
    assert(*i++ == 4);
    assert(*i++ == 5);
    assert(*i++ == 6);
    assert(*i++ == 1);
    assert(*i++ == 1);
    assert(*i++ == 1);
    assert(*i++ == 1);
    assert(*i++ == 1);
    assert(*i++ == 1);
    assert(*i++ == 1);
    assert(*i++ == 1);
    }
}
