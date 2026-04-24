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

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_sort_sort_sort_comp(void) {
    {
    std::vector<int> v(1000);
    for (int i = 0; static_cast<std::size_t>(i) < v.size(); ++i)
        v[i] = i;
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::reverse(v.begin(), v.end());
    TC_ASSERT_EXPR(std::is_sorted(v.begin(), v.end()));
    }

#if TEST_STD_VER >= 11
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

  TC_SUCCESS_RESULT();


  return 0;
}
