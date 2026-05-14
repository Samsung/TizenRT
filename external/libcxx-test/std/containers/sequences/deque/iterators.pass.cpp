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
// <deque>

// Test nested types and default template args:

// template <class T, class Allocator = allocator<T> >
// class deque;

// iterator, const_iterator

#include <deque>
#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_iterators(void) {
    {
    typedef std::deque<int> C;
    C c;
    C::iterator i;
    i = c.begin();
    C::const_iterator j;
    j = c.cbegin();
    TC_ASSERT_EXPR(i == j);
    }
#if TEST_STD_VER >= 11
    {
    typedef std::deque<int, min_allocator<int>> C;
    C c;
    C::iterator i;
    i = c.begin();
    C::const_iterator j;
    j = c.cbegin();
    TC_ASSERT_EXPR(i == j);
    }
#endif
#if TEST_STD_VER > 11
    { // N3644 testing
        std::deque<int>::iterator ii1{}, ii2{};
        std::deque<int>::iterator ii4 = ii1;
        std::deque<int>::const_iterator cii{};
        TC_ASSERT_EXPR( ii1 == ii2 );
        TC_ASSERT_EXPR( ii1 == ii4 );

        TC_ASSERT_EXPR(!(ii1 != ii2 ));

        TC_ASSERT_EXPR( (ii1 == cii ));
        TC_ASSERT_EXPR( (cii == ii1 ));
        TC_ASSERT_EXPR(!(ii1 != cii ));
        TC_ASSERT_EXPR(!(cii != ii1 ));
        TC_ASSERT_EXPR(!(ii1 <  cii ));
        TC_ASSERT_EXPR(!(cii <  ii1 ));
        TC_ASSERT_EXPR( (ii1 <= cii ));
        TC_ASSERT_EXPR( (cii <= ii1 ));
        TC_ASSERT_EXPR(!(ii1 >  cii ));
        TC_ASSERT_EXPR(!(cii >  ii1 ));
        TC_ASSERT_EXPR( (ii1 >= cii ));
        TC_ASSERT_EXPR( (cii >= ii1 ));
        TC_ASSERT_EXPR(cii - ii1 == 0);
        TC_ASSERT_EXPR(ii1 - cii == 0);

//         std::deque<int> c;
//         TC_ASSERT_EXPR( ii1 != c.cbegin());
//         TC_ASSERT_EXPR( cii != c.begin());
//         TC_ASSERT_EXPR( cii != c.cend());
//         TC_ASSERT_EXPR( ii1 != c.end());
    }
#endif

  return 0;
}
