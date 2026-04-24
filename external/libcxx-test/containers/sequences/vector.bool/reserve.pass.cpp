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

// void reserve(size_type n);

#include <vector>
#include <cassert>
#include <stdexcept>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        std::vector<bool> v;
        v.reserve(10);
        TC_ASSERT_EXPR(v.capacity() >= 10);
    }
    {
        std::vector<bool> v(100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() >= 150);
    }
#if TEST_STD_VER >= 11
    {
        std::vector<bool, min_allocator<bool>> v;
        v.reserve(10);
        TC_ASSERT_EXPR(v.capacity() >= 10);
    }
    {
        std::vector<bool, explicit_allocator<bool>> v;
        v.reserve(10);
        TC_ASSERT_EXPR(v.capacity() >= 10);
    }
    {
        std::vector<bool, min_allocator<bool>> v(100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() >= 150);
    }
#endif
#ifndef TEST_HAS_NO_EXCEPTIONS
    if (!TEST_IS_CONSTANT_EVALUATED) {
        std::vector<bool, limited_allocator<bool, 10> > v;
        v.reserve(5);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            // A typical implementation would allocate chunks of bits.
            // In libc++ the chunk has the same size as the machine word. It is
            // reasonable to assume that in practice no implementation would use
            // 64 kB or larger chunks.
            v.reserve(10 * 65536);
            TC_ASSERT_EXPR(false);
        } catch (const std::length_error&) {
            // no-op
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(v.capacity() >= 5);
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_reserve(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
