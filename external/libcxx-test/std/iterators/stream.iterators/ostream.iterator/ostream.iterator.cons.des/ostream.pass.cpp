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

// class ostream_iterator

// ostream_iterator(ostream_type& s);

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

struct MyTraits : std::char_traits<char> {};

typedef std::basic_ostringstream<char, MyTraits> StringStream;
typedef std::basic_ostream<char, MyTraits> BasicStream;

void operator&(BasicStream const&) {}

int tc_libcxx_iterators_ostream_iterator_cons_des_ostream(void)
{
    {
        std::ostringstream outf;
        std::ostream_iterator<int> i(outf);
        TC_ASSERT_EXPR(outf.good());
    }
    {
        StringStream outf;
        std::ostream_iterator<int, char, MyTraits> i(outf);
        TC_ASSERT_EXPR(outf.good());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
