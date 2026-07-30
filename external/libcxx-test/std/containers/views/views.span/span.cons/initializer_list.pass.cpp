//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <span>

#include <span>
#include <cassert>
#include <cstddef>
#include "libcxx_tc_common.h"

struct Sink {
    constexpr Sink() = default;
    constexpr Sink(Sink*) {}
};

constexpr std::size_t count(std::span<const Sink> sp) {
    return sp.size();
}

template<int N>
constexpr std::size_t countn(std::span<const Sink, N> sp) {
    return sp.size();
}

constexpr bool test() {
    Sink a[10];
    TC_ASSERT_EXPR(count({a}) == 10);
    TC_ASSERT_EXPR(count({a, a+10}) == 10);
    TC_ASSERT_EXPR(countn<10>({a}) == 10);
    return true;
}

int tc_containers_views_views_span_span_cons_initializer_list(void) {
    test();
    static_assert(test());

    return 0;
}
