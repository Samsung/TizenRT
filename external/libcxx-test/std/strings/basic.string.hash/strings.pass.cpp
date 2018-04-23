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

// <functional>

// template <class T>
// struct hash
//     : public unary_function<T, size_t>
// {
//     size_t operator()(T val) const;
// };

// Not very portable

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <type_traits>

#include "test_macros.h"

template <class T>
static int
test()
{
    typedef std::hash<T> H;
    static_assert((std::is_same<typename H::argument_type, T>::value), "" );
    static_assert((std::is_same<typename H::result_type, std::size_t>::value), "" );
    ASSERT_NOEXCEPT(H()(T()));

    H h;
    std::string g1 = "1234567890";
    std::string g2 = "1234567891";
    T s1(g1.begin(), g1.end());
    T s2(g2.begin(), g2.end());
    TC_ASSERT_EXPR(h(s1) != h(s2));
    return 0;
}

int tc_libcxx_strings_basic_string_hash_strings(void)
{
    TC_ASSERT_FUNC((test<std::string>()));
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    TC_ASSERT_FUNC((test<std::u16string>()));
    TC_ASSERT_FUNC((test<std::u32string>()));
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    TC_ASSERT_FUNC((test<std::wstring>()));
    TC_SUCCESS_RESULT();
    return 0;
}
