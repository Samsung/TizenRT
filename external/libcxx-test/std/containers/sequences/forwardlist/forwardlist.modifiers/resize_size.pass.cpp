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

// void resize(size_type n);

#include <forward_list>
#include <cassert>

#include "DefaultOnly.h"

int main()
{
    {
        typedef DefaultOnly T;
        typedef std::forward_list<T> C;
        C c;
        c.resize(0);
        assert(distance(c.begin(), c.end()) == 0);
        c.resize(10);
        assert(distance(c.begin(), c.end()) == 10);
        c.resize(20);
        assert(distance(c.begin(), c.end()) == 20);
        c.resize(5);
        assert(distance(c.begin(), c.end()) == 5);
        c.resize(0);
        assert(distance(c.begin(), c.end()) == 0);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4};
        C c(std::begin(t), std::end(t));

        c.resize(3);
        assert(distance(c.begin(), c.end()) == 3);
        assert(*next(c.begin(), 0) == 0);
        assert(*next(c.begin(), 1) == 1);
        assert(*next(c.begin(), 2) == 2);

        c.resize(6);
        assert(distance(c.begin(), c.end()) == 6);
        assert(*next(c.begin(), 0) == 0);
        assert(*next(c.begin(), 1) == 1);
        assert(*next(c.begin(), 2) == 2);
        assert(*next(c.begin(), 3) == 0);
        assert(*next(c.begin(), 4) == 0);
        assert(*next(c.begin(), 5) == 0);

        c.resize(6);
        assert(distance(c.begin(), c.end()) == 6);
        assert(*next(c.begin(), 0) == 0);
        assert(*next(c.begin(), 1) == 1);
        assert(*next(c.begin(), 2) == 2);
        assert(*next(c.begin(), 3) == 0);
        assert(*next(c.begin(), 4) == 0);
        assert(*next(c.begin(), 5) == 0);
    }
}
