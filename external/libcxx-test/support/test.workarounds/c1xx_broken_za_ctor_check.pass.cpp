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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// Verify TEST_WORKAROUND_C1XX_BROKEN_ZA_CTOR_CHECK.

#include <type_traits>

#include "test_workarounds.h"

struct X {
    X(int) {}

    X(X&&) = default;
    X& operator=(X&&) = default;

private:
    X(const X&) = default;
    X& operator=(const X&) = default;
};

void PushFront(X&&) {}

template<class T = int>
auto test(int) -> decltype(PushFront(std::declval<T>()), std::true_type{});
auto test(long) -> std::false_type;

int main() {
#if defined(TEST_WORKAROUND_C1XX_BROKEN_ZA_CTOR_CHECK)
    static_assert(!decltype(test(0))::value, "");
#else
    static_assert(decltype(test(0))::value, "");
#endif
}
