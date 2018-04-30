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

// istreambuf_iterator
//
// istreambuf_iterator() throw();
//
// All specializations of istreambuf_iterator shall have a trivial copy constructor,
//    a constexpr default constructor and a trivial destructor.

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_iterators_istreambuf_iterator_cons_default(void)
{
    {
        typedef std::istreambuf_iterator<char> T;
        T it;
        TC_ASSERT_EXPR(it == T());
#if TEST_STD_VER >= 11
        constexpr T it2;
        (void)it2;
#endif
    }
    {
        typedef std::istreambuf_iterator<wchar_t> T;
        T it;
        TC_ASSERT_EXPR(it == T());
#if TEST_STD_VER >= 11
        constexpr T it2;
        (void)it2;
#endif
    }
    TC_SUCCESS_RESULT();
    return 0;
}
