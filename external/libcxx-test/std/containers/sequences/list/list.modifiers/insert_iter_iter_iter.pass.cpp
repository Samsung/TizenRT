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

// <list>

// template <InputIterator Iter>
//   iterator insert(const_iterator position, Iter first, Iter last);

#include <list>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

template <class List>
void test() {
    int a1[] = {1, 2, 3};
    List l1;
    typename List::iterator i = l1.insert(l1.begin(), a1, a1+3);
    assert(i == l1.begin());
    assert(l1.size() == 3);
    assert(distance(l1.begin(), l1.end()) == 3);
    i = l1.begin();
    assert(*i == 1);
    ++i;
    assert(*i == 2);
    ++i;
    assert(*i == 3);
    int a2[] = {4, 5, 6};
    i = l1.insert(i, a2, a2+3);
    assert(*i == 4);
    assert(l1.size() == 6);
    assert(distance(l1.begin(), l1.end()) == 6);
    i = l1.begin();
    assert(*i == 1);
    ++i;
    assert(*i == 2);
    ++i;
    assert(*i == 4);
    ++i;
    assert(*i == 5);
    ++i;
    assert(*i == 6);
    ++i;
    assert(*i == 3);

}

int main()
{
    test<std::list<int> >();
}
