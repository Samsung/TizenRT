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

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// iterator       begin()        {return __table_.begin();}
// iterator       end()          {return __table_.end();}
// const_iterator begin()  const {return __table_.begin();}
// const_iterator end()    const {return __table_.end();}
// const_iterator cbegin() const {return __table_.begin();}
// const_iterator cend()   const {return __table_.end();}

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"

int tc_libcxx_containers_unord_multimap_iterators(void)
{
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        TC_ASSERT_EXPR(c.size() == 6);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        C::iterator i;
        i = c.begin();
        i->second = "ONE";
        TC_ASSERT_EXPR(i->second == "ONE");
    }
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        TC_ASSERT_EXPR(c.size() == 6);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        C::const_iterator i;
    }
#if TEST_STD_VER > 11
    { // N3644 testing
        typedef std::unordered_multimap<int,double> C;
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
