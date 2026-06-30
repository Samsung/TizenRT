//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class set

// allocator_type get_allocator() const

#include <set>
#include <cassert>

#include "test_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_get_allocator(void) {
    {
        std::allocator<int> alloc;
        const std::set<int> s(alloc);
        TC_ASSERT_EXPR(s.get_allocator() == alloc);
    }
    {
        other_allocator<int> alloc(1);
        const std::set<int, std::less<int>, other_allocator<int> > s(alloc);
        TC_ASSERT_EXPR(s.get_allocator() == alloc);
    }
    return 0;
}
