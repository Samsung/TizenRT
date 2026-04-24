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

// template<ForwardIterator Iter, Callable Generator>
//   requires OutputIterator<Iter, Generator::result_type>
//         && CopyConstructible<Generator>
//   constexpr void      // constexpr after c++17
//   generate(Iter first, Iter last, Generator gen);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct gen_test
{
    TEST_CONSTEXPR int operator()() const {return 1;}
};


#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {0, 1, 2, 3, 4};

    std::generate(std::begin(ia), std::end(ia), gen_test());

    return std::all_of(std::begin(ia), std::end(ia), [](int x) { return x == 1; })
        ;
    }
#endif


template <class Iter>
void
test()
{
    const unsigned n = 4;
    int ia[n] = {0};
    std::generate(Iter(ia), Iter(ia+n), gen_test());
    TC_ASSERT_EXPR(ia[0] == 1);
    TC_ASSERT_EXPR(ia[1] == 1);
    TC_ASSERT_EXPR(ia[2] == 1);
    TC_ASSERT_EXPR(ia[3] == 1);
}

int tc_algorithms_alg_modifying_operations_alg_generate_generate(void) {
    test<forward_iterator<int*> >();
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
