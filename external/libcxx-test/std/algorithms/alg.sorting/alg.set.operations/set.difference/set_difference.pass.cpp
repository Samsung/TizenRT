//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<InputIterator InIter1, InputIterator InIter2, typename OutIter>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && HasLess<InIter2::value_type, InIter1::value_type>
//         && HasLess<InIter1::value_type, InIter2::value_type>
//   constexpr OutIter       // constexpr after C++17
//   set_difference(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2,
//                  OutIter result);

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
        T expected[] = {11, 31};
        OutIter end = std::set_difference(Iter1(a), Iter1(a+4), Iter2(b), Iter2(b+5), OutIter(result));
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+2, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
    {
        T result[20] = {};
        T expected[] = {22, 42, 52};
        OutIter end = std::set_difference(Iter1(b), Iter1(b+5), Iter2(a), Iter2(a+4), OutIter(result));
        TC_ASSERT_EXPR(std::lexicographical_compare(result, base(end), expected, expected+3, T::less) == 0);
        for (const T *it = base(end); it != result+20; ++it) {
            TC_ASSERT_EXPR(it->value == 0);
        }
    }
}

// Individual test functions - each calls test4 with concrete types
// Stack is freed between calls, preventing stack overflow

// TrivialSortable tests
void test_trivial_cpp17input_cpp17output() {
    test4<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_cpp17input_forward() {
    test4<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_cpp17input_bidirectional() {
    test4<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, bidirectional_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_cpp17input_random() {
    test4<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, random_access_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_cpp17input_pointer() {
    test4<TrivialSortable, cpp17_input_iterator<const TrivialSortable*>, const TrivialSortable*, cpp17_output_iterator<TrivialSortable*> >();
}

void test_trivial_forward_cpp17output() {
    test4<TrivialSortable, forward_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_forward_forward() {
    test4<TrivialSortable, forward_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_forward_bidirectional() {
    test4<TrivialSortable, forward_iterator<const TrivialSortable*>, bidirectional_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_forward_random() {
    test4<TrivialSortable, forward_iterator<const TrivialSortable*>, random_access_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_forward_pointer() {
    test4<TrivialSortable, forward_iterator<const TrivialSortable*>, const TrivialSortable*, cpp17_output_iterator<TrivialSortable*> >();
}

void test_trivial_bidirectional_cpp17output() {
    test4<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_bidirectional_forward() {
    test4<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_bidirectional_bidirectional() {
    test4<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, bidirectional_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_bidirectional_random() {
    test4<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, random_access_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_bidirectional_pointer() {
    test4<TrivialSortable, bidirectional_iterator<const TrivialSortable*>, const TrivialSortable*, cpp17_output_iterator<TrivialSortable*> >();
}

void test_trivial_random_cpp17output() {
    test4<TrivialSortable, random_access_iterator<const TrivialSortable*>, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_random_forward() {
    test4<TrivialSortable, random_access_iterator<const TrivialSortable*>, forward_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_random_bidirectional() {
    test4<TrivialSortable, random_access_iterator<const TrivialSortable*>, bidirectional_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_random_random() {
    test4<TrivialSortable, random_access_iterator<const TrivialSortable*>, random_access_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_random_pointer() {
    test4<TrivialSortable, random_access_iterator<const TrivialSortable*>, const TrivialSortable*, cpp17_output_iterator<TrivialSortable*> >();
}

void test_trivial_pointer_cpp17output() {
    test4<TrivialSortable, const TrivialSortable*, cpp17_input_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_pointer_forward() {
    test4<TrivialSortable, const TrivialSortable*, forward_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_pointer_bidirectional() {
    test4<TrivialSortable, const TrivialSortable*, bidirectional_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_pointer_random() {
    test4<TrivialSortable, const TrivialSortable*, random_access_iterator<const TrivialSortable*>, cpp17_output_iterator<TrivialSortable*> >();
}
void test_trivial_pointer_pointer() {
    test4<TrivialSortable, const TrivialSortable*, const TrivialSortable*, cpp17_output_iterator<TrivialSortable*> >();
}

// NonTrivialSortable tests
void test_nontrivial_cpp17input_cpp17output() {
    test4<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_cpp17input_forward() {
    test4<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_cpp17input_bidirectional() {
    test4<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, bidirectional_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_cpp17input_random() {
    test4<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, random_access_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_cpp17input_pointer() {
    test4<NonTrivialSortable, cpp17_input_iterator<const NonTrivialSortable*>, const NonTrivialSortable*, cpp17_output_iterator<NonTrivialSortable*> >();
}

void test_nontrivial_forward_cpp17output() {
    test4<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_forward_forward() {
    test4<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_forward_bidirectional() {
    test4<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, bidirectional_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_forward_random() {
    test4<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, random_access_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_forward_pointer() {
    test4<NonTrivialSortable, forward_iterator<const NonTrivialSortable*>, const NonTrivialSortable*, cpp17_output_iterator<NonTrivialSortable*> >();
}

void test_nontrivial_bidirectional_cpp17output() {
    test4<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_bidirectional_forward() {
    test4<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_bidirectional_bidirectional() {
    test4<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, bidirectional_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_bidirectional_random() {
    test4<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, random_access_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_bidirectional_pointer() {
    test4<NonTrivialSortable, bidirectional_iterator<const NonTrivialSortable*>, const NonTrivialSortable*, cpp17_output_iterator<NonTrivialSortable*> >();
}

void test_nontrivial_random_cpp17output() {
    test4<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_random_forward() {
    test4<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, forward_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_random_bidirectional() {
    test4<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, bidirectional_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_random_random() {
    test4<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, random_access_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_random_pointer() {
    test4<NonTrivialSortable, random_access_iterator<const NonTrivialSortable*>, const NonTrivialSortable*, cpp17_output_iterator<NonTrivialSortable*> >();
}

void test_nontrivial_pointer_cpp17output() {
    test4<NonTrivialSortable, const NonTrivialSortable*, cpp17_input_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_pointer_forward() {
    test4<NonTrivialSortable, const NonTrivialSortable*, forward_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_pointer_bidirectional() {
    test4<NonTrivialSortable, const NonTrivialSortable*, bidirectional_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_pointer_random() {
    test4<NonTrivialSortable, const NonTrivialSortable*, random_access_iterator<const NonTrivialSortable*>, cpp17_output_iterator<NonTrivialSortable*> >();
}
void test_nontrivial_pointer_pointer() {
    test4<NonTrivialSortable, const NonTrivialSortable*, const NonTrivialSortable*, cpp17_output_iterator<NonTrivialSortable*> >();
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_difference_set_difference(void) {
    // TrivialSortable tests
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
    
    // NonTrivialSortable tests
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
