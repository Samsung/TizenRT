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
// UNSUPPORTED: c++03

// <list>

// void push_back(value_type&& x);

#include <list>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_modifiers_push_back_rvalue(void) {
    {
    std::list<MoveOnly> l1;
    l1.push_back(MoveOnly(1));
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(1));
    l1.push_back(MoveOnly(2));
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(1));
    TC_ASSERT_EXPR(l1.back() == MoveOnly(2));
    }
    {
    std::list<MoveOnly, min_allocator<MoveOnly>> l1;
    l1.push_back(MoveOnly(1));
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(1));
    l1.push_back(MoveOnly(2));
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(1));
    TC_ASSERT_EXPR(l1.back() == MoveOnly(2));
    }

  return 0;
}
