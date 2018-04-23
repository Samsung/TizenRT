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

// static int assign(initializer_list<value_type> il);

#include <vector>
#include <cassert>
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
