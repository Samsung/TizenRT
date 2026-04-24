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
// <memory>

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template <class T> constexpr T* to_address(T* p) noexcept;
// template <class Ptr> constexpr auto to_address(const Ptr& p) noexcept;

#include <memory>

#include <array>
#include <cassert>
#include <span>
#include <string>
#include <string_view>
#include <valarray>
#include <vector>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template<class C>
void test_container_iterators(C c)
{
    const C& cc = c;
    TC_ASSERT_EXPR(std::to_address(c.begin()) == c.data());
    TC_ASSERT_EXPR(std::to_address(c.end()) == c.data() + c.size());
    TC_ASSERT_EXPR(std::to_address(cc.begin()) == cc.data());
    TC_ASSERT_EXPR(std::to_address(cc.end()) == cc.data() + cc.size());
}

void test_valarray_iterators()
{
    std::valarray<int> v(100);
    int *p = std::to_address(std::begin(v));
    int *q = std::to_address(std::end(v));
    TC_ASSERT_EXPR(q - p == 100);
}

int tc_utilities_memory_pointer_conversion_to_address_std_iterators(void) {
    test_container_iterators(std::array<int, 3>());
    test_container_iterators(std::vector<int>(3));
    test_container_iterators(std::string("abc"));
    test_container_iterators(std::string_view("abc"));
    test_container_iterators(std::span<const char>("abc"));
    test_valarray_iterators();

    return 0;
}
