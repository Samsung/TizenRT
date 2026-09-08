//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <tuple>

#include "Emplaceable.h"
#include "DefaultOnly.h"

int tc_libcxx_containers_map_modifiers_emplace(void)
{
    {
        typedef std::map<int, DefaultOnly> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        TC_ASSERT_EXPR(DefaultOnly::count == 0);
        R r = m.emplace();
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(m.begin()->first == 0);
        TC_ASSERT_EXPR(m.begin()->second == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 1);
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple());
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(next(m.begin())->first == 1);
        TC_ASSERT_EXPR(next(m.begin())->second == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 2);
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple());
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(r.first == next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(next(m.begin())->first == 1);
        TC_ASSERT_EXPR(next(m.begin())->second == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 2);
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
        typedef std::map<int, Emplaceable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.emplace(std::piecewise_construct, std::forward_as_tuple(2),
                                                  std::forward_as_tuple());
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == Emplaceable());
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple(2, 3.5));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == Emplaceable(2, 3.5));
        r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                                                std::forward_as_tuple(2, 3.5));
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == Emplaceable(2, 3.5));
    }
    {
        typedef std::map<int, double> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.emplace(M::value_type(2, 3.5));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == 3.5);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
