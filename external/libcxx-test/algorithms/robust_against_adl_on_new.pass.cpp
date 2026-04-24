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
// <algorithm>

#include <algorithm>
#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {
    int i = 0;
    bool operator<(const A& rhs) const { return i < rhs.i; }
    static bool isEven(const A& a) { return a.i % 2 == 0; }
};

void *operator new(std::size_t, A*) = delete;

int tc_algorithms_robust_against_adl_on_new(void) {
    A a[4] = {};
    std::sort(a, a+4);
    std::sort(a, a+4, std::less<A>());
    std::partition(a, a+4, A::isEven);
    std::stable_sort(a, a+4);
    std::stable_sort(a, a+4, std::less<A>());
    std::stable_partition(a, a+4, A::isEven);
    std::inplace_merge(a, a+2, a+4);
    std::inplace_merge(a, a+2, a+4, std::less<A>());

    return 0;
}
