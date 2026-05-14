//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <typeindex>

// class type_index

// type_index(const type_index& ti);

// UNSUPPORTED: no-rtti

#include <typeindex>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_type_index_type_index_overview_copy_ctor(void) {
    std::type_index t1(typeid(int));
    std::type_index t2 = t1;
    TC_ASSERT_EXPR(t2 == t1);

  return 0;
}
