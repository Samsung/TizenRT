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

//       mapped_type& at(const key_type& k);
// const mapped_type& at(const key_type& k) const;

#include <map>
#include <cassert>

#include "test_macros.h"

int main()
{
    {
        typedef std::pair<const int, double> V;
        V ar[] =
        {
            V(1, 1.5),
            V(2, 2.5),
            V(3, 3.5),
            V(4, 4.5),
            V(5, 5.5),
            V(7, 7.5),
            V(8, 8.5),
        };
        std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        assert(m.size() == 7);
        assert(m.at(1) == 1.5);
        m.at(1) = -1.5;
        assert(m.at(1) == -1.5);
        assert(m.at(2) == 2.5);
        assert(m.at(3) == 3.5);
        assert(m.at(4) == 4.5);
        assert(m.at(5) == 5.5);
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            TEST_IGNORE_NODISCARD m.at(6);
            assert(false);
        }
        catch (std::out_of_range&)
        {
        }
#endif
        assert(m.at(7) == 7.5);
        assert(m.at(8) == 8.5);
        assert(m.size() == 7);
    }
    {
        typedef std::pair<const int, double> V;
        V ar[] =
        {
            V(1, 1.5),
            V(2, 2.5),
            V(3, 3.5),
            V(4, 4.5),
            V(5, 5.5),
            V(7, 7.5),
            V(8, 8.5),
        };
        const std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        assert(m.size() == 7);
        assert(m.at(1) == 1.5);
        assert(m.at(2) == 2.5);
        assert(m.at(3) == 3.5);
        assert(m.at(4) == 4.5);
        assert(m.at(5) == 5.5);
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            TEST_IGNORE_NODISCARD m.at(6);
            assert(false);
        }
        catch (std::out_of_range&)
        {
        }
#endif
        assert(m.at(7) == 7.5);
        assert(m.at(8) == 8.5);
        assert(m.size() == 7);
    }
}
