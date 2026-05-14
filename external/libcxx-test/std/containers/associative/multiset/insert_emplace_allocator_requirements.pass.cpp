//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class multiset

// insert(...)

// UNSUPPORTED: c++03

#include <set>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../set_allocator_requirement_test_templates.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_insert_emplace_allocator_requirements(void) {
  testMultisetInsert<TCT::multiset<> >();
  testMultisetEmplace<TCT::multiset<> >();

  return 0;
}
