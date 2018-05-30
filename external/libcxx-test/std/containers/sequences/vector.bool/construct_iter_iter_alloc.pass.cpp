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

// <vector>
// vector<bool>

// template <class InputIter> vector(InputIter first, InputIter last,
//                                   const allocator_type& a);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"

template <class C, class Iterator>
static int
test(Iterator first, Iterator last, const typename C::allocator_type& a)
{
    C c(first, last, a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == static_cast<std::size_t>(std::distance(first, last)));
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i, ++first)
        TC_ASSERT_EXPR(*i == *first);
    return 0;
}

int tc_libcxx_containers_vector_bool_construct_iter_iter_alloc(void)
{
    bool a[] = {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0};
    bool* an = a + sizeof(a)/sizeof(a[0]);
    {
    std::allocator<bool> alloc;
    TC_ASSERT_FUNC((test<std::vector<bool> >(input_iterator<const bool*>(a), input_iterator<const bool*>(an), alloc)));
    TC_ASSERT_FUNC((test<std::vector<bool> >(forward_iterator<const bool*>(a), forward_iterator<const bool*>(an), alloc)));
    TC_ASSERT_FUNC((test<std::vector<bool> >(bidirectional_iterator<const bool*>(a), bidirectional_iterator<const bool*>(an), alloc)));
    TC_ASSERT_FUNC((test<std::vector<bool> >(random_access_iterator<const bool*>(a), random_access_iterator<const bool*>(an), alloc)));
    TC_ASSERT_FUNC((test<std::vector<bool> >(a, an, alloc)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
