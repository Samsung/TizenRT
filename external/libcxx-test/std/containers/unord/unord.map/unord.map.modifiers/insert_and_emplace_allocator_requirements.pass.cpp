//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <unordered_map>

// class unordered_map

// insert(...);
// emplace(...);

// UNSUPPORTED: c++03


#include <unordered_map>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../../map_allocator_requirement_test_templates.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_map_unord_map_modifiers_insert_and_emplace_allocator_requirements(void) {
  testMapInsert<TCT::unordered_map<> >();
  testMapInsertHint<TCT::unordered_map<> >();
  testMapEmplace<TCT::unordered_map<> >();
  testMapEmplaceHint<TCT::unordered_map<> >();

  return 0;
}
