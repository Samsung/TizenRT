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

// class map

// bool empty() const;

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_containers_map_access_empty(void)
{
    {
    typedef std::map<int, double> M;
    M m;
    TC_ASSERT_EXPR(m.empty());
    m.insert(M::value_type(1, 1.5));
    TC_ASSERT_EXPR(!m.empty());
    m.clear();
    TC_ASSERT_EXPR(m.empty());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
