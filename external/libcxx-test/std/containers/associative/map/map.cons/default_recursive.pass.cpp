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

// <map>

// class map

// map();

#include <map>
#include "test_macros.h"
#include "libcxx_tc_common.h"
struct X
{
    std::map<int, X> m;
    std::map<int, X>::iterator i;
    std::map<int, X>::const_iterator ci;
    std::map<int, X>::reverse_iterator ri;
    std::map<int, X>::const_reverse_iterator cri;
};

int tc_libcxx_containers_map_cons_default_recursive(void)
{
    TC_SUCCESS_RESULT();
    return 0;
}
