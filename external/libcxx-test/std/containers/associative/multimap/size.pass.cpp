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

// <map>

// class multimap

// size_type size() const;

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_multimap_size(void)
{
    {
    typedef std::multimap<int, double> M;
    M m;
    TC_ASSERT_EXPR(m.size() == 0);
    m.insert(M::value_type(2, 1.5));
    TC_ASSERT_EXPR(m.size() == 1);
    m.insert(M::value_type(1, 1.5));
    TC_ASSERT_EXPR(m.size() == 2);
    m.insert(M::value_type(3, 1.5));
    TC_ASSERT_EXPR(m.size() == 3);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
