//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <list>
// UNSUPPORTED: c++03, c++11, c++14

// template <class InputIterator, class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
//    list(InputIterator, InputIterator, Allocator = Allocator())
//    -> list<typename iterator_traits<InputIterator>::value_type, Allocator>;
//
// template<ranges::input_range R, class Allocator = allocator<ranges::range_value_t<R>>>
//   list(from_range_t, R&&, Allocator = Allocator())
//     -> list<ranges::range_value_t<R>, Allocator>; // C++23

#include <array>
#include <cassert>
#include <climits> // INT_MAX
#include <cstddef>
#include <iterator>
#include <list>

#include "deduction_guides_sfinae_checks.h"
#include "test_macros.h"
#include "test_iterators.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

struct A {};

int tc_containers_sequences_list_list_cons_deduct(void) {

//  Test the explicit deduction guides
    {
    const int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::list lst(std::begin(arr), std::end(arr));

    static_assert(std::is_same_v<decltype(lst), std::list<int>>, "");
    TC_ASSERT_EXPR(std::equal(lst.begin(), lst.end(), std::begin(arr), std::end(arr)));
    }

    {
    const long arr[] = {INT_MAX, 1L, 2L, 3L };
    std::list lst(std::begin(arr), std::end(arr), std::allocator<long>());
    static_assert(std::is_same_v<decltype(lst)::value_type, long>, "");
    TC_ASSERT_EXPR(lst.size() == 4);
    auto it = lst.begin();
    TC_ASSERT_EXPR(*it++ == INT_MAX);
    TC_ASSERT_EXPR(*it++ == 1L);
    TC_ASSERT_EXPR(*it++ == 2L);
    }

//  Test the implicit deduction guides

    {
//  We don't expect this one to work.
//  std::list lst(std::allocator<int>()); // list (allocator &)
    }

    {
    std::list lst(1, A{}); // list (size_type, T)
    static_assert(std::is_same_v<decltype(lst)::value_type, A>, "");
    static_assert(std::is_same_v<decltype(lst)::allocator_type, std::allocator<A>>, "");
    TC_ASSERT_EXPR(lst.size() == 1);
    }

    {
    std::list lst(1, A{}, test_allocator<A>()); // list (size_type, T, allocator)
    static_assert(std::is_same_v<decltype(lst)::value_type, A>, "");
    static_assert(std::is_same_v<decltype(lst)::allocator_type, test_allocator<A>>, "");
    TC_ASSERT_EXPR(lst.size() == 1);
    }

    {
    std::list lst{1U, 2U, 3U, 4U, 5U}; // list(initializer-list)
    static_assert(std::is_same_v<decltype(lst)::value_type, unsigned>, "");
    TC_ASSERT_EXPR(lst.size() == 5);
    auto it = lst.begin();
    std::advance(it, 2);
    TC_ASSERT_EXPR(*it == 3U);
    }

    {
    std::list lst({1.0, 2.0, 3.0, 4.0}, test_allocator<double>()); // list(initializer-list, allocator)
    static_assert(std::is_same_v<decltype(lst)::value_type, double>, "");
    static_assert(std::is_same_v<decltype(lst)::allocator_type, test_allocator<double>>, "");
    TC_ASSERT_EXPR(lst.size() == 4);
    auto it = lst.begin();
    std::advance(it, 3);
    TC_ASSERT_EXPR(*it == 4.0);
    }

    {
    std::list<long double> source;
    std::list lst(source); // list(list &)
    static_assert(std::is_same_v<decltype(lst)::value_type, long double>, "");
    static_assert(std::is_same_v<decltype(lst)::allocator_type, std::allocator<long double>>, "");
    TC_ASSERT_EXPR(lst.size() == 0);
    }

    {
        typedef test_allocator<short> Alloc;
        typedef test_allocator<int> ConvertibleToAlloc;

        {
        std::list<short, Alloc> source;
        std::list lst(source, Alloc(2));
        static_assert(std::is_same_v<decltype(lst), decltype(source)>);
        }

        {
        std::list<short, Alloc> source;
        std::list lst(source, ConvertibleToAlloc(2));
        static_assert(std::is_same_v<decltype(lst), decltype(source)>);
        }

        {
        std::list<short, Alloc> source;
        std::list lst(std::move(source), Alloc(2));
        static_assert(std::is_same_v<decltype(lst), decltype(source)>);
        }

        {
        std::list<short, Alloc> source;
        std::list lst(std::move(source), ConvertibleToAlloc(2));
        static_assert(std::is_same_v<decltype(lst), decltype(source)>);
        }
    }

#if TEST_STD_VER >= 23
    {
      {
        std::list c(std::from_range, std::array<int, 0>());
        static_assert(std::is_same_v<decltype(c), std::list<int>>);
      }

      {
        using Alloc = test_allocator<int>;
        std::list c(std::from_range, std::array<int, 0>(), Alloc());
        static_assert(std::is_same_v<decltype(c), std::list<int, Alloc>>);
      }
    }
#endif

    SequenceContainerDeductionGuidesSfinaeAway<std::list, std::list<int>>();

    return 0;
}
