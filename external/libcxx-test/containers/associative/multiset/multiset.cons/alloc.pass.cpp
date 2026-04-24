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

// class multiset

// multiset(const allocator_type& a);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_alloc(void) {
    typedef std::less<int> C;
    typedef test_allocator<int> A;
    std::multiset<int, C, A> m(A(5));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.get_allocator() == A(5));

  return 0;
}
