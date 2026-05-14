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
//  vector<bool>

// template <class... Args> iterator emplace(const_iterator pos, Args&&... args);

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

        C::iterator i = c.emplace(c.cbegin());
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);

        i = c.emplace(c.cend(), true);
        TC_ASSERT_EXPR(i == c.end()-1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);

        i = c.emplace(c.cbegin()+1, true);
        TC_ASSERT_EXPR(i == c.begin()+1);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c[1] == true);
        TC_ASSERT_EXPR(c.back() == true);
    }
    {
        typedef std::vector<bool, min_allocator<bool>> C;
        C c;

        C::iterator i = c.emplace(c.cbegin());
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);

        i = c.emplace(c.cend(), true);
        TC_ASSERT_EXPR(i == c.end()-1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);

        i = c.emplace(c.cbegin()+1, true);
        TC_ASSERT_EXPR(i == c.begin()+1);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c[1] == true);
        TC_ASSERT_EXPR(c.back() == true);
    }

    return true;
}

int tc_containers_sequences_vector_bool_emplace(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
