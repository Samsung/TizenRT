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
// <vector>
// vector<bool>

// template <class InputIter> vector(InputIter first, InputIter last);

#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C, class Iterator>
TEST_CONSTEXPR_CXX20 void test(Iterator first, Iterator last)
{
    C c(first, last);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == static_cast<std::size_t>(std::distance(first, last)));
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i, ++first)
        TC_ASSERT_EXPR(*i == *first);
}

TEST_CONSTEXPR_CXX20 bool tests()
{
    bool a[] = {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0};
    bool* an = a + sizeof(a)/sizeof(a[0]);
    test<std::vector<bool> >(cpp17_input_iterator<const bool*>(a), cpp17_input_iterator<const bool*>(an));
    test<std::vector<bool> >(forward_iterator<const bool*>(a), forward_iterator<const bool*>(an));
    test<std::vector<bool> >(bidirectional_iterator<const bool*>(a), bidirectional_iterator<const bool*>(an));
    test<std::vector<bool> >(random_access_iterator<const bool*>(a), random_access_iterator<const bool*>(an));
    test<std::vector<bool> >(a, an);
#if TEST_STD_VER >= 11
    test<std::vector<bool, min_allocator<bool>> >(cpp17_input_iterator<const bool*>(a), cpp17_input_iterator<const bool*>(an));
    test<std::vector<bool, min_allocator<bool>> >(forward_iterator<const bool*>(a), forward_iterator<const bool*>(an));
    test<std::vector<bool, min_allocator<bool>> >(bidirectional_iterator<const bool*>(a), bidirectional_iterator<const bool*>(an));
    test<std::vector<bool, min_allocator<bool>> >(random_access_iterator<const bool*>(a), random_access_iterator<const bool*>(an));
    test<std::vector<bool, min_allocator<bool>> >(a, an);
#endif

    return true;
}

int tc_containers_sequences_vector_bool_construct_iter_iter(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
