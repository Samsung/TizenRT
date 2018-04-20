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

// void reserve(size_type n);

#include <vector>
#include <cassert>


int main()
{
    {
        std::vector<bool> v;
        v.reserve(10);
        assert(v.capacity() >= 10);
    }
    {
        std::vector<bool> v(100);
        assert(v.capacity() >= 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() >= 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() >= 150);
    }
}
