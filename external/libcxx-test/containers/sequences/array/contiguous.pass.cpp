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
// <array>

// An array is a contiguous container

#include <array>
#include <cassert>
#include <memory>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Container>
TEST_CONSTEXPR_CXX14 void assert_contiguous(Container const& c)
{
    for (std::size_t i = 0; i < c.size(); ++i)
        TC_ASSERT_EXPR(*(c.begin() + i) == *(std::addressof(*c.begin()) + i));
}

TEST_CONSTEXPR_CXX17 bool tests()
{
    assert_contiguous(std::array<double, 0>());
    assert_contiguous(std::array<double, 1>());
    assert_contiguous(std::array<double, 2>());
    assert_contiguous(std::array<double, 3>());

    assert_contiguous(std::array<char, 0>());
    assert_contiguous(std::array<char, 1>());
    assert_contiguous(std::array<char, 2>());
    assert_contiguous(std::array<char, 3>());

    return true;
}

int tc_containers_sequences_array_contiguous(void) {
    tests();
#if TEST_STD_VER >= 17 // begin() & friends are constexpr in >= C++17 only
    static_assert(tests(), "");
#endif
    return 0;
}
