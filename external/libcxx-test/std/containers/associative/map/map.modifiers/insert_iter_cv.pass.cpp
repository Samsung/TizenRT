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

// <map>

// class map

// iterator insert(const_iterator position, const value_type& v);

#include <map>
#include <cassert>

#include "test_macros.h"

template <class Container>
void do_insert_iter_cv_test()
{
    typedef Container M;
    typedef typename M::iterator R;
    typedef typename M::value_type VT;

    M m;
    const VT v1(2, 2.5);
    R r = m.insert(m.end(), v1);
    assert(r == m.begin());
    assert(m.size() == 1);
    assert(r->first == 2);
    assert(r->second == 2.5);

    const VT v2(1, 1.5);
    r = m.insert(m.end(), v2);
    assert(r == m.begin());
    assert(m.size() == 2);
    assert(r->first == 1);
    assert(r->second == 1.5);

    const VT v3(3, 3.5);
    r = m.insert(m.end(), v3);
    assert(r == prev(m.end()));
    assert(m.size() == 3);
    assert(r->first == 3);
    assert(r->second == 3.5);

    const VT v4(3, 4.5);
    r = m.insert(m.end(), v4);
    assert(r == prev(m.end()));
    assert(m.size() == 3);
    assert(r->first == 3);
    assert(r->second == 3.5);
}

int main()
{
    do_insert_iter_cv_test<std::map<int, double> >();
}
