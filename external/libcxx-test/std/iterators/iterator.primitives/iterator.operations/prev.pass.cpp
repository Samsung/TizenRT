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

// template <BidirectionalIterator Iter>
//   Iter prev(Iter x, Iter::difference_type n = 1);

#include <iterator>
#include <cassert>

#include "test_iterators.h"

template <class It>
void
test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    assert(std::prev(i, n) == x);

   /* Fix:
    It (*prev)(It, typename std::iterator_traits<It>::difference_type) = std::prev;
    assert(prev(i, n) == x);
    */
}

template <class It>
void
test(It i, It x)
{
    assert(std::prev(i) == x);
}

#if TEST_STD_VER > 14
template <class It>
constexpr bool
constexpr_test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    return std::prev(i, n) == x;
}

template <class It>
constexpr bool
constexpr_test(It i, It x)
{
    return std::prev(i) == x;
}
#endif

int main()
{
    {
    const char* s = "1234567890";
    test(bidirectional_iterator<const char*>(s+10), 10, bidirectional_iterator<const char*>(s));
    test(random_access_iterator<const char*>(s+10), 10, random_access_iterator<const char*>(s));
    test(s+10, 10, s);

    test(bidirectional_iterator<const char*>(s+1), bidirectional_iterator<const char*>(s));
    test(random_access_iterator<const char*>(s+1), random_access_iterator<const char*>(s));
    test(s+1, s);
    }
#if TEST_STD_VER > 14
    {
    constexpr const char* s = "1234567890";
    static_assert( constexpr_test(bidirectional_iterator<const char*>(s+10), 10, bidirectional_iterator<const char*>(s)), "" );
    static_assert( constexpr_test(random_access_iterator<const char*>(s+10), 10, random_access_iterator<const char*>(s)), "" );
    static_assert( constexpr_test(s+10, 10, s), "" );

    static_assert( constexpr_test(bidirectional_iterator<const char*>(s+1), bidirectional_iterator<const char*>(s)), "" );
    static_assert( constexpr_test(random_access_iterator<const char*>(s+1), random_access_iterator<const char*>(s)), "" );
    static_assert( constexpr_test(s+1, s), "" );
    }
#endif

}
