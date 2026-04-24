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
// <algorithm>

// template<InputIterator InIter, OutputIterator<auto, InIter::reference> OutIter>
//   constexpr OutIter   // constexpr after C++17
//   copy_n(InIter first, InIter::difference_type n, OutIter result);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "user_defined_integral.h"
#include "libcxx_tc_common.h"

typedef UserDefinedIntegral<unsigned> UDI;

class PaddedBase {
public:
  TEST_CONSTEXPR PaddedBase(std::int16_t a, std::int8_t b) : a_(a), b_(b) {}

  std::int16_t a_;
  std::int8_t b_;
};

class Derived : public PaddedBase {
public:
  TEST_CONSTEXPR Derived(std::int16_t a, std::int8_t b, std::int8_t c) : PaddedBase(a, b), c_(c) {}

  std::int8_t c_;
};

template <class InIter, class OutIter>
TEST_CONSTEXPR_CXX20 void
test_copy_n()
{
  {
    const unsigned N = 1000;
    int ia[N] = {};
    for (unsigned i = 0; i < N; ++i)
        ia[i] = i;
    int ib[N] = {0};

    OutIter r = std::copy_n(InIter(ia), UDI(N/2), OutIter(ib));
    TC_ASSERT_EXPR(base(r) == ib+N/2);
    for (unsigned i = 0; i < N/2; ++i)
        TC_ASSERT_EXPR(ia[i] == ib[i]);
  }

  { // Make sure that padding bits aren't copied
    Derived src(1, 2, 3);
    Derived dst(4, 5, 6);
    std::copy_n(static_cast<PaddedBase*>(&src), 1, static_cast<PaddedBase*>(&dst));
    TC_ASSERT_EXPR(dst.a_ == 1);
    TC_ASSERT_EXPR(dst.b_ == 2);
    TC_ASSERT_EXPR(dst.c_ == 6);
  }

  { // Make sure that overlapping ranges can be copied
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::copy_n(a + 3, 7, a);
    int expected[] = {4, 5, 6, 7, 8, 9, 10, 8, 9, 10};
    TC_ASSERT_EXPR(std::equal(a, a + 10, expected));
  }
}

TEST_CONSTEXPR_CXX20 bool
test()
{
    test_copy_n<cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_n<cpp17_input_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_n<cpp17_input_iterator<const int*>, forward_iterator<int*> >();
    test_copy_n<cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_n<cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_n<cpp17_input_iterator<const int*>, int*>();

    test_copy_n<forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_n<forward_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_n<forward_iterator<const int*>, forward_iterator<int*> >();
    test_copy_n<forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_n<forward_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_n<forward_iterator<const int*>, int*>();

    test_copy_n<bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_n<bidirectional_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_n<bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test_copy_n<bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_n<bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_n<bidirectional_iterator<const int*>, int*>();

    test_copy_n<random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test_copy_n<random_access_iterator<const int*>, cpp17_input_iterator<int*> >();
    test_copy_n<random_access_iterator<const int*>, forward_iterator<int*> >();
    test_copy_n<random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test_copy_n<random_access_iterator<const int*>, random_access_iterator<int*> >();
    test_copy_n<random_access_iterator<const int*>, int*>();

    test_copy_n<const int*, cpp17_output_iterator<int*> >();
    test_copy_n<const int*, cpp17_input_iterator<int*> >();
    test_copy_n<const int*, forward_iterator<int*> >();
    test_copy_n<const int*, bidirectional_iterator<int*> >();
    test_copy_n<const int*, random_access_iterator<int*> >();
    test_copy_n<const int*, int*>();

  return true;
}

int tc_algorithms_alg_modifying_operations_alg_copy_copy_n(void) {
    test();

#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;
}
