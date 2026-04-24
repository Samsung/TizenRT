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

// explicit set(const key_compare& comp) const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_set_cons_compare(void) {
    typedef test_less<int> C;
    const std::set<int, C> m(C(3));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    TC_ASSERT_EXPR(m.value_comp() == C(3));

  return 0;
}
