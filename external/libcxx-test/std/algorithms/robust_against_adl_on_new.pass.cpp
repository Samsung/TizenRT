//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>
#include <algorithm>
#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {
struct A {
    int i = 0;
    bool operator<(const A& rhs) const { return i < rhs.i; }
    static bool isEven(const A& a) { return a.i % 2 == 0; }
};

} // namespace

// operator new overloads must be at global scope, not inside a namespace
void *operator new(std::size_t, A*) = delete;

int tc_libcxx_algorithms_robust_against_adl_on_new(void) {
    A a[4] = {};
    std::sort(a, a+4);
    std::sort(a, a+4, std::less<A>());
    std::partition(a, a+4, A::isEven);
    std::stable_sort(a, a+4);
    std::stable_sort(a, a+4, std::less<A>());
    std::stable_partition(a, a+4, A::isEven);
    std::inplace_merge(a, a+2, a+4);
    std::inplace_merge(a, a+2, a+4, std::less<A>());

    TC_SUCCESS_RESULT();


    return 0;
}
