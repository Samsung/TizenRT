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

// <vector>
// vector<bool>

// void swap(vector& x);

#include <vector>
#include <cassert>
#include "test_allocator.h"

int main()
{
    {
        std::vector<bool> v1(100);
        std::vector<bool> v2(200);
        v1.swap(v2);
        assert(v1.size() == 200);
        assert(v1.capacity() >= 200);
        assert(v2.size() == 100);
        assert(v2.capacity() >= 100);
    }
    {
        typedef test_allocator<bool> A;
        std::vector<bool, A> v1(100, true, A(1, 1));
        std::vector<bool, A> v2(200, false, A(1, 2));
        swap(v1, v2);
        assert(v1.size() == 200);
        assert(v1.capacity() >= 200);
        assert(v2.size() == 100);
        assert(v2.capacity() >= 100);
        assert(v1.get_allocator().get_id() == 1);
        assert(v2.get_allocator().get_id() == 2);
    }
    {
        typedef other_allocator<bool> A;
        std::vector<bool, A> v1(100, true, A(1));
        std::vector<bool, A> v2(200, false, A(2));
        swap(v1, v2);
        assert(v1.size() == 200);
        assert(v1.capacity() >= 200);
        assert(v2.size() == 100);
        assert(v2.capacity() >= 100);
        assert(v1.get_allocator() == A(2));
        assert(v2.get_allocator() == A(1));
    }
    {
        std::vector<bool> v(2);
        std::vector<bool>::reference r1 = v[0];
        std::vector<bool>::reference r2 = v[1];
        r1 = true;
        using std::swap;
        swap(r1, r2);
        assert(v[0] == false);
        assert(v[1] == true);
    }
}
