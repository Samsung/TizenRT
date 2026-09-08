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

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// float max_load_factor() const;
// static int max_load_factor(float mlf);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_containers_unord_map_max_load_factor(void)
{
    {
        typedef std::unordered_map<int, std::string> C;
        const C c;
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef std::unordered_map<int, std::string> C;
        C c;
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
        c.max_load_factor(2.5);
        TC_ASSERT_EXPR(c.max_load_factor() == 2.5);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
