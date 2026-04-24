/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
// REQUIRES: c++11 || c++14 || c++17 || c++20

// This test makes sure that we don't apply P1951 before C++23, since that is
// a breaking change. The examples in this test are taken from Richard Smith's
// comments on https://llvm.org/D109066.

#include <cassert>
#include <utility>
#include <vector>
#include "libcxx_tc_common.h"

struct A {
    int *p_;
    A(int *p) : p_(p) { *p_ += 1; }
    A(const A& a) : p_(a.p_) { *p_ += 1; }
    ~A() { *p_ -= 1; }
};

int tc_utilities_utility_pairs_pairs_pair_ctor_brace_init_P1951(void) {
    // Example 1:
    // Without P1951, we call the `pair(int, const A&)` constructor (the converting constructor is not usable because
    // we can't deduce from an initializer list), which creates the A temporary as part of the call to f. With P1951,
    // we call the `pair(U&&, V&&)` constructor, which creates a A temporary inside the pair constructor, and that
    // temporary doesn't live long enough any more.
    {
        int i = 0;
        auto f = [&](std::pair<std::vector<int>, const A&>) { TC_ASSERT_EXPR(i >= 1); };
        f({{42, 43}, &i});
    }

    // Example 2:
    // Here, n doesn't need to be captured if we call the `pair(const int&, const long&)` constructor, because
    // the lvalue-to-rvalue conversion happens in the lambda. But if we call the `pair(U&&, V&&)` constructor
    // (deducing V = int), then n does need to be captured.
    {
        const int n = 5;
        (void) []{ std::pair<int, long>({1}, n); };
    }

    return 0;
}
