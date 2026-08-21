//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <deque>

// explicit deque(const allocator_type& a);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "../../../NotConstructible.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class T, class Allocator>
void
test(const Allocator& a)
{
    std::deque<T, Allocator> d(a);
    TC_ASSERT_EXPR(d.size() == 0);
    TC_ASSERT_EXPR(d.get_allocator() == a);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
}

int tc_containers_sequences_deque_deque_cons_alloc(void) {
    test<int>(std::allocator<int>());
    test<NotConstructible>(test_allocator<NotConstructible>(3));
#if TEST_STD_VER >= 11
    test<int>(min_allocator<int>());
    test<int>(safe_allocator<int>());
    test<NotConstructible>(min_allocator<NotConstructible>{});
    test<NotConstructible>(safe_allocator<NotConstructible>{});
    test<int>(explicit_allocator<int>());
    test<NotConstructible>(explicit_allocator<NotConstructible>{});
#endif

  return 0;
}
