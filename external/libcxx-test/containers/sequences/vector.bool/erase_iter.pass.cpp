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
// <vector>
// vector<bool>

// iterator erase(const_iterator position);

#include <vector>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    bool a1[] = {1, 0, 1};
    {
    std::vector<bool> l1(a1, a1+3);
    std::vector<bool>::const_iterator i = l1.begin();
    ++i;
    std::vector<bool>::iterator j = l1.erase(i);
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 2);
    TC_ASSERT_EXPR(*j == true);
    TC_ASSERT_EXPR(*l1.begin() == 1);
    TC_ASSERT_EXPR(*std::next(l1.begin()) == true);
    j = l1.erase(j);
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 1);
    TC_ASSERT_EXPR(*l1.begin() == true);
    j = l1.erase(l1.begin());
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 0);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 0);
    }
#if TEST_STD_VER >= 11
    {
    std::vector<bool, min_allocator<bool>> l1(a1, a1+3);
    std::vector<bool, min_allocator<bool>>::const_iterator i = l1.begin();
    ++i;
    std::vector<bool, min_allocator<bool>>::iterator j = l1.erase(i);
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 2);
    TC_ASSERT_EXPR(*j == true);
    TC_ASSERT_EXPR(*l1.begin() == 1);
    TC_ASSERT_EXPR(*std::next(l1.begin()) == true);
    j = l1.erase(j);
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 1);
    TC_ASSERT_EXPR(*l1.begin() == true);
    j = l1.erase(l1.begin());
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 0);
    TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 0);
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_erase_iter(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
