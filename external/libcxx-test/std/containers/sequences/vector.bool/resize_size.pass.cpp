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

// void resize(size_type sz);

#include <vector>
#include <cassert>


int main()
{
    {
        std::vector<bool> v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() >= 100);
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
        v.reserve(400);
        v.resize(300);  // check the case when resizing and we already have room
        assert(v.size() == 300);
        assert(v.capacity() >= 400);
    }
}
