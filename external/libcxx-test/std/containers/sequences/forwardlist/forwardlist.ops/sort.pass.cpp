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

// <forward_list>

// void sort();

#include <forward_list>
#include <iterator>
#include <algorithm>
#include <vector>
#include <random>
#include <cassert>


std::mt19937 randomness;

template <class C>
void test(int N)
{
    typedef typename C::value_type T;
    typedef std::vector<T> V;
    V v;
    for (int i = 0; i < N; ++i)
        v.push_back(i);
    std::shuffle(v.begin(), v.end(), randomness);
    C c(v.begin(), v.end());
    c.sort();
    assert(distance(c.begin(), c.end()) == N);
    typename C::const_iterator j = c.begin();
    for (int i = 0; i < N; ++i, ++j)
        assert(*j == i);
}

int main()
{
    for (int i = 0; i < 40; ++i)
        test<std::forward_list<int> >(i);
}
