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
// <list>

// ~list()

// no emplace_back in C++03
// UNSUPPORTED: c++03

#include <list>
#include <cassert>
#include <set>

#include "test_macros.h"
#include "libcxx_tc_common.h"


std::set<int> destroyed;

struct Foo {
    explicit Foo(int i) : value(i) { }
    ~Foo() { destroyed.insert(value); }
    int value;
};

int tc_containers_sequences_list_list_cons_dtor(void) {
    {
        std::list<Foo> list;
        list.emplace_back(1);
        list.emplace_back(2);
        list.emplace_back(3);
        TC_ASSERT_EXPR(destroyed.empty());
    }
    TC_ASSERT_EXPR(destroyed.count(1) == 1);
    TC_ASSERT_EXPR(destroyed.count(2) == 1);
    TC_ASSERT_EXPR(destroyed.count(3) == 1);

    return 0;
}
