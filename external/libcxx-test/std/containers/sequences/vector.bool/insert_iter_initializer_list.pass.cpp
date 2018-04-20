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

// <vector>

// iterator insert(const_iterator p, initializer_list<value_type> il);

#include <vector>
#include <cassert>


int main()
{
    {
    std::vector<bool> d(10, true);
    std::vector<bool>::iterator i = d.insert(d.cbegin() + 2, {false, true, true, false});
    assert(d.size() == 14);
    assert(i == d.begin() + 2);
    assert(d[0] == true);
    assert(d[1] == true);
    assert(d[2] == false);
    assert(d[3] == true);
    assert(d[4] == true);
    assert(d[5] == false);
    assert(d[6] == true);
    assert(d[7] == true);
    assert(d[8] == true);
    assert(d[9] == true);
    assert(d[10] == true);
    assert(d[11] == true);
    assert(d[12] == true);
    assert(d[13] == true);
    }
}
