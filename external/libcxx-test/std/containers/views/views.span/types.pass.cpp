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

// template<class ElementType, size_t Extent = dynamic_extent>
// class span {
// public:
//  // constants and types
//  using element_type           = ElementType;
//  using value_type             = remove_cv_t<ElementType>;
//  using size_type              = size_t;
//  using difference_type        = ptrdiff_t;
//  using pointer                = element_type *;
//  using reference              = element_type &;
//  using const_pointe           = const element_type *;
//  using const_reference        = const element_type &;
//  using iterator               = implementation-defined;
//  using const_iterator         = implementation-defined;
//  using reverse_iterator       = std::reverse_iterator<iterator>;
//  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
//
//  static constexpr size_type extent = Extent;
//

#include <span>
#include <cassert>
#include <iterator>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename S, typename Iter>
void testIterator()
{
    typedef std::iterator_traits<Iter> ItT;

    ASSERT_SAME_TYPE(typename ItT::iterator_category, std::random_access_iterator_tag);
    ASSERT_SAME_TYPE(typename ItT::value_type,        typename S::value_type);
    ASSERT_SAME_TYPE(typename ItT::reference,         typename S::reference);
    ASSERT_SAME_TYPE(typename ItT::pointer,           typename S::pointer);
    ASSERT_SAME_TYPE(typename ItT::difference_type,   typename S::difference_type);
}

template <typename S, typename ElementType, std::size_t Size>
void testSpan()
{
    ASSERT_SAME_TYPE(typename S::element_type,    ElementType);
    ASSERT_SAME_TYPE(typename S::value_type,      std::remove_cv_t<ElementType>);
    ASSERT_SAME_TYPE(typename S::size_type,       std::size_t);
    ASSERT_SAME_TYPE(typename S::difference_type, std::ptrdiff_t);
    ASSERT_SAME_TYPE(typename S::pointer,         ElementType *);
    ASSERT_SAME_TYPE(typename S::const_pointer,   const ElementType *);
    ASSERT_SAME_TYPE(typename S::reference,       ElementType &);
    ASSERT_SAME_TYPE(typename S::const_reference, const ElementType &);

    static_assert(S::extent == Size); // check that it exists

    testIterator<S, typename S::iterator>();
    testIterator<S, typename S::reverse_iterator>();
}


template <typename T>
void test()
{
    testSpan<std::span<               T>,                T, std::dynamic_extent>();
    testSpan<std::span<const          T>, const          T, std::dynamic_extent>();
    testSpan<std::span<      volatile T>,       volatile T, std::dynamic_extent>();
    testSpan<std::span<const volatile T>, const volatile T, std::dynamic_extent>();

    testSpan<std::span<               T, 5>,                T, 5>();
    testSpan<std::span<const          T, 5>, const          T, 5>();
    testSpan<std::span<      volatile T, 5>,       volatile T, 5>();
    testSpan<std::span<const volatile T, 5>, const volatile T, 5>();
}

struct A{};

int tc_containers_views_views_span_types(void) {
    test<int>();
    test<long>();
    test<double>();
    test<std::string>();
    test<A>();

  return 0;
}
