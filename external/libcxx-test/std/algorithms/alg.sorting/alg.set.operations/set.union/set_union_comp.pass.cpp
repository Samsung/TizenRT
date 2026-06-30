//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<InputIterator InIter1, InputIterator InIter2, typename OutIter,
//          CopyConstructible Compare>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && Predicate<Compare, InIter1::value_type, InIter2::value_type>
//         && Predicate<Compare, InIter2::value_type, InIter1::value_type>
//   constexpr OutIter       // constexpr after C++17
//   set_union(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2,
//             OutIter result, Compare comp);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "../../sortable_helpers.h"
#include "libcxx_tc_common.h"

namespace {

// Core test function - tests one specific combination of iterator types
template<class T, class Iter1, class Iter2, class OutIter>
TEST_CONSTEXPR_CXX20 void test4()
{
    const T a[] = {11, 33, 31, 41};
    const T b[] = {22, 32, 43, 42, 52};
    {
        T result[20] = {};
        T expected[] = {11, 22, 33, 31, 41, 42, 52};
        OutIter end = std::set_union(Iter1(a), Iter1(a+4), Iter2(b), Iter2(b+5), OutIter(result), typename T::Comparator());
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+7, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
    {
        T result[20] = {};
        T expected[] = {11, 22, 32, 31, 43, 42, 52};
        OutIter end = std::set_union(Iter1(b), Iter1(b+5), Iter2(a), Iter2(a+4), OutIter(result), typename T::Comparator());
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+7, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
}

// Individual test functions - each calls test4 with concrete types
// Stack is freed between calls, preventing stack overflow

// TrivialSortableWithComp tests
void test_trivial_cpp17input_cpp17output() {
    test4<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_cpp17input_forward() {
    test4<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_cpp17input_bidirectional() {
    test4<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_cpp17input_random() {
    test4<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, random_access_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_cpp17input_pointer() {
    test4<TrivialSortableWithComp, cpp17_input_iterator<const TrivialSortableWithComp*>, const TrivialSortableWithComp*, cpp17_output_iterator<TrivialSortableWithComp*> >();
}

void test_trivial_forward_cpp17output() {
    test4<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_forward_forward() {
    test4<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_forward_bidirectional() {
    test4<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_forward_random() {
    test4<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, random_access_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_forward_pointer() {
    test4<TrivialSortableWithComp, forward_iterator<const TrivialSortableWithComp*>, const TrivialSortableWithComp*, cpp17_output_iterator<TrivialSortableWithComp*> >();
}

void test_trivial_bidirectional_cpp17output() {
    test4<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_bidirectional_forward() {
    test4<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_bidirectional_bidirectional() {
    test4<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_bidirectional_random() {
    test4<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, random_access_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_bidirectional_pointer() {
    test4<TrivialSortableWithComp, bidirectional_iterator<const TrivialSortableWithComp*>, const TrivialSortableWithComp*, cpp17_output_iterator<TrivialSortableWithComp*> >();
}

void test_trivial_random_cpp17output() {
    test4<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_random_forward() {
    test4<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, forward_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_random_bidirectional() {
    test4<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_random_random() {
    test4<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, random_access_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_random_pointer() {
    test4<TrivialSortableWithComp, random_access_iterator<const TrivialSortableWithComp*>, const TrivialSortableWithComp*, cpp17_output_iterator<TrivialSortableWithComp*> >();
}

void test_trivial_pointer_cpp17output() {
    test4<TrivialSortableWithComp, const TrivialSortableWithComp*, cpp17_input_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_pointer_forward() {
    test4<TrivialSortableWithComp, const TrivialSortableWithComp*, forward_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_pointer_bidirectional() {
    test4<TrivialSortableWithComp, const TrivialSortableWithComp*, bidirectional_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_pointer_random() {
    test4<TrivialSortableWithComp, const TrivialSortableWithComp*, random_access_iterator<const TrivialSortableWithComp*>, cpp17_output_iterator<TrivialSortableWithComp*> >();
}
void test_trivial_pointer_pointer() {
    test4<TrivialSortableWithComp, const TrivialSortableWithComp*, const TrivialSortableWithComp*, cpp17_output_iterator<TrivialSortableWithComp*> >();
}

// NonTrivialSortableWithComp tests
void test_nontrivial_cpp17input_cpp17output() {
    test4<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_cpp17input_forward() {
    test4<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_cpp17input_bidirectional() {
    test4<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_cpp17input_random() {
    test4<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_cpp17input_pointer() {
    test4<NonTrivialSortableWithComp, cpp17_input_iterator<const NonTrivialSortableWithComp*>, const NonTrivialSortableWithComp*, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}

void test_nontrivial_forward_cpp17output() {
    test4<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_forward_forward() {
    test4<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_forward_bidirectional() {
    test4<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_forward_random() {
    test4<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_forward_pointer() {
    test4<NonTrivialSortableWithComp, forward_iterator<const NonTrivialSortableWithComp*>, const NonTrivialSortableWithComp*, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}

void test_nontrivial_bidirectional_cpp17output() {
    test4<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_bidirectional_forward() {
    test4<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_bidirectional_bidirectional() {
    test4<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_bidirectional_random() {
    test4<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_bidirectional_pointer() {
    test4<NonTrivialSortableWithComp, bidirectional_iterator<const NonTrivialSortableWithComp*>, const NonTrivialSortableWithComp*, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}

void test_nontrivial_random_cpp17output() {
    test4<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_random_forward() {
    test4<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_random_bidirectional() {
    test4<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_random_random() {
    test4<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_random_pointer() {
    test4<NonTrivialSortableWithComp, random_access_iterator<const NonTrivialSortableWithComp*>, const NonTrivialSortableWithComp*, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}

void test_nontrivial_pointer_cpp17output() {
    test4<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, cpp17_input_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_pointer_forward() {
    test4<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, forward_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_pointer_bidirectional() {
    test4<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, bidirectional_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_pointer_random() {
    test4<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, random_access_iterator<const NonTrivialSortableWithComp*>, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}
void test_nontrivial_pointer_pointer() {
    test4<NonTrivialSortableWithComp, const NonTrivialSortableWithComp*, const NonTrivialSortableWithComp*, cpp17_output_iterator<NonTrivialSortableWithComp*> >();
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_union_set_union_comp(void) {
    // TrivialSortableWithComp tests
    test_trivial_cpp17input_cpp17output();
    test_trivial_cpp17input_forward();
    test_trivial_cpp17input_bidirectional();
    test_trivial_cpp17input_random();
    test_trivial_cpp17input_pointer();
    
    test_trivial_forward_cpp17output();
    test_trivial_forward_forward();
    test_trivial_forward_bidirectional();
    test_trivial_forward_random();
    test_trivial_forward_pointer();
    
    test_trivial_bidirectional_cpp17output();
    test_trivial_bidirectional_forward();
    test_trivial_bidirectional_bidirectional();
    test_trivial_bidirectional_random();
    test_trivial_bidirectional_pointer();
    
    test_trivial_random_cpp17output();
    test_trivial_random_forward();
    test_trivial_random_bidirectional();
    test_trivial_random_random();
    test_trivial_random_pointer();
    
    test_trivial_pointer_cpp17output();
    test_trivial_pointer_forward();
    test_trivial_pointer_bidirectional();
    test_trivial_pointer_random();
    test_trivial_pointer_pointer();
    
    // NonTrivialSortableWithComp tests
    test_nontrivial_cpp17input_cpp17output();
    test_nontrivial_cpp17input_forward();
    test_nontrivial_cpp17input_bidirectional();
    test_nontrivial_cpp17input_random();
    test_nontrivial_cpp17input_pointer();
    
    test_nontrivial_forward_cpp17output();
    test_nontrivial_forward_forward();
    test_nontrivial_forward_bidirectional();
    test_nontrivial_forward_random();
    test_nontrivial_forward_pointer();
    
    test_nontrivial_bidirectional_cpp17output();
    test_nontrivial_bidirectional_forward();
    test_nontrivial_bidirectional_bidirectional();
    test_nontrivial_bidirectional_random();
    test_nontrivial_bidirectional_pointer();
    
    test_nontrivial_random_cpp17output();
    test_nontrivial_random_forward();
    test_nontrivial_random_bidirectional();
    test_nontrivial_random_random();
    test_nontrivial_random_pointer();
    
    test_nontrivial_pointer_cpp17output();
    test_nontrivial_pointer_forward();
    test_nontrivial_pointer_bidirectional();
    test_nontrivial_pointer_random();
    test_nontrivial_pointer_pointer();

    TC_SUCCESS_RESULT();

    return 0;
}
