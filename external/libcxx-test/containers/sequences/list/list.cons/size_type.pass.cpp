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
// <list>

// explicit list(size_type n);

#include <list>
#include <cassert>
#include <cstddef>
#include "test_macros.h"
#include "DefaultOnly.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class T, class Allocator>
void
test3(unsigned n, Allocator const &alloc = Allocator())
{
#if TEST_STD_VER > 11
    typedef std::list<T, Allocator> C;
    {
    C d(n, alloc);
    TC_ASSERT_EXPR(d.size() == n);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == n);
    TC_ASSERT_EXPR(d.get_allocator() == alloc);
    }
#else
    ((void)n);
    ((void)alloc);
#endif
}


int tc_containers_sequences_list_list_cons_size_type(void) {
    {
        std::list<int> l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
    }
    {
        // Add 2 for implementations that dynamically allocate a sentinel node and container proxy.
        std::list<int, limited_allocator<int, 3 + 2> > l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
    }
#if TEST_STD_VER > 11
    {
        typedef std::list<int, min_allocator<int> > C;
        C l(3, min_allocator<int> ());
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        C::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
        test3<int, min_allocator<int>> (3);
    }
#endif
#if TEST_STD_VER >= 11
    {
        std::list<DefaultOnly> l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
    }
    {
        std::list<int, min_allocator<int>> l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int, min_allocator<int>>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 0);
    }
    {
        std::list<DefaultOnly, min_allocator<DefaultOnly>> l(3);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
    }
#endif

  return 0;
}
