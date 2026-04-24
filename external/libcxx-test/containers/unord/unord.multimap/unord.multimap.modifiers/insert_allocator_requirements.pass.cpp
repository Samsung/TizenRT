/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
// <unordered_map>

// class unordered_multimap

// insert(...)

// UNSUPPORTED: c++03

#include <unordered_map>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../../map_allocator_requirement_test_templates.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multimap_unord_multimap_modifiers_insert_allocator_requirements(void) {
  testMultimapInsert<TCT::unordered_multimap<> >();
  testMultimapInsertHint<TCT::unordered_multimap<> >();

  return 0;
}
