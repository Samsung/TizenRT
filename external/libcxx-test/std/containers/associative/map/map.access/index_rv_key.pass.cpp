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

// mapped_type& operator[](key_type&& k);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "MoveOnly.h"
#include "container_test_types.h"

int tc_libcxx_containers_map_access_index_rv_key(void)
{
    std::map<MoveOnly, double> m;
    TC_ASSERT_EXPR(m.size() == 0);
    TC_ASSERT_EXPR(m[1] == 0.0);
    TC_ASSERT_EXPR(m.size() == 1);
    m[1] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 2);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 2);
    TC_SUCCESS_RESULT();
    return 0;
}
