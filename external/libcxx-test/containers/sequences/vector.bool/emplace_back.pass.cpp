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
// UNSUPPORTED: c++03, c++11
// <vector>
//  vector.bool

// template <class... Args> reference emplace_back(Args&&... args);
// return type is 'reference' in C++17; 'void' before

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        typedef std::vector<bool> C;
        C c;
#if TEST_STD_VER > 14
        typedef C::reference Ref;
        Ref r1 = c.emplace_back();
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);
        r1 = true;
        TC_ASSERT_EXPR(c.front() == true);
        r1 = false;
        Ref r2 = c.emplace_back(true);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);
        r2 = false;
        TC_ASSERT_EXPR(c.back() == false);
        r2 = true;
#else
        c.emplace_back();
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);
        c.emplace_back(true);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);
#endif
        c.emplace_back(true);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c[1] == true);
        TC_ASSERT_EXPR(c.back() == true);
    }
    {
        typedef std::vector<bool, min_allocator<bool>> C;
        C c;

#if TEST_STD_VER > 14
        typedef C::reference Ref;
        Ref r1 = c.emplace_back();
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);
        r1 = true;
        TC_ASSERT_EXPR(c.front() == true);
        r1 = false;
        Ref r2 = c.emplace_back(true);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);
        r2 = false;
        TC_ASSERT_EXPR(c.back() == false);
        r2 = true;
#else
        c.emplace_back();
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);
        c.emplace_back(true);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);
#endif
        c.emplace_back(true);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c[1] == true);
        TC_ASSERT_EXPR(c.back() == true);
    }

    return true;
}

int tc_containers_sequences_vector_bool_emplace_back(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
