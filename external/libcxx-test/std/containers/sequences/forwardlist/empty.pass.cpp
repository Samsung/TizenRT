//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <forward_list>

// class forward_list

// bool empty() const noexcept;

#include <forward_list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_forwardlist_empty(void) {
    {
    typedef std::forward_list<int> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    TC_ASSERT_EXPR(c.empty());
    c.push_front(C::value_type(1));
    TC_ASSERT_EXPR(!c.empty());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::forward_list<int, min_allocator<int>> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    TC_ASSERT_EXPR(c.empty());
    c.push_front(C::value_type(1));
    TC_ASSERT_EXPR(!c.empty());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    }
#endif

  return 0;
}
