//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class set

// bool empty() const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_empty(void) {
    {
    typedef std::set<int> M;
    M m;
    TC_ASSERT_EXPR(m.empty());
    m.insert(M::value_type(1));
    TC_ASSERT_EXPR(!m.empty());
    m.clear();
    TC_ASSERT_EXPR(m.empty());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::set<int, std::less<int>, min_allocator<int>> M;
    M m;
    TC_ASSERT_EXPR(m.empty());
    m.insert(M::value_type(1));
    TC_ASSERT_EXPR(!m.empty());
    m.clear();
    TC_ASSERT_EXPR(m.empty());
    }
#endif

  return 0;
}
