//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <deque>

// class deque

// allocator_type get_allocator() const

#include <deque>
#include <cassert>

#include "test_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_get_allocator(void) {
    {
        std::allocator<int> alloc;
        const std::deque<int> d(alloc);
        TC_ASSERT_EXPR(d.get_allocator() == alloc);
    }
    {
        other_allocator<int> alloc(1);
        const std::deque<int, other_allocator<int> > d(alloc);
        TC_ASSERT_EXPR(d.get_allocator() == alloc);
    }

    return 0;
}
