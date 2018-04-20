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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// template <class U>
//   requires HasConstructor<Iter, const U&>
//   constexpr reverse_iterator(const reverse_iterator<U> &u);
//
// constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class It, class U>
static int
test(U u)
{
    const std::reverse_iterator<U> r2(u);
    std::reverse_iterator<It> r1 = r2;
    TC_ASSERT_EXPR(r1.base() == u);
    return 0;
}

struct Base {};
struct Derived : Base {};

int tc_libcxx_iterators_reverse_iter_cons_reverse_iterator(void)
{
    Derived d;

    TC_ASSERT_FUNC((test<bidirectional_iterator<Base*> >(bidirectional_iterator<Derived*>(&d))));
    TC_ASSERT_FUNC((test<random_access_iterator<const Base*> >(random_access_iterator<Derived*>(&d))));
    TC_ASSERT_FUNC((test<Base*>(&d)));

#if TEST_STD_VER > 14
    {
        constexpr const Derived *p = nullptr;
        constexpr std::reverse_iterator<const Derived *>     it1 = std::make_reverse_iterator(p);
        constexpr std::reverse_iterator<const Base *>        it2(it1);
        static_assert(it2.base() == p);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
