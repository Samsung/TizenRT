//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// void swap(priority_queue& c)
//     noexcept(__is_nothrow_swappable<container_type>::value &&
//              __is_nothrow_swappable<Compare>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <queue>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_special_swap_noexcept(void) {
    {
        typedef std::priority_queue<MoveOnly> C;
        static_assert(noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
    }

  return 0;
}
