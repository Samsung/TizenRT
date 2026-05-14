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
// <set>

// class set

// bool empty() const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_empty(void) {
    {
    typedef std::set<int> M;
    M m;
    TC_ASSERT_EXPR(m.empty());
    m.insert(M::value_type(1));
    TC_ASSERT_EXPR(!m.empty());
    m.clear();
    TC_ASSERT_EXPR(m.empty());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::set<int, std::less<int>, min_allocator<int>> M;
    M m;
    TC_ASSERT_EXPR(m.empty());
    m.insert(M::value_type(1));
    TC_ASSERT_EXPR(!m.empty());
    m.clear();
    TC_ASSERT_EXPR(m.empty());
    }
#endif

  return 0;
}
