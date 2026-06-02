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
// template<InputIterator InIter1, InputIterator InIter2, typename OutIter,
//          CopyConstructible Compare>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && Predicate<Compare, InIter1::value_type, InIter2::value_type>
//         && Predicate<Compare, InIter2::value_type, InIter1::value_type>
//   constexpr OutIter       // constexpr after C++17
//   merge(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2,
//         OutIter result, Compare comp);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "../sortable_helpers.h"
#include "libcxx_tc_common.h"

namespace {
template<class T, class Iter1, class Iter2, class OutIter>
TEST_CONSTEXPR_CXX20 void test_merge_comp_single()
{
    const T a[] = {11, 33, 31, 41};
    const T b[] = {22, 32, 43, 42, 52};
    {
        T result[20] = {};
        T expected[] = {11, 22, 33, 31, 32, 41, 43, 42, 52};
        OutIter end = std::merge(Iter1(a), Iter1(a+4), Iter2(b), Iter2(b+5), OutIter(result), typename T::Comparator());
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+9, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
    {
        T result[20] = {};
        T expected[] = {11, 22, 32, 33, 31, 43, 42, 41, 52};
        OutIter end = std::merge(Iter1(b), Iter1(b+5), Iter2(a), Iter2(a+4), OutIter(result), typename T::Comparator());
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+9, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
}

// ============================================================================
// TrivialSortableWithComp tests - grouped by output iterator type
// ============================================================================

// cpp17_output_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_cpp17_input_cpp17_output() {
    test_merge_comp_single<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_forward_cpp17_output() {
    test_merge_comp_single<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_bidirectional_cpp17_output() {
    test_merge_comp_single<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_random_access_cpp17_output() {
    test_merge_comp_single<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_pointer_cpp17_output() {
    test_merge_comp_single<TrivialSortableWithComp, const TrivialSortableWithComp*, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*>>();
}

// forward_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_cpp17_input_forward_output() {
    test_merge_comp_single<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, forward_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_forward_forward_output() {
    test_merge_comp_single<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, forward_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_bidirectional_forward_output() {
    test_merge_comp_single<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, forward_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_random_access_forward_output() {
    test_merge_comp_single<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, forward_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_pointer_forward_output() {
    test_merge_comp_single<TrivialSortableWithComp, const TrivialSortableWithComp*, forward_iterator<const TrivialSortableWithComp*>, forward_iterator<TrivialSortableWithComp*>>();
}

// bidirectional_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_cpp17_input_bidirectional_output() {
    test_merge_comp_single<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_forward_bidirectional_output() {
    test_merge_comp_single<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_bidirectional_bidirectional_output() {
    test_merge_comp_single<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_random_access_bidirectional_output() {
    test_merge_comp_single<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_pointer_bidirectional_output() {
    test_merge_comp_single<TrivialSortableWithComp, const TrivialSortableWithComp*, cpp17_input_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<TrivialSortableWithComp*>>();
}

// random_access_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_cpp17_input_random_access_output() {
    test_merge_comp_single<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, random_access_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_forward_random_access_output() {
    test_merge_comp_single<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, random_access_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_bidirectional_random_access_output() {
    test_merge_comp_single<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, random_access_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_random_access_random_access_output() {
    test_merge_comp_single<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, random_access_iterator<TrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_pointer_random_access_output() {
    test_merge_comp_single<TrivialSortableWithComp, const TrivialSortableWithComp*, cpp17_input_iterator<const TrivialSortableWithComp*>, random_access_iterator<TrivialSortableWithComp*>>();
}

// pointer output
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_cpp17_input_pointer_output() {
    test_merge_comp_single<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, TrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_forward_pointer_output() {
    test_merge_comp_single<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, TrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_bidirectional_pointer_output() {
    test_merge_comp_single<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, TrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_random_access_pointer_output() {
    test_merge_comp_single<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, TrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_trivial_pointer_pointer_output() {
    test_merge_comp_single<TrivialSortableWithComp, const TrivialSortableWithComp*, cpp17_input_iterator<const TrivialSortableWithComp*>, TrivialSortableWithComp*>();
}

// ============================================================================
// NonTrivialSortableWithComp tests - grouped by output iterator type
// ============================================================================

// cpp17_output_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_cpp17_input_cpp17_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_forward_cpp17_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_bidirectional_cpp17_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_random_access_cpp17_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_pointer_cpp17_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*>>();
}

// forward_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_cpp17_input_forward_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, forward_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_forward_forward_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, forward_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_bidirectional_forward_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, forward_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_random_access_forward_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, forward_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_pointer_forward_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, forward_iterator<const NonTrivialSortableWithComp*>, forward_iterator<NonTrivialSortableWithComp*>>();
}

// bidirectional_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_cpp17_input_bidirectional_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_forward_bidirectional_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_bidirectional_bidirectional_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_random_access_bidirectional_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_pointer_bidirectional_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, cpp17_input_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<NonTrivialSortableWithComp*>>();
}

// random_access_iterator output
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_cpp17_input_random_access_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_forward_random_access_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_bidirectional_random_access_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_random_access_random_access_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<NonTrivialSortableWithComp*>>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_pointer_random_access_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, cpp17_input_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<NonTrivialSortableWithComp*>>();
}

// pointer output
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_cpp17_input_pointer_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, NonTrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_forward_pointer_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, NonTrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_bidirectional_pointer_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, NonTrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_random_access_pointer_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, NonTrivialSortableWithComp*>();
}
TEST_CONSTEXPR_CXX20 void test_merge_comp_nontrivial_pointer_pointer_output() {
    test_merge_comp_single<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, cpp17_input_iterator<const NonTrivialSortableWithComp*>, NonTrivialSortableWithComp*>();
}

// ============================================================================
// Group functions - call all tests for each output iterator type
// ============================================================================

TEST_CONSTEXPR_CXX20 void test_all_trivial_comp_with_cpp17_output() {
    test_merge_comp_trivial_cpp17_input_cpp17_output();
    test_merge_comp_trivial_forward_cpp17_output();
    test_merge_comp_trivial_bidirectional_cpp17_output();
    test_merge_comp_trivial_random_access_cpp17_output();
    test_merge_comp_trivial_pointer_cpp17_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_comp_with_forward_output() {
    test_merge_comp_trivial_cpp17_input_forward_output();
    test_merge_comp_trivial_forward_forward_output();
    test_merge_comp_trivial_bidirectional_forward_output();
    test_merge_comp_trivial_random_access_forward_output();
    test_merge_comp_trivial_pointer_forward_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_comp_with_bidirectional_output() {
    test_merge_comp_trivial_cpp17_input_bidirectional_output();
    test_merge_comp_trivial_forward_bidirectional_output();
    test_merge_comp_trivial_bidirectional_bidirectional_output();
    test_merge_comp_trivial_random_access_bidirectional_output();
    test_merge_comp_trivial_pointer_bidirectional_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_comp_with_random_access_output() {
    test_merge_comp_trivial_cpp17_input_random_access_output();
    test_merge_comp_trivial_forward_random_access_output();
    test_merge_comp_trivial_bidirectional_random_access_output();
    test_merge_comp_trivial_random_access_random_access_output();
    test_merge_comp_trivial_pointer_random_access_output();
}

TEST_CONSTEXPR_CXX20 void test_all_trivial_comp_with_pointer_output() {
    test_merge_comp_trivial_cpp17_input_pointer_output();
    test_merge_comp_trivial_forward_pointer_output();
    test_merge_comp_trivial_bidirectional_pointer_output();
    test_merge_comp_trivial_random_access_pointer_output();
    test_merge_comp_trivial_pointer_pointer_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_comp_with_cpp17_output() {
    test_merge_comp_nontrivial_cpp17_input_cpp17_output();
    test_merge_comp_nontrivial_forward_cpp17_output();
    test_merge_comp_nontrivial_bidirectional_cpp17_output();
    test_merge_comp_nontrivial_random_access_cpp17_output();
    test_merge_comp_nontrivial_pointer_cpp17_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_comp_with_forward_output() {
    test_merge_comp_nontrivial_cpp17_input_forward_output();
    test_merge_comp_nontrivial_forward_forward_output();
    test_merge_comp_nontrivial_bidirectional_forward_output();
    test_merge_comp_nontrivial_random_access_forward_output();
    test_merge_comp_nontrivial_pointer_forward_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_comp_with_bidirectional_output() {
    test_merge_comp_nontrivial_cpp17_input_bidirectional_output();
    test_merge_comp_nontrivial_forward_bidirectional_output();
    test_merge_comp_nontrivial_bidirectional_bidirectional_output();
    test_merge_comp_nontrivial_random_access_bidirectional_output();
    test_merge_comp_nontrivial_pointer_bidirectional_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_comp_with_random_access_output() {
    test_merge_comp_nontrivial_cpp17_input_random_access_output();
    test_merge_comp_nontrivial_forward_random_access_output();
    test_merge_comp_nontrivial_bidirectional_random_access_output();
    test_merge_comp_nontrivial_random_access_random_access_output();
    test_merge_comp_nontrivial_pointer_random_access_output();
}

TEST_CONSTEXPR_CXX20 void test_all_nontrivial_comp_with_pointer_output() {
    test_merge_comp_nontrivial_cpp17_input_pointer_output();
    test_merge_comp_nontrivial_forward_pointer_output();
    test_merge_comp_nontrivial_bidirectional_pointer_output();
    test_merge_comp_nontrivial_random_access_pointer_output();
    test_merge_comp_nontrivial_pointer_pointer_output();
}

// ============================================================================
// Main test function - calls all test groups
// ============================================================================

TEST_CONSTEXPR_CXX20 bool run_all_merge_comp_tests() {
    // TrivialSortableWithComp tests
    test_all_trivial_comp_with_cpp17_output();
    test_all_trivial_comp_with_forward_output();
    test_all_trivial_comp_with_bidirectional_output();
    test_all_trivial_comp_with_random_access_output();
    test_all_trivial_comp_with_pointer_output();
    
    // NonTrivialSortableWithComp tests
    test_all_nontrivial_comp_with_cpp17_output();
    test_all_nontrivial_comp_with_forward_output();
    test_all_nontrivial_comp_with_bidirectional_output();
    test_all_nontrivial_comp_with_random_access_output();
    test_all_nontrivial_comp_with_pointer_output();
    
    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_merge_merge_comp(void) {
    run_all_merge_comp_tests();

#if TEST_STD_VER > 17
    static_assert(run_all_merge_comp_tests());
#endif

    TC_SUCCESS_RESULT();

    return 0;
}
