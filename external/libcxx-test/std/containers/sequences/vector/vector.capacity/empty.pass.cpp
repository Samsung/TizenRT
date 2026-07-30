//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <vector>

// class vector

// bool empty() const noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests() {
    {
    typedef std::vector<int> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    TC_ASSERT_EXPR(c.empty());
    c.push_back(C::value_type(1));
    TC_ASSERT_EXPR(!c.empty());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::vector<int, min_allocator<int>> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    TC_ASSERT_EXPR(c.empty());
    c.push_back(C::value_type(1));
    TC_ASSERT_EXPR(!c.empty());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    }
    {
      typedef std::vector<int, safe_allocator<int>> C;
      C c;
      ASSERT_NOEXCEPT(c.empty());
      TC_ASSERT_EXPR(c.empty());
      c.push_back(C::value_type(1));
      TC_ASSERT_EXPR(!c.empty());
      c.clear();
      TC_ASSERT_EXPR(c.empty());
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_capacity_empty(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
