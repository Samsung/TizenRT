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

// <forward_list>

// explicit forward_list(const allocator_type& a);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "test_allocator.h"
#include "NotConstructible.h"

int tc_libcxx_containers_forwardlist_cons_alloc_pass(void)
{
    {
        typedef test_allocator<NotConstructible> A;
        typedef A::value_type T;
        typedef std::forward_list<T, A> C;
        C c(A(12));
        TC_ASSERT_EXPR(c.get_allocator() == A(12));
        TC_ASSERT_EXPR(c.empty());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
