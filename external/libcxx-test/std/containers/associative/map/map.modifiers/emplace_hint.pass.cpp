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
//   iterator emplace_hint(const_iterator position, Args&&... args);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "Emplaceable.h"
#include "DefaultOnly.h"

int tc_libcxx_containers_map_modifiers_emplace_hint(void)
{
    {
        typedef std::map<int, DefaultOnly> M;
        typedef M::iterator R;
        M m;
        TC_ASSERT_EXPR(DefaultOnly::count == 0);
        R r = m.emplace_hint(m.end());
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(m.begin()->first == 0);
        TC_ASSERT_EXPR(m.begin()->second == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 1);
        r = m.emplace_hint(m.end(), std::piecewise_construct,
                                       std::forward_as_tuple(1),
                                       std::forward_as_tuple());
        TC_ASSERT_EXPR(r == next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(next(m.begin())->first == 1);
        TC_ASSERT_EXPR(next(m.begin())->second == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 2);
        r = m.emplace_hint(m.end(), std::piecewise_construct,
                                       std::forward_as_tuple(1),
                                       std::forward_as_tuple());
        TC_ASSERT_EXPR(r == next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(next(m.begin())->first == 1);
        TC_ASSERT_EXPR(next(m.begin())->second == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 2);
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
        typedef std::map<int, Emplaceable> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.end(), std::piecewise_construct,
                                       std::forward_as_tuple(2),
                                       std::forward_as_tuple());
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == Emplaceable());
        r = m.emplace_hint(m.end(), std::piecewise_construct,
                                    std::forward_as_tuple(1),
                                    std::forward_as_tuple(2, 3.5));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == Emplaceable(2, 3.5));
        r = m.emplace_hint(m.end(), std::piecewise_construct,
                                    std::forward_as_tuple(1),
                                    std::forward_as_tuple(2, 3.5));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == Emplaceable(2, 3.5));
    }
    {
        typedef std::map<int, double> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.end(), M::value_type(2, 3.5));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == 3.5);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
