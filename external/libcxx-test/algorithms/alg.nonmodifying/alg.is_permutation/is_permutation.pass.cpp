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
// <algorithm>

// template<class ForwardIterator1, class ForwardIterator2>
//   constexpr bool   // constexpr after C++17
//   is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
//                  ForwardIterator2 first2);

#include <algorithm>
#include <cassert>

#include "test_iterators.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {0, 0, 0};
    int ib[] = {1, 1, 0};
    int ic[] = {1, 0, 1};
    int id[] = {1};
    return !std::is_permutation(std::begin(ia), std::end(ia), std::begin(ib))
        && !std::is_permutation(std::begin(ia), std::end(ia), std::begin(ib), std::end(ib))
        &&  std::is_permutation(std::begin(ib), std::end(ib), std::begin(ic))
        &&  std::is_permutation(std::begin(ib), std::end(ib), std::begin(ic), std::end(ic))
        && !std::is_permutation(std::begin(ic), std::end(ic), std::begin(id), std::end(id))
        ;
    }
#endif

int tc_algorithms_alg_nonmodifying_alg_is_permutation_is_permutation(void) {
    {
        const int ia[] = {0};
        const int ib[] = {0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + 0),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + 0),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + 0)) == true);
#endif
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0};
        const int ib[] = {1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }

    {
        const int ia[] = {0, 0};
        const int ib[] = {0, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 0};
        const int ib[] = {0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0};
        const int ib[] = {1, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0};
        const int ib[] = {1, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 1};
        const int ib[] = {0, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 1};
        const int ib[] = {0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 1};
        const int ib[] = {1, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
#endif
    }
    {
        const int ia[] = {0, 1};
        const int ib[] = {1, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {1, 0};
        const int ib[] = {0, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {1, 0};
        const int ib[] = {0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
#endif
    }
    {
        const int ia[] = {1, 0};
        const int ib[] = {1, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
#endif
    }
    {
        const int ia[] = {1, 0};
        const int ib[] = {1, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {1, 1};
        const int ib[] = {0, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {1, 1};
        const int ib[] = {0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {1, 1};
        const int ib[] = {1, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {1, 1};
        const int ib[] = {1, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
#endif
    }

    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 0, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 0, 2};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 1, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 1, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 1, 2};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 2, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 2, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 0};
        const int ib[] = {1, 2, 2};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 1};
        const int ib[] = {1, 0, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 1};
        const int ib[] = {1, 0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 1, 2};
        const int ib[] = {1, 0, 2};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 1, 2};
        const int ib[] = {1, 2, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 1, 2};
        const int ib[] = {2, 1, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 1, 2};
        const int ib[] = {2, 0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 1};
        const int ib[] = {1, 0, 1};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }
    {
        const int ia[] = {0, 0, 1};
        const int ib[] = {1, 0, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib + 1),
                                   forward_iterator<const int*>(ib + sa)) == false);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 1, 2, 3, 0, 5, 6, 2, 4, 4};
        const int ib[] = {4, 2, 3, 0, 1, 4, 0, 5, 6, 2};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == true);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == true);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib + 1 ),
                                   forward_iterator<const int*>(ib + sa)) == false);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa - 1)) == false);
#endif
    }
    {
        const int ia[] = {0, 1, 2, 3, 0, 5, 6, 2, 4, 4};
        const int ib[] = {4, 2, 3, 0, 1, 4, 0, 5, 6, 0};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib)) == false);
#if TEST_STD_VER >= 14
        TC_ASSERT_EXPR(std::is_permutation(forward_iterator<const int*>(ia),
                                   forward_iterator<const int*>(ia + sa),
                                   forward_iterator<const int*>(ib),
                                   forward_iterator<const int*>(ib + sa)) == false);
#endif
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
