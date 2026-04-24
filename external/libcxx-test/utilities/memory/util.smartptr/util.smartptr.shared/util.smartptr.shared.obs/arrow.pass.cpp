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
// <memory>

// shared_ptr

// T* operator->() const;

#include <memory>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow(void) {
    const std::shared_ptr<std::pair<int, int> > p(new std::pair<int, int>(3, 4));
    TC_ASSERT_EXPR(p->first == 3);
    TC_ASSERT_EXPR(p->second == 4);
    p->first = 5;
    p->second = 6;
    TC_ASSERT_EXPR(p->first == 5);
    TC_ASSERT_EXPR(p->second == 6);

  return 0;
}
