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

// <vector>

// static int push_back(const value_type& x);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <cstddef>
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_modifiers_push_back(void)
{
    {
        std::vector<int> c;
        c.push_back(0);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(2);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(3);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(4);
        TC_ASSERT_EXPR(c.size() == 5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
    }
    {
        // libc++ needs 15 because it grows by 2x (1 + 2 + 4 + 8).
        // Use 17 for implementations that dynamically allocate a container proxy
        // and grow by 1.5x (1 for proxy + 1 + 2 + 3 + 4 + 6).
        std::vector<int, limited_allocator<int, 17> > c;
        c.push_back(0);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(2);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(3);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
        c.push_back(4);
        TC_ASSERT_EXPR(c.size() == 5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == j);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
