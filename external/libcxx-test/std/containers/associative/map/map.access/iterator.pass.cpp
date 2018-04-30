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

// <map>

// class map

//       iterator begin();
// const_iterator begin() const;
//       iterator end();
// const_iterator end()   const;
//
//       reverse_iterator rbegin();
// const_reverse_iterator rbegin() const;
//       reverse_iterator rend();
// const_reverse_iterator rend()   const;
//
// const_iterator         cbegin()  const;
// const_iterator         cend()    const;
// const_reverse_iterator crbegin() const;
// const_reverse_iterator crend()   const;

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"

int tc_libcxx_containers_map_access_iterator(void)
{
    {
        typedef std::pair<const int, double> V;
        V ar[] =
        {
            V(1, 1),
            V(1, 1.5),
            V(1, 2),
            V(2, 1),
            V(2, 1.5),
            V(2, 2),
            V(3, 1),
            V(3, 1.5),
            V(3, 2),
            V(4, 1),
            V(4, 1.5),
            V(4, 2),
            V(5, 1),
            V(5, 1.5),
            V(5, 2),
            V(6, 1),
            V(6, 1.5),
            V(6, 2),
            V(7, 1),
            V(7, 1.5),
            V(7, 2),
            V(8, 1),
            V(8, 1.5),
            V(8, 2)
        };
        std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.rbegin(), m.rend())) == m.size());
        std::map<int, double>::iterator i;
        i = m.begin();
        std::map<int, double>::const_iterator k = i;
        TC_ASSERT_EXPR(i == k);
        for (int j = 1; static_cast<std::size_t>(j) <= m.size(); ++j, ++i)
        {
            TC_ASSERT_EXPR(i->first == j);
            TC_ASSERT_EXPR(i->second == 1);
            i->second = 2.5;
            TC_ASSERT_EXPR(i->second == 2.5);
        }
    }
    {
        typedef std::pair<const int, double> V;
        V ar[] =
        {
            V(1, 1),
            V(1, 1.5),
            V(1, 2),
            V(2, 1),
            V(2, 1.5),
            V(2, 2),
            V(3, 1),
            V(3, 1.5),
            V(3, 2),
            V(4, 1),
            V(4, 1.5),
            V(4, 2),
            V(5, 1),
            V(5, 1.5),
            V(5, 2),
            V(6, 1),
            V(6, 1.5),
            V(6, 2),
            V(7, 1),
            V(7, 1.5),
            V(7, 2),
            V(8, 1),
            V(8, 1.5),
            V(8, 2)
        };
        const std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.cbegin(), m.cend())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.rbegin(), m.rend())) == m.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.crbegin(), m.crend())) == m.size());
        std::map<int, double>::const_iterator i;
        i = m.begin();
        for (int j = 1; static_cast<std::size_t>(j) <= m.size(); ++j, ++i)
        {
            TC_ASSERT_EXPR(i->first == j);
            TC_ASSERT_EXPR(i->second == 1);
        }
    }
#if TEST_STD_VER > 11
    { // N3644 testing
        typedef std::map<int, double> C;
        C::iterator ii1{}, ii2{};
        C::iterator ii4 = ii1;
        C::const_iterator cii{};
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
