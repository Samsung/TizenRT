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

// <string>

// explicit basic_string(const Allocator& a = Allocator());

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
static int
test()
{
    {
#if TEST_STD_VER > 14
    static_assert((noexcept(S{})), "" );
#elif TEST_STD_VER >= 11
    static_assert((noexcept(S()) == noexcept(typename S::allocator_type())), "" );
#endif
    S s;
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s.data());
    TC_ASSERT_EXPR(s.size() == 0);
    TC_ASSERT_EXPR(s.capacity() >= s.size());
    TC_ASSERT_EXPR(s.get_allocator() == typename S::allocator_type());
    }
    {
#if TEST_STD_VER > 14
    static_assert((noexcept(S{typename S::allocator_type{}})), "" );
#elif TEST_STD_VER >= 11
    static_assert((noexcept(S(typename S::allocator_type())) == std::is_nothrow_copy_constructible<typename S::allocator_type>::value), "" );
#endif
    S s(typename S::allocator_type(5));
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s.data());
    TC_ASSERT_EXPR(s.size() == 0);
    TC_ASSERT_EXPR(s.capacity() >= s.size());
    TC_ASSERT_EXPR(s.get_allocator() == typename S::allocator_type(5));
    }
    return 0;
}

int tc_libcxx_strings_string_cons_alloc(void)
{
    TC_ASSERT_FUNC((test<std::basic_string<char, std::char_traits<char>, test_allocator<char> > >()));
    TC_SUCCESS_RESULT();
    return 0;
}
