//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
#include "test_macros.h"
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
