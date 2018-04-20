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

// move_iterator

// requires RandomAccessIterator<Iter>
//   unspecified operator[](difference_type n) const;
//
//  constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"
#include <memory>

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
static int
test(It i, typename std::iterator_traits<It>::difference_type n,
     typename std::iterator_traits<It>::value_type x)
{
    typedef typename std::iterator_traits<It>::value_type value_type;
    const std::move_iterator<It> r(i);
    value_type rr = r[n];
    TC_ASSERT_EXPR(rr == x);
    return 0;
}

struct do_nothing
{
    void operator()(void*) const {}
};

int tc_libcxx_iterators_move_iter_op_index_difference_type(void)
{
    {
        char s[] = "1234567890";
        test(random_access_iterator<char*>(s+5), 4, '0');
        test(s+5, 4, '0');
    }
#if TEST_STD_VER >= 11
    {
        int i[5];
        typedef std::unique_ptr<int, do_nothing> Ptr;
        Ptr p[5];
        for (unsigned j = 0; j < 5; ++j)
            p[j].reset(i+j);
        test(p, 3, Ptr(i+3));
    }
#endif
#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    typedef std::move_iterator<const char *> MI;
    constexpr MI it1 = std::make_move_iterator(p);
    static_assert(it1[0] == '1', "");
    static_assert(it1[5] == '6', "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
