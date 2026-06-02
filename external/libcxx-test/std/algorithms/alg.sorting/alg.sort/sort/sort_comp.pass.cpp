//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>
//
// template<RandomAccessIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires ShuffleIterator<Iter>
//         && CopyConstructible<Compare>
//   void
//   sort(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <functional>
#include <vector>
#include <cassert>
#include <cstddef>
#include <memory>

#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {

struct indirect_less
{
    template <class P>
    bool operator()(const P& x, const P& y)
        {return *x < *y;}
};

void test_sort_comp_vector_int_greater()
{
    std::vector<int> v(1000);
    for (int i = 0; static_cast<std::size_t>(i) < v.size(); ++i)
        v[i] = i;
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::reverse(v.begin(), v.end());
    TC_ASSERT_EXPR(std::is_sorted(v.begin(), v.end()));
}

#if TEST_STD_VER >= 11
void test_sort_comp_vector_unique_ptr_indirect()
{
    std::vector<std::unique_ptr<int> > v(1000);
    for (int i = 0; static_cast<std::size_t>(i) < v.size(); ++i)
        v[i].reset(new int(i));
    std::sort(v.begin(), v.end(), indirect_less());
    TC_ASSERT_EXPR(std::is_sorted(v.begin(), v.end(), indirect_less()));
    TC_ASSERT_EXPR(*v[0] == 0);
    TC_ASSERT_EXPR(*v[1] == 1);
    TC_ASSERT_EXPR(*v[2] == 2);
}
#endif

} 

int tc_libcxx_algorithms_alg_sorting_alg_sort_sort_sort_comp(void) {
    test_sort_comp_vector_int_greater();

#if TEST_STD_VER >= 11
    test_sort_comp_vector_unique_ptr_indirect();
#endif

    TC_SUCCESS_RESULT();

    return 0;
}
