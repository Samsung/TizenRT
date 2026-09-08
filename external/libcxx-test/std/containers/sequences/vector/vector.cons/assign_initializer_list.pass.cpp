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

// <vector>

// static int assign(initializer_list<value_type> il);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "asan_testing.h"

template <typename Vec>
static int test ( Vec &v )
{
    v.assign({3, 4, 5, 6});
    TC_ASSERT_EXPR(v.size() == 4);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    TC_ASSERT_EXPR(v[0] == 3);
    TC_ASSERT_EXPR(v[1] == 4);
    TC_ASSERT_EXPR(v[2] == 5);
    TC_ASSERT_EXPR(v[3] == 6);
    return 0;
}

int tc_libcxx_containers_vector_cons_assign_initializer_list(void)
{
    {
    typedef std::vector<int> V;
    V d1;
    V d2;
    d2.reserve(10);  // no reallocation during assign.
    TC_ASSERT_FUNC((test(d1)));
    TC_ASSERT_FUNC((test(d2)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
