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

// const_pointer data() const;

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "asan_testing.h"

struct Nasty {
    Nasty() : i_(0) {}
    Nasty(int i) : i_(i) {}
    ~Nasty() {}

   // Nasty * operator&() const { TC_ASSERT_EXPR(false); return nullptr; }
    int i_;
    };

int tc_libcxx_containers_vector_data_data_const(void)
{
    {
        const std::vector<int> v;
        TC_ASSERT_EXPR(v.data() == 0);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        const std::vector<int> v(100);
        TC_ASSERT_EXPR(v.data() == std::addressof(v.front()));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<Nasty> v(100);
        TC_ASSERT_EXPR(v.data() == std::addressof(v.front()));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
