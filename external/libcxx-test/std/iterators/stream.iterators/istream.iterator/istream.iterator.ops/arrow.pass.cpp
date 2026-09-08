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

// class istream_iterator

// const T* operator->() const;

#include <iterator>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    double d_;
    int i_;
};

void operator&(A const&) {}

std::istream& operator>>(std::istream& is, A& a)
{
    return is >> a.d_ >> a.i_;
}

int tc_libcxx_iterators_istream_iterator_ops_arrow(void)
{
    std::istringstream inf("1.5  23 ");
    std::istream_iterator<A> i(inf);
    TC_ASSERT_EXPR(i->d_ == 1.5);
    TC_ASSERT_EXPR(i->i_ == 23);
    TC_SUCCESS_RESULT();
    return 0;
}
