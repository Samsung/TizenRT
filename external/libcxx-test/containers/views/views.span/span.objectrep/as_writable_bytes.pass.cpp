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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <span>

// template <class ElementType, size_t Extent>
//     span<byte,
//          Extent == dynamic_extent
//              ? dynamic_extent
//              : sizeof(ElementType) * Extent>
//     as_writable_bytes(span<ElementType, Extent> s) noexcept;


#include <span>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template<typename Span>
void testRuntimeSpan(Span sp)
{
    ASSERT_NOEXCEPT(std::as_writable_bytes(sp));

    auto spBytes = std::as_writable_bytes(sp);
    using SB = decltype(spBytes);
    ASSERT_SAME_TYPE(std::byte, typename SB::element_type);

    if constexpr (sp.extent == std::dynamic_extent)
        TC_ASSERT_EXPR(spBytes.extent == std::dynamic_extent);
    else
        TC_ASSERT_EXPR(spBytes.extent == sizeof(typename Span::element_type) * sp.extent);

    TC_ASSERT_EXPR(static_cast<void*>(spBytes.data()) == static_cast<void*>(sp.data()));
    TC_ASSERT_EXPR(spBytes.size() == sp.size_bytes());
}

struct A{};
int iArr2[] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9};

int tc_containers_views_views_span_span_objectrep_as_writable_bytes(void) {
    testRuntimeSpan(std::span<int>        ());
    testRuntimeSpan(std::span<long>       ());
    testRuntimeSpan(std::span<double>     ());
    testRuntimeSpan(std::span<A>          ());
    testRuntimeSpan(std::span<std::string>());

    testRuntimeSpan(std::span<int, 0>        ());
    testRuntimeSpan(std::span<long, 0>       ());
    testRuntimeSpan(std::span<double, 0>     ());
    testRuntimeSpan(std::span<A, 0>          ());
    testRuntimeSpan(std::span<std::string, 0>());

    testRuntimeSpan(std::span<int>(iArr2, 1));
    testRuntimeSpan(std::span<int>(iArr2, 2));
    testRuntimeSpan(std::span<int>(iArr2, 3));
    testRuntimeSpan(std::span<int>(iArr2, 4));
    testRuntimeSpan(std::span<int>(iArr2, 5));

    testRuntimeSpan(std::span<int, 1>(iArr2 + 5, 1));
    testRuntimeSpan(std::span<int, 2>(iArr2 + 4, 2));
    testRuntimeSpan(std::span<int, 3>(iArr2 + 3, 3));
    testRuntimeSpan(std::span<int, 4>(iArr2 + 2, 4));
    testRuntimeSpan(std::span<int, 5>(iArr2 + 1, 5));

    std::string s;
    testRuntimeSpan(std::span<std::string>(&s, (std::size_t) 0));
    testRuntimeSpan(std::span<std::string>(&s, 1));

  return 0;
}
