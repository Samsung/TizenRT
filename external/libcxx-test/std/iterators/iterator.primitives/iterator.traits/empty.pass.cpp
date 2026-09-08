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

// template<class NotAnIterator>
// struct iterator_traits
// {
// };

#include <iterator>
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct not_an_iterator
{
};

template <class T>
struct has_value_type
{
private:
    struct two {char lx; char lxx;};
    template <class U> static two test(...);
    template <class U> static char test(typename U::value_type* = 0);
public:
    static const bool value = sizeof(test<T>(0)) == 1;
};

int tc_libcxx_iterators_iterator_traits_empty(void)
{
    typedef std::iterator_traits<not_an_iterator> It;
    static_assert(!(has_value_type<It>::value), "");
    TC_SUCCESS_RESULT();
    return 0;
}
