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
//
// UNSUPPORTED: no-threads

// <memory>

// shared_ptr

// template <class T>
// shared_ptr<T>
// atomic_load(const shared_ptr<T>* p)

// UNSUPPORTED: c++03

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_load(void) {
    {
        std::shared_ptr<int> p(new int(3));
        std::shared_ptr<int> q = std::atomic_load(&p);
        TC_ASSERT_EXPR(*q == *p);
    }

  return 0;
}
