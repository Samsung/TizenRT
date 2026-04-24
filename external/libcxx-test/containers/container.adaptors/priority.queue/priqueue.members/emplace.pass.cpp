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

// template <class... Args> void emplace(Args&&... args);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "../../../Emplaceable.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_members_emplace(void) {
    std::priority_queue<Emplaceable> q;
    q.emplace(1, 2.5);
    TC_ASSERT_EXPR(q.top() == Emplaceable(1, 2.5));
    q.emplace(3, 4.5);
    TC_ASSERT_EXPR(q.top() == Emplaceable(3, 4.5));
    q.emplace(2, 3.5);
    TC_ASSERT_EXPR(q.top() == Emplaceable(3, 4.5));

  return 0;
}
