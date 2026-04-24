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

// <queue>

// priority_queue();

// void push(value_type&& v);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_members_push_rvalue(void) {
    std::priority_queue<MoveOnly> q;
    q.push(1);
    TC_ASSERT_EXPR(q.top() == 1);
    q.push(3);
    TC_ASSERT_EXPR(q.top() == 3);
    q.push(2);
    TC_ASSERT_EXPR(q.top() == 3);

  return 0;
}
