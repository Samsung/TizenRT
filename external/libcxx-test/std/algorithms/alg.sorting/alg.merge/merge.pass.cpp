//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>
//
// template<InputIterator InIter1, InputIterator InIter2, typename OutIter>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && HasLess<InIter2::value_type, InIter1::value_type>
//         && HasLess<InIter1::value_type, InIter2::value_type>
//   constexpr OutIter       // constexpr after C++17
//   merge(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2,
//         OutIter result);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "../sortable_helpers.h"
#include "libcxx_tc_common.h"

namespace {
template<class T, class Iter1, class Iter2, class OutIter>
TEST_CONSTEXPR_CXX20 void test_merge_single()
{
    const T a[] = {11, 33, 31, 41};
    const T b[] = {22, 32, 43, 42, 52};
    {
        T result[20] = {};
        T expected[] = {11, 22, 33, 31, 32, 41, 43, 42, 52};
        OutIter end = std::merge(Iter1(a), Iter1(a+4), Iter2(b), Iter2(b+5), OutIter(result));
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+9, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
    {
        T result[20] = {};
        T expected[] = {11, 22, 32, 33, 31, 43, 42, 41, 52};
        OutIter end = std::merge(Iter1(b), Iter1(b+5), Iter2(a), Iter2(a+4), OutIter(result));
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+9, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
}

// ============================================================================
// TrivialSortable tests - grouped by output iterator type
// ============================================================================

// cpp17_output_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_trivial_cpp17_input_cpp17_output() {
    test_merge_single<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_forward_cpp17_output() {
    test_merge_single<TrivialSortable, forward_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_bidirectional_cpp17_output() {
    test_merge_single<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_random_access_cpp17_output() {
    test_merge_single<TrivialSortable, random_access_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_pointer_cpp17_output() {
    test_merge_single<TrivialSortable, const TrivialSortable*, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*>>();
}

// forward_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_trivial_cpp17_input_forward_output() {
    test_merge_single<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, forward_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_forward_forward_output() {
    test_merge_single<TrivialSortable, forward_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, forward_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_bidirectional_forward_output() {
    test_merge_single<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, forward_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_random_access_forward_output() {
    test_merge_single<TrivialSortable, random_access_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, forward_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_pointer_forward_output() {
    test_merge_single<TrivialSortable, const TrivialSortable*, forward_iterator<const TrivialSortable*>, forward_iterator<TrivialSortable*>>();
}

// bidirectional_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_trivial_cpp17_input_bidirectional_output() {
    test_merge_single<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, bidirectional_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_forward_bidirectional_output() {
    test_merge_single<TrivialSortable, forward_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, bidirectional_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_bidirectional_bidirectional_output() {
    test_merge_single<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, bidirectional_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_random_access_bidirectional_output() {
    test_merge_single<TrivialSortable, random_access_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, bidirectional_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_pointer_bidirectional_output() {
    test_merge_single<TrivialSortable, const TrivialSortable*, cpp17_input_iterator<const TrivialSortable*>, bidirectional_iterator<TrivialSortable*>>();
}

// random_access_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_trivial_cpp17_input_random_access_output() {
    test_merge_single<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, random_access_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_forward_random_access_output() {
    test_merge_single<TrivialSortable, forward_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, random_access_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_bidirectional_random_access_output() {
    test_merge_single<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, random_access_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_random_access_random_access_output() {
    test_merge_single<TrivialSortable, random_access_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, random_access_iterator<TrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_pointer_random_access_output() {
    test_merge_single<TrivialSortable, const TrivialSortable*, cpp17_input_iterator<const TrivialSortable*>, random_access_iterator<TrivialSortable*>>();
}

// pointer output
TEST_CONSTEXPR_CXX20 void test_merge_trivial_cpp17_input_pointer_output() {
    test_merge_single<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, TrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_forward_pointer_output() {
    test_merge_single<TrivialSortable, forward_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, TrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_bidirectional_pointer_output() {
    test_merge_single<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, TrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_random_access_pointer_output() {
    test_merge_single<TrivialSortable, random_access_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, TrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_trivial_pointer_pointer_output() {
    test_merge_single<TrivialSortable, const TrivialSortable*, cpp17_input_iterator<const TrivialSortable*>, TrivialSortable*>();
}

// ============================================================================
// NonTrivialSortable tests - grouped by output iterator type
// ============================================================================

// cpp17_output_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_cpp17_input_cpp17_output() {
    test_merge_single<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_forward_cpp17_output() {
    test_merge_single<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_bidirectional_cpp17_output() {
    test_merge_single<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_random_access_cpp17_output() {
    test_merge_single<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_pointer_cpp17_output() {
    test_merge_single<NonTrivialSortable, const NonTrivialSortable*, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*>>();
}

// forward_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_cpp17_input_forward_output() {
    test_merge_single<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, forward_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_forward_forward_output() {
    test_merge_single<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, forward_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_bidirectional_forward_output() {
    test_merge_single<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, forward_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_random_access_forward_output() {
    test_merge_single<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, forward_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_pointer_forward_output() {
    test_merge_single<NonTrivialSortable, const NonTrivialSortable*, forward_iterator<const NonTrivialSortable*>, forward_iterator<NonTrivialSortable*>>();
}

// bidirectional_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_cpp17_input_bidirectional_output() {
    test_merge_single<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, bidirectional_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_forward_bidirectional_output() {
    test_merge_single<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, bidirectional_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_bidirectional_bidirectional_output() {
    test_merge_single<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, bidirectional_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_random_access_bidirectional_output() {
    test_merge_single<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, bidirectional_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_pointer_bidirectional_output() {
    test_merge_single<NonTrivialSortable, const NonTrivialSortable*, cpp17_input_iterator<const NonTrivialSortable*>, bidirectional_iterator<NonTrivialSortable*>>();
}

// random_access_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_cpp17_input_random_access_output() {
    test_merge_single<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, random_access_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_forward_random_access_output() {
    test_merge_single<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, random_access_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_bidirectional_random_access_output() {
    test_merge_single<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, random_access_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_random_access_random_access_output() {
    test_merge_single<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, random_access_iterator<NonTrivialSortable*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_pointer_random_access_output() {
    test_merge_single<NonTrivialSortable, const NonTrivialSortable*, cpp17_input_iterator<const NonTrivialSortable*>, random_access_iterator<NonTrivialSortable*>>();
}

// pointer output
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_cpp17_input_pointer_output() {
    test_merge_single<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, NonTrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_forward_pointer_output() {
    test_merge_single<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, NonTrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_bidirectional_pointer_output() {
    test_merge_single<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, NonTrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_random_access_pointer_output() {
    test_merge_single<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, NonTrivialSortable*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_nontrivial_pointer_pointer_output() {
    test_merge_single<NonTrivialSortable, const NonTrivialSortable*, cpp17_input_iterator<const NonTrivialSortable*>, NonTrivialSortable*>();
}

// ============================================================================
// Group functions - call all tests for each output iterator type
// ============================================================================

TEST_CONSTEXPR_CXX20 void test_all_trivial_with_cpp17_output() {
    test_merge_trivial_cpp17_input_cpp17_output();
    test_merge_trivial_forward_cpp17_output();
    test_merge_trivial_bidirectional_cpp17_output();
    test_merge_trivial_random_access_cpp17_output();
    test_merge_trivial_pointer_cpp17_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_with_forward_output() {
    test_merge_trivial_cpp17_input_forward_output();
    test_merge_trivial_forward_forward_output();
    test_merge_trivial_bidirectional_forward_output();
    test_merge_trivial_random_access_forward_output();
    test_merge_trivial_pointer_forward_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_with_bidirectional_output() {
    test_merge_trivial_cpp17_input_bidirectional_output();
    test_merge_trivial_forward_bidirectional_output();
    test_merge_trivial_bidirectional_bidirectional_output();
    test_merge_trivial_random_access_bidirectional_output();
    test_merge_trivial_pointer_bidirectional_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_with_random_access_output() {
    test_merge_trivial_cpp17_input_random_access_output();
    test_merge_trivial_forward_random_access_output();
    test_merge_trivial_bidirectional_random_access_output();
    test_merge_trivial_random_access_random_access_output();
    test_merge_trivial_pointer_random_access_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_with_pointer_output() {
    test_merge_trivial_cpp17_input_pointer_output();
    test_merge_trivial_forward_pointer_output();
    test_merge_trivial_bidirectional_pointer_output();
    test_merge_trivial_random_access_pointer_output();
    test_merge_trivial_pointer_pointer_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_with_cpp17_output() {
    test_merge_nontrivial_cpp17_input_cpp17_output();
    test_merge_nontrivial_forward_cpp17_output();
    test_merge_nontrivial_bidirectional_cpp17_output();
    test_merge_nontrivial_random_access_cpp17_output();
    test_merge_nontrivial_pointer_cpp17_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_with_forward_output() {
    test_merge_nontrivial_cpp17_input_forward_output();
    test_merge_nontrivial_forward_forward_output();
    test_merge_nontrivial_bidirectional_forward_output();
    test_merge_nontrivial_random_access_forward_output();
    test_merge_nontrivial_pointer_forward_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_with_bidirectional_output() {
    test_merge_nontrivial_cpp17_input_bidirectional_output();
    test_merge_nontrivial_forward_bidirectional_output();
    test_merge_nontrivial_bidirectional_bidirectional_output();
    test_merge_nontrivial_random_access_bidirectional_output();
    test_merge_nontrivial_pointer_bidirectional_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_with_random_access_output() {
    test_merge_nontrivial_cpp17_input_random_access_output();
    test_merge_nontrivial_forward_random_access_output();
    test_merge_nontrivial_bidirectional_random_access_output();
    test_merge_nontrivial_random_access_random_access_output();
    test_merge_nontrivial_pointer_random_access_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_with_pointer_output() {
    test_merge_nontrivial_cpp17_input_pointer_output();
    test_merge_nontrivial_forward_pointer_output();
    test_merge_nontrivial_bidirectional_pointer_output();
    test_merge_nontrivial_random_access_pointer_output();
    test_merge_nontrivial_pointer_pointer_output();
}

// ============================================================================
// Main test function - calls all test groups
// ============================================================================

TEST_CONSTEXPR_CXX20 bool run_all_merge_tests() {
    // TrivialSortable tests
    test_all_trivial_with_cpp17_output();
    test_all_trivial_with_forward_output();
    test_all_trivial_with_bidirectional_output();
    test_all_trivial_with_random_access_output();
    test_all_trivial_with_pointer_output();
    
    // NonTrivialSortable tests
    test_all_nontrivial_with_cpp17_output();
    test_all_nontrivial_with_forward_output();
    test_all_nontrivial_with_bidirectional_output();
    test_all_nontrivial_with_random_access_output();
    test_all_nontrivial_with_pointer_output();
    
    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_merge_merge(void) {
    run_all_merge_tests();

#if TEST_STD_VER > 17
    static_assert(run_all_merge_tests());
#endif

    TC_SUCCESS_RESULT();

    return 0;
}
