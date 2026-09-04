//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template <ObjectType T> T* addressof(T& r);

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    void operator&() const {}
};

struct nothing {
    operator char&()
    {
        static char c;
        return c;
    }
};

int tc_utilities_memory_specialized_algorithms_specialized_addressof_addressof(void) {
    {
    int i;
    double d;
    TC_ASSERT_EXPR(std::addressof(i) == &i);
    TC_ASSERT_EXPR(std::addressof(d) == &d);
    A* tp = new A;
    const A* ctp = tp;
    TC_ASSERT_EXPR(std::addressof(*tp) == tp);
    TC_ASSERT_EXPR(std::addressof(*ctp) == tp);
    delete tp;
    }
    {
    union
    {
        nothing n;
        int i;
    };
    TC_ASSERT_EXPR(std::addressof(n) == (void*)std::addressof(i));
    }

  return 0;
}
