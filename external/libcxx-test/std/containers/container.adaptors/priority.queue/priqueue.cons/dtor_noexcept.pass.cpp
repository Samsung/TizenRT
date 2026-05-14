//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// ~priority_queue() // implied noexcept;

// UNSUPPORTED: c++03

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_cons_dtor_noexcept(void) {
    {
        typedef std::priority_queue<MoveOnly> C;
        static_assert(std::is_nothrow_destructible<C>::value, "");
    }

  return 0;
}
