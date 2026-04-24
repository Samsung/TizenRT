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
 ****************************************************************************///
//
// <algorithm>
// UNSUPPORTED: c++03, c++11, c++14

// template<class InputIterator, class Size, class Function>
//    constexpr InputIterator      // constexpr after C++17
//    for_each_n(InputIterator first, Size n, Function f);


#include <algorithm>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

namespace {
#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 6, 7};
    int expected[] = {3, 5, 8, 9};
    const std::size_t N = 4;

    auto it = std::for_each_n(std::begin(ia), N, [](int &a) { a += 2; });
    return it == (std::begin(ia) + N)
        && std::equal(std::begin(ia), std::end(ia), std::begin(expected))
        ;
    }
#endif

struct for_each_test
{
    for_each_test(int c) : count(c) {}
    int count;
    void operator()(int& i) {++i; ++count;}
};

} // namespace

int tc_libcxx_algorithms_alg_nonmodifying_alg_foreach_for_each_n(void) {
    typedef cpp17_input_iterator<int*> Iter;
    int ia[] = {0, 1, 2, 3, 4, 5};
    const unsigned s = sizeof(ia)/sizeof(ia[0]);

    {
    auto f = for_each_test(0);
    Iter it = std::for_each_n(Iter(ia), 0, std::ref(f));
    TC_ASSERT_EXPR(it == Iter(ia));
    TC_ASSERT_EXPR(f.count == 0);
    }

    {
    auto f = for_each_test(0);
    Iter it = std::for_each_n(Iter(ia), s, std::ref(f));

    TC_ASSERT_EXPR(it == Iter(ia+s));
    TC_ASSERT_EXPR(f.count == s);
    for (unsigned i = 0; i < s; ++i)
        TC_ASSERT_EXPR(ia[i] == static_cast<int>(i+1));
    }

    {
    auto f = for_each_test(0);
    Iter it = std::for_each_n(Iter(ia), 1, std::ref(f));

    TC_ASSERT_EXPR(it == Iter(ia+1));
    TC_ASSERT_EXPR(f.count == 1);
    for (unsigned i = 0; i < 1; ++i)
        TC_ASSERT_EXPR(ia[i] == static_cast<int>(i+2));
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
