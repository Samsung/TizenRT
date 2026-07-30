//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <vector>

// void clear() noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
    int a[] = {1, 2, 3};
    std::vector<int> c(a, a+3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    LIBCPP_ASSERT(c.__invariants());
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
    }
#if TEST_STD_VER >= 11
    {
    int a[] = {1, 2, 3};
    std::vector<int, min_allocator<int>> c(a, a+3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    LIBCPP_ASSERT(c.__invariants());
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
    }
    {
      int a[] = {1, 2, 3};
      std::vector<int, safe_allocator<int>> c(a, a + 3);
      ASSERT_NOEXCEPT(c.clear());
      c.clear();
      TC_ASSERT_EXPR(c.empty());
      LIBCPP_ASSERT(c.__invariants());
      LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_clear(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
