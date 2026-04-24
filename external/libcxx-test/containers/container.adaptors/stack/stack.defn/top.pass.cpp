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
// <stack>

// reference top();

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_stack_stack_defn_top(void) {
    std::stack<int> q;
    TC_ASSERT_EXPR(q.size() == 0);
    q.push(1);
    q.push(2);
    q.push(3);
    int& ir = q.top();
    TC_ASSERT_EXPR(ir == 3);

  return 0;
}
