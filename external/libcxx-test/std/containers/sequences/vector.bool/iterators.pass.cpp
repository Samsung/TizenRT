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

// <vector>

// iterator       begin();
// iterator       end();
// const_iterator begin()  const;
// const_iterator end()    const;
// const_iterator cbegin() const;
// const_iterator cend()   const;

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include <iterator>

#include "test_macros.h"

int tc_libcxx_containers_vector_bool_iterators(void)
{
    {
        typedef bool T;
        typedef std::vector<T> C;
        C c;
        C::iterator i = c.begin();
        C::iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef bool T;
        typedef std::vector<T> C;
        const C c;
        C::const_iterator i = c.begin();
        C::const_iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef bool T;
        typedef std::vector<T> C;
        C c;
        C::const_iterator i = c.cbegin();
        C::const_iterator j = c.cend();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
        TC_ASSERT_EXPR(i == c.end());
    }
    {
        typedef bool T;
        typedef std::vector<T> C;
        C::iterator i;
        C::const_iterator j;
    }
#if TEST_STD_VER > 11
    { // N3644 testing
        std::vector<bool>::iterator ii1{}, ii2{};
        std::vector<bool>::iterator ii4 = ii1;
        std::vector<bool>::const_iterator cii{};
        TC_ASSERT_EXPR ( ii1 == ii2 );
        TC_ASSERT_EXPR ( ii1 == ii4 );

        TC_ASSERT_EXPR (!(ii1 != ii2 ));

        TC_ASSERT_EXPR ( (ii1 == cii ));
        TC_ASSERT_EXPR ( (cii == ii1 ));
        TC_ASSERT_EXPR (!(ii1 != cii ));
        TC_ASSERT_EXPR (!(cii != ii1 ));
        TC_ASSERT_EXPR (!(ii1 <  cii ));
        TC_ASSERT_EXPR (!(cii <  ii1 ));
        TC_ASSERT_EXPR ( (ii1 <= cii ));
        TC_ASSERT_EXPR ( (cii <= ii1 ));
        TC_ASSERT_EXPR (!(ii1 >  cii ));
        TC_ASSERT_EXPR (!(cii >  ii1 ));
        TC_ASSERT_EXPR ( (ii1 >= cii ));
        TC_ASSERT_EXPR ( (cii >= ii1 ));
        TC_ASSERT_EXPR (cii - ii1 == 0);
        TC_ASSERT_EXPR (ii1 - cii == 0);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
