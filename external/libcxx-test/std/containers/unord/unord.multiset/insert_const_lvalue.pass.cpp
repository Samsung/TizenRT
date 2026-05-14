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
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// iterator insert(const value_type& x);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template<class Container>
void do_insert_const_lvalue_test()
{
    typedef Container C;
    typedef typename C::iterator R;
    typedef typename C::value_type VT;
    C c;
    const VT v1(3.5);
    R r = c.insert(v1);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(*r == 3.5);

    r = c.insert(v1);
    TC_ASSERT_EXPR(c.size() == 2);
    TC_ASSERT_EXPR(*r == 3.5);

    const VT v2(4.5);
    r = c.insert(v2);
    TC_ASSERT_EXPR(c.size() == 3);
    TC_ASSERT_EXPR(*r == 4.5);

    const VT v3(5.5);
    r = c.insert(v3);
    TC_ASSERT_EXPR(c.size() == 4);
    TC_ASSERT_EXPR(*r == 5.5);
}

int tc_containers_unord_unord_multiset_insert_const_lvalue(void) {
    do_insert_const_lvalue_test<std::unordered_multiset<double> >();
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multiset<double, std::hash<double>,
            std::equal_to<double>, min_allocator<double>> C;
        do_insert_const_lvalue_test<C>();
    }
#endif

  return 0;
}
