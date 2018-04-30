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

// <map>

// class map

// template <class... Args>
//   pair<iterator, bool> emplace(Args&&... args);

#include <map>
#include <cassert>
#include <tuple>

#include "Emplaceable.h"
#include "DefaultOnly.h"

int main()
{
    {
        typedef std::map<int, DefaultOnly> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        assert(DefaultOnly::count == 0);
        R r = m.emplace();
        assert(r.second);
        assert(r.first == m.begin());
        assert(m.size() == 1);
        assert(m.begin()->first == 0);
        assert(m.begin()->second == DefaultOnly());
        assert(DefaultOnly::count == 1);
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple());
        assert(r.second);
        assert(r.first == next(m.begin()));
        assert(m.size() == 2);
        assert(next(m.begin())->first == 1);
        assert(next(m.begin())->second == DefaultOnly());
        assert(DefaultOnly::count == 2);
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple());
        assert(!r.second);
        assert(r.first == next(m.begin()));
        assert(m.size() == 2);
        assert(next(m.begin())->first == 1);
        assert(next(m.begin())->second == DefaultOnly());
        assert(DefaultOnly::count == 2);
    }
    assert(DefaultOnly::count == 0);
    {
        typedef std::map<int, Emplaceable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.emplace(std::piecewise_construct, std::forward_as_tuple(2),
                                                  std::forward_as_tuple());
        assert(r.second);
        assert(r.first == m.begin());
        assert(m.size() == 1);
        assert(m.begin()->first == 2);
        assert(m.begin()->second == Emplaceable());
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple(2, 3.5));
        assert(r.second);
        assert(r.first == m.begin());
        assert(m.size() == 2);
        assert(m.begin()->first == 1);
        assert(m.begin()->second == Emplaceable(2, 3.5));
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple(2, 3.5));
        assert(!r.second);
        assert(r.first == m.begin());
        assert(m.size() == 2);
        assert(m.begin()->first == 1);
        assert(m.begin()->second == Emplaceable(2, 3.5));
    }
    {
        typedef std::map<int, double> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.emplace(M::value_type(2, 3.5));
        assert(r.second);
        assert(r.first == m.begin());
        assert(m.size() == 1);
        assert(m.begin()->first == 2);
        assert(m.begin()->second == 3.5);
    }
}
