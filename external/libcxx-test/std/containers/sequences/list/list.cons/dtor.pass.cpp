//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
