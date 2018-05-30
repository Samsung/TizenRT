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

// <list>

// iterator       begin();
// iterator       end();
// const_iterator begin()  const;
// const_iterator end()    const;
// const_iterator cbegin() const;
// const_iterator cend()   const;

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <iterator>

#include "test_macros.h"

struct A
{
    int first;
    int second;
};

int tc_libcxx_containers_list_iterators(void)
{
    {
        typedef int T;
        typedef std::list<T> C;
        C c;
        C::iterator i = c.begin();
        C::iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef int T;
        typedef std::list<T> C;
        const C c;
        C::const_iterator i = c.begin();
        C::const_iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef int T;
        typedef std::list<T> C;
        C c;
        C::const_iterator i = c.cbegin();
        C::const_iterator j = c.cend();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
        TC_ASSERT_EXPR(i == c.end());
    }
    {
        typedef int T;
        typedef std::list<T> C;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c(std::begin(t), std::end(t));
        C::iterator i = c.begin();
        TC_ASSERT_EXPR(*i == 0);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
        *i = 10;
        TC_ASSERT_EXPR(*i == 10);
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 10);
    }
    {
        typedef int T;
        typedef std::list<T> C;
        C::iterator i;
        C::const_iterator j;
    }
#if TEST_STD_VER > 11
    {
        std::list<int>::iterator ii1{}, ii2{};
        std::list<int>::iterator ii4 = ii1;
        std::list<int>::const_iterator cii{};
        TC_ASSERT_EXPR ( ii1 == ii2 );
        TC_ASSERT_EXPR ( ii1 == ii4 );

        TC_ASSERT_EXPR (!(ii1 != ii2 ));

        TC_ASSERT_EXPR ( (ii1 == cii ));
        TC_ASSERT_EXPR ( (cii == ii1 ));
        TC_ASSERT_EXPR (!(ii1 != cii ));
        TC_ASSERT_EXPR (!(cii != ii1 ));
    }
#endif

    TC_SUCCESS_RESULT();
    return 0;
}
