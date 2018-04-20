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

// <utility>

// template <class T1, class T2> struct pair

// tuple_size<pair<T1, T2> >::value

#include <utility>
#include "libcxx_tc_common.h"

int tc_libcxx_utilities_pair_astuple_tuple_size(void)
{
    {
        typedef std::pair<int, short> P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    {
        typedef std::pair<int, short> const P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    {
        typedef std::pair<int, short> volatile P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    {
        typedef std::pair<int, short> const volatile P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
