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

// weak_ptr

// template<class U> bool owner_before(const weak_ptr<U>& b) const noexcept;

#include <memory>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_owner_before_weak_ptr(void) {
    const std::shared_ptr<int> p1(new int);
    const std::shared_ptr<int> p2 = p1;
    const std::shared_ptr<int> p3(new int);
    const std::weak_ptr<int> w1(p1);
    const std::weak_ptr<int> w2(p2);
    const std::weak_ptr<int> w3(p3);
    TC_ASSERT_EXPR(!w1.owner_before(w2));
    TC_ASSERT_EXPR(!w2.owner_before(w1));
    TC_ASSERT_EXPR(w1.owner_before(w3) || w3.owner_before(w1));
    TC_ASSERT_EXPR(w3.owner_before(w1) == w3.owner_before(w2));
    ASSERT_NOEXCEPT(w1.owner_before(w2));

  return 0;
}
