//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
