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

// const_pointer data() const;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

struct Nasty {
    TEST_CONSTEXPR Nasty() : i_(0) {}
    TEST_CONSTEXPR Nasty(int i) : i_(i) {}
    TEST_CONSTEXPR_CXX20 ~Nasty() {}

    Nasty * operator&() const { TC_ASSERT_EXPR(false); return nullptr; }
    int i_;
};

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        const std::vector<int> v;
        TC_ASSERT_EXPR(v.data() == 0);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        const std::vector<int> v(100);
        TC_ASSERT_EXPR(v.data() == std::addressof(v.front()));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<Nasty> v(100);
        TC_ASSERT_EXPR(v.data() == std::addressof(v.front()));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#if TEST_STD_VER >= 11
    {
        const std::vector<int, min_allocator<int>> v;
        TC_ASSERT_EXPR(v.data() == 0);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        const std::vector<int, min_allocator<int>> v(100);
        TC_ASSERT_EXPR(v.data() == &v.front());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<Nasty, min_allocator<Nasty>> v(100);
        TC_ASSERT_EXPR(v.data() == std::addressof(v.front()));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      const std::vector<int, safe_allocator<int>> v;
      TC_ASSERT_EXPR(v.data() == 0);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      const std::vector<int, safe_allocator<int>> v(100);
      TC_ASSERT_EXPR(v.data() == &v.front());
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      std::vector<Nasty, safe_allocator<Nasty>> v(100);
      TC_ASSERT_EXPR(v.data() == std::addressof(v.front()));
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_data_data_const(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
