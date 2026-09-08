//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <typeindex>

// class type_index

// const char* name() const;

// UNSUPPORTED: no-rtti

#include <typeindex>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_type_index_type_index_members_name(void) {
    const std::type_info& ti = typeid(int);
    std::type_index t1 = typeid(int);
    TC_ASSERT_EXPR(std::string(t1.name()) == ti.name());

  return 0;
}
