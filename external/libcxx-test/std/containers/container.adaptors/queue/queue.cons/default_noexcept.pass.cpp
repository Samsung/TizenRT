//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <queue>

// queue()
//        noexcept(is_nothrow_default_constructible<container_type>::value);

// This tests a conforming extension

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_queue_queue_cons_default_noexcept(void) {
#if defined(_LIBCPP_VERSION)
    {
        typedef std::queue<MoveOnly> C;
        static_assert(std::is_nothrow_default_constructible<C>::value, "");
    }
#endif

  return 0;
}
