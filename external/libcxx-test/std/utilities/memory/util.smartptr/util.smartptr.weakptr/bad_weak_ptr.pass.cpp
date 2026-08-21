//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// class bad_weak_ptr
//     : public std::exception
// {
// public:
//     bad_weak_ptr();
// };

#include <exception>
#include <memory>
#include <type_traits>
#include <cassert>
#include <cstring>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_weakptr_bad_weak_ptr(void) {
    static_assert((std::is_base_of<std::exception, std::bad_weak_ptr>::value), "");
    std::bad_weak_ptr e;
    std::bad_weak_ptr e2 = e;
    e2 = e;
    TC_ASSERT_EXPR(std::strcmp(e.what(), "bad_weak_ptr") == 0);

  return 0;
}
